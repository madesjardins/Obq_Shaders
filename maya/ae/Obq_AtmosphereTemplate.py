# 2016-12-18 5.46 AM

import pymel.core as pm
import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_AtmosphereHelpURL():
	#Add the Obq_Shader docs URL to the Attribute Editor help menu
	ObqNodeType = 'Obq_Atmosphere'
	ObqNodeHelpURL = 'https://github.com/madesjardins/Obq_Shaders/wiki/Obq_Atmosphere'
	ObqHelpCommand = 'addAttributeEditorNodeHelp("' + ObqNodeType + '", "showHelp -absolute \\"' +ObqNodeHelpURL +'\\"");'
	mel.eval(ObqHelpCommand)

class AEObq_AtmosphereTemplate(ShaderAETemplate):
	convertToMayaStyle = True
	
	def setup(self):
		self.addSwatch()

		self.beginScrollLayout()

		self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

		#pm.picture(image='Obq_shader_header.png', parent="AttrEdObq_AtmosphereFormLayout")
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
		self.addControl("radiusFalloffMode",  label="Mode")##self.addCustom('radiusFalloffMode', Obq_AtmosphereCreateRadiusFalloffMode, Obq_AtmosphereSetRadiusFalloffMode)
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
		self.addControl("compensateForCloudOpacity",  label="Compensate")#self.addCustom('compensateForCloudOpacity', Obq_AtmosphereCreateCompensateForCloudOpacity, Obq_AtmosphereSetCompensateForCloudOpacity)
		self.endLayout()
		self.beginLayout("Color/Opacity compensation", collapse=False)
		self.addControl("divideColorByOpacity",  label="Divide")#self.addCustom('divideColorByOpacity', Obq_AtmosphereCreateDivideColorByOpacity, Obq_AtmosphereSetDivideColorByOpacity)Divide
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

