# 2014-11-30 12.54 pm
# Todo:
# Implement the ColorspaceIn and Colorspace Out UI elements

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

colorspaceInModeEnumOp = [
    (3, 'CIE XYZ'), 
    (0, 'XSI Linear'), 
    (1, 'XSI sRGB'), 
    (2, 'CIE RGB'), 
]

def Obq_ColorspaceConverterCreateColorspaceInMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConverterColorspaceInMode', attribute=attr, label="Colorspace In", 
                               enumeratedItem=colorspaceInModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_ColorspaceConverterSetColorspaceInMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConverterColorspaceInMode', edit=True, attribute=attr)

colorspaceOutModeEnumOp = [
    (3, 'CIE XYZ'), 
    (0, 'XSI Linear'), 
    (1, 'XSI sRGB'), 
    (2, 'CIE RGB'), 
]

def Obq_ColorspaceConverterCreateColorspaceOutMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConverterColorspaceOutMode', attribute=attr, label="Colorspace Out", 
                               enumeratedItem=colorspaceOutModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_ColorspaceConverterSetColorspaceOutMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConverterColorspaceOutMode', edit=True, attribute=attr)
    
csGammaInModeEnumOp = [
    (0, 'Linear'), 
    (3, 'sRGB'), 
    (5, 'Rec.709'), 
    (1, '1.8 (Apple)'), 
    (2, '2.2'), 
    (4, '2.4'), 
    (6, 'L*'), 
]

def Obq_ColorspaceConverterCreatecsGammaInMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsGammaInMode', attribute=attr, label="Gamma In", 
                               enumeratedItem=csGammaInModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_ColorspaceConverterSetcsGammaInMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsGammaInMode', edit=True, attribute=attr)
    
    
csGammaOutModeEnumOp = [
    (0, 'Linear'), 
    (3, 'sRGB'), 
    (5, 'Rec.709'), 
    (1, '1.8 (Apple)'), 
    (2, '2.2'), 
    (4, '2.4'), 
    (6, 'L*'), 
]

def Obq_ColorspaceConverterCreatecsGammaOutMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsGammaOutMode', attribute=attr, label="Gamma Out", 
                               enumeratedItem=csGammaOutModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_ColorspaceConverterSetcsGammaOutMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsGammaOutMode', edit=True, attribute=attr)
        
csIlluminantInModeEnumOp = [
    (0, 'A'), 
    (1, 'B'), 
    (2, 'C'), 
    (3, 'D50'), 
    (4, 'D55'), 
    (5, 'D65'), 
    (6, 'D75'), 
    (7, 'E'), 
    (8, 'F2'), 
    (9, 'F7'), 
    (10, 'F11'), 
]

def Obq_ColorspaceConverterCreatecsIlluminantInMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsIlluminantInMode', attribute=attr, label="Illuminant In", 
                               enumeratedItem=csIlluminantInModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_ColorspaceConverterSetcsIlluminantInMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsIlluminantInMode', edit=True, attribute=attr)
    
csIlluminantOutModeEnumOp = [
    (0, 'A'), 
    (1, 'B'), 
    (2, 'C'), 
    (3, 'D50'), 
    (4, 'D55'), 
    (5, 'D65'), 
    (6, 'D75'), 
    (7, 'E'), 
    (8, 'F2'), 
    (9, 'F7'), 
    (10, 'F11'), 
]

def Obq_ColorspaceConverterCreatecsIlluminantOutMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsIlluminantOutMode', attribute=attr, label="Illuminant Out", 
                               enumeratedItem=csIlluminantOutModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_ColorspaceConverterSetcsIlluminantOutMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsIlluminantOutMode', edit=True, attribute=attr)

csPrimariesInModeEnumOp = [
    (19, 'CIE XYZ'), 
    (0, 'CIE XYZ'), 
    (1, 'Adobe RGB 1998'), 
    (2, 'Apple RGB'), 
    (3, 'Best RGB'), 
    (4, 'Beta RGB'), 
    (5, 'Bruce RGB'), 
    (6, 'CIE RGB'), 
    (7, 'ColorMatch RGB'), 
    (8, 'Don RGB 4'), 
    (9, 'ECI RGB V2'), 
    (10, 'EKTA Space PS5'), 
    (11, 'NTSC RGB'), 
    (12, 'PAL/SECAM RGB'), 
    (13, 'ProPhoto'), 
    (14, 'SMPTE_C'), 
    (15, 'sRGB'), 
    (16, 'Wide Gamut RGB'), 
    (17, 'Rec.709'), 
    (18, 'Rec.2020'), 
    (19, 'CIE XYZ'), 
]

def Obq_ColorspaceConverterCreatecsPrimariesInMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsPrimariesInMode', attribute=attr, label="Primaries In", 
                               enumeratedItem=csPrimariesInModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_ColorspaceConverterSetcsPrimariesInMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsPrimariesInMode', edit=True, attribute=attr)
    
    
csPrimariesOutModeEnumOp = [
    (19, 'CIE XYZ'), 
    (0, 'CIE XYZ'), 
    (1, 'Adobe RGB 1998'), 
    (2, 'Apple RGB'), 
    (3, 'Best RGB'), 
    (4, 'Beta RGB'), 
    (5, 'Bruce RGB'), 
    (6, 'CIE RGB'), 
    (7, 'ColorMatch RGB'), 
    (8, 'Don RGB 4'), 
    (9, 'ECI RGB V2'), 
    (10, 'EKTA Space PS5'), 
    (11, 'NTSC RGB'), 
    (12, 'PAL/SECAM RGB'), 
    (13, 'ProPhoto'), 
    (14, 'SMPTE_C'), 
    (15, 'sRGB'), 
    (16, 'Wide Gamut RGB'), 
    (17, 'Rec.709'), 
    (18, 'Rec.2020'), 
    (19, 'CIE XYZ'), 
]

def Obq_ColorspaceConverterCreatecsPrimariesOutMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsPrimariesOutMode', attribute=attr, label="Primaries Out", 
                               enumeratedItem=csPrimariesOutModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_ColorspaceConverterSetcsPrimariesOutMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ColorspaceConvertercsPrimariesOutMode', edit=True, attribute=attr)
    
    
    
def Obq_ColorspaceConverterHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_ColorspaceConverter'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_ColorspaceConverter.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class AEObq_ColorspaceConverterTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):

        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_ColorspaceConverterFormLayout")
        Obq_ColorspaceConverterHelpURL()

        self.beginLayout('Colorspace Converter Attributes', collapse=False)
         
        self.beginLayout("Input", collapse=False)
        self.addControl("color", label="Color")
        self.addCustom("csPrimariesIn", Obq_ColorspaceConverterCreatecsPrimariesInMode, Obq_ColorspaceConverterSetcsPrimariesInMode)
        self.addCustom("csGammaIn", Obq_ColorspaceConverterCreatecsGammaInMode, Obq_ColorspaceConverterSetcsGammaInMode)
        self.addCustom("csIlluminantIn", Obq_ColorspaceConverterCreatecsIlluminantInMode, Obq_ColorspaceConverterSetcsIlluminantInMode)
        self.endLayout()
        
        self.beginLayout("Output", collapse=False )
        self.addCustom("csPrimariesOut", Obq_ColorspaceConverterCreatecsPrimariesOutMode, Obq_ColorspaceConverterSetcsPrimariesOutMode)
        self.addCustom("csGammaOut", Obq_ColorspaceConverterCreatecsGammaOutMode, Obq_ColorspaceConverterSetcsGammaOutMode)
        self.addCustom("csIlluminantOut", Obq_ColorspaceConverterCreatecsIlluminantOutMode, Obq_ColorspaceConverterSetcsIlluminantOutMode)
        self.endLayout()
        
        self.beginLayout("Options", collapse=False)
        self.beginNoOptimize()
        self.addControl("switchInOut", label="Switch In and Out")
        self.addControl("useBradford", label="Use Bradford chromatic adaptation matrix")
        self.addControl("clamp", label="Clamp")
        self.endNoOptimize()
        self.endLayout()

        self.beginLayout("Load Colorspace Presets", collapse=True)
        #self.addCustom("colorspaceIn", Obq_ColorspaceConverterCreateColorspaceInMode, Obq_ColorspaceConverterSetColorspaceInMode)
        #self.addCustom("colorspaceOut", Obq_ColorspaceConverterCreateColorspaceOutMode, Obq_ColorspaceConverterSetColorspaceOutMode)
        self.endLayout()
        
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        # Hide the NormalCamera and HardwareColor Extra Attributes
        self.suppress('normalCamera')
        self.suppress('hardwareColor')

        self.addExtraControls()
        self.endScrollLayout()
