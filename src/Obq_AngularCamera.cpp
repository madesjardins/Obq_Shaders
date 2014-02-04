/*
Obq_AngularCamera.cpp :

Obq_AngularCamera is a lens shader that renders angular map.

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

#include "Obq_Common.h"

// Arnold thingy
// 
AI_CAMERA_NODE_EXPORT_METHODS(Obq_AngularCameraMethods);

// Param enum for fast direct access
//
enum Obq_AngularCameraParams { p_angle, p_flip, p_overRender};

const AtVector c_ZAxis = {0.0f,0.0f, 1.0f};
const AtVector c_nZAxis = {0.0f,0.0f,-1.0f};

// Shader Data Structure
//
typedef struct 
{
	float angle;	
	float tanFov;
	bool flip;		
	bool faceFront;	
	bool overRender;
	float lenMax;	// normal over render
}
ShaderData;

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

node_parameters
{
	AiParameterFLT("angle" , 360);			// spread in degrees (360 = whole sphere)
	AiParameterBOOL("flip" , false);		// forward<->backward flip (opposite view)
	AiParameterBOOL("overRender" , false);	// render full image instead of usefull circle
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));

	// Initialize
	data->angle = 360;
	data->flip = false;
	data->overRender = false;
	data->tanFov =std::tan(c_Pi);

	// Set data
	AiCameraInitialize(node, data);
}

node_update
{
	AiCameraUpdate(node, false);
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);

	// get parameters
	data->angle = c_Radians*params[p_angle].FLT/2.0f;
	data->flip = params[p_flip].BOOL;
	data->overRender = params[p_overRender].BOOL;
	data->tanFov = std::tan(data->angle);
	
	// Update shaderData variables
	AtNode* options = AiUniverseGetOptions();
	
	// get Filter width to adjust rendered area
	float filterWidth = 8.0f;
	AtNode* output_filter = AiNodeLookUpByName("sitoa_output_filter");
	if(AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(output_filter),"width"))
		filterWidth = AiNodeGetFlt(output_filter,"width");

	data->lenMax = 1.0f +  filterWidth/(static_cast<float>(AiNodeGetInt(options,"xres"))/2.0f) + AI_EPSILON;

}

node_finish
{
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);
	AiFree(data);
	AiCameraDestroy(node);
}

camera_create_ray 
{

	// AspectRatio
	ShaderData *data = (ShaderData*)AiCameraGetLocalData(node);

	float len2 = input->sx*input->sx + input->sy*input->sy;

	// trivial reject
	if(!data->overRender && len2 > data->lenMax)
	{
		output->weight = 0.0f;	// this will prevent evaluation
		output->dir = c_nZAxis; // forward
	}
	else
	{

		// Scale derivatives
		//float dsx = input->dsx*data->tanFov;
		//float dsy = input->dsy*data->tanFov;

		// Direction
		float len = std::sqrt(len2);
		if(len == 0.0f)
		{
			output->dir = c_ZAxis;
		}
		else
		{
			AtVector k = {-input->sy/len,input->sx/len,0.0f};
			output->dir = RodriguesRotation(c_ZAxis,k,data->angle*len);
		}

#if OBQ_AI_VERSION >= 40100
		output->dir.x = -output->dir.x;
#endif

		// flip forward<->backward looking camera
		if(data->flip)
		{
			output->dir.x = -output->dir.x;
			output->dir.z = -output->dir.z;
		}

		///////////////////////////////////
		// Derivatives thanks to Alan King ( I don't think it's valid for this type of camera)
		///////////////////////////////////
		//if(data->calculateDerivatives)
		//{
		//	AtVector d = output->dir*std::abs(((-1.0f/data->tanFov)/output->dir.z));

		//	float d_dot_d = AiV3Dot(d, d);
		//	float temp = 1.0f / std::sqrtf(d_dot_d * d_dot_d * d_dot_d);

		//	// Already initialized to 0's, only compute the non zero coordinates
		//	output->dDdx.x = (d_dot_d * dsx - (d.x * dsx) * d.x) * temp;
		//	output->dDdx.y = (              - (d.x * dsx) * d.y) * temp;
		//	output->dDdx.z = (              - (d.x * dsx) * d.z) * temp;

		//	output->dDdy.x = (              - (d.y * dsy) * d.x) * temp;
		//	output->dDdy.y = (d_dot_d * dsy - (d.y * dsy) * d.y) * temp;
		//	output->dDdy.z = (              - (d.y * dsy) * d.z) * temp;
		//}
	}
}

//node_loader
//{
//	if (i != 0)
//		return FALSE;
//	node->methods     = Obq_AngularCameraMethods;
//	node->output_type = AI_TYPE_UNDEFINED;
//	node->name        = "Obq_AngularCamera";
//	node->node_type   = AI_NODE_CAMERA;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}
