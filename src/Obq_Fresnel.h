/*
Obq_Fresnel :

	a dielectric and a complex fresnel term node. It uses pbrt spectrum class.

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

#include "O_Common.h"
#include "pbrt/spectrum.h"

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqFresnelMethods);

// enum for parameters
//
enum ObqFresnelParams { p_mode, p_iorFilename, p_lambdaUnits, p_iorRGB, p_kRGB, p_method, p_iorInRGB, p_ratioFsFp, p_transmittance, p_backfaceMode, p_useLUT, p_LUTSampleSize, p_useFullSpectrum, p_degamma, p_outputGamma, p_lambdasStr, p_etasStr, p_ksStr, p_inputType, p_ccMode, p_hueShift, p_saturationMod, p_satOp, p_roughness, p_roughnessSampleSize, p_xyz2rgb};

// enum for fresnel equations
//
enum {REFRACTIVE, METALS};

// enum for units
//
enum {UNITS_NANO, UNITS_MICRO};

// enum for mode
//
enum {MODE_CUSTOM, MODE_PRESET_LEGACY, MODE_FILES};

// enum for backfacemode
enum {BACKFACE_SAME, BACKFACE_SWAP, BACKFACE_WHITE, BACKFACE_BLACK};

// enum for colorcorrection
enum {CC_OFF, CC_HSV, CC_HLS};

// enum for colorcorrection
enum {CC_ADD, CC_MUL, CC_POW};

// shader data struct
//
typedef struct 
{
	int mode;
	int method;
	float eta[3];
	float k[3];
	bool useLUT;
	int LUTRes;
	float LUTResM1;
	float LUTSampleSize;
	AtColor* LUT;
	bool useLUTInterpolation;
	int roughnessRes;
	float roughnessResM1;
	float roughnessSampleSize;
	bool useFullSpectrum;
	bool degamma;
	int lambdasSize;
	float* lambdas;
	float* etas;
	float* ks;
	bool transfer2sRGB;
	int xyz2rgb;
	int ccMode;
	int satOp;
}
ShaderData;


// load a file
//
inline bool loadSPD(const char* filename, std::vector<float>* lambdas, std::vector<float>* values, float unitConversion){

	char buffer[256];
	std::string line;

	std::ifstream ifs ( filename , std::ifstream::in );
	
	if(ifs==NULL)
	{
		AiMsgWarning("Can't open file : \"%s\". Will use ior = 1,1,1.",filename);
		return false;
	}
	
	while (ifs.good()){
		ifs.getline (buffer,256);
		line = std::string(buffer);
		std::string buf;
		std::stringstream ss(line);
		std::vector<std::string> tokens;
		float lambda, v;

		// need to tokenize the string
		while(ss >> buf)
			tokens.push_back(buf);
		if(tokens.size() >= 2)
			if(from_string<float>(lambda, tokens[0], std::dec) && from_string<float>(v, tokens[1], std::dec))
			{
				float ulambda = unitConversion*lambda;
				if(ulambda >= MIN_LAMBDA && ulambda <=  MAX_LAMBDA)
				{
					lambdas->push_back( ulambda );
					values->push_back( v );
				}
			}
	}
	ifs.close();
	if(lambdas->size() == 0)
	{
		AiMsgWarning("There was a problem with file : \"%s\". Will use ior = 1,1,1.",filename);
		return false;
	}

	return true;
}

// load a file
//
inline bool loadTXT(const char* filename, std::vector<float>* lambdas, std::vector<float>* etas,std::vector<float>* ks, float unitConversion){

	char buffer[256];
	std::string line;
	
	std::ifstream ifs ( filename , std::ifstream::in );
	
	if(ifs==NULL)
	{
		AiMsgWarning("Can't open file : \"%s\". Will use ior = 1,1,1 and k = 0,0,0",filename);
		return false;
	}
	
	while (ifs.good()){
		ifs.getline (buffer,256);
		line = std::string(buffer);
		std::string buf;
		std::stringstream ss(line);
		std::vector<std::string> tokens;
		float lambda, eta, k;

		// need to tokenize the string
		while(ss >> buf)
			tokens.push_back(buf);
		
		if(tokens.size() >= 2 && from_string<float>(lambda, tokens[0], std::dec) && from_string<float>(eta, tokens[1], std::dec))
		{
			float ulambda = unitConversion*lambda;
			if(ulambda >= MIN_LAMBDA && ulambda <=  MAX_LAMBDA)
			{
				lambdas->push_back( ulambda );
				etas->push_back( eta );
				if(tokens.size() >=3 && from_string<float>(k, tokens[2], std::dec))
					ks->push_back( k );
				else
					ks->push_back(0.0f);
			}
		}
	}
	ifs.close();
	if(lambdas->size() == 0)
	{
		AiMsgWarning("There was a problem with file : \"%s\". Will use ior = 1,1,1 and k = 0,0,0",filename);
		return false;
	}
	
	return true;
}

// Strings are space separated
inline bool loadStrings(const char* lsStr, const char* etasStr, const char* ksStr, std::vector<float>* lambdas, std::vector<float>* etas,std::vector<float>* ks, float unitConversion)
{
	
	std::string lsStr_s(lsStr), etasStr_s(etasStr), ksStr_s(ksStr);
	std::string buf;
	
	// LAMBDAs
	std::stringstream ss(lsStr_s);
	std::vector<std::string> tokens;
	float lambda, eta, k;
	
	while(ss >> buf)
		tokens.push_back(buf);

	for( std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		if(from_string<float>(lambda, (*it), std::dec))
		{
			float ulambda = unitConversion*lambda;
			if(ulambda >= MIN_LAMBDA && ulambda <=  MAX_LAMBDA)
				lambdas->push_back( ulambda );
		}
	}
	tokens.clear();
	
	// ETAs
	std::stringstream ssn(etasStr_s);
	while(ssn >> buf)
		tokens.push_back(buf);

	for( std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		if(from_string<float>(eta, (*it), std::dec))
			etas->push_back( eta );

	}
	tokens.clear();

	// Ks
	std::stringstream ssk(ksStr_s);
	while(ssk >> buf)
		tokens.push_back(buf);

	for( std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		if(from_string<float>(k, (*it), std::dec))
			ks->push_back( k );
	}

	// check for return
	return (lambdas->size() == etas->size() && (lambdas->size() == ks->size() || ks->size() == 0));
}

// load a file
//
inline bool loadCSV(const char* filename, std::vector<float>* lambdas, std::vector<float>* etas,std::vector<float>* ks, float unitConversion){

	char buffer[256];
	std::string line;
	
	std::ifstream ifs ( filename , std::ifstream::in );
	
	if(ifs==NULL)
	{
		AiMsgWarning("Can't open file : \"%s\". Will use ior = 1,1,1 and k = 0,0,0",filename);
		return false;
	}
	
	while (ifs.good()){
		ifs.getline (buffer,256);
		line = std::string(buffer);
		std::string buf;
		std::stringstream ss(line);
		std::vector<std::string> tokens;
		float lambda, eta, k;

		// need to tokenize the string
		std::vector<std::string> v;
		split(line, ',', v);

		if(v.size() >= 2 && from_string<float>(lambda, v[0], std::dec) && from_string<float>(eta, v[1], std::dec))
		{
			float ulambda = unitConversion*lambda;
			if(ulambda >= MIN_LAMBDA && ulambda <=  MAX_LAMBDA)
			{
				lambdas->push_back( ulambda );
				etas->push_back( eta );
				if(v.size() >=3 && from_string<float>(k, v[2], std::dec))
					ks->push_back( k );
				else
					ks->push_back(0.0f);
			}
		}
	}
	ifs.close();
	if(lambdas->size() == 0)
	{
		AiMsgWarning("There was a problem with file : \"%s\". Be sure the good units were chosen. Will use ior = 1,1,1 and k = 0,0,0",filename);
		return false;
	}
	
	return true;
}
