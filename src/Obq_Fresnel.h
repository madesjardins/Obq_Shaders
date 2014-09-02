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

#include "Obq_Common.h"
#include "pbrt/spectrum.h"
#include <vector>

#define MIN_LAMBDA 360.f
#define MAX_LAMBDA 830.f

// Arnold stuff
//
AI_SHADER_NODE_EXPORT_METHODS(ObqFresnelSimpleMethods);

// enum for parameters
//
enum ObqFresnelSimpleParams { p_mode, p_iorFilename,p_lambdaUnits, p_iorRGB, p_kRGB, p_method, p_iorInRGB, p_ratioFsFp, p_transmittance, p_backfaceMode, p_degamma, p_useLUT, p_LUTSampleSize, p_useFullSpectrum};

// enum for fresnel equations
//
enum {REFRACTIVE, METALS};

// enum for file type
//
enum {TYPE_CSV,TYPE_TXT,TYPE_SPD, TYPE_BAD};

// enum for units
//
enum {UNITS_NANO, UNITS_MICRO};

// enum for mode
//
enum {MODE_CUSTOM, MODE_PRESETS, MODE_FILES};

// enum for backfacemode
enum {BACKFACE_SAME, BACKFACE_SWAP, BACKFACE_WHITE, BACKFACE_BLACK};

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
	float LUTSampleSize;
	AtColor* LUT;
	bool useLUTInterpolation;
	bool useFullSpectrum;
	int lambdasSize;
	float* lambdas;
	float* etas;
	float* ks;
	bool degamma;
}
ShaderData;



// Check if file exists
//
inline bool fileExists(const char* filename)
{
    std::ifstream ifs ( filename , std::ifstream::in );

    if(ifs==NULL)
        return false;

    ifs.close();
    return true;

}

// check for file type
//
inline short fileType(const char* filename)
{
	std::string fname(filename);
	
	if(fname.size() < 4)
		return TYPE_BAD;

	if(fname.compare(fname.size()-4,4,".csv") == 0 || fname.compare(fname.size()-4,4,".CSV") == 0 )
		return TYPE_CSV;
	if(fname.compare(fname.size()-4,4,".txt") == 0 || fname.compare(fname.size()-4,4,".TXT") == 0 )
		return TYPE_TXT;
	if(fname.compare(fname.size()-4,4,".spd") == 0 || fname.compare(fname.size()-4,4,".SPD") == 0 )
		return TYPE_SPD;

	return TYPE_BAD;
}

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

// split a string with character c
//
// Shi Chuan : http://www.blog.highub.com/c-plus-plus/c-parse-split-delimited-string/
//
inline void split(const  std::string& s, char c, std::vector<std::string>& v) {
   std::string::size_type i = 0;
   std::string::size_type j = s.find(c);
   while (j != std::string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);
      if (j == std::string::npos)
         v.push_back(s.substr(i, s.length( )));
   }
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
