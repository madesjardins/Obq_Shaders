import pymel.core as pm
import maya.cmds as cmds
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def Obq_ToonSimpleCreateCompMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('Obq_ToonSimpleCompMode', attribute=attr, label="Compositing Mode", 
                               enumeratedItem=[(0, 'Add'), (1, 'Over'), (2, 'Max'), (3, 'Screen'), (22, 'Bump Difference')])    
    cmds.setUITemplate(popTemplate=True)

def Obq_ToonSimpleSetCompMode(attr):
    cmds.attrEnumOptionMenuGrp('Obq_ToonSimpleCompMode', edit=True, attribute=attr)
    
class AEObq_ToonSimpleTemplate(ShaderAETemplate):
    convertToMayaStyle = True
    
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        
        self.beginLayout( "Global", collapse=False )
        
        self.beginLayout("Ambient", collapse=False)
        self.addControl("ambient_color", label="Color")
        self.addControl("ambient_scale", label="Scale")
        self.beginNoOptimize()
        self.addControl("ambient_multAmbience", label="Multiply by scene ambience")
        self.addControl("ambient_noInternal", label="No Internal Ambient")
        self.endNoOptimize()
        self.endLayout()

        self.beginLayout("Opacity", collapse=False)
        self.addControl("opacity",  label="Color")
        self.endLayout()

        self.beginLayout("Options", collapse=False)
        self.addCustom('global_compMode', Obq_ToonSimpleCreateCompMode, Obq_ToonSimpleSetCompMode)
        self.beginNoOptimize()
        self.addControl("global_multByLightColor",  label="Multiply by Light Color")
        self.addControl("global_clamp",  label="Clamp Before Compositing")
        self.endNoOptimize()
        self.endLayout()
        
        self.endLayout() # End Global
        
        self.beginLayout( "Diffuse", collapse=False )
        self.addControl("diffuse_color",  label="Color")
        self.addControl("diffuse_scale",  label="Scale")
        self.addControl("diffuse_coverage",  label="Coverage")
        self.addControl("diffuse_softness",  label="Softness")
        self.addControl("diffuse_noInternal",  label="No Internal Diffuse")
        self.endLayout()

        self.beginLayout( "Highlight", collapse=True )
        self.addControl("highlight_color",  label="Color")
        self.addControl("highlight_scale",  label="Scale")
        self.addControl("highlight_exponent",  label="Exponent")
        self.addControl("highlight_coverage",  label="Coverage")
        self.addControl("highlight_softness",  label="Softness")
        self.addControl("highlight_noInternal",  label="No Internal Highlight")
        self.endLayout()
        

        self.beginLayout( "Rimlight", collapse=True )
        self.addControl("rimlight_color",  label="Color")
        self.addControl("rimlight_scale",  label="Scale")
        self.addControl("rimlight_coverage",  label="Coverage")
        self.addControl("rimlight_softness",  label="Softness")
        self.addControl("rimlight_noInternal",  label="No Internal rimlight")
        self.endLayout()

        self.beginLayout( "AOVs", collapse=True )
        self.addControl("ambient_fb_str",  label="Ambient")
        self.addControl("diffuse_fb_str",  label="Diffuse")
        self.addControl("highlight_fb_str",  label="Highlight")
        self.addControl("rimlight_fb_str",  label="Rimlight")
        self.addControl("contour_fb_str",  label="Contour")
        self.beginLayout( "Options", collapse=True )
        self.beginNoOptimize()
        self.addControl("putAlphaInFb",  label="Set Alpha to Over Alpha")
        self.addControl("normalizeObjectID",  label="Contour")
        self.endNoOptimize()
        self.endLayout()
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

      
        self.addExtraControls()
        self.endScrollLayout()

