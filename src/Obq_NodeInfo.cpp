/*
Obq_NodeInfo :

A simple passthrough node that gives info

*------------------------------------------------------------------------
Copyright (c) 2012-2015 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)

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
AI_SHADER_NODE_EXPORT_METHODS(ObqNodeInfoMethods);


// enum for parameters
//
enum ObqNodeInfoParams
{
	p_passthrough,
	p_node_name
};

enum {	THISNODE, OTHERNODE, UNIVERSE, CAMERA };

node_parameters
{
	AiParameterRGBA("passthrough",1.0f,1.0f,1.0f,1.0f);
	AiParameterINT("mode",0);
	AiParameterSTR("node_name","");
}

node_initialize
{
	AiMsgInfo("#### Node Info <Node Initialize> [%s]",AiNodeGetName(node));
	AtNode* nodeToQuery;
	int mode = AiNodeGetInt(node,"mode");
	
	ObqPluginID plugin = findPluginID(node);

	switch(mode)
	{
	case OTHERNODE:		
		{
			// TODO :: check if MtoA, SItoA, HtoA, C4DtoA...
			if(plugin == SITOA)
			{
				std::string nodeName(AiNodeGetName(AiUniverseGetCamera()));
				std::size_t sitoaIndex = nodeName.rfind(".SItoA.");
				std::string lastPartName(nodeName.substr(sitoaIndex));
				AiMsgInfo("#### Node Info <Querying: %s>",std::string(AiNodeGetStr(node, "node_name")).append(lastPartName).c_str());
				nodeToQuery = AiNodeLookUpByName(std::string(AiNodeGetStr(node, "node_name")).append(lastPartName).c_str());
			}
			else
			{
				nodeToQuery = AiNodeLookUpByName(AiNodeGetStr(node, "node_name"));
				AiMsgInfo("#### Node Info <Querying: %s>",AiNodeGetStr(node, "node_name"));
			}
			break;
		}
	case UNIVERSE:
		nodeToQuery = AiUniverseGetOptions();
		break;
	case CAMERA:		
		nodeToQuery = AiUniverseGetCamera();
		break;
	default:
		nodeToQuery = node;
		break;
	}
	
	if(nodeToQuery == NULL)
	{
		AiMsgError("#### Node Info <Query FAILED> ");
		return;
	}

	AiMsgInfo("#### Node name : %s, Entry : %s",AiNodeGetName(nodeToQuery), AiNodeEntryGetName(AiNodeGetNodeEntry(nodeToQuery)));

	AiMsgInfo("\t-- Parameter --");

	AtParamIterator *piter = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(nodeToQuery));
	while (!AiParamIteratorFinished(piter))
	{
		const AtParamEntry *pentry = AiParamIteratorGetNext(piter);
		AiMsgInfo("\t\tName : %s, Type : %s", AiParamGetName(pentry),AiParamGetTypeName(AiParamGetType(pentry)));
		if(AiParamGetType(pentry) == AI_TYPE_ARRAY)
		{
			AtArray* a = AiNodeGetArray(nodeToQuery,AiParamGetName(pentry));
			if(a!=NULL)
				AiMsgInfo("\t\t\tArray Type : %s, num : %u ", AiParamGetTypeName(a->type),a->nelements);
			else
				AiMsgInfo("\t\t\tArray not available.");
		}
		else if(AiParamGetType(pentry) == AI_TYPE_STRING)
			AiMsgInfo("\t\t\tValue : %s", AiNodeGetStr(nodeToQuery,AiParamGetName(pentry)));
		else if(AiParamGetType(pentry) == AI_TYPE_FLOAT)
			AiMsgInfo("\t\t\tValue : %f", AiNodeGetFlt(nodeToQuery,AiParamGetName(pentry)));
		else if(AiParamGetType(pentry) == AI_TYPE_INT)
			AiMsgInfo("\t\t\tValue : %i", AiNodeGetInt(nodeToQuery,AiParamGetName(pentry)));

	}
	AiParamIteratorDestroy(piter);

	AiMsgInfo("\t-- User Parameter --");
	AtUserParamIterator *upiter = AiNodeGetUserParamIterator(nodeToQuery);
	while (!AiUserParamIteratorFinished(upiter))
	{
		const AtUserParamEntry *upentry = AiUserParamIteratorGetNext(upiter);
		AiMsgInfo("\t\tName : %s, Type : %s", AiUserParamGetName(upentry),AiParamGetTypeName(AiUserParamGetType(upentry)));
		if(AiUserParamGetType(upentry) == AI_TYPE_ARRAY)
		{
			//AtArray* a = AiNodeGetArray(nodeToQuery,AiUserParamGetName(upentry));
			//if(a!=NULL)
			AiMsgInfo("\t\t\tArray Type : %s", AiParamGetTypeName(AiUserParamGetArrayType(upentry)));
			//else
			//	AiMsgInfo("\t\t\tArray not available.");
		}
		else if(AiUserParamGetType(upentry) == AI_TYPE_STRING)
			AiMsgInfo("\t\t\tValue : %s", AiNodeGetStr(nodeToQuery,AiUserParamGetName(upentry)));
		else if(AiUserParamGetType(upentry) == AI_TYPE_FLOAT)
			AiMsgInfo("\t\t\tValue : %f", AiNodeGetFlt(nodeToQuery,AiUserParamGetName(upentry)));
		else if(AiUserParamGetType(upentry) == AI_TYPE_INT)
			AiMsgInfo("\t\t\tValue : %i", AiNodeGetInt(nodeToQuery,AiUserParamGetName(upentry)));
	}
	AiUserParamIteratorDestroy(upiter);

	AiMsgInfo("\t-- Metadata --");
	AtMetaDataIterator *mpiter = AiNodeEntryGetMetaDataIterator(AiNodeGetNodeEntry(nodeToQuery));
	while (!AiMetaDataIteratorFinished(mpiter))
	{
		const AtMetaDataEntry *mpentry = AiMetaDataIteratorGetNext(mpiter);
		AiMsgInfo("\t\tName : %s, Param : %s, Type : %s", mpentry->name, mpentry->param,AiParamGetTypeName(mpentry->type));
		if(mpentry->type == AI_TYPE_ARRAY)
			AiMsgInfo("\t\t\tArray Type : %s, Num : %u", AiParamGetTypeName(mpentry->value.ARRAY->type),mpentry->value.ARRAY->nelements);
	}
	AiMetaDataIteratorDestroy(mpiter);
}

node_update
{
	AiMsgInfo("#### Node Info <Node Update> [%s]",AiNodeGetName(node));
}

node_finish
{
	AiMsgInfo("#### Node Info <Node Finish> [%s]",AiNodeGetName(node));
}

shader_evaluate
{
	sg->out.RGBA = AiShaderEvalParamRGBA(p_passthrough);
}

//node_loader
//{
//	if (i > 0)
//		return false;
//
//	node->methods      = ObqNodeInfoMethods;
//	node->output_type  = AI_TYPE_RGBA;
//	node->name         = "Obq_NodeInfo";
//	node->node_type    = AI_NODE_SHADER;
//#ifdef _WIN32
//	strcpy_s(node->version, AI_VERSION);
//#else
//	strcpy(node->version, AI_VERSION);
//#endif
//
//	return true;
//}
