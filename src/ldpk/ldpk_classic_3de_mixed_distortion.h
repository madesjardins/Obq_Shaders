// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011 - Science-D-Visions. Current version: 1.1


#ifndef ldpk_classic_3de_mixed_distortion_sdv
#define ldpk_classic_3de_mixed_distortion_sdv

//! @file ldpk_classic_3de_mixed_distortion.h
//! @brief Degree-2 anamorphic and degree-4 radial mixed model.

#include "ldpk/ldpk_generic_distortion_base.h"

namespace ldpk
	{
//! @brief Degree-2 anamorphic and degree-4 radial mixed model.
	template <class VEC2,class MAT2>
	class classic_3de_mixed_distortion:public ldpk::generic_distortion_base<VEC2,MAT2,5>
		{
	private:
		typedef VEC2 vec2_type;

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
		inline void update() const
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
		inline vec2_type operator()(const vec2_type& p) const
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
//		mat2_type jacobi(const vec2_type& p_dn) const
//			{
//			}
		std::ostream& out(std::ostream& cout) const
			{
			int p = int(cout.precision());
			cout.precision(5);
			cout << "        Distortion: " << std::right << std::fixed << _ld << "\n";
			cout << "Anamorphic Squeeze: " << std::right << std::fixed << _sq << "\n";
			cout << "       Curvature X:" << std::right << std::fixed << _cx << "\n";
			cout << "       Curvature Y:" << std::right << std::fixed << _cy << "\n";
			cout << "Quartic Distortion:" << std::right << std::fixed << _qu << "\n";
			cout.precision(p);
			return cout;
			}

		std::string getString(){
			char buf[200];
			sprintf(buf,"Distortion = %f , Anamorphic Squeeze = %f , Curvature X = %f , Curvature Y = %f , Quartic Distortion = %f", _ld, _sq, _cx,_cy, _qu);
			
			return std::string(buf);
		}
		};
	}

#endif
