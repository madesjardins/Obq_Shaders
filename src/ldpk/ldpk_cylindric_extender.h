// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef ldpk_cylindric_extender_sdv
#define ldpk_cylindric_extender_sdv

#include <ldpk/ldpk_extender_base.h>

namespace ldpk
	{
	template <class VEC2,class MAT2>
	class cylindric_extender:public extender_base<VEC2,MAT2>
		{
	public:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;
	private:
		double _phi,_b;
		mat2_type _m,_inv_m;

		void calc_m()
			{
			mat2_type para = tensq(vec2_type(cos(_phi * M_PI / 180.0),sin(_phi * M_PI / 180.0)));
			_m = _b * para + mat2_type(1.0);
			_inv_m = invert(_m);
			}
	public:
		cylindric_extender()
			{
			_phi = 0.0;
			_b = 0.0;
			_m = mat2_type(1.0);
			_inv_m = mat2_type(1.0);
			}
//! The cylindric extender has a parameter called phi.
		void set_phi(double phi)
			{ _phi = phi;calc_m(); }
		double get_phi() const
			{ return _phi; }
//! This parameter expresses the strength of the cylindric deformation ("bending").
		void set_b(double b)
			{ _b = b;calc_m(); }
		double get_b() const
			{ return _b; }
//! eval() is per definition removal of lens distortion (undistort).
		vec2_type eval(const vec2_type& p) const
			{ return _m * p; }
//! eval_inv() is applying lens distortion (distort)
		vec2_type eval_inv(const vec2_type& q) const
			{ return _inv_m * q; }
//! Generally (but not here), an initial value is needed for calculating the inverse.
		vec2_type eval_inv(const vec2_type& q,const vec2_type& p_start) const
			{ return _inv_m * q; }
//! The matrix for this extender
		const mat2_type& get_mat() const
			{ return _m; }
//! The inverse matrix for this extender
		const mat2_type& get_mat_inv() const
			{ return _inv_m; }
		};

//! @brief "Symmetric" version. Scaling by sqrt(1+b) in para-direction and 1/sqrt(1+b) in ortho-direction.
//! The important thing is, that we have turned an almost-symmetry into a perfect symmetry:
//! phi -> phi + 90deg, b -> 1/b. The drawback is, that this models the curved beam splitter
//! only if focal length is corrected slightly, by a factor sqrt(1+b). Nevertheless we are using
//! this model in "3DE4 Radial - Standard, Degree 4".
	template <class VEC2,class MAT2>
	class cylindric_extender_2:public extender_base<VEC2,MAT2>
		{
	public:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;
	private:
		double _phi,_b;
		mat2_type _m,_inv_m;

		void calc_m()
			{
			double q = sqrt(1.0 + _b),c = cos(_phi * M_PI / 180.0),s = sin(_phi * M_PI / 180.0);
			_m = mat2_type(c*c*q + s*s/q,(q - 1.0/q)*c*s,(q - 1.0/q)*c*s,c*c/q + s*s*q);
			_inv_m = invert(_m);
			}
	public:
		cylindric_extender_2()
			{
			_phi = 0.0;
			_b = 0.0;
			_m = mat2_type(1.0);
			_inv_m = mat2_type(1.0);
			}
//! The cylindric extender has a parameter called phi.
		void set_phi(double phi)
			{ _phi = phi;calc_m(); }
		double get_phi() const
			{ return _phi; }
//! This parameter expresses the strength of the cylindric deformation ("bending").
		void set_b(double b)
			{ _b = b;calc_m(); }
		double get_b() const
			{ return _b; }
//! eval() is per definition removal of lens distortion (undistort).
		vec2_type eval(const vec2_type& p) const
			{ return _m * p; }
//! eval_inv() is applying lens distortion (distort)
		virtual vec2_type eval_inv(const vec2_type& q) const
			{ return _inv_m * q; }
//! Generally (but not here), an initial value is needed for calculating the inverse.
		vec2_type eval_inv(const vec2_type& q,const vec2_type& p_start) const
			{ return _inv_m * q; }
//! The matrix for this extender
		const mat2_type& get_mat() const
			{ return _m; }
//! The inverse matrix for this extender
		const mat2_type& get_mat_inv() const
			{ return _inv_m; }
		};
	}

#endif
