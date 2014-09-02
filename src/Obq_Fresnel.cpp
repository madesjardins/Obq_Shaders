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
   AiParameterINT("mode", 1);								// Fresnel Mode, custom, presets, files
   AiParameterSTR("iorFilename","");						// eta filename
   AiParameterINT("lambdaUnits", 0);						// 0 = nano, 1 = micro (*1000)
   AiParameterRGB("iorRGB",0.069151f,0.475555f,1.425f);		// refractive indices
   AiParameterRGB("kRGB",3.306609f,2.534851f,1.879887f);	// extinction coeffs
   AiParameterINT("method", 1);								// Method to compute IOR
   AiParameterRGB("iorInRGB",1.0f,1.0f,1.0f);				// Media in which the object is
   AiParameterFLT("ratioFsFp", 0.5f);						// Ratio Fs Fp
   AiParameterBOOL("transmittance", false);					// T = 1 - R
   AiParameterINT("backfaceMode",1);						// backface mode
   AiParameterBOOL("degamma",true);							// degamma 2.2 for metal
   AiParameterBOOL("useLUT",true);							// use LUT for metals
   AiParameterFLT("LUTSampleSize", 0.5f);					// size of a sample, default 0.5°
   AiParameterBOOL("useFullSpectrum", false);				// use full spectrum in equation
}

node_initialize
{
	ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
	
	data->eta[0] = 1.0;	data->eta[1] = 1.0;	data->eta[2] = 1.0;
	data->k[0] = 0.0;	data->k[1] = 0.0;	data->k[2] = 0.0;
	Spectrum r;

	data->LUT = NULL;
	data->lambdas = NULL ;
	data->etas = NULL;
	data->ks = NULL;

	AiNodeSetLocalData(node,data);
}


node_update
{

	// Access shader Data
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
	
	data->eta[0] = 1.0;	data->eta[1] = 1.0;	data->eta[2] = 1.0;
	data->k[0] = 0.0;	data->k[1] = 0.0;	data->k[2] = 0.0;
	bool fileOK = false;
	
	data->useFullSpectrum = params[p_useFullSpectrum].BOOL;
	if(data->useFullSpectrum && ((params[p_mode].INT != MODE_FILES) || (params[p_method].INT == REFRACTIVE)))
	{
		data->useFullSpectrum = false;
		//AiMsgWarning("Can only use full spectrum with file mode for metals, reverting to approximation.");
	}

	data->degamma = params[p_degamma].BOOL;

	// reset tables for lambdas, etas and ks
	if(data->lambdas != NULL)
	{
		delete[] data->lambdas;
		data->lambdas = NULL;
	}
	if(data->etas != NULL) 
	{
		delete[] data->etas;
		data->etas = NULL;
	}
	if(data->ks != NULL) 
	{
		delete[] data->ks;
		data->ks = NULL;
	}
	

	if(params[p_mode].INT == MODE_FILES)
	{
		short ft = fileType(params[p_iorFilename].STR);
		float unitConversion = ((params[p_lambdaUnits].INT==UNITS_NANO)?1.0f:1000.0f);

		std::vector<float> lambdas;
		std::vector<float> etas;
		std::vector<float> ks;

		switch(ft)
		{
		case TYPE_CSV:
			{
				
				bool ret = loadCSV(params[p_iorFilename].STR,&lambdas,&etas,&ks,unitConversion);
				if(ret)
				{
					if(!data->useFullSpectrum)
					{
						Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size());
						s.ToRGB(data->eta);
						s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size());
						s.ToRGB(data->k);
					}
				}
				break;
			}
		case TYPE_TXT:
			{
				bool ret = loadTXT(params[p_iorFilename].STR,&lambdas,&etas,&ks,unitConversion);
				if(ret)
				{
					if(!data->useFullSpectrum)
					{
						Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size());
						s.ToRGB(data->eta);
						s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size());
						s.ToRGB(data->k);
					}
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
					Spectrum s;
					if(!data->useFullSpectrum)
					{
						s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size());
						s.ToRGB(data->eta);
					}

					lambdas.clear();

					if(kFilename.size() < 6 || !fileExists(kFilename.c_str()))
						break;
					ret = loadSPD(kFilename.c_str(),&lambdas,&ks,unitConversion);
					if(ret)
					{
						if(!data->useFullSpectrum)
						{
							s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size());
							s.ToRGB(data->k);
						}
					}
				}
			break;
			}
		default :
			AiMsgWarning("File : \"%s\" is not of a recognized format. Will use ior = 1,1,1 and k = 0,0,0.",params[p_iorFilename].STR);
		}

		// copy to global
		if(data->useFullSpectrum)
		{
			AiMsgInfo("labda size = %u",lambdas.size());
			data->lambdasSize = static_cast<int>(lambdas.size());
			data->lambdas = new float[data->lambdasSize];
			data->etas = new float[data->lambdasSize];
			data->ks = new float[data->lambdasSize];
			AiMsgInfo("--------------------------------------");
			AiMsgInfo("IOR : ");

			for(int i = 0; i < data->lambdasSize; i++)
			{
				data->lambdas[i] = lambdas[i];
				data->etas[i] = etas[i];
				data->ks[i] = ks[i];
				AiMsgInfo("      lambda[%i] : %f %s = ( %f, %f )", i, data->lambdas[i], ((params[p_lambdaUnits].INT==UNITS_NANO)?"nm":"um"), data->etas[i], data->ks[i]);
			}
			AiMsgInfo("--------------------------------------");
		}
		else
		{
			fileOK = true;
			AiMsgInfo("--------------------------------------");
			AiMsgInfo("IOR : \tn red\t= %f",data->eta[0]);
			AiMsgInfo("      \tn green\t= %f",data->eta[1]);
			AiMsgInfo("      \tn blue\t= %f",data->eta[2]);
			AiMsgInfo("      \tk red\t= %f",data->k[0]);
			AiMsgInfo("      \tk green\t= %f",data->k[1]);
			AiMsgInfo("      \tk blue\t= %f",data->k[2]);
			AiMsgInfo("--------------------------------------");
		}
		
		lambdas.clear();
		etas.clear();
		ks.clear();
	}

	//-------
	// LUT
	if(data->LUT != NULL)
	{
		//delete data->LUT;
		AiFree(data->LUT);
		data->LUT = NULL;
	}

	// Make sure nothing is plugged into the IOR
	if(params[p_useLUT].BOOL)
		data->useLUT = true;
	else
		data->useLUT = false;

	if(data->useLUT && (AiNodeIsLinked(node,"iorRGB") || AiNodeIsLinked(node,"kRGB") || AiNodeIsLinked(node,"ratioFsFp") ||  AiNodeIsLinked(node,"iorInRGB")))
	{
		data->useLUT = false;
		AiMsgWarning("Obq_Fresnel : A parameter used for LUT calculation as a texture plugged in, cannot use LUT. Reverting to non-LUT mode.");
	}

	if(data->useLUT)
	{
		data->LUTRes = std::max(static_cast<int>(std::ceil(90.0/params[p_LUTSampleSize].FLT)),1);
		data->LUTSampleSize = 90.0f/static_cast<float>(data->LUTRes);

		// Memory allocation
		if(params[p_method].INT == METALS)
			data->LUT = (AtColor*) AiMalloc((data->LUTRes+1)*sizeof(AtColor));
		else
			data->LUT = (AtColor*) AiMalloc(2*(data->LUTRes+1)*sizeof(AtColor));
		
		// Calculate and store in array
		double lutres = data->LUTRes;
		double ratioFsFp = params[p_ratioFsFp].FLT;
		double ratioFpFs = (1.0-ratioFsFp);
		
		if(!data->useFullSpectrum)
		{
			// get iorRGB from params if not in file mode
			if(!fileOK)
			{
				data->eta[0] = params[p_iorRGB].RGB.r; data->eta[1] = params[p_iorRGB].RGB.g; data->eta[2] = params[p_iorRGB].RGB.b;
				data->k[0] = params[p_kRGB].RGB.r; data->k[1] = params[p_kRGB].RGB.g; data->k[2] = params[p_kRGB].RGB.b;
			}

			if(params[p_method].INT == METALS)
			{
				for(int i = 0; i <= data->LUTRes; i++)
				{
					double theta = static_cast<double>(i)/lutres*c_PiOver2__d;

					double costheta = cos(theta);
					double cos2theta = pow(costheta,2);
					double colorNs[] = {data->eta[0],data->eta[1],data->eta[2]};
					double colorKs[] = {data->k[0],data->k[1],data->k[2]};
					double colorFs[] = {0.0,0.0,0.0};

					// metal
					for(int j =0; j < 3; j++)
					{
						double tmp_f = colorNs[j]*colorNs[j] + colorKs[j]*colorKs[j];
						double tmp = tmp_f*cos2theta;
						double _2etacos = 2.0*colorNs[j]*costheta;
						double Rparl2 = (tmp - (_2etacos) + 1.0)/
							(tmp + (_2etacos) + 1.0);
						double Rperp2 = (tmp_f - (_2etacos) + cos2theta)/
							(tmp_f + (_2etacos) + cos2theta);

						colorFs[j] = (ratioFsFp*Rperp2 + ratioFpFs*Rparl2);
					}

					// degamma 2.2 by default
					degamma(colorFs);
					
					// to sRGB
					if(!data->degamma)
						linear2sRGB(colorFs);

					// Out color
					data->LUT[i].r = static_cast<float>(Minimax(0.0,colorFs[0],1.0));
					data->LUT[i].g = static_cast<float>(Minimax(0.0,colorFs[1],1.0));
					data->LUT[i].b = static_cast<float>(Minimax(0.0,colorFs[2],1.0));

					// Test if color is corrupted
					if(AiColorCorrupted(data->LUT[i]))
						data->LUT[i] = AI_RGB_WHITE;
				}
			}
			else // REFRACTIVE
			{

				double colorNsIN[] = {params[p_iorInRGB].RGB.r,params[p_iorInRGB].RGB.g, params[p_iorInRGB].RGB.b};
				double colorKs[] = {0.0f,0.0f,0.0f};

				for(int i = 0; i <= data->LUTRes; i++)
				{
					double theta = static_cast<double>(i)/lutres*c_PiOver2__d;

					double costheta = cos(theta);

					for( int k =0; k <2; k++)
					{
						double colorNs[] = {data->eta[0],data->eta[1],data->eta[2]};
						double colorNs1[] = {colorNsIN[0],colorNsIN[1],colorNsIN[2]};
						int index = i;

						if(k) // ---- N2->N1
						{
							colorNs[0] = colorNsIN[0] ; colorNs[1] = colorNsIN[1] ; colorNs[2] = colorNsIN[2];
							colorNs1[0] = data->eta[0] ; colorNs1[1] = data->eta[1] ; colorNs1[2] = data->eta[2];
							index += data->LUTRes+1;
						}


						double max_n1 = mmax(colorNs1[0],colorNs1[1],colorNs1[2]);
						double mmin_n2 = mmin(colorNs[0],colorNs[1],colorNs[2]);

						double colorFs[] = {0.0,0.0,0.0};

						double n2_n1 = mmin_n2/max_n1;
						double theta_c = (n2_n1 <= 1.0)?asin(n2_n1)-AI_EPSILON: c_2Pi+AI_EPSILON;

						// TOTAL INTERN REFLECTION (arnold will tir)
						if(theta>=theta_c)
						{
							colorFs[0] = 0.0;
							colorFs[1] = 0.0;
							colorFs[2] = 0.0;
						}
						else
						{
							for(int j = 0 ; j<3; j++){
								double sqrtPart = sqrt(1.0 - pow((colorNs1[j]/colorNs[j])*sin(theta),2));
								double Fs = pow(( colorNs1[j]*costheta - colorNs[j]*sqrtPart)/
									(colorNs1[j]*costheta + colorNs[j]*sqrtPart),2);
								double Fp = pow(( colorNs1[j]*sqrtPart - colorNs[j]*costheta )/
									(colorNs1[j]*sqrtPart + colorNs[j]*costheta),2);
								colorFs[j] = (ratioFsFp*Fs + ratioFpFs*Fp);
							}
						}

						// Out color
						data->LUT[index].r = static_cast<float>(Minimax(0.0,colorFs[0],1.0));
						data->LUT[index].g = static_cast<float>(Minimax(0.0,colorFs[1],1.0));
						data->LUT[index].b = static_cast<float>(Minimax(0.0,colorFs[2],1.0));

						// Test if color is corrupted
						if(AiColorCorrupted(data->LUT[index]))
							data->LUT[index] = AI_RGB_WHITE;
					}
				}
			}
		}
		else // Full spectrum for metal
		{

			for(int i = 0; i <= data->LUTRes; i++)
			{
				double theta = static_cast<double>(i)/lutres*c_PiOver2__d;

				double costheta = cos(theta);
				double cos2theta = pow(costheta,2);

				double colorFs[] = {0.0,0.0,0.0};
				float* fs = new float[data->lambdasSize];

				// metal
				for(int j =0; j <data->lambdasSize; j++)
				{
					double tmp_f = data->etas[j]*data->etas[j] + data->ks[j]*data->ks[j];
					double tmp = tmp_f*cos2theta;
					double _2etacos = 2.0*data->etas[j]*costheta;
					double Rparl2 = (tmp - (_2etacos) + 1.0)/
						(tmp + (_2etacos) + 1.0);
					double Rperp2 = (tmp_f - (_2etacos) + cos2theta)/
						(tmp_f + (_2etacos) + cos2theta);

					fs[j] = static_cast<float>((ratioFsFp*Rperp2 + ratioFpFs*Rparl2));
				}

				Spectrum s = Spectrum::FromSampled(data->lambdas, fs, data->lambdasSize);
				delete[] fs;

				s.ToRGB(colorFs);

				// degamma by default
				degamma(colorFs);

				// convert to sRGB
				if(!data->degamma)
					linear2sRGB(colorFs);


				// Out color
				data->LUT[i].r = static_cast<float>(Minimax(0.0,colorFs[0],1.0));
				data->LUT[i].g = static_cast<float>(Minimax(0.0,colorFs[1],1.0));
				data->LUT[i].b = static_cast<float>(Minimax(0.0,colorFs[2],1.0));

				// Test if color is corrupted
				if(AiColorCorrupted(data->LUT[i]))
					data->LUT[i] = AI_RGB_WHITE;
			}
		}
	}
}

node_finish
{
	// Desallocate shader data
	ShaderData* data = (ShaderData*)AiNodeGetLocalData(node);
	if(data->LUT != NULL)
	{
		//delete data->LUT;
		AiFree(data->LUT);
		data->LUT = NULL;
	}
		// reset tables for lambdas, etas and ks
	if(data->lambdas != NULL)
	{
		delete[] data->lambdas;
		data->lambdas = NULL;
	}
	if(data->etas != NULL) 
	{
		delete[] data->etas;
		data->etas = NULL;
	}
	if(data->ks != NULL) 
	{
		delete[] data->ks;
		data->ks = NULL;
	}
	AiFree(data);
}

shader_evaluate
{
	ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

	AtColor iorRGB, kRGB, iorInRGB;

	//BACKFACE
	AtVector s_normal = sg->N;
	double s_ndotv = AiV3Dot(s_normal, sg->Rd);
	bool backface = (s_ndotv >= 0.0);
	s_ndotv = Minimax(AI_EPSILON,abs(s_ndotv),1.0);
	
	if(backface)
		s_normal = -s_normal;


	int backfaceMode = AiShaderEvalParamInt(p_backfaceMode);

	int method = AiShaderEvalParamInt(p_method);
	bool transmittance = AiShaderEvalParamBool(p_transmittance);

	// back face and trivial white or black
	if( backfaceMode > BACKFACE_SWAP && backface && method == REFRACTIVE)
	{
		if(backfaceMode == BACKFACE_WHITE)
			sg->out.RGB = AI_RGB_WHITE;
		else
			sg->out.RGB = AI_RGB_BLACK;
	}
	// LUT
	else if(data->useLUT)
	{

		// get theta
		double theta = acos(s_ndotv);
			
		float index_f = static_cast<float>(Minimax(0.0,theta/c_PiOver2,1.0))*static_cast<float>(data->LUTRes);

		if(backfaceMode == BACKFACE_SWAP && backface && method == REFRACTIVE)
		{
			index_f += (data->LUTRes+1);
		}

		float index_floor = floor(index_f);
		float index_ceil = ceil(index_f);
		float t = index_f - index_floor;

		sg->out.RGB = (1.0f-t)*data->LUT[static_cast<int>(index_floor)] + t*data->LUT[static_cast<int>(index_ceil)];

	}
	else if(data->useFullSpectrum) // metal
	{
		double ratioFsFp = double(AiShaderEvalParamFlt(p_ratioFsFp));
		double ratioFpFs = (1.0-ratioFsFp);
		
		double costheta = s_ndotv;

		double colorFs[] = {0.0,0.0,0.0};

		double cos2theta = pow(costheta,2);
		float* fs = new float[data->lambdasSize];


		for(int i =0; i < data->lambdasSize; i++)
		{

			double tmp_f = data->etas[i]*data->etas[i] + data->ks[i]*data->ks[i];
			double tmp = tmp_f*cos2theta;
			double _2etacos = 2.0*data->etas[i]*costheta;
			double Rparl2 = (tmp - (_2etacos) + 1.0)/
				(tmp + (_2etacos) + 1.0);
			double Rperp2 = (tmp_f - (_2etacos) + cos2theta)/
				(tmp_f + (_2etacos) + cos2theta);

			fs[i] = static_cast<float>((ratioFsFp*Rperp2 + ratioFpFs*Rparl2));
		}

		Spectrum s = Spectrum::FromSampled(data->lambdas, fs, data->lambdasSize);
		
		s.ToRGB(colorFs);
		delete[] fs;

		degamma(colorFs);
		if(!data->degamma)
			linear2sRGB(colorFs);
		
		sg->out.RGB.r = static_cast<float>(Minimax(0.0,colorFs[0],1.0));
		sg->out.RGB.g = static_cast<float>(Minimax(0.0,colorFs[1],1.0));
		sg->out.RGB.b = static_cast<float>(Minimax(0.0,colorFs[2],1.0));
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
		
		double costheta = s_ndotv;

		double colorNs[] = {iorRGB.r,iorRGB.g,iorRGB.b};
		double colorNs1[] = {iorInRGB.r,iorInRGB.g,iorInRGB.b};
		double colorKs[] = {kRGB.r,kRGB.g,kRGB.b};
		double colorFs[] = {0.0,0.0,0.0};

		// Thanks to PBRT
		switch(method){ 
			
			// Fresnel
		case REFRACTIVE :
			{
				double theta = acos(s_ndotv);
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

				//if(data->degamma)
				//	degamma();
				//{
				//	colorFs[0] = std::pow(colorFs[0],2.2);
				//	colorFs[1] = std::pow(colorFs[1],2.2);
				//	colorFs[2] = std::pow(colorFs[2],2.2);
				//}

				break;
			}
			// Complex Fresnel
		case METALS:
			{
				double cos2theta = pow(costheta,2);
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
				degamma(colorFs);
				if(!data->degamma)
					linear2sRGB(colorFs);

				break;
			}
		}

		// Out color
		sg->out.RGB.r = float(Minimax(0.0,colorFs[0],1.0));
		sg->out.RGB.g = float(Minimax(0.0,colorFs[1],1.0));
		sg->out.RGB.b = float(Minimax(0.0,colorFs[2],1.0));

		// Test if color is corrupted
		if(AiColorCorrupted(sg->out.RGB))
			sg->out.RGB = AI_RGB_BLACK;

	}
	// Transmittance T = 1 - R
	if(transmittance)
	{
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
