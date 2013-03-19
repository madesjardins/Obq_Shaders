/*
Obq_Opacity v2.06.0b (SItoA 2.6.0 - Arnold 4.0.11.0) :

	a simple but optimized opacity cut-out.

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

#include "ai.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqOpacitySimpleMethods);

// enum for parameters
//
enum ObqOpacitySimpleParams { p_color, p_channel, p_opacityRGBA, p_opacityScalar };

// enum for channel
//
enum {RED, GREEN, BLUE, ALPHA, RGB, SCALAR};

// shader data struct
//
typedef struct 
{
	int channel;
}
ShaderData;

// minimax clamps b between a and c
//
// @param a		The lower bound
// @param b		The value that will be clamped
// @param c		The upper bound
// @return		b clamped between a and c
inline float Minimax(float a, float b, float c)
{
	if(b<a) 
		return a; 
	else if(b>c) 
		return c;
	else
		return b;
}


node_parameters
{
	AiParameterRGBA("color",1.0f,1.0f,1.0f,1.0f);
	AiParameterINT("channel", 3);		
    AiParameterRGBA("opacityRGBA",1.0f,1.0f,1.0f,1.0f);
    AiParameterFLT("opacityScalar", 1.0f);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->channel = 3;
	AiNodeSetLocalData(node,data);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	data->channel = params[p_channel].INT;
}

node_finish
{
	// Desallocate shader data
	AiFree((ShaderData*)AiNodeGetLocalData(node));
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	switch(data->channel)
	{
	case RED:
		{
			float op = AiShaderEvalParamRGBA(p_opacityRGBA).r;
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = Minimax(0.0f,op,1.0f);
			break;
		}
	case GREEN:
		{
			float op = AiShaderEvalParamRGBA(p_opacityRGBA).g;
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = Minimax(0.0f,op,1.0f);
			break;
		}
	case BLUE:
		{
			float op = AiShaderEvalParamRGBA(p_opacityRGBA).b;
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = Minimax(0.0f,op,1.0f);
			break;
		}
	case ALPHA:
		{
			float op = AiShaderEvalParamRGBA(p_opacityRGBA).a;
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = Minimax(0.0f,op,1.0f);
			break;
		}
	case RGB:
		{
			AtColor op = AiColorClamp(AiShaderEvalParamRGBA(p_opacityRGBA).rgb(),0.0f,1.0f);
			if(op.r != 0.0f || op.g != 0.0f || op.b != 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = op;
			break;
		}
	case SCALAR:
		{
			float op = AiShaderEvalParamFlt(p_opacityScalar);
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = Minimax(0.0f,op,1.0f);
			break;
		}
	}
}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqOpacitySimpleMethods;
//   node->output_type  = AI_TYPE_RGBA;
//   node->name         = "Obq_Opacity";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
