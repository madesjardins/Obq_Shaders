/*
Obq_TransformShadingPoint :

transform shading point

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
AI_SHADER_NODE_EXPORT_METHODS(ObqTransformShadingPointMethods);

// coordinates space
enum Coordinates{OBJECT, WORLD};
static const char* CoordinatesNames[] = 
{
	"Object",
	"World",
    NULL
};

enum TransformMode{MODE_SRT, MODE_MATRIX};
static const char* TransformModeNames[] = 
{
	"SRT",
    "Matrix",
    NULL
};
enum RotationOrder{ROT_XYZ, ROT_XZY, ROT_YXZ, ROT_YZX, ROT_ZXY, ROT_ZYX};
static const char* RotationOrderNames[] = 
{
	"XYZ",
	"XZY",
	"YXZ",
	"YZX",
	"ZXY",
	"ZYX",
    NULL
};
typedef struct 
{
	int coordinates;
	AtMatrix mat;
	bool invert;
}
ShaderData;

// enum for parameters
//
enum ObqTransformShadingParams
{
	p_passthrough,
	p_coordinates,
	p_transforms,
	p_scale,
	p_rotation,
	p_translation,
	p_invert,
	p_mode,
	p_rotationOrder
};

node_parameters
{
	AiParameterRGBA("passthrough",1.0f,1.0f,1.0f,1.0f);
	AiParameterENUM("coordinates",OBJECT,CoordinatesNames);
	AiParameterMTX("transforms",AI_M4_IDENTITY);
	AiParameterVEC("scale",1.0f,1.0f,1.0f);
	AiParameterVEC("rotation",0.0f,0.0f,0.0f);
	AiParameterVEC("translation",0.0f,0.0f,0.0f);
	AiParameterBOOL("invert",true);
	AiParameterENUM("mode",MODE_SRT,TransformModeNames);
	AiParameterENUM("rotationOrder",ROT_XYZ,RotationOrderNames);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	AiNodeSetLocalData(node,data);
	data->coordinates = OBJECT;
	data->invert = true;
}

node_update
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->coordinates = params[p_coordinates].INT;
	data->invert = params[p_invert].BOOL;
	if(params[p_mode].INT == MODE_MATRIX)
	{
		AtMatrix m;
		AiM4Copy(m,*(params[p_transforms].pMTX));
		if(data->invert)
			AiM4Invert(m,data->mat);
		else
			AiM4Copy(data->mat,m);
	}
	else
	{
		AtMatrix scale;
		AiM4Scaling (scale, &params[p_scale].VEC);
		
		AtMatrix rotX, rotYtmp, rotY, rotZ;
		AiM4RotationX (rotX, params[p_rotation].VEC.x);
		AiM4RotationY (rotYtmp, params[p_rotation].VEC.y);
		AiM4Transpose(rotYtmp,rotY);
		AiM4RotationZ (rotZ, params[p_rotation].VEC.z);

		AtMatrix rot, tmp;

		// Rotation Order
		switch(params[p_rotationOrder].INT)
		{
		case ROT_XYZ:
			AiM4Mult(tmp,rotX,rotY);
			AiM4Mult(rot,tmp,rotZ);
			break;
		case ROT_XZY:
			AiM4Mult(tmp,rotX,rotZ);
			AiM4Mult(rot,tmp,rotY);
			break;
		case ROT_YXZ:
			AiM4Mult(tmp,rotY,rotX);
			AiM4Mult(rot,tmp,rotZ);
			break;
		case ROT_YZX:
			AiM4Mult(tmp,rotY,rotZ);
			AiM4Mult(rot,tmp,rotX);
			break;
		case ROT_ZXY:
			AiM4Mult(tmp,rotZ,rotX);
			AiM4Mult(rot,tmp,rotY);
			break;
		case ROT_ZYX:
			AiM4Mult(tmp,rotZ,rotY);
			AiM4Mult(rot,tmp,rotX);
			break;
		default:
			AiM4Mult(tmp,rotX,rotY);
			AiM4Mult(rot,tmp,rotZ);
			break;
		}

		AtMatrix m;
		AiM4Mult(m,scale,rot);
		AtVector t = params[p_translation].VEC;
		m[3][0] = t.x;
		m[3][1] = t.y;
		m[3][2] = t.z;

		if(data->invert)
			AiM4Invert(m,data->mat);
		else
			AiM4Copy(data->mat,m);

	}

		AiMsgDebug("Transforms : ");
		AiMsgDebug("           %f %f %f %f",data->mat[0][0],data->mat[0][1],data->mat[0][2],data->mat[0][3]);
		AiMsgDebug("           %f %f %f %f",data->mat[1][0],data->mat[1][1],data->mat[1][2],data->mat[1][3]);
		AiMsgDebug("           %f %f %f %f",data->mat[2][0],data->mat[2][1],data->mat[2][2],data->mat[2][3]);
		AiMsgDebug("           %f %f %f %f",data->mat[3][0],data->mat[3][1],data->mat[3][2],data->mat[3][3]);
}

node_finish
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	switch(data->coordinates)
	{
	case OBJECT:
		{
			AtPoint p_temp = sg->P;
			AiM4PointByMatrixMult(&sg->P,data->mat,&p_temp);
			sg->out.RGBA = AiShaderEvalParamRGBA(p_passthrough);
			sg->P = p_temp;
		}
		break;
	case WORLD:
		{
			AtPoint p_temp = sg->Po;
			AiM4PointByMatrixMult(&sg->Po,data->mat,&p_temp);
			sg->out.RGBA = AiShaderEvalParamRGBA(p_passthrough);
			sg->Po = p_temp;
		}
		break;
	}
}

//node_loader
//{
//	if (i > 0)
//		return false;
//
//	node->methods      = ObqTransformShadingPointMethods;
//	node->output_type  = AI_TYPE_RGBA;
//	node->name         = "Obq_TransformShadingPoint";
//	node->node_type    = AI_NODE_SHADER;
//#ifdef _WIN32
//	strcpy_s(node->version, AI_VERSION);
//#else
//	strcpy(node->version, AI_VERSION);
//#endif
//
//	return true;
//}
