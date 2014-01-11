/*
Obq_Thickness :

	is a shader that tells information concerning the thickness of a surface in a certain direction
	or the number of objects in this direction

*------------------------------------------------------------------------
Copyright (c) 2012-2014 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)

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
#include "Obq_Thickness.h"


// parameters
//
node_parameters
{
	AiParameterINT("sampleLevel", 1);						// number of ray shot ^ 2
	AiParameterFLT("coneAngle", 25.0f);					// cone angle
	AiParameterBOOL ("useVdotNForMaxAngle",false);		// limit max angle with geometric normal
	AiParameterFLT("cosLobeGloss", 0.0f );				// cos^n
	AiParameterBOOL ( "useSampleCount",false);
	AiParameterFLT ( "sampleCount",10.0f);
	AiParameterFLT ( "sampleCountMultiplier",1.0f);
	AiParameterINT("backface",1);						// backface options
	AiParameterBOOL("intersectOthers",true);			// thickness of object considering other objects inside
	AiParameterINT("maxRayDepth", 10);					// maximum ray depth
	AiParameterINT("normalizeMode",2);					// normalize with max ray lenght [0,1]
	AiParameterFLT("maxRayLength", 10.0f);				// maximum lenght of a thickness ray
	AiParameterBOOL("usemaxRayLength",false);			// use maximum ray lenght (false = AI_BIG)
	AiParameterINT("dirMode",0);						// direction of thickness mode
	AiParameterBOOL("dirInverted",false);				// invert direction
	AiParameterBOOL("dirSmooth",false);					// use smooth normals
	AiParameterBOOL("dirGeometric",false);				// use geometric normal
	AiParameterBOOL("dirSafe",true);					// use safe reflections
	AiParameterVEC("customDirection",0.0f,1.0f,0.0f);	// a custom vector for direction purposes
	AiParameterFLT("ior", 1.33f);						// index of refraction for refracting ray
	AiParameterINT("noHitMode",1);						// If no hit occurs, is it considered thick or thin ? 0 = thin, 1 = thick
	AiParameterBOOL("multNdotR",false);					// multiply increment by n dot v ( good for spheres )
	AiParameterFLT("NdotRExp",4.0f);					// exponent for NdotR
	AiParameterFLT("enterSurface",0.0f);
	AiParameterFLT("geometricLimitMultiplier",1.0f);
	AiParameterINT("thicknessMode",0);
	AiParameterBOOL("multiplyByDepth",false);
}



node_initialize
{
	// initialisation of sampler
   ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
   data->sampler = NULL;
   AiNodeSetLocalData(node,data);

}

node_update
{

	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	// destroy the sampler
	AiSamplerDestroy(data->sampler);

	// get sampleLevel
	int sampleLevel = AiNodeGetInt(node, "sampleLevel");

	// Initializing AiSampler for multi-ray
	if(sampleLevel > 1)
		data->sampler = AiSampler(sampleLevel, 2);
	else
		data->sampler = NULL;

}


node_finish
{
	// destroy sampler data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	AiSamplerDestroy(data->sampler);

	AiFree(data);
}



shader_evaluate
{

	// Shading Normals
	AtVector fnormal = sg->Nf, normal = sg->N;
	float scalarout = 0.0f;
	int backface = AiShaderEvalParamBool(p_backface);

	// back-facing normal of grazing angle ? Trivial reject
	// front facing or treat as equal
	if((backface == 3 || fnormal == normal))
	{
		// Parameters
		int sampleLevel = AiShaderEvalParamInt(p_sampleLevel);
		float coneAngle = AiShaderEvalParamFlt(p_coneAngle)/2.0f;
		float gloss = AiShaderEvalParamFlt(p_cosLobeGloss);
		bool intersectOthers = AiShaderEvalParamBool(p_intersectOthers);
		int maxRayDepth = AiShaderEvalParamInt(p_maxRayDepth);
		bool useMaxRayLength = AiShaderEvalParamBool(p_useMaxRayLength);
		float maxRayLength = AiShaderEvalParamFlt(p_maxRayLength);
		float noHitValue = ((AiShaderEvalParamInt(p_noHitMode) == 0)?0.0f:float(AI_INFINITE));
		bool multNdotR = AiShaderEvalParamBool(p_multNdotR);
		float NdotRExp = AiShaderEvalParamFlt(p_NdotRExp);
		int thicknessMode = AiShaderEvalParamBool(p_thicknessMode);
		bool numOfHitMode = (thicknessMode == 1);
		bool volumeMode = (thicknessMode == 2);
		bool multiplyByDepth = AiShaderEvalParamBool(p_multiplyByDepth);
		float enterSurface = AiShaderEvalParamFlt(p_enterSurface);

		// Max Distance to use for ray tracing purpose
		double maxDistance2use = AI_BIG;
		if(useMaxRayLength)
		   maxDistance2use = maxRayLength;

		int normalizeMode = AiShaderEvalParamInt(p_normalizeMode);
		int dirMode = AiShaderEvalParamInt(p_dirMode);

		bool dirInverted = AiShaderEvalParamBool(p_dirInverted);
		bool dirSmooth = AiShaderEvalParamBool(p_dirSmooth);
		bool dirGeometric = AiShaderEvalParamBool(p_dirGeometric);
		bool dirSafe = AiShaderEvalParamBool(p_dirSafe);

		// Type of direction
		AtVector rayDir[1];
		//enum { D_VIEW, D_NOR, D_NOR_FF, D_RFL, D_RFR, D_CUSTOM };
		switch(dirMode){
		case D_VIEW: // "Surface Normal Direction"
			rayDir[0] = sg->Rd;
			break;
		case D_NOR: // "Surface Normal Direction"
			if(dirGeometric)
				rayDir[0] = sg->Ng;
			else if(dirSmooth)
				rayDir[0] = sg->Ns;
			else
				rayDir[0] = sg->N;
			break;
		case D_NOR_FF:  // "Normal FaceForward Direction"
			if(dirGeometric)
				rayDir[0] = sg->Ngf;
			else if(dirSmooth)
			{
				if(AiV3Dot(sg->Ns,sg->Rd) < 0.0f) // face forward
					rayDir[0] = sg->Ns;
				else
					rayDir[0] = -sg->Ns;
			}
			else
				rayDir[0] = sg->Nf;
			break;
		case D_RFL:  // "Reflection Direction"
			if(dirGeometric)
					AiReflect(&sg->Rd, &sg->Ngf, rayDir);
			else if(dirSmooth)
			{
				if(dirSafe)
					AiReflectSafe(&sg->Rd, &sg->Ns, &sg->Ng, rayDir);
				else
					AiReflect(&sg->Rd, &sg->Ns, rayDir);
			}
			else
			{
				if(dirSafe)
					AiReflectSafe(&sg->Rd, &sg->Nf, &sg->Ngf, rayDir);
				else
					AiReflect(&sg->Rd, &sg->Nf, rayDir);
			}
			break;
		case D_RFR: // "Refraction Direction"
			if(dirGeometric)
			{
				if(!AiRefract(&sg->Rd, &sg->Ngf, rayDir, 1.0f, AiShaderEvalParamFlt(p_ior)))
					AiReflect(&sg->Rd, &sg->Ngf, rayDir);
			}
			else if(dirSmooth)
			{
				if(!AiRefract(&sg->Rd, &sg->Ns, rayDir, 1.0f, AiShaderEvalParamFlt(p_ior)))
				{
					if(dirSafe)
						AiReflectSafe(&sg->Rd, &sg->Ns, &sg->Ng, rayDir);
					else
						AiReflect(&sg->Rd, &sg->Ns, rayDir);
				}
			}
			else
			{
				if(!AiRefract(&sg->Rd, &sg->Nf, rayDir, 1.0f, AiShaderEvalParamFlt(p_ior)))
				{
					if(dirSafe)
						AiReflectSafe(&sg->Rd, &sg->Nf, &sg->Ngf, rayDir);
					else
						AiReflect(&sg->Rd, &sg->Nf, rayDir);
				}
			}
			break;
		case D_CUSTOM: // "Custom Direction"
			rayDir[0] =  AiShaderEvalParamVec(p_customDirection);
			break;
		default:
			rayDir[0] = sg->Rd;
		}

		// Invert ?
		if(dirInverted)
			rayDir[0] = -rayDir[0];


		// use sample count
		bool useSampleCount =  AiShaderEvalParamBool(p_useSampleCount);
		int sampleCount = int(std::floor(0.5f + AiShaderEvalParamFlt(p_sampleCount)*AiShaderEvalParamFlt(p_sampleCountMultiplier)));

		// Simple_case one ray
		if(sampleLevel == 1 || coneAngle == 0.0f || gloss == 1.0f || ( useSampleCount && sampleCount <=1 ))
		{
			// We create a probe and send it
			AtPoint p[1] = {sg->P - enterSurface*AI_EPSILON*sg->Ngf};

			scalarout = depthRayTrace(sg,p,rayDir, intersectOthers, maxRayDepth, maxDistance2use,noHitValue, numOfHitMode, multNdotR, NdotRExp, volumeMode,multiplyByDepth);
		}
		// MultiRay case
		else
		{
			
			// For Sampler & environment checking
			ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
		
#if OBQ_AI_VERSION >= 40100
			float rndTmp[2];
#else
			double rndTmp[2];
#endif
			float rnd[2];
			AtSamplerIterator* iter;

			// direction
			AtVector V = rayDir[0];
			
			// exponent
			float cosLobeExponent = GlossToExponent(gloss);

			// Max angle deviation
			float maxAngle = float(coneAngle*AI_DTOR);

			// use normal as max
			if( AiShaderEvalParamBool(p_useVdotNForMaxAngle) )
			{
				float NangleV = AiShaderEvalParamFlt(p_geometricLimitMultiplier) * std::acos(1.0f-std::max(0.0f,std::min(std::abs(AiV3Dot(V,sg->Ng)),1.0f))); // maximum angle based on normal
				
			
				maxAngle = std::min(NangleV,maxAngle);
			}

			AtVector Vperp = {(V.z - V.y), (V.x - V.z), (V.y - V.x)}; // a perpendicular vector
			Vperp = AiV3Normalize(Vperp);
			AtVector KcrossV = AiV3Cross(V, Vperp);

			// Get an iterator to loop over the samples
			iter = AiSamplerIterator(data->sampler, sg);

			if(sampleCount < 1)
				sampleCount = 1;
			else if(sampleCount > sampleLevel*sampleLevel)
				sampleCount = sampleLevel*sampleLevel;

			int iSample = 0, nSamples = useSampleCount?sampleCount:sampleLevel*sampleLevel;

			while(iSample < nSamples && AiSamplerGetSample(iter, rndTmp)) 
			{ 

				rnd[0] = static_cast<float>(rndTmp[0]);
				rnd[1] = static_cast<float>(rndTmp[1]);

				AtPoint p[1] = {sg->P - enterSurface*AI_EPSILON*sg->Ngf};

				// get theta phi
				float theta, phi;
				UVToSpherical(float(rnd[0]),float(rnd[1]),maxAngle,cosLobeExponent,&theta, &phi);

				// simplified rodrigues rotations for theta
				AtVector Vt = V*std::cos(theta) + KcrossV*std::sin(theta);

				// Phi
				AtVector R = RodriguesRotation(Vt,V,phi);

				// value accumulation
				scalarout += depthRayTrace(sg, p, &R,intersectOthers, maxRayDepth, maxDistance2use,noHitValue,numOfHitMode, multNdotR, NdotRExp, volumeMode,multiplyByDepth);
				iSample++;
			}

			// Averaging
			scalarout/=float(iSample);
		}


		// Normalize if wanted
		if(normalizeMode > 0){

			if(scalarout > maxRayLength)
				scalarout = 1.0f;
			else
				scalarout/=maxRayLength;

			// inverse normalize
			if(normalizeMode == 2)
				scalarout = 1.0f-scalarout;
		}
	}
	else
	{
		if(backface == 0)
			scalarout = 0.0f;
		else if(backface == 1)
			scalarout = 1.0f;
		else
			scalarout = AiShaderEvalParamFlt(p_maxRayLength);

	}

	// output
	sg->out.FLT = scalarout;
	
}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqThicknessSimpleMethods;
//   node->output_type  = AI_TYPE_FLOAT;
//   node->name         = "Obq_Thickness";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
