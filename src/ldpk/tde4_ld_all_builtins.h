// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.7


#pragma once

#include <ldpk/tde4_ldp_classic_3de_mixed.h>
#include <ldpk/tde4_ldp_anamorphic_deg_6.h>
#include <ldpk/tde4_ldp_radial_deg_8.h>
#include <ldpk/tde4_ldp_radial_decentered_deg_4_cylindric.h>
#include <ldpk/tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy.h>

typedef tde4_ldp_classic_3de_mixed<ldpk::vec2d,ldpk::mat2d> tde4_ldp_classic_3de_mixed_builtin;
typedef tde4_ldp_anamorphic_deg_6<ldpk::vec2d,ldpk::mat2d> tde4_ldp_anamorphic_deg_6_builtin;
typedef tde4_ldp_radial_deg_8<ldpk::vec2d,ldpk::mat2d> tde4_ldp_radial_deg_8_builtin;
typedef tde4_ldp_radial_decentered_deg_4_cylindric<ldpk::vec2d,ldpk::mat2d> tde4_ldp_radial_decentered_deg_4_cylindric_builtin;
typedef tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy<ldpk::vec2d,ldpk::mat2d> tde4_ldp_anamorphic_deg_4_rotate_squeeze_xy_builtin;
