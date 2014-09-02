/*This is a trimmed version of SItoA's CameraProjection Shader. Instead of outputing color, this one outputs the camera raster space coordinates.*/

#include "Obq_Common.h"

AI_SHADER_NODE_EXPORT_METHODS(ObqCameraProjectionSpaceMethods);

enum ObqCameraProjectionSpaceParams
{
	p_use_render_camera,
	p_projection_camera,
	p_camera_aspect_ratio
};

node_parameters
{
	AiParameterBool( "use_render_camera"      , true );
	AiParameterStr ( "projection_camera"       , ""    );
	AiParameterFlt ( "camera_aspect_ratio"     , 1.0f  );
}

typedef struct
{
	bool useCurrentCamera;
	AtNode*  camera;
	AtMatrix projectionMatrix;

} ShaderData;

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	AiNodeSetLocalData(node, data);
}

node_update
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	AtNode *options = AiUniverseGetOptions();
	float aspect_ratio = AiNodeGetFlt(options, "aspect_ratio");

	AtNode* camera = AiUniverseGetCamera();
	bool defaultCamera = true;
	data->useCurrentCamera = AiNodeGetBool(node,"use_render_camera");

	if(!data->useCurrentCamera)
	{
		const char *cameraName = AiNodeGetStr(node, "projection_camera");
		if(strcmp(cameraName, "") != 0) // Use a custom camera for the projection
		{

			AtNode* temp_camera = AiNodeLookUpByName(cameraName); // We can have a string in the parameter but the camera may have been deleted
			if (temp_camera)
			{
				camera = temp_camera;
				aspect_ratio = 1.0f / AiNodeGetFlt(node, "camera_aspect_ratio");
				defaultCamera = false;
			}
			else
			{
				AiMsgError("[sitoa] Camera \"%s\" used in \"%s\" doesn't exist anymore", cameraName, AiNodeGetName(node));
			}

		}

		data->camera = camera;

		float fov = AiNodeGetFlt(camera, "fov");
		AtPoint2 screen_window_min = AiNodeGetPnt2(camera, "screen_window_min");
		AtPoint2 screen_window_max = AiNodeGetPnt2(camera, "screen_window_max");

		float near_clip = AiNodeGetFlt(camera, "near_clip");
		float far_clip = AiNodeGetFlt(camera, "far_clip"); 

		AiM4Identity(data->projectionMatrix);

		if (defaultCamera)
		{
			// Since we export aspect_ratio in SItoA as ((windowxres / windowyres) * ImagePixelAspectRatio) / Camera Aspect Ratio
			// we need to reverse the operation to get the real aspect ratio, but only if the camera is the default one
			unsigned int windowxres = (unsigned int)AiNodeGetInt(options, "xres");
			unsigned int windowyres = (unsigned int)AiNodeGetInt(options, "yres");
			float window_ratio = 1.0f / (windowxres / (float)windowyres);
			aspect_ratio *= window_ratio;
		}


		float x_win_center   = 0.5f * (screen_window_min.x + screen_window_max.x);
		float y_win_center   = 0.5f * (screen_window_min.y + screen_window_max.y) * aspect_ratio;
		float inv_x_win_size = 2.0f / (screen_window_max.x - screen_window_min.x);
		float inv_y_win_size = 2.0f /((screen_window_max.y - screen_window_min.y) * aspect_ratio);

		if (AiNodeIs(camera, "persp_camera"))
		{
			float cf = 1.0f / tanf((float)AI_DTOR * fov * 0.5f);

			data->projectionMatrix[0][0] = cf * inv_x_win_size;
			data->projectionMatrix[1][0] = 0;
			data->projectionMatrix[2][0] = x_win_center * inv_x_win_size;
			data->projectionMatrix[3][0] = 0;
			data->projectionMatrix[0][1] = 0;
			data->projectionMatrix[1][1] = cf * inv_y_win_size;
			data->projectionMatrix[2][1] = y_win_center * inv_y_win_size;
			data->projectionMatrix[3][1] = 0;
			data->projectionMatrix[0][2] = 0;
			data->projectionMatrix[1][2] = 0;
			data->projectionMatrix[2][2] = - far_clip / (far_clip - near_clip);
			data->projectionMatrix[3][2] =   near_clip * data->projectionMatrix[2][2];
			data->projectionMatrix[0][3] = 0;
			data->projectionMatrix[1][3] = 0;
			data->projectionMatrix[2][3] = -1;
			data->projectionMatrix[3][3] = 0;
		}
		else if(AiNodeIs(camera, "ortho_camera"))
		{
			data->projectionMatrix[0][0] = inv_x_win_size;
			data->projectionMatrix[1][0] = 0;
			data->projectionMatrix[2][0] = 0;
			data->projectionMatrix[3][0] = x_win_center * inv_x_win_size;
			data->projectionMatrix[0][1] = 0;
			data->projectionMatrix[1][1] = inv_y_win_size;
			data->projectionMatrix[2][1] = 0;
			data->projectionMatrix[3][1] = y_win_center * inv_y_win_size;
			data->projectionMatrix[0][2] = 0;
			data->projectionMatrix[1][2] = 0;
			data->projectionMatrix[2][2] = -1 / (far_clip - near_clip);
			data->projectionMatrix[3][2] = -near_clip / (far_clip - near_clip);
			data->projectionMatrix[0][3] = 0;
			data->projectionMatrix[1][3] = 0;
			data->projectionMatrix[2][3] = 0;
			data->projectionMatrix[3][3] = 1;
		}
	}
}

node_finish
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	if(data->useCurrentCamera)
	{
		sg->out.VEC.x = myMAP01F(sg->sx);
		sg->out.VEC.y = myMAP01F(sg->sy);

		sg->out.VEC.z = 0.0f;
	}
	else
	{
		AtMatrix viewMatrix;
		AtPoint camP;
		AtHPoint camProjected;

		AiWorldToCameraMatrix(data->camera, sg->time, viewMatrix);   

		// Transform shading point from world coordinates into camera space and project it

		AiM4PointByMatrixMult(&camP, viewMatrix, &sg->P);

		camProjected.x = camP.x;
		camProjected.y = camP.y;
		camProjected.z = camP.z;
		camProjected.w = 1.0;
		AiM4HPointByMatrixMult(&camProjected, data->projectionMatrix, &camProjected);

		camProjected.x = camProjected.x / camProjected.w;
		camProjected.y = camProjected.y / camProjected.w;

		// Remap camProjected to the [-1, 1] range in x and y
		float u = myMAP01F(camProjected.x);
		float v = myMAP01F(camProjected.y);
		sg->out.VEC.x = u;
		sg->out.VEC.y = v;
		sg->out.VEC.z = 0.0f;
	} 
}

//node_loader
//{
//	if (i > 0)
//		return false;
//
//	node->methods      = ObqCameraProjectionSpaceMethods;
//	node->output_type  = AI_TYPE_VECTOR;
//	node->name         = "Obq_CameraProjectionSpace";
//	node->node_type    = AI_NODE_SHADER;
//
//	strcpy_s(node->version, AI_VERSION);
//
//	return true;
//}