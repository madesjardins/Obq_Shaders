/*
Obq_SpectrumColor :

	It uses pbrt spectrum class to output a color based on SPDs.

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

#include "Obq_SpectrumColor.h"

AI_SHADER_NODE_EXPORT_METHODS(ObqSpectrumColorMethods);

enum ObqSpectrumColorParams { p_mode, p_filename, p_lambdaUnits, p_lambdasStr, p_intensitiesStr, p_xyz2rgb, p_gamma};


node_parameters
{
   AiParameterINT("mode", 1);						// Files or String
   AiParameterSTR("filename","");					// filename
   AiParameterINT("lambdaUnits", 0);				// 0 = nano, 1 = micro (*1000)
   AiParameterSTR("lambdasStr","460.0 530.0 650.0");	// SPDs strings space separated
   AiParameterSTR("intensitiesStr","0.0 1.0 0.0");	// ""
   AiParameterINT("xyz2rgb", SRGB_ILLUM_D65);		// primaries and illuminant
   AiParameterINT("gamma", OUTPUT_LINEAR);			// gamma
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	
	Spectrum r;
	data->rgb[0] = 0.0f;data->rgb[1] = 0.0f;data->rgb[2] = 0.0f;
	AiNodeSetLocalData(node,data);
}


node_update
{

	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	int mode = params[p_mode].INT;

	bool fileOK = false;

	float unitConversion = ((params[p_lambdaUnits].INT==UNITS_NANO)?1.0f:1000.0f);

	int xyz2rgb = params[p_xyz2rgb].INT;

	std::vector<float> lambdas;
	std::vector<float> intensities;
	
	if(mode == MODE_FILES)
	{
		short ft = fileType(params[p_filename].STR);
		bool ret = false;
		switch(ft)
		{
		case TYPE_CSV: // comma seperated
			ret = loadCSV(params[p_filename].STR,&lambdas,&intensities,unitConversion);
			break;
		case TYPE_TXT: // space separated
			ret = loadTXT(params[p_filename].STR,&lambdas,&intensities,unitConversion);
			break;
		case TYPE_SPD: // space seperated
			ret = loadSPD(params[p_filename].STR,&lambdas,&intensities,unitConversion);
			break;
		default :
			AiMsgError("File : \"%s\" is not of a recognized format.",params[p_filename].STR);
		}

		if(ret)
		{
			Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(intensities[0]), (int)lambdas.size(),xyz2rgb);
			s.ToRGB(data->rgb);			
		}
		else
			AiMsgError("There was a problem with file : \"%s\"",params[p_filename].STR);
	}
	else // INPUT_STRING
	{
		bool ret = loadStrings(params[p_lambdasStr].STR,params[p_intensitiesStr].STR,&lambdas,&intensities,unitConversion);

		if(ret)
		{
			Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(intensities[0]), (int)lambdas.size(),xyz2rgb);
			s.ToRGB(data->rgb);
		}
		else
			AiMsgError("Strings are not valid, check sizes (%u, %u, %u).",lambdas.size(),intensities.size());
	}

	// transfer to sRGB gamma ?
	if(params[p_gamma].INT == OUTPUT_SRGB && xyz2rgb != KEEP_XYZ)
		linear2sRGB(data->rgb);

	// copy to global
	AiMsgInfo("Obq_SpectrumColor : \t(R,G,B)\t= (%f,%f,%f)",data->rgb[0],data->rgb[1],data->rgb[2]);

	lambdas.clear();
	intensities.clear();
}

node_finish
{
	// Desallocate shader data
	ShaderData* data = (ShaderData*)AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	sg->out.RGB.r = data->rgb[0];
	sg->out.RGB.g = data->rgb[1];
	sg->out.RGB.b = data->rgb[2];
}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqSpectrumColorMethods;
//   node->output_type  = AI_TYPE_RGB;
//   node->name         = "Obq_SpectrumColor";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
