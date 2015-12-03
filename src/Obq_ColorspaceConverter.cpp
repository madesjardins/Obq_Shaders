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
#include "O_Common.h"
#include "O_Colorspaces.h"

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
	"D60",
	"D65",
	"D75",
	"E",
	"F2",
	"F7",
	"F11",
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
	AiParameterENUM("csGammaIn",G_sRGB, OGammaIDNames); 
	AiParameterENUM("csGammaOut",G_LINEAR,OGammaIDNames); 
	AiParameterENUM("csIlluminantIn",ILMNT_D65,OIlluminantIDNames); 
	AiParameterENUM("csIlluminantOut",ILMNT_D65,OIlluminantIDNames); 
	AiParameterENUM("csPrimariesIn",CS_sRGB, OCSIDNames); 
	AiParameterENUM("csPrimariesOut",CS_sRGB, OCSIDNames); 
	AiParameterBOOL("switchInOut",false);
	AiParameterBOOL("useBradford",true);
	AiParameterBOOL("clamp",true);
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
	
	data->colorspaceConverter = new OColorspaceConverter(	OGammaID(params[p_CSGammaIn].INT),OIlluminantID(params[p_CSIlluminantIn].INT),OCSID(params[p_CSPrimariesIn].INT),
															OGammaID(params[p_CSGammaOut].INT),OIlluminantID(params[p_CSIlluminantOut].INT),OCSID(params[p_CSPrimariesOut].INT),
															params[p_switchInOut].BOOL, params[p_useBradford].BOOL);
	data->clamp = params[p_clamp].BOOL;

	AiMsgInfo("%s",data->colorspaceConverter->getInfo().c_str());
	OColorspace csout = data->colorspaceConverter->getCSOut();

	glm::mat3 mout = data->colorspaceConverter->getConversionMatrix();
	AiMsgInfo("Matrix IN to OUT ->\n\t%f, %f, %f\n\t%f, %f, %f\n\t%f, %f, %f",mout[0][0],mout[1][0],mout[2][0],mout[0][1],mout[1][1],mout[2][1],mout[0][2],mout[1][2],mout[2][2]);

	glm::vec3 converted_rgb = data->colorspaceConverter->convertRGB(params[p_color].RGBA.r,params[p_color].RGBA.g,params[p_color].RGBA.b);
	AiMsgDebug("Color (%f, %f, %f) -> ( %f, %f, %f)",params[p_color].RGBA.r,params[p_color].RGBA.g,params[p_color].RGBA.b,converted_rgb.r,converted_rgb.g,converted_rgb.b);
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
		colorIn.r = CLAMP(colorIn.r,0.0f,1.0f);
		colorIn.g = CLAMP(colorIn.g,0.0f,1.0f);
		colorIn.b = CLAMP(colorIn.b,0.0f,1.0f);
	}

	glm::vec3 converted_rgb = data->colorspaceConverter->convertRGB(colorIn.r,colorIn.g,colorIn.b);

	if(data->clamp)
	{
		sg->out.RGBA.r = CLAMP(converted_rgb.r, 0.0f, 1.0f);
		sg->out.RGBA.g = CLAMP(converted_rgb.g, 0.0f, 1.0f);
		sg->out.RGBA.b = CLAMP(converted_rgb.b, 0.0f, 1.0f);
	}
	else
	{
		sg->out.RGBA.r = converted_rgb.r;
		sg->out.RGBA.g = converted_rgb.g;
		sg->out.RGBA.b = converted_rgb.b;
	}

	sg->out.RGBA.a = colorIn.a;
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

