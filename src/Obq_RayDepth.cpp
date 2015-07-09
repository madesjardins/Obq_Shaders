/*
Obq_RayDepth :

Outputs the ray depth of the particular ray type

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
AI_SHADER_NODE_EXPORT_METHODS(ObqRayDepthMethods);

enum OBQTYPE {ALL, REFL, REFR, DIFF, GLOSS};

typedef struct 
{
	int type;
}
ShaderData;

// enum for parameters
//
enum ObqRayDepthParams
{
	p_type
};

node_parameters
{
	AiParameterINT("type",0);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->type = ALL;
	AiNodeSetLocalData(node,data);
}

node_update
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->type = params[p_type].INT;

}

node_finish
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	switch(data->type)
	{
	case ALL:
		sg->out.INT = sg->Rr;
		break;
	case REFL:
		sg->out.INT = sg->Rr_refl;
		break;
	case REFR:
		sg->out.INT = sg->Rr_refr;
		break;
	case DIFF:
		sg->out.INT = sg->Rr_diff;
		break;
	default:
		sg->out.INT = sg->Rr_gloss;
	}
}

//node_loader
//{
//	if (i > 0)
//		return false;
//
//	node->methods      = ObqRayDepthMethods;
//	node->output_type  = AI_TYPE_INT;
//	node->name         = "Obq_RayDepth";
//	node->node_type    = AI_NODE_SHADER;
//#ifdef _WIN32
//	strcpy_s(node->version, AI_VERSION);
//#else
//	strcpy(node->version, AI_VERSION);
//#endif
//
//	return true;
//}
