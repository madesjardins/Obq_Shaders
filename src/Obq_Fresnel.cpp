/*
Obq_Fresnel :

	a dielectric and a complex fresnel term node. It uses pbrt spectrum class.

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

#include "Obq_Fresnel.h"

using namespace std;

node_parameters
{
   AiParameterINT("mode", 1);						// Fresnel Mode, custom, presets, files
   AiParameterSTR("iorFilename","");				// eta filename
   AiParameterINT("lambdaUnits", 0);				// 0 = nano, 1 = micro (*1000)
   AiParameterRGB("iorRGB",0.069151f,0.475555f,1.425f);	// refractive indices
   AiParameterRGB("kRGB",3.306609f,2.534851f,1.879887f);			// extinction coeffs
   AiParameterINT("method", 1);						// Method to compute IOR
   AiParameterRGB("iorInRGB",1.0f,1.0f,1.0f);		// Media in which the object is
   AiParameterFLT("ratioFsFp", 0.5f);				// Ratio Fs Fp
   AiParameterBOOL("transmittance", false);			// T = 1 - R
   AiParameterINT("backfaceMode",1);				// backface mode
   AiParameterBOOL("degamma",true);				//degamma 2.2

}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	
	data->eta[0] = 1.0;	data->eta[1] = 1.0;	data->eta[2] = 1.0;
	data->k[0] = 0.0;	data->k[1] = 0.0;	data->k[2] = 0.0;
	Spectrum r;
	AiNodeSetLocalData(node,data);
}



node_update
{

	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	data->eta[0] = 1.0;	data->eta[1] = 1.0;	data->eta[2] = 1.0;
	data->k[0] = 0.0;	data->k[1] = 0.0;	data->k[2] = 0.0;

	if(params[p_mode].INT == MODE_FILES)
	{
		vector<float> lambdas;
		vector<float> etas;
		vector<float> ks;
		short ft = fileType(params[p_iorFilename].STR);
		float unitConversion = ((params[p_lambdaUnits].INT==UNITS_NANO)?1.0f:1000.0f);
		switch(ft)
		{
		case TYPE_CSV:
			{
				bool ret = loadCSV(params[p_iorFilename].STR,&lambdas,&etas,&ks,unitConversion);
				if(ret)
				{
					Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size());
					s.ToRGB(data->eta);
					s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size());
					s.ToRGB(data->k);
					lambdas.clear();
					etas.clear();
					ks.clear();
				}
				break;
			}
		case TYPE_TXT:
			{
				bool ret = loadTXT(params[p_iorFilename].STR,&lambdas,&etas,&ks,unitConversion);
				if(ret)
				{

					Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size());
					s.ToRGB(data->eta);
					s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size());
					s.ToRGB(data->k);
					lambdas.clear();
					etas.clear();
					ks.clear();

				}
				break;
			}
		case TYPE_SPD:
			{
				std::string etaFilename(params[p_iorFilename].STR);
                std::string kFilename("");

                if(etaFilename.compare(etaFilename.size()-8,8,".eta.spd") == 0)
                {
                    kFilename = etaFilename.substr(0,etaFilename.size()-8) + std::string(".k.spd");
                }
                else if(etaFilename.compare(etaFilename.size()-6,6,".k.spd") == 0)
                {
                    kFilename = etaFilename;
                    etaFilename = etaFilename.substr(0,etaFilename.size()-6) + std::string(".eta.spd");
                }

				bool ret = loadSPD(etaFilename.c_str(),&lambdas,&etas,unitConversion);
				if(ret)
				{
					Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size());
					s.ToRGB(data->eta);
					lambdas.clear();
					etas.clear();
					if(kFilename.size() < 6 || !fileExists(kFilename.c_str()))
						break;
					ret = loadSPD(kFilename.c_str(),&lambdas,&ks,unitConversion);
					if(ret)
					{
						s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size());
						s.ToRGB(data->k);
						lambdas.clear();
						ks.clear();
					}
				}
			break;
			}
		default :
			AiMsgWarning("File : \"%s\" is not of a recognized format. Will use ior = 1,1,1 and k = 0,0,0.",params[p_iorFilename].STR);
		}
		
		AiMsgInfo("--------------------------------------");
		AiMsgInfo("IOR : \tn red\t= %f",data->eta[0]);
		AiMsgInfo("      \tn green\t= %f",data->eta[1]);
		AiMsgInfo("      \tn blue\t= %f",data->eta[2]);
		AiMsgInfo("      \tk red\t= %f",data->k[0]);
		AiMsgInfo("      \tk green\t= %f",data->k[1]);
		AiMsgInfo("      \tk blue\t= %f",data->k[2]);
		AiMsgInfo("--------------------------------------");

	}
}

node_finish
{
	// Desallocate shader data
	AiFree((ShaderData*)AiNodeGetLocalData(node));
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	AtColor iorRGB, kRGB, iorInRGB;

	//bool backface = (sg->Nf!=sg->N);

	//BACKFACE
	AtVector s_normal = sg->N;
	double s_ndotv = AiV3Dot(s_normal, sg->Rd);
	bool backface = (s_ndotv >= 0.0);
	s_ndotv = Minimax(AI_EPSILON,abs(s_ndotv),1.0);
	
	if(backface)
		s_normal = -s_normal;


	int backfaceMode = AiShaderEvalParamInt(p_backfaceMode);

	int method = AiShaderEvalParamInt(p_method);

	// back face and trivial white or black
	if( backfaceMode > BACKFACE_SWAP && backface && method == REFRACTIVE)
	{
		if(backfaceMode == BACKFACE_WHITE)
			sg->out.RGB = AI_RGB_WHITE;
		else
			sg->out.RGB = AI_RGB_BLACK;
	}
	else
	{
		// back face and swap
		if(backfaceMode == BACKFACE_SWAP && backface && method == REFRACTIVE)
		{
			iorRGB = AiShaderEvalParamRGB(p_iorInRGB);
			kRGB = AI_RGB_BLACK;

			if(AiShaderEvalParamInt(p_mode)==MODE_FILES)
			{
				iorInRGB.r = data->eta[0]; iorInRGB.g = data->eta[1]; iorInRGB.b = data->eta[2];
			}
			else
			{
				iorInRGB = AiShaderEvalParamRGB(p_iorRGB);
				
			}
		}
		else // Normal
		{
			if(AiShaderEvalParamInt(p_mode)==MODE_FILES)
			{
				iorRGB.r = data->eta[0]; iorRGB.g = data->eta[1]; iorRGB.b = data->eta[2];
				kRGB.r = data->k[0];	 kRGB.g = data->k[1];	  kRGB.b = data->k[2];
			}
			else
			{
				iorRGB = AiShaderEvalParamRGB(p_iorRGB);
				kRGB = AiShaderEvalParamRGB(p_kRGB);
			}

			if(method == REFRACTIVE)
				iorInRGB = AiShaderEvalParamRGB(p_iorInRGB);
		}


		double ratioFsFp = double(AiShaderEvalParamFlt(p_ratioFsFp));
		double ratioFpFs = (1.0-ratioFsFp);
		bool transmittance = AiShaderEvalParamBool(p_transmittance);
		//double ndotv;

		// Shading Normal
		//AtVector normal = sg->Nf , v = -sg->Rd;
		//ndotv =  AiV3Dot(normal,v);

		// Just to make sure ndotv is between -1 and 1
		//ndotv = Minimax(-1.0, ndotv, 1.0);

		double theta = acos(s_ndotv);
		double costheta = s_ndotv;
		double cos2theta = pow(costheta,2);
		double sintheta = sin(theta);
		double sin2theta = pow(sintheta,2);
		double tantheta = tan(theta);
		double tan2theta = pow(tantheta,2);

		double colorNs[] = {iorRGB.r,iorRGB.g,iorRGB.b};
		double colorNs1[] = {iorInRGB.r,iorInRGB.g,iorInRGB.b};
		double colorKs[] = {kRGB.r,kRGB.g,kRGB.b};
		double colorFs[] = {0.0,0.0,0.0};

		// Thanks to PBRT
		switch(method){ 
			
			// Fresnel
		case REFRACTIVE :
			{
				//total intern reflection : 
				//get minimum n2 and maximum n1
				double max_n1 = mmax(colorNs1[0],colorNs1[1],colorNs1[2]);
				double mmin_n2 = mmin(colorNs[0],colorNs[1],colorNs[2]);
				double n2_n1 = mmin_n2/max_n1;
				//bool doTirCheck = n2_n1 <= 1.0;
				double theta_c = (n2_n1 <= 1.0)?asin(n2_n1)-AI_EPSILON: c_2Pi+AI_EPSILON;

				// TOTAL INTERN REFLECTION (arnold will tir)
				if(theta>=theta_c)
				{
					colorFs[0] = 0.0;
					colorFs[1] = 0.0;
					colorFs[2] = 0.0;
					break;
				}

				for(int i = 0 ; i<3; i++){
					double sqrtPart = sqrt(1.0 - pow((colorNs1[i]/colorNs[i])*sin(theta),2));
					double Fs = pow(( colorNs1[i]*costheta - colorNs[i]*sqrtPart)/
						(colorNs1[i]*costheta + colorNs[i]*sqrtPart),2);
					double Fp = pow(( colorNs1[i]*sqrtPart - colorNs[i]*costheta )/
						(colorNs1[i]*sqrtPart + colorNs[i]*costheta),2);
					colorFs[i] = (ratioFsFp*Fs + ratioFpFs*Fp);
				}
				break;
			}
			// Complex Fresnel
		case METALS:
			for(int i =0; i < 3; i++){

				double tmp_f = colorNs[i]*colorNs[i] + colorKs[i]*colorKs[i];
				double tmp = tmp_f*cos2theta;
				double _2etacos = 2.0*colorNs[i]*costheta;
				double Rparl2 = (tmp - (_2etacos) + 1.0)/
					(tmp + (_2etacos) + 1.0);
				double Rperp2 = (tmp_f - (_2etacos) + cos2theta)/
					(tmp_f + (_2etacos) + cos2theta);

				colorFs[i] = (ratioFsFp*Rperp2 + ratioFpFs*Rparl2);
			}
			if(AiShaderEvalParamBool(p_degamma))
			{
				colorFs[0] = std::pow(colorFs[0],2.2);
				colorFs[1] = std::pow(colorFs[1],2.2);
				colorFs[2] = std::pow(colorFs[2],2.2);
			}
			break;
		}

		// Out color
		sg->out.RGB.r = float(Minimax(0.0,colorFs[0],1.0));
		sg->out.RGB.g = float(Minimax(0.0,colorFs[1],1.0));
		sg->out.RGB.b = float(Minimax(0.0,colorFs[2],1.0));

		// Test if color is corrupted
		if(AiColorCorrupted(sg->out.RGB))
			sg->out.RGB = AI_RGB_BLACK;

		// Transmittance T = 1 - R
		if(transmittance){
			if(method == REFRACTIVE)
			{
				sg->out.RGB.r = 1.0f-sg->out.RGB.r;
				sg->out.RGB.g = 1.0f-sg->out.RGB.g;
				sg->out.RGB.b = 1.0f-sg->out.RGB.b;
			}
			else
			{
				sg->out.RGB.r = 0.0f;
				sg->out.RGB.g = 0.0f;
				sg->out.RGB.b = 0.0f;
			}
		}
	}

}

//node_loader
//{
//   if (i > 0)
//      return FALSE;
//
//   node->methods      = ObqFresnelSimpleMethods;
//   node->output_type  = AI_TYPE_RGB;
//   node->name         = "Obq_Fresnel";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
