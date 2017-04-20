# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

  
def Obq_TransformShadingPointHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_TransformShadingPoint'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_TransformShadingPoint'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_TransformShadingPointTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_TransformShadingPointFormLayout")
		Obq_TransformShadingPointHelpURL()
		
		self.beginLayout("Main", collapse=False )
		
		self.addControl("passthrough", label="Passthrough Shader")
		self.addControl("coordinates", label="Space")
		self.addControl("mode", label="Transforms mode")
		self.addControl("invert", label="Invert")
		self.addControl("scale", label="Scale")
		self.addControl("rotationOrder", label="Rotation Order")
		self.addControl("rotation", label="Rotation")
		self.addControl("translation", label="Translation")
		self.addControl("transforms", label="Transforms")
		self.endLayout() #End Main
		
		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
