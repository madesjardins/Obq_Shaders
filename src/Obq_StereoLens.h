/*
Obq_StereoLens.h :

Obq_StereoLens is a lens shader that renders both left and right cameras 
of a stereo rig in the same frame, saving you one scene loading time for
each pair of stereo frame. This shader is applied on the center camera.

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

#include "O_Common.h"

// Arnold thingy
// 
AI_CAMERA_NODE_EXPORT_METHODS(ObqStereoLensMethods);

// Param enum for fast direct access
//
enum Obq_StereoLensParams { p_viewMode, p_leftCamera, p_rightCamera, p_filmbackX, p_leftCenterOffset, p_rightCenterOffset, 
	p_recalculateDistanceForSideCameras, p_totalOverscanPixels, p_useDof, p_focusDistance, p_apertureSize, p_apertureAspectRatio, p_usePolygonalAperture, p_apertureBlades, p_apertureBladeCurvature, p_apertureRotation, p_focusPlaneIsPlane,p_bokehInvert, p_bokehBias, p_bokehGain, p_fov};

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
	bool bokehInvert;
	float bokehBias;
	float bokehGain;
	float overscanRatio;
	AtNode* leftCamera;
	AtNode* rightCamera;
	AtNode* centerCamera;
	AtMatrix center2leftCameraMatrix;
	AtMatrix center2rightCameraMatrix;

	float focusDistance;
	bool focusPlaneIsPlane;
	bool usePolygonalAperture;

	int viewMode;

}
ShaderData;



