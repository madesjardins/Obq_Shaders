// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef tde4_ldp_all_par_types_sdv
#define tde4_ldp_all_par_types_sdv

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_generic_anamorphic_distortion.h>
#include <ldpk/ldpk_rotation_extender.h>
#include <ldpk/ldpk_squeeze_extender.h>
#include <ldpk/ldpk_linear_extender.h>
#include <fstream>

//! @file tde4_ldp_all_par_types.h
//! @brief Example plugin class containing one parameter of each type.

//! @brief Example plugin class containing one parameter of each type.
//! The main purpose of this model is to test the various parameter types.
//! It is based on a degree-4-anamorphic model and allows to control
//! the lower order coefficients and squeeze parameters.
template <class VEC2,class MAT2>
class tde4_ldp_all_par_types:public ldpk::ldp_builtin<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef ldpk::ldp_builtin<VEC2,MAT2> base_type;

	static const char* _para[9];

	bool _debugging_enabled;
	std::string _debugging_filename;
	std::ofstream _debugging_fout;
	int _dummy;

// Wir kombinieren hier anamorphische Verzeichung Grad 4
// mit einer Rotation und einem Squeeze. Das ist der anamorphe Anteil
	ldpk::generic_anamorphic_distortion<vec2_type,mat2_type,4> _anamorphic;
	ldpk::rotation_extender<vec2_type,mat2_type> _rotation;
	ldpk::squeeze_x_extender<vec2_type,mat2_type> _squeeze_x;
	ldpk::squeeze_y_extender<vec2_type,mat2_type> _squeeze_y;
	ldpk::squeeze_x_extender<vec2_type,mat2_type> _pa;

//! Concatenating extenders for better performance
	ldpk::linear_extender<vec2_type,mat2_type> _rot_sqx_sqy_pa;
	ldpk::linear_extender<vec2_type,mat2_type> _pa_rot;

	bool decypher(const char* name,int& i)
		{
		typedef base_type bt;
		int n;
		getNumParameters(n);
		for(i = 0;i < n;++i)
			{
			if(0 == strcmp(name,_para[i]))
				{ return true; }
			}
		return false;
		}
	bool initializeParameters()
		{
		typedef base_type bt;
		bt::check_builtin_parameters();
		_pa.set_sq(bt::pa());
// This method is the last one invoked, before the object can be used,
// therefore we have to prepare the concatenated extenders here.
		_rot_sqx_sqy_pa.set(_rotation,_squeeze_x,_squeeze_y,_pa);
		if(_squeeze_x.get_sq() == 0)
        {
                // LDPK:
                // std::cerr << "tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy::initializeParameters, error: Squeeze-X is 0." << std::endl;
                // OBQ:
                AiMsgError("tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy::initializeParameters, error: Squeeze-X is 0.");
        }
		if(_squeeze_y.get_sq() == 0)
        {
                // LDPK:
                // std::cerr << "tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy::initializeParameters, error: Squeeze-Y is 0." << std::endl;
                AiMsgError("tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy::initializeParameters, error: Squeeze-Y is 0.");
        }
		_pa_rot.set(_pa,_rotation);
		_anamorphic.prepare();
		if(_debugging_enabled && _debugging_fout.is_open())
			{
			_debugging_fout << "initializeParameters()" << std::endl;
			}
		return true;
		}
	bool getNumParameters(int& n)
		{
		n = 9;
		return true;
		}
	bool getParameterName(int i,char* identifier)
		{
		strcpy(identifier,_para[i]);
		return true;
		}
	bool setParameterValue(const char *identifier,double v)
		{
		typedef base_type bt;
		int i;
		if(_debugging_enabled && _debugging_fout.is_open())
			{ _debugging_fout << "setParameterValue(\"" << identifier << "\"," << v << ")" << std::endl; }
// Does the base class know the parameter?
		if(bt::set_builtin_parameter_value(identifier,v))
			{ return true; }
		if(!decypher(identifier,i))
			{ return false; }
		switch(i)
			{
// adjustable double and double parameters
			case 0:
			case 1:
			case 2:
			case 3:
				{
// parameters 0, 1, 2 and 3 in anamorphic.
				if(_anamorphic.get_coeff(i) != v)
					{ bt::no_longer_uptodate_lut(); }
				_anamorphic.set_coeff(i,v);
				return true;
				}
			}
		return false;
		}
	bool setParameterValue(const char *identifier,const char* t)
		{
		typedef base_type bt;
		int i;
		if(_debugging_enabled && _debugging_fout.is_open())
			{ _debugging_fout << "setParameterValue(\"" << identifier << "\",\"" << t << "\")" << std::endl; }
// Does the base class know the parameter?
		if(!decypher(identifier,i))
			{ return false; }
		switch(i)
			{
// string parameters
			case 4:
				{
				double v;
				sscanf(t,"%lf",&v);
				if(_squeeze_x.get_sq() != v)
					{ bt::no_longer_uptodate_lut(); }
				_squeeze_x.set_sq(v);
				return true;
				}
			case 5:
				{
				double v;
				sscanf(t,"%lf",&v);
				if(_squeeze_y.get_sq() != v)
					{ bt::no_longer_uptodate_lut(); }
				_squeeze_y.set_sq(v);
				return true;
				}
// file parameter
			case 6:
				{
				if(_debugging_filename != t)
					{
					_debugging_fout.close();
					_debugging_filename = t;
					if(t != "")
						{
// We cannot do this here.
//						std::cout << "open " << _debugging_filename << std::endl;
//						_debugging_fout.open(_debugging_filename.c_str());
						}
					}
				return true;
				}
			}
		return false;
		}
	bool setParameterValue(const char *identifier,bool v)
		{
		typedef base_type bt;
		int i;
		if(_debugging_enabled && _debugging_fout.is_open())
			{ _debugging_fout << "setParameterValue(\"" << identifier << "\"," << v << ")" << std::endl; }
// Does the base class know the parameter?
		if(!decypher(identifier,i))
			{ return false; }
		switch(i)
			{
// bool parameter
			case 7:
				{
				_debugging_enabled = v;
				return true;
				}
			}
		return false;
		}
	bool setParameterValue(const char *identifier,int v)
		{
		typedef base_type bt;
		int i;
		if(_debugging_enabled && _debugging_fout.is_open())
			{ _debugging_fout << "setParameterValue(\"" << identifier << "\"," << v << ")" << std::endl; }
// Does the base class know the parameter?
		if(!decypher(identifier,i))
			{ return false; }
		switch(i)
			{
// int parameter
			case 8:
				{
				_dummy = v;
				return true;
				}
			}
		return false;
		}
	virtual bool undistort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type q =	bt::map_dn_to_unit(
					_rot_sqx_sqy_pa.eval(
						_anamorphic.eval(
							_pa_rot.eval_inv(
								bt::map_unit_to_dn(vec2_type(x0,y0))))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	virtual bool distort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
// The distort-method without initial values is not constant by semantics,
// since it may cause an update of the lookup-tables. Implementing a Nuke node
// it turned out that we need to prevent threads from trying so simultaneously.
// By the following double check of is_uptodate_lut() we keep the mutex lock
// out of our frequently called distort stuff (for performance reasons) and
// prevent threads from updating without need.
		if(!bt::is_uptodate_lut())
			{
			bt::lock();
			if(!bt::is_uptodate_lut())
				{
				bt::update_lut();
				}
			bt::unlock();
			}
// Get initial value from lookup-table
		vec2_type qs = bt::get_lut().get_initial_value(vec2_type(x0,y0));
// Call version of distort with initial value.
		vec2_type q =	bt::map_dn_to_unit(
					_pa_rot.eval(
						_anamorphic.map_inverse(
							_rot_sqx_sqy_pa.eval_inv(
								bt::map_unit_to_dn(vec2_type(x0,y0))),
							_rot_sqx_sqy_pa.eval_inv(
								bt::map_unit_to_dn(qs)))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	virtual bool distort(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type q =	bt::map_dn_to_unit(
					_pa_rot.eval(
						_anamorphic.map_inverse(
							_rot_sqx_sqy_pa.eval_inv(
								bt::map_unit_to_dn(vec2_type(x0,y0))),
							_rot_sqx_sqy_pa.eval_inv(
								bt::map_unit_to_dn(vec2_type(x1_start,y1_start))))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
public:
// Mutex initialized and destroyed in baseclass.
	tde4_ldp_all_par_types():_debugging_enabled(false)
		{ }
	~tde4_ldp_all_par_types()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 All Parameter Types [Plugin]");
#else
		strcpy(name,"3DE4 All Parameter Types");
#endif
		return true;
		}
	bool getParameterType(const char* identifier,tde4_ldp_ptype& ptype)
		{
		typedef base_type bt;
		int i;
		if(bt::get_builtin_parameter_type(identifier,ptype)) return true;
		if(!decypher(identifier,i)) return false;
		switch(i)
			{
			case 0:
			case 1:
				ptype = TDE4_LDP_ADJUSTABLE_DOUBLE;break;
			case 2:
			case 3:
				ptype = TDE4_LDP_DOUBLE;break;
			case 4:
			case 5:
				ptype = TDE4_LDP_STRING;break;
			case 6:
				ptype = TDE4_LDP_FILE;break;
			case 7:
				ptype = TDE4_LDP_TOGGLE;break;
			case 8:
				ptype = TDE4_LDP_INT;break;
			}
		return true;
		}
	bool getParameterDefaultValue(const char* identifier,double& v)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		switch(i)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				{
				v = 0.0;
				return true;
				}
			}
		return false;
		}
	bool getParameterDefaultValue(const char* identifier,char* v)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		switch(i)
			{
			case 4:
			case 5:
				{
				strcpy(v,"1.0");
				return true;
				}
			case 6:
				{
				strcpy(v,"");
				return true;
				}
			}
		return false;
		}
	bool getParameterDefaultValue(const char* identifier,bool& v)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		switch(i)
			{
			case 7:
				{
				v = false;
				return true;
				}
			}
		return false;
		}
	bool getParameterDefaultValue(const char* identifier,int& v)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		switch(i)
			{
			case 8:
				{
				v = -1;
				return true;
				}
			}
		return false;
		}
	bool getParameterRange(const char* identifier,double& a,double& b)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		a = -0.5;
		b = 0.5;
		return true;
		}
//! Tested against difference quotients.
	bool getJacobianMatrix(double x0,double y0,double& m00,double& m01,double& m10,double& m11)
		{
		typedef base_type bt;
		mat2_type m = _rot_sqx_sqy_pa.get_mat()
					* _anamorphic.jacobi(
						_pa_rot.eval_inv(
							bt::map_unit_to_dn(vec2_type(x0,y0))))
						* _pa_rot.get_mat_inv();
		mat2_type u2d(bt::w_fb_cm() / bt::r_fb_cm(),0.0,0.0,bt::h_fb_cm() / bt::r_fb_cm());
		mat2_type d2u(bt::r_fb_cm() / bt::w_fb_cm(),0.0,0.0,bt::r_fb_cm() / bt::h_fb_cm());
		m = d2u * m * u2d;
		m00 = m[0][0];m01 = m[0][1];m10 = m[1][0];m11 = m[1][1];
		return true;
		}
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_all_par_types<VEC2,MAT2>::_para[9] = {
	"Par - Adjustable Double [Cx02]",
	"Par - Adjustable Double [Cy02]",
	"Par - Double [Cx22]",
	"Par - Double [Cy22]",
	"Par - String [Squeeze-X]",
	"Par - String [Squeeze-Y]",
	"Par - File [Debug Output]",
	"Par - Toggle [Debugging On/Off]",
	"Par - Int [Dunno]",
	};

#endif
