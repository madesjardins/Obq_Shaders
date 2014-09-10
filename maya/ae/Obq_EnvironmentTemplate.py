# 2014-09-10 07.28 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

envMappingModeEnumOp = [
    (0, 'Mirrored Ball'), 
    (1, 'Angular Map'), 
    (2, 'Latitude-Longitude'), 
    (3, 'Vertical Cross'), 
]

def Obq_EnvironmentCreateMappingMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_EnvironmentMappingMode', attribute=attr, label="Mapping Mode", 
                               enumeratedItem=envMappingModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_EnvironmentSetMappingMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_EnvironmentMappingMode', edit=True, attribute=attr)

OnSurfaceModeEnumOp = [
    (0, 'View Direction'), 
    (1, 'View Direction (Inverted)'), 
    (2, 'Surface Normal Direction'), 
    (3, 'Surface Normal Direction (no bump)'), 
    (4, 'Surface Normal Direction (Inverted)'), 
    (5, 'Surface Normal Direction (no bump, Inverted)'), 
    (6, 'Surface Normal Direction (Front-Facing)'), 
    (7, 'Surface Normal Direction (Back-Facing)'), 
    (8, 'Reflection Direction'), 
    (9, 'Reflection Direction (no bump)'), 
    (10, 'Refraction Direction'), 
    (11, 'Refraction Direction (no bump)'), 
    (12, 'Custom Direction'), 
]

def Obq_EnvironmentCreateOnSurfaceMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_EnvironmentOnSurfaceMode', attribute=attr, label="Direction", 
                               enumeratedItem=OnSurfaceModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_EnvironmentSetOnSurfaceMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_EnvironmentOnSurfaceMode', edit=True, attribute=attr)

def Obq_EnvironmentHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Environment'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Environment.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_EnvironmentTemplate(ShaderAETemplate):
    convertToMayaStyle = True

    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image="Obq_shader_header.png", parent="AttrEdObq_EnvironmentFormLayout")
        Obq_EnvironmentHelpURL()

        self.beginLayout("Texture", collapse=False)
        
        self.beginLayout("Image", collapse=False)
        self.beginNoOptimize()
        self.addControl("tex", label="Image")
        self.addControl("considerAlpha", label="Use Alpha")
        self.addControl("opaque", label="Opaque background")
        self.addCustom("mapMode", Obq_EnvironmentCreateMappingMode, Obq_EnvironmentSetMappingMode)
        self.addControl("flipU", label="Flip U")
        self.addControl("flipV", label="Flip V")
        self.endNoOptimize()
        self.endLayout()

        self.beginLayout("Ray Options", collapse=False)
        self.addCustom("onSurfaceMode", Obq_EnvironmentCreateOnSurfaceMode, Obq_EnvironmentSetOnSurfaceMode)
        self.addControl("ior", label="IOR")
        self.addControl("customDirection", label="Custom Direction")
        self.endLayout()

        self.beginLayout("Intensities", collapse=False)
        self.addControl("intensityCam", label="Camera")
        self.addControl("intensityDifGI", label="Diffuse GI")
        self.addControl("intensityRfl", label="Reflection")
        self.addControl("intensityGlossy", label="Glossy")
        self.addControl("intensityRfr", label="Refraction")
        self.endLayout()

        self.endLayout() # End Texture


        self.beginLayout("Roughness", collapse=False)
        self.addControl("sampleLevel", label="Sample Level")
        self.addControl("coneAngle", label="Cone Angle")
        self.addControl("cosLobeGloss", label="")
        self.addControl("useSampleCount", label="")
        self.addControl("sampleCount", label="")
        self.addControl("sampleCountMultiplier", label="")
        self.endLayout()

        self.beginLayout("Transformations", collapse=False)
        self.addControl("globalRotation", label="Use rotation at a global scope")
        self.addControl("rotation", label="Rotation")
        self.endLayout()

        # self.beginLayout("Options", collapse=False)
        # self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')
      
        self.addExtraControls()
        self.endScrollLayout()
