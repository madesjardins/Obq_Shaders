# 2014-09-10 05.58 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

radiusFalloffEnumOp = [
    (0, "Don't Use Radius Falloff"), 
    (1, "Get Center From Object Matrix"), 
    (2, "Get Center From Object Center Vector")
]

def Obq_AtmosphereCreateRadiusFalloffMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_AtmosphereRadiusFalloffMode', attribute=attr, label="Mode", enumeratedItem=radiusFalloffEnumOp, annotation="This mode was designed for spherical objects like planets. \nSamples closer to the edge of the sphere absorb less. The different modes are :\n-Don't use Radius Falloff : should be selected for simple ray \nmarching mode.\n-Get center from object matrix : use the matrix of the object \nto find center. In order for this to work, you must use \nthe user option \"preserve_scene_data true\".-Get center from object center vector : use the object center \nvector in the \"Geo\" tab as center.")    
    cmds.setUITemplate(popTemplate=True)

def Obq_AtmosphereSetRadiusFalloffMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_AtmosphereRadiusFalloffMode', edit=True, attribute=attr)

compensateEnumOp = [
    (0, "Never"), 
    (1, "By dropping max opacity"), 
    (2, "By raising total opacity")
]

def Obq_AtmosphereCreateCompensateForCloudOpacity(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_AtmosphereCompensateForCloudOpacity', attribute=attr, label="Compensate", enumeratedItem=compensateEnumOp, annotation="When a non-opaque object is hit during the ray marching process, \na compensation must be done.")    
    cmds.setUITemplate(popTemplate=True)

def Obq_AtmosphereSetCompensateForCloudOpacity(attr):
    cmds.attrEnumOptionMenuGrp('Obq_AtmosphereCompensateForCloudOpacity', edit=True, attribute=attr)

divideEnumOp = [
    (0, "Never"), 
    (1, "By dropping max opacity"), 
    (2, "By raising total opacity")
]

def Obq_AtmosphereCreateDivideColorByOpacity(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_AtmosphereDivideColorByOpacity', attribute=attr, label="Divide", enumeratedItem=divideEnumOp, annotation="Because there will be a double alpha multiplication effect if \nleft as-is for values lower than 1, a compensation must be \ndone, a sort of unpremult operation.\n\nNever : don't unpremult color.\nBefore Bias-Gain : unpremult before bias and gain operations [default].\nAfter Bias-Gain : unpremult after bias and gain operations.")    
    cmds.setUITemplate(popTemplate=True)

def Obq_AtmosphereSetDivideColorByOpacity(attr):
    cmds.attrEnumOptionMenuGrp('Obq_AtmosphereDivideColorByOpacity', edit=True, attribute=attr, annotation="The ray marching will also occur for reflection and refraction \nrays and not only camera rays.")

def Obq_AtmosphereHelpURL():
    #Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Atmosphere'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Atmosphere.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_AtmosphereTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_AtmosphereFormLayout")
        Obq_AtmosphereHelpURL()

        self.beginLayout( "Main", collapse=False )

        self.beginLayout("Volume", collapse=False)
        self.addControl("colorKd", label="Diffuse Color", annotation="The diffuse color of media.")
        self.addControl("use3dKd", label="Enable 3D color texture", annotation="Enables the evaluation of a 3D texture and not just the \nsurface texture for diffuse color.")
        self.addControl("absorption", label="Absorption Coefficient", annotation="The absorption coefficient of the media")
        self.addControl("use3dAbsorption", label="Enable 3D absorption function", annotation="Enables the evaluation of a 3D texture and not just the \nsurface texture for absorption.")
        self.addControl("asymmetry", label="Asymmetry", annotation="Asymmetry of the phase function, negative value correspond \nto back-scattering, 0 is isotropic and positive correspond \nto forward-scattering.")
        self.endLayout()
        self.beginLayout("Radius Falloff", collapse=False)
        self.addCustom('radiusFalloffMode', Obq_AtmosphereCreateRadiusFalloffMode, Obq_AtmosphereSetRadiusFalloffMode)
        self.addControl("falloffStart",  label="Start", annotation="Radius ratio at which the falloff start. ")
        self.addControl("falloffExponent",  label="Speed", annotation="Exponent that modifies the rate of the falloff.")
        self.endLayout()
        self.beginLayout("Silhouette Falloff", collapse=False)
        self.addControl("useBiasGainDot",  label="Enable", annotation="Enables a silhouette falloff similar to an incidence \ncamera/surface node.")
        self.addControl("biasDot",  label="Bias", annotation="Controls the contrast. Values below 0.5 create larger dark areas, \nvalues above 0.5 create larger light areas. ")
        self.addControl("gainDot",  label="Object Center", annotation="Controls the brightness of the effect.")
        self.endLayout()
        self.endLayout() # End Main


        self.beginLayout("Geometric Information", collapse=True)
        self.addControl("objectCenter",  label="Object Center", annotation="Center of the sphere. Used for \"Get center from \nobject center vector\" radius falloff mode.")
        self.endLayout()


        self.beginLayout("Ray Marching", collapse=True)
        self.beginLayout("Steps", collapse=False)
        self.addControl("stepSize",  label="Size", annotation="Size of each steps in the ray marching process in units. It \nis important to scale the steps based on the scale of the \nscene. The bigger the steps, the faster the render.")
        self.addControl("stepJitter",  label="Jitter ratio", annotation="Random step size ratio modification to prevent artifacts.")
        self.endLayout()
        self.beginLayout("Ray length", collapse=False)
        self.addControl("useMaximumRayLength",  label="Enable maximum ray length", annotation="Enables a maximum ray length after which the march stops and \nreturn accumulated color.")
        self.addControl("maximumRayLength",  label="Maximum ray length", annotation="Maximum ray length.")
        self.endLayout()
        self.endLayout() # End Ray Marching


        self.beginLayout("Bias-Gain", collapse=True)
        self.beginLayout("Final Opacity Modification", collapse=False)
        self.addControl("useBiasGain",  label="Enable", annotation="Enables a bias and gain operation on the final opacity.")
        self.addControl("bias",  label="Bias", annotation="Controls the contrast. Values below 0.5 create larger dark areas, \nvalues above 0.5 create larger light areas.")
        self.addControl("gain",  label="Gain", annotation="Controls the brightness of the effect.")
        self.endLayout()
        self.endLayout() # End Bias-Gain


        self.beginLayout("Advanced Options", collapse=True)
        self.beginLayout("Non-opaque hit compensation", collapse=False)
        self.addCustom('compensateForCloudOpacity', Obq_AtmosphereCreateCompensateForCloudOpacity, Obq_AtmosphereSetCompensateForCloudOpacity)
        self.endLayout()
        self.beginLayout("Color/Opacity compensation", collapse=False)
        self.addCustom('divideColorByOpacity', Obq_AtmosphereCreateDivideColorByOpacity, Obq_AtmosphereSetDivideColorByOpacity)
        self.endLayout()
        self.beginLayout("Ray Options", collapse=False)
        self.addControl("secondaryTypesToo",  label="Enable for reflection and refraction rays", annotation="The ray marching will also occur for reflection and \nrefraction rays and not only camera rays.")
        self.endLayout()
        self.endLayout() # End Advanced Options


        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()

