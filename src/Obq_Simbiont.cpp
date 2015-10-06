/*

Obq_Simbiont :

	a Darktree node in Softimage that reads .dsts files for the Arnold Renderer.
	DarkTree (C) 1998-2011 Darkling Simulations

*------------------------------------------------------------------------
Copyright (c) 2012-2014 Marc-Antoine Desjardins, ObliqueFX (marcantoinedesjardins@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*------------------------------------------------------------------------

*/

#include "O_Common.h"
#include "dte/dtengine.h"
#include "dte/vmat.h"

#define NUMBEROFTWEAKS 16

AI_SHADER_NODE_EXPORT_METHODS(ObqSimbiontMethods);

enum ObqSimbiontParams { p_filename,p_sampleSize,p_bumpScale,p_frameNumber,p_cameraNear,
	p_cameraFar,p_continuousRep,p_regionRep, p_evaluateRGB, p_evaluateAlpha, p_outRRR, 
	p_coordinateSystem, p_customCoordinates, p_coordinateScale,
	p_globalTweaksOnly, p_componentLibsPath,
	p_tweakEnable0, p_tweakName0, p_tweakColor0, p_tweakScalar0,
	p_tweakEnable1, p_tweakName1, p_tweakColor1, p_tweakScalar1,
	p_tweakEnable2, p_tweakName2, p_tweakColor2, p_tweakScalar2,
	p_tweakEnable3, p_tweakName3, p_tweakColor3, p_tweakScalar3,
	p_tweakEnable4, p_tweakName4, p_tweakColor4, p_tweakScalar4,
	p_tweakEnable5, p_tweakName5, p_tweakColor5, p_tweakScalar5,
	p_tweakEnable6, p_tweakName6, p_tweakColor6, p_tweakScalar6,
	p_tweakEnable7, p_tweakName7, p_tweakColor7, p_tweakScalar7,
	p_tweakEnable8, p_tweakName8, p_tweakColor8, p_tweakScalar8,
	p_tweakEnable9, p_tweakName9, p_tweakColor9, p_tweakScalar9,
	p_tweakEnable10, p_tweakName10, p_tweakColor10, p_tweakScalar10,
	p_tweakEnable11, p_tweakName11, p_tweakColor11, p_tweakScalar11,
	p_tweakEnable12, p_tweakName12, p_tweakColor12, p_tweakScalar12,
	p_tweakEnable13, p_tweakName13, p_tweakColor13, p_tweakScalar13,
	p_tweakEnable14, p_tweakName14, p_tweakColor14, p_tweakScalar14,
	p_tweakEnable15, p_tweakName15, p_tweakColor15, p_tweakScalar15
};

// p_tweakEnable0
#define TWEAKS0 16

static const char* ObqEvalRGBModeNames[] = 
{
	"Black",
    "Color",
    "Percent",
    "Elevation",
    "Diffuse level",
	"Diffuse Func",
	"Luminosity",
	"Specular Level",
	"Specular Func",
	"Specular Color",
	"Glossiness",
	"Metal Level",
	"Anisotropy",
	"Aniso Dir",
	"Reflectivity",
	"Environment",
	"Transparency",
	"Refraction",
	"Clear Coat Level",
	"Clear Coat Glossiness",
	"Clear Coat Thickness",
	"Clear Coat Smoothness",
	"Bump",
	"Alpha",
    NULL
};

static const char* ObqEvalAlphaModeNames[] = 
{
	"Solid",
	"Color",
	"Percent",
	"Elevation",
	"Diffuse level",
	"Diffuse Func",
	"Luminosity",
	"Specular Level",			
	"Specular Func",
	"Specular Color",
	"Glossiness",	
	"Metal Level",		
	"Anisotropy",
	"Aniso Dir",
	"Reflectivity",
	"Environment",
	"Transparency",
	"Refraction",
	"Clear Coat Level",
	"Clear Coat Glossiness",
	"Clear Coat Thickness",			
	"Clear Coat Smoothness",
	"Bump",
	"Alpha",
    NULL
};
enum ObqCoordinateSystem{
	COORD_OBJECT,
	COORD_WORLD,
	COORD_UV,
	COORD_CAMERA,
	COORD_RASTER,
	COORD_CUSTOM
};
static const char* ObqCoordinateSystemNames[] = 
{
	"Object Space",
	"World Space",
	"UV Space",
	"Camera Space",
	"Raster Space",
	"Custom",
    NULL
};
node_parameters
{
	AiParameterSTR("filename", "");
	AiParameterFLT("sampleSize",0.2f);
	AiParameterFLT("bumpScale",1.0f);
	AiParameterFLT("frameNumber",0.0f);
	AiParameterFLT("cameraNear",0.0f);
	AiParameterFLT("cameraFar",1000.0f);
	AiParameterFLT("continuousRep",0.0f);
	AiParameterINT("regionRep",0);
	AiParameterENUM("evaluateRGB",1,ObqEvalRGBModeNames);
	AiParameterENUM("evaluateAlpha",0,ObqEvalAlphaModeNames);
	AiParameterBOOL("outRRR",false);
	AiParameterENUM("coordinateSystem",COORD_OBJECT,ObqCoordinateSystemNames);
	AiParameterVEC("customCoordinates",0.0f,0.0f,0.0f);
	AiParameterVEC("coordinateScale",1.0f,1.0f,1.0f);
	AiParameterBOOL("globalTweaksOnly",true);
	AiParameterSTR("componentLibsPath","");

	// Tweak parameters
	for(unsigned int i = 0; i < NUMBEROFTWEAKS; i++)
	{
		char paramEnable[20];
		sprintf_s(paramEnable,"tweakEnable%u",i);
		char paramName[20];
		sprintf_s(paramName,"tweakName%u",i);
		char paramColor[20];
		sprintf_s(paramColor,"tweakColor%u",i);
		char paramScalar[20];
		sprintf_s(paramScalar,"tweakScalar%u",i);

		AiParameterBOOL(paramEnable,false);  AiParameterSTR(paramName,"");	AiParameterRGB(paramColor,0.0f,0.0f,0.0f);  AiParameterFLT(paramScalar,0.0f);
	}
}

//---------------------------
// Shader Data Structure
//---------------------------
typedef struct 
{
	void*  dte;									// the dark tree texture instance
	bool fileLoaded;						// file loaded successfully ?
	bool tweakEnabled[NUMBEROFTWEAKS];		// tweak enable array
	bool tweakValid[NUMBEROFTWEAKS];		// valid tweak array
	unsigned int tweakTags[NUMBEROFTWEAKS];		// tags for tweaks array
	unsigned int tweakTypes[NUMBEROFTWEAKS];	// types of tweaks array
	unsigned int dynTweakNum;					// number of valid and active tweaks to build evaluated array
	DSdte::DynamicTweakValue_struct *dynTweak;	// poi nter to array of dynamic tweaks structures to fill with tweak values
	AtCritSec	critical_section;				// Critical section
	int evalRGB;
	int evalAlpha;
	int coordinateSystem;
}
ShaderData;


// callback function for dte messaging
//
// @ param instance : the dte
// @ param severity : 0=fatal, 1=error, 2=warning, 3=information
// @ param source	: name of the source sending the message
// @ param message	: the message
//------------------------------------
void logMessage(void *instance, int severity, const char* const source, const char* const message){

	switch(severity){
	case 0:
		AiMsgFatal("[%s] : %s",source,message);
		break;
	case 1:
		AiMsgError("[%s] : %s",source,message);
		break;
	case 2:
		AiMsgWarning("[%s] : %s",source,message);
		break;
	case 3:
		AiMsgInfo("[%s] : %s",source,message);
		break;
	default:
		AiMsgDebug("[%s] : %s",source,message);
		break;
	}
}


node_initialize
{
	// Allocation and initialization of shader data
   ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
   AiNodeSetLocalData(node,data);
   data->dynTweakNum = 0;

	// Dtengine initialization
	DSdte::Initialize();

	// Initialize the Component paths.
	DSdte::ComponentLibsAdd(AiNodeGetStr(node, "componentLibsPath"));
	DSdte::ComponentLibsRebuild();

	// Set message handler
	DSdte::SetMessageHandler(logMessage);

	// create a new instance (maximum of 16 threads can have access to instance)
	data->dte = DSdte::Create(16,16);

	// initialisation of the critical section (not used)
	AiCritSecInit(&data->critical_section);

	data->fileLoaded = false;
	
}

node_update
{
	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	AiCritSecEnter(&data->critical_section);

	// get constant value
	data->evalRGB = params[p_evaluateRGB].INT - 1;		// -1 to match DTE Constants
	data->evalAlpha = params[p_evaluateAlpha].INT - 1;	// -1 to match DTE Constants
	data->coordinateSystem = params[p_coordinateSystem].INT;

	//load the file
	data->fileLoaded = DSdte::Load(data->dte,AiNodeGetStr(node, "filename"));
	///////////
	// TWEAKS
	///////////

	if(data->fileLoaded){

		// Get number of tweaks in the file
		unsigned int tweakNum = DSdte::Tweak_Num(data->dte);

		AiMsgInfo("[DSdte] : ------------------------------- Tweaks -------------------------------------------");
		// For all of them, print the name and the type
		std::string tweakName;
		for(unsigned int i = 0; i< tweakNum; i++)
		{
			// Get Name
			if(i==0)
				tweakName = std::string(DSdte::Tweak_First(data->dte));
			else
				tweakName = std::string(DSdte::Tweak_Next(data->dte,tweakName.c_str()));

			// Get Type
			int tweakType = DSdte::Tweak_Type(data->dte,tweakName.c_str());

			// Print Tweaks
			std::string tweakTypeStr;
			switch(tweakType)
			{
			case DSdte::DTE_TWEAK_FLOAT:
				tweakTypeStr = std::string("FLOAT : -> scalar input.");
				break;
			case DSdte::DTE_TWEAK_INT :
				tweakTypeStr = std::string("INT : -> rounded scalar input.");
				break;
			case DSdte::DTE_TWEAK_BOOL :
				tweakTypeStr = std::string("BOOL : -> scalar input > 0 = TRUE.");
				break;
			case DSdte::DTE_TWEAK_COLOR :
				tweakTypeStr = std::string("COLOR : -> color input.");
				break;
			case DSdte::DTE_TWEAK_PERCENT :
				tweakTypeStr = std::string("PERCENT : -> scalar input.");
				break;
			case DSdte::DTE_TWEAK_BUMP :
				tweakTypeStr = std::string("BUMP : -> scalar input.");
				break;
			case DSdte::DTE_TWEAK_MAPPING:
				tweakTypeStr = std::string("MAPPING : -> rounded scalar input.");
				break;
			case DSdte::DTE_TWEAK_SAMPLING:
				tweakTypeStr = std::string("SAMPLING : -> rounded scalar input.");
				break;
			default:
				tweakTypeStr = std::string("UNKNOWN : -> will not be tweak.");
				break;
			}

			AiMsgInfo("[DSdte] : Tweak # %i   , Name : %s    , Type : %s",i,tweakName.c_str(),tweakTypeStr.c_str());
		}
		AiMsgInfo("[DSdte] : ------------------------------- Tweaks -------------------------------------------");

		// Free memory for tweaks
		if(data->dynTweakNum > 0 ){
			for(unsigned int i = 0; i < data->dynTweakNum; i++)
				AiFree(data->dynTweak[i].pValue);
			AiFree(data->dynTweak);
		}
		data->dynTweakNum = 0;
		data->dynTweak = NULL;

		//-------------
		// User tweaks
		for(unsigned int i = 0; i < NUMBEROFTWEAKS ; i++){

			char paramEnable[20];
			sprintf_s(paramEnable,"tweakEnable%u",i);

			// initialize
			data->tweakEnabled[i] = false;

			// If enable in ui
			if(AiNodeGetBool(node,paramEnable)){

				// name of tweak in spdl
				char paramName[20];
				sprintf_s(paramName,"tweakName%u",i);
				std::string tweakName(AiNodeGetStr(node,paramName));
				
				// Check if tweakname is valid
				data->tweakValid[i] = DSdte::Tweak_Exists(data->dte, tweakName.c_str());

				// If valid tweak
				if(data->tweakValid[i]){

					// Get type
					data->tweakTypes[i] = DSdte::Tweak_Type(data->dte,  tweakName.c_str());

					// If it's a known type, enable the tweak
					if(data->tweakTypes[i] != DSdte::DTE_TWEAK_UNKNOWN){
						data->tweakEnabled[i] = true;
						data->tweakTags[i] = DSdte::Tweak_AccessTag(data->dte, tweakName.c_str());
						data->dynTweakNum++;
						AiMsgInfo("[DSdte] : Tweak '%s' is active.",tweakName.c_str());

					}
					else
					AiMsgWarning("[DSdte] : Tweak '%s' has invalid type and will be deactivated.",tweakName.c_str());
				}
				else
					AiMsgWarning("[DSdte] : Tweak '%s' is not a valid tweak name.",tweakName.c_str());
			}
		}

		// Allocate memory for tweak and initialize
		if(data->dynTweakNum > 0)
		{
			// Allocate struct
			data->dynTweak = (DSdte::DynamicTweakValue_struct *) AiMalloc(data->dynTweakNum*sizeof(DSdte::DynamicTweakValue_struct));

			// Allocate values depending on types
			unsigned int dynTweakIndex = 0;
			for(unsigned int i = 0; i < NUMBEROFTWEAKS ; i++)
			{
				if(data->tweakEnabled[i])
				{
					float* f;
					AtRGB c;
					int* j;
					bool *b;
					DSdte::Vec3_struct* v3;
					char scalar[20];
					sprintf_s(scalar,"tweakScalar%u",i);
					char color[20];
					sprintf_s(color,"tweakColor%u",i);

					// Set the tag
					data->dynTweak[dynTweakIndex].Tag = data->tweakTags[i];

					// Allocate and set value
					switch(data->tweakTypes[i])
					{
					case DSdte::DTE_TWEAK_FLOAT:;
					case DSdte::DTE_TWEAK_PERCENT :;
					case DSdte::DTE_TWEAK_BUMP :
						f = (float*) AiMalloc( sizeof(float) );
						*f = AiNodeGetFlt(node,scalar);
						data->dynTweak[dynTweakIndex].pValue = f;
						break;
					case DSdte::DTE_TWEAK_INT :;
					case DSdte::DTE_TWEAK_MAPPING :;
					case DSdte::DTE_TWEAK_SAMPLING :
						j = (int*) AiMalloc( sizeof(int) );
						*j = int((AiNodeGetFlt(node,scalar)+0.5f));
						data->dynTweak[dynTweakIndex].pValue = j;
						break;
					case DSdte::DTE_TWEAK_BOOL :
						b = (bool*) AiMalloc( sizeof(bool) );
						*b = AiNodeGetFlt(node,scalar)>0.0f;
						data->dynTweak[dynTweakIndex].pValue = b;
						break;
					case DSdte::DTE_TWEAK_COLOR :
						c = AiNodeGetRGB(node,color);
						v3 = (DSdte::Vec3_struct*) AiMalloc (sizeof(DSdte::Vec3_struct));
						v3->n[0] = c.r;
						v3->n[1] = c.g;
						v3->n[2] = c.b;
						data->dynTweak[dynTweakIndex].pValue = v3;
						break;
					default:
						AiMsgError("[DSdte] : Unknown tweak type !!");
						break;
					}
					// Increment the index of the used tweaks
					dynTweakIndex++;
				}
			}
		}
	}
	
	AiCritSecLeave(&data->critical_section);
}


node_finish
{
	
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	// Destroy the data and free memory
	if(data->dynTweakNum > 0 ){
		for(unsigned int i = 0; i < data->dynTweakNum; i++)
			AiFree(data->dynTweak[i].pValue);
		AiFree(data->dynTweak);
	}
	data->dynTweakNum = 0;
	data->dynTweak = NULL;
	
	// Destroy the texture instance
	DSdte::Destroy(data->dte);

	// Close the critical section
	AiCritSecClose(&data->critical_section);

	// Desallocate shader data
	AiFree(data);

}


shader_evaluate
{
	// Get the data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	// Check if a valid file was loaded
	if(data->fileLoaded)
	{
		// Prepare the render state struct for evaluation
		// See the DTEngine reference for more info
		DSdte::RenderState_struct rs;
		AtVector v;
		switch(data->coordinateSystem)
		{
		case COORD_OBJECT : 
			rs.point = DSdte::Vec3ToStruct(DSmath::Vec3(sg->Po.x, sg->Po.y, sg->Po.z));
			break;
		case COORD_WORLD:
			rs.point = DSdte::Vec3ToStruct(DSmath::Vec3(sg->P.x, sg->P.y, sg->P.z));
			break;
		case COORD_UV :
			rs.point = DSdte::Vec3ToStruct(DSmath::Vec3( sg->u  , sg->v, 0.0f));
			break;
		case COORD_CAMERA :
			rs.point = DSdte::Vec3ToStruct(DSmath::Vec3( sg->sx  , sg->sy, 0.0f));
			break;
		case COORD_RASTER :
			rs.point = DSdte::Vec3ToStruct(DSmath::Vec3( float(sg->x)  , float(sg->y), 0.0f));
			break;
		case COORD_CUSTOM :
			v = AiShaderEvalParamVec(p_customCoordinates);
			rs.point = DSdte::Vec3ToStruct(DSmath::Vec3( v.x , v.y, v.z));
			break;
		default:
			rs.point = DSdte::Vec3ToStruct(DSmath::Vec3(sg->Po.x, sg->Po.y, sg->Po.z));
			break;
		}

		// Scale
		v = AiShaderEvalParamVec(p_coordinateScale);
		rs.point.n[0]*=v.x;
		rs.point.n[1]*=v.y;
		rs.point.n[2]*=v.z;

		rs.normal = DSdte::Vec3ToStruct(DSmath::Vec3(sg->Nf.x, sg->Nf.y, sg->Nf.z));
		rs.sampleSize = AiShaderEvalParamFlt(p_sampleSize);
		rs.bumpScale = AiShaderEvalParamFlt(p_bumpScale);
		rs.frameNumber = AiShaderEvalParamFlt(p_frameNumber);
		rs.incident = DSdte::Vec3ToStruct(DSmath::Vec3(sg->Rd.x, sg->Rd.y, sg->Rd.z));
		float camNear = AiShaderEvalParamFlt(p_cameraNear), camFar =  AiShaderEvalParamFlt(p_cameraFar);
		// No division by inf
		if(camFar <= camNear) camFar = camNear+1.0f;
		rs.cameraDistanceP = (float(sg->Rl)-camNear)/(camFar-camNear);
		rs.continuousRep = AiShaderEvalParamFlt(p_continuousRep);
		rs.regionRep = AiShaderEvalParamInt(p_regionRep);;

		// What do we have to evaluate
		//int evalRGB = AiShaderEvalParamInt(p_evaluateRGB);
		//int evalAlpha = AiShaderEvalParamInt(p_evaluateAlpha);

		/////////
		// TWEAKS
		/////////
		if(data->dynTweak > 0 && !AiShaderEvalParamBool(p_globalTweaksOnly))
		{

			// Allocate struct
			DSdte::DynamicTweakValue_struct *dynTweak = (DSdte::DynamicTweakValue_struct *) AiMalloc(data->dynTweakNum*sizeof(DSdte::DynamicTweakValue_struct));

			// Allocate values depending on types
			unsigned int dynTweakIndex = 0;
			for(unsigned int i = 0; i < NUMBEROFTWEAKS ; i++)
			{
				if(data->tweakEnabled[i])
				{
					float* f;
					AtRGB c;
					int* j;
					bool *b;
					DSdte::Vec3_struct* v3;

					// Set the tag
					dynTweak[dynTweakIndex].Tag = data->tweakTags[i];

					// Allocate and set value
					switch(data->tweakTypes[i])
					{
					case DSdte::DTE_TWEAK_FLOAT:;
					case DSdte::DTE_TWEAK_PERCENT :;
					case DSdte::DTE_TWEAK_BUMP :
						f = (float*) AiMalloc( sizeof(float) );
						*f = AiShaderEvalParamFlt(int(p_tweakEnable0) + i*4 + 3);
						dynTweak[dynTweakIndex].pValue = f;
						break;
					case DSdte::DTE_TWEAK_INT :;
					case DSdte::DTE_TWEAK_MAPPING :;
					case DSdte::DTE_TWEAK_SAMPLING :
						j = (int*) AiMalloc( sizeof(int) );
						*j = int(AiShaderEvalParamFlt(int(p_tweakEnable0) + i*4 + 3)+0.5f);
						dynTweak[dynTweakIndex].pValue = j;
						break;
					case DSdte::DTE_TWEAK_BOOL :
						b = (bool*) AiMalloc( sizeof(bool) );
						*b = AiShaderEvalParamFlt(int(p_tweakEnable0) + i*4 + 3)>0.0f;
						dynTweak[dynTweakIndex].pValue = b;
						break;
					case DSdte::DTE_TWEAK_COLOR :
						c = AiShaderEvalParamRGB(int(p_tweakEnable0) + i*4 + 2);
						v3 = (DSdte::Vec3_struct*) AiMalloc (sizeof(DSdte::Vec3_struct));
						v3->n[0] = c.r;
						v3->n[1] = c.g;
						v3->n[2] = c.b;
						dynTweak[dynTweakIndex].pValue = v3;
						break;
					default:
						AiMsgError("[DSdte] : Unknown tweak type !!");
						break;
					}
					// Increment the index of the used tweaks
					dynTweakIndex++;
				}
			}
			
			// RGB evaluation
			if(data->evalRGB >= 0)
			{
				rs.evaluateAs = (DSdte::DTE_EVALUATE) data->evalRGB;
				DSdte::Vec3_struct retVal = DSdte::Evaluate(data->dte, &rs,dynTweak,data->dynTweakNum);
				sg->out.RGBA.r = retVal.n[0];
				sg->out.RGBA.g = retVal.n[1];
				sg->out.RGBA.b = retVal.n[2];
			} 
			else 
				sg->out.RGBA = AI_RGBA_BLACK;

			// Alpha evaluation
			if(data->evalAlpha >= 0)
			{
				rs.evaluateAs = (DSdte::DTE_EVALUATE) data->evalAlpha;
				DSdte::Vec3_struct retVal = DSdte::Evaluate(data->dte, &rs,dynTweak, data->dynTweakNum);
				sg->out.RGBA.a = retVal.n[0];
			} else 
				sg->out.RGBA.a = 1.0f;
			

			// Free tweak memory
			if(data->dynTweakNum > 0 ){
				for(unsigned int i = 0; i < data->dynTweakNum; i++)
					AiFree(dynTweak[i].pValue);
				AiFree(dynTweak);
			}
			

		}
		else // No tweaks or global
		{

			// RGB evaluation
			if(data->evalRGB >= 0)
			{
				rs.evaluateAs = (DSdte::DTE_EVALUATE) data->evalRGB;
				DSdte::Vec3_struct retVal = DSdte::Evaluate(data->dte, &rs,data->dynTweak,data->dynTweakNum);
				sg->out.RGBA.r = retVal.n[0];
				sg->out.RGBA.g = retVal.n[1];
				sg->out.RGBA.b = retVal.n[2];
			} 
			else 
				sg->out.RGBA = AI_RGBA_BLACK;

			// Alpha evaluation
			if(data->evalAlpha >= 0)
			{
				rs.evaluateAs = (DSdte::DTE_EVALUATE) data->evalAlpha;
				DSdte::Vec3_struct retVal = DSdte::Evaluate(data->dte, &rs,data->dynTweak,data->dynTweakNum);
				sg->out.RGBA.a = retVal.n[0];
			} else 
				sg->out.RGBA.a = 1.0f;
		}

		// Put Red channel in all the others (grayscale)
		if(AiShaderEvalParamBool(p_outRRR)){
			sg->out.RGBA.g = sg->out.RGBA.r ;
			sg->out.RGBA.b = sg->out.RGBA.r ;
		}

	}
	else
		sg->out.RGBA = AI_RGBA_BLACK;

	
}

node_loader
{
   if (i > 0)
      return false;

   node->methods      = ObqSimbiontMethods;
   node->output_type  = AI_TYPE_RGBA;
   node->name         = "Obq_Simbiont";
   node->node_type    = AI_NODE_SHADER;
#ifdef _WIN32
   strcpy_s(node->version, AI_VERSION);
#else
   strcpy(node->version, AI_VERSION);
#endif
   return true;
}
