/*
Obq_Shaders__Core5.cpp :

This is the main for Obq_Shaders.dll. It's a node loader.

*------------------------------------------------------------------------
Copyright (c) 2017 Marc-Antoine Desjardins, ObliqueFX (marcantoinedesjardins@gmail.com)

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

extern const AtNodeMethods* ObqLensDistortionMethods;
extern const AtNodeMethods* ObqColorMethods;
extern const AtNodeMethods* ObqColorspaceConverterMethods;
extern const AtNodeMethods* ObqStereoLensMethods;

enum SHADERS
{
	OBQ_LENSDISTORTION = 0,
	OBQ_COLOR = 1,
	OBQ_COLORSPACECONVERTER = 2,
	OBQ_STEREOLENS = 3
};

node_loader
{
	switch (i) 
	{     
	case OBQ_LENSDISTORTION:
		node->methods     = ObqLensDistortionMethods;
		node->output_type = AI_TYPE_UNDEFINED;
		node->name        = "Obq_LensDistortion";
		node->node_type   = AI_NODE_CAMERA;
		break;
	case OBQ_COLOR:
		node->methods      = ObqColorMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Color";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_COLORSPACECONVERTER:
		node->methods      = ObqColorspaceConverterMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_ColorspaceConverter";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_STEREOLENS:
		node->methods     = ObqStereoLensMethods;
		node->output_type = AI_TYPE_UNDEFINED;
		node->name        = "Obq_StereoLens";
		node->node_type   = AI_NODE_CAMERA;
		break;
	default:
		return false;      
	}

#ifdef _WIN32
	strcpy(node->version, AI_VERSION);
#else
	sprintf(node->version, AI_VERSION);
#endif

	return true;
}
