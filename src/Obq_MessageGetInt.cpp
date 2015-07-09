/*
Obq_MessageGetInt :

	Returns the value in the message passing queue with the corresponding key.
	If the key doesn't exist, returns the default value

	Used with the Obq_Toon in gradient mode, this allows the lookup in gradient mode instead of image.
	Just plug this into the input of the gradient mixer.

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
AI_SHADER_NODE_EXPORT_METHODS(ObqMessageGetIntMethods);

// enum for parameters
//
enum ObqMessageGetIntParams { 
	p_key,
	p_default
};

// shader data struct
//
typedef struct 
{
	const char* key;
}
ShaderData;

node_parameters
{
	AiParameterSTR("key", "Oi");
	AiParameterINT("defaultValue", 0);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->key = "Oi";
	AiNodeSetLocalData(node,data);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->key = params[p_key].STR;
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

	int val = 0;
	// Was it previously created or not
	if(AiStateGetMsgInt(data->key,&val))
		sg->out.INT = val;
	else
		sg->out.INT = AiShaderEvalParamInt(p_default);
}

//node_loader
//{
//   if (i > 0)
//      return false;
//
//   node->methods      = ObqMessageGetIntMethods;
//   node->output_type  = AI_TYPE_INT;
//   node->name         = "Obq_MessageGetInt";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return true;
//}
