# 2014-11-30 11.50 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

modeEnumOp = [
    (0, 'Diffuse Shadowed'),
    (1, 'Diffuse Unshadowed'),
    (2, 'Shadow Ratio'),
    (3, 'Shadow Difference'),
]

traceTypeEnumOp = [
    (0, 'Receiver'),
    (1, 'Caster'),
    (2, 'Caster and Receiver'),
]

def Obq_ShadowCreateMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ShadowMode', attribute=attr, label="Mode",
                               enumeratedItem=modeEnumOp)
    cmds.setUITemplate(popTemplate=True)

def Obq_ShadowCreateTraceType(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ShadowTraceType', attribute=attr, label="Trace Type",
                               enumeratedItem=traceTypeEnumOp)
    cmds.setUITemplate(popTemplate=True)

def Obq_ShadowSetMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ShadowMode', edit=True, attribute=attr)


def Obq_ShadowSetTraceType(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ShadowTraceType', edit=True, attribute=attr)


def Obq_ShadowHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Shadow'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Shadow.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_ShadowTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_ShadowFormLayout")
        Obq_ShadowHelpURL()

        self.beginLayout("Options", collapse=False )
        self.addCustom("mode", Obq_ShadowCreateMode, Obq_ShadowSetMode)
        self.addCustom("traceType", Obq_ShadowCreateTraceType, Obq_ShadowSetTraceType)
        self.addControl("shadowsOnUnlit", label="Shadows on unlit faces")
        self.endLayout() #End Options

        self.beginLayout("Opacity", collapse=False )
        self.addControl("opacity", label="Opacity")
        self.endLayout() #End Opacity

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
