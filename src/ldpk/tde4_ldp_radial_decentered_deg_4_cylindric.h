// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef tde4_ldp_radial_decentered_deg_4_cylindric_sdv
#define tde4_ldp_radial_decentered_deg_4_cylindric_sdv

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_radial_decentered_distortion.h>
#include <ldpk/ldpk_cylindric_extender.h>

//! @file tde4_ldp_radial_decentered_deg_4_cylindric.h
//! @brief Plugin class for radial distortion with decentering
//! and optional compensation for beam-splitter artefacts.

//! @brief Plugin class for radial distortion with decentering
//! and optional compensation for beam-splitter artefacts.
//! Parameters can be calculated by 3DE's Matrix Tool.
template <class VEC2,class MAT2>
class tde4_ldp_radial_decentered_deg_4_cylindric:public ldpk::ldp_builtin<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef ldpk::ldp_builtin<VEC2,MAT2> base_type;

	ldpk::radial_decentered_distortion<vec2_type,mat2_type> _radial;
	ldpk::cylindric_extender_2<vec2_type,mat2_type> _cylindric;

	static const char* _para[8];

	bool decypher(const char* name,int& i)
		{
		typedef base_type bt;
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
	bool initializeParameters()
		{
		typedef base_type bt;
		bt::check_builtin_parameters();
		return true;
		}
	bool getNumParameters(int& n)
		{
		n = 8;
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
// Does the base class know the parameter?
		if(bt::set_builtin_parameter_value(identifier,v))
			{
			return true;
			}
		if(!decypher(identifier,i))
			{
			return false;
			}
		if(i < 6)
			{
			if(_radial.get_coeff(i) != v)
				{ bt::no_longer_uptodate_lut(); }
			_radial.set_coeff(i,v);
			}
		else if(i == 6)
			{
			if(_cylindric.get_phi() != v)
				{ bt::no_longer_uptodate_lut(); }
			_cylindric.set_phi(v);
			}
		else if(i == 7)
			{
			if(_cylindric.get_b() != v)
				{ bt::no_longer_uptodate_lut(); }
			_cylindric.set_b(v);
			}
		return true;
		}
	virtual bool undistort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type q =	bt::map_dn_to_unit(
					_cylindric.eval(
						_radial.eval(
							bt::map_unit_to_dn(vec2_type(x0,y0)))));
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
					_radial.map_inverse(
						_cylindric.eval_inv(
							bt::map_unit_to_dn(vec2_type(x0,y0))),
						_cylindric.eval_inv(
							bt::map_unit_to_dn(qs))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	virtual bool distort(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type q =	bt::map_dn_to_unit(
					_radial.map_inverse(
						_cylindric.eval_inv(
							bt::map_unit_to_dn(vec2_type(x0,y0))),
						_cylindric.eval_inv(
							bt::map_unit_to_dn(vec2_type(x1_start,y1_start)))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
public:
// Mutex initialized and destroyed in baseclass.
	tde4_ldp_radial_decentered_deg_4_cylindric()
		{ }
	~tde4_ldp_radial_decentered_deg_4_cylindric()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Radial - Standard, Degree 4 [Plugin]");
#else
		strcpy(name,"3DE4 Radial - Standard, Degree 4");
#endif
		return true;
		}
	bool getParameterType(const char* identifier,tde4_ldp_ptype& ptype)
		{
		typedef base_type bt;
		int i;
		if(bt::get_builtin_parameter_type(identifier,ptype)) return true;
		if(!decypher(identifier,i)) return false;
		ptype = TDE4_LDP_ADJUSTABLE_DOUBLE;
		return true;
		}
	bool getParameterDefaultValue(const char* identifier,double& v)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		v = 0.0;
		return true;
		}
	bool getParameterRange(const char* identifier,double& a,double& b)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		if((i == 0) || (i == 3))
			{
// Distortion - Degree 2, Quartic Distortion - Degree 4
			a = -0.5;b = 0.5;
			}
		else if((i == 1) || (i == 2) || (i == 4) || (i == 5))
			{
// U2,V2,U4,V4.
			a = -0.5;b = 0.5;
			}
		else if(i == 6)
			{
// Phi - Cylindric Direction. In the symmetric extender version -45 to 45 would be sufficient.
			a = -90.0;b = +90.0;
			}
		else if(i == 7)
			{
// B - Cylindric Bending
			a = -0.1;b = 0.1;
			}
		return true;
		}
	bool getJacobianMatrix(double x0,double y0,double& m00,double& m01,double& m10,double& m11)
		{
		typedef base_type bt;
		mat2_type m = _cylindric.get_mat()
					* _radial.jacobi(
						bt::map_unit_to_dn(vec2_type(x0,y0)));
// to myself: Eigentlich w/2,h/2 bei beiden. Kuerzt sich weg.
		mat2_type u2d(bt::w_fb_cm() / bt::r_fb_cm(),0.0,0.0,bt::h_fb_cm() / bt::r_fb_cm());
		mat2_type d2u(bt::r_fb_cm() / bt::w_fb_cm(),0.0,0.0,bt::r_fb_cm() / bt::h_fb_cm());
		m = d2u * m * u2d;
		m00 = m[0][0];m01 = m[0][1];m10 = m[1][0];m11 = m[1][1];
		return true;
		}
		
	bool setParameterValue2(const char *identifier,double v){return setParameterValue(identifier, v);}
	bool initializeParameters2(){return initializeParameters();}
	bool undistort2(double x0,double y0,double &x1,double &y1){return undistort(x0,y0,x1,y1);}
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_radial_decentered_deg_4_cylindric<VEC2,MAT2>::_para[8] = {
	"Distortion - Degree 2",
	"U - Degree 2",
	"V - Degree 2",
	"Quartic Distortion - Degree 4",
	"U - Degree 4",
	"V - Degree 4",
	"Phi - Cylindric Direction",
	"B - Cylindric Bending"
	};

#endif
