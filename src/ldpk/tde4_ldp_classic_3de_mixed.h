// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef tde4_ldp_classic_3de_mixed_sdv
#define tde4_ldp_classic_3de_mixed_sdv

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_classic_3de_mixed_distortion.h>

//! @file tde4_ldp_classic_3de_mixed.h
//! @brief Degree-2 anamorphic and degree-4 radial mixed model.

//! @brief Degree-2 anamorphic and degree-4 radial mixed model.
//! Does not compensate for decentering.
//! Parameters can be calculated by 3DE's Matrix Tool.
template <class VEC2,class MAT2>
class tde4_ldp_classic_3de_mixed:public ldpk::ldp_builtin<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef ldpk::ldp_builtin<VEC2,MAT2> base_type;

	ldpk::classic_3de_mixed_distortion<vec2_type,mat2_type> _distortion;

	static const char* _para[5];

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
		n = 5;
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
		if(_distortion.get_coeff(i) != v)
			{
			bt::no_longer_uptodate_lut();
			}
		_distortion.set_coeff(i,v);
		return true;
		}
	virtual bool undistort(double x0,double y0,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type q = bt::map_dn_to_unit(_distortion.eval(bt::map_unit_to_dn(vec2_type(x0,y0))));
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
		vec2_type q = bt::map_dn_to_unit(_distortion.map_inverse(bt::map_unit_to_dn(vec2_type(x0,y0)),bt::map_unit_to_dn(qs)));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
	virtual bool distort(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		typedef base_type bt;
		vec2_type q = bt::map_dn_to_unit(_distortion.map_inverse(bt::map_unit_to_dn(vec2_type(x0,y0)),bt::map_unit_to_dn(vec2_type(x1_start,y1_start))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
public:
// Mutex initialized and destroyed in baseclass.
	tde4_ldp_classic_3de_mixed()
		{ }
	~tde4_ldp_classic_3de_mixed()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE Classic LD Model [Plugin]");
#else
		strcpy(name,"3DE Classic LD Model");
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
// Alle Parameter haben als Default-Wert 0, ausser Squeeze
		v = (i == 1) ? 1.0 : 0.0;
		return true;
		}
	bool getParameterRange(const char* identifier,double& a,double& b)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
// squeeze
		if(i == 1)
			{
			a = 1.0 / 4.0;
			b = 4.0;
			}
		else
			{
			a = -0.5;
			b =  0.5;
			};
		return true;
		}
//! Tested against difference quotients.
	bool getJacobianMatrix(double x0,double y0,double& m00,double& m01,double& m10,double& m11)
		{
		typedef base_type bt;
		mat2_type m = _distortion.jacobi(bt::map_unit_to_dn(vec2_type(x0,y0)));
		mat2_type u2d(bt::w_fb_cm() / bt::r_fb_cm(),0.0,0.0,bt::h_fb_cm() / bt::r_fb_cm());
		mat2_type d2u(bt::r_fb_cm() / bt::w_fb_cm(),0.0,0.0,bt::r_fb_cm() / bt::h_fb_cm());
		m = d2u * m * u2d;
		m00 = m[0][0];m01 = m[0][1];m10 = m[1][0];m11 = m[1][1];
		return true;
		}

	///////////// OBQ_ADD ////////////////
	bool setParameterValue2(const char *identifier,double v)
	{
		return setParameterValue(identifier, v);
	}

	bool initializeParameters2()
	{
		return initializeParameters();
	}

	bool undistort2(double x0,double y0,double &x1,double &y1)
	{
		return undistort(x0,y0,x1,y1);
	}
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_classic_3de_mixed<VEC2,MAT2>::_para[5] = {
	"Distortion",
	"Anamorphic Squeeze",
	"Curvature X",
	"Curvature Y",
	"Quartic Distortion"
	};

#endif
