# 2014-09-10 07.48 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_ToonSimpleHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_ToonSimple'
	ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_ToonSimple.html'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_ToonSimpleTemplate(ShaderAETemplate):
	convertToMayaStyle = True

	def setup(self):
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image="Obq_shader_header.png", parent="AttrEdObq_ToonSimpleFormLayout")
		Obq_ToonSimpleHelpURL()

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
		self.beginNoOptimize()
		self.addControl("global_multByLightColor",  label="Multiply by Light Color")
		self.addControl("global_clamp",  label="Clamp Before Compositing")
		self.endNoOptimize()
		self.endLayout()
		
		self.endLayout() # End Global
		
		self.beginLayout("Diffuse", collapse=False)
		self.addControl("diffuse_color",  label="Color")
		self.addControl("diffuse_scale",  label="Scale")
		self.addControl("diffuse_coverage",  label="Coverage")
		self.addControl("diffuse_softness",  label="Softness")
		self.addControl("diffuse_noInternal",  label="No Internal Diffuse")
		self.endLayout()

		self.beginLayout("Highlight", collapse=True)
		self.addControl("highlight_color",  label="Color")
		self.addControl("highlight_scale",  label="Scale")
		self.addControl("highlight_exponent",  label="Exponent")
		self.addControl("highlight_coverage",  label="Coverage")
		self.addControl("highlight_softness",  label="Softness")
		self.addControl("highlight_noInternal",  label="No Internal Highlight")
		self.endLayout()
		

		self.beginLayout("Rimlight", collapse=True)
		self.addControl("rimlight_color",  label="Color")
		self.addControl("rimlight_scale",  label="Scale")
		self.addControl("rimlight_coverage",  label="Coverage")
		self.addControl("rimlight_softness",  label="Softness")
		self.addControl("rimlight_noInternal",  label="No Internal rimlight")
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
