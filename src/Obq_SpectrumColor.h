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


// enum for units
enum {UNITS_NANO, UNITS_MICRO};

// enum for mode
enum {MODE_FILES, MODE_STRING};

// shader data struct
typedef struct 
{
	float rgb[3];
}
ShaderData;


// load a file
//
inline bool loadSPD(const char* filename, std::vector<float>* lambdas, std::vector<float>* values, float unitConversion){

	char buffer[256];
	std::string line;

	std::ifstream ifs ( filename , std::ifstream::in );
	
	if(!ifs.is_open())
	{
		AiMsgError("Can't open file : \"%s\".",filename);
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
		AiMsgError("There was a problem with file : \"%s\".",filename);
		return false;
	}

	return true;
}

// load a file
inline bool loadTXT(const char* filename, std::vector<float>* lambdas, std::vector<float>* values, float unitConversion){

	char buffer[256];
	std::string line;
	
	std::ifstream ifs ( filename , std::ifstream::in );
	
	if(!ifs.is_open())
	{
		AiMsgError("Can't open file : \"%s\".",filename);
		return false;
	}
	
	while (ifs.good()){
		ifs.getline (buffer,256);
		line = std::string(buffer);
		std::string buf;
		std::stringstream ss(line);
		std::vector<std::string> tokens;
		float lambda, value;

		// need to tokenize the string
		while(ss >> buf)
			tokens.push_back(buf);
		
		if(tokens.size() >= 2 && from_string<float>(lambda, tokens[0], std::dec) && from_string<float>(value, tokens[1], std::dec))
		{
			float ulambda = unitConversion*lambda;
			if(ulambda >= MIN_LAMBDA && ulambda <=  MAX_LAMBDA)
			{
				lambdas->push_back( ulambda );
				values->push_back( value );
			}
		}
	}
	ifs.close();
	if(lambdas->size() == 0)
	{
		AiMsgError("There was a problem with file : \"%s\".",filename);
		return false;
	}
	
	return true;
}

// Strings are space separated
inline bool loadStrings(const char* lStr, const char* valueStr, std::vector<float>* lambdas, std::vector<float>* values, float unitConversion)
{
	int flag = 0;
		
	std::string lStr_s(lStr), valueStr_s(valueStr);
	std::string buf;
	
	// LAMBDAs
	std::stringstream ss(lStr_s);
	std::vector<std::string> tokens;
	float lambda, value;
	
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
	
	// Values
	std::stringstream ssv(valueStr_s);
	while(ssv >> buf)
		tokens.push_back(buf);

	for( std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		if(from_string<float>(value, (*it), std::dec))
			values->push_back( value );

	}
	tokens.clear();

	// check for return
	return (lambdas->size() == values->size());
}

// load a file
//
inline bool loadCSV(const char* filename, std::vector<float>* lambdas, std::vector<float>* values, float unitConversion){

	char buffer[256];
	std::string line;
	
	std::ifstream ifs ( filename , std::ifstream::in );
	
	if(!ifs.is_open())
	{
		AiMsgError("Can't open file : \"%s\".",filename);
		return false;
	}
	
	while (ifs.good()){
		ifs.getline (buffer,256);
		line = std::string(buffer);
		std::string buf;
		std::stringstream ss(line);
		std::vector<std::string> tokens;
		float lambda, value;

		// need to tokenize the string
		std::vector<std::string> v;
		split(line, ',', v);

		if(v.size() >= 2 && from_string<float>(lambda, v[0], std::dec) && from_string<float>(value, v[1], std::dec))
		{
			float ulambda = unitConversion*lambda;
			if(ulambda >= MIN_LAMBDA && ulambda <=  MAX_LAMBDA)
			{
				lambdas->push_back( ulambda );
				values->push_back( value );
			}
		}
	}
	ifs.close();
	if(lambdas->size() == 0)
	{
		AiMsgWarning("There was a problem with file : \"%s\". Be sure the good units were chosen.",filename);
		return false;
	}
	
	return true;
}
