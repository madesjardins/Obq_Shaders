# 2014-10-19 11.01 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_ColorHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Color'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Color.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_ColorTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_ColorFormLayout")
        Obq_ColorHelpURL()

        self.beginLayout("Color", collapse=False )
        self.addControl("color", label="Color")
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
