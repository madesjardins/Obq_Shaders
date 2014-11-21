/*
Obq_TransformUV :

Just plug this between the image node and the clip.

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

#include "O_Common.h"


// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqTransformUVMethods);

// enum for parameters
//
enum ObqTransformUVParams {p_tex, p_mode,p_scaleX, p_scaleY, p_rotate, p_translateX, p_translateY, p_pivotX, p_pivotY};

node_parameters
{
	AiParameterRGBA("tex",0.0f,0.0f,0.0f,0.0f);
	AiParameterINT("mode", 4);
	AiParameterFLT("scaleX", 1.0f);
	AiParameterFLT("scaleY", 1.0f);
	AiParameterFLT("rotate", 0.0f);
	AiParameterFLT("translateX", 0.0f);
	AiParameterFLT("translateY", 0.0f);
	AiParameterFLT("pivotX", 0.5f);
	AiParameterFLT("pivotY", 0.5f);
}

// enum mode
enum ObqTransformUVMode {SRT,STR,RST,RTS,TSR,TRS};

node_initialize
{  
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
	// save uv
	float saved_u = sg->u;
	float saved_v = sg->v;
	
	// pivot var
	float pivotX = AiShaderEvalParamFlt(p_pivotX);
	float pivotY = AiShaderEvalParamFlt(p_pivotY);

	// rotation var
	float angle = float(AiShaderEvalParamFlt(p_rotate)*AI_DTOR);
	float costheta = std::cos(angle);
	float sintheta = std::sin(angle);


	// GO !! pivot
	float u = saved_u-pivotX;
	float v = saved_v-pivotY;

	// MODE
	switch(AiShaderEvalParamInt(p_mode))
	{
	case SRT:
		// scale
		u*=AiShaderEvalParamFlt(p_scaleX);
		v*=AiShaderEvalParamFlt(p_scaleY);
		// rotate
		sg->u = costheta*u - sintheta*v;
		sg->v = sintheta*u + costheta*v;
		// translate
		sg->u+=AiShaderEvalParamFlt(p_translateX);
		sg->v+=AiShaderEvalParamFlt(p_translateY);
		break;
	case STR:
		// scale
		u*=AiShaderEvalParamFlt(p_scaleX);
		v*=AiShaderEvalParamFlt(p_scaleY);
		// translate
		u+=AiShaderEvalParamFlt(p_translateX);
		v+=AiShaderEvalParamFlt(p_translateY);
		// rotate
		sg->u = costheta*u - sintheta*v;
		sg->v = sintheta*u + costheta*v;
		break;
	case RST:
		// rotate
		sg->u = costheta*u - sintheta*v;
		sg->v = sintheta*u + costheta*v;
		// scale
		sg->u*=AiShaderEvalParamFlt(p_scaleX);
		sg->v*=AiShaderEvalParamFlt(p_scaleY);
		// translate
		sg->u+=AiShaderEvalParamFlt(p_translateX);
		sg->v+=AiShaderEvalParamFlt(p_translateY);
		break;
	case RTS:
		// rotate
		sg->u = costheta*u - sintheta*v;
		sg->v = sintheta*u + costheta*v;
		// translate
		sg->u+=AiShaderEvalParamFlt(p_translateX);
		sg->v+=AiShaderEvalParamFlt(p_translateY);
		// scale
		sg->u*=AiShaderEvalParamFlt(p_scaleX);
		sg->v*=AiShaderEvalParamFlt(p_scaleY);
		break;
	case TSR:
		// translate
		u+=AiShaderEvalParamFlt(p_translateX);
		v+=AiShaderEvalParamFlt(p_translateY);
		// scale
		u*=AiShaderEvalParamFlt(p_scaleX);
		v*=AiShaderEvalParamFlt(p_scaleY);
		// rotate
		sg->u = costheta*u - sintheta*v;
		sg->v = sintheta*u + costheta*v;
		break;
	case TRS:
		// translate
		u+=AiShaderEvalParamFlt(p_translateX);
		v+=AiShaderEvalParamFlt(p_translateY);
		// rotate
		sg->u = costheta*u - sintheta*v;
		sg->v = sintheta*u + costheta*v;
		// scale
		sg->u*=AiShaderEvalParamFlt(p_scaleX);
		sg->v*=AiShaderEvalParamFlt(p_scaleY);
		break;
	}

	// unpivot
	sg->u += pivotX;
	sg->v += pivotY;

	// get the pixel
	sg->out.RGBA = AiShaderEvalParamRGBA(p_tex);
	
	// restore uvs
	sg->u = saved_u;
	sg->v = saved_v;

}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqTransformUVMethods;
//   node->output_type  = AI_TYPE_RGBA;
//   node->name         = "Obq_TransformUV";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
