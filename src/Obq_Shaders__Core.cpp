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

#include "O_Common.h"

extern AtNodeMethods* ObqAngularCameraMethods;
extern AtNodeMethods* ObqAtmosphereMethods;
extern AtNodeMethods* ObqBend4StereoMethods;
extern AtNodeMethods* ObqCameraProjectionSpaceMethods;
extern AtNodeMethods* ObqChangeRangeMethods;
extern AtNodeMethods* ObqColorMethods;
extern AtNodeMethods* ObqColorspaceConverterMethods;
extern AtNodeMethods* ObqEnvironmentMethods;
extern AtNodeMethods* ObqEtchingMethods;
extern AtNodeMethods* ObqFresnelMethods;
extern AtNodeMethods* ObqHairMethods;
extern AtNodeMethods* ObqIESLightFilterMethod;
extern AtNodeMethods* ObqLensDistortionMethods;
extern AtNodeMethods* ObqLightSaturationFilterMethod;
extern AtNodeMethods* ObqMeasuredMaterialMethods;
extern AtNodeMethods* ObqMessageGetBoolMethods;
extern AtNodeMethods* ObqMessageGetColorMethods;
extern AtNodeMethods* ObqMessageGetFltMethods;
extern AtNodeMethods* ObqMessageGetIntMethods;
extern AtNodeMethods* ObqMessageGetVectorMethods;
extern AtNodeMethods* ObqMessageSetBoolMethods;
extern AtNodeMethods* ObqMessageSetColorMethods;
extern AtNodeMethods* ObqMessageSetFltMethods;
extern AtNodeMethods* ObqMessageSetIntMethods;
extern AtNodeMethods* ObqMessageSetVectorMethods;
extern AtNodeMethods* ObqMessageStoreBoolMethods;
extern AtNodeMethods* ObqMessageStoreColorMethods;
extern AtNodeMethods* ObqMessageStoreFltMethods;
extern AtNodeMethods* ObqMessageStoreIntMethods;
extern AtNodeMethods* ObqMessageStoreVectorMethods;
extern AtNodeMethods* ObqOpacityMethods;
extern AtNodeMethods* ObqRandomColorMethods;
extern AtNodeMethods* ObqRandomIDMethods;
extern AtNodeMethods* ObqRayDepthMethods;
extern AtNodeMethods* ObqRGBComEmitMethods;
extern AtNodeMethods* ObqRGBComLightFilterMethod;
extern AtNodeMethods* ObqRoot2TipMethods;
extern AtNodeMethods* ObqSetAlphaMethods;
extern AtNodeMethods* ObqShadowMethods;
extern AtNodeMethods* ObqSpectrumColorMethods;
extern AtNodeMethods* ObqStereoLensMethods;
extern AtNodeMethods* ObqSurfaceEnvironmentMethods;
extern AtNodeMethods* ObqThicknessMethods;
extern AtNodeMethods* ObqToonMethods;
extern AtNodeMethods* ObqToonSimpleMethods;
extern AtNodeMethods* ObqTransformShadingPointMethods;
extern AtNodeMethods* ObqTransformUVMethods;
extern AtNodeMethods* ObqUVRemapLensDistortionMethods;
extern AtNodeMethods* ObqVolumeThicknessMethods;


enum SHADERS
{
	OBQ_ANGULARCAMERA,
	OBQ_ATMOSPHERE,
	OBQ_BEND4STEREO,
	OBQ_CAMERAPROJECTIONSPACE,
	OBQ_CHANGERANGE,
	OBQ_COLOR,
	OBQ_COLORSPACECONVERTER,
	OBQ_ENVIRONMENT,
	OBQ_ETCHING,
	OBQ_FRESNEL,
	OBQ_HAIR,
	OBQ_IESLIGHTFILTER,
	OBQ_LENSDISTORTION,
	OBQ_LIGHTSATURATIONFILTER,
	OBQ_MEASUREDMATERIAL,
	OBQ_MESSAGEGETBOOL,
	OBQ_MESSAGEGETCOLOR,
	OBQ_MESSAGEGETFLT,
	OBQ_MESSAGEGETINT,
	OBQ_MESSAGEGETVECTOR,
	OBQ_MESSAGESETBOOL,
	OBQ_MESSAGESETCOLOR,
	OBQ_MESSAGESETFLT,
	OBQ_MESSAGESETINT,
	OBQ_MESSAGESETVECTOR,
	OBQ_MESSAGESTOREBOOL,
	OBQ_MESSAGESTORECOLOR,
	OBQ_MESSAGESTOREFLT,
	OBQ_MESSAGESTOREINT,
	OBQ_MESSAGESTOREVECTOR,
	OBQ_OPACITY,
	OBQ_RANDOMCOLOR,
	OBQ_RANDOMID,
	OBQ_RAYDEPTH,
	OBQ_RGBCOMEMIT,
	OBQ_RGBCOMLIGHTFILTER,
	OBQ_ROOT2TIP,
	OBQ_SETALPHA,
	OBQ_SHADOW,
	OBQ_SPECTRUMCOLOR,
	OBQ_STEREOLENS,
	OBQ_SURFACEENVIRONMENT,
	OBQ_THICKNESS,
	OBQ_TOON,
	OBQ_TOONSIMPLE,
	OBQ_TRANSFORMSHADINGPOINT,
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
		node->methods      = ObqAtmosphereMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Atmosphere";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_BEND4STEREO:
		node->methods      = ObqBend4StereoMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Bend4Stereo";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_CAMERAPROJECTIONSPACE:
		node->methods      = ObqCameraProjectionSpaceMethods;
		node->output_type  = AI_TYPE_VECTOR;
		node->name         = "Obq_CameraProjectionSpace";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_CHANGERANGE:
		node->methods      = ObqChangeRangeMethods;
		node->output_type  = AI_TYPE_FLOAT;
		node->name         = "Obq_ChangeRange";
		node->node_type    = AI_NODE_SHADER;
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
	case OBQ_ENVIRONMENT:
		node->methods      = ObqEnvironmentMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Environment";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_ETCHING:
		node->methods      = ObqEtchingMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Etching";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_FRESNEL:
		node->methods      = ObqFresnelMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Fresnel";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_HAIR:
		node->methods      = ObqHairMethods;
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
	case OBQ_LENSDISTORTION:
		node->methods     = ObqLensDistortionMethods;
		node->output_type = AI_TYPE_UNDEFINED;
		node->name        = "Obq_LensDistortion";
		node->node_type   = AI_NODE_CAMERA;
		break;
	case OBQ_LIGHTSATURATIONFILTER:
		node->methods      = ObqLightSaturationFilterMethod;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_LightSaturationFilter";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MEASUREDMATERIAL:
		node->methods      = ObqMeasuredMaterialMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_MeasuredMaterial";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGEGETBOOL:
		node->methods      = ObqMessageGetBoolMethods;
		node->output_type  = AI_TYPE_BOOLEAN;
		node->name         = "Obq_MessageGetBool";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGEGETCOLOR:
		node->methods      = ObqMessageGetColorMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageGetColor";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGEGETFLT:
		node->methods      = ObqMessageGetFltMethods;
		node->output_type  = AI_TYPE_FLOAT;
		node->name         = "Obq_MessageGetFlt";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGEGETINT:
		node->methods      = ObqMessageGetIntMethods;
		node->output_type  = AI_TYPE_INT;
		node->name         = "Obq_MessageGetInt";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGEGETVECTOR:
		node->methods      = ObqMessageGetVectorMethods;
		node->output_type  = AI_TYPE_VECTOR;
		node->name         = "Obq_MessageGetVector";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESETBOOL:
		node->methods      = ObqMessageSetBoolMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageSetBool";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESETCOLOR:
		node->methods      = ObqMessageSetColorMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageSetColor";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESETFLT:
		node->methods      = ObqMessageSetFltMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageSetFlt";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESETINT:
		node->methods      = ObqMessageSetIntMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageSetInt";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESETVECTOR:
		node->methods      = ObqMessageSetVectorMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageSetVector";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESTOREBOOL:
		node->methods      = ObqMessageStoreBoolMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageStoreBool";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESTORECOLOR:
		node->methods      = ObqMessageStoreColorMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageStoreColor";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESTOREFLT:
		node->methods      = ObqMessageStoreFltMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageStoreFlt";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESTOREINT:
		node->methods      = ObqMessageStoreIntMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageStoreInt";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_MESSAGESTOREVECTOR:
		node->methods      = ObqMessageStoreVectorMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_MessageStoreVector";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_OPACITY:
		node->methods      = ObqOpacityMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_Opacity";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_RANDOMCOLOR:
		node->methods      = ObqRandomColorMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_RandomColor";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_RANDOMID:
		node->methods      = ObqRandomIDMethods;
		node->output_type  = AI_TYPE_INT;
		node->name         = "Obq_RandomID";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_RAYDEPTH:
		node->methods      = ObqRayDepthMethods;
		node->output_type  = AI_TYPE_INT;
		node->name         = "Obq_RayDepth";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_RGBCOMEMIT:
		node->methods      = ObqRGBComEmitMethods;
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
		node->methods      = ObqRoot2TipMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Root2Tip";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_SETALPHA:
		node->methods      = ObqSetAlphaMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_SetAlpha";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_SHADOW:
		node->methods      = ObqShadowMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Shadow";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_SPECTRUMCOLOR:
		node->methods      = ObqSpectrumColorMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_SpectrumColor";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_STEREOLENS:
		node->methods     = ObqStereoLensMethods;
		node->output_type = AI_TYPE_UNDEFINED;
		node->name        = "Obq_StereoLens";
		node->node_type   = AI_NODE_CAMERA;
		break;
	case OBQ_SURFACEENVIRONMENT:
		node->methods      = ObqSurfaceEnvironmentMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_SurfaceEnvironment";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_THICKNESS:
		node->methods      = ObqThicknessMethods;
		node->output_type  = AI_TYPE_FLOAT;
		node->name         = "Obq_Thickness";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_TOON:
		node->methods      = ObqToonMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_Toon";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_TOONSIMPLE:
		node->methods      = ObqToonSimpleMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_ToonSimple";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_TRANSFORMSHADINGPOINT:
		node->methods      = ObqTransformShadingPointMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_TransformShadingPoint";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_TRANSFORMUV:
		node->methods      = ObqTransformUVMethods;
		node->output_type  = AI_TYPE_RGBA;
		node->name         = "Obq_TransformUV";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_UVREMAPLENSDISTORTION:
		node->methods      = ObqUVRemapLensDistortionMethods;
		node->output_type  = AI_TYPE_RGB;
		node->name         = "Obq_UVRemapLensDistortion";
		node->node_type    = AI_NODE_SHADER;
		break;
	case OBQ_VOLUMETHICKNESS:
		node->methods      = ObqVolumeThicknessMethods;
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
