// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013,2014 - Science-D-Visions. Current version: 1.8.1


#ifndef ldpk_ldpk_sdv
#define ldpk_ldpk_sdv

#include <ldpk/ldpk_error.h>
#include <ldpk/ldpk_vec2d.h>
#include <ldpk/ldpk_lookup_table.h>
#include <ldpk/ldpk_plugin_loader.h>
#include <ldpk/ldpk_model_parser.h>
#include <ldpk/ldpk_table_generator.h>
#include <ldpk/ldpk_generic_distortion_base.h>
#include <ldpk/ldpk_radial_decentered_distortion.h>
#include <ldpk/ldpk_generic_radial_distortion.h>
#include <ldpk/ldpk_generic_anamorphic_distortion.h>
#include <ldpk/ldpk_classic_3de_mixed_distortion.h>

//! @mainpage LDPK - The Lens Distortion Plugin Kit
//!
//! @section About
//! The current version of LDPK is 1.8.1
//! The <a href="../../tex/ldpk.pdf">PDF-document</a> contains some information on the underlying mathematics
//! of the built-in lens distortion models.
//!
//! @section Files
//! A list of files can be found <a href="../../misc/list_of_files.html">here</a>. We left out
//! some of the less interesting files in the documentation directories.
//!
//! @section Binaries
//! As of version 1.6 of the $LDPK, only 64-bit architectures are supported.
//! For each of the supported platforms there is a simple program in <code style="color:#a00">$LDPK/bin</code>
//! which you can use in order to check, if your plugin is accepted by the executable.
//! They have been compiled on the same platforms using the same compiler as 3DE4.
//! These binaries are:
//! - <code>tde4_plugin_info.linux</code>
//! - <code>tde4_plugin_info.osx</code>
//!
//! @section Python
//! As of version 1.6 of the LDPK there are python bindings for the five built-in lens distortion models (OSX, Linux):
//! - <code>$LDPK/compiled/ldpk/&lt;arch&gt;/python/lens_distortion_plugins.so</code>
//!
//! In <code style="color:#a00">$LDPK/source</code> there is a source
//! <code>tde4_lens_distortion_plugins.module.C</code> for building python bindings.
//! If the pre-compiled version does not meet your requirements you can compile them yourself.
//! In order to do this please run the appropriate script make_python_bindings.&lt;arch&gt;.csh in <code style="color:#a00">$LDPK/script</code>.
//! The make-script will create a file named <code>$LDPK/python/&lt;arch&gt;/lens_distortion_plugins.so</code>.
//! If you move this to an appropriate location, you can import this in python e.g. with
//! 
//! \code
//! import lens_distortion_plugins as ldp
//! \endcode
//!
//! Please have a look at the sample programs in <code style="color:#a00">$LDPK/python</code>
//! and read <a href="../../sphinx/build/html/index.html">this document</a>.
//!
//! @section Nuke
//! As of version 1.7 of the LDPK there are compiled plugins based on the five built-in lens distortion models
//! for various versions of Nuke (from <a href="http://www.thefoundry.co.uk/products/nuke-product-family/nuke/">The Foundry</a>)
//! In the documentation and in 3DE4 we will refer to these nodes as the <span style="font-weight:bold;color:#a00">LD_3DE4 Lens Distortion Nodes</span>
//! or briefly the LD_3DE4 nodes.
//! - <code>$LDPK/compiled/nuke/&lt;arch&gt;/Nuke&lt;X.Y&gt;/LD_3DE4_Anamorphic_Degree_6.[so,dylib,dll]</code>
//! - <code>$LDPK/compiled/nuke/&lt;arch&gt;/Nuke&lt;X.Y&gt;/LD_3DE4_Anamorphic_Standard_Degree_4.[so,dylib,dll]</code>
//! - <code>$LDPK/compiled/nuke/&lt;arch&gt;/Nuke&lt;X.Y&gt;/LD_3DE4_Radial_Fisheye_Degree_8.[so,dylib,dll]</code>
//! - <code>$LDPK/compiled/nuke/&lt;arch&gt;/Nuke&lt;X.Y&gt;/LD_3DE4_Radial_Standard_Degree_4.[so,dylib,dll]</code>
//! - <code>$LDPK/compiled/nuke/&lt;arch&gt;/Nuke&lt;X.Y&gt;/LD_3DE_Classic_LD_Model.[so,dylib,dll]</code>
//!
//! They are fairly easy to install, you simply copy them to your Nuke plugin directory.
//! Currently, Nuke versions 6.2, 6.3, 7.0, 8.0 and 9.0 on Linux and OSX are supported (Windows: 6.3, 7.0, 8.0, 9.0).
//! If you need to compile the Nuke plugins yourself, please have a look at the scripts
//! <code>make_nuke_nodes.&lt;arch&gt;.csh</code> in <code style="color:#a00">$LDPK/script</code>.
//! Please insert the paths to your Nuke installation directory and consult The Foundry's documentation
//! for the appropriate compiler version.
//! If you miss support for any version of Nuke here (but not prior to 6.2) please let us know, we will add this
//! if possible.
//! \image html menu_nuke_ld_3de4.png
//! We have modified <code style="color:#a00">$NUKE/plugins/menu.py</code> as in the following code snippet, but you may handle this according to your needs.
//! Also, it might be helpful to consult Nuke's documentation about adding commands to the GUI.
//! \code
//! nuke.menu("Nodes").addCommand("3DE4/LD_3DE4_Anamorphic_Standard_Degree_4", "nuke.createNode('LD_3DE4_Anamorphic_Standard_Degree_4')")
//! nuke.menu("Nodes").addCommand("3DE4/LD_3DE4_Anamorphic_Degree_6", "nuke.createNode('LD_3DE4_Anamorphic_Degree_6')")
//! nuke.menu("Nodes").addCommand("3DE4/LD_3DE4_Radial_Standard_Degree_4", "nuke.createNode('LD_3DE4_Radial_Standard_Degree_4')")
//! nuke.menu("Nodes").addCommand("3DE4/LD_3DE4_Radial_Fisheye_Degree_8", "nuke.createNode('LD_3DE4_Radial_Fisheye_Degree_8')")
//! nuke.menu("Nodes").addCommand("3DE4/LD_3DE_Classic_LD_Model", "nuke.createNode('LD_3DE_Classic_LD_Model')")
//! \endcode
//! In <code style="color:#a00">$LDPK/3de4/python</code> you will find an export script named
//! <center>
//! <code>export_nuke_LD_3DE4_Lens_Distortion_Node.py</code>
//! </center>
//! which you run from within 3DE4. In order to do this please copy it into your 3DE4 installation, at <code style="color:#a00">$TDE4/sys_data/py_scripts/</code>.
//! In 3DE4's GUI the script appears in <code>Main Window -> 3DE4 -> File -> Export</code>
//!
//! \image html menu_3de4_file_export.png
//!
//! In addition to the seven built-in parameters and the model-specific parameters the plugins have four parameters
//! which allow to specify the field of view as defined in 3DE4:
//! - "fov left [unit coord]"
//! - "fov bottom [unit coord]"
//! - "fov right [unit coord]"
//! - "fov top [unit coord]"
//!
//! Default values are 0,0,1,1. By definition, the field of view in 3DE4 corresponds to the area represented by
//! filmback width and height. In contrast to previous solutions, field of view is now taken into account.
//!
//! @section Classes
//! The LDPK contains various classes. It is <b>important to know</b>
//! that most of them are <b>not required</b> in order to develop a plugin.
//! In the following we will classify all classes according to their importance.
//! Note that you only need one API class in order to develop a plugin with at least
//! basic functionality.
//! - <b>Important</b>
//!   - tde4_ld_plugin - This is the base class of all lens distortion plugins for 3DE4.
//!     Historically, this is declared in the global namespace.
//!
//! - <b>Useful</b>
//!   - ldpk::ldp_builtin - This class may serve as a base class
//!     for your distortion model. It manages the seven built-in parameters
//!     and the lookup-table that might be helpful for calculating the inverse
//!     distortion function. It also contains the mutex used in the derived classes
//!	and methods for mapping between coordinate systems.
//!	In the 3DE4 plugin classes tde4_ldp_anamorphic_deg_6
//!     and tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy you see, how the lookup-table
//!     is used (see methods distort()).
//!
//! - <b>Nice to have</b> - Once you have developed a plugin
//!   you probably want to test it. These classes will help you to do so,
//!   but alternatively you can also copy your plugin to 3DE4's plugin directory
//!   and test it directly with 3DE4.
//!   - ldpk::plugin_loader - Load a plugin
//!   - ldpk::model_parser - A convenient way to set parameters in the plugin from a data stream.
//!   - ldpk::table_generator - Visualization with gnuplot
//!
//! - <b>Self consistency</b> - Some of our classes
//!   require two-dimensional vector and matrix classes.
//!   Here are two classes which do the job, but you might
//!   replace them by math classes used in your organization.
//!   - ldpk::vec2d
//!   - ldpk::mat2d
//!
//! - <b>3DE4-built-in lens distortin models</b>
//!   - tde4_ldp_radial_decentered_deg_4_cylindric - The standard for non-anamorphic lenses.
//!     Compensates for displaced lenses and optionally for beam splitter artefacts in
//!     beam splitter based stereo rigs.
//!   - tde4_ldp_radial_deg_8 - A high degree radial distortion model which combines an degree-8
//!     radial model with fisheye correction.
//!   - tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy - This model should be the standard for anamorphic lenses.
//!     It has a degree-4 anamorphic model and compensates for unprecisely mounted anamorphic lenses.
//!   - tde4_ldp_anamorphic_deg_6 - A simple degree-6 anamorphic model.
//!   - tde4_ldp_classic_3de_mixed - The classic mixed degree-2 / degree-4 model. Usually, radial-decentered-deg-4-cylincdric
//!     should be used instead of this one. We keep this in the release for compatibility.
//!
//! - <b>Baseclass for Nuke plugins</b>
//!   - nuke_ld_3de4_base - The baseclass for developing Nuke plugins from LDPK lens distortion classes.
//!     In <code style="color:#a00">$LDPK/source</code>
//!     you will find the sources for building Nuke plugins around 3DE4 lens distortion plugins.
//!     Take any of these as a starting point for your own Nuke plugin, but note that we do not directly
//!     derive from <b>tde4_ld_plugin</b> but from <b>ldpk::ldp_builtin</b>, which has certain properties
//!     we need for all our lens distortion models.
//!
//!
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
