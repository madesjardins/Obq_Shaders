// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.4


#ifndef ldpk_generic_distortion_base_sdv
#define ldpk_generic_distortion_base_sdv

//! @file ldpk_generic_distortion_base.h
//! @brief Base class for distortion models

#include <ldpk/ldpk_error.h>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

namespace ldpk
	{
//! @brief Base class for a distortion model with N parameters.
//! You may find it useful to derive your own distortion model class from this one.
//! It contains methods for inverting the distortion model function.
//! We derive this class from the standard unary function class
//! in order to get a well-defined function object.
	template <class VEC2,class MAT2,int N>
	class generic_distortion_base:public std::unary_function<VEC2,VEC2>
		{
	private:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;

// For the inverse mapping we have termination conditions
		double _epsilon;
// After n_max_iter we give up. This should not happen.
		int _n_max_iter;
// n_post_iter is the number of iterations we perform after
// epsilon is reached.
		int _n_post_iter;
// For debugging: count iterations
		mutable int _k_iter;
// For debugging: maximum number of iterations occured
		mutable int _k_max_iter;
// For debugging: value being compared to _epsilon.
		mutable double _diff;

	protected:
//! A derived class may check if the index is valid.
		void check_range(int i) const
			{
			if((i < 0) || (i >= N))
				{
				throw error_index_out_of_range(i,0,N - 1);
				}
			}
	public:
		generic_distortion_base()
			{
			_epsilon = 1e-6;
			_n_max_iter = 20;
			_n_post_iter = 2;
			}
//! Number of parameters, that is N.
		int get_num_parameters() const
			{ return N; }
//! @brief Configure iterative procedure for map_inverse(). Call this, if you don't agree
//! with the default values.
		void setup_map_inverse(int n_max_iter,int n_post_iter,double epsilon)
			{
			_n_max_iter = n_max_iter;
			_n_post_iter = n_post_iter;
			_epsilon = epsilon;
			}
//! @brief After changing one or more coefficients of the model, call this (future use).
//! The derived class may prepare some data structure for fast/precise evalutation..
		virtual void done()
			{ }
//! There must be methods to address coefficients by one single index i in [0,N[
		virtual double get_coeff(int i) const = 0;
		virtual void set_coeff(int i,double) = 0;

//! Reset k_max_iter for debugging purposes.
		void reset_k_max_iter()
			{ _k_max_iter = 0; }
//! @brief Number of iterations until epsilon was reached. This value is reset at the beginning
//! of each iterative calculation.
		int get_k_iter() const
			{ return _k_iter; }
//! @brief By this value you can check how much iterations per pixel were required
//! to warp an entire image or sequence. Use reset_k_max_iter() to reset to 0.
		int get_k_max_iter() const
			{ return _k_iter; }
//! @brief User-defined maximum number of iterations applied in map_inverse in order to
//! fulfill the termination condition.
		int get_n_max_iter() const
			{ return _n_max_iter; }
//! @brief User-defined number of additional iterations to be applied when
//! the termination condition is fulfilled (which we call post-iterations).
		int get_n_post_iter() const
			{ return _n_post_iter; }
//! @brief Difference between is-value and should-be-value in map_inverse().
//! Inverse mapping is implemented as fixed-point problem. map_inverse()
//! terminates, when diff is smaller than epsilon or the number of iterations
//! exceeds n_max_iter.
		double get_diff() const
			{ return _diff; }
//! Remove distortion. This method is non-iterative.
		virtual vec2_type operator()(const vec2_type& p) const = 0;
//! Same as method instead of operator
		vec2_type eval(const vec2_type& p) const
			{ return (*this)(p); }
//! @brief Jacobi-Matrix. The result is a matrix g_{ij} = d/dp_j f(p)_i, where f represents the undistort-function.
//! We compute this by means of difference quotients. This requires four evaluations. For better performance,
//! you can implement the analytic form in your derived distortion class.
		virtual mat2_type jacobi(const vec2_type& p_dn) const
			{
			const double epsilon = 1e-4;
			return	trans(mat2_type((eval(p_dn + vec2_type(epsilon,0)) - eval(p_dn - vec2_type(epsilon,0))) / (2.0 * epsilon),
						(eval(p_dn + vec2_type(0,epsilon)) - eval(p_dn - vec2_type(0,epsilon))) / (2.0 * epsilon)));
			}
//! Not all distortion functions will support this
		virtual void derive(double* dg,int n_parameters,const vec2_type& p_dn) const
			{
			std::cout << "ldpk::generic_distortion_base::derive: not implemented" << std::endl;
			}
//! @brief Inverse mapping by solving the fixed point equation without providing initial values.
//! Virtual, because the derived class might use some smart data structure for calculating an initial value.
		virtual vec2_type map_inverse(const vec2_type& q) const
			{
			vec2_type p = q - ((*this)(q) - q);
// Iterate until |f(p_iter) - q| < epsilon.
// We count the iterations for debugging purposes.
			_k_iter = 0;
			for(int i = 0;i < _n_max_iter;++i)
				{
				vec2_type q_iter = (*this)(p);
				p = p + q - q_iter;
				++_k_iter;
				_diff = norm2(q_iter - q);
				if(_diff < _epsilon)
					{
					break;
					}
				}
// We improve precision by post-iterations.
			for(int i = 0;i < _n_post_iter;++i)
				{
				vec2_type q_iter = (*this)(p);
				p = p + q - q_iter;
				}
// k_max_iter counts the number of iterations for the worst case.
			if(_k_iter > _k_max_iter)
				{
				_k_max_iter = _k_iter;
				}
			return p;
			}
//! For given q, we are looking for p so that f(p) = q. p_start is near to p.
		vec2_type map_inverse(const vec2_type& q,const vec2_type& p_start) const
			{
			vec2_type p = p_start;
			for(_k_iter = 0;_k_iter < 20;++_k_iter)
				{
// A Newton iteration
				mat2_type g = jacobi(p);
				vec2_type q_cmp(eval(p));
				try
					{
					p += invert(g) * (q - q_cmp);
					}
				catch(...)
					{
					return vec2_type();
					}
				if(norm2(q - q_cmp) < 1e-8)
					{
					break;
					}
				}
// k_max_iter counts the number of iterations for the worst case.
			if(_k_iter > _k_max_iter)
				{
				_k_max_iter = _k_iter;
				}
			return p;
			}
//! @brief The derived class implements a method for printing values
//! inside 3DE4's matrix tool dialog. This functionality
//! is currently not supported by tde4_ld_plugin.
		virtual std::ostream& out(std::ostream& cout) const
			{ return cout; }
		};
	}

#endif
