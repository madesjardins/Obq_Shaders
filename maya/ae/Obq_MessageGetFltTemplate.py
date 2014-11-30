# 2014-11-30 12.54 pm
# Note: Maya's internal .message attribute is automatically linked into the string field by Arnold.

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

modeModeEnumOp = [
    (0, 'Custom'), 
    (1, 'Preset'), 
]

def Obq_MessageGetFltCreateTypeMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_MessageGetFltModeMode', attribute=attr, label="Mode", 
                               enumeratedItem=modeModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_MessageGetFltSetTypeMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_MessageGetFltModeMode', edit=True, attribute=attr)
    
presetModeEnumOp = [
    (0, 'Obq_Toon'), 
]

def Obq_MessageGetFltCreatePresetMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_MessageGetFltPresetMode', attribute=attr, label="Mode", 
                               enumeratedItem=presetModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_MessageGetFltSetPresetMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_MessageGetFltPresetMode', edit=True, attribute=attr)
    
    
def Obq_MessageGetFltHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_MessageGetFlt'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_MessageGetFlt.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_MessageGetFltTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        #self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_MessageGetFltFormLayout")
        Obq_MessageGetFltHelpURL()

        self.beginLayout("Main", collapse=False )
        self.addControl("defaultValue", label="Default Value")
        #self.addCustom("mode", Obq_MessageGetFltCreateModeMode, Obq_MessageGetFltSetModeMode)
        #self.addCustom("preset", Obq_MessageGetFltCreatePresetMode, Obq_MessageGetFltSetPresetMode)
        self.addControl("message", label="Message")
        self.endLayout() #End Main
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
