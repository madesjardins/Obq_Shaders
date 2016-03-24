// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef ldp_builtin_sdv
#define ldp_builtin_sdv

#include <ldpk/ldpk_vec2d.h>
#include <ldpk/tde4_ld_plugin.h>
#include <ldpk/ldpk_lookup_table.h>
#include <algorithm>
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
	template <class VEC2,class MAT2>
	class ldp_builtin:public tde4_ld_plugin
		{
	private:
		typedef VEC2 vec2_type;
		typedef MAT2 mat2_type;
		typedef box<VEC2> box2_type;

//! Names of built-in parameters
		static const char* _para_builtin[7];
//! Values for built-in parameters
		double _value_builtin[7];

//! Diagonal radius, required for diagonally normalized coordinates.
		double _r_fb_cm;

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

//! @brief A cubic polynomial [0,1] -> R^2 : t->q(t) is formed by funcation values qa=q(0) and qb=q(1)
//! and derivatives with respect to t, dqa = q'(0) and dqb = q'(1). We calculate the (at most)
//! four values for which the curve assumes extremal values with respect to the two coordinates of q(t).
//! t_out points to space for four doubles. Return value is the number of parameter values found,
//! the number of defined values t_out points to.
		static int compute_extremal_parameters_of_spline(	const vec2_type& qa,const vec2_type& qb,
									const vec2_type& dqa,const vec2_type& dqb,
									double* t_out)
			{
			vec2_type b = dqa;
			vec2_type c = -3.0 * qa + 3.0 * qb - 2.0 * dqa - dqb;
			vec2_type d =  2.0 * qa - 2.0 * qb +       dqa + dqb;
			vec2_type disc;
			int i_out = 0;

			for(int i = 0;i < 2;++i)
				{
				if(d[i] != 0.0)
					{
					double d_mul_3 = 3.0 * d[i];
					disc[i] = (c[i] * c[i]) / (d_mul_3 * d_mul_3) - b[i] / d_mul_3;
// equal 0 means saddle point, i.e. not an extreme value, less than 0 means no extremum and np saddle point.
// We are only interested in local extrema, therefore we check against 0.0 and 1.0.
					if(disc[i] > 0.0)
						{
						double t,r = sqrt(disc[i]);
						t = -c[i] / d_mul_3 + r;
						if((t > 0.0) && (t < 1.0))
							{ t_out[i_out++] = t; }
						t = -c[i] / d_mul_3 - r;
						if((t > 0.0) && (t < 1.0))
							{ t_out[i_out++] = t; }
						}
					}
				else if(c[i] != 0.0)
					{
// Not a cubic polynomial? Treat as quadratic.
					double t = b[i] / (-2.0 * c[i]);
					if((t > 0.0) && (t < 1.0))
						{ t_out[i_out++] = t; }
					}
				}
			return i_out;
			}
		template <int I> void extend_box_by_spline(	const vec2_type& pa,const vec2_type& pb,
								const vec2_type& qa,const vec2_type& qb,
								const mat2_type& ja,const mat2_type& jb,
								box2_type& bbox)
			{
			double t_out[4];
			vec2_type dqa = ja * (pb - pa);
			vec2_type dqb = jb * (pb - pa);
			int n_out = compute_extremal_parameters_of_spline(qa,qb,dqa,dqb,t_out);
			for(int i = 0;i < n_out;++i)
				{
				if((t_out[i] <= 0.0) || (t_out[i] >= 1.0))
					{
                    // OBQ:
                    AiMsgError("ldpk::ldp_builtin: extend_box_by_spline: implementation error, needs fix.");
                    // LDPK:
                    // std::cerr << "ldpk::ldp_builtin: extend_box_by_spline: implementation error, needs fix." << std::endl;
                    }
				vec2_type qt,pt;
				pt = (1.0 - t_out[i]) * pa + t_out[i] * pb;
				qt = apply_mapping<I>(pt);
				bbox.extend(qt);
				}
			}
//! I=1: Jacobian of undistort at p, I=0: Jacobian of distort at p (but need q for computing).
		template <int I> mat2_type get_jacobian_matrix(const vec2_type& p,const vec2_type& q)
			{
			mat2_type j;
			if(I)
				{
				getJacobianMatrix(p[0],p[1],j[0][0],j[0][1],j[1][0],j[1][1]);
				return j;
				}
			else
				{
// Checked with difference quotients. This is called the Inverse Function Theorem.
				getJacobianMatrix(q[0],q[1],j[0][0],j[0][1],j[1][0],j[1][1]);
				return invert(j);
				}
			}
//! I=1: undistort, I=0: distort.
		template <int I> vec2_type apply_mapping(const vec2_type& p)
			{
			vec2_type q;
			if(I)
				{ undistort(p[0],p[1],q[0],q[1]); }
			else
				{ distort(p[0],p[1],q[0],q[1]); }
			return q;
			}
		template <int I> void get_bounding_box(	double xa_in,double ya_in,double xb_in,double yb_in,
							double& xa_out,double& ya_out,double& xb_out,double& yb_out,
							int nx,int ny)
			{
			double dx_in = xb_in - xa_in,dy_in = yb_in - ya_in;
			box2_type bbox;

			vec2_type pa,pb;
			vec2_type qa,qb;
			mat2_type ja,jb;
// ya_in
			pb = vec2_type(xa_in,ya_in);
			qb = apply_mapping<I>(pb);
			jb = get_jacobian_matrix<I>(pb,qb);
			bbox.extend(qb);
			for(int ix = 1;ix <= nx;++ix)
				{
				double x_in = xa_in + dx_in * double(ix) / nx;
				double y_in = ya_in;
// original
				pa = pb;qa = qb;ja = jb;
				pb = vec2_type(x_in,y_in);
				qb = apply_mapping<I>(pb);
				jb = get_jacobian_matrix<I>(pb,qb);
				extend_box_by_spline<I>(pa,pb,qa,qb,ja,jb,bbox);
				bbox.extend(qb);
				}
// yb_in
			pb = vec2_type(xa_in,yb_in);
			qb = apply_mapping<I>(pb);
			jb = get_jacobian_matrix<I>(pb,qb);
			bbox.extend(qb);
			for(int ix = 1;ix <= nx;++ix)
				{
				double x_in = xa_in + dx_in * double(ix) / nx;
				double y_in = yb_in;
// copy-paste
				pa = pb;qa = qb;ja = jb;
				pb = vec2_type(x_in,y_in);
				qb = apply_mapping<I>(pb);
				jb = get_jacobian_matrix<I>(pb,qb);
				extend_box_by_spline<I>(pa,pb,qa,qb,ja,jb,bbox);
				bbox.extend(qb);
				}
// xa_in
			pb = vec2_type(xa_in,ya_in);
			qb = apply_mapping<I>(pb);
			jb = get_jacobian_matrix<I>(pb,qb);
			bbox.extend(qb);
			for(int iy = 1;iy <= ny;++iy)
				{
				double x_in = xa_in;
				double y_in = ya_in + dy_in * double(iy) / ny;
// copy-paste
				pa = pb;qa = qb;ja = jb;
				pb = vec2_type(x_in,y_in);
				qb = apply_mapping<I>(pb);
				jb = get_jacobian_matrix<I>(pb,qb);
				extend_box_by_spline<I>(pa,pb,qa,qb,ja,jb,bbox);
				bbox.extend(qb);
				}
// xb_in
			pb = vec2_type(xb_in,ya_in);
			qb = apply_mapping<I>(pb);
			jb = get_jacobian_matrix<I>(pb,qb);
			bbox.extend(qb);
			for(int iy = 1;iy <= ny;++iy)
				{
				double x_in = xb_in;
				double y_in = ya_in + dy_in * double(iy) / ny;
// copy-paste
				pa = pb;qa = qb;ja = jb;
				pb = vec2_type(x_in,y_in);
				qb = apply_mapping<I>(pb);
				jb = get_jacobian_matrix<I>(pb,qb);
				extend_box_by_spline<I>(pa,pb,qa,qb,ja,jb,bbox);
				bbox.extend(qb);
				}
			xa_out = bbox.a()[0];
			ya_out = bbox.a()[1];
			xb_out = bbox.b()[0];
			yb_out = bbox.b()[1];
			}
	protected:
//! The Mutex, used in derived classes, initialized in constructor.
#ifdef _WINDOWS
		CRITICAL_SECTION _critsec;
#else
		pthread_mutex_t _mutex;
#endif

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
//! @brief This method should be invoked by the derived classes in initializeParameters().
//! It write error messages to stderr if parameters are not initialized properly.
//! We check filmback width, filmback height, focal length and pixel aspect.
//! Focus distance is not checked currently.
		void check_builtin_parameters()
			{
			if(w_fb_cm() == 0)
				{
                // OBQ:
                AiMsgError("ldp_builtin: filmback width is 0.");
                // LDPK:
                // std::cerr << "ldp_builtin: filmback width is 0." << std::endl;
                }
			if(h_fb_cm() == 0)
				{
                // OBQ:
                AiMsgError("ldp_builtin: filmback height is 0.");
                // LDPK:
                // std::cerr << "ldp_builtin: filmback height is 0." << std::endl;
                }
			if(fl_cm() == 0)
				{
                // OBQ:
                AiMsgError("ldp_builtin: focal length is 0.");
                // LDPK:
                // std::cerr << "ldp_builtin: focal length is 0." << std::endl;
                }
			if(pa() == 0)
				{
                // OBQ:
                AiMsgError("ldp_builtin: pixel aspect is 0.");
                // LDPK:
                std::cerr << "ldp_builtin: pixel aspect is 0." << std::endl;
                }
			_r_fb_cm = sqrt(w_fb_cm() * w_fb_cm() + h_fb_cm() * h_fb_cm()) / 2.0;
			}
//@}
		vec2_type map_unit_to_dn(const vec2_type& p_unit) const
			{
			vec2_type p_cm((p_unit[0] - 1.0/2.0) * w_fb_cm() - x_lco_cm(),(p_unit[1] - 1.0/2.0) * h_fb_cm() - y_lco_cm());
			return p_cm / r_fb_cm();
			}
		vec2_type map_dn_to_unit(const vec2_type& p_dn) const
			{
			vec2_type p_cm(p_dn * r_fb_cm());
			p_cm += vec2_type(w_fb_cm() / 2 + x_lco_cm(),h_fb_cm() / 2 + y_lco_cm());
			return vec2_type(p_cm[0] / w_fb_cm(),p_cm[1] / h_fb_cm());
			}
		void lock()
			{
#ifdef _WINDOWS
			EnterCriticalSection(&_critsec);
#else
			pthread_mutex_lock(&this->_mutex);
#endif
			}
		void unlock()
			{
#ifdef _WINDOWS
			LeaveCriticalSection(&_critsec);
#else
			pthread_mutex_unlock(&this->_mutex);
#endif
			}
	public:
		ldp_builtin():_uptodate_lut(false)
			{
#ifdef _WINDOWS
			InitializeCriticalSection(&_critsec);
#else
			int r = pthread_mutex_init(&_mutex,NULL);
			if(r)
				{
                    // OBK:
                    AiMsgError("ldpk::ldp_builtin::pthread_mutex_init: %s",strerror(r));
                    // LDPK:
                    // std::cerr << "ldpk::ldp_builtin::pthread_mutex_init: " << strerror(r) << std::endl;
                }
#endif
// We set all built-in parameters to zero. That way we can reliably find out
// if the application initializies properly (i.e. pixel aspect needs to be set).
			for(int i = 0;i < get_num_builtin_parameters();++i)
				{ _value_builtin[i] = 0; }
			}
		virtual ~ldp_builtin()
			{
#ifdef _WINDOWS
			DeleteCriticalSection(&_critsec);
#else
			int r = pthread_mutex_destroy(&_mutex);
			if(r)
				{
                // OBQ:
                AiMsgError("ldpk::ldp_builtin::pthread_mutex_destroy: ", strerror(r));
                // LDPK:
                // std::cerr << "ldpk::ldp_builtin::pthread_mutex_destroy: " << strerror(r) << std::endl;
                }
#endif
			}
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

		double r_fb_cm() const
			{ return _r_fb_cm; }
//@}
		virtual void getBoundingBoxUndistort(double xa_in,double ya_in,double xb_in,double yb_in,double& xa_out,double& ya_out,double& xb_out,double& yb_out,int nx,int ny)
			{ get_bounding_box<1>(xa_in,ya_in,xb_in,yb_in,xa_out,ya_out,xb_out,yb_out,nx,ny); }
		virtual void getBoundingBoxDistort(double xa_in,double ya_in,double xb_in,double yb_in,double& xa_out,double& ya_out,double& xb_out,double& yb_out,int nx,int ny)
			{ get_bounding_box<0>(xa_in,ya_in,xb_in,yb_in,xa_out,ya_out,xb_out,yb_out,nx,ny); }
		};

// Built-in parameter names. Do not change.
	template <class VEC2,class MAT2>
	const char* ldp_builtin<VEC2,MAT2>::_para_builtin[7] = {
		"tde4_focal_length_cm",
		"tde4_filmback_width_cm",
		"tde4_filmback_height_cm",
		"tde4_lens_center_offset_x_cm",
		"tde4_lens_center_offset_y_cm",
		"tde4_pixel_aspect",
		"tde4_custom_focus_distance_cm"
		};
	template <class VEC2,class MAT2>
	para_builtin_enum ldp_builtin<VEC2,MAT2>::find_builtin_parameter_by_name(const char* identifier) const
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
	template <class VEC2,class MAT2>
	bool ldp_builtin<VEC2,MAT2>::set_builtin_parameter_value(const char* identifier,double v)
		{
		int i = find_builtin_parameter_by_name(identifier);
		if(i < 0)
			{ return false; }
		if(_value_builtin[i] != v)
			{
// Whenever a parameter is changed, the lookup table becomes obsolete.
			no_longer_uptodate_lut();
			_value_builtin[i] = v;
			}
		return true;
		}
	template <class VEC2,class MAT2>
	void ldp_builtin<VEC2,MAT2>::update_lut()
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
