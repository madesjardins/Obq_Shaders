/*
Obq_Bend4Stereo:

	Fake incoming ray direction to give same specular for stereo

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

#include "Obq_Common.h"

// Arnold Thingy
//
AI_SHADER_NODE_EXPORT_METHODS(ObqBend4StereoSimpleMethods);

enum ObqBend4StereoSimpleParams { p_shaderIn, p_bendMode, p_leftCamera, p_centerCamera, p_rightCamera };

enum ObqBendModes {BEND_NONE, BEND_LEFT, BEND_RIGHT, BEND_CENTER};

// Shader Data Structure
//
typedef struct 
{
	int	bendMode;
	AtPoint leftCameraPos;
	AtPoint rightCameraPos;
	AtMatrix left2centerCameraMatrix;
	AtMatrix right2centerCameraMatrix;
	AtMatrix left2rightCameraMatrix;
	AtMatrix right2leftCameraMatrix;
}
ShaderData;

// parameters
//
node_parameters
{
	AiParameterRGBA("shaderIn", 1.0f,1.0f,1.0f,1.0f);					// shaderIn
	AiParameterINT("bendMode" , 0);
	AiParameterSTR("leftCamera" , "StereoCamera_Left");
	AiParameterSTR("centerCamera" , "StereoCamera");
	AiParameterSTR("rightCamera" , "StereoCamera_Right");
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	AiNodeSetLocalData(node,data);
}

node_update
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	
	data->bendMode = params[p_bendMode].INT;
	
	// Get .SItoA. index
	std::string camNodeName(AiNodeGetName(node));

	size_t sitoaIndex = camNodeName.rfind(".SItoA.");
	size_t lastPindex = camNodeName.rfind(".");
	if(lastPindex > sitoaIndex+6)
		lastPindex-=sitoaIndex;

	// Get all 3 cameras and all 3 matrices
	AtMatrix centerCameraMatrix,leftCameraMatrix,rightCameraMatrix;
	AtNode* leftCamera = NULL, *centerCamera=NULL, *rightCamera = NULL;
	std::string nameEnding(camNodeName.substr(sitoaIndex,lastPindex).c_str());
	
	leftCamera = AiNodeLookUpByName((std::string(params[p_leftCamera].STR).append(nameEnding).c_str()));
	AiNodeGetMatrix(leftCamera,"matrix",leftCameraMatrix);

	centerCamera = AiNodeLookUpByName((std::string(params[p_centerCamera].STR).append(nameEnding).c_str()));
	AiNodeGetMatrix(centerCamera,"matrix",centerCameraMatrix);

	rightCamera = AiNodeLookUpByName((std::string(params[p_rightCamera].STR).append(nameEnding).c_str()));
	AiNodeGetMatrix(rightCamera,"matrix",rightCameraMatrix);

	if(centerCamera==NULL && data->bendMode == BEND_CENTER)
	{
		AiMsgError("Center camera doesn't exist. Don't forget the Model name if any. ex: Model.CenterCamera");
	}
	else if(leftCamera==NULL && (data->bendMode == BEND_LEFT || data->bendMode == BEND_CENTER) )
	{
		AiMsgError("Left camera doesn't exist. Don't forget the Model name if any. ex: Model.LeftCamera");
	}
	else if( rightCamera==NULL && (data->bendMode == BEND_RIGHT || data->bendMode == BEND_CENTER) )
	{
		AiMsgError("Right camera doesn't exist. Don't forget the Model name if any. ex: Model.RightCamera");
	}
	else
	{
		////////////////////////////////////////////
		// calculate leftToCenter and rightToCenter
		////////////////////////////////////////////
		AtMatrix iLeftCameraMatrix, iRightCameraMatrix;
		AiM4Invert(leftCameraMatrix, iLeftCameraMatrix);
		AiM4Invert(rightCameraMatrix, iRightCameraMatrix);

		AiM4Mult(data->left2centerCameraMatrix, centerCameraMatrix,iLeftCameraMatrix);
		AiM4Mult(data->right2centerCameraMatrix, centerCameraMatrix, iRightCameraMatrix);
		AiM4Mult(data->left2rightCameraMatrix, rightCameraMatrix,iLeftCameraMatrix);
		AiM4Mult(data->right2leftCameraMatrix, leftCameraMatrix, iRightCameraMatrix);

		data->leftCameraPos.x = leftCameraMatrix[3][0];
		data->leftCameraPos.y = leftCameraMatrix[3][1];
		data->leftCameraPos.z = leftCameraMatrix[3][2];

		data->rightCameraPos.x = rightCameraMatrix[3][0];
		data->rightCameraPos.y = rightCameraMatrix[3][1];
		data->rightCameraPos.z = rightCameraMatrix[3][2];

	}

}

node_finish
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	bool left = AiV3Dist(sg->Ro,data->leftCameraPos) < AiV3Dist(sg->Ro,data->rightCameraPos);

	// If it's a camera ray and this is the first bounce
	if(sg->Rt & AI_RAY_CAMERA && sg->Rr == 0 && data->bendMode != BEND_NONE && !(left && data->bendMode == BEND_LEFT) && !(!left && data->bendMode == BEND_RIGHT))
	{
		// Old and new variables
		AtPoint newOrigin, oldOrigin = sg->Ro;
		AtVector oldDirection = sg->Rd;
		double oldLength = sg->Rl;
		
		// Calculate new origin
		switch(data->bendMode)
		{
		case BEND_CENTER : 
			if( left )
				AiM4PointByMatrixMult(&newOrigin,data->left2centerCameraMatrix,&sg->Ro);
			else
				AiM4PointByMatrixMult(&newOrigin,data->right2centerCameraMatrix,&sg->Ro);
			break;
		case BEND_LEFT : 
				AiM4PointByMatrixMult(&newOrigin,data->right2leftCameraMatrix,&sg->Ro);
			break;
		case BEND_RIGHT:
			AiM4PointByMatrixMult(&newOrigin,data->left2rightCameraMatrix,&sg->Ro);
			break;
		}

		// Calculate new direction and length
		sg->Ro = newOrigin;
		sg->Rd = AiV3Normalize(sg->P-newOrigin);
		sg->Rl = AiV3Dist(sg->P,newOrigin);
		
		// Evaluate shader with bent direction
		sg->out.RGBA = AiShaderEvalParamRGBA(p_shaderIn);

		// Put back old values
		sg->Ro = oldOrigin;
		sg->Rd = oldDirection;
		sg->Rl = oldLength;
	}
	else
	{
		// Evaluate shader with bent direction
		sg->out.RGBA = AiShaderEvalParamRGBA(p_shaderIn);
	}
}

//node_loader
//{
//	if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqBend4StereoSimpleMethods;
//	node->output_type  = AI_TYPE_RGBA;
//	node->name         = "Obq_Bend4Stereo";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}


