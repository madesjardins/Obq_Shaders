
/*
Obq_UVRemapLensDistortion :

Just plug this node into the arnold_uv_remap_persp_camera to generate
distortion maps on the fly. 3DEqualizer distortion models uses headers
from the Science-D-Visions LDPK.

*------------------------------------------------------------------------
Copyright (c) 2012-2014 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)
LDPK : (C) 2013 - Science-D-Visions. Current version: 1.7
PFBarrel  : Copyright (C) 2012 The Pixel Farm Ltd.  Current version: 1.3

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

/*
MUST DO with newer ldpk in public : 
	In tde4_ldp public:
		bool setParameterValue2(const char *identifier,double v){return setParameterValue(identifier, v);}
		bool initializeParameters2(){return initializeParameters();}
		bool undistort2(double x0,double y0,double &x1,double &y1){return undistort(x0,y0,x1,y1);}
		//std::ostream& info(std::ostream& stream) const {return _.out(stream);}
	ldpk_plugin_loader
		comment the code but not include in ldpk_plugin_loader
	Where needed:
		+ #define M_PI 3.14159265358979
		+ change cout et cerr pour AiMsgInfo et AiMsgError
*/

#ifndef OBQUVREMAPLENSDISTORTION_H
#define OBQUVREMAPLENSDISTORTION_H

#include <glm/glm.hpp>
#include "Obq_Common.h"

/////////////////
//        PFTRACK
inline glm::dvec2 calcAbsCent(float x, int w, float y, int h) 
{ 
	return glm::dvec2(x*w, y*h); 
}

inline glm::dvec2 calcNorm(int w, int h) 
{ 
	double n= sqrt(2.0)/sqrt((double)(w*w+h*h)); 
	return glm::dvec2(n, 1.0/n); 
}

inline glm::dvec2 solveDistort(const glm::dvec2 &C3C5, const glm::dvec2 &absCent, double squeeze, double invSqueeze, const glm::dvec2 &norm, const glm::dvec2 &p)
{
  glm::dvec2 cent= absCent*norm.x;
  glm::dvec2 x= p*norm.x;

  // remove anamorphic squeeze
  cent.y *= invSqueeze;
  x.y *= invSqueeze;

  // distort
  const double px= x.x-cent.x;
  const double py= x.y-cent.y;

  const double px2= px*px;
  const double py2= py*py;
  const double r= sqrt(px2+py2);
  const double r2= r*r;
#ifdef THREE_POWER
  const double dr_r= r2*r*(C3C5.x+r2*C3C5.y);
#else
  const double dr_r= r2*(C3C5.x+r2*C3C5.y);
#endif

  // re-apply squeeze and remove normalization
  return glm::dvec2(x.x+px*dr_r, (x.y+py*dr_r)*squeeze)*norm.y;
}

/////////////////
//        NUKE

// NUKE Distortion model
//
// Use the Nuke distortion model to distort undistorted camera rays
//
// @param out			The result will be put here
// @param x				The x coordinate [-1,1]
// @param y				The x coordinate [-1/aspectRatio,1/aspectRatio]
// @param k1			The first distortion coefficient
// @param k2			The second distortion coefficient
// @param centerX		The relative center offset in X
// @param centerY		The relative center offset in Y
// @param anaSq			The anamorphic squeeze
// @param asyX			The asymetric distortion coefficient in X
// @param asyY			The asymetric distortion coefficient in X
// @param aspectRatio	The aspect ratio of the frame
//
void distortNuke(AtPoint* out, double x, double y, 
	double k1, double k2, 
	double centerX, double centerY, 
	double anaSq, double asyX, double asyY, 
	double aspectRatio)
{

	// Aspect ratio
	if(aspectRatio < 1.0)
	{
		centerX/=aspectRatio;
		centerY/=aspectRatio;
		
		// Decentering
		x-=centerX;
		y-=centerY;

		// Normalization compensation
		x*=aspectRatio;
		y*=aspectRatio;
	}
	else
	{
		x-=centerX;
		y-=centerY;
	}

	// Radius
	const double x2 = x*x;
	const double y2 = y*y;
	const double r2 = x2+y2;
	const double r4 = r2*r2;

	// Quick Test
	if(anaSq==0.0f) 
		anaSq = 0.001;

	// Anamorphic squeeze
	asyX/=anaSq;
	const double k1x = k1/anaSq;
	const double k2x = k2/anaSq;

	// Distortion
	x /= (1.0 + k1x*r2 + asyX*y2 + k2x*r4);
	y /= (1.0 + k1*r2 + asyY*x2 + k2*r4);
	
	// Aspect Ratio
	if(aspectRatio < 1.0)
	{
		x/=aspectRatio;
		y/=aspectRatio;
	}

	// Re-center
	// Out
	out->x = float(x+centerX);
	out->y = float(y+centerY);
}

#endif //OBQUVREMAPLENSDISTORTION_H