# 2014-12-02 02.44 am
# Note: Presets have been done as Maya stock attrPresets
#       ior4standard and iorExpression are not yet implemented

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_FresnelHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_Fresnel'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_Fresnel.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_FresnelTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_FresnelFormLayout")
        Obq_FresnelHelpURL()

        self.beginLayout("Main", collapse=False )
        
        self.beginLayout("Settings", collapse=False )
        self.addControl("mode", label="Mode")
        self.addControl("inputType", label="Input Type")
        self.endLayout() #End Settings
        
        self.beginLayout("SPD (Strings or File *.csv, *.txt, *.spd)", collapse=False )
        self.addControl("iorFilename", label="Filename")
        self.addControl("lambdasStr", label="Lambda")
        self.addControl("etasStr", label="Eta (n)")
        self.addControl("ksStr", label="k")
        self.addControl("lambdaUnits", label="Wavelength Units")
        self.endLayout() #End File
        
        self.beginLayout("Refractive Indices", collapse=False )
        self.addControl("iorRGB", label="Eta (n)")
        self.endLayout() #End Refractive Indices
        
        self.beginLayout("Extinction Coefficients", collapse=False )
        self.addControl("kRGB", label="k")
        self.endLayout() #End Extinction Coefficients
        
        self.beginLayout("Equations", collapse=False )
        self.addControl("method", label="Material Type")
        self.endLayout() #End Equations
        
        self.beginLayout("Options", collapse=False )
        self.addControl("roughness", label="Roughness")
        self.addControl("ratioFsFp", label="Ratio of polarized Fs/Fp")
        self.addControl("backfaceMode", label="Backface")
        self.endLayout() #End Options
        
        self.beginLayout("Output", collapse=False )
        self.beginNoOptimize()
        self.addControl("transmittance", label="Output Refraction Color")
        self.endNoOptimize()
        self.endLayout() #End Output
        
        self.beginLayout("Advanced", collapse=True )
        
        self.beginLayout("Media In Refractive Indices", collapse=False )
        self.addControl("iorInRGB", label="Eta (n)")
        self.endLayout() #End Media In Refractive Indices
        
        self.beginLayout("LookUp Table", collapse=True )
        self.addControl("useLUT", label="Enable precalculated LUT")
        self.addControl("LUTSampleSize", label="Sample size (degrees)")
        self.addControl("roughnessSampleSize", label="Roughness sample size")
        self.endLayout() #End LookUp Table
        
        self.beginLayout("Spectrum", collapse=False )
        self.addControl("useFullSpectrum", label="Use full spectrum data in equations (metal)")
        self.endLayout() #End Spectrum
        
        self.beginLayout("ColorSpace", collapse=True )
        self.addControl("xyz2rgb", label="XYZ to RGB Matrix")
        self.addControl("outputGamma", label="Gamma")
        self.addControl("degamma", label="DeGamma metals for linear workflow (LEGACY)")
        self.endLayout() #End ColorSpace
        
        self.beginLayout("Color Adjustment", collapse=True )
        self.addControl("ccMode", label="Mode")
        self.addControl("hueShift", label="Hue Shift (degree)")
        
        self.beginLayout("Saturation", collapse=True )
        self.addControl("saturationMod", label="Modifier")        
        self.addControl("saturationOp", label="Operation")
        self.endLayout() #End Saturation
     
        self.endLayout() #End Color Adjustment
        
        self.endLayout() #End Advanced
        
        self.endLayout() #End Main
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
