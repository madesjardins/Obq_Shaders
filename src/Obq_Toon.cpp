/*
Obq_Toon :

	Toon and custom toon shader

*------------------------------------------------------------------------
Copyright (c) 2013 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)

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


#include "Obq_Toon.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqToonSimpleMethods);


node_parameters
{
	AiParameterINT("global_loopLightMode",0);
	AiParameterINT("global_compMode",1);
	AiParameterINT("global_loopMode",0);
	AiParameterBOOL("global_multByLightColor",true);

	AiParameterRGB("ambient_color",1.0f,1.0f,1.0f);
	AiParameterFLT("ambient_scale",0.0f);
	AiParameterBOOL("ambient_multAmbience",false);

	AiParameterRGB("diffuse_color",1.0f,1.0f,1.0f);
	AiParameterFLT("diffuse_scale",1.0f);
	AiParameterFLT("diffuse_coverage",1.0f);
	AiParameterFLT("diffuse_softness",1.0);
	AiParameterINT("diffuse_mode",0);
	AiParameterRGB("diffuse_gradient",1.0f,1.0f,1.0f);
	AiParameterRGB("diffuse_image",1.0f,1.0f,1.0f);
	AiParameterINT("diffuse_imageType",0);

	AiParameterRGB("highlight_color",1.0f,1.0f,1.0f);
	AiParameterFLT("highlight_scale",0.0f);
	AiParameterFLT("highlight_exponent",50.0f);
	AiParameterFLT("highlight_coverage",1.0f);
	AiParameterFLT("highlight_softness",1.0f);
	AiParameterINT("highlight_mode",0);
	AiParameterRGB("highlight_gradient",1.0f,1.0f,1.0f);
	AiParameterRGB("highlight_image",1.0f,1.0f,1.0f);
	AiParameterINT("highlight_imageType",0);

	AiParameterRGB("rimlight_color",1.0f,1.0f,1.0f);
	AiParameterFLT("rimlight_scale",0.0f);
	AiParameterFLT("rimlight_coverage",1.0f);
	AiParameterFLT("rimlight_softness",1.0f);
	AiParameterINT("rimlight_mode",0);
	AiParameterRGB("rimlight_gradient",1.0f,1.0f,1.0f);
	AiParameterRGB("rimlight_image",1.0f,1.0f,1.0f);
	AiParameterINT("rimlight_imageType",0);

	AiParameterSTR("ambient_fb_str","");
	AiParameterSTR("diffuse_fb_str","");
	AiParameterSTR("highlight_fb_str","");
	AiParameterSTR("rimlight_fb_str","");
	AiParameterSTR("contour_fb_str","");

	AiParameterBOOL("putAlphaInFb",false);
	AiParameterBOOL("diffuse_autoRemap",true);
	AiParameterFLT("diffuse_imageRemapLeft",0.0f);
	AiParameterFLT("diffuse_imageRemapRight",1.0f);
	AiParameterBOOL("highlight_autoRemap",true);
	AiParameterFLT("highlight_imageRemapLeft",.0f);
	AiParameterFLT("highlight_imageRemapRight",1.0f);
	AiParameterBOOL("rimlight_autoRemap",true);
	AiParameterFLT("rimlight_imageRemapLeft",.0f);
	AiParameterFLT("rimlight_imageRemapRight",1.0f);
	AiParameterFLT("diffuse_imageRemapBias",0.5);
	AiParameterFLT("diffuse_imageRemapGain",0.5);
	AiParameterFLT("highlight_imageRemapBias",0.5);
	AiParameterFLT("highlight_imageRemapGain",0.5);
	AiParameterFLT("rimlight_imageRemapBias",0.5);
	AiParameterFLT("rimlight_imageRemapGain",0.5);

	AiParameterBool("output_imageOnUV",false);

	AiParameterRGB("opacity",1.0f,1.0f,1.0f);

	AiParameterBool("normalizeObjectID",true);

	AiParameterBool("ambient_noInternal",true);
	AiParameterBool("diffuse_noInternal",true);
	AiParameterBool("highlight_noInternal",true);
	AiParameterBool("rimlight_noInternal",true);

	AiParameterBool("diffuse_linearDot",false);
	AiParameterBool("highlight_linearDot",false);
	AiParameterBool("rimlight_linearDot",false);

	AiParameterRGB("diffuse_shaderInput",0.0f,0.0f,0.0f);
	AiParameterRGB("highlight_shaderInput",0.0f,0.0f,0.0f);

	AiParameterINT("global_remapMode",OBQ_REMAPRGB);
	AiParameterBool("global_remapMultByColor",false);

	AiParameterBool("global_clamp",false);
	//AiParameterArray("light_list",AiArrayAllocate(0, 0, AI_TYPE_POINTER));
}

node_initialize
{
	//AiMsgInfo("-*/-*/ node_initialize -*/-*/");
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));

	data->global_loopLightMode = OBQ_LOOPLIGHTADD;
	data->global_compMode = OBQ_COMPOVER;
	data->global_loopMode = OBQ_LOOPADD;
	data->global_multByLightColor = true;

	data->ambient_multAmbience = false;

	data->diffuse_mode = OBQ_MODEPARAMS;
	data->diffuse_imageType = OBQ_IT1D;

	data->highlight_mode = OBQ_MODEPARAMS;
	data->highlight_imageType = OBQ_IT1D;

	data->rimlight_mode = OBQ_MODEPARAMS;
	data->rimlight_imageType = OBQ_IT1D;

	data->ambient_fb_str = "";
	data->diffuse_fb_str = "";
	data->highlight_fb_str = "";
	data->rimlight_fb_str = "";
	data->contour_fb_str = "";
	data->putAlphaInFb = false;
	data->output_imageOnUV = false;

	data->output_ambient_fb =	false;
	data->output_diffuse_fb =	false;
	data->output_highlight_fb = false;
	data->output_rimlight_fb =	false;
	data->output_contour_fb =	false;

	data->ambience = AI_RGB_BLACK;

	data->diffuse_autoRemap = true;
	data->highlight_autoRemap = true;
	data->rimlight_autoRemap = true;

	data->diffuse_autoRemapLeft = -AI_EPSILON;
	data->highlight_autoRemapLeft = -AI_EPSILON;
	data->rimlight_autoRemapLeft = -AI_EPSILON;

	data->diffuse_autoRemapRight = 1.0f+AI_EPSILON;
	data->highlight_autoRemapRight = 1.0f+AI_EPSILON;
	data->rimlight_autoRemapRight = 1.0f+AI_EPSILON;

	//object map for ID
	data->objectMap = new std::map<AtNode*,float>;

	data->ambient_noInternal = true;
	data->diffuse_noInternal = true;
	data->highlight_noInternal = true;
	data->rimlight_noInternal = true;

	data->diffuse_linearDot = false;
	data->highlight_linearDot = false;
	data->rimlight_linearDot = false;

	data->diffuse_doInput = false;
	data->highlight_doInput = false;

	data->global_remapMode = OBQ_REMAPRGB;
	data->global_remapMultByColor = false;

	data->global_clamp = true;

	AiNodeSetLocalData(node,data);
	
}

node_update
{
	//AiMsgInfo("-*/-*/ node_update -*/-*/");
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	data->global_loopLightMode = params[p_global_loopLightMode].INT;
	data->global_compMode = params[p_global_compMode].INT;
	data->global_loopMode = params[p_global_loopMode].INT;
	data->global_multByLightColor = params[p_global_multByLightColor].BOOL;

	data->ambient_multAmbience = params[p_ambient_multAmbience].BOOL;

	data->diffuse_mode = params[p_diffuse_mode].INT;
	data->diffuse_imageType = params[p_diffuse_imageType].INT;

	data->highlight_mode = params[p_highlight_mode].INT;
	data->highlight_imageType = params[p_highlight_imageType].INT;

	data->rimlight_mode = params[p_rimlight_mode].INT;
	data->rimlight_imageType = params[p_rimlight_imageType].INT;

	data->ambient_fb_str = params[p_ambient_fb_str].STR;
	data->diffuse_fb_str = params[p_diffuse_fb_str].STR;
	data->highlight_fb_str = params[p_highlight_fb_str].STR;
	data->rimlight_fb_str = params[p_rimlight_fb_str].STR;
	data->contour_fb_str = params[p_contour_fb_str].STR;
	data->putAlphaInFb = params[p_putAlphaInFb].BOOL;
	data->output_imageOnUV = params[p_output_imageOnUV].BOOL;


	// Get Ambience from Options
	AtNode* options = AiUniverseGetOptions();
	if(AiNodeLookUpUserParameter(options, "ambience") != NULL)
		data->ambience = AiNodeGetRGB(options, "ambience");

	data->diffuse_autoRemap = params[p_diffuse_autoRemap].BOOL;
	data->highlight_autoRemap = params[p_highlight_autoRemap].BOOL;
	data->rimlight_autoRemap = params[p_rimlight_autoRemap].BOOL;


	std::string shaderName(AiNodeGetName(node));
	size_t pos = shaderName.find(".SItoA.");
	if(pos != std::string::npos)
	{
		shaderName = shaderName.substr (0, pos);
		pos = shaderName.find_first_of(".");
		shaderName = shaderName.substr (pos+1);
		pos = shaderName.find_first_of(".");
		shaderName = shaderName.substr (pos+1);
	}

	// 2 pixel left, two pixel right
	if(data->diffuse_mode == OBQ_MODEIMAGE && data->diffuse_autoRemap)
	{
		AtUInt32 w = 128, h = 128;
		const AtNode* diffuse_image = AiNodeGetLink(node, "diffuse_image");
		if(diffuse_image!=NULL)
		{
			const char* fileName = AiNodeGetStr(diffuse_image,"SourceFileName");
			if(AiTextureGetResolution(fileName,&w,&h))
			{
				float epsilonRes = MAX(AI_EPSILON,MIN(0.5f,2.0f/static_cast<float>(w)));
				data->diffuse_autoRemapLeft = -epsilonRes;
				data->diffuse_autoRemapRight = 1.0f+epsilonRes;
			}
			else
				w = 0; 
		}
		AiMsgInfo("%s : Using diffuse image auto remap [%f, %f], (Resx = %i).", shaderName.c_str(),data->diffuse_autoRemapLeft,data->diffuse_autoRemapRight,w);
	}

	if(data->highlight_mode == OBQ_MODEIMAGE && data->highlight_autoRemap)
	{
		AtUInt32 w = 128, h = 128;
		const AtNode* highlight_image = AiNodeGetLink(node, "highlight_image");
		if(highlight_image!=NULL)
		{
			const char* fileName = AiNodeGetStr(highlight_image,"SourceFileName");
			if(AiTextureGetResolution(fileName,&w,&h))
			{
				float epsilonRes = MAX(AI_EPSILON,MIN(0.5f,2.0f/static_cast<float>(w)));
				data->highlight_autoRemapLeft = -epsilonRes;
				data->highlight_autoRemapRight = 1.0f+epsilonRes;
			}
			else
				w = 0; 
		}
		AiMsgInfo("%s : Using highlight image auto remap [%f, %f], (Resx = %i).",shaderName.c_str(),data->highlight_autoRemapLeft,data->highlight_autoRemapRight, w);
	}

	if(data->rimlight_mode == OBQ_MODEIMAGE && data->rimlight_autoRemap)
	{
		AtUInt32 w = 128, h = 128;
		const AtNode* rimlight_image = AiNodeGetLink(node, "rimlight_image");
		if(rimlight_image!=NULL)
		{
			const char* fileName = AiNodeGetStr(rimlight_image,"SourceFileName");
			if(AiTextureGetResolution(fileName,&w,&h))
			{
				float epsilonRes = MAX(AI_EPSILON,MIN(0.5f,2.0f/static_cast<float>(w)));
				data->rimlight_autoRemapLeft = -epsilonRes;
				data->rimlight_autoRemapRight = 1.0f+epsilonRes;
			}
			else
				w = 0; 
		}
		AiMsgInfo("%s : Using rimlight image auto remap [%f, %f], (Resx = %i).", shaderName.c_str(), data->rimlight_autoRemapLeft,data->rimlight_autoRemapRight,w);
	}


	// OBJECT IDs
	if(data->objectMap->size())
		data->objectMap->clear();
	AtNodeIterator* nIt = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
	float id = 0.0f;
	while (!AiNodeIteratorFinished(nIt))
		data->objectMap->insert ( std::pair<AtNode*,float>(AiNodeIteratorGetNext(nIt),++id) ); // on ++id pour avoir au moins 1 pour les objets et 0 pour le background
	AiNodeIteratorDestroy(nIt);

	if(params[p_normalizeObjectID].BOOL && id > 0.0f)
	{
		std::map<AtNode*,float>::iterator it;
		for (std::map<AtNode*,float>::iterator it=data->objectMap->begin(); it!=data->objectMap->end(); ++it)
			it->second/=id;
	}

	data->ambient_noInternal = params[p_ambient_noInternal].BOOL;
	data->diffuse_noInternal = params[p_diffuse_noInternal].BOOL;
	data->highlight_noInternal = params[p_highlight_noInternal].BOOL;
	data->rimlight_noInternal = params[p_rimlight_noInternal].BOOL;

	data->diffuse_linearDot = params[p_diffuse_linearDot].BOOL;
	data->highlight_linearDot = params[p_highlight_linearDot].BOOL;
	data->rimlight_linearDot = params[p_rimlight_linearDot].BOOL;

	// REMAP
	data->global_remapMode = params[p_global_remapMode].INT;
	data->global_remapMultByColor = params[p_global_remapMultByColor].BOOL;

	// Using shader inputs
	if(data->global_loopMode == OBQ_LOOPREMAPCURVEINPUTS)
	{
		data->diffuse_doInput = AiNodeIsLinked(node,"diffuse_shaderInput");
		data->highlight_doInput = AiNodeIsLinked(node,"highlight_shaderInput");
	}

	data->global_clamp = params[p_global_clamp].BOOL;

}

node_finish
{
	
	// Desallocate shader data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	delete data->objectMap;
	AiFree(data);
}


shader_evaluate
{
	// opacity
	sg->out_opacity = AiShaderEvalParamRGB(p_opacity);

	if(sg->Rt & AI_RAY_SHADOW)
	{
		sg->out.RGB =  AI_RGB_BLACK;
		return;
	}
	else
	{

		bool backface = ( AiV3Dot(sg->Rd,sg->N) > 0);

		// Access shader Data
		ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

		// diffuse
		bool		do_diffuse = false;
		float		diffuse_scale		=	( (backface && data->diffuse_noInternal)?0.0f:AiShaderEvalParamFlt(p_diffuse_scale));
		AtColor		diffuse_color = AI_RGB_BLACK;
		if(diffuse_scale > 0.0f)
		{
			diffuse_color	=	AiShaderEvalParamRGB(p_diffuse_color);
			do_diffuse = (diffuse_color.r > 0.0 || diffuse_color.g > 0.0 || diffuse_color.b > 0.0);
		}
		float	diffuse_coverage	= (do_diffuse?AiShaderEvalParamFlt(p_diffuse_coverage):1.0f);
		float	diffuse_softness	= (do_diffuse?AiShaderEvalParamFlt(p_diffuse_softness):1.0f);


		// highlight
		bool	do_highlight = false;
		float		highlight_scale		=	( (backface && data->highlight_noInternal)?0.0f:AiShaderEvalParamFlt(p_highlight_scale));
		AtColor		highlight_color = AI_RGB_BLACK;

		if(highlight_scale > 0.0f)
		{
			highlight_color	=	AiShaderEvalParamRGB(p_highlight_color);
			do_highlight = (highlight_color.r > 0.0 || highlight_color.g > 0.0 || highlight_color.b > 0.0);
		}
		float	highlight_coverage	= (do_highlight?AiShaderEvalParamFlt(p_highlight_coverage):1.0f);
		float	highlight_softness	= (do_highlight?AiShaderEvalParamFlt(p_highlight_softness):1.0f);
		float	highlight_exponent	= (do_highlight?AiShaderEvalParamFlt(p_highlight_exponent):50.0f);

		// rimlight
		bool		do_rimlight = false;
		float		rimlight_scale		=	( (backface && data->rimlight_noInternal)?0.0f:AiShaderEvalParamFlt(p_rimlight_scale));
		AtColor		rimlight_color = AI_RGB_BLACK;
		if(rimlight_scale > 0.0f)
		{
			rimlight_color	=	AiShaderEvalParamRGB(p_rimlight_color);
			do_rimlight = (rimlight_color.r > 0.0 || rimlight_color.g > 0.0 || rimlight_color.b > 0.0);
		}
		float	rimlight_coverage	= (do_rimlight?AiShaderEvalParamFlt(p_rimlight_coverage):1.0f);
		float	rimlight_softness	= (do_rimlight?AiShaderEvalParamFlt(p_rimlight_softness):1.0f);

		bool	do_DiffuseAndImage = (do_diffuse && data->diffuse_mode==OBQ_MODEIMAGE);
		float	diffuse_imageRemapLeft = (do_DiffuseAndImage ?(data->diffuse_autoRemap?data->diffuse_autoRemapLeft:AiShaderEvalParamFlt(p_diffuse_imageRemapLeft)):0.0f);
		float	diffuse_imageRemapRight = (do_DiffuseAndImage?(data->diffuse_autoRemap?data->diffuse_autoRemapRight:AiShaderEvalParamFlt(p_diffuse_imageRemapRight)):1.0f);
		float	diffuse_imageRemapBias = (do_DiffuseAndImage ?AiShaderEvalParamFlt(p_diffuse_imageRemapBias):0.5f);
		float	diffuse_imageRemapGain = (do_DiffuseAndImage ?1.0f-(AiShaderEvalParamFlt(p_diffuse_imageRemapGain)):0.5f);
		bool	do_diffuseRemap = (do_DiffuseAndImage?(diffuse_imageRemapLeft!=0.0f || diffuse_imageRemapRight!=1.0f || diffuse_imageRemapBias!=0.5f || diffuse_imageRemapGain != 0.5f):false);

		bool	do_HighlightAndImage = (do_highlight && data->highlight_mode==OBQ_MODEIMAGE);
		float	highlight_imageRemapLeft = (do_HighlightAndImage?(data->highlight_autoRemap?data->highlight_autoRemapLeft:AiShaderEvalParamFlt(p_highlight_imageRemapLeft)):0.0f);
		float	highlight_imageRemapRight = (do_HighlightAndImage?(data->highlight_autoRemap?data->highlight_autoRemapRight:AiShaderEvalParamFlt(p_highlight_imageRemapRight)):1.0f);
		float	highlight_imageRemapBias = (do_HighlightAndImage?AiShaderEvalParamFlt(p_highlight_imageRemapBias):0.5f);
		float	highlight_imageRemapGain = (do_HighlightAndImage?1.0f - (AiShaderEvalParamFlt(p_highlight_imageRemapGain)):0.5f);
		bool	do_highlightRemap = (do_HighlightAndImage?(highlight_imageRemapLeft!=0.0f || highlight_imageRemapRight!=1.0f || highlight_imageRemapBias!=0.5f || highlight_imageRemapGain != 0.5f):false);


		bool	do_RimlightAndImage = (do_rimlight && data->rimlight_mode==OBQ_MODEIMAGE);
		float	rimlight_imageRemapLeft = (do_RimlightAndImage ?(data->rimlight_autoRemap?data->rimlight_autoRemapLeft:AiShaderEvalParamFlt(p_rimlight_imageRemapLeft)):0.0f);
		float	rimlight_imageRemapRight = (do_RimlightAndImage?(data->rimlight_autoRemap?data->rimlight_autoRemapRight:AiShaderEvalParamFlt(p_rimlight_imageRemapRight)):1.0f);
		float	rimlight_imageRemapBias = (do_RimlightAndImage ?AiShaderEvalParamFlt(p_rimlight_imageRemapBias):0.5f);
		float	rimlight_imageRemapGain = (do_RimlightAndImage ?1.0f - (AiShaderEvalParamFlt(p_rimlight_imageRemapGain)):0.5f);
		bool	do_rimlightRemap = (do_RimlightAndImage?(rimlight_imageRemapLeft!=0.0f || rimlight_imageRemapRight!=1.0f || rimlight_imageRemapBias!=0.5f || rimlight_imageRemapGain != 0.5f):false);
				
		float sgu = sg->u;
		float sgv = sg->v;
		
		// TEST for render map output 
		if(data->output_imageOnUV)
		{

			switch(data->diffuse_mode)
			{
			case OBQ_MODEIMAGE:
				{
					AtVector	texCoords;
					inode_image_make_coord(texCoords, sg->u, 0.0f, data->diffuse_imageType,do_diffuseRemap,diffuse_imageRemapLeft,diffuse_imageRemapRight,diffuse_imageRemapBias,diffuse_imageRemapGain); // calculate coordinate
					sg->u = texCoords.x;
					sg->v = texCoords.y;
					sg->out.RGB = AiShaderEvalParamRGB(p_diffuse_image);//fetch color at that coordonate
					sg->u = sgu;
					sg->v = sgv;
					break;
				}
			case OBQ_MODEGRADIENT: // IF you render map a gradient, you'll have to add one pixel of the same color on each side of the image
				AiStateSetMsgFlt(c_ToonGradientMessage, sg->u);
				sg->out.RGB = AiShaderEvalParamRGB(p_diffuse_gradient);
				break;
			default:
				break;
			}
			return;
		}

		AtColor sum_tex=AI_RGB_BLACK;

		AtColor		texColor;
		AtVector	texCoords;

		AtVector	Nf = sg->Nf,
					V = -sg->Rd; // Useful vectors

		//AtColor diffuse_sum_color=AI_RGB_BLACK;		// final sum
		AtColor diffuse_loop_color=AI_RGB_BLACK;		// use for the light loop
		AtColor diffuse_sample_color=AI_RGB_BLACK;	// use for the sample loop

		//AtColor highlight_sum_color=AI_RGB_BLACK;	// final sum
		AtColor highlight_loop_color=AI_RGB_BLACK;	// use for the light loop
		AtColor highlight_sample_color=AI_RGB_BLACK;	// use for the sample loop

		//initialize result
		sg->out.RGB = AI_RGB_BLACK;

		// We need to sample lights
		if(do_diffuse || do_highlight)
		{

			///////////////////
			// LOOPMODE
			switch(data->global_loopMode)
			{
			//-------------------
			// LOOPADD
			case OBQ_LOOPADD:
				{

					AiLightsPrepare(sg);

					while(AiLightsGetSample(sg)){

						if(do_diffuse && AiLightGetAffectDiffuse(sg->Lp))
						{
							float NdotL = AiV3Dot(Nf,sg->Ld);

							if(data->diffuse_linearDot) NdotL = 1.0f-acos(CLAMP(NdotL,0.0f,1.0f))/c_PiOver2; // Linearize

							float dot =  inode_dot_modify(NdotL, diffuse_coverage, diffuse_softness);

							switch(data->diffuse_mode)
							{
							case OBQ_MODEIMAGE:
								inode_image_make_coord(texCoords, dot, 0.0f, data->diffuse_imageType,do_diffuseRemap,diffuse_imageRemapLeft,diffuse_imageRemapRight,diffuse_imageRemapBias,diffuse_imageRemapGain); // calculate coordinate
								sg->u = texCoords.x;
								sg->v = texCoords.y;
								diffuse_sample_color = AiShaderEvalParamRGB(p_diffuse_image);//fetch color at that coordonate
								sg->u = sgu;
								sg->v = sgv;
								break;
							case OBQ_MODEGRADIENT:
								AiStateSetMsgFlt(c_ToonGradientMessage, dot);
								diffuse_sample_color = AiShaderEvalParamRGB(p_diffuse_gradient);
								break;
							default:
								diffuse_sample_color = dot;
								break;
							}

							// multiply by light color ?
							if(data->global_multByLightColor)
								diffuse_sample_color*=sg->Li;

							// sum for this sample
							diffuse_loop_color+= diffuse_sample_color*sg->we;

						}

						if(do_highlight && AiLightGetAffectSpecular(sg->Lp))
						{
							// NdotH
							AtVector H = sg->Ld+V;
							H = AiV3Normalize(H);

							float NdotH =AiV3Dot(Nf,H);

							if(data->highlight_linearDot) NdotH = 1.0f-acos(CLAMP(NdotH,0.0f,1.0f))/c_PiOver2; // Linearize

							float dot =  inode_dot_modify(NdotH, highlight_coverage, highlight_softness, highlight_exponent);

							switch(data->highlight_mode)
							{
							case OBQ_MODEIMAGE:
								inode_image_make_coord(texCoords, dot, 0.0f, data->highlight_imageType,do_highlightRemap,highlight_imageRemapLeft,highlight_imageRemapRight,highlight_imageRemapBias,highlight_imageRemapGain);		// calculate coordinate
								sg->u = texCoords.x;
								sg->v = texCoords.y;
								highlight_sample_color = AiShaderEvalParamRGB(p_highlight_image);//fetch color at that coordonate
								sg->u = sgu;
								sg->v = sgv;
								break;
							case OBQ_MODEGRADIENT:
								AiStateSetMsgFlt(c_ToonGradientMessage, dot);
								highlight_sample_color = AiShaderEvalParamRGB(p_highlight_gradient);
								break;
							default:
								highlight_sample_color = dot;
								break;
							}

							// multiply by light color ?
							if(data->global_multByLightColor)
								highlight_sample_color*= sg->Li;

							highlight_loop_color+=highlight_sample_color*sg->we;
						}
					}
					break;
				}
			//-------------------
			// LOOPREMAPCURVE
			case OBQ_LOOPREMAPCURVE:
				{

					// loop on each light
					AiLightsPrepare(sg);

					while(AiLightsGetSample(sg))
					{

						if(do_diffuse && AiLightGetAffectDiffuse(sg->Lp))
						{
							float NdotL = AiV3Dot(Nf,sg->Ld);

							float dot =  inode_dot_modify(NdotL, diffuse_coverage, diffuse_softness);

							if(data->diffuse_linearDot) NdotL = 1.0f- acos(CLAMP(NdotL,0.0f,1.0f))/c_PiOver2; // Linearize

							diffuse_sample_color = dot;

							// multiply by light color ?
							if(data->global_multByLightColor)
								diffuse_sample_color*=sg->Li;

							// sum for this sample
							diffuse_loop_color+=diffuse_sample_color*sg->we;

						}

						if(do_highlight && AiLightGetAffectSpecular(sg->Lp))
						{
							// NdotH
							AtVector H = sg->Ld+V;
							H = AiV3Normalize(H);

							float NdotH =AiV3Dot(Nf,H);

							if(data->highlight_linearDot) NdotH = 1.0f-acos(CLAMP(NdotH,0.0f,1.0f))/c_PiOver2; // Linearize

							float dot =  inode_dot_modify(NdotH, highlight_coverage, highlight_softness, highlight_exponent);
							highlight_sample_color = dot;

							// multiply by light color ?
							if(data->global_multByLightColor)
								highlight_sample_color*=sg->Li;

							// sum for this sample	
							highlight_loop_color+= highlight_sample_color*sg->we;
						}
					}

					// DIFFUSE
					if(do_diffuse)
					{
						if(data->global_remapMode == OBQ_REMAPRGB)
						{
							switch(data->diffuse_mode)
							{
							case OBQ_MODEIMAGE:
								//pour chaque canal
								for(AtUInt32 i = 0; i < 3; ++i)
								{
									inode_image_make_coord(texCoords, diffuse_loop_color[i], 0.0f, data->diffuse_imageType,do_diffuseRemap,diffuse_imageRemapLeft,diffuse_imageRemapRight,diffuse_imageRemapBias,diffuse_imageRemapGain); // calculate coordinate
									sg->u = texCoords.x;
									sg->v = texCoords.y;
									texColor = AiShaderEvalParamRGB(p_diffuse_image);//fetch color at that coordonate
									sg->u = sgu;
									sg->v = sgv;
									diffuse_loop_color[i] = texColor[i];
								}
								break;
							case OBQ_MODEGRADIENT:
								for(AtUInt32 i = 0; i < 3; ++i)
								{
									AiStateSetMsgFlt(c_ToonGradientMessage, diffuse_loop_color[i]);
									texColor = AiShaderEvalParamRGB(p_diffuse_gradient);
									diffuse_loop_color[i] = texColor[i];
								}
								break;
							default:
								break;
							}
						}
						else
						{
							// AVERAGE OR LUMINANCE REMAP
							AtColor input = diffuse_loop_color;

							float dot = (data->global_remapMode == OBQ_REMAPAVERAGE ? (input.r+input.g+input.b)/3.0f:getLuminance(input));
							
							switch(data->diffuse_mode)
							{
							case OBQ_MODEIMAGE:
								inode_image_make_coord(texCoords, dot, 0.0f, data->diffuse_imageType,do_diffuseRemap,diffuse_imageRemapLeft,diffuse_imageRemapRight,diffuse_imageRemapBias,diffuse_imageRemapGain); // calculate coordinate
								sg->u = texCoords.x;
								sg->v = texCoords.y;
								diffuse_loop_color = AiShaderEvalParamRGB(p_diffuse_image);//fetch color at that coordonate
								sg->u = sgu;
								sg->v = sgv;
								break;
							case OBQ_MODEGRADIENT:
								AiStateSetMsgFlt(c_ToonGradientMessage, dot);
								diffuse_loop_color = AiShaderEvalParamRGB(p_diffuse_gradient);
								break;
							default:
								diffuse_loop_color = dot;
								break;
							}

							// multiply by normalized color ?
							if(data->global_remapMultByColor)
							{
								AtColor hsv;
								RGBtoHSV(input,hsv);
								hsv.b = 1.0;
								AtColor rgb;
								HSVtoRGB(hsv,rgb);
								diffuse_loop_color*=rgb;
							}
						}
					}

					// HIGHLIGHT
					if(do_highlight)
					{
						if(data->global_remapMode == OBQ_REMAPRGB)
						{
							switch(data->highlight_mode)
							{
							case OBQ_MODEIMAGE:
								for(AtUInt32 i = 0; i < 3; ++i)
								{
									inode_image_make_coord(texCoords, highlight_loop_color[i], 0.0f, data->highlight_imageType,do_highlightRemap,highlight_imageRemapLeft,highlight_imageRemapRight,highlight_imageRemapBias,highlight_imageRemapGain);		// calculate coordinate
									sg->u = texCoords.x;
									sg->v = texCoords.y;
									texColor = AiShaderEvalParamRGB(p_highlight_image);//fetch color at that coordonate
									sg->u = sgu;
									sg->v = sgv;
									highlight_loop_color[i] = texColor[i];
								}
								break;
							case OBQ_MODEGRADIENT:
								for(AtUInt32 i = 0; i < 3; ++i)
								{
									AiStateSetMsgFlt(c_ToonGradientMessage, highlight_loop_color[i]);
									texColor = AiShaderEvalParamRGB(p_highlight_gradient);
									highlight_loop_color[i] = texColor[i];
								}
								break;
							default:
								break;
							}
						}
						else
						{
							// AVERAGE OR LUMINANCE REMAP
							AtColor input = highlight_loop_color;
							float dot = (data->global_remapMode == OBQ_REMAPAVERAGE ? (input.r+input.g+input.b)/3.0f:getLuminance(input));

							switch(data->highlight_mode)
							{
							case OBQ_MODEIMAGE:
								inode_image_make_coord(texCoords, dot, 0.0f, data->highlight_imageType,do_highlightRemap,highlight_imageRemapLeft,highlight_imageRemapRight,highlight_imageRemapBias,highlight_imageRemapGain);		// calculate coordinate
								sg->u = texCoords.x;
								sg->v = texCoords.y;
								highlight_loop_color = AiShaderEvalParamRGB(p_highlight_image);//fetch color at that coordonate
								sg->u = sgu;
								sg->v = sgv;
								break;
							case OBQ_MODEGRADIENT:
								AiStateSetMsgFlt(c_ToonGradientMessage, dot);
								highlight_loop_color = AiShaderEvalParamRGB(p_highlight_gradient);
								break;
							default:
								highlight_loop_color = dot;
								break;
							}

							// multiply by normalized color ?
							if(data->global_remapMultByColor)
							{
								AtColor hsv;
								RGBtoHSV(input,hsv);
								hsv.b = 1.0;
								AtColor rgb;
								HSVtoRGB(hsv,rgb);
								highlight_loop_color*=rgb;
							}
						}
					}
					break;
				}
			//-------------------
			// LOOP REMAP CURVE INPUTS
			case OBQ_LOOPREMAPCURVEINPUTS:
				{
					// DIFFUSE
					if(do_diffuse && data->diffuse_doInput)
					{
						// SHADER INPUT MODE
						if(data->global_remapMode == OBQ_REMAPRGB)
						{
							// Get Input
							diffuse_loop_color = AiColorClamp(AiShaderEvalParamRGB(p_diffuse_shaderInput),0.0f,1.0f);

							switch(data->diffuse_mode)
							{
							case OBQ_MODEIMAGE:
								// for each channel
								for(AtUInt32 i = 0; i < 3; ++i)
								{
									// Linearize
									if(data->highlight_linearDot) diffuse_loop_color[i] = 1.0f-acos(diffuse_loop_color[i])/c_PiOver2; 
									
									// REMAP
									diffuse_loop_color[i] =  inode_dot_modify(diffuse_loop_color[i], diffuse_coverage, diffuse_softness);

									inode_image_make_coord(texCoords, diffuse_loop_color[i], 0.0f, data->diffuse_imageType,do_diffuseRemap,diffuse_imageRemapLeft,diffuse_imageRemapRight,diffuse_imageRemapBias,diffuse_imageRemapGain); // calculate coordinate
									sg->u = texCoords.x;
									sg->v = texCoords.y;
									texColor = AiShaderEvalParamRGB(p_diffuse_image);//fetch color at that coordonate
									sg->u = sgu;
									sg->v = sgv;
									diffuse_loop_color[i] = texColor[i];
								}
								break;
							case OBQ_MODEGRADIENT:
								for(AtUInt32 i = 0; i < 3; ++i)
								{
									// Linearize
									if(data->highlight_linearDot) diffuse_loop_color[i] = 1.0f-acos(diffuse_loop_color[i])/c_PiOver2; 
									
									// REMAP
									diffuse_loop_color[i] =  inode_dot_modify(diffuse_loop_color[i], diffuse_coverage, diffuse_softness);

									AiStateSetMsgFlt(c_ToonGradientMessage, diffuse_loop_color[i]);
									texColor = AiShaderEvalParamRGB(p_diffuse_gradient);
									diffuse_loop_color[i] = texColor[i];
								}
								break;
							default:
								break;
							}
						}
						else
						{
							AtColor input = AiShaderEvalParamRGB(p_diffuse_shaderInput);
							float dot = CLAMP( (data->global_remapMode == OBQ_REMAPAVERAGE ? (input.r+input.g+input.b)/3.0f:getLuminance(input) ),0.0f,1.0f);
							
							if(data->diffuse_linearDot) dot = 1.0f-acos(dot)/c_PiOver2; // Linearize

							dot =  inode_dot_modify(dot, diffuse_coverage, diffuse_softness);

							switch(data->diffuse_mode)
							{
							case OBQ_MODEIMAGE:
								inode_image_make_coord(texCoords, dot, 0.0f, data->diffuse_imageType,do_diffuseRemap,diffuse_imageRemapLeft,diffuse_imageRemapRight,diffuse_imageRemapBias,diffuse_imageRemapGain); // calculate coordinate
								sg->u = texCoords.x;
								sg->v = texCoords.y;
								diffuse_loop_color = AiShaderEvalParamRGB(p_diffuse_image);//fetch color at that coordonate
								sg->u = sgu;
								sg->v = sgv;
								break;
							case OBQ_MODEGRADIENT:
								AiStateSetMsgFlt(c_ToonGradientMessage, dot);
								diffuse_loop_color = AiShaderEvalParamRGB(p_diffuse_gradient);
								break;
							default:
								diffuse_loop_color = dot;
								break;
							}

							// multiply by normalized color ?
							if(data->global_remapMultByColor)
							{
								AtColor hsv;
								RGBtoHSV(input,hsv);
								hsv.b = 1.0;
								AtColor rgb;
								HSVtoRGB(hsv,rgb);
								diffuse_loop_color*=rgb;
							}
						}
					}

					// Highlight
					if(do_highlight && data->highlight_doInput)
					{
						// SHADER INPUT MODE
						if(data->global_remapMode == OBQ_REMAPRGB)
						{
							// Get Color
							highlight_loop_color = AiColorClamp(AiShaderEvalParamRGB(p_highlight_shaderInput),0.0f,1.0f);

							switch(data->highlight_mode)
							{
							case OBQ_MODEIMAGE:
								for(AtUInt32 i = 0; i < 3; ++i)
								{
									if(data->highlight_linearDot) highlight_loop_color[i] = 1.0f-acos(highlight_loop_color[i])/c_PiOver2; // Linearize

									highlight_loop_color[i] =  inode_dot_modify(highlight_loop_color[i], highlight_coverage, highlight_softness, highlight_exponent);

									inode_image_make_coord(texCoords, highlight_loop_color[i], 0.0f, data->highlight_imageType,do_highlightRemap,highlight_imageRemapLeft,highlight_imageRemapRight,highlight_imageRemapBias,highlight_imageRemapGain);		// calculate coordinate
									sg->u = texCoords.x;
									sg->v = texCoords.y;
									texColor = AiShaderEvalParamRGB(p_highlight_image);//fetch color at that coordonate
									sg->u = sgu;
									sg->v = sgv;
									highlight_loop_color[i] = texColor[i];
								}
								break;
							case OBQ_MODEGRADIENT:
								for(AtUInt32 i = 0; i < 3; ++i)
								{
									if(data->highlight_linearDot) highlight_loop_color[i] = 1.0f-acos(highlight_loop_color[i])/c_PiOver2; // Linearize

									highlight_loop_color[i] =  inode_dot_modify(highlight_loop_color[i], highlight_coverage, highlight_softness, highlight_exponent);

									AiStateSetMsgFlt(c_ToonGradientMessage, highlight_loop_color[i]);
									texColor = AiShaderEvalParamRGB(p_highlight_gradient);
									highlight_loop_color[i] = texColor[i];
								}
								break;
							default:
								break;
							}
						}
						else
						{
							AtColor input = AiShaderEvalParamRGB(p_highlight_shaderInput);
							float dot = CLAMP( (data->global_remapMode == OBQ_REMAPAVERAGE ? (input.r+input.g+input.b)/3.0f:getLuminance(input) ),0.0f,1.0f);
							dot =  inode_dot_modify(dot, highlight_coverage, highlight_softness, highlight_exponent);

							switch(data->highlight_mode)
							{
							case OBQ_MODEIMAGE:
								inode_image_make_coord(texCoords, dot, 0.0f, data->highlight_imageType,do_highlightRemap,highlight_imageRemapLeft,highlight_imageRemapRight,highlight_imageRemapBias,highlight_imageRemapGain);		// calculate coordinate
								sg->u = texCoords.x;
								sg->v = texCoords.y;
								highlight_loop_color = AiShaderEvalParamRGB(p_highlight_image);//fetch color at that coordonate
								sg->u = sgu;
								sg->v = sgv;
								break;
							case OBQ_MODEGRADIENT:
								AiStateSetMsgFlt(c_ToonGradientMessage, dot);
								highlight_loop_color = AiShaderEvalParamRGB(p_highlight_gradient);
								break;
							default:
								highlight_loop_color = dot;
								break;
							}

							// multiply by normalized color ?
							if(data->global_remapMultByColor)
							{
								AtColor hsv;
								RGBtoHSV(input,hsv);
								hsv.b = 1.0;
								AtColor rgb;
								HSVtoRGB(hsv,rgb);
								highlight_loop_color*=rgb;
							}
						}
					}
					break;
				}
			//--------------
			// USE MODES
			default :
				{
					//float diffuse_dot_sum = 0.0f;
					//float highlight_dot_sum = 0.0f;

					//AtColor lightColor_sum = AI_RGB_BLACK;
					AtColor lightColor_loop;
					if(data->global_loopLightMode == OBQ_LOOPLIGHTMULT)
						lightColor_loop = AI_RGB_WHITE;
					else
						lightColor_loop = AI_RGB_BLACK;

					float diffuse_dot_loop = 0.0f;
					float highlight_dot_loop = 0.0f;


					AiLightsPrepare(sg);

					// loop on each light
					while(AiLightsGetSample(sg))
					{

						float diffuse_dot_sample = 0.0f;

						if(do_diffuse && AiLightGetAffectDiffuse(sg->Lp))
						{
							float NdotL = AiV3Dot(Nf,sg->Ld);

							if(data->diffuse_linearDot) NdotL = 1.0f-acos(CLAMP(NdotL,0.0f,1.0f))/c_PiOver2; // Linearize

							diffuse_dot_sample = inode_dot_modify(NdotL, diffuse_coverage, diffuse_softness)*sg->we;
						}

						float highlight_dot_sample = 0.0f;
						if(do_highlight && AiLightGetAffectSpecular(sg->Lp))
						{
							// NdotH
							AtVector H = sg->Ld+V;
							H = AiV3Normalize(H);

							float NdotH =AiV3Dot(Nf,H);

							if(data->highlight_linearDot) NdotH = 1.0f-acos(CLAMP(NdotH,0.0f,1.0f))/c_PiOver2; // Linearize

							highlight_dot_sample =  inode_dot_modify(NdotH, highlight_coverage, highlight_softness, highlight_exponent)*sg->we;
						}

						//Qu'est-ce qu'on fait avec les lumières ? It's hard to know if we should *sg->we
						if(data->global_multByLightColor && (AiLightGetAffectDiffuse(sg->Lp) || AiLightGetAffectSpecular(sg->Lp)))
						{
							switch(data->global_loopLightMode)
							{
							case OBQ_LOOPLIGHTADD:
								lightColor_loop+=sg->Li*sg->we;
								break;
							case OBQ_LOOPLIGHTADDCLAMPED:
								lightColor_loop.r = CLAMP(lightColor_loop.r+sg->Li.r*sg->we,0.0f,1.0f);
								lightColor_loop.g = CLAMP(lightColor_loop.g+sg->Li.g*sg->we,0.0f,1.0f);
								lightColor_loop.b = CLAMP(lightColor_loop.b+sg->Li.b*sg->we,0.0f,1.0f);
								break;
							case OBQ_LOOPLIGHTMAX:
								lightColor_loop.r = MAX(lightColor_loop.r,sg->Li.r);
								lightColor_loop.g = MAX(lightColor_loop.g,sg->Li.g);
								lightColor_loop.b = MAX(lightColor_loop.b,sg->Li.b);
								break;
							default:
								lightColor_loop*=sg->Li*sg->we;
								break;
							}
						}

						// What do we do with the dots ?
						if(data->global_loopMode == OBQ_LOOPUSEADD)
						{
							// What to do with Lights ? * + max ?
							if(do_diffuse)	diffuse_dot_loop+=diffuse_dot_sample;
							if(do_highlight)highlight_dot_loop+=highlight_dot_sample;
						}
						else
						{
							if(do_diffuse)	diffuse_dot_loop = MAX(diffuse_dot_loop,diffuse_dot_sample);
							if(do_highlight)highlight_dot_loop = MAX(highlight_dot_loop,highlight_dot_sample);
						}


					}

					// End of light loop
					if(do_diffuse)
					{
						float dot = CLAMP(diffuse_dot_loop,0.0f,1.0f);
						switch(data->diffuse_mode)
						{
						case OBQ_MODEIMAGE:
							inode_image_make_coord(texCoords, dot, 0.0f, data->diffuse_imageType,do_diffuseRemap,diffuse_imageRemapLeft,diffuse_imageRemapRight,diffuse_imageRemapBias,diffuse_imageRemapGain); // calculate coordinate
							sg->u = texCoords.x;
							sg->v = texCoords.y;
							diffuse_loop_color = AiShaderEvalParamRGB(p_diffuse_image);//fetch color at that coordonate
							sg->u = sgu;
							sg->v = sgv;
							break;
						case OBQ_MODEGRADIENT:
							AiStateSetMsgFlt(c_ToonGradientMessage, dot);
							diffuse_loop_color = AiShaderEvalParamRGB(p_diffuse_gradient);
							break;
						default:
							diffuse_loop_color = dot;
							break;
						}

						// multiply by light color ?
						if(data->global_multByLightColor)
							diffuse_loop_color*=lightColor_loop;
					}

					if(do_highlight)
					{
						float dot = CLAMP(highlight_dot_loop,0.0f,1.0f);
						switch(data->highlight_mode)
						{
						case OBQ_MODEIMAGE:
							inode_image_make_coord(texCoords, dot, 0.0f, data->highlight_imageType,do_highlightRemap,highlight_imageRemapLeft,highlight_imageRemapRight,highlight_imageRemapBias,highlight_imageRemapGain); // calculate coordinate
							sg->u = texCoords.x;
							sg->v = texCoords.y;
							highlight_loop_color = AiShaderEvalParamRGB(p_highlight_image);//fetch color at that coordonate
							sg->u = sgu;
							sg->v = sgv;
							break;
						case OBQ_MODEGRADIENT:
							AiStateSetMsgFlt(c_ToonGradientMessage, dot);
							highlight_loop_color = AiShaderEvalParamRGB(p_highlight_gradient);
							break;
						default:
							highlight_loop_color = dot;
							break;
						}

						// multiply by light color ?
						if(data->global_multByLightColor)
							highlight_loop_color*=lightColor_loop;
					}
					break;
				}
			}


			if(do_diffuse)
				diffuse_loop_color*= (diffuse_color*diffuse_scale);

			if(do_highlight)
				highlight_loop_color*= (highlight_color*highlight_scale);

		}

		
		AtColor		ambient_color		= AiShaderEvalParamRGB(p_ambient_color);
		float		ambient_scale		= AiShaderEvalParamFlt(p_ambient_scale);


		////////////////////////
		// SWITCH ON COMP MODE
		///////////////////////



		if(ambient_scale > 0.0f && !AiColorIsZero(ambient_color) && !(backface && data->ambient_noInternal))
		{
			AtColor tmp = ambient_color*ambient_scale*(data->ambient_multAmbience?data->ambience:AI_RGB_WHITE);
			AtRGBA ambient_sum_color = AiRGBtoRGBA((data->global_clamp?AiColorClamp(tmp,0.0f,1.0f):tmp));

			ambient_sum_color.a = 1.0f;//CLAMP(AiColorMaxRGB(tmp),0.0f,1.0f); // Always 1

			sg->out.RGB += AiRGBAtoRGB(ambient_sum_color);

			if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->ambient_fb_str, AI_TYPE_RGB))
			{
				//if(!data->putAlphaInFb)
				//	ambient_sum_color.a = 1.0f;
				AiAOVSetRGBA (sg, data->ambient_fb_str, ambient_sum_color);
			}
		}

		if(do_diffuse)
		{	
			if(data->global_clamp)
				diffuse_loop_color = AiColorClamp(diffuse_loop_color,0.0f,1.0f);

			AtRGBA diffuse_sum_color = AiRGBtoRGBA(diffuse_loop_color);
			diffuse_sum_color.a = CLAMP(AiColorMaxRGB(diffuse_loop_color),0.0f,1.0f);

			switch(data->global_compMode)
			{
			case OBQ_COMPOVER:
				sg->out.RGB.r = diffuse_sum_color.r + (1.0f-diffuse_sum_color.a)*sg->out.RGB.r;
				sg->out.RGB.g = diffuse_sum_color.g + (1.0f-diffuse_sum_color.a)*sg->out.RGB.g;
				sg->out.RGB.b = diffuse_sum_color.b + (1.0f-diffuse_sum_color.a)*sg->out.RGB.b;
				break;
			case OBQ_COMPSCREEN: //A+B-AB (if A and B [0,1], else A if A>B, else B)
				{
					for(AtUInt32 ch = 0; ch < 3; ++ch)
					{
						float max = MAX(sg->out.RGB[ch],diffuse_sum_color[ch]);
						float min = MIN(sg->out.RGB[ch],diffuse_sum_color[ch]);
						if(max>=0.0f && min <= 1.0f)
							sg->out.RGB[ch] = diffuse_sum_color[ch] + sg->out.RGB[ch] - diffuse_sum_color[ch]*sg->out.RGB[ch];
						else
							sg->out.RGB[ch] = max;
					}
					break;
				}
			case OBQ_COMPADD:
				sg->out.RGB+= AiRGBAtoRGB(diffuse_sum_color);
				break;
			default:
				sg->out.RGB.r = MAX(diffuse_sum_color.r,sg->out.RGB.r);
				sg->out.RGB.g = MAX(diffuse_sum_color.g,sg->out.RGB.g);
				sg->out.RGB.b = MAX(diffuse_sum_color.b,sg->out.RGB.b);
				break;
			}

			if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->diffuse_fb_str, AI_TYPE_RGB))
			{
				if(!data->putAlphaInFb)
					diffuse_sum_color.a = 1.0f;
				AiAOVSetRGBA (sg, data->diffuse_fb_str, diffuse_sum_color);
			}
		}

		if(do_highlight) 
		{
			if(data->global_clamp)
			{
				sg->out.RGB = AiColorClamp(sg->out.RGB,0.0f,1.0f);
				highlight_loop_color = AiColorClamp(highlight_loop_color,0.0f,1.0f);
			}

			AtRGBA highlight_sum_color = AiRGBtoRGBA(highlight_loop_color);
			highlight_sum_color.a = CLAMP(AiColorMaxRGB(highlight_loop_color),0.0f,1.0f);
			switch(data->global_compMode)
			{
			case OBQ_COMPOVER:
				sg->out.RGB.r = highlight_sum_color.r + (1.0f-highlight_sum_color.a)*sg->out.RGB.r;
				sg->out.RGB.g = highlight_sum_color.g + (1.0f-highlight_sum_color.a)*sg->out.RGB.g;
				sg->out.RGB.b = highlight_sum_color.b + (1.0f-highlight_sum_color.a)*sg->out.RGB.b;
				break;
			case OBQ_COMPSCREEN: //A+B-AB (if A and B [0,1], else A if A>B, else B)
				{
					for(AtUInt32 ch = 0; ch < 3; ++ch)
					{
						float max = MAX(sg->out.RGB[ch],highlight_sum_color[ch]);
						float min = MIN(sg->out.RGB[ch],highlight_sum_color[ch]);
						if(max>=0.0f && min <= 1.0f)
							sg->out.RGB[ch] = highlight_sum_color[ch] + sg->out.RGB[ch] - highlight_sum_color[ch]*sg->out.RGB[ch];
						else
							sg->out.RGB[ch] = max;
					}
					break;
				}
			case OBQ_COMPADD:
				sg->out.RGB+=AiRGBAtoRGB(highlight_sum_color);
				break;
			default:
				sg->out.RGB.r = MAX(highlight_sum_color.r,sg->out.RGB.r);
				sg->out.RGB.g = MAX(highlight_sum_color.g,sg->out.RGB.g);
				sg->out.RGB.b = MAX(highlight_sum_color.b,sg->out.RGB.b);
				break;
			}

			if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->highlight_fb_str, AI_TYPE_RGB))
			{
				if(!data->putAlphaInFb)
					highlight_sum_color.a = 1.0f;
				AiAOVSetRGBA (sg, data->highlight_fb_str, highlight_sum_color);
			}
		}

		if(do_rimlight)
		{
			AtColor rimlight_loop_color;

			float NdotV = inode_dot_modify(AiV3Dot(Nf,V), rimlight_coverage, rimlight_softness, 1.0f,true);

			if(data->rimlight_linearDot) NdotV = acos(1.0f-CLAMP(NdotV,0.0f,1.0f))/c_PiOver2; // Linearize
			
			switch(data->rimlight_mode)
			{
			case OBQ_MODEIMAGE:
				inode_image_make_coord(texCoords, NdotV, 0.0f, data->rimlight_imageType,do_rimlightRemap,rimlight_imageRemapLeft,rimlight_imageRemapRight,rimlight_imageRemapBias,rimlight_imageRemapGain); // calculate coordinate
				sg->u = texCoords.x;
				sg->v = texCoords.y;
				rimlight_loop_color = AiShaderEvalParamRGB(p_rimlight_image);//fetch color at that coordonate
				sg->u = sgu;
				sg->v = sgv;
				break;
			case OBQ_MODEGRADIENT:
				AiStateSetMsgFlt(c_ToonGradientMessage, NdotV);
				rimlight_loop_color = AiShaderEvalParamRGB(p_rimlight_gradient);
				break;
			default:
				rimlight_loop_color = NdotV;
				break;
			}
			rimlight_loop_color *= (rimlight_color*rimlight_scale);

			if(data->global_clamp)
			{
				sg->out.RGB = AiColorClamp(sg->out.RGB,0.0f,1.0f);
				rimlight_loop_color = AiColorClamp(rimlight_loop_color,0.0f,1.0f);
			}

			AtRGBA rimlight_sum_color = AiRGBtoRGBA(rimlight_loop_color);
			rimlight_sum_color.a = CLAMP(AiColorMaxRGB(rimlight_loop_color),0.0f,1.0f);

			switch(data->global_compMode)
			{
			case OBQ_COMPOVER:
				sg->out.RGB.r = rimlight_sum_color.r + (1.0f-rimlight_sum_color.a)*sg->out.RGB.r;
				sg->out.RGB.g = rimlight_sum_color.g + (1.0f-rimlight_sum_color.a)*sg->out.RGB.g;
				sg->out.RGB.b = rimlight_sum_color.b + (1.0f-rimlight_sum_color.a)*sg->out.RGB.b;
				break;
			case OBQ_COMPSCREEN: //A+B-AB (if A and B [0,1], else A if A>B, else B)
				{
					for(AtUInt32 ch = 0; ch < 3; ++ch)
					{
						float max = MAX(sg->out.RGB[ch],rimlight_sum_color[ch]);
						float min = MIN(sg->out.RGB[ch],rimlight_sum_color[ch]);
						if(max>=0.0f && min <= 1.0f)
							sg->out.RGB[ch] = rimlight_sum_color[ch] + sg->out.RGB[ch] - rimlight_sum_color[ch]*sg->out.RGB[ch];
						else
							sg->out.RGB[ch] = max;
					}
					break;
				}
			case OBQ_COMPADD:
				sg->out.RGB += AiRGBAtoRGB(rimlight_sum_color);
				break;
			default:
				sg->out.RGB.r = MAX(rimlight_sum_color.r,sg->out.RGB.r);
				sg->out.RGB.g = MAX(rimlight_sum_color.g,sg->out.RGB.g);
				sg->out.RGB.b = MAX(rimlight_sum_color.b,sg->out.RGB.b);
				break;
			}

			if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->rimlight_fb_str, AI_TYPE_RGB))
			{
				if(!data->putAlphaInFb)
					rimlight_sum_color.a = 1.0f;
				AiAOVSetRGBA (sg, data->rimlight_fb_str, rimlight_sum_color);
			}
		}

		if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->contour_fb_str, AI_TYPE_RGB))
		{
			AtRGBA contour_color;

			// Ray length
			contour_color.r = static_cast<float>(sg->Rl);
			
			// Polygon ID
			contour_color.g = static_cast<float>(sg->fi);

			// Object ID
			contour_color.b = (*data->objectMap)[sg->Op];
			
			// Incidence
			AtVector tmp = -sg->Rd;
			contour_color.a = AiV3Dot(tmp,sg->Nf);

			AiAOVSetRGBA (sg, data->contour_fb_str, contour_color);
		}
	}
}
