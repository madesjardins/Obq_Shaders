/*
Obq_MeasuredMaterial v2.09.0 (SItoA 2.9.0 - Arnold 4.0.15.1) :


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

#include "Obq_MeasuredMaterial.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqMeasuredMaterialSimpleMethods);

// enum for parameters
//
enum ObqMeasuredMaterialSimpleParams { p_filename, p_resolution, p_samples, p_direct_color, p_direct_scale,p_indirect_color, p_indirect_scale,p_useImportanceSampling, p_useHalfs, p_useJitters, p_binMaxDif, p_useMIS, p_direct_aov, p_indirect_aov};

// shader data struct
//
typedef struct 
{
	ObqISBrdf* brdf;
	int samples;
	bool useImportanceSampling;
	AtSampler *sampler;		// 2
	bool useJitters;
	int binMaxDif;
	bool useMIS;
	int Rr;
	int Rr_diff;
	const char* direct_aov;
	const char* indirect_aov;

}
ShaderData;

node_parameters
{
	AiParameterSTR("filename","");
	AiParameterINT("resolution", 91);
	AiParameterINT("samples", 4);
	AiParameterRGB("direct_color",1.0f,1.0f,1.0f);
	AiParameterFLT("direct_scale", 1.0f);
	AiParameterRGB("indirect_color",1.0f,1.0f,1.0f);
	AiParameterFLT("indirect_scale", 1.0f);
	AiParameterBOOL("useImportanceSampling", true);
	AiParameterBOOL("useHalfs",false);
	AiParameterBOOL("useJitters",false);
	AiParameterINT("binMaxDif",5);
	AiParameterBOOL("useMIS",true);
	AiParameterSTR("direct_AOV","");
	AiParameterSTR("indirect_AOV","");

}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	data->brdf = NULL;
	data->sampler = NULL;
	AiNodeSetLocalData(node,data);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	data->useImportanceSampling = params[p_useImportanceSampling].BOOL;
	data->useMIS = params[p_useMIS].BOOL;

	if(data->brdf != NULL)
		delete data->brdf;

	AiSamplerDestroy(data->sampler);
	data->samples = params[p_samples].INT;
	data->sampler = AiSampler(data->samples, 2);
	data->useJitters = params[p_useJitters].BOOL;
	data->binMaxDif = params[p_binMaxDif].INT;
	data->direct_aov = params[p_direct_aov].STR;
	data->indirect_aov = params[p_indirect_aov].STR;
	AtNode* options = AiUniverseGetOptions();
	data->Rr = AiNodeGetInt(options,"GI_total_depth");
	data->Rr_diff = AiNodeGetInt(options, "GI_diffuse_depth");

	data->brdf = new ObqISBrdf(params[p_filename].STR, static_cast<AtUInt32>(std::max(params[p_resolution].INT,3)),params[p_useHalfs].BOOL, USELUMINANCE);
}

node_finish
{
	// Desallocate shader data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	delete data->brdf;
	AiSamplerDestroy(data->sampler);
	AiFree(data);
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

		AtVector omega_o = -sg->Rd;
		AtVector V = sg->Nf;
		AtVector W;
		if( std::abs(AiV3Dot(V,omega_o)) < 1.0f-AI_EPSILON) // ok to make cross
			W = AiV3Normalize(AiV3Cross(omega_o,V));
		else
		{
			// make an arbitrary but perpendicular vector to V (isotrope anyway)
			W.x = V.z - V.y;
			W.y = V.x - V.z;
			W.z = V.y - V.x;
			W = AiV3Normalize(W);
		}

		AtVector U = AiV3Normalize(AiV3Cross(V,W));

		float cosTheta_o = std::min(std::abs(AiV3Dot(omega_o,V)),1.0f);
		float theta_o = std::acos(cosTheta_o);

		///////////////////////
		// DIRECT SAMPLING
		//////////////////////
		AtColor direct_scale = AiShaderEvalParamRGB(p_direct_color)*AiShaderEvalParamFlt(p_direct_scale);
		AtColor direct_loop_color = AI_RGB_BLACK;

		if(!AiColorIsSmall( direct_scale ))
		{
			bool hemi = sg->fhemi;
			sg->fhemi = true;

			MeasuredBRDF mBrdf(data->brdf,-sg->Rd,U,V,W,sg->Ng,data->binMaxDif,data->useJitters);

			AiLightsPrepare(sg);

			if(data->useMIS)
			{
				while(AiLightsGetSample(sg))
				{
					if(AiV3Dot(sg->Ld,V) > 0.0f)
						direct_loop_color += AiEvaluateLightSample(sg, &mBrdf, MeasuredBRDFEvalSample, MeasuredBRDFEvalBrdf, MeasuredBRDFEvalPdf);
				}
			}
			else
			{
				while(AiLightsGetSample(sg))
				{
					// Get the spherical angles of the Light direction relative to the hair fibre
					AtVector omega_i = sg->Ld;
					float cosTheta_i = std::min(std::abs(AiV3Dot(omega_i,V)),1.0f);
					if(cosTheta_i > 0.0f)
					{
						float theta_i = std::acos(cosTheta_i);

						float deltaPhi = calculateDeltaPhi(omega_i,omega_o,V,cosTheta_i,cosTheta_o);

						AtColor lightC = sg->Li*sg->we*cosTheta_i;

						AtColor brdf_value = data->brdf->getDataInterpolated(theta_i,theta_o,deltaPhi);

						direct_loop_color +=	brdf_value*lightC;
					}
				}
			}

			sg->fhemi = hemi; // put hemispherical thigny back
			
			// direct scaling
			direct_loop_color*=direct_scale;
		}


		///////////////////////
		// INDIRECT SAMPLING
		//////////////////////
		AtColor indirect_scale = AiShaderEvalParamRGB(p_indirect_color)*AiShaderEvalParamFlt(p_indirect_scale);
		AtColor indirect_loop_color = AI_RGB_BLACK;

		if(data->samples > 0 && !AiColorIsSmall(indirect_scale) && (sg->Rr < data->Rr && sg->Rr_diff < data->Rr_diff))
		{

			AtSamplerIterator* iter = AiSamplerIterator(data->sampler, sg);
			double rnd[2];

			int badPdf=0;

			while(AiSamplerGetSample(iter, rnd)) 
			{

				AtVector dir;
				float pdf = 1.0f;
				AtColor brdf_value;

				if(data->useImportanceSampling)
				{
					data->brdf->getSample(brdf_value,dir,pdf,-sg->Rd,U,V,W,rnd[0],data->binMaxDif,data->useJitters);
				}
				else
				{
					pdf = c_1Over2Pi; // uniform
					uniformDirOnHemisphere(dir,rnd);
					AiV3RotateToFrame(dir,U,V,W);
				}

				if (pdf > AI_EPSILON)
				{	

					AtScrSample sample_diffuse;
					AtRay ray;
					AiMakeRay(&ray, AI_RAY_DIFFUSE, &sg->P, &dir, AI_BIG, sg);

					bool bg = false;

					if(!AiTrace(&ray, &sample_diffuse))
					{
						AiTraceBackground(&ray, &sample_diffuse);
						bg=true;
					}


					// trivial sample reject
					if(!AiColorIsSmall(sample_diffuse.color))
					{
						AtVector omega_i = dir;
						float cosTheta_i = std::min(std::abs(AiV3Dot(omega_i,V)),1.0f);
						float theta_i = std::acos(cosTheta_i);

						float deltaPhi = calculateDeltaPhi(omega_i,omega_o,V,cosTheta_i,cosTheta_o);

						AtColor lightC = sample_diffuse.color*std::abs(cosTheta_i)/pdf;

						if(AiColorCorrupted(lightC))
							lightC = AI_RGB_BLACK;

						// get BRDF value
						if(!data->useImportanceSampling)
							brdf_value = data->brdf->getDataInterpolated(theta_i,theta_o,deltaPhi);

						indirect_loop_color+=brdf_value*lightC;

					}
					else
					{
						++badPdf;
					}
				}
			}

			indirect_scale*= (1.0f/(static_cast<float>(std::max(1,AiSamplerGetSampleCount(iter)-badPdf))));

			indirect_loop_color*=indirect_scale;
		}

		sg->out.RGB = direct_loop_color + indirect_loop_color;
		sg->out_opacity = AI_RGB_WHITE; // TODO : Manage opacity
	}
}

//node_loader
//{
//	if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqMeasuredMaterialSimpleMethods;
//	node->output_type  = AI_TYPE_RGB;
//	node->name         = "Obq_MeasuredMaterial";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}


