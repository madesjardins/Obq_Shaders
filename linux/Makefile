EXT = so
OBQVERSION = a5_0_0_2
OBQVERSIONBIN = a5_0_0_x
TARGETNAME = Obq_Shaders__Core__$(OBQVERSION)
SRCPATH = ../src
BINPATH = ../bin/$(OBQVERSIONBIN)
GLMPATH = /home/madesjardins/Softwares/glm
ARNOLD = /home/madesjardins/Arnold/Arnold-5.0.0.2-linux
INCLUDES = -I$(ARNOLD)/include -I. -I$(SRCPATH) -I$(GLMPATH) -I$(SRCPATH)/dte -I$(SRCPATH)/ldpk 
LINKINCLUDES = -L$(ARNOLD)/bin
CPP = g++
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

Obq_Shaders__Core5.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Shaders__Core5.cpp

Obq_Color5.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_Color5.cpp

Obq_ColorspaceConverter5.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_ColorspaceConverter5.cpp

Obq_LensDistortion5.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_LensDistortion5.cpp

Obq_StereoLens5.o:
	$(CPP) $(CPPFLAGS) $(SRCPATH)/Obq_StereoLens5.cpp

$(BINPATH)/$(TARGETNAME).$(EXT): Obq_Shaders__Core5.o Obq_Color5.o Obq_ColorspaceConverter5.o Obq_LensDistortion5.o Obq_StereoLens5.o
	$(CPP) -o $(BINPATH)/$(TARGETNAME).$(EXT) $(LINKFLAGS) Obq_Shaders__Core5.o Obq_Color5.o Obq_ColorspaceConverter5.o Obq_LensDistortion5.o Obq_StereoLens5.o $(LINKING)
