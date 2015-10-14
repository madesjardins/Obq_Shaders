// Copyright (c) 2013, Kettle Studio Ltd. All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials 
// provided with the distribution.
// Neither the name of the authors nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written 
// permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS 
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// modified by Marc-Antoine Desjardins [Obq_Shaders] 2015
#pragma once

#include <ai.h>

#include <vector>
#include <exception>
#include <stdexcept>
#include <string>
#include <cstring>
#include <cstdio>

#include "arnold_utils.h"

class CGrid2DAS;

class CKettleBaker{
public:
	CKettleBaker(AtNode* node, AtNode* camera_node, const char* uvspace); // added multi-uv support
	~CKettleBaker();

	bool findSurfacePoint(const AtPoint2& uv, AtVector& pout, AtVector& nout);

	unsigned int numElements() const;
	void boundingBox(const unsigned int& element, float bbox[4]) const;
	bool intersect(const unsigned int& element, 
		const AtVector2& pt, AtVector2& bary) const;
	bool isGood(){return init;}
	static void* operator new(size_t s)	{return AiMalloc(s);}
	static void operator delete(void* p){AiFree(p);}

private:
	struct STriangleIndex{ // struct for storing the data indices for a triangle
		AtUInt32 x,y,z;
	};
	
	// Vertex Position Data
	std::vector<AtVector> mVList;
	std::vector<STriangleIndex> mVIdxs;

	// Normal Data
	std::vector<AtVector> mNList;
	std::vector<STriangleIndex> mNIdxs;

	// UV Data
	std::vector<AtVector2> mUVList;
	std::vector<STriangleIndex> mUVIdxs;

	// Transformation Matrix
	AtMatrix mTMat;
	AtMatrix mCMat;

	// Other data
	AtUInt32 mTriangleCount;

	CGrid2DAS* pAS;

	bool init;
};
