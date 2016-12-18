# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_Bend4StereoHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_Bend4Stereo'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_Bend4Stereo'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)


class AEObq_Bend4StereoTemplate(ShaderAETemplate):
	convertToMayaStyle = True

	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		#self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image="Obq_shader_header.png")#, parent="AttrEdObq_Bend4StereoFormLayout")

		Obq_Bend4StereoHelpURL()

		#self.beginLayout("Main", collapse=False)
		self.beginLayout("Bend Mode", collapse=False)
		self.addControl("bendMode",  label="Bend Mode")#self.addCustom('bendMode', Obq_Bend4StereoCreateBlendMode, Obq_Bend4StereoSetBlendMode)
		self.endLayout()

		self.beginLayout("Cameras", collapse=False)
		self.addControl("centerCamera",  label="Center")
		self.addControl("leftCamera",  label="Left")
		self.addControl("rightCamera",  label="Right")
		self.endLayout()
	   #self.endLayout() # End Main

		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
