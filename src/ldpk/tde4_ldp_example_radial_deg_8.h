// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011 - Science-D-Visions. Current version: 1.1


#ifndef tde4_ldp_example_radial_deg_8_sdv
#define tde4_ldp_example_radial_deg_8_sdv

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_example_radial_distortion.h>

//! @file tde4_ldp_example_radial_deg_8.h
//! @brief Example plugin class for radial distortion

#define LDPK_WITH_LUT_SDV

//! @brief The plugin class for the example distortion model.
//! We implement a symmetric model of degree 8.
class tde4_ldp_example_radial_deg_8:public ldpk::ldp_builtin<ldpk::vec2d>
	{
private:
	typedef ldpk::vec2d vec2_type;
	typedef ldpk::ldp_builtin<ldpk::vec2d> base_type;

//! Degree 8 distortion model
	ldpk::example_radial_distortion<8> _distortion;
//! Diagonal radius of filmback in cm. We need this in the example because
//! the distortion model is based on diagonally normalized coordinates.
	double _r_fb_cm;

//! Parameter names, degrees 2,4,6,8, that's a total of four.
	static const char* _para[4];

	bool decypher(const char* name,int& i)
		{
// Run through parameter names until name is found.
		int n;
		getNumParameters(n);
		for(i = 0;i < n;++i)
			{
			if(0 == strcmp(name,_para[i]))
				{
				return true;
				}
			}
		return false;
		}

//! @brief We have defined our example distortion model in terms of diagonally normalized coordinates,
//! around fixed point at (0,0). Therefore we need methods to map the plugin unit coordinates
//! with given lens center offset to normalized coordinates...
	vec2_type map_unit_to_dn(const vec2_type& p_unit) const
		{
		vec2_type p_cm((p_unit[0] - 1.0/2.0) * w_fb_cm() - x_lco_cm(),(p_unit[1] - 1.0/2.0) * h_fb_cm() - y_lco_cm());
		return p_cm / _r_fb_cm;
		}
//! ...and vice versa.
	vec2_type map_dn_to_unit(const vec2_type& p_dn) const
		{
		vec2_type p_cm(p_dn * _r_fb_cm);
		p_cm += vec2_type(w_fb_cm() / 2 + x_lco_cm(),h_fb_cm() / 2 + y_lco_cm());
		return vec2_type(p_cm[0] / w_fb_cm(),p_cm[1] / h_fb_cm());
		}
public:
	tde4_ldp_example_radial_deg_8()
		{ }
	bool getModelName(char *name)
		{
		strcpy(name,"Example Radial, Degree 8 [Plugin]");
		return true;
		}
//! The model has four parameters.
	bool getNumParameters(int& n)
		{
		n = 4;
		return true;
		}
//! Parameter names see tde4_ldp_example_radial_deg_8.C
	bool getParameterName(int i,char* identifier)
		{
		strcpy(identifier,_para[i]);
		return true;
		}
//! In our example all model-dependent parameters are adjustable-double.
	bool getParameterType(const char* identifier,tde4_ldp_ptype& ptype)
		{
		int i;
// One of the built-in parameters?
		if(base_type::get_builtin_parameter_type(identifier,ptype))
			{
			return true;
			}
		if(!decypher(identifier,i))
			{
			return false;
			}
		ptype = TDE4_LDP_ADJUSTABLE_DOUBLE;
		return true;
		}
//! All parameters of this model have a default-value of 0.0.
	bool getParameterDefaultValue(const char* identifier,double& v)
		{
		int i;
		if(!decypher(identifier,i))
			{
			return false;
			}
		v = 0.0;
		return true;
		}
//! @brief By mathematics, there are no limitations for the range in our example model.
//! These values have a certain impact however, if you use the Parameter Adjustment
//! in 3DE in order to calculate the model parameters.
	bool getParameterRange(const char* identifier,double& a,double& b)
		{
		int i;
		if(!decypher(identifier,i))
			{
			return false;
			}
		a = -0.5;
		b = 0.5;
		return true;
		}
	bool setParameterValue(const char *identifier,double v)
		{
		int i;
// One of the built-in parameters?
		if(base_type::set_builtin_parameter_value(identifier,v))
			{
			return true;
			}
		if(!decypher(identifier,i))
			{
			return false;
			}
#ifdef LDPK_WITH_LUT_SDV
		if(_distortion.get_coeff(i) != v)
			{
			no_longer_uptodate_lut();
			}
		_distortion.set_coeff(i,v);
#endif
		return true;
		}
	bool initializeParameters()
		{
// Calculated diagonal radius, needed for mapping to diagonally normalized coordinates.
		_r_fb_cm = sqrt(w_fb_cm() * w_fb_cm() + h_fb_cm() * h_fb_cm()) / 2.0;
		}
//! @brief We provide derivatives with respect to parameters,
//! in order to get the matrix tool in 3DE4 running.
	bool providesParameterDerivatives()
		{ return true; }
//! Calculate the derivatives
	bool calcParameterDerivatives(double x,double y,int num_parameters,double *g)
		{
		_distortion.derive(g,num_parameters,map_unit_to_dn(vec2_type(x,y)));
		return true;
		}
	bool undistort(double x0,double y0,double &x1,double &y1)
		{
		vec2_type q = map_dn_to_unit(_distortion(map_unit_to_dn(vec2_type(x0,y0))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	bool distort(double x0,double y0,double &x1,double &y1)
		{
#ifdef LDPK_WITH_LUT_SDV
// Update the lookup table if necessary, then get an initial value
// and call the warp function (map_inverse in the model) with this initial value.
		if(!is_uptodate_lut()) update_lut();
// Initial value
		vec2_type qs = get_lut().get_initial_value(vec2_type(x0,y0));
// Calculate distorted point position using initial value
		vec2_type q = map_dn_to_unit(_distortion.map_inverse(map_unit_to_dn(vec2_type(x0,y0)),map_unit_to_dn(qs)));
#else
// No lookup table support, gambatte!
		vec2_type q = map_dn_to_unit(_distortion.map_inverse(map_unit_to_dn(vec2_type(x0,y0))));
#endif
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	bool distort(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		vec2_type q = map_dn_to_unit(_distortion.map_inverse(map_unit_to_dn(vec2_type(x0,y0)),map_unit_to_dn(vec2_type(x1_start,y1_start))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	std::ostream& out(std::ostream& cout) const
		{ return _distortion.out(cout); }
	};

#endif
