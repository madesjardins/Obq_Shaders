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
#include <cstring>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#ifdef _WIN32
	#include <glm\glm.hpp>
#else
	#include <glm/glm.hpp>
#endif


#define OBQ_AI_VERSION (AI_VERSION_ARCH_NUM*10000+AI_VERSION_MAJOR_NUM*100+AI_VERSION_MINOR_NUM)

#define MIN_LAMBDA 360.f
#define MAX_LAMBDA 830.f

// enum for file type
//
enum ObqFileType {TYPE_CSV, TYPE_TXT, TYPE_SPD, TYPE_BAD};

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
static const double c_EpsilonF__d = 0.0001;
static const double c_Sixth__d = 1.0/6.0;

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
static const float c_EpsilonF = static_cast<float>(c_EpsilonF__d);
static const float c_Sixth = static_cast<float>(c_Sixth__d);

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


inline double angle(const glm::dvec3& A,const glm::dvec3& B)
{
	double dot = glm::dot(A,B);
	if(dot>=1.0)
		return 0.0;
	else if(dot <= -1.0)
		return c_Pi;
	else
		return std::acos(dot);
}

inline float angle(const glm::vec3& A,const glm::vec3& B)
{
	float dot = glm::dot(A,B);
	if(dot>1.0f)
		return 0.0f;
	else if(dot < -1.0f)
		return c_Pi;
	else
		return std::acos(dot);
}

inline float angle(const AtVector& A,const AtVector& B)
{
	float dot = AiV3Dot(A,B);
	if(dot>1.0f)
		return 0.0f;
	else if(dot < -1.0f)
		return c_Pi;
	else
		return std::acos(dot);
}

inline float angle(const AtVector2& A,const AtVector2& B)
{
	float dot = AiV2Dot(A,B);
	if(dot>1.0f)
		return 0.0f;
	else if(dot < -1.0f)
		return c_Pi;
	else
		return std::acos(dot);
}

inline float angle(const glm::vec2& A,const glm::vec2& B)
{
	float dot = glm::dot(A,B);
	if(dot>1.0f)
		return 0.0f;
	else if(dot < -1.0f)
		return c_Pi;
	else
		return std::acos(dot);
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


//-----------------------------------------------------------
/*! Rotates a vector V around the X axis with angle theta. */
//-----------------------------------------------------------
inline glm::dvec3 rotateVectorAroundX(const glm::dvec3& V, /*!< [in] : Vector to rotate.*/
	double theta  /*!< [in] : The angle of rotation.*/)
{
	double cosTheta = glm::cos(theta);
	double sinTheta = glm::sin(theta);

	return glm::dvec3(V.x,cosTheta*V.y-sinTheta*V.z,sinTheta*V.y+cosTheta*V.z);
}

//-----------------------------------------------------------
/*! Rotates a vector V around the Y axis with angle theta. */
//-----------------------------------------------------------
inline glm::dvec3 rotateVectorAroundY(const glm::dvec3& V, /*!< [in] : Vector to rotate.*/
	double theta  /*!< [in] : The angle of rotation.*/)
{
	double cosTheta = glm::cos(theta);
	double sinTheta = glm::sin(theta);

	return glm::dvec3(V.x*cosTheta+V.z*sinTheta,V.y,V.z*cosTheta - V.x*sinTheta);
}

//-----------------------------------------------------------
/*! Rotates a vector V around the Z axis with angle theta. */
//-----------------------------------------------------------
inline glm::dvec3 rotateVectorAroundZ(const glm::dvec3& V, /*!< [in] : Vector to rotate.*/
	double theta  /*!< [in] : The angle of rotation.*/)
{
	double cosTheta = glm::cos(theta);
	double sinTheta = glm::sin(theta);

	return glm::dvec3(cosTheta*V.x - sinTheta*V.y,sinTheta*V.x+cosTheta*V.y,V.z);
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


//----------------------------------------------------------------------
/*! \brief Calculates solid angle of triangle abc (3d points) using the Girard formula. */
//-----------------------------------------------------------------------
inline double getSolidAngleOfTriangle(glm::dvec3 a, glm::dvec3 b,glm::dvec3 c)
{
	// calculate angle A between projection of b and c on tangent plane to a
	glm::dvec3 n_a = -a;
	glm::dvec3 v_ab = glm::normalize(b-a);
	glm::dvec3 v_ac = glm::normalize(c-a);
	glm::dvec3 t_ab = glm::normalize(v_ab - glm::dot(n_a,v_ab) * (n_a));
	glm::dvec3 t_ac = glm::normalize(v_ac - glm::dot(n_a,v_ac) * (n_a));

	double A = angle(t_ab,t_ac);

	glm::dvec3 n_b = -b;
	glm::dvec3 v_ba = glm::normalize(a-b);
	glm::dvec3 v_bc = glm::normalize(c-b);
	glm::dvec3 t_ba = glm::normalize(v_ba - glm::dot(n_b,v_ba) * (n_b));
	glm::dvec3 t_bc = glm::normalize(v_bc - glm::dot(n_b,v_bc) * (n_b));

	double B = angle(t_ba,t_bc);

	glm::dvec3 n_c = -c;
	glm::dvec3 v_cb = glm::normalize(b-c);
	glm::dvec3 v_ca = glm::normalize(a-c);
	glm::dvec3 t_cb = glm::normalize(v_cb - glm::dot(n_c,v_cb) * (n_c));
	glm::dvec3 t_ca = glm::normalize(v_ca - glm::dot(n_c,v_ca) * (n_c));

	double C = angle(t_cb,t_ca);

	return A+B+C-c_Pi;
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

//------------------------------------------------------------------------------------------------------------
/*! \brief Calculate the delta phi.
*   Calculate the angle between the projected view direction and light direction vectors onto the surface. */
//------------------------------------------------------------------------------------------------------------
inline float calculateDeltaPhi(AtVector L, AtVector V, AtVector N, float ndotl, float ndotv){

	// test pour cas spéciaux triviaux
	if(ndotv >= 1.0-AI_EPSILON || ndotl >= 1.0-AI_EPSILON)
		return 0.0;

	//http://www.gamedev.net/topic/345149-projecting-a-vector-on-a-plane/
	AtVector Lp = AiV3Normalize(L - ndotl * N);
	AtVector Vp = AiV3Normalize(V - ndotv * N);

	return angle(Vp, Lp);
}

//------------------------------------------------------------------------------------------------------------
/*! \brief Calculate the delta phi.
*   Calculate the angle between the projected view direction and light direction vectors onto the surface. */
//------------------------------------------------------------------------------------------------------------
inline float calculateDeltaPhi(const AtVector& L, const AtVector& V, const AtVector& N){

	float ndotl = CLAMP(AiV3Dot(L,N),-1.0f,1.0f), ndotv = CLAMP(AiV3Dot(V,N),-1.0f,1.0f);

	// test pour cas spéciaux triviaux
	if(ndotv >= 1.0f-AI_EPSILON || ndotl >= 1.0f-AI_EPSILON)
		return 0.0f;

	//http://www.gamedev.net/topic/345149-projecting-a-vector-on-a-plane/
	AtVector Lp = AiV3Normalize(L - ndotl * N);
	AtVector Vp = AiV3Normalize(V - ndotv * N);

	return angle(Vp, Lp);
}

inline void cartesians2sphericalDeltaCoords(AtVector& coords, const AtVector& L, const AtVector& V, const AtVector& N)
{
    coords[0] = angle(L,N);
	coords[1] = angle(V,N);
    coords[2] = calculateDeltaPhi(L,V,N);
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
	if(h>=360.0f)
		h-=360.0f;
	else if(h<0.0f)
		h+=360.0f;
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


// taken from Arnold color_utils.h
inline float HLSValue(float in_n1, float in_n2, float in_hue)
{
	// h is 0->1, l 0->1  s 0->1

	in_hue = in_hue - FLOOR(in_hue);

	if (in_hue < c_Sixth) // 60 º
		return (in_n1 + (in_n2 - in_n1) * in_hue / c_Sixth );
	else if (in_hue < 3 * c_Sixth) // 180 º
		return in_n2;
	else if(in_hue < 4 * c_Sixth) // 240 º
		return (in_n1 + (in_n2 - in_n1) * (4 * c_Sixth - in_hue) / c_Sixth);

	return in_n1;
}


inline void RGBtoHLS(const AtColor& rgb, AtColor& hls)
{     
	// Given r,g,b 0->1 h is 0->1, l 0->1  s 0->1. If s = 0, h = -1

	float delta;

	float cmax = MAX(rgb.r, MAX(rgb.g, rgb.b)); // luminant delta
	float cmin = MIN(rgb.r, MIN(rgb.g, rgb.b));

	hls.g = (cmax + cmin)*0.5f;

	if (cmax == cmin)
	{
		// achromatic case
		hls.r = -1.0f;
		hls.b = 0.0f;
	}
	else
	{
		// chromatic case
		// first calculate the saturation
		if (hls.g < 0.5f)
			hls.b = (cmax - cmin) / (cmax + cmin);
		else
			hls.b = (cmax - cmin) / (2.0f - cmax - cmin);

		delta = cmax - cmin;                   

		if (rgb.r == cmax)
			hls.r = (rgb.g - rgb.b) / delta;           
		else if (rgb.g == cmax)
			hls.r = 2.0f + (rgb.b - rgb.r) / delta;     
		else
			hls.r = 4.0f + (rgb.r -rgb.g) / delta;

		hls.r *= c_Sixth;

		while (hls.r < 0.0f)
			hls.r += 1.0f; // make sure it is nonnegative
	}
}

inline void HLStoRGB(const AtColor& hls, AtColor& rgb)
{
	// h is 0->1, l 0->1  s 0->1. If s = 0, h = -1

	if (hls.b == 0.0f || hls.r == -1.0f)
	{
		rgb.r = hls.g;
		rgb.g = hls.g;
		rgb.b = hls.g;
	}
	else
	{
		float m1,m2;

		if (hls.g <= 0.5f)
			m2 = hls.g * (1.0f + hls.b);
		else
			m2 = hls.g + hls.b - (hls.g * hls.b);

		m1 = 2.0f * hls.g - m2;

		rgb.r = HLSValue(m1, m2, hls.r + 1.0f/3.0f); // 120º   
		rgb.g = HLSValue(m1, m2, hls.r);         
		rgb.b = HLSValue(m1, m2, hls.r - 1.0f/3.0f);   // - 120º
	}
}


// minimax clamps b between a and c
//
// @param a		The lower bound
// @param b		The value that will be clamped
// @param c		The upper bound
// @return		b clamped between a and c
inline float minimax(float a, float b, float c)
{
	if(b<a) 
		return a; 
	else if(b>c) 
		return c;
	else
		return b;
}

// minimax clamps b between a and c
//
// @param a		The lower bound
// @param b		The value that will be clamped
// @param c		The upper bound
// @return		b clamped between a and c
inline double minimax(double a, double b, double c)
{
	if(b<a) 
		return a; 
	else if(b>c) 
		return c;
	else
		return b;
}

inline double mmax(double a, double b, double c)
{
	if(a>b)
	{
		if(a>c)
			return a;
		else
			return c;
	}
	else if(b>c)
		return b;
	else
		return c;
}

inline double mmin(double a, double b, double c)
{
	if(a<b)
	{
		if(a<c)
			return a;
		else
			return c;
	}
	else if(b<c)
		return b;
	else
		return c;
}


// degamma
inline void degamma(double* color)
{
	color[0] = std::pow(color[0],2.2);
	color[1] = std::pow(color[1],2.2);
	color[2] = std::pow(color[2],2.2);
}

inline void sRGB2Linear(double* color)
{
	for(int i=0; i<3; i++)
	{
		double a = minimax(0.0,color[i],1.0);
		if(a <= 0.03928)
			color[i] = a/12.92;
		else
			color[i] = std::exp(2.4 * std::log((a + 0.055) / 1.055));
	}
	//for(int i=0; i<3; i++)
	//{
	//	double a = minimax(0.0,color[i],1.0);
	//	if(a <= 0.040449936)
	//		color[i]= a/12.92;
	//	else
	//		color[i] = std::pow(((a + 0.055) / 1.055),2.4);
	//}
}

inline void sRGB2Linear(float* color)
{
	for(int i=0; i<3; i++)
	{
		float a = minimax(0.0f,color[i],1.0f);
		if(a <= 0.03928f)
			color[i] = a/12.92f;
		else
			color[i] = std::exp(2.4f * std::log((a + 0.055f) / 1.055f));
	}
}

inline void linear2sRGB(double* color)
{
	for(int i=0; i<3; i++)
	{
		double a = minimax(0.0,color[i],1.0);
		if(a <= 0.00304)
			color[i] = a*12.92;
		else
			color[i] = 1.055*std::exp((1.0/2.4) * std::log(a))-0.055;
	}
	/*for(int i=0; i<3; i++)
	{
		double a = minimax(0.0,color[i],1.0);
		if(a <= 0.0031308)
			color[i] = a*12.92;
		else
			color[i] = 1.099*std::pow(1.055*a,1.0/2.4)-0.055;
	}*/
}

inline void linear2sRGB(float* color)
{
	for(int i=0; i<3; i++)
	{
		float a = minimax(0.0f,color[i],1.0f);
		if(a <= 0.00304f)
			color[i] = a*12.92f;
		else
			color[i] = 1.055f*std::exp((1.0f/2.4f) * std::log(a))-0.055f;
	}
}

// Gabriel Fleseriu
template <class T>
inline bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

// maps from [-1,1] to [0,1]
inline float myMAP01F(float a)
{
	return (a + 1.0f) * 0.5f;
} 

// maps from [0,1] to [-1,1]
inline float myMAPm11F(float a)
{
	return 2.0f*a - 1.0f;
}

// maps from [-1,1] to [0,1]
inline double myMAP01D(double a)
{
	return (a + 1.0) * 0.5;
} 

// maps from [0,1] to [-1,1]
inline double myMAPm11D(double a)
{
	return 2.0*a - 1.0;
}


// pbrt source code Copyright(c) 1998-2010 Matt Pharr and Greg Humphreys. This section is a modified part of pbrt.
//
inline void ConcentricSampleDisk(float u1, float u2, int nBlades, float bladeCurvature, float rotation, float *dx, float *dy, bool bokehInvert = false, float bokehBias = 0.5f, float bokehGain = 0.5f) {
	float r, theta;

	// Map uniform random numbers to $[-1,1]^2$
	float sx = 2.0f * u1 - 1.0f;
	float sy = 2.0f * u2 - 1.0f;

	// Map square to $(r,\theta)$

	// Handle degeneracy at the origin
	if (sx == 0.0f && sy == 0.0f) 
	{
		*dx=0.0f;
		*dy = 0.0f;
	}
	if (sx >= -sy) {
		if (sx > sy) {
			// Handle first region of disk
			r = sx;
			if (sy > 0.0f) theta = sy/r;
			else          theta = 8.0f + sy/r;
		}
		else {
			// Handle second region of disk
			r = sy;
			theta = 2.0f - sx/r;
		}
	}
	else {
		if (sx <= sy) {
			// Handle third region of disk
			r = -sx;
			theta = 4.0f - sy/r;
		}
		else {
			// Handle fourth region of disk
			r = -sy;
			theta = 6.0f + sx/r;
		}
	}
	theta *= c_Pi / 4.0f;

	//bokeh quality
	if(bokehBias !=0.5f)
		r = BIAS(r,bokehBias);
	if(bokehGain !=0.5f)
		r = GAIN(r,bokehGain);
	
	if(bokehInvert)
		r= 1.0f-r;

	//----
	// polygonal blade if needed
	if(nBlades > 0 && bladeCurvature != 1.0f)
	{
		// what are floor and ceil of the phi
		rotation = static_cast<float>(c_Radians*rotation);
		float angleArc = c_2Pi / nBlades;
		float t = static_cast<float>((theta - rotation)/angleArc);
		float tf = std::floor(t);
		float tc = std::ceil(t);
		
		// t entre 0 et 1
		t-=tf;
		
		// what are those 2 closest points
		float pf[2] = {	static_cast<float>(r*std::cos(angleArc*tf+rotation)), 
						static_cast<float>(r*std::sin(angleArc*tf+rotation))};

		float pc[2] = {	static_cast<float>(r*std::cos(angleArc*tc+rotation)), 
						static_cast<float>(r*std::sin(angleArc*tc+rotation))};

		// where are we on that line at 0
		float p0[2] = {	pc[0]*t + pf[0]*(1.0f-t) , 
						pc[1]*t + pf[1]*(1.0f-t)};

		// positive blade curvature
		if(bladeCurvature == 0.0f)
		{
			*dx = p0[0];
			*dy = p0[1];
		}
		else if(bladeCurvature > 0.0f )
		{
			*dx = bladeCurvature*(r * std::cos( theta )) + (1.0f-bladeCurvature)*p0[0];
			*dy = bladeCurvature*(r * std::sin( theta )) + (1.0f-bladeCurvature)*p0[1];
		}
		else
		{
			// Positive
			bladeCurvature = -bladeCurvature;

			// at 0 we are at p0, at -1 we are here
			float pm1[2] = {0.0f,0.0f};

			if(t < 0.5f)	// on the floor line
			{
				pm1[0] = (2.0f*(0.5f-t))*pf[0];
				pm1[1] = (2.0f*(0.5f-t))*pf[1];
			}
			else if(t > 0.5) // on the ceil line
			{
				pm1[0] = (2.0f*(t-0.5f))*pc[0];
				pm1[1] = (2.0f*(t-0.5f))*pc[1];
			}

			*dx = pm1[0]*bladeCurvature+(1.0f-bladeCurvature)*p0[0];
			*dy = pm1[1]*bladeCurvature+(1.0f-bladeCurvature)*p0[1];
		}
	}
	else
	{
		*dx = r * std::cos(theta);
		*dy = r * std::sin(theta);
	}

}


// Check if file exists
//
inline bool fileExists(const char* filename)
{
    std::ifstream ifs ( filename , std::ifstream::in );

    if(ifs==NULL)
        return false;

    ifs.close();
    return true;

}

// check for file type
//
inline short fileType(const char* filename)
{
	std::string fname(filename);
	
	if(fname.size() < 4)
		return TYPE_BAD;

	if(fname.compare(fname.size()-4,4,".csv") == 0 || fname.compare(fname.size()-4,4,".CSV") == 0 )
		return TYPE_CSV;
	else if(fname.compare(fname.size()-4,4,".txt") == 0 || fname.compare(fname.size()-4,4,".TXT") == 0 )
		return TYPE_TXT;
	else if(fname.compare(fname.size()-4,4,".spd") == 0 || fname.compare(fname.size()-4,4,".SPD") == 0 )
		return TYPE_SPD;

	return TYPE_BAD;
}

// split a string with character c
//
// Shi Chuan : http://www.blog.highub.com/c-plus-plus/c-parse-split-delimited-string/
//
inline void split(const  std::string& s, char c, std::vector<std::string>& v) {
   std::string::size_type i = 0;
   std::string::size_type j = s.find(c);
   while (j != std::string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);
      if (j == std::string::npos)
         v.push_back(s.substr(i, s.length( )));
   }
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
		double totalOmega = 0.0;

		for(int j = 0; j < toRes; j++){
			double omega = 1.0;


			//////////////////
			// CALCUL DU OMEGA

			// top
			if(j==0)
			{
				//float t = c_PiOver2/static_cast<float>(toRes-1);
				//float ph = c_Pi/static_cast<float>(dpRes-1);
				//AtVector a = {0.0f,1.0f,0.0f};
				//AtVector b = {std::sin(t/2.0f),std::cos(t/2.0f),0.0f};
				//AtVector c = rotateVectorAroundY(b,ph/2.0f);
				//omega = 2.0f*static_cast<float>(dpRes-1)*2.0f*getSolidAngleOfTriangle(a, b, c);

				double t = c_PiOver2__d/static_cast<double>(toRes-1);
				double ph = c_Pi__d/static_cast<double>(dpRes-1);
				glm::dvec3 a(0.0,1.0,0.0);
				glm::dvec3 b(glm::sin(t/2.0),glm::cos(t/2.0),0.0);
				glm::dvec3 c = rotateVectorAroundY(b,ph/2.0);
				omega = 2.0*static_cast<double>(dpRes-1)*2.0*getSolidAngleOfTriangle(a, b, c);

			}
			// bottom
			else if(j == toRes-1)
			{

				/*float t = c_PiOver2/static_cast<float>(toRes-1);
				float jt = static_cast<float>(j)*t;
				float tover2 = t/2.0f;
				float ph = c_Pi/static_cast<float>(dpRes-1);
				float phover2 = ph/2.0f;
				AtVector a = {std::sin(jt),std::cos(jt),0.0f};
				AtVector d = rotateVectorAroundY(a,phover2);
				AtVector b = {std::sin(jt-tover2),std::cos(jt-tover2),0.0f};
				AtVector c = rotateVectorAroundY(b,phover2);

				omega = 2.0f*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d));*/

				double t = c_PiOver2__d/static_cast<double>(toRes-1);
				double jt = static_cast<double>(j)*t;
				double tover2 = t/2.0;
				double ph = c_Pi__d/static_cast<double>(dpRes-1);
				double phover2 = ph/2.0;
				glm::dvec3 a(glm::sin(jt),glm::cos(jt),0.0);
				glm::dvec3 d = rotateVectorAroundY(a,phover2);
				glm::dvec3 b(glm::sin(jt-tover2),glm::cos(jt-tover2),0.0);
				glm::dvec3 c = rotateVectorAroundY(b,phover2);

				omega = 2.0*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d));

			}
			// rest
			else
			{

				//float t = c_PiOver2/static_cast<float>(toRes-1);
				//float jt = static_cast<float>(j)*t;
				//float tover2 = t/2.0f;
				//float ph = c_Pi/static_cast<float>(dpRes-1);
				//float phover2 = ph/2.0f;
				//AtVector a = {std::sin(jt),std::cos(jt),0.0f};
				//AtVector d = rotateVectorAroundY(a,phover2);
				//AtVector b = {std::sin(jt-tover2),std::cos(jt-tover2),0.0f};
				//AtVector c = rotateVectorAroundY(b,phover2);
				//AtVector e = {std::sin(jt+tover2),std::cos(jt+tover2),0.0f};
				//AtVector f = rotateVectorAroundY(e,phover2);

				//omega = 2.0f*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d) + getSolidAngleOfTriangle(a, d, e) + getSolidAngleOfTriangle(d, f, e));

				double t = c_PiOver2__d/static_cast<double>(toRes-1);
				double jt = static_cast<double>(j)*t;
				double tover2 = t/2.0;
				double ph = c_Pi__d/static_cast<double>(dpRes-1);
				double phover2 = ph/2.0;
				glm::dvec3 a(glm::sin(jt),glm::cos(jt),0.0);
				glm::dvec3 d = rotateVectorAroundY(a,phover2);
				glm::dvec3 b(glm::sin(jt-tover2),glm::cos(jt-tover2),0.0);
				glm::dvec3 c = rotateVectorAroundY(b,phover2);
				glm::dvec3 e(glm::sin(jt+tover2),glm::cos(jt+tover2),0.0);
				glm::dvec3 f = rotateVectorAroundY(e,phover2);

				omega = 2.0*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d) + getSolidAngleOfTriangle(a, d, e) + getSolidAngleOfTriangle(d, f, e));


			}

			///////////////////////
			// ASSIGNATION DU OMEGA (reversed for theta-Phi hair )

			if(j==0) // calotte
				m_omega[(toRes-1)-j] = static_cast<float>(omega/2.0);
			else if(j == toRes-1) // On doit doubler pour aller chercher l'autre hémisphère
				m_omega[(toRes-1)-j] = static_cast<float>(2.0*omega);
			else
				m_omega[(toRes-1)-j] = static_cast<float>(omega);
		}

		
		//AiMsgInfo("Hemisphere solid angle = %f (%f pi)",getTotalOmega(),getTotalOmega()/c_Pi);
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


