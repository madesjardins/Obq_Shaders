EXT = so
OBQVERSION = a4_2_10_0
TARGETNAME = Obq_Shaders__Core__$(OBQVERSION)
SRCPATH = ../src
BINPATH = ../bin/$(OBQVERSION)
GLMPATH = /home/madesjardins/Softwares/glm
ARNOLD = /home/madesjardins/Arnold/Arnold-4.2.10.0-linux
INCLUDES = -I$(ARNOLD)/include -I. -I$(SRCPATH) -I$(GLMPATH) -I$(SRCPATH)/dte -I$(SRCPATH)/ldpk 
LINKINCLUDES = -L$(ARNOLD)/bin
CPP = g++-4.8
COMMONFLAGS = 
OPTIMIZE = -O3 -Wall
# create dependency files: -MMD 
CPPFLAGS = -c -fmessage-length=0 -fPIC -MMD $(OPTIMIZE) $(COMMONFLAGS) $(INCLUDES)
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

Obq_TransformShadingPoint.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_TransformShadingPoint.cpp

Obq_MessageGetColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetColor.cpp

Obq_MessageSetFlt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetFlt.cpp

Obq_CameraProjectionSpace.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_CameraProjectionSpace.cpp

Obq_Shadow.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shadow.cpp

Obq_RGBComLightFilter.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RGBComLightFilter.cpp

Obq_Shaders__Core.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shaders__Core.cpp

Obq_MessageSetInt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetInt.cpp

Obq_Color.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Color.cpp

Obq_Etching.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Etching.cpp

Obq_MessageGetBool.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetBool.cpp

Obq_SurfaceEnvironment.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SurfaceEnvironment.cpp

Obq_MessageSetVector.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetVector.cpp

Obq_MessageGetVector.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetVector.cpp

Obq_MeasuredMaterial.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MeasuredMaterial.cpp

Obq_Hair.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Hair.cpp

Obq_AngularCamera.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_AngularCamera.cpp

Obq_MessageStoreColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreColor.cpp

Obq_NodeInfo.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_NodeInfo.cpp

Obq_ColorspaceConverter.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ColorspaceConverter.cpp

Obq_MessageStoreBool.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreBool.cpp

Obq_RandomColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RandomColor.cpp

Obq_Toon.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Toon.cpp

Obq_UVRemapLensDistortion.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_UVRemapLensDistortion.cpp

Obq_Root2Tip.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Root2Tip.cpp

Obq_RandomID.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RandomID.cpp

spectrum.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/pbrt/spectrum.cpp

Obq_VolumeThickness.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_VolumeThickness.cpp

O_ISBrdf.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/O_ISBrdf.cpp

Obq_Bend4Stereo.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Bend4Stereo.cpp

Obq_MessageStoreInt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreInt.cpp

Obq_RGBComEmit.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RGBComEmit.cpp

Grid2DAS.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/kettle/Grid2DAS.cpp

KettleBaker.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/kettle/KettleBaker.cpp

Obq_Atmosphere.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Atmosphere.cpp

Obq_LightSaturationFilter.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_LightSaturationFilter.cpp

Obq_SpectrumColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SpectrumColor.cpp

Obq_MessageGetInt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetInt.cpp

Obq_Thickness.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Thickness.cpp

Obq_ToonSimple.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ToonSimple.cpp

Obq_RayDepth.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_RayDepth.cpp

Obq_TransformUV.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_TransformUV.cpp

Obq_MessageGetFlt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageGetFlt.cpp

Obq_Fresnel.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Fresnel.cpp

Obq_MessageStoreVector.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreVector.cpp

Obq_ChangeRange.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ChangeRange.cpp

Obq_MessageSetBool.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetBool.cpp

Obq_MessageSetColor.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageSetColor.cpp

Obq_StereoLens.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_StereoLens.cpp

Obq_MessageStoreFlt.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_MessageStoreFlt.cpp

Obq_KettleUVStereoLens.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_KettleUVStereoLens.cpp

Obq_Environment.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Environment.cpp

Obq_LensDistortion.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_LensDistortion.cpp

Obq_SetAlpha.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_SetAlpha.cpp

Obq_Opacity.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Opacity.cpp

Obq_IESLightFilter.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_IESLightFilter.cpp

$(BINPATH)/$(TARGETNAME).$(EXT): Obq_TransformShadingPoint.o Obq_MessageGetColor.o Obq_MessageSetFlt.o Obq_CameraProjectionSpace.o Obq_Shadow.o Obq_RGBComLightFilter.o Obq_Shaders__Core.o Obq_MessageSetInt.o Obq_Color.o Obq_Etching.o Obq_MessageGetBool.o Obq_SurfaceEnvironment.o Obq_MessageSetVector.o Obq_MessageGetVector.o Obq_MeasuredMaterial.o Obq_Hair.o Obq_AngularCamera.o Obq_MessageStoreColor.o Obq_NodeInfo.o Obq_ColorspaceConverter.o Obq_MessageStoreBool.o Obq_RandomColor.o Obq_Toon.o Obq_UVRemapLensDistortion.o Obq_Root2Tip.o Obq_RandomID.o spectrum.o Obq_VolumeThickness.o O_ISBrdf.o Obq_Bend4Stereo.o Obq_MessageStoreInt.o Obq_RGBComEmit.o Grid2DAS.o KettleBaker.o Obq_Atmosphere.o Obq_LightSaturationFilter.o Obq_SpectrumColor.o Obq_MessageGetInt.o Obq_Thickness.o Obq_ToonSimple.o Obq_RayDepth.o Obq_TransformUV.o Obq_MessageGetFlt.o Obq_Fresnel.o Obq_MessageStoreVector.o Obq_ChangeRange.o Obq_MessageSetBool.o Obq_MessageSetColor.o Obq_StereoLens.o Obq_MessageStoreFlt.o Obq_KettleUVStereoLens.o Obq_Environment.o Obq_LensDistortion.o Obq_SetAlpha.o Obq_Opacity.o Obq_IESLightFilter.o
	$(CPP) -o $(BINPATH)/$(TARGETNAME).$(EXT) $(LINKFLAGS) Obq_TransformShadingPoint.o Obq_MessageGetColor.o Obq_MessageSetFlt.o Obq_CameraProjectionSpace.o Obq_Shadow.o Obq_RGBComLightFilter.o Obq_Shaders__Core.o Obq_MessageSetInt.o Obq_Color.o Obq_Etching.o Obq_MessageGetBool.o Obq_SurfaceEnvironment.o Obq_MessageSetVector.o Obq_MessageGetVector.o Obq_MeasuredMaterial.o Obq_Hair.o Obq_AngularCamera.o Obq_MessageStoreColor.o Obq_NodeInfo.o Obq_ColorspaceConverter.o Obq_MessageStoreBool.o Obq_RandomColor.o Obq_Toon.o Obq_UVRemapLensDistortion.o Obq_Root2Tip.o Obq_RandomID.o spectrum.o Obq_VolumeThickness.o O_ISBrdf.o Obq_Bend4Stereo.o Obq_MessageStoreInt.o Obq_RGBComEmit.o Grid2DAS.o KettleBaker.o Obq_Atmosphere.o Obq_LightSaturationFilter.o Obq_SpectrumColor.o Obq_MessageGetInt.o Obq_Thickness.o Obq_ToonSimple.o Obq_RayDepth.o Obq_TransformUV.o Obq_MessageGetFlt.o Obq_Fresnel.o Obq_MessageStoreVector.o Obq_ChangeRange.o Obq_MessageSetBool.o Obq_MessageSetColor.o Obq_StereoLens.o Obq_MessageStoreFlt.o Obq_KettleUVStereoLens.o Obq_Environment.o Obq_LensDistortion.o Obq_SetAlpha.o Obq_Opacity.o Obq_IESLightFilter.o $(LINKING)
