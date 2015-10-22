# -*- coding:Utf8 -*-

#######################################################
# Author : Marc-Antoine Desjardins (ObliqueFX)
# 20145
#
# This Python script generates the Makefiles for Obq_Shaders
#######################################################

import os
import sys
import glob
import shutil

###########
# CONSTANT
###########
ignoredFiles = [".","..","src/Obq_Simbiont.cpp","src/kettle/kettle_bake.cpp",]

def printHelp():
	print('Help : python Obq_GenerateMakefile w.x.y.z [arnold_parent_path] [glm_path]\n For Linux or OSX Makefiles with Arnold version w.x.y.z.\nARNOLD_PARENT_PATH or GLM_PATH will be used if set as environment variables.')

def writeMakefileHeader(makefile, systemBuild, version, arnoldPath, glmPath):

	extension = "so"
	cpp = "g++"
	linkFlags = "-shared -Wl,--no-undefined"
	if systemBuild == "darwin":
		extension = "dylib"
		linkFlags = "-dynamiclib -Wl"
	# check if path exists
	arnold_version_path = arnoldPath+"/Arnold-"+version+"-"+systemBuild
	if not os.path.exists(arnold_version_path):
		print("Arnold version not found : '%s'."%arnold_version_path)
		quit()
	if not os.path.exists(glmPath):
		print("GLM path is not valid : '%s'"%glmPath)
		quit()
	
	# names
	obq_version = "a"+version.replace(".","_") 						# a4_2_10_1
	obq_version_bin = obq_version[0:obq_version.rindex("_")+1]+"x"	# a4_2_10_x

	makefile.write("EXT = " + extension +"""
OBQVERSION = """+obq_version+"""
OBQVERESIONBIN = """+obq_version_bin+"""
TARGETNAME = Obq_Shaders__Core__$(OBQVERSION)
SRCPATH = ../src
BINPATH = ../bin/$(OBQVERESIONBIN)
GLMPATH = """+glmPath+"""
ARNOLD = """+arnold_version_path+"""
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
	
	if len(sys.argv) < 2:
		print("Error : Version of build is missing.")
		printHelp()
		quit()
	
	# Check platform
	if "linux" in sys.platform:
		systemBuild = "linux"
		systemName = "linux"
	elif "darwin" in sys.platform:
		systemBuild = "darwin"
		systemName = "macosx"
	else:
		print("Error : This script is for use with linux or macosx only, this is '%s'."%sys.platform)
		quit()
	
	# environment variables or arguments
	if os.getenv('ARNOLD_PARENT_PATH') is not None:
		arnoldPath = os.getenv('ARNOLD_PARENT_PATH')
	elif len(sys.argv) > 2:
		arnoldPath = sys.argv[2]
	else:
		print("Error : No path for arnold specified as argument and no ARNOLD_PARENT_PATH environment variable set.")
		printHelp()
		quit()
		
	if os.getenv('GLM_PATH') is not None:
		glmPath = os.getenv('GLM_PATH')
	elif len(sys.argv) == 3:
		glmPath = sys.argv[2]
	elif len(sys.argv) == 4:
		glmPath = sys.argv[3]
	else:
		print("Error : No path for glm specified as argument and no GLM_PATH environment variable set.")
		printHelp()
		quit()
		
	# arnold version
	version = sys.argv[1]
			
	#-- Get all non Ignored files, then keep cpp only
	cppFiles = getAllSourceFiles()
			
	#--- Write Makefiles Headers 
	filename = systemName+"/Makefile"
	with open(filename,'w') as makefile:
	
			
		writeMakefileHeader(makefile,systemBuild, version, arnoldPath, glmPath )
	
		targetDep = ""
		for f in cppFiles:
			nameIndex = f.rfind("/")
			name_o = f[nameIndex+1:-4]+".o"
			targetDep += " "+name_o
			name_cpp = f.replace("src/","$(SRCPATH)/")
			makefile.write(name_o+":\n")
			makefile.write("\t$(CPP) $(CPPFLAGS) "+name_cpp+"\n")
			makefile.write("\n")
		
		makefile.write("$(BINPATH)/$(TARGETNAME).$(EXT):"+targetDep+"\n")
		makefile.write("\t$(CPP) -o $(BINPATH)/$(TARGETNAME).$(EXT) $(LINKFLAGS)"+targetDep+" $(LINKING)\n")
	
		makefile.close()
			
			
	#else:
		#print('Version "'+str(version)+'" doesn\'t exist in dictionary.')
		#printHelp()
		#quit()
	

if __name__ == "__main__":
    main()
