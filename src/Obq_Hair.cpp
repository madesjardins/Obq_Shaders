/*
Obq_Hair :

Implementation of Ou et al. ISHair

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

#include "Obq_Hair.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqHairMethods);


node_parameters
{
	AiParameterRGB("opacity",1.0f,1.0f,1.0f);
	AiParameterFLT("alpha_R",-7.5f);
	AiParameterFLT("alpha_TT",3.75f);
	AiParameterFLT("alpha_TRT",11.25f);
	AiParameterFLT("beta_R",7.5f);
	AiParameterFLT("beta_TT",3.75f);
	AiParameterFLT("beta_TRT",15.0f);
	AiParameterRGB("color_R",1.0f,1.0f,1.0f);
	AiParameterRGB("color_TT",1.0f,0.867f,0.667f);
	AiParameterRGB("color_TRT",1.0f,0.8f,0.5f);
	AiParameterRGB("color_g",1.0f,0.733f,0.333f);
	AiParameterFLT("gamma_TT",7.5f);
	AiParameterFLT("gamma_g",17.5f);
	AiParameterFLT("phi_g",37.5f);
	AiParameterFLT("scale_R",0.154f);
	AiParameterFLT("scale_TT",0.461f);
	AiParameterFLT("scale_TRT",0.077f);
	AiParameterFLT("scale_g",0.308f);
	AiParameterFLT("alpha_R__pre",-7.5f);
	AiParameterFLT("alpha_TT__pre",3.75f);
	AiParameterFLT("alpha_TRT__pre",11.25f);
	AiParameterFLT("beta_R__pre",7.5f);
	AiParameterFLT("beta_TT__pre",3.75f);
	AiParameterFLT("beta_TRT__pre",15.0f);
	AiParameterRGB("color_R__pre",1.0f,1.0f,1.0f);
	AiParameterRGB("color_TT__pre",1.0f,0.867f,0.667f);
	AiParameterRGB("color_TRT__pre",1.0f,0.8f,0.5f);
	AiParameterRGB("color_g__pre",1.0f,0.733f,0.333f);
	AiParameterFLT("gamma_TT__pre",7.5f);
	AiParameterFLT("gamma_g__pre",17.5f);
	AiParameterFLT("phi_g__pre",37.5f);
	AiParameterFLT("scale_R__pre",0.154f);
	AiParameterFLT("scale_TT__pre",0.461f);
	AiParameterFLT("scale_TRT__pre",0.077f);
	AiParameterFLT("scale_g__pre",0.308f);
	AiParameterINT("samples", 4);
	AiParameterFLT("scale_direct",1.0f);
	AiParameterRGB("color_direct",1.0f,1.0f,1.0f);
	AiParameterFLT("scale_indirect",1.0f);
	AiParameterRGB("color_indirect",1.0f,1.0f,1.0f);
	AiParameterFLT("alpha_back",0.0f);
	AiParameterFLT("beta_back",0.0f);
	AiParameterFLT("scale_back",1.0f);
	AiParameterRGB("color_back",1.0f,1.0f,1.0f);
	AiParameterFLT("scale_forward",1.0f);
	AiParameterRGB("color_forward",1.0f,1.0f,1.0f);
	AiParameterBOOL("use_pre_only",true);
	AiParameterRGB("shadow_opacity",1.0f,1.0f,1.0f);
	AiParameterFLT("numGMS_modifier",1.0f);
	AiParameterBOOL("normal_shifts_widths",true);
	AiParameterRGB("color_ray_glossy",0.0f,0.0f,0.0f);
	AiParameterINT("mode_ray_glossy",1);
	AiParameterRGB("color_ray_diffuse",0.0f,0.0f,0.0f);
	AiParameterINT("mode_ray_diffuse",1);
	AiParameterFLT("d_f",0.7f);
	AiParameterFLT("d_f__pre",0.7f);
	AiParameterFLT("d_b",0.7f);
	AiParameterFLT("d_b__pre",0.7f);
	AiParameterINT("aov_mode",1);
	AiParameterBOOL("singleScatteringMode",true);
	AiParameterBOOL("useImportanceSampling",true);
	AiParameterBOOL("unlinkOpacity",false);
	AiParameterRGB("color_ray_reflected",0.0f,0.0f,0.0f);
	AiParameterINT("mode_ray_reflected",3);
	AiParameterRGB("color_ray_refracted",0.0f,0.0f,0.0f);
	AiParameterINT("mode_ray_refracted",3);
	AiParameterBOOL("checkShaderNameToo",false);
	//AiParameterBOOL("useCritical",false);
	AiParameterINT("modeMIS",0);
	AiParameterINT("MISrandomLobeSamples",1);
	AiParameterINT("samples_R",0);
	AiParameterINT("samples_TT",0);
	AiParameterINT("samples_TRT",0);
	AiParameterINT("samples_G",0);
	AiParameterINT("samples_DS",0);
	AiParameterINT("modeMIS_DS",0);
	AiParameterSTR("aov_dir_R","");
	AiParameterSTR("aov_dir_TRT","");
	AiParameterSTR("aov_dir_g","");
	AiParameterSTR("aov_dir_TT","");
	AiParameterSTR("aov_dir_back","");
	AiParameterSTR("aov_dir_scatter","");
	AiParameterSTR("aov_dir_scatter_back","");
	AiParameterSTR("aov_indir_R","");
	AiParameterSTR("aov_indir_TRT","");
	AiParameterSTR("aov_indir_g","");
	AiParameterSTR("aov_indir_TT","");
	AiParameterSTR("aov_indir_back","");
	AiParameterSTR("aov_indir_scatter","");
	AiParameterSTR("aov_indir_scatter_back","");
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	AiNodeSetLocalData(node,data);
	data->sampler = NULL;
	//AiCritSecInit(&data->criticalSec);
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	//Precalculation
	data->hairData.evaluateParams(node);
	data->hairData.precalculations();

	AiSamplerDestroy(data->sampler);
	data->samples = data->hairData.get_n_samples();
	data->sampler = AiSampler(data->samples, 2);

	data->aov_mode = AiNodeGetInt(node,"aov_mode");

	AtNode *options = AiUniverseGetOptions();
	
	data->max_diffuse_depth = AiNodeGetInt(options, "GI_diffuse_depth");
	data->max_glossy_depth  = AiNodeGetInt(options, "GI_glossy_depth");


	data->aov_dir_R = AiNodeGetStr(node,"aov_dir_R");
	data->aov_dir_TRT = AiNodeGetStr(node,"aov_dir_TRT");
	data->aov_dir_g = AiNodeGetStr(node,"aov_dir_g");
	data->aov_dir_TT = AiNodeGetStr(node,"aov_dir_TT");
	data->aov_dir_back = AiNodeGetStr(node,"aov_dir_back");
	data->aov_dir_scatter = AiNodeGetStr(node,"aov_dir_scatter");
	data->aov_dir_scatter_back = AiNodeGetStr(node,"aov_dir_scatter_back");
	data->aov_indir_R = AiNodeGetStr(node,"aov_indir_R");
	data->aov_indir_TRT = AiNodeGetStr(node,"aov_indir_TRT");
	data->aov_indir_g = AiNodeGetStr(node,"aov_indir_g");
	data->aov_indir_TT = AiNodeGetStr(node,"aov_indir_TT");
	data->aov_indir_back = AiNodeGetStr(node,"aov_indir_back");
	data->aov_indir_scatter = AiNodeGetStr(node,"aov_indir_scatter");
	data->aov_indir_scatter_back = AiNodeGetStr(node,"aov_indir_scatter_back");

	srand (static_cast<unsigned int>(time(NULL)));
}

node_finish
{
	// Desallocate shader data
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	AiSamplerDestroy(data->sampler);
	//AiCritSecClose(&data->criticalSec);
	AiFree(data);
}

shader_evaluate
{
	// Do opacity and early-out if possible

	// Actual opacity of hair for early outs
	AtRGB opacity = AiShaderEvalParamRGB(p_opacity);
	float geo_opacity = 1.0f;
	if (AiUDataGetFlt("geo_opacity", &geo_opacity))
    {
        opacity *= geo_opacity;
    }

	if(AiColorIsSmall(opacity))
	{
		sg->out_opacity = AI_RGB_BLACK;
		return;
	}
	
	ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);
	
	bool selfRay = false;
	if(sg->Rt != AI_RAY_CAMERA)
	{
		selfRay = (sg->shader == sg->psg->shader);
		if(selfRay == false && data->hairData.get_checkShaderNameToo())	// Check name of the shader too
		{
			selfRay = (strstr(AiNodeGetName(sg->psg->shader),"Obq_Hair")!=NULL);
		}
	}

	// We need to know if this is from AiEvaluateLightSample

	//----------------
	// SHADOW RAYS
	if(sg->Rt & AI_RAY_SHADOW)
	{
	

		// back face is always fully transparent
		if(AiV3Dot(sg->Rd,sg->N) >= -AI_EPSILON)
		{
			sg->out_opacity = AI_RGB_BLACK;
			return;
		}

		//----------
		// SELF RAY
		if(selfRay)
		{

			// test variable
			int gm = 0;

			char lightNameMode[64]; // you should make sure this is large enough
#ifdef _WIN32
			sprintf_s(lightNameMode, "ObqH_SSM_%u_%u",sg->psg->transp_index,sg->psg->Rr);
#else
			sprintf(lightNameMode, "ObqH_SSM_%u_%u",sg->psg->transp_index,sg->psg->Rr);
#endif
			

			//------------------------------
			//	Self-shadow Scattering mode
			if(AiStateGetMsgInt(lightNameMode, &gm))
			{
				if(gm == 1) // in Shadow scattering mode AiLightsPrepare
				{
					AtUInt32* counterArray;

					// Check if a previous message passing was created with that light
					char lightName[96]; // you should make sure this is large enough

#ifdef _WIN32
			sprintf_s(lightName, "ObqH_%p_%u_%u",sg->psg->Lp, sg->psg->transp_index, sg->psg->Rr); // safer to use snprintf...
#else
			sprintf(lightName, "ObqH_%p_%u_%u",sg->psg->Lp, sg->psg->transp_index, sg->psg->Rr); // safer to use snprintf...
#endif

					
					
					// Was it previously created or not
					if(AiStateGetMsgPtr(lightName,(void**)(&counterArray)))
					{
						
						if(counterArray!=NULL)
						{
							counterArray[sg->psg->i]++;// get the value associated with this sample
						}
					}
					else
					{
						counterArray = new AtUInt32[sg->psg->n]();				// AiArrayAllocate(, 1, AI_TYPE_UINT);	// create the array
						counterArray[sg->psg->i] = 1;							//AiArraySetUInt(counterArray, sg->psg->i,1);						// set this sample value to 1
						AiStateSetMsgPtr(lightName,counterArray);				// set pointer to light hex string message
						
					}

					sg->out_opacity = AI_RGB_BLACK;
					return;
				}
				else if(gm == 2) // During MIS AiEvaluateLightSample EDITED : since they are called one sample at a time, no need to keep an array
				{
					// Get the pointer to the MISShadowCounting
					char lightName[96]; // you should make sure this is large enough
#ifdef _WIN32
					sprintf_s(lightName, "ObqHm_%u_%u",sg->psg->transp_index, sg->psg->Rr); // safer to use snprintf...
#else
					sprintf(lightName, "ObqHm_%u_%u",sg->psg->transp_index, sg->psg->Rr); // safer to use snprintf...
#endif

					

					// Was it previously created or not
					AtUInt32* count;
					if(AiStateGetMsgPtr(lightName,(void**)(&count)))
					{
						if(count!=NULL)
						{
							(*count)+= 1;
						}
					}

					sg->out_opacity = AI_RGB_BLACK;
					return;
				}
			}

			//-----------------
			// Non counting Shadow Rays
			sg->out_opacity = (data->hairData.get_unlinkOpacity()?data->hairData.get_shadow_opacity():opacity);
			return;
		}

		// Other shadow ray not from hair
		sg->out_opacity = opacity; // AI_RGB_WHITE
		return;
	}
	////////////////////////
	// GLOSSY
	////////////////////////
	else if(sg->Rt & AI_RAY_GLOSSY) // indirect rays are of type glossy
	{
		if(selfRay)
		{
			bool gm = false;

			// self glossy rays for scattering count intersection
			char sampleNameMode[64];
#ifdef _WIN32
					sprintf_s(sampleNameMode, "ObqH_GSM_%u_%u",sg->psg->transp_index, sg->psg->Rr);
#else
					sprintf(sampleNameMode, "ObqH_GSM_%u_%u",sg->psg->transp_index, sg->psg->Rr);
#endif

			if(AiStateGetMsgBool(sampleNameMode, &gm))
			{
				if(gm) 
				{
					AtUInt32 numGMS = 0;
					char sampleNameCount[64];
#ifdef _WIN32
					sprintf_s(sampleNameCount, "ObqH_GSN_%u_%u",sg->psg->transp_index, sg->psg->Rr);
#else
					sprintf(sampleNameCount, "ObqH_GSN_%u_%u",sg->psg->transp_index, sg->psg->Rr);
#endif

					


					if(AiStateGetMsgUInt(sampleNameCount, &numGMS))
					{
						++numGMS;
						AiStateSetMsgUInt(sampleNameCount, numGMS);
					}

					sg->out.RGB = AI_RGB_BLACK;
					sg->out_opacity = AI_RGB_BLACK;
					return;
				}
			}

			sg->out.RGB = AI_RGB_BLACK; // optimization, this could use the same mode

			sg->out_opacity = (data->hairData.get_unlinkOpacity()?data->hairData.get_shadow_opacity():opacity);;
			return;

		}

		//------------------
		// Other glossy rays
		int mode = data->hairData.get_mode_ray_glossy();

		if(mode == OBQ_HAIR_COLOR)
		{
			sg->out.RGB =  AiShaderEvalParamRGB(p_color_ray_glossy);
		}
		else
		{
			AtVector U = AiV3Normalize(sg->dPdv);
			AtVector V = AiV3Normalize(AiV3Cross(sg->Nf,U));
			AtVector W = AiV3Normalize(AiV3Cross(U, V));
			
			// Get the spherical angles of the exitant direction relative to the hair fibre
			AtVector wo = -sg->Rd;
			float theta_r, phi_r;
			cartesian2sphericalHair(theta_r, phi_r, wo, U,V,W);
			
			ObqHair hairData;
			hairData.evaluateParams(sg,node,data->hairData,U,V,W,wo,theta_r,phi_r);

			AtColor direct_R = AI_RGB_BLACK, direct_TT = AI_RGB_BLACK,direct_TRT = AI_RGB_BLACK,direct_g = AI_RGB_BLACK,
					indirect_R = AI_RGB_BLACK, indirect_TT = AI_RGB_BLACK, indirect_TRT = AI_RGB_BLACK, indirect_g = AI_RGB_BLACK;

			hairData.direct_shading_NoScat(direct_R,direct_TT,direct_TRT,direct_g,sg);

			if(mode == OBQ_HAIR_DIRECTINDIRECTNOSCAT && sg->Rr_gloss < data->max_glossy_depth)
				hairData.indirect_shading_NoScat_uni(indirect_R,indirect_TT,indirect_TRT,indirect_g,sg,data->sampler,data->samples,U,V,W,theta_r,phi_r);

			sg->out.RGB = direct_R + direct_TT + direct_TRT + direct_g + indirect_R + indirect_TT + indirect_TRT + indirect_g;

			if(AiColorCorrupted(sg->out.RGB))
				sg->out.RGB = AI_RGB_BLACK;
		}

		// Optimization opaque
		sg->out_opacity = AI_RGB_WHITE;

	}
	////////////////////
	// DIFFUSE RAY
	////////////////////
	else if(sg->Rt & AI_RAY_DIFFUSE)
	{
		// there are no self ray here

		// normal glossy or diffuse ray not from hair (simple optimization) 
		int mode = data->hairData.get_mode_ray_diffuse();

		if(mode == OBQ_HAIR_COLOR)
		{
			sg->out.RGB = AiShaderEvalParamRGB(p_color_ray_diffuse);
		}
		else
		{

			AtVector U = AiV3Normalize(sg->dPdv);
			AtVector V = AiV3Normalize(AiV3Cross(sg->Nf,U));
			AtVector W = AiV3Normalize(AiV3Cross(U, V));

			// Get the spherical angles of the exitant direction relative to the hair fibre
			AtVector wo = -sg->Rd;
			float theta_r, phi_r;
			cartesian2sphericalHair(theta_r, phi_r, wo, U,V,W);

			ObqHair hairData;
			hairData.evaluateParams(sg,node,data->hairData,U,V,W,wo,theta_r,phi_r);

			AtColor direct_R = AI_RGB_BLACK, direct_TT = AI_RGB_BLACK,direct_TRT = AI_RGB_BLACK,direct_g = AI_RGB_BLACK,
					indirect_R = AI_RGB_BLACK, indirect_TT = AI_RGB_BLACK, indirect_TRT = AI_RGB_BLACK, indirect_g = AI_RGB_BLACK;

			hairData.direct_shading_NoScat(direct_R,direct_TT,direct_TRT,direct_g,sg);

			if(mode == OBQ_HAIR_DIRECTINDIRECTNOSCAT && sg->Rr_gloss < data->max_glossy_depth)
				hairData.indirect_shading_NoScat_uni(indirect_R,indirect_TT,indirect_TRT,indirect_g,sg,data->sampler,data->samples,U,V,W,theta_r,phi_r);

			sg->out.RGB = direct_R + direct_TT + direct_TRT + direct_g + indirect_R + indirect_TT + indirect_TRT + indirect_g;
			
			if(AiColorCorrupted(sg->out.RGB))
				sg->out.RGB = AI_RGB_BLACK;
		}

		// Optimization opaque
		sg->out_opacity = AI_RGB_WHITE;

	}
	///////////////////////////
	// REFLECTED RAY
	///////////////////////////
	else if(sg->Rt & AI_RAY_REFLECTED && data->hairData.get_mode_ray_reflected()!= OBQ_HAIR_LIKECAMERA)
	{
		// there are no self ray here

		// normal glossy or diffuse ray not from hair (simple optimization) 
		int mode = data->hairData.get_mode_ray_reflected();

		if(mode == OBQ_HAIR_COLOR)
		{
			sg->out.RGB = AiShaderEvalParamRGB(p_color_ray_reflected);
		}
		else
		{

			AtVector U = AiV3Normalize(sg->dPdv);
			AtVector V = AiV3Normalize(AiV3Cross(sg->Nf,U));
			AtVector W = AiV3Normalize(AiV3Cross(U, V));

			// Get the spherical angles of the exitant direction relative to the hair fibre
			AtVector wo = -sg->Rd;
			float theta_r, phi_r;
			cartesian2sphericalHair(theta_r, phi_r, wo, U,V,W);

			ObqHair hairData;
			hairData.evaluateParams(sg,node,data->hairData,U,V,W,wo,theta_r,phi_r);

			AtColor direct_R = AI_RGB_BLACK, direct_TT = AI_RGB_BLACK,direct_TRT = AI_RGB_BLACK,direct_g = AI_RGB_BLACK,
					indirect_R = AI_RGB_BLACK, indirect_TT = AI_RGB_BLACK, indirect_TRT = AI_RGB_BLACK, indirect_g = AI_RGB_BLACK;

			hairData.direct_shading_NoScat(direct_R,direct_TT,direct_TRT,direct_g,sg);

			if(mode == OBQ_HAIR_DIRECTINDIRECTNOSCAT && sg->Rr_gloss < data->max_glossy_depth)
				hairData.indirect_shading_NoScat_uni(indirect_R,indirect_TT,indirect_TRT,indirect_g,sg,data->sampler,data->samples,U,V,W,theta_r,phi_r);

			sg->out.RGB = direct_R + direct_TT + direct_TRT + direct_g + indirect_R + indirect_TT + indirect_TRT + indirect_g;
			
			if(AiColorCorrupted(sg->out.RGB))
				sg->out.RGB = AI_RGB_BLACK;
		}

		// Optimization opaque
		sg->out_opacity = AI_RGB_WHITE;

	}
	////////////////////////
	// REFRACTED RAY
	////////////////////////
	else if(sg->Rt & AI_RAY_REFRACTED && data->hairData.get_mode_ray_refracted()!= OBQ_HAIR_LIKECAMERA)
	{
		// there are no self rays here

		// normal glossy or diffuse ray not from hair (simple optimization) 
		int mode = data->hairData.get_mode_ray_refracted();

		if(mode == OBQ_HAIR_COLOR)
		{
			sg->out.RGB = AiShaderEvalParamRGB(p_color_ray_refracted);
		}
		else
		{

			AtVector U = AiV3Normalize(sg->dPdv);
			AtVector V = AiV3Normalize(AiV3Cross(sg->Nf,U));
			AtVector W = AiV3Normalize(AiV3Cross(U, V));

			// Get the spherical angles of the exitant direction relative to the hair fibre
			AtVector wo = -sg->Rd;
			float theta_r, phi_r;
			cartesian2sphericalHair(theta_r, phi_r, wo, U,V,W);

			ObqHair hairData;
			hairData.evaluateParams(sg,node,data->hairData,U,V,W,wo,theta_r,phi_r);

			AtColor direct_R = AI_RGB_BLACK, direct_TT = AI_RGB_BLACK,direct_TRT = AI_RGB_BLACK,direct_g = AI_RGB_BLACK,
					indirect_R = AI_RGB_BLACK, indirect_TT = AI_RGB_BLACK, indirect_TRT = AI_RGB_BLACK, indirect_g = AI_RGB_BLACK;

			hairData.direct_shading_NoScat(direct_R,direct_TT,direct_TRT,direct_g,sg);

			if(mode == OBQ_HAIR_DIRECTINDIRECTNOSCAT && sg->Rr_gloss < data->max_glossy_depth)
				hairData.indirect_shading_NoScat_uni(indirect_R,indirect_TT,indirect_TRT,indirect_g,sg,data->sampler,data->samples,U,V,W,theta_r,phi_r);

			sg->out.RGB = direct_R + direct_TT + direct_TRT + direct_g + indirect_R + indirect_TT + indirect_TRT + indirect_g;
			
			if(AiColorCorrupted(sg->out.RGB))
				sg->out.RGB = AI_RGB_BLACK;
		}

		// Optimization opaque
		sg->out_opacity = AI_RGB_WHITE;

	}
	//////////////////////////////////////
	// CAMERA RAYS, reflection, refraction
	//////////////////////////////////////
	else
	{

		ShaderData *data = (ShaderData*) AiNodeGetLocalData(node);

		// Get a local coordinate frame based on the hair fibre direction
        AtVector U = AiV3Normalize(sg->dPdv);
		AtVector V = AiV3Normalize(AiV3Cross(sg->Nf,U));
		AtVector W = AiV3Normalize(AiV3Cross(U, V));

        // Get the spherical angles of the exitant direction relative to the hair fibre
        AtVector wo = -sg->Rd;
        float theta_r, phi_r;
		cartesian2sphericalHair(theta_r, phi_r, wo, U,V,W);

		ObqHair hairData;
		hairData.evaluateParams(sg,node,data->hairData,U,V,W,wo,theta_r,phi_r);

		AtColor loop_cR__direct = AI_RGB_BLACK, 
				loop_cTT__direct = AI_RGB_BLACK, 
				loop_cTRT__direct = AI_RGB_BLACK,	
				loop_cg__direct = AI_RGB_BLACK, 
				loop_Color = AI_RGB_BLACK, 
				loop_f_direct = AI_RGB_BLACK, 
				loop_f_back_direct = AI_RGB_BLACK,
				loop_f_scatter = AI_RGB_BLACK, 
				loop_f_back_scatter = AI_RGB_BLACK;	

		AtColor loop_Indirect = AI_RGB_BLACK, 
				loop_Indirect_f_scatter = AI_RGB_BLACK, loop_Indirect_f_back_scatter = AI_RGB_BLACK, 
				loop_Indirect_f_direct = AI_RGB_BLACK, loop_Indirect_f_back_direct = AI_RGB_BLACK,
				loop_cR__indirect = AI_RGB_BLACK, 
				loop_cTT__indirect = AI_RGB_BLACK, 
				loop_cTRT__indirect = AI_RGB_BLACK,	
				loop_cg__indirect = AI_RGB_BLACK;
		
		
		bool forceSingle = false;

		
		if( (sg->Rt & AI_RAY_REFLECTED || sg->Rt & AI_RAY_REFRACTED) && sg->Rr != sg->Rr_refl + sg->Rr_refr)
			forceSingle = true;

		//-----------------
		// DUAL SCATTERING
		if(!data->hairData.get_singleScattering() && !forceSingle)
		{
			//-----------------
			// DIRECT LIGHTING
			if(data->hairData.get_modeMIS() == 0)
				hairData.direct_shading_full(loop_f_scatter,loop_f_back_scatter, loop_cR__direct, loop_cTT__direct, loop_cTRT__direct, loop_cg__direct,loop_f_back_direct,sg,data->hairData,/*&data->criticalSec,*/U,V,W,theta_r,phi_r);
			else
				hairData.direct_shading_full_MIS_Options(loop_f_scatter,loop_f_back_scatter, loop_cR__direct, loop_cTT__direct, loop_cTRT__direct, loop_cg__direct,loop_f_back_direct,sg,data->hairData,/*&data->criticalSec,*/U,V,W,theta_r,phi_r);

			//-----------------
			// INDIRECT LIGHTING (uniform for now) TODO : Ou et Al.
			hairData.indirect_shading_Full_uni(loop_Indirect_f_scatter,loop_Indirect_f_back_scatter,loop_cR__indirect,loop_cTT__indirect,loop_cTRT__indirect, loop_cg__indirect,loop_Indirect_f_back_direct,sg, data->hairData, data->sampler,/*&data->criticalSec,*/ data->samples,U,V,W,theta_r, phi_r);
		}
		else
		{
		
			hairData.direct_shading_NoScat(loop_cR__direct, loop_cTT__direct, loop_cTRT__direct, loop_cg__direct,sg);
			hairData.indirect_shading_NoScat_uni(loop_cR__indirect,loop_cTT__indirect,loop_cTRT__indirect, loop_cg__indirect,sg, data->sampler, data->samples,U,V,W,theta_r, phi_r);
		
		}
		
		// Corruption test
		if(AiColorCorrupted(loop_cR__direct))		loop_cR__direct = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_cTT__direct))		loop_cTT__direct = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_cTRT__direct))		loop_cTRT__direct = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_cg__direct))		loop_cg__direct = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_f_back_direct))	loop_f_back_direct = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_f_scatter))		loop_f_scatter = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_f_back_scatter))	loop_f_back_scatter = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_cR__indirect))		loop_cR__indirect = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_cTT__indirect))	loop_cTT__indirect = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_cTRT__indirect))	loop_cTRT__indirect = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_cg__indirect))		loop_cg__indirect = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_Indirect_f_back_direct))	loop_Indirect_f_back_direct = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_Indirect_f_scatter))		loop_Indirect_f_scatter = AI_RGB_BLACK;
		if(AiColorCorrupted(loop_Indirect_f_back_scatter))	loop_Indirect_f_back_scatter = AI_RGB_BLACK;

		
		// Write result
		sg->out.RGB =	loop_cR__direct		+ loop_cTT__direct	 + loop_cTRT__direct   + loop_cg__direct   + loop_f_back_direct		     + loop_f_scatter		   + loop_f_back_scatter +
						loop_cR__indirect	+ loop_cTT__indirect + loop_cTRT__indirect + loop_cg__indirect + loop_Indirect_f_back_direct + loop_Indirect_f_scatter + loop_Indirect_f_back_scatter;

		sg->out_opacity = opacity;

		// Write AOVs
		if(sg->Rt & AI_RAY_CAMERA)
		{
			switch(data->aov_mode)
			{
			case 1:	// All
				AiAOVSetRGB (sg,data->aov_dir_R,loop_cR__direct);
				AiAOVSetRGB (sg,data->aov_dir_TT,loop_cTT__direct);
				AiAOVSetRGB (sg,data->aov_dir_TRT,loop_cTRT__direct);
				AiAOVSetRGB (sg,data->aov_dir_g,loop_cg__direct);

				AiAOVSetRGB (sg,data->aov_dir_back,loop_f_back_direct);
				AiAOVSetRGB (sg,data->aov_dir_scatter,loop_f_scatter);
				AiAOVSetRGB (sg,data->aov_dir_scatter_back,loop_f_back_scatter);

				AiAOVSetRGB (sg,data->aov_indir_R,loop_cR__indirect);
				AiAOVSetRGB (sg,data->aov_indir_TT,loop_cTT__indirect);
				AiAOVSetRGB (sg,data->aov_indir_TRT,loop_cTRT__indirect);
				AiAOVSetRGB (sg,data->aov_indir_g,loop_cg__indirect);

				AiAOVSetRGB (sg,data->aov_indir_back,loop_Indirect_f_back_direct);
				AiAOVSetRGB (sg,data->aov_indir_scatter,loop_Indirect_f_scatter);
				AiAOVSetRGB (sg,data->aov_indir_scatter_back,loop_Indirect_f_back_scatter);
				break;
			case 2: // merge dir/indir
				AiAOVSetRGB (sg,data->aov_dir_R,loop_cR__direct + loop_cR__indirect);
				AiAOVSetRGB (sg,data->aov_dir_TT,loop_cTT__direct + loop_cTT__indirect);
				AiAOVSetRGB (sg,data->aov_dir_TRT,loop_cTRT__direct + loop_cTRT__indirect);
				AiAOVSetRGB (sg,data->aov_dir_g,loop_cg__direct + loop_cg__indirect);

				AiAOVSetRGB (sg,data->aov_dir_back,loop_f_back_direct + loop_Indirect_f_back_direct);
				AiAOVSetRGB (sg,data->aov_dir_scatter,loop_f_scatter + loop_Indirect_f_scatter);
				AiAOVSetRGB (sg,data->aov_dir_scatter_back,loop_f_back_scatter + loop_Indirect_f_back_scatter);
				break;
			case 3: // merge lobes
				AiAOVSetRGB (sg,data->aov_dir_R,loop_cR__direct+loop_cTT__direct+loop_cTRT__direct+loop_cg__direct);

				AiAOVSetRGB (sg,data->aov_dir_back,loop_f_back_direct);
				AiAOVSetRGB (sg,data->aov_dir_scatter,loop_f_scatter);
				AiAOVSetRGB (sg,data->aov_dir_scatter_back,loop_f_back_scatter);

				AiAOVSetRGB (sg,data->aov_indir_R,loop_cR__indirect+loop_cTT__indirect+loop_cTRT__indirect+loop_cg__indirect);

				AiAOVSetRGB (sg,data->aov_indir_back,loop_Indirect_f_back_direct);
				AiAOVSetRGB (sg,data->aov_indir_scatter,loop_Indirect_f_scatter);
				AiAOVSetRGB (sg,data->aov_indir_scatter_back,loop_Indirect_f_back_scatter);
				break;
			case 4: // merge lobes + merge dir/indir
				AiAOVSetRGB (sg,data->aov_dir_R,loop_cR__direct+loop_cTT__direct+loop_cTRT__direct+loop_cg__direct + loop_cR__indirect+loop_cTT__indirect+loop_cTRT__indirect+loop_cg__indirect);

				AiAOVSetRGB (sg,data->aov_dir_back,loop_f_back_direct + loop_Indirect_f_back_direct);
				AiAOVSetRGB (sg,data->aov_dir_scatter,loop_f_scatter + loop_Indirect_f_scatter);
				AiAOVSetRGB (sg,data->aov_dir_scatter_back,loop_f_back_scatter + loop_Indirect_f_back_scatter);
				break;
			case 5:
				AiAOVSetRGB (sg,data->aov_dir_R,loop_cR__direct+loop_cTT__direct+loop_cTRT__direct+loop_cg__direct);
				AiAOVSetRGB (sg,data->aov_dir_scatter,loop_f_back_direct+loop_f_scatter+loop_f_back_scatter);

				AiAOVSetRGB (sg,data->aov_indir_R,loop_cR__indirect+loop_cTT__indirect+loop_cTRT__indirect+loop_cg__indirect);
				AiAOVSetRGB (sg,data->aov_indir_scatter,loop_Indirect_f_back_direct+loop_Indirect_f_scatter+loop_Indirect_f_back_scatter);
				break;
			case 6:
				AiAOVSetRGB (sg,data->aov_dir_R,loop_cR__direct+loop_cTT__direct+loop_cTRT__direct+loop_cg__direct+loop_cR__indirect+loop_cTT__indirect+loop_cTRT__indirect+loop_cg__indirect);
				AiAOVSetRGB (sg,data->aov_dir_scatter,loop_f_back_direct+loop_f_scatter+loop_f_back_scatter+loop_Indirect_f_back_direct+loop_Indirect_f_scatter+loop_Indirect_f_back_scatter);
				break;
			case 7:
				AiAOVSetRGB (sg,data->aov_dir_R,loop_cR__direct);
				AiAOVSetRGB (sg,data->aov_dir_TT,loop_cTT__direct);
				AiAOVSetRGB (sg,data->aov_dir_TRT,loop_cTRT__direct);
				AiAOVSetRGB (sg,data->aov_dir_g,loop_cg__direct);

				AiAOVSetRGB (sg,data->aov_dir_scatter,loop_f_scatter+loop_f_back_direct+loop_f_back_scatter);

				AiAOVSetRGB (sg,data->aov_indir_R,loop_cR__indirect);
				AiAOVSetRGB (sg,data->aov_indir_TT,loop_cTT__indirect);
				AiAOVSetRGB (sg,data->aov_indir_TRT,loop_cTRT__indirect);
				AiAOVSetRGB (sg,data->aov_indir_g,loop_cg__indirect);

				AiAOVSetRGB (sg,data->aov_indir_scatter,loop_Indirect_f_scatter+loop_Indirect_f_back_direct+loop_Indirect_f_back_scatter);
				break;
			case 8:
				AiAOVSetRGB (sg,data->aov_dir_R,loop_cR__direct+loop_cR__indirect);
				AiAOVSetRGB (sg,data->aov_dir_TT,loop_cTT__direct+loop_cTT__indirect);
				AiAOVSetRGB (sg,data->aov_dir_TRT,loop_cTRT__direct+loop_cTRT__indirect);
				AiAOVSetRGB (sg,data->aov_dir_g,loop_cg__direct+loop_cg__indirect);

				AiAOVSetRGB (sg,data->aov_dir_scatter,loop_f_scatter+loop_f_back_direct+loop_f_back_scatter+loop_Indirect_f_scatter+loop_Indirect_f_back_direct+loop_Indirect_f_back_scatter);

				break;
			default:
				break;
			}
		}
	}
}

//node_loader
//{
//	if (i > 0)
//		return false;
//
//	node->methods      = ObqHairParams;
//	node->output_type  = AI_TYPE_RGB;
//	node->name         = "Obq_Hair";
//	node->node_type    = AI_NODE_SHADER;
//#ifdef _WIN32
//	strcpy_s(node->version, AI_VERSION);
//#else
//	strcpy(node->version, AI_VERSION);
//#endif
//
//	return true;
//}

/////////////////
/////////////////
// MIS Single SCAT
/////////////////
/////////////////

static AtVector ObqHairSingleScatEvalSample(const void *brdf, float rx, float ry)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
	
	float rnd[2] = {myRandom(),ry};
	if( hairData->get_MIScurrentUseRandom() )
	{
		rnd[0] = myRandom();
		rnd[1] = myRandom();
	}
	(*(hairData->m_MIScurrentStep)) = 1;
	return hairData->sample_dir(rnd,myRandom());
}

static AtColor ObqHairSingleScatEvalBrdf(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
		
	if((*(hairData->m_MIScurrentStep)) == 1) // brdf sample
	{
		(*(hairData->m_MIScurrentStep)) = 2;
		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		AtColor c = hairData->S_scaled(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());

		return c*std::abs(std::cos(theta_i));
	}
	else // light sample
	{
		return hairData->get_MISspecialBCSDFColor();
	}
	return AI_RGB_BLACK;
}

static float ObqHairSingleScatEvalPdf(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;

	if((*(hairData->m_MIScurrentStep)) == 2) // brdf sample
	{

		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());
		return hairData->compute_pdf(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
	}
	else	// light
	{
		return hairData->get_MISspecialPdf();
	}
	
	return 0.0f;
}

//-- SEPARATE LOBES
static AtVector ObqHairSingleScatEvalSample_separateLobes(const void *brdf, float rx, float ry)
{
	const ObqHair *hairData = (const ObqHair*)brdf;

	//AtVector indir;
	float theta_o = 0.0f, phi_o = 0.0f;
	float rnd[2] = {rx,ry};
	if(hairData->get_MIScurrentUseRandom())
	{
		rnd[0] = myRandom();
		rnd[1] = myRandom();
	}
	
	(*(hairData->m_MIScurrentStep)) = 1;

	switch(hairData->get_MIScurrentLobe())
	{
	case 0:
		hairData->sample_R_lobe(theta_o, phi_o, rnd, hairData->get_theta_r(),hairData->get_phi_r());
		break;
	case 1:
		hairData->sample_TT_lobe(theta_o, phi_o, rnd, hairData->get_theta_r(),hairData->get_phi_r());
		break;
	case 2:
		hairData->sample_TRT_G_lobe(theta_o, phi_o, rnd, hairData->get_theta_r(),hairData->get_phi_r());
		break;
	case 3:
		hairData->sample_G_lobe(theta_o, phi_o, rnd, hairData->get_theta_r(),hairData->get_phi_r());
		break;
	default:
		AiMsgWarning("No lobe selected");
		break;
	}

	return sphericalHair2cartesian(theta_o,phi_o,hairData->get_U(),hairData->get_V(),hairData->get_W());

}

static AtColor ObqHairSingleScatEvalBrdf_separateLobes(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
		
	if((*(hairData->m_MIScurrentStep)) == 1) // brdf sample
	{
		(*(hairData->m_MIScurrentStep)) = 2;

		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());
		AtColor c = AI_RGB_BLACK;
		switch(hairData->get_MIScurrentLobe())
		{
		case 0:
			c = hairData->S_scaled_R(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
			break;
		case 1:
			c = hairData->S_scaled_TT(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
			break;
		case 2:
			c = hairData->S_scaled_TRT(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
			break;
		case 3:
			c = hairData->S_scaled_g(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
			break;
		default:
			AiMsgWarning("No lobe selected");
			break;
		}

		return c*std::abs(std::cos(theta_i));
	}
	else // light sample
	{
			return hairData->get_MISspecialBCSDFColor();
	}

	return AI_RGB_BLACK;
}

static float ObqHairSingleScatEvalPdf_separateLobes(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;

	if((*(hairData->m_MIScurrentStep)) == 2) // brdf sample
	{
		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		switch(hairData->get_MIScurrentLobe())
		{
		case 0:
			return hairData->compute_R_pdf(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
		case 1:
			return hairData->compute_TT_pdf(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
		case 2:
			return hairData->compute_TRT_G_pdf(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
		case 3:
			return hairData->compute_G_pdf(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
		default:
			AiMsgWarning("No lobe selected");
			break;
		}
	}
	else	// light
	{
			return hairData->get_MISspecialPdf();
	}
	return 0.0f;
}

/////////////////
/////////////////
//
// MIS DUAL SCAT
//
/////////////////
/////////////////

//////////////////////
// EVAL SAMPLE 
//////////////////////
static AtVector ObqHairDualScatEvalSample(const void *brdf, float rx, float ry)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
	
	float rnd[2] = {rx,ry};
	if(hairData->get_MIScurrentUseRandom())
	{
		rnd[0] = myRandom();
		rnd[1] = myRandom();
	}

	AtVector indir = hairData->sample_dir(rnd,myRandom());
	
	(*(hairData->m_MIScurrentStep)) = 1;
	
	return indir;
}

static AtVector ObqHairDualScatEvalSample_separateLobes(const void *brdf, float rx, float ry)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
	
	float theta_o = 0.0f, phi_o = 0.0f;
	float rnd[2] = {rx,ry};
	if(hairData->get_MIScurrentUseRandom())
	{
		rnd[0] = myRandom();
		rnd[1] = myRandom();
	}

	switch(hairData->get_MIScurrentLobe())
	{
	case 0:
		hairData->sample_R_lobe(theta_o, phi_o, rnd, hairData->get_theta_r(),hairData->get_phi_r());
		break;
	case 1:
		hairData->sample_TT_lobe(theta_o, phi_o, rnd, hairData->get_theta_r(),hairData->get_phi_r());
		break;
	case 2:
		hairData->sample_TRT_G_lobe(theta_o, phi_o, rnd, hairData->get_theta_r(),hairData->get_phi_r());
		break;
	case 3:
		hairData->sample_G_lobe(theta_o, phi_o, rnd, hairData->get_theta_r(),hairData->get_phi_r());
		break;
	default:
		AiMsgWarning("No lobe selected");
		break;
	}

	(*(hairData->m_MIScurrentStep)) = 1;
	
	return sphericalHair2cartesian(theta_o,phi_o,hairData->get_U(),hairData->get_V(),hairData->get_W());
}

static AtVector ObqHairDualScatEvalSample_DS(const void *brdf, float rx, float ry)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
	
	float rnd[2] = {rx,ry};
	if(hairData->get_MIScurrentUseRandom())
	{
		rnd[0] = myRandom();
		rnd[1] = myRandom();
	}
	AtVector indir;
	uniformPointSphere(indir,rnd);

	(*(hairData->m_MIScurrentStep)) = 1;
	
	return indir;
}
//////////////////////
// EVAL BRDF 
//////////////////////

//-- Full single AOV
static AtColor ObqHairDualScatEvalBrdf(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
		
	bool from1 = ((*(hairData->m_MIScurrentStep)) == 1);
	
	// Count was done during AiLightsPrepare or during loop for mis

	if(!from1)
	{
		
		return hairData->get_MISspecialBCSDFColor();
	}
	else
	{
		
		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		float numGMS_f = 0.0f;

		AtColor forwardScale = hairData->get_scale_forward()*hairData->get_color_forward();
		bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);

		AtColor backScale = hairData->get_scale_back()*hairData->get_color_back();
		bool backScaleIsSmall = AiColorIsSmall(backScale);

		AtUInt32 nIntersections = 0; 

		nIntersections = hairData->get_MIScurrentShadowCounter();
		((*(hairData->m_MIScurrentStep)) = 2);		

		numGMS_f = std::pow(static_cast<float>(nIntersections),hairData->get_numGMS_modifier());

		float theta_r = hairData->get_theta_r();
		float phi_r = hairData->get_phi_r();
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;

		AtColor selfOpacity = (hairData->get_unlinkOpacity()?hairData->get_shadow_opacity():hairData->getOpacity());
		AtColor directFraction = (numGMS_f==0.0 || selfOpacity == AI_RGB_BLACK ?AI_RGB_WHITE:AI_RGB_WHITE-colorChannelPow(AiColorClamp(selfOpacity,0.0f,1.0f),numGMS_f));

		bool directFractionIsSmall = AiColorIsSmall(directFraction);

		AtColor forward_fraction = AI_RGB_WHITE - directFraction;

		AtColor loop_f_back_scatter = AI_RGB_BLACK,
			loop_f_scatter = AI_RGB_BLACK,
			loop_f_back_direct = AI_RGB_BLACK,
			loop_f_direct_cR = AI_RGB_BLACK,
			loop_f_direct_cTT = AI_RGB_BLACK,
			loop_f_direct_cTRT = AI_RGB_BLACK,
			loop_f_direct_cg = AI_RGB_BLACK;

		// At least forward scattering once -> Scatter
		if(!AiColorIsSmall(forward_fraction))
		{
			AtColor sigma_bar_f_2 = hairData->getDHairData()->get_beta_bar_f_2(theta_i)*numGMS_f;
			AtColor a_bar_f_color = hairData->getDHairData()->get_a_bar_f(theta_i);
			AtColor T_f = { hairData->get_d_f()*std::pow(a_bar_f_color.r,numGMS_f),	hairData->get_d_f()*std::pow(a_bar_f_color.g,numGMS_f),	hairData->get_d_f()*std::pow(a_bar_f_color.b,numGMS_f)};
			forward_fraction = colorChannelMax(T_f-directFraction,0.0f);
			AtColor f_s_scatter = hairData->S_G(theta_i,phi_i,theta_r,phi_r,hairData->getDHairData()->get_sigma_bar_b_2(theta_i),*hairData->getDHairData());
			f_s_scatter *= forwardScale;

			if(!backScaleIsSmall)
			{
				AtColor f_back_scatter = 2.0f*hairData->getDHairData()->get_A_bar_b(theta_i)*gN(colorChannelMax(hairData->getDHairData()->get_sigma_bar_b_2(theta_i) + sigma_bar_f_2 + hairData->get_beta_back(),AI_EPSILON) ,hairData->getDHairData()->get_Delta_bar_b(theta_i),theta_h+hairData->get_alpha_back())/(c_Pi*std::cos(theta_d)*std::cos(theta_d));
				f_back_scatter *= backScale;
				loop_f_back_scatter = forward_fraction*hairData->get_d_f()*c_Pi*hairData->get_d_b()*f_back_scatter;
			}

			loop_f_scatter = forward_fraction*hairData->get_d_f()*f_s_scatter;
		}

		//else // -> direct
		if(!directFractionIsSmall)
		{
			AtColor cR = AI_RGB_BLACK, cTT = AI_RGB_BLACK, cTRT = AI_RGB_BLACK, cg = AI_RGB_BLACK;
			hairData->S_scaled(cR, cTT, cTRT, cg, theta_i,phi_i,theta_r,phi_r);

			loop_f_direct_cR		=	directFraction*cR;
			loop_f_direct_cTT		=	directFraction*cTT;
			loop_f_direct_cTRT		=	directFraction*cTRT;
			loop_f_direct_cg		=	directFraction*cg;

			if(!backScaleIsSmall)
			{
				AtColor f_back_direct = 2.0f*hairData->getDHairData()->get_A_bar_b(theta_i) * gN(colorChannelMax(hairData->getDHairData()->get_sigma_bar_b_2(theta_i)+hairData->get_beta_back(),AI_EPSILON),hairData->getDHairData()->get_Delta_bar_b(theta_i),theta_h+hairData->get_alpha_back())/(c_Pi*std::cos(theta_i)*std::cos(theta_i));
				loop_f_back_direct	=	directFraction*hairData->get_d_b()*f_back_direct*backScale;//*lightC
			}

		}

		AtColor c = (loop_f_direct_cR + loop_f_direct_cTT + loop_f_direct_cTRT + loop_f_direct_cg) + (loop_f_back_scatter + loop_f_scatter + loop_f_back_direct) ;

		return c*std::abs(std::cos(theta_i));
	}
	return AI_RGB_BLACK;
}

static AtColor ObqHairDualScatEvalBrdf_separateDS(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
		
	bool from1 = ((*(hairData->m_MIScurrentStep)) == 1);
	
	// Count was done during AiLightsPrepare or during loop for mis

	if(!from1)
	{
			return hairData->get_MISspecialBCSDFColor();
	}
	else
	{
		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		float numGMS_f = 0.0f;

		AtColor forwardScale = hairData->get_scale_forward()*hairData->get_color_forward();
		bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);

		AtColor backScale = hairData->get_scale_back()*hairData->get_color_back();
		bool backScaleIsSmall = AiColorIsSmall(backScale);

		AtUInt32 nIntersections = 0; 

		nIntersections = hairData->get_MIScurrentShadowCounter();
	
		((*(hairData->m_MIScurrentStep)) = 2);		

		numGMS_f = std::pow(static_cast<float>(nIntersections),hairData->get_numGMS_modifier());

		float theta_r = hairData->get_theta_r();
		float phi_r = hairData->get_phi_r();
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;

		AtColor selfOpacity = (hairData->get_unlinkOpacity()?hairData->get_shadow_opacity():hairData->getOpacity());
		AtColor directFraction = (numGMS_f==0.0 || selfOpacity == AI_RGB_BLACK ?AI_RGB_WHITE:AI_RGB_WHITE-colorChannelPow(AiColorClamp(selfOpacity,0.0f,1.0f),numGMS_f));

		bool directFractionIsSmall = AiColorIsSmall(directFraction);

		//else // -> direct
		AtColor c = AI_RGB_BLACK;
		if(!directFractionIsSmall)
				c = directFraction*hairData->S_scaled(theta_i,phi_i,theta_r,phi_r);

		return c*std::abs(std::cos(theta_i));
	}

	return AI_RGB_BLACK;
}


static AtColor ObqHairDualScatEvalBrdf_separateDS_separateLobes(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
		
	bool from1 = ((*(hairData->m_MIScurrentStep)) == 1);
	
	// Count was done during AiLightsPrepare or during loop for mis

	if(!from1)
	{
		return hairData->get_MISspecialBCSDFColor();
	}
	else
	{
		((*(hairData->m_MIScurrentStep)) = 2);	

		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		float numGMS_f = 0.0f;

		AtColor forwardScale = hairData->get_scale_forward()*hairData->get_color_forward();
		bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);

		AtColor backScale = hairData->get_scale_back()*hairData->get_color_back();
		bool backScaleIsSmall = AiColorIsSmall(backScale);

		AtUInt32 nIntersections = 0; 

		nIntersections = hairData->get_MIScurrentShadowCounter();

		numGMS_f = std::pow(static_cast<float>(nIntersections),hairData->get_numGMS_modifier());

		float theta_r = hairData->get_theta_r();
		float phi_r = hairData->get_phi_r();
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;

		AtColor selfOpacity = (hairData->get_unlinkOpacity()?hairData->get_shadow_opacity():hairData->getOpacity());
		AtColor directFraction = (numGMS_f==0.0 || selfOpacity == AI_RGB_BLACK ?AI_RGB_WHITE:AI_RGB_WHITE-colorChannelPow(AiColorClamp(selfOpacity,0.0f,1.0f),numGMS_f));

		bool directFractionIsSmall = AiColorIsSmall(directFraction);

		//else // -> direct
		AtColor c = AI_RGB_BLACK;
		if(!directFractionIsSmall)
		{
			switch(hairData->get_MIScurrentLobe())
			{
			case OBQ_LOBE_R:
				c = directFraction*hairData->S_scaled_R(theta_i,phi_i,theta_r,phi_r);
				break;
			case OBQ_LOBE_TT:
				c = directFraction*hairData->S_scaled_TT(theta_i,phi_i,theta_r,phi_r);
				break;
			case OBQ_LOBE_TRT:
				c = directFraction*hairData->S_scaled_TRT(theta_i,phi_i,theta_r,phi_r);
				break;
			case OBQ_LOBE_G:
				c = directFraction*hairData->S_scaled_g(theta_i,phi_i,theta_r,phi_r);
				break;
			default:
				break;
			}
		}

		return c*std::abs(std::cos(theta_i));
	}

	return AI_RGB_BLACK;
}



static AtColor ObqHairDualScatEvalBrdf_separateLobes(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
		
	bool from1 = ((*(hairData->m_MIScurrentStep)) == 1);


	// Count was done during AiLightsPrepare or during loop for mis

	if(!from1)
	{
		return hairData->get_MISspecialBCSDFColor();
	}
	else
	{
		((*(hairData->m_MIScurrentStep)) = 2);	

		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		float numGMS_f = 0.0f;

		AtColor forwardScale = hairData->get_scale_forward()*hairData->get_color_forward();
		bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);

		AtColor backScale = hairData->get_scale_back()*hairData->get_color_back();
		bool backScaleIsSmall = AiColorIsSmall(backScale);

		AtUInt32 nIntersections = 0; 

		nIntersections = hairData->get_MIScurrentShadowCounter();
			
		numGMS_f = std::pow(static_cast<float>(nIntersections),hairData->get_numGMS_modifier());

		float theta_r = hairData->get_theta_r();
		float phi_r = hairData->get_phi_r();
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;

		AtColor selfOpacity = (hairData->get_unlinkOpacity()?hairData->get_shadow_opacity():hairData->getOpacity());
		AtColor directFraction = (numGMS_f==0.0 || selfOpacity == AI_RGB_BLACK ?AI_RGB_WHITE:AI_RGB_WHITE-colorChannelPow(AiColorClamp(selfOpacity,0.0f,1.0f),numGMS_f));

		bool directFractionIsSmall = AiColorIsSmall(directFraction);

		AtColor forward_fraction = AI_RGB_WHITE - directFraction;

		AtColor loop_f_back_scatter = AI_RGB_BLACK,
			loop_f_scatter = AI_RGB_BLACK,
			loop_f_back_direct = AI_RGB_BLACK,
			loop_f_direct_cR = AI_RGB_BLACK,
			loop_f_direct_cTT = AI_RGB_BLACK,
			loop_f_direct_cTRT = AI_RGB_BLACK,
			loop_f_direct_cg = AI_RGB_BLACK;

		// At least forward scattering once -> Scatter
		if(!AiColorIsSmall(forward_fraction))
		{
			AtColor sigma_bar_f_2 = hairData->getDHairData()->get_beta_bar_f_2(theta_i)*numGMS_f;
			AtColor a_bar_f_color = hairData->getDHairData()->get_a_bar_f(theta_i);
			AtColor T_f = { hairData->get_d_f()*std::pow(a_bar_f_color.r,numGMS_f),	hairData->get_d_f()*std::pow(a_bar_f_color.g,numGMS_f),	hairData->get_d_f()*std::pow(a_bar_f_color.b,numGMS_f)};
			forward_fraction = colorChannelMax(T_f-directFraction,0.0f);
			AtColor f_s_scatter = hairData->S_G(theta_i,phi_i,theta_r,phi_r,hairData->getDHairData()->get_sigma_bar_b_2(theta_i),*hairData->getDHairData());
			f_s_scatter *= forwardScale;

			if(!backScaleIsSmall)
			{
				AtColor f_back_scatter = 2.0f*hairData->getDHairData()->get_A_bar_b(theta_i)*gN(colorChannelMax(hairData->getDHairData()->get_sigma_bar_b_2(theta_i) + sigma_bar_f_2 + hairData->get_beta_back(),AI_EPSILON) ,hairData->getDHairData()->get_Delta_bar_b(theta_i),theta_h+hairData->get_alpha_back())/(c_Pi*std::cos(theta_d)*std::cos(theta_d));
				f_back_scatter *= backScale;
				loop_f_back_scatter = forward_fraction*hairData->get_d_f()*c_Pi*hairData->get_d_b()*f_back_scatter;
			}

			loop_f_scatter = forward_fraction*hairData->get_d_f()*f_s_scatter;
		}

		//else // -> direct
		if(!directFractionIsSmall)
		{
			AtColor cR = AI_RGB_BLACK, cTT = AI_RGB_BLACK, cTRT = AI_RGB_BLACK, cg = AI_RGB_BLACK;
			hairData->S_scaled(cR, cTT, cTRT, cg, theta_i,phi_i,theta_r,phi_r);

			loop_f_direct_cR		=	directFraction*cR;
			loop_f_direct_cTT		=	directFraction*cTT;
			loop_f_direct_cTRT		=	directFraction*cTRT;
			loop_f_direct_cg		=	directFraction*cg;

			if(!backScaleIsSmall)
			{
				AtColor f_back_direct = 2.0f*hairData->getDHairData()->get_A_bar_b(theta_i) * gN(colorChannelMax(hairData->getDHairData()->get_sigma_bar_b_2(theta_i)+hairData->get_beta_back(),AI_EPSILON),hairData->getDHairData()->get_Delta_bar_b(theta_i),theta_h+hairData->get_alpha_back())/(c_Pi*std::cos(theta_i)*std::cos(theta_i));
				loop_f_back_direct	=	directFraction*hairData->get_d_b()*f_back_direct*backScale;//*lightC
			}

		}

		AtColor c = (loop_f_direct_cR + loop_f_direct_cTT + loop_f_direct_cTRT + loop_f_direct_cg) + (loop_f_back_scatter + loop_f_scatter + loop_f_back_direct) ;

		return c*std::abs(std::cos(theta_i));
	}
	return AI_RGB_BLACK;
}


static AtColor ObqHairDualScatEvalBrdf_DS(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
		
	bool from1 = ((*(hairData->m_MIScurrentStep)) == 1);

	// Count was done during AiLightsPrepare or during loop for mis

	if(!from1)
	{
		return hairData->get_MISspecialBCSDFColor();
	}
	else
	{
		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		float numGMS_f = 0.0f;

		AtColor forwardScale = hairData->get_scale_forward()*hairData->get_color_forward();
		bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);

		AtColor backScale = hairData->get_scale_back()*hairData->get_color_back();
		bool backScaleIsSmall = AiColorIsSmall(backScale);

		AtUInt32 nIntersections = 0; 

		nIntersections = hairData->get_MIScurrentShadowCounter();
		((*(hairData->m_MIScurrentStep)) = 2);		

		numGMS_f = std::pow(static_cast<float>(nIntersections),hairData->get_numGMS_modifier());

		float theta_r = hairData->get_theta_r();
		float phi_r = hairData->get_phi_r();
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;

		AtColor selfOpacity = (hairData->get_unlinkOpacity()?hairData->get_shadow_opacity():hairData->getOpacity());
		AtColor directFraction = (numGMS_f==0.0 || selfOpacity == AI_RGB_BLACK ?AI_RGB_WHITE:AI_RGB_WHITE-colorChannelPow(AiColorClamp(selfOpacity,0.0f,1.0f),numGMS_f));

		bool directFractionIsSmall = AiColorIsSmall(directFraction);

		AtColor forward_fraction = AI_RGB_WHITE - directFraction;

		AtColor loop_f_back_scatter = AI_RGB_BLACK,
			loop_f_scatter = AI_RGB_BLACK,
			loop_f_back_direct = AI_RGB_BLACK,
			loop_f_direct_cR = AI_RGB_BLACK,
			loop_f_direct_cTT = AI_RGB_BLACK,
			loop_f_direct_cTRT = AI_RGB_BLACK,
			loop_f_direct_cg = AI_RGB_BLACK;

		// At least forward scattering once -> Scatter
		if(!AiColorIsSmall(forward_fraction))
		{
			AtColor sigma_bar_f_2 = hairData->getDHairData()->get_beta_bar_f_2(theta_i)*numGMS_f;
			AtColor a_bar_f_color = hairData->getDHairData()->get_a_bar_f(theta_i);
			AtColor T_f = { hairData->get_d_f()*std::pow(a_bar_f_color.r,numGMS_f),	hairData->get_d_f()*std::pow(a_bar_f_color.g,numGMS_f),	hairData->get_d_f()*std::pow(a_bar_f_color.b,numGMS_f)};
			forward_fraction = colorChannelMax(T_f-directFraction,0.0f);
			AtColor f_s_scatter = hairData->S_G(theta_i,phi_i,theta_r,phi_r,hairData->getDHairData()->get_sigma_bar_b_2(theta_i),*hairData->getDHairData());
			f_s_scatter *= forwardScale;

			if(!backScaleIsSmall)
			{
				AtColor f_back_scatter = 2.0f*hairData->getDHairData()->get_A_bar_b(theta_i)*gN(colorChannelMax(hairData->getDHairData()->get_sigma_bar_b_2(theta_i) + sigma_bar_f_2 + hairData->get_beta_back(),AI_EPSILON) ,hairData->getDHairData()->get_Delta_bar_b(theta_i),theta_h+hairData->get_alpha_back())/(c_Pi*std::cos(theta_d)*std::cos(theta_d));
				f_back_scatter *= backScale;
				loop_f_back_scatter = forward_fraction*hairData->get_d_f()*c_Pi*hairData->get_d_b()*f_back_scatter;
			}

			loop_f_scatter = forward_fraction*hairData->get_d_f()*f_s_scatter;
		}

		//else // -> direct
		if(!directFractionIsSmall && !backScaleIsSmall)
			{
				AtColor f_back_direct = 2.0f*hairData->getDHairData()->get_A_bar_b(theta_i) * gN(colorChannelMax(hairData->getDHairData()->get_sigma_bar_b_2(theta_i)+hairData->get_beta_back(),AI_EPSILON),hairData->getDHairData()->get_Delta_bar_b(theta_i),theta_h+hairData->get_alpha_back())/(c_Pi*std::cos(theta_i)*std::cos(theta_i));
				loop_f_back_direct	=	directFraction*hairData->get_d_b()*f_back_direct*backScale;//*lightC
			}

		AtColor c = (loop_f_back_scatter + loop_f_scatter + loop_f_back_direct) ;

		return c*std::abs(std::cos(theta_i));
	}
	return AI_RGB_BLACK;
}

//////////////////////
// EVAL PDF 
//////////////////////

static float ObqHairDualScatEvalPdf(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
	if((*(hairData->m_MIScurrentStep)) == 2) // brdf sample
	{
		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		return hairData->compute_pdf(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
	}
	else
	{
			return  hairData->get_MISspecialPdf();
	}

	return 0.0f;
}

static float ObqHairDualScatEvalPdf_separateLobes(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;

	if((*(hairData->m_MIScurrentStep)) == 2) // brdf sample
	{
		float theta_i, phi_i;
		cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		switch(hairData->get_MIScurrentLobe())
		{
		case 0:
			return hairData->compute_R_pdf(theta_i, phi_i, hairData->get_theta_r(), hairData->get_phi_r());
		case 1:
			return hairData->compute_TT_pdf(theta_i, phi_i, hairData->get_theta_r(), hairData->get_phi_r());
		case 2:
			return hairData->compute_TRT_G_pdf(theta_i, phi_i, hairData->get_theta_r(), hairData->get_phi_r());
		case 3:
			return hairData->compute_G_pdf(theta_i, phi_i, hairData->get_theta_r(), hairData->get_phi_r());
		default:
			AiMsgWarning("No lobe selected");
			return 0.0f;
		}
	}
	else
	{
		return hairData->get_MISspecialPdf();
	}

	return 0.0f;

}

static float ObqHairDualScatEvalPdf_DS(const void *brdf, const AtVector *indir)
{
	const ObqHair *hairData = (const ObqHair*)brdf;
	if((*(hairData->m_MIScurrentStep)) == 2) // brdf sample
	{
		//float theta_i, phi_i;
		//cartesian2sphericalHair(theta_i, phi_i, *indir, hairData->get_U(),hairData->get_V(),hairData->get_W());

		return c_1Over4Pi;
		//return hairData->compute_pdf(theta_i, phi_i,hairData->get_theta_r(),hairData->get_phi_r());
	}
	else
	{
		return hairData->get_MISspecialPdf();
	}

	return 0.0f;
}
