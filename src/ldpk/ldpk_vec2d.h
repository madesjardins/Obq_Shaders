// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.7


#ifndef ldpk_vec2d_sdv
#define ldpk_vec2d_sdv

#include <ldpk/ldpk_error.h>
#include <iostream>
#include <cmath>

//! @file ldpk_vec2d.h
//! @brief Double-valued vector and matrix class.

namespace ldpk
	{
//! @brief A class for two-dimensional double-valued vectors
//! We have added this class and ldpk::mat2d in order to
//! deliver a self-consistent kit. You may use this one or
//! your own vector and matrix classes, at your choice.
//! If you prefer your own classes, it will be necessary to
//! add a handful of functions like dot(), dotsq(), norm2() for vec2d and invert() for mat2d,
//! which provide standard functions, but do not have standardized nomenclature.
	class vec2d
		{
	private:
		double _x0,_x1;
	public:
//! @name Constructors
//@{
//! Default: null vector
		vec2d():_x0(0),_x1(0)
			{ }
//! Copy constructor
		vec2d(const vec2d& a):_x0(a._x0),_x1(a._x1)
			{ }
//! Constructing by components
		vec2d(double x0,double x1):_x0(x0),_x1(x1)
			{ }
//@}
//! @name Arithmetic operations
//@{
		vec2d& operator += (const vec2d& a)
			{ _x0 += a._x0;_x1 += a._x1;return *this; }
		vec2d operator + (const vec2d& a) const
			{ vec2d r(*this);return r += a; }
		vec2d& operator -= (const vec2d& a)
			{ _x0 -= a._x0;_x1 -= a._x1;return *this; }
		vec2d operator - (const vec2d& a) const
			{ vec2d r(*this);return r -= a; }
		vec2d& operator *= (double q)
			{ _x0 *= q;_x1 *= q;return *this; }
		vec2d operator * (double q) const
			{ vec2d r(*this);return r *= q; }
		friend vec2d operator * (double q,const vec2d& a)
			{ vec2d r(a);return r *= q; }
		vec2d& operator /= (double q)
			{ _x0 /= q;_x1 /= q;return *this; }
		vec2d operator / (double q) const
			{ vec2d r(*this);return r /= q; }
		vec2d operator - ()
			{ return vec2d(-_x0,-_x1); }
//@}
//! @name Accessing components
//@{
		const double& operator [] (int i) const
			{ return *(&_x0 + i); }
		double& operator [] (int i)
			{ return *(&_x0 + i); }
//@}
//! @name Special vector functions
//@{
//! Inner product square
		friend double dotsq(const vec2d& a)
			{ return a[0] * a[0] + a[1] * a[1]; }
//! Inner product
		friend double dot(const vec2d& a,const vec2d& b)
			{ return a[0] * b[0] + a[1] * b[1]; }
//! Euclidian norm
		friend double norm2(const vec2d& a)
			{ return ::sqrt(dotsq(a)); }
//! Unit vector
		friend vec2d unit(const vec2d& v)
			{ return v / norm2(v); }
//@}
//! @name I/O functions
//@{
		friend std::ostream& operator << (std::ostream& cout,const vec2d& a)
			{ return cout << a[0] << " " << a[1]; }
		friend std::istream& operator >> (std::istream& cin,vec2d& a)
			{ return cin >> a[0] >> a[1]; }
//@}
		};
//! A class for double-valued 2x2-matrices. The matrix class for ldpk::vec2d.
	class mat2d
		{
	private:
		vec2d _x0,_x1;
	public:
//! @name Constructors
//@{
//! Default: null matrix
		mat2d():_x0(1,0),_x1(0,1)
			{ }
//! Copy constructor
		mat2d(const mat2d& a):_x0(a[0]),_x1(a[1])
			{ }
//! Scalar matrix
		mat2d(double s):_x0(s,0),_x1(0,s)
			{ }
//! Constructing by components (numbers)
		mat2d(double a00,double a01,double a10,double a11):_x0(a00,a01),_x1(a10,a11)
			{ }
//! Constructing by components (row vectors)
		mat2d(const vec2d& x0,const vec2d& x1):_x0(x0),_x1(x1)
			{ }
//@}
//! @name Arithmetic operations
//@{
		mat2d& operator += (const mat2d& a)
			{ _x0 += a._x0;_x1 += a._x1;return *this; }
		mat2d operator + (const mat2d& a) const
			{ mat2d r(*this);return r += a; }
		mat2d& operator -= (const mat2d& a)
			{ _x0 -= a._x0;_x1 -= a._x1;return *this; }
		mat2d operator - (const mat2d& a) const
			{ mat2d r(*this);return r -= a; }
		mat2d& operator *= (double q)
			{ _x0 *= q;_x1 *= q;return *this; }
		mat2d operator * (double q) const
			{ mat2d r(*this);return r *= q; }
		friend mat2d operator * (double q,const mat2d& a)
			{ mat2d r(a);return r *= q; }
//! Matrix multiplication
		mat2d operator * (const mat2d& a) const
			{
			const mat2d& t(*this);
			return mat2d(	t[0][0] * a[0][0] + t[0][1] * a[1][0],
					t[0][0] * a[0][1] + t[0][1] * a[1][1],
					t[1][0] * a[0][0] + t[1][1] * a[1][0],
					t[1][0] * a[0][1] + t[1][1] * a[1][1]);
			}
		mat2d& operator /= (double q)
			{ _x0 /= q;_x1 /= q;return *this; }
		mat2d operator / (double q) const
			{ mat2d r(*this);return r /= q; }
		mat2d operator - ()
			{ return mat2d(-_x0,-_x1); }
//! Apply matrix to vector
		vec2d operator * (const vec2d& a) const
			{ return vec2d(dot(_x0,a),dot(_x1,a)); }
//@}
//! @name Accessing components (row vectors)
//@{
		const vec2d& operator [] (int i) const
			{ return *(&_x0 + i); }
		vec2d& operator [] (int i)
			{ return *(&_x0 + i); }
//@}
//! @name Special matrix functions
//@{
//! Determinant
		friend double det(const mat2d& a)
			{ return a[0][0] * a[1][1] - a[0][1] * a[1][0]; }
//! Transposed
		friend mat2d trans(const mat2d& a)
			{ return mat2d(a[0][0],a[1][0],a[0][1],a[1][1]); }
//! Inverse
		friend mat2d invert(const mat2d& a)
			{ return mat2d(a[1][1],-a[0][1],-a[1][0],a[0][0]) / det(a); }
//@}
//! @name I/O functions
//@{
		friend std::ostream& operator << (std::ostream& cout,const mat2d& a)
			{ return cout << a[0] << " " << a[1]; }
		friend std::istream& operator >> (std::istream& cin,mat2d& a)
			{ return cin >> a[0] >> a[1]; }
//@}
		};

//! @name Tensor products
//@{
//! Tensor (dyadic) product of vectors
	inline mat2d ten(const vec2d& a,const vec2d& b)
		{ return mat2d(a[0] * b[0],a[0] * b[1],a[1] * b[0],a[1] * b[1]); }
//! Tensor (dyadic) product square
	inline mat2d tensq(const vec2d& a)
		{ return mat2d(a[0] * a[0],a[0] * a[1],a[1] * a[0],a[1] * a[1]); }
//@}
//! @brief A simple box class for double precision points in 2d.
//! We will extend this as needed.
	template <class VEC2>
	class box
		{
	public:
		typedef VEC2 vec2_type;
		typedef box<VEC2> this_type;
	private:
		vec2_type _a,_b;
	public:
		box():_a(HUGE_VAL,HUGE_VAL),_b(-HUGE_VAL,-HUGE_VAL)
			{ }
		void reset()
			{
			_a = vec2_type(HUGE_VAL,HUGE_VAL);
			_b = vec2_type(-HUGE_VAL,-HUGE_VAL);
			}
		const vec2_type& a() const
			{ return _a; }
		const vec2_type& b() const
			{ return _b; }
		bool is_empty() const
			{ return (_b[0] < _a[0]) || (_b[1] < _a[1]); }
		void extend_x(double x)
			{
			if(x < _a[0]) _a[0] = x;
			if(x > _b[0]) _b[0] = x;
			}
		void extend_y(double y)
			{
			if(y < _a[1]) _a[1] = y;
			if(y > _b[1]) _b[1] = y;
			}
		void extend(int i,double q)
			{
			if(i == 0) extend_x(q);
			if(i == 1) extend_y(q);
			}
		void extend(const vec2_type& p)
			{
			extend_x(p[0]);
			extend_y(p[1]);
			}
		this_type& merge(const this_type& q)
			{
			extend(q.a());
			extend(q.b());
			return *this;
			}
		friend this_type merge(const this_type& q,const this_type& r)
			{
			this_type s(q);
			return s.merge(r);
			}
		};
	}

#endif
