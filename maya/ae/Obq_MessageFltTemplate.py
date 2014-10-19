# 2014-10-19 04.06 pm

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

def Obq_MessageFltCreateTypeMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_MessageFltModeMode', attribute=attr, label="Mode", 
                               enumeratedItem=modeModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_MessageFltSetTypeMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_MessageFltModeMode', edit=True, attribute=attr)
    
presetModeEnumOp = [
    (0, 'Obq_Toon'), 
]

def Obq_MessageFltCreatePresetMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_MessageFltPresetMode', attribute=attr, label="Mode", 
                               enumeratedItem=presetModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_MessageFltSetPresetMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_MessageFltPresetMode', edit=True, attribute=attr)
    
    
def Obq_MessageFltHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Color'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_MessageFlt.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_MessageFltTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        #self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_MessageFltFormLayout")
        Obq_MessageFltHelpURL()

        self.beginLayout("Main", collapse=False )
        self.addControl("defaultValue", label="Default Value")
        #self.addCustom("mode", Obq_MessageFltCreateModeMode, Obq_MessageFltSetModeMode)
        #self.addCustom("preset", Obq_MessageFltCreatePresetMode, Obq_MessageFltSetPresetMode)
        self.addControl("message", label="Message")
        self.endLayout() #End Main
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
