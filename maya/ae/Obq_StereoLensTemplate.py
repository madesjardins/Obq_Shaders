# 2014-09-10 05.57 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.customShapeAttributes import CameraTemplate as CameraTemplate

ViewModeEnumOp = [
    (0, 'Center Camera'),
    (1, 'Left Camera'),
    (2, 'Right Camera'),
    (3, 'Stereo Camera <left-right>'),
    (4, 'Stereo Camera <down-up>')
]

def Obq_StereoLensCreateViewMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_StereoLensViewMode', attribute=attr, label="Camera", enumeratedItem=ViewModeEnumOp)    
    cmds.setUITemplate(popTemplate=True)

def Obq_StereoLensSetViewMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_StereoLensViewMode', edit=True, attribute=attr)

def Obq_StereoLensHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_StereoLens'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_StereoLens.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)

class Obq_StereoLensTemplate(CameraTemplate):
    def setup(self):
        self.beginLayout("Obq_StereoLens", collapse=False)

        Obq_StereoLensHelpURL()

        self.beginLayout("View Mode", collapse=False)
        self.addCustom('aiViewMode', Obq_StereoLensCreateViewMode, Obq_StereoLensSetViewMode)
        self.endLayout()

        self.beginLayout("Cameras", collapse=False)
        self.addControl("aiLeftCamera", label="Left")
        self.addControl("aiRightCamera", label="Right")
        self.addControl("aiCameraStatus", label="Status")
        self.endLayout()

        self.beginLayout("Camera Interest", collapse=False)
        self.addControl("aiCameraInterest", label="Camera Interest")
        #self.addControl("aiInterest", label="Interest")
        self.addControl("aiInterestX", label="Interest X")
        self.addControl("aiInterestY", label="Interest Y")
        self.addControl("aiInterestZ", label="Interest Z")
        self.addControl("aiCameraInterestStatus", label="Camera Interest Status")
        self.endLayout()

        self.beginLayout("DoF Advanced Options", collapse=False)
        self.addControl("aiRecalculateDistanceForSideCameras", label="Recalculate focus distance for left and right cameras")
        self.addControl("aiFocalPlaneIsPlane", label="Focal plane is a plane")
        self.addControl("aiUseRigDof", label="Use a custom/rig focus distance")
        self.addControl("aiRigDepth", label="Focus distance")
        self.endLayout()

        self.beginLayout("Automatic Overscan", collapse=False)
        self.addControl("aiUseOverscan", label="Enable")
        self.endLayout()

        self.beginLayout("Target Resolution", collapse=False)
        self.addControl("aiTargetResolutionX", label="Width")
        self.addControl("aiTargetResolutionY", label="Height")
        self.endLayout()

        self.beginLayout("Render Resolution", collapse=False)
        self.addControl("aiRenderResolutionX", label="Width")
        self.addControl("aiRenderResolutionY", label="Height")
        self.addControl("aiUpdatePassResolution", label="Automatic update of pass output resolution")
        self.endLayout()

        self.beginLayout("Nuke Info", collapse=False)
        self.addControl("aiLeftCropInfo", label="Left crop")
        self.addControl("aiRghtCropInfo", label="Right crop")
        self.endLayout()

        self.beginLayout("Debug", collapse=False)
        self.addControl("aiDof", label="Enable DoF")
        self.addControl("aiUseCameraInterest", label="Use camera interest as focus point")
        self.addControl("aiFilterSize", label="Filter size")
        self.addControl("aiFilmbackX", label="Filmback X")
        self.addControl("aiLeftCenterOffset", label="Left Optical Center Offset")
        self.addControl("aiRightCenterOffset", label="Right Optical Center Offset")
        self.addControl("aiTotalOverscanPixels", label="Total Pixel Overscan")
        self.endLayout()

        self.endLayout()     # end Obq_StereoLens layout

        self.beginLayout("Options", collapse=True)
        self.addCommonAttributes()
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")
        self.endLayout()

templates.registerTranslatorUI(Obq_StereoLensTemplate, "camera", "Obq_StereoLens")
templates.registerTranslatorUI(Obq_StereoLensTemplate, "stereoRigCamera", "Obq_StereoLens")
