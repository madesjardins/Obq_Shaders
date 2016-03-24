// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef ldpk_model_parser_sdv
#define ldpk_model_parser_sdv

#include <ldpk/tde4_ld_plugin.h>
#include <ldpk/ldpk_error.h>
#include <iostream>
#include <string>

//! @file ldpk_model_parser.h
//! @brief Parsing parameters from a stream

namespace ldpk
	{
//! @brief A class for reading distortion model parameters from a stream.
//! Might be helpful for testing. Just another nice-to-have...
	class model_parser
		{
	private:
		tde4_ld_plugin *_model;
		bool _verbose;

//! Parameter name. The return value false indicates end of stream.
//! true means a string was parsed to s.
		bool parse_string_optional(std::istream& cin,std::string& s);
//! String-valued parameters
		void parse_string(std::istream& cin,const std::string& par,std::string& s);
//! Double-valued parameters
		void parse_double(std::istream& cin,const std::string& par,double& v);
//! Integer-valued parameters
		void parse_int(std::istream& cin,const std::string& par,int& v);
//! Bool-valued parameters: "true" or "false"
		void parse_bool(std::istream& cin,const std::string& par,bool& v);

	public:
		model_parser(bool verbose = true):_model(0),_verbose(verbose)
			{ }
		void set_model(tde4_ld_plugin* m)
			{ _model = m; }
//! @brief Read parameter-value pairs from stream and set in model.
//! Parameter names are as defined in the plugin. Parameters names must be
//! enclosed in quotation marks, if they contain white spaces.
//! If an error occurs, the method will throw an exception.
		void parse(std::istream& cin);
		};
	}

#endif
