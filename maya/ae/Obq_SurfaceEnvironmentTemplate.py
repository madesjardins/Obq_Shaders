# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_SurfaceEnvironmentHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_SurfaceEnvironment'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_SurfaceEnvironment'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_SurfaceEnvironmentTemplate(ShaderAETemplate):
	convertToMayaStyle = True

	def setup(self):
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image="Obq_shader_header.png", parent="AttrEdObq_SurfaceEnvironmentFormLayout")
		Obq_SurfaceEnvironmentHelpURL()

		self.beginLayout("Texture", collapse=False)

		self.beginLayout("Pass Environment Shader", collapse=False)
		self.addControl("usePassEnvShader", label="Use The Render Layer Environment Shader")
		self.addControl("rayTypeEnv", label="Ray Type")
		self.endLayout()
		
		self.beginLayout("Image", collapse=False)
		self.beginNoOptimize()
		self.addControl("tex", label="Image")
		self.addControl("considerAlpha", label="Use Alpha")
		self.addControl("mapMode", label="Mapping Mode")
		self.addControl("flipU", label="Flip U")
		self.addControl("flipV", label="Flip V")
		self.endNoOptimize()
		self.endLayout()

		self.beginLayout("Ray Options", collapse=False)
		self.addControl("onSurfaceMode", label="Direction")
		self.addControl("ior", label="IOR")
		self.addControl("customDirection", label="Custom Direction")
		self.endLayout()

		self.beginLayout("Intensity", collapse=False)
		self.addControl("intensity", label="Camera")
		self.endLayout()

		self.endLayout() # End Texture


		self.beginLayout("Roughness", collapse=False)
		self.addControl("sampleLevel", label="Sample Level")
		self.addControl("coneAngle", label="Cone Angle")
		self.addControl("cosLobeGloss", label="")
		self.addControl("useSampleCount", label="")
		self.addControl("sampleCount", label="")
		self.addControl("sampleCountMultiplier", label="")
		self.endLayout()

		self.beginLayout("Transformations", collapse=False)
		self.addControl("globalRotation", label="Use rotation at a global scope")
		self.addControl("rotation", label="Rotation")
		self.endLayout()
		
		self.beginLayout("AOV", collapse=False)
		self.addControl("passthrough", label="Passthrough Shader")
		self.addControl("renderChannel", label="AOV Name")
		self.endLayout()

		# self.beginLayout("Options", collapse=False)
		# self.endLayout()

		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')
	  
		self.addExtraControls()
		self.endScrollLayout()
