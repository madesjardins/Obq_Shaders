# 2015-05-27 08.14 pm

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.templates as templates
#from mtoa.ui.ae.customShapeAttributes import CameraTemplate as CameraTemplate


# -------------------------------------------------------------------
#  Node Help

def Obq_KettleUVStereoLensHelpURL():
	# Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_KettleUVStereoLens'
	ObqNodeHelpURL = 'http://s3aws.obliquefx.com/public/shaders/help_files/Obq_KettleUVStereoLens.html'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

# -------------------------------------------------------------------

class Obq_KettleUVStereoLensTemplate(templates.AttributeTemplate):
	def setup(self):
		self.beginLayout("Obq_KettleUVStereoLens", collapse=False)
		
		Obq_KettleUVStereoLensHelpURL()
		
		self.beginLayout("Main", collapse=False)
		
		self.beginLayout("Rendered View", collapse=False)
		self.addControl("aiViewMode",  label="View Mode")
		self.endLayout() # end Rendered View Layout
		
		self.beginLayout("Polymeshes", collapse=False)
		
		self.beginLayout("Origin", collapse=False)
		self.addControl("aiOriginPolymesh",  label="Origin Polymesh")
		self.addControl("aiUvSetOrigin",  label="UV Set")
		self.endLayout() # end Origin Layout
		
		self.beginLayout("Target", collapse=False)
		self.addControl("aiTargetPolymesh",  label="Target Polymesh")
		self.addControl("aiUvSetTarget",  label="UV Set")
		self.endLayout() # end Target Layout
		
		self.beginLayout("Status", collapse=True)
		# self.addControl("aiPolymeshesStatus",  label="Status")
		self.endLayout() # end Target Layout
		
		self.endLayout() # end Polymeshes Layout
		

		self.endLayout() # end Main Layout
		
		
		self.beginLayout("Stereo", collapse=False)
		
		self.beginLayout("Settings", collapse=False)
		self.addControl("aiStereoType",  label="Stereo Type")
		# self.addCustom("aiInteraxialMode", Obq_KettleUVStereoLensCreateInteraxialMode, Obq_KettleUVStereoLensSetInteraxialMode)
		self.addControl("aiInteraxialSeparation",  label="Interaxial Separation")
		self.addControl("aiZeroParallaxMode",  label="Zero Parallax Mode")
		self.addControl("aiZeroParallaxDistance",  label="Zero Parallax Distance")
		self.endLayout() # end Settings Layout
		
		self.beginLayout("Automatic Overscan", collapse=True)
		# self.addControl("aiFilterSize",  label="Filter Size")
		
		self.beginLayout("Target Resolution", collapse=False)
		# self.addControl("aiTargetResolutionX",  label="Width")
		# self.addControl("aiTargetResolutionY",  label="Height")
		self.endLayout() # end Target Resolution Layout
		
		self.beginLayout("Render Resolution", collapse=False)
		# self.addControl("aiRenderResolutionX",  label="Width")
		# self.addControl("aiRenderResolutionY",  label="Height")
		
		# self.addControl("aiUpdatePassResolution",  label="Automatic Update of Pass Output Resolution")
		self.endLayout() # end Render Resolution Layout
		
		self.beginLayout("Nuke Info", collapse=False)
		#self.addControl("aiNukeCropInfo",  label="Nuke Crop")
		self.endLayout() # end Nuke Info Layout
		
		self.endLayout() # end Automatic Overscan Layout
		self.endLayout() # end Stereo Layout
				
		# ---------------------------------------------------------------------        
		
		self.beginLayout("Region", collapse=False)

		self.beginLayout("Render Region", collapse=False)
		self.addControl("aiUseRenderRegion",  label="Render Region Only")
		self.addControl("aiCropToRegion",  label="Crop to Region")
		self.endLayout() # end Render Region Layout 
		
		self.beginLayout("Lower Bound UV", collapse=False)
		#self.addControl("aiRegionU0",  label="U0")
		#self.addControl("aiRegionV0",  label="V0")
		self.addControl("aiRegionU0",  label="")
		self.addControl("aiRegionV0",  label="")
		self.endLayout() # end Lower bound UV Layout 
		
		self.beginLayout("Higher Bound UV", collapse=False)
		#self.addControl("aiRegionU1",  label="U1")
		#self.addControl("aiRegionV1",  label="V1")
		self.addControl("aiRegionU1",  label="")
		self.addControl("aiRegionV1",  label="")
		self.endLayout() # end Higher bound UV Layout 
		
		self.endLayout() # end Region Layout
		
		# ---------------------------------------------------------------------
	   
		self.beginLayout("Advanced", collapse=False)
		
		self.beginLayout("Acceleration", collapse=False)
		self.addControl("aiGridSize",  label="Grid Size")
		self.endLayout() # end Acceleration Layout  

		self.beginLayout("Other", collapse=False)
		self.addControl("aiInteraxialEpsilon",  label="Interaxial Sep. Epsilon")
		self.endLayout() # end Other Layout 
		
		self.endLayout() # end Advanced Layout 
		
		# ---------------------------------------------------------------------
		
		self.beginLayout("Debug", collapse=False)
		
		self.beginLayout("Cameras", collapse=False)
		self.addControl("aiOriginCamera",  label="Origin Camera")
		self.addControl("aiTargetCamera",  label="Target Camera")
		self.endLayout() # end Cameras Layout 
		
		self.beginLayout("Overscan", collapse=False)
		self.addControl("aiTotalOverscanPixels",  label="Total Pixel Overscan")
		self.endLayout() # end Overscan Layout
		
		self.endLayout() # end Debug Layout 

		# ---------------------------------------------------------------------
		
		self.endLayout() # end Obq_KettleUVStereoLens layout

		self.beginLayout("Options", collapse=True )
		self.addControl("aiUserOptions", label="User Options")
		self.endLayout()

templates.registerTranslatorUI(Obq_KettleUVStereoLensTemplate, "camera", "Obq_KettleUVStereoLens")
#templates.registerTranslatorUI(Obq_KettleUVStereoLensTemplate, "stereoRigCamera", "Obq_KettleUVStereoLens")
