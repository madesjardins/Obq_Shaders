/*
Obq_Shadow v2.10.0 (SItoA 2.10.0 - Arnold 4.0.16.2) :

	shadow shader

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

#include "Obq_Common.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqShadowSimpleMethods);

// enum for parameters
//
enum ObqShadowSimpleParams { p_mode};

// shader data struct
//
typedef struct 
{
	int mode;
}
ShaderData;

enum ObqShadowModes {MODE_SHADOWED, MODE_UNSHADOWED_SKIP, MODE_RATIO_SKIP, MODE_DIFFERENCE_SKIP, MODE_OCCLUSION, MODE_MATTE};

node_parameters
{
	AiParameterINT("mode",2);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->mode = 0;
	AiNodeSetLocalData(node,data);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	
	data->mode = params[p_mode].INT;
}

node_finish
{
	// Desallocate shader data
	AiFree((ShaderData*)AiNodeGetLocalData(node));
}

shader_evaluate
{

	if(sg->Rt & AI_RAY_SHADOW) // TODO : manage opacity
	{
		sg->out_opacity = AI_RGB_WHITE;
	}
	else // TODO : manage other rays
	{

		ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

		switch(data->mode)
		{
		case MODE_SHADOWED:
			{
				AiLightsPrepare(sg);
				while(AiLightsGetSample(sg))
				{
					float cosTheta_i = std::min(std::abs(AiV3Dot(sg->Ld,sg->Nf)),1.0f);
					sg->out.RGB +=	sg->Li*sg->we*cosTheta_i;
				}
				break;
			}
			case MODE_UNSHADOWED_SKIP:
			{
				sg->skip_shadow = true;
				AiLightsPrepare(sg);
				sg->skip_shadow = false;

				while(AiLightsGetSample(sg))
				{
					float cosTheta_i = std::min(AiV3Dot(sg->Ld,sg->Nf),1.0f);
					
					if(cosTheta_i > 0.0f)
					{
						sg->out.RGB +=	sg->Liu*sg->we*cosTheta_i;

					}
				}
				
				break;
			}
			case MODE_RATIO_SKIP:
			{
				AtColor loop_color_s = AI_RGB_BLACK, loop_color_u = AI_RGB_BLACK;
				sg->skip_shadow = true;
				AiLightsPrepare(sg);
				sg->skip_shadow = false;
				while(AiLightsGetSample(sg))
				{
					float cosTheta_i = AiV3Dot(sg->Ld,sg->Nf);

					//Trace shadow
					if(cosTheta_i > 0.0f)
					{
						AtRay ray;
						AtPoint tmp = sg->P+AI_EPSILON*sg->Ngf;
						AiMakeRay(&ray,AI_RAY_SHADOW, &tmp, &sg->Ld,sg->Ldist,sg);
						AtScrSample sample;
						AiTrace(&ray,&sample);
						
						loop_color_u +=	sg->Liu*sg->we*cosTheta_i;
						loop_color_s +=	((AI_RGB_WHITE-sample.opacity)*sg->Liu)*sg->we*cosTheta_i;
					}
					
				}

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
				break;
			}
			case MODE_DIFFERENCE_SKIP:
			{
				sg->skip_shadow = true;
				AiLightsPrepare(sg);
				sg->skip_shadow = false;
				
				while(AiLightsGetSample(sg))
				{
					float cosTheta_i = AiV3Dot(sg->Ld,sg->Nf);
					
					if(cosTheta_i > 0.0f)
					{
						//Trace shadow
						AtRay ray;
						AtPoint tmp = sg->P+AI_EPSILON*sg->Ngf;
						AiMakeRay(&ray,AI_RAY_SHADOW, &tmp, &sg->Ld, sg->Ldist,sg);
						AtScrSample sample;
						AiTrace(&ray,&sample);

						sg->out.RGB +=	(sg->Liu*sample.opacity)*sg->we*cosTheta_i;

					}
					
				}
				break;
			}
			case MODE_OCCLUSION:
			{
				AiLightsPrepare(sg);
				while(AiLightsGetSample(sg))
				{
					sg->out.RGB +=	sg->Lo*sg->we;
				}
				break;
			}
			default: //MODE_MATTE:
			{
				sg->out.RGB = AiLightsGetShadowMatte(sg);
				break;
			}
		}
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
