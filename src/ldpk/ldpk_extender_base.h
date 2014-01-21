// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.7


#ifndef ldpk_extender_base_sdv
#define ldpk_extender_base_sdv

#include <functional>

namespace ldpk
	{
//! @brief Base class of all extenders
//! The concept of extenders as turned out to be useful in the new-style
//! distortion models which compensate for beam-splitter effects or rotated
//! anamorphic lenses. Essentially, an extender maps from vec2_type to vec2_type.
//! It also provides the inverse mapping. Up to now, all extenders have a simple
//! representation as a 2x2-matrix (i.e. they are linear maps),
//! but it might be necessary in the future to extend this to affine transformations.
	template <class VEC2,class MAT2>
	class extender_base
		{
	public:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;

		virtual ~extender_base()
			{ }
//! eval() is by definition removal of lens distortion (undistort).
		virtual vec2_type eval(const vec2_type& p) const = 0;
//! eval_inv() is applying lens distortion (distort)
		virtual vec2_type eval_inv(const vec2_type& q) const = 0;
//! Generally (but not here), an initial value is needed for calculating the inverse.
		virtual vec2_type eval_inv(const vec2_type& q,const vec2_type& p_start) const = 0;
//! The matrix for this extender.
		virtual const mat2_type& get_mat() const = 0;
//! The inverse matrix for this extender
		virtual const mat2_type& get_mat_inv() const = 0;
		};
	}

#endif
