EXT = dylib
OBQVERSION = a4_2_11_0
OBQVERSIONBIN = a4_2_11_x
TARGETNAME = Obq_Shaders__Core__$(OBQVERSION)
SRCPATH = ../src
BINPATH = ../bin/$(OBQVERSIONBIN)
GLMPATH = /Users/madesjardins/Softwares/glm/
ARNOLD = /Users/madesjardins/Arnold//Arnold-4.2.11.0-darwin
INCLUDES = -I$(ARNOLD)/include -I. -I$(SRCPATH) -I$(GLMPATH) -I$(SRCPATH)/dte -I$(SRCPATH)/ldpk 
LINKINCLUDES = -L$(ARNOLD)/bin
CPP = g++
COMMONFLAGS = 
OPTIMIZE = -O3 -Wall
# create dependency files: -MMD 
CPPFLAGS = -c -fmessage-length=0 -fPIC -MMD $(OPTIMIZE) $(COMMONFLAGS) $(INCLUDES)
LINKFLAGS = -dynamiclib -Wl
LIBS = -lai
LINKING = $(LINKINCLUDES) $(LIBS)

all: $(BINPATH)/$(TARGETNAME).$(EXT)

# clean/clobber

clean:
	-rm -f *~ *.o *.d

clobber: clean
	-rm -f $(BINPATH)/$(TARGETNAME).$(EXT)

# common

Grid2DAS.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/kettle/Grid2DAS.cpp

KettleBaker.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/kettle/KettleBaker.cpp

O_ISBrdf.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/O_ISBrdf.cpp

Obq_AngularCamera.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_AngularCamera.cpp

Obq_Atmosphere.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Atmosphere.cpp

Obq_Bend4Stereo.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Bend4Stereo.cpp

Obq_CameraProjectionSpace.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_CameraProjectionSpace.cpp

Obq_ChangeRange.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ChangeRange.cpp

Obq_Color.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Color.cpp

Obq_ColorspaceConverter.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ColorspaceConverter.cpp

Obq_Environment.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Environment.cpp

Obq_Etching.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Etching.cpp

Obq_Fresnel.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Fresnel.cpp

Obq_Hair.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Hair.cpp

Obq_IESLightFilter.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_IESLightFilter.cpp

Obq_KettleUVStereoLens.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_KettleUVStereoLens.cpp

Obq_LensDistortion.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_LensDistortion.cpp

Obq_LightSaturationFilter.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_LightSaturationFilter.cpp

Obq_MeasuredMaterial.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MeasuredMaterial.cpp

Obq_MessageGetBool.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetBool.cpp

Obq_MessageGetColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetColor.cpp

Obq_MessageGetFlt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetFlt.cpp

Obq_MessageGetInt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetInt.cpp

Obq_MessageGetVector.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetVector.cpp

Obq_MessageSetBool.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetBool.cpp

Obq_MessageSetColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetColor.cpp

Obq_MessageSetFlt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetFlt.cpp

Obq_MessageSetInt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetInt.cpp

Obq_MessageSetVector.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetVector.cpp

Obq_MessageStoreBool.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreBool.cpp

Obq_MessageStoreColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreColor.cpp

Obq_MessageStoreFlt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreFlt.cpp

Obq_MessageStoreInt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreInt.cpp

Obq_MessageStoreVector.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreVector.cpp

Obq_NodeInfo.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_NodeInfo.cpp

Obq_Opacity.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Opacity.cpp

Obq_RandomColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RandomColor.cpp

Obq_RandomID.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RandomID.cpp

Obq_RayDepth.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RayDepth.cpp

Obq_RGBComEmit.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RGBComEmit.cpp

Obq_RGBComLightFilter.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RGBComLightFilter.cpp

Obq_Root2Tip.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Root2Tip.cpp

Obq_SetAlpha.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SetAlpha.cpp

Obq_Shaders__Core.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shaders__Core.cpp

Obq_Shadow.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shadow.cpp

Obq_SpectrumColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SpectrumColor.cpp

Obq_StereoLens.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_StereoLens.cpp

Obq_SurfaceEnvironment.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SurfaceEnvironment.cpp

Obq_Thickness.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Thickness.cpp

Obq_Toon.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Toon.cpp

Obq_ToonSimple.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ToonSimple.cpp

Obq_TransformShadingPoint.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_TransformShadingPoint.cpp

Obq_TransformUV.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_TransformUV.cpp

Obq_UVRemapLensDistortion.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_UVRemapLensDistortion.cpp

Obq_VolumeThickness.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_VolumeThickness.cpp

spectrum.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/pbrt/spectrum.cpp

$(BINPATH)/$(TARGETNAME).$(EXT): Grid2DAS.o KettleBaker.o O_ISBrdf.o Obq_AngularCamera.o Obq_Atmosphere.o Obq_Bend4Stereo.o Obq_CameraProjectionSpace.o Obq_ChangeRange.o Obq_Color.o Obq_ColorspaceConverter.o Obq_Environment.o Obq_Etching.o Obq_Fresnel.o Obq_Hair.o Obq_IESLightFilter.o Obq_KettleUVStereoLens.o Obq_LensDistortion.o Obq_LightSaturationFilter.o Obq_MeasuredMaterial.o Obq_MessageGetBool.o Obq_MessageGetColor.o Obq_MessageGetFlt.o Obq_MessageGetInt.o Obq_MessageGetVector.o Obq_MessageSetBool.o Obq_MessageSetColor.o Obq_MessageSetFlt.o Obq_MessageSetInt.o Obq_MessageSetVector.o Obq_MessageStoreBool.o Obq_MessageStoreColor.o Obq_MessageStoreFlt.o Obq_MessageStoreInt.o Obq_MessageStoreVector.o Obq_NodeInfo.o Obq_Opacity.o Obq_RandomColor.o Obq_RandomID.o Obq_RayDepth.o Obq_RGBComEmit.o Obq_RGBComLightFilter.o Obq_Root2Tip.o Obq_SetAlpha.o Obq_Shaders__Core.o Obq_Shadow.o Obq_SpectrumColor.o Obq_StereoLens.o Obq_SurfaceEnvironment.o Obq_Thickness.o Obq_Toon.o Obq_ToonSimple.o Obq_TransformShadingPoint.o Obq_TransformUV.o Obq_UVRemapLensDistortion.o Obq_VolumeThickness.o spectrum.o
	$(CPP) -o $(BINPATH)/$(TARGETNAME).$(EXT) $(LINKFLAGS) Grid2DAS.o KettleBaker.o O_ISBrdf.o Obq_AngularCamera.o Obq_Atmosphere.o Obq_Bend4Stereo.o Obq_CameraProjectionSpace.o Obq_ChangeRange.o Obq_Color.o Obq_ColorspaceConverter.o Obq_Environment.o Obq_Etching.o Obq_Fresnel.o Obq_Hair.o Obq_IESLightFilter.o Obq_KettleUVStereoLens.o Obq_LensDistortion.o Obq_LightSaturationFilter.o Obq_MeasuredMaterial.o Obq_MessageGetBool.o Obq_MessageGetColor.o Obq_MessageGetFlt.o Obq_MessageGetInt.o Obq_MessageGetVector.o Obq_MessageSetBool.o Obq_MessageSetColor.o Obq_MessageSetFlt.o Obq_MessageSetInt.o Obq_MessageSetVector.o Obq_MessageStoreBool.o Obq_MessageStoreColor.o Obq_MessageStoreFlt.o Obq_MessageStoreInt.o Obq_MessageStoreVector.o Obq_NodeInfo.o Obq_Opacity.o Obq_RandomColor.o Obq_RandomID.o Obq_RayDepth.o Obq_RGBComEmit.o Obq_RGBComLightFilter.o Obq_Root2Tip.o Obq_SetAlpha.o Obq_Shaders__Core.o Obq_Shadow.o Obq_SpectrumColor.o Obq_StereoLens.o Obq_SurfaceEnvironment.o Obq_Thickness.o Obq_Toon.o Obq_ToonSimple.o Obq_TransformShadingPoint.o Obq_TransformUV.o Obq_UVRemapLensDistortion.o Obq_VolumeThickness.o spectrum.o $(LINKING)
