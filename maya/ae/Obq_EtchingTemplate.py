# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
	
def Obq_EtchingHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_Etching'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_Etching'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_EtchingTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_EtchingFormLayout")
		Obq_EtchingHelpURL()

		self.beginLayout("Color", collapse=False )
		self.beginLayout("Bright", collapse=False )
		self.addControl("bright_color", label="Bright Color")
		self.addControl("bright_point", label="Bright Point")
		self.addControl("multiply_bright_color", label="Multiply by normalized input")
		self.endLayout() #End Brightness
		self.beginLayout("Dark", collapse=False )
		self.addControl("dark_color", label="Dark Color")
		self.addControl("dark_point", label="Dark Point")
		self.addControl("multiply_dark_color", label="Multiply by normalized input")
		self.addControl("normalized_black_color", label="Normalized Black Color")
		self.endLayout() #End Dark
		self.endLayout() #End Color
		
		
		self.beginLayout("Signal", collapse=False )
		self.beginLayout("Parameters", collapse=False )
		self.addControl("frequency", label="Frequency")
		self.addControl("offset", label="Offset")
		self.addControl("feather", label="Feather")
		self.addControl("use_linear_signal", label="Use linear wave type")
		self.endLayout() #End Parameters
		self.beginLayout("Auto-Filtering", collapse=False )
		self.addControl("autoFiltering", label="Enable")
		self.beginLayout("Advanced", collapse=False )
		self.addControl("autoFilteringAdvanced", label="Show advanced options")
		self.addControl("autoFilteringOut", label="Show auto-filter level")
		self.addControl("autoFilteringMode", label="Apply to")
		self.addControl("autoFilteringU", label="Also affected by U")
		self.addControl("autoFilteringBias", label="Bias")
		self.addControl("autoFilteringGain", label="Gain")
		self.addControl("autoFilteringAutoRange", label="Range based on Frequency")
		self.addControl("autoFilteringStart", label="Start")
		self.addControl("autoFilteringEnd", label="End")
		self.endLayout() #End Advanced
		self.endLayout() #End Auto-Filtering
		self.beginLayout("Dots", collapse=False )
		self.beginNoOptimize()
		self.addControl("enable_dots", label="Enable dots")
		self.addControl("progressive_dots", label="Dots appear progressively")
		self.addControl("bright_dots_start", label="Bright dots start(ratio)")
		self.addControl("dark_dots_start", label="Dark dots start(ratio)")
		self.endNoOptimize()
		self.endLayout() #End Dots
		self.endLayout() #End Signal
		self.beginLayout("Input", collapse=False )
		self.addControl("shading_input", label="Shading Input")
		self.addControl("coordinates_input", label="Coordinates input")
		self.beginLayout("Options", collapse=False )
		self.addControl("use_average_rgb", label="Use Average RGB instead of luminance")
		self.addControl("mix", label="Mix")
		self.endLayout() #End Options
		self.endLayout() #End Input
		
		self.beginLayout("AOVs", collapse=False )
		self.addControl("aov_matte", label="Matte")
		self.addControl("aov_input", label="Shader input")
		self.endLayout() #End AOVs        
		
		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
