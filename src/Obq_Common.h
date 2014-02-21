/*
Obq_Common :

	Common header for the Obq_Shaders

*------------------------------------------------------------------------
Copyright (c) 2012-2014 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*------------------------------------------------------------------------
*/
#ifndef OBQCOMMON_H
#define OBQCOMMON_H

#include <ai.h>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>



#define OBQ_AI_VERSION (AI_VERSION_ARCH_NUM*10000+AI_VERSION_MAJOR_NUM*100+AI_VERSION_MINOR_NUM)

static const int c_ObqHemisphereThetaRes = 32;
static const float c_ObqHemisphereThetaResM1f = c_ObqHemisphereThetaRes-1.0f;

static const double c_Pi__d = 3.1415926535897931;
static const double c_2Pi__d = 6.2831853071795862;
static const double c_4Pi__d = c_Pi__d*4.0;
static const double c_1OverPi__d = 0.3183098861837906;
static const double c_1Over2Pi__d = 0.1591549430918953;
static const double c_1Over4Pi__d = 1.0/c_4Pi__d;
static const double c_PiOver2__d = 1.5707963267948966;
static const double c_Radians__d = 0.0174532925199433;
static const double c_Degrees__d = 57.295779513082323;
static const double c_E__d = 2.7182818284590452;
static const double c_Log_2_E__d = 1.4426950408889634; 
static const double c_Sqrt2__d = 1.4142135623730951;
static const double c_Sqrt3__d = 1.7320508075688772;
static const double c_Golden__d = 1.6180339887498948;

static const float c_Pi = static_cast<float>(c_Pi__d);
static const float c_2Pi = static_cast<float>(c_2Pi__d);
static const float c_4Pi = static_cast<float>(c_4Pi__d);
static const float c_1OverPi = static_cast<float>(c_1OverPi__d);
static const float c_1Over2Pi = static_cast<float>(c_1Over2Pi__d);
static const float c_1Over4Pi = static_cast<float>(c_1Over4Pi__d);
static const float c_PiOver2 = static_cast<float>(c_PiOver2__d);
static const float c_Radians = static_cast<float>(c_Radians__d);
static const float c_Degrees = static_cast<float>(c_Degrees__d);
static const float c_E = static_cast<float>(c_E__d);
static const float c_Log_2_E = static_cast<float>(c_Log_2_E__d);
static const float c_Sqrt2 = static_cast<float>(c_Sqrt2__d);
static const float c_Sqrt3 = static_cast<float>(c_Sqrt3__d);
static const float c_Golden = static_cast<float>(c_Golden__d);

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
	float t = static_cast<float>(LERP(uv[1],0.0,c_2Pi__d));
	float r = std::sqrt(1.0f-std::pow(V.z,2));
	V.x = r*std::cos(t);
	V.y = r*std::sin(t);
}

inline void uniformPointSphere(AtVector& V, float uv[])
{
	V.z = static_cast<float>(LERP(uv[0],-1.0f,1.0f));
	float t = static_cast<float>(LERP(uv[1],0.0f,c_2Pi));
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


inline double myRandom__d()
{
	return static_cast<double>(std::rand())/static_cast<double>(RAND_MAX);
}

inline float myRandom()
{
	return static_cast<float>(static_cast<double>(std::rand())/static_cast<double>(RAND_MAX));
}


// From http://www.cs.rit.edu/~ncs/color/t_convert.html
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
//		if s == 0, then h = -1 (undefined)
inline void RGBtoHSV(const AtColor& rgb, AtColor& hsv)
{
	float min, max, delta;

	min = MIN( MIN( rgb.r, rgb.g), rgb.b );
	max = MAX( MAX( rgb.r, rgb.g), rgb.b );
	hsv.b = max;				// v

	delta = max - min;

	if( max != 0 )
		hsv.g = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined

		hsv.g = 1.0f;
		hsv.r = 1.0f;
		return;
	}

	if( rgb.r == max )
		hsv.r = ( rgb.g - rgb.b ) / delta;		// between yellow & magenta
	else if( rgb.g == max )
		hsv.r = 2.0f + ( rgb.b - rgb.r ) / delta;	// between cyan & yellow
	else
		hsv.r = 4.0f + ( rgb.r - rgb.g ) / delta;	// between magenta & cyan

	hsv.r *= 60;				// degrees
	if( hsv.r < 0 )
		hsv.r += 360;
}

inline void HSVtoRGB( const AtColor& hsv, AtColor& rgb)
{
	int i;
	float f, p, q, t;

	if( hsv.g == 0 ) {
		// achromatic (grey)
		rgb.r = rgb.g = rgb.b = hsv.b;
		return;
	}
	float h = hsv.r;
	h /= 60;			// sector 0 to 5
	i = static_cast<int>(std::floor( h ));
	f = h - i;			// factorial part of h
	p = hsv.b * ( 1.0f- hsv.g );
	q = hsv.b * ( 1.0f - hsv.g * f );
	t = hsv.b * ( 1.0f - hsv.g * ( 1.0f - f ) );

	switch( i ) {
	case 0:
		rgb.r = hsv.b;
		rgb.g = t;
		rgb.b = p;
		break;
	case 1:
		rgb.r = q;
		rgb.g = hsv.b;
		rgb.b = p;
		break;
	case 2:
		rgb.r = p;
		rgb.g = hsv.b;
		rgb.b = t;
		break;
	case 3:
		rgb.r = p;
		rgb.g = q;
		rgb.b = hsv.b;
		break;
	case 4:
		rgb.r = t;
		rgb.g = p;
		rgb.b = hsv.b;
		break;
	default:		// case 5:
		rgb.r = hsv.b;
		rgb.g = p;
		rgb.b = q;
		break;
	}

}

inline float getLuminance(AtColor& rgb)
{
	return 0.2126f*rgb.r + 0.7152f*rgb.g + 0.0722f*rgb.b;
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


