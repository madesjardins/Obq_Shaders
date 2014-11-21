/****************************************************************************
*              iesUtils.h: utilities for parsing IES data files
*      This is part of the yafaray package
*      Copyright (C) 2009  Bert Buchholz and Rodrigo Placencia
*
*      This library is free software; you can redistribute it and/or
*      modify it under the terms of the GNU Lesser General Public
*      License as published by the Free Software Foundation; either
*      version 2.1 of the License, or (at your option) any later version.
*
*      This library is distributed in the hope that it will be useful,
*      but WITHOUT ANY WARRANTY; without even the implied warranty of
*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*      Lesser General Public License for more details.
*
*      You should have received a copy of the GNU Lesser General Public
*      License along with this library; if not, write to the Free Software
*      Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
/*
	Modified By Marc-Antoine Desjardins Nov-2012 : madesjardins@obliquefx.com
*/

#ifndef IESUTILS_H
#define IESUTILS_H

#include "O_Common.h"

#define TYPE_C 1
#define TYPE_B 2
#define TYPE_A 3

void getAngles(float &u, float &v, const AtVector &dir, const float &costheta)
{
        u = (dir.z >= 1.f) ? 0.f : float(AI_RTOD * std::acos(dir.z));
        
        if(dir.y < 0)
        {
                u = 360.f - u;
        }
       
        v = (costheta >= 1.f) ? 0.f : float(AI_RTOD * std::acos(costheta));
}


class IESData_t
{
public:

	IESData_t();
	~IESData_t();
	bool parseIESFile(const std::string file);
	float getRadiance(float hAng, float vAng) const;
	float getMaxVAngle() const { return maxVAngle; }

private:


	float* vertAngleMap; //<! vertical spherical angles
	float* horAngleMap; //<! horizontal sperical angles
	float** radMap; //<! spherical radiance map corresponding with entries to the angle maps
	int horAngles; //<! number of angles in the 2 directions
	int vertAngles;

	float maxRad;
	float maxVAngle;

	int type;
};

IESData_t::IESData_t()
{
}

IESData_t::~IESData_t()
{
	if(vertAngleMap) delete [] vertAngleMap;
	if(horAngleMap) delete [] horAngleMap;
	if(radMap)
	{
		for(int i = 0; i < horAngles; i++) delete [] radMap[i];
		delete [] radMap;
	}
}

float IESData_t::getRadiance(float h, float v) const {

	int x = 0, y = 0;
	float rad = 0.f;
	float hAng = 0.f, vAng = 0.f;

	if(type == TYPE_C)
	{
		hAng = h;
		vAng = v;
	}
	else
	{
		hAng = v;
		vAng = h;
		if(type == TYPE_B)
		{
			hAng += 90;
			if(hAng > 360.f) hAng -= 360.f;
		}
	}

	if(hAng > 180.f && horAngleMap[horAngles-1] <= 180.f) hAng = 360.f - hAng;
	if(hAng > 90.f && horAngleMap[horAngles-1] <= 90.f) hAng -= 90.f;

	if(vAng > 90.f && vertAngleMap[vertAngles-1] <= 90.f) vAng -= 90.f;

	for(int i = 0;i < horAngles; i++)
	{
		if(horAngleMap[i] <= hAng && horAngleMap[i+1] > hAng)
		{
			x = i;
		}
	}

	for(int i = 0;i < vertAngles; i++)
	{
		if(vertAngleMap[i] <= vAng && vertAngleMap[i+1] > vAng)
		{
			y = i;
			break;
		}
	}


	if(hAng == horAngleMap[x] && vAng == vertAngleMap[y])
	{
		rad = radMap[x][y];
	}
	else
	{
		int x1 = x, x2 = x+1;
		int y1 = y, y2 = y+1;

		float dX = (hAng - horAngleMap[x1]) / (horAngleMap[x2] - horAngleMap[x1]);
		float dY = (vAng - vertAngleMap[y1]) / (vertAngleMap[y2] - vertAngleMap[y1]);

		float rx1 = ((1.f - dX) * radMap[x1][y1]) + (dX * radMap[x2][y1]);
		float rx2 = ((1.f - dX) * radMap[x1][y2]) + (dX * radMap[x2][y2]);

		rad = ((1.f - dY) * rx1) + (dY * rx2);
	}


	return (rad * maxRad);
}

// IES description: http://lumen.iee.put.poznan.pl/kw/iesna.txt

bool IESData_t::parseIESFile(const std::string iesFile)
{
	using namespace std;

	AiMsgInfo("IES Parser: Parsing IES file %s ", iesFile.c_str());

	ifstream fin(iesFile.c_str(), std::ios::in);

	if (!fin)
	{
		AiMsgError("IES Parser: Could not open IES file: %s",iesFile.c_str());
		return false;
	}

	string line;
	string dummy;

	fin >> line;

	while (line.find("TILT=") == string::npos)
	{
		fin >> line;
	}

	if(line.find("TILT=") != string::npos)
	{
		if(line == "TILT=INCLUDE")
		{
			AiMsgInfo("IES Parser: Tilt data included in IES file. Skiping...");

			int pairs = 0;

			fin >> line;
			fin >> pairs;

			for(int i = 0; i < (pairs * 2); i++) fin >> line;

			AiMsgInfo("IES Parser: Tilt data skipped.");
		}
		else if(line == "TILT=NONE")
		{
			AiMsgInfo("IES Parser: No tilt data.");
		}
		else if(line == "TILT=NONE")
		{
			AiMsgInfo("IES Parser: Tilt data in another file.");
		}
	}
	else
	{
		fin.close();

		AiMsgInfo("IES Parser: Tilt not found IES invalid!");

		return false;
	}

	float candelaMult = 0.f;

	fin >> line;
	AiMsgInfo("IES Parser: Number of lamps: %s",line.c_str());
	fin >> line;
	AiMsgInfo("IES Parser: lumens per lamp: %s",line.c_str());
	fin >> candelaMult;
	candelaMult *= 0.001f;
	AiMsgInfo("IES Parser: Candela multiplier (kcd): %f", candelaMult);
	fin >> vertAngles;
	AiMsgInfo("IES Parser: Vertical Angles: %i",vertAngles);
	fin >> horAngles;
	AiMsgInfo("IES Parser: Horizontal Angles: %i",horAngles);
	type = 0;
	fin >> type;
	AiMsgInfo("IES Parser: Photometric Type: %i", type);
	fin >> line;
	AiMsgInfo("IES Parser: Units Type: %s", line.c_str());

	float w = 0.f, l = 0.f, h = 0.f;

	fin >> w;
	fin >> l;
	fin >> h;

	AiMsgInfo("IES Parser: Luminous opening dimensions:");
	AiMsgInfo("IES Parser: (Width, Length, Height) = (%f, %f, %f)",w,l,h);
	AiMsgInfo("IES Parser: Lamp Geometry: ");

	//Check geometry type
	if(w == 0.f && l == 0.f && h == 0.f)
	{
		AiMsgInfo("Point Light");
	}
	else if(w >= 0.f && l >= 0.f && h >= 0.f)
	{
		AiMsgInfo("Rectangular Light");
	}
	else if(w < 0.f && l == 0.f && h == 0.f)
	{
		AiMsgInfo("Circular Light");
	}
	else if(w < 0.f && l == 0.f && h < 0.f)
	{
		AiMsgInfo("Shpere Light");
	}
	else if(w < 0.f && l == 0.f && h >= 0.f)
	{
		AiMsgInfo("Vertical Cylindric Light");
	}
	else if(w == 0.f && l >= 0.f && h < 0.f)
	{
		AiMsgInfo("Horizontal Cylindric Light (Along width)");
	}
	else if(w >= 0.f && l == 0.f && h < 0.f)
	{
		AiMsgInfo("Horizontal Cylindric Light (Along length)");
	}
	else if(w < 0.f && l >= 0.f && h >= 0.f)
	{
		AiMsgInfo("Elipse Light (Along width)");
	}
	else if(w >= 0.f && l < 0.f && h >= 0.f)
	{
		AiMsgInfo("Elipse Light (Along length)");
	}
	else if(w < 0.f && l >= 0.f && h < 0.f)
	{
		AiMsgInfo("Elipsoid Light (Along width)");
	}
	else if(w >= 0.f && l < 0.f && h < 0.f)
	{
		AiMsgInfo("Elipsoid Light (Along length)");
	}

	fin >> line;
	AiMsgInfo("IES Parser: Ballast Factor: %s",line.c_str());
	fin >> line;
	AiMsgInfo("IES Parser: Ballast-Lamp Photometric Factor: %s",line.c_str());
	fin >> line;
	AiMsgInfo("IES Parser: Input Watts: %s", line.c_str());

	vertAngleMap = new float[vertAngles];

	maxVAngle = 0.f;

	AiMsgInfo("IES Parser: Vertical Angle Map (Only in Debug verbosity): ");

	for (int i = 0; i < vertAngles; ++i)
	{
		fin >> vertAngleMap[i];
		if(maxVAngle < vertAngleMap[i]) 
			maxVAngle = vertAngleMap[i];
		AiMsgDebug("%f",vertAngleMap[i]);
	}


	if(vertAngleMap[0] > 0.f)
	{
		AiMsgInfo("IES Parser: Vertical Angle Map (transformed) (Only in Debug verbosity):");
		float minus = vertAngleMap[0];
		maxVAngle -= minus;
		for (int i = 0; i < vertAngles; ++i)
		{
			vertAngleMap[i] -= minus;
			AiMsgDebug("%f",vertAngleMap[i]);
		}
		
	}

	AiMsgInfo("IES Parser: Max vertical angle (degrees): %f",maxVAngle);

	maxVAngle = float(AI_DTOR*maxVAngle);

	AiMsgInfo("IES Parser: Max vertical angle (radians): %f",maxVAngle);

	bool hAdjust = false;

	if(type == TYPE_C && horAngles == 1)
	{
		horAngles++;
		hAdjust = true;
	}

	horAngleMap = new float[horAngles];

	AiMsgInfo("IES Parser: Horizontal Angle Map (Only in Debug verbosity):");

	for (int i = 0; i < horAngles; ++i)
	{
		if(i == horAngles - 1 && hAdjust) horAngleMap[i] = 180.f;
		else fin >> horAngleMap[i];
		AiMsgDebug("%f",horAngleMap[i]);
	}
	maxRad = 0.f;

	radMap = new float*[horAngles];
	for (int i = 0; i < horAngles; ++i)
	{
		radMap[i] = new float[vertAngles];
		for (int j = 0; j < vertAngles; ++j)
		{
			if(i == horAngles - 1 && hAdjust) radMap[i][j] = radMap[i-1][j];
			else  fin >> radMap[i][j];
			if(maxRad < radMap[i][j]) maxRad = radMap[i][j];
		}
	}

	AiMsgInfo("IES Parser: maxRad = %f", maxRad);
	maxRad = 1.f / maxRad;

	fin.close();

	AiMsgInfo("IES Parser: IES File parsed successfully");

	return true;
}

#endif //IESUTILS_H
