# 2014-11-30 12.54 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

typeModeEnumOp = [
    (0, 'All'), 
    (1, 'Reflection'), 
    (2, 'Refraction'), 
    (3, 'Diffuse'), 
    (4, 'Glossy'), 
]

def Obq_RayDepthCreateTypeMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_RayDepthTypeMode', attribute=attr, label="Ray Type", 
                               enumeratedItem=typeModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_RayDepthSetTypeMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_RayDepthTypeMode', edit=True, attribute=attr)
    
def Obq_RayDepthHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_RayDepth'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_RayDepth.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_RayDepthTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_RayDepthFormLayout")
        Obq_RayDepthHelpURL()

        self.beginLayout("Ray Type", collapse=False )
        self.addCustom("type", Obq_RayDepthCreateTypeMode, Obq_RayDepthSetTypeMode)
        self.endLayout() #End Ray Type
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
