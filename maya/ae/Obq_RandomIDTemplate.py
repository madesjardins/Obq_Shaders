# 2014-12-01 12.03 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


def Obq_RandomIDHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_RandomID'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_RandomID.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_RandomIDTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        #self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_RandomIDFormLayout")
        Obq_RandomIDHelpURL()

        self.beginLayout("Options", collapse=False)
        self.addControl("randMax", label="Number of values")
        self.addControl("seed", label="Seed")
        self.endLayout() #End Options
        
        self.beginLayout("Strip", collapse=False )
        self.beginNoOptimize()
        self.addControl("stripModelName", label="Model")
        self.addControl("stripFrameNumber", label="Frame")
        self.endNoOptimize()
        self.endLayout() #End Strip
        
        self.beginLayout("Instances", collapse=False )
        self.beginNoOptimize()
        self.addControl("stripInstanceFrameNumber", label="Frame")
        self.addControl("stripInstanceID", label="ID")
        self.addControl("stripInstanceShape", label="Shape")
        self.endNoOptimize()
        self.endLayout() #End Instances
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
