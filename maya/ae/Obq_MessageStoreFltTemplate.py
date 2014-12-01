# 2014-12-01 12.50 am
# Note:   AOVName button not enabled

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_MessageStoreFltHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_MessageStoreFlt'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_MessageStoreFlt.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_MessageStoreFltTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        #self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_MessageStoreFltFormLayout")
        Obq_MessageStoreFltHelpURL()

        self.beginLayout("Main", collapse=False )
        self.addControl("passthrough", label="Passthrough")
        self.addControl("storeAfter", label="Store value after passthrough evaluation")
        self.addSeparator()
        self.addControl("key", label="Key")
        self.addControl("AOVName", label="Store In")

        self.addSeparator()
        self.addControl("defaultValue", label="Default Value")
        self.endLayout() #End Main
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
