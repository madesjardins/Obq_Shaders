/*
Obq_MeasuredMaterial.h :

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
#ifndef OBQMEASUREDMATERIAL_H
#define OBQMEASUREDMATERIAL_H

#include "O_ISBrdf.h"

#define USELUMINANCE true

inline void spherical2cartesian(AtVector& out, const glm::dvec2& thetaPhi){

	double sintheta = std::sin(thetaPhi[0]);
	out.x = static_cast<float>(std::cos(thetaPhi[1])*sintheta);
	out.y = static_cast<float>(std::cos(thetaPhi[0]));
	out.z = static_cast<float>(std::sin(thetaPhi[1])*sintheta);
}

inline void uniformDirOnHemisphere(AtVector& out, double rnd[2])
{
	spherical2cartesian(out,glm::dvec2(std::acos(rnd[0]),rnd[1]*c_2Pi));	
}

inline void uniformDirOnHemisphere(AtVector& out, float rnd[2])
{
	spherical2cartesian(out,glm::dvec2(std::acos(rnd[0]),rnd[1]*c_2Pi));	
}

class MeasuredBRDF{

public:
	MeasuredBRDF(ISBrdf* _measured = NULL, AtVector _minusRd = AI_V3_Y, AtVector _U = AI_V3_X, AtVector _V = AI_V3_Y, AtVector _W = AI_V3_Z, AtVector _Ng = AI_V3_Y, AtUInt32 _binMaxDif = 13,bool _useJitters = false):
	  U(_U), V(_V), W(_W),
		  Ng(_Ng),
		  minusRd(_minusRd),
		  measured(_measured),
		  binMaxDif(_binMaxDif),
		  useJitters(_useJitters)
	  {
		  currentPdf = new float;
		  currentColor = new AtColor;
		  currentStep = new short;
	  }
	  ~MeasuredBRDF()
	  {
		  delete currentPdf; 
		  delete currentColor;
		  delete currentStep;
	  }
	  AtVector U,V,W,Ng,minusRd;
	  AtUInt32 currentIndex;
	  ISBrdf* measured;
	  AtUInt32 binMaxDif;
	  bool useJitters;
	  float* currentPdf;
	  AtColor* currentColor;
	  bool print;
	  short* currentStep;

};

static AtVector MeasuredBRDFEvalSample(const void *brdf, float rx, float ry)
{
	const MeasuredBRDF *measured = (const MeasuredBRDF*)brdf;
	ISBrdf* isBrdf = measured->measured;
	AtVector omega;
	AtColor c = AI_RGB_BLACK;
	float pdf = 1.0f;
	
	// binary search the cdf to get sample
	isBrdf->getSample(c,omega,pdf,measured->minusRd,measured->U,measured->V,measured->W,rx,measured->binMaxDif,measured->useJitters);

	if (AiV3Dot(omega, measured->Ng) < 0 )
	{
		(*(measured->currentStep))  = 0;
		return AI_V3_ZERO;
	}
	else // set the values
	{
		(*(measured->currentColor)) = c;
		(*(measured->currentPdf))   = pdf;
		(*(measured->currentStep))  = 1;
		return omega;
	}
}

static AtColor MeasuredBRDFEvalBrdf(const void *brdf, const AtVector *indir)
{
	const MeasuredBRDF *measured = (const MeasuredBRDF*)brdf;
	ISBrdf* isBrdf = measured->measured;

	// Not seen test
	if (AiV3Dot(*indir, measured->Ng) < 0 )
	{
		(*(measured->currentStep)) = 0;
		return AI_RGB_BLACK;
	}

	// BRDF sample
	if(*measured->currentStep == 1)
	{
		(*(measured->currentStep)) = 2;
		
		AtColor c = *measured->currentColor;

		return c*AiV3Dot(measured->V,*indir);
	}
	else // Light sample
	{
		AtVector coords;
		cartesians2sphericalDeltaCoords(coords,*indir,measured->minusRd,measured->V);
		AtColor c = isBrdf->getDataInterpolated(coords[0],coords[1],coords[2]);
		float pdf = static_cast<float>(isBrdf->getPdfValueOfColor(c)/isBrdf->getSumPdfs(coords[0]));
		
		(*(measured->currentStep))  = 2;
		(*(measured->currentColor)) = c;
		(*(measured->currentPdf))   = pdf;

		return c*AiV3Dot(measured->V,*indir);
	}

}

static float MeasuredBRDFEvalPdf(const void *brdf, const AtVector *indir)
{
	const MeasuredBRDF *measured = (const MeasuredBRDF*)brdf;
	ISBrdf* isBrdf = measured->measured;

	if(AiV3Dot(*indir, measured->Ng) < 0 )
	{
		(*(measured->currentStep)) = 0;
		return 0.0f;
	}

	if((*(measured->currentStep)) == 2) // there's a pdf value waiting
	{
		(*(measured->currentStep)) = 0;
		return (*(measured->currentPdf));
	}
	else // should never go here
	{
		AiMsgError("Obq_MeasuredMaterial !E! : Something went wring during the MIS evaluation");
		
		(*(measured->currentStep)) = 0;
		AtVector coords;
		cartesians2sphericalDeltaCoords(coords,*indir,measured->minusRd,measured->V);
		AtColor c = isBrdf->getDataInterpolated(coords[0],coords[1],coords[2]);
		float pdf = static_cast<float>(isBrdf->getPdfValueOfColor(c)/isBrdf->getSumPdfs(coords[0]));

		return pdf;
	}
}


#endif