/*
Obq_Atmosphere :

Earth's atmosphere

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

#include "Obq_Atmosphere.h"

// parameters
//
node_parameters
{
	AiParameterRGB("colorKd", 0.5f,0.7f,1.0f);					
	AiParameterBOOL("use3dKd", false);							
	AiParameterFLT("absorption", 0.5f);							
	AiParameterBOOL("use3dAbsorption", false);					
	AiParameterFLT("asymmetry", 0.0f);							
	AiParameterFLT("stepSize",1.0f);							
	AiParameterFLT("stepJitter",1.0f);							
	AiParameterBOOL("useBiasGain",false);						
	AiParameterFLT("bias",0.5f);
	AiParameterFLT("gain",0.5f);
	AiParameterBOOL("useBiasGainDot",false);					
	AiParameterFLT("biasDot",0.5f);
	AiParameterFLT("gainDot",0.5f);
	AiParameterINT("radiusFalloffMode",0);
	AiParameterFLT("falloffStart",0.8f);
	AiParameterFLT("falloffExponent",1.0f);
	AiParameterVEC("objectCenter",0.f,0.f,0.f);
	AiParameterINT("compensateForCloudOpacity",ATMO_COMP_DROP);
	AiParameterINT("divideColorByOpacity",ATMO_BEFORE);
	AiParameterBOOL("useMaximumRayLength",false);					
	AiParameterFLT("maximumRayLength",10.0f);
	AiParameterBOOL("secondaryTypesToo",false);
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
    AiNodeSetLocalData(node,data);
}

node_update
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->falloffMode = AiNodeGetInt(node,"radiusFalloffMode");
	data->secondaryTypesToo = AiNodeGetBool(node,"secondaryTypesToo");
}

node_finish
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	// Shadows not altered ( object must be non-opaque )
	if(sg->Rt == AI_RAY_SHADOW)
	{
		sg->out.RGBA = AI_RGBA_BLACK;
		sg->out_opacity = AI_RGB_BLACK; // fully transparent maybe should traceback

		return;
	}
	else if(sg->Rt == AI_RAY_CAMERA || (data->secondaryTypesToo && (sg->Rt == AI_RAY_REFLECTED || sg->Rt == AI_RAY_REFRACTED || sg->Rt == AI_RAY_GLOSSY)))
	{
		// backface ?
		if(sg->Ng != sg->Ngf)
		{
			int atmo = -1;
			if(AiMessageGetIntFunc(sg,"ObqAtmo", &atmo) && atmo == ATMO_FIRST)
				AiMessageSetIntFunc(sg,"ObqAtmo",ATMO_BF);

			sg->out.RGBA = AI_RGBA_BLACK;
			sg->out_opacity = AI_RGB_BLACK; // hit with epsilon opacity to receive hit event
			return;
		}
		else if(-AiV3Dot(sg->Ngf,sg->Rd) < AI_EPSILON) // silhouette sometimes white hack
		{
			int atmo = -1;
			if(AiMessageGetIntFunc(sg,"ObqAtmo", &atmo) && atmo == ATMO_FIRST)
				AiMessageSetIntFunc(sg,"ObqAtmo",ATMO_BF);

			sg->out.RGBA = AI_RGBA_BLACK;
			sg->out_opacity = AI_RGB_BLACK; // hit with epsilon opacity to receive hit event
			return;
		}
		else
		{
			///////////////////////
			// FRONTFACE CAMERA RAY
			///////////////////////
			ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

			//////////////////
			// RADIUS FALLOFF
			//////////////////
			AtVector objectCenter = AI_V3_ZERO;
			bool useRadiusFalloff = false;
			float objectRadius = 1.0f;
			float falloffStart = 1.0f;
			float falloffMax = 1.0f;
			float falloffExponent = 1.0f;
			AtMatrix transforms;
			bool use3dabsorption = AiShaderEvalParamBool(p_use3dAbsorption);
			bool use3dKd = AiShaderEvalParamBool(p_use3dKd);
			if(use3dabsorption || use3dKd || data->falloffMode==RFM_MATRIX)
				AiNodeGetMatrix(sg->Op, "matrix", transforms);

			switch(data->falloffMode)
			{
			case RFM_MATRIX: // for this to work you have to set Arnold options : preserve_scene_data true
				{
				//AtMatrix transforms;
				//AiNodeGetMatrix(sg->Op, "matrix", transforms); did before
				objectCenter.x = transforms[3][0];
				objectCenter.y = transforms[3][1];
				objectCenter.z = transforms[3][2];
				useRadiusFalloff = true;
				break;
				}
			case RFM_VECTOR:
				objectCenter = AiShaderEvalParamVec(p_objectCenter);
				useRadiusFalloff = true;
				break;
			default:
				break;
			}

			// Update variables
			if(useRadiusFalloff)
			{
				objectRadius = AiV3Dist(sg->P,objectCenter);
				falloffStart = AiShaderEvalParamFlt(p_falloffStart);
				falloffMax = 1.0f-falloffStart;
				falloffExponent = AiShaderEvalParamFlt(p_falloffExponent);
				if(falloffStart == 1.0f || falloffExponent == 0.0f)
					useRadiusFalloff = false;
			}


			/////////////////////////
			//	DOT BIAS GAIN
			////////////////////////
			float dotBias = 1.0f;
			if( AiShaderEvalParamBool(p_useBiasGainDot))
			{
				
				// remap opacity based on dot product
				float bias = AiShaderEvalParamFlt(p_biasDot);
				float gain = 1.0f-AiShaderEvalParamFlt(p_gainDot);


				float t = AiV3Dot(sg->Nf,-sg->Rd);
				
				// test fo nans
				if(t>0.0)
					 CLAMP(t,0.0f,1.0f);
				else
					t=0.0f;

				// Bias if != 0.5
				if(bias !=0.5f)
					t = BIAS(t,bias);

				// Gain if !=0.5
				if(gain != 0.5f)
					t = GAIN(t,gain);

				dotBias = t;
			}


			// jitter
			float stepSize = AiShaderEvalParamFlt(p_stepSize);
			float stepJitter = 2.0f*AiShaderEvalParamFlt(p_stepJitter);
			bool useJitter = (stepJitter>0);


			// INIT output
			sg->out.RGBA = AI_RGBA_BLACK;
			sg->out_opacity = AI_RGB_BLACK;
			float maxAccumulatedOpacity = 0.99f; // could be an exposed threshold
			float cumulativeOpacity = 0.0;

			// while we hit object
			bool continueTracing = true;

			// Origin for the start
			AtPoint originProbe = sg->P + AI_EPSILON*sg->Rd;

			// compensate for non-opaque object
			int compensateForCloudOpacity = AiShaderEvalParamInt(p_compensateForCloudOpacity);



			/////////////////////////////////////////////////
			// While we continue tracing for marching section
			/////////////////////////////////////////////////
			bool useMaxRayLength = AiShaderEvalParamBool(p_useMaximumRayLength);
			float maxRayLength =  useMaxRayLength?AiShaderEvalParamFlt(p_maximumRayLength):float(AI_INFINITE);

			while(continueTracing)
			{

				// send probe to know where the next hit occurs
				AtShaderGlobals* sgProbe = new AtShaderGlobals(*sg);
				AtRay rayProbe;
				AiMakeRay(&rayProbe, AI_RAY_CAMERA, &originProbe, &sg->Rd, AI_INFINITE, sgProbe);
				
				// if such an intersection
				if(AiTraceProbe(&rayProbe,sgProbe))
				{
					////////////////////////
					// PROBE HIT
					////////////////////////
					
					// get shading info
					AtColor kd = AiShaderEvalParamRGB(p_colorKd);
					float asymmetry = AiShaderEvalParamFlt(p_asymmetry);

					// distance to next intersection
					float distance = (float)sgProbe->Rl;
					
					// maximum rayLength
					if(useMaxRayLength){
						distance = std::min(distance, maxRayLength);
						maxRayLength-=distance;
					}

					AtPoint originTrace = originProbe;

					// Create the shaderglobals for marching
					AtShaderGlobals  *sgMarch = new AtShaderGlobals(*sg);

					// isotrope shading
					sgMarch->Nf.x = 0.0f; sgMarch->Nf.y = 0.0f; sgMarch->Nf.z = 0.0f;
					sgMarch->P = originTrace; // the current point is the first point

					// thickness of each step
					float absorption = AiShaderEvalParamFlt(p_absorption);
					float k = 1.55f*asymmetry-0.55f*asymmetry*asymmetry*asymmetry;

					/////////////////
					// marching loop
					/////////////////
					while(distance >= AI_EPSILON)
					{
						AtColor c = AI_RGB_BLACK;

						//////////////////////
						// RECALCULATE RADIUS
						//////////////////////
						float radiusFalloff = 1.0f;
						if(useRadiusFalloff)
						{
							float dist = AiV3Dist(sgMarch->P, objectCenter);
							float ratio = dist/objectRadius;
							CLAMP(ratio,0.0f,1.0f);

							if(ratio > falloffStart)
							{
								float t = (ratio-falloffStart)/falloffMax;
								radiusFalloff = std::pow(1.0f-t,falloffExponent);
							}

						}
						/////////////////////
						// marching step size
						/////////////////////
						float dz =  stepSize + (useJitter ? ((float(rand()) / float(RAND_MAX)) - 0.5f) * stepJitter * stepSize : 0.f);
						CLAMP(dz,AI_EPSILON,distance); // at least epsilon

						// Calculate thickness of this step
						if(use3dabsorption)
						{ // in 3d ?
							AiM4PointByMatrixMult(&sgMarch->Po,transforms,&sgMarch->P);
							absorption = AiShaderEvalParamFuncFlt(sgMarch,node,p_absorption);
						}
						if(use3dKd)
						{
							if(!use3dabsorption)
								AiM4PointByMatrixMult(&sgMarch->Po,transforms,&sgMarch->P);
							kd = AiShaderEvalParamFuncRGB(sgMarch,node,p_colorKd);
						}

						float thickness = radiusFalloff*(1.0f-std::exp(-dz*absorption))*dotBias;
						CLAMP(thickness,0.0f,1.0f);

						//calculate lighting
						if(thickness > 0.0f)
						{
							AiLightsPrepare(sgMarch);
							while (AiLightsGetSample(sgMarch))
							{
								if(AiLightGetAffectDiffuse(sgMarch->Lp))
								{
									float kcostheta = k*AiV3Dot(sgMarch->Rd,sgMarch->Ld);
									float ph = c_1Over4Pi*(1.f-k*k)/((1.f-kcostheta)*(1.f-kcostheta));
									c += sgMarch->we*sgMarch->Li*ph*kd;
								}
							}

							// all diffuse given
							float adjustedAlpha = (1.0f-cumulativeOpacity)*thickness;
							cumulativeOpacity += adjustedAlpha;
							sg->out.RGBA.r += adjustedAlpha*c.r;
							sg->out.RGBA.g += adjustedAlpha*c.g;
							sg->out.RGBA.b += adjustedAlpha*c.b;

							// Check for compensation mode
							if(compensateForCloudOpacity == ATMO_COMP_RAISE)
								sg->out_opacity += adjustedAlpha*AI_RGB_WHITE;	
							else
								sg->out_opacity = cumulativeOpacity*AI_RGB_WHITE;

							// check if already to 1
							if(cumulativeOpacity >= maxAccumulatedOpacity)
							{
								continueTracing = false;
								break;
							}
						}

						// check for remaining distance
						distance-=dz;
						if(distance < AI_EPSILON) // saves another iteration
							break;
						
						// go to next point
						sgMarch->P += dz*sgProbe->Rd;

					}// End marching loop

					// delete sgMarch
					delete sgMarch;
					
					// get the distance to the next intersection
					double max_distance = sgProbe->Rl+AI_EPSILON;
					
					////////////////////////////////
					// NOW TRACE TO GET opacity hit
					///////////////////////////////
					AtRay rayTrace;
					AtShaderGlobals  *sgTrace = new AtShaderGlobals(*sg);
					AiMessageSetIntFunc(sgTrace,"ObqAtmo",ATMO_FIRST);
					AiMakeRay(&rayTrace, AI_RAY_CAMERA, &originProbe, &sg->Rd, max_distance, sgTrace);
					AtScrSample	sample;
				

					// If hit nothing, means opacity == 0
					if(!AiTrace(&rayTrace,&sample))
					{

						// if we just hit the back face, stop after this iteration
						int atmo = ATMO_FIRST;
						if( AiMessageGetIntFunc(sgTrace,"ObqAtmo",&atmo) && atmo==ATMO_BF)
							continueTracing = false;

					}
					else if( sample.opacity == AI_RGB_WHITE || maxRayLength < AI_EPSILON )
					{
						continueTracing = false; // hit hard surface so even with cloud compensation, it stops	
					}
					else
					{
						// check for compensation
						if(compensateForCloudOpacity == ATMO_COMP_DROP)
							maxAccumulatedOpacity -= (1.0f-cumulativeOpacity)*sample.opacity.r;
						else if(compensateForCloudOpacity == ATMO_COMP_RAISE)
							cumulativeOpacity += (1.0f-cumulativeOpacity)*sample.opacity.r;

						// check for cumulative max
						if( cumulativeOpacity >= maxAccumulatedOpacity)
							continueTracing = false;
					}

					delete sgTrace;	

				}
				else
				{
					// probe didnot hit anything which is weird or infinite
					continueTracing = false;
					sg->out_opacity = AI_RGB_BLACK;
					sg->out.RGBA = AI_RGBA_BLACK;
				}

				originProbe = sgProbe->P + AI_EPSILON*sg->Rd;

				delete sgProbe;
				
			} // end continueTracing
			
			/////////////////////////
			// POST EFFECT BIAS GAIN
			////////////////////////

			// remap rgb
			int divideMode = AiShaderEvalParamInt(p_divideColorByOpacity) ;

			if(divideMode == ATMO_BEFORE)
			{
				
				if(sg->out_opacity.r > AI_EPSILON)
				{
					sg->out.RGBA.r /= sg->out_opacity.r;
					sg->out.RGBA.g /= sg->out_opacity.r;
					sg->out.RGBA.b /= sg->out_opacity.r;
				}
				else
				{
					sg->out.RGBA = AI_RGBA_BLACK;
					sg->out_opacity = AI_RGB_BLACK;
				}
			}


			///////////////////////////
			//	FINAL THICKNESS CONTROL
			///////////////////////////

			if( AiShaderEvalParamBool(p_useBiasGain) )
			{

				// remap opacity based on dot product
				float bias = AiShaderEvalParamFlt(p_bias);
				float gain = 1.0f-AiShaderEvalParamFlt(p_gain);


				float t = sg->out_opacity.r;

				// Bias if != 0.5
				if(bias !=0.5f)
					t = BIAS(t,bias);

				// Gain if !=0.5
				if(gain != 0.5f)
					t = GAIN(t,gain);

				sg->out_opacity = t*AI_RGB_WHITE;
			}

	
			///////////////
			// DIVIDE AFTER
			///////////////
			if(divideMode == ATMO_AFTER)
			{
				
				if(sg->out_opacity.r > AI_EPSILON)
				{
					sg->out.RGBA.r /= sg->out_opacity.r;
					sg->out.RGBA.g /= sg->out_opacity.r;
					sg->out.RGBA.b /= sg->out_opacity.r;
				}
				else
				{
					sg->out.RGBA = AI_RGBA_BLACK;
					sg->out_opacity = AI_RGB_BLACK;
				}
			}

				sg->out.RGBA.a = 1.0f;
		}
		
	}
	else
	{
		// fully transparent for other types of ray
		sg->out.RGBA = AI_RGBA_BLACK;
		sg->out_opacity = AI_RGB_BLACK;
	}

}

//node_loader
//{
//	if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqAtmosphereMethods;
//	node->output_type  = AI_TYPE_RGBA;
//	node->name         = "Obq_Atmosphere";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}

