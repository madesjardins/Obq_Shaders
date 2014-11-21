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
	AiParameterINT("mode", MODE_CUSTOM);						// Fresnel Mode, custom legacy, presets, files
	AiParameterSTR("iorFilename","");						// eta filename
	AiParameterINT("lambdaUnits", 0);						// 0 = nano, 1 = micro (*1000)
	AiParameterRGB("iorRGB",0.100899f,0.401072f,1.569860f);	// refractive indices Default is Gold
	AiParameterRGB("kRGB",3.404634f,2.516492f,1.776652f);	// extinction coeffs
	AiParameterINT("method", 1);								// Method to compute IOR
	AiParameterRGB("iorInRGB",1.0f,1.0f,1.0f);				// Media in which the object is
	AiParameterFLT("ratioFsFp", 0.5f);						// Ratio Fs Fp
	AiParameterBOOL("transmittance", false);					// T = 1 - R
	AiParameterINT("backfaceMode",1);						// backface mode
	AiParameterBOOL("useLUT",true);							// use LUT for metals
	AiParameterFLT("LUTSampleSize", 0.5f);					// size of a sample, default 0.5°
	AiParameterBOOL("useFullSpectrum", true);				// use full spectrum in equation
	AiParameterBOOL("degamma",false);						// old and bad linear workflow degamma
	AiParameterINT("outputGamma",0);							// gamma operation None
	AiParameterSTR("lambdasStr","364.699982 375.700012 387.5 400.0 413.300018 427.5 442.799988 459.199982 476.900024 495.899994 516.600037 539.099976 563.599976 652.599976 688.799988 729.300049 774.900024 826.600037");	// SPDs strings space separated for gold
	AiParameterSTR("etasStr","1.716 1.696 1.674 1.658 1.636 1.616 1.562 1.426 1.242 0.916 0.608 0.402 0.306 0.166 0.16 0.164 0.174 0.188");																					// ""
	AiParameterSTR("ksStr","1.862 1.906 1.936 1.956 1.958 1.94 1.904 1.846 1.796 1.84 2.12 2.54 2.88 3.15 3.8 4.35 4.86 5.39");																								// ""
	AiParameterINT("inputType",INPUT_RGB);					// RGB Legacy or string SPD
	AiParameterINT("ccMode",CC_OFF);						// Color correction mode
	AiParameterFLT("hueShift", 0.0f);						// hue shift
	AiParameterFLT("saturationMod", 1.0f);					// saturation modifier
	AiParameterINT("saturationOp",CC_MUL);					// saturation operator
	AiParameterFLT("roughness", 0.0f);						// roughness parameter for GTR2 (GGX) filtering
	AiParameterFLT("roughnessSampleSize", 0.05f);			// LUT roughness sample size
	AiParameterINT("xyz2rgb",SRGB_ILLUM_D65_BRADFORD_E);	// XYZ to RGB Matrix

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

	int mode = params[p_mode].INT;
	int inputType = params[p_inputType].INT;
	data->ccMode = params[p_ccMode].INT;
	data->satOp = params[p_satOp].INT;
	data->xyz2rgb = params[p_xyz2rgb].INT;

	bool fileOK = false;
	data->method = params[p_method].INT;

	data->useFullSpectrum = params[p_useFullSpectrum].BOOL;
	if(data->useFullSpectrum && (!(params[p_mode].INT == MODE_FILES || params[p_inputType].INT == INPUT_STRING) || data->method == REFRACTIVE))
	{
		data->useFullSpectrum = false;
		//AiMsgWarning("Can only use full spectrum with file mode for metals, reverting to approximation.");
	}
	data->degamma = params[p_degamma].BOOL;
	data->transfer2sRGB = params[p_outputGamma].INT==OUTPUT_SRGB;

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

	if(mode == MODE_FILES || inputType == INPUT_STRING)
	{
		float unitConversion = ((params[p_lambdaUnits].INT==UNITS_NANO)?1.0f:1000.0f);

		std::vector<float> lambdas;
		std::vector<float> etas;
		std::vector<float> ks;
		
		if(mode == MODE_FILES)
		{
			short ft = fileType(params[p_iorFilename].STR);
			switch(ft)
			{
			case TYPE_CSV:
				{

					bool ret = loadCSV(params[p_iorFilename].STR,&lambdas,&etas,&ks,unitConversion);
					if(ret)
					{
						if(!data->useFullSpectrum)
						{
							Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size(),data->xyz2rgb);
							s.ToRGB(data->eta);
							s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size(),data->xyz2rgb);
							s.ToRGB(data->k);
						}
						fileOK = true;
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
							Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size(),data->xyz2rgb);
							s.ToRGB(data->eta);
							s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size(),data->xyz2rgb);
							s.ToRGB(data->k);
						}
						fileOK = true;
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
							s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size(),data->xyz2rgb);
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
								s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size(),data->xyz2rgb);
								s.ToRGB(data->k);
							}
							fileOK = true;
						}
					}
					break;
				}
			default :
				AiMsgError("File : \"%s\" is not of a recognized format. Will use ior = 1,1,1 and k = 0,0,0.",params[p_iorFilename].STR);
			}
		}
		else // INPUT_STRING
		{
			bool ret = loadStrings(params[p_lambdasStr].STR,params[p_etasStr].STR,params[p_ksStr].STR,&lambdas,&etas,&ks,unitConversion);

			if(ret)
			{
				if(!data->useFullSpectrum)
				{
					Spectrum s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(etas[0]), (int)lambdas.size(),data->xyz2rgb);
					s.ToRGB(data->eta);
					s = Spectrum::FromSampled((float*)&(lambdas[0]), (float*)&(ks[0]), (int)lambdas.size(),data->xyz2rgb);
					s.ToRGB(data->k);
				}
				fileOK = true;
			}
			else
				AiMsgError("String : strings are not valid, check sizes (%u, %u, %u). Will use ior = 1,1,1 and k = 0,0,0.",lambdas.size(),etas.size(),ks.size());
		}

		// copy to global
		if(data->useFullSpectrum && fileOK)
		{
			
			data->lambdasSize = static_cast<int>(lambdas.size());
			data->lambdas = new float[data->lambdasSize];
			data->etas = new float[data->lambdasSize];
			data->ks = new float[data->lambdasSize];
			

			std::string info_lambdas("");
			std::string info_etas("");
			std::string info_ks("");
			char buf[256];
			bool filemode = (mode == MODE_FILES);
			for(int i = 0; i < data->lambdasSize; i++)
			{
				data->lambdas[i] = lambdas[i];
				data->etas[i] = etas[i];
				data->ks[i] = ks[i];
				if(filemode)
				{
					sprintf(buf,"%f ",lambdas[i]);
					info_lambdas.append(buf);
					sprintf(buf,"%f ",etas[i]);
					info_etas.append(buf);
					sprintf(buf,"%f ",ks[i]);
					info_ks.append(buf);
				}
			}

			if(filemode)
			{
				AiMsgInfo("--------------------------------------");
				AiMsgInfo("IOR strings form :");
				AiMsgInfo("\tlambda : %s",info_lambdas.c_str());
				AiMsgInfo("\tEta (n): %s",info_etas.c_str());
				AiMsgInfo("\tk      : %s",info_ks.c_str());
				AiMsgInfo("--------------------------------------");
			}
		}
		else
		{
			AiMsgInfo("--------------------------------------");
			AiMsgInfo("IOR RGB Form:");
			AiMsgInfo("\tn (R,G,B)\t= (%f,%f,%f)",data->eta[0],data->eta[1],data->eta[2]);
			AiMsgInfo("\tk (R,G,B)\t= (%f,%f,%f)",data->k[0],data->k[1],data->k[2]);
			AiMsgInfo("--------------------------------------");
		}
		
		lambdas.clear();
		etas.clear();
		ks.clear();
	}
	else // use RGB params
	{
		data->eta[0] = params[p_iorRGB].RGB.r; data->eta[1] = params[p_iorRGB].RGB.g; data->eta[2] = params[p_iorRGB].RGB.b;
		data->k[0] = params[p_kRGB].RGB.r; data->k[1] = params[p_kRGB].RGB.g; data->k[2] = params[p_kRGB].RGB.b;
	}

	// If the file or strings was bad, don't use the fullSpectrum mode -> using IOR (1,1,1,0,0,0)
	if(!fileOK)
		data->useFullSpectrum = false;

	//-------
	// LUT
	if(data->LUT != NULL)
	{
		//delete data->LUT;
		AiFree(data->LUT);
		data->LUT = NULL;
	}

	bool ratioFsFp_isTextured = AiNodeIsLinked(node,"ratioFsFp");
	bool roughness_isTextured = AiNodeIsLinked(node,"roughness");

	// Make sure nothing is plugged into the IOR
	data->useLUT = params[p_useLUT].BOOL;
	if(data->useLUT && (AiNodeIsLinked(node,"iorRGB") || AiNodeIsLinked(node,"kRGB") || ratioFsFp_isTextured ||  AiNodeIsLinked(node,"iorInRGB")))
	{
		data->useLUT = false;
		AiMsgWarning("Obq_Fresnel : A parameter used for LUT calculation has a linked texture, cannot use LUT. Reverting to non-LUT mode.");
	}

	if(data->useLUT) // TODO should only be like this
	{

		float LUTSampleSize = params[p_LUTSampleSize].FLT;
		data->LUTResM1 = (LUTSampleSize==0.0f?0.0f: std::ceil(90.0f/LUTSampleSize));
		data->LUTRes = static_cast<int>(data->LUTResM1 + 1);
		data->LUTSampleSize = (data->LUTRes==1?90.0f:90.0f/data->LUTResM1);
		
		float roughnessSampleSize = params[p_roughnessSampleSize].FLT;
		data->roughnessResM1 = ((roughnessSampleSize==0.0f || !roughness_isTextured)?0.0f:std::ceil(1.0f/roughnessSampleSize));
		data->roughnessRes = static_cast<int>(data->roughnessResM1)+1;
		data->roughnessSampleSize = (data->roughnessRes==1?1.0f:1.0f/data->roughnessResM1);


		// Memory allocation
		if(data->method == METALS)
			data->LUT = (AtColor*) AiMalloc(data->roughnessRes * data->LUTRes * sizeof(AtColor));
		else
			data->LUT = (AtColor*) AiMalloc(data->roughnessRes * 2 * data->LUTRes * sizeof(AtColor));
		


		// Calculate and store in array
		double lutres = data->LUTResM1;
		double ratioFsFp = params[p_ratioFsFp].FLT;
		double ratioFpFs = (1.0-ratioFsFp);
		
		if(!data->useFullSpectrum)
		{
			if(data->method == METALS)
			{
				for(int i = 0; i < data->LUTRes; i++)
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

					// For compatibility
					if(data->degamma)
						degamma(colorFs);

					if(data->transfer2sRGB)
						linear2sRGB(colorFs);

					// Out color
					data->LUT[i].r = static_cast<float>(minimax(0.0,colorFs[0],1.0));
					data->LUT[i].g = static_cast<float>(minimax(0.0,colorFs[1],1.0));
					data->LUT[i].b = static_cast<float>(minimax(0.0,colorFs[2],1.0));

					// Test if color is corrupted
					if(AiColorCorrupted(data->LUT[i]))
						data->LUT[i] = AI_RGB_WHITE;
				}

			}
			else // REFRACTIVE
			{

				double colorNsIN[] = {params[p_iorInRGB].RGB.r,params[p_iorInRGB].RGB.g, params[p_iorInRGB].RGB.b};
				double colorKs[] = {0.0f,0.0f,0.0f};

				for(int i = 0; i < data->LUTRes; i++)
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
							index += data->LUTRes;
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

						if(data->transfer2sRGB)
							linear2sRGB(colorFs);

						// Out color
						data->LUT[index].r = static_cast<float>(minimax(0.0,colorFs[0],1.0));
						data->LUT[index].g = static_cast<float>(minimax(0.0,colorFs[1],1.0));
						data->LUT[index].b = static_cast<float>(minimax(0.0,colorFs[2],1.0));

						// Test if color is corrupted
						if(AiColorCorrupted(data->LUT[index]))
							data->LUT[index] = AI_RGB_WHITE;
					}
				}

			}
		}
		else // Full spectrum for metal
		{

			for(int i = 0; i < data->LUTRes; i++)
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

				Spectrum s = Spectrum::FromSampled(data->lambdas, fs, data->lambdasSize,data->xyz2rgb);
				delete[] fs;

				s.ToRGB(colorFs);

				// degamma 2.2 DEPRICATED
				if(data->degamma)
						degamma(colorFs);
				if(data->transfer2sRGB)
					linear2sRGB(colorFs);


				// Out color
				data->LUT[i].r = static_cast<float>(minimax(0.0,colorFs[0],1.0));
				data->LUT[i].g = static_cast<float>(minimax(0.0,colorFs[1],1.0));
				data->LUT[i].b = static_cast<float>(minimax(0.0,colorFs[2],1.0));

				// Test if color is corrupted
				if(AiColorCorrupted(data->LUT[i]))
					data->LUT[i] = AI_RGB_WHITE;
			}
		}

		//-----------
		// ROUGHNESS
		if(!roughness_isTextured) // single value
		{
			float alpha = params[p_roughness].FLT;
			if(alpha!=0.0f) // else it's all done
			{
				float alpha2 = alpha*alpha;
				float alpha2overPi = alpha2/c_Pi;
				float alpha2minus1 = alpha2-1.0f;

				float* weights = new float[data->LUTRes];
				AtColor* colors = new AtColor[(data->method==REFRACTIVE?2*data->LUTRes:data->LUTRes)];

				// calculate weight
				for(int i = 0; i < data->LUTRes; i++)
				{
					float theta = static_cast<float>(i)/data->LUTResM1*c_PiOver2;
					float costheta = std::cos(theta);
					float denom = 1.0f+alpha2minus1*costheta*costheta;

					weights[i] = (denom==0.0f?1.0f:alpha2overPi/(denom*denom));
				}

				//calculate color
				for(int i = 0; i < data->LUTRes; i++)
				{
					AtColor c = AI_RGB_BLACK;
					AtColor c2 = AI_RGB_BLACK;// refractive
					float w = 0.0f;
					for(int j = 0; j < data->LUTRes; j++)
					{
						int index_diff = std::abs(i-j);
						w += weights[index_diff];
						c += weights[index_diff] * data->LUT[j];
						if(data->method == REFRACTIVE)
							c2 += weights[index_diff] * data->LUT[data->LUTRes+j];
					}
					if(w == 0.0f)
						w = 1.0f;
					colors[i] = c/w;
					if(data->method == REFRACTIVE)
						colors[data->LUTRes + i] = c2/w;
				}

				//switch color
				int roughlutres = data->LUTRes;
				if(data->method == REFRACTIVE)
					roughlutres*=2;

				for(int i = 0; i < roughlutres; i++)
					data->LUT[i] = colors[i];

				delete[] weights;
				delete[] colors;
			}
		}
		else if(data->roughnessRes != 1)	// must do roughness precalc GGX
		{

			float* weights = new float[data->LUTRes];
			for(int a = 1; a < data->roughnessRes; a++)
			{
				float alpha = static_cast<float>(a)/data->roughnessResM1;
				float alpha2 = alpha*alpha;
				float alpha2overPi = alpha2/c_Pi;
				float alpha2minus1 = alpha2-1.0f;

				// reset
				for(int i = 0; i < data->LUTRes; i++)
					weights[i] = 0.0f;

				// calculate weight
				for(int i = 0; i < data->LUTRes; i++)
				{
					float theta = static_cast<float>(i)/data->LUTResM1*c_PiOver2;
					float costheta = std::cos(theta);
					float denom = 1.0f+alpha2minus1*costheta*costheta;

					weights[i] = (denom==0.0f?1.0f:alpha2overPi/(denom*denom));
				}

				for(int i = 0; i < data->LUTRes; i++)
				{
					AtColor c = AI_RGB_BLACK;
					AtColor c2 = AI_RGB_BLACK; // refractive
					float w = 0.0f;
					for(int j = 0; j < data->LUTRes; j++)
					{
						int index_diff = std::abs(i-j);
						w += weights[index_diff];
						c += weights[index_diff]*data->LUT[j];
						if(data->method == REFRACTIVE)
							c2 += weights[index_diff] * data->LUT[data->LUTRes+j];
					}
					if(w == 0.0f)
						w = 1.0f;
					
					if(data->method == REFRACTIVE)
					{
						data->LUT[a*2*data->LUTRes + i] = c/w;
						data->LUT[a*2*data->LUTRes + data->LUTRes + i] = c2/w;
					}
					else
						data->LUT[a*data->LUTRes + i] = c/w;
				}
			}

			delete[] weights;
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
	double s_ndotv = CLAMP(AiV3Dot(s_normal, sg->Rd),-1.0f,1.0f);
	bool backface = (s_ndotv >= 0.0);
	s_ndotv = std::max(c_EpsilonF__d,abs(s_ndotv));
	
	if(backface)
		s_normal = -s_normal;


	int backfaceMode = AiShaderEvalParamInt(p_backfaceMode);

	int method = data->method;
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
		
		float index_f = static_cast<float>(minimax(0.0,theta/c_PiOver2,1.0))*data->LUTResM1;

		// No Roughness
		if(data->roughnessRes == 1)
		{
			if(method == REFRACTIVE && backfaceMode == BACKFACE_SWAP && backface)
				index_f += data->LUTRes;

			int index_floor = static_cast<int>(floor(index_f));
			int index_ceil = static_cast<int>(ceil(index_f));
			float t = index_f - static_cast<float>(index_floor);

			sg->out.RGB = ((1.0f-t)*data->LUT[index_floor] + t*data->LUT[index_ceil]);
		}
		else // Roughness
		{
			int lutres = data->LUTRes;

			if(method == REFRACTIVE)
			{
				lutres*=2;
				if( backfaceMode == BACKFACE_SWAP && backface)
					index_f += data->LUTRes;
			}

			float rough = CLAMP(AiShaderEvalParamFlt(p_roughness),0.0f,1.0f)*static_cast<float>(data->roughnessResM1);
			int rough_f = static_cast<int>(std::floor(rough));
			int rough_c = static_cast<int>(std::ceil(rough));
			float rough_t = rough - static_cast<float>(rough_f);

			int index_floor = static_cast<int>(floor(index_f));
			int index_ceil = static_cast<int>(ceil(index_f));
			float t = index_f - static_cast<float>(index_floor);

			int roughedIndex_ff = rough_f*lutres + index_floor;
			int roughedIndex_fc = rough_f*lutres + index_ceil;
			int roughedIndex_cf = rough_c*lutres + index_floor;
			int roughedIndex_cc = rough_c*lutres + index_ceil;

			AtColor value_ff = data->LUT[roughedIndex_ff];
			AtColor value_fc = data->LUT[roughedIndex_fc];
			AtColor value_cf = data->LUT[roughedIndex_cf];
			AtColor value_cc = data->LUT[roughedIndex_cc];

			sg->out.RGB = ((1.0f-rough_t)*((1.0f-t)*value_ff + t*value_fc) + rough_t*((1.0f-t)*value_cf + t*value_cc));
		}
	}
	else if(data->useFullSpectrum) // metal
	{
		double ratioFsFp = double(AiShaderEvalParamFlt(p_ratioFsFp));
		double ratioFpFs = (1.0-ratioFsFp);
		double roughness = AiShaderEvalParamFlt(p_roughness);
		double costheta = s_ndotv*(1.0 - roughness) + roughness*0.3333333; // Cheap/poor approximation

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

		Spectrum s = Spectrum::FromSampled(data->lambdas, fs, data->lambdasSize,data->xyz2rgb);
		
		s.ToRGB(colorFs);
		delete[] fs;

		// degamma 2.2 DEPRICATED
		if(data->degamma)
			degamma(colorFs);
					
		if(data->transfer2sRGB)
			linear2sRGB(colorFs);
		
		sg->out.RGB.r = static_cast<float>(minimax(0.0,colorFs[0],1.0));
		sg->out.RGB.g = static_cast<float>(minimax(0.0,colorFs[1],1.0));
		sg->out.RGB.b = static_cast<float>(minimax(0.0,colorFs[2],1.0));
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
		double roughness = AiShaderEvalParamFlt(p_roughness);
		double costheta = s_ndotv*(1.0 - roughness) + roughness*0.3333333; // Cheap/poor approximation

		double colorNs[] = {iorRGB.r,iorRGB.g,iorRGB.b};
		double colorNs1[] = {iorInRGB.r,iorInRGB.g,iorInRGB.b};
		double colorKs[] = {kRGB.r,kRGB.g,kRGB.b};
		double colorFs[] = {0.0,0.0,0.0};

		// Thanks to PBRT
		switch(method){ 
			
			// Fresnel
		case REFRACTIVE :
			{
				double theta = acos(costheta);
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
				if(data->transfer2sRGB)
					linear2sRGB(colorFs);
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
				// degamma 2.2 DEPRICATED
				if(data->degamma)
					degamma(colorFs);
				if(data->transfer2sRGB)
					linear2sRGB(colorFs);

				break;
			}
		}

		// Out color
		sg->out.RGB.r = float(minimax(0.0,colorFs[0],1.0));
		sg->out.RGB.g = float(minimax(0.0,colorFs[1],1.0));
		sg->out.RGB.b = float(minimax(0.0,colorFs[2],1.0));


		// Test if color is corrupted
		if(AiColorCorrupted(sg->out.RGB))
			sg->out.RGB = AI_RGB_BLACK;

	}

	// Color adjustment for minor tweak
	if( data->ccMode != CC_OFF)
	{
		// HSV
		float hueShift = AiShaderEvalParamFlt(p_hueShift);
		float saturationMod = AiShaderEvalParamFlt(p_saturationMod);
		switch(data->ccMode)
		{
		case CC_HSV:	
			{
				AtColor hsv;
				RGBtoHSV(sg->out.RGB, hsv);

				// H
				hsv.r += hueShift; // [0,360]

				// S
				switch(data->satOp)
				{
				case CC_MUL:
					hsv.g *= saturationMod;
					break;
				case CC_ADD:
					hsv.g += saturationMod;
					break;
				case CC_POW:
				default:
					hsv.g = std::pow(hsv.g,saturationMod);
					break;
				}
				

				hsv.g = CLAMP(hsv.g, 0.0f, 1.0f);

				HSVtoRGB(hsv,sg->out.RGB);
				break;
			}
		case CC_HLS:	// same as arnold CC
		default:
			{
				AtColor hls;
				RGBtoHLS(sg->out.RGB, hls);
				// H
				hls.r += hueShift/360.0f; // [0, 1]

				// S
				switch(data->satOp)
				{
				case CC_MUL:
					hls.b *= saturationMod;
					break;
				case CC_ADD:
					hls.b += saturationMod;
					break;
				case CC_POW:
				default:
					hls.b = std::pow(hls.b,saturationMod);
					break;
				}
				hls.b = CLAMP(hls.b, 0.0f, 1.0f);

				HLStoRGB(hls,sg->out.RGB);
				break;
			}
		}
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
//   node->methods      = ObqFresnelMethods;
//   node->output_type  = AI_TYPE_RGB;
//   node->name         = "Obq_Fresnel";
//   node->node_type    = AI_NODE_SHADER;
//   strcpy(node->version, AI_VERSION);
//   return TRUE;
//}
