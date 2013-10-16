EXT = so
TARGETNAME = Obq_Shaders_v2XX__Lite_NoHair
SRCPATH = ../src
BINPATH = ../bin
GLMPATH = path_to_glm
ARNOLD = path_to_Arnold
INCLUDES = -I$(ARNOLD)/include -I. -I$(SRCPATH) -I$(GLMPATH)
LINKINCLUDES = -L$(ARNOLD)/bin
CPP = g++
COMMONFLAGS = 
OPTIMIZE = -O3 -Wall
# create dependency files: -MMD 
CPPFLAGS = -c -fmessage-length=0 -fPIC -MMD \
$(OPTIMIZE) $(COMMONFLAGS) $(INCLUDES)
LINKFLAGS = -shared -Wl,--no-undefined
LIBS = -lai
LINKING = $(LINKINCLUDES) $(LIBS)

all: $(BINPATH)/$(TARGETNAME).$(EXT)

# clean/clobber

clean:
	-rm -f *~ *.o *.d

clobber: clean
	-rm -f $(BINPATH)/$(TARGETNAME).$(EXT)

# common

Obq_Shaders.o: \
$(SRCPATH)/Obq_Shaders.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shaders.cpp

# shared

spectrum.o: \
$(SRCPATH)/pbrt/spectrum.cpp \
$(SRCPATH)/pbrt/spectrum.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/pbrt/spectrum.cpp -o spectrum.o

shared: \
spectrum.o

SHARED = \
spectrum.o

# Obq_AngularCamera

Obq_AngularCamera.o: \
$(SRCPATH)/Obq_AngularCamera.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_AngularCamera.cpp

# Obq_Atmosphere

Obq_Atmosphere.o: \
$(SRCPATH)/Obq_Atmosphere.cpp \
$(SRCPATH)/Obq_Atmosphere.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Atmosphere.cpp

# Obq_Bend4Stereo

Obq_Bend4Stereo.o: \
$(SRCPATH)/Obq_Bend4Stereo.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Bend4Stereo.cpp

# Obq_ChangeRange

Obq_ChangeRange.o: \
$(SRCPATH)/Obq_ChangeRange.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ChangeRange.cpp

# Obq_Environment

Obq_Environment.o: \
$(SRCPATH)/Obq_Environment.cpp \
$(SRCPATH)/Obq_Environment.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Environment.cpp

# Obq_Fresnel

Obq_Fresnel.o: \
$(SRCPATH)/Obq_Fresnel.cpp \
$(SRCPATH)/Obq_Fresnel.h \
$(SRCPATH)/pbrt/spectrum.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Fresnel.cpp

# Obq_IESLightFilter

Obq_IESLightFilter.o: \
$(SRCPATH)/Obq_IESLightFilter.cpp \
$(SRCPATH)/yafaray/yafaray_ies_parser.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_IESLightFilter.cpp

# Obq_MeasuredMaterial

Obq_MeasuredMaterial.o: \
$(SRCPATH)/Obq_MeasuredMaterial.cpp \
$(SRCPATH)/Obq_MeasuredMaterial.h \
$(SRCPATH)/ObqISBrdf.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MeasuredMaterial.cpp

ObqISBrdf.o: \
$(SRCPATH)/ObqISBrdf.cpp \
$(SRCPATH)/ObqISBrdf.h \
$(SRCPATH)/Obq_BrdfRead.h \
glm/glm.hpp \
glm/core/_fixes.hpp \
glm/core/setup.hpp \
glm/core/_detail.hpp \
glm/core/_vectorize.hpp \
glm/core/type.hpp \
glm/core/type_half.hpp \
glm/core/type_half.inl \
glm/core/type_float.hpp \
glm/core/type_int.hpp \
glm/core/type_gentype.hpp \
glm/core/type_size.hpp \
glm/core/type_vec1.hpp \
glm/core/type_vec.hpp \
glm/core/_swizzle.hpp \
glm/core/_swizzle_func.hpp \
glm/core/type_vec1.inl \
glm/core/type_vec2.hpp \
glm/core/type_vec2.inl \
glm/core/type_vec3.hpp \
glm/core/type_vec3.inl \
glm/core/type_vec4.hpp \
glm/core/type_vec4.inl \
glm/core/type_mat2x2.hpp \
glm/core/type_mat.hpp \
glm/core/type_mat2x2.inl \
glm/core/type_mat2x3.hpp \
glm/core/type_mat2x3.inl \
glm/core/type_mat2x4.hpp \
glm/core/type_mat2x4.inl \
glm/core/type_mat3x2.hpp \
glm/core/type_mat3x2.inl \
glm/core/type_mat3x3.hpp \
glm/core/type_mat3x3.inl \
glm/core/type_mat3x4.hpp \
glm/core/type_mat3x4.inl \
glm/core/type_mat4x2.hpp \
glm/core/type_mat4x2.inl \
glm/core/type_mat4x3.hpp \
glm/core/type_mat4x3.inl \
glm/core/type_mat4x4.hpp \
glm/core/type_mat4x4.inl \
glm/core/func_trigonometric.hpp \
glm/core/func_trigonometric.inl \
glm/core/func_exponential.hpp \
glm/core/func_exponential.inl \
glm/core/func_common.hpp \
glm/core/func_common.inl \
glm/core/func_packing.hpp \
glm/core/func_packing.inl \
glm/core/func_geometric.hpp \
glm/core/func_geometric.inl \
glm/core/func_matrix.hpp \
glm/core/func_matrix.inl \
glm/core/func_vector_relational.hpp \
glm/core/func_vector_relational.inl \
glm/core/func_integer.hpp \
glm/core/func_integer.inl \
glm/core/func_noise.hpp \
glm/core/func_noise.inl
	$(CPP) $(CPPFLAGS) $(SRCPATH)/ObqISBrdf.cpp

# Obq_Opacity

Obq_Opacity.o: \
$(SRCPATH)/Obq_Opacity.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Opacity.cpp

# Obq_RandomColor

Obq_RandomColor.o: \
$(SRCPATH)/Obq_RandomColor.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RandomColor.cpp

# Obq_RGBComEmit

Obq_RGBComEmit.o: \
$(SRCPATH)/Obq_RGBComEmit.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RGBComEmit.cpp

# Obq_RGBComLightFilter

Obq_RGBComLightFilter.o: \
$(SRCPATH)/Obq_RGBComLightFilter.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RGBComLightFilter.cpp

# Obq_Root2Tip

Obq_Root2Tip.o: \
$(SRCPATH)/Obq_Root2Tip.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Root2Tip.cpp

# Obq_Shadow
Obq_Shadow.o: \
$(SRCPATH)/Obq_Shadow.cpp \
$(SRCPATH)/Obq_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shadow.cpp

# Obq_StereoLens

Obq_StereoLens.o: \
$(SRCPATH)/Obq_StereoLens.cpp \
$(SRCPATH)/Obq_StereoLens.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_StereoLens.cpp

# Obq_SurfaceEnvironment

Obq_SurfaceEnvironment.o: \
$(SRCPATH)/Obq_SurfaceEnvironment.cpp \
$(SRCPATH)/Obq_SurfaceEnvironment.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SurfaceEnvironment.cpp

# Obq_Thickness

Obq_Thickness.o: \
$(SRCPATH)/Obq_Thickness.cpp \
$(SRCPATH)/Obq_Thickness.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Thickness.cpp

# Obq_TransformUV

Obq_TransformUV.o: \
$(SRCPATH)/Obq_TransformUV.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_TransformUV.cpp

# Obq_UVRemapLensDistortion

Obq_UVRemapLensDistortion.o: \
$(SRCPATH)/Obq_UVRemapLensDistortion.cpp \
$(SRCPATH)/ldpk/ldpk.h \
$(SRCPATH)/ldpk/ldpk_error.h \
$(SRCPATH)/ldpk/ldpk_vec2d.h \
$(SRCPATH)/ldpk/ldpk_lookup_table.h \
$(SRCPATH)/ldpk/ldpk_plugin_loader.h \
$(SRCPATH)/ldpk/tde4_ld_plugin.h \
$(SRCPATH)/ldpk/ldpk_model_parser.h \
$(SRCPATH)/ldpk/ldpk_table_generator.h \
$(SRCPATH)/ldpk/ldpk_generic_distortion_base.h \
$(SRCPATH)/ldpk/ldpk_example_radial_distortion.h \
$(SRCPATH)/ldpk/ldpk_radial_decentered_distortion.h \
$(SRCPATH)/ldpk/ldpk_generic_radial_distortion.h \
$(SRCPATH)/ldpk/ldpk_power_ct.h \
$(SRCPATH)/ldpk/ldpk_generic_anamorphic_distortion.h \
$(SRCPATH)/ldpk/ldpk_classic_3de_mixed_distortion.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_UVRemapLensDistortion.cpp

# Obq_VolumeThickness

Obq_VolumeThickness.o: \
$(SRCPATH)/Obq_VolumeThickness.cpp \
$(SRCPATH)/Obq_VolumeThickness.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_VolumeThickness.cpp

# Obq_Shaders

$(BINPATH)/$(TARGETNAME).$(EXT): \
Obq_AngularCamera.o \
Obq_Atmosphere.o \
Obq_Bend4Stereo.o \
Obq_ChangeRange.o \
Obq_Environment.o \
Obq_Fresnel.o \
Obq_IESLightFilter.o \
ObqISBrdf.o \
Obq_MeasuredMaterial.o \
Obq_Opacity.o \
Obq_RandomColor.o \
Obq_RGBComEmit.o \
Obq_RGBComLightFilter.o \
Obq_Root2Tip.o \
Obq_Shadow.o \
Obq_StereoLens.o \
Obq_SurfaceEnvironment.o \
Obq_Thickness.o \
Obq_TransformUV.o \
Obq_UVRemapLensDistortion.o \
Obq_VolumeThickness.o \
shared \
Obq_Shaders.o
	$(CPP) -o $(BINPATH)/$(TARGETNAME).$(EXT) $(LINKFLAGS) \
Obq_AngularCamera.o \
Obq_Atmosphere.o \
Obq_Bend4Stereo.o \
Obq_ChangeRange.o \
Obq_Environment.o \
Obq_Fresnel.o \
Obq_IESLightFilter.o \
ObqISBrdf.o \
Obq_MeasuredMaterial.o \
Obq_Opacity.o \
Obq_RandomColor.o \
Obq_RGBComEmit.o \
Obq_RGBComLightFilter.o \
Obq_Root2Tip.o \
Obq_Shadow.o \
Obq_StereoLens.o \
Obq_SurfaceEnvironment.o \
Obq_Thickness.o \
Obq_TransformUV.o \
Obq_UVRemapLensDistortion.o \
Obq_VolumeThickness.o \
$(SHARED) \
Obq_Shaders.o \
$(LINKING)
