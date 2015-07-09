/*
Obq_RandomID :

random int based on object name.

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



// Arnold Thingy
//
AI_SHADER_NODE_EXPORT_METHODS(ObqRandomIDMethods);

// enum of parameters
//
enum ObqRandomIDParams {p_randMax, p_seed, p_stripModelName, p_stripFrameNumber, p_stripInstanceFrameNumber, p_stripInstanceID,p_stripInstanceShape, p_XtoA};

typedef struct 
{
	const char* XtoA_name;
	const char* XtoA_instance_name;
	int XtoA_instance_len;
	ObqPluginID plugin;
}
ShaderData;

// parameters
//
node_parameters
{
	AiParameterINT("randMax", 4);	
	AiParameterINT("seed", 233);
	AiParameterBOOL("stripModelName", false);
	AiParameterBOOL("stripFrameNumber", true);
	AiParameterBOOL("stripInstanceFrameNumber", true);
	AiParameterBOOL("stripInstanceID", false);
	AiParameterBOOL("stripInstanceShape", false);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));

	data->plugin = findPluginID(node);

	switch(data->plugin)
	{
	case MTOA:
		data->XtoA_name = "";
		data->XtoA_instance_name = ".Instance";
		data->XtoA_instance_len = 9;
		break;
	case SITOA:
	default:
		data->XtoA_name = ".SItoA.";
		data->XtoA_instance_name = ".SItoA.Instance";
		data->XtoA_instance_len = 15;
		break;
	}

	AiNodeSetLocalData(node,data);
}

node_update
{
}

node_finish
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);

	// GET NAME (This is far from optimal)
	std::string name(AiNodeGetName(sg->Op));
	
	// TEST .SItoA.
	std::size_t len = name.length();
	std::size_t lastXtoA = (data->plugin==SITOA?name.rfind(data->XtoA_name):len);
	std::size_t startNameIndex = 0;
	std::size_t endNameIndex = lastXtoA;

	if(endNameIndex==std::string::npos)
	{
		AiMsgError("[Obq_RandomID] : Couldn't find the frame number separator. Name is : %s and separator is %s",name.c_str(),data->XtoA_name);
		sg->out.RGBA = AI_RGBA_BLACK;
		return; 
	}

	// INSTANCE TEST
	std::size_t firstXtoA = (data->plugin==SITOA?name.find(data->XtoA_name):len);

	// STRIP MODEL
	if(AiShaderEvalParamBool(p_stripModelName))
	{
		
		std::size_t startDotModel = name.find('.');
		if(startDotModel < firstXtoA)
			startNameIndex = startDotModel+1;
	}

	// NOT STRIP FRAME#
	if(!AiShaderEvalParamBool(p_stripFrameNumber) || endNameIndex >= len)
		endNameIndex = len;

	// INSTANCE
	bool stripInstanceFrameNumber = AiShaderEvalParamBool(p_stripInstanceFrameNumber);
	bool stripInstanceID = AiShaderEvalParamBool(p_stripInstanceID);
	bool stripInstanceShape = AiShaderEvalParamBool(p_stripInstanceShape);

	if(stripInstanceFrameNumber || stripInstanceID || stripInstanceShape)
	{
		std::size_t startInstanceXtoA = name.find(data->XtoA_instance_name,(data->plugin==SITOA?firstXtoA:0));
		if(startInstanceXtoA != std::string::npos)
		{
			// strip the XtoA.Instance
			std::size_t startInstanceFrameNumber = startInstanceXtoA + data->XtoA_instance_len;
			std::size_t startInstanceID = name.find(".",startInstanceFrameNumber+1);
			std::size_t startInstanceShape = name.find(" ",startInstanceID);

			std::string instanceName("");

			if(!stripInstanceFrameNumber)
				instanceName += name.substr(startInstanceFrameNumber,startInstanceID-startInstanceFrameNumber);

			if(!stripInstanceID)
				instanceName += name.substr(startInstanceID,startInstanceShape-startInstanceID);

			if(!stripInstanceShape)
				instanceName += name.substr(startInstanceShape,lastXtoA-startInstanceShape);

			name = name.substr(0,startInstanceXtoA) + instanceName + name.substr(lastXtoA);
			endNameIndex -= ((lastXtoA-startInstanceXtoA) - instanceName.length());
		}
	}

	// Add character
	unsigned int a = AiShaderEvalParamInt(p_seed);

	for(std::size_t i = startNameIndex; i < endNameIndex; i++)
		//a += unsigned int(name[i]*name[i]*name[i])%31;
		a += (unsigned int)(name[i]*name[i])%73;
		//a += unsigned int(name[i]);

	// thanks to Mike F : http://stackoverflow.com/questions/167735/fast-pseudo-random-number-generator-for-procedural-content
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);

	sg->out.INT = static_cast<int>(a%AiShaderEvalParamInt(p_randMax));

}

//node_loader
//{
//	if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqRandomIDMethods;
//	node->output_type  = AI_TYPE_INT;
//	node->name         = "Obq_RandomID";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}
