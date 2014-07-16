/*
Obq_Shaders__Core.cpp :

This is the main for Obq_Shaders.dll. It's a node loader.

*------------------------------------------------------------------------
Copyright (c) 2014 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)

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

extern AtNodeMethods* ObqAtmosphereSimpleMethods;
extern AtNodeMethods* ObqAngularCameraMethods;
extern AtNodeMethods* ObqBend4StereoSimpleMethods;
extern AtNodeMethods* ObqChangeRangeSimpleMethods;
extern AtNodeMethods* ObqColorSimpleMethods;
extern AtNodeMethods* ObqEnvironmentSimpleMethods;
extern AtNodeMethods* ObqEtchingSimpleMethods;
extern AtNodeMethods* ObqFresnelSimpleMethods;
extern AtNodeMethods* ObqHairSimpleMethods;
extern AtNodeMethods* ObqIESLightFilterMethod;
extern AtNodeMethods* ObqMeasuredMaterialSimpleMethods;
extern AtNodeMethods* ObqMessageFltSimpleMethods;
extern AtNodeMethods* ObqOpacitySimpleMethods;
extern AtNodeMethods* ObqRandomColorSimpleMethods;
extern AtNodeMethods* ObqRGBComEmitSimpleMethods;
extern AtNodeMethods* ObqRGBComLightFilterMethod;
extern AtNodeMethods* ObqRoot2TipSimpleMethods;
extern AtNodeMethods* ObqShadowSimpleMethods;
extern AtNodeMethods* ObqStereoLensMethods;
extern AtNodeMethods* ObqSurfaceEnvironmentSimpleMethods;
extern AtNodeMethods* ObqThicknessSimpleMethods;
extern AtNodeMethods* ObqToonSimpleMethods;
extern AtNodeMethods* ObqToonSimpleSimpleMethods;
extern AtNodeMethods* ObqTransformUVSimpleMethods;
extern AtNodeMethods* ObqUVRemapLensDistortionSimpleMethods;
extern AtNodeMethods* ObqVolumeThicknessSimpleMethods;


enum SHADERS
{
	OBQ_ANGULARCAMERA,
	OBQ_ATMOSPHERE,
	OBQ_BEND4STEREO,
	OBQ_CHANGERANGE,
	OBQ_COLOR,
	OBQ_ENVIRONMENT,
	OBQ_ETCHING,
	OBQ_FRESNEL,
	OBQ_HAIR,
	OBQ_IESLIGHTFILTER,
	OBQ_MEASUREDMATERIAL,
	OBQ_MESSAGEFLT,
	OBQ_OPACITY,
	OBQ_RANDOMCOLOR,
	OBQ_RGBCOMEMIT,
	OBQ_RGBCOMLIGHTFILTER,
	OBQ_ROOT2TIP,
	OBQ_SHADOW,
	OBQ_STEREOLENS,
	OBQ_SURFACEENVIRONMENT,
	OBQ_THICKNESS,
	OBQ_TOON,
	OBQ_TOONSIMPLE,
	OBQ_TRANSFORMUV,
	OBQ_UVREMAPLENSDISTORTION,
	OBQ_VOLUMETHICKNESS
};

node_loader
{
	switch (i) 
	{     
	case OBQ_ANGULARCAMERA:
		node->methods     = ObqAngularCameraMethods;
		node->output_type = AI_TYPE_UNDEFINED;
		node->name        = "Obq_AngularCamera";
		node->node_type   = AI_NODE_CAMERA;
		break;
	case OBQ_ATMOSPHERE:
		node->methods      = ObqAtmosphereSimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Atmosphere";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_BEND4STEREO:
		node->methods      = ObqBend4StereoSimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Bend4Stereo";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_CHANGERANGE:
		node->methods      = ObqChangeRangeSimpleMethods;
		node->output_type  = AI_TYPE_FLOAT;
		node->name         = "Obq_ChangeRange";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_COLOR:
		node->methods      = ObqColorSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Color";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_ENVIRONMENT:
		node->methods      = ObqEnvironmentSimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Environment";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_ETCHING:
		node->methods      = ObqEtchingSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Etching";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_FRESNEL:
		node->methods      = ObqFresnelSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Fresnel";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_HAIR:
		node->methods      = ObqHairSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Hair";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_IESLIGHTFILTER:
		node->methods      = ObqIESLightFilterMethod;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_IESLightFilter";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MEASUREDMATERIAL:
		node->methods      = ObqMeasuredMaterialSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_MeasuredMaterial";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGEFLT:
		node->methods      = ObqMessageFltSimpleMethods;
		node->output_type  = AI_TYPE_FLOAT;
		node->name         = "Obq_MessageFlt";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_OPACITY:
		node->methods      = ObqOpacitySimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Opacity";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_RANDOMCOLOR:
		node->methods      = ObqRandomColorSimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_RandomColor";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_RGBCOMEMIT:
		node->methods      = ObqRGBComEmitSimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_RGBComEmit";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_RGBCOMLIGHTFILTER:
		node->methods      = ObqRGBComLightFilterMethod;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_RGBComLightFilter";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_ROOT2TIP:
		node->methods      = ObqRoot2TipSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Root2Tip";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_SHADOW:
		node->methods      = ObqShadowSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Shadow";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_STEREOLENS:
		node->methods     = ObqStereoLensMethods;
		node->output_type = AI_TYPE_UNDEFINED;
		node->name        = "Obq_StereoLens";
		node->node_type   = AI_NODE_CAMERA;
		break;
	case OBQ_SURFACEENVIRONMENT:
		node->methods      = ObqSurfaceEnvironmentSimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_SurfaceEnvironment";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_THICKNESS:
		node->methods      = ObqThicknessSimpleMethods;
		node->output_type  = AI_TYPE_FLOAT;
		node->name         = "Obq_Thickness";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_TOON:
		node->methods      = ObqToonSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Toon";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_TOONSIMPLE:
		node->methods      = ObqToonSimpleSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_ToonSimple";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_TRANSFORMUV:
		node->methods      = ObqTransformUVSimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_TransformUV";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_UVREMAPLENSDISTORTION:
		node->methods      = ObqUVRemapLensDistortionSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_UVRemapLensDistortion";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_VOLUMETHICKNESS:
		node->methods      = ObqVolumeThicknessSimpleMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_VolumeThickness";
		node->node_type    = AI_NODE_SHADER;
		break;
	default:
		return false;      
	}
#ifdef _WIN32
	sprintf_s(node->version, AI_VERSION);
#else
	sprintf(node->version, AI_VERSION);
#endif

	return true;
}
