#include "O_Common.h"

#include "KettleBaker.h"

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
	p_crop_to_region
};

enum ViewMode{CENTER, LEFT, RIGHT, STEREOLR, STEREODU, BAKE, NORMAL_G, NORMAL_S};

enum StereoType{NEUTRAL, PARALLEL, CONVERGED};

enum ZeroParallaxMode{USETARGET,USEDISTANCE};

enum InteraxialMode{INU,RED,GREEN,BLUE};

node_parameters
{
	AiParameterSTR("origin_polymesh", "");
	AiParameterSTR("origin_camera", "");
	AiParameterSTR("target_polymesh", "");
	AiParameterSTR("target_camera", "");

	AiParameterINT("grid_size", 16);

	AiParameterINT("view_mode", RIGHT);					// TODO : Center, Left, Right, Stereo <left-right>, Stereo <down-up>, Bake, Normals_Geometry, Normals_Shading
	AiParameterINT("stereo_type", CONVERGED);			// TODO : Neutral (don't change anything UV-to-UV), Parallel (align both on middle vector), Converged (to a specific distance )
	AiParameterINT("interaxial_mode", BLUE);			// NOT USED BLUE is default
	AiParameterFLT("interaxial_epsilon", 0.001f);		// value used to search
	AiParameterFLT("interaxial_separation", -0.1f);		// in u coordinates typically, if >0 it goes to the left
	AiParameterINT("zero_parallax_mode",0);				// 0 = use target, 1 = Use distance
	AiParameterFLT("zero_parallax_distance", false);	//
	AiParameterINT("total_overscan_pixels",0);			// number of additional pixels needed to annihilate filtering

	AiParameterBOOL("use_render_region", false);
	AiParameterFLT("regionU0", 0.0f);
	AiParameterFLT("regionV0", 0.0f);
	AiParameterFLT("regionU1", 1.0f);
	AiParameterFLT("regionV1", 1.0f);
	AiParameterBOOL("crop_to_region", false);
}



struct kettle_bakeData{ // might needed for other data than the baker node, what calculates the transformations
	CKettleBaker* baker;
	//AtMatrix constraining_camera_matrix;
	//AtMatrix baking_camera_imatrix;

	kettle_bakeData() : baker(0){}

	~kettle_bakeData(){if(baker !=NULL) delete baker;}

	static void* operator new(size_t s)	{return AiMalloc((unsigned long)s);}
	static void operator delete(void* p){AiFree(p);}	
};

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
	kettle_bakeData* kdata;
	AtNode* origin_camera_node;
	AtNode* target_camera_node;
	AtMatrix origin_camera_matrix;
	AtMatrix target_camera_matrix;
	AtMatrix ibaking_camera_matrix;
	bool use_render_region;
	float regionU0;
	float regionV0;
	float regionU1;
	float regionV1;
	bool crop_to_region;
};



inline bool findOriginAndTargetWorld(AtPoint2 screen_uv, ShaderData* data, AtPoint& posO_world, AtPoint& posT_world)
{
	AtVector position;
	AtVector positionT;
	AtVector normal;
	AtVector normalT;

	if(data->kdata[0].baker->findSurfacePoint(screen_uv, position, normal) && data->kdata[1].baker->findSurfacePoint(screen_uv, positionT,normalT))
	{
		AiM4PointByMatrixMult(&posO_world,data->origin_camera_matrix,&position);
		AiM4PointByMatrixMult(&posT_world,data->target_camera_matrix,&positionT);
		return true;
	}
	else
		return false;
}

inline bool findOriginWorld(AtPoint2 screen_uv, ShaderData* data, AtPoint& posO_world)
{
	AtVector position;
	AtVector normal;

	if(data->kdata[0].baker->findSurfacePoint(screen_uv, position,normal))
	{
		AiM4PointByMatrixMult(&posO_world,data->origin_camera_matrix,&position);
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
	data->kdata = new kettle_bakeData[2];

	// Get all nodes
	std::string camNodeName(AiNodeGetName(node));
	std::size_t sitoaIndex = camNodeName.rfind(".SItoA.");

	std::string lastPartName(camNodeName.substr(sitoaIndex));

	AtNode* origin_polymesh_node = AiNodeLookUpByName(std::string(AiNodeGetStr(node, "origin_polymesh")).append(lastPartName).c_str());
	data->origin_camera_node = AiNodeLookUpByName(std::string(AiNodeGetStr(node, "origin_camera")).append(lastPartName).c_str());
	AtNode* target_polymesh_node = AiNodeLookUpByName(std::string(AiNodeGetStr(node, "target_polymesh")).append(lastPartName).c_str());
	data->target_camera_node = AiNodeLookUpByName(std::string(AiNodeGetStr(node, "target_camera")).append(lastPartName).c_str());


	if(origin_polymesh_node == NULL)
	{
		AiMsgError("The origin polymesh was not found! : %s", AiNodeGetStr(node, "origin_polymesh"));
	}
	else if(data->origin_camera_node == NULL)
	{
		AiMsgError("The origin camera was not found! : %s ",AiNodeGetStr(node, "origin_camera"));
	}
	else if(target_polymesh_node == NULL)
	{
		AiMsgError("The target polymesh was not found! : %s ",AiNodeGetStr(node, "target_polymesh"));
	}
	else if(data->target_camera_node == NULL)
	{
		AiMsgError("The target camera was not found! : %s ",AiNodeGetStr(node, "target_camera"));
	}
	else
	{
		// TODO : use arrays for independant animated meshes
		//AtArray* origin_matrix_list = AiNodeGetArray(origin_camera_node, "matrix");
		//AtArray* target_matrix_list = AiNodeGetArray(target_camera_node, "matrix");
		//AtArray* baking_matrix_list = AiNodeGetArray(node, "matrix");

		// select which uv set

		try
		{
			data->kdata[0].baker = new CKettleBaker(origin_polymesh_node, node);
			data->kdata[1].baker = new CKettleBaker(target_polymesh_node, node);
		}
		catch(std::exception ex)
		{
			delete data->kdata[0].baker;
			delete data->kdata[1].baker;
			data->kdata[0].baker = 0;
			data->kdata[1].baker = 0;
			AiMsgError("[KettleBaker] Exception caught at Node Initialize %s", ex.what());
		}
	}

	AiCameraInitialize(node, data);
}

node_update
{
	AiCameraUpdate(node, false);

	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);

	// Get all matrix, if we restrain the cameras together (ie, they are the same, no need to get the array)
	AtMatrix camera_matrix;

	AiNodeGetMatrix(data->origin_camera_node,"matrix",data->origin_camera_matrix);
	AiNodeGetMatrix(data->target_camera_node,"matrix",data->target_camera_matrix);
	AiNodeGetMatrix(node,"matrix",camera_matrix);
	AiM4Invert(camera_matrix,data->ibaking_camera_matrix);

	// Update shaderData variables
	AtNode* options = AiUniverseGetOptions();

	// Overscan
	float w = static_cast<float>(AiNodeGetInt(options,"xres"));
	float h = static_cast<float>(AiNodeGetInt(options,"yres"));

	data->viewMode = AiNodeGetInt(node,"view_mode");
	data->interaxialMode = AiNodeGetInt(node,"interaxial_mode");
	data->interaxialEpsilon = AiNodeGetFlt(node,"interaxial_epsilon");
	data->interaxialSeparation = AiNodeGetFlt(node,"interaxial_separation");
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
	//if(data->stereoType >= CONVERGED)
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

}

node_finish
{
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);
	delete[] data->kdata;
	AiFree(data);
	AiCameraDestroy(node);
}

camera_create_ray 
{
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);
	//AiMsgInfo("Relative Time = %f",input->relative_time);
	output->dDdx = AI_V3_ZERO;
	output->dDdy = AI_V3_ZERO;
	output->dOdx = AI_V3_ZERO;
	output->dOdy = AI_V3_ZERO;
	output->weight = 0.0f;

	if(data->kdata[0].baker == 0 ||  data->kdata[1].baker == 0)
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

	if(!findOriginAndTargetWorld(screen_uv, data, posO_world, posT_world))
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

	if(!findOriginWorld(screen_uv_El, data, posO_world_El))
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

	// pose in camera
	AiM4PointByMatrixMult(&output->origin,data->ibaking_camera_matrix,&posO_world_Blue);


	// calculate direction
	AtVector dir_world_Blue;

	if( data->zeroParallaxMode == USETARGET)
		dir_world_Blue = posT_world - posO_world_Blue;
	else
		dir_world_Blue = (posO_world + data->zeroParallaxDistance*AiV3Normalize(posT_world - posO_world)) - posO_world_Blue;

	AtVector dir_cam;
	AiM4VectorByMatrixMult(&dir_cam,data->ibaking_camera_matrix,&dir_world_Blue);
	AiV3Normalize(output->dir,dir_cam );

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

