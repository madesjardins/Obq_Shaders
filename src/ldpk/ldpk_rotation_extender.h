// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.7


#ifndef ldpk_rotation_extender_sdv
#define ldpk_rotation_extender_sdv

#include <ldpk/ldpk_extender_base.h>

namespace ldpk
	{
//! @brief The rotation extender simply rotates the incident ray
//! around the optical axis. We need this e.g. for the new-style
//! squeeze-rotation anamorphic distortion model.
	template <class VEC2,class MAT2>
	class rotation_extender:public extender_base<VEC2,MAT2>
		{
	public:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;
	private:
		double _phi;
		mat2_type _m_rot,_inv_m_rot;

	public:
		rotation_extender()
			{
			_phi = 0.0;
			_m_rot = mat2_type(1.0);
			_inv_m_rot = mat2_type(1.0);
			}
//! The rotation extender has one parameter called phi (in radians).
		void set_phi(double phi)
			{
			_phi = phi;
			_m_rot = mat2_type(cos(_phi),-sin(_phi),sin(_phi),cos(_phi));
			_inv_m_rot = trans(_m_rot);
			}
//! Getter
		double get_phi() const
			{ return _phi; }

//! eval() is per definition removal of lens distortion (undistort).
		vec2_type eval(const vec2_type& p) const
			{ return _m_rot * p; }
//! eval_inv() is applying lens distortion (distort)
		vec2_type eval_inv(const vec2_type& q) const
			{ return _inv_m_rot * q; }
//! Generally (but not here), an initial value is needed for calculating the inverse.
		vec2_type eval_inv(const vec2_type& q,const vec2_type& p_start) const
			{ return _inv_m_rot * q; }
//! The matrix for this extender
		const mat2_type& get_mat() const
			{ return _m_rot; }
//! The inverse matrix for this extender
		const mat2_type& get_mat_inv() const
			{ return _inv_m_rot; }
		};
	}

#endif
