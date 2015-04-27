# -*- coding:Utf8 -*-

#######################################################
# Author : Marc-Antoine Desjardins (ObliqueFX)
# 2014
#
# This Python script generates the doc for Obq_Shaders
#######################################################

import glob
import shutil

#########
# "ENUM"
#########

# cat (categories): { home = 0, help = 1, documentation = 2, News = 3, download = 4, about = 5, install = 6 }
# proj (project)  : { Obq_Shaders = 0}

###########
# CONSTANT
###########

PATH_WEBSITE = "http://s3aws.obliquefx.com/public/shaders/"
PATH_HELPFILES = "help_files/"
PATH_BUILDS = "builds/"
PATH_RN = "release_notes/"
PATH_IMAGES = "images/"
PATH_HFIMAGES = PATH_HELPFILES+"images/"

#####################
#		TOOLS	  	#
#####################

def createImage(imageFilename, alternate = "", width = 0, height = 0):
	ret = '<img src="'+imageFilename+'" alt="'+alternate+'"'
	if width > 0:
		ret = ret + ' width = "' + str(width) + 'px"'
	if height > 0:
		ret = ret + ' height = "' + str(height) + 'px"'
	ret = ret + '>'
	return ret

def createLink(address, message):
	return '<a href="'+address+'">'+message+'</a>'
	
def writeRNLink(file, release):
	file.write('<a href="'+release+'.txt">'+release.replace("src/release_notes\\","")+'</a>\n')
	
def getPrefix(id,prefs):
	if id < len(prefs):
		return prefs[id]
	else:
		return prefs[0]

def writeComment(file, comment):
	file.write('<!-- '+comment+' -->\n')
		
def br(file):
	file.write('<br>\n')

def hr(file):
	file.write('<hr/>\n')

def cleanStr(str):
	return str.replace("\n","").lstrip().rstrip()

#####################
#		DIVS	  	#
#####################

#- TOP BANNER
def writeTopBanner(file, proj = 0, cat = 0):
	prefix=getPrefix(cat,[PATH_IMAGES,"../"+PATH_IMAGES])
	file.write('<div id="banner">'+createImage(prefix+"banner.jpg","Banner")+'</div>\n')
	
	
#----------------
#- TOP MENU
def writeTopMenu(file, proj = 0, cat = 0):
	file.write('<div id="topMenu">\n')
	file.write('<div id="topMenu-left">\n')
	prefix=getPrefix(cat,[PATH_IMAGES,"../"+PATH_IMAGES])
	file.write(createLink("http://www.obliquefx.com",createImage(prefix+"oblique.png","Oblique Logo",150)))
	file.write('</div>\n')
	file.write('<div id="topMenu-right">')
	file.write('<table border="0" cellpadding="9" cellspacing="0"><tr>\n')
	
	prefix=getPrefix(cat,["","../"])
	
	file.write('<td width="50"> </td>\n')
	file.write('<td>'+createLink(prefix+"index.html", "Home")+'</td>\n')
	file.write('<td>|</td>\n')
	file.write('<td>'+createLink(prefix+"news.html", "News")+'</td>\n')
	file.write('<td>|</td>\n')
	file.write('<td>'+createLink(prefix+"documentation.html", "Documentation")+'</td>\n')
	file.write('<td>|</td>\n')
	file.write('<td>'+createLink(prefix+"downloads.html", "Downloads")+'</td>\n')
	file.write('<td>|</td>\n')
	file.write('<td>'+createLink(prefix+"about.html", "About")+'</td>\n')
	file.write('<td width="50"> </td>\n')
	
	file.write('</tr></table>\n')
	file.write('</div>\n')
	#hr(file)
	file.write('</div>\n')

	
#----------------
#- TOP SIDEBAR
def writeContentSidebar(file, proj=0, cat=0):
	file.write('<div id="content-sidebar">\n')
	file.write('<p align="left">Node Reference</p>\n')
	file.write('<table border="0" cellpadding ="2" cellspacing ="0">\n')

	prefix=getPrefix(cat,[PATH_HELPFILES,""])
	
	#loop on all .help in src folder
	aHelpFiles = glob.glob("src/*.obqh")
	for helpfile in aHelpFiles:
		shaderName = helpfile.replace(".obqh","").replace("src\\","")
		file.write('<tr><td width="2"></td><td>'+createLink(prefix + shaderName+".html", shaderName)+'</td></tr>\n')

	file.write('</table>\n')
	file.write('</div>\n')	

	
#--------------------
#- MAIN CONTENT HELP
def writeContentHelp(file, proj=0, cat=0, helpfile = ""):
	file.write('<div id="content-help">')
	br(file)

	# switch categories
	
	# HOME
	if cat == 0:
		file.write('<h2>Obq_Shaders</h2>\n')
		file.write('<p>A collection of Arnold shaders created by Marc-Antoine Desjardins at '+createLink("http://www.obliquefx.com","Oblique FX")+'.</p>\n')
		file.write('<h3>News</h3>\n')
		with open("src/news.txt",'r') as f:
			lines = f.readlines()
			for line in lines:
				if line == "\n":
					break;
				elif len(line) > 2 and line[0] != "#":
					file.write('<p>'+cleanStr(line)+'</p>\n')
			f.close()
		br(file)
		file.write('<h3>Latest Build</h5>\n')
		# put latest build
		with open("src/info_builds.txt",'r') as f:
			lines = f.readlines()
			file.write('<p>'+createLink(PATH_WEBSITE+PATH_BUILDS+cleanStr(lines[0]), cleanStr(lines[1]).replace(" ","&nbsp;"))+" [" + cleanStr(lines[2])+ "] "+createLink(PATH_WEBSITE+PATH_RN+cleanStr(lines[3]),"Release Notes")+'</p>\n')
			f.close()
		file.write('<h6 style="text-align:left;color:darkred;">*Please read the provided documentation for more information on how to install them, thank you.*</h6>\n')
		
	# HELP FILES
	elif cat == 1:
		with open(helpfile,'r') as f:
			lines = f.readlines()
			fignum = 0
			for line in lines:
				if line[0] != "%":
					continue
					
				sublines = line.split(" :: ")
				
				#switch
				if sublines[0].find("%shadername") != -1:
					file.write('<h2>'+cleanStr(sublines[1])+'</h2>\n')
					br(file)
				elif sublines[0].find("%class") != -1 and len(sublines) == 2:
					file.write('<h5>Class</h5>\n')
					file.write('<p>'+cleanStr(sublines[1])+'</p>\n')
				elif sublines[0].find("%output") != -1 and len(sublines) == 2:
					br(file)
					file.write('<h5>Output</h5>\n')
					file.write('<p>'+cleanStr(sublines[1])+'</p>\n')
				elif sublines[0].find("%synopsisStart") != -1:
					br(file)
					file.write('<h5>Synopsis</h5>\n')
				elif sublines[0].find("%synopsisEnd") != -1:
					br(file)
				elif sublines[0].find("%image") != -1 and len(sublines) == 4:
					# first we need to copy
					br(file)
					relImagePath = cleanStr(sublines[1]).lstrip("src/")
					shutil.copyfile(cleanStr(sublines[1]), PATH_HELPFILES+relImagePath)
					file.write('<img src="'+relImagePath+'" alt="'+cleanStr(sublines[2])+'" ')
					woh = int(cleanStr(sublines[3]))
					if woh > 0:
						file.write('width='+str(woh))
					elif woh < 0:
						file.write('height='+str(-woh))
					file.write(' >\n')
					fignum+=1					
				elif sublines[0].find("%idesc") != -1 and len(sublines) == 2:
					file.write('<p class="idesc">Fig.'+str(fignum)+' : '+cleanStr(sublines[1])+'</p>\n')
				elif sublines[0].find("%pp") != -1 and len(sublines) == 2:
					file.write('<p>'+cleanStr(sublines[1])+'</p>\n')
				elif sublines[0].find("%usageStart") != -1:
					file.write('<h5>Usage</h5>\n')
					#br(file)
				elif sublines[0].find("%usageEnd") != -1:
					br(file)
					file.write('<hr/>\n')
					br(file)
				elif sublines[0].find("%br") != -1:
					file.write('<br/>\n')
				elif sublines[0].find("%param") != -1 and len(sublines) == 2:
					br(file)
					file.write('<h5>'+cleanStr(sublines[1])+'</h5>\n')
				elif sublines[0].find("%section") != -1 and len(sublines) == 2:
					br(file)
					hr(file)
					file.write('<h4>'+cleanStr(sublines[1])+'</h4>\n')
				elif sublines[0].find("%note") != -1 and len(sublines) == 2:	
					file.write('<p class="note" >Note : '+cleanStr(sublines[1])+'</p>\n')
				elif sublines[0].find("%warning") != -1 and len(sublines) == 2:	
					file.write('<p class="warning">Warning : '+cleanStr(sublines[1])+'</p>\n')
				elif sublines[0].find("%enumStart") != -1:	
					file.write('<ul>\n')
				elif sublines[0].find("%enumItem") != -1:	
					file.write('<li>'+cleanStr(sublines[1])+'</li>\n')
				elif sublines[0].find("%enumEnd") != -1:	
					file.write('</ul>\n')
					#br(file)
				elif sublines[0].find("%exampleStart") != -1:
					br(file)
					hr(file)
					br(file)
				elif sublines[0].find("%exampleEnd") != -1:
					br(file)
				elif sublines[0].find("%exampleItem") != -1:
					file.write('<h5> Example : '+cleanStr(sublines[1])+'</h5>\n')
				elif sublines[0].find("%tableStart") != -1:	
					file.write('<table borders="0" cellpadding=0 cellspacing=0 width=700>\n')
				elif sublines[0].find("%tableItem") != -1:	
					file.write('<tr><td width=15> </td><td width=20 valign="top" align="center">&bull;</td>')
					for i in range(2,len(sublines)):
						if i != 2:
							file.write('<td valign="top" align="center" width = 10>:</td>')
							file.write('<td valign="top" align="justify">'+cleanStr(sublines[i])+'</td>')
						else:
							file.write('<td valign="top" align="left" width="'+cleanStr(sublines[1])+'"><span class="enumElem">'+cleanStr(sublines[i])+'</span></td>')
					file.write('</tr>\n')
				elif sublines[0].find("%tableEnd") != -1:	
					file.write('</table>\n')
			f.close()
	# DOCUMENTATION
	elif cat == 2:
		file.write('<h3>Help Files</h3>\n')
		file.write('<p class = "install">Use the left sidebar to navigate between help files.</p>\n')
		br(file)
		file.write('<h3>How to install</h3>\n')
		with open("src/INSTALL.html",'r') as f:
			lines = f.readlines()
			for line in lines:
				if line.find("DOCTYPE") == -1 and line.find("title>") == -1 and line.find("head>") == -1 and line.find("body>") == -1:
					file.write(line)
			f.close()
		br(file)
		br(file)
		
	# News
	elif cat == 3:
		file.write('<h3>News</h3>\n')
		with open("src/news.txt",'r') as f:
			lines = f.readlines()
			for line in lines:
				if line == "\n":
					file.write('<br>');
				elif len(line) > 2 and line[0] != "#":
					file.write('<p>'+cleanStr(line)+'</p>\n')
			f.close()
		br(file)

		
	#Downloads
	elif cat == 4:
		with open("src/info_builds.txt",'r') as f:
			lines = f.readlines()
			file.write('<h3>Latest Version</h3>\n')
			file.write('<p>'+ createLink(PATH_WEBSITE+PATH_BUILDS+cleanStr(lines[0]), cleanStr(lines[1]).replace(" ","&nbsp;"))+" [" + cleanStr(lines[2])+ "] "+createLink(PATH_WEBSITE+PATH_RN+cleanStr(lines[3]),"Release Notes")+'</p>\n')
			br(file)
			br(file)
			file.write('<h3>Previous Versions</h3>\n')
			for listID in range(1,(len(lines)+1)/5):
				file.write('<p>'+ createLink(PATH_WEBSITE+PATH_BUILDS+cleanStr(lines[5*listID]), cleanStr(lines[5*listID+1]).replace(" ","&nbsp;"))+" [" + cleanStr(lines[5*listID+2])+ "] "+createLink(PATH_WEBSITE+PATH_RN+cleanStr(lines[5*listID+3]),"Release Notes")+'</p>\n')
				br(file)
			br(file)
			br(file)
			f.close()
	elif cat == 5:
		file.write('<h3>About</h3>\n')
		file.write('<p>The Obq_Shaders collection will only grow stronger with the help of its users. Please address your questions, feature requests and bug reports to <span style="color:darkblue;">madesjardins(AT)obliquefx.com</span>.</p>')
		file.write('<p class = "warning">The help files are in great need of your nice renders. If you\'re using an Obq_Shader and want to show your work, please send me a render along with a render tree screenshot and some infos (artist, description, machine, settings, render times).</p>')
	file.write('</div>\n')

	
#--------------------
#- MAIN CONTENT DIV
def writeContent(file, proj=0, cat=0, helpfile = ""):
	file.write('<div id="content">\n')
	writeContentSidebar(file, proj, cat)
	
	writeContentHelp(file, proj, cat, helpfile)
	
	file.write('</div>\n')	

	
#--------------------
#- FOOTER	
def writeFooter(file, proj=0, cat=0):
	file.write('<div id="footer">Copyright &copy; 2011-2014, ObliqueFX. All Rights Reserved. </div>\n')


#--------------------
#- PAGE CONTAINER
def writePageContainer(file, proj=0, cat=0, helpfile=""):
	# start container
	file.write('<div id="page-container">\n')
	
	#banner
	writeTopBanner(file, proj, cat)
	
	# top
	writeTopMenu(file, proj, cat)
	
	# content
	writeContent(file, proj, cat, helpfile)
	
	# Footer
	writeFooter(file, proj, cat)
	
	# end container
	file.write('</div>\n')

	
#--------------------
#- HTML
def writeHtml(file,title, pageName, proj=0, cat=0, helpfile=""):
	# Start HTML
	file.write('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">\n')
	file.write('<html>\n')
	file.write('<head>\n')
	file.write('<meta http-equiv="Content-type" content="text/html; charset=UTF-8" />\n')
	file.write('<title>'+title+'-'+pageName+'</title>\n')
	file.write('<meta http-equiv="Content-Language" content="en-us" />\n')
	file.write('<meta http-equiv="imagetoolbar" content="no" />\n')
	file.write('<meta name="MSSmartTagsPreventParsing" content="true" />\n')
	file.write('<meta name="description" content="Obq_Shaders for Arnold" />\n')
	file.write('<meta name="keywords" content="Obq_Shaders" />\n')
	file.write('<meta name="author" content="Marc-Antoine Desjardins - Oblique FX" />\n')
	prefix=getPrefix(cat,["","../"])
	file.write('<style type="text/css" media="all">@import "'+prefix+'css/master.css";</style>\n')
	
	file.write('</head>\n')
	file.write('<body>\n')
	
	# container DIV
	writePageContainer(file, proj, cat, helpfile)
	
	# End HTML
	file.write('</body>\n')
	file.write('</html>\n')


###########
# __main__
###########

#------------
# index.html
with open('index.html','w') as fileIndex:
	writeHtml(fileIndex,"Obq Arnold Shaders","Home",0, 0)
	fileIndex.close()

#-----------
# helpfiles to do
with open('src/helpFilesToDo.txt','r') as helpFilesToDo:
	aHelpFiles = helpFilesToDo.readlines()
	for helpfile in aHelpFiles:
		shader_name = cleanStr(helpfile.replace(".obqh","").replace("src\\",""))
		if shader_name != '' and '#' not in shader_name:
			with open('help_files/'+shader_name+'.html','w') as fileIndex:
				writeHtml(fileIndex,"Obq Arnold Shaders",shader_name,0, 1, cleanStr(helpfile))
				fileIndex.close()
	helpFilesToDo.close()
	
#-----------
# All helpfiles
#aHelpFiles = glob.glob("src/*.obqh")
#for helpFile in aHelpFiles:
#	shader_name = helpFile.replace(".obqh","").replace("src\\","")
#	with open('help_files/'+shader_name+'.html','w') as fileIndex:
#		writeHtml(fileIndex,"Obq Arnold Shaders",shader_name,0, 1, helpFile)
#		fileIndex.close()
	
#------------
# documentation.html
with open('documentation.html','w') as fileIndex:
	writeHtml(fileIndex,"Obq Arnold Shaders","Documentation",0, 2)
	fileIndex.close()

#------------
# news.html
with open('news.html','w') as fileIndex:
	writeHtml(fileIndex,"Obq Arnold Shaders","News",0, 3)
	fileIndex.close()
	
#------------
# downloads.html
with open('downloads.html','w') as fileIndex:
	writeHtml(fileIndex,"Obq Arnold Shaders","Downloads",0, 4)
	fileIndex.close()

#------------
# About.html
with open('about.html','w') as fileIndex:
	writeHtml(fileIndex,"Obq Arnold Shaders","About",0, 5)
	fileIndex.close()