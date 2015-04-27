EXT = so
OBQVERSION = v304
TARGETNAME = Obq_Shaders__Core__$(OBQVERSION)
SRCPATH = ../src
BINPATH = ../bin/$(OBQVERSION)
GLMPATH = /home/madesjardins/Softwares/glm
ARNOLD = /home/madesjardins/Arnold/Arnold-4.2.3.1-linux
INCLUDES = -I$(ARNOLD)/include -I. -I$(SRCPATH) -I$(GLMPATH)
LINKINCLUDES = -L$(ARNOLD)/bin
CPP = g++-4.1
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

Obq_Shaders__Core.o: \
$(SRCPATH)/Obq_Shaders__Core.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shaders__Core.cpp

# shared

spectrum.o: \
$(SRCPATH)/pbrt/spectrum.cpp \
$(SRCPATH)/pbrt/spectrum.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/pbrt/spectrum.cpp -o spectrum.o

O_ISBrdf.o: \
$(SRCPATH)/O_ISBrdf.cpp \
$(SRCPATH)/O_ISBrdf.h \
$(SRCPATH)/O_BrdfRead.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/O_ISBrdf.cpp -o O_ISBrdf.o

shared: \
spectrum.o \
O_ISBrdf.o

SHARED = \
spectrum.o \
O_ISBrdf.o

# Obq_AngularCamera

Obq_AngularCamera.o: \
$(SRCPATH)/Obq_AngularCamera.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_AngularCamera.cpp 

# Obq_Atmosphere

Obq_Atmosphere.o: \
$(SRCPATH)/Obq_Atmosphere.cpp \
$(SRCPATH)/Obq_Atmosphere.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Atmosphere.cpp

# Obq_Bend4Stereo

Obq_Bend4Stereo.o: \
$(SRCPATH)/Obq_Bend4Stereo.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Bend4Stereo.cpp

# Obq_CameraProjectionSpace

Obq_CameraProjectionSpace.o: \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_CameraProjectionSpace.cpp

# Obq_ChangeRange

Obq_ChangeRange.o: \
$(SRCPATH)/Obq_ChangeRange.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ChangeRange.cpp

# Obq_Color

Obq_Color.o: \
$(SRCPATH)/Obq_Color.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Color.cpp

# Obq_ColorspaceConverter

Obq_ColorspaceConverter.o: \
$(SRCPATH)/Obq_ColorspaceConverter.cpp \
$(SRCPATH)/O_Colorspaces.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ColorspaceConverter.cpp


# Obq_Environment

Obq_Environment.o: \
$(SRCPATH)/Obq_Environment.cpp \
$(SRCPATH)/Obq_Environment.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Environment.cpp

# Obq_Etching

Obq_Etching.o: \
$(SRCPATH)/Obq_Etching.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Etching.cpp


# Obq_Fresnel

Obq_Fresnel.o: \
$(SRCPATH)/Obq_Fresnel.cpp \
$(SRCPATH)/Obq_Fresnel.h \
$(SRCPATH)/pbrt/spectrum.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Fresnel.cpp

# Obq_Hair

Obq_Hair.o: \
$(SRCPATH)/Obq_Hair.cpp \
$(SRCPATH)/Obq_Hair.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Hair.cpp

# Obq_IESLightFilter

Obq_IESLightFilter.o: \
$(SRCPATH)/Obq_IESLightFilter.cpp \
$(SRCPATH)/yafaray/yafaray_ies_parser.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_IESLightFilter.cpp

# Obq_LensDistortion

Obq_LensDistortion.o: \
$(SRCPATH)/Obq_LensDistortion.cpp \
$(SRCPATH)/Obq_LensDistortion.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_LensDistortion.cpp

# Obq_LightSaturationFilter

Obq_LightSaturationFilter.o: \
$(SRCPATH)/Obq_LightSaturationFilter.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_LightSaturationFilter.cpp

# Obq_MeasuredMaterial

Obq_MeasuredMaterial.o: \
$(SRCPATH)/Obq_MeasuredMaterial.cpp \
$(SRCPATH)/Obq_MeasuredMaterial.h \
$(SRCPATH)/O_ISBrdf.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MeasuredMaterial.cpp

# Obq_MessageGetFlt

Obq_MessageGetFlt.o: \
$(SRCPATH)/Obq_MessageGetFlt.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetFlt.cpp

# Obq_MessageSetFlt

Obq_MessageSetFlt.o: \
$(SRCPATH)/Obq_MessageSetFlt.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetFlt.cpp

# Obq_MessageStoreFlt

Obq_MessageStoreFlt.o: \
$(SRCPATH)/Obq_MessageStoreFlt.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreFlt.cpp

# Obq_Opacity

Obq_Opacity.o: \
$(SRCPATH)/Obq_Opacity.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Opacity.cpp

# Obq_RandomColor

Obq_RandomColor.o: \
$(SRCPATH)/Obq_RandomColor.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RandomColor.cpp

# Obq_RandomID

Obq_RandomID.o: \
$(SRCPATH)/Obq_RandomID.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RandomID.cpp

# Obq_RayDepth

Obq_RayDepth.o: \
$(SRCPATH)/Obq_RayDepth.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RayDepth.cpp

# Obq_RGBComEmit

Obq_RGBComEmit.o: \
$(SRCPATH)/Obq_RGBComEmit.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RGBComEmit.cpp

# Obq_RGBComLightFilter

Obq_RGBComLightFilter.o: \
$(SRCPATH)/Obq_RGBComLightFilter.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RGBComLightFilter.cpp

# Obq_Root2Tip

Obq_Root2Tip.o: \
$(SRCPATH)/Obq_Root2Tip.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Root2Tip.cpp

# Obq_SetAlpha

Obq_SetAlpha.o: \
$(SRCPATH)/Obq_SetAlpha.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SetAlpha.cpp

# Obq_Shadow

Obq_Shadow.o: \
$(SRCPATH)/Obq_Shadow.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shadow.cpp


# Obq_SpectrumColor

Obq_SpectrumColor.o: \
$(SRCPATH)/Obq_SpectrumColor.cpp \
$(SRCPATH)/Obq_SpectrumColor.h \
$(SRCPATH)/pbrt/spectrum.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SpectrumColor.cpp

# Obq_StereoLens

Obq_StereoLens.o: \
$(SRCPATH)/Obq_StereoLens.cpp \
$(SRCPATH)/Obq_StereoLens.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_StereoLens.cpp

# Obq_SurfaceEnvironment

Obq_SurfaceEnvironment.o: \
$(SRCPATH)/Obq_SurfaceEnvironment.cpp \
$(SRCPATH)/Obq_SurfaceEnvironment.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SurfaceEnvironment.cpp

# Obq_Thickness

Obq_Thickness.o: \
$(SRCPATH)/Obq_Thickness.cpp \
$(SRCPATH)/Obq_Thickness.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Thickness.cpp

# Obq_Toon

Obq_Toon.o: \
$(SRCPATH)/Obq_Toon.cpp \
$(SRCPATH)/Obq_Toon.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Toon.cpp

# Obq_ToonSimple

Obq_ToonSimple.o: \
$(SRCPATH)/Obq_ToonSimple.cpp \
$(SRCPATH)/Obq_ToonSimple.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ToonSimple.cpp

# Obq_TransformShadingPoint

Obq_TransformShadingPoint.o: \
$(SRCPATH)/Obq_TransformShadingPoint.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_TransformShadingPoint.cpp

# Obq_TransformUV

Obq_TransformUV.o: \
$(SRCPATH)/Obq_TransformUV.cpp \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_TransformUV.cpp

# Obq_UVRemapLensDistortion

Obq_UVRemapLensDistortion.o: \
$(SRCPATH)/Obq_UVRemapLensDistortion.cpp \
$(SRCPATH)/Obq_LensDistortion.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_UVRemapLensDistortion.cpp

# Obq_VolumeThickness

Obq_VolumeThickness.o: \
$(SRCPATH)/Obq_VolumeThickness.cpp \
$(SRCPATH)/Obq_VolumeThickness.h \
$(SRCPATH)/O_Common.h
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_VolumeThickness.cpp

# Obq_Shaders__Core

$(BINPATH)/$(TARGETNAME).$(EXT): \
Obq_AngularCamera.o \
Obq_Atmosphere.o \
Obq_Bend4Stereo.o \
Obq_CameraProjectionSpace.o \
Obq_ChangeRange.o \
Obq_Color.o \
Obq_ColorspaceConverter.o \
Obq_Environment.o \
Obq_Etching.o \
Obq_Fresnel.o \
Obq_Hair.o \
Obq_IESLightFilter.o \
Obq_LensDistortion.o \
Obq_LightSaturationFilter.o \
Obq_MeasuredMaterial.o \
Obq_MessageGetFlt.o \
Obq_MessageSetFlt.o \
Obq_MessageStoreFlt.o \
Obq_Opacity.o \
Obq_RandomColor.o \
Obq_RandomID.o \
Obq_RayDepth.o \
Obq_RGBComEmit.o \
Obq_RGBComLightFilter.o \
Obq_Root2Tip.o \
Obq_SetAlpha.o \
Obq_Shadow.o \
Obq_SpectrumColor.o \
Obq_StereoLens.o \
Obq_SurfaceEnvironment.o \
Obq_Thickness.o \
Obq_Toon.o \
Obq_ToonSimple.o \
Obq_TransformShadingPoint.o \
Obq_TransformUV.o \
Obq_UVRemapLensDistortion.o \
Obq_VolumeThickness.o \
shared \
Obq_Shaders__Core.o
	$(CPP) -o $(BINPATH)/$(TARGETNAME).$(EXT) $(LINKFLAGS) \
Obq_AngularCamera.o \
Obq_Atmosphere.o \
Obq_Bend4Stereo.o \
Obq_CameraProjectionSpace.o \
Obq_ChangeRange.o \
Obq_Color.o \
Obq_ColorspaceConverter.o \
Obq_Environment.o \
Obq_Etching.o \
Obq_Fresnel.o \
Obq_Hair.o \
Obq_IESLightFilter.o \
Obq_LensDistortion.o \
Obq_LightSaturationFilter.o \
Obq_MeasuredMaterial.o \
Obq_MessageGetFlt.o \
Obq_MessageSetFlt.o \
Obq_MessageStoreFlt.o \
Obq_Opacity.o \
Obq_RandomColor.o \
Obq_RandomID.o \
Obq_RayDepth.o \
Obq_RGBComEmit.o \
Obq_RGBComLightFilter.o \
Obq_Root2Tip.o \
Obq_SetAlpha.o \
Obq_Shadow.o \
Obq_SpectrumColor.o \
Obq_StereoLens.o \
Obq_SurfaceEnvironment.o \
Obq_Thickness.o \
Obq_Toon.o \
Obq_ToonSimple.o \
Obq_TransformShadingPoint.o \
Obq_TransformUV.o \
Obq_UVRemapLensDistortion.o \
Obq_VolumeThickness.o \
$(SHARED) \
Obq_Shaders__Core.o \
$(LINKING)

