// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef ldpk_error_sdv
#define ldpk_error_sdv

#include <exception>
#include <iostream>
#include <string.h>
#include <string>
#ifdef _WINDOWS
// Windows macros interfere with std library.
#define NOMINMAX
#include <windows.h>
#else
#include <pthread.h>
#endif

//! @file ldpk_error.h
//! @brief Exception classes for ldpk.

namespace ldpk
	{
//! Base class for errors
	class error_base:public std::exception
		{
	public:
		virtual const char* what() const throw() = 0;
		virtual std::ostream& out(std::ostream& cout) const
			{ return cout; }
		};
//! Some index was out of range.
	class error_index_out_of_range:public error_base
		{
	private:
		int _i,_a,_b;
	public:
		error_index_out_of_range(int i,int a,int b):_i(i),_a(a),_b(b)
			{ }
		~error_index_out_of_range() throw()
			{ }
		const char* what() const throw()
			{ return "ldpk::error: index out of range"; }
		std::ostream& out(std::ostream& cout) const
			{ return cout << _i << ", range [" << _a << "," << _b << "]"; }
		};
//! Unexpected end of stream. This can occur in ldpk::model_parser.
	class error_end_of_stream:public error_base
		{
	private:
	public:
		~error_end_of_stream() throw()
			{ }
		const char* what() const throw()
			{ return "ldpk::error: end of stream"; }
		};
//! Undefined parameter
	class error_undefined_parameter:public error_base
		{
	private:
// There's a general warning not to use strings in exceptions, which I ignore.
// The gain of clarity in error messages dominates the low risk of irregular termination.
		std::string _obj;
	public:
		error_undefined_parameter(const std::string& obj):_obj(obj)
			{ }
		~error_undefined_parameter() throw()
			{ }
		const char* what() const throw()
			{ return "ldpk::error: undefined parameter"; }
		std::ostream& out(std::ostream& cout) const
			{ return cout << _obj; }
		};
//! A parse error
	class error_could_not_read_value:public error_base
		{
	private:
		std::string _parname;
	public:
		error_could_not_read_value(const std::string& parname):_parname(parname)
			{ }
		~error_could_not_read_value() throw()
			{ }
		const char* what() const throw()
			{ return "ldpk::error: could not read value"; }
		std::ostream& out(std::ostream& cout) const
			{ return cout << "for parameter '" << _parname << "'"; }
		};
//! Another parse error
	class error_could_not_read_parameter_name:public error_base
		{
	private:
	public:
		~error_could_not_read_parameter_name() throw()
			{ }
		const char* what() const throw()
			{ return "ldpk::error: could not read parameter name"; }
		};
//! No model was set in one of the utility classes
	class error_no_model_specified:public error_base
		{
	private:
	public:
		~error_no_model_specified() throw()
			{ }
		const char* what() const throw()
			{ return "ldpk::error: no model specified"; }
		};
//! An error occurred while loading or linking the plugin
	class error_dynamic_link:public error_base
		{
	private:
		char _dlmsg[256];
	public:
// Windows complains about strncpy, using _CRT_SECURE_NO_WARNINGS
		error_dynamic_link(const char* dlmsg)
			{ strncpy(_dlmsg,dlmsg,256); }
		~error_dynamic_link() throw()
			{ }
		const char* what() const throw()
			{ return "ldpk::error: dynamic load/link"; }
		};
	}

#endif
