/*
Obq_ChangeRange:

	a version of the classic scalar_change_range with some extra features.

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

#include "O_Common.h"

AI_SHADER_NODE_EXPORT_METHODS(ObqChangeRangeMethods);

enum ObqChangeRangeParams { p_input, p_oStart, p_oEnd, p_nStart, p_nEnd, p_clamp, p_bias, p_gain};



node_parameters
{
	AiParameterFLT("input", 0.5f);					// scalar input
	AiParameterFLT("oStart", 0.0f);					// old Start
	AiParameterFLT("oEnd", 1.0f);					// old End
	AiParameterFLT("nStart", 0.2f);					// new Start
	AiParameterFLT("nEnd", 0.8f);					// new End
	AiParameterBOOL("clamp", true);					// clamp ? or linear extrapolation
	AiParameterFLT("bias", 0.0f);					// bias
	AiParameterFLT("gain", 1.0f);					// gain
}

node_initialize
{
}

node_update
{
}


node_finish
{
}


shader_evaluate
{
	
	float input = AiShaderEvalParamFlt(p_input);
	float oStart = AiShaderEvalParamFlt(p_oStart);
	float nStart = AiShaderEvalParamFlt(p_nStart);
	float oEnd = AiShaderEvalParamFlt(p_oEnd);
	float nEnd = AiShaderEvalParamFlt(p_nEnd);
	bool clamp = AiShaderEvalParamBool(p_clamp);
	bool clamped = false;

	/////////////////////////////////////
	// Part 1 : Parametrisation of input
	/////////////////////////////////////
	float t = 0.0f;

	if(oEnd == oStart)
		t = 0.0f;			// simple test to prevent /0
	else if(oEnd < oStart)
		t = 1.0f-(input - oEnd)/(oStart-oEnd);		// Inverted old range
	else
		t = (input - oStart)/(oEnd-oStart);

	////////////////////////////////
	// Clamp values to 0.0 < t < 1.0
	////////////////////////////////
	if(clamp)
	{
		if(t <= 0.0f)
		{
			t = 0.0f;
			clamped = true;
		}
		else if(t >= 1.0f)
		{
			t = 1.0f;
			clamped = true;
		}
	}

	/////////////////
	// Bias and Gain ( no need if values were clamped )
	/////////////////
	if(!clamped)
	{
		float bias = AiShaderEvalParamFlt(p_bias);
		float gain = 1.0f-AiShaderEvalParamFlt(p_gain);
		
		if(bias !=0.5f)
			t = BIAS(t,bias);
		
		if(gain !=0.5f)
			t = GAIN(t,gain);

	}
	/////////////////////////////////
	// Part 2 : remapping and outpout
	/////////////////////////////////
	if(nEnd < nStart)
	{
		sg->out.FLT = (1.0f - t)*(nStart-nEnd) + nEnd;	// inverted range
	}
	else
	{
		sg->out.FLT = t*(nEnd-nStart) + nStart;	
	}
}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqChangeRangeMethods;
//   node->output_type  = AI_TYPE_FLOAT;
//   node->name         = "Obq_ChangeRange";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
