/*
Obq_Hair.h :

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
#ifndef OBQHAIR_H
#define OBQHAIR_H

#include "O_Common.h"


const int c_a_bar_f_nSamples = c_ObqHemisphereThetaRes;

static AtVector ObqHairSingleScatEvalSample(const void *brdf, float rx, float ry);
static AtColor ObqHairSingleScatEvalBrdf(const void *brdf, const AtVector *indir);
static float ObqHairSingleScatEvalPdf(const void *brdf, const AtVector *indir);
static AtVector ObqHairSingleScatEvalSample_separateLobes(const void *brdf, float rx, float ry);
static AtColor ObqHairSingleScatEvalBrdf_separateLobes(const void *brdf, const AtVector *indir);
static float ObqHairSingleScatEvalPdf_separateLobes(const void *brdf, const AtVector *indir);

static AtVector ObqHairDualScatEvalSample(const void *brdf, float rx, float ry);
static AtColor ObqHairDualScatEvalBrdf(const void *brdf, const AtVector *indir);
static AtColor ObqHairDualScatEvalBrdf_separateDS(const void *brdf, const AtVector *indir);
static float ObqHairDualScatEvalPdf(const void *brdf, const AtVector *indir);

static AtVector ObqHairDualScatEvalSample_separateLobes(const void *brdf, float rx, float ry);
static AtColor ObqHairDualScatEvalBrdf_separateLobes(const void *brdf, const AtVector *indir);
static AtColor ObqHairDualScatEvalBrdf_separateDS_separateLobes(const void *brdf, const AtVector *indir);
static float ObqHairDualScatEvalPdf_separateLobes(const void *brdf, const AtVector *indir);
static float ObqHairDualScatEvalPdf_separateLobes_E(const void *brdf, const AtVector *indir);


static AtVector ObqHairDualScatEvalSample_DS(const void *brdf, float rx, float ry);
static AtColor ObqHairDualScatEvalBrdf_DS(const void *brdf, const AtVector *indir);
static float ObqHairDualScatEvalPdf_DS(const void *brdf, const AtVector *indir);


// enum for parameters
//
enum ObqHairParams
{
	p_opacity,
	p_alpha_R,
	p_alpha_TT,
	p_alpha_TRT,
	p_beta_R,
	p_beta_TT,
	p_beta_TRT,
	p_color_R,
	p_color_TT,
	p_color_TRT,
	p_color_g,
	p_gamma_TT,
	p_gamma_g,
	p_phi_g,
	p_scale_R,
	p_scale_TT,
	p_scale_TRT,
	p_scale_g,
	p_alpha_R__pre,
	p_alpha_TT__pre,
	p_alpha_TRT__pre,
	p_beta_R__pre,
	p_beta_TT__pre,
	p_beta_TRT__pre,
	p_color_R__pre,
	p_color_TT__pre,
	p_color_TRT__pre,
	p_color_g__pre,
	p_gamma_TT__pre,
	p_gamma_g__pre,
	p_phi_g__pre,
	p_scale_R__pre,
	p_scale_TT__pre,
	p_scale_TRT__pre,
	p_scale_g__pre,
	p_samples,
	p_scale_direct,
	p_color_direct,
	p_scale_indirect,
	p_color_indirect,
	p_alpha_back,
	p_beta_back,
	p_scale_back,
	p_color_back,
	p_scale_forward,
	p_color_forward,
	p_use_pre_only,
	p_shadow_opacity,
	p_numGMS_modifier,
	p_normal_shifts_widths,
	p_color_ray_glossy,
	p_mode_ray_glossy,
	p_color_ray_diffuse,
	p_mode_ray_diffuse,
	p_d_f,
	p_d_f__pre,
	p_d_b,
	p_d_b__pre,
	p_aov_mode,
	p_singleScatteringMode,
	p_useImportanceSampling,
	p_unlinkOpacity,
	p_color_ray_reflected,
	p_mode_ray_reflected,
	p_color_ray_refracted,
	p_mode_ray_refracted,
	p_checkShaderNameToo,
	//p_useCritical,
	p_modeMIS,
	p_MISrandomLobeSamples,
	p_samples_R,
	p_samples_TT,
	p_samples_TRT,
	p_samples_G,
	p_samples_DS,
	p_modeMIS_DS,
	p_aov_dir_R,
	p_aov_dir_TRT,
	p_aov_dir_g,
	p_aov_dir_TT,
	p_aov_dir_back,
	p_aov_dir_scatter,
	p_aov_dir_scatter_back,
	p_aov_indir_R,
	p_aov_indir_TRT,
	p_aov_indir_g,
	p_aov_indir_TT,
	p_aov_indir_back,
	p_aov_indir_scatter,
	p_aov_indir_scatter_back
};

enum ObqHairRayMode{
	OBQ_HAIR_COLOR,
	OBQ_HAIR_DIRECTNOSCAT,
	OBQ_HAIR_DIRECTINDIRECTNOSCAT,
	OBQ_HAIR_LIKECAMERA
};

enum ObqHairLobes{
	OBQ_LOBE_R,
	OBQ_LOBE_TT,
	OBQ_LOBE_TRT,
	OBQ_LOBE_G,
	OBQ_LOBE_S,
	OBQ_LOBE_SB,
	OBQ_LOBE_B
};

//-------------------------------------------------------------------------------------
// Transform direction omega in cartesian coordinates to spherical for hair
//-------------------------------------------------------------------------------------
inline void cartesian2sphericalHair(float& _theta, float& _phi, AtVector omega, AtVector U, AtVector V, AtVector W)
{

	float omegaDotU = CLAMP(AiV3Dot(omega,U),-1.0f,1.0f);
	_theta = c_PiOver2-std::acos(omegaDotU);

	if(std::abs(omegaDotU) == 1.0f)
	{
		_phi = 0.0f;
	}
	else
	{
		AtVector omegap = AiV3Normalize(omega - omegaDotU * U);
		float phi = std::acos(CLAMP(AiV3Dot(omegap,V),-1.0f,1.0f));
		float d2W = CLAMP(AiV3Dot(omegap,W),-1.0f,1.0f);
		if(d2W<0.0f)
			phi = c_2Pi-phi;

		_phi = phi;
	}
}

//-------------------------------------------------------------------------------------
// Transform direction omega in cartesian coordinates to spherical for hair
//-------------------------------------------------------------------------------------
inline AtVector sphericalHair2cartesian(float theta, float phi, AtVector U, AtVector V, AtVector W)
{
	AtVector v_temp = rodriguesRotation(U,W,c_PiOver2-theta);
	return rodriguesRotation(v_temp,U,phi);
}

//-------------------------------------------------------------------------------------
// Non normalized Gaussian: Unit Height
//-------------------------------------------------------------------------------------

inline float g(float Beta2, float alpha, float theta_h) 
{
	return std::exp( -std::pow(theta_h-alpha,2)/(2.0f*Beta2));
}


// cauchy
//inline float g(float gamma2, float x0, float x) 
//{
//	return c_1OverPi*std::sqrt(gamma2)/(std::pow(x-x0,2)+gamma2);
//}


inline AtColor g(AtColor Beta2, AtColor alpha, float theta_h) 
{
	AtColor ret = {g(Beta2.r,alpha.r,theta_h),g(Beta2.g,alpha.g,theta_h),g(Beta2.b,alpha.b,theta_h)};
	return ret;
}
//-------------------------------------------------------------------------------------
// Normalized Gaussian : Unit area
//-------------------------------------------------------------------------------------
inline float gN(float Beta2, float alpha, float theta_h) 
{

	return std::exp(-std::pow(theta_h-alpha,2)/(2.0f*Beta2))/std::sqrt(2.0f*c_Pi*Beta2);

}

inline AtColor gN(AtColor Beta2, AtColor alpha, float theta_h) 
{
	AtColor ret = {gN(Beta2.r,alpha.r,theta_h),gN(Beta2.g,alpha.g,theta_h),gN(Beta2.b,alpha.b,theta_h)};
	return ret;
}


//-------------------------------------------------------------------------------------
// return index for precomputed array [-Pi/2, Pi/2 ] -> [0, 2*(res-1)]
//-------------------------------------------------------------------------------------
inline int indexFromSignedTheta(float theta)
{
	return CLAMP(static_cast<int>((theta/c_PiOver2)*static_cast<float>(c_ObqHemisphereThetaRes-1)+0.5f)+(c_ObqHemisphereThetaRes-1),0,2*c_a_bar_f_nSamples-2);
}

//-------------------------------------------------------------------------------------
// return index for precomputed array [0, Pi ] -> [0, 2*(res-1)]
//-------------------------------------------------------------------------------------
inline int indexFromUnsignedTheta(float theta)
{
	return CLAMP(static_cast<int>((theta/c_Pi)*static_cast<float>(c_ObqHemisphereThetaRes-1)+0.5f),0,2*c_a_bar_f_nSamples-2);
}

//-------------------------------------------------------------------------------------
// return angle from index [0, 2*(res-1)] -> [-Pi/2,Pi/2]
//-------------------------------------------------------------------------------------
inline float signedThetaFromIndex(int index)
{
	return static_cast<float>(index)/static_cast<float>(c_ObqHemisphereThetaRes-1)*c_PiOver2 + c_PiOver2;
}

//-------------------------------------------------------------------------------------
// return angle from index [0, 2*(res-1)] -> [-Pi/2,Pi/2]
//-------------------------------------------------------------------------------------
inline float unsignedThetaFromIndex(int index)
{
	return static_cast<float>(index)/static_cast<float>(c_ObqHemisphereThetaRes-1)*c_PiOver2 + c_PiOver2;
}

inline float calcPhi(float phi_i, float phi_r)
{
	float phi = std::abs(phi_r - phi_i);
	if(phi > c_Pi)
		phi = c_2Pi - phi;
	return phi;
}

///////////////////////////////////
//
// Obq HAIR CLASS
//
//////////////////////////////////
class ObqHair
{
public:

	ObqHair()
	{
		m_MIScurrentStep = new short;
	}

	~ObqHair()
	{
		delete m_MIScurrentStep;
	}

	inline void evaluateParams(AtShaderGlobals* sg, const AtNode* node, ObqHair& hairData, AtVector U, AtVector V, AtVector W, AtVector wo, float theta_r, float phi_r)
	{
		// Evaluate params from shaderGlobals if parameter is not linked, use preprocess values
		m_alpha_R = (hairData.m_pre_alpha_R?hairData.m_alpha_R:-AiShaderEvalParamFlt(p_alpha_R)*c_Radians);
		m_beta_R = std::max((hairData.m_pre_beta_R?hairData.m_beta_R:AiShaderEvalParamFlt(p_beta_R)*c_Radians),AI_EPSILON);

		if(hairData.m_normal_shifts_widths)
		{
			m_alpha_TT = -m_alpha_R/2.0f;
			m_alpha_TRT = -3.0f*m_alpha_R/2.0f;
			m_beta_TT = m_beta_R/2.0f;
			m_beta_TRT = 2.0f*m_beta_R;
		}
		else
		{
			m_alpha_TT = (hairData.m_pre_alpha_TT?hairData.m_alpha_TT:-AiShaderEvalParamFlt(p_alpha_TT)*c_Radians);
			m_alpha_TRT = (hairData.m_pre_alpha_TRT?hairData.m_alpha_TRT:-AiShaderEvalParamFlt(p_alpha_TRT)*c_Radians);
			m_beta_TT = std::max((hairData.m_pre_beta_TT?hairData.m_beta_TT:-AiShaderEvalParamFlt(p_beta_TT)*c_Radians),AI_EPSILON);
			m_beta_TRT = std::max((hairData.m_pre_beta_TRT?hairData.m_beta_TRT:-AiShaderEvalParamFlt(p_beta_TRT)*c_Radians),AI_EPSILON);
		}

		m_beta2_R = std::pow(m_beta_R,2);
		m_beta2_TT = std::pow(m_beta_TT,2);
		m_beta2_TRT = std::pow(m_beta_TRT,2);
		m_gamma_TT = std::max((hairData.m_pre_gamma_TT?hairData.m_gamma_TT:AiShaderEvalParamFlt(p_gamma_TT)*c_Radians),AI_EPSILON);
		m_gamma_g = std::max((hairData.m_pre_gamma_g?hairData.m_gamma_g:AiShaderEvalParamFlt(p_gamma_g)*c_Radians),AI_EPSILON);
		m_phi_g = (hairData.m_pre_phi_g?hairData.m_phi_g:AiShaderEvalParamFlt(p_phi_g)*c_Radians);
		m_I_R = (hairData.m_pre_I_R?hairData.m_I_R:AiShaderEvalParamRGB(p_color_R));
		m_I_TT = (hairData.m_pre_I_TT?hairData.m_I_TT:AiShaderEvalParamRGB(p_color_TT));
		m_I_TRT = (hairData.m_pre_I_TRT?hairData.m_I_TRT:AiShaderEvalParamRGB(p_color_TRT));
		m_I_g = (hairData.m_pre_I_g?hairData.m_I_g:AiShaderEvalParamRGB(p_color_g));
		m_scale_R = (hairData.m_pre_scale_R?hairData.m_scale_R:AiShaderEvalParamFlt(p_scale_R));
		m_scale_TT = (hairData.m_pre_scale_TT?hairData.m_scale_TT:AiShaderEvalParamFlt(p_scale_TT));
		m_scale_TRT = (hairData.m_pre_scale_TRT?hairData.m_scale_TRT:AiShaderEvalParamFlt(p_scale_TRT));
		m_scale_g = (hairData.m_pre_scale_g?hairData.m_scale_g:AiShaderEvalParamFlt(p_scale_g));
		m_d_f = (hairData.m_pre_d_f?hairData.m_d_f:AiShaderEvalParamFlt(p_d_f));
		m_d_b = (hairData.m_pre_d_b?hairData.m_d_b:AiShaderEvalParamFlt(p_d_b));


		m_samples = hairData.m_samples;
		m_shadow_opacity = hairData.m_shadow_opacity;
		m_numGMS_modifier = hairData.m_numGMS_modifier;
		m_unlinkOpacity = hairData.m_unlinkOpacity;
		m_checkShaderNameToo = hairData.m_checkShaderNameToo;
		//m_useCritical = hairData.m_useCritical;

		m_opacity = AiShaderEvalParamRGB(p_opacity);

		m_scale_direct = AiShaderEvalParamFlt(p_scale_direct);
		m_color_direct = AiShaderEvalParamFlt(p_color_direct);
		m_scale_indirect = AiShaderEvalParamFlt(p_scale_indirect);
		m_color_indirect = AiShaderEvalParamFlt(p_color_indirect);
		m_alpha_back = -AiShaderEvalParamFlt(p_alpha_back)*c_Radians;					// Dual scattering
		m_beta_back = AiShaderEvalParamFlt(p_beta_back)*c_Radians;
		m_scale_back = AiShaderEvalParamFlt(p_scale_back);
		m_color_back = AiShaderEvalParamRGB(p_color_back);
		m_scale_forward = AiShaderEvalParamFlt(p_scale_forward);
		m_color_forward = AiShaderEvalParamRGB(p_color_forward);

		m_mode_ray_glossy = hairData.m_mode_ray_glossy;
		m_mode_ray_diffuse = hairData.m_mode_ray_diffuse;
		m_mode_ray_reflected = hairData.m_mode_ray_reflected;
		m_mode_ray_refracted = hairData.m_mode_ray_refracted;
		m_color_ray_glossy =  (hairData.m_mode_ray_glossy == OBQ_HAIR_COLOR?AiShaderEvalParamRGB(p_color_ray_glossy):AI_RGB_BLACK);
		m_color_ray_diffuse = (hairData.m_mode_ray_diffuse == OBQ_HAIR_COLOR?AiShaderEvalParamRGB(p_color_ray_diffuse):AI_RGB_BLACK);
		m_color_ray_reflected = (hairData.m_mode_ray_reflected == OBQ_HAIR_COLOR?AiShaderEvalParamRGB(p_color_ray_reflected):AI_RGB_BLACK);
		m_color_ray_refracted = (hairData.m_mode_ray_refracted == OBQ_HAIR_COLOR?AiShaderEvalParamRGB(p_color_ray_refracted):AI_RGB_BLACK);

		m_useImportanceSampling = hairData.m_useImportanceSampling;

		m_E_R = 4.0f*std::sqrt(c_2Pi)*m_beta_R*m_scale_R*((m_I_R.r+m_I_R.g+m_I_R.b)/3.0f);
		m_E_TT = c_2Pi*m_beta_TT*m_gamma_TT*m_scale_TT*((m_I_TT.r+m_I_TT.g+m_I_TT.b)/3.0f);
		m_E_TRT = 4.0f*std::sqrt(c_2Pi)*m_beta_TRT*m_scale_TRT*((m_I_TRT.r+m_I_TRT.g+m_I_TRT.b)/3.0f);
		m_E_G = c_4Pi*m_beta_TRT*m_gamma_g*m_scale_g*((m_I_g.r+m_I_g.g+m_I_g.b)/3.0f);
		m_E_Sum = m_E_R+m_E_TT+m_E_TRT+m_E_G;

		// normalization
		m_E_R/=m_E_Sum;
		m_E_TT/=m_E_Sum;
		m_E_TRT/=m_E_Sum;
		m_E_G/=m_E_Sum;

		m_modeMIS = hairData.m_modeMIS;

		m_sg = sg;
		m_dHairData = &hairData;

		m_U = U;
		m_V = V;
		m_W = W;
		m_wo = wo;
		m_theta_r = theta_r;
		m_phi_r = phi_r;

		// prepare for importance sampling
		prepareImportanceSampling();

		m_MISrandomLobeSamples = hairData.m_MISrandomLobeSamples;

		m_samples_R = hairData.m_samples_R;
		m_samples_TT = hairData.m_samples_TT;
		m_samples_TRT = hairData.m_samples_TRT;
		m_samples_G = hairData.m_samples_G;
		m_samples_DS = hairData.m_samples_DS;
		m_modeMIS_DS = hairData.m_modeMIS_DS;

		m_MISsamples = hairData.m_MISsamples;

	}

	inline void evaluateParams(const AtNode* node)
	{
		// Evaluate params from shaderGlobals
		m_normal_shifts_widths = AiNodeGetBool(node,"normal_shifts_widths");

		m_alpha_R = -AiNodeGetFlt(node,"alpha_R__pre")*c_Radians;
		m_beta_R = std::max(AiNodeGetFlt(node,"beta_R__pre")*c_Radians,AI_EPSILON);

		if(m_normal_shifts_widths)
		{
			m_alpha_TT = -m_alpha_R/2.0f;
			m_alpha_TRT = -3.0f*m_alpha_R/2.0f;
			m_beta_TT = m_beta_R/2.0f;
			m_beta_TRT = 2.0f*m_beta_R;
		}
		else
		{
			m_alpha_TT = -AiNodeGetFlt(node,"alpha_TT__pre")*c_Radians;
			m_alpha_TRT = -AiNodeGetFlt(node,"alpha_TRT__pre")*c_Radians;
			m_beta_TT = std::max(AiNodeGetFlt(node,"beta_TT__pre")*c_Radians,AI_EPSILON);
			m_beta_TRT = std::max(AiNodeGetFlt(node,"beta_TRT__pre")*c_Radians,AI_EPSILON);
		}

		m_beta2_R = std::pow(m_beta_R,2);
		m_beta2_TT = std::pow(m_beta_TT,2);
		m_beta2_TRT = std::pow(m_beta_TRT,2);
		m_gamma_TT = std::max(AiNodeGetFlt(node,"gamma_TT__pre")*c_Radians,AI_EPSILON);
		m_gamma_g = std::max(AiNodeGetFlt(node,"gamma_g__pre")*c_Radians,AI_EPSILON);
		m_phi_g = AiNodeGetFlt(node,"phi_g__pre")*c_Radians;
		m_I_R = AiNodeGetRGB(node,"color_R__pre");
		m_I_TT = AiNodeGetRGB(node,"color_TT__pre");
		m_I_TRT = AiNodeGetRGB(node,"color_TRT__pre");
		m_I_g = AiNodeGetRGB(node,"color_g__pre");
		m_scale_R = AiNodeGetFlt(node,"scale_R__pre");
		m_scale_TT = AiNodeGetFlt(node,"scale_TT__pre");
		m_scale_TRT = AiNodeGetFlt(node,"scale_TRT__pre");
		m_scale_g = AiNodeGetFlt(node,"scale_g__pre");
		m_samples = AiNodeGetInt(node,"samples");
		m_shadow_opacity = AiNodeGetRGB(node,"shadow_opacity");
		m_numGMS_modifier = AiNodeGetFlt(node,"numGMS_modifier");
		m_d_f = AiNodeGetFlt(node,"d_f__pre");
		m_d_b = AiNodeGetFlt(node,"d_b__pre");


		m_use_pre_only = AiNodeGetBool(node,"use_pre_only");
		m_pre_beta_R = (m_use_pre_only || !AiNodeIsLinked(node,"beta_R"));
		m_pre_beta_TT = (m_use_pre_only || !AiNodeIsLinked(node,"beta_TT"));
		m_pre_beta_TRT = (m_use_pre_only || !AiNodeIsLinked(node,"beta_TRT"));
		m_pre_alpha_R = (m_use_pre_only || !AiNodeIsLinked(node,"alpha_R"));
		m_pre_alpha_TT = (m_use_pre_only || !AiNodeIsLinked(node,"alpha_TT"));	
		m_pre_alpha_TRT = (m_use_pre_only || !AiNodeIsLinked(node,"alpha_TRT"));
		m_pre_gamma_TT = (m_use_pre_only || !AiNodeIsLinked(node,"gamma_TT"));
		m_pre_gamma_g = (m_use_pre_only || !AiNodeIsLinked(node,"gamma_g"));
		m_pre_phi_g = (m_use_pre_only || !AiNodeIsLinked(node,"phi_g"));
		m_pre_I_R = (m_use_pre_only || !AiNodeIsLinked(node,"color_R"));
		m_pre_I_TT = (m_use_pre_only || !AiNodeIsLinked(node,"color_TT"));
		m_pre_I_TRT = (m_use_pre_only || !AiNodeIsLinked(node,"color_TRT"));
		m_pre_I_g = (m_use_pre_only || !AiNodeIsLinked(node,"color_g"));
		m_pre_scale_R = (m_use_pre_only || !AiNodeIsLinked(node,"scale_R"));
		m_pre_scale_TT = (m_use_pre_only || !AiNodeIsLinked(node,"scale_TT"));
		m_pre_scale_TRT = (m_use_pre_only || !AiNodeIsLinked(node,"scale_TRT"));
		m_pre_scale_g = (m_use_pre_only || !AiNodeIsLinked(node,"scale_g"));
		m_pre_d_f =  (m_use_pre_only || !AiNodeIsLinked(node,"d_f"));		
		m_pre_d_b =  (m_use_pre_only || !AiNodeIsLinked(node,"d_b"));		

		m_mode_ray_glossy = AiNodeGetInt(node,"mode_ray_glossy");
		m_mode_ray_diffuse = AiNodeGetInt(node,"mode_ray_diffuse");
		m_mode_ray_reflected = AiNodeGetInt(node,"mode_ray_reflected");
		m_mode_ray_refracted = AiNodeGetInt(node,"mode_ray_refracted");

		m_singleScattering = AiNodeGetBool(node, "singleScatteringMode");

		m_useImportanceSampling = AiNodeGetBool(node, "useImportanceSampling");

		m_unlinkOpacity = AiNodeGetBool(node, "unlinkOpacity");

		m_checkShaderNameToo = AiNodeGetBool(node, "checkShaderNameToo");
		//m_useCritical = AiNodeGetBool(node, "useCritical");

		m_modeMIS = AiNodeGetInt(node, "modeMIS");

		m_MISrandomLobeSamples = AiNodeGetInt(node, "MISrandomLobeSamples");

		m_samples_R = AiNodeGetInt(node, "samples_R");
		m_samples_TT = AiNodeGetInt(node, "samples_TT");
		m_samples_TRT = AiNodeGetInt(node, "samples_TRT");
		m_samples_G = AiNodeGetInt(node, "samples_G");
		m_samples_DS = AiNodeGetInt(node, "samples_DS");
		m_modeMIS_DS = AiNodeGetInt(node, "modeMIS_DS");

		switch(m_modeMIS)
		{
		case 1:
			m_MISsamples = m_MISrandomLobeSamples;
			break;
		case 2:
			m_MISsamples = m_MISrandomLobeSamples;
			break;
		case 3:
			if(m_modeMIS_DS == 1)
				m_MISsamples = m_samples_R + m_samples_TT + m_samples_TRT + m_samples_G + m_samples_DS;
			else if(m_modeMIS_DS == 2)
				m_MISsamples = m_samples_R + m_samples_TT + m_samples_TRT + m_samples_G;
			else
				m_MISsamples = m_samples_R + m_samples_TT + m_samples_TRT + m_samples_G;
			break;
		default:
			m_MISsamples = 0;
		}
	}



	///////////////////////////////////////
	// Getters : 
	///////////////////////////////////////
	inline AtColor getDirectScale() const {return m_scale_direct*m_color_direct;}
	inline AtColor getIndirectScale() const {return m_scale_indirect*m_color_indirect;}
	inline float get_alpha_back() const { return m_alpha_back; }
	inline float get_beta_back() const { return m_beta_back; }
	inline float get_scale_back() const { return m_scale_back; }
	inline AtColor get_color_back() const { return m_color_back; }
	inline float get_scale_forward() const { return m_scale_forward; }
	inline AtColor get_color_forward() const { return m_color_forward; }
	inline int get_n_samples() const {return m_samples;}
	inline AtColor get_shadow_opacity() const {return m_shadow_opacity;}
	inline float get_numGMS_modifier()const {return m_numGMS_modifier;}
	inline AtColor get_color_ray_glossy() const {return m_color_ray_glossy;}
	inline AtColor get_color_ray_diffuse() const {return m_color_ray_diffuse;}
	inline int get_mode_ray_glossy() const {return m_mode_ray_glossy;}
	inline int get_mode_ray_diffuse() const {return m_mode_ray_diffuse;}
	inline int get_mode_ray_reflected() const {return m_mode_ray_reflected;}
	inline int get_mode_ray_refracted() const {return m_mode_ray_refracted;}
	inline bool  get_singleScattering() const {return m_singleScattering;}
	inline bool get_unlinkOpacity() const {return m_unlinkOpacity;}
	inline bool get_checkShaderNameToo() const {return m_checkShaderNameToo;}
	//inline bool get_criticalSec() const {return m_useCritical;}
	inline int get_modeMIS() const {return m_modeMIS;}
	inline AtVector get_U() const {return m_U;}
	inline AtVector get_V() const {return m_V;}
	inline AtVector get_W() const {return m_W;}
	inline AtVector get_wo() const {return m_wo;}
	inline float get_theta_r() const {return m_theta_r;}
	inline float get_phi_r() const {return m_phi_r;}
	inline AtShaderGlobals* getSG() const {return m_sg;}
	inline AtColor getOpacity() const {return m_opacity;}
	inline ObqHair* getDHairData() const {return m_dHairData;}
	inline float get_d_f() const {return m_d_f;}
	inline float get_d_b() const {return m_d_b;}
	inline AtByte get_MIScurrentLobe() const {return m_MIScurrentLobe;}
	inline AtUInt32 get_MIScurrentShadowCounter() const {return m_MIScurrentShadowCounter;}
	inline bool get_MIScurrentUseRandom() const {return m_MIScurrentUseRandom;}
	inline float get_MISspecialPdf() const {return m_MISspecialPdf;}
	inline AtColor get_MISspecialBCSDFColor() const {return m_MISspecialBCSDFColor;}
	
	inline float get_E_R() const {return m_E_R;}
	inline float get_E_TT() const {return m_E_TT;}
	inline float get_E_TRT() const {return m_E_TRT;}
	inline float get_E_G() const {return m_E_G;}

	inline int get_samples_R() const {return m_samples_R;}
	inline int get_samples_TT() const {return m_samples_TT;}
	inline int get_samples_TRT() const {return m_samples_TRT;}
	inline int get_samples_G() const {return m_samples_G;}
	inline int get_samples_DS() const {return m_samples_G;}

	inline int get_MISsamples() const {return m_MISsamples;}
	inline AtUInt32 get_nIntersectionsSP() const {return m_nIntersectionsSP;}

	inline AtByte getRandomLobeE()
	{
		double random = myRandom__d();

		if(random < m_E_R)
			return OBQ_LOBE_R;
		else if(random < m_E_R+m_E_TT)
			return OBQ_LOBE_TT;
		else if (random < m_E_R + m_E_TT + m_E_TRT)
			return OBQ_LOBE_TRT;
		else
			return OBQ_LOBE_G;

	}

	inline AtByte getRandomLobeU()
	{
		double random = myRandom__d();

		if(random < 0.25)
			return OBQ_LOBE_R;
		else if(random < 0.5)
			return OBQ_LOBE_TT;
		else if (random < 0.75)
			return OBQ_LOBE_TRT;
		else
			return OBQ_LOBE_G;
	}

	inline float getEnergyFromLobe(AtByte l)
	{
		switch(l)
		{
		case OBQ_LOBE_R :
			return m_E_R;
		case OBQ_LOBE_TT:
			return m_E_TT;
		case OBQ_LOBE_TRT:
			return m_E_TRT;
		case OBQ_LOBE_G:
			return m_E_G;
		default:
			return 0;
		}
	}

	///////////////////////////////////////////////////
	// setters THIS MUST BE CALLED BEFORE EACH AiEval
	inline void setMIScurrentInfo(AtByte MIScurrentLobe, bool MIScurrentUseRandom = false)
	{
		*m_MIScurrentStep = 0;
		m_MIScurrentShadowCounter = 0;
		m_MIScurrentLobe = MIScurrentLobe;
		m_MIScurrentUseRandom = MIScurrentUseRandom;
	}

	///////////////////////////////////////////////////
	// setters THIS MUST BE CALLED BEFORE EACH Loop that calls AiEval in Dual Scattering
	inline void setMISspecialInfo(AtColor MISspecialBCSDFColor, float MISspecialPdf, AtUInt32 nIntersectionsSP)
	{
		m_MISspecialPdf = MISspecialPdf;
		m_MISspecialBCSDFColor = MISspecialBCSDFColor;
		m_nIntersectionsSP = nIntersectionsSP;
	}

	inline void resetMIScurrentInfo()
	{
		*m_MIScurrentStep = 0;
		m_MIScurrentLobe = 0;
		m_MIScurrentShadowCounter = 0;
		m_MIScurrentUseRandom = false;
		m_MISspecialPdf = 0.0f;
		m_MISspecialBCSDFColor = AI_RGB_BLACK;
	}


	///////////////////////////////////////////////////////
	// theta [-Pi/2, Pi/2] -> index : [0,2(res-1)] ([0,Pi])
	////////////////////////////////////////////////////////
	inline AtColor get_a_bar_f(float theta) const
	{
		return m_a_bar_f[indexFromSignedTheta(theta)];
	}

	inline AtColor get_a_bar_b(float theta) const
	{
		return m_a_bar_b[indexFromSignedTheta(theta)];
	}

	inline AtColor get_A_bar_b(float theta) const
	{
		return m_A_bar_b[indexFromSignedTheta(theta)];
	}

	inline AtColor get_Delta_bar_b(float theta) const
	{
		return m_Delta_bar_b[indexFromSignedTheta(theta)];
	}

	inline AtColor get_beta_bar_f_2(float theta) const
	{
		int index = indexFromSignedTheta(theta);
		return m_beta_bar_f[index]*m_beta_bar_f[index];
	}

	inline AtColor get_sigma_bar_b_2(float theta) const
	{
		int index = indexFromSignedTheta(theta);
		return m_sigma_bar_b[index]*m_sigma_bar_b[index];
	}

	inline float get_N_G_R(float theta) const
	{
		int index = indexFromSignedTheta(theta);
		return m_N_G_R[index];
	}

	inline float get_N_G_TT(float theta) const
	{
		int index = indexFromSignedTheta(theta);
		return m_N_G_TT[index];
	}

	inline float get_N_G_TRT(float theta) const
	{
		int index = indexFromSignedTheta(theta);
		return m_N_G_TRT[index];
	}

	inline float get_N_G_g(float theta) const
	{
		int index = indexFromSignedTheta(theta);
		return m_N_G_g[index];
	}


	// Ou et al.
	inline float M_R(float theta_h) const {	return g(m_beta2_R, m_alpha_R, theta_h); }
	inline float N_R(float phi) const{ return std::cos(phi/2.0f); }

	inline float M_TT(float theta_h) const {	return g(m_beta2_TT, m_alpha_TT, theta_h); }
	inline float N_TT(float phi) const {	return g(m_gamma_TT*m_gamma_TT,0.0f,c_Pi-phi); }

	inline float M_TRT(float theta_h) const { return g(m_beta2_TRT, m_alpha_TRT, theta_h); }
	inline float N_TRT(float phi) const { return std::cos(phi/2.0f); }

	inline float N_g(float phi) const { return g(std::pow(m_gamma_g,2), 0.0f, abs(phi) - m_phi_g); }

	// MxG
	inline AtColor M_R_G(float theta_h, AtColor sigma_bar_f_2) const
	{	
		AtColor ret = {gN(m_beta2_R+sigma_bar_f_2.r, m_alpha_R, theta_h),gN(m_beta2_R+sigma_bar_f_2.g, m_alpha_R, theta_h),gN(m_beta2_R+sigma_bar_f_2.b, m_alpha_R, theta_h)};

		return ret; 
	}
	inline AtColor M_TT_G(float theta_h, AtColor sigma_bar_f_2) const
	{	
		AtColor ret = {gN(m_beta2_TT+sigma_bar_f_2.r, m_alpha_TT, theta_h),gN(m_beta2_TT+sigma_bar_f_2.g, m_alpha_TT, theta_h),gN(m_beta2_TT+sigma_bar_f_2.b, m_alpha_TT, theta_h)};
		return ret; 
	}
	inline AtColor M_TRT_G(float theta_h, AtColor sigma_bar_f_2) const
	{	
		AtColor ret = {gN(m_beta2_TRT+sigma_bar_f_2.r, m_alpha_TRT, theta_h),gN(m_beta2_TRT+sigma_bar_f_2.g, m_alpha_TRT, theta_h),gN(m_beta2_TRT+sigma_bar_f_2.b, m_alpha_TRT, theta_h)};
		return ret; 
	}




	///////////////
	//	Ou et al.
	///////////////

	//(theta_r, phi_r) = compute_angle(viewing direction)
	inline void sample_R_lobe(float &theta_o, float &phi_o, float uv[2], float theta_r, float phi_r) const
	{
		//float a_R = std::atan(((c_PiOver2 + theta_r)/2.0f - m_alpha_R) / m_beta_R);
		//float b_R = std::atan(((-c_PiOver2 + theta_r)/2.0f - m_alpha_R) / m_beta_R);
		float t = m_beta_R * std::tan(uv[0] * (m_a_R - m_b_R) + m_b_R);
		float theta_h = t + m_alpha_R;
		float theta_i = CLAMP(2.0f * theta_h - theta_r,-c_PiOver2+AI_EPSILON,c_PiOver2-AI_EPSILON);
		float phi = 2.0f * std::asin(1.0f - 2.0f * uv[1]);
		float phi_i = phi_r - phi;
		float phi_pdf = std::cos(phi/2.0f) / 4.0f;
		theta_o = theta_i;
		phi_o = phi_i;
	}

	//(theta_r, phi_r) = compute_angle(viewing direction)
	inline void sample_TT_lobe(float &theta_o, float &phi_o, float uv[2], float theta_r, float phi_r) const
	{
		//float a_TT = std::atan(((c_PiOver2 + theta_r)/2.0f - m_alpha_TT) / m_beta_TT);
		//float b_TT = std::atan(((-c_PiOver2 + theta_r)/2.0f - m_alpha_TT) / m_beta_TT);
		//float c_TT = 2.0f * std::atan(c_PiOver2 / m_gamma_TT);
		float t = m_beta_TT * std::tan(uv[0] * (m_a_TT - m_b_TT) + m_b_TT);
		float theta_h = t + m_alpha_TT;
		float theta_i = CLAMP(2.0f * theta_h - theta_r,-c_PiOver2+AI_EPSILON,c_PiOver2-AI_EPSILON);
		float p = m_gamma_TT * tan((uv[1] - 0.5f) * m_c_TT);
		float phi = p + c_Pi;
		float phi_i = phi_r - phi;
		theta_o = theta_i;
		phi_o = phi_i;
	}


	inline void sample_TRT_G_lobe(float &theta_o, float &phi_o, float uv[2], float theta_r, float phi_r) const
	{
		//float a_TRT = std::atan(((c_PiOver2 + theta_r)/2.0f - m_alpha_TRT) / m_beta_TRT);
		//float b_TRT = std::atan(((-c_PiOver2 + theta_r)/2.0f - m_alpha_TRT) / m_beta_TRT);
		float t = m_beta_TRT * tan(uv[0] * (m_a_TRT - m_b_TRT) + m_b_TRT);
		float theta_h = t + m_alpha_TRT;
		float theta_i = CLAMP(2.0f * theta_h - theta_r,-c_PiOver2+AI_EPSILON,c_PiOver2-AI_EPSILON);
		float phi = 2.0f * std::asin(1.0f - 2.0f * uv[1]);
		float phi_i = phi_r - phi;
		float phi_pdf = std::cos(phi/2.0f) / 4.0f;
		theta_o = theta_i;
		phi_o = phi_i;
	}

	inline void sample_G_lobe(float &theta_o, float &phi_o, float uv[2], float theta_r, float phi_r) const
	{
		//float a_TRT = std::atan(((c_PiOver2 + theta_r)/2.0f - m_alpha_TRT) / m_beta_TRT);
		//float b_TRT = std::atan(((-c_PiOver2 + theta_r)/2.0f - m_alpha_TRT) / m_beta_TRT);
		//float c_G = std::atan((c_PiOver2 - m_phi_g) / m_gamma_g);
		//float d_G = std::atan(-m_phi_g / m_gamma_g);
		float t = m_beta_TRT * std::tan(uv[0] * (m_a_TRT - m_b_TRT) + m_b_TRT);
		float theta_h = t + m_alpha_TRT;
		float theta_i = CLAMP(2.0f * theta_h - theta_r,-c_PiOver2+AI_EPSILON,c_PiOver2-AI_EPSILON);
		float sign = 1.0f;

		if(uv[1] < 0.5f)
		{
			uv[1] = 2.0f * uv[1];
			sign = 1.0f;
		}
		else
		{
			uv[1] = 2.0f * (1.0f - uv[1]);
			sign = -1.0f;
		}

		float p = m_gamma_g * tan(uv[1] * (m_c_G - m_d_G) + m_d_G);
		float phi = sign * (p + m_phi_g);
		float phi_i = phi_r - phi;

		theta_o = theta_i;
		phi_o = phi_i;
	}


	// compute the pdf of primary highlight  (theta_r, phi_r) = compute_angle(I) (theta_i, phi_i) = compute_angle(L)
	inline float compute_R_pdf(float theta_i, float phi_i, float theta_r, float phi_r) const
	{

		if(c_PiOver2 - std::abs(theta_i) < AI_EPSILON)
			return 0.0f;

		//float a_R = std::atan(((c_PiOver2 + theta_r)/2.0f - m_alpha_R) / m_beta_R);
		//float b_R = std::atan(((-c_PiOver2 + theta_r)/2 - m_alpha_R) / m_beta_R);
		float theta_h = (theta_i + theta_r) / 2.0f;
		float t = theta_h - m_alpha_R;
		float theta_pdf = m_beta_R / (t*t + m_beta2_R) / (2.0f*(m_a_R - m_b_R) * std::cos(theta_i));
		float phi = calcPhi(phi_i,phi_r);//phi_r - phi_i;
		float phi_pdf = cos(phi/2.0f) / 4.0f;
		return std::abs(theta_pdf * phi_pdf);
	}



	// compute the pdf of transmission (theta_r, phi_r) = compute_angle(I) (theta_i, phi_i) = compute_angle(L)
	inline float compute_TT_pdf(float theta_i, float phi_i, float theta_r, float phi_r) const
	{
		if(c_PiOver2 - std::abs(theta_i) < AI_EPSILON)
			return 0.0f;

		//float a_TT = std::atan(((c_PiOver2 + theta_r)/2.0f - m_alpha_TT) / m_beta_TT);
		//float b_TT = std::atan(((-c_PiOver2 + theta_r)/2.0f - m_alpha_TT) / m_beta_TT);
		//float c_TT = 2.0f * std::atan(c_PiOver2 / m_gamma_TT);
		float theta_h = (theta_i + theta_r) / 2.0f;
		float t = theta_h - m_alpha_TT;
		float theta_pdf = m_beta_TT / (t*t + m_beta2_TT) / (2.0f*(m_a_TT - m_b_TT) * std::cos(theta_i));
		float phi = calcPhi(phi_i,phi_r);//std::abs(phi_r - phi_i);
		float phi_pdf = 0.0f;
		if(phi < c_PiOver2)
			phi_pdf = 0.0f;
		else
		{
			float p = c_Pi - phi;
			phi_pdf = (m_gamma_TT / (p * p + m_gamma_TT * m_gamma_TT)) / m_c_TT;
		}
		return std::abs(theta_pdf * phi_pdf);
	}


	// compute the pdf of secondary highlight without glint (theta_r, phi_r) = compute_angle(I) (theta_i, phi_i) = compute_angle(L)
	inline float compute_TRT_G_pdf(float theta_i, float phi_i, float theta_r, float phi_r) const
	{
		if(c_PiOver2 - std::abs(theta_i) < AI_EPSILON)
			return 0.0f;

		/*float a_TRT = std::atan(((c_PiOver2 + theta_r)/2.0f - m_alpha_TRT) / m_beta_TRT);
		float b_TRT = std::atan(((-c_PiOver2 + theta_r)/2.0f - m_alpha_TRT) / m_beta_TRT);*/
		float theta_h = (theta_i + theta_r) / 2.0f;
		float t = theta_h - m_alpha_TRT;
		float theta_pdf = m_beta_TRT / (t*t + m_beta2_TRT) / (2.0f*(m_a_TRT - m_b_TRT) * std::cos(theta_i));
		float phi = calcPhi(phi_i,phi_r);//phi_r - phi_i;
		float phi_pdf = std::cos(phi/2.0f) / 4.0f;

		return std::abs(theta_pdf * phi_pdf);
	}

	// compute the pdf of glint term (theta_r, phi_r) = compute_angle(I) (theta_i, phi_i) = compute_angle(L)


	inline float compute_G_pdf(float theta_i, float phi_i, float theta_r, float phi_r) const
	{
		if(c_PiOver2 - std::abs(theta_i) < AI_EPSILON)
			return 0.0f;

		//float a_TRT = std::atan(((c_PiOver2 + theta_r)/2.0f - m_alpha_TRT) / m_beta_TRT);
		//float b_TRT = std::atan(((-c_PiOver2 + theta_r)/2.0f - m_alpha_TRT) / m_beta_TRT);
		//float c_G = std::atan((c_PiOver2 - m_phi_g) / m_gamma_g);
		//float d_G = std::atan(-m_phi_g / m_gamma_g);
		float theta_h = (theta_i + theta_r) / 2.0f;
		float t = theta_h - m_alpha_TRT;
		float theta_pdf = m_beta_TRT / (t*t + m_beta2_TRT) / (2.0f*(m_a_TRT - m_b_TRT) * std::cos(theta_i));
		float phi =  calcPhi(phi_i,phi_r);//std::abs(phi_r - phi_i);
		float p = phi - m_phi_g;
		float phi_pdf = m_gamma_g / (p*p + m_gamma_g * m_gamma_g) / (2.0f * (m_c_G - m_d_G));

		return std::abs(theta_pdf * phi_pdf);
	}

	// Uniform version ie not energy based
	inline float compute_pdf2(float theta_i, float phi_i, float theta_r, float phi_r) const
	{
		float pdf_R = compute_R_pdf(theta_i, phi_i, theta_r, phi_r);
		float pdf_TT = compute_TT_pdf(theta_i, phi_i, theta_r, phi_r);
		float pdf_TRT_G = compute_TRT_G_pdf(theta_i, phi_i, theta_r, phi_r);
		float pdf_G = compute_G_pdf(theta_i, phi_i, theta_r, phi_r);
		return (pdf_R + pdf_TT + pdf_TRT_G + pdf_G) / 4.0f;
	}

	// energy based
	inline float compute_pdf(float theta_i, float phi_i, float theta_r, float phi_r) const
	{
		float pdf_R = compute_R_pdf(theta_i, phi_i, theta_r, phi_r)*m_E_R;
		float pdf_TT = compute_TT_pdf(theta_i, phi_i, theta_r, phi_r)*m_E_TT;
		float pdf_TRT_G = compute_TRT_G_pdf(theta_i, phi_i, theta_r, phi_r)*m_E_TRT;
		float pdf_G = compute_G_pdf(theta_i, phi_i, theta_r, phi_r)*m_E_G;
		return pdf_R + pdf_TT + pdf_TRT_G + pdf_G;
	}


	inline float sample_brdf2(float uv[2], float& theta_o, float& phi_o, float theta_r, float phi_r) const
	{
		if(uv[0] < 0.5f && uv[1] < 0.5f)
		{
			// Sample R lobe
			uv[0] = 2.0f * uv[0];
			uv[1] = 2.0f * uv[1];
			sample_R_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		}
		else if(uv[0] >= 0.5f && uv[1] < 0.5f)
		{
			// Sample TT lobe
			uv[0] = 2.0f * (1.0f - uv[0]);
			uv[1] = 2.0f * uv[1];
			sample_TT_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		}
		else if (uv[0] < 0.5f && uv[1] >= 0.5f)
		{
			// Sample TRT-G lobe
			uv[0] = 2.0f * uv[0];
			uv[1] = 2.0f * (1.0f - uv[1]);
			sample_TRT_G_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		}
		else
		{
			// Sample glint lobe
			uv[0] = 2.0f * (1.0f - uv[0]);
			uv[1] = 2.0f * (1.0f - uv[1]);
			sample_G_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		}
		float pdf = compute_pdf2(theta_o, phi_o,theta_r,phi_r);
		return pdf;
	}

	inline AtVector sample_dir2(float uv[2],float theta_r, float phi_r, AtVector U, AtVector V, AtVector W ) const
	{
		float theta_o, phi_o;

		if(uv[0] < 0.5f && uv[1] < 0.5f)
		{
			// Sample R lobe
			uv[0] = 2.0f * uv[0];
			uv[1] = 2.0f * uv[1];
			sample_R_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		}
		else if(uv[0] >= 0.5f && uv[1] < 0.5f)
		{
			// Sample TT lobe
			uv[0] = 2.0f * (1.0f - uv[0]);
			uv[1] = 2.0f * uv[1];
			sample_TT_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		}
		else if (uv[0] < 0.5f && uv[1] >= 0.5f)
		{
			// Sample TRT-G lobe
			uv[0] = 2.0f * uv[0];
			uv[1] = 2.0f * (1.0f - uv[1]);
			sample_TRT_G_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		}
		else
		{
			// Sample glint lobe
			uv[0] = 2.0f * (1.0f - uv[0]);
			uv[1] = 2.0f * (1.0f - uv[1]);
			sample_G_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		}

		return sphericalHair2cartesian(theta_o,phi_o,U,V,W);
	}

	inline float sample_brdf(float uv[2], double random, float& theta_o, float& phi_o, float theta_r, float phi_r) const
	{
		if(random < m_E_R)
			sample_R_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		else if(random < m_E_R+m_E_TT)
			sample_TT_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		else if (random < m_E_R + m_E_TT + m_E_TRT)
			sample_TRT_G_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		else
			sample_G_lobe(theta_o, phi_o, uv, theta_r,phi_r);

		return compute_pdf(theta_o, phi_o,theta_r,phi_r);
	}

	inline float sample_brdf(float uv[2],float& theta_o, float& phi_o, float theta_r, float phi_r) const
	{
		float random = myRandom();

		if(random < m_E_R)
			sample_R_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		else if(random < m_E_R+m_E_TT)
			sample_TT_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		else if (random < m_E_R + m_E_TT + m_E_TRT)
			sample_TRT_G_lobe(theta_o, phi_o, uv, theta_r,phi_r);
		else
			sample_G_lobe(theta_o, phi_o, uv, theta_r,phi_r);

		return compute_pdf(theta_o, phi_o,theta_r,phi_r);
	}


	inline AtVector sample_dir(float uv[2], double random) const
	{
		float theta_o, phi_o;
		if(random < m_E_R)
			sample_R_lobe(theta_o, phi_o, uv, m_theta_r,m_phi_r);
		else if(random < m_E_R+m_E_TT)
			sample_TT_lobe(theta_o, phi_o, uv, m_theta_r,m_phi_r);
		else if (random < m_E_R + m_E_TT + m_E_TRT)
			sample_TRT_G_lobe(theta_o, phi_o, uv, m_theta_r,m_phi_r);
		else
			sample_G_lobe(theta_o, phi_o, uv, m_theta_r,m_phi_r);

		return sphericalHair2cartesian(theta_o,phi_o,m_U,m_V,m_W);
	}


	// MUST BE CALLED BEFORE USING THE IMPORTANCE SAMPLING FUNCTIONS
	inline void prepareImportanceSampling()
	{
		m_a_R = std::atan2(((c_PiOver2 + m_theta_r)/2.0f - m_alpha_R), m_beta_R);
		m_b_R = std::atan2(((-c_PiOver2 + m_theta_r)/2.0f - m_alpha_R), m_beta_R);
		m_a_TT = std::atan2(((c_PiOver2 + m_theta_r)/2.0f - m_alpha_TT), m_beta_TT);
		m_b_TT = std::atan2(((-c_PiOver2 + m_theta_r)/2.0f - m_alpha_TT), m_beta_TT);
		m_c_TT = 2.0f * std::atan2(c_PiOver2, m_gamma_TT);
		m_a_TRT = std::atan2(((c_PiOver2 + m_theta_r)/2.0f - m_alpha_TRT) , m_beta_TRT);
		m_b_TRT = std::atan2(((-c_PiOver2 + m_theta_r)/2.0f - m_alpha_TRT) , m_beta_TRT);
		m_c_G = std::atan2((c_PiOver2 - m_phi_g) , m_gamma_g);
		m_d_G = std::atan2(-m_phi_g , m_gamma_g);
	}


	// Hair BSDF : Ou et al. 
	AtColor S(float theta_i, float phi_i, float theta_r, float phi_r)
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);


		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);


		AtColor cR = m_I_R*M_R(theta_h)*N_R(phi)/cos2theta_d;
		AtColor cTT = m_I_TT*M_TT(theta_h)*N_TT(phi)/cos2theta_d;
		AtColor cTRT = m_I_TRT*M_TRT(theta_h)*N_TRT(phi)/cos2theta_d;
		//AtColor cg = m_I_TRT*M_TRT(theta_h)*m_I_g*N_g(phi)/cos2theta_d;
		AtColor cg = M_TRT(theta_h)*m_I_g*N_g(phi)/cos2theta_d;

		return cR + cTT + cTRT + cg;
	}

	// Hair BSDF : Ou et al. 
	inline void S(AtColor& cR, AtColor& cTT, AtColor& cTRT, AtColor& cg, float theta_i, float phi_i, float theta_r, float phi_r)
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);


		cR = m_I_R*M_R(theta_h)*N_R(phi)/cos2theta_d;
		cTT = m_I_TT*M_TT(theta_h)*N_TT(phi)/cos2theta_d;
		cTRT = m_I_TRT*M_TRT(theta_h)*N_TRT(phi)/cos2theta_d;
		cg =	M_TRT(theta_h)*m_I_g*N_g(phi)/cos2theta_d;

	}

	inline void S_scaled(AtColor& cR, AtColor& cTT, AtColor& cTRT, AtColor& cg, float theta_i, float phi_i, float theta_r, float phi_r) const
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);

		cR =	m_scale_R*	m_I_R*M_R(theta_h)*		N_R(phi)		/cos2theta_d;
		cTT =	m_scale_TT*	m_I_TT*M_TT(theta_h)*	N_TT(phi)		/cos2theta_d;
		cTRT =	m_scale_TRT*m_I_TRT*M_TRT(theta_h)*	N_TRT(phi)		/cos2theta_d;
		cg =	m_scale_g*	M_TRT(theta_h)*	m_I_g*N_g(phi)	/cos2theta_d;

	}

	inline AtColor S_scaled(const float theta_i, const float phi_i, const float theta_r, const float phi_r) const 
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);

		AtColor cR =	m_scale_R*	m_I_R*M_R(theta_h)*		N_R(phi)		/cos2theta_d;
		AtColor cTT =	m_scale_TT*	m_I_TT*M_TT(theta_h)*	N_TT(phi)		/cos2theta_d;
		AtColor cTRT =	m_scale_TRT*m_I_TRT*M_TRT(theta_h)*	N_TRT(phi)		/cos2theta_d;
		AtColor cg =	m_scale_g*M_TRT(theta_h)*	m_I_g*N_g(phi)	/cos2theta_d;

		return cR+cTT+cTRT+cg;
	}

	inline AtColor S_scaled_R(const float theta_i, const float phi_i, const float theta_r, const float phi_r) const 
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);

		return m_scale_R*	m_I_R*M_R(theta_h)*		N_R(phi)		/cos2theta_d;
	}

	inline AtColor S_scaled_TT(const float theta_i, const float phi_i, const float theta_r, const float phi_r) const 
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);

		return m_scale_TT*	m_I_TT*M_TT(theta_h)*	N_TT(phi)		/cos2theta_d;
	}

	inline AtColor S_scaled_TRT(const float theta_i, const float phi_i, const float theta_r, const float phi_r) const 
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);

		return m_scale_TRT*m_I_TRT*M_TRT(theta_h)*	N_TRT(phi)		/cos2theta_d;
	}

	inline AtColor S_scaled_g(const float theta_i, const float phi_i, const float theta_r, const float phi_r) const 
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);

		return m_scale_g*M_TRT(theta_h)*	m_I_g*N_g(phi)	/cos2theta_d;
	}

	// Aritistic
	AtColor S_G(float theta_i, float phi_i, float theta_r, float phi_r, AtColor sigma_bar_f_2, ObqHair& hairData) const
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);


		AtColor cR =   m_I_R*M_R_G(theta_h,sigma_bar_f_2)*hairData.get_N_G_R(theta_i)/cos2theta_d;
		AtColor cTT =  m_I_TT*M_TT_G(theta_h,sigma_bar_f_2)*hairData.get_N_G_TT(theta_i)/cos2theta_d;
		AtColor cTRT = m_I_TRT*M_TRT_G(theta_h,sigma_bar_f_2)*hairData.get_N_G_TRT(theta_i)/cos2theta_d;
		AtColor cg =   M_TRT_G(theta_h,sigma_bar_f_2)*m_I_g*hairData.get_N_G_g(theta_i)/cos2theta_d;

		return cR + cTT + cTRT + cg;
	}



	/////////////////////////////
	// Sadeghi Artistic
	/////////////////////////////
	inline void f_prime_s_full(float& cR_norm, float& cTT_norm, float& cTRT_norm, float& cg_norm, float theta_i, ObqHemisphere& hemi)
	{
		cR_norm=0.0f;
		cTT_norm=0.0f;
		cTRT_norm=0.0f;
		cg_norm=0.0f;

		int res = c_ObqHemisphereThetaRes;
		int resM1 = res - 1;
		float resF = static_cast<float>(res);
		float resFM1 = resF - 1.0f;

		AtColor loop_full = AI_RGB_BLACK;
		float totalOmega = 0.0f;
		for(int theta_r_id = -resM1; theta_r_id < res; theta_r_id++)
		{

			float theta_r = static_cast<float>(theta_r_id)/resFM1*c_PiOver2;

			float theta_h = (theta_r+theta_i)/2.0f;
			float theta_d = (theta_r-theta_i)/2.0f;

			float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);

			if(std::abs(theta_r_id) == resM1) // poles
			{
				float phi = 0.0f;
				float omega = hemi.getOmega(std::abs(theta_r_id))*2.0f;
				cR_norm +=	(M_R(theta_h)*	N_R(phi)	/cos2theta_d) * omega;
				cTT_norm +=	(M_TT(theta_h)*	N_TT(phi)	/cos2theta_d) * omega;
				cTRT_norm +=(M_TRT(theta_h)*N_TRT(phi)	/cos2theta_d) * omega;
				cg_norm +=	(M_TRT(theta_h)*N_g(phi)	/cos2theta_d) * omega;

				totalOmega+=omega;
			}
			else
			{
				for(int phi_id = 0; phi_id < 4*resM1; phi_id++)
				{

					float phi = (phi_id <= 2*resM1? static_cast<float>(phi_id)/(2.0f*resFM1)*c_Pi:static_cast<float>(4*resM1-phi_id)/(2.0f*resFM1)*c_Pi );
					float omega = hemi.getOmega(std::abs(theta_r_id));

					cR_norm +=	(M_R(theta_h)*	N_R(phi)	/cos2theta_d) * omega;
					cTT_norm +=	(M_TT(theta_h)*	N_TT(phi)	/cos2theta_d) * omega;
					cTRT_norm +=(M_TRT(theta_h)*N_TRT(phi)	/cos2theta_d) * omega;
					cg_norm +=	(M_TRT(theta_h)*N_g(phi)	/cos2theta_d) * omega;

					totalOmega += omega;
				}
			}
		}

		cR_norm  *=	m_scale_R;
		cTT_norm *=	m_scale_TT;
		cTRT_norm*= m_scale_TRT;
		cg_norm  *=	m_scale_g;

	}
	////////// - Colored Version
	inline void f_prime_s_full(AtColor& cR_norm, AtColor& cTT_norm, AtColor& cTRT_norm, AtColor& cg_norm, float theta_i, ObqHemisphere& hemi)
	{
		float R_norm=0.0f, TT_norm=0.0f, TRT_norm=0.0f, g_norm=0.0f;
		f_prime_s_full(R_norm, TT_norm, TRT_norm, g_norm, theta_i, hemi);
		cR_norm = R_norm*m_I_R;
		cTT_norm = TT_norm*m_I_TT;
		cTRT_norm = TRT_norm*m_I_TRT;
		cg_norm = g_norm*m_I_g;
	}
	/////// - total
	inline AtColor f_prime_s_full(float theta_i, ObqHemisphere& hemi)
	{
		float R_norm=0.0f, TT_norm=0.0f, TRT_norm=0.0f, g_norm=0.0f;
		f_prime_s_full(R_norm, TT_norm, TRT_norm, g_norm, theta_i, hemi);

		return R_norm*m_I_R + TT_norm*m_I_TT + TRT_norm*m_I_TRT + g_norm*m_I_g;
	}


	////// Sodeghi Artistic BCSDF
	inline void f_prime_uncolored(float& cR, float& cTT, float& cTRT, float& cg,float theta_i, float phi_i, float theta_r, float phi_r)
	{
		float theta_h = (theta_r+theta_i)/2.0f;
		float theta_d = (theta_r-theta_i)/2.0f;
		float phi = calcPhi(phi_i,phi_r);

		float cos2theta_d = CLAMP(static_cast<float>(std::pow(std::cos(theta_d),2)),0.0001f,1.0f);

		cR =	m_scale_R*	M_R(theta_h)*	N_R(phi)	/cos2theta_d;
		cTT =	m_scale_TT*	M_TT(theta_h)*	N_TT(phi)	/cos2theta_d;
		cTRT =	m_scale_TRT*M_TRT(theta_h)*	N_TRT(phi)	/cos2theta_d;
		cg =	m_scale_g*	M_TRT(theta_h)*	N_g(phi)	/cos2theta_d;

	}

	///////////////////////////////////////
	// Shading functions : 
	///////////////////////////////////////


	inline void direct_shading_full(AtColor& loop_f_scatter, AtColor& loop_f_back_scatter, AtColor& loop_f_direct_cR, AtColor& loop_f_direct_cTT, AtColor& loop_f_direct_cTRT, AtColor& loop_f_direct_cg, AtColor& loop_f_back_direct, AtShaderGlobals* sg, ObqHair& dHairData, /*AtCritSec* criticalSec,*/ AtVector& U, AtVector& V, AtVector& W, float theta_r, float phi_r){

		AtColor directScale = getDirectScale();

		if(!AiColorIsSmall(directScale))
		{
			bool hemi = sg->fhemi;
			sg->fhemi = false;

			// Prepare lights in slef scattering mode
			char lightNameMode[64];
#ifdef _WIN32
			sprintf_s(lightNameMode, "ObqH_SSM_%u_%u",sg->transp_index, sg->Rr);
#else
			sprintf(lightNameMode, "ObqH_SSM_%u_%u",sg->transp_index, sg->Rr);
#endif
			// TEST to be sure there are no consflict here:
			int test = 0;
			bool alreadyThere = false;

			//if(m_useCritical)	AiCritSecEnter(criticalSec);

			alreadyThere = AiStateGetMsgInt(lightNameMode, &test);

			if(!alreadyThere)	AiStateSetMsgInt(lightNameMode, 1);

			//if(m_useCritical)	AiCritSecLeave(criticalSec);

			// if theres
			if(alreadyThere)
			{
				direct_shading_NoScat(loop_f_direct_cR, loop_f_direct_cTT, loop_f_direct_cTRT, loop_f_direct_cg,sg);
				return;
			}

			AiLightsPrepare(sg);

			AiStateSetMsgInt(lightNameMode, 0);

			AtNode* previous = NULL;
			int sampleNum = 0;

			while(AiLightsGetSample(sg))
			{

				// Get the spherical angles of the Light direction relative to the hair fibre
				AtVector wi = sg->Ld;
				float theta_i, phi_i;
				cartesian2sphericalHair(theta_i, phi_i, wi, U,V,W);

				if(sg->Lp == previous)
				{
					++sampleNum;
				}
				else
				{
					previous = sg->Lp;
					sampleNum = 0;
				}

				// DUAL SCATTERING ( Forward)
				//-----------------
				// DualScattering Tf

				float numGMS_f = 0.0f;

				AtColor forwardScale = get_scale_forward()*get_color_forward();
				bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);

				AtColor backScale = get_scale_back()*get_color_back();
				bool backScaleIsSmall = AiColorIsSmall(backScale);

				AtUInt32 nIntersections = 0; 

				// Count was done during AiLightsPrepare ?
				AtUInt32* counterArray;
				char lightName[96];
#ifdef _WIN32
			sprintf_s(lightName, "ObqH_%p_%u_%u", sg->Lp, sg->transp_index, sg->Rr); // safer to use snprintf...
#else
			sprintf(lightName, "ObqH_%p_%u_%u", sg->Lp, sg->transp_index, sg->Rr); // safer to use snprintf...
#endif
				

				if(AiStateGetMsgPtr(lightName,(void**)(&counterArray)))
				{
					nIntersections = counterArray[sampleNum];

					// if last sample, free memory of array
					if(sampleNum == sg->n - 1)
						delete[] counterArray;
				}

				numGMS_f = std::pow(static_cast<float>(nIntersections),get_numGMS_modifier());

				float theta_h = (theta_r+theta_i)/2.0f;
				float theta_d = (theta_r-theta_i)/2.0f;

				AtColor lightC = sg->Li*sg->we*std::abs(std::cos(theta_i));

				AtColor selfOpacity = (m_unlinkOpacity?m_shadow_opacity:m_opacity);
				AtColor directFraction = (numGMS_f==0.0 || selfOpacity == AI_RGB_BLACK ?AI_RGB_WHITE:AI_RGB_WHITE-colorChannelPow(AiColorClamp(selfOpacity,0.0f,1.0f),numGMS_f));

				bool direcFractionIsSmall = AiColorIsSmall(directFraction);

				AtColor forward_fraction = AI_RGB_WHITE - directFraction;

				// At least forward scattering once -> Scatter
				if(!AiColorIsSmall(forward_fraction))
				{
					bool print = (rand()%100 == 56);

					AtColor sigma_bar_f_2 = dHairData.get_beta_bar_f_2(theta_i)*numGMS_f;

					AtColor a_bar_f_color = dHairData.get_a_bar_f(theta_i);

					AtColor T_f = { m_d_f*std::pow(a_bar_f_color.r,numGMS_f),	m_d_f*std::pow(a_bar_f_color.g,numGMS_f),	m_d_f*std::pow(a_bar_f_color.b,numGMS_f)};

					forward_fraction = colorChannelMax(T_f-directFraction,0.0f);

					AtColor f_s_scatter = S_G(theta_i,phi_i,theta_r,phi_r,dHairData.get_sigma_bar_b_2(theta_i),dHairData);

					f_s_scatter *= forwardScale;

					if(!backScaleIsSmall)
					{
						AtColor f_back_scatter = 2.0f*dHairData.get_A_bar_b(theta_i)*gN(colorChannelMax(dHairData.get_sigma_bar_b_2(theta_i) + sigma_bar_f_2 + get_beta_back(),AI_EPSILON) ,dHairData.get_Delta_bar_b(theta_i),theta_h+get_alpha_back())/(c_Pi*std::cos(theta_d)*std::cos(theta_d));

						f_back_scatter *= backScale;

						loop_f_back_scatter += forward_fraction*lightC*m_d_f*c_Pi*m_d_b*f_back_scatter;

					}

					loop_f_scatter += forward_fraction*lightC*m_d_f*f_s_scatter;

				}

				//else // -> direct
				if(!direcFractionIsSmall)
				{
					AtColor cR = AI_RGB_BLACK, cTT = AI_RGB_BLACK, cTRT = AI_RGB_BLACK, cg = AI_RGB_BLACK;

					S_scaled(cR, cTT, cTRT, cg, theta_i,phi_i,theta_r,phi_r);

					loop_f_direct_cR		+=	directFraction*lightC*cR;
					loop_f_direct_cTT		+=	directFraction*lightC*cTT;
					loop_f_direct_cTRT		+=	directFraction*lightC*cTRT;
					loop_f_direct_cg		+=	directFraction*lightC*cg;

					if(!backScaleIsSmall)
					{
						AtColor f_back_direct = 2.0f*dHairData.get_A_bar_b(theta_i) * gN(colorChannelMax(dHairData.get_sigma_bar_b_2(theta_i)+get_beta_back(),AI_EPSILON),dHairData.get_Delta_bar_b(theta_i),theta_h+get_alpha_back())/(c_Pi*std::cos(theta_i)*std::cos(theta_i));
						loop_f_back_direct	+=	directFraction*lightC*m_d_b*f_back_direct*backScale;
					}

				}

			}
			sg->fhemi = hemi; // put hemispherical thigny back

			loop_f_direct_cR		*=directScale;
			loop_f_direct_cTT		*=directScale;
			loop_f_direct_cTRT		*=directScale;
			loop_f_direct_cg		*=directScale;

			loop_f_back_direct	*=directScale;
			loop_f_scatter		*=directScale;
			loop_f_back_scatter	*=directScale;
		}



	}

	/////////////////////////////////////////
	//// Shading functions : direct full MIS
	/////////////////////////////////////////
	inline void direct_shading_full_MIS_Options(AtColor& loop_f_scatter, AtColor& loop_f_back_scatter, AtColor& loop_f_direct_cR, AtColor& loop_f_direct_cTT, AtColor& loop_f_direct_cTRT, AtColor& loop_f_direct_cg, AtColor& loop_f_back_direct, AtShaderGlobals* sg, ObqHair& dHairData, /*AtCritSec* criticalSec,*/ AtVector& U, AtVector& V, AtVector& W, float theta_r, float phi_r){

		AtColor directScale = getDirectScale();

		if(!AiColorIsSmall(directScale))
		{
			bool hemi = sg->fhemi;
			sg->fhemi = false;

			// Prepare lights in slef scattering mode
			char lightNameMode[64];

#ifdef _WIN32
			sprintf_s(lightNameMode, "ObqH_SSM_%u_%u",sg->transp_index, sg->Rr);
#else
			sprintf(lightNameMode, "ObqH_SSM_%u_%u",sg->transp_index, sg->Rr);
#endif

			

			// TEST to be sure there are no consflict here:
			int test = 0;
			bool alreadyThere = false;

			//if(m_useCritical)	AiCritSecEnter(criticalSec);
			alreadyThere = AiStateGetMsgInt(lightNameMode, &test);
			if(!alreadyThere)	AiStateSetMsgInt(lightNameMode, 1);
			//if(m_useCritical)	AiCritSecLeave(criticalSec);

			// if theres
			if(alreadyThere)
			{
				direct_shading_NoScat(loop_f_direct_cR, loop_f_direct_cTT, loop_f_direct_cTRT, loop_f_direct_cg,sg);
				return;
			}

			AiLightsPrepare(sg);
			//AiCritSecEnter(criticalSec);
			AiStateSetMsgInt(lightNameMode, 2);
			//AiCritSecLeave(criticalSec);
			resetMIScurrentInfo();

			char lightNameCounter[64];

#ifdef _WIN32
			sprintf_s(lightNameCounter, "ObqHm_%u_%u",sg->transp_index, sg->Rr);
#else
			sprintf(lightNameCounter, "ObqHm_%u_%u",sg->transp_index, sg->Rr);
#endif

			


			AiStateSetMsgPtr(lightNameCounter, (void*)(&m_MIScurrentShadowCounter) );

			AtNode* previous = NULL;
			int sampleNum = 0;



			while(AiLightsGetSample(sg))
			{
				// Get the spherical angles of the Light direction relative to the hair fibre
				AtVector wi = sg->Ld;
				float theta_i, phi_i;
				cartesian2sphericalHair(theta_i, phi_i, wi, U,V,W);

				if(sg->Lp == previous) {
					++sampleNum;
				}else{
					previous = sg->Lp;
					sampleNum = 0;
				}


				// DUAL SCATTERING ( Forward)
				//-----------------
				// DualScattering Tf

				float numGMS_f = 0.0f;

				AtColor forwardScale = get_scale_forward()*get_color_forward();
				bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);

				AtColor backScale = get_scale_back()*get_color_back();
				bool backScaleIsSmall = AiColorIsSmall(backScale);

				AtUInt32 nIntersections = 0; 

				// Count was done during AiLightsPrepare ?
				AtUInt32* counterArray;
				char lightName[96];
#ifdef _WIN32
			sprintf_s(lightName, "ObqH_%p_%u_%u", sg->Lp, sg->transp_index, sg->Rr); // safer to use snprintf...
#else
			sprintf(lightName, "ObqH_%p_%u_%u", sg->Lp, sg->transp_index, sg->Rr); // safer to use snprintf...
#endif

				

				if(AiStateGetMsgPtr(lightName,(void**)(&counterArray)))
				{
					nIntersections = counterArray[sampleNum];

					// if last sample, free memory of array
					if(sampleNum == sg->n - 1)
						delete[] counterArray;
				}

				numGMS_f = std::pow(static_cast<float>(nIntersections),get_numGMS_modifier());

				float theta_h = (theta_r+theta_i)/2.0f;
				float theta_d = (theta_r-theta_i)/2.0f;

				float cosTheta = std::abs(std::cos(theta_i));
				AtColor lightC = sg->Li*sg->we*cosTheta;
				
				AtColor selfOpacity = (m_unlinkOpacity?m_shadow_opacity:m_opacity);
				AtColor directFraction = (numGMS_f==0.0 || selfOpacity == AI_RGB_BLACK ?AI_RGB_WHITE:AI_RGB_WHITE-colorChannelPow(AiColorClamp(selfOpacity,0.0f,1.0f),numGMS_f));

				bool direcFractionIsSmall = AiColorIsSmall(directFraction);

				AtColor forward_fraction = AI_RGB_WHITE - directFraction;

				AtColor loopThis_f_back_scatter = AI_RGB_BLACK,
					loopThis_f_scatter = AI_RGB_BLACK,
					loopThis_f_back_direct = AI_RGB_BLACK,
					loopThis_f_direct_cR = AI_RGB_BLACK,
					loopThis_f_direct_cTT = AI_RGB_BLACK,
					loopThis_f_direct_cTRT = AI_RGB_BLACK,
					loopThis_f_direct_cg = AI_RGB_BLACK;

				// At least forward scattering once -> Scatter
				if(!AiColorIsSmall(forward_fraction))
				{
					AtColor sigma_bar_f_2 = dHairData.get_beta_bar_f_2(theta_i)*numGMS_f;
					AtColor a_bar_f_color = dHairData.get_a_bar_f(theta_i);
					AtColor T_f = { m_d_f*std::pow(a_bar_f_color.r,numGMS_f),	m_d_f*std::pow(a_bar_f_color.g,numGMS_f),	m_d_f*std::pow(a_bar_f_color.b,numGMS_f)};
					forward_fraction = colorChannelMax(T_f-directFraction,0.0f);
					AtColor f_s_scatter = S_G(theta_i,phi_i,theta_r,phi_r,dHairData.get_sigma_bar_b_2(theta_i),dHairData);
					f_s_scatter *= forwardScale;

					if(!backScaleIsSmall)
					{
						AtColor f_back_scatter = 2.0f*dHairData.get_A_bar_b(theta_i)*gN(colorChannelMax(dHairData.get_sigma_bar_b_2(theta_i) + sigma_bar_f_2 + get_beta_back(),AI_EPSILON) ,dHairData.get_Delta_bar_b(theta_i),theta_h+get_alpha_back())/(c_Pi*std::cos(theta_d)*std::cos(theta_d));
						f_back_scatter *= backScale;
						loopThis_f_back_scatter = forward_fraction*m_d_f*c_Pi*m_d_b*f_back_scatter;//forward_fraction*lightC*m_d_f*c_Pi*m_d_b*f_back_scatter;
					}
					loopThis_f_scatter = forward_fraction*m_d_f*f_s_scatter;//forward_fraction*lightC*m_d_f*f_s_scatter;
				}

				//else // -> direct
				if(!direcFractionIsSmall)
				{

					AtColor cR = AI_RGB_BLACK, cTT = AI_RGB_BLACK, cTRT = AI_RGB_BLACK, cg = AI_RGB_BLACK;

					S_scaled(cR, cTT, cTRT, cg, theta_i,phi_i,theta_r,phi_r);

					loopThis_f_direct_cR		=	directFraction*cR;	//directFraction*lightC*cR;
					loopThis_f_direct_cTT		=	directFraction*cTT;	//directFraction*lightC*cTT;
					loopThis_f_direct_cTRT		=	directFraction*cTRT;//directFraction*lightC*cTRT;
					loopThis_f_direct_cg		=	directFraction*cg;	//directFraction*lightC*cg;


					if(!backScaleIsSmall)
					{
						AtColor f_back_direct = 2.0f*dHairData.get_A_bar_b(theta_i) * gN(colorChannelMax(dHairData.get_sigma_bar_b_2(theta_i)+get_beta_back(),AI_EPSILON),dHairData.get_Delta_bar_b(theta_i),theta_h+get_alpha_back())/(c_Pi*std::cos(theta_i)*std::cos(theta_i));
						loopThis_f_back_direct	=	directFraction*m_d_b*f_back_direct*backScale;//directFraction*lightC*m_d_b*f_back_direct*backScale;
					}
				}

				////////////////////////////////////////////////////////////////
				/// MIS SWITCH
				///////////////////////////////////////////////////////////////
				switch(m_modeMIS)
				{
				case 0 :
					loop_f_back_scatter		+=  loopThis_f_back_scatter*lightC;
					loop_f_scatter			+=  loopThis_f_scatter*lightC;
					loop_f_back_direct		+=  loopThis_f_back_direct*lightC;
					loop_f_direct_cR		+=	loopThis_f_direct_cR*lightC;
					loop_f_direct_cTT		+=	loopThis_f_direct_cTT*lightC;
					loop_f_direct_cTRT		+=	loopThis_f_direct_cTRT*lightC;
					loop_f_direct_cg		+=	loopThis_f_direct_cg*lightC;
					break;
				case 1:
					{

						if(m_modeMIS_DS == 0)
						{

							setMISspecialInfo((loopThis_f_direct_cR + loopThis_f_direct_cTT + loopThis_f_direct_cTRT + loopThis_f_direct_cg)*cosTheta,compute_pdf(theta_i,phi_i,m_theta_r, m_phi_r), nIntersections);

							AtColor ccR = AI_RGB_BLACK;
							
							for(int i = 0; i < m_MISrandomLobeSamples; i++)
							{
								setMIScurrentInfo(0,i!=0); // has to be true
								ccR	+=	AiEvaluateLightSample(sg, this, ObqHairDualScatEvalSample, ObqHairDualScatEvalBrdf_separateDS, ObqHairDualScatEvalPdf);
							}

							loop_f_direct_cR += ccR/static_cast<float>(m_MISrandomLobeSamples);

							loop_f_back_scatter		+= loopThis_f_back_scatter*lightC;
							loop_f_scatter			+= loopThis_f_scatter*lightC;
							loop_f_back_direct		+= loopThis_f_back_direct*lightC;

						}
						else if(m_samples_DS == 0 )
						{
							
							setMISspecialInfo((loopThis_f_back_scatter + loopThis_f_scatter + loopThis_f_back_direct + loopThis_f_direct_cR + loopThis_f_direct_cTT + loopThis_f_direct_cTRT + loopThis_f_direct_cg)*cosTheta,compute_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);
							
							AtColor ccR = AI_RGB_BLACK;
							
							for(int i = 0; i < m_MISrandomLobeSamples; i++)
							{
								setMIScurrentInfo(0,i!=0); // has to be true
								ccR	+=	AiEvaluateLightSample(sg, this, ObqHairDualScatEvalSample, ObqHairDualScatEvalBrdf, ObqHairDualScatEvalPdf);
							}

							loop_f_direct_cR += ccR/static_cast<float>(m_MISrandomLobeSamples);
						}
						else
						{
							// non dual
							setMISspecialInfo((loopThis_f_direct_cR + loopThis_f_direct_cTT + loopThis_f_direct_cTRT + loopThis_f_direct_cg)*cosTheta,compute_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);

							AtColor ccR = AI_RGB_BLACK;
							for(int i = 0; i < m_MISrandomLobeSamples; i++)
							{
								setMIScurrentInfo(0,i!=0); // has to be true
								ccR	+=	AiEvaluateLightSample(sg, this, ObqHairDualScatEvalSample, ObqHairDualScatEvalBrdf_separateDS, ObqHairDualScatEvalPdf);
							}

							loop_f_direct_cR += ccR/static_cast<float>(m_MISrandomLobeSamples);

							// dual
							setMISspecialInfo((loopThis_f_back_scatter+loopThis_f_scatter+loopThis_f_back_direct)*cosTheta,c_1Over4Pi,0); // no need for intersection because it won't be used

							// MIS DS
							AtColor cScatter = AI_RGB_BLACK;
							for(int i = 0; i < m_samples_DS; i++)
							{
								setMIScurrentInfo(0,i!=0);
								cScatter	+= AiEvaluateLightSample(sg, this, ObqHairDualScatEvalSample_DS, ObqHairDualScatEvalBrdf_DS, ObqHairDualScatEvalPdf_DS);
							}

							loop_f_scatter += cScatter/static_cast<float>(m_samples_DS);

						}
					}
					break;
				case 2:
					{

						// ina any case, Lobes are calculated seperately

						float sampling[4] = {0.0f,0.0f,0.0f,0.0f}; // the four lobes
						AtColor color[4] = {AI_RGB_BLACK,AI_RGB_BLACK,AI_RGB_BLACK,AI_RGB_BLACK};

						for(int i = 0; i < m_MISrandomLobeSamples; i++)
						{
							AtByte lobe = getRandomLobeE();
							sampling[lobe]+=1.0f;
							switch(lobe)
							{
							case OBQ_LOBE_R :
								setMISspecialInfo(loopThis_f_direct_cR*cosTheta,compute_R_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);
								break;
							case OBQ_LOBE_TT:
								setMISspecialInfo(loopThis_f_direct_cTT*cosTheta,compute_TT_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);
								break;
							case OBQ_LOBE_TRT:
								setMISspecialInfo(loopThis_f_direct_cTRT*cosTheta,compute_TRT_G_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);
								break;
							case OBQ_LOBE_G:
								setMISspecialInfo(loopThis_f_direct_cg*cosTheta,compute_G_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);
								break;
							default:
								setMISspecialInfo(AI_RGB_BLACK,0.0f,nIntersections);
								break;
							}

							setMIScurrentInfo(lobe,i!=0);
							color[lobe] += AiEvaluateLightSample(sg, this, ObqHairDualScatEvalSample_separateLobes, ObqHairDualScatEvalBrdf_separateDS_separateLobes, ObqHairDualScatEvalPdf_separateLobes);
						}

						loop_f_direct_cR		+=	(sampling[OBQ_LOBE_R]	!=0.0f?	color[OBQ_LOBE_R]/sampling[OBQ_LOBE_R]		:lightC*loopThis_f_direct_cR);
						loop_f_direct_cTT		+=	(sampling[OBQ_LOBE_TT]	!=0.0f?	color[OBQ_LOBE_TT]/sampling[OBQ_LOBE_TT]	:lightC*loopThis_f_direct_cTT);
						loop_f_direct_cTRT		+=	(sampling[OBQ_LOBE_TRT]	!=0.0f?	color[OBQ_LOBE_TRT]/sampling[OBQ_LOBE_TRT]	:lightC*loopThis_f_direct_cTRT);
						loop_f_direct_cg		+=	(sampling[OBQ_LOBE_G]	!=0.0f?	color[OBQ_LOBE_G]/sampling[OBQ_LOBE_G]		:lightC*loopThis_f_direct_cg);


						if(m_modeMIS_DS == 0 || m_samples_DS == 0)
						{
							// Separate non MIS DS
							loop_f_back_scatter		+= loopThis_f_back_scatter*lightC;
							loop_f_scatter			+= loopThis_f_scatter*lightC;
							loop_f_back_direct		+= loopThis_f_back_direct*lightC;

						}
						else
						{

							setMISspecialInfo((loopThis_f_back_scatter+loopThis_f_scatter+loopThis_f_back_direct)*cosTheta,c_1Over4Pi,nIntersections);

							// MIS DS
							AtColor cScatter = AI_RGB_BLACK;
							for(int i = 0; i < m_samples_DS; i++)
							{
								setMIScurrentInfo(0,i!=0);
								cScatter	+= AiEvaluateLightSample(sg, this, ObqHairDualScatEvalSample_DS, ObqHairDualScatEvalBrdf_DS, ObqHairDualScatEvalPdf_DS);
							}

							loop_f_scatter += cScatter/static_cast<float>(m_samples_DS);

						}
						break;
					}
				case 3:
					{
						if(m_samples_R!=0)
						{

							setMISspecialInfo(loopThis_f_direct_cR*cosTheta,compute_R_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);

							for(int i = 0; i < m_samples_R; i++)
							{
								setMIScurrentInfo(OBQ_LOBE_R,i!=0);
								loop_f_direct_cR		+= AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairDualScatEvalBrdf_separateDS_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
							}
						}
						else
							loop_f_direct_cR += loopThis_f_direct_cR*lightC;

						if(m_samples_TT!=0)
						{
							setMISspecialInfo(loopThis_f_direct_cTT*cosTheta,compute_TT_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);
							for(int i = 0; i < m_samples_TT; i++)
							{
								setMIScurrentInfo(OBQ_LOBE_TT,i!=0);
								loop_f_direct_cTT		+= AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairDualScatEvalBrdf_separateDS_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
							}
						}
						else
							loop_f_direct_cTT	+= lightC*loopThis_f_direct_cTT;


						if(m_samples_TRT!=0)
						{
							setMISspecialInfo(loopThis_f_direct_cTRT*cosTheta,compute_TRT_G_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);
							for(int i = 0; i < m_samples_TRT; i++)
							{
								setMIScurrentInfo(OBQ_LOBE_TRT,i!=0);
								loop_f_direct_cTRT		+= AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairDualScatEvalBrdf_separateDS_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
							}
						}
						else
							loop_f_direct_cTRT	+= lightC*loopThis_f_direct_cTRT;

						if(m_samples_G!=0)
						{
							setMISspecialInfo(loopThis_f_direct_cg*cosTheta,compute_G_pdf(theta_i,phi_i,m_theta_r, m_phi_r),nIntersections);
							for(int i = 0; i < m_samples_G; i++)
							{
								setMIScurrentInfo(OBQ_LOBE_G,i!=0);
								loop_f_direct_cg		+= AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairDualScatEvalBrdf_separateDS_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
							}
						}
						else
							loop_f_direct_cg	+= lightC*loopThis_f_direct_cg;
	

						if(m_modeMIS_DS == 0 || m_samples_DS==0)
						{
							loop_f_back_scatter		+= loopThis_f_back_scatter*lightC;
							loop_f_scatter			+= loopThis_f_scatter*lightC;
							loop_f_back_direct		+= loopThis_f_back_direct*lightC;
						}
						else
						{
							setMISspecialInfo((loopThis_f_back_scatter + loopThis_f_scatter + loopThis_f_back_direct)*cosTheta,c_1Over4Pi,nIntersections);

							for(int i = 0; i < m_samples_DS; i++)
							{
								setMIScurrentInfo(OBQ_LOBE_S,i!=0);
								loop_f_scatter		+= AiEvaluateLightSample(sg, this, ObqHairDualScatEvalSample_DS, ObqHairDualScatEvalBrdf_DS, ObqHairDualScatEvalPdf_DS);
							}

						}
						break;
					}
				default:
					loop_f_back_scatter		+= loopThis_f_back_scatter*lightC;
					loop_f_scatter			+= loopThis_f_scatter*lightC;
					loop_f_back_direct		+= loopThis_f_back_direct*lightC;
					loop_f_direct_cR		+=	loopThis_f_direct_cR*lightC;
					loop_f_direct_cTT		+=	loopThis_f_direct_cTT*lightC;
					loop_f_direct_cTRT		+=	loopThis_f_direct_cTRT*lightC;
					loop_f_direct_cg		+=	loopThis_f_direct_cg*lightC;
					break;
				}
			}

			// DIVISION for sampling parameter
			if(m_modeMIS == 3)
			{
				if(m_samples_R>1)
					loop_f_direct_cR/=static_cast<float>(m_samples_R);
				if(m_samples_TT>1)
					loop_f_direct_cTT/=static_cast<float>(m_samples_TT);
				if(m_samples_TRT>1)
					loop_f_direct_cTRT/=static_cast<float>(m_samples_TRT);
				if(m_samples_G>1)
					loop_f_direct_cg/=static_cast<float>(m_samples_G);

				if(m_modeMIS_DS == 1 && m_samples_DS>1)
					loop_f_scatter/=static_cast<float>(m_samples_DS);
			}

			resetMIScurrentInfo();
			sg->fhemi = hemi; // put hemispherical thigny back
			AiStateSetMsgInt(lightNameMode, 0);

			loop_f_direct_cR		*=directScale;
			loop_f_direct_cTT		*=directScale;
			loop_f_direct_cTRT		*=directScale;
			loop_f_direct_cg		*=directScale;

			loop_f_back_direct	*=	directScale;
			loop_f_scatter		*=	directScale;
			loop_f_back_scatter	*=	directScale;
		}

	}




	
	///////////////////////////////////////
	// Shading functions : DIRECT NO SCAT
	///////////////////////////////////////
	inline void direct_shading_NoScat(AtColor& loop_f_direct_R, AtColor& loop_f_direct_TT, AtColor& loop_f_direct_TRT, AtColor& loop_f_direct_g, AtShaderGlobals* sg)
	{
		AtColor scale = getDirectScale();
		if(!AiColorIsSmall( scale ))
		{
			bool hemi = sg->fhemi;
			sg->fhemi = false;

			// Prepare lights and send self shadow rays not counting
			AiLightsPrepare(sg);

			//////////////
			// MIS MODES
			//////////////
			switch(m_modeMIS)
			{
			case 0: // NO_MIS
				while(AiLightsGetSample(sg))
				{

					// Get the spherical angles of the Light direction relative to the hair fibre
					AtVector wi = sg->Ld;
					float theta_i, phi_i;
					cartesian2sphericalHair(theta_i, phi_i, wi, m_U,m_V,m_W);

					AtColor lightC = sg->Li*sg->we*std::abs(std::cos(theta_i));

					AtColor cR = AI_RGB_BLACK, cTT = AI_RGB_BLACK, cTRT = AI_RGB_BLACK, cg = AI_RGB_BLACK;
					S_scaled(cR,cTT, cTRT, cg, theta_i,phi_i, m_theta_r, m_phi_r);

					loop_f_direct_R		+=	lightC*cR;
					loop_f_direct_TT	+=	lightC*cTT;
					loop_f_direct_TRT	+=	lightC*cTRT;
					loop_f_direct_g		+=	lightC*cg;

				}
				break;
			case 1: // ALL N times
				{
					while(AiLightsGetSample(sg))
					{

						AtVector wi = sg->Ld;
						float theta_i, phi_i;
						cartesian2sphericalHair(theta_i, phi_i, wi, m_U,m_V,m_W);

						//AtColor lightC = sg->Li*sg->we*std::abs(std::cos(theta_i));

						AtColor c = S_scaled(theta_i,phi_i, m_theta_r, m_phi_r);
						
						setMISspecialInfo(c*std::abs(std::cos(theta_i)),compute_pdf(theta_i,phi_i,m_theta_r, m_phi_r),0);

						for(int i = 0; i < m_MISrandomLobeSamples; i++)
						{
							setMIScurrentInfo(0,i!=0);
							loop_f_direct_R	+=	AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample, ObqHairSingleScatEvalBrdf, ObqHairSingleScatEvalPdf);
						}
					}
					loop_f_direct_R/=static_cast<float>(m_MISrandomLobeSamples);
				}
				break;
			case 2: // N times RANDOM   (faster if we chose the lobes at the beginning, but... should be tested)
				while(AiLightsGetSample(sg))
				{
					// Calculate the non mis part
					AtVector wi = sg->Ld;
					float theta_i, phi_i;
					cartesian2sphericalHair(theta_i, phi_i, wi, m_U,m_V,m_W);

					float cosTheta = std::abs(std::cos(theta_i));
					AtColor lightC = sg->Li*sg->we*cosTheta;

					AtColor cR, cTT, cTRT, cg;
					S_scaled(cR,cTT, cTRT, cg, theta_i,phi_i, m_theta_r, m_phi_r);

					float sampling[4] = {0.0f,0.0f,0.0f,0.0f}; // the four lobes
					AtColor color[4] = {AI_RGB_BLACK,AI_RGB_BLACK,AI_RGB_BLACK,AI_RGB_BLACK};

					for(int i = 0; i < m_MISrandomLobeSamples; i++)
					{
						AtByte lobe = getRandomLobeE();
						sampling[lobe]+=1.0f;
						switch(lobe)
						{
						case OBQ_LOBE_R :
							setMISspecialInfo(cR*cosTheta,compute_R_pdf(theta_i,phi_i,m_theta_r, m_phi_r),0);
							break;
						case OBQ_LOBE_TT:
							setMISspecialInfo(cTT*cosTheta,compute_TT_pdf(theta_i,phi_i,m_theta_r, m_phi_r),0);
							break;
						case OBQ_LOBE_TRT:
							setMISspecialInfo(cTRT*cosTheta,compute_TRT_G_pdf(theta_i,phi_i,m_theta_r, m_phi_r),0);
							break;
						case OBQ_LOBE_G:
							setMISspecialInfo(cg*cosTheta,compute_G_pdf(theta_i,phi_i,m_theta_r, m_phi_r),0);
							break;
						default:
							setMISspecialInfo(AI_RGB_BLACK,0.0f,0);
							break;
						}

						setMIScurrentInfo(lobe,i!=0);
						color[lobe] += AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairSingleScatEvalBrdf_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
					}


					loop_f_direct_R		+=	(sampling[OBQ_LOBE_R]!=0.0f?color[OBQ_LOBE_R]/sampling[OBQ_LOBE_R]:lightC*cR);
					loop_f_direct_TT	+=	(sampling[OBQ_LOBE_TT]!=0.0f?color[OBQ_LOBE_TT]/sampling[OBQ_LOBE_TT]:lightC*cTT);
					loop_f_direct_TRT	+=	(sampling[OBQ_LOBE_TRT]!=0.0f?color[OBQ_LOBE_TRT]/sampling[OBQ_LOBE_TRT]:lightC*cTRT);
					loop_f_direct_g		+=	(sampling[OBQ_LOBE_G]!=0.0f?color[OBQ_LOBE_G]/sampling[OBQ_LOBE_G]:lightC*cg);
				}
				break;
			case 3:
				{
					bool sampleR = m_samples_R!=0, 
						sampleTT = m_samples_TT!=0,
						sampleTRT = m_samples_TRT!=0,
						sampleG = m_samples_G!=0;

					while(AiLightsGetSample(sg))
					{
						// Calculate the non mis part
						AtColor cR, cTT, cTRT, cg;

						AtVector wi = sg->Ld;
						float theta_i, phi_i;
						cartesian2sphericalHair(theta_i, phi_i, wi, m_U,m_V,m_W);
						float cosTheta = std::abs(std::cos(theta_i));
						AtColor lightC = sg->Li*sg->we*cosTheta;

						S_scaled(cR,cTT, cTRT, cg, theta_i,phi_i, m_theta_r, m_phi_r);

						if(sampleR)
						{
							setMISspecialInfo(cR*cosTheta,compute_R_pdf(theta_i,phi_i,m_theta_r,m_phi_r),0);
							for(int i = 0; i < m_samples_R; i++)
							{
								
								setMIScurrentInfo(OBQ_LOBE_R,i!=0);
								loop_f_direct_R		+= AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairSingleScatEvalBrdf_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
							}
						}
						else
							loop_f_direct_R		+= lightC*cR;

						if(sampleTT)
						{
							setMISspecialInfo(cTT*cosTheta,compute_TT_pdf(theta_i,phi_i,m_theta_r,m_phi_r),0);

							for(int i = 0; i < m_samples_TT; i++)
							{
								setMIScurrentInfo(OBQ_LOBE_TT,i!=0);
								loop_f_direct_TT		+= AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairSingleScatEvalBrdf_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
							}
						}
						else
							loop_f_direct_TT	+= lightC*cTT;

						if(sampleTRT)
						{
							setMISspecialInfo(cTRT*cosTheta,compute_TRT_G_pdf(theta_i,phi_i,m_theta_r,m_phi_r),0);

							for(int i = 0; i < m_samples_TRT; i++)
							{
								setMIScurrentInfo(OBQ_LOBE_TRT,i!=0);
								loop_f_direct_TRT		+= AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairSingleScatEvalBrdf_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
							}
						}
						else
							loop_f_direct_TRT	+= lightC*cTRT;

						if(sampleG)
							{
							setMISspecialInfo(cg*cosTheta,compute_G_pdf(theta_i,phi_i,m_theta_r,m_phi_r),0);

							for(int i = 0; i < m_samples_G; i++)
							{
								setMIScurrentInfo(OBQ_LOBE_G,i!=0);
								loop_f_direct_g		+= AiEvaluateLightSample(sg, this, ObqHairSingleScatEvalSample_separateLobes, ObqHairSingleScatEvalBrdf_separateLobes, ObqHairSingleScatEvalPdf_separateLobes);
							}
						}
						else
							loop_f_direct_g	+= lightC*cg;
					}

					loop_f_direct_R		/=	(m_samples_R>1?static_cast<float>(m_samples_R):1.0f);
					loop_f_direct_TT	/=	(m_samples_TT>1?static_cast<float>(m_samples_TT):1.0f);
					loop_f_direct_TRT	/=	(m_samples_TRT>1?static_cast<float>(m_samples_TRT):1.0f);
					loop_f_direct_g		/=	(m_samples_G>1?static_cast<float>(m_samples_G):1.0f);
				}
				break;
			default:
				AiMsgError("Bad MIS mode");
				break;
			}

			resetMIScurrentInfo();
			sg->fhemi = hemi; // put hemispherical thigny back
		}

		loop_f_direct_R*=scale;
		loop_f_direct_TT*=scale;
		loop_f_direct_TRT*=scale;
		loop_f_direct_g*=scale;
	}

	//-------------------
	// INDIRECT LIGHTING 
	//-------------------
	inline void indirect_shading_Full_uni(AtColor& loop_Indirect_f_scatter, AtColor& loop_Indirect_f_back_scatter, AtColor& loop_Indirect_f_direct_cR, AtColor& loop_Indirect_f_direct_cTT,AtColor& loop_Indirect_f_direct_cTRT,AtColor& loop_Indirect_f_direct_cg, AtColor& loop_Indirect_f_back_direct, AtShaderGlobals* sg, ObqHair& dHairData, AtSampler* sampler, /*AtCritSec* criticalSec,*/ int samples, AtVector& U, AtVector& V, AtVector& W, float theta_r, float phi_r)
	{

		AtColor indirectScale = getIndirectScale();

		//for a certain number of indirect samples {u,v} determine the direction with the pdf
		if(samples > 0 && !AiColorIsSmall(getIndirectScale()))
		{

			char sampleNameMode[64];
#ifdef _WIN32
			sprintf_s(sampleNameMode, "ObqH_GSM_%u_%u",sg->transp_index, sg->Rr);
#else
			sprintf(sampleNameMode, "ObqH_GSM_%u_%u",sg->transp_index, sg->Rr);
#endif
			
			// Glossy scattering mode

			// TEST to be sure there are no consflict here:
			bool test = false;
			bool alreadyThere = false;

			//if(m_useCritical)	AiCritSecEnter(criticalSec);

			alreadyThere = AiStateGetMsgBool(sampleNameMode, &test);

			if(!alreadyThere)	AiStateSetMsgBool(sampleNameMode, true);

			//if(m_useCritical)	AiCritSecLeave(criticalSec);

			// if theres
			if(alreadyThere)
			{
				indirect_shading_NoScat_uni(loop_Indirect_f_direct_cR,loop_Indirect_f_direct_cTT,loop_Indirect_f_direct_cTRT, loop_Indirect_f_direct_cg,sg, sampler, samples,U,V,W,theta_r, phi_r);
				return;
			}

			AtSamplerIterator* iter = AiSamplerIterator(sampler, sg);
#if OBQ_AI_VERSION >= 40100
			float rndTmp[2];
#else
			double rndTmp[2];
#endif
			
			
			float rnd[2];
			int badPdf=0;

			while(AiSamplerGetSample(iter, rndTmp)) 
			{
				rnd[0] = static_cast<float>(rndTmp[0]);
				rnd[1] = static_cast<float>(rndTmp[1]);

				AtVector dir;
				float pdf = 1.0f;

				if(m_useImportanceSampling)
				{
					float theta = 0.0f, phi = 0.0f;
					pdf = std::max(sample_brdf(rnd,theta,phi,theta_r,phi_r),0.0f);

					dir = sphericalHair2cartesian(theta,phi,U,V,W);
				}
				else
				{
					pdf = c_1Over4Pi; // uniform
					uniformPointSphere(dir,rnd);
				}

				if (pdf > AI_EPSILON)
				{	

					AtScrSample sample_diffuse;
					AtRay ray;
					AiMakeRay(&ray, AI_RAY_GLOSSY, &sg->P, &dir, AI_BIG, sg);


					// initialise message passing
					AtUInt32 numGMS = 0;																// initialize num events
					float numGMS_f = 0.0f;
					bool bg = false;

					AtColor forwardScale = get_scale_forward()*get_color_forward();
					bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);
					AtColor backScale = get_scale_back()*get_color_back();
					bool backScaleIsSmall = AiColorIsSmall(backScale);


					char sampleNameCount[64];
#ifdef _WIN32
					sprintf_s(sampleNameCount, "ObqH_GSN_%u_%u",sg->transp_index, sg->Rr);
#else
					sprintf(sampleNameCount, "ObqH_GSN_%u_%u",sg->transp_index, sg->Rr);
#endif
		
					AiStateSetMsgUInt(sampleNameCount, 0);							// and count # of intersection

					if(!AiTrace(&ray, &sample_diffuse))
					{
						AiTraceBackground(&ray, &sample_diffuse);
						bg=true;
					}

					// trivial sample reject
					if(!AiColorIsSmall(sample_diffuse.color))
					{

						double distance = sample_diffuse.z;

						//sample.color // couleur comme la lumière
						float theta_i, phi_i;
						cartesian2sphericalHair(theta_i, phi_i, dir, U,V,W);

						AtColor lightC = sample_diffuse.color*std::abs(std::cos(theta_i))/pdf;

						if(AiColorCorrupted(lightC))
							lightC = AI_RGB_BLACK;

						AtColor forwardScale = get_scale_forward()*get_color_forward();
						bool forwardScaleIsSmall = AiColorIsSmall(forwardScale);

						AtColor backScale = get_scale_back()*get_color_back();
						bool backScaleIsSmall = AiColorIsSmall(backScale);

						AiStateGetMsgUInt(sampleNameCount, &numGMS);											// Get num events
						numGMS_f = std::pow(static_cast<float>(numGMS),get_numGMS_modifier());			// Modified number of intersection

						float theta_h = (theta_r+theta_i)/2.0f;
						float theta_d = (theta_r-theta_i)/2.0f;

						AtColor selfOpacity = (m_unlinkOpacity?m_shadow_opacity:m_opacity);
						AtColor directFraction = (numGMS_f==0.0 || selfOpacity == AI_RGB_BLACK ?AI_RGB_WHITE:AI_RGB_WHITE-colorChannelPow(AiColorClamp(selfOpacity,0.0f,1.0f),numGMS_f));

						bool direcFractionIsSmall = AiColorIsSmall(directFraction);

						AtColor forward_fraction = AI_RGB_WHITE - directFraction;

						// At least forward scattering once -> Scatter
						if(!AiColorIsSmall(forward_fraction))
						{

							AtColor sigma_bar_f_2 = dHairData.get_beta_bar_f_2(theta_i)*numGMS_f;

							AtColor a_bar_f_color = dHairData.get_a_bar_f(theta_i);

							AtColor T_f = { m_d_f*std::pow(a_bar_f_color.r,numGMS_f),	m_d_f*std::pow(a_bar_f_color.g,numGMS_f),	m_d_f*std::pow(a_bar_f_color.b,numGMS_f)};

							forward_fraction = colorChannelMax(T_f-directFraction,0.0f);

							AtColor f_s_scatter = S_G(theta_i,phi_i,theta_r,phi_r,dHairData.get_sigma_bar_b_2(theta_i),dHairData);

							f_s_scatter *= forwardScale;

							if(!backScaleIsSmall)
							{
								AtColor f_back_scatter = 2.0f*dHairData.get_A_bar_b(theta_i)*gN(colorChannelMax(dHairData.get_sigma_bar_b_2(theta_i) + sigma_bar_f_2 + get_beta_back(),AI_EPSILON) ,dHairData.get_Delta_bar_b(theta_i),theta_h+get_alpha_back())/(c_Pi*std::cos(theta_d)*std::cos(theta_d));
								f_back_scatter *= backScale;

								loop_Indirect_f_back_scatter += forward_fraction*lightC*m_d_f*c_Pi*m_d_b*f_back_scatter;
							}

							loop_Indirect_f_scatter += forward_fraction*lightC*m_d_f*f_s_scatter;

						}

						//else // -> direct
						if(!direcFractionIsSmall)
						{
							AtColor cR = AI_RGB_BLACK, cTT = AI_RGB_BLACK, cTRT = AI_RGB_BLACK, cg = AI_RGB_BLACK;

							S_scaled(cR, cTT, cTRT, cg, theta_i,phi_i,theta_r,phi_r);

							loop_Indirect_f_direct_cR		+=	directFraction*lightC*cR;
							loop_Indirect_f_direct_cTT		+=	directFraction*lightC*cTT;
							loop_Indirect_f_direct_cTRT		+=	directFraction*lightC*cTRT;
							loop_Indirect_f_direct_cg		+=	directFraction*lightC*cg;

							if(!backScaleIsSmall)
							{
								AtColor f_back_direct = 2.0f*dHairData.get_A_bar_b(theta_i) * gN(colorChannelMax(dHairData.get_sigma_bar_b_2(theta_i)+get_beta_back(),AI_EPSILON),dHairData.get_Delta_bar_b(theta_i),theta_h+get_alpha_back())/(c_Pi*std::cos(theta_i)*std::cos(theta_i));
								loop_Indirect_f_back_direct	+=	directFraction*lightC*m_d_b*f_back_direct*backScale;
							}

						}
					}
					AiStateSetMsgUInt(sampleNameCount, 0);							// reset counter
				}
				else
				{
					++badPdf;
				}
			}
			AiStateSetMsgBool(sampleNameMode, false);
			// Integration over the full sphere
			//indirectScale*= AiSamplerGetSampleInvCount(iter)*c_4Pi;
			indirectScale*= (1.0f/(static_cast<float>(std::max(1,AiSamplerGetSampleCount(iter)-badPdf))));
			//indirectScale*= AiSamplerGetSampleInvCount(iter);

			loop_Indirect_f_scatter *= indirectScale;
			loop_Indirect_f_back_scatter*= indirectScale;

			loop_Indirect_f_direct_cR*= indirectScale;
			loop_Indirect_f_direct_cTT*= indirectScale;
			loop_Indirect_f_direct_cTRT*= indirectScale;
			loop_Indirect_f_direct_cg*= indirectScale;
			loop_Indirect_f_back_direct*= indirectScale;

		}
	}




	//-------------------
	// INDIRECT LIGHTING NoSCATTERING
	//-------------------
	inline void indirect_shading_NoScat_uni(AtColor& loop_Indirect_f_direct_R, AtColor& loop_Indirect_f_direct_TT, AtColor& loop_Indirect_f_direct_TRT, AtColor& loop_Indirect_f_direct_g, AtShaderGlobals* sg, AtSampler* sampler, int samples, AtVector& U, AtVector& V, AtVector& W, float theta_r, float phi_r)
	{

		//for a certain number of indirect samples {u,v} determine the direction with the pdf
		if(samples > 0 && !AiColorIsSmall(getIndirectScale()))
		{
			AtSamplerIterator* iter = AiSamplerIterator(sampler, sg);
			
#if OBQ_AI_VERSION >= 40100
			float rndTmp[2];
#else
			double rndTmp[2];
#endif
			float rnd[2];
			int badPdf=0;

			while(AiSamplerGetSample(iter, rndTmp)) 
			{
				rnd[0] = static_cast<float>(rndTmp[0]);
				rnd[1] = static_cast<float>(rndTmp[1]);

				AtVector dir;
				float pdf = 1.0f;

				if(m_useImportanceSampling)
				{
					float theta = 0.0f, phi = 0.0f;
					pdf = std::max(sample_brdf(rnd,theta,phi,theta_r,phi_r),0.0f);

					dir = sphericalHair2cartesian(theta,phi,U,V,W);
				}
				else
				{
					pdf = c_1Over4Pi; // uniform
					uniformPointSphere(dir,rnd);
				}

				if (pdf > AI_EPSILON)
				{	

					AtScrSample sample_diffuse;
					AtRay ray;

					AiMakeRay(&ray, AI_RAY_GLOSSY, &sg->P, &dir, AI_BIG, sg);

					if(!AiTrace(&ray, &sample_diffuse))
						AiTraceBackground(&ray, &sample_diffuse);

					// trivial sample reject
					if(!AiColorIsSmall(sample_diffuse.color))
					{

						double distance = sample_diffuse.z;

						//sample.color // couleur comme la lumière
						float theta_i, phi_i;
						cartesian2sphericalHair(theta_i, phi_i, dir, U,V,W);

						AtColor lightC = sample_diffuse.color*std::abs(std::cos(theta_i))/pdf;

						if(AiColorCorrupted(lightC))
							lightC = AI_RGB_BLACK;

						float theta_h = (theta_r+theta_i)/2.0f;
						float theta_d = (theta_r-theta_i)/2.0f;

						AtColor cR = AI_RGB_BLACK, cTT = AI_RGB_BLACK, cTRT = AI_RGB_BLACK, cg = AI_RGB_BLACK;

						S_scaled(cR, cTT, cTRT, cg, theta_i,phi_i,theta_r,phi_r);

						loop_Indirect_f_direct_R +=		lightC*cR;
						loop_Indirect_f_direct_TT +=	lightC*cTT;
						loop_Indirect_f_direct_TRT +=	lightC*cTRT;
						loop_Indirect_f_direct_g +=		lightC*cg;

					}
				}
				else
				{
					++badPdf;
				}
			}
			AtColor scale =  getIndirectScale()*(1.0f/(static_cast<float>(std::max(1,AiSamplerGetSampleCount(iter)-badPdf))));

			loop_Indirect_f_direct_R*= scale;
			loop_Indirect_f_direct_TT*= scale;
			loop_Indirect_f_direct_TRT*= scale;
			loop_Indirect_f_direct_g*= scale;

		}
	}



	// Simplified integration only on omega_o because hair is isotropic in phi_i
	inline void precalculations()
	{

		ObqHemisphere hemi;		// Solid angles info

		// resolution means how many values between 0 and PIover2.  [-PIover2,PIover2] has 2*res-1 values -> [-(res-1),(res-1)]
		int res = c_ObqHemisphereThetaRes;
		float resF = static_cast<float>(res);

		// theta_d  --  NOTE: This theta_d is the longitudinal inclination of the hair irt direction to the light 
		for(int theta_d = -(res-1); theta_d < res; theta_d++)
		{
			float omegaTotal = 0.0f;
			float theta_d_rad = static_cast<float>(theta_d)/static_cast<float>(res-1)*c_PiOver2;
			float cosThetaD = std::cos(theta_d_rad);

			// normalization
			float f_prime_r_full,f_prime_tt_full,f_prime_trt_full,f_prime_g_full;
			f_prime_s_full(f_prime_r_full,f_prime_tt_full,f_prime_trt_full,f_prime_g_full,theta_d_rad,hemi); 
			float f_prime_full = f_prime_r_full + f_prime_tt_full + f_prime_trt_full + f_prime_g_full;


			// initialize to black
			int index = theta_d+(res-1);

			m_a_bar_f[index] = AI_RGB_BLACK;
			m_a_bar_b[index] = AI_RGB_BLACK;

			m_beta_bar_f[index] = AI_RGB_BLACK;
			m_beta_bar_b[index] = AI_RGB_BLACK;

			AtColor alpha_bar_f_Over = AI_RGB_BLACK; 
			AtColor alpha_bar_f_Under = AI_RGB_BLACK;
			AtColor alpha_bar_b_Over = AI_RGB_BLACK; 
			AtColor alpha_bar_b_Under = AI_RGB_BLACK;

			AtColor beta_bar_f_Over = AI_RGB_BLACK; 
			AtColor beta_bar_f_Under = AI_RGB_BLACK;
			AtColor beta_bar_b_Over = AI_RGB_BLACK; 
			AtColor beta_bar_b_Under = AI_RGB_BLACK;

			m_A_bar_b[index] = AI_RGB_BLACK;

			m_Delta_bar_b[index] = AI_RGB_BLACK;

			m_sigma_bar_b[index] = AI_RGB_BLACK;

			m_N_G_R[index]	= 0.0f;
			m_N_G_TT[index]	= 0.0f;
			m_N_G_TRT[index]= 0.0f;
			m_N_G_g[index]	= 0.0f;

			// Phi_d is always 0, only phi_o changes here
			float phi_d_rad = 0.0f;

			// w_o, for all thetas and phis on the coresponding hemispheres
			for(int theta_o = -(res-1); theta_o < res; theta_o++)
			{
				// theta_o
				float theta_o_rad = static_cast<float>(theta_o)/(resF-1.0f)*c_PiOver2;

				if(std::abs(theta_o) == res-1) // Poles only have one phi
				{
					float omega_thetaPhi_o = hemi.getOmega(std::abs(theta_o));

					omegaTotal+=omega_thetaPhi_o;

					// TODO : Simplify because _f = _b here 
					float	f_prime_R_f, f_prime_TT_f, f_prime_TRT_f, f_prime_g_f,
						f_prime_R_b, f_prime_TT_b, f_prime_TRT_b, f_prime_g_b;
					f_prime_uncolored(f_prime_R_f, f_prime_TT_f, f_prime_TRT_f, f_prime_g_f,theta_d_rad, phi_d_rad,theta_o_rad,0.0f);
					f_prime_uncolored(f_prime_R_b, f_prime_TT_b, f_prime_TRT_b, f_prime_g_b,theta_d_rad, phi_d_rad,theta_o_rad,0.0f);

					AtColor under_f = f_prime_R_f*m_I_R + f_prime_TT_f*m_I_TT + f_prime_TRT_f*m_I_TRT + f_prime_g_f*m_I_g;
					AtColor under_b = f_prime_R_b*m_I_R + f_prime_TT_b*m_I_TT + f_prime_TRT_b*m_I_TRT + f_prime_g_b*m_I_g;

					m_a_bar_f[index] += cosThetaD*omega_thetaPhi_o*(f_prime_R_f/f_prime_full*m_I_R + f_prime_TT_f/f_prime_full*m_I_TT + f_prime_TRT_f/f_prime_full*m_I_TRT + f_prime_g_f/f_prime_full*m_I_g);	//m_a_bar_f[index] += cosThetaD*omega_thetaPhi_o*S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);
					m_a_bar_b[index] += cosThetaD*omega_thetaPhi_o*(f_prime_R_b/f_prime_full*m_I_R + f_prime_TT_b/f_prime_full*m_I_TT + f_prime_TRT_b/f_prime_full*m_I_TRT + f_prime_g_b/f_prime_full*m_I_g);	//m_a_bar_b[index] += cosThetaD*omega_thetaPhi_o*S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);

					alpha_bar_f_Over  += m_alpha_R*f_prime_R_f*m_I_R + m_alpha_TT*f_prime_TT_f*m_I_TT + m_alpha_TRT*(f_prime_TRT_f*m_I_TRT + f_prime_g_f*m_I_g);	//S_alpha_bar(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);
					alpha_bar_f_Under += under_f;																													//S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);

					alpha_bar_b_Over  += m_alpha_R*f_prime_R_b*m_I_R + m_alpha_TT*f_prime_TT_b*m_I_TT + m_alpha_TRT*(f_prime_TRT_b*m_I_TRT + f_prime_g_b*m_I_g);	//S_alpha_bar(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);
					alpha_bar_b_Under += under_b;																													//S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);

					beta_bar_f_Over  += m_beta_R*f_prime_R_f*m_I_R + m_beta_TT*f_prime_TT_f*m_I_TT + m_beta_TRT*(f_prime_TRT_f*m_I_TRT + f_prime_g_f*m_I_g);		//S_beta_bar(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);
					beta_bar_f_Under += under_f;																													//S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);

					beta_bar_b_Over  += m_beta_R*f_prime_R_b*m_I_R + m_beta_TT*f_prime_TT_b*m_I_TT + m_beta_TRT*(f_prime_TRT_b*m_I_TRT + f_prime_g_b*m_I_g);		//S_beta_bar(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);
					beta_bar_b_Under += under_b;																													//S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);


				}
				else // Not poles
				{
					for(int phi_o = -(res-1); phi_o < res;phi_o++)
					{
						float phi_o_rad = c_Pi + static_cast<float>(phi_o)/(resF-1.0f)*c_PiOver2; 

						float omega_thetaPhi_o = hemi.getOmega(std::abs(theta_o));
						if(std::abs(phi_o)==(res-1))
							omega_thetaPhi_o/=2.0f;

						omegaTotal+=omega_thetaPhi_o;

						// get scaled _X components
						float	f_prime_R_f, f_prime_TT_f, f_prime_TRT_f, f_prime_g_f,
							f_prime_R_b, f_prime_TT_b, f_prime_TRT_b, f_prime_g_b;
						f_prime_uncolored(f_prime_R_f, f_prime_TT_f, f_prime_TRT_f, f_prime_g_f,theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);
						f_prime_uncolored(f_prime_R_b, f_prime_TT_b, f_prime_TRT_b, f_prime_g_b,theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);
						AtColor under_f = f_prime_R_f*m_I_R + f_prime_TT_f*m_I_TT + f_prime_TRT_f*m_I_TRT + f_prime_g_f*m_I_g;
						AtColor under_b = f_prime_R_b*m_I_R + f_prime_TT_b*m_I_TT + f_prime_TRT_b*m_I_TRT + f_prime_g_b*m_I_g;

						m_a_bar_f[index] += cosThetaD*omega_thetaPhi_o*(f_prime_R_f/f_prime_full*m_I_R + f_prime_TT_f/f_prime_full*m_I_TT + f_prime_TRT_f/f_prime_full*m_I_TRT + f_prime_g_f/f_prime_full*m_I_g);	//m_a_bar_f[index] += cosThetaD*omega_thetaPhi_o*S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);
						m_a_bar_b[index] += cosThetaD*omega_thetaPhi_o*(f_prime_R_b/f_prime_full*m_I_R + f_prime_TT_b/f_prime_full*m_I_TT + f_prime_TRT_b/f_prime_full*m_I_TRT + f_prime_g_b/f_prime_full*m_I_g);	//m_a_bar_b[index] += cosThetaD*omega_thetaPhi_o*S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);

						alpha_bar_f_Over  += m_alpha_R*f_prime_R_f*m_I_R + m_alpha_TT*f_prime_TT_f*m_I_TT + m_alpha_TRT*(f_prime_TRT_f*m_I_TRT + f_prime_g_f*m_I_g);	//S_alpha_bar(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);
						alpha_bar_f_Under += under_f;																													//S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);

						alpha_bar_b_Over  += m_alpha_R*f_prime_R_b*m_I_R + m_alpha_TT*f_prime_TT_b*m_I_TT + m_alpha_TRT*(f_prime_TRT_b*m_I_TRT + f_prime_g_b*m_I_g);	//S_alpha_bar(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);
						alpha_bar_b_Under += under_b;																													//S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);

						beta_bar_f_Over  += m_beta_R*f_prime_R_f*m_I_R + m_beta_TT*f_prime_TT_f*m_I_TT + m_beta_TRT*(f_prime_TRT_f*m_I_TRT + f_prime_g_f*m_I_g);		//S_beta_bar(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);
						beta_bar_f_Under += under_f;																													//S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad);

						beta_bar_b_Over  += m_beta_R*f_prime_R_b*m_I_R + m_beta_TT*f_prime_TT_b*m_I_TT + m_beta_TRT*(f_prime_TRT_b*m_I_TRT + f_prime_g_b*m_I_g);		//S_beta_bar(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);
						beta_bar_b_Under += under_b;																													//S_scaled(theta_d_rad, phi_d_rad,theta_o_rad,phi_o_rad-c_Pi);
					}
				}
			}



			// We got the integral over phi_i out since no eccentricity, so no need for a 1/Pi factor
			// m_a_bar_f[index]/=c_Pi;
			// m_a_bar_b[index]/=c_Pi;

			m_alpha_bar_f[index] = alpha_bar_f_Over/colorChannelMax(alpha_bar_f_Under,AI_EPSILON);
			m_alpha_bar_b[index] = alpha_bar_b_Over/colorChannelMax(alpha_bar_b_Under,AI_EPSILON);

			m_beta_bar_f[index] = beta_bar_f_Over/colorChannelMax(beta_bar_f_Under,AI_EPSILON);
			m_beta_bar_b[index] = beta_bar_b_Over/colorChannelMax(beta_bar_b_Under,AI_EPSILON);

			AtColor a_bar_f = m_a_bar_f[index];
			AtColor a_bar_f_2 = a_bar_f*a_bar_f;

			AtColor a_bar_b = m_a_bar_b[index];
			AtColor a_bar_b_2 = a_bar_b*a_bar_b;
			AtColor a_bar_b_3 = a_bar_b_2*a_bar_b;

			AtColor beta_bar_f = m_beta_bar_f[index];
			AtColor beta_bar_f_2 = beta_bar_f*beta_bar_f;

			AtColor beta_bar_b = m_beta_bar_b[index];
			AtColor beta_bar_b_2 = beta_bar_b*beta_bar_b;

			AtColor white_minus_under = AI_RGB_WHITE-a_bar_f_2;
			AtColor white_minus_under_2 = white_minus_under*white_minus_under;
			AtColor white_minus_under_3 = white_minus_under_2*white_minus_under;

			m_A_bar_b[index] = ((a_bar_b*a_bar_f_2)/white_minus_under)+((a_bar_b_2*a_bar_b*a_bar_f_2)/white_minus_under_2);

			m_Delta_bar_b[index] =	m_alpha_bar_b[index]*( AI_RGB_WHITE-((2.0f*a_bar_b_2)/white_minus_under_2)) +
				m_alpha_bar_f[index]*(( 2.0f*white_minus_under_2 + 4.0f*a_bar_f_2*a_bar_b_2)/white_minus_under_3);

			m_sigma_bar_b[index] = (AI_RGB_WHITE + m_d_b*a_bar_f_2)*
				((a_bar_b*colorChannelSqrt(2.0f*beta_bar_f_2 + a_bar_b_2) + a_bar_b_3*colorChannelSqrt(2.0f*beta_bar_f_2 + a_bar_b_2))/
				(a_bar_b + a_bar_b_3*(2.0f*beta_bar_f + 3.0f*beta_bar_b)));




			// N_G_Xs

			for(int phi_prime = 0; phi_prime < res;phi_prime++)
			{
				float phi_prime_rad = static_cast<float>(phi_prime)/(resF-1.0f)*c_PiOver2 + c_PiOver2;
				float omega_phi_prime = c_PiOver2/(resF-1.0f);
				if(phi_prime==0 || phi_prime==(res-1))
					omega_phi_prime/=2.0f;

				m_N_G_R[index]	+= N_R(phi_prime_rad)*omega_phi_prime;
				m_N_G_TT[index]	+= N_TT(phi_prime_rad)*omega_phi_prime;
				m_N_G_TRT[index]+= N_TRT(phi_prime_rad)*omega_phi_prime;
				m_N_G_g[index]	+= N_g(phi_prime_rad)*omega_phi_prime;

			}

			m_N_G_R[index]	*= 2.0f/c_Pi;
			m_N_G_TT[index]	*= 2.0f/c_Pi;
			m_N_G_TRT[index]*= 2.0f/c_Pi;
			m_N_G_g[index]	*= 2.0f/c_Pi;

		}
	}

	inline AtColor sigmaBarF2(float theta_d,float nHairs) const
	{
		int index = indexFromSignedTheta(theta_d);
		AtColor color;
		color.r = std::pow(m_beta_bar_f[index].r,2);
		color.g = std::pow(m_beta_bar_f[index].g,2);
		color.b = std::pow(m_beta_bar_f[index].b,2);
		return color*nHairs;
	}

	inline void setFrameEtc(AtVector U, AtVector V, AtVector W, AtVector wo, float theta_r, float phi_r)
	{
		m_U = U;
		m_V = V;
		m_W = W;
		m_wo = wo;
		m_theta_r = theta_r;
		m_phi_r = phi_r;
	}	

	// SAMPLE INFO
	short*   m_MIScurrentStep;	// hack for MIS

protected:
	float m_beta_R, m_beta2_R;		// Widths of gaussian, squared
	float m_beta_TT, m_beta2_TT;
	float m_beta_TRT, m_beta2_TRT;
	float m_alpha_R;				// Mean of gaussian
	float m_alpha_TT;	
	float m_alpha_TRT;
	float m_gamma_TT;				// azimuthal width
	float m_gamma_g;
	float m_phi_g;					// half angle between peaks
	AtColor m_I_R;					// Color
	AtColor m_I_TT;
	AtColor m_I_TRT;
	AtColor m_I_g;
	float m_scale_R;				// Scales, ideally, the sum is < 1
	float m_scale_TT;
	float m_scale_TRT;
	float m_scale_g;
	int m_samples;					// number of samples
	AtColor m_shadow_opacity;
	float m_numGMS_modifier;		// number of intersection modifier
	float m_scale_direct;			// direct global scale
	AtColor m_color_direct;
	float m_scale_indirect;			// indirect global scale
	AtColor m_color_indirect;

	float m_alpha_back;				// Dual scattering
	float m_beta_back;
	float m_scale_back;
	AtColor m_color_back;
	float m_scale_forward;
	AtColor m_color_forward;


	// Precalculated
	AtColor m_a_bar_f[2*c_a_bar_f_nSamples-1]; //Precalculated average attenuation forward/backward
	AtColor m_a_bar_b[2*c_a_bar_f_nSamples-1];

	AtColor m_alpha_bar_f[2*c_a_bar_f_nSamples-1]; //Precalculated average attenuation forward/backward
	AtColor m_alpha_bar_b[2*c_a_bar_f_nSamples-1];

	AtColor m_beta_bar_f[2*c_a_bar_f_nSamples-1];	//Precalculated average scattering variance forward/backward
	AtColor m_beta_bar_b[2*c_a_bar_f_nSamples-1];

	AtColor m_Delta_bar_b[2*c_a_bar_f_nSamples-1];

	AtColor m_sigma_bar_b[2*c_a_bar_f_nSamples-1];

	AtColor m_A_bar_b[2*c_a_bar_f_nSamples-1];

	float m_N_G_R[2*c_a_bar_f_nSamples-1];

	float m_N_G_TT[2*c_a_bar_f_nSamples-1];

	float m_N_G_TRT[2*c_a_bar_f_nSamples-1];

	float m_N_G_g[2*c_a_bar_f_nSamples-1];

	// use preprocessing parameters if no connection
	bool m_use_pre_only;			// use preprocessing parameters
	bool m_pre_beta_R;		// Widths of gaussian, squared
	bool m_pre_beta_TT;
	bool m_pre_beta_TRT;
	bool m_pre_alpha_R;				// Mean of gaussian
	bool m_pre_alpha_TT;	
	bool m_pre_alpha_TRT;
	bool m_pre_gamma_TT;				// azimuthal width
	bool m_pre_gamma_g;
	bool m_pre_phi_g;					// half angle between peaks
	bool m_pre_I_R;					// Color
	bool m_pre_I_TT;
	bool m_pre_I_TRT;
	bool m_pre_I_g;
	bool m_pre_scale_R;				// Scales, ideally, the sum is < 1
	bool m_pre_scale_TT;
	bool m_pre_scale_TRT;
	bool m_pre_scale_g;

	bool m_normal_shifts_widths;

	AtColor	m_color_ray_glossy;
	int	m_mode_ray_glossy;
	AtColor	m_color_ray_diffuse;
	int	m_mode_ray_diffuse;
	AtColor	m_color_ray_reflected;
	int	m_mode_ray_reflected;
	AtColor	m_color_ray_refracted;
	int	m_mode_ray_refracted;

	float m_d_f;
	float m_d_b;
	bool m_pre_d_f;
	bool m_pre_d_b;

	bool m_singleScattering;
	bool m_useImportanceSampling;
	int m_MISrandomLobeSamples;

	// precalc value samples for importance sampling
	float m_a_R;
	float m_a_TT;
	float m_a_TRT;
	float m_b_R;
	float m_b_TT;
	float m_b_TRT;
	float m_c_TT;
	float m_c_G;
	float m_d_G;

	float m_E_R;
	float m_E_TT;
	float m_E_TRT;
	float m_E_G;
	float m_E_Sum;


	int m_samples_R;
	int m_samples_TT;
	int m_samples_TRT;
	int m_samples_G;
	int m_samples_DS;
	int m_modeMIS_DS;

	bool m_unlinkOpacity;
	AtColor m_opacity;

	bool m_checkShaderNameToo;
	//bool m_useCritical;

	int m_modeMIS;
	AtVector m_U, m_V, m_W, m_wo;
	float m_theta_r, m_phi_r;


	AtShaderGlobals* m_sg;
	ObqHair* m_dHairData;

	// SAMPLE INFO
	AtByte m_MIScurrentLobe;			// current lob
	AtUInt32 m_MIScurrentShadowCounter; // mis shadow counter
	bool m_MIScurrentUseRandom;			// use random values for evalSample instead of provided
	float   m_MISspecialPdf;			// hack for MIS
	AtColor m_MISspecialBCSDFColor;		// hack for MIS

	int m_MISsamples;
	AtUInt32 m_nIntersectionsSP;
};

typedef struct 
{
	ObqHair hairData;
	AtSampler *sampler;
	int samples;
	int aov_mode;
	int max_diffuse_depth;
	int max_glossy_depth;
	//AtCritSec criticalSec;
	const char* aov_dir_R;
	const char* aov_dir_TRT;
	const char* aov_dir_g;
	const char* aov_dir_TT;
	const char* aov_dir_back;
	const char* aov_dir_scatter;
	const char* aov_dir_scatter_back;
	const char* aov_indir_R;
	const char* aov_indir_TRT;
	const char* aov_indir_g;
	const char* aov_indir_TT;
	const char* aov_indir_back;
	const char* aov_indir_scatter;
	const char* aov_indir_scatter_back;
}
ShaderData;

#endif // OBQTESTHAIR_H
