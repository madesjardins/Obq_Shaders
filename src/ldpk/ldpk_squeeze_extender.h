// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef ldpk_squeeze_xy_extender_sdv
#define ldpk_squeeze_xy_extender_sdv

#include <ldpk/ldpk_extender_base.h>

namespace ldpk
	{
//! @brief The squeeze-x/y-extender scales the optical-axis-perpendicular coordinates
//! of the incident ray towards the optical axis, either in x or in y direction.
	template <class VEC2,class MAT2>
	class squeeze_x_extender:public extender_base<VEC2,MAT2>
		{
	public:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;
	private:
		double _sq;
		mat2_type _m_sq,_inv_m_sq;

	public:
		squeeze_x_extender()
			{
			_sq = 1.0;
			_m_sq = mat2_type(1.0);
			_inv_m_sq = mat2_type(1.0);
			}
// The squeeze extender has one parameter called sq.
		void set_sq(double sq)
			{
			_sq = sq;
			_m_sq = mat2_type(_sq,0.0,0.0,1.0);
			_inv_m_sq = mat2_type(1.0 / _sq,0.0,0.0,1.0);
			}
		double get_sq() const
			{ return _sq; }

//! eval() is per definition removal of lens distortion (undistort).
		vec2_type eval(const vec2_type& p) const
			{ return _m_sq * p; }
//! eval_inv() is applying lens distortion (distort)
		vec2_type eval_inv(const vec2_type& q) const
			{ return _inv_m_sq * q; }
//! Generally (but not here), an initial value is needed for calculating the inverse.
		vec2_type eval_inv(const vec2_type& q,const vec2_type& p_start) const
			{ return _inv_m_sq * q; }
//! The matrix for this extender
		const mat2_type& get_mat() const
			{ return _m_sq; }
//! The inverse matrix for this extender
		const mat2_type& get_mat_inv() const
			{ return _inv_m_sq; }
		};
	template <class VEC2,class MAT2>
	class squeeze_y_extender:public extender_base<VEC2,MAT2>
		{
	public:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;
	private:
		double _sq;
		mat2_type _m_sq,_inv_m_sq;

	public:
		squeeze_y_extender()
			{
			_sq = 1.0;
			_m_sq = mat2_type(1.0);
			_inv_m_sq = mat2_type(1.0);
			}
// The squeeze extender has one parameter called sq.
		void set_sq(double sq)
			{
			_sq = sq;
			_m_sq = mat2_type(1.0,0.0,0.0,_sq);
			_inv_m_sq = mat2_type(1.0,0.0,0.0,1.0 / _sq);
			}
		double get_sq() const
			{ return _sq; }

//! eval() is per definition removal of lens distortion (undistort).
		vec2_type eval(const vec2_type& p) const
			{ return _m_sq * p; }
//! eval_inv() is applying lens distortion (distort)
		vec2_type eval_inv(const vec2_type& q) const
			{ return _inv_m_sq * q; }
//! Generally (but not here), an initial value is needed for calculating the inverse.
		vec2_type eval_inv(const vec2_type& q,const vec2_type& p_start) const
			{ return _inv_m_sq * q; }
//! The matrix for this extender
		const mat2_type& get_mat() const
			{ return _m_sq; }
//! The inverse matrix for this extender
		const mat2_type& get_mat_inv() const
			{ return _inv_m_sq; }
		};
	}

#endif
