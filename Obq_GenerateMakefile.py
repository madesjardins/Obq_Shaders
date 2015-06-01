# -*- coding:Utf8 -*-

#######################################################
# Author : Marc-Antoine Desjardins (ObliqueFX)
# 20145
#
# This Python script generates the Makefiles for Obq_Shaders
#######################################################

import sys
import glob
import shutil

###########
# CONSTANT
###########
PATH_ARNOLD = "/home/madesjardins/Arnold"
PATH_GLM = "/home/madesjardins/Softwares/glm"
sitoa2arnold = {'v209': "4.0.15.1", 'v210': "4.0.16.6", 'v300': "4.1.3.5", 'v301': "4.2.0.5", 'v302': "4.2.1.3", 'v303': "4.2.2.0", 'v304': "4.2.3.1", 'v305': "4.2.4.3", 'v306': "4.2.6.2"}
ignoredFiles = [".","..","src/Obq_Simbiont.cpp","src/kettle/kettle_bake.cpp",]

def printHelp():
	global sitoa2arnold
	print('Help : Simply call "python Obq_GenerateMakefile {linux|macosx} vXXX ARNOLD_PARENT_PATH GLM_PATH" to create Linux or OSX Makefiles.')
	versions = sitoa2arnold.keys()
	versions.sort()
	print('Allowed versions : '+str(versions))

def writeMakefileHeader(file, systemBuild, version, arnoldPath, glmPath):

	extension = "so"
	cpp = "g++-4.1"
	linkFlags = "-shared -Wl,--no-undefined"
	if systemBuild == "osx":
		extension = "dylib"
		cpp = "g++"
		linkFlags = "-dynamiclib -Wl"
	
	file.write("EXT = " + extension +"""
OBQVERSION = """+version+"""
TARGETNAME = Obq_Shaders__Core__$(OBQVERSION)
SRCPATH = ../src
BINPATH = ../bin/$(OBQVERSION)
GLMPATH = """+glmPath+"""
ARNOLD = """+arnoldPath+"/Arnold-"+str(sitoa2arnold[version])+"""-linux
INCLUDES = -I$(ARNOLD)/include -I. -I$(SRCPATH) -I$(GLMPATH) -I$(SRCPATH)/dte -I$(SRCPATH)/ldpk 
LINKINCLUDES = -L$(ARNOLD)/bin
CPP = """+cpp+"""
COMMONFLAGS = 
OPTIMIZE = -O3 -Wall
# create dependency files: -MMD 
CPPFLAGS = -c -fmessage-length=0 -fPIC -MMD $(OPTIMIZE) $(COMMONFLAGS) $(INCLUDES)
LINKFLAGS = """ + linkFlags+"""
LIBS = -lai
LINKING = $(LINKINCLUDES) $(LIBS)

all: $(BINPATH)/$(TARGETNAME).$(EXT)

# clean/clobber

clean:
	-rm -f *~ *.o *.d

clobber: clean
	-rm -f $(BINPATH)/$(TARGETNAME).$(EXT)

# common

""")

	
# returns a list of all cpp files to compile (maximum depth = 1)
def getAllSourceFiles():
	global ignoredFiles
	
	# dependencyDictionnary
	allFiles = []
	#list all files in source
	for fof in glob.glob("src/*"):
		fof = fof.replace("\\","/")
		if fof in ignoredFiles:
			continue
			
		# check folder
		if fof.find(".") == -1:
			for fof2 in glob.glob(fof+"/*") :
				fof2 = fof2.replace("\\","/")
				if fof2 in ignoredFiles:
					continue
				if ".cpp" in fof2:
					allFiles.append(fof2)
		elif ".cpp" in fof:
			allFiles.append(fof)
			
	return allFiles

def main():
	global sitoa2arnold
	global PATH_ARNOLD
	global PATH_GLM
	
	if len(sys.argv) < 3:
		printHelp()
		quit()
		
	systemBuild = sys.argv[1]
	if systemBuild not in ["linux","macosx"]:
		printHelp()
		quit()
	
	version = sys.argv[2]
	
	if sitoa2arnold.has_key(version):
		
		#-- Get all non Ignored files, then keep cpp only
		cppFiles = getAllSourceFiles()
				
		#--- Write Makefiles Headers 
		filename = systemBuild+"/Makefile"
		with open(filename,'w') as file:
		
			arnoldPath = PATH_ARNOLD
			glmPath = PATH_GLM
			if len(sys.argv) == 5:
				arnoldPath = sys.argv[3]
				glmPath = sys.argv[4]
				
			writeMakefileHeader(file,systemBuild, version, arnoldPath, glmPath )
		
			targetDep = ""
			for f in cppFiles:
				nameIndex = f.rfind("/")
				name_o = f[nameIndex+1:-4]+".o"
				targetDep += " "+name_o
				name_cpp = f.replace("src/","$(SRCPATH)/")
				file.write(name_o+":\n")
				file.write("\t$(CPP) $(CPPFLAGS) "+name_cpp+"\n")
				file.write("\n")
			
			file.write("$(BINPATH)/$(TARGETNAME).$(EXT):"+targetDep+"\n")
			file.write("\t$(CPP) -o $(BINPATH)/$(TARGETNAME).$(EXT) $(LINKFLAGS)"+targetDep+" $(LINKING)\n")
		
			file.close()
			
			
	else:
		print('Version "'+str(version)+'" doesn\'t exist in dictionary.')
		printHelp()
		quit()
	

if __name__ == "__main__":
    main()