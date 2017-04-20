# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_ToonHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_Toon'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_Toon'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_ToonTemplate(ShaderAETemplate):
	convertToMayaStyle = True

	def setup(self):
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image="Obq_shader_header.png", parent="AttrEdObq_ToonFormLayout")
		Obq_ToonHelpURL()

		self.beginLayout("Global", collapse=False)
		
		self.beginLayout("Ambient", collapse=False)
		self.addControl("ambient_color", label="Color")
		self.addControl("ambient_scale", label="Scale")
		self.beginNoOptimize()
		self.addControl("ambient_multAmbience", label="Multiply by scene ambience")
		self.addControl("ambient_noInternal", label="No Internal Ambient")
		self.endNoOptimize()
		self.endLayout()

		self.beginLayout("Opacity", collapse=False)
		self.addControl("opacity",  label="Color")
		self.endLayout()

		self.beginLayout("Options", collapse=False)
		self.addControl("global_compMode", label="Compositing")
		self.addControl("global_loopMode", label="Loop Mode")
		self.addControl("global_loopLightMode", label="Lights")
		self.addControl("global_remapMode", label="Remap")
		self.addControl("global_remapMultByColor", label="Multiply by normalized input color")
		self.beginNoOptimize()
		self.addControl("global_multByLightColor",  label="Multiply by Light Color")
		self.addControl("global_clamp",  label="Clamp Before Compositing")
		self.endNoOptimize()
		self.endLayout()
		
		self.beginLayout("Special Output", collapse=False)
		self.addControl("output_imageOnUV",  label="Check To Render Map Image (Use diffuse tab)")
		self.endLayout()
		
		self.endLayout() # End Global
		
		self.beginLayout("Diffuse", collapse=False)
		self.addControl("diffuse_color",  label="Color")
		self.addControl("diffuse_scale",  label="Scale")
		self.addControl("diffuse_coverage",  label="Coverage")
		self.addControl("diffuse_softness",  label="Softness")
		self.addControl("diffuse_noInternal",  label="No Internal Diffuse")
		
		self.addControl("diffuse_linearDot",  label="Linearize Dot Product")
		self.addControl("diffuse_mode",  label="Mode")
		self.addControl("diffuse_image",  label="Image Gradient")
		
		self.beginLayout("Image Remap", collapse=False)
		self.addControl("diffuse_autoRemap",  label="Use Auto Remap")
		self.beginNoOptimize()
		self.addControl("diffuse_imageRemapLeft",  label="Left Value")
		self.addControl("diffuse_imageRemapRight",  label="Right Value")
		self.endNoOptimize()
		self.beginNoOptimize()
		self.addControl("diffuse_imageRemapBias",  label="Bias")
		self.addControl("diffuse_imageRemapGain",  label="Gain")
		self.endNoOptimize()
		self.endLayout()
		
		self.endLayout()

		self.beginLayout("Highlight", collapse=True)
		self.addControl("highlight_color",  label="Color")
		self.addControl("highlight_scale",  label="Scale")
		self.addControl("highlight_exponent",  label="Exponent")
		self.addControl("highlight_coverage",  label="Coverage")
		self.addControl("highlight_softness",  label="Softness")
		self.addControl("highlight_noInternal",  label="No Internal Highlight")
		
		self.addControl("highlight_linearDot",  label="Linearize Dot Product")
		self.addControl("highlight_mode",  label="Mode")
		self.addControl("highlight_image",  label="Image Gradient")
		
		self.beginLayout("Image Remap", collapse=False)
		self.addControl("highlight_autoRemap",  label="Use Auto Remap")
		self.beginNoOptimize()
		self.addControl("highlight_imageRemapLeft",  label="Left Value")
		self.addControl("highlight_imageRemapRight",  label="Right Value")
		self.endNoOptimize()
		self.beginNoOptimize()
		self.addControl("highlight_imageRemapBias",  label="Bias")
		self.addControl("highlight_imageRemapGain",  label="Gain")
		self.endNoOptimize()
		self.endLayout()
		
		self.endLayout()
		

		self.beginLayout("Rimlight", collapse=True)
		self.addControl("rimlight_color",  label="Color")
		self.addControl("rimlight_scale",  label="Scale")
		self.addControl("rimlight_coverage",  label="Coverage")
		self.addControl("rimlight_softness",  label="Softness")
		self.addControl("rimlight_noInternal",  label="No Internal rimlight")
		
		self.addControl("rimlight_linearDot",  label="Linearize Dot Product")
		self.addControl("rimlight_mode",  label="Mode")
		self.addControl("rimlight_image",  label="Image Gradient")
		
		self.beginLayout("Image Remap", collapse=False)
		self.addControl("rimlight_autoRemap",  label="Use Auto Remap")
		self.beginNoOptimize()
		self.addControl("rimlight_imageRemapLeft",  label="Left Value")
		self.addControl("rimlight_imageRemapRight",  label="Right Value")
		self.endNoOptimize()
		self.beginNoOptimize()
		self.addControl("rimlight_imageRemapBias",  label="Bias")
		self.addControl("rimlight_imageRemapGain",  label="Gain")
		self.endNoOptimize()
		self.endLayout()
		
		self.endLayout()

		self.beginLayout("AOVs", collapse=True)
		self.addControl("ambient_fb_str",  label="Ambient")
		self.addControl("diffuse_fb_str",  label="Diffuse")
		self.addControl("highlight_fb_str",  label="Highlight")
		self.addControl("rimlight_fb_str",  label="Rimlight")
		self.addControl("contour_fb_str",  label="Contour")
		self.beginLayout("Options", collapse=True)
		self.beginNoOptimize()
		self.addControl("putAlphaInFb",  label="Set Alpha to Over Alpha")
		self.addControl("normalizeObjectID",  label="Contour")
		self.endNoOptimize()
		self.endLayout()
		self.endLayout()

		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')
	  
		self.addExtraControls()
		self.endScrollLayout()
