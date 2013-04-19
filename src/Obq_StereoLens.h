/*
Obq_StereoLens.h	 v2.06.0a (win64 - SItoA 2.6.0 - Arnold 4.0.11.0) :

Obq_StereoLens is a lens shader that renders both left and right cameras 
of a stereo rig in the same frame, saving you one scene loading time for
each pair of stereo frame. This shader is applied on the center camera.

*------------------------------------------------------------------------
Copyright (c) 2012 Marc-Antoine Desjardins, ObliqueFX (madesjardins@obliquefx.com)

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

#include <ai.h>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string> 

// Arnold thingy
// 
AI_CAMERA_NODE_EXPORT_METHODS(Obq_StereoLensMethods);

// Param enum for fast direct access
//
enum Obq_StereoLensParams { p_viewMode, p_leftCamera, p_rightCamera, p_filmbackX, p_leftCenterOffset, p_rightCenterOffset, p_dof, 
	p_useRigDof, p_rigDepth, p_useCameraInterest, p_cameraInterest, p_interestX, p_interestY, p_interestZ, p_recalculateDistanceForSideCameras, p_focalPlaneIsPlane, p_totalOverscanPixels, p_fov};

// View Mode enum
//
enum ObqViewMode{THISCAMERA,LEFTCAMERA, RIGHTCAMERA, STEREOLR,STEREODU};


// Shader Data Structure
//
typedef struct 
{
	bool useDof;
	float aspect;
	float width;
	float height;
	float pixelRatio;
	float leftCenterOffset;
	float rightCenterOffset;
	float tan_myFov;
	float focalDistanceC;
	float focalDistanceL;
	float focalDistanceR;
	float apertureSize;
	float apertureAspectRatio;
	int apertureBlades;
	float apertureRotation;
	float apertureBladeCurvature;
	float overscanRatio;
	AtNode* leftCamera;
	AtNode* rightCamera;
	AtNode* centerCamera;
	AtNode* cameraInterest;
	AtMatrix center2leftCameraMatrix;
	AtMatrix center2rightCameraMatrix;
}
ShaderData;


// Transform uv [0,1)^2 coordinates in a disk coordinates
//
// SHIRLEY, Peter AND CHIU, Kenneth. "A Low Distortion Map Between Disk and Square"
//
// @param u					The x [0,1) coordinate.
// @param v					The y [0,1) coordinate.
// @param nBlades			The number of blades for polygonal DoF. ( 3 -> triangle specular, etc.)
// @param bladeCurvature	The 1 = perfectly round, 0 = sharp, <0 = star shape.
// @param rotation			The rotation in degree of the aperture shape.
// @param dx				The output mapped x coordinate [-1,1].
// @param dy				The output mapped y coordinate.[-1,1].
//
inline void ConcentricSampleDisk(float u, float v, int nBlades, float bladeCurvature, float rotation, float *dx, float *dy )
{
	double phi, r;
	double a = 2.0*u-1.0; // (a,b) is now on [-1,1]^2
	double b = 2.0*v-1.0;
	if (a > -b) // region 1 or 2
	{
		if (a > b) // region 1, also |a| > |b|
		{
			r = a;
			phi = (AI_PI/4.0 ) * (b/a);
		}
		else // region 2, also |b| > |a|
		{
			r = b;
			phi = (AI_PI/4.0) * (2.0 - (a/b));
		}
	}
	else // region 3 or 4
	{
		if (a < b) // region 3, also |a| >= |b|, a != 0
		{
			r = -a;
			phi = (AI_PI/4.0) * (4.0 + (b/a));
		}
		else // region 4, |b| >= |a|, but a==0 and b==0 could occur.
		{
			r = -b;
			if (b != 0.0)
				phi = (AI_PI/4.0) * (6.0 - (a/b));
			else
				phi = 0.0;
		}
	}

	// polygonal blade if needed
	if(nBlades > 0 && bladeCurvature != 1.0f)
	{
		// what are floor and ceil of the phi
		rotation = static_cast<float>(AI_DTOR*rotation);
		float angleArc = static_cast<float>(AI_PITIMES2 / nBlades);
		float t = static_cast<float>((phi - rotation)/angleArc);
		float tf = std::floor(t);
		float tc = std::ceil(t);
		
		// t entre 0 et 1
		t-=tf;
		
		// what are those 2 closest points
		float pf[2] = {	static_cast<float>(r*std::cos(angleArc*tf+rotation)), 
						static_cast<float>(r*std::sin(angleArc*tf+rotation))};

		float pc[2] = {	static_cast<float>(r*std::cos(angleArc*tc+rotation)), 
						static_cast<float>(r*std::sin(angleArc*tc+rotation))};

		// where are we on that line at 0
		float p0[2] = {	pc[0]*t + pf[0]*(1.0f-t) , 
						pc[1]*t + pf[1]*(1.0f-t)};

		// positive blade curvature
		if(bladeCurvature == 0.0f)
		{
			*dx = p0[0];
			*dy = p0[1];
		}
		else if(bladeCurvature > 0.0f )
		{
			*dx = bladeCurvature*float(r * std::cos( phi )) + (1.0f-bladeCurvature)*p0[0];
			*dy = bladeCurvature*float(r * std::sin( phi )) + (1.0f-bladeCurvature)*p0[1];
		}
		else
		{
			// Positive
			bladeCurvature = -bladeCurvature;

			// at 0 we are at p0, at -1 we are here
			float pm1[2] = {0.0f,0.0f};

			if(t < 0.5f)	// on the floor line
			{
				pm1[0] = (2.0f*(0.5f-t))*pf[0];
				pm1[1] = (2.0f*(0.5f-t))*pf[1];
			}
			else if(t > 0.5) // on the ceil line
			{
				pm1[0] = (2.0f*(t-0.5f))*pc[0];
				pm1[1] = (2.0f*(t-0.5f))*pc[1];
			}

			*dx = pm1[0]*bladeCurvature+(1.0f-bladeCurvature)*p0[0];
			*dy = pm1[1]*bladeCurvature+(1.0f-bladeCurvature)*p0[1];

			/* //GOOD but different
			bladeCurvature = 1.0f/(1.0f-bladeCurvature);

			// where would we be at -1
			float pm1[2] = {0.0f,0.0f};

			if(t < 0.5f)	// on the floor line
			{
				pm1[0] = (2.0f*(0.5f-t))*pf[0];
				pm1[1] = (2.0f*(0.5f-t))*pf[1];
			}
			else if(t > 0.5) // on the ceil line
			{
				pm1[0] = (2.0f*(t-0.5f))*pc[0];
				pm1[1] = (2.0f*(t-0.5f))*pc[1];
			}

			// value
			*dx = p0[0]*bladeCurvature+(1.0f-bladeCurvature)*pm1[0];
			*dy = p0[1]*bladeCurvature+(1.0f-bladeCurvature)*pm1[1];
			*/
		}
	}
	else
	{
		*dx = float(r * std::cos( phi ));
		*dy = float(r * std::sin( phi ));
	}
}
