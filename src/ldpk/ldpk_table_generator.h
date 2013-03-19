// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011 - Science-D-Visions. Current version: 1.1


#ifndef ldpk_table_generator_sdv
#define ldpk_table_generator_sdv

#include "ldpk/tde4_ld_plugin.h"
#include <iostream>

//! @file ldpk_table_generator.h
//! Generate table data from plugin for visualization

namespace ldpk
	{
//! @brief Generate table data from plugin for visualization.
	class table_generator
		{
	private:
		tde4_ld_plugin* _model;
		int _nx,_ny;
	public:
		table_generator():_model(0),_nx(21),_ny(15)
			{ }
		void set_model(tde4_ld_plugin* m)
			{ _model = m; }
//! Number of grid points per dimension.
		void set_grid_size(int nx,int ny)
			{ _nx = nx;_ny = ny; }
//! Generate table data (use command 'plot "file" with vector' in gnuplot)
		void out_gnuplot(std::ostream& out);
//! Generate svg-grid, including styles
		void out_svg(std::ostream& out);
		};
	}

#endif
