/*
Obq_IESLightFilter :

IES light filter

Using Yafaray's old ies parser.

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
#include "yafaray/yafaray_ies_parser.h"

//---------------
// Arnold thingy
AI_SHADER_NODE_EXPORT_METHODS(ObqIESLightFilterMethod);

//-------------------
// Arnold enum params
enum Obq_IESLightFilter_Params {p_filename};

//const char *enum_Obq_LightFilter[] = {"Obq_IESLightFilter"};

//-------------
// Shader Data
typedef struct 
{
   IESData_t iesdata;
}
ShaderData;


//-----------------------
// Arnold node parameters
node_parameters
{
   AiParameterStr ( "filename", "D:\\IES.ies" );
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

	// Try to load .IES
	if(!data->iesdata.parseIESFile(std::string(AiNodeGetStr(node,"filename"))))
		AiMsgError("[Obq_IESLightFilter] Error reading file : \"%s\"",AiNodeGetStr(node,"filename"));
}


//----------------
// Arnold evaluate
shader_evaluate
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	
	// Get transform matrix of the light
	AtMatrix mat, imat;
	AtVector dir;
	AiNodeGetMatrix(sg->Lp, "matrix", mat);			// not optimal, but ok for now
	AiM4Invert(mat,imat);							// Get inverse transform
	
	AtVector tmp = -sg->Ld;
	AiM4VectorByMatrixMult(&dir,imat,&tmp);

	// Dir -> (phi-theta)°
	float u, v;
	dir.y=-dir.y;						// To get lamp pointing in Y direction instead of -Y
	AiMappingLatLong(&dir, &u, &v);		// theta-phi parametrization
	u = u*360.0f;						// phi in degrees
	v = v*180.0f;						// theta in degrees

	// Unoccluded intensity
	sg->Liu *= data->iesdata.getRadiance(u,v);
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
//	node->methods      = Obq_IESLightFilter_Method;
//	node->output_type  = AI_TYPE_RGB;
//	node->name         = "Obq_IESLightFilter";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}
