# 2015-04-25 08.13 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.templates as templates
#from mtoa.ui.ae.customShapeAttributes import CameraTemplate as CameraTemplate
 
def Obq_AngularCameraHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_AngularCamera'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_AngularCamera.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)
 
class Obq_AngularCameraTemplate(templates.AttributeTemplate):
    def setup(self):
        self.beginLayout("Obq_AngularCamera", collapse=False)
        
        Obq_AngularCameraHelpURL()
        
        self.beginNoOptimize()
        self.addControl("aiAngle", label="Angle", annotation="Angle")
        self.addControl("aiFlip", label="Opposite View", annotation="Opposite View")
        self.addControl("aiOverRender", label="Over Render", annotation="Over Render")
        self.endNoOptimize()

        self.endLayout()     # end Obq_AngularCamera layout

        self.beginLayout("Options", collapse=True )
        self.addControl("aiUserOptions", label="User Options")
        self.endLayout()

templates.registerTranslatorUI(Obq_AngularCameraTemplate, "camera", "Obq_AngularCamera")
#templates.registerTranslatorUI(Obq_AngularCameraTemplate, "stereoRigCamera", "Obq_AngularCamera")
