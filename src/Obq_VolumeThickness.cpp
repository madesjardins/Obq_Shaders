/*
Obq_VolumeThickness:



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
#include "Obq_VolumeThickness.h"

// ENUM MENU
static const char* ObqVTThicknessModeNames[] = 
{
	"Sphere Particles",
    "Custom Thickness",
    NULL
};
static const char* OvtAutoNames[] = 
{
	"Always",
	"Shadow-Only",
    NULL
};
static const char* OvtDiffuseNames[] = 
{
	"None",
	"Direct",
	"Direct*opacity",
	"Direct*sqrt(opacity)",
	"Full",
	"Full (Emission only)",
    NULL
};
static const char* ovtAlphaNames[] = 
{
	"Alpha Parameter",
    "Diffuse Alpha",
    "Emission Alpha",
    "Max of Diffuse and Emission Alpha",
    NULL
};
 
// parameters
//
node_parameters
{
	AiParameterRGBA("colorIn", 0.0f, 0.0f, 0.0f, 0.0f);// shader
	AiParameterRGBA("emissionColor", 0.0f, 0.0f, 0.0f, 0.0f);// shader
	AiParameterENUM("alphaMode", 0,ovtAlphaNames);
	AiParameterFLT("alpha", 0);
	AiParameterENUM("thicknessMode", 0, ObqVTThicknessModeNames);
	AiParameterFLT("thickness", 0.0f);					// thickness
	AiParameterFLT("accumulatedThicknessThreshold",1.0);
	AiParameterFLT("radius",1.0);
	AiParameterFLT("exponent",1.0);
	AiParameterBOOL("useAbsorption",false);
	AiParameterFLT("absorption",0.0);
	AiParameterENUM("diffuseMode",3,OvtDiffuseNames);
	AiParameterENUM("autoTransparencyMode",0,OvtAutoNames);
	AiParameterFLT("shadowModifier",1);
	AiParameterFLT("lightPropagationDistance",0.1f);
	AiParameterBOOL("doubleHemisphericalDiffuse",true);
	AiParameterFLT("lightPropagationDistanceDiffuse",0.1f);
}


node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	AiNodeSetLocalData (node, data);
}

node_update
{

	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);

	AtNode* options = AiUniverseGetOptions();
	data->max_diffuse_depth = AiNodeGetInt(options, "GI_diffuse_depth");

	data->diffuseMode = AiNodeGetInt(node,"diffuseMode");
	data->autoTransparencyMode = AiNodeGetInt(node,"autoTransparencyMode");

	data->accumulatedThicknessThreshold = AiNodeGetFlt(node,"accumulatedThicknessThreshold");

	data->lightPropagationDistance = AiNodeGetFlt(node,"lightPropagationDistance");
	data->useLightPropagation = (data->lightPropagationDistance>0.0f);

	data->lightPropagationDistanceDiffuse = AiNodeGetFlt(node,"lightPropagationDistanceDiffuse");
	data->useLightPropagationDiffuse = (data->lightPropagationDistanceDiffuse>0.0f);

	data->shadowModifier = AiNodeGetFlt(node,"shadowModifier");
	data->useShadowModifier = (data->shadowModifier<1.0f);
	
	data->alphaMode = AiNodeGetInt(node,"alphaMode");

	data->doubleHemisphericalDiffuse = AiNodeGetBool(node, "doubleHemisphericalDiffuse");
	
}

node_finish
{
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	// shader data
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);

	unsigned long myRand = rand()%1000;

	/////////////////////////////////
	//	SHADOWS
	/////////////////////////////////
	if(sg->Rt == AI_RAY_SHADOW)
	{
		// BACKFACE
		if(sg->Ng != sg->Ngf)
			sg->out_opacity = 0.0;
		else
		{
			// Get Alpha
			float alpha = 0.0f;
			switch(data->alphaMode)
			{
			case ALPHA_ALPHA:
				alpha = AiShaderEvalParamFlt(p_alpha);
				break;
			case ALPHA_DIFFUSE:
				alpha = AiShaderEvalParamRGBA(p_colorIn).a;
				break;
			case ALPHA_EMISSION:
				alpha = AiShaderEvalParamRGBA(p_emissionColor).a;
				break;
			case ALPHA_MAX:
				alpha = std::max(AiShaderEvalParamRGBA(p_colorIn).a,AiShaderEvalParamRGBA(p_emissionColor).a);
				break;
			}

			if(alpha != 0.0f)
				sg->out_opacity = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));
			else
				sg->out_opacity = 0.0f;
		}
		return;
	}
	/////////////////////////////////
	//	AUTO-TRANSPARENCY ALWAYS
	/////////////////////////////////
	else if(data->autoTransparencyMode == AUTO_ALWAYS)
	{
		// backface
		if(sg->Ng != sg->Ngf)
		{
			sg->out.RGBA = AI_RGBA_BLACK;
			sg->out_opacity = 0.0;
			return;
		}
		///////////////
		// DIFFUSE RAY
		///////////////
		else if(sg->Rt == AI_RAY_DIFFUSE)
		{
			// get color
			AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
			bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
			AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

			// Get Alpha
			float alpha = 0.0f;
			switch(data->alphaMode)
			{
			case ALPHA_ALPHA:
				alpha = AiShaderEvalParamFlt(p_alpha);
				break;
			case ALPHA_DIFFUSE:
				alpha = diffuse.a;
				break;
			case ALPHA_EMISSION:
				alpha = emission.a;
				break;
			case ALPHA_MAX:
				alpha = std::max(diffuse.a,emission.a);
				break;
			}

			// calculate opacity
			float opacity = 0.0f;
			
			//check alpha
			if(alpha > 0.0f)
				opacity = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));
			
			//check opacity
			if(opacity > 1.0f)
				opacity=1.0f;
			else if(opacity <= 0.0f)
			{
				// trivial case
				sg->out.RGBA.r = 0.0f;
				sg->out.RGBA.g = 0.0f;
				sg->out.RGBA.b = 0.0f;
				sg->out.RGBA.a = 0.0f;
				sg->out_opacity = 0.0f;
				return;
			}

			////////////////////
			// DIFFUSE CAST MODE
			////////////////////

			AtColor color = AI_RGB_BLACK;

			if(data->diffuseMode == DIFFUSE_EMISSION)
			{
				sg->out.RGBA.r = emission.r;
				sg->out.RGBA.g = emission.g;
				sg->out.RGBA.b = emission.b;
				sg->out.RGBA.a = 1.0f;
				sg->out_opacity = opacity;
				return;
			}
			else if(data->diffuseMode == DIFFUSE_NORMAL)
			{

				//check for already calculated lighting
				AtPoint lightPoint;
				bool lightPointCalculated = false;

				if(data->useLightPropagationDiffuse && AiMessageGetPntFunc (sg,"ovt_diffuseLightPoint", &lightPoint))
				{
					// point exists

					// how far is it
					float distance2 = AiV3Dist2(lightPoint,sg->P);
					float propagationMax = data->lightPropagationDistanceDiffuse*data->lightPropagationDistanceDiffuse;

					if(distance2 <= propagationMax)
					{
						//we can use the old lighting
						if(AiMessageGetRGBFunc(sg,"ovt_diffuseLightColor", &color))
							lightPointCalculated = true;
						else
							color = AI_RGB_BLACK;
					}
				}

				if(!lightPointCalculated)
				{
					addAllDirect(sg,data,kdColorNonZero,&color);

					// Indirect lighting
					if ( sg->Rr_diff < data->max_diffuse_depth && kdColorNonZero)
					{
						color += AiIndirectDiffuse(&sg->Nf, sg);

						// double hemispherical sampling
						if(data->doubleHemisphericalDiffuse)
						{
							AtPoint tempP = sg->P;
							sg->P -= AI_EPSILON*sg->Ngf;
							sg->Nf = -sg->Nf;
							sg->Ngf = -sg->Ngf;
							color += AiIndirectDiffuse(&sg->Nf, sg);
							sg->Nf = -sg->Nf;
							sg->Ngf = -sg->Ngf;
							sg->P = tempP;
						}
					}

					// update lightPoint and lightColor
					if(data->useLightPropagationDiffuse)
					{
						AiMessageSetPntFunc(sg,"ovt_diffuseLightPoint",sg->P);
						AiMessageSetRGBFunc(sg,"ovt_diffuseLightColor",color);
					}
				}
				sg->out.RGBA.r = color.r*diffuse.r + emission.r;
				sg->out.RGBA.g = color.g*diffuse.g + emission.g;
				sg->out.RGBA.b = color.b*diffuse.b + emission.b;
				sg->out.RGBA.a = 1.0f;
				sg->out_opacity = opacity;
				return;
			}
			else if(data->diffuseMode == DIFFUSE_BLACK)
			{
				////////////////////////
				// BLACK DIFFUSE MODE
				////////////////////////

				sg->out.RGBA.r = 0.0f;
				sg->out.RGBA.g = 0.0f;
				sg->out.RGBA.b = 0.0f;
				sg->out.RGBA.a = 1.0f;
				sg->out_opacity = 1.0f;
				return;
			}
			else
			{
				/////////////////////
				// OTHER DIFFUSE MODE
				/////////////////////

				switch(data->diffuseMode)
				{
				case DIFFUSE_DIRECT:
					addAllDirect(sg,data,kdColorNonZero,&color);
					opacity = 1.0;
					break;
				case DIFFUSE_DIRECTOPACITY:
					addAllDirect(sg,data,kdColorNonZero,&color);
					break;
				case DIFFUSE_DIRECTSQRTOPACITY:
					opacity = std::sqrt(opacity);
					addAllDirect(sg,data,kdColorNonZero,&color);
					break;
				default:
					break;
				} 

				sg->out.RGBA.r = (color.r*diffuse.r + emission.r)*opacity;
				sg->out.RGBA.g = (color.g*diffuse.g + emission.g)*opacity;
				sg->out.RGBA.b = (color.b*diffuse.b + emission.b)*opacity;
				sg->out.RGBA.a = 1.0;
				sg->out_opacity = 1.0;
				return;
			}
		}
		else
		{
			/////////////////////////
			// CAMERA RAY and others
			/////////////////////////

			// get color
			AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
			bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
			AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

			// Get Alpha
			float alpha = 0.0f;
			switch(data->alphaMode)
			{
			case ALPHA_ALPHA:
				alpha = AiShaderEvalParamFlt(p_alpha);
				break;
			case ALPHA_DIFFUSE:
				alpha = diffuse.a;
				break;
			case ALPHA_EMISSION:
				alpha = emission.a;
				break;
			case ALPHA_MAX:
				alpha = std::max(diffuse.a,emission.a);
				break;
			}

			// calculate opacity
			float opacity = 0.0f;
			if(alpha > 0.0f)
				opacity = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));

			//clamp
			if(opacity <= 0.0f)
			{
				// trivial case
				sg->out.RGBA.r = 0.0f;
				sg->out.RGBA.g = 0.0f;
				sg->out.RGBA.b = 0.0f;
				sg->out.RGBA.a = 0.0f;
				sg->out_opacity = 0.0f;
				return;
			}
			else if(opacity > 1.0f)
				opacity=1.0f;

			// Direct Lighting
			AtColor color = AI_RGB_BLACK;

			//check for already calculated lighting
			AtPoint lightPoint;
			bool lightPointCalculated = false;

			if(data->useLightPropagation && AiMessageGetPntFunc (sg,"ovt_lightPoint", &lightPoint))
			{
				// point exists

				// how far is it
				float distance2 = AiV3Dist2(lightPoint,sg->P);
				float propagationMax = data->lightPropagationDistance*data->lightPropagationDistance;
				
				if(distance2 <= propagationMax)
				{
					//we can use the old lighting
					if(AiMessageGetRGBFunc(sg,"ovt_lightColor", &color))
						lightPointCalculated = true;
					else
						color = AI_RGB_BLACK;
				}
			}
			
			if(!lightPointCalculated)
			{
				addAllDirect(sg,data,kdColorNonZero,&color);

				// Indirect lighting
				if ( sg->Rr_diff < data->max_diffuse_depth && kdColorNonZero)
				{
					color += AiIndirectDiffuse(&sg->Nf, sg);
					// double hemispherical sampling
					if(data->doubleHemisphericalDiffuse)
					{
						AtPoint tempP = sg->P;
						sg->P -= AI_EPSILON*sg->Ngf;
						sg->Nf = -sg->Nf;
						sg->Ngf = -sg->Ngf;
						color += AiIndirectDiffuse(&sg->Nf, sg);
						sg->Nf = -sg->Nf;
						sg->Ngf = -sg->Ngf;
						sg->P = tempP;
					}
				}
			
				// update lightPoint and lightColor
				if(data->useLightPropagation)
				{
					AiMessageSetPntFunc(sg,"ovt_lightPoint",sg->P);
					AiMessageSetRGBFunc(sg,"ovt_lightColor",color);
				}
			}

			// output
			sg->out.RGBA.r = color.r*diffuse.r + emission.r;
			sg->out.RGBA.g = color.g*diffuse.g + emission.g;
			sg->out.RGBA.b = color.b*diffuse.b + emission.b;
			sg->out.RGBA.a = 1.0f;
			sg->out_opacity = opacity;
			return;
		}
	}
	/////////////////////////////////////////////////
	//	AUTO-TRANSPARENCY SHADOW-ONLY SPECIAL DIFFUSE
	/////////////////////////////////////////////////
	else if(sg->Rt == AI_RAY_DIFFUSE)
	{
		// backface test
		bool backface = sg->Ng != sg->Ngf;

		///////////////////////////////
		// DIFFUSE NORMAL/EMISSION MODE
		///////////////////////////////
		if(data->diffuseMode == DIFFUSE_NORMAL || data->diffuseMode == DIFFUSE_EMISSION)
		{

			// is it a new hit ?

			int flag = -1;
			int depth = -1;
			bool flagSet = AiMessageGetIntFunc(sg,"ovt_diffuseFlag", &flag);
			bool depthSet = AiMessageGetIntFunc(sg,"ovt_diffuseDepth", &depth);

			if(!flagSet || flag == DIFFUSE_IT || flag == UNKNOWN_IT || flag == RESET_IT || (flag==FIRST_IT && sg->Rr != depth+1))
			{

				////////////////
				//	NEW HIT
				////////////////
				//AiMsgInfo("%i : Diffuse new Hit with sg->Rr = %i and sg->Rr_diff = %i", myRand, sg->Rr, sg->Rr_diff);
				AtPoint currentPoint = sg->P + AI_EPSILON*sg->Rd;

				// get diffuse
				AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
				bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
				AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

				// Get Alpha
				float alpha = 0.0f;
				switch(data->alphaMode)
				{
				case ALPHA_ALPHA:
					alpha = AiShaderEvalParamFlt(p_alpha);
					break;
				case ALPHA_DIFFUSE:
					alpha = diffuse.a;
					break;
				case ALPHA_EMISSION:
					alpha = emission.a;
					break;
				case ALPHA_MAX:
					alpha = std::max(diffuse.a,emission.a);
					break;
				}

				// if backface, we want to shoot, but not calculate this
				if(backface)
					alpha = 0.0f;

				// Calculate Opacity
				float cumulativeOpacity = 0.0f;
				if(alpha > 0.0f)
					cumulativeOpacity = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));

				//clamp
				if(cumulativeOpacity > 1.0f)
					cumulativeOpacity=1.0f;

				if(cumulativeOpacity <= 0.0f)
				{
					sg->out.RGBA = AI_RGBA_BLACK;
					cumulativeOpacity = 0.0f;
				}
				else // need to calculate shading
				{

					// Direct Lighting
					AtColor color = AI_RGB_BLACK;

					if(data->diffuseMode != DIFFUSE_EMISSION){

						//check for already calculated lighting
						AtPoint lightPoint;
						bool lightPointCalculated = false;

						if(data->useLightPropagationDiffuse && AiMessageGetPntFunc (sg,"ovt_lightPointDiffuse", &lightPoint))
						{
							// point exists

							// how far is it
							float distance2 = AiV3Dist2(lightPoint,sg->P);
							float propagationMax = data->lightPropagationDistanceDiffuse*data->lightPropagationDistanceDiffuse;

							if(distance2 <= propagationMax)
							{
								//we can use the old lighting
								if(AiMessageGetRGBFunc(sg,"ovt_lightColorDiffuse", &color))
									lightPointCalculated = true;
								else
									color = AI_RGB_BLACK;
							}
						}

						if(!lightPointCalculated)
						{

							addAllDirect(sg,data,kdColorNonZero,&color);

							// Indirect lighting
							if (sg->Rr_diff < data->max_diffuse_depth && kdColorNonZero )
							{
								AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
								//AiMsgInfo("%i : Diffuse new Hit sending indirect with sg->Rr = %i and sg->Rr_diff = %i", myRand, sg->Rr, sg->Rr_diff);
								AiMessageSetIntFunc(sgTrace,"ovt_diffuseFlag", DIFFUSE_IT);
								AiMessageSetIntFunc(sgTrace,"ovt_diffuseDepth", sg->Rr);
								color += AiIndirectDiffuse(&sg->Nf, sgTrace);
								delete sgTrace;

								// double hemispherical sampling
								if(data->doubleHemisphericalDiffuse)
								{
									// double hemispherical sampling
									AtShaderGlobals  *sgTrace2 = new AtShaderGlobals(*sg);
									AiMessageSetIntFunc(sgTrace2,"ovt_diffuseFlag", DIFFUSE_IT);
									AiMessageSetIntFunc(sgTrace2,"ovt_diffuseDepth", sg->Rr);
									//AtPoint tempP = sgTrace2->P;
									sgTrace2->P -= AI_EPSILON*sgTrace2->Ngf;
									sgTrace2->Nf = -sgTrace2->Nf;
									sgTrace2->Ngf = -sgTrace2->Ngf;
									color += AiIndirectDiffuse(&sgTrace2->Nf, sgTrace2);
									delete sgTrace2;
								}
							}
							// update lightPoint and lightColor
							if(data->useLightPropagationDiffuse)
							{
								AiMessageSetPntFunc(sg,"ovt_lightPointDiffuse",sg->P);
								AiMessageSetRGBFunc(sg,"ovt_lightColorDiffuse",color);
							}
						}
					}
					sg->out.RGBA.r = cumulativeOpacity*(diffuse.r*color.r + emission.r);
					sg->out.RGBA.g = cumulativeOpacity*(diffuse.g*color.g + emission.g);
					sg->out.RGBA.b = cumulativeOpacity*(diffuse.b*color.b + emission.b);
					sg->out.RGBA.a = cumulativeOpacity;
					//sg->out_opacity = 1.0f;
				}


				//////////////
				// Main Loop
				//////////////
				unsigned long loopIteration = 0;
				//sgTrace->Rr--;
				int Rr_diff = sg->Rr_diff-1;
				sg->Rr_diff = Rr_diff;
				while(cumulativeOpacity < data->accumulatedThicknessThreshold){
					// on va lancer un rayon avec un flag FIRST_IT
					AtRay newRay;

					AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
					AiMessageSetIntFunc(sgTrace,"ovt_diffuseFlag", FIRST_IT);
					AiMessageSetIntFunc(sgTrace,"ovt_diffuseDepth", sg->Rr);
					AiMakeRay(&newRay, sg->Rt, &currentPoint, &sg->Rd, AI_INFINITE, sgTrace);

					AtScrSample	sample;
					//AiMsgInfo("%i : Diffuse new Hit tracing with sg->Rr = %i and sg->Rr_diff = %i", myRand, sg->Rr, sg->Rr_diff);
					if(!AiTrace(&newRay, &sample))
					{
						////AiMsgInfo("01 : !AiTrace...  tracing background... AI_RAY_DIFFUSE : sgTrace->Rr = %i, sgTrace->Rdiff = %i, sgTrace->Rt = %i",sgTrace->Rr, sgTrace->Rr_diff, sgTrace->Rt);
						AiTraceBackground(&newRay,&sample);
						//AiMsgInfo("%i : Diffuse new Hit background", myRand);
						float adjustedAlpha = (1.0f-cumulativeOpacity)*sample.alpha;
						cumulativeOpacity += adjustedAlpha;
						sg->out.RGBA.r += adjustedAlpha*sample.color.r;
						sg->out.RGBA.g += adjustedAlpha*sample.color.g;
						sg->out.RGBA.b += adjustedAlpha*sample.color.b;
						sg->out.RGBA.a = 1.0; //cumulativeOpacity;
						delete sgTrace;
						break;
					}
					else
					{
						////AiMsgInfo("02 : Hitting diffuse : sgTrace->Rr = %i, sgTrace->Rdiff = %i, sgTrace->Rt = %i",sgTrace->Rr, sgTrace->Rr_diff, sgTrace->Rt);
						//////////////////
						// THERE WAS A HIT
						//////////////////
						//AiMsgInfo("%i : Diffuse new Hit hitting", myRand);
						int traceValue = -1;

						if(!AiMessageGetIntFunc(sgTrace,"ovt_diffuseFlag",&traceValue))
						{ 
							float adjustedAlpha = (1.0f-cumulativeOpacity)*sample.alpha;
							cumulativeOpacity += adjustedAlpha;
							sg->out.RGBA.r += adjustedAlpha*sample.color.r;
							sg->out.RGBA.g += adjustedAlpha*sample.color.g;
							sg->out.RGBA.b += adjustedAlpha*sample.color.b;
							sg->out.RGBA.a = cumulativeOpacity;

							// update lightPoint and lightColor
							if(data->useLightPropagation)
							{
								// get 
								AtPoint lightPoint;
								AiMessageGetPntFunc (sgTrace,"ovt_lightPointDiffuse", &lightPoint);
								AtColor lightColor = AI_RGB_BLACK;
								AiMessageGetRGBFunc(sgTrace,"ovt_lightColorDiffuse", &lightColor);
								AiMessageSetPntFunc(sg,"ovt_lightPointDiffuse",lightPoint);
								AiMessageSetRGBFunc(sg,"ovt_lightColorDiffuse",lightColor);
							}

							delete sgTrace;

							AiMessageSetIntFunc(sg,"ovt_diffuseFlag",UNKNOWN_IT);
							AiMessageSetIntFunc(sg,"ovt_diffuseDepth",sg->Rr);
							sg->P = currentPoint;
							return;
						}
						else if(traceValue == BACKFACE_IT)
						{						
							// update current position
							currentPoint = sample.point + AI_EPSILON*sg->Rd;
							delete sgTrace;
						}
						else if(traceValue == FRONTFACE_IT)
						{
							int depth=-1;

							float adjustedAlpha = (1.0f-cumulativeOpacity)*sample.alpha;
							cumulativeOpacity += adjustedAlpha;

							sg->out.RGBA.r += adjustedAlpha*sample.color.r;
							sg->out.RGBA.g += adjustedAlpha*sample.color.g;
							sg->out.RGBA.b += adjustedAlpha*sample.color.b;
							sg->out.RGBA.a = cumulativeOpacity;
							currentPoint = sample.point + AI_EPSILON*sg->Rd;
							if(data->useLightPropagation)
							{
								// get 
								AtPoint lightPoint;
								AiMessageGetPntFunc (sgTrace,"ovt_lightPointDiffuse", &lightPoint);
								AtColor lightColor = AI_RGB_BLACK;
								AiMessageGetRGBFunc(sgTrace,"ovt_lightColorDiffuse", &lightColor);
								AiMessageSetPntFunc(sg,"ovt_lightPointDiffuse",lightPoint);
								AiMessageSetRGBFunc(sg,"ovt_lightColorDiffuse",lightColor);
							}
							delete sgTrace;
						}
						else
						{
							int depth=-1;

							float adjustedAlpha = (1.0f-cumulativeOpacity)*sample.alpha;
							cumulativeOpacity += adjustedAlpha;
							sg->out.RGBA.r += adjustedAlpha*sample.color.r;
							sg->out.RGBA.g += adjustedAlpha*sample.color.g;
							sg->out.RGBA.b += adjustedAlpha*sample.color.b;
							sg->out.RGBA.a = cumulativeOpacity;
							currentPoint = sample.point;
							if(data->useLightPropagation)
							{
								// get 
								AtPoint lightPoint;
								AiMessageGetPntFunc (sgTrace,"ovt_lightPointDiffuse", &lightPoint);
								AtColor lightColor = AI_RGB_BLACK;
								AiMessageGetRGBFunc(sgTrace,"ovt_lightColorDiffuse", &lightColor);
								AiMessageSetPntFunc(sg,"ovt_lightPointDiffuse",lightPoint);
								AiMessageSetRGBFunc(sg,"ovt_lightColorDiffuse",lightColor);
							}
							delete sgTrace;

							AiMessageSetIntFunc(sg,"ovt_diffuseFlag",UNKNOWN_IT);
							AiMessageSetIntFunc(sg,"ovt_diffuseDepth",sg->Rr);
							sg->P = currentPoint;
							return;
						}
					}

				}

				if(cumulativeOpacity >= data->accumulatedThicknessThreshold)
				{
					// Special adjustement
					float oneOverOp = ((cumulativeOpacity<=0.0f)?1.0f:1.0f/cumulativeOpacity);
					sg->out.RGBA.r *= oneOverOp;
					sg->out.RGBA.g *= oneOverOp;
					sg->out.RGBA.b *= oneOverOp;
					sg->out.RGBA.a = 1.0;
				}
				
				sg->out.RGBA.a = 1.0;
				
				AiMessageSetIntFunc(sg,"ovt_diffuseFlag", RESET_IT);
				sg->Rr_diff = Rr_diff+1;
				/*sg->out.RGBA.r = 1.0f;
				sg->out.RGBA.g = 0.0f;
				sg->out.RGBA.b = 1.0f;
				sg->out.RGBA.a = 1.0f;*/
			}
			////////////////////////
			// BACKFACE DIFFUSE RAY
			////////////////////////
			else if(backface)
			{
				sg->out.RGBA = AI_RGBA_BLACK; 
				//AiMsgInfo("%i : Diffuse Backface with new sg->Rr = %i and sg->Rr_diff = %i", myRand, sg->Rr, sg->Rr_diff);
				AiMessageSetIntFunc(sg, "ovt_diffuseFlag", BACKFACE_IT);
				AiMessageSetIntFunc(sg, "ovt_diffuseDepth", sg->Rr);
				return;
			}
			else if(flag==FIRST_IT)
			{

				AiMessageSetIntFunc(sg,"ovt_diffuseFlag", FRONTFACE_IT);
				AiMessageSetIntFunc(sg,"ovt_diffuseDepth",sg->Rr);
				//AiMsgInfo("%i : Diffuse FIRST_IT hit with new sg->Rr = %i and sg->Rr_diff = %i", myRand, sg->Rr, sg->Rr_diff);
				// get diffuse
				AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
				bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
				AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

				// Get Alpha
				float alpha = 0.0f;
				switch(data->alphaMode)
				{
				case ALPHA_ALPHA:
					alpha = AiShaderEvalParamFlt(p_alpha);
					break;
				case ALPHA_DIFFUSE:
					alpha = diffuse.a;
					break;
				case ALPHA_EMISSION:
					alpha = emission.a;
					break;
				case ALPHA_MAX:
					alpha = std::max(diffuse.a,emission.a);
					break;
				}

				// Calculate Opacity
				float thickness = 0.0f;
				if(alpha > 0.0f)
					thickness = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));

				//clamp
				if(thickness > 1.0f)
					thickness =1.0f;

				if(thickness <= 0.0f)
				{
					sg->out.RGBA = AI_RGBA_BLACK;
					return;
				}
				else // need to calculate shading
				{

					// Direct Lighting
					AtColor color = AI_RGB_BLACK;

					if(data->diffuseMode != DIFFUSE_EMISSION){

						//check for already calculated lighting
						AtPoint lightPoint;
						bool lightPointCalculated = false;

						if(data->useLightPropagationDiffuse && AiMessageGetPntFunc (sg,"ovt_lightPointDiffuse", &lightPoint))
						{
							// point exists

							// how far is it
							float distance2 = AiV3Dist2(lightPoint,sg->P);
							float propagationMax = data->lightPropagationDistanceDiffuse*data->lightPropagationDistanceDiffuse;

							if(distance2 <= propagationMax)
							{
								//we can use the old lighting
								if(AiMessageGetRGBFunc(sg,"ovt_lightColorDiffuse", &color))
									lightPointCalculated = true;
								else
									color = AI_RGB_BLACK;
							}
						}

						if(!lightPointCalculated)
						{
							addAllDirect(sg,data,kdColorNonZero,&color);

							// indirect
							if ( sg->Rr_diff < data->max_diffuse_depth && kdColorNonZero)
							{
								AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
								AiMessageSetIntFunc(sgTrace,"ovt_diffuseFlag", DIFFUSE_IT);
								AiMessageSetIntFunc(sgTrace,"ovt_diffuseDepth", sg->Rr);
								color += AiIndirectDiffuse(&sg->Nf, sgTrace);
								delete sgTrace;
								// double hemispherical sampling
								if(data->doubleHemisphericalDiffuse)
								{
									// double hemispherical sampling
									AtShaderGlobals  *sgTrace2 = new AtShaderGlobals(*sg);
									AiMessageSetIntFunc(sgTrace2,"ovt_diffuseFlag", DIFFUSE_IT);
									AiMessageSetIntFunc(sgTrace2,"ovt_diffuseDepth", sg->Rr);
									AtPoint tempP = sgTrace2->P;
									sgTrace2->P -= AI_EPSILON*sgTrace2->Ngf;
									sgTrace2->Nf = -sgTrace2->Nf;
									sgTrace2->Ngf = -sgTrace2->Ngf;
									color += AiIndirectDiffuse(&sgTrace2->Nf, sgTrace2);
									delete sgTrace2;
								}
							}
							// update lightPoint and lightColor
							if(data->useLightPropagationDiffuse)
							{
								AiMessageSetPntFunc(sg,"ovt_lightPointDiffuse",sg->P);
								AiMessageSetRGBFunc(sg,"ovt_lightColorDiffuse",color);
							}
						}
					}

					sg->out.RGBA.r = diffuse.r*color.r + emission.r;
					sg->out.RGBA.g = diffuse.g*color.g + emission.g;
					sg->out.RGBA.b = diffuse.b*color.b + emission.b;
					sg->out.RGBA.a = thickness;
					sg->out_opacity = 1.0f; 
					return;
				}
			}
			else
			{
				// get shading
				AiMessageSetIntFunc(sg, "ovt_diffuseFlag", UNKNOWN_IT);
				AiMessageSetIntFunc(sg,"ovt_diffuseDepth",sg->Rr);
				//AiMsgInfo("%i : Diffuse UNKNOWN_IT hit", myRand);
				// get diffuse
				AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
				bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
				AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

				// Get Alpha
				float alpha = 0.0f;
				switch(data->alphaMode)
				{
				case ALPHA_ALPHA:
					alpha = AiShaderEvalParamFlt(p_alpha);
					break;
				case ALPHA_DIFFUSE:
					alpha = diffuse.a;
					break;
				case ALPHA_EMISSION:
					alpha = emission.a;
					break;
				case ALPHA_MAX:
					alpha = std::max(diffuse.a,emission.a);
					break;
				}

				// Calculate Opacity
				float thickness = 0.0f;
				if(alpha > 0.0f)
					thickness = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));

				//clamp
				if(thickness > 1.0f)
					thickness = 1.0f;

				if(thickness <= 0.0f)
				{
					sg->out.RGBA = AI_RGBA_BLACK;
					return;
				}
				else // need to calculate shading
				{

					// Direct Lighting
					AtColor color = AI_RGB_BLACK;

					if(data->diffuseMode != DIFFUSE_EMISSION){

						//check for already calculated lighting
						AtPoint lightPoint;
						bool lightPointCalculated = false;

						if(data->useLightPropagationDiffuse && AiMessageGetPntFunc (sg,"ovt_lightPointDiffuse", &lightPoint))
						{
							// point exists

							// how far is it
							float distance2 = AiV3Dist2(lightPoint,sg->P);
							float propagationMax = data->lightPropagationDistanceDiffuse*data->lightPropagationDistanceDiffuse;

							if(distance2 <= propagationMax)
							{
								//we can use the old lighting
								if(AiMessageGetRGBFunc(sg,"ovt_lightColorDiffuse", &color))
									lightPointCalculated = true;
								else
									color = AI_RGB_BLACK;
							}
						}

						if(!lightPointCalculated)
						{
							addAllDirect(sg,data,kdColorNonZero,&color);

							if ( sg->Rr_diff < data->max_diffuse_depth && kdColorNonZero)
							{
								AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
								AiMessageSetIntFunc(sgTrace,"ovt_diffuseFlag", DIFFUSE_IT);
								AiMessageSetIntFunc(sgTrace,"ovt_diffuseDepth", sg->Rr);
								color += AiIndirectDiffuse(&sg->Nf, sgTrace);
								delete sgTrace;
								// double hemispherical sampling
								if(data->doubleHemisphericalDiffuse)
								{
									// double hemispherical sampling
									AtShaderGlobals  *sgTrace2 = new AtShaderGlobals(*sg);
									AiMessageSetIntFunc(sgTrace2,"ovt_diffuseFlag", DIFFUSE_IT);
									AiMessageSetIntFunc(sgTrace2,"ovt_diffuseDepth", sg->Rr);
									AtPoint tempP = sgTrace2->P;
									sgTrace2->P -= AI_EPSILON*sgTrace2->Ngf;
									sgTrace2->Nf = -sgTrace2->Nf;
									sgTrace2->Ngf = -sgTrace2->Ngf;
									color += AiIndirectDiffuse(&sgTrace2->Nf, sgTrace2);
									delete sgTrace2;
								}
							}

							// update lightPoint and lightColor
							if(data->useLightPropagationDiffuse)
							{
								AiMessageSetPntFunc(sg,"ovt_lightPointDiffuse",sg->P);
								AiMessageSetRGBFunc(sg,"ovt_lightColorDiffuse",color);
							}
						}
					}

					sg->out.RGBA.r = diffuse.r*color.r + emission.r;
					sg->out.RGBA.g = diffuse.g*color.g + emission.g;
					sg->out.RGBA.b = diffuse.b*color.b + emission.b;
					sg->out.RGBA.a = thickness;
					sg->out_opacity = 1.0f; 
					return;
				}
			}
		}
		else if(data->diffuseMode == DIFFUSE_BLACK)
		{
			////////////////////////
			// BLACK DIFFUSE MODE
			////////////////////////

			sg->out.RGBA.r = 0.0f;
			sg->out.RGBA.g = 0.0f;
			sg->out.RGBA.b = 0.0f;
			sg->out.RGBA.a = 1.0f;
			return;
		}
		else
		{
			/////////////////////
			// OTHER DIFFUSE MODE
			/////////////////////
			// get color
			AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
			bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
			AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

			// Get Alpha
			float alpha = 0.0f;
			switch(data->alphaMode)
			{
			case ALPHA_ALPHA:
				alpha = AiShaderEvalParamFlt(p_alpha);
				break;
			case ALPHA_DIFFUSE:
				alpha = diffuse.a;
				break;
			case ALPHA_EMISSION:
				alpha = emission.a;
				break;
			case ALPHA_MAX:
				alpha = std::max(diffuse.a,emission.a);
				break;
			}

			float opacity = 0.0f;
			if(alpha > 0.0f)
				opacity = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));

			//clamp
			if(opacity <= 0.0f)
			{
				// trivial case
				sg->out.RGBA.r = 0.0f;
				sg->out.RGBA.g = 0.0f;
				sg->out.RGBA.b = 0.0f;
				sg->out.RGBA.a = 1.0f;
				return;
			}
			else if(opacity > 1.0f)
				opacity=1.0f;

			AtColor color = AI_RGB_BLACK;

			switch(data->diffuseMode)
			{
			case DIFFUSE_DIRECT:
				addAllDirect(sg,data,kdColorNonZero,&color);
				opacity = 1.0;
				break;
			case DIFFUSE_DIRECTOPACITY:
				addAllDirect(sg,data,kdColorNonZero,&color);
				break;
			case DIFFUSE_DIRECTSQRTOPACITY:
				opacity = std::sqrt(opacity);
				addAllDirect(sg,data,kdColorNonZero,&color);
				break;
			default:
				break;
			} 

			sg->out.RGBA.r = (color.r*diffuse.r + emission.r)*opacity;
			sg->out.RGBA.g = (color.g*diffuse.g + emission.g)*opacity;
			sg->out.RGBA.b = (color.b*diffuse.b + emission.b)*opacity;
			sg->out.RGBA.a = 1.0;
			return;
		}
	}
	////////////////////////////////////////////
	//	AUTO-TRANSPARENCY SHADOW-ONLY OTHER RAYS
	////////////////////////////////////////////
	else
	{
		int flag = -1;
		int depth = -1;
		bool flagSet = AiMessageGetIntFunc(sg,"ovt_flag", &flag);
		bool depthSet = AiMessageGetIntFunc(sg,"ovt_depth", &depth);
		

		if(!flagSet || flag == UNKNOWN_IT || flag == RESET_IT || (flag==FIRST_IT && sg->Rr != depth+1))
		{
			////////////////
			//	NEW HIT
			////////////////
			//AiMsgInfo("%i : other new Hit", myRand);
			AtPoint currentPoint = sg->P + AI_EPSILON*sg->Rd;

			// get diffuse
			AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
			bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
			AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

			// Get Alpha
			float alpha = 0.0f;
			switch(data->alphaMode)
			{
			case ALPHA_ALPHA:
				alpha = AiShaderEvalParamFlt(p_alpha);
				break;
			case ALPHA_DIFFUSE:
				alpha = diffuse.a;
				break;
			case ALPHA_EMISSION:
				alpha = emission.a;
				break;
			case ALPHA_MAX:
				alpha = std::max(diffuse.a,emission.a);
				break;
			}

			// Calculate Opacity
			float cumulativeOpacity = 0.0f;
			if(alpha > 0.0f)
				cumulativeOpacity = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));

			//clamp
			if(cumulativeOpacity > 1.0f)
				cumulativeOpacity=1.0f;
			
			if(cumulativeOpacity <= 0.0f)
			{
				sg->out.RGBA = AI_RGBA_BLACK;
				cumulativeOpacity = 0.0f;
			}
			else // need to calculate shading
			{

				// Direct Lighting
				AtColor color = AI_RGB_BLACK;


				//check for already calculated lighting
				AtPoint lightPoint;
				bool lightPointCalculated = false;

				if(data->useLightPropagation && AiMessageGetPntFunc (sg,"ovt_lightPoint", &lightPoint))
				{
					// point exists

					// how far is it
					float distance2 = AiV3Dist2(lightPoint,sg->P);
					float propagationMax = data->lightPropagationDistance*data->lightPropagationDistance;

					if(distance2 <= propagationMax)
					{
						//we can use the old lighting
						if(AiMessageGetRGBFunc(sg,"ovt_lightColor", &color))
							lightPointCalculated = true;
						else
							color = AI_RGB_BLACK;
					}
				}

				if(!lightPointCalculated)
				{
					addAllDirect(sg,data,kdColorNonZero,&color);

					// Indirect lighting
					if (sg->Rr_diff < data->max_diffuse_depth)
					{
						AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
						AiMessageSetIntFunc(sgTrace,"ovt_diffuseFlag", DIFFUSE_IT);
						AiMessageSetIntFunc(sgTrace,"ovt_diffuseDepth", sg->Rr);
						//AiMsgInfo("%i : Other new Hit indirect 0 with sg->Rr = %i and sg->Rr_diff = %i", myRand, sgTrace->Rr, sgTrace->Rr_diff);
						color += AiIndirectDiffuse(&sg->Nf, sgTrace);
						delete sgTrace;
						// double hemispherical sampling
						if(data->doubleHemisphericalDiffuse)
						{
							// double hemispherical sampling
							AtShaderGlobals  *sgTrace2 = new AtShaderGlobals(*sg);
							AiMessageSetIntFunc(sgTrace2,"ovt_diffuseFlag", DIFFUSE_IT);
							AiMessageSetIntFunc(sgTrace2,"ovt_diffuseDepth", sg->Rr);
							//AiMsgInfo("%i : Other new Hit indirect 1 with sg->Rr = %i and sg->Rr_diff = %i", myRand, sgTrace2->Rr, sgTrace2->Rr_diff);
							AtPoint tempP = sgTrace2->P;
							sgTrace2->P -= AI_EPSILON*sgTrace2->Ngf;
							sgTrace2->Nf = -sgTrace2->Nf;
							sgTrace2->Ngf = -sgTrace2->Ngf;
							color += AiIndirectDiffuse(&sgTrace2->Nf, sgTrace2);
							delete sgTrace2;
						}
					}

					// update lightPoint and lightColor
					if(data->useLightPropagation)
					{
						AiMessageSetPntFunc(sg,"ovt_lightPoint",sg->P);
						AiMessageSetRGBFunc(sg,"ovt_lightColor",color);
					}
				}

				sg->out.RGBA.r = cumulativeOpacity*(diffuse.r*color.r + emission.r);
				sg->out.RGBA.g = cumulativeOpacity*(diffuse.g*color.g + emission.g);
				sg->out.RGBA.b = cumulativeOpacity*(diffuse.b*color.b + emission.b);
				sg->out.RGBA.a = cumulativeOpacity;
				sg->out_opacity = 1.0f;
			}

			//////////////
			// Main Loop
			//////////////
			unsigned long loopIteration = 0;
			while(cumulativeOpacity < data->accumulatedThicknessThreshold){
				// on va lancer un rayon avec un flag FIRST_IT
				AtRay newRay;

				AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
				AiMessageSetIntFunc(sgTrace,"ovt_flag", FIRST_IT);
				AiMessageSetIntFunc(sgTrace,"ovt_depth", sg->Rr);
				
		
				AiMakeRay(&newRay, sg->Rt, &currentPoint, &sg->Rd, AI_INFINITE, sgTrace);
				//AiMsgInfo("%i : Other new Hit tracing", myRand);
				AtScrSample	sample;
				if(!AiTrace(&newRay, &sample))
				{
					////AiMsgInfo("00 : !AiTrace...  tracing background... OTHER_RAYS");
					AiTraceBackground(&newRay,&sample);
					//AiMsgInfo("%i : Other new Hit Background", myRand);
					float adjustedAlpha = (1.0f-cumulativeOpacity)*sample.alpha;
					cumulativeOpacity += adjustedAlpha;
					sg->out.RGBA.r += adjustedAlpha*sample.color.r;
					sg->out.RGBA.g += adjustedAlpha*sample.color.g;
					sg->out.RGBA.b += adjustedAlpha*sample.color.b;
					sg->out.RGBA.a = cumulativeOpacity;
					delete sgTrace;
					break;
				}
				else
				{
					//////////////////
					// THERE WAS A HIT
					//////////////////
					//AiMsgInfo("%i : Other new Hit hitting", myRand);
					int traceValue = -1;
					if(!AiMessageGetIntFunc(sgTrace,"ovt_flag",&traceValue))
					{ 
						float adjustedAlpha = (1.0f-cumulativeOpacity)*sample.alpha;
						cumulativeOpacity += adjustedAlpha;
						sg->out.RGBA.r += adjustedAlpha*sample.color.r;
						sg->out.RGBA.g += adjustedAlpha*sample.color.g;
						sg->out.RGBA.b += adjustedAlpha*sample.color.b;
						sg->out.RGBA.a = cumulativeOpacity;

						// update lightPoint and lightColor
						if(data->useLightPropagation)
						{
							// get 
							AtPoint lightPoint;
							AiMessageGetPntFunc (sgTrace,"ovt_lightPoint", &lightPoint);
							AtColor lightColor = AI_RGB_BLACK;
							AiMessageGetRGBFunc(sgTrace,"ovt_lightColor", &lightColor);
							AiMessageSetPntFunc(sg,"ovt_lightPoint",lightPoint);
							AiMessageSetRGBFunc(sg,"ovt_lightColor",lightColor);
						}

						delete sgTrace;

						AiMessageSetIntFunc(sg,"ovt_flag",UNKNOWN_IT);
						AiMessageSetIntFunc(sg,"ovt_depth",sg->Rr);
						sg->P = currentPoint;
						return;
					}
					else if(traceValue == BACKFACE_IT)
					{						
						// update current position
						currentPoint = sample.point + AI_EPSILON*sg->Rd;
						delete sgTrace;
					}
					else if(traceValue == FRONTFACE_IT)
					{
						int depth=-1;

						float adjustedAlpha = (1.0f-cumulativeOpacity)*sample.alpha;
						cumulativeOpacity += adjustedAlpha;

						sg->out.RGBA.r += adjustedAlpha*sample.color.r;
						sg->out.RGBA.g += adjustedAlpha*sample.color.g;
						sg->out.RGBA.b += adjustedAlpha*sample.color.b;
						sg->out.RGBA.a = cumulativeOpacity;
						currentPoint = sample.point + AI_EPSILON*sg->Rd;

						// update lightPoint and lightColor
						if(data->useLightPropagation)
						{
							// get 
							AtPoint lightPoint;
							AiMessageGetPntFunc (sgTrace,"ovt_lightPoint", &lightPoint);
							AtColor lightColor = AI_RGB_BLACK;
							AiMessageGetRGBFunc(sgTrace,"ovt_lightColor", &lightColor);
							AiMessageSetPntFunc(sg,"ovt_lightPoint",lightPoint);
							AiMessageSetRGBFunc(sg,"ovt_lightColor",lightColor);
						}

						delete sgTrace;
					}
					else
					{
						int depth=-1;

						float adjustedAlpha = (1.0f-cumulativeOpacity)*sample.alpha;
						cumulativeOpacity += adjustedAlpha;
						sg->out.RGBA.r += adjustedAlpha*sample.color.r;
						sg->out.RGBA.g += adjustedAlpha*sample.color.g;
						sg->out.RGBA.b += adjustedAlpha*sample.color.b;
						sg->out.RGBA.a = cumulativeOpacity;
						currentPoint = sample.point;

						// update lightPoint and lightColor
						if(data->useLightPropagation)
						{
							// get 
							AtPoint lightPoint;
							AiMessageGetPntFunc (sgTrace,"ovt_lightPoint", &lightPoint);
							AtColor lightColor = AI_RGB_BLACK;
							AiMessageGetRGBFunc(sgTrace,"ovt_lightColor", &lightColor);
							AiMessageSetPntFunc(sg,"ovt_lightPoint",lightPoint);
							AiMessageSetRGBFunc(sg,"ovt_lightColor",lightColor);
						}

						delete sgTrace;

						AiMessageSetIntFunc(sg,"ovt_flag",UNKNOWN_IT);
						AiMessageSetIntFunc(sg,"ovt_depth",sg->Rr);
						sg->P = currentPoint;
						return;
					}
				}

			}

			if(cumulativeOpacity >= data->accumulatedThicknessThreshold)
			{
				// Special adjustement
				float oneOverOp = ((cumulativeOpacity<=0.0f)?1.0f:1.0f/cumulativeOpacity);
				sg->out.RGBA.r *= oneOverOp;
				sg->out.RGBA.g *= oneOverOp;
				sg->out.RGBA.b *= oneOverOp;
				sg->out.RGBA.a = 1.0;
			}

			AiMessageSetIntFunc(sg,"ovt_flag", RESET_IT);
		}
		///////////////////////////////////////////
		//	AUTO-TRANSPARENCY SHADOW-ONLY BACKFACE
		///////////////////////////////////////////
		else if(sg->Ng != sg->Ngf)
		{
			sg->out.RGBA = AI_RGBA_BLACK; 
			//AiMsgInfo("%i : Other Backface", myRand);
			AiMessageSetIntFunc(sg, "ovt_flag", BACKFACE_IT);
			AiMessageSetIntFunc(sg, "ovt_depth", sg->Rr);
			return;
		}
		else if(flag==FIRST_IT)
		{

			AiMessageSetIntFunc(sg,"ovt_flag", FRONTFACE_IT);
			AiMessageSetIntFunc(sg,"ovt_depth",sg->Rr);
			//AiMsgInfo("%i : Other FIRST_IT hit", myRand);
			// get diffuse
			AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
			bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
			AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

			// Get Alpha
			float alpha = 0.0f;
			switch(data->alphaMode)
			{
			case ALPHA_ALPHA:
				alpha = AiShaderEvalParamFlt(p_alpha);
				break;
			case ALPHA_DIFFUSE:
				alpha = diffuse.a;
				break;
			case ALPHA_EMISSION:
				alpha = emission.a;
				break;
			case ALPHA_MAX:
				alpha = std::max(diffuse.a,emission.a);
				break;
			}

			// Calculate Opacity
			float thickness = 0.0f;
			if(alpha > 0.0f)
				thickness = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));

			//clamp
			if(thickness > 1.0f)
				thickness =1.0f;
			
			if(thickness <= 0.0f)
			{
				sg->out.RGBA = AI_RGBA_BLACK;
				return;
			}
			else // need to calculate shading
			{

				// Direct Lighting
				AtColor color = AI_RGB_BLACK;
				//check for already calculated lighting
				AtPoint lightPoint;
				bool lightPointCalculated = false;

				if(data->useLightPropagation && AiMessageGetPntFunc (sg,"ovt_lightPoint", &lightPoint))
				{
					// point exists

					// how far is it
					float distance2 = AiV3Dist2(lightPoint,sg->P);
					float propagationMax = data->lightPropagationDistance*data->lightPropagationDistance;

					if(distance2 <= propagationMax)
					{
						//we can use the old lighting
						if(AiMessageGetRGBFunc(sg,"ovt_lightColor", &color))
							lightPointCalculated = true;
						else
							color = AI_RGB_BLACK;
					}
				}

				if(!lightPointCalculated)
				{
					addAllDirect(sg,data,kdColorNonZero,&color);

					if ( sg->Rr_diff < data->max_diffuse_depth && kdColorNonZero)
					{
						AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
						AiMessageSetIntFunc(sgTrace,"ovt_diffuseFlag", DIFFUSE_IT);
						AiMessageSetIntFunc(sgTrace,"ovt_diffuseDepth", sg->Rr);
						color += AiIndirectDiffuse(&sg->Nf, sgTrace);
						delete sgTrace;
						// double hemispherical sampling
						if(data->doubleHemisphericalDiffuse)
						{
							// double hemispherical sampling
							AtShaderGlobals  *sgTrace2 = new AtShaderGlobals(*sg);
							AiMessageSetIntFunc(sgTrace2,"ovt_diffuseFlag", DIFFUSE_IT);
							AiMessageSetIntFunc(sgTrace2,"ovt_diffuseDepth", sg->Rr);
							AtPoint tempP = sgTrace2->P;
							sgTrace2->P -= AI_EPSILON*sgTrace2->Ngf;
							sgTrace2->Nf = -sgTrace2->Nf;
							sgTrace2->Ngf = -sgTrace2->Ngf;
							color += AiIndirectDiffuse(&sgTrace2->Nf, sgTrace2);
							delete sgTrace2;
						}
					}
					// update lightPoint and lightColor
					if(data->useLightPropagation)
					{
						AiMessageSetPntFunc(sg,"ovt_lightPoint",sg->P);
						AiMessageSetRGBFunc(sg,"ovt_lightColor",color);
					}
				}

				sg->out.RGBA.r = diffuse.r*color.r + emission.r;
				sg->out.RGBA.g = diffuse.g*color.g + emission.g;
				sg->out.RGBA.b = diffuse.b*color.b + emission.b;
				sg->out.RGBA.a = thickness;
				sg->out_opacity = 1.0f; 
				return;
			}
		}
		else
		{
			// get shading
			AiMessageSetIntFunc(sg, "ovt_flag", UNKNOWN_IT);
			AiMessageSetIntFunc(sg,"ovt_depth",sg->Rr);
			////AiMsgInfo("%i : Other UNKNOWN_IT hit", myRand);
			
			// get diffuse
			AtRGBA diffuse = AiShaderEvalParamRGBA(p_colorIn);
			bool kdColorNonZero = (diffuse.r > 0.0f || diffuse.g > 0.0f || diffuse.b > 0.0f);
			AtRGBA emission = AiShaderEvalParamRGBA(p_emissionColor);

			// Get Alpha
			float alpha = 0.0f;
			switch(data->alphaMode)
			{
			case ALPHA_ALPHA:
				alpha = AiShaderEvalParamFlt(p_alpha);
				break;
			case ALPHA_DIFFUSE:
				alpha = diffuse.a;
				break;
			case ALPHA_EMISSION:
				alpha = emission.a;
				break;
			case ALPHA_MAX:
				alpha = std::max(diffuse.a,emission.a);
				break;
			}

			// Calculate Opacity
			float thickness = 0.0f;
			if(alpha > 0.0f)
				thickness = alpha*(1.0f-std::exp(-AiShaderEvalParamFlt(p_absorption)*(2.0f*AiShaderEvalParamFlt(p_radius)*std::pow(AiV3Dot(sg->Rd,-sg->Nf),AiShaderEvalParamFlt(p_exponent)))));

			//clamp
			if(thickness > 1.0f)
				thickness =1.0f;

			if(thickness <= 0.0f)
			{
				sg->out.RGBA = AI_RGBA_BLACK;
				return;
			}
			else // need to calculate shading
			{

				// Direct Lighting
				AtColor color = AI_RGB_BLACK;

				//check for already calculated lighting
				AtPoint lightPoint;
				bool lightPointCalculated = false;

				if(data->useLightPropagation && AiMessageGetPntFunc (sg,"ovt_lightPoint", &lightPoint))
				{
					// point exists

					// how far is it
					float distance2 = AiV3Dist2(lightPoint,sg->P);
					float propagationMax = data->lightPropagationDistance*data->lightPropagationDistance;

					if(distance2 <= propagationMax)
					{
						//we can use the old lighting
						if(AiMessageGetRGBFunc(sg,"ovt_lightColor", &color))
							lightPointCalculated = true;
						else
							color = AI_RGB_BLACK;
					}
				}

				if(!lightPointCalculated)
				{
					addAllDirect(sg,data,kdColorNonZero,&color);

					if ( sg->Rr_diff < data->max_diffuse_depth && kdColorNonZero)
					{
						AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
						AiMessageSetIntFunc(sgTrace,"ovt_diffuseFlag", DIFFUSE_IT);
						AiMessageSetIntFunc(sgTrace,"ovt_diffuseDepth", sg->Rr);
						color += AiIndirectDiffuse(&sg->Nf, sgTrace);
						delete sgTrace;

						// double hemispherical sampling
						if(data->doubleHemisphericalDiffuse)
						{
							// double hemispherical sampling
							AtShaderGlobals  *sgTrace2 = new AtShaderGlobals(*sg);
							AiMessageSetIntFunc(sgTrace2,"ovt_diffuseFlag", DIFFUSE_IT);
							AiMessageSetIntFunc(sgTrace2,"ovt_diffuseDepth", sg->Rr);
							AtPoint tempP = sgTrace2->P;
							sgTrace2->P -= AI_EPSILON*sgTrace2->Ngf;
							sgTrace2->Nf = -sgTrace2->Nf;
							sgTrace2->Ngf = -sgTrace2->Ngf;
							color += AiIndirectDiffuse(&sgTrace2->Nf, sgTrace2);
							delete sgTrace2;
						}
					}

					// update lightPoint and lightColor
					if(data->useLightPropagation)
					{
						AiMessageSetPntFunc(sg,"ovt_lightPoint",sg->P);
						AiMessageSetRGBFunc(sg,"ovt_lightColor",color);
					}
				}

				sg->out.RGBA.r = diffuse.r*color.r + emission.r;
				sg->out.RGBA.g = diffuse.g*color.g + emission.g;
				sg->out.RGBA.b = diffuse.b*color.b + emission.b;
				sg->out.RGBA.a = thickness;
				sg->out_opacity = 1.0f; 
				return;
			}
		}
	}
}



//node_loader
//{
//	if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqVolumeThicknessMethods;
//	node->output_type  = AI_TYPE_RGBA;
//	node->name         = "Obq_VolumeThickness";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}


