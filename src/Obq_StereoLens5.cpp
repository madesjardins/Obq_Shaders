/*
Obq_StereoLens5.cpp :

Obq_StereoLens is a lens shader that renders both left and right cameras 
of a stereo rig in the same frame, saving you one scene loading time for
each pair of stereo frame. This shader is applied on the center camera.

*------------------------------------------------------------------------
Copyright (c) 2012-2017 Marc-Antoine Desjardins, ObliqueFX (marcantoinedesjardins@gmail.com)

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

#include "Obq_StereoLens5.h"

// Arnold thingy
// 
AI_CAMERA_NODE_EXPORT_METHODS(ObqStereoLensMethods);

// ENUM MENU
static const char* ObqViewModeNames[] = 
{
	"Center Camera",
    "Left Camera",
    "Right Camera",
    "Stereo Camera <Side-by-Side>",
    "Stereo Camera <Over-Under>",
    NULL
};

// Parameters
//
node_parameters
{
	AiParameterEnum("viewMode1" , THISCAMERA, ObqViewModeNames);
	AiParameterStr("leftCamera" , "Camera_L");
	AiParameterStr("rightCamera" , "Camera_R");
	AiParameterFlt("filmbackX" , 0.0f);
	AiParameterFlt("leftCenterOffset" , 0.0f);
	AiParameterFlt("rightCenterOffset" , 0.0f);
	AiParameterBool("recalculateDistanceForSideCameras", true);
	AiParameterInt("totalOverscanPixels",0);
	
	AiParameterBool("useDof", false);
	AiParameterFlt("focusDistance", 100.0f);
	AiParameterFlt("apertureSize", 0.1f);
	AiParameterFlt("apertureAspectRatio", 1.0f);
	AiParameterBool("usePolygonalAperture", true);
	AiParameterInt("apertureBlades", 5);
	AiParameterFlt("apertureBladeCurvature", 0.0f);
	AiParameterFlt("apertureRotation", 0.0f);
	AiParameterBool("focusPlaneIsPlane", true);
	AiParameterBool("bokehInvert", false);
	AiParameterFlt("bokehBias", 0.5f);
	AiParameterFlt("bokehGain", 0.5f);

	AiParameterFlt("fov", 60.0f);

	AiMetaDataSetBool(nentry, NULL, "is_perspective", true);
	
}


node_initialize
{
	MyCameraData *data = new MyCameraData();
	data->aspect = 1.0f;

	// Initialize
	data->aspect = 1.0f;
	data->width = 1920.0f;
	data->height = 1080.0f;
	data->pixelRatio = 1.0f;
	data->leftCamera = NULL;
	data->rightCamera = NULL;
	data->centerCamera = NULL;
	data->leftCenterOffset = 0.0f;
	data->rightCenterOffset = 0.0f;
	data->tan_fov = 1.0f;
	data->useDof = false;
	data->apertureSize = 0.1f;
	data->apertureAspectRatio = 1.0f;
	data->apertureBlades = 5;
	data->apertureBladeCurvature = 0.0f;
	data->apertureRotation = 0.0f;
	data->bokehInvert = false;
	data->bokehBias = 0.5f;
	data->bokehGain = 0.5f;
	data->focalDistanceC = 100.0f;
	data->focalDistanceL = 100.0f;
	data->focalDistanceR = 100.0f;
	data->overscanRatio = 1.0f;
	data->viewMode = THISCAMERA;
	data->usePolygonalAperture = true;
	data->focusDistance = 100.0f;
	data->focusPlaneIsPlane = true;

	// Update shaderData variables
	AtNode* options = AiUniverseGetOptions();
	
	// Overscan
	float w = float(AiNodeGetInt(options, "xres"));
	float h = float(AiNodeGetInt(options, "yres"));

	data->viewMode = AiNodeGetInt(node, "viewMode1");

	switch(data->viewMode)
	{
	case STEREOLR:
		data->width = w - AiNodeGetInt(node, "totalOverscanPixels");
		data->height = h;
		data->overscanRatio = w / data->width;
		break;
	case STEREODU:
		data->width = w;
		data->height = h - AiNodeGetInt(node, "totalOverscanPixels");
		data->overscanRatio = h / data->height;
		break;
	default:
		data->width = w;
		data->height = h;
		data->overscanRatio = 1.0f;
		break;
	}

	// Aspect
	data->pixelRatio = AiNodeGetFlt(options, "pixel_aspect_ratio");
	data->aspect = data->width / (data->height / data->pixelRatio);
	ObqPluginID plugin = findPluginID(node);

	// Get all 3 cameras and all 3 matrices
	AtMatrix centerCameraMatrix, leftCameraMatrix, rightCameraMatrix;
	data->centerCamera = node;

	if(plugin == SITOA)
	{
		std::string camNodeName(AiNodeGetName(node));
		std::size_t sitoaIndex = camNodeName.rfind(".SItoA.");

		data->leftCamera = AiNodeLookUpByName((std::string(AiNodeGetStr(node, "leftCamera")).append(camNodeName.substr(sitoaIndex)).c_str()));
		data->rightCamera = AiNodeLookUpByName((std::string(AiNodeGetStr(node, "rightCamera")).append(camNodeName.substr(sitoaIndex)).c_str()));
		
	}
	else
	{
		data->leftCamera = AiNodeLookUpByName(AiNodeGetStr(node, "leftCamera"));
		data->rightCamera = AiNodeLookUpByName(AiNodeGetStr(node, "rightCamera"));
	}


	if(data->centerCamera == NULL)
	{
		AiMsgError("Error with center camera... this is not normal...");
		delete data;
		return;
	}
	else if(data->leftCamera == NULL)
	{
		AiMsgError("Left camera doesn't exist. Don't forget the Model name if any. ex: Model.LeftCamera");
		delete data;
		return;
	}
	else if(data->rightCamera == NULL)
	{
		AiMsgError("Right camera doesn't exist. Don't forget the Model name if any. ex: Model.RightCamera");
		delete data;
		return;
	}
	else
	{
		centerCameraMatrix = AiNodeGetMatrix(node, "matrix");
		leftCameraMatrix = AiNodeGetMatrix(data->leftCamera, "matrix");
		rightCameraMatrix = AiNodeGetMatrix(data->rightCamera, "matrix");

		// Field of view
		float fov = AiNodeGetFlt(node, "fov"); // good default value from current camera but left or right should be 
		float aFov = AiNodeGetFlt(data->leftCamera, "fov");
		if(aFov != NULL)
		{
			fov = aFov;
		}
		else
		{
			aFov = AiNodeGetFlt(data->rightCamera, "fov"); // If left camera doesn't have one, get horizontal angle from right camera
			if(aFov != NULL)
			{
				fov = aFov;
			}
			// If neither left nor right, get fov from this camera (always good)
		}

		data->tan_fov = static_cast<float>(std::tan(fov * c_Radians__d / 2.0));
		// Rotation and Translation Matrix
		AtMatrix iCenterCameraMatrix = AiM4Invert(centerCameraMatrix);
		data->center2leftCameraMatrix = AiM4Mult(leftCameraMatrix, iCenterCameraMatrix);
		data->center2rightCameraMatrix = AiM4Mult(rightCameraMatrix, iCenterCameraMatrix);

		// Optical Center Offsets
		float filmbackX = AiNodeGetFlt(node, "filmbackX");
		data->leftCenterOffset  = (filmbackX != 0.0 ? 2.0f * AiNodeGetFlt(node, "leftCenterOffset") / filmbackX : 0.0f);
		data->rightCenterOffset = (filmbackX != 0.0 ? 2.0f * AiNodeGetFlt(node, "rightCenterOffset") / filmbackX : 0.0f);

		// DOF
		data->useDof = AiNodeGetBool(node, "useDof");
		if(data->useDof)
		{
			// DOF related values
			data->focalDistanceC = AiNodeGetFlt(node, "focusDistance");
			data->apertureSize = AiNodeGetFlt(node, "apertureSize");
			data->apertureAspectRatio = AiNodeGetFlt(node, "apertureAspectRatio");
			data->usePolygonalAperture = AiNodeGetBool(node, "usePolygonalAperture");
			data->apertureBlades = AiNodeGetInt(node, "apertureBlades");
			data->apertureBladeCurvature = AiNodeGetFlt(node, "apertureBladeCurvature");
			data->apertureRotation = AiNodeGetFlt(node, "apertureRotation");
			data->bokehInvert = AiNodeGetBool(node, "bokehInvert");
			data->bokehBias = AiNodeGetFlt(node, "bokehBias");
			data->bokehGain = 1.0f-AiNodeGetFlt(node, "bokehGain");

			if(data->apertureAspectRatio <= 0.0f)
				data->apertureAspectRatio = 0.001f;

			data->focusPlaneIsPlane = AiNodeGetBool(node, "focusPlaneIsPlane");

			// Focal Plane
			if(AiNodeGetBool(node, "recalculateDistanceForSideCameras"))
			{
				// calculate distance between center and left and between center and right
				float dLeft = std::sqrt(data->center2leftCameraMatrix[3][0] * data->center2leftCameraMatrix[3][0] + data->center2leftCameraMatrix[3][1] * data->center2leftCameraMatrix[3][1] + data->center2leftCameraMatrix[3][2] * data->center2leftCameraMatrix[3][2]);
				float dRight = std::sqrt(data->center2rightCameraMatrix[3][0] * data->center2rightCameraMatrix[3][0] + data->center2rightCameraMatrix[3][1] * data->center2rightCameraMatrix[3][1] + data->center2rightCameraMatrix[3][2] * data->center2rightCameraMatrix[3][2]);

				// Set the focal distances;
				data->focalDistanceL = std::sqrt(dLeft * dLeft + data->focalDistanceC * data->focalDistanceC);
				data->focalDistanceR = std::sqrt(dRight * dRight + data->focalDistanceC * data->focalDistanceC);
			}
			else
			{
				data->focalDistanceL = data->focalDistanceC;
				data->focalDistanceR = data->focalDistanceC;
			}
		}

		AiMsgInfo("----------------------------------------------------");
		AiMsgInfo("CameraParams : fov = %f", fov);
		AiMsgInfo("CameraParams : filmback width = %f", AiNodeGetFlt(node, "filmbackX"));
		AiMsgInfo("CameraParams : left optical center offset = %f", AiNodeGetFlt(node, "leftCenterOffset"));
		AiMsgInfo("CameraParams : right optical center offset = %f", AiNodeGetFlt(node, "rightCenterOffset"));
		AiMsgInfo("CameraParams : overscan ratio = %f", data->overscanRatio);
		AiMsgInfo("CenterToLeftCameraMatrix :");
		AiMsgInfo("%f %f %f %f", data->center2leftCameraMatrix[0][0], data->center2leftCameraMatrix[0][1], data->center2leftCameraMatrix[0][2], data->center2leftCameraMatrix[0][3]);
		AiMsgInfo("%f %f %f %f", data->center2leftCameraMatrix[1][0], data->center2leftCameraMatrix[1][1], data->center2leftCameraMatrix[1][2], data->center2leftCameraMatrix[1][3]);
		AiMsgInfo("%f %f %f %f", data->center2leftCameraMatrix[2][0], data->center2leftCameraMatrix[2][1], data->center2leftCameraMatrix[2][2], data->center2leftCameraMatrix[2][3]);
		AiMsgInfo("%f %f %f %f", data->center2leftCameraMatrix[3][0], data->center2leftCameraMatrix[3][1], data->center2leftCameraMatrix[3][2], data->center2leftCameraMatrix[3][3]);
		AiMsgInfo("CenterToRightCameraMatrix :");
		AiMsgInfo("%f %f %f %f", data->center2rightCameraMatrix[0][0], data->center2rightCameraMatrix[0][1], data->center2rightCameraMatrix[0][2], data->center2rightCameraMatrix[0][3]);
		AiMsgInfo("%f %f %f %f", data->center2rightCameraMatrix[1][0], data->center2rightCameraMatrix[1][1], data->center2rightCameraMatrix[1][2], data->center2rightCameraMatrix[1][3]);
		AiMsgInfo("%f %f %f %f", data->center2rightCameraMatrix[2][0], data->center2rightCameraMatrix[2][1], data->center2rightCameraMatrix[2][2], data->center2rightCameraMatrix[2][3]);
		AiMsgInfo("%f %f %f %f", data->center2rightCameraMatrix[3][0], data->center2rightCameraMatrix[3][1], data->center2rightCameraMatrix[3][2], data->center2rightCameraMatrix[3][3]);
		if(data->useDof)
		{
			AiMsgInfo("CameraParams : focal distance = %f", data->focalDistanceC);
			AiMsgInfo("CameraParams : focal distance from left   = %f", data->focalDistanceL);
			AiMsgInfo("CameraParams : focal distance from right  = %f", data->focalDistanceR);
			AiMsgInfo("CameraParams : aperture size = %f", data->apertureSize);
			AiMsgInfo("CameraParams : aperture ratio = %f", data->apertureAspectRatio);
			AiMsgInfo("CameraParams : use polygonal aperture = %s", (data->usePolygonalAperture ? "True" : "False"));
			AiMsgInfo("CameraParams : aperture blades = %i", data->apertureBlades);
			AiMsgInfo("CameraParams : aperture blade curvature = %f", data->apertureBladeCurvature);
			AiMsgInfo("CameraParams : aperture blade rotation = %f", data->apertureRotation);
		}
		AiMsgInfo("----------------------------------------------------");
	}

	// Set data
	AiCameraInitialize(node);
	AiNodeSetLocalData(node, data);

}

node_update
{
	// MyCameraData *data = (MyCameraData*)AiNodeGetLocalData(node);
	AiCameraUpdate(node, false);
}

node_finish
{
	MyCameraData *data = (MyCameraData*)AiNodeGetLocalData(node);
	delete(data);
}

camera_create_ray 
{

	MyCameraData *data = (MyCameraData*)AiNodeGetLocalData(node);
	// AspectRatio
	float aspect = data->aspect;
	
	// Scale derivatives
	float dsx = input.dsx * data->tan_fov;
	float dsy = input.dsy * data->tan_fov;

	// ViewMode
	switch(data->viewMode)
	{
	case THISCAMERA:
		{
			// Direction and optical center offset 
			AtVector p(input.sx, input.sy, -1.0f / data->tan_fov);
			output.dir = AiV3Normalize(p - output.origin);

			//DOF
			if(data->useDof && data->apertureSize > 0.0f){
				float lensU;
				float lensV;
				ConcentricSampleDisk(input.lensx, input.lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation, &lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
				lensU *= data->apertureSize;
				lensV *= data->apertureSize;
				float ft = (data->focusPlaneIsPlane ? std::abs(data->focalDistanceC / output.dir.z) : data->focalDistanceC);
				AtVector Pfocus = output.dir * ft;

				// Focal Aspect Ratio
				lensV *= data->apertureAspectRatio;

				output.origin.x = lensU;
				output.origin.y = lensV;
				output.origin.z = 0.0;
				output.dir = AiV3Normalize(Pfocus - output.origin);
			}

			break;
		}
	case LEFTCAMERA:
		{
			// Direction and optical center offset 
			AtVector p(input.sx + data->leftCenterOffset, input.sy, -1.0f / data->tan_fov);
			AtVector dir = AiV3Normalize(p - output.origin);
			bool outputDone = false;

			if(data->useDof && data->apertureSize > 0.0f){
				float lensU;
				float lensV;
				ConcentricSampleDisk(input.lensx, input.lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation, &lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
				lensU *= data->apertureSize;
				lensV *= data->apertureSize;

				float ft = data->focusPlaneIsPlane ? std::abs(data->focalDistanceL / dir.z) : data->focalDistanceL;
				AtVector Pfocus = dir * ft;

				// Focal Aspect Ratio
				lensV*=data->apertureAspectRatio;

				output.origin.x = lensU;
				output.origin.y = lensV;
				output.origin.z = 0.0;
				dir = AiV3Normalize(Pfocus - output.origin);
			}

			// Check if output already done
			if(!outputDone)
			{
				// Rotate
				AtVector dir2 = AiM4VectorByMatrixMult(data->center2leftCameraMatrix, dir);
				output.dir = AiV3Normalize(dir2);

				// Move origin
				const AtVector po(output.origin.x, output.origin.y, output.origin.z);
				output.origin = AiM4PointByMatrixMult(data->center2leftCameraMatrix, po);
			}
			break;
		}
	case RIGHTCAMERA:
		{
			// Direction and optical center offset 
			AtVector p(input.sx + data->rightCenterOffset, input.sy, -1.0f / data->tan_fov);
			AtVector dir = AiV3Normalize(p - output.origin);
			bool outputDone = false;

			// DOF
			if(data->useDof && data->apertureSize >0.0f){
				float lensU;
				float lensV;
				ConcentricSampleDisk(input.lensx, input.lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation, &lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
				lensU *= data->apertureSize;
				lensV *= data->apertureSize;
				float ft = data->focusPlaneIsPlane ? std::abs(data->focalDistanceR / dir.z) : data->focalDistanceR;
				AtVector Pfocus = dir * ft;

				// Focal Aspect Ratio
				lensV *= data->apertureAspectRatio;

				output.origin.x = lensU;
				output.origin.y = lensV;
				output.origin.z = 0.0;
				dir = AiV3Normalize(Pfocus - output.origin);
			}

			if(!outputDone)
			{
				// Rotate
				AtVector dir2 = AiM4VectorByMatrixMult(data->center2rightCameraMatrix, dir);
				output.dir = AiV3Normalize(dir2);

				// Move origin
				const AtVector po(output.origin.x, output.origin.y, output.origin.z);
				output.origin = AiM4PointByMatrixMult(data->center2rightCameraMatrix, po);
			}
			break;
		}
	case STEREOLR:
		{

			// Equal derivatives y is ok, x is bad
			dsx=dsy;

			// Left Camera
			if(input.sx < 0)
			{
				// Direction and optical center offset 
				AtVector p( (input.sx + 0.5f) * 2.0f * data->overscanRatio+data->leftCenterOffset, input.sy, -1.0f/data->tan_fov);
				AtVector dir = AiV3Normalize(p - output.origin);
				bool outputDone = false;

				// DOF
				if(data->useDof && data->apertureSize > 0.0f){
					float lensU;
					float lensV;
					ConcentricSampleDisk(input.lensx, input.lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation, &lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
					lensU *= data->apertureSize;
					lensV *= data->apertureSize;
					float ft = data->focusPlaneIsPlane ? std::abs(data->focalDistanceL / dir.z) : data->focalDistanceL;
					AtVector Pfocus = dir * ft;
					// Focal Aspect Ratio
					lensV *= data->apertureAspectRatio;

					output.origin.x = lensU;
					output.origin.y = lensV;
					output.origin.z = 0.0;
					dir = AiV3Normalize(Pfocus - output.origin);
				}

				if(!outputDone)
				{
					// Rotate
					AtVector dir2 =	AiM4VectorByMatrixMult(data->center2leftCameraMatrix, dir);
					output.dir = AiV3Normalize(dir2);

					// Move origin
					const AtVector po(output.origin.x, output.origin.y, output.origin.z);
					output.origin = AiM4PointByMatrixMult(data->center2leftCameraMatrix, po);
				}

				// derivatives and overscan
				//dsx*=data->overscanRatio;

			}
			else // Right camera
			{
				AtVector p((input.sx - 0.5f) * 2.0f * data->overscanRatio+data->rightCenterOffset, input.sy, -1.0f/data->tan_fov);
				AtVector dir = AiV3Normalize(p - output.origin);
				bool outputDone = false;

				// DOF
				if(data->useDof && data->apertureSize >0.0f){
					float lensU;
					float lensV;
					ConcentricSampleDisk(input.lensx, input.lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation, &lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
					lensU *= data->apertureSize;
					lensV *= data->apertureSize;
					float ft = data->focusPlaneIsPlane ? std::abs(data->focalDistanceR / dir.z) : data->focalDistanceR;
					AtVector Pfocus = dir * ft;

					// Focal Aspect Ratio
					lensV *= data->apertureAspectRatio;

					output.origin.x = lensU;
					output.origin.y = lensV;
					output.origin.z = 0.0;
					dir = AiV3Normalize(Pfocus - output.origin);
				}

				if(!outputDone)
				{
					// Rotate
					AtVector dir2 = AiM4VectorByMatrixMult(data->center2rightCameraMatrix, dir);
					output.dir = AiV3Normalize(dir2);

					// Move origin
					const AtVector po(output.origin.x, output.origin.y, output.origin.z);
					output.origin = AiM4PointByMatrixMult(data->center2rightCameraMatrix, po);
				}
			}
			break;
		}
	case STEREODU:
		{
			// Equal derivatives y is ok, x is bad
			dsy=dsx;

			// Left Camera
			if(input.sy < 0)
			{
				AtVector p(input.sx + data->leftCenterOffset, (input.sy + 0.5f / (aspect / data->overscanRatio)) * 2.0f * data->overscanRatio, -1.0f/data->tan_fov);
				AtVector dir = AiV3Normalize(p - output.origin);
				bool outputDone = false;

				// DOF
				if(data->useDof && data->apertureSize > 0.0f){
					float lensU;
					float lensV;
					ConcentricSampleDisk(input.lensx, input.lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation, &lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
					lensU *= data->apertureSize;
					lensV *= data->apertureSize;
					float ft = data->focusPlaneIsPlane ? std::abs(data->focalDistanceL / dir.z) : data->focalDistanceL;
					AtVector Pfocus = dir * ft;

					// Focal Aspect Ratio
					lensV *= data->apertureAspectRatio;

					output.origin.x = lensU;
					output.origin.y = lensV;
					output.origin.z = 0.0;
					dir = AiV3Normalize(Pfocus - output.origin);
				}

				if(!outputDone)
				{
					// Rotate
					AtVector dir2 =	AiM4VectorByMatrixMult(data->center2leftCameraMatrix, dir);
					output.dir = AiV3Normalize(dir2);

					// Move origin
					const AtVector po(output.origin.x, output.origin.y, output.origin.z);
					output.origin = AiM4PointByMatrixMult(data->center2leftCameraMatrix, po);
				}

			}
			else // Right camera
			{
				AtVector p(input.sx + data->rightCenterOffset, (input.sy - 0.5f / (aspect / data->overscanRatio)) * 2.0f * data->overscanRatio, -1.0f/data->tan_fov);
				AtVector dir = AiV3Normalize(p - output.origin);
				bool outputDone = false;

				// DOF
				if(data->useDof && data->apertureSize >0.0f){
					float lensU;
					float lensV;
					ConcentricSampleDisk(input.lensx, input.lensy, data->apertureBlades, data->apertureBladeCurvature, data->apertureRotation, &lensU, &lensV, data->bokehInvert, data->bokehBias, data->bokehGain);
					lensU *= data->apertureSize;
					lensV *= data->apertureSize;
					float ft = data->focusPlaneIsPlane ? std::abs(data->focalDistanceR / dir.z) : data->focalDistanceR;
					AtVector Pfocus = dir * ft;

					// Focal Aspect Ratio
					lensV *= data->apertureAspectRatio;

					output.origin.x = lensU;
					output.origin.y = lensV;
					output.origin.z = 0.0;
					dir = AiV3Normalize(Pfocus - output.origin);
				}

				if(!outputDone)
				{
					// Rotate
					AtVector dir2 = AiM4VectorByMatrixMult(data->center2rightCameraMatrix, dir);
					output.dir = AiV3Normalize(dir2);

					// Move origin
					const AtVector po(output.origin.x, output.origin.y, output.origin.z);
					output.origin = AiM4PointByMatrixMult(data->center2rightCameraMatrix, po);
				}

			}
			break;
		}
	default:
		AtVector p(input.sx, input.sy, -1.0f / data->tan_fov);
		output.dir = AiV3Normalize(p - output.origin);
		break;
	}

	///////////////////////////////////
	// Derivatives thanks to Alan King
	///////////////////////////////////
	AtVector d = output.dir*std::abs(((-1.0f / data->tan_fov) / output.dir.z));

	float d_dot_d = AiV3Dot(d, d);
	float temp = 1.0f / std::sqrt(d_dot_d * d_dot_d * d_dot_d);

	// Already initialized to 0's, only compute the non zero coordinates
	output.dDdx.x = (d_dot_d * dsx - (d.x * dsx) * d.x) * temp;
	output.dDdx.y = (              - (d.x * dsx) * d.y) * temp;
	output.dDdx.z = (              - (d.x * dsx) * d.z) * temp;

	output.dDdy.x = (              - (d.y * dsy) * d.x) * temp;
	output.dDdy.y = (d_dot_d * dsy - (d.y * dsy) * d.y) * temp;
	output.dDdy.z = (              - (d.y * dsy) * d.z) * temp;
}

//bool CameraReverseRay(const AtNode *,const AtVector &,float,AtVector2 &)
camera_reverse_ray
{
   const MyCameraData* data = (MyCameraData*)AiNodeGetLocalData(node);
 
   // Note: we ignore distortion to compute the screen projection
   // compute projection factor: avoid divide by zero and flips when crossing the camera plane
   float coeff = 1 / AiMax(fabsf(Po.z * data->tan_fov), 1e-3f);
   Ps.x = Po.x * coeff;
   Ps.y = Po.y * coeff;
   return true;
}