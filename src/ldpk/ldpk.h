// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.4


#ifndef ldpk_ldpk_sdv
#define ldpk_ldpk_sdv

#include <ldpk/ldpk_error.h>
#include <ldpk/ldpk_vec2d.h>
#include <ldpk/ldpk_lookup_table.h>
#include <ldpk/ldpk_plugin_loader.h>
#include <ldpk/ldpk_model_parser.h>
#include <ldpk/ldpk_table_generator.h>
#include <ldpk/ldpk_generic_distortion_base.h>
#include <ldpk/ldpk_example_radial_distortion.h>
#include <ldpk/ldpk_radial_decentered_distortion.h>
#include <ldpk/ldpk_generic_radial_distortion.h>
#include <ldpk/ldpk_generic_anamorphic_distortion.h>
#include <ldpk/ldpk_classic_3de_mixed_distortion.h>

//! @mainpage LDPK - The Lens Distortion Plugin Kit
//!
//! @section Documents
//! The <a href="../../tex/ldpk.pdf">PDF-document</a>.
//!
//! @section Binaries
//! For each of the supported platforms there is a simple program in $LDPK/bin
//! which you can use in order to check, if your plugin is accepted by the executable.
//! They have been compiled on the same platforms using the same compiler as 3DE4.
//! These binaries are:
//! - tde4_plugin_info.linux64
//! - tde4_plugin_info.linux
//! - tde4_plugin_info.osx64
//! - tde4_plugin_info.osx
//!
//! @section Classes
//! The LDPK contains various classes. It is <b>important to know</b>
//! that most of them are <b>not required</b> in order to develop a plugin.
//! In the following we will classify all classes according to their importance.
//! Note that you only need one API class in order to develop a plugin with at least
//! basic functionality.
//! - <b>Obligatory</b>
//!   - tde4_ld_plugin - This is the base class of all lens distortion plugins for 3DE4.
//!     Historically, this is declared in the global namespace.
//!
//! - <b>Recommended</b>
//!   - ldpk::generic_distortion_base - This class serves as a base class
//!     for your distortion model. It provides methods for calculating the inverse
//!     function.
//!   - Distortion classes - These are function object classes which represent the distortion model.
//!     - ldpk::radial_decentered_distortion - An implementation of 3DE4's built-in model "Radial, Standard - Degree 8".
//!     - ldpk::generic_radial_distortion - An implementation of 3DE4's built-in model "Radial, Fisheye - Degree 8"
//!     - ldpk::generic_anamorphic_distortion - An implementation of 3DE4's built-in model "Anamorphic - Degree 6"
//!     - ldpk::classic_3de_mixed_distortion - An implementation of 3DE4's built-in model "3DE Classic LD Model"
//!   - Plugin classes - For each distortion class there is a plugin class, which converts from camera coordinates
//!     to unit coordinates and takes care about <b>lens center offset</b>. These are the classes 3DE4 communicates with.
//!     For developing a compositing node, we recommend to build the node around one (or more) of these classes,
//!     because they all have the same interface and are completely equipped with lookup tables for inverting the model function.
//!     - tde4_ldp_radial_decentered_deg_4_cylindric
//!     - tde4_ldp_radial_deg_8
//!     - tde4_ldp_anamorphic_deg_6
//!     - tde4_ldp_classic_3de_mixed
//!   - ldpk::ldp_builtin - This class contains the seven built-in parameters
//!     and provides a lookup-table mechanism for inverting the distortion function.
//!   - ldpk::lookup_table - A class that provides a lookup-table for the inverse
//!     distortion function. This class is used by ldpk::ldp_builtin.
//!
//! - <b>Nice to have</b> - Once you have developed a plugin
//!   you probably want to test it. These classes will help you to do so,
//!   but alternatively you can also copy your plugin to 3DE4's plugin directory
//!   and test it directly with 3DE4.
//!   - ldpk::plugin_loader - Load a plugin
//!   - ldpk::model_parser - A convenient way to set parameters in the plugin from a data stream.
//!   - ldpk::table_generator - Visualization with gnuplot
//!
//! - <b>Examples</b> - In the document, we illustrate the development
//!   of a 3DE4 lens distortion plugin by means of these classes.
//!   - ldpk::example_radial_distortion - A radial distortion function class
//!   - tde4_ldp_example_radial_deg_8 - A plugin for radial distortion of degree eight.
//!
//! - <b>Self consistency</b> - Some of our classes including the examples
//!   require two-dimensional vector and matrix classes.
//!   Here are two classes which do the job, but you might
//!   replace them by math classes used in your organization.
//!   - ldpk::vec2d
//!   - ldpk::mat2d

//! @file ldpk.h
//! @brief Convenience header for use in the LDPK

//! @brief The namespace of (most of the) things
//! related to the Lens Distortion Plugin Kit.
namespace ldpk
	{
//! text representation of parameter types
	inline const char* text(::tde4_ldp_ptype ptype)
		{
		switch(ptype)
			{
			case TDE4_LDP_STRING: return "string";
			case TDE4_LDP_DOUBLE: return "double";
			case TDE4_LDP_INT: return "int";
			case TDE4_LDP_FILE: return "file";
			case TDE4_LDP_TOGGLE: return "toggle";
			case TDE4_LDP_ADJUSTABLE_DOUBLE: return "adjustable double";
			}
		return "<error>";
		}
	}

#endif
