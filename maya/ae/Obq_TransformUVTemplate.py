# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

  
def Obq_TransformUVHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_TransformUV'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_TransformUV'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_TransformUVTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):

		# Hide the node swatch preview until the shader is able to render a preview
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_TransformUVFormLayout")
		Obq_TransformUVHelpURL()
		
		self.beginLayout("Main", collapse=False )
		
		self.addControl("tex", label="Texture (Image File Node)")
		
		self.beginLayout("Mode", collapse=False )
		self.addControl("mode", label="Transform order")
		self.endLayout() #End Mode
		
		self.beginLayout("Scale", collapse=False )
		self.beginNoOptimize()
		self.addControl("scaleX", label="X")
		self.addControl("scaleY", label="Y")
		self.endNoOptimize()
		self.endLayout() #End Scale
		
		self.beginLayout("Rotate", collapse=False )
		self.addControl("rotate", label="Angle (degrees)")
		self.endLayout() #End Rotate
		
		self.beginLayout("Translate", collapse=False )
		self.beginNoOptimize()
		self.addControl("translateX", label="X")
		self.addControl("translateY", label="Y")
		self.endNoOptimize()
		self.endLayout() #End Translate
		
		self.beginLayout("Center", collapse=False )
		self.beginNoOptimize()
		self.addControl("pivotX", label="X")
		self.addControl("pivotY", label="Y")
		self.endNoOptimize()
		self.endLayout() #End Center
		
		self.endLayout() #End mAIN
		
		# include/call base class/node attributes
		pm.mel.AEdependNodeTemplate(self.nodeName)

		# Hide the NormalCamera and HardwareColor Extra Attributes
		self.suppress('normalCamera')
		self.suppress('hardwareColor')

		self.addExtraControls()
		self.endScrollLayout()
