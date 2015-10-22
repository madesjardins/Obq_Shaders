/*
Obq_ToonSimple :

	a Toon shader

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

#ifndef OBQTOONSIMPLE_H
#define OBQTOONSIMPLE_H

#include "O_Common.h"
#include <map>

enum Obq_SimpleCompMode{OBQ_COMPADD = 0, OBQ_COMPOVER, OBQ_COMPMAX, OBQ_COMPSCREEN}; // Screen mode : A+B-AB (if A and B [0,1], else A if A>B, else B)

// enum for parameters
//
enum ObqMessageGetFltParams { 
	p_global_compMode,
	p_global_multByLightColor,

	p_ambient_color,
	p_ambient_scale,
	p_ambient_multAmbience,

	p_diffuse_color,
	p_diffuse_scale,
	p_diffuse_coverage,
	p_diffuse_softness,

	p_highlight_color,
	p_highlight_scale,
	p_highlight_exponent,
	p_highlight_coverage,
	p_highlight_softness,

	p_rimlight_color,
	p_rimlight_scale,
	p_rimlight_coverage,
	p_rimlight_softness,

	p_ambient_fb_str,
	p_diffuse_fb_str,
	p_highlight_fb_str,
	p_rimlight_fb_str,
	p_contour_fb_str,
	p_putAlphaInFb,

	p_opacity,
	p_normalizeObjectID,
	p_ambient_noInternal,
	p_diffuse_noInternal,
	p_highlight_noInternal,
	p_rimlight_noInternal,

	p_global_clamp
};

// shader data struct
//
typedef struct 
{
	int global_compMode;
	bool global_multByLightColor;

	bool ambient_multAmbience;

	const char* ambient_fb_str;
	const char* diffuse_fb_str;
	const char* highlight_fb_str;
	const char* rimlight_fb_str;
	const char* contour_fb_str;

	bool output_ambient_fb;
	bool output_diffuse_fb;
	bool output_highlight_fb;
	bool output_rimlight_fb;
	bool output_contour_fb;

	bool putAlphaInFb;

	AtColor ambience;

	std::map<AtNode*,float>* objectMap;

	bool ambient_noInternal;
	bool diffuse_noInternal;
	bool highlight_noInternal;
	bool rimlight_noInternal;

	bool global_clamp;

	bool global_noInternal;
}
ShaderData;

//#define MINMAX
inline float inode_dot_modify(float dot, float coverage = 1.0f, float softness = 1.0f, float exponent = 1.0f, bool isRimlight = false){
	
	if(isRimlight)
		dot = 1.0f-dot;

	if(exponent != 1.0f) 
		dot = std::pow(dot,exponent);


	if(coverage != 1.0f)
	{
		if(coverage == 0.0f)
			dot = 0.0f;
		else if(dot <= 1.0f-coverage)
			dot = 0.0f;
		else
			dot = 1.0f-(1.0f-dot)/coverage;
	}

	if(softness != 1.0f)
	{
		if(softness == 0.0f)
		{
			if( dot > 0.0f)
				dot = 1.0f;
			else
				dot = 0.0f;
		}
		else if(dot < softness)
			dot=dot/softness;
		else
			dot=1.0f;
	}

	return CLAMP(dot,0.0f,1.0f);
}


#endif