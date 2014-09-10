# 2014-09-10 06.28 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

blendEnumOp = [
    (0, "Don\'t bend"), 
    (1, "Bend from left"), 
    (2, "Bend from right"),
    (3, "Bend from center")
]

def Obq_Bend4StereoCreateBlendMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_Bend4StereoBlendMode', attribute=attr, label="Bend Mode", enumeratedItem=blendEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_Bend4StereoSetBlendMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_Bend4StereoBlendMode', edit=True, attribute=attr)

def Obq_Bend4StereoHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Bend4Stereo'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Bend4Stereo.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)


class AEObq_Bend4StereoTemplate(ShaderAETemplate):
    convertToMayaStyle = True

    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        #self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image="Obq_shader_header.png", parent="AttrEdObq_Bend4StereoFormLayout")

        Obq_Bend4StereoHelpURL()

        #self.beginLayout("Main", collapse=False)
        self.beginLayout("Bend Mode", collapse=False)
        self.addCustom('bendMode', Obq_Bend4StereoCreateBlendMode, Obq_Bend4StereoSetBlendMode)
        self.endLayout()

        self.beginLayout("Cameras", collapse=False)
        self.addControl("centerCamera",  label="Center")
        self.addControl("leftCamera",  label="Left")
        self.addControl("rightCamera",  label="Right")
        self.endLayout()
       #self.endLayout() # End Main

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
