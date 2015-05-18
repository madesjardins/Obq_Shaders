# 2015-05-18 12.01 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.templates as templates
#from mtoa.ui.ae.customShapeAttributes import CameraTemplate as CameraTemplate
 
viewModeEnumOp = [
    (0, "Single (Right)"), 
    (1, "Left"), 
    (2, "Stereo <left-right>"),
    (3, "Stereo <down-up>")
]

def Obq_KettleUVStereoLensCreateViewMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_KettleUVStereoLensViewMode', attribute=attr, label="View Mode", enumeratedItem=viewModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_KettleUVStereoLensSetViewMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_KettleUVStereoLensViewMode', edit=True, attribute=attr)
    
stereoTypeEnumOp = [
    (0, "Neutral"), 
    (1, "Parallel"), 
    (2, "Converged")
]

def Obq_KettleUVStereoLensCreateStereoType(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_KettleUVStereoLensStereoType', attribute=attr, label="Stereo Type", enumeratedItem=stereoTypeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_KettleUVStereoLensSetStereoType(attr):
    cmds.attrEnumOptionMenuGrp('Obq_KettleUVStereoLensStereoType', edit=True, attribute=attr)
    
interaxialModeEnumOp = [
    (0, "Neutral"), 
    (1, "Parallel"), 
    (2, "Converged")
]

def Obq_KettleUVStereoLensCreateInteraxialMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_KettleUVStereoLensInteraxialMode', attribute=attr, label="Interaxial Separation", enumeratedItem=interaxialModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)
    
def Obq_KettleUVStereoLensSetInteraxialMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_KettleUVStereoLensInteraxialMode', edit=True, attribute=attr) 
    
def Obq_KettleUVStereoLensHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_KettleUVStereoLens'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_KettleUVStereoLens.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)
 
class Obq_KettleUVStereoLensTemplate(templates.AttributeTemplate):
    def setup(self):
        self.beginLayout("Obq_KettleUVStereoLens", collapse=False)
        
        Obq_KettleUVStereoLensHelpURL()
        
        self.beginLayout("Main", collapse=False)
        
        self.beginLayout("Polymeshes", collapse=False)
        self.addControl("origin_polymesh",  label="Origin Polymesh")
        self.addControl("target_polymesh",  label="Origin Camera")
        self.addControl("polymeshesStatus",  label="Status")
        self.endLayout() # end Polymeshes Layout
        
        self.beginLayout("Stereo", collapse=False)
        
        self.beginLayout("Rendered View", collapse=False)
        self.addCustom('view_mode', Obq_KettleUVStereoLensCreateViewMode, Obq_KettleUVStereoLensSetViewMode) 
        self.endLayout() # end Rendered View Layout
        
        self.beginLayout("Settings", collapse=False)
        self.addCustom('stereo_type', Obq_KettleUVStereoLensCreateStereoType, Obq_KettleUVStereoLensSetStereoType)
        self.addCustom('interaxial_mode', Obq_KettleUVStereoLensCreateInteraxialMode, Obq_KettleUVStereoLensSetInteraxialMode)
        
        self.addControl("interaxial_separation",  label="Interaxial Separation")
        self.addControl("zero_parallax_mode",  label="Zero Parallax Mode")
        self.addControl("zero_parallax_distance",  label="Zero Parallax Distance")
        self.endLayout() # end Settings Layout
        
        self.beginLayout("Automatic Overscan", collapse=False)
        self.addControl("filterSize",  label="Filter size")
        
        self.beginLayout("Target Resolution", collapse=False)
        self.addControl("targetResolutionX",  label="Width")
        self.addControl("targetResolutionY",  label="Height")
        self.endLayout() # end Target Resolution Layout
        
        self.beginLayout("Render Resolution", collapse=False)
        self.addControl("renderResolutionX",  label="Width")
        self.addControl("renderResolutionY",  label="Height")
        
        self.addControl("updatePassResolution",  label="Automatic update of pass output resolution")
        self.endLayout() # end Render Resolution Layout
        
        self.beginLayout("Nuke Info", collapse=False)
        self.addControl("leftCropInfo",  label="Left crop")
        self.addControl("rightCropInfo",  label="Right crop")
        self.endLayout() # end Nuke Info Layout
        
        self.endLayout() # end Automatic Overscan Layout
        self.endLayout() # end Stereo Layout
                
        # ---------------------------------------------------------------------        
        
        self.beginLayout("Region", collapse=False)

        self.beginLayout("Render Region", collapse=False)
        self.addControl("useRenderRegion",  label="Render Region Only")
        self.addControl("cropToRegion",  label="Crop to Region")
        self.endLayout() # end Render Region Layout 
        
        self.beginLayout("Lower bound UV", collapse=False)
        self.addControl("regionU0",  label="U0")
        self.addControl("regionV0",  label="V0")
        self.endLayout() # end Lower bound UV Layout 
        
        self.beginLayout("Higher bound UV", collapse=False)
        self.addControl("regionU1",  label="U1")
        self.addControl("regionV1",  label="V1")
        self.endLayout() # end Higher bound UV Layout 
        
        self.endLayout() # end Region Layout
       
        # ---------------------------------------------------------------------
       
        self.beginLayout("Advanced", collapse=False)
        
        self.beginLayout("Acceleration", collapse=False)
        self.addControl("grid_size",  label="Grid Size")
        self.endLayout() # end Acceleration Layout  

        self.beginLayout("Other", collapse=False)
        self.addControl("interaxial_epsilon",  label="Interaxial Separation Epsilon")
        self.endLayout() # end Other Layout 
        
        self.endLayout() # end Advanced Layout 
        
        # ---------------------------------------------------------------------
        
        self.beginLayout("Debug", collapse=False)
        
        self.beginLayout("Cameras", collapse=False)
        self.addControl("origin_camera",  label="Origin Camera")
        self.addControl("target_camera",  label="Target Camera")
        self.endLayout() # end Cameras Layout 
        
        self.beginLayout("Overscan", collapse=False)
        self.addControl("totalOverscanPixels",  label="Total Pixel Overscan")
        self.endLayout() # end Overscan Layout
        
        self.endLayout() # end Debug Layout 

        # ---------------------------------------------------------------------
        
        self.endLayout() # end Main Layout
        self.endLayout() # end Obq_KettleUVStereoLens layout

        self.beginLayout("Options", collapse=True )
        self.addControl("aiUserOptions", label="User Options")
        self.endLayout()

templates.registerTranslatorUI(Obq_KettleUVStereoLensTemplate, "camera", "Obq_KettleUVStereoLens")
#templates.registerTranslatorUI(Obq_KettleUVStereoLensTemplate, "stereoRigCamera", "Obq_KettleUVStereoLens")
