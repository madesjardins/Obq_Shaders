/*
Obq_UVRemapLensDistortion v2.06.0a (SItoA 2.6.0 - Arnold 4.0.11.0) :

Just plug this node into the arnold_uv_remap_persp_camera to generate
distortion maps on the fly. 3DEqualizer distortion models uses headers
from the Science-D-Visions LDPK.

*------------------------------------------------------------------------
Copyright (c) 2013 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)
LDPK : (C) 2011 - Science-D-Visions. Current version: 1.1

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

#include "ai.h"
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ldpk/ldpk.h>
#include <ldpk/ldpk_cylindric_extender.h>
#include <ldpk/ldpk_rotation_extender.h>
#include <ldpk/ldpk_squeeze_extender.h>
#include <ldpk/ldpk_linear_extender.h>
// Struct
//
typedef struct 
{
	float aspect;
	float width;
	float height;
	float pixelRatio;
	AtPoint pixelOffset;
	ldpk::classic_3de_mixed_distortion<ldpk::vec2d,ldpk::mat2d>* classic3d;
	ldpk::generic_anamorphic_distortion<ldpk::vec2d,ldpk::mat2d,6>* anamorphic6;
	ldpk::generic_radial_distortion<ldpk::vec2d,ldpk::mat2d,8>* fisheye8;
	ldpk::radial_decentered_distortion<ldpk::vec2d,ldpk::mat2d>* standard4;

	// radial decentered cylindric degree 4 
	ldpk::radial_decentered_distortion<ldpk::vec2d,ldpk::mat2d>* _radial;
	ldpk::cylindric_extender_2<ldpk::vec2d,ldpk::mat2d>* _cylindric;

	// anamorphic rotation squeeze 
	ldpk::generic_anamorphic_distortion<ldpk::vec2d,ldpk::mat2d,4>* _anamorphic;
	ldpk::rotation_extender<ldpk::vec2d,ldpk::mat2d>* _rotation;
	ldpk::squeeze_x_extender<ldpk::vec2d,ldpk::mat2d>* _squeeze_x;
	ldpk::squeeze_y_extender<ldpk::vec2d,ldpk::mat2d>* _squeeze_y;
	ldpk::squeeze_x_extender<ldpk::vec2d,ldpk::mat2d>* _pa;

//! Concatenating extenders for better performance
	ldpk::linear_extender<ldpk::vec2d,ldpk::mat2d>* _rot_sqx_sqy_pa;
	ldpk::linear_extender<ldpk::vec2d,ldpk::mat2d>* _pa_rot;
}
ShaderData;


// Arnold thingy
//
AI_SHADER_NODE_EXPORT_METHODS(ObqUVRemapLensDistortionSimpleMethods);


// Enum for params
//
enum ObqUVRemapLensDistortionSimpleParams {p_distortionModel, p_k1, p_k2, p_centerX, p_centerY, p_anamorphicSqueeze, p_asymmetricDistortionX,  p_asymmetricDistortionY, p_centerX3DEq, p_centerY3DEq,p_filmbackX3DEq, p_filmbackY3DEq,p_pixelRatio3DEq, p_c3dc00, p_c3dc01, p_c3dc02, p_c3dc03, p_c3dc04, p_ana6c00, p_ana6c01,p_ana6c02,p_ana6c03,p_ana6c04,p_ana6c05,p_ana6c06,p_ana6c07,p_ana6c08,p_ana6c09, p_ana6c10, p_ana6c11,p_ana6c12,p_ana6c13,p_ana6c14,p_ana6c15,p_ana6c16,p_ana6c17, p_fish8c00, p_fish8c01, p_fish8c02, p_fish8c03, p_stand4c00, p_stand4c01, p_stand4c02, p_stand4c03, p_stand4c04, p_stand4c05, p_raddec4c00, p_raddec4c01, p_raddec4c02, p_raddec4c03, p_raddec4c04, p_raddec4c05, p_raddec4c06, p_raddec4c07, p_ana4c00, p_ana4c01,p_ana4c02,p_ana4c03,p_ana4c04,p_ana4c05,p_ana4c06,p_ana4c07,p_ana4c08,p_ana4c09, p_ana4c10, p_ana4c11,p_ana4c12};

// Enum for distortion model
//
enum ObqDistortionModel{NUKE,CLASSIC3DE,ANAMORPHIC6,FISHEYE8,STANDARD4,RADIAL_DECENTERED_CYLINDRIC4, ANAMORPHIC4};


node_parameters
{
	AiParameterINT("distortionModel" , 0);
	AiParameterFLT("k1" , 0.0f);
	AiParameterFLT("k2" , 0.0f);
	AiParameterFLT("centerX" , 0.0f);
	AiParameterFLT("centerY" , 0.0f);
	AiParameterFLT("anamorphicSqueeze" , 1.0f);
	AiParameterFLT("asymmetricDistortionX" , 0.0f);
	AiParameterFLT("asymmetricDistortionY" , 0.0f);
	AiParameterFLT("centerX3DEq" , 0.0f);
	AiParameterFLT("centerY3DEq" , 0.0f);
	AiParameterFLT("filmbackX3DEq" , 0.3465f);
	AiParameterFLT("filmbackY3DEq" , 0.1949f);
	AiParameterFLT("pixelRatio3DEq" , 1.0f);
	AiParameterFLT("c3dc00" , 0.0f);
	AiParameterFLT("c3dc01" , 1.0f);
	AiParameterFLT("c3dc02" , 0.0f);
	AiParameterFLT("c3dc03" , 0.0f);
	AiParameterFLT("c3dc04" , 0.0f);
	AiParameterFLT("ana6c00" , 0.0f);
	AiParameterFLT("ana6c01" , 0.0f);
	AiParameterFLT("ana6c02" , 0.0f);
	AiParameterFLT("ana6c03" , 0.0f);
	AiParameterFLT("ana6c04" , 0.0f);
	AiParameterFLT("ana6c05" , 0.0f);
	AiParameterFLT("ana6c06" , 0.0f);
	AiParameterFLT("ana6c07" , 0.0f);
	AiParameterFLT("ana6c08" , 0.0f);
	AiParameterFLT("ana6c09" , 0.0f);
	AiParameterFLT("ana6c10" , 0.0f);
	AiParameterFLT("ana6c11" , 0.0f);
	AiParameterFLT("ana6c12" , 0.0f);
	AiParameterFLT("ana6c13" , 0.0f);
	AiParameterFLT("ana6c14" , 0.0f);
	AiParameterFLT("ana6c15" , 0.0f);
	AiParameterFLT("ana6c16" , 0.0f);
	AiParameterFLT("ana6c17" , 0.0f);
	AiParameterFLT("fish8c00" , 0.0f);
	AiParameterFLT("fish8c01" , 0.0f);
	AiParameterFLT("fish8c02" , 0.0f);
	AiParameterFLT("fish8c03" , 0.0f);
	AiParameterFLT("stand4c00" , 0.0f);
	AiParameterFLT("stand4c01" , 0.0f);
	AiParameterFLT("stand4c02" , 0.0f);
	AiParameterFLT("stand4c03" , 0.0f);
	AiParameterFLT("stand4c04" , 0.0f);
	AiParameterFLT("stand4c05" , 0.0f);
	AiParameterFLT("raddec4c00" , 0.0f);
	AiParameterFLT("raddec4c01" , 0.0f);
	AiParameterFLT("raddec4c02" , 0.0f);
	AiParameterFLT("raddec4c03" , 0.0f);
	AiParameterFLT("raddec4c04" , 0.0f);
	AiParameterFLT("raddec4c05" , 0.0f);
	AiParameterFLT("raddec4c06" , 0.0f);
	AiParameterFLT("raddec4c07" , 0.0f);
	AiParameterFLT("ana4c00" , 0.0f);
	AiParameterFLT("ana4c01" , 0.0f);
	AiParameterFLT("ana4c02" , 0.0f);
	AiParameterFLT("ana4c03" , 0.0f);
	AiParameterFLT("ana4c04" , 0.0f);
	AiParameterFLT("ana4c05" , 0.0f);
	AiParameterFLT("ana4c06" , 0.0f);
	AiParameterFLT("ana4c07" , 0.0f);
	AiParameterFLT("ana4c08" , 0.0f);
	AiParameterFLT("ana4c09" , 0.0f);
	AiParameterFLT("ana4c10" , 0.0f);
	AiParameterFLT("ana4c11" , 1.0f);
	AiParameterFLT("ana4c12" , 1.0f);
}


// NUKE Distortion model
//
// Use the Nuke distortion model to distort undistorted camera rays
//
// @param out			The result will be put here
// @param x				The x coordinate [-1,1]
// @param y				The x coordinate [-1/aspectRatio,1/aspectRatio]
// @param k1			The first distortion coefficient
// @param k2			The second distortion coefficient
// @param centerX		The relative center offset in X
// @param centerY		The relative center offset in Y
// @param anaSq			The anamorphic squeeze
// @param asyX			The asymetric distortion coefficient in X
// @param asyY			The asymetric distortion coefficient in X
// @param aspectRatio	The aspect ratio of the frame
//
void distortNuke(AtPoint* out, double x, double y, 
	double k1, double k2, 
	double centerX, double centerY, 
	double anaSq, double asyX, double asyY, 
	double aspectRatio)
{

	// Aspect ratio
	if(aspectRatio < 1.0)
	{
		centerX/=aspectRatio;
		centerY/=aspectRatio;
		// Decentering
		x-=centerX;
		y-=centerY;

		// Normalization compensation
		x*=aspectRatio;
		y*=aspectRatio;
	}
	else
	{
		x-=centerX;
		y-=centerY;
	}

	// Radius
	const double x2 = x*x;
	const double y2 = y*y;
	const double r2 = x2+y2;
	const double r4 = r2*r2;

	// Quick Test
	if(anaSq==0.0f) 
		anaSq = 0.001;

	// Anamorphic squeeze
	asyX/=anaSq;
	const double k1x = k1/anaSq;
	const double k2x = k2/anaSq;

	// Distortion
	x /= (1.0 + k1x*r2 + asyX*y2 + k2x*r4);
	y /= (1.0 + k1*r2 + asyY*x2 + k2*r4);
	
	// Aspect Ratio
	if(aspectRatio < 1.0)
	{
		x/=aspectRatio;
		y/=aspectRatio;
	}

	// Re-center
	// Out
	out->x = float(x+centerX);
	out->y = float(y+centerY);
}

node_initialize
{  
	// Allocate Memory
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));

	// Initialize
	data->aspect = 1.0f;
	data->width = 1920.0f;
	data->height = 1080.0f;
	data->pixelRatio = 1.0f;
	data->pixelOffset.x = -0.5f/data->width;
	data->pixelOffset.y = -0.5f/data->height;

	// Allocate
	data->classic3d = new ldpk::classic_3de_mixed_distortion<ldpk::vec2d,ldpk::mat2d>;
	data->anamorphic6 = new ldpk::generic_anamorphic_distortion<ldpk::vec2d,ldpk::mat2d,6>; // 18 params
	data->fisheye8 = new ldpk::generic_radial_distortion<ldpk::vec2d,ldpk::mat2d,8>;		// 4 params
	data->standard4 = new ldpk::radial_decentered_distortion<ldpk::vec2d,ldpk::mat2d>;		// 6 param

	data->_radial = new ldpk::radial_decentered_distortion<ldpk::vec2d,ldpk::mat2d>;
	data->_cylindric = new ldpk::cylindric_extender_2<ldpk::vec2d,ldpk::mat2d>;


	data->_anamorphic = new ldpk::generic_anamorphic_distortion<ldpk::vec2d,ldpk::mat2d,4>;
	data->_rotation = new ldpk::rotation_extender<ldpk::vec2d,ldpk::mat2d>;
	data->_squeeze_x = new ldpk::squeeze_x_extender<ldpk::vec2d,ldpk::mat2d>;
	data->_squeeze_y = new ldpk::squeeze_y_extender<ldpk::vec2d,ldpk::mat2d>;
	data->_pa = new ldpk::squeeze_x_extender<ldpk::vec2d,ldpk::mat2d>;
	data->_rot_sqx_sqy_pa = new ldpk::linear_extender<ldpk::vec2d,ldpk::mat2d>;
	data->_pa_rot = new ldpk::linear_extender<ldpk::vec2d,ldpk::mat2d>;
	
	// Set data
	AiNodeSetLocalData(node,data);
	
}

node_update
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	// Update shaderData variables
	AtNode* options = AiUniverseGetOptions();
	data->width  = float(AiNodeGetInt(options,"xres"));
	data->height = float(AiNodeGetInt(options,"yres"));
	
	data->pixelRatio = 1.0f/AiNodeGetFlt(options,"aspect_ratio");
	data->aspect = data->width/(data->height/data->pixelRatio);
	data->pixelOffset.x = -0.5f/data->width;
	data->pixelOffset.y = -0.5f/data->height;

	// Conditionnal update because spdl logic changes the hidden parameters
	switch(params[p_distortionModel].INT)
	{
	case CLASSIC3DE:
		data->classic3d->set_coeff(0,params[p_c3dc00].FLT);
		data->classic3d->set_coeff(1,params[p_c3dc01].FLT);
		data->classic3d->set_coeff(2,params[p_c3dc02].FLT);
		data->classic3d->set_coeff(3,params[p_c3dc03].FLT);
		data->classic3d->set_coeff(4,params[p_c3dc04].FLT);
		data->classic3d->eval(ldpk::vec2d(0.0,0.0)); // updates
		break;
	case ANAMORPHIC6:
		data->anamorphic6->set_coeff(0,params[p_ana6c00].FLT);
		data->anamorphic6->set_coeff(1,params[p_ana6c01].FLT);
		data->anamorphic6->set_coeff(2,params[p_ana6c02].FLT);
		data->anamorphic6->set_coeff(3,params[p_ana6c03].FLT);
		data->anamorphic6->set_coeff(4,params[p_ana6c04].FLT);
		data->anamorphic6->set_coeff(5,params[p_ana6c05].FLT);
		data->anamorphic6->set_coeff(6,params[p_ana6c06].FLT);
		data->anamorphic6->set_coeff(7,params[p_ana6c07].FLT);
		data->anamorphic6->set_coeff(8,params[p_ana6c08].FLT);
		data->anamorphic6->set_coeff(9,params[p_ana6c09].FLT);
		data->anamorphic6->set_coeff(10,params[p_ana6c10].FLT);
		data->anamorphic6->set_coeff(11,params[p_ana6c11].FLT);
		data->anamorphic6->set_coeff(12,params[p_ana6c12].FLT);
		data->anamorphic6->set_coeff(13,params[p_ana6c13].FLT);
		data->anamorphic6->set_coeff(14,params[p_ana6c14].FLT);
		data->anamorphic6->set_coeff(15,params[p_ana6c15].FLT);
		data->anamorphic6->set_coeff(16,params[p_ana6c16].FLT);
		data->anamorphic6->set_coeff(17,params[p_ana6c17].FLT);
		data->anamorphic6->eval(ldpk::vec2d(0.0,0.0));
		break;
	case FISHEYE8:
		data->fisheye8->set_coeff(0,params[p_fish8c00].FLT);
		data->fisheye8->set_coeff(1,params[p_fish8c01].FLT);
		data->fisheye8->set_coeff(2,params[p_fish8c02].FLT);
		data->fisheye8->set_coeff(3,params[p_fish8c03].FLT);
		data->fisheye8->set_coeff(4,0.0);
		data->fisheye8->set_coeff(5,0.0);
		data->fisheye8->set_coeff(6,0.0);
		data->fisheye8->set_coeff(7,0.0);
		data->fisheye8->eval(ldpk::vec2d(0.0,0.0));
		break;
	case STANDARD4:
		data->standard4->set_coeff(0,params[p_stand4c00].FLT);
		data->standard4->set_coeff(1,params[p_stand4c01].FLT);
		data->standard4->set_coeff(2,params[p_stand4c02].FLT);
		data->standard4->set_coeff(3,params[p_stand4c03].FLT);
		data->standard4->set_coeff(4,params[p_stand4c04].FLT);
		data->standard4->set_coeff(5,params[p_stand4c05].FLT);
		data->standard4->eval(ldpk::vec2d(0.0,0.0));
		break;
	case RADIAL_DECENTERED_CYLINDRIC4:
		data->_radial->set_coeff(0,params[p_raddec4c00].FLT);
		data->_radial->set_coeff(1,params[p_raddec4c01].FLT);
		data->_radial->set_coeff(2,params[p_raddec4c02].FLT);
		data->_radial->set_coeff(3,params[p_raddec4c03].FLT);
		data->_radial->set_coeff(4,params[p_raddec4c04].FLT);
		data->_radial->set_coeff(5,params[p_raddec4c05].FLT);
		data->_cylindric->set_phi(params[p_raddec4c06].FLT);
		data->_cylindric->set_b(params[p_raddec4c07].FLT);
		data->_cylindric->eval(data->_radial->eval(ldpk::vec2d(0.0,0.0)));
		break;
	case ANAMORPHIC4:
		data->_anamorphic->set_coeff(0,params[p_ana4c00].FLT);
		data->_anamorphic->set_coeff(1,params[p_ana4c01].FLT);
		data->_anamorphic->set_coeff(2,params[p_ana4c02].FLT);
		data->_anamorphic->set_coeff(3,params[p_ana4c03].FLT);
		data->_anamorphic->set_coeff(4,params[p_ana4c04].FLT);
		data->_anamorphic->set_coeff(5,params[p_ana4c05].FLT);
		data->_anamorphic->set_coeff(6,params[p_ana4c06].FLT);
		data->_anamorphic->set_coeff(7,params[p_ana4c07].FLT);
		data->_anamorphic->set_coeff(8,params[p_ana4c08].FLT);
		data->_anamorphic->set_coeff(9,params[p_ana4c09].FLT);
		data->_rotation->set_phi(params[p_ana4c10].FLT*AI_DTOR);
		data->_squeeze_x->set_sq(params[p_ana4c11].FLT);
		data->_squeeze_y->set_sq(params[p_ana4c12].FLT);
		data->_pa->set_sq(data->aspect);
		data->_rot_sqx_sqy_pa->set(*(data->_rotation), (*data->_squeeze_x), (*data->_squeeze_y), (*data->_pa));
		data->_pa_rot->set((*data->_pa),(*data->_rotation));
		data->_anamorphic->prepare();
		data->_rot_sqx_sqy_pa->eval(data->_anamorphic->eval(data->_pa_rot->eval_inv(ldpk::vec2d(0.0,0.0))));
		break;
	}

}

node_finish
{
	// Desallocate shader data memory
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	delete data->classic3d;
	delete data->anamorphic6;
	delete data->fisheye8;
	delete data->standard4;
	delete data->_radial;
	delete data->_cylindric;
	delete data->_anamorphic;
	delete data->_rotation;
	delete data->_squeeze_x;
	delete data->_squeeze_y;
	delete data->_pa;
	delete data->_rot_sqx_sqy_pa;
	delete data->_pa_rot;
	AiFree(data);
}

shader_evaluate
{
	const AtParamValue* params = AiNodeGetParams(node);

	// AspectRatio
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	float aspect = data->aspect;

	// Distorted point
	AtPoint dp;
	int model = params[p_distortionModel].INT;

	// Remap with fix
	double x =  (sg->u + data->pixelOffset.x - 0.5f)*2.0f;
	double y = ((sg->v + data->pixelOffset.y - 0.5f)*2.0f)/aspect;

	// Model
	if(model == NUKE)
	{
		distortNuke(&dp,x,y,params[p_k1].FLT, params[p_k2].FLT, params[p_centerX].FLT, params[p_centerY].FLT, params[p_anamorphicSqueeze].FLT, params[p_asymmetricDistortionX].FLT,params[p_asymmetricDistortionY].FLT,	aspect);
	}
	else
	{

		// Offset Center
		double centerX = 2.0*params[p_centerX3DEq].FLT/params[p_filmbackX3DEq].FLT;
		double centerY = 2.0*params[p_centerY3DEq].FLT/params[p_filmbackY3DEq].FLT/aspect;

		// Pixel Ratio doesn't change anything because the offset and the filmback uses the same pixel ratio
		// double pixelRatio = params[p_pixelRatio3DEq].FLT;

		// Offset center
		x-=centerX;
		y-=centerY;

		// Diagonally normalized
		const double norm = sqrt(1.0+(1.0/(aspect*aspect)));
		x/=norm;
		y/=norm;

		ldpk::vec2d p; // Distorted point

		switch(model)
		{
		case CLASSIC3DE: // 3DEq model
			p = (data->classic3d->eval(ldpk::vec2d(x,y)));
			break;
		case ANAMORPHIC6: // 3DEq model
			p = (data->anamorphic6->eval(ldpk::vec2d(x,y)));
			break;
		case FISHEYE8: // 3DEq model
			p = (data->fisheye8->eval(ldpk::vec2d(x,y)));
			break;
		case STANDARD4: // 3DEq model
			p = (data->standard4->eval(ldpk::vec2d(x,y)));
			break;
		case RADIAL_DECENTERED_CYLINDRIC4:
			p = (data->_cylindric->eval(data->_radial->eval(ldpk::vec2d(x,y))));
			break;
		case ANAMORPHIC4:
			p = data->_rot_sqx_sqy_pa->eval(data->_anamorphic->eval(data->_pa_rot->eval_inv(ldpk::vec2d(x,y))));
			break;
		}

		// Renorm and recenter
		dp.x = float(p[0]*norm+centerX);
		dp.y = float(p[1]*norm+centerY);

	}

	// Compute exit coordinates
	sg->out.RGB.r = (dp.x+1.0f)/2.0f;
	sg->out.RGB.g = (dp.y*aspect+1.0f)/2.0f;
	sg->out.RGB.b = 0.0f;

	// Test if color is corrupted and warn about it
	if(AiColorCorrupted(sg->out.RGB)){
		AiMsgWarning("!! Obq_UVRemapLensDistortion : Corrupted coordinates found !!");
		sg->out.RGB = AI_RGB_BLACK;
	}
}

//node_loader
//{
//	if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqUVRemapLensDistortionSimpleMethods;
//	node->output_type  = AI_TYPE_RGB;
//	node->name         = "Obq_UVRemapLensDistortion";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}
