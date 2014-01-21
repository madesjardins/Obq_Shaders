// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.7


#ifndef ldpk_lookup_table_sdv
#define ldpk_lookup_table_sdv

#include <vector>
#include <iostream>

//! @file ldpk_lookup_table.h

namespace ldpk
	{
//! @brief Use your own two-dimensional double-valued vector type as VEC2.
	template <class VEC2>
	class lookup_table
		{
	public:
//! This class
		typedef lookup_table<VEC2> this_type;
//! Your two-dimensional double-valued vector type
		typedef VEC2 vec2_type;
	private:
//! Internal. Type for storing the lut-value and a flag for the paint-algo.
		struct sample_type
			{
			bool done;
			vec2_type value;
			sample_type():done(false)
				{ }
			};

		typedef typename std::vector<sample_type> samples_type;
		
		samples_type _samples;
		int _nx,_ny;
		vec2_type _lc_fov;
// Index nearest to lc.
		int _ix_lc,_iy_lc;
// Counter for the paint algorithm.
		int _k;
// Current index in paint algorithm.
		int _ix_current,_iy_current;

		void clip_index(int& ix,int& iy) const
			{
			if(ix < 0) ix = 0;
			if(ix >= _nx) ix = _nx - 1;
			if(iy < 0) iy = 0;
			if(iy >= _ny) iy = _ny - 1;
			}
		const sample_type& get_sample(int ix,int iy) const
			{ return _samples.at(ix + _nx * iy); }
		sample_type& get_sample(int ix,int iy)
			{ return _samples.at(ix + _nx * iy); }
//! Convert normalized FOV-coordinates to indices, nearest match.
		void calc_index(const vec2_type& p_fov,int& ix,int& iy) const
			{
			ix = int(floor(p_fov[0] * (_nx - 1) + 0.5));
			iy = int(floor(p_fov[1] * (_ny - 1) + 0.5));
			}
//! Convert index to normalized FOV-coordinates.
		vec2_type calc_position(int ix,int iy) const
			{
			return vec2_type(double(ix) / (_nx - 1),double(iy) / (_ny - 1));
			}
	public:
//! @name Initializing lookup table
// @{
		lookup_table():_nx(0),_ny(0),_lc_fov(.5,.5)
			{ }
//! Initialize a grid with nx * ny sample points. Implies reset().
		void init(int nx,int ny)
			{
			_nx = nx;
			_ny = ny;
			_samples.resize(_nx * _ny);
			reset();
			}
//! @brief Lens Center in normalized FOV-coordinates.
//! LC is the fixed point of all distortion models. We use it
//! as starting point for iterating through the grid. Default is (0.5,0.5).
		void set_lens_center_fov(const vec2_type& lc_fov)
			{
			_lc_fov = lc_fov;
			}
// @}

//! @name Methods for building the lookup table
//! @brief We denote the original unwarped point by p and the warped point by q.
//! The lookup table is constructed like this:
//! You implement a loop using reset() and next(). The table
//! will give you the next position for warping by get_p_current_fov().
//! Then call the warp method in your distortion model instance using the
//! initial value obtained by get_current_initial_value(). Finally enter the result
//! (i.e. the warped position) in the lookup table by calling set_value()
// @{
//! Use reset() and next() to build a loop for generating positions. See example. 
		void reset();
//! False means no value to be generated, loop is done. See example.
		bool next();
//! Current x-position while painting
		int get_ix_current() const
			{ return _ix_current; }
//! Current y-position while painting
		int get_iy_current() const
			{ return _iy_current; }
//! @brief Current position in normalized FOV-coordinates. This is the position
//! you want to warp next, while building the lookup table.
		vec2_type get_p_current_fov() const
			{ return calc_position(_ix_current,_iy_current); }
//! Appropriate initial value for calculating warp at current position obtained with get_p_current_fov().
		vec2_type get_current_initial_value() const;
//! @brief Set value for given index pair. Index pairs out of bounds will raise an exception.
//! Use this method to insert the warped position from warping p_current_fov.
		void set_q_current_fov(const vec2_type& q_fov)
			{ _samples.at(_ix_current + _nx * _iy_current).value = q_fov; }
// @}

//! @name Methods for taking benefit from the lookup table
//! @brief When ::tde4_ld_plugin::distort(x0,y0,x1,y1) (i.e. without initial_values) is called,
//! the lookup table is consulted. An initial value is obtained from get_initial_value().
//! Then tde4_ld_plugin::distort(x0,y0,x1,y1) calls tde4_ld_plugin::distort(x0,y0,x1s,y1s,x1,y1)
//! where (x1s,y1s) is the initial value.
// @{
//! Direct access to table data, raises exception if index is out of bounds.
		const vec2_type& get_value(int ix,int iy) const
			{
			return _samples.at(ix + _nx * iy).value;
			}
//! @brief Once you have built the lookup table, use this method to extract
//! an initial value from lookup table for the position specified in FOV-coordinates.
//! Positions out of bounds will be clipped.
		const vec2_type& get_value(const vec2_type& p_fov) const
			{
			int ix,iy;
			calc_index(p_fov,ix,iy);
			clip_index(ix,iy);
			return _samples[ix + _nx * iy].value;
			}
//! @brief Currently same as get_value(), but we might add more sophisticated
//! methods to generate an initial value later.
		const vec2_type& get_initial_value(const vec2_type& p_fov) const
			{
			return get_value(p_fov);
			}
// @}

//! @name Methods for debugging and testing
// @{
//! Visualizing the paint algo for debugging purposes
		std::ostream& out_state(std::ostream& cout) const
			{
			for(int iy = _ny - 1;iy >= 0;--iy)
				{
				for(int ix = 0;ix < _nx;++ix)
					{
					if((ix == _ix_current) && (iy == _iy_current))
						{
						cout << 'o';
						}
					else
						{
						cout << (get_sample(ix,iy).done ? '*' : '.');
						}
					}
				cout << std::endl;
				}
			return cout;
			}
//! @brief Output as a vector field for gnuplot
//! Use command "plot 'test.i'  with vector" or similar in gnuplot.
		std::ostream& out_gnuplot(std::ostream& cout) const
			{
			for(int iy = 0;iy < _ny;++iy)
				{
				for(int ix = 0;ix < _nx;++ix)
					{
					vec2_type p,q,d;
					p = calc_position(ix,iy);
					q = get_value(ix,iy);
					d = q - p;
					cout << p[0] << " " << p[1] << " " << d[0] << " " << d[1] << std::endl;
					}
				}
			return cout;
			}
// @}
		};

	template <class VEC2>
	void lookup_table<VEC2>::reset()
		{
		for(typename samples_type::iterator i = _samples.begin();i != _samples.end();++i)
			{
			i->done = false;
			}
// Reset counter
		_k = 0;
// Calculate index of Lens Center
		calc_index(_lc_fov,_ix_lc,_iy_lc);
		}
	template <class VEC2>
	bool lookup_table<VEC2>::next()
		{
// Each position painted
		if(_k >= _samples.size()) return false;

		int kx = _k % _nx;
		if(kx <= _ix_lc)
			{
// going left
			_ix_current = _ix_lc - kx;
			}
		else
			{
// going right
			_ix_current = _ix_lc + (kx - _ix_lc);
			}
		int ky = _k / _nx;
		if(ky <= _iy_lc)
			{
// going down
			_iy_current = _iy_lc - ky;
			}
		else
			{
// going up
			_iy_current = _iy_lc + (ky - _iy_lc);
			}
		get_sample(_ix_current,_iy_current).done = true;
		++_k;
		return true;
		}
	template <class VEC2>
	VEC2 lookup_table<VEC2>::get_current_initial_value() const
		{
		if(_iy_current == _iy_lc)
			{
			if(_ix_current == _ix_lc)
				{
				return calc_position(_ix_current,_iy_current);
				}
			else if(_ix_current < _ix_lc)
				{
				return get_value(_ix_current + 1,_iy_current);
				}
			else
				{
				return get_value(_ix_current - 1,_iy_current);
				}
			}
		else if(_iy_current < _iy_lc)
			{
			return get_value(_ix_current,_iy_current + 1);
			}
		else
			{
			return get_value(_ix_current,_iy_current - 1);
			}
		}
	}

#endif
