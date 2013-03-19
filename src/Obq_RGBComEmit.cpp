/*
Obq_RGBComEmit v2.06.0a (SItoA 2.6.0 - Arnold 4.0.11.0):



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
#include <ai.h>
AI_SHADER_NODE_EXPORT_METHODS(ObqRGBComEmitSimpleMethods);
 
// parameters
//
node_parameters
{
	AiParameterRGBA ( "passthroughC", 1.0f,1.0f,1.0f, 1.0f );
}

enum ObqRGBComEmitSimpleParams{p_passthroughC};

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
	AiStateSetMsgBool("ComRGB", true);
	sg->out.RGBA = AiShaderEvalParamRGBA(p_passthroughC);
	AiStateSetMsgBool("ComRGB", false);
}



//node_loader
//{
//	if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqRGBComEmitSimpleMethods;
//	node->output_type  = AI_TYPE_RGBA;
//	node->name         = "Obq_RGBComEmit";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}


