# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

  
def Obq_VolumeThicknessHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_VolumeThickness'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_VolumeThickness'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_VolumeThicknessTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_VolumeThicknessFormLayout")
		Obq_VolumeThicknessHelpURL()
		
		self.beginLayout("Particle Color", collapse=False )
		
		self.addControl("colorIn", label="Diffuse")
		self.addControl("emissionColor", label="Emission")
		self.addControl("alphaMode", label="Alpha mode")
		self.addControl("alpha", label="Alpha")
		
		self.endLayout() #End Particle Color
		
		self.beginLayout("Thickness Parameters", collapse=False )
		
		self.addControl("radius", label="Radius")
		self.addControl("exponent", label="Exponent")
		self.addControl("absorption", label="Absorption coefficient")
		self.addControl("accumulatedThicknessThreshold", label="Accumulated threshold")
		
		self.endLayout() #End Thickness Parameters
		
		self.beginLayout("Auto-Transparency", collapse=False )
	   
		self.addControl("autoTransparencyMode", label="Mode Used")
	   
		self.endLayout() #End Auto-Transparency Parameters
		
		self.beginLayout("Advanced Ray Options", collapse=False )
		
		self.addControl("diffuseMode", label="GI response")
		self.addControl("doubleHemisphericalDiffuse", label="Double hemispherical diffuse sampling")
		self.addControl("lightPropagationDistance", label="Direct illumination sampling distance")
		self.addControl("lightPropagationDistanceDiffuse", label="Indirect illumination sampling distance")
		
		self.endLayout() #End Thickness Parameters
		
		self.beginLayout("Shadows Options", collapse=False )
	   
		self.addControl("shadowModifier", label="Occlusion modifier")
	   
		self.endLayout() #End Auto-Transparency Parameters
		
		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
