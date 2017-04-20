# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_ThicknessHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_Thickness'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_Thickness'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_ThicknessTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_ThicknessFormLayout")
		Obq_ThicknessHelpURL()

		self.beginLayout("Main", collapse=False )
		self.beginLayout("Direction", collapse=False )
		self.addControl("dirMode", label="Direction Mode")
		self.addControl("ior", label="IOR")
		self.beginLayout("Modifiers", collapse=False )
		self.addControl("dirInverted", label="Invert")
		self.addControl("dirSmooth", label="Smooth")
		self.addControl("dirGeometric", label="Geometric")
		self.endLayout() #End Modifiers
		self.beginLayout("Custom Direction", collapse=True )
		self.addControl("customDirection", label="Custom Direction")
		self.endLayout() #End Custom Direction
		self.endLayout() #End Direction
		self.beginLayout("Thickness", collapse=False )
		self.addControl("thicknessMode", label="Thickness Mode")
		self.beginLayout("Options", collapse=False )
		self.addControl("multNdotR", label="Multiply by NdotR")
		self.addControl("NdotRExp", label="Exponent")
		self.addControl("multiplyByDepth", label="Consider thickness of intersecting volumes")
		self.endLayout() #End Options
		self.endLayout() #End Thickness
		self.beginLayout("Options", collapse=False )
		self.addControl("backface", label="Backface is ")
		self.addControl("intersectOthers", label="Intersect Others")
		self.addControl("noHitMode", label="No hit means surface is ")
		self.endLayout() #End Options
		self.beginLayout("Ray Settings", collapse=False )
		self.addControl("maxRayDepth", label="Maximum Ray Depth")
		self.addControl("maxRayLength", label="Maximum Ray Length")
		self.addControl("usemaxRayLength", label="Use Max Ray Length")
		self.endLayout() #End Ray Settings
		self.beginLayout("Output Settings", collapse=False )
		self.addControl("normalizeMode", label="Normalize Mode")
		self.endLayout() #End Output Settings
		self.endLayout() #End Main
		
		self.beginLayout("Blur", collapse=True )
		self.beginLayout("Samples", collapse=False )
		self.addControl("sampleLevel", label="Sample Level")
		self.addControl("useSampleCount", label="Clamp Sample Count")
		self.addControl("sampleCount", label="Max Sample Count")
		self.addControl("sampleCountMultiplier", label="Max Sample Count Multiplier")
		self.beginLayout("Amount", collapse=False )
		self.addControl("coneAngle", label="Cone Angle")
		self.addControl("cosLobeGloss", label="Gloss")
		self.endLayout() #End Amount
		self.endLayout() #End Samples
		self.endLayout() #End Blur
		
		self.beginLayout("Advanced Options", collapse=True )
		self.addControl("dirSafe", label="Use safe directions")
		self.addControl("enterSurface", label="Enter surface by (0.0001 units)")
		self.addControl("useVdotNForMaxAngle", label="Use geometric normal to limit cone angle")
		self.addControl("geometricLimitMultiplier", label="Multiply geometric limit by")
		self.endLayout() #End Advanced Options
		
	   
		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
