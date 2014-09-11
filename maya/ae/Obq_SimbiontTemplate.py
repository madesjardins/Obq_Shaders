# 2014-09-11 01.02 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

evaluateRGBEnumOp = [
    (-1, 'Black'), 
    (0, 'Color'), 
    (1, 'Percent'), 
    (2, 'Elevation'), 
    (3, 'Diffuse level'), 
    (4, 'Diffuse Func'), 
    (5, 'Luminosity'), 
    (6, 'Specular Level'), 
    (7, 'Specular Func'), 
    (8, 'Specular Color'), 
    (9, 'Glossiness'), 
    (10, 'Metal Level'), 
    (11, 'Anisotropy'), 
    (12, 'Aniso Dir'), 
    (13, 'Reflectivity'), 
    (14, 'Environment'), 
    (15, 'Transparency'), 
    (16, 'Refraction'), 
    (17, 'Clear Coat Level'), 
    (18, 'Clear Coat Glossiness'), 
    (19, 'Clear Coat Thickness'), 
    (20, 'Clear Coat Smoothness'), 
    (21, 'Bump'), 
    (22, 'Alpha'), 
]

def Obq_SimbiontCreateEvaluateRGB(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_SimbiontEvaluateRGBE', attribute=attr, label="RGB", 
                               enumeratedItem=evaluateRGBEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_SimbiontSetEvaluateRGB(attr):
    cmds.attrEnumOptionMenuGrp('Obq_SimbiontEvaluateRGBE', edit=True, attribute=attr)

evaluateAlphaEnumOp = [
    (-1, 'Black'), 
    (0, 'Color'), 
    (1, 'Percent'), 
    (2, 'Elevation'), 
    (3, 'Diffuse level'), 
    (4, 'Diffuse Func'), 
    (5, 'Luminosity'), 
    (6, 'Specular Level'), 
    (7, 'Specular Func'), 
    (8, 'Specular Color'), 
    (9, 'Glossiness'), 
    (10, 'Metal Level'), 
    (11, 'Anisotropy'), 
    (12, 'Aniso Dir'), 
    (13, 'Reflectivity'), 
    (14, 'Environment'), 
    (15, 'Transparency'), 
    (16, 'Refraction'), 
    (17, 'Clear Coat Level'), 
    (18, 'Clear Coat Glossiness'), 
    (19, 'Clear Coat Thickness'), 
    (20, 'Clear Coat Smoothness'), 
    (21, 'Bump'), 
    (22, 'Alpha'), 
]

def Obq_SimbiontCreateEvaluateAlpha(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_SimbiontEvaluateAlpha', attribute=attr, label="Alpha", 
                               enumeratedItem=evaluateAlphaEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_SimbiontSetEvaluateAlpha(attr):
    cmds.attrEnumOptionMenuGrp('Obq_SimbiontEvaluateAlpha', edit=True, attribute=attr)

coordinateSystemEnumOp = [
    (0, 'Object Space'), 
    (1, 'World Space'), 
    (2, 'UV Space'), 
    (3, 'Camera Space'), 
    (4, 'Raster Space'), 
    (5, 'Custom'), 
]

def Obq_SimbiontCreateCoordinateSystem(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_SimbiontCoordinateSystem', attribute=attr, label="Coordinate System", 
                               enumeratedItem=coordinateSystemEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_SimbiontSetCoordinateSystem(attr):
    cmds.attrEnumOptionMenuGrp('Obq_SimbiontCoordinateSystem', edit=True, attribute=attr)

def Obq_SimbiontHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Simbiont'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Simbiont.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_SimbiontTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        # Hide the node swatch preview until the shader is able to render a preview
        #self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_SimbiontFormLayout")
        Obq_SimbiontHelpURL()

        #self.beginNoOptimize()

        self.beginLayout("Simbiont", collapse=False )

        self.beginLayout("Dark Tree", collapse=False )
        #Turn the filename field into a folder picker
        self.addControl("filename", label="DarkTree File", annotation="The Dark Tree file you want to load.")
        #Turn the componentLibsPath field into a folder picker
        #self.addControl("componentLibsPath", label="DarkTree component libs Path")
        self.addControl("componentLibsPath", label="DT Component Libs Path", annotation="This is the path to the DarkTree Engine \"dte_components\" \nlibraries. If this path is incorrect, you will see red Xs as \nthe resulting texture.")
        self.endLayout()

        self.beginLayout("Output", collapse=True )
        #self.addControl("evaluateRGB", label="RGB")
        self.addCustom("evaluateRGB", Obq_SimbiontCreateEvaluateRGB, Obq_SimbiontSetEvaluateRGB)

        self.addControl("outRRR", label="Grayscale")
        #self.addControl("evaluateAlpha", label="Alpha")
        self.addCustom("evaluateAlpha", Obq_SimbiontCreateEvaluateAlpha, Obq_SimbiontSetEvaluateAlpha)
        self.endLayout()

        self.beginLayout("Bump", collapse=True )
        self.addControl("sampleSize", label="Sample Size")
        self.addControl("bumpScale", label="Bump Scale")
        self.endLayout()

        self.beginLayout("Camera", collapse=True )
        self.addControl("cameraNear", label="Camera Near")
        self.addControl("cameraFar", label="Camera Far")
        self.endLayout()

        self.beginLayout("Others", collapse=True )
        self.addControl("continuousRep", label="Continuous Rep")
        self.addControl("regionRep", label="Region Rep")
        self.endLayout()

        self.beginLayout("Animation", collapse=False )
        self.addControl("frameNumber", label="Time")
        self.endLayout()

        self.beginLayout("Coordinates", collapse=True )
        #self.addControl("coordinateSystem", label="Coordinate System")
        self.addCustom("coordinateSystem", Obq_SimbiontCreateCoordinateSystem, Obq_SimbiontSetCoordinateSystem)
        self.addControl("customCoordinates", label="Custom Coordinates")
        self.endLayout()
        self.beginLayout("Transform", collapse=True )
        self.addControl("coordinateScale", label="Coordinate Scale")
        self.endLayout()


        self.beginLayout("Tweaks", collapse=False )

        self.beginLayout("Acceleration", collapse=True )
        self.addControl("globalTweaksOnly", label="Global Tweaks Only")
        self.endLayout()
        self.beginLayout("Tweak Actions", collapse=True )
        self.endLayout()

        self.beginLayout("Tweak # 0", collapse=False )
        self.addControl("tweakEnable0", label="Enable")
        self.addControl("tweakName0", label="Name")
        self.addControl("tweakColor0", label="Color")
        self.addControl("tweakScalar0", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 1", collapse=False )
        self.addControl("tweakEnable1", label="Enable")
        self.addControl("tweakName1", label="Name")
        self.addControl("tweakColor1", label="Color")
        self.addControl("tweakScalar1", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 2", collapse=False )
        self.addControl("tweakEnable2", label="Enable")
        self.addControl("tweakName2", label="Name")
        self.addControl("tweakColor2", label="Color")
        self.addControl("tweakScalar2", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 3", collapse=False )
        self.addControl("tweakEnable3", label="Enable")
        self.addControl("tweakName3", label="Name")
        self.addControl("tweakColor3", label="Color")
        self.addControl("tweakScalar3", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 4", collapse=False )
        self.addControl("tweakEnable4", label="Enable")
        self.addControl("tweakName4", label="Name")
        self.addControl("tweakColor4", label="Color")
        self.addControl("tweakScalar4", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 5", collapse=False )
        self.addControl("tweakEnable5", label="Enable")
        self.addControl("tweakName5", label="Name")
        self.addControl("tweakColor5", label="Color")
        self.addControl("tweakScalar5", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 6", collapse=False )
        self.addControl("tweakEnable6", label="Enable")
        self.addControl("tweakName6", label="Name")
        self.addControl("tweakColor6", label="Color")
        self.addControl("tweakScalar6", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 7", collapse=False )
        self.addControl("tweakEnable7", label="Enable")
        self.addControl("tweakName7", label="Name")
        self.addControl("tweakColor7", label="Color")
        self.addControl("tweakScalar7", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 8", collapse=False )
        self.addControl("tweakEnable8", label="Enable")
        self.addControl("tweakName8", label="Name")
        self.addControl("tweakColor8", label="Color")
        self.addControl("tweakScalar8", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 9", collapse=False )
        self.addControl("tweakEnable9", label="Enable")
        self.addControl("tweakName9", label="Name")
        self.addControl("tweakColor9", label="Color")
        self.addControl("tweakScalar9", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 10", collapse=False )
        self.addControl("tweakEnable10", label="Enable")
        self.addControl("tweakName10", label="Name")
        self.addControl("tweakColor10", label="Color")
        self.addControl("tweakScalar10", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 11", collapse=False )
        self.addControl("tweakEnable11", label="Enable")
        self.addControl("tweakName11", label="Name")
        self.addControl("tweakColor11", label="Color")
        self.addControl("tweakScalar11", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 12", collapse=False )
        self.addControl("tweakEnable12", label="Enable")
        self.addControl("tweakName12", label="Name")
        self.addControl("tweakColor12", label="Color")
        self.addControl("tweakScalar12", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 13", collapse=False )
        self.addControl("tweakEnable13", label="Enable")
        self.addControl("tweakName13", label="Name")
        self.addControl("tweakColor13", label="Color")
        self.addControl("tweakScalar13", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 14", collapse=False )
        self.addControl("tweakEnable14", label="Enable")
        self.addControl("tweakName14", label="Name")
        self.addControl("tweakColor14", label="Color")
        self.addControl("tweakScalar14", label="Float")
        self.endLayout()

        self.beginLayout("Tweak # 15", collapse=False )
        self.addControl("tweakEnable15", label="Enable")
        self.addControl("tweakName15", label="Name")
        self.addControl("tweakColor15", label="Color")
        self.addControl("tweakScalar15", label="Float")
        self.endLayout()

        self.endLayout() # End Tweaks Layout

        self.endLayout() # End Simbiont Layout

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()

