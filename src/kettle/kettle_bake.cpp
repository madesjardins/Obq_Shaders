#include <ai.h>

#include "KettleBaker.h"

AI_CAMERA_NODE_EXPORT_METHODS(kettle_bakeMethods)

enum kettle_bakeParams 
{
	p_input_object,
	p_normal_offset,
	p_hide_destination_object
};

const char* acc_types[] = {"Null", "Grid", "Adaptive", 0};

node_parameters
{
	AiParameterSTR("input_object", "");
	AiParameterFLT("normal_offset", 0.1f);
	AiParameterBOOL("hide_destination_object", 0.1f);

	AiParameterENUM("acc_type", 1, acc_types);
	AiParameterINT("grid_size", 16);
}

struct kettle_bakeData{ // might needed for other data than the baker node, what calculates the transformations
	CKettleBaker* baker;
	float normal_offset;
	bool hide_destination_object;

	kettle_bakeData() : baker(0), normal_offset(0.001f), hide_destination_object(false){}

	~kettle_bakeData(){delete baker;} // AFAIK delete checks for null pointers

	static void* operator new(size_t s)	{return AiMalloc((unsigned long)s);}
	static void operator delete(void* p){AiFree(p);}	
};

node_initialize
{
	kettle_bakeData* data = new kettle_bakeData();	

	const char* input_object = AiNodeGetStr(node, "input_object");
	AtNode* input_node = AiNodeLookUpByName(input_object);

	if(!AiNodeGetBool(AiUniverseGetOptions(), "preserve_scene_data"))
	{
		data->baker = 0;
		AiMsgError("Preserve Scene Data is not set!");
	}
	else
	{
		if(input_node == 0)
		{
			AiMsgError("The input object is not found!");
			AiMsgError(input_object);
		}
		else
		{
			try
			{
				data->baker = new CKettleBaker(input_node, node);
				data->normal_offset = AiNodeGetFlt(node, "normal_offset");
				data->hide_destination_object = AiNodeGetBool(node, "hide_destination_object");

				if (data->hide_destination_object){
					AiNodeSetInt(node, "visibility", 0);
				}
			}
			catch(std::exception ex)
			{
				delete data->baker;
				data->baker = 0;
				AiMsgError("[KettleBaker] Exception caught at Node Initialize %s", ex.what());
			}
		}
	}

	AiCameraInitialize(node, data);
}

node_update
{
   //AiCameraUpdate(node, false);
}

node_finish
{
	kettle_bakeData *data = (kettle_bakeData*)AiCameraGetLocalData(node);  
	delete data;
	AiCameraDestroy(node);
}

camera_create_ray 
{
	kettle_bakeData *data = (kettle_bakeData*)AiCameraGetLocalData(node); 

	output->weight = 0.0f;

	if(data->baker == 0)
		return;

	const AtPoint2 screen_uv = {(input->sx + 1.0f) * 0.5f, (input->sy + 1.0f) * 0.5f};

	AtVector position;
	AtVector normal;

	if(data->baker->findSurfacePoint(screen_uv, position, normal))
	{
		output->dir = -normal;
		output->origin = position + data->normal_offset * normal;
		output->dDdx = AI_V3_ZERO;
		output->dDdy = AI_V3_ZERO;
		output->dOdx = AI_V3_ZERO;
		output->dOdy = AI_V3_ZERO;
		output->weight = 1.0f;
	}
}
