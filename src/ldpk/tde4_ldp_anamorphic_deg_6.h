// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011 - Science-D-Visions. Current version: 1.1


#ifndef tde4_ldp_anamorphic_deg_6_sdv
#define tde4_ldp_anamorphic_deg_6_sdv

#include <ldpk/tde4_ldp_common_sdv.h>
#include <ldpk/ldpk_generic_anamorphic_distortion.h>

//! @file tde4_ldp_anamorphic_deg_6.h
//! @brief Plugin class for anamorphic distortion

//! @brief Plugin class for anamorphic distortion.
//! Does not compensate for decentering.
//! Parameters can be calculated by 3DE's Matrix Tool.
template <class VEC2,class MAT2>
class tde4_ldp_anamorphic_deg_6:public tde4_ldp_common_sdv<VEC2,MAT2,18>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef tde4_ldp_common_sdv<VEC2,MAT2,18> base_type;
// Anamorphic distortion of degree 6.
	ldpk::generic_anamorphic_distortion<vec2_type,mat2_type,6> _distortion;

	static const char* _para[18];

	const char** get_para_names() const
		{ return _para; }
	ldpk::generic_distortion_base<vec2_type,mat2_type,18>& get_distortion_base()
		{ return _distortion; }
public:
	tde4_ldp_anamorphic_deg_6()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Anamorphic, Degree 6 [Plugin]");
#else
		strcpy(name,"3DE4 Anamorphic, Degree 6");
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
const char* tde4_ldp_anamorphic_deg_6<VEC2,MAT2>::_para[18] = {
	"Cx02 - Degree 2","Cy02 - Degree 2",
	"Cx22 - Degree 2","Cy22 - Degree 2",

	"Cx04 - Degree 4","Cy04 - Degree 4",
	"Cx24 - Degree 4","Cy24 - Degree 4",
	"Cx44 - Degree 4","Cy44 - Degree 4",

	"Cx06 - Degree 6","Cy06 - Degree 6",
	"Cx26 - Degree 6","Cy26 - Degree 6",
	"Cx46 - Degree 6","Cy46 - Degree 6",
	"Cx66 - Degree 6","Cy66 - Degree 6"
	};

#endif
