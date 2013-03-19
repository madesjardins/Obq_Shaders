// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011 - Science-D-Visions. Current version: 1.1


#ifndef tde4_ldp_radial_deg_8_sdv
#define tde4_ldp_radial_deg_8_sdv

#include <ldpk/tde4_ldp_common_sdv.h>
#include <ldpk/ldpk_generic_radial_distortion.h>

//! @file tde4_ldp_radial_deg_8.h
//! @brief Plugin class for radial distortion

//! @brief Plugin class for radial distortion.
//! Does not compensate for decentering.
//! Parameters can be calculated by 3DE's Matrix Tool.
template <class VEC2,class MAT2>
class tde4_ldp_radial_deg_8:public tde4_ldp_common_sdv<VEC2,MAT2,4>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef tde4_ldp_common_sdv<VEC2,MAT2,4> base_type;

	ldpk::generic_radial_distortion<vec2_type,mat2_type,4> _distortion;

	static const char* _para[4];

	const char** get_para_names() const
		{ return _para; }
	ldpk::generic_distortion_base<vec2_type,mat2_type,4>& get_distortion_base()
		{ return _distortion; }
public:
	tde4_ldp_radial_deg_8()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Radial - Fisheye, Degree 8 [Plugin]");
#else
		strcpy(name,"3DE4 Radial - Fisheye, Degree 8");
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
		v = 0.0;
		return true;
		}
	bool getParameterRange(const char* identifier,double& a,double& b)
		{
		typedef base_type bt;
		int i;
		if(!bt::decypher(identifier,i)) return false;
		a = -0.5;
		b = 0.5;
		return true;
		}
	bool providesParameterDerivatives()
		{ return true; }
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_radial_deg_8<VEC2,MAT2>::_para[4] = {
	"Distortion - Degree 2",
	"Quartic Distortion - Degree 4",
	"Degree 6",
	"Degree 8"
	};

#endif
