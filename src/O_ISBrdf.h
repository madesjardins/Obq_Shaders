/*
O_ISBrdf.h :

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
#ifndef ISBrdf_H
#define ISBrdf_H

#include "O_Common.h"
#include <queue>
#include "O_BrdfRead.h"


////-----------------------------------------------------------
///*! Rotates a vector V around the X axis with angle theta. */
////-----------------------------------------------------------
//inline glm::dvec3 rotateVectorAroundX(const glm::dvec3& V, /*!< [in] : Vector to rotate.*/
//	double theta  /*!< [in] : The angle of rotation.*/)
//{
//	double cosTheta = glm::cos(theta);
//	double sinTheta = glm::sin(theta);
//
//	return glm::dvec3(V.x,cosTheta*V.y-sinTheta*V.z,sinTheta*V.y+cosTheta*V.z);
//}
//
////-----------------------------------------------------------
///*! Rotates a vector V around the Y axis with angle theta. */
////-----------------------------------------------------------
//inline glm::dvec3 rotateVectorAroundY(const glm::dvec3& V, /*!< [in] : Vector to rotate.*/
//	double theta  /*!< [in] : The angle of rotation.*/)
//{
//	double cosTheta = glm::cos(theta);
//	double sinTheta = glm::sin(theta);
//
//	return glm::dvec3(V.x*cosTheta+V.z*sinTheta,V.y,V.z*cosTheta - V.x*sinTheta);
//}
//
////-----------------------------------------------------------
///*! Rotates a vector V around the Z axis with angle theta. */
////-----------------------------------------------------------
//inline glm::dvec3 rotateVectorAroundZ(const glm::dvec3& V, /*!< [in] : Vector to rotate.*/
//	double theta  /*!< [in] : The angle of rotation.*/)
//{
//	double cosTheta = glm::cos(theta);
//	double sinTheta = glm::sin(theta);
//
//	return glm::dvec3(cosTheta*V.x - sinTheta*V.y,sinTheta*V.x+cosTheta*V.y,V.z);
//}

//inline double angle(const glm::dvec3& A,const glm::dvec3& B)
//{
//	double dot = glm::dot(A,B);
//	if(dot>=1.0)
//		return 0.0;
//	else if(dot <= -1.0)
//		return c_Pi;
//	else
//		return std::acos(dot);
//}
//
//inline float angle(const glm::vec3& A,const glm::vec3& B)
//{
//	float dot = glm::dot(A,B);
//	if(dot>1.0f)
//		return 0.0f;
//	else if(dot < -1.0f)
//		return c_Pi;
//	else
//		return std::acos(dot);
//}
//
//inline float angle(const AtVector& A,const AtVector& B)
//{
//	float dot = AiV3Dot(A,B);
//	if(dot>1.0f)
//		return 0.0f;
//	else if(dot < -1.0f)
//		return c_Pi;
//	else
//		return std::acos(dot);
//}
//
//inline float angle(const AtVector2& A,const AtVector2& B)
//{
//	float dot = AiV2Dot(A,B);
//	if(dot>1.0f)
//		return 0.0f;
//	else if(dot < -1.0f)
//		return c_Pi;
//	else
//		return std::acos(dot);
//}
//
//inline float angle(const glm::vec2& A,const glm::vec2& B)
//{
//	float dot = glm::dot(A,B);
//	if(dot>1.0f)
//		return 0.0f;
//	else if(dot < -1.0f)
//		return c_Pi;
//	else
//		return std::acos(dot);
//}

////----------------------------------------------------------------------
///*! \brief Calculates solid angle of triangle abc (3d points) using the Girard formula. */
////-----------------------------------------------------------------------
//inline double getSolidAngleOfTriangle(glm::dvec3 a, glm::dvec3 b,glm::dvec3 c)
//{
//	// calculate angle A between projection of b and c on tangent plane to a
//	glm::dvec3 n_a = -a;
//	glm::dvec3 v_ab = glm::normalize(b-a);
//	glm::dvec3 v_ac = glm::normalize(c-a);
//	glm::dvec3 t_ab = glm::normalize(v_ab - glm::dot(n_a,v_ab) * (n_a));
//	glm::dvec3 t_ac = glm::normalize(v_ac - glm::dot(n_a,v_ac) * (n_a));
//
//	double A = angle(t_ab,t_ac);
//
//	glm::dvec3 n_b = -b;
//	glm::dvec3 v_ba = glm::normalize(a-b);
//	glm::dvec3 v_bc = glm::normalize(c-b);
//	glm::dvec3 t_ba = glm::normalize(v_ba - glm::dot(n_b,v_ba) * (n_b));
//	glm::dvec3 t_bc = glm::normalize(v_bc - glm::dot(n_b,v_bc) * (n_b));
//
//	double B = angle(t_ba,t_bc);
//
//	glm::dvec3 n_c = -c;
//	glm::dvec3 v_cb = glm::normalize(b-c);
//	glm::dvec3 v_ca = glm::normalize(a-c);
//	glm::dvec3 t_cb = glm::normalize(v_cb - glm::dot(n_c,v_cb) * (n_c));
//	glm::dvec3 t_ca = glm::normalize(v_ca - glm::dot(n_c,v_ca) * (n_c));
//
//	double C = angle(t_cb,t_ca);
//
//	return A+B+C-c_Pi;
//}
//
////------------------------------------------------------------------------------------------------------------
///*! \brief Calculate the delta phi.
//*   Calculate the angle between the projected view direction and light direction vectors onto the surface. */
////------------------------------------------------------------------------------------------------------------
//inline float calculateDeltaPhi(AtVector L, AtVector V, AtVector N, float ndotl, float ndotv){
//
//	// test pour cas spéciaux triviaux
//	if(ndotv >= 1.0-AI_EPSILON || ndotl >= 1.0-AI_EPSILON)
//		return 0.0;
//
//	//http://www.gamedev.net/topic/345149-projecting-a-vector-on-a-plane/
//	AtVector Lp = AiV3Normalize(L - ndotl * N);
//	AtVector Vp = AiV3Normalize(V - ndotv * N);
//
//	return angle(Vp, Lp);
//}
//
////------------------------------------------------------------------------------------------------------------
///*! \brief Calculate the delta phi.
//*   Calculate the angle between the projected view direction and light direction vectors onto the surface. */
////------------------------------------------------------------------------------------------------------------
//inline float calculateDeltaPhi(const AtVector& L, const AtVector& V, const AtVector& N){
//
//	float ndotl = CLAMP(AiV3Dot(L,N),-1.0f,1.0f), ndotv = CLAMP(AiV3Dot(V,N),-1.0f,1.0f);
//
//	// test pour cas spéciaux triviaux
//	if(ndotv >= 1.0f-AI_EPSILON || ndotl >= 1.0f-AI_EPSILON)
//		return 0.0f;
//
//	//http://www.gamedev.net/topic/345149-projecting-a-vector-on-a-plane/
//	AtVector Lp = AiV3Normalize(L - ndotl * N);
//	AtVector Vp = AiV3Normalize(V - ndotv * N);
//
//	return angle(Vp, Lp);
//}
//
//inline void cartesians2sphericalDeltaCoords(AtVector& coords, const AtVector& L, const AtVector& V, const AtVector& N)
//{
//    coords[0] = angle(L,N);
//	coords[1] = angle(V,N);
//    coords[2] = calculateDeltaPhi(L,V,N);
//}

///////////////////////////
//
// Simple bin for IS data
//
///////////////////////////
class ObqISBin{
public:
	ObqISBin(float _theta= 0.0f, float _phi= 0.0f, double _pdf = 0.0, double _cdf = 0.0, double _solid = 0.0):theta(_theta),phi(_phi),pdf(_pdf),cdf(_cdf),solid(_solid){;}
	void set(float _theta, float _phi, double _pdf, double _cdf, double _solid){theta=_theta;phi=_phi;pdf=_pdf;cdf=_cdf;solid=_solid;}
	bool operator>(const ObqISBin& rhs) const
	{
			return pdf>rhs.pdf;
	}
	bool operator>=(const ObqISBin& rhs) const
	{
			return (pdf>=rhs.pdf);
	}
	bool operator<(const ObqISBin& rhs) const
	{
			return (pdf<rhs.pdf);
	}
	bool operator<=(const ObqISBin& rhs) const
	{
			return (pdf<=rhs.pdf);
	}
	bool operator==(const ObqISBin& rhs) const
	{
			return (pdf==rhs.pdf);
	}
	bool operator!=(const ObqISBin& rhs) const
	{
			return (pdf!=rhs.pdf);
	}
	float theta;
	float phi;
	double pdf;
	double cdf;
	double solid;
};

inline bool compareObqISBins (const ObqISBin& lhs, const ObqISBin& rhs) { return (lhs<rhs); }

///////////////////////////
//
// ISBrdf file format : res isRes
//
///////////////////////////
class ISBrdf{
public:
	ISBrdf(bool _useHalfs = false, bool _useLuminance = false): m_init(false),m_isInit(false), m_useHalfs(_useHalfs), m_useLuminance(_useLuminance){;}
	~ISBrdf()
	{
		destroyData();
	}

	inline void destroyData(){
		if(m_init)
		{
			delete[] m_data;
			delete[] m_solidAngles;
		}

		if(m_isInit)
		{
			delete[] m_isData;
			delete[] m_isSolidAngles;
			delete[] m_isSumPdf;
			delete[] m_half_GoE;
			delete[] m_half_L;
		}

		m_init = false;
		m_isInit = false;
	}

	ISBrdf(const char* filename, AtUInt32 isRes, bool _useHalfs = false, bool _useLuminance = true):
		m_init(false),
		m_isInit(false),
		m_useHalfs(_useHalfs),
		m_useLuminance(_useLuminance)
	{


		// load normal brdf with solid angles data calculated in double
		AiMsgInfo("Now opening brdf : \"%s\"", filename);
		std::string fn(filename);

		std::string filetype = fn.substr(fn.rfind("."));

		if(filetype.compare(".mskbrdf")==0)
			readMSKBrdf(filename);
		else if(filetype.compare(".binary")==0)
			readRegIso(filename);
		else
			AiMsgError("BRDF not of suported type.");

		// use Importance sampling
		if(isRes > 0)
		{
			AiMsgInfo("Creating Importance sampling data", filename);
			createIS(isRes);
		}

	}

	//----------------------------------------------------------------------------------
	/*! \brief Returns pdf value for a certain color using different method */
	//----------------------------------------------------------------------------------
		inline double getPdfValueOfColor(const AtColor& color)
		{
			if(m_useLuminance)
				return (0.2126*color.r + 0.7152*color.g + 0.0722*color.b);
			else
				return (color.r + color.g + color.b)/3.0;
		}

	//----------------------------------------------------------------------------------
	/*! \brief Returns the index in one dimension with the 3 indices. Extra carefully */
	//----------------------------------------------------------------------------------
	inline AtUInt32 getArrayIndex(  AtUInt32 ti,    /*!< [in] : Index of thetaIn.*/
		AtUInt32 to,    /*!< [in] : Index of thetaOut.*/
		AtUInt32 dp    /*!< [in] : Index of deltaPhi.*/
		){

			if( to == 0 || ti == 0)
				dp = 0;

			return ti*m_toDpRes+to*m_2xRes+dp;

	}
	//----------------------------------------------------------------------------------
	/*! \brief Returns the index in one dimension with the 3 indices. Extra carefully */
	//----------------------------------------------------------------------------------
	inline AtUInt32 getArrayIndex(  float ti,    /*!< [in] : Index of thetaIn.*/
		float to,    /*!< [in] : Index of thetaOut.*/
		float dp    /*!< [in] : Index of deltaPhi.*/
		){

			if( to == 0.0f || ti == 0.0f)
				dp = 0.0f;

			return static_cast<AtUInt32>(ti)*m_toDpRes+static_cast<AtUInt32>(to)*m_2xRes+static_cast<AtUInt32>(dp);

	}

	//----------------------------------------------------------------------------------------
	/*! \brief Set the corresponding bin with this color.*/
	//----------------------------------------------------------------------------------------
	inline void setDataFromIndices(AtUInt32 ti,      /*!< [in] : Value of thetaIn in radians.*/
		AtUInt32 to,     /*!< [in] : Value of thetaOut in radians.*/
		AtUInt32 dp,  /*!< [in] : Value of deltaPhi in radians.*/
		glm::dvec3 color//, /*!< [in] : Color to set to the bin.*/
		//MSKChannelType channelType = e_RayTypeDiffuse
		)
	{

		// if to equals 0 then all the delta phis are equal
		if(to==0 || ti == 0)
			dp=0;

		AtUInt32 index = getArrayIndex(ti,to,dp);
		m_data[index].r = static_cast<float>(color.r);
		m_data[index].g = static_cast<float>(color.g);
		m_data[index].b = static_cast<float>(color.b);
	}

	//-----------------------------------------------------------------------------------------------
	/*! \brief Returns the index of a parameter based on the maximum value and the (resolution-1). */
	//-----------------------------------------------------------------------------------------------
	inline AtVector getIndexFromAngleInterpolated(  float angle,          /*!< [in] : Angle of parameter.*/
		float resolutionM1d,  /*!< [in] : Resolution - 1 of parameter.*/
		float maxAngle        /*!< [in] : Maximum value for this parameter.*/
		){

			float index = angle/maxAngle*resolutionM1d;
			AtVector v = {glm::floor(index),glm::ceil(index),index};
			return v;

	}

	//-----------------------------------------------------------------------------------------------
	/*! \brief Returns the index of a parameter based on the maximum value and the (resolution-1). */
	//-----------------------------------------------------------------------------------------------
	inline float getAngleFromIndex( AtUInt32 index,          /*!< [in] : index of parameter.*/
									float resolutionM1f,  /*!< [in] : Resolution - 1 of parameter.*/
									float maxAngle        /*!< [in] : Maximum value for this parameter.*/
									)
	{
			return maxAngle*static_cast<float>(index)/resolutionM1f;
	}

	inline void testAndCorrectThetaPhi(float& thetaIn,           /*!< [in] : Value of thetaIn in radians.*/
		float& thetaOut,           /*!< [in] : Value of thetaOut in radians.*/
		float& deltaPhi)
	{
		// tests
		if(thetaIn < 0.0f)
			thetaIn = 0.0f;
		if(thetaOut < 0.0f)
			thetaOut = 0.0f;
		if(deltaPhi < 0.0f)
			deltaPhi = std::abs(deltaPhi);
		if(deltaPhi > c_Pi)
			deltaPhi = c_2Pi - deltaPhi;
	}

	//---------------------------------------------------
	/*! \brief Add this color in the corresponding bin. (only valid for simulated brdf multichannel)*/
	//---------------------------------------------------
	inline AtColor getDataInterpolated(	float thetaIn,           /*!< [in] : Value of thetaIn in radians.*/
		float thetaOut,           /*!< [in] : Value of thetaOut in radians.*/
		float deltaPhi)
	{


		// trivial
		if(thetaIn > c_PiOver2 || thetaOut> c_PiOver2)
			return AI_RGB_BLACK;

		// tests
		testAndCorrectThetaPhi(thetaIn,thetaOut,deltaPhi);

		// divide the thetaIns
		AtVector tis = getIndexFromAngleInterpolated(thetaIn,m_resM1f,c_PiOver2);

		AtVector tos = getIndexFromAngleInterpolated(thetaOut,m_resM1f,c_PiOver2);
		AtVector dps = getIndexFromAngleInterpolated(deltaPhi,m_2xResM1f,c_Pi);

		// ti-floor, to-floor, dp-floor
		AtColor tiftofdpf = m_data[getArrayIndex(tis[0],tos[0],dps[0])];

		// ti-floor, to-floor, dp-ceil
		AtColor tiftofdpc = m_data[getArrayIndex(tis[0],tos[0],dps[1])];

		// ti-floor, to-ceil,  dp-floor
		AtColor tiftocdpf = m_data[getArrayIndex(tis[0],tos[1],dps[0])];

		// ti-floor, to-ceil,  dp-ceil
		AtColor tiftocdpc = m_data[getArrayIndex(tis[0],tos[1],dps[1])];

		// ti-ceil,  to-floor, dp-floor
		AtColor tictofdpf = m_data[getArrayIndex(tis[1],tos[0],dps[0])];

		// ti-ceil,  to-floor, dp-ceil
		AtColor tictofdpc = m_data[getArrayIndex(tis[1],tos[0],dps[1])];

		// ti-ceil,  to-ceil,  dp-floor
		AtColor tictocdpf = m_data[getArrayIndex(tis[1],tos[1],dps[0])];

		// ti-ceil,  to-ceil,  dp-ceil
		AtColor tictocdpc = m_data[getArrayIndex(tis[1],tos[1],dps[1])];

		tis[2] = tis[2]-tis[0];
		tos[2] = tos[2]-tos[0];
		dps[2] = dps[2]-dps[0];

		AtColor tiftof = tiftofdpf*(1.0f - dps[2]) + dps[2]*tiftofdpc;
		AtColor tiftoc = tiftocdpf*(1.0f - dps[2]) + dps[2]*tiftocdpc;
		AtColor tif = tiftof*(1.0f-tos[2])+tos[2]*tiftoc;


		AtColor tictof = tictofdpf*(1.0f - dps[2]) + dps[2]*tictofdpc;
		AtColor tictoc = tictocdpf*(1.0f - dps[2]) + dps[2]*tictocdpc;
		AtColor tic = tictof*(1.0f-tos[2])+tos[2]*tictoc;

		return tif*(1.0f-tis[2])+tis[2]*tic;

	}

	//---------------------------------------------------
	/*! \brief Add this color in the corresponding bin. (only valid for simulated brdf multichannel)*/
	//---------------------------------------------------
	inline AtColor getData(	float thetaIn,           /*!< [in] : Value of thetaIn in radians.*/
							float thetaOut,           /*!< [in] : Value of thetaOut in radians.*/
							float deltaPhi)
	{


		// trivial
		if(thetaIn > c_PiOver2 || thetaOut> c_PiOver2)
			return AI_RGB_BLACK;

		// tests
		testAndCorrectThetaPhi(thetaIn,thetaOut,deltaPhi);

		// divide the thetaIns
		AtVector tis = getIndexFromAngleInterpolated(thetaIn,m_resM1f,c_PiOver2);
		AtVector tos = getIndexFromAngleInterpolated(thetaOut,m_resM1f,c_PiOver2);
		AtVector dps = getIndexFromAngleInterpolated(deltaPhi,m_2xResM1f,c_Pi);

		// ti-floor, to-floor, dp-floor
		AtColor c = m_data[getArrayIndex(tis[0],tos[0],dps[0])];

		return c;

	}

	//----------------------------------------------------------------------------------
	/*! \brief Returns the index in one dimension with the 3 indices. Extra carefully */
	//----------------------------------------------------------------------------------
	inline AtUInt32 getIsArrayIndex(  AtUInt32 isTi,    /*!< [in] : Index of thetaIn.*/
		AtUInt32 isTo = 0,    /*!< [in] : Index of thetaOut.*/
		AtUInt32 isDp = 0   /*!< [in] : Index of deltaPhi.*/
		){
			if(isTo==0)
				return isTi*m_isDirRes;
			else
				return isTi*m_isDirRes + 1 + (isTo-1)*m_is2xRes + isDp;

	}

	// load an isbrdf
	void load(const char* filename);

	// write an isbrdf
	void write(const char* filename);

	// take a .brdf or mskbrdf and create an isbrdf
	void initializeBrdf(AtUInt32 resolution);
	void initializeIsBrdf(AtUInt32 resolution);
	void createFromBrdf(const char* filename);


	inline AtUInt32 getToIndex(float theta_o) const {
		return static_cast<AtUInt32>(std::floor(CLAMP(theta_o/c_PiOver2,0.0f,1.0f)*m_isResM1f+0.5f));
	}

	// Binary search
	inline const ObqISBin& findBin(double& t, float theta_o, double rnd, AtUInt32 binMaxDif)
	{
		bool info = true;//(rand()%1000 == 468);

		AtUInt32 a = 0;
		AtUInt32 b = m_isDirRes-1;
		
		AtUInt32 to = getToIndex(theta_o); // round
		AtUInt32 baseIndex = to*m_isDirRes; // index for all the IS direction

		if(m_useHalfs)
		{
			if(rnd >= 0.5)
				a = m_half_GoE[to];
			else
				b = m_half_L[to];
		}

		
		AtUInt32 current = (a+b)/2; // (a+b)/2
		
		
		//AiMsgInfo("Halfs : half_GoE = %u (%f), half_L = %u (%f)", m_half_GoE[ti], m_isData[baseIndex + m_half_GoE[ti]].cdf, m_half_L[ti], m_isData[baseIndex + m_half_L[ti]].cdf);

		while(b-a>binMaxDif)
		{
			//AiMsgInfo("FindBin : %f :  a = %u (%f), current = %u (%f), b = %u (%f)", rnd, a,m_isData[baseIndex + a].cdf, current, m_isData[baseIndex + current].cdf, b, m_isData[baseIndex + b].cdf);

			if(m_isData[baseIndex + current].cdf > rnd)
			{
				b=current;
				current = (a+b)/2;
			}
			else
			{
				a=current;
				current = (a+b)/2;
			}
		}
		///AiMsgInfo("Binary reserch done : %f :  a = %u (%f), current = %u (%f), b = %u (%f)", rnd, a,m_isData[baseIndex + a].cdf, current, m_isData[baseIndex + current].cdf, b, m_isData[baseIndex + b].cdf);
		
		// We should have found an interval worth searching for
		if(m_isData[baseIndex + a].cdf >= rnd)
		{
			t = 0.0;
			//AiMsgInfo("Bin Found : %f :  a = %u (%f)", rnd, a,m_isData[baseIndex + a].cdf);
			return m_isData[baseIndex + a];
		}

		for(AtUInt32 i = a; i < b; i++)
		{
			//AiMsgInfo("Iterative search : %f :  a = %u (%f), i = %u (%f), b = %u (%f)", rnd, a, m_isData[baseIndex + a].cdf, i, m_isData[baseIndex + i].cdf, b, m_isData[baseIndex + b].cdf);

			if(m_isData[baseIndex + i + 1 ].cdf > rnd)
			{
				double aa = m_isData[baseIndex + i].cdf, bb = m_isData[baseIndex + i + 1].cdf;
				double bbmaa = bb-aa;
				t = (rnd-aa)/bbmaa;
				//AiMsgInfo("bin found found : %f :  i = %u (%f), i+1 = %u (%f) -> t = %f", rnd, i,m_isData[baseIndex + i].cdf, i+1,m_isData[baseIndex + i+1].cdf, t);
				return m_isData[baseIndex + i];
			}
		}
		
		
		double aa = m_isData[baseIndex + b].cdf, bb = 1.0;
		double bbmaa = bb-aa;
		t = (rnd-aa)/bbmaa;
		//AiMsgInfo("bin found found : %f :  b = %u (%f), 1.0 -> t = %f", rnd, b, m_isData[baseIndex + b].cdf, t);
		return m_isData[baseIndex + b];

	}

	// U is in the plane of in and V
	inline void getSample(AtColor& c, AtVector& out, float& pdf, const AtVector& in, const AtVector& U, const AtVector& V, const AtVector& W, double rnd, AtUInt32 binMaxDif, bool useJitters)
	{
		// theta in
		float theta_o = std::acos(CLAMP(AiV3Dot(in,V),0.0f,1.0f));

		// get the IS direction
		double t = 0.0;
		ObqISBin bin = findBin(t, theta_o,rnd,binMaxDif);

		// Get phi sign (only necessary for non pole)
		double phiSign = 1.0;
		if(bin.theta > 0.0f)
		{
			if(t >=0.5)
			{
				phiSign = -1.0;
				t = (t-0.5)*2.0; // [0,1) interval
			}
			else
			{
				t*=2.0; // [0,1) interval
			}
		}

		// jitter
		double theta_jitter, phi_jitter;
		if(useJitters)
		{
			double resAngle = c_PiOver2__d/static_cast<double>(m_isResM1f); //full resolution
			double resSubd = 10000.0;

			double l = t*resSubd;
			double m = std::floor(l);
			double n = (l - m); 
			m/=resSubd;

			if(bin.theta == c_PiOver2) // bottom
			{
				theta_jitter = -m*resAngle/2.0;
				phi_jitter = (n-0.5)*resAngle;
			}
			else if(bin.theta == 0.0f) // top
			{
				theta_jitter = m*resAngle/2.0;
				phi_jitter = n*c_2Pi__d;
			}
			else
			{
				theta_jitter = (m-0.5)*resAngle;
				phi_jitter = (n-0.5)*resAngle;
			}
		}
		else
		{
			theta_jitter = 0.0f;
			phi_jitter = 0.0f;
		}
		// get direction
		float theta = CLAMP(bin.theta + static_cast<float>(theta_jitter),0.0f,c_PiOver2);
		float phi = (phiSign>=0.5?bin.phi:-bin.phi) + static_cast<float>(phi_jitter);

		out.x = std::sin(theta)*std::cos(phi);
		out.y = std::cos(theta);
		out.z = std::sin(theta)*std::sin(phi);

		c = getDataInterpolated(theta_o,theta,phi);

		pdf = static_cast<float>(bin.pdf); //static_cast<float>((bin.theta==0.0f?bin.pdf:bin.pdf/*/2.0*/)/**bin.solid*/);

		// get BRDF
		AiV3RotateToFrame (out, U, V, W);
	}

	bool readMSKBrdf(const char* filename);
	bool readRegIso(const char* filename);
	bool createIS(AtUInt32 isRes);
	inline float getISResM1f(){return m_isResM1f;}
	inline float getIS2xResM1f(){return m_is2xResM1f;}
	inline double getSumPdfs(float theta)
	{
		return m_isSumPdf[static_cast<AtUInt32>((theta/c_PiOver2)*m_isResM1f+0.5)];
	}
protected:
	// BRDF
	AtUInt32 m_res;			// Theta_i and theta_o resolution
	AtUInt32 m_2xRes;		// delta phi (phi_d) (2*res-1)
	AtUInt32 m_resM1;		// res-1
	AtUInt32 m_2xResM1;
	AtUInt32 m_toDpRes;		//theta_o*phi_d
	float m_resf;			// float version
	float m_2xResf;
	float m_resM1f;
	float m_2xResM1f;
	float m_toDpResf;
	AtColor* m_data;		// data
	float* m_solidAngles;	// solid angles info

	// Importance samplings
	AtUInt32 m_isRes;		// Theta_i and Theta_o res for importance sampling
	AtUInt32 m_isResM1;		// res-1
	AtUInt32 m_is2xRes;		// DP res for importance sampling
	AtUInt32 m_is2xResM1;
	AtUInt32 m_isToDpRes;		//theta_o*phi_d
	AtUInt32 m_isDirRes;	// The number of directions (normally isRes*is2Res)
	float m_isResf;			// Theta_i and Theta_o res for importance sampling
	float m_isResM1f;			// Theta_i and Theta_o res for importance sampling
	float m_is2xResf;		// DP res for importance sampling// the theta variation applicable (+- ((Pi/2)/m_isResM1f)/2)
	float m_is2xResM1f;
	float m_isToDpResf;
	float m_isThetaVar;		// the theta variation applicable (+- ((Pi/2)/m_isResM1f)/2)
	float m_isPhiVar;		// the phi variation applicable (+- m_is2xRes/2)
	ObqISBin* m_isData;		//	data
	//std::vector<ObqISBin>* m_isDataV; // sorted data (m_isRes vectors)
	float* m_isSolidAngles; // solid angles info
	double* m_isSumPdf;		// pdfsInfo

	// general
	bool m_init;			// initialised ?
	bool m_isInit;			// initialised ?

	bool m_useHalfs; // to test if priority queues are faster than sorting afterwords
	bool m_useLuminance;

	AtUInt32* m_half_GoE;	// cdf value for random >= 0.5
	AtUInt32* m_half_L;		// cdf value for random < 0.5
};

#endif