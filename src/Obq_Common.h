#ifndef OBQCOMMON_H
#define OBQCOMMON_H

#include "ai.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

static const int c_ObqHemisphereThetaRes = 32;
static const float c_ObqHemisphereThetaResM1f = c_ObqHemisphereThetaRes-1.0f;
static const float c_Pi = static_cast<float>(AI_PI);
static const float c_2Pi = static_cast<float>(AI_PITIMES2);
static const float c_4Pi = static_cast<float>(4.0*AI_PI);
static const float c_1OverPi = static_cast<float>(1.0/AI_PI);
static const float c_1Over2Pi = static_cast<float>(1.0/AI_PITIMES2);
static const float c_1Over4Pi = static_cast<float>(1.0/(2.0*AI_PITIMES2));
static const float c_PiOver2 = static_cast<float>(AI_PIOVER2);
static const float c_RADIANS = static_cast<float>(AI_DTOR);
static const float c_DEGREES = static_cast<float>(AI_RTOD);


/////////////////////////////////////////////////////////
// OBQ COMMON FUNCTIONS
/////////////////////////////////////////////////////////

// Not to be use for fast computation
template <class T>
inline std::string to_string(T t, std::ios_base & (*f)(std::ios_base&))
{
  std::ostringstream oss;
  oss << f << t;
  return oss.str();
}

// A safe way to return the angle between 2 vectors
//
inline float angleBetween(const AtVector& A,const AtVector& B)
{
    float dot = AiV3Dot(A,B);
    if(dot>1.0f)
        return 0.0f;
    else if(dot < -1.0f)
        return c_Pi;
    else
        return std::acos(dot);
}

inline float AiV3DotClamped(const AtVector& A,const AtVector& B)
{
	return CLAMP(AiV3Dot(A,B),-1.0f,1.0f);
}

//-----------------------------------------------------------
/*! Rotates a vector V around the X axis with angle theta. */
//-----------------------------------------------------------
inline AtVector rotateVectorAroundX(const AtVector& V,	/*!< [in] : Vector to rotate.*/
									float angle			/*!< [in] : The angle of rotation.*/)
{
    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);
	AtVector ret = {V.x,cosTheta*V.y-sinTheta*V.z,sinTheta*V.y+cosTheta*V.z};
	return ret;
}

//-----------------------------------------------------------
/*! Rotates a vector V around the Y axis with angle theta. */
//-----------------------------------------------------------
inline AtVector rotateVectorAroundY(const AtVector& V,	/*!< [in] : Vector to rotate.*/
									float angle			/*!< [in] : The angle of rotation.*/)
{
    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);
	AtVector ret = {V.x*cosTheta+V.z*sinTheta,V.y,V.z*cosTheta - V.x*sinTheta};
    
	return ret;
}

//-----------------------------------------------------------
/*! Rotates a vector V around the Z axis with angle theta. */
//-----------------------------------------------------------
inline AtVector rotateVectorAroundZ(const AtVector& V,	/*!< [in] : Vector to rotate.*/
									float angle			/*!< [in] : The angle of rotation.*/)
{
    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);
	AtVector ret = {cosTheta*V.x - sinTheta*V.y,sinTheta*V.x+cosTheta*V.y,V.z};
    
	return ret;
}


//----------------------------------------------------------------------
/*! Rotate a vector around an arbitray axis. */
//-----------------------------------------------------------------------
inline AtVector rodriguesRotation(AtVector v,  /*!< [in] : Vector to rotate.*/
                                    AtVector k,  /*!< [in] : Vector to rotate v around.*/
                                    float angle    /*!< [in] : Angle of rotation.*/){
    if(angle == 0.0)
        return v;

    float cosA = std::cos(angle);
    return v*cosA + AiV3Cross(k,v)*std::sin(angle) + k*AiV3Dot(k,v)*(1.0f-cosA);

}

//-------------------------------------------------------------------------------------
/*! Given 2 uniform samples, return sampling direction on sphere // Jaewon Jung */
//-------------------------------------------------------------------------------------
inline void uniformPointSphere(AtVector& V, double uv[])
{
	V.z = static_cast<float>(LERP(uv[0],-1.0,1.0));
	float t = static_cast<float>(LERP(uv[1],0.0,AI_PITIMES2));
	float r = std::sqrt(1.0f-std::pow(V.z,2));
	V.x = r*std::cos(t);
	V.y = r*std::sin(t);
}

//-----------------------------------------------------------
/*! Assures color has channels with minimum of value. */
//-----------------------------------------------------------
inline AtColor colorChannelMax(const AtColor& color,float value)
{
	AtColor ret = {std::max(color.r,value),std::max(color.g,value),std::max(color.b,value)};
	return ret;
}

//-----------------------------------------------------------
/*! Pow for all channels. */
//-----------------------------------------------------------
inline AtColor colorChannelPow(const AtColor& color,int value)
{
	AtColor ret = {std::pow(color.r,value),std::pow(color.g,value),std::pow(color.b,value)};
	return ret;
}
inline AtColor colorChannelPow(const AtColor& color,float value)
{
	AtColor ret = {std::pow(color.r,value),std::pow(color.g,value),std::pow(color.b,value)};
	return ret;
}

//-----------------------------------------------------------
/*! Square root for all channel. */
//-----------------------------------------------------------
inline AtColor colorChannelSqrt(const AtColor& color)
{
	AtColor ret = {std::sqrt(color.r),std::sqrt(color.g),std::sqrt(color.b)};
	return ret;
}


inline double myRandom()
{
	return static_cast<double>(std::rand())/static_cast<double>(RAND_MAX);
}
/////////////////////////////////////////////////////////
// OBQ CLASSES
/////////////////////////////////////////////////////////

class ObqHemisphere
{
public:

    //------------------------
    /*! \brief Constructor. */
    //------------------------
	ObqHemisphere(){

		// Initialize resolution
		int m_res = c_ObqHemisphereThetaRes;

		// common res
		float m_resM1d = static_cast<float>(m_res-1);
		int m_2xRes = 2*m_res-1;
		float m_2xResM1d = static_cast<float>(m_2xRes-1);

		////////////////
		// OMEGA and COS
		/////////////////

		int toRes = m_res, dpRes = m_2xRes;

		//initialize solid angle of bins
		//-- SOLID ANGLES
		float totalOmega = 0.0f;

		for(int j = 0; j < toRes; j++){
			float omega = 1.0f;


			//////////////////
			// CALCUL DU OMEGA

			// top
			if(j==0)
			{
				float t = c_PiOver2/static_cast<float>(toRes-1);
				float ph = c_Pi/static_cast<float>(dpRes-1);
				AtVector a = {0.0f,1.0f,0.0f};
				AtVector b = {std::sin(t/2.0f),std::cos(t/2.0f),0.0f};
				AtVector c = rotateVectorAroundY(b,ph/2.0f);
				omega = 2.0f*static_cast<float>(dpRes-1)*2.0f*getSolidAngleOfTriangle(a, b, c);

			}
			// bottom
			else if(j == toRes-1)
			{

				float t = c_PiOver2/static_cast<float>(toRes-1);
				float jt = static_cast<float>(j)*t;
				float tover2 = t/2.0f;
				float ph = c_Pi/static_cast<float>(dpRes-1);
				float phover2 = ph/2.0f;
				AtVector a = {std::sin(jt),std::cos(jt),0.0f};
				AtVector d = rotateVectorAroundY(a,phover2);
				AtVector b = {std::sin(jt-tover2),std::cos(jt-tover2),0.0f};
				AtVector c = rotateVectorAroundY(b,phover2);

				omega = 2.0f*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d));

			}
			// rest
			else
			{

				float t = c_PiOver2/static_cast<float>(toRes-1);
				float jt = static_cast<float>(j)*t;
				float tover2 = t/2.0f;
				float ph = c_Pi/static_cast<float>(dpRes-1);
				float phover2 = ph/2.0f;
				AtVector a = {std::sin(jt),std::cos(jt),0.0f};
				AtVector d = rotateVectorAroundY(a,phover2);
				AtVector b = {std::sin(jt-tover2),std::cos(jt-tover2),0.0f};
				AtVector c = rotateVectorAroundY(b,phover2);
				AtVector e = {std::sin(jt+tover2),std::cos(jt+tover2),0.0f};
				AtVector f = rotateVectorAroundY(e,phover2);

				omega = 2.0f*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d) + getSolidAngleOfTriangle(a, d, e) + getSolidAngleOfTriangle(d, f, e));

			}

			///////////////////////
			// ASSIGNATION DU OMEGA (reversed for theta-Phi hair )

			if(j==0) // calotte
				m_omega[(toRes-1)-j] = omega/2.0f;
			else if(j == toRes-1) // On doit doubler pour aller chercher l'autre hémisphère
				m_omega[(toRes-1)-j] = 2.0f*omega;
			else
				m_omega[(toRes-1)-j] = omega;
		}
	}

    //------------------------
    /*! \brief Destructor. */
    //------------------------
    ~ObqHemisphere(){;}

    //-----------------------------------------------------------------------------------------------
    /*! \brief Returns the index of a parameter based on the maximum value and the (resolution-1). */
    //-----------------------------------------------------------------------------------------------
    inline AtUInt32 getRoundIndex(	float angle ) const {

        return static_cast<AtUInt32>(angle/c_PiOver2*c_ObqHemisphereThetaResM1f+0.5f);

    }

    //--------------------------------------------------------------
    /*! \brief Gets the omega of a bin. */
    //--------------------------------------------------------------
    inline float getOmega(AtUInt32 index) const
    {
        return m_omega[index];
    }

    //----------------------------------------------------------------------
    /*! \brief Calculates solid angle of triangle abc (3d points) using the Girard formula. */
    //-----------------------------------------------------------------------
    inline float getSolidAngleOfTriangle(AtVector a, AtVector b, AtVector c)
    {
        // calculate angle A between projection of b and c on tangent plane to a
        AtVector n_a = -a;
        AtVector v_ab = AiV3Normalize(b-a);
        AtVector v_ac = AiV3Normalize(c-a);
        AtVector t_ab = AiV3Normalize(v_ab - AiV3DotClamped(n_a,v_ab) * (n_a));
        AtVector t_ac = AiV3Normalize(v_ac - AiV3DotClamped(n_a,v_ac) * (n_a));

        float A = angleBetween(t_ab,t_ac);

        AtVector n_b = -b;
        AtVector v_ba = AiV3Normalize(a-b);
        AtVector v_bc = AiV3Normalize(c-b);
        AtVector t_ba = AiV3Normalize(v_ba - AiV3DotClamped(n_b,v_ba) * (n_b));
        AtVector t_bc = AiV3Normalize(v_bc - AiV3DotClamped(n_b,v_bc) * (n_b));

        float B = angleBetween(t_ba,t_bc);

        AtVector n_c = -c;
        AtVector v_cb = AiV3Normalize(b-c);
        AtVector v_ca = AiV3Normalize(a-c);
        AtVector t_cb = AiV3Normalize(v_cb - AiV3DotClamped(n_c,v_cb) * (n_c));
        AtVector t_ca = AiV3Normalize(v_ca - AiV3DotClamped(n_c,v_ca) * (n_c));

        float C = angleBetween(t_cb,t_ca);

        return std::abs(A+B+C-c_Pi);
    }
	
    //-----------------------------------------------------------------------------------------------
    /*! \brief Returns the index of a parameter based on the maximum value and the (resolution-1). */
    //-----------------------------------------------------------------------------------------------
    inline float getAngle(	AtUInt32 index,          /*!< [in] : Angle index.*/
                            float resolutionM1,  /*!< [in] : Resolution - 1 of parameter.*/
                            float maxAngle        /*!< [in] : Maximum value for this parameter.*/
                          ){

        return static_cast<float>(index)*maxAngle/resolutionM1;

    }

    //----------------------------------------------------------------------------------------------
    /*! \brief Set the corresponding bin with this color (Only valid for SingleChannelBRDF). */
    //----------------------------------------------------------------------------------------------
    inline void setData(float theta,		 /*!< [in] : Angle of parameter.*/
                        float omega			 /*!< [in] : Solid angle associated with this theta.*/
                        )
    {

        AtUInt32 index = getRoundIndex(theta);

        m_omega[index]=omega;

    }
	
    //---------------------------------------------------
    /*! \brief get this color in the corresponding bin. */
    //---------------------------------------------------
    inline float getData(float theta)           /*!< [in] : Value of thetaIn in radians.*/
    {
        // calculate thetas
        AtUInt32 index = getRoundIndex(theta);

		// get the data
        return m_omega[index];
    }

	    //---------------------------------------------------
    /*! \brief get this color in the corresponding bin. */
    //---------------------------------------------------
    inline float getData(AtUInt32 index)           /*!< [in] : Value of thetaIn in radians.*/
    {
		// get the data
        return m_omega[index];
    }


	//---------------------------------------------------
    /*! \brief Return solid angle of hemisphere (normally 2Pi). */
    //---------------------------------------------------
	inline float getTotalOmega()
	{
		float omegaTotal = 0.0f;
		int res = c_ObqHemisphereThetaRes;
		for(int theta = -(res-1); theta < res; theta++)
		{
			if(std::abs(theta) == res-1) // calotte
			{
				omegaTotal += m_omega[std::abs(theta)];
			}
			else //pas calotte
			{
				for(int phi = 0; phi < 2*res-1;phi++)
				{
					if(phi==0 || phi == 2*(res-1)){
						omegaTotal += m_omega[std::abs(theta)]/2.0f;
					}
					else
					{
						omegaTotal += m_omega[std::abs(theta)];
					}
				}
			}
		}
		return omegaTotal;
	}


private:
    float m_omega[c_ObqHemisphereThetaRes];		/*!< The data. */
};

#endif // OBQCOMMON_H


