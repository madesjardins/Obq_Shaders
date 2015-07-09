/*
Obq_Opacity :

	a simple but optimized opacity cut-out.

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
AI_SHADER_NODE_EXPORT_METHODS(ObqOpacityMethods);

// enum for parameters
//
enum ObqOpacityParams { p_color, p_channel, p_opacityRGBA, p_opacityScalar, p_hard, p_threshold, p_invert };

// enum for channel
//
enum {OPRED, OPGREEN, OPBLUE, OPALPHA, OPRGB, OPSCALAR,INPUTALPHA};

// shader data struct
//
typedef struct 
{
	int channel;
}
ShaderData;


node_parameters
{
	AiParameterRGBA("color",1.0f,1.0f,1.0f,1.0f);
	AiParameterINT("channel", INPUTALPHA);		
    AiParameterRGBA("opacityRGBA",1.0f,1.0f,1.0f,1.0f);
    AiParameterFLT("opacityScalar", 1.0f);
	AiParameterBOOL("hard",false);
	AiParameterFLT("threshold", 0.0f);
	AiParameterBOOL("invert",false);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->channel = INPUTALPHA;
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
	case INPUTALPHA:
		{
			sg->out.RGBA =  AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = minimax(0.0f,sg->out.RGBA.a,1.0f);
			sg->out.RGBA.a = 1.0f;
			break;
		}
	case OPRED:
		{
			float op = AiShaderEvalParamRGBA(p_opacityRGBA).r;
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = minimax(0.0f,op,1.0f);
			break;
		}
	case OPGREEN:
		{
			float op = AiShaderEvalParamRGBA(p_opacityRGBA).g;
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = minimax(0.0f,op,1.0f);
			break;
		}
	case OPBLUE:
		{
			float op = AiShaderEvalParamRGBA(p_opacityRGBA).b;
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = minimax(0.0f,op,1.0f);
			break;
		}
	case OPALPHA:
		{
			float op = AiShaderEvalParamRGBA(p_opacityRGBA).a;
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = minimax(0.0f,op,1.0f);
			break;
		}
	case OPRGB:
		{
			AtColor op = AiColorClamp(AiShaderEvalParamRGBA(p_opacityRGBA).rgb(),0.0f,1.0f);
			if(op.r != 0.0f || op.g != 0.0f || op.b != 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = op;
			break;
		}
	case OPSCALAR:
		{
			float op = AiShaderEvalParamFlt(p_opacityScalar);
			if(op > 0.0f)
				sg->out.RGBA = AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = minimax(0.0f,op,1.0f);
			break;
		}
	default:
		{
			sg->out.RGBA =  AiShaderEvalParamRGBA(p_color);
			sg->out_opacity = minimax(0.0f,sg->out.RGBA.a,1.0f);
			sg->out.RGBA.a = 1.0f;
			break;
		}
	}

	// hard opacity
	if(AiShaderEvalParamBool(p_hard))
	{
		float threshold = AiShaderEvalParamFlt(p_threshold);
		// RED
		if(sg->out_opacity.r > threshold)
			sg->out_opacity.r = 1.0f;
		else
			sg->out_opacity.r = 0.0f;
		// GREEN
		if(sg->out_opacity.g > threshold)
			sg->out_opacity.g = 1.0f;
		else
			sg->out_opacity.g = 0.0f;
		// BLUE
		if(sg->out_opacity.b > threshold)
			sg->out_opacity.b = 1.0f;
		else
			sg->out_opacity.b = 0.0f;
	}

	// Invert ?
	if(AiShaderEvalParamBool(p_invert))
		sg->out_opacity = AI_RGB_WHITE - sg->out_opacity;

}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqOpacityMethods;
//   node->output_type  = AI_TYPE_RGBA;
//   node->name         = "Obq_Opacity";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
