/*
Obq_Root2Tip :

	LERP between root and tip

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

#include "O_Common.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqRoot2TipMethods);

// enum for parameters
//
enum ObqRoot2TipParams { p_root, p_tip, p_mode};

// shader data struct
//
typedef struct 
{
	int mode;
}
ShaderData;

enum ObqInterpolationMode{OBQ_LIN, OBQ_HERM};
static const char* ObqInterpolationModeNames[] = 
{
	"Linear",
    "Hermite",
    NULL
};

node_parameters
{
	AiParameterRGB("root",0.0f,0.0f,0.0f);
	AiParameterRGB("tip",1.0f,1.0f,1.0f);
    AiParameterENUM("mode",OBQ_LIN,ObqInterpolationModeNames);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->mode = OBQ_LIN;
	AiNodeSetLocalData(node,data);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->mode = AiNodeGetInt(node,"mode");
}

node_finish
{
	// Desallocate shader data
	AiFree((ShaderData*)AiNodeGetLocalData(node));
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	switch(data->mode)
	{
	case OBQ_HERM:
			sg->out.RGB = AiColorHerp(sg->bv, AiShaderEvalParamRGB(p_root), AiShaderEvalParamRGB(p_tip));
			break;
	default:
		sg->out.RGB = AiColorLerp(sg->bv, AiShaderEvalParamRGB(p_root), AiShaderEvalParamRGB(p_tip));
		break;
	}
	sg->out_opacity = AI_RGB_WHITE;
}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqRoot2TipMethods;
//   node->output_type  = AI_TYPE_RGBA;
//   node->name         = "Obq_Root2Tip";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
