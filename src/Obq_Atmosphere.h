/*
Obq_Atmosphere :

	Earth's atmosphere

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

#ifndef OBQATMOSPHERE_H
#define OBQATMOSPHERE_H

#include "O_Common.h"


// Arnold Thingy
//
AI_SHADER_NODE_EXPORT_METHODS(ObqAtmosphereMethods);

// enum of parameters
//
enum ObqAtmosphereParams { p_colorKd, /*p_use3dKd,*/ p_absorption, /*p_use3dAbsorption,*/ p_asymmetry, p_stepSize, p_stepJitter, p_useBiasGain, p_bias, p_gain,p_useBiasGainDot, p_biasDot, p_gainDot,p_radiusFalloffMode, p_falloffStart, p_falloffExponent, p_objectCenter, p_compensateForCloudOpacity, p_divideColorByOpacity, p_useMaximumRayLength, p_maximumRayLength, p_secondaryTypesToo};

enum ObqAtmosphereFalloffMode {RFM_NONE, RFM_MATRIX, RFM_VECTOR};
static const char* ObqAtmosphereFalloffModeNames[] = 
{
	"Don't Use Radius Falloff",
    "Get Center From Object Matrix",
    "Get Center From Object Center Vector",
    NULL
};

enum ObqAtmo{ATMO_FIRST, ATMO_BF};

enum ObqAtmoDivide{ATMO_NEVER, ATMO_BEFORE, ATMO_AFTER};
static const char* ObqAtmoDivideNames[] = 
{
	"Never",
    "Before Bias-Gain",
    "After Bias-Gain",
    NULL
};

enum ObqAtmoCompensate{ATMO_COMP_NEVER, ATMO_COMP_DROP, ATMO_COMP_RAISE};
static const char* ObqAtmoCompensateNames[] = 
{
	"Never",
    "By dropping max opacity",
    "By raising total opacity",
    NULL
};

// Shader Data Structure
//
typedef struct 
{
	int	falloffMode;
	int compensateForCloudOpacity; 
	int divideColorByOpacity;
	bool secondaryTypesToo;
	bool use3dKd;
	bool use3dAbsorption;
}
ShaderData;

#endif //OBQATMOSPHERE_H