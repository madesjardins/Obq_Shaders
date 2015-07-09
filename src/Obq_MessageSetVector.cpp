/*
Obq_MessageSetVector :

	Sets the value in the message passing queue with message as key.

*------------------------------------------------------------------------
Copyright (c) 2015 Marc-Antoine Desjardins, ObliqueFX (marcantoinedesjardins@gmail.com)

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

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqMessageSetVectorMethods);

// enum for parameters
//
enum ObqMessageSetVectorParams { 
	p_passthrough,
	p_key,
	p_value,
	p_setAfter,
	p_restoreValue,
	p_defaultRestoredValue
};

// shader data struct
//
typedef struct 
{
	const char* key;
	bool setAfter;
	bool restoreValue;

}
ShaderData;

node_parameters
{
	AiParameterRGBA("passthrough", 1.0f,1.0f,1.0f,1.0f);
	AiParameterSTR("key", "Ov");
	AiParameterVEC("value", 0.0f, 0.0f, 0.0f);
	AiParameterBOOL("setAfter", false);
	AiParameterBOOL("restoreValue", false);
	AiParameterVEC("defaultRestoredValue",  0.0f, 0.0f, 0.0f);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->key = "Ov";
	data->setAfter = false;
	data->restoreValue = false;
	AiNodeSetLocalData(node,data);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->key = params[p_key].STR;
	data->setAfter = params[p_setAfter].BOOL;
	data->restoreValue = params[p_restoreValue].BOOL;
}

node_finish
{
	// Desallocate shader data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AtVector value = AiShaderEvalParamVec(p_value);

	if(data->setAfter)
	{
		sg->out.RGBA = AiShaderEvalParamRGBA(p_passthrough);
		AiStateSetMsgVec(data->key, value);
	}
	else
	{
		AtVector restoredValue = AI_V3_ZERO;
		if(data->restoreValue)
			if(!AiStateGetMsgVec(data->key, &restoredValue))
				restoredValue = AiShaderEvalParamVec(p_defaultRestoredValue);

		AiStateSetMsgVec(data->key, value);
		sg->out.RGBA = AiShaderEvalParamRGBA(p_passthrough);

		if(data->restoreValue)
			AiStateSetMsgVec(data->key, restoredValue);
	}
}

//node_loader
//{
//   if (i > 0)
//      return false;
//
//   node->methods      = ObqMessageSetVectorMethods;
//   node->output_type  = AI_TYPE_VECTOR;
//   node->name         = "Obq_MessageSetVector";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return true;
//}
