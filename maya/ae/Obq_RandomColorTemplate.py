# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


def Obq_RandomColorHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_RandomColor'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_RandomColor'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_RandomColorTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		#self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_RandomColorFormLayout")
		Obq_RandomColorHelpURL()


		self.beginLayout("Options", collapse=False )
		self.addControl("randMax", label="Number of values")
		self.addControl("seed", label="Seed")
		self.endLayout()
		
		self.beginLayout("Strip", collapse=False )
		self.beginNoOptimize()
		self.addControl("stripModelName", label="Strip Model Name")
		self.addControl("stripFrameNumber", label="Strip Frame Number")
		self.endNoOptimize()
		self.endLayout()

		self.beginLayout("Instances", collapse=False )
		self.beginNoOptimize()
		self.addControl("stripInstanceFrameNumber", label="Strip Instances Frame Number")
		self.addControl("stripInstanceID", label="Strip Instances ID")
		self.addControl("stripInstanceShape", label="Strip Instances Shape")
		self.endNoOptimize()
		self.endLayout()

		self.beginLayout("Inputs", collapse=False)
		self.addControl("color01", label="Color Input 01")
		self.addControl("color02", label="Color Input 02")
		self.addControl("color03", label="Color Input 03")
		self.addControl("color04", label="Color Input 04")
		self.addControl("color05", label="Color Input 05")
		self.addControl("color06", label="Color Input 06")
		self.addControl("color07", label="Color Input 07")
		self.addControl("color08", label="Color Input 08")
		self.addControl("color09", label="Color Input 09")
		self.addControl("color10", label="Color Input 10")
		self.addControl("color11", label="Color Input 11")
		self.addControl("color12", label="Color Input 12")
		self.addControl("color13", label="Color Input 13")
		self.addControl("color14", label="Color Input 14")
		self.addControl("color15", label="Color Input 15")
		self.addControl("color16", label="Color Input 16")
		self.endLayout()

		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()

