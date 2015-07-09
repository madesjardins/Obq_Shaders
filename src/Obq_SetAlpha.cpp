/*
Obq_SetAlpha :

Sets the alpha of an RGB color

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
AI_SHADER_NODE_EXPORT_METHODS(ObqSetAlphaMethods);


// enum for parameters
//
enum ObqSetAlphaParams
{
	p_color,
	p_alpha
};

node_parameters
{
	AiParameterRGB("color",1.0f,1.0f,1.0f);
	AiParameterFLT("alpha",1.0f);
}

node_initialize
{;}

node_update
{;}

node_finish
{;}

shader_evaluate
{
	AtColor c = AiShaderEvalParamRGB(p_color);
	sg->out.RGBA.r = c.r;
	sg->out.RGBA.g = c.g;
	sg->out.RGBA.b = c.b;
	sg->out.RGBA.a = AiShaderEvalParamFlt(p_alpha);
}

//node_loader
//{
//	if (i > 0)
//		return false;
//
//	node->methods      = ObqSetAlphaMethods;
//	node->output_type  = AI_TYPE_RGBA;
//	node->name         = "Obq_SetAlpha";
//	node->node_type    = AI_NODE_SHADER;
//#ifdef _WIN32
//	strcpy_s(node->version, AI_VERSION);
//#else
//	strcpy(node->version, AI_VERSION);
//#endif
//
//	return true;
//}
