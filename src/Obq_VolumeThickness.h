/*
Obq_VolumeThickness.h :


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
#ifndef OBQVOLUMETHICKNESS_H
#define OBQVOLUMETHICKNESS_H

#include "O_Common.h"

// Arnold Thingy
//
AI_SHADER_NODE_EXPORT_METHODS(ObqVolumeThicknessMethods);

// enum of parameters
//
enum ObqVolumeThicknessParams {p_colorIn, p_emissionColor, p_alphaMode, p_alpha, p_thicknessMode, p_thickness, p_accumulatedThicknessThreshold, p_radius, p_exponent, p_useAbsorption, p_absorption, p_diffuseMode, p_autoTransparencyMode, p_shadowModifier, p_lightPropagationDistance, p_doubleHemisphericalDiffuse, p_lightPropagationDistanceDiffuse };

enum ObqVTThicknessMode{SPHERE_MODE, THICKNESS_MODE};
enum OvtFlag{FIRST_IT, NEXT_IT, FRONTFACE_IT, BACKFACE_IT, DIFFUSE_IT, UNKNOWN_IT, RESET_IT};
enum OvtAuto{AUTO_ALWAYS, AUTO_SHADOWONLY};
enum OvtDiffuse{ DIFFUSE_BLACK, DIFFUSE_DIRECT, DIFFUSE_DIRECTOPACITY, DIFFUSE_DIRECTSQRTOPACITY, DIFFUSE_NORMAL, DIFFUSE_EMISSION};
enum ovtAlpha{ALPHA_ALPHA, ALPHA_DIFFUSE, ALPHA_EMISSION, ALPHA_MAX};


typedef struct 
{
   int max_diffuse_depth;
   int diffuseMode;
   float accumulatedThicknessThreshold;
   int autoTransparencyMode;
   bool useLightPropagation;
   float lightPropagationDistance;
   bool useLightPropagationDiffuse;
   float lightPropagationDistanceDiffuse;
   bool useShadowModifier;
   float shadowModifier;
   int alphaMode;
   //bool diffuseNonZero;
   bool doubleHemisphericalDiffuse;
}
ShaderData;


inline void addAllDirect(AtShaderGlobals *sg, ShaderData* data, bool kdColorNonZero, AtColor* out) // Schlick k = 0;
{

	if(kdColorNonZero)
	{
		////////////////////////////
		// Normal 0 gives all lights
		////////////////////////////
		AtVector nf = sg->Nf;
		sg->Nf.x = 0.0;sg->Nf.y = 0.0;sg->Nf.z = 0.0;

		AiLightsPrepare(sg);
		while (AiLightsGetSample(sg))
			if(AiLightGetAffectDiffuse(sg->Lp))
			{
				if(data->useShadowModifier)
					*out += sg->we*c_1Over4Pi*(sg->Liu*(1.0f-data->shadowModifier) + data->shadowModifier*sg->Li);
				else
					*out += sg->we*c_1Over4Pi*sg->Li;
			}

			sg->Nf = nf;
	}
}


#endif // OBQVOLUMETHICKNESS_H
