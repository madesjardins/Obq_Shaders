/*
Obq_RGBComLightFilter v2.06.0b (SItoA 2.6.0 - Arnold 4.0.11.0):

Send receive test.

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

//---------------
// Arnold thingy
AI_SHADER_NODE_EXPORT_METHODS(ObqRGBComLightFilterMethod);

//-------------------
// Arnold enum params
enum Obq_RGBComLightFilter_Params {p_defaultC, p_channel};

const AtRGB colorArray[] = {AI_RGB_RED, AI_RGB_GREEN, AI_RGB_BLUE};

//const char *enum_Obq_LightFilter[] = {"Obq_RGBComLightFilter"};

typedef struct 
{
	AtColor defaultC;
	int channel;
   
}
ShaderData;

//-----------------------
// Arnold node parameters
node_parameters
{
   AiParameterRGB ( "defaultC", 1.0f,1.0f,1.0f );
   AiParameterInt ( "channel", 3 );
}


//------------------
// Arnold initialize
node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	AiNodeSetLocalData (node, data);
}


//--------------
// Arnold update
node_update
{	
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	data->channel = AiNodeGetInt(node,"channel");
	data->defaultC = AiNodeGetRGB(node,"defaultC");
}


//----------------
// Arnold evaluate
shader_evaluate
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	
	// Unoccluded intensity
	bool ret = false;
	if(data->channel != 3)
		if(AiStateGetMsgBool("ComRGB", &ret))
			if(ret)
			{
				sg->Liu = colorArray[data->channel];
				return;
			}

	sg->Liu =  data->defaultC;
}


//--------------
// Arnold finish
node_finish
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	AiFree(data);
}

////-------------------
//// Arnold node loader
//node_loader
//{
//if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqRGBComLightFilterMethod;
//	node->output_type  = AI_TYPE_RGB;
//	node->name         = "Obq_RGBComLightFilter";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}
