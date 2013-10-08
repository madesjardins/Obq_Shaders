// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.4


#ifndef tde4_ldp_common_sdv_sdv
#define tde4_ldp_common_sdv_sdv

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_generic_distortion_base.h>

//! @file tde4_ldp_common_sdv_8.h
//! @brief Common properties for all 3DE4-built-in models from SDV.

//! @brief Common properties for all 3DE4-built-in models from SDV.
//! All SDV models use diagonally normalized coordinates.
//! You probably won't need this class for your own models.
template <class VEC2,class MAT2,int N>
class tde4_ldp_common_sdv:public ldpk::ldp_builtin<VEC2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef ldpk::ldp_builtin<VEC2> base_type;

//! Built-ins use diag-norm coordinates.
	double _r_fb_cm;

//! Built-ins have a const-char*-array for parameter names.
	virtual const char** get_para_names() const = 0;
//! Built-ins communicate with distortion base class.
	virtual ldpk::generic_distortion_base<vec2_type,mat2_type,N>& get_distortion_base() = 0;

protected:
	pthread_mutex_t _mutex;

	tde4_ldp_common_sdv()
		{
		int r = pthread_mutex_init(&_mutex,NULL);
		if(r) std::cerr << "tde4_ldp_common_sdv::pthread_mutex_init: " << strerror(r) << std::endl;
		}
	~tde4_ldp_common_sdv()
		{
		int r = pthread_mutex_destroy(&_mutex);
		if(r) std::cerr << "tde4_ldp_common_sdv::pthread_mutex_destroy: " << strerror(r) << std::endl;
		}
	double r_fb_cm() const
		{ return _r_fb_cm; }
	bool decypher(const char* name,int& i)
		{
		typedef base_type bt;
		int n;
		getNumParameters(n);
		const char** para = get_para_names();
		for(i = 0;i < n;++i)
			{
			if(0 == strcmp(name,para[i]))
				{
				return true;
				}
			}
		return false;
		}
	vec2_type map_unit_to_dn(const vec2_type& p_unit) const
		{
		typedef base_type bt;
		vec2_type p_cm((p_unit[0] - 1.0/2.0) * bt::w_fb_cm() - bt::x_lco_cm(),(p_unit[1] - 1.0/2.0) * bt::h_fb_cm() - bt::y_lco_cm());
		return p_cm / _r_fb_cm;
		}
	vec2_type map_dn_to_unit(const vec2_type& p_dn) const
		{
		typedef base_type bt;
		vec2_type p_cm(p_dn * _r_fb_cm);
		p_cm += vec2_type(bt::w_fb_cm() / 2 + bt::x_lco_cm(),bt::h_fb_cm() / 2 + bt::y_lco_cm());
		return vec2_type(p_cm[0] / bt::w_fb_cm(),p_cm[1] / bt::h_fb_cm());
		}
	bool initializeParameters()
		{
		typedef base_type bt;
		_r_fb_cm = sqrt(bt::w_fb_cm() * bt::w_fb_cm() + bt::h_fb_cm() * bt::h_fb_cm()) / 2.0;
		return true;
		}
	bool getNumParameters(int& n)
		{
		n = N;
		return true;
		}
	bool getParameterName(int i,char* identifier)
		{
		const char** para = get_para_names();
		strcpy(identifier,para[i]);
		return true;
		}
	bool setParameterValue(const char *identifier,double v)
		{
		typedef base_type bt;
		int i;
// Does the base class know the parameter?
		if(bt::set_builtin_parameter_value(identifier,v))
			{
			return true;
			}
		if(!decypher(identifier,i))
			{
			return false;
			}
		if(get_distortion_base().get_coeff(i) != v)
			{
			bt::no_longer_uptodate_lut();
			}
		get_distortion_base().set_coeff(i,v);
		return true;
		}
	virtual bool undistort(double x0,double y0,double &x1,double &y1)
		{
		vec2_type q = map_dn_to_unit(get_distortion_base().eval(map_unit_to_dn(vec2_type(x0,y0))));
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
			pthread_mutex_lock(&_mutex);
			if(!bt::is_uptodate_lut())
				{
				bt::update_lut();
				}
			pthread_mutex_unlock(&_mutex);
			}

// Get initial value from lookup-table
		vec2_type qs = bt::get_lut().get_initial_value(vec2_type(x0,y0));
// Call version of distort with initial value.
		vec2_type q = map_dn_to_unit(get_distortion_base().map_inverse(map_unit_to_dn(vec2_type(x0,y0)),map_unit_to_dn(qs)));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	virtual bool distort(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		vec2_type q = map_dn_to_unit(get_distortion_base().map_inverse(map_unit_to_dn(vec2_type(x0,y0)),map_unit_to_dn(vec2_type(x1_start,y1_start))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
//! If derivatives are provided, they are calc'd like this. The derived class implements providesDerivatives().
	bool calcParameterDerivatives(double x,double y,int num_parameters,double *dg)
		{
		get_distortion_base().derive(dg,num_parameters,map_unit_to_dn(vec2_type(x,y)));
		return true;
		}
	std::ostream& out(std::ostream& cout) const
		{ return get_distortion_base().out(cout); }
	};


#endif
