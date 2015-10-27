# 2014-11-30 12.54 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_Root2TipHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_Root2Tip'
	ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Root2Tip.html'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_Root2TipTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_Root2TipFormLayout")
		Obq_Root2TipHelpURL()

		self.beginLayout("Main", collapse=False )
		self.addControl("root", label="Root Color")
		self.addControl("tip", label="Root Color""Tip Color")
		self.addControl("mode", label="Root Color""Interpolation")
		self.endLayout() #End Main
		
		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
