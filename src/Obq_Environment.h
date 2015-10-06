/*
Obq_Environment.h :

	Based on sib_environment and sphericalLightFilter by nozon.

*------------------------------------------------------------------------
Copyright (c) 2012-2014 Marc-Antoine Desjardins, ObliqueFX (marcantoinedesjardins@gmail.com)

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
#ifndef OBQENVIRONMENT_H
#define OBQENVIRONMENT_H

#include "O_Common.h"


// This is an Arnold thingy
//
AI_SHADER_NODE_EXPORT_METHODS(ObqEnvironmentMethods);


// This is the enumeration for parameters
//
enum ObqEnvironmentParams { p_tex, p_considerAlpha, p_mapMode, p_sampleLevel, p_coneAngle, p_cosLobeGloss, p_useSampleCount, p_sampleCount,p_sampleCountMultiplier, p_onSurfaceMode, p_ior, p_customDirection, p_intensityCam, p_intensityDifGI, p_intensityRfl, p_intensityGlossy, p_intensityRfr, p_globalRotation , p_rotation, p_flipU, p_flipV, p_opaque };


// This is the enumeration for the mapping mode
//
enum ObqMapMode { MIRROREDBALL,ANGULARMAP,LATLONG,CUBICMAP };
static const char* ObqMapModeNames[] = 
{
	"Mirrored Ball",
    "Angular Map",
    "Latitude-Longitude",
    "Vertical Cross",
    NULL
};

// This is the enumeration for the possible directions in surface mode
//
enum ObqOnSurfaceMode { D_VIEW, D_VIEW_INV, D_NOR, D_NOR_SMOOTH, D_NOR_INV, D_NOR_INV_SMOOTH, D_NOR_FF, D_NOR_FF_INV, D_RFL, D_RFL_SMOOTH, D_RFR, D_RFR_SMOOTH, D_CUSTOM };
static const char* ObqOnSurfaceModeNames[] = 
{
	"View Direction",
	"View Direction (Inverted)",
	"Surface Normal Direction",
	"Surface Normal Direction (no bump)",
	"Surface Normal Direction (Inverted)",
	"Surface Normal Direction (no bump, Inverted)",
	"Surface Normal Direction (Front-Facing)",
	"Surface Normal Direction (Back-Facing)",
	"Reflection Direction",
	"Reflection Direction (no bump)",
	"Refraction Direction",
	"Refraction Direction (no bump)",
	"Custom Direction",
	NULL
};


// Shader Data Structure
//
typedef struct 
{
	AtMatrix rotMatrix;					// Matrix rotation
	int mapMode;						// Mapping Mode
	AtSampler   *sampler;				// the random samples
}
ShaderData;


// Adds two AtRGBA and put the results in the first one.
//
// @param a   This is the first AtRGBA to be added and the result is put in here
// @param b   This is the second AtRGBA
//
inline void ObqRGBAAdd(AtRGBA& a, const AtRGBA& b)
{
	a.r += b.r;
	a.g += b.g;
	a.b += b.b;
	a.a += b.a;
}

// Multiplies a AtRGBA by a float and put the results in the AtRGBA.
//
// @param a   This is the AtRGBA to be multiplied and the result is put in here
// @param b   This is the floating point number that multiplies the AtRGBA
//
inline void ObqRGBAScale(AtRGBA& a, float b)
{
	a.r *= b;
	a.g *= b;
	a.b *= b;
	a.a *= b;
}


// Importance sampling spherical coordinates random direction of a cos^n lobe 
//
// This function transforms uv [0,1)^2 into spherical coordinates (rho = 1)
// and puts the results in the params theta and phi
//
// @param u			First [0,1) random coordinates
// @param v			Second [0,1) random coordinates
// @param maxAngle	The maximum theta angle
// @param n			The exponent
// @param theta		The theta out
// @param phi		The phi out
//
inline void UVToSpherical(float u, float v, float maxAngle, float n, float* theta, float* phi){

	*theta = std::acos(std::pow(u,1.0f/(n+1.0f))) * maxAngle/c_PiOver2;
	*phi = v*c_2Pi;
}


// Rotate vector v around vector k by angle radians. 
//
// This function uses the Rodrigues equation to rotate a vector around an arbitrary vector
//
// @param v		The vector to rotate
// @param k		The arbitrary vector around which v is rotated
// @param angle	The amount in radians the rotation is
// @return		The rotated vector
//
inline AtVector RodriguesRotation(AtVector v, AtVector k, float angle)
{
	float cosA = std::cos(angle);
	return v*cosA + AiV3Cross(k,v)*std::sin(angle) + k*AiV3Dot(k,v)*(1.0f-cosA);
}


// Translates a gloss [0,1] value to an exponent [0,infinite). 
//
// @param g		The gloss amount
// @return		The equivalent exponent
//
inline float GlossToExponent(float g)
{
    return g/std::pow(1.0f-g,8.0f);
}


#endif // OBQENVIRONMENT_H
