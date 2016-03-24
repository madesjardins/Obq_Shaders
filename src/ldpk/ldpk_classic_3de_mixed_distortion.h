// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef ldpk_classic_3de_mixed_distortion_sdv
#define ldpk_classic_3de_mixed_distortion_sdv

//! @file ldpk_classic_3de_mixed_distortion.h
//! @brief Degree-2 anamorphic and degree-4 radial mixed model.

#include <ldpk/ldpk_generic_distortion_base.h>

namespace ldpk
	{
//! @brief Degree-2 anamorphic and degree-4 radial mixed model.
	template <class VEC2,class MAT2>
	class classic_3de_mixed_distortion:public ldpk::generic_distortion_base<VEC2,MAT2,5>
		{
	private:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;

		union
			{
			struct
				{
				double _ld,_sq,_cx,_cy,_qu;
				};
			double _c[5];
			};
		mutable double _cxx,_cxy,_cyx,_cyy,_cxxx,_cxxy,_cxyy,_cyxx,_cyyx,_cyyy;
		mutable bool _uptodate;

		void update() const
			{
			_cxx = _ld / _sq;
			_cxy = (_ld + _cx) / _sq;
			_cyx = _ld + _cy;
			_cyy = _ld;
			_cxxx = _qu / _sq;
			_cxxy = 2.0 * _qu / _sq;
			_cxyy = _qu / _sq;
			_cyxx = _qu;
			_cyyx = 2.0 * _qu;
			_cyyy = _qu;
			_uptodate = true;
			}
	public:
		classic_3de_mixed_distortion():_ld(0),_sq(1),_cx(0),_cy(0),_qu(0),_uptodate(false)
			{ }
//! Get coefficient as demanded by base class
		double get_coeff(int i) const
			{ return _c[i]; }
//! Set coefficient as demanded by base class
		void set_coeff(int i,double q)
			{ _c[i] = q;_uptodate = false; }

//! Remove distortion. p is a point in diagonally normalized coordinates.
		vec2_type operator()(const vec2_type& p) const
			{
			if(!_uptodate) update();
			double p0_2 = p[0] * p[0];
			double p1_2 = p[1] * p[1];
			double p0_4 = p0_2 * p0_2;
			double p1_4 = p1_2 * p1_2;
			double p01_2 = p0_2 * p1_2;

			vec2_type q;
			q[0] = p[0] * (1 + _cxx * p0_2 + _cxy * p1_2 + _cxxx * p0_4 + _cxxy * p01_2 + _cxyy * p1_4);
			q[1] = p[1] * (1 + _cyx * p0_2 + _cyy * p1_2 + _cyxx * p0_4 + _cyyx * p01_2 + _cyyy * p1_4);
			return q;
			}
// @brief Analytic version of the Jacobi-matrix
		mat2_type jacobi(const vec2_type& p_dn) const
			{
			if(!_uptodate) update();
			double x = p_dn[0],x2 = x * x,x3 = x2 * x,x4 = x2 * x2;
			double y = p_dn[1],y2 = y * y,y3 = y2 * y,y4 = y2 * y2;
			mat2_type m;
			m[0][0] = 1.0 + x2 * 3.0 * _cxx + y2 * _cxy
				+ x4 * 5.0 * _cxxx + x2 * y2 * 3.0 * _cxxy + y4 * _cxyy;
			m[1][1] = 1.0 + x2 * _cyx + y2 * 3.0 * _cyy
				+ x4 * _cyxx + x2 * y2 * 3.0 * _cyyx + y4 * 5.0 * _cyyy;
			m[0][1] = x * y * 2.0 * _cxy + x3 * y * 2.0 * _cxxy + x * y3 * 4.0 * _cxyy;
			m[1][0] = x * y * 2.0 * _cyx + x3 * y * 4.0 * _cyxx + x * y3 * 2.0 * _cyyx;
			return m;
			}
		std::ostream& out(std::ostream& cout) const
			{
			int p = int(cout.precision());
			cout.precision(5);
			cout << "        Distortion: " << std::right << std::fixed << _ld << "\n";
			cout << "Anamorphic Squeeze: " << std::right << std::fixed << _sq << "\n";
			cout << "       Curvature X: " << std::right << std::fixed << _cx << "\n";
			cout << "       Curvature Y: " << std::right << std::fixed << _cy << "\n";
			cout << "Quartic Distortion: " << std::right << std::fixed << _qu << "\n";
			cout.precision(p);
			return cout;
			}
		};
	}

#endif
