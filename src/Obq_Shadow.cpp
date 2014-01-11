/*
Obq_Shadow :

	shadow shader

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

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqShadowSimpleMethods);

// enum for parameters
//
enum ObqShadowSimpleParams { p_mode, p_traceType, p_opacity, p_shadowsOnUnlit };

// shader data struct
//
typedef struct 
{
	int mode;
	int traceType;
	bool shadowsOnUnlit;
}
ShaderData;

enum ObqShadowModes {MODE_SHADOWED, MODE_UNSHADOWED, MODE_RATIO, MODE_DIFFERENCE};
enum ObqShadowTraceType {TRACE_RECEIVER, TRACE_CASTER, TRACE_BOTH};

node_parameters
{
	AiParameterINT("mode",2);
	AiParameterINT("traceType", TRACE_BOTH);
	AiParameterRGB("opacity", 1.0f,1.0f,1.0f);
	AiParameterBOOL("shadowsOnUnlit",false);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->mode = MODE_RATIO;
	data->traceType = TRACE_BOTH;
	data->shadowsOnUnlit = false;
	AiNodeSetLocalData(node,data);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->mode = params[p_mode].INT;
	data->traceType = params[p_traceType].INT;
	data->shadowsOnUnlit = params[p_shadowsOnUnlit].BOOL;
}

node_finish
{
	// Desallocate shader data
	AiFree((ShaderData*)AiNodeGetLocalData(node));
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	
	AtColor Opacity = AiColorClamp(AiShaderEvalParamRGB(p_opacity),0.0f,1.0f);

	if(sg->Rt & AI_RAY_SHADOW)
	{
		
		if(data->traceType >= TRACE_CASTER && (data->mode == MODE_RATIO || data->mode == MODE_DIFFERENCE))
		{
			// How much light remains
			AtColor currentT = AI_RGB_WHITE;
			AiStateGetMsgRGB("OSc", &currentT);
			AiStateSetMsgRGB("OSc", ((currentT)*(AI_RGB_WHITE - Opacity)));
			
			sg->out_opacity = AI_RGB_BLACK; // passthrough
			sg->out.RGB = AI_RGB_BLACK;
		}
		else
		{
			sg->out_opacity = Opacity;
			sg->out.RGB = AI_RGB_WHITE;
		}
		return;
	}
	else
	{
		switch(data->mode)
		{
		case MODE_SHADOWED:
			{
				AiLightsPrepare(sg);
				while(AiLightsGetSample(sg))
				{
					float cosTheta_i = std::min(std::abs(AiV3Dot(sg->Ld,sg->Nf)),1.0f);
					sg->out.RGB +=	sg->Li*sg->we*cosTheta_i*c_1OverPi;
				}
				break;
			}
			case MODE_UNSHADOWED:
			{
				if(data->shadowsOnUnlit)
					sg->fhemi = false;

				sg->skip_shadow = true;
				AiLightsPrepare(sg);
				sg->skip_shadow = false;

				while(AiLightsGetSample(sg))
				{
					float cosTheta_i = std::min(AiV3Dot(sg->Ld,sg->Nf),1.0f);
					
					if(cosTheta_i > 0.0f)
					{
						sg->out.RGB +=	sg->Liu*sg->we*cosTheta_i*c_1OverPi;
					}
					else if(data->shadowsOnUnlit)
					{
						sg->out.RGB +=	sg->Liu*sg->we*std::abs(cosTheta_i)*c_1OverPi;
					}
				}
				
				if(data->shadowsOnUnlit)
					sg->fhemi = true;
				
				break;
			}
			case MODE_RATIO:
			{
				if(data->traceType == TRACE_CASTER)
				{
					sg->out.RGB = AI_RGB_WHITE;
				}
				else
				{
					if(data->shadowsOnUnlit)
						sg->fhemi = false;

					AtColor loop_color_s = AI_RGB_BLACK, loop_color_u = AI_RGB_BLACK;

					sg->skip_shadow = true;
					AiLightsPrepare(sg);
					sg->skip_shadow = false;

					AiStateSetMsgRGB("OSc", AI_RGB_WHITE);

					while(AiLightsGetSample(sg))
					{
						float cosTheta_i = AiV3Dot(sg->Ld,sg->Nf);

						//Trace shadow
						if(cosTheta_i > 0.0f)
						{
							AtRay ray;
							AtPoint tmp = sg->P;//+AI_EPSILON*sg->Ngf;
							AiMakeRay(&ray,AI_RAY_SHADOW, &tmp, &sg->Ld,sg->Ldist,sg);
							AtScrSample sample;
							bool hit = AiTrace(&ray,&sample);
							AtColor OSc = AI_RGB_WHITE, factor = AI_RGB_WHITE;

							AiStateGetMsgRGB("OSc", &OSc);

							factor = (AI_RGB_WHITE-(AI_RGB_WHITE-sample.opacity)*(AI_RGB_WHITE-OSc));

							loop_color_u +=	sg->Liu*sg->we*cosTheta_i*c_1OverPi;
							loop_color_s +=	factor*sg->Liu*sg->we*cosTheta_i*c_1OverPi;
						}
						else if(data->shadowsOnUnlit)
						{
							loop_color_u +=	sg->Liu*sg->we*std::abs(cosTheta_i)*c_1OverPi;
							//loop_color_s += 0*c_1OverPi;
						}
						AiStateSetMsgRGB("OSc", AI_RGB_WHITE);
					}
					if(data->shadowsOnUnlit)
						sg->fhemi = true;

					if(loop_color_u.r > 0.0f) 
						sg->out.RGB.r = loop_color_s.r/loop_color_u.r;
					else 
						sg->out.RGB.r = 1.0f;

					if(loop_color_u.g > 0.0f) 
						sg->out.RGB.g = loop_color_s.g/loop_color_u.g;
					else 
						sg->out.RGB.g = 1.0f;

					if(loop_color_u.b > 0.0f) 
						sg->out.RGB.b = loop_color_s.b/loop_color_u.b;
					else 
						sg->out.RGB.b = 1.0f;
				}
				break;
			}
			default : //case MODE_DIFFERENCE:
			{
				if(data->traceType == TRACE_CASTER)
				{
					sg->out.RGB = AI_RGB_BLACK;
				}
				else
				{
					if(data->shadowsOnUnlit)
						sg->fhemi = false;

					sg->skip_shadow = true;
					AiLightsPrepare(sg);
					sg->skip_shadow = false;

					AiStateSetMsgRGB("OSc", AI_RGB_WHITE);
					while(AiLightsGetSample(sg))
					{
						float cosTheta_i = AiV3Dot(sg->Ld,sg->Nf);

						if(cosTheta_i > 0.0f)
						{
							//Trace shadow
							AtRay ray;
							AtPoint tmp = sg->P;//+AI_EPSILON*sg->Ngf;
							AiMakeRay(&ray,AI_RAY_SHADOW, &tmp, &sg->Ld, sg->Ldist,sg);
							AtScrSample sample;
							bool hit = AiTrace(&ray,&sample);

							AtColor OSc = AI_RGB_WHITE, factor = AI_RGB_WHITE;

							AiStateGetMsgRGB("OSc", &OSc);

							factor = (AI_RGB_WHITE-sample.opacity)*(AI_RGB_WHITE-OSc);

							sg->out.RGB +=	factor*sg->Liu*sg->we*cosTheta_i*c_1OverPi;		

						}
						else if(data->shadowsOnUnlit)
						{
							sg->out.RGB +=	sg->Liu*sg->we*std::abs(cosTheta_i)*c_1OverPi;
						}
						AiStateSetMsgRGB("OSc", AI_RGB_WHITE);
					}
					if(data->shadowsOnUnlit)
						sg->fhemi = true;
				}
				break;
			}
		}

		// Opacity
		sg->out_opacity = Opacity;
	}
}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqShadowSimpleMethods;
//   node->output_type  = AI_TYPE_RGB;
//   node->name         = "Obq_Shadow";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
