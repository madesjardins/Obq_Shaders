# 2015-04-25 10.57 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.templates as templates
#from mtoa.ui.ae.customShapeAttributes import CameraTemplate as CameraTemplate
 
LensDistortionModelEnumOp = [
    (7, 'PFBarrel'), 
    (0, 'Nuke'), 
    (1, '3DE Classic LD Model'), 
    (2, '3DE4 Anamorphic, Degree 6'), 
    (3, '3DE4 Radial - Fisheye, Degree 8'),
    (4, '(DEPRICATED) 3DE4 Radial - Standard, Degree 4'),
    (5, '3DE4 Radial - Decentered Cylindric, Degree 4'),
    (6, '3DE4 Anamorphic Rotate Squeeze, Degree 4'),
    (8, 'none')
]

def Obq_LensDistortionCreateModelMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_LensDistortionModelMode', attribute=attr, label="Lens Distortion Model", 
                               enumeratedItem=LensDistortionModelEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_LensDistortionSetModelMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_LensDistortionModelMode', edit=True, attribute=attr)
    

def Obq_LensDistortionHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_LensDistortion'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_LensDistortion.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)
 
class Obq_LensDistortionTemplate(templates.AttributeTemplate):
    def setup(self):
        self.beginLayout("Obq_LensDistortion", collapse=False)
        
        Obq_LensDistortionHelpURL()
        
        self.beginNoOptimize()
        
        self.beginLayout("Distortion Model", collapse=False)
        #self.addControl("aiDistortionModel", label="Distortion Model", annotation="Distortion Model")
        self.addCustom("aiDistortionModel", Obq_LensDistortionCreateModelMode, Obq_LensDistortionSetModelMode)
        self.endLayout()
        
        self.beginLayout("Distortion Coefficients", collapse=False)
        self.addControl("aiK1", label="Radial Distortion 1", annotation="Radial Distortion 1")
        self.addControl("aiK2", label="Radial Distortion 2", annotation="Radial Distortion 2")
        self.addControl("aiAnamorphicSqueeze", label="Anamorphic Squeeze", annotation="Anamorphic Squeeze")
        self.addControl("aiAsymmetricDistortionX", label="Asymmetric Distortion X", annotation="Asymmetric Distortion X")
        self.addControl("aiAsymmetricDistortionY", label="Asymmetric Distortion Y", annotation="Asymmetric Distortion Y")
        self.addControl("aiC3dc00", label="Distortion", annotation="Distortion")
        self.addControl("aiC3dc01", label="Anamorphic Squeeze", annotation="Anamorphic Squeeze")
        self.addControl("aiC3dc02", label="Curvature X", annotation="Curvature X")
        self.addControl("aiC3dc03", label="Curvature Y", annotation="Curvature Y")
        self.addControl("aiC3dc04", label="Quartic Distortion", annotation="Quartic Distortion")
        self.addControl("aiAna6c00", label="Cx02 - Degree 2", annotation="Cx02 - Degree 2")
        self.addControl("aiAna6c01", label="Cy02 - Degree 2", annotation="Cy02 - Degree 2")
        self.addControl("aiAna6c02", label="Cx22 - Degree 2", annotation="Cx22 - Degree 2")
        self.addControl("aiAna6c03", label="Cy22 - Degree 2", annotation="Cy22 - Degree 2")
        self.addControl("aiAna6c04", label="Cx04 - Degree 4", annotation="Cx04 - Degree 4")
        self.addControl("aiAna6c05", label="Cy04 - Degree 4", annotation="Cy04 - Degree 4")
        self.addControl("aiAna6c06", label="Cx24 - Degree 4", annotation="Cx24 - Degree 4")
        self.addControl("aiAna6c07", label="Cy24 - Degree 4", annotation="Cy24 - Degree 4")
        self.addControl("aiAna6c08", label="Cx44 - Degree 4", annotation="Cx44 - Degree 4")
        self.addControl("aiAna6c09", label="Cy44 - Degree 4", annotation="Cy44 - Degree 4")
        self.addControl("aiAna6c10", label="Cx06 - Degree 6", annotation="Cx06 - Degree 6")
        self.addControl("aiAna6c11", label="Cy06 - Degree 6", annotation="Cy06 - Degree 6")
        self.addControl("aiAna6c12", label="Cx26 - Degree 6", annotation="Cx26 - Degree 6")
        self.addControl("aiAna6c13", label="Cy26 - Degree 6", annotation="Cy26 - Degree 6")
        self.addControl("aiAna6c14", label="Cx46 - Degree 6", annotation="Cx46 - Degree 6")
        self.addControl("aiAna6c15", label="Cy46 - Degree 6", annotation="Cy46 - Degree 6")
        self.addControl("aiAna6c16", label="Cx66 - Degree 6", annotation="Cx66 - Degree 6")
        self.addControl("aiAna6c17", label="Cy66 - Degree 6", annotation="Cy66 - Degree 6")
        self.addControl("aiFish8c00", label="Distortion - Degree 2", annotation="Distortion - Degree 2")
        self.addControl("aiFish8c01", label="Quartic Distortion - Degree 4", annotation="Quartic Distortion - Degree 4")
        self.addControl("aiFish8c02", label="Degree 6", annotation="Degree 6")
        self.addControl("aiFish8c03", label="Degree 8", annotation="Degree 8")
        self.addControl("aiStand4c00", label="Distortion - Degree 2", annotation="Distortion - Degree 2")
        self.addControl("aiStand4c01", label="U - Degree 2", annotation="U - Degree 2")
        self.addControl("aiStand4c02", label="V - Degree 2", annotation="V - Degree 2")
        self.addControl("aiStand4c03", label="Quartic Distortion - Degree 4", annotation="Quartic Distortion - Degree 4")
        self.addControl("aiStand4c04", label="U - Degree 4", annotation="U - Degree 4")
        self.addControl("aiStand4c05", label="V - Degree 4", annotation="V - Degree 4")
        self.addControl("aiRaddec4c00", label="Distortion - Degree 2", annotation="Distortion - Degree 2")
        self.addControl("aiRaddec4c01", label="U - Degree 2", annotation="U - Degree 2")
        self.addControl("aiRaddec4c02", label="V - Degree 2", annotation="V - Degree 2")
        self.addControl("aiRaddec4c03", label="Quartic Distortion - Degree 4", annotation="Quartic Distortion - Degree 4")
        self.addControl("aiRaddec4c04", label="U - Degree 4", annotation="U - Degree 4")
        self.addControl("aiRaddec4c05", label="V - Degree 4", annotation="V - Degree 4")
        self.addControl("aiRaddec4c06", label="Phi - Cylindric Direction", annotation="Phi - Cylindric Direction")
        self.addControl("aiRaddec4c07", label="B - Cylindric Bending", annotation="B - Cylindric Bending")
        self.addControl("aiAna4c00", label="Cx02 - Degree 2", annotation="Cx02 - Degree 2")
        self.addControl("aiAna4c01", label="Cy02 - Degree 2", annotation="Cy02 - Degree 2")
        self.addControl("aiAna4c02", label="Cx22 - Degree 2", annotation="Cx22 - Degree 2")
        self.addControl("aiAna4c03", label="Cy22 - Degree 2", annotation="Cy22 - Degree 2")
        self.addControl("aiAna4c04", label="Cx04 - Degree 4", annotation="Cx04 - Degree 4")
        self.addControl("aiAna4c05", label="Cy04 - Degree 4", annotation="Cy04 - Degree 4")
        self.addControl("aiAna4c06", label="Cx24 - Degree 4", annotation="Cx24 - Degree 4")
        self.addControl("aiAna4c07", label="Cy24 - Degree 4", annotation="Cy24 - Degree 4")
        self.addControl("aiAna4c08", label="Cx44 - Degree 4", annotation="Cx44 - Degree 4")
        self.addControl("aiAna4c09", label="Cx44 - Degree 4", annotation="Cx44 - Degree 4")
        self.addControl("aiAna4c10", label="Lens Rotation", annotation="Lens Rotation")
        self.addControl("aiAna4c11", label="Squeeze-X", annotation="Squeeze-X")
        self.addControl("aiAna4c12", label="Squeeze-Y", annotation="Squeeze-Y")
        self.addControl("aiPfC3", label="Low Order", annotation="Low Order")
        self.addControl("aiPfC5", label="High Order", annotation="High Order")
        self.addControl("aiPfSqueeze", label="Anamorphic Squeeze", annotation="Anamorphic Squeeze")
        self.endLayout()
        
        
        self.beginLayout("Camera Info", collapse=False)
        
        self.beginLayout("Film Aperture", collapse=False)
        self.addControl("aiFilmbackX3DEq", label="Width (cm)", annotation="Width (cm)")
        self.addControl("aiFilmbackY3DEq", label="Height (cm)", annotation="Height (cm)")
        self.addControl("aiPixelRatio3DEq", label="Pixel Ratio", annotation="Pixel Ratio")
        self.endLayout()
        
        self.beginLayout("Center Offset", collapse=False)
        self.addControl("aiCenterX", label="X", annotation="X")
        self.addControl("aiCenterY", label="Y", annotation="Y")
        self.addControl("aiCenterX3DEq", label="X (cm)", annotation="X (cm)")
        self.addControl("aiCenterY3DEq", label="Y (cm)", annotation="Y (cm)")
        self.endLayout()
        
        self.beginLayout("Lens Center", collapse=False)
        self.addControl("aiPfXp", label="X (ratio)", annotation="X (ratio)")
        self.addControl("aiPfYp", label="Y (ratio)", annotation="Y (ratio)")
        self.endLayout()
        
        self.beginLayout("Focal", collapse=False)
        self.addControl("aiFocal3DEq", label="Focal Length (cm)", annotation="Focal Length (cm)")
        self.addControl("aiFocusDistance3DEq", label="Focus Distance (cm)", annotation="Focus Distance (cm)")
        self.endLayout()
        self.endLayout()   # End of Camera Info
        

        self.beginLayout("Depth of Field", collapse=False)
        
        self.addControl("aiUseDof", label="Enable", annotation="Enable")
        
        self.beginLayout("Focus Distance", collapse=False)
        self.addControl("aiFocusDistance", label="Distance", annotation="Distance")
        self.addControl("aiFocusPlaneIsPlane", label="Focus Plane is a Plane", annotation="Focus Plane is a Plane")
        self.endLayout()
        
        self.beginLayout("Aperture", collapse=False)
        self.addControl("aiApertureSize", label="Size", annotation="Size")
        self.addControl("aiApertureAspectRatio", label="Aspect Ratio", annotation="Aspect Ratio")
        self.addControl("aiUsePolygonalAperture", label="Polygonal Aperture", annotation="Polygonal Aperture")
        self.addControl("aiApertureBlades", label="Blades", annotation="Blades")
        self.addControl("aiApertureBladeCurvature", label="Blade Curvature", annotation="Blade Curvature")
        self.addControl("aiApertureRotation", label="Rotation", annotation="Rotation")
        self.endLayout()
                
        self.beginLayout("Bokeh Quality", collapse=False)
        self.addControl("aiBokehInvert", label="Invert", annotation="Invert")
        self.addControl("aiBokehBias", label="Bias", annotation="Bias")
        self.addControl("aiBokehGain", label="Gain", annotation="Gain")
        self.endLayout()
                
        self.beginLayout("Presets", collapse=True)
        self.endLayout()
        
        self.endLayout()   # End Depth of Field
        
        
        # self.addControl("aiFov", label="FOV", annotation="FOV")
        
        # self.addControl("position", label="Position", annotation="Position")
        # self.addControl("look_at", label="Look At", annotation="Look At")
        # self.addControl("up", label="Up", annotation="Up")
        # self.addControl("matrix", label="Matrix", annotation="Matrix")
        # self.addControl("near_clip", label="Near Clip", annotation="Near Clip")
        # self.addControl("far_clip", label="Far Clip", annotation="Far Clip")
        # self.addControl("shutter_start", label="Shutter Start", annotation="Shutter Start")
        # self.addControl("shutter_end", label="Shutter End", annotation="Shutter End")
        # self.addControl("shutter_type", label="Shutter Type", annotation="Shutter Type")
        # self.addControl("shutter_curve", label="Shutter Curve", annotation="Shutter Curve")
        # self.addControl("rolling_shutter", label="Rolling Shutter", annotation="Rolling Shutter")
        # self.addControl("rolling_shutter_duration", label="Rolling Shutter Duration", annotation="Rolling Shutter Duration")
        # self.addControl("filtermap", label="Filtermap", annotation="Filtermap")
        # self.addControl("handedness", label="Handedness", annotation="Handedness")
        # self.addControl("time_samples", label="Time Samples", annotation="Time Samples")
        # self.addControl("screen_window_min", label="Screen Window Min", annotation="Screen Window Min")
        # self.addControl("screen_window_max", label="Screen Window Max", annotation="Screen Window Max")
        # self.addControl("exposure", label="Exposure", annotation="Exposure")

        self.endNoOptimize()

        self.endLayout()     # end Obq_LensDistortion layout

        self.beginLayout("Options", collapse=True )
        self.addControl("aiUserOptions", label="User Options")
        self.endLayout()

templates.registerTranslatorUI(Obq_LensDistortionTemplate, "camera", "Obq_LensDistortion")
#templates.registerTranslatorUI(Obq_LensDistortionTemplate, "stereoRigCamera", "Obq_LensDistortion")
