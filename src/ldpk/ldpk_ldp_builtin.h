// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.4


#ifndef ldp_builtin_sdv
#define ldp_builtin_sdv

#include <ldpk/ldpk_vec2d.h>
#include <ldpk/tde4_ld_plugin.h>
#include <ldpk/ldpk_lookup_table.h>
#include <string.h>

namespace ldpk
	{
//! An enum-type for built-in parameters. Also used to address elements in parameter name array.
	enum para_builtin_enum
		{
		para_not_a_builtin = -1,
		para_focal_length = 0,
		para_filmback_width = 1,
		para_filmback_height = 2,
		para_lens_center_offset_x = 3,
		para_lens_center_offset_y = 4,
		para_pixel_aspect = 5,
		para_custom_focus_distance = 6
		};

//! @brief This class handles the built-in parameter and the lookup table.
//! You may find it useful for your own distortion model, since it implements
//! the built-in parameters and the lookup table mechanism. But in contrast
//! to tde4_ld_plugin it's not stringent to use it from a technical point of view.
//! Alternatively you can implement all this in your plugin class.
	template <class VEC2>
	class ldp_builtin:public tde4_ld_plugin
		{
	private:
		typedef VEC2 vec2_type;

//! Names of built-in parameters
		static const char* _para_builtin[7];
//! Values for built-in parameters
		double _value_builtin[7];

//! The lookup table
		ldpk::lookup_table<vec2_type> _lut;
//! @brief Is the lookup table up-to-date? Whenever a parameter
//! is changed it becomes obsolete. This class implements this behaviour
//! for the built-in parameters. The derived class implements it
//! for the model dependent parameters.
		bool _uptodate_lut;

//! @brief If 'identifier' is one of the built-in parameters the corresponing enum-value is return.
//! Otherwise, the error value < 0 is returned.
		para_builtin_enum find_builtin_parameter_by_name(const char* identifier) const;

	protected:
//! @name Lookup table methods
//! @brief checking up-to-date status, declaring obsolete, updating, constant access
//@{
//! @brief The derived class uses this in order to check
//! if the lookup table needs an update.
		bool is_uptodate_lut() const
			{ return _uptodate_lut; }
//! @brief This class and the derived class mark the lookup table as obsolete
//! when some parameter was changed.
		void no_longer_uptodate_lut()
			{ _uptodate_lut = false; }
//! @brief Update the lookup table. The derived class will do this, if necessary,
//! when distort() without initial value is invoked.
		void update_lut();
//! The derived class has constant access, since it needs to consult it.
		const ldpk::lookup_table<vec2_type>& get_lut() const
			{ return _lut; }
//@}
//! @name Built-in parameters
//@{
//! There are seven built-in parameters
		int get_num_builtin_parameters() const
			{ return 7; }
//! The return values indicates, if 'identifier' refers to a built-in parameter.
		bool set_builtin_parameter_value(const char* identifier,double v);
//! @brief The return values indicates, if 'identifier' refers to a built-in parameter.
//! Currently, all built-in parameters are double-valued.
		bool get_builtin_parameter_type(const char* identifier,tde4_ldp_ptype& ptype) const
			{
			if(find_builtin_parameter_by_name(identifier) != para_not_a_builtin)
				{
				ptype = TDE4_LDP_DOUBLE;
				return true;
				}
			return false;
			}

//@}
//! @name Filmback, Lens Center Offset, Focal Length and Pixel Aspect.
//! @brief I prefer short names that look like variable names in the documentation.
//@{
		double w_fb_cm() const
			{ return _value_builtin[para_filmback_width]; }
		double h_fb_cm() const
			{ return _value_builtin[para_filmback_height]; }
		double x_lco_cm() const
			{ return _value_builtin[para_lens_center_offset_x]; }
		double y_lco_cm() const
			{ return _value_builtin[para_lens_center_offset_y]; }
		double fl_cm() const
			{ return _value_builtin[para_focal_length]; }
		double pa() const
			{ return _value_builtin[para_pixel_aspect]; }
		double fd_cm() const
			{ return _value_builtin[para_custom_focus_distance]; }
//@}
	public:
		ldp_builtin():_uptodate_lut(false)
			{ }
		};

// Built-in parameter names. Do not change.
	template <class VEC2>
	const char* ldp_builtin<VEC2>::_para_builtin[7] = {
		"tde4_focal_length_cm",
		"tde4_filmback_width_cm",
		"tde4_filmback_height_cm",
		"tde4_lens_center_offset_x_cm",
		"tde4_lens_center_offset_y_cm",
		"tde4_pixel_aspect",
		"tde4_custom_focus_distance_cm"
		};
	template <class VEC2>
	para_builtin_enum ldp_builtin<VEC2>::find_builtin_parameter_by_name(const char* identifier) const
		{
		for(int i = 0;i < get_num_builtin_parameters();++i)
			{
			if(strcmp(_para_builtin[i],identifier) == 0)
				{
				return para_builtin_enum(i);
				}
			}
		return para_not_a_builtin;
		}
	template <class VEC2>
	bool ldp_builtin<VEC2>::set_builtin_parameter_value(const char* identifier,double v)
		{
		int i = find_builtin_parameter_by_name(identifier);
		if(i < 0) return false;
		if(_value_builtin[i] != v)
			{
// Whenever a parameter is changed, the lookup table becomes obsolete.
			no_longer_uptodate_lut();
			_value_builtin[i] = v;
			}
		}
	template <class VEC2>
	void ldp_builtin<VEC2>::update_lut()
		{
		_lut.init(21,15);
		_lut.reset();
		while(_lut.next())
			{
			vec2_type qs = _lut.get_current_initial_value();
			vec2_type q,p = _lut.get_p_current_fov();
// Call the inverse distortion function with initial values
			distort(p[0],p[1],qs[0],qs[1],q[0],q[1]);
			_lut.set_q_current_fov(q);
			}
		_uptodate_lut = true;
		}
	}

#endif
