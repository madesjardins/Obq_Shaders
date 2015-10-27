# 2014-11-30 10.10 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_MessageSetIntHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_MessageSetInt'
	ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_MessageSet.html'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_MessageSetIntTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		#self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_MessageSetIntFormLayout")
		Obq_MessageSetIntHelpURL()

		self.beginLayout("Main", collapse=False )
		self.addControl("passthrough", label="Passthrough")
		self.addControl("setAfter", label="Set value after passthrough evaluation")
		self.addSeparator()
		self.addControl("key", label="Key")
		self.addSeparator()
		self.addControl("value", label="Value")
		self.addSeparator()
		self.addControl("restoreValue", label="Restore value after if any")
		self.addControl("defaultRestoredValue", label="Default restored value")
		self.endLayout() #End Main
		
		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
