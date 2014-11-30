# 2014-11-30 08.28 am

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.templates as templates
#from mtoa.ui.ae.customShapeAttributes import CameraTemplate as CameraTemplate
 
class Obq_AngularCameraTemplate(templates.AttributeTemplate):
    def setup(self):
        self.beginLayout("Obq_AngularCamera", collapse=False)
        self.beginNoOptimize()
        self.addControl("aiAngle", label="Angle", annotation="Angle")
        self.addControl("aiFlip", label="Opposite View", annotation="Opposite View")
        self.addControl("aiOverRender", label="Over Render", annotation="Over Render")
        self.endNoOptimize()
        self.endLayout()

        self.beginLayout( "Options", collapse=True )
        #self.addCommonAttributes()
        #self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")
        self.endLayout()

templates.registerTranslatorUI(Obq_AngularCameraTemplate, "camera", "Obq_AngularCamera")
#templates.registerTranslatorUI(Obq_AngularCameraTemplate, "stereoRigCamera", "Obq_AngularCamera")
