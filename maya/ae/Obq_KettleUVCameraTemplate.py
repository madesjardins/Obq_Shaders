# 2015-05-15 06.03 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.templates as templates
#from mtoa.ui.ae.customShapeAttributes import CameraTemplate as CameraTemplate
 
def Obq_KettleUVCameraHelpURL():
    # Add the Obq_Shader docs URL to the Attribute Editor help menu
    ObqNodeType = 'Obq_KettleUVCamera'
    ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_KettleUVCamera.html'
    ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
    mel.eval(ObqHelpCommand)
 
class Obq_KettleUVCameraTemplate(templates.AttributeTemplate):
    def setup(self):
        self.beginLayout("Obq_KettleUVCamera", collapse=False)
        
        Obq_KettleUVCameraHelpURL()
        
        self.beginNoOptimize()
        self.endNoOptimize()

        self.endLayout()     # end Obq_KettleUVCamera layout

        self.beginLayout("Options", collapse=True )
        self.addControl("aiUserOptions", label="User Options")
        self.endLayout()

templates.registerTranslatorUI(Obq_KettleUVCameraTemplate, "camera", "Obq_KettleUVCamera")
#templates.registerTranslatorUI(Obq_KettleUVCameraTemplate, "stereoRigCamera", "Obq_KettleUVCamera")
