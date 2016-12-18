# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_MeasuredMaterialHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_MeasuredMaterial'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_MeasuredMaterial'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_MeasuredMaterialTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_MeasuredMaterialFormLayout")
		Obq_MeasuredMaterialHelpURL()

		self.beginLayout("Main", collapse=False )
		
		self.beginLayout("BRDF", collapse=False )
		self.addControl("filename", label="Filename")
		self.addControl("resolution", label="Resolution")
		self.endLayout() # End BRDF
		
		self.beginLayout("Direct", collapse=False )
		self.addControl("direct_color", label="Direct Color")
		self.addControl("direct_scale", label="Direct Scale")
		self.addControl("useMIS", label="Use MIS (test)")
		self.endLayout() # End Direct
		
		self.beginLayout("Indirect", collapse=False )
		self.addControl("indirect_color", label="Indirect Color")
		self.addControl("indirect_scale", label="Indirect Scale")
		self.addControl("useImportanceSampling", label="Use importance sampling (test)")
		self.addControl("samples", label="Samples")
		self.endLayout() # End Indirect
		
		self.beginLayout("Options", collapse=False )
		self.addControl("binMaxDif", label="Stop binary search when in range of")
		self.addControl("useJitters", label="Use jitters")
		self.addControl("useHalfs", label="Use half hints")
		self.endLayout() # End Options
		
		self.endLayout() # End Main
		
		self.beginLayout("AOVs", collapse=False )
		self.addControl("direct_AOV", label="Direct AOV")
		self.addControl("indirect_AOV", label="Indirect AOV")
		self.endLayout()
		

		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
