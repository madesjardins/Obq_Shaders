# 2014-09-10 06.32 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_ChangeRangeHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_ChangeRange'
	ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_ChangeRange.html'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)


class AEObq_ChangeRangeTemplate(ShaderAETemplate):
	convertToMayaStyle = True

	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		#self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image="Obq_shader_header.png", parent="AttrEdObq_ChangeRangeFormLayout")

		Obq_ChangeRangeHelpURL()

		self.beginLayout("Main", collapse=False)
		self.addControl("input",  label="Input")
		self.addControl("oStart",  label="Old Range Start")
		self.addControl("oEnd",  label="Old Range End")
		self.addControl("nStart",  label="New Range Start")
		self.addControl("nEnd",  label="New Range End")
		self.endLayout()

		self.beginLayout("Extra Options", collapse=False)
		self.addControl("clamp",  label="Clamp")
		self.addControl("bias",  label="Bias")
		self.addControl("gain",  label="Gain")
		self.endLayout()

		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
