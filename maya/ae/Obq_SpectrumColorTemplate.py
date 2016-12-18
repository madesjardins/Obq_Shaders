# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

  
def Obq_SpectrumColorHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_SpectrumColor'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_SpectrumColor'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_SpectrumColorTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_SpectrumColorFormLayout")
		Obq_SpectrumColorHelpURL()
		
		self.beginLayout("Settings", collapse=False )
		self.addControl("mode", label="Mode")
		self.endLayout() #Settings
		
		self.beginLayout("SPD (Strings or File *.csv, *.txt, *.spd)", collapse=False )
		self.addControl("filename", label="Filename")
		self.addControl("lambdasStr", label="Lambda")
		self.addControl("intensitiesStr", label="Intensities")
		self.addControl("lambdaUnits", label="Wavelength Units")
		self.endLayout() #SPD
		
		self.beginLayout("Colorspace", collapse=False )
		self.addControl("xyz2rgb", label="XYZ to RGB Matrix")
		self.addControl("gamma", label="Gamma")
		self.endLayout() #Colorspace
		
		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
