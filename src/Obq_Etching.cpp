/*
Obq_Etching

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



typedef struct 
{
   const char* aov_matte;
   const char* aov_input;
   bool autoFilteringOut;
}
ShaderData;


// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqEtchingSimpleMethods);

// enum for parameters
//
enum ObqEtchingSimpleParams { 
	p_shading_input, 
	p_texture_space_input, 
	p_use_linear_signal, 
	p_bright_color,
	p_bright_point,
	p_dark_color,
	p_dark_point,
	p_frequency,
	p_offset,
	p_feather,
	p_use_average_rgb,
	p_multiply_bright_color, 
	p_multiply_dark_color,
	p_mix,
	p_enable_dots,
	p_bright_dots_start,
	p_dark_dots_start,
	p_progressive_dots,
	p_aov_matte,
	p_aov_input,
	p_autoFiltering,
	p_autoFilteringBias,
	p_autoFilteringGain,
	p_autoFilteringStart,
	p_autoFilteringEnd,
	p_autoFilteringOut,
	p_autoFilteringU,
	p_autoFilteringMode,
	p_autoFilteringAutoRange
};

enum obqSignals{OBQ_BOX, OBQ_TRIANGLE, OBQ_WAVE};

node_parameters
{
	AiParameterRGB("shading_input", 1.0f,1.0f,1.0f);
	AiParameterVEC("texture_space_input", 0.0f,0.0f,0.0f);
	AiParameterBOOL("use_linear_signal",false);
	AiParameterRGB("bright_color", 1.0f,1.0f,1.0f);
	AiParameterFLT("bright_point",1.0f);
	AiParameterRGB("dark_color", 0.0f,0.0f,0.0f);
	AiParameterFLT("dark_point",0.0f);
	AiParameterFLT("frequency",50.0f);
	AiParameterFLT("offset",0.0);
	AiParameterFLT("feather",0.0f);
	AiParameterBOOL("use_average_rgb",false);
	AiParameterBOOL("multiply_bright_color",false);	
	AiParameterBOOL("multiply_dark_color",false);
	AiParameterFLT("mix",0.0f);
	AiParameterBOOL("enable_dots",false);
	AiParameterFLT("bright_dots_start",0.9f);
	AiParameterFLT("dark_dots_start",0.1f);
	AiParameterBOOL("progressive_dots",false);
	AiParameterSTR("aov_matte","Obq_Etching_Matte");
	AiParameterSTR("aov_input","Obq_Etching_Input");
	AiParameterBOOL("autoFiltering",false);
	AiParameterFLT("autoFilteringBias",0.725f);
	AiParameterFLT("autoFilteringGain",0.125f);
	AiParameterFLT("autoFilteringStart",0.002f);
	AiParameterFLT("autoFilteringEnd",0.02f);
	AiParameterBOOL("autoFilteringOut",false);
	AiParameterBOOL("autoFilteringU",false);
	AiParameterINT("autoFilteringMode",0);
	AiParameterBOOL("autoFilteringAutoRange",true);
}

node_initialize
{
	// initialisation of sampler
   ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
   data->aov_matte = "Obq_Etching_Matte";
   data->aov_input = "Obq_Etching_Input";
   AiNodeSetLocalData(node,data);
}

node_update
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	data->aov_matte = params[p_aov_matte].STR;
	data->aov_input = params[p_aov_input].STR;
	data->autoFilteringOut = params[p_autoFilteringOut].BOOL;
	
}

node_finish
{
	// destroy sampler data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	// Get parameters
	float frequency = AiShaderEvalParamFlt(p_frequency);
	float feather = MAX( 0.0f, MIN(AiShaderEvalParamFlt(p_feather),1.0f));
	float mix = MAX( 0.0f, MIN( AiShaderEvalParamFlt(p_mix),1.0f));

	bool autoFiltering = AiShaderEvalParamBool(p_autoFiltering);

	// Auto-filtering
	if(autoFiltering)
	{
		//starting at feather, being more aggressive will get you to feather = 1 faster, while less aggressive will keep you closer to actual feather
		//if(aggressiveness == 1.0f) // more
		//		tf=1.0f;
		//	else if(aggressiveness == -1.0f) // more
		//		tf=0.0f;
		//	else if(aggressiveness > 0.0f) // more
		//		tf = std::pow(tf,(1.0f-aggressiveness));
		//	else if(aggressiveness < 0.0f) // less
		//		tf = std::pow(tf,1.0f/(1.0f+aggressiveness));
	
		bool autoRange = AiShaderEvalParamBool(p_autoFilteringAutoRange);
		float start = (autoRange?0.1f/frequency:AiShaderEvalParamFlt(p_autoFilteringStart));
		float end =   (autoRange?1.0f/frequency:MAX(AiShaderEvalParamFlt(p_autoFilteringEnd),start));

		float tf = sqrt(sg->dvdx*sg->dvdx + sg->dvdy*sg->dvdy);
		
		// Take U in consideration also ?Q
		if(AiShaderEvalParamBool(p_autoFilteringU))
			tf = MAX(tf,static_cast<float>(sqrt(sg->dudx*sg->dudx + sg->dudy*sg->dudy)));
		
		// clamp to be sure
		if(tf>1.0f)	
			tf = 1.0f;
		
		// remap
		if(tf >= end)
			tf=1.0f;
		else if(tf <= start)
			tf = 0.0f;
		else
		{

			tf = (tf-start)/(end-start);
			
			// Bias and gain
			float bias =  AiShaderEvalParamFlt(p_autoFilteringBias);
			float gain =  AiShaderEvalParamFlt(p_autoFilteringGain);

			if(bias!=0.5)
				tf = BIAS(tf,bias);
			if(gain!=0.5)
				tf = GAIN(tf,1.0f-gain);
		}
		
		// Debug
		if(data->autoFilteringOut)
		{
			sg->out.RGB = tf;
			return;
		}

		// Apply to feather and or mix
		int autoFilteringMode = AiShaderEvalParamBool(p_autoFilteringMode);
		if(autoFilteringMode > 0 )
			mix += (1.0f-mix)*tf;

		if(autoFilteringMode < 2)
			feather += (1.0f-feather)*tf;
		
	}

	// Other Parameters
	AtColor shading_input =  AiShaderEvalParamRGB(p_shading_input);
	AtVector tex_input =  AiShaderEvalParamVec(p_texture_space_input);
	AtColor bright_color = AiShaderEvalParamRGB(p_bright_color);
	AtColor dark_color = AiShaderEvalParamRGB(p_dark_color);
	bool use_linear_signal = AiShaderEvalParamBool(p_use_linear_signal);
	float bright_point = AiShaderEvalParamFlt(p_bright_point)-AI_EPSILON;
	float dark_point = MIN(AiShaderEvalParamFlt(p_dark_point)+AI_EPSILON,bright_point);
	float offset = AiShaderEvalParamFlt(p_offset);
	bool use_average_rgb = AiShaderEvalParamBool(p_use_average_rgb);
	bool mult_bright = AiShaderEvalParamBool(p_multiply_bright_color);
	bool mult_dark = AiShaderEvalParamBool(p_multiply_dark_color);
	
	bool enable_dots = AiShaderEvalParamBool(p_enable_dots);

	// Compute shading Value
	float value =   (use_average_rgb? (shading_input.r+shading_input.g+shading_input.b)/3.0f : getLuminance(shading_input));
	float value_n = (value-dark_point)/(bright_point - dark_point);

	// Compute Signal
	float signal = 0.0f;

	//float T = 0.0f;

	if(use_linear_signal)	// Linear
	{
		float x = (frequency*tex_input.y - offset);
		x-=std::floor(x); // keep decimals only

		if(x<0.0f) // between [0,1)
			x = 1.0f-x;

		if(x < 0.25f) // f(x)
			signal = (4.0f*x+1.0f)/2.0f;
		else if( x < 0.75f)
			signal = (2.0f - 4.0f*(x-0.25f))/2.0f;
		else
			signal = 2.0f*(x-0.75f);		
	}
	else
		signal =  (std::sin(c_2Pi*(frequency*tex_input.y - offset))+1.0f)/2.0f; // sine

	// Final color
	float c = MIN(1.0f, signal + feather);
	float f = MAX(0.0f, signal - feather);

	float t = 0.0f;
	if(enable_dots)
	{
		bool progressive_dots = AiShaderEvalParamBool(p_progressive_dots);
		float bright_dots_start = AiShaderEvalParamFlt(p_bright_dots_start);
		float dark_dots_start = AiShaderEvalParamFlt(p_dark_dots_start);

		// clamp value, dark area should have dots
		if(value_n > bright_dots_start && bright_dots_start < 1.0f)	// Bright Dots in dark lines
		{
			float value_over = MIN(1.0f,(value_n - bright_dots_start)/(1.0f-bright_dots_start));
			value_n = bright_dots_start;

			if(value_n >= c) // bright
			{
				t = 1.0f;
			}
			else // Dark dotted area
			{
				// get dot frequencey
				float freq_v = frequency;
				float sig_v = 0.0f;

				if(use_linear_signal)	// Linear
				{
					freq_v = frequency/(1.0f-bright_dots_start)/2.0f;

					float x = (freq_v*tex_input.x - offset);

					int period = static_cast<int>(std::floor(x)); // keep integer
					x-=period; // keep decimals only

					if(x<0.0f) // between [0,1)
						x = 1.0f-x;

					if(x < 0.25f) // f(x)
						sig_v = (4.0f*x+1.0f)/2.0f;
					else if( x < 0.75f)
						sig_v = (2.0f - 4.0f*(x-0.25f))/2.0f;
					else
						sig_v = 2.0f*(x-0.75f);
				}
				else
				{
					float l = 1.0f-2.0f*(0.25f-(std::asin(2.0f*(bright_dots_start-0.5f))/c_2Pi));
					freq_v = frequency/l;

					sig_v =  (std::sin(c_2Pi*(freq_v*tex_input.x - offset))+1.0f)/2.0f; // sine
				}

				float feather_v =MIN(1.0f,feather*freq_v/frequency);
				float c2 = MIN(1.0f, sig_v + feather_v);
				float f2 = MAX(0.0f, sig_v - feather_v);

				// for tt				
				float tt = 0.0f;
				if(0.5f >= c2) // bright
					tt = 1.0f;
				else if(0.5f <= f2) // dark
					tt = 0.0f;
				else
					tt = (0.5f - f2)/(c2-f2);

				if(progressive_dots)
				{
					if(value_over < 0.5f)
						tt*= (value_over)*2.0f;
					else
						tt = MIN(1.0f,tt + 2.0f*(value_over-0.5f));
				}
				else if(value_over >= 1.0f)
					tt = 1.0f;
				

				// for t
				if(value_n <= f) // dark
					t = 0.0f;
				else
					t = (value_n - f)/(c-f);

				// Comp t and tt
				t = MAX(t,tt);
			}
		}
		else if(value_n < dark_dots_start && dark_dots_start > 0.0f)	// Dark Dots in bright lines
		{
			float value_under = MIN(1.0f,value_n/(dark_dots_start));
			value_n = dark_dots_start;

			if(value_n <= f) // dark
			{
				t = 0.0f;
			}
			else // bright dotted area
			{
				// get dot frequencey
				float freq_v = frequency;
				float sig_v = 0.0f;

				if(use_linear_signal)	// Linear
				{
					freq_v = frequency/dark_dots_start/2.0f;

					float x = (freq_v*tex_input.x - offset);

					int period = static_cast<int>(std::floor(x)); // keep integer
					x-=period; // keep decimals only

					if(x<0.0f) // between [0,1)
						x = 1.0f-x;

					if(x < 0.25f) // f(x)
						sig_v = (4.0f*x+1.0f)/2.0f;
					else if( x < 0.75f)
						sig_v = (2.0f - 4.0f*(x-0.25f))/2.0f;
					else
						sig_v = 2.0f*(x-0.75f);
				}
				else
				{
					float l = 1.0f-2.0f*((std::asin(2.0f*((1.0f-dark_dots_start)-0.5f))/c_2Pi));
					freq_v = frequency/l;

					sig_v =  (std::sin(c_2Pi*(freq_v*tex_input.x - offset))+1.0f)/2.0f; // sine
				}

				float feather_v = MIN(1.0f,feather*freq_v/frequency);
				float c2 = MIN(1.0f, sig_v + feather_v);
				float f2 = MAX(0.0f, sig_v - feather_v);

				// for tt				
				float tt = 0.0f;
				if(0.5f >= c2) // bright
					tt = 1.0f;
				else if(0.5f <= f2) // dark
					tt = 0.0f;
				else
					tt = (0.5f - f2)/(c2-f2);

				if(progressive_dots)
				{
					if(value_under < 0.5f)
						tt*= (value_under)*2.0f;
					else
						tt = MAX(0.0f,MIN(1.0f,tt + 2.0f*(value_under-0.5f)));
				}
				else if(value_under <= 0.0f)
					tt = 0.0f;
				

				// for t
				if(value_n >= c) // bright
					t = 1.0f;
				else
					t = (value_n - f)/(c-f);

				// Comp t and tt
				t = MIN(t,tt);
			}
		}
		else
		{
			if(value_n >= c) // bright
				t = 1.0f;
			else if(value_n <= f) // dark
				t = 0.0f;
			else
				t = (value_n - f)/(c-f);
		}
	}
	else
	{
		if(value_n >= c) // bright
			t = 1.0f;
		else if(value_n <= f) // dark
			t = 0.0f;
		else
			t = (value_n - f)/(c-f);
	}


	// Write AOVs
	if(sg->Rt & AI_RAY_CAMERA)
	{
		if(AiAOVEnabled (data->aov_matte, AI_TYPE_RGB))
			AiAOVSetRGB (sg, data->aov_matte, t*AI_RGB_WHITE);
		else if(AiAOVEnabled (data->aov_matte, AI_TYPE_FLOAT))
			AiAOVSetFlt (sg, data->aov_matte, t);

		if(AiAOVEnabled (data->aov_input, AI_TYPE_RGB))
			AiAOVSetRGB (sg, data->aov_input, shading_input);
	}

	// Multiply by normalized (HSV with v=1)
	if( mult_bright || mult_dark)
	{
		AtColor hsv;
		RGBtoHSV(shading_input,hsv);
		hsv.b = 1.0;
		AtColor rgb;
		HSVtoRGB(hsv,rgb);
		if(mult_bright)
			bright_color*= rgb;

		if(mult_dark)
			dark_color*=rgb;
	}

	// MIX
	if(mix > 0.0f)
		sg->out.RGB = mix*shading_input + (1.0f-mix)*(t*bright_color+(1.0f-t)*dark_color);
	else
		sg->out.RGB = t*bright_color+(1.0f-t)*dark_color;

}

//node_loader
//{
//	if (i > 0)
//		return FALSE;
//
//	node->methods      = ObqEtchingSimpleMethods;
//	node->output_type  = AI_TYPE_RGB;
//	node->name         = "Obq_Etching";
//	node->node_type    = AI_NODE_SHADER;
//	strcpy(node->version, AI_VERSION);
//	return TRUE;
//}
