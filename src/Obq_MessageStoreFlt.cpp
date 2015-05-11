/*
Obq_MessageStoreFlt :

	Returns the value in the message passing queue with the corresponding key.
	If the key doesn't exist, returns the default value

	Used with the Obq_Toon in gradient mode, this allows the lookup in gradient mode instead of image.
	Just plug this into the input of the gradient mixer.

*------------------------------------------------------------------------
Copyright (c) 2013 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)

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

#include "O_Common.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqMessageStoreFltMethods);

// enum for parameters
//
enum ObqMessageStoreFltParams { 
	p_passthrough,
	p_key,
	p_defaultValue,
	p_AOVName,
	p_storeAfter
};

// shader data struct
//
typedef struct 
{
	const char* key;
	const char* AOVName;
	bool storeAfter;
}
ShaderData;

node_parameters
{
	AiParameterRGBA("passthrough", 1.0f,1.0f,1.0f,1.0f);
	AiParameterSTR("key", "O1");
	AiParameterFLT("defaultValue", 0.0f);
	AiParameterSTR("AOVName", "");
	AiParameterBOOL("storeAfter", true);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->key = "O1";
	data->AOVName = "";
	data->storeAfter = true;
	AiNodeSetLocalData(node,data);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->key = params[p_key].STR;
	data->AOVName = params[p_AOVName].STR;
	data->storeAfter = params[p_storeAfter].BOOL;

	if(data->AOVName && std::strlen(data->AOVName))
		AiAOVRegister(data->AOVName, AI_TYPE_RGBA, AI_AOV_BLEND_OPACITY);
}

node_finish
{
	// Desallocate shader data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	if(sg->Rt & AI_RAY_CAMERA)
	{
		ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

		float val = 0.0f;

		if(data->storeAfter)
			sg->out.RGBA = AiShaderEvalParamRGBA(p_passthrough);

			// Was it previously created or not
			if(AiStateGetMsgFlt(data->key,&val))
				AiAOVSetFlt (sg, data->AOVName, val);
			else
				AiAOVSetFlt (sg, data->AOVName, AiShaderEvalParamFlt(p_defaultValue));

		if(!data->storeAfter)
			sg->out.RGBA = AiShaderEvalParamRGBA(p_passthrough);
	}
	else
		sg->out.RGBA = AiShaderEvalParamRGBA(p_passthrough);
}

//node_loader
//{
//   if (i > 0)
//      return false;
//
//   node->methods      = ObqMessageStoreFltMethods;
//   node->output_type  = AI_TYPE_RGBA;
//   node->name         = "Obq_MessageStoreFlt";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return true;
//}
