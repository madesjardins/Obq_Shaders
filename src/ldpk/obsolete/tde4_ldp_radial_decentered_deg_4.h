#ifndef tde4_ldp_radial_decentered_deg_4_sdv
#define tde4_ldp_radial_decentered_deg_4_sdv

#include <ldpk/tde4_ldp_common_sdv.h>
#include <ldpk/ldpk_radial_decentered_distortion.h>

//! @file tde4_ldp_radial_decentered_deg_4.h
//! @brief Plugin class for radial distortion

//! @brief Plugin class for radial distortion.
//! Compensates for decentering.
//! Parameters can be calculated by 3DE's Matrix Tool.
template <class VEC2,class MAT2>
class tde4_ldp_radial_decentered_deg_4:public tde4_ldp_common_sdv<VEC2,MAT2,6>
	{
private:
	typedef VEC2 vec2_type;
	typedef MAT2 mat2_type;
	typedef tde4_ldp_common_sdv<VEC2,MAT2,6> base_type;

	ldpk::radial_decentered_distortion<vec2_type,mat2_type> _distortion;

	static const char* _para[6];

	const char** get_para_names() const
		{ return _para; }
	ldpk::generic_distortion_base<vec2_type,mat2_type,6>& get_distortion_base()
		{ return _distortion; }
public:
	tde4_ldp_radial_decentered_deg_4()
		{ }
	bool getModelName(char *name)
		{
#ifdef LDPK_COMPILE_AS_PLUGIN_SDV
		strcpy(name,"3DE4 Radial - Standard, Degree 4 [Plugin]");
#else
		strcpy(name,"3DE4 Radial - Standard, Degree 4");
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
		if((i == 0) || (i == 3))
			{
			a = -0.5;
			b = 0.5;
			}
		else
			{
			a = -0.1;
			b = 0.1;
			}
		return true;
		}
	bool providesParameterDerivatives()
		{ return true; }
	};

template <class VEC2,class MAT2>
const char* tde4_ldp_radial_decentered_deg_4<VEC2,MAT2>::_para[6] = {
	"Distortion - Degree 2",
	"U - Degree 2",
	"V - Degree 2",
	"Quartic Distortion - Degree 4",
	"U - Degree 4",
	"V - Degree 4"
	};

#endif
