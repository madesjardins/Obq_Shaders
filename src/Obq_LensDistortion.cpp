/*
Obq_StereoLens.cpp :

Obq_StereoLens is a lens shader that renders both left and right cameras 
of a stereo rig in the same frame, saving you one scene loading time for
each pair of stereo frame. This shader is applied on the center camera.

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

#include "Obq_LensDistortion.h"

// Arnold thingy
// 
AI_CAMERA_NODE_EXPORT_METHODS(ObqLensDistortionMethods);

// Param enum for fast direct access
//
enum Obq_LensDistortionParams { p_useDof, p_focusDistance, p_apertureSize, p_apertureAspectRatio, p_usePolygonalAperture, p_apertureBlades, p_apertureBladeCurvature, p_apertureRotation, p_focusPlaneIsPlane, p_bokehInvert, p_bokehBias, p_bokehGain, p_distortionModel, p_k1, p_k2, p_centerX, p_centerY, p_anamorphicSqueeze, p_asymmetricDistortionX,  p_asymmetricDistortionY, p_centerX3DEq, p_centerY3DEq,p_filmbackX3DEq, p_filmbackY3DEq,p_pixelRatio3DEq, p_c3dc00, p_c3dc01, p_c3dc02, p_c3dc03, p_c3dc04, p_ana6c00, p_ana6c01,p_ana6c02,p_ana6c03,p_ana6c04,p_ana6c05,p_ana6c06,p_ana6c07,p_ana6c08,p_ana6c09, p_ana6c10, p_ana6c11,p_ana6c12,p_ana6c13,p_ana6c14,p_ana6c15,p_ana6c16,p_ana6c17, p_fish8c00, p_fish8c01, p_fish8c02, p_fish8c03, p_stand4c00, p_stand4c01, p_stand4c02, p_stand4c03, p_stand4c04, p_stand4c05, p_raddec4c00, p_raddec4c01, p_raddec4c02, p_raddec4c03, p_raddec4c04, p_raddec4c05, p_raddec4c06, p_raddec4c07, p_ana4c00, p_ana4c01,p_ana4c02,p_ana4c03,p_ana4c04,p_ana4c05,p_ana4c06,p_ana4c07,p_ana4c08,p_ana4c09, p_ana4c10, p_ana4c11,p_ana4c12, p_focal3DEq, p_focusDistance3DEq,p_pfC3, p_pfC5, p_pfSqueeze, p_pfXp, p_pfYp, p_fov};

// ENUM MENU
static const char* ObqDistortionModelNames[] = 
{
	"Nuke",
    "(3DE-1) Classic LD Model",
    "(3DE-2) Anamorphic, Degree 6",
    "(3DE-3) Radial - Fisheye, Degree 8",
    "(3DE-4) Radial - Standard, Degree 4 (DEPRICATED)",
	"(3DE-5) Radial - Decentered Cylindric, Degree 4",
	"(3DE-6) Anamorphic Rotate Squeeze, Degree 4",
	"PFBarrel",
	"None",
    NULL
};

// Shader Data Structure
//
typedef struct 
{
	float aspect;
	float width;
	float height;
	float pixelRatio;
	float tan_myFov;
	bool useDof;
	float focusDistance;
	bool focusPlaneIsPlane;
	float apertureSize;
	bool bokehInvert;
	float bokehBias;
	float bokehGain;
	float apertureAspectRatio;
	bool usePolygonalAperture;
	int apertureBlades;
	float apertureRotation;
	float apertureBladeCurvature;

	int distortionModel;
	AtPoint pixelOffset;
	tde4_ldp_classic_3de_mixed<ldpk::vec2d,ldpk::mat2d>* classic3de;							//classic 3DE
	tde4_ldp_anamorphic_deg_6<ldpk::vec2d,ldpk::mat2d>* anamorphic6;							//anamorphic 6
	tde4_ldp_radial_deg_8<ldpk::vec2d,ldpk::mat2d>* fisheye8;									//fisheye 8
	ldpk::radial_decentered_distortion<ldpk::vec2d,ldpk::mat2d>* standard4;						// Standard 4 old version 
	tde4_ldp_radial_decentered_deg_4_cylindric<ldpk::vec2d,ldpk::mat2d>* radial4cylindric;		// standard 4 with cylindric
	tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy< ldpk::vec2d , ldpk::mat2d >* anamorphicDeg4Sq;	// anamorphic_deg_4_rotate
	
	glm::dvec2 pf_C3C5;		// PFTrack model
	glm::dvec2 pf_absCent;
	double pf_squeeze;
	double pf_invSqueeze;
	glm::dvec2 pf_norm;

	//Nuke
	float n_k1;
	float n_k2;
	float n_centerX;
	float n_centerY;
	float n_anamorphicSqueeze;
	float n_asymmetricDistortionX;
	float n_asymmetricDistortionY;

}
ShaderData;

// Parameters
//
node_parameters
{
	AiParameterBOOL("useDof",false);
	AiParameterFLT("focusDistance",100.0f);
	AiParameterFLT("apertureSize",0.1f);
	AiParameterFLT("apertureAspectRatio",1.0f);
	AiParameterBOOL("usePolygonalAperture",true);
	AiParameterINT("apertureBlades",5);
	AiParameterFLT("apertureBladeCurvature",0.0f);
	AiParameterFLT("apertureRotation",0.0f);
	AiParameterBOOL("focusPlaneIsPlane",true);
	AiParameterBOOL("bokehInvert",false);
	AiParameterFLT("bokehBias",0.5f);
	AiParameterFLT("bokehGain",0.5f);

	AiParameterENUM("distortionModel" , PFBARREL, ObqDistortionModelNames);
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
	AiParameterFLT("focal3DEq" , 5.0f);
	AiParameterFLT("focusDistance3DEq" , 100.0f);
	AiParameterFLT("pfC3" , 0.0f);
	AiParameterFLT("pfC5" , 0.0f);
	AiParameterFLT("pfSqueeze" , 1.0f);
	AiParameterFLT("pfXp" , 0.5f);
	AiParameterFLT("pfYp" , 0.5f);

	AtArray* a = AiArray(2, 1, AI_TYPE_FLOAT, 0.0f,0.0f);
	AiParameterARRAY("fov",a);	

	AiMetaDataSetBool(mds, NULL, "is_perspective", true);
}


node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->aspect = 1.0f;

	// Initialize
	data->aspect = 1.0f;
	data->width = 1920.0f;
	data->height = 1080.0f;
	data->pixelRatio = 1.0f;
	data->pixelOffset.x = 0.0f;//-0.5f/data->width;
	data->pixelOffset.y = 0.0f;//-0.5f/data->height;

	data->tan_myFov = 1.0f;
	data->useDof = false;
	data->usePolygonalAperture = true;
	data->apertureSize = 0.1f;
	data->bokehInvert = false;
	data->bokehBias = 0.5f;
	data->bokehGain = 0.5f;
	data->apertureAspectRatio = 1.0f;
	data->apertureBlades = 5;
	data->apertureBladeCurvature = 0.0f;
	data->apertureRotation = 0.0f;
	data->focusDistance = 100.0f;
	data->focusPlaneIsPlane = true;

	data->distortionModel = NONE;
	// Allocate
	data->classic3de = new tde4_ldp_classic_3de_mixed<ldpk::vec2d,ldpk::mat2d>; // 5 params
	data->anamorphic6 = new tde4_ldp_anamorphic_deg_6<ldpk::vec2d,ldpk::mat2d>; // 18 params
	data->fisheye8 = new tde4_ldp_radial_deg_8<ldpk::vec2d,ldpk::mat2d>; // 4 params
	data->standard4 = new ldpk::radial_decentered_distortion<ldpk::vec2d,ldpk::mat2d>;		// 6 param
	data->radial4cylindric = new tde4_ldp_radial_decentered_deg_4_cylindric<ldpk::vec2d,ldpk::mat2d>; //8 params (6 de standard4 + 2 cylindric)
	data->anamorphicDeg4Sq = new tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy< ldpk::vec2d , ldpk::mat2d>; // 12 params

	data->pf_C3C5 = glm::dvec2(0.0,0.0);		// PFTrack model
	glm::dvec2 pf_absCent = glm::dvec2(0.0,0.0);
	double pf_squeeze = 1.0;
	double pf_invSqueeze = 1.0;
	glm::dvec2 pf_norm = glm::dvec2(1.0,1.0);

	// nuke
	data->n_k1 = 0.0f;
	data->n_k2 = 0.0f;
	data->n_centerX = 0.0f;
	data->n_centerY = 0.0f;
	data->n_anamorphicSqueeze = 1.0f;
	data->n_asymmetricDistortionX = 0.0f;
	data->n_asymmetricDistortionY = 0.0f;

	// Set data
	AiCameraInitialize(node, data);
}

node_update
{
	AiCameraUpdate(node, false);

	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);

	// Update shaderData variables
	AtNode* options = AiUniverseGetOptions();
	data->width  = static_cast<float>(AiNodeGetInt(options,"xres"));
	data->height = static_cast<float>(AiNodeGetInt(options,"yres"));


	// Aspect
	data->pixelRatio = 1.0f/AiNodeGetFlt(options,"aspect_ratio");
	data->aspect = data->width/(data->height/data->pixelRatio);

	// Maybe not useful in this case
	data->pixelOffset.x = 0.0f;//-0.5f/data->width;
	data->pixelOffset.y = 0.0f;//-0.5f/data->height;

	// Field of view
	float fov = AiArrayGetFlt(AiNodeGetArray(node, "fov"),0);
	data->tan_myFov = static_cast<float>(std::tan(fov * c_Radians__d / 2.0));
		
	// DOF
	data->useDof = params[p_useDof].BOOL;

	// DOF related values
	data->focusDistance = params[p_focusDistance].FLT;
	data->focusPlaneIsPlane = params[p_focusPlaneIsPlane].BOOL;
	data->apertureSize = params[p_apertureSize].FLT;
	data->bokehInvert = params[p_bokehInvert].BOOL;
	data->bokehBias = params[p_bokehBias].FLT;
	data->bokehGain = 1.0f-params[p_bokehGain].FLT;
	data->apertureAspectRatio = params[p_apertureAspectRatio].FLT;
	if(data->apertureAspectRatio<=0.0f)
		data->apertureAspectRatio = 0.0001f;

	data->usePolygonalAperture = params[p_usePolygonalAperture].BOOL;
	data->apertureBlades = params[p_apertureBlades].INT;
	data->apertureBladeCurvature = params[p_apertureBladeCurvature].FLT;
	data->apertureRotation = params[p_apertureRotation].FLT;

	//AiMsgInfo("-------DEPTH OF FIELD---------");
	//AiMsgInfo("useDof = %s", (data->useDof?"True":"False"));
	//AiMsgInfo("focusDistance = %f", data->focusDistance);
	//AiMsgInfo("apertureSize = %f", data->apertureSize);
	//AiMsgInfo("apertureAspectRatio = %f", data->apertureAspectRatio);
	//AiMsgInfo("usePolygonalAperture = %s", (data->usePolygonalAperture?"True":"False"));
	//AiMsgInfo("apertureBlades = %i", data->apertureBlades);
	//AiMsgInfo("apertureBladeCurvature = %f", data->apertureBladeCurvature);
	//AiMsgInfo("apertureRotation = %f", data->apertureRotation);
	//AiMsgInfo("------------------------------");


	//////////////////////////////
	// Prepare distortion model
	data->distortionModel = params[p_distortionModel].INT;
	switch(data->distortionModel)
	{
	case NUKE:
		{
			data->n_k1 = params[p_k1].FLT;
			data->n_k2 = params[p_k2].FLT;
			data->n_centerX = params[p_centerX].FLT;
			data->n_centerY = params[p_centerY].FLT;
			data->n_anamorphicSqueeze = params[p_anamorphicSqueeze].FLT;
			data->n_asymmetricDistortionX = params[p_asymmetricDistortionX].FLT;
			data->n_asymmetricDistortionY = params[p_asymmetricDistortionY].FLT;
			break;
		}
	case PFBARREL:
		{
			data->pf_absCent = calcAbsCent(params[p_pfXp].FLT,static_cast<int>(data->width),params[p_pfYp].FLT,static_cast<int>(data->height));
			data->pf_norm = calcNorm(static_cast<int>(data->width),static_cast<int>(data->height));
			data->pf_squeeze = params[p_pfSqueeze].FLT;
			data->pf_invSqueeze = 1.0/data->pf_squeeze;
			data->pf_C3C5 = glm::dvec2(params[p_pfC3].FLT,params[p_pfC5].FLT);
			break;
		}
	case CLASSIC3DE:
		{
		// built-in
		data->classic3de->setParameterValue2("tde4_focal_length_cm",			params[p_focal3DEq].FLT);
		data->classic3de->setParameterValue2("tde4_filmback_width_cm",			params[p_filmbackX3DEq].FLT);
		data->classic3de->setParameterValue2("tde4_filmback_height_cm",			params[p_filmbackY3DEq].FLT);
		data->classic3de->setParameterValue2("tde4_lens_center_offset_x_cm",	params[p_centerX3DEq].FLT);
		data->classic3de->setParameterValue2("tde4_lens_center_offset_y_cm",	params[p_centerY3DEq].FLT);
		data->classic3de->setParameterValue2("tde4_pixel_aspect",				params[p_pixelRatio3DEq].FLT);
		data->classic3de->setParameterValue2("tde4_custom_focus_distance_cm",	params[p_focusDistance3DEq].FLT);
		// model
		data->classic3de->setParameterValue2("Distortion",			params[p_c3dc00].FLT);
		data->classic3de->setParameterValue2("Anamorphic Squeeze",	params[p_c3dc01].FLT);
		data->classic3de->setParameterValue2("Curvature X",			params[p_c3dc02].FLT);
		data->classic3de->setParameterValue2("Curvature Y",			params[p_c3dc03].FLT);
		data->classic3de->setParameterValue2("Quartic Distortion",	params[p_c3dc04].FLT);
		data->classic3de->initializeParameters2();
		break;
		}
	case ANAMORPHIC6:
		{
		// built-in
		data->anamorphic6->setParameterValue2("tde4_focal_length_cm",			params[p_focal3DEq].FLT);
		data->anamorphic6->setParameterValue2("tde4_filmback_width_cm",			params[p_filmbackX3DEq].FLT);
		data->anamorphic6->setParameterValue2("tde4_filmback_height_cm",		params[p_filmbackY3DEq].FLT);
		data->anamorphic6->setParameterValue2("tde4_lens_center_offset_x_cm",	params[p_centerX3DEq].FLT);
		data->anamorphic6->setParameterValue2("tde4_lens_center_offset_y_cm",	params[p_centerY3DEq].FLT);
		data->anamorphic6->setParameterValue2("tde4_pixel_aspect",				params[p_pixelRatio3DEq].FLT);
		data->anamorphic6->setParameterValue2("tde4_custom_focus_distance_cm",	params[p_focusDistance3DEq].FLT);

		data->anamorphic6->setParameterValue2("Cx02 - Degree 2",params[p_ana6c00].FLT);
		data->anamorphic6->setParameterValue2("Cy02 - Degree 2",params[p_ana6c01].FLT);
		data->anamorphic6->setParameterValue2("Cx22 - Degree 2",params[p_ana6c02].FLT);
		data->anamorphic6->setParameterValue2("Cy22 - Degree 2",params[p_ana6c03].FLT);
		data->anamorphic6->setParameterValue2("Cx04 - Degree 4",params[p_ana6c04].FLT);
		data->anamorphic6->setParameterValue2("Cy04 - Degree 4",params[p_ana6c05].FLT);
		data->anamorphic6->setParameterValue2("Cx24 - Degree 4",params[p_ana6c06].FLT);
		data->anamorphic6->setParameterValue2("Cy24 - Degree 4",params[p_ana6c07].FLT);
		data->anamorphic6->setParameterValue2("Cx44 - Degree 4",params[p_ana6c08].FLT);
		data->anamorphic6->setParameterValue2("Cy44 - Degree 4",params[p_ana6c09].FLT);
		data->anamorphic6->setParameterValue2("Cx06 - Degree 6",params[p_ana6c10].FLT);
		data->anamorphic6->setParameterValue2("Cy06 - Degree 6",params[p_ana6c11].FLT);
		data->anamorphic6->setParameterValue2("Cx26 - Degree 6",params[p_ana6c12].FLT);
		data->anamorphic6->setParameterValue2("Cy26 - Degree 6",params[p_ana6c13].FLT);
		data->anamorphic6->setParameterValue2("Cx46 - Degree 6",params[p_ana6c14].FLT);
		data->anamorphic6->setParameterValue2("Cy46 - Degree 6",params[p_ana6c15].FLT);
		data->anamorphic6->setParameterValue2("Cx66 - Degree 6",params[p_ana6c16].FLT);
		data->anamorphic6->setParameterValue2("Cy66 - Degree 6",params[p_ana6c17].FLT);
		
		data->anamorphic6->initializeParameters2();

		break;
		}
	case FISHEYE8:
		{
		// built-in
		data->fisheye8->setParameterValue2("tde4_focal_length_cm",			params[p_focal3DEq].FLT);
		data->fisheye8->setParameterValue2("tde4_filmback_width_cm",		params[p_filmbackX3DEq].FLT);
		data->fisheye8->setParameterValue2("tde4_filmback_height_cm",		params[p_filmbackY3DEq].FLT);
		data->fisheye8->setParameterValue2("tde4_lens_center_offset_x_cm",	params[p_centerX3DEq].FLT);
		data->fisheye8->setParameterValue2("tde4_lens_center_offset_y_cm",	params[p_centerY3DEq].FLT);
		data->fisheye8->setParameterValue2("tde4_pixel_aspect",				params[p_pixelRatio3DEq].FLT);
		data->fisheye8->setParameterValue2("tde4_custom_focus_distance_cm",	params[p_focusDistance3DEq].FLT);

		data->fisheye8->setParameterValue2("Distortion - Degree 2",params[p_fish8c00].FLT);
		data->fisheye8->setParameterValue2("Quartic Distortion - Degree 4",params[p_fish8c01].FLT);
		data->fisheye8->setParameterValue2("Degree 6",params[p_fish8c02].FLT);
		data->fisheye8->setParameterValue2("Degree 8",params[p_fish8c03].FLT);

		data->fisheye8->initializeParameters2();
		break;
		}
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
		{
		data->radial4cylindric->setParameterValue2("tde4_focal_length_cm",			params[p_focal3DEq].FLT);
		data->radial4cylindric->setParameterValue2("tde4_filmback_width_cm",		params[p_filmbackX3DEq].FLT);
		data->radial4cylindric->setParameterValue2("tde4_filmback_height_cm",		params[p_filmbackY3DEq].FLT);
		data->radial4cylindric->setParameterValue2("tde4_lens_center_offset_x_cm",	params[p_centerX3DEq].FLT);
		data->radial4cylindric->setParameterValue2("tde4_lens_center_offset_y_cm",	params[p_centerY3DEq].FLT);
		data->radial4cylindric->setParameterValue2("tde4_pixel_aspect",				params[p_pixelRatio3DEq].FLT);
		data->radial4cylindric->setParameterValue2("tde4_custom_focus_distance_cm",	params[p_focusDistance3DEq].FLT);

		data->radial4cylindric->setParameterValue2("Distortion - Degree 2",params[p_raddec4c00].FLT);
		data->radial4cylindric->setParameterValue2("U - Degree 2",params[p_raddec4c01].FLT);
		data->radial4cylindric->setParameterValue2("V - Degree 2",params[p_raddec4c02].FLT);
		data->radial4cylindric->setParameterValue2("Quartic Distortion - Degree 4",params[p_raddec4c03].FLT);
		data->radial4cylindric->setParameterValue2("U - Degree 4",params[p_raddec4c04].FLT);
		data->radial4cylindric->setParameterValue2("V - Degree 4",params[p_raddec4c05].FLT);
		data->radial4cylindric->setParameterValue2("Phi - Cylindric Direction",params[p_raddec4c06].FLT);
		data->radial4cylindric->setParameterValue2("B - Cylindric Bending",params[p_raddec4c07].FLT);

		data->radial4cylindric->initializeParameters2();
		
		break;
		}
	case ANAMORPHIC4:
		{
		// built-in
		data->anamorphicDeg4Sq->setParameterValue2("tde4_focal_length_cm",			params[p_focal3DEq].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("tde4_filmback_width_cm",		params[p_filmbackX3DEq].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("tde4_filmback_height_cm",		params[p_filmbackY3DEq].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("tde4_lens_center_offset_x_cm",	params[p_centerX3DEq].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("tde4_lens_center_offset_y_cm",	params[p_centerY3DEq].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("tde4_pixel_aspect",				params[p_pixelRatio3DEq].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("tde4_custom_focus_distance_cm",	params[p_focusDistance3DEq].FLT);

		// anamorphic
		data->anamorphicDeg4Sq->setParameterValue2("Cx02 - Degree 2",params[p_ana4c00].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Cy02 - Degree 2",params[p_ana4c01].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Cx22 - Degree 2",params[p_ana4c02].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Cy22 - Degree 2",params[p_ana4c03].FLT);

		data->anamorphicDeg4Sq->setParameterValue2("Cx04 - Degree 4",params[p_ana4c04].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Cy04 - Degree 4",params[p_ana4c05].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Cx24 - Degree 4",params[p_ana4c06].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Cy24 - Degree 4",params[p_ana4c07].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Cx44 - Degree 4",params[p_ana4c08].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Cy44 - Degree 4",params[p_ana4c09].FLT);

		// this
		data->anamorphicDeg4Sq->setParameterValue2("Lens Rotation",params[p_ana4c10].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Squeeze-X",params[p_ana4c11].FLT);
		data->anamorphicDeg4Sq->setParameterValue2("Squeeze-Y",params[p_ana4c12].FLT);

		data->anamorphicDeg4Sq->initializeParameters2();
		
		}
		break;
	default: // NO DISTORTION
		break;
	}
	

}

node_finish
{
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);

	delete data->classic3de;
	delete data->anamorphic6;
	delete data->fisheye8;
	delete data->standard4;
	delete data->radial4cylindric;
	delete data->anamorphicDeg4Sq;

	AiFree(data);
	AiCameraDestroy(node);
}

camera_create_ray 
{
	// User params
	//const AtParamValue* params = AiNodeGetParams(node);

	// AspectRatio
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);
	

	//////////////////
	// DISTORTION
	/////////////////
	float sx = input->sx;
	float sy = input->sy;

	// Distorted and Undistorted points, no need for pixel offset in this case
	double x = myMAP01F(sx), y = myMAP01F(sy*data->aspect);
	double xu=x, yu=y;


	// Model
	switch(data->distortionModel)
	{
	case PFBARREL:
	{
		glm::dvec2 xyu = solveDistort(data->pf_C3C5,data->pf_absCent,data->pf_squeeze,data->pf_invSqueeze,data->pf_norm,glm::dvec2(data->width*(x),data->height*(y)));
		xu = xyu.x/data->width;
		yu = xyu.y/data->height;
		break;
	}
	case NUKE:
	{
		AtPoint dp;

		// Remap with fix
		x =  (x - 0.5f)*2.0f;
		y = ((y - 0.5f)*2.0f)/data->aspect;

		distortNuke(&dp,x,y,data->n_k1, data->n_k2, data->n_centerX, data->n_centerY, data->n_anamorphicSqueeze, data->n_asymmetricDistortionX,data->n_asymmetricDistortionY,	data->aspect);

		// Compute exit coordinates
		xu = (dp.x+1.0)/2.0;
		yu = (dp.y*data->aspect+1.0)/2.0;
		break;
	}
	case CLASSIC3DE: // 3DEq model
		data->classic3de->undistort2(x, y, xu, yu);
		break;
	case ANAMORPHIC6: // 3DEq model
		data->anamorphic6->undistort2(x, y, xu, yu);
		break;
	case FISHEYE8: // 3DEq model
		data->fisheye8->undistort2(x, y, xu, yu);
		break;
	case STANDARD4: // Old 3DEq model replaced by next, leave until next release
		{
			ldpk::vec2d p = (data->standard4->eval(ldpk::vec2d(x,y)));
			xu = p[0];
			yu = p[1];
			break;
		}
	case RADIAL_DECENTERED_CYLINDRIC4: // 3DEq model
		data->radial4cylindric->undistort2(x, y, xu, yu);
		break;
	case ANAMORPHIC4:// 3DEq model
		data->anamorphicDeg4Sq->undistort2(x, y, xu, yu);
		break;
	default:
		break;
	}

	// send undistorted coord
	sx = static_cast<float>(myMAPm11D(xu));
	sy = static_cast<float>(myMAPm11D(yu))/data->aspect;


	//////////////////
	// END DISTORTION
	/////////////////
	
	// Scale derivatives
	float dsx = input->dsx*data->tan_myFov;
	float dsy = input->dsy*data->tan_myFov;

	// Direction
	AtPoint p = { sx, sy, -1.0f/data->tan_myFov };
	output->dir = AiV3Normalize(p - output->origin);

	//////////////////
	// DEPTH OF FIELD
	/////////////////
	if(data->useDof && data->apertureSize > 0.0f)
	{
		float lensU = 0.0f, lensV = 0.0f;
		ConcentricSampleDisk(input->lensx, input->lensy, (data->usePolygonalAperture?data->apertureBlades:0), data->apertureBladeCurvature, data->apertureRotation,&lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
		lensU*=data->apertureSize;
		lensV*=data->apertureSize;
		float ft = ((data->focusPlaneIsPlane)?std::abs(data->focusDistance/output->dir.z):data->focusDistance);
		AtPoint Pfocus = output->dir*ft;

		// Focal Aspect Ratio
		lensV*=data->apertureAspectRatio;
		output->origin.x = lensU;
		output->origin.y = lensV;
		output->origin.z = 0.0;
		output->dir = AiV3Normalize(Pfocus - output->origin);
	}


	///////////////////////////////////
	// Derivatives thanks to Alan King
	///////////////////////////////////
	AtVector d = output->dir*std::abs(((-1.0f/data->tan_myFov)/output->dir.z));

	float d_dot_d = AiV3Dot(d, d);
	float temp = 1.0f / std::sqrt(d_dot_d * d_dot_d * d_dot_d);

	// Already initialized to 0's, only compute the non zero coordinates
	output->dDdx.x = (d_dot_d * dsx - (d.x * dsx) * d.x) * temp;
	output->dDdx.y = (              - (d.x * dsx) * d.y) * temp;
	output->dDdx.z = (              - (d.x * dsx) * d.z) * temp;

	output->dDdy.x = (              - (d.y * dsy) * d.x) * temp;
	output->dDdy.y = (d_dot_d * dsy - (d.y * dsy) * d.y) * temp;
	output->dDdy.z = (              - (d.y * dsy) * d.z) * temp;
}

//node_loader
//{
//	if (i != 0)
//		return FALSE;
//	node->methods     = ObqLensDistortionMethods;
//	node->output_type = AI_TYPE_UNDEFINED;
//	node->name        = "Obq_LensDistortion";
//	node->node_type   = AI_NODE_CAMERA;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}
