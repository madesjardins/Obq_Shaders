/*
Obq_Toon :

	a Toon shader

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

#ifndef OBQTOON_H
#define OBQTOON_H

#include "O_Common.h"
#include <map>

static const char* c_ToonGradientMessage = "OToon";

enum Obq_Mode{OBQ_MODEPARAMS = 0, OBQ_MODEIMAGE, OBQ_MODEGRADIENT}; 
enum Obq_CompMode{OBQ_COMPADD = 0, OBQ_COMPOVER, OBQ_COMPMAX, OBQ_COMPSCREEN}; // Screen mode : A+B-AB (if A and B [0,1], else A if A>B, else B)
enum Obq_LoopMode{OBQ_LOOPADD = 0, OBQ_LOOPUSEADD, OBQ_LOOPUSEMAX,OBQ_LOOPREMAPCURVE, OBQ_LOOPREMAPCURVEINPUTS};
enum Obq_LoopLightMode{OBQ_LOOPLIGHTADD = 0, OBQ_LOOPLIGHTADDCLAMPED, OBQ_LOOPLIGHTMAX, OBQ_LOOPLIGHTMULT};
enum Obq_ImageType{OBQ_IT1D = 0, OBQ_IT2D};
enum Obq_RemapMode{OBQ_REMAPAVERAGE = 0, OBQ_REMAPLUMINANCE, OBQ_REMAPRGB}; 

// enum for parameters
//
enum ObqMessageGetFltParams { 
	p_global_loopLightMode,
	p_global_compMode,
	p_global_loopMode,
	p_global_multByLightColor,

	p_ambient_color,
	p_ambient_scale,
	p_ambient_multAmbience,

	p_diffuse_color,
	p_diffuse_scale,
	p_diffuse_coverage,
	p_diffuse_softness,
	p_diffuse_mode,
	p_diffuse_gradient,
	p_diffuse_image,
	p_diffuse_imageType,

	p_highlight_color,
	p_highlight_scale,
	p_highlight_exponent,
	p_highlight_coverage,
	p_highlight_softness,
	p_highlight_mode,
	p_highlight_gradient,
	p_highlight_image,
	p_highlight_imageType,

	p_rimlight_color,
	p_rimlight_scale,
	p_rimlight_coverage,
	p_rimlight_softness,
	p_rimlight_mode,
	p_rimlight_gradient,
	p_rimlight_image,
	p_rimlight_imageType,
	p_ambient_fb_str,
	p_diffuse_fb_str,
	p_highlight_fb_str,
	p_rimlight_fb_str,
	p_contour_fb_str,
	p_putAlphaInFb,
	p_diffuse_autoRemap,
	p_diffuse_imageRemapLeft,
	p_diffuse_imageRemapRight,
	p_highlight_autoRemap,
	p_highlight_imageRemapLeft,
	p_highlight_imageRemapRight,
	p_rimlight_autoRemap,
	p_rimlight_imageRemapLeft,
	p_rimlight_imageRemapRight,
	p_diffuse_imageRemapBias,
	p_diffuse_imageRemapGain,
	p_highlight_imageRemapBias,
	p_highlight_imageRemapGain,
	p_rimlight_imageRemapBias,
	p_rimlight_imageRemapGain,
	p_output_imageOnUV,
	p_opacity,
	p_normalizeObjectID,
	p_ambient_noInternal,
	p_diffuse_noInternal,
	p_highlight_noInternal,
	p_rimlight_noInternal,
	p_diffuse_linearDot,
	p_highlight_linearDot,
	p_rimlight_linearDot,
	p_diffuse_shaderInput,
	p_highlight_shaderInput,
	p_global_remapMode,
	p_global_remapMultByColor,
	p_global_clamp
};

// shader data struct
//
typedef struct 
{
	int global_loopLightMode;
	int global_compMode;
	int global_loopMode;
	bool global_multByLightColor;

	bool ambient_multAmbience;

	int diffuse_mode;
	int diffuse_imageType;

	int highlight_mode;
	int highlight_imageType;

	int rimlight_mode;
	int rimlight_imageType;
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
	bool output_imageOnUV;
	AtColor ambience;

	
	bool diffuse_autoRemap;
	bool highlight_autoRemap;
	bool rimlight_autoRemap;

	float diffuse_autoRemapLeft;
	float highlight_autoRemapLeft;
	float rimlight_autoRemapLeft;

	float diffuse_autoRemapRight;
	float highlight_autoRemapRight;
	float rimlight_autoRemapRight;

	std::map<AtNode*,float>* objectMap;

	bool ambient_noInternal;
	bool diffuse_noInternal;
	bool highlight_noInternal;
	bool rimlight_noInternal;
	
	bool diffuse_linearDot;
	bool highlight_linearDot;
	bool rimlight_linearDot;

	bool diffuse_doInput;
	bool highlight_doInput;

	int global_remapMode;
	bool global_remapMultByColor;

	bool global_clamp;
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


inline float changeRange(float input, float oStart, float oEnd, float bias, float gain)
{
	float t = 0.0f;

	if(bias !=0.5f)
		t = BIAS(t,bias);

	if(gain !=0.5f)
			t = GAIN(t,gain);

	// this is done after to make sure auto remap works
	if(oEnd == oStart)
		t = 0.0f;			// simple test to prevent /0
	else if(oEnd < oStart)
		t = 1.0f-(input - oEnd)/(oStart-oEnd);		// Inverted old range
	else
		t = (input - oStart)/(oEnd-oStart);

	// no need to clamped, it's done later
	return t;
}


// TODO : manage more than horizontal gradient
inline void inode_image_make_coord(AtVector &texCoords, float dot, float angle, int imageType, bool do_remap = false, float left = 0.0f, float right = 1.0f, float bias = 0.5f, float gain = 0.5f)
{

	//gradient in horizontal
	float nDot = (do_remap?changeRange(dot, left, right, bias, gain):dot);
	texCoords.x= CLAMP(nDot,0.0f,0.9999f);	// otherwise, this loops because 1 = 0, or cause artifacts with arnold
	texCoords.y=0.5f;						// in the middle
	texCoords.z=0.0f;
}


#endif