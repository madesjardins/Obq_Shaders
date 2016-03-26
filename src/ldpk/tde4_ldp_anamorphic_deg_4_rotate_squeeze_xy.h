// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy_xy_sdv
#define tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy_xy_sdv

#include <ldpk/ldpk_ldp_builtin.h>
#include <ldpk/ldpk_generic_anamorphic_distortion.h>
#include <ldpk/ldpk_rotation_extender.h>
#include <ldpk/ldpk_squeeze_extender.h>
#include <ldpk/ldpk_linear_extender.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

//! @file tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy.h
//! Degree-4 anamorphic model with anamorphic lens rotation

template <class VEC2,class MAT2>
class tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy:public ldpk::ldp_builtin<VEC2,MAT2>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef ldpk::ldp_builtin<VEC2,MAT2> base_type;

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

	static const char* _para[13];

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
		_pa.set_sq(bt::pa());
// This method is the last one invoked, before the object can be used,
// therefore we have to prepare the concatenated extenders here.
		_rot_sqx_sqy_pa.set(_rotation,_squeeze_x,_squeeze_y,_pa);
		if(_squeeze_x.get_sq() == 0)
			{ AiMsgError("tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy::initializeParameters, error: Squeeze-X is 0."); }
		if(_squeeze_y.get_sq() == 0)
			{ AiMsgError("tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy::initializeParameters, error: Squeeze-Y is 0."); }
		_pa_rot.set(_pa,_rotation);
		_anamorphic.prepare();
		return true;
		}
	bool getNumParameters(int& n)
		{
		n = 13;
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
		if(i < 10)
			{
			if(_anamorphic.get_coeff(i) != v)
				{ bt::no_longer_uptodate_lut(); }
			_anamorphic.set_coeff(i,v);
			}
		else if(i == 10)
			{
			if(_rotation.get_phi() != v / 180.0 * M_PI)
				{ bt::no_longer_uptodate_lut(); }
			_rotation.set_phi(v / 180.0 * M_PI);
			}
		else if(i == 11)
			{
			if(_squeeze_x.get_sq() != v)
				{ bt::no_longer_uptodate_lut(); }
			_squeeze_x.set_sq(v);
			}
		else if(i == 12)
			{
			if(_squeeze_y.get_sq() != v)
				{ bt::no_longer_uptodate_lut(); }
			_squeeze_y.set_sq(v);
			}

		return true;
		}
// slightly faster version
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
/*
// original version
	virtual bool undistort(double x0,double y0,double &x1,double &y1)
		{
		vec2_type q =	bt::map_dn_to_unit(
					_rotation.eval(
						_squeeze_x.eval(
							_squeeze_y.eval(
								_pa.eval(
									_anamorphic.eval(
										_rotation.eval_inv(
											_pa.eval_inv(
												bt::map_unit_to_dn(vec2_type(x0,y0))))))))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
*/
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
//		vec2_type qs(x0,y0);
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
/*
// original version
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
//		vec2_type qs(x0,y0);
// Call version of distort with initial value.
		vec2_type q =	bt::map_dn_to_unit(
					_pa.eval(
						_rotation.eval(
							_anamorphic.map_inverse(
								_pa.eval_inv(
									_squeeze_y.eval_inv(
										_squeeze_x.eval_inv(
											_rotation.eval_inv(
												bt::map_unit_to_dn(vec2_type(x0,y0)))))),
								_pa.eval_inv(
									_squeeze_y.eval_inv(
										_squeeze_x.eval_inv(
											_rotation.eval_inv(
												bt::map_unit_to_dn(qs)))))))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
*/
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
/*
// original version
	virtual bool distort(double x0,double y0,double x1_start,double y1_start,double &x1,double &y1)
		{
		vec2_type q =	bt::map_dn_to_unit(
					_pa.eval(
						_rotation.eval(
							_anamorphic.map_inverse(
								_pa.eval_inv(
									_squeeze_y.eval_inv(
										_squeeze_x.eval_inv(
											_rotation.eval_inv(
												bt::map_unit_to_dn(vec2_type(x0,y0)))))),
								_pa.eval_inv(
									_squeeze_y.eval_inv(
										_squeeze_x.eval_inv(
											_rotation.eval_inv(
												bt::map_unit_to_dn(vec2_type(x1_start,y1_start))))))))));
		x1 = q[0];
		y1 = q[1];
		return true;
		}
*/
public:
// Mutex initialized and destroyed in baseclass.
	tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy()
		{ }
	~tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Anamorphic - Standard, Degree 4 [Plugin]");
#else
		strcpy(name,"3DE4 Anamorphic - Standard, Degree 4");
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
		if(i < 11)
			{
			v = 0.0;
			}
		else if((i == 11) || (i == 12))
			{
			v = 1.0;
			}
		return true;
		}
	bool getParameterRange(const char* identifier,double& a,double& b)
		{
		typedef base_type bt;
		int i;
		if(!decypher(identifier,i)) return false;
		if(i < 10)
			{
			a = -0.5;
			b = 0.5;
			}
		else if(i == 10)
			{
// Lens Rotation in degree.
			a = -2.0;
			b = +2.0;
			}
		else if((i == 11) || (i == 12))
			{
// Squeeze X/Y
			a = 0.9;
			b = 1.1;
			}
		else
			{
			AiMsgError("getParameterRange: i = %i, out of range",i);
			}
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

		bool setParameterValue2(const char *identifier,double v){return setParameterValue(identifier, v);}
		bool initializeParameters2(){return initializeParameters();}
		bool undistort2(double x0,double y0,double &x1,double &y1){return undistort(x0,y0,x1,y1);}
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy<VEC2,MAT2>::_para[13] = {
	"Cx02 - Degree 2","Cy02 - Degree 2",
	"Cx22 - Degree 2","Cy22 - Degree 2",

	"Cx04 - Degree 4","Cy04 - Degree 4",
	"Cx24 - Degree 4","Cy24 - Degree 4",
	"Cx44 - Degree 4","Cy44 - Degree 4",

	"Lens Rotation","Squeeze-X","Squeeze-Y"
	};

#endif
