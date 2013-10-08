// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011 - Science-D-Visions. Current version: 1.1


#ifndef ldpk_generic_anamorphic_distortion_sdv
#define ldpk_generic_anamorphic_distortion_sdv

//! @file gld_generic_anamorphic_distortion.h
//! @brief A polynomial anamorphic distortion model of degree N (even).

#include "ldpk/ldpk_generic_distortion_base.h"

namespace ldpk
	{
//! @brief
//! A polynomial distortion model for anamorphic distortion without decntering.
//! N is the degree of the polynomial, even.
//! VEC2 is a float-valued, two-component vector type.
//! The model has the following properties:
//! - all monomials are products of even powers of x and y.
//! - x- and y- direction have their own coefficients.
//! - Degree 2 ->  4 Parameters
//! - Degree 4 -> 10 Parameters
//! - Degree 6 -> 18 Parameters
//! - Degree N -> (N + 2) * (N + 4) / 4 - 2 Parameters
	template <class VEC2,class MAT2,int N>
	class generic_anamorphic_distortion:public ldpk::generic_distortion_base<VEC2,MAT2,(N + 2) * (N + 4) / 4 - 2>
		{
	public:
		typedef generic_distortion_base<VEC2,MAT2,(N + 2) * (N + 4) / 4 - 2> base_type;
		typedef VEC2 vec2_type;
	private:
// We represent the coefficients by a two-dimensional array.
// The upper right triangle including diagonal is used for the model, all others are ignored.
// For the generic anamorphic model, we have different values in x- and y- direction.
		double _cx[(N / 2) + 1][(N / 2) + 1];
		double _cy[(N / 2) + 1][(N / 2) + 1];
// Array for linear index access.
		double *_c[(N + 2) * (N + 4) / 4 - 2];

	public:
		generic_anamorphic_distortion()
			{
// Default is the identity.
			int k = 0;
			for(int i_phi = 0;i_phi <= N;i_phi += 2)
				{
				for(int i_r = 0;i_r <= N;i_r += 2)
					{
					cx(i_phi,i_r,0);
					cy(i_phi,i_r,0);
					}
				}
			_cx[0][0] = 1.0;
			_cy[0][0] = 1.0;
// Linearizing coefficients. The order of loops is important.
			for(int i_r = 0;i_r <= N;i_r += 2)
				{
				for(int i_phi = 0;i_phi <= i_r;i_phi += 2)
					{
					if((i_phi != 0) || (i_r != 0))
						{
						_c[k++] = &_cx[i_phi >> 1][i_r >> 1];
						_c[k++] = &_cy[i_phi >> 1][i_r >> 1];
						}
					}
				}
			if(k != base_type::get_num_parameters())
				{
				std::cerr << "generic_anamorphic_distortion: bad implementation for N = " << N;
				std::cerr << ". This needs to be fixed." << std::endl;
				}
			}
//! Get coefficient as demanded by base class
		double get_coeff(int i) const
			{
			base_type::check_range(i);
			return *_c[i];
			}
//! Set coefficient as demanded by base class
		void set_coeff(int i,double q)
			{
			base_type::check_range(i);
			*_c[i] = q;
			}

// The coefficients of the Generic Polynomial Model fulfill
// the following conditions:
// - Indices are even
// - i_r in [0,N]
// - i_r >= i_phi
//! x-direction
		double cx(int i_phi,int i_r) const
			{ return _cx[i_phi >> 1][i_r >> 1]; }
		void cx(int i_phi,int i_r,double c)
			{ _cx[i_phi >> 1][i_r >> 1] = c; }
//! y-direction
		double cy(int i_phi,int i_r) const
			{ return _cy[i_phi >> 1][i_r >> 1]; }
		void cy(int i_phi,int i_r,double c)
			{ _cy[i_phi >> 1][i_r >> 1] = c; }

//! @brief As usual, we define the distortion mapping in diagonally normalized coordinates,
//! (hence the suffix _dn). The operator expects, that p_dn is already shifted so that
//! the lens center is (0,0).
		vec2_type operator()(const vec2_type& p_dn) const
			{
// Our generic model is based on polar coordinates,
// so we represent the point as radius and angle.
			double r = norm2(p_dn);
			double phi = atan2(p_dn[1],p_dn[0]);
			vec2_type q;
// We calculate powers of r in advance for better performance.
			double r_pow[N + 1];
			r_pow[0] = 1.0;
			for(int i = 2;i < N + 1;i += 2)
				{
				r_pow[i] = (r * r) * r_pow[i - 2];
				}
// Evaluating the polynomial
			for(int i_phi = 0;i_phi <= N;i_phi += 2)
				{
				double cos_i_phi = cos(i_phi * phi);
				for(int i_r = i_phi;i_r <= N;i_r += 2)
					{
// That is coefficient times cosine times power of r.
					q[0] += cx(i_phi,i_r) * cos_i_phi * r_pow[i_r];
					q[1] += cy(i_phi,i_r) * cos_i_phi * r_pow[i_r];
					}
				}
			return vec2_type(p_dn[0] * q[0],p_dn[1] * q[1]);
			}

//! @brief Derivative wrt distortion coefficients.
//! For performance reasons we calculate all derivatives simultaneously.
//! g points to an array with (N / 2 + 1) * (N / 2 + 2) - 2 Elements.
//! That is 4 for N=2, 10 for N=4, 18 for N=6.
		void derive(double* dg,int n_parameters,const vec2_type& p_dn) const
			{
			int size = 2 * n_parameters;
// Our generic model is based on polar coordinates,
// so we represent the point as radius and angle.
			double r = norm2(p_dn);
			double phi = atan2(p_dn[1],p_dn[0]);
// We calculate powers of r and cosine-terms in advance for better performance.
			double r_pow[N + 1];
			r_pow[0] = 1.0;
			for(int i = 2;i < N + 1;i += 2)
				{
				r_pow[i] = (r * r) * r_pow[i - 2];
				}
			double cos_phi[N + 1];
			for(int i_phi = 0;i_phi < N + 1;i_phi += 2)
				{
				cos_phi[i_phi] = cos(i_phi * phi);
				}

// The order of coefficients is: 1. powers of r and 2. angular frequencies.
			int k = 0;
			for(int i_r = 0;i_r <= N;i_r += 2)
				{
				for(int i_phi = 0;i_phi <= i_r;i_phi += 2)
					{
// Omit (0,0) which is a constant
					if(i_r || i_phi)
						{
// Since the model is linear in each coefficient, derivatives are constants.
						double cr = cos_phi[i_phi] * r_pow[i_r];
						dg[k++] = p_dn[0] * cr;
						dg[k++] = 0;
// We calculate at most the first n derivatives, where n is the size of dg.
						if(k == size) return;

						dg[k++] = 0;
						dg[k++] = p_dn[1] * cr;
// We calculate at most the first n derivatives, where n is the size of dg.
						if(k == size) return;
						}
					}
				}
			}
		std::ostream& out(std::ostream& cout) const
			{
			int p = int(cout.precision());
			cout.precision(5);
			for(int i_r = 0;i_r <= N;i_r += 2)
				{
// One row Cx
				if(i_r) cout << "    Cx(i," << i_r << "): ";
				for(int i_phi = 0;i_phi <= i_r;i_phi += 2)
					{
					if(i_r || i_phi)
						{
						cout.width(8);
						cout << std::right << std::fixed << cx(i_phi,i_r) << " ";
						}
					}
				if(i_r) cout << "\n";
// One row Cy
				if(i_r) cout << "    Cy(i," << i_r << "): ";
				for(int i_phi = 0;i_phi <= i_r;i_phi += 2)
					{
					if(i_r || i_phi)
						{
						cout.width(8);
						cout << std::right << std::fixed << cy(i_phi,i_r) << " ";
						}
					}
				if(i_r) cout << "\n";
				}
			cout.precision(p);
			return cout;
			}
		};
	}

#endif
