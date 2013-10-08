// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.4


#ifndef tde4_ldp_classic_3de_mixed_sdv
#define tde4_ldp_classic_3de_mixed_sdv

#include <ldpk/tde4_ldp_common_sdv.h>
#include <ldpk/ldpk_classic_3de_mixed_distortion.h>

//! @file tde4_ldp_classic_3de_mixed.h
//! @brief Degree-2 anamorphic and degree-4 radial mixed model.

//! @brief Degree-2 anamorphic and degree-4 radial mixed model.
//! Does not compensate for decentering.
//! Parameters can be calculated by 3DE's Matrix Tool.
template <class VEC2,class MAT2>
class tde4_ldp_classic_3de_mixed:public tde4_ldp_common_sdv<VEC2,MAT2,5>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef tde4_ldp_common_sdv<VEC2,MAT2,5> base_type;

	ldpk::classic_3de_mixed_distortion<vec2_type,mat2_type> _distortion;
	double _r_fb_cm;

	enum para_enum {
		para_ld = 0,
		para_sq = 1,
		para_cx = 2,
		para_cy = 3,
		para_qu = 4
		};

	static const char* _para[5];

	const char** get_para_names() const
		{ return _para; }
	ldpk::generic_distortion_base<vec2_type,mat2_type,5>& get_distortion_base()
		{ return _distortion; }
public:
	tde4_ldp_classic_3de_mixed()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE Classic LD Model [Plugin]");
#else
		strcpy(name,"3DE Classic LD Model");
#endif
		return true;
		}
	bool getParameterType(const char* identifier,tde4_ldp_ptype& ptype)
		{
		typedef base_type bt;
		int i;
		if(bt::get_builtin_parameter_type(identifier,ptype)) return true;
		if(!bt::decypher(identifier,i)) return false;
		ptype = TDE4_LDP_ADJUSTABLE_DOUBLE;
		return true;
		}
	bool getParameterDefaultValue(const char* identifier,double& v)
		{
		typedef base_type bt;
		int i;
		if(!bt::decypher(identifier,i)) return false;
// Alle Parameter haben als Default-Wert 0, ausser Squeeze
		v = (i == para_sq) ? 1.0 : 0.0;
		return true;
		}
	bool getParameterRange(const char* identifier,double& a,double& b)
		{
		typedef base_type bt;
		int i;
		if(!bt::decypher(identifier,i)) return false;
// squeeze
		if(i == para_sq)
			{
			a = 1.0 / 4.0;
			b = 4.0;
			}
		else
			{
			a = -0.5;
			b =  0.5;
			};
		return true;
		}
//! Tested against difference quotients.
	bool getJacobianMatrix(double x0,double y0,double& m00,double& m01,double& m10,double& m11)
		{
		typedef base_type bt;
		mat2_type m = _distortion.jacobi(map_unit_to_dn(vec2_type(x0,y0)));
// to myself: Eigentlich w/2,h/2 bei beiden. Kuerzt sich weg.
		mat2_type u2d(bt::w_fb_cm() / bt::r_fb_cm(),0.0,0.0,bt::h_fb_cm() / bt::r_fb_cm());
		mat2_type d2u(bt::r_fb_cm() / bt::w_fb_cm(),0.0,0.0,bt::r_fb_cm() / bt::h_fb_cm());
		m = d2u * m * u2d;
		m00 = m[0][0];m01 = m[0][1];m10 = m[1][0];m11 = m[1][1];
		return true;
		}
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_classic_3de_mixed<VEC2,MAT2>::_para[5] = {
	"Distortion",
	"Anamorphic Squeeze",
	"Curvature X",
	"Curvature Y",
	"Quartic Distortion"
	};

#endif
