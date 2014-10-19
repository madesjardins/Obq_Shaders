# 2014-10-19 05.13 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

channelModeEnumOp = [
    (0, 'Red'), 
    (1, 'Green'), 
    (2, 'Blue'), 
    (3, 'Alpha'), 
    (4, 'RGB'), 
    (5, 'Scalar Input'), 
]

def Obq_OpacityCreateChannelMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_OpacityChannelMode', attribute=attr, label="Channel", 
                               enumeratedItem=channelModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_OpacitySetChannelMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_OpacityChannelMode', edit=True, attribute=attr)
    
def Obq_OpacityHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Color'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Opacity.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_OpacityTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_OpacityFormLayout")
        Obq_OpacityHelpURL()
        
        self.beginLayout("Color", collapse=False )
        self.addControl("color", label="Color")
        self.endLayout() #End Color
        
        self.beginLayout("Opacity", collapse=False )
        self.addCustom("channel", Obq_OpacityCreateChannelMode, Obq_OpacitySetChannelMode)
        self.addControl("opacityRGBA", label="Opacity RGBA")
        self.addControl("opacityScalar", label="Opacity Scalar")
        self.endLayout() #End Opacity
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
