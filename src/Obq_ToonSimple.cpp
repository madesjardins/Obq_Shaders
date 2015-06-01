/*
Obq_ToonSimple :

	Simple Toon shader

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


#include "Obq_ToonSimple.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqToonSimpleMethods);


node_parameters
{
	AiParameterINT("global_compMode",1);
	AiParameterBOOL("global_multByLightColor",true);

	AiParameterRGB("ambient_color",1.0f,1.0f,1.0f);
	AiParameterFLT("ambient_scale",0.0f);
	AiParameterBOOL("ambient_multAmbience",false);

	AiParameterRGB("diffuse_color",1.0f,1.0f,1.0f);
	AiParameterFLT("diffuse_scale",1.0f);
	AiParameterFLT("diffuse_coverage",1.0f);
	AiParameterFLT("diffuse_softness",1.0);

	AiParameterRGB("highlight_color",1.0f,1.0f,1.0f);
	AiParameterFLT("highlight_scale",0.0f);
	AiParameterFLT("highlight_exponent",50.0f);
	AiParameterFLT("highlight_coverage",1.0f);
	AiParameterFLT("highlight_softness",1.0f);

	AiParameterRGB("rimlight_color",1.0f,1.0f,1.0f);
	AiParameterFLT("rimlight_scale",0.0f);
	AiParameterFLT("rimlight_coverage",1.0f);
	AiParameterFLT("rimlight_softness",1.0f);

	AiParameterSTR("ambient_fb_str","");
	AiParameterSTR("diffuse_fb_str","");
	AiParameterSTR("highlight_fb_str","");
	AiParameterSTR("rimlight_fb_str","");
	AiParameterSTR("contour_fb_str","");

	AiParameterBOOL("putAlphaInFb",false);

	AiParameterRGB("opacity",1.0f,1.0f,1.0f);

	AiParameterBool("normalizeObjectID",true);

	AiParameterBool("ambient_noInternal",true);
	AiParameterBool("diffuse_noInternal",true);
	AiParameterBool("highlight_noInternal",true);
	AiParameterBool("rimlight_noInternal",true);

	AiParameterBool("global_clamp",false);
}

node_initialize
{
	//AiMsgInfo("-*/-*/ node_initialize -*/-*/");
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));

	data->global_compMode = OBQ_COMPOVER;
	data->global_multByLightColor = true;

	data->ambient_multAmbience = false;


	data->ambient_fb_str = "";
	data->diffuse_fb_str = "";
	data->highlight_fb_str = "";
	data->rimlight_fb_str = "";
	data->contour_fb_str = "";
	data->putAlphaInFb = false;

	data->output_ambient_fb =	false;
	data->output_diffuse_fb =	false;
	data->output_highlight_fb = false;
	data->output_rimlight_fb =	false;
	data->output_contour_fb =	false;

	data->ambience = AI_RGB_BLACK;

	//object map for ID
	data->objectMap = new std::map<AtNode*,float>;

	data->ambient_noInternal = true;
	data->diffuse_noInternal = true;
	data->highlight_noInternal = true;
	data->rimlight_noInternal = true;

	data->global_clamp = true;

	AiNodeSetLocalData(node,data);
	
}

node_update
{
	//AiMsgInfo("-*/-*/ node_update -*/-*/");
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	data->global_compMode = params[p_global_compMode].INT;
	data->global_multByLightColor = params[p_global_multByLightColor].BOOL;

	data->ambient_multAmbience = params[p_ambient_multAmbience].BOOL;

	data->ambient_fb_str = params[p_ambient_fb_str].STR;
	data->diffuse_fb_str = params[p_diffuse_fb_str].STR;
	data->highlight_fb_str = params[p_highlight_fb_str].STR;
	data->rimlight_fb_str = params[p_rimlight_fb_str].STR;
	data->contour_fb_str = params[p_contour_fb_str].STR;
	data->putAlphaInFb = params[p_putAlphaInFb].BOOL;

	// Register AOV with AI_AOV_BLEND_OPACITY
	if(data->ambient_fb_str && std::strlen(data->ambient_fb_str))		AiAOVRegister(data->ambient_fb_str,		AI_TYPE_RGBA,AI_AOV_BLEND_OPACITY);
	if(data->diffuse_fb_str && std::strlen(data->diffuse_fb_str))		AiAOVRegister(data->diffuse_fb_str,		AI_TYPE_RGBA,AI_AOV_BLEND_OPACITY);
	if(data->highlight_fb_str && std::strlen(data->highlight_fb_str))	AiAOVRegister(data->highlight_fb_str,	AI_TYPE_RGBA,AI_AOV_BLEND_OPACITY);
	if(data->rimlight_fb_str && std::strlen(data->rimlight_fb_str))		AiAOVRegister(data->rimlight_fb_str,	AI_TYPE_RGBA,AI_AOV_BLEND_OPACITY);
	if(data->contour_fb_str && std::strlen(data->contour_fb_str))		AiAOVRegister(data->contour_fb_str,		AI_TYPE_RGBA,AI_AOV_BLEND_OPACITY);

	// Get Ambience from Options
	AtNode* options = AiUniverseGetOptions();
	if(AiNodeLookUpUserParameter(options, "ambience") != NULL)
		data->ambience = AiNodeGetRGB(options, "ambience");

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

	data->global_noInternal = data->ambient_noInternal || data->diffuse_noInternal || data->highlight_noInternal || data->rimlight_noInternal;

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
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	// opacity noInternal
	bool backface = ( AiV3Dot(sg->Rd,sg->N) > 0);
	if(backface && data->global_noInternal)
	{
		sg->out_opacity = AI_RGB_BLACK;
		return;
	}

	sg->out_opacity = AiShaderEvalParamRGB(p_opacity);

	// Shadow
	if(sg->Rt & AI_RAY_SHADOW)
	{
		sg->out.RGB =  AI_RGB_BLACK;
		return;
	}
	else
	{

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

		
		AtColor sum_tex=AI_RGB_BLACK;

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
			AiLightsPrepare(sg);

			while(AiLightsGetSample(sg)){

				if(do_diffuse && AiLightGetAffectDiffuse(sg->Lp))
				{
					float NdotL = AiV3Dot(Nf,sg->Ld);

					float dot =  inode_dot_modify(NdotL, diffuse_coverage, diffuse_softness);

					diffuse_sample_color = dot;

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

					float dot =  inode_dot_modify(NdotH, highlight_coverage, highlight_softness, highlight_exponent);

					highlight_sample_color = dot;

					// multiply by light color ?
					if(data->global_multByLightColor)
						highlight_sample_color*= sg->Li;

					highlight_loop_color+=highlight_sample_color*sg->we;
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
		else if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->ambient_fb_str, AI_TYPE_RGBA))
		{
			AtRGBA solidBlack = AI_RGBA_BLACK;
			solidBlack.a = 1.0f;

			AiAOVSetRGBA (sg, data->ambient_fb_str, solidBlack);
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
		else if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->diffuse_fb_str, AI_TYPE_RGBA))
		{
			AtRGBA solidBlack = AI_RGBA_BLACK;
			
			if(!data->putAlphaInFb)
				solidBlack.a = 1.0f;
			else
				solidBlack.a = 0.0f;

			AiAOVSetRGBA (sg, data->diffuse_fb_str, solidBlack);
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
		else if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->highlight_fb_str, AI_TYPE_RGBA))
		{
			AtRGBA solidBlack = AI_RGBA_BLACK;
			
			if(!data->putAlphaInFb)
				solidBlack.a = 1.0f;
			else
				solidBlack.a = 0.0f;

			AiAOVSetRGBA (sg, data->highlight_fb_str, solidBlack);
		}

		if(do_rimlight)
		{
			AtColor rimlight_loop_color;

			float NdotV = inode_dot_modify(AiV3Dot(Nf,V), rimlight_coverage, rimlight_softness, 1.0f,true);

			rimlight_loop_color = NdotV;

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
		else if(sg->Rt & AI_RAY_CAMERA && AiAOVEnabled(data->rimlight_fb_str, AI_TYPE_RGBA))
		{
			AtRGBA solidBlack = AI_RGBA_BLACK;
			
			if(!data->putAlphaInFb)
				solidBlack.a = 1.0f;
			else
				solidBlack.a = 0.0f;

			AiAOVSetRGBA (sg, data->rimlight_fb_str, solidBlack);
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
