/*
Obq_Colorspace :

Convertes between different colorspaces

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
#include "O_Common5.h"
#include "O_Colorspaces5.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqColorspaceConverterMethods);

enum {CLAMP_INOUT, CLAMP_IN, CLAMP_OUT,CLAMP_NONE};

enum ObqColorspaceParams
{
	p_color,
	p_CSGammaIn, p_CSGammaOut,
	p_CSIlluminantIn, p_CSIlluminantOut,
	p_CSPrimariesIn, p_CSPrimariesOut,
	p_switchInOut,
	p_useBradford,
	p_clamp
};

typedef struct 
{
	OColorspaceConverter* colorspaceConverter;
	bool clamp;
}
ShaderData;

// ENUM MENU
static const char* OIlluminantIDNames[] = 
{
	"A",
	"B",
	"C",
	"D50",
	"D55",
	"D65",
	"D75",
	"E",
	"F2",
	"F7",
	"F11",
	"D60",
    NULL
};
// Gamma IDs
static const char* OGammaIDNames[] = 
{
	"Linear",
	"1.8 (Apple)",
	"2.2",
	"sRGB",
	"2.4",
	"Rec.709",
	"L*",
    NULL
};

// ColorSpace name
static const char* OCSIDNames[] = 
{
	"XSI Linear RGB",
	"Adobe RGB 1998",
	"Apple RGB",
	"Best RGB",
	"Beta RGB",
	"Bruce RGB",
	"CIE RGB",
	"ColorMatch RGB",
	"Don RGB 4",
	"ECI RGB V2",
	"EKTA Space PS5",
	"NTSC RGB",
	"PAL/SECAM RGB",
	"ProPhoto",
	"SMPTE_C",
	"sRGB",
	"Wide Gamut RGB",
	"Rec.709",
	"Rec.2020",
	"CIE XYZ",
	"Alexa Wide Gammut RGB",
	"ACEScg",
    NULL
};


node_parameters
{
	AiParameterRGBA("color",1.0f,1.0f,1.0f,1.0f);
	AiParameterEnum("csGammaIn",G_sRGB, OGammaIDNames); 
	AiParameterEnum("csGammaOut",G_LINEAR,OGammaIDNames); 
	AiParameterEnum("csIlluminantIn",ILMNT_D65,OIlluminantIDNames); 
	AiParameterEnum("csIlluminantOut",ILMNT_D65,OIlluminantIDNames); 
	AiParameterEnum("csPrimariesIn",CS_sRGB, OCSIDNames); 
	AiParameterEnum("csPrimariesOut",CS_sRGB, OCSIDNames); 
	AiParameterBool("switchInOut",false);
	AiParameterBool("useBradford",true);
	AiParameterBool("clamp",true);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	AiNodeSetLocalData(node,data);
	data->colorspaceConverter = NULL;
	data->clamp = true;
}

node_update
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	if(data->colorspaceConverter!=NULL)
		delete data->colorspaceConverter;
	
	data->colorspaceConverter = new OColorspaceConverter(	OGammaID(AiNodeGetInt(node, "csGammaIn")), OIlluminantID(AiNodeGetInt(node, "csIlluminantIn")),OCSID(AiNodeGetInt(node, "csPrimariesIn")),
															OGammaID(AiNodeGetInt(node, "csGammaOut")),OIlluminantID(AiNodeGetInt(node, "csIlluminantOut")),OCSID(AiNodeGetInt(node, "csPrimariesOut")),
															AiNodeGetBool(node, "switchInOut"), AiNodeGetBool(node, "useBradford"));
	data->clamp = AiNodeGetBool(node, "clamp");

	AiMsgInfo("%s",data->colorspaceConverter->getInfo().c_str());
	OColorspace csout = data->colorspaceConverter->getCSOut();

	glm::mat3 mout = data->colorspaceConverter->getConversionMatrix();
	AiMsgInfo("Matrix IN to OUT ->\n\t%f, %f, %f\n\t%f, %f, %f\n\t%f, %f, %f",mout[0][0],mout[1][0],mout[2][0],mout[0][1],mout[1][1],mout[2][1],mout[0][2],mout[1][2],mout[2][2]);

	glm::vec3 converted_rgb = data->colorspaceConverter->convertRGB(AiNodeGetRGBA(node, "color").r, AiNodeGetRGBA(node, "color").g, AiNodeGetRGBA(node, "color").b);
	AiMsgDebug("Color (%f, %f, %f) -> ( %f, %f, %f)",AiNodeGetRGBA(node, "color").r, AiNodeGetRGBA(node, "color").g, AiNodeGetRGBA(node, "color").b,converted_rgb.r,converted_rgb.g,converted_rgb.b);
}

node_finish
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	if(data->colorspaceConverter !=NULL)
		delete data->colorspaceConverter;
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AtRGBA colorIn = AiShaderEvalParamRGBA(p_color);

	// need to clamp ?
	if(data->clamp)
	{
		colorIn.r = AiClamp(colorIn.r, 0.0f, 1.0f);
		colorIn.g = AiClamp(colorIn.g, 0.0f, 1.0f);
		colorIn.b = AiClamp(colorIn.b, 0.0f, 1.0f);
	}

	glm::vec3 converted_rgb = data->colorspaceConverter->convertRGB(colorIn.r,colorIn.g,colorIn.b);

	if(data->clamp)
	{
		sg->out.RGBA().r = AiClamp(converted_rgb.r, 0.0f, 1.0f);
		sg->out.RGBA().g = AiClamp(converted_rgb.g, 0.0f, 1.0f);
		sg->out.RGBA().b = AiClamp(converted_rgb.b, 0.0f, 1.0f);
	}
	else
	{
		sg->out.RGBA().r = converted_rgb.r;
		sg->out.RGBA().g = converted_rgb.g;
		sg->out.RGBA().b = converted_rgb.b;
	}

	sg->out.RGBA().a = colorIn.a;
}

//node_loader
//{
//	if (i > 0)
//		return false;
//
//	node->methods      = ObqColorspaceConverterMethods;
//	node->output_type  = AI_TYPE_RGBA;
//	node->name         = "Obq_ColorspaceConverter";
//	node->node_type    = AI_NODE_SHADER;
//#ifdef _WIN32
//	strcpy_s(node->version, AI_VERSION);
//#else
//	strcpy(node->version, AI_VERSION);
//#endif
//
//	return true;
//}

