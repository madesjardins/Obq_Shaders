// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011 - Science-D-Visions. Current version: 1.1


#ifndef ldpk_example_radial_distortion_sdv
#define ldpk_example_radial_distortion_sdv

//! @file ldpk_example_radial_distortion.h
//! @brief A polynomial radially symmetric model of degree N (even)

#include "ldpk/ldpk_generic_distortion_base.h"
#include "ldpk/ldpk_vec2d.h"
#include <iostream>

namespace ldpk
	{
//! @brief A polynomial model with N coefficients for radially symmetric lenses.
//! This class contains the model for normalized coordinates with diagonal radius = 1.
//! Lens center is at (0,0), because we take it into account in the plugin class.
//! We use our utility vec2d/mat2d classes for this example.
	template <int N>
	class example_radial_distortion:public generic_distortion_base<vec2d,mat2d,N>
		{
	public:
		typedef generic_distortion_base<vec2d,mat2d,N> base_type;
		typedef vec2d vec2_type;
	private:
// The coefficients
		double _c[N];
	public:
		example_radial_distortion()
			{
			for(int i = 0;i < N;++i) _c[i] = 0.0;
			}
//! Get coefficient as demanded by base class
		double get_coeff(int i) const
			{ return c(i); }
//! Set coefficient as demanded by base class
		void set_coeff(int i,double q)
			{ c(i,q); }
//! Get coefficient c[i], 0 <= i < N  (i.e. coefficient power r^(2i))
		double c(int i) const
			{
			base_type::check_range(i);
			return _c[i];
			}
//! Set coefficient c[i], 0 <= i < N.
		void c(int i,double q)
			{
			base_type::check_range(i);
			_c[i] = q;
			}

//! Remove distortion. p_dn is a point in diagonally normalized coordinates.
		vec2_type operator()(const vec2_type& p_dn) const
			{
			double r = norm2(p_dn);
			double q(1.0);
// Evaluating the polynomial. Not the fastest way, but we avoid Horner
// because it's numerically unstable. A better way would be Clenshaw's method.
			for(int i = 0;i < N;++i)
				{
				double r_pow = std::pow(r,2 * (i + 1));
				q += c(i) * r_pow;
				}
			return vec2_type(p_dn[0] * q,p_dn[1] * q);
			}
//! @brief Derivative with respect to distortion coefficients.
//! dg points to an array with 2 * N Elements.
		void derive(double* dg,int n_parameters,const vec2_type& p_dn) const
			{
			int size = 2 * n_parameters;
			double r = norm2(p_dn);
			int k = 0;
			for(int i = 0;i < N;++i)
				{
				double r_pow = std::pow(r,2 * (i + 1));
				dg[k++] = p_dn[0] * r_pow;
				dg[k++] = p_dn[1] * r_pow;
// We calculate the first n derivatives, where n is the size of g.
				if(k == size) return;
				}
// Unreachable
			std::cerr << "example_radial_distortion: n_parameters out of range" << std::endl;
			}
		std::ostream& out(std::ostream& cout) const
			{
			int p = cout.precision();
			cout.precision(5);
			for(int i = 0;i < N;++i)
				{
				cout << "    C(" << i << "): " << std::right << std::fixed << c(i) << "\n";
				}
			cout.precision(p);
			return cout;
			}
		};
	}

#endif
