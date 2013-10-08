// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.4


#pragma once

#include <ldpk/ldpk_extender_base.h>

namespace ldpk
	{
//! A general linear extender, based on a 2x2-matrix.
	template <class VEC2,class MAT2>
	class linear_extender:public extender_base<VEC2,MAT2>
		{
	public:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;
	private:
		mat2_type _m,_inv_m;
	public:
//! Default: unit matrix
		linear_extender()
			{
			_m = mat2_type(1.0);
			_inv_m = mat2_type(1.0);
			}
//! Passing the matrix
		void set(const mat2_type& m)
			{
			_m = m;
			_inv_m = invert(_m);
			} 
//! Building the matrix from two extenders. Order is m(E0) * m(E1).
		template <class E0,class E1>
		void set(const E0& e0,const E1& e1)
			{
			_m = e0.get_mat() * e1.get_mat();
			_inv_m = invert(_m);
			}
//! Building the matrix from three extenders. Order is m(E0) * m(E1) * m(E2).
		template <class E0,class E1,class E2>
		void set(const E0& e0,const E1& e1,const E2& e2)
			{
			_m = e0.get_mat() * e1.get_mat() * e2.get_mat();
			_inv_m = invert(_m);
			}
//! Building the matrix from four extenders. Order is m(E0) * m(E1) * m(E2) * m(E3).
		template <class E0,class E1,class E2,class E3>
		void set(const E0& e0,const E1& e1,const E2& e2,const E3& e3)
			{
			_m = e0.get_mat() * e1.get_mat() * e2.get_mat() * e3.get_mat();
			_inv_m = invert(_m);
			}
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
	}
