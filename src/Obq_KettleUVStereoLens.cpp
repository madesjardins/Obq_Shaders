/*
Obq_KettleUVStereoLens :

A mesh to mesh or UV bake lens shader

*------------------------------------------------------------------------
Copyright (c) 2012-2015 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)

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
// KettleBaker
// Copyright (c) 2013, Kettle Studio Ltd. All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials 
// provided with the distribution.
// Neither the name of the authors nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written 
// permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS 
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "O_Common.h"

#ifdef _WIN32
#include "kettle\KettleBaker.h"
#else
#include "kettle/KettleBaker.h"
#endif

AI_CAMERA_NODE_EXPORT_METHODS(ObqKettleUVStereoLensMethods)

	enum kettle_bakeParams 
{
	p_origin_polymesh,
	p_origin_camera,
	p_target_polymesh,
	p_target_camera,
	p_grid_size,
	p_view_mode,
	p_stereo_type,
	p_interaxial_mode,
	p_interaxial_epsilon,
	p_interaxial_separation,
	p_zero_parallax_mode,
	p_zero_parallax_distance,
	p_total_overscan_pixels,
	p_use_render_region,
	p_regionU0,
	p_regionV0,
	p_regionU1,
	p_regionV1,
	p_crop_to_region,
	p_uv_set_origin,
	p_uv_set_target,
	p_use_dof,
	p_aperture_size_,
	p_aperture_aspect_ratio_,
	p_use_polygonal_aperture,
	p_aperture_blades_,
	p_aperture_blade_curvature_,
	p_aperture_rotation_,
	p_bokeh_invert,
	p_bokeh_bias,
	p_bokeh_gain
};

enum ViewMode{CENTER, LEFT, RIGHT, STEREOLR, STEREODU, BAKE, NORMAL};

enum StereoType{PARALLEL, CONVERGED};

enum ZeroParallaxMode{USETARGET,USEDISTANCE};

enum InteraxialMode{INU,RED,GREEN,BLUE};

node_parameters
{
	AiParameterSTR("origin_polymesh", "");
	AiParameterSTR("origin_camera", "");
	AiParameterSTR("target_polymesh", "");
	AiParameterSTR("target_camera", "");

	AiParameterINT("grid_size", 16);

	AiParameterINT("view_mode", CENTER);				// Center, Left, Right, Stereo <left-right>, Stereo <down-up>, Bake, Normals
	AiParameterINT("stereo_type", CONVERGED);			// Parallel (align both on middle vector), Converged (to a specific distance )
	AiParameterINT("interaxial_mode", BLUE);			// NOT USED BLUE is default
	AiParameterFLT("interaxial_epsilon", 0.001f);		// value used to search
	AiParameterFLT("interaxial_separation", -0.6f);		// in u coordinates typically, if >0 it goes to the left
	AiParameterINT("zero_parallax_mode",USETARGET);		// 0 = use target, 1 = Use distance
	AiParameterFLT("zero_parallax_distance", 100.0);	//
	AiParameterINT("total_overscan_pixels",0);			// number of additional pixels needed to annihilate filtering

	AiParameterBOOL("use_render_region", false);
	AiParameterFLT("regionU0", 0.0f);
	AiParameterFLT("regionV0", 0.0f);
	AiParameterFLT("regionU1", 1.0f);
	AiParameterFLT("regionV1", 1.0f);
	AiParameterBOOL("crop_to_region", false);

	AiParameterSTR("uv_set_origin", "DefaultUV");		// uvspace
	AiParameterSTR("uv_set_target", "DefaultUV");		// uvspace

	AiParameterBOOL("use_dof",false);					// available in non bake/normal mode
	//AiParameterFLT("focus_distance",100.0f);			//is zero_parallax_distance
	AiParameterFLT("aperture_size_",0.1f);
	AiParameterFLT("aperture_aspect_ratio_",1.0f);
	AiParameterBOOL("use_polygonal_aperture",true);
	AiParameterINT("aperture_blades_",5);
	AiParameterFLT("aperture_blade_curvature_",0.0f);
	AiParameterFLT("aperture_rotation_",0.0f);
	AiParameterBOOL("bokeh_invert",false);
	AiParameterFLT("bokeh_bias",0.5f);
	AiParameterFLT("bokeh_gain",0.5f);
}

struct ShaderData{
	float aspect;
	float width;
	float height;
	float pixelRatio;
	int zeroParallaxMode;
	float zeroParallaxDistance;
	int interaxialMode;
	float interaxialEpsilon;
	float interaxialSeparation;
	int viewMode;
	int stereoType;
	float overscanRatio;
	CKettleBaker* bakerOrigin;
	CKettleBaker* bakerTarget;
	AtNode* origin_camera_node;
	AtNode* target_camera_node;
	AtMatrix origin_camera_matrix;
	AtMatrix target_camera_matrix;
	AtMatrix ibaking_camera_matrix;
	AtArray* a_origin_camera_matrix;
	AtArray* a_target_camera_matrix;
	AtArray* a_baking_camera_matrix;
	bool use_render_region;
	float regionU0;
	float regionV0;
	float regionU1;
	float regionV1;
	bool crop_to_region;

	bool useDof;
	float apertureSize;
	float apertureAspectRatio;
	int apertureBlades;
	float apertureRotation;
	float apertureBladeCurvature;
	bool bokehInvert;
	float bokehBias;
	float bokehGain;
	bool usePolygonalAperture;
};



inline bool findOriginAndTargetWorld(AtPoint2 screen_uv, ShaderData* data, AtPoint& posO_world, AtPoint& posT_world, float time)
{
	AtVector position;
	AtVector positionT;
	AtVector normal;
	AtVector normalT;

	if(data->bakerOrigin->findSurfacePoint(screen_uv, position, normal) && data->bakerTarget->findSurfacePoint(screen_uv, positionT,normalT))
	{
		AtMatrix origin_camera_matrix, target_camera_matrix;
		AiArrayInterpolateMtx(data->a_origin_camera_matrix,time,0,origin_camera_matrix);
		AiArrayInterpolateMtx(data->a_target_camera_matrix,time,0,target_camera_matrix);

		AiM4PointByMatrixMult(&posO_world,origin_camera_matrix,&position);
		AiM4PointByMatrixMult(&posT_world,target_camera_matrix,&positionT);
		
		return true;
	}
	else
		return false;
}

inline bool findOriginWorld(AtPoint2 screen_uv, ShaderData* data, AtPoint& posO_world, float time)
{
	AtVector position;
	AtVector normal;

	if(data->bakerOrigin->findSurfacePoint(screen_uv, position,normal))
	{
		AtMatrix origin_camera_matrix;
		AiArrayInterpolateMtx(data->a_origin_camera_matrix,time,0,origin_camera_matrix);

		AiM4PointByMatrixMult(&posO_world,origin_camera_matrix,&position);
		return true;
	}
	else
		return false;
}

// bake does not need motion
inline bool findOriginWorld(AtPoint2 screen_uv, ShaderData* data, AtPoint& posO_world, AtVector& normalO_world, float time)
{
	AtVector position;
	AtVector normal;

	if(data->bakerOrigin->findSurfacePoint(screen_uv, position, normal))
	{
		AiM4PointByMatrixMult(&posO_world,data->origin_camera_matrix,&position);
		AiM4VectorByMatrixMult(&normalO_world,data->origin_camera_matrix,&normal);
		return true;
	}
	else
		return false;
}

inline float wrapAround(float v)
{
	if(v < 0.0f)
		return 1.0f+v;
	else if(v >= 1.0f)
		return v-1.0f;
	else
		return v;

}

// v [0,1] -> interp lo->hi
inline float remapRegion(float v, float lo, float hi)
{
	return v*hi+(1.0f-v)*lo;
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->bakerOrigin = NULL;
	data->bakerTarget = NULL;

	// Get all nodes
	std::string camNodeName(AiNodeGetName(node));
	std::size_t sitoaIndex = camNodeName.rfind(".SItoA.");

	std::string lastPartName(camNodeName.substr(sitoaIndex));

	data->viewMode = AiNodeGetInt(node,"view_mode");

	AtNode* origin_polymesh_node = AiNodeLookUpByName(std::string(AiNodeGetStr(node, "origin_polymesh")).append(lastPartName).c_str());
	data->origin_camera_node = AiNodeLookUpByName(std::string(AiNodeGetStr(node, "origin_camera")).append(lastPartName).c_str());
	AtNode* target_polymesh_node = (data->viewMode < BAKE?AiNodeLookUpByName(std::string(AiNodeGetStr(node, "target_polymesh")).append(lastPartName).c_str()):NULL);
	data->target_camera_node = (data->viewMode < BAKE?AiNodeLookUpByName(std::string(AiNodeGetStr(node, "target_camera")).append(lastPartName).c_str()):NULL);
	
	if(origin_polymesh_node == NULL)
	{
		AiMsgError("[Obq_KettleUVStereoLens] The origin polymesh was not found! : \"%s\"", AiNodeGetStr(node, "origin_polymesh"));
		AiFree(data);
		return;
	}
	else if(data->origin_camera_node == NULL)
	{
		AiMsgError("[Obq_KettleUVStereoLens] The origin camera was not found! :\"%s\"",AiNodeGetStr(node, "origin_camera"));
		AiFree(data);
		return;
	}
	else if(data->viewMode < BAKE && target_polymesh_node == NULL)
	{
		AiMsgError("[Obq_KettleUVStereoLens] The target polymesh was not found! :\"%s\"",AiNodeGetStr(node, "target_polymesh"));
		AiFree(data);
		return;
	}
	else if(data->viewMode < BAKE && data->target_camera_node == NULL)
	{
		AiMsgError("[Obq_KettleUVStereoLens] The target camera was not found! : \"%s\"",AiNodeGetStr(node, "target_camera"));
		AiFree(data);
		return;
	}
	else
	{

			data->bakerOrigin = new CKettleBaker(origin_polymesh_node, node, AiNodeGetStr(node,"uv_set_origin"));
			if(data->bakerOrigin->isGood())
			{
				if(data->viewMode < BAKE)
				{
					data->bakerTarget = new CKettleBaker(target_polymesh_node, node, AiNodeGetStr(node,"uv_set_target"));
					if(data->bakerTarget->isGood()!=1)
					{
						delete data->bakerTarget;
						delete data->bakerOrigin;
						data->bakerOrigin = NULL;
						data->bakerTarget = NULL;
						AiFree(data);
						AiMsgError("[Obq_KettleUVStereoLens] There was an problem while creating the target data.");
						return;
					}
				}
			}
			else
			{
				delete data->bakerOrigin;
				data->bakerOrigin = NULL;
				AiFree(data);
				AiMsgError("[Obq_KettleUVStereoLens] There was an problem while creating the origin data.");
				return;
			}
	

	// Get all matrix, if we restrain the cameras together (ie, they are the same, no need to get the array)
	AtMatrix camera_matrix;

	AiNodeGetMatrix(data->origin_camera_node,"matrix",data->origin_camera_matrix);
	data->a_origin_camera_matrix = AiNodeGetArray(data->origin_camera_node, "matrix");
	if(data->viewMode < BAKE)
	{
		AiNodeGetMatrix(data->target_camera_node,"matrix",data->target_camera_matrix);
		data->a_target_camera_matrix = AiNodeGetArray(data->target_camera_node, "matrix");
	}
	AiNodeGetMatrix(node,"matrix",camera_matrix);
	AiM4Invert(camera_matrix,data->ibaking_camera_matrix);
	data->a_baking_camera_matrix = AiNodeGetArray(node, "matrix"); // will need to be inverted on the fly


	// Update shaderData variables
	AtNode* options = AiUniverseGetOptions();

	// Overscan
	float w = static_cast<float>(AiNodeGetInt(options,"xres"));
	float h = static_cast<float>(AiNodeGetInt(options,"yres"));

	
	data->interaxialMode = AiNodeGetInt(node,"interaxial_mode");
	data->interaxialEpsilon = AiNodeGetFlt(node,"interaxial_epsilon");
	
	data->interaxialSeparation = ( (data->viewMode != CENTER &&  data->viewMode < BAKE) ? AiNodeGetFlt(node,"interaxial_separation"):0.0f);

	switch(data->viewMode)
	{
	case STEREOLR:
		data->width  = w - static_cast<float>(AiNodeGetInt(node,"total_overscan_pixels"));
		data->height  = h;
		data->overscanRatio = w/data->width;
		break;
	case STEREODU:
		data->width  = w;
		data->height  = h - static_cast<float>(AiNodeGetInt(node,"total_overscan_pixels"));
		data->overscanRatio = h/data->height;
		break;
	default:
		data->width  = w;
		data->height  = h;
		data->overscanRatio = 1.0f;
		break;
	}

	// Aspect
	data->pixelRatio = 1.0f/AiNodeGetFlt(options,"aspect_ratio");
	data->aspect = data->width/(data->height/data->pixelRatio);

	data->stereoType = AiNodeGetInt(node,"stereo_type");
	data->zeroParallaxMode = AiNodeGetInt(node,"zero_parallax_mode");
	//if(data->stereoType >= CONVERGED)	// keep it for dof even if parallel
	data->zeroParallaxDistance = AiNodeGetFlt(node,"zero_parallax_distance");

	data->use_render_region = AiNodeGetBool(node,"use_render_region");

	float regionU0 = AiNodeGetFlt(node,"regionU0");
	float regionV0 = AiNodeGetFlt(node,"regionV0");
	float regionU1 = AiNodeGetFlt(node,"regionU1");
	float regionV1 = AiNodeGetFlt(node,"regionV1");

	data->regionU0 = MIN(regionU0,regionU1);
	data->regionV0 = MIN(regionV0,regionV1);
	data->regionU1 = MAX(regionU0,regionU1);
	data->regionV1 = MAX(regionV0,regionV1);

	data->crop_to_region = AiNodeGetBool(node,"crop_to_region");

	// DOF
	data->useDof = AiNodeGetBool(node,"use_dof");
	data->apertureSize = AiNodeGetFlt(node,"aperture_size_");
	if(data->useDof)
	{
		// DOF related values
		
		data->apertureAspectRatio = AiNodeGetFlt(node,"aperture_aspect_ratio_");
		data->usePolygonalAperture = AiNodeGetBool(node,"use_polygonal_aperture");
		data->apertureBlades = AiNodeGetInt(node,"aperture_blades_");
		data->apertureBladeCurvature = AiNodeGetFlt(node,"aperture_blade_curvature_");
		data->apertureRotation = AiNodeGetFlt(node,"aperture_rotation_");
		data->bokehInvert = AiNodeGetBool(node,"bokeh_invert");
		data->bokehBias = AiNodeGetFlt(node,"bokeh_bias");
		data->bokehGain = 1.0f-AiNodeGetFlt(node,"bokeh_gain");

		if(data->apertureAspectRatio<=0.0f)
			data->apertureAspectRatio = 0.001f;
	}

	AiCameraInitialize(node, data);
	}
}

node_update
{
	AiCameraUpdate(node, false);
}

node_finish
{
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);
	if(data != NULL)
	{
		if(data->bakerOrigin != NULL)
			delete data->bakerOrigin;
		if(data->bakerTarget != NULL)
			delete data->bakerTarget;
		AiFree(data);
	}
	AiCameraDestroy(node);
}

camera_create_ray 
{
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);
	
	output->dDdx = AI_V3_ZERO;
	output->dDdy = AI_V3_ZERO;
	output->dOdx = AI_V3_ZERO;
	output->dOdy = AI_V3_ZERO;
	output->weight = 0.0f;
	
	if(data->bakerOrigin == NULL ||  (data->bakerTarget == NULL && data->viewMode < BAKE))
		return;

	// calculate the uv

	float offset = 0.0f;
	float sx = input->sx;
	float sy = input->sy;

	bool isRight = true;

	//------------------
	// CONVERGED ON TARGET + BLUE

	if(data->viewMode == LEFT)
	{
		isRight = false;
	}
	else if(data->viewMode == STEREOLR)
	{
		if(input->sx < 0) // left
		{
			sx = (input->sx+0.5f)*2.0f*data->overscanRatio;
			isRight = false;
		}
		else // right
		{
			sx = (input->sx-0.5f)*2.0f*data->overscanRatio;
		}
	}
	else if(data->viewMode == STEREODU)
	{
		if(input->sy < 0) // left
		{
			sy = (input->sy+0.5f/(data->aspect/data->overscanRatio))*2.0f*data->overscanRatio;
			isRight = false;
		}
		else
		{
			sy = (input->sy-0.5f/(data->aspect/data->overscanRatio))*2.0f*data->overscanRatio;
		}
	}

	
	float leftSign = (data->interaxialSeparation>0?1.0f:-1.0f);

	//calculate no offset
	AtPoint2 screen_uv; 
	if(data->use_render_region)
	{
		// Crop
		if(data->crop_to_region)
		{
			screen_uv.x = wrapAround(remapRegion((sx + 1.0f) * 0.5f,data->regionU0,data->regionU1));
			screen_uv.y = wrapAround(remapRegion((sy + 1.0f) * 0.5f,data->regionV0,data->regionV1));
		}
		else
		{
			screen_uv.x = wrapAround((sx + 1.0f) * 0.5f);
			screen_uv.y = wrapAround((sy + 1.0f) * 0.5f);

			// if outside of region, do not trace ray
			if(screen_uv.x < data->regionU0 || screen_uv.x > data->regionU1 || screen_uv.y < data->regionV0 || screen_uv.y > data->regionV1)
			{
				output->weight = 0.0f;
				return;
			}
		}
	}
	else
	{
		screen_uv.x = wrapAround((sx + 1.0f) * 0.5f);
		screen_uv.y = wrapAround((sy + 1.0f) * 0.5f);
	}

	AtPoint posO_world, posT_world;

	//-----------------------
	// NON BAKE MODE
	if(data->viewMode < BAKE)
	{
		if(!findOriginAndTargetWorld(screen_uv, data, posO_world, posT_world, input->relative_time))
			return;

		// calculate direction to target from center
		AtVector dirN_world = AiV3Normalize(posT_world - posO_world);

		// calculate epsilon left 3d space U vector
		AtPoint2 screen_uv_El;

		if(data->use_render_region && data->crop_to_region)
		{ 
			screen_uv_El.x = wrapAround(remapRegion((sx + 1.0f) * 0.5f,data->regionU0,data->regionU1) + leftSign*data->interaxialEpsilon);
			screen_uv_El.y = wrapAround(remapRegion((sy + 1.0f) * 0.5f,data->regionV0,data->regionV1));
		}
		else
		{ 
			screen_uv_El.x = wrapAround((sx + 1.0f) * 0.5f + leftSign*data->interaxialEpsilon);
			screen_uv_El.y = wrapAround((sy + 1.0f) * 0.5f);
		}
		AtPoint posO_world_El;
		
		if(!findOriginWorld(screen_uv_El, data, posO_world_El, input->relative_time))
			return;

		AtVector center2ElDirN_world = AiV3Normalize(posO_world_El-posO_world);

		// calculate upV
		AtVector upV, upVn;
		AiV3Cross (upV, dirN_world, center2ElDirN_world);
		AiV3Normalize(upVn,upV);

		// calculate Left vector
		AtVector leftV, leftVn;
		AiV3Cross (leftV, upVn, dirN_world);
		AiV3Normalize(leftVn,leftV);

		// calculate position world
		float separationSign = (isRight?-1.0f:1.0f);
		AtVector posO_world_Blue = posO_world + separationSign*leftVn*data->interaxialSeparation/2.0f;
		AtVector posO_world_Blue_nodof = posO_world_Blue;

		if(data->useDof && data->apertureSize >0.0f)
		{
			float lensU;
			float lensV;
			ConcentricSampleDisk(input->lensx, input->lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation,&lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
			lensU*=data->apertureSize;
			lensV*=data->apertureSize*data->apertureAspectRatio;
			posO_world_Blue+=lensU*leftVn;
			posO_world_Blue+=lensV*upVn;
		}

		// calculate direction
		AtVector dir_world_Blue;

		if(data->stereoType == CONVERGED)
		{

			if( data->zeroParallaxMode == USETARGET)
			{
				dir_world_Blue = posT_world - posO_world_Blue;
			}
			else
				dir_world_Blue = (posO_world + data->zeroParallaxDistance*AiV3Normalize(posT_world - posO_world)) - posO_world_Blue;
		}
		else
		{
			//dir_world_Blue = dirN_world;
			AiV3Normalize(dir_world_Blue,(posO_world_Blue_nodof + data->zeroParallaxDistance*AiV3Normalize(posT_world - posO_world)) - posO_world_Blue);
		}

		// pose in camera
		//-- motion --
		AtMatrix baking_camera_matrix, ibaking_camera_matrix;
		AiArrayInterpolateMtx(data->a_baking_camera_matrix,input->relative_time,0,baking_camera_matrix);
		AiM4Invert(baking_camera_matrix, ibaking_camera_matrix);
		//-- motion --

		AiM4PointByMatrixMult(&output->origin,/*data->*/ibaking_camera_matrix,&posO_world_Blue);

		AtVector dir_cam;
		AiM4VectorByMatrixMult(&dir_cam,/*data->*/ibaking_camera_matrix,&dir_world_Blue);
		AiV3Normalize(output->dir,dir_cam );

	}
	else
	{
		//-----------------------
		// BAKE MODE
		AtVector normO_world;

		if(!findOriginWorld(screen_uv, data, posO_world, normO_world, input->relative_time))
			return;


		// calculate direction to target from center
		AtVector dirN_world,dir_cam;
		if(data->viewMode == BAKE)
		{
			dirN_world = -AiV3Normalize(normO_world);
			posO_world -= dirN_world*data->interaxialEpsilon;
		}
		else // NORMAL
		{
			if(data->useDof && data->apertureSize >0.0f)
			{
				// calculate epsilon left 3d space U vector
				AtPoint2 screen_uv_El;

				if(data->use_render_region && data->crop_to_region)
				{ 
					screen_uv_El.x = wrapAround(remapRegion((sx + 1.0f) * 0.5f,data->regionU0,data->regionU1) + leftSign*data->interaxialEpsilon);
					screen_uv_El.y = wrapAround(remapRegion((sy + 1.0f) * 0.5f,data->regionV0,data->regionV1));
				}
				else
				{ 
					screen_uv_El.x = wrapAround((sx + 1.0f) * 0.5f + leftSign*data->interaxialEpsilon);
					screen_uv_El.y = wrapAround((sy + 1.0f) * 0.5f);
				}
				AtPoint posO_world_El;
				AtVector norm0_world_El;
				if(!findOriginWorld(screen_uv_El, data, posO_world_El, norm0_world_El, input->relative_time))
					return;

				AtVector center2ElDirN_world = AiV3Normalize(posO_world_El-posO_world);

				// calculate upV
				AtVector upV, upVn;
				AiV3Cross (upV, dirN_world, center2ElDirN_world);
				AiV3Normalize(upVn,upV);

				// calculate Left vector
				AtVector leftV, leftVn;
				AiV3Cross (leftV, upVn, dirN_world);
				AiV3Normalize(leftVn,leftV);

				AtVector posO_world_nodof = posO_world;

				float lensU;
				float lensV;
				ConcentricSampleDisk(input->lensx, input->lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation,&lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
				lensU*=data->apertureSize;
				lensV*=data->apertureSize*data->apertureAspectRatio;
				posO_world+=lensU*leftVn;
				posO_world+=lensV*upVn;

				AiV3Normalize(dirN_world,(posO_world_nodof + data->zeroParallaxDistance*AiV3Normalize(normO_world)) - posO_world);
			}
			else
			{
				dirN_world = AiV3Normalize(normO_world);
			}
		}

		// pose in camera
		//-- no motion when baking --
		//AtMatrix baking_camera_matrix, ibaking_camera_matrix;
		//AiArrayInterpolateMtx(data->a_baking_camera_matrix,input->relative_time,0,baking_camera_matrix);
		//AiM4Invert(baking_camera_matrix, ibaking_camera_matrix);
		//-- no motion when baking --

		AiM4PointByMatrixMult(&output->origin,data->ibaking_camera_matrix,&posO_world);
		AiM4VectorByMatrixMult(&dir_cam,data->ibaking_camera_matrix,&dirN_world);
		AiV3Normalize(output->dir,dir_cam );
	}

	output->weight = 1.0f;
}
//node_loader
//{
//	if (i > 0)
//      return false;
//
//#ifdef UNIX
//	sprintf(node->version, AI_VERSION);
//#else
//	sprintf_s(node->version, AI_VERSION);
//#endif
//	node->methods      = (AtNodeMethods*) ObqKettleUVStereoLensMethods;
//	node->output_type  = AI_TYPE_UNDEFINED;
//	node->name         = "Obq_KettleUVStereoLens";
//	node->node_type    = AI_NODE_CAMERA;
//
//	return true;
//}

