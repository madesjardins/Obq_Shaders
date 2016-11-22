// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef ldpk_generic_radial_distortion_sdv
#define ldpk_generic_radial_distortion_sdv

//! @file ldpk_generic_radial_distortion.h
//! @brief A polynomial radially symmetric model of degree N (even)

#include <ldpk/ldpk_generic_distortion_base.h>
#include <ldpk/ldpk_power_ct.h>
#include <iostream>
#include <string>
#include <sstream>

namespace ldpk
	{
//! @brief A polynomial model with N coefficients for radially symmetric lenses.
//! This class contains the model for normalized coordinates with diagonal radius = 1.
//! Lens center is at (0,0), because we take it into account in the plugin class.
	template <class VEC2,class MAT2,int N>
	class generic_radial_distortion:public ldpk::generic_distortion_base<VEC2,MAT2,N>
		{
	public:
		typedef generic_distortion_base<VEC2,MAT2,N> base_type;
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;
	private:
// The coefficients
		double _c[N];
	public:
		generic_radial_distortion()
			{
			for(int i = 0;i < N;++i) _c[i] = 0.0;
			}
//! Get coefficient c[i], 0 <= i < N  (i.e. coefficient power r^(2i))
		double get_coeff(int i) const
			{
			base_type::check_range(i);
			return _c[i];
			}
//! Set coefficient c[i], 0 <= i < N.
		void set_coeff(int i,double q)
			{
			base_type::check_range(i);
			_c[i] = q;
			}
//! Remove distortion. p_dn is a point in diagonally normalized coordinates.
		vec2_type operator()(const vec2_type& p_dn) const
			{
// Compute powers of r^2: 1,r^2,r^4,...,r^(2N)
			double r2pow[N + 1];
			double q(1.0),r2 = dotsq(p_dn);
			power_ct<double,N + 1>().build(r2,r2pow);

			for(int i = 0;i < N;++i)
				{
				q += _c[i] * r2pow[i + 1];
				}
			return vec2_type(p_dn[0] * q,p_dn[1] * q);
			}
//! @brief Analytic version of the Jacobi-matrix, about 2.5 times faster than
//! the base class version which uses difference quotients.
		mat2_type jacobi(const vec2_type& p_dn) const
			{
// Compute powers of r^2: 1,r^2,r^4,...,r^(2N)
			double r2pow[N + 1];
			double r2 = dotsq(p_dn);
			power_ct<double,N + 1>().build(r2,r2pow);

			double qa(1.0),qb(0.0);
			for(int i = 0;i < N;++i)
				{
				qa += _c[i] * r2pow[i + 1];
				qb += _c[i] * (2 * i + 2) * r2pow[i];
				}
			return mat2_type(qa) + mat2_type(tensq(p_dn) * qb);
			}
//! @brief Derivative with respect to distortion coefficients.
//! dg points to an array with 2 * N Elements.
		void derive(double* dg,int n_parameters,const vec2_type& p_dn) const
			{
			int size = 2 * n_parameters;
// Compute powers of r^2: 1,r^2,r^4,...,r^(2N)
			double r2pow[N + 1];
			double r2 = dotsq(p_dn);
			power_ct<double,N + 1>().build(r2,r2pow);

			int k = 0;
			for(int i = 0;i < N;++i)
				{
				dg[k++] = p_dn[0] * r2pow[i + 1];
				dg[k++] = p_dn[1] * r2pow[i + 1];
// We calculate the first n derivatives, where n is the size of g.
				if(k == size) return;
				}
// Unreachable
			AiMsgError("example_radial_distortion: n_parameters out of range");
			}
		std::ostream& out(std::ostream& cout) const
			{
			int p = static_cast<int>(cout.precision());
			cout.precision(5);
			for(int i = 0;i < N;++i)
				{
				cout << "    C(" << 2 * i + 2 << "): " << std::right << std::fixed << _c[i] << "\n";
				}
			cout.precision(p);
			return cout;
			}
		};
	}

#endif
