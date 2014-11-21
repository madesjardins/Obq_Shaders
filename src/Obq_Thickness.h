/*
Obq_Thickness.h :

	is a shader that tells information concerning the thickness of a surface in a certain direction
	or the number of objects in this direction

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
#ifndef OBQTHICKNESS_H
#define OBQTHICKNESS_H

#include "O_Common.h"

// Arnold Thingy
//
AI_SHADER_NODE_EXPORT_METHODS(ObqThicknessMethods);

// enum of parameters
//
enum ObqThicknessParams { p_sampleLevel, p_coneAngle, p_useVdotNForMaxAngle, p_cosLobeGloss, p_useSampleCount, p_sampleCount,p_sampleCountMultiplier, p_backface, p_intersectOthers, p_maxRayDepth, p_normalizeMode, p_maxRayLength, p_useMaxRayLength, p_dirMode, p_dirInverted, p_dirSmooth, p_dirGeometric, p_dirSafe, p_customDirection, p_ior, p_noHitMode, p_multNdotR, p_NdotRExp, p_enterSurface, p_geometricLimitMultiplier, p_thicknessMode, p_multiplyByDepth};

// enum for direction mode
//
enum { D_VIEW, D_NOR, D_NOR_FF, D_RFL, D_RFR, D_CUSTOM };

// Shader Data Structure
//
typedef struct 
{
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


//-------------------------------------------------------------------------------
// This function trace a ray to calculate depth of hit pixel in direction rayDir
//
//	@param sg				: the shaderGlobal
//	@param p				: the collision point from which we calculate depth
//	@param rayDir			: direction in which we want to calculate thickness
//	@param intersectOthers	: do we intersect other objects ?
//	@param maxDepth			: the maximum ray depth
//	@param maxLength		: the maximum lenght of a ray
//	@param noHitValue		: the value if there are no hit
//	@param numOfHitMode		: 2nd thickness mode
//	@param multNdotR		: multiply by NdotR
//	@param exponent			: eponent for dot
//	@param volumeMode		: 3rd mode of thickness
//	@param multiplyByDepth	: For 3rd mode to consider intersecting volumes
//---------------------------------------------------------------------------------
inline float depthRayTrace(AtShaderGlobals *sg, AtPoint* p, AtVector* rayDir, bool intersectOthers, int maxDepth, double oMaxLength, float noHitValue, bool numOfHitMode, bool multNdotR, float exponent, bool volumeMode, bool multiplyByDepth)
{
	// temporary sg and probe ray
	AtShaderGlobals  *sgProbe = new AtShaderGlobals(*sg);
	AtRay *newRay = new AtRay();
	float NdotRs = 0.0;
	float numOfHit = 0.0;

	
	if(numOfHitMode)
		numOfHit = multNdotR?std::pow(std::abs(AiV3Dot(sg->Rd,sg->Nf)),exponent) :1.0f;
	else if(multNdotR)
		NdotRs = std::pow(std::abs(AiV3Dot(sg->Rd,sg->Nf)),exponent);
	
	// Temporary origin
	AtPoint tempOrigin = sg->P; 

	// lenght
	double maxLength = oMaxLength;

	// object normal sign ( ++ on face forward, -- on backward )
	long onsign = 1; 
	
	// if we are inside a thickness volume
	if(volumeMode && AiV3Dot(sg->Ng,sg->Rd) > 0.0f)
	{
		numOfHit = AiV3Dist(tempOrigin,sg->Ro);
		onsign = 0;
	}
	
	// maxray depth ?
	bool useMaxDepth = maxDepth > 0;
	if(!useMaxDepth)
		maxDepth = 1;

	
	do{
		AiMakeRay(newRay, AI_RAY_CAMERA, p, rayDir, maxLength, sgProbe);
			
		// If an intersection occurs, calculate de distance and return it
		// If no intersection occurs, return 0
		
		if(AiTraceProbe(newRay,sgProbe))
		{

			if(volumeMode)
			{
				p[0] = sgProbe->P;

				float dot = AiV3Dot(sgProbe->Rd,sgProbe->Ng);
				if( dot < 0 ) // enter volume
				{
					onsign++;
					if(multiplyByDepth && onsign > 1)
					{
						if(multNdotR)
							numOfHit+=AiV3Dist(p[0],tempOrigin)*NdotRs;
						else
							numOfHit+=float(onsign-1)*AiV3Dist(p[0],tempOrigin);

						tempOrigin = p[0];
					}
					else if(onsign == 1) // enter new volume
					{
						tempOrigin = p[0];
					}

					if(multNdotR)
							NdotRs += std::pow(std::abs(AiV3Dot(sgProbe->Rd,sgProbe->Nf)),exponent);		// on add le nouveau NdotR

				}
				else if(dot > 0) // exit volume
				{
					onsign--;
					if(multiplyByDepth && onsign > 0)
					{
						if(multNdotR)
						{
							numOfHit+=AiV3Dist(p[0],tempOrigin)*NdotRs;
						}
						else
							numOfHit+=float(onsign+1)*AiV3Dist(p[0],tempOrigin);

						tempOrigin = p[0];
					}
					else if(onsign == 0) // we've just left a segment
					{
						if(multNdotR)
						{
							numOfHit+=AiV3Dist(p[0],tempOrigin)*NdotRs;
						}
						else
							numOfHit+=AiV3Dist(p[0],tempOrigin); 	//add distance
					}
					else if(onsign < 0) // interieur d'un volume return
					{
						delete newRay;
						delete sgProbe;

						return numOfHit;
					}

					if(multNdotR)
							NdotRs = std::max(0.0f, NdotRs - std::pow(std::abs(AiV3Dot(sgProbe->Rd,sgProbe->Nf)),exponent));

				}
				//else do nothing it's like a no hit
			}
			else if(numOfHitMode)
			{
				p[0] = sgProbe->P;

				if(multNdotR)
					numOfHit+=std::pow(std::abs(AiV3Dot(sgProbe->Rd,sgProbe->Nf)),exponent);
				else
					numOfHit++;
			}
			else if(intersectOthers || sgProbe->Op == sg->Op){
				float ret = AiV3Length(sgProbe->P - sg->P);
				delete newRay;
				delete sgProbe;
				return ret;
			}
			else
			{
				// Trace new ray from this intersection
				p[0] = sgProbe->P;
			}

			// Substract from max lenght
			maxLength = oMaxLength - AiV3Dist(sg->P,sgProbe->P);

		}
		else // No Hit
		{
			delete newRay;
			delete sgProbe;
			if(numOfHitMode)
				return numOfHit;
			else if( volumeMode)
			{
				return numOfHit;
			}
			else
				return noHitValue;
		}
		if(useMaxDepth)
			maxDepth--;

	}while( maxDepth > 0 );

	// free memory
	delete newRay;
	delete sgProbe;

	if(numOfHitMode)
		return numOfHit;
	else if( volumeMode)
	{
		if(onsign > 0 && !multiplyByDepth) // we were not finish and never accumulated distance until now
		{
			if(multNdotR)
				numOfHit+=AiV3Dist(p[0],tempOrigin)*NdotRs;
			else
				numOfHit+=AiV3Dist(p[0],tempOrigin);
		}

		return numOfHit;
	}
	else
		return noHitValue;
}



#endif // OBQTHICKNESS_H
