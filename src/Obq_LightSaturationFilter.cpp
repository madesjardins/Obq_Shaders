/*
Obq_LightSaturationFilter :

Send receive test.

*------------------------------------------------------------------------
Copyright (c) 2012-2015 Marc-Antoine Desjardins, ObliqueFX (marcantoinedesjardins@gmail.com)

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

//---------------
// Arnold thingy
AI_SHADER_NODE_EXPORT_METHODS(ObqLightSaturationFilterMethod);

//-------------------
// Arnold enum params
enum Obq_LightSaturationFilter_Params {p_mode, p_key, p_triggerValue, p_saturation, p_defaultSaturation};

enum Obq_LightSaturationFilter_Mode {SIMPLE, TRIGGER, ASVALUE};
static const char* Obq_LightSaturationFilter_ModeNames[] = 
{
	"Simple saturation modifier",
    "Use trigger message",
    "Use message as saturation",
    NULL
};
// shader data struct
//
typedef struct 
{
	int mode;
	const char* key;
	float triggerValue;
	float saturation;
	float defaultSaturation;
}
ShaderData;

//-----------------------
// Arnold node parameters
node_parameters
{
	AiParameterENUM("mode", SIMPLE,Obq_LightSaturationFilter_ModeNames);
	AiParameterSTR("key", "O1");
	AiParameterFLT("triggerValue", 1.0f);
	AiParameterFLT("saturation", 0.0f);
	AiParameterFLT("defaultSaturation", 1.0f);
}


//------------------
// Arnold initialize
node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->mode = SIMPLE;
	data->key = "O1";
	data->triggerValue = 1.0f;
	data->saturation = 0.0f;
	data->defaultSaturation = 1.0f;
	AiNodeSetLocalData (node, data);
}


//--------------
// Arnold update
node_update
{	
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	data->mode = AiNodeGetInt(node,"mode");
	data->key = AiNodeGetStr(node,"key");
	data->triggerValue = AiNodeGetFlt(node,"triggerValue");
	data->saturation = minimax(0.0f,AiNodeGetFlt(node,"saturation"),1.0f);
	data->defaultSaturation = minimax(0.0f,AiNodeGetFlt(node,"defaultSaturation"),1.0f);
}


inline void saturateLiu(AtColor& liu, float saturation)
{
	float oneMinusSaturation = 1.0f-saturation;
	float mixluminance = oneMinusSaturation*(0.2126f*liu.r + 0.7152f*liu.g + 0.0722f*liu.b);
	liu.r = mixluminance + saturation*liu.r;
	liu.g = mixluminance + saturation*liu.g;
	liu.b = mixluminance + saturation*liu.b;
}

//----------------
// Arnold evaluate
shader_evaluate
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	
	// Unoccluded intensity
	float ret = -1.0f;
	if(data->mode == SIMPLE)
	{
		saturateLiu(sg->Liu,data->saturation);
	}
	else if(AiStateGetMsgFlt(data->key, &ret)) // a message exists
	{	
		if(data->mode == TRIGGER && ret == data->triggerValue )
			saturateLiu(sg->Liu,data->saturation);
		else if(data->mode == ASVALUE)
			saturateLiu(sg->Liu,minimax(0.0f,ret,1.0f));
		else
			saturateLiu(sg->Liu,data->defaultSaturation);
	}
	else
		saturateLiu(sg->Liu,data->defaultSaturation);
}


//--------------
// Arnold finish
node_finish
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	AiFree(data);
}

//node_loader
//{
//   if (i > 0)
//      return false;
//
//   node->methods      = ObqLightSaturationFilterMethod;
//   node->output_type  = AI_TYPE_RGB;
//   node->name         = "Obq_LightSaturationFilter";
//   node->node_type    = AI_NODE_SHADER;
//#ifdef _WIN32
//   strcpy_s(node->version, AI_VERSION);
//#else
//   strcpy(node->version, AI_VERSION);
//#endif
//   return true;
//}

