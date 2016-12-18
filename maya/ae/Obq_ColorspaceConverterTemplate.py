# 2016-12-18 5.46 AM
# Todo:
# Implement the ColorspaceIn and Colorspace Out UI elements

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

# colorspaceInModeEnumOp = [
	# (3, 'CIE XYZ'), 
	# (0, 'XSI Linear'), 
	# (1, 'XSI sRGB'), 
	# (2, 'CIE RGB'), 
# ]

# def Obq_ColorspaceConverterCreateColorspaceInMode(attr):
	# cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
	# cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConverterColorspaceInMode', attribute=attr, label="Colorspace In", 
							   # enumeratedItem=colorspaceInModeEnumOp)    
	# cmds.setUITemplate(popTemplate=True)
	
# def Obq_ColorspaceConverterSetColorspaceInMode(attr):
	# cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConverterColorspaceInMode', edit=True, attribute=attr)

# colorspaceOutModeEnumOp = [
	# (3, 'CIE XYZ'), 
	# (0, 'XSI Linear'), 
	# (1, 'XSI sRGB'), 
	# (2, 'CIE RGB'), 
# ]

# def Obq_ColorspaceConverterCreateColorspaceOutMode(attr):
	# cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
	# cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConverterColorspaceOutMode', attribute=attr, label="Colorspace Out", 
							   # enumeratedItem=colorspaceOutModeEnumOp)    
	# cmds.setUITemplate(popTemplate=True)
	
# def Obq_ColorspaceConverterSetColorspaceOutMode(attr):
	# cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConverterColorspaceOutMode', edit=True, attribute=attr)
	
   
def Obq_ColorspaceConverterHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_ColorspaceConverter'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_ColorspaceConverter'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_ColorspaceConverterTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_ColorspaceConverterFormLayout")
		Obq_ColorspaceConverterHelpURL()

		self.beginLayout('Colorspace Converter Attributes', collapse=False)
		 
		self.beginLayout("Input", collapse=False)
		self.addControl("color", label="Color")
		self.addControl("csPrimariesIn", label="Primaries")
		self.addControl("csGammaIn", label="Gamma")
		self.addControl("csIlluminantIn", label="Illuminant")
		self.endLayout()
		
		self.beginLayout("Output", collapse=False )
		self.addControl("csPrimariesOut", label="Primaries")
		self.addControl("csGammaOut", label="Gamma")
		self.addControl("csIlluminantOut", label="Illuminant")
		self.endLayout()
		
		self.beginLayout("Options", collapse=False)
		self.beginNoOptimize()
		self.addControl("switchInOut", label="Switch In and Out")
		self.addControl("useBradford", label="Use Bradford chromatic adaptation matrix")
		self.addControl("clamp", label="Clamp")
		self.endNoOptimize()
		self.endLayout()

		self.beginLayout("Load Colorspace Presets", collapse=True)
		#self.addCustom("colorspaceIn", Obq_ColorspaceConverterCreateColorspaceInMode, Obq_ColorspaceConverterSetColorspaceInMode)
		#self.addCustom("colorspaceOut", Obq_ColorspaceConverterCreateColorspaceOutMode, Obq_ColorspaceConverterSetColorspaceOutMode)
		self.endLayout()
		
		self.endLayout()

		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
