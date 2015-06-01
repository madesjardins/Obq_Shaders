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

#include "Grid2DAS.h"

CGrid2DAS::CGrid2DAS(const CKettleBaker* _is, int grid_size) : mNumElements(_is->numElements()), is(_is), mGridSize((unsigned int)MAX(8, grid_size))
{
	const unsigned int grid_count = mGridSize * mGridSize;
	mGrid.resize(grid_count);
	const unsigned int grid_reserve_size = grid_count / mNumElements;
	for(register unsigned int i = 0; i < grid_count; ++i)
		mGrid[i].reserve(grid_reserve_size);
	
	float bbox[4];
	unsigned int coords[4];
	const unsigned int grid_max = mGridSize - 1;
	const float grid_f = (float)mGridSize;
	for(register unsigned int i = 0; i < mNumElements; ++i)
	{
		is->boundingBox(i, bbox); // small offset for the coordinates
		coords[0] = (unsigned int)CLAMP((int)((bbox[0] - 0.001f) * grid_f), 0, (int)grid_max);
		coords[1] = (unsigned int)CLAMP((int)((bbox[1] - 0.001f) * grid_f), 0, (int)grid_max);
		coords[2] = (unsigned int)CLAMP((int)((bbox[2] + 0.001f) * grid_f), 0, (int)grid_max) + 1;
		coords[3] = (unsigned int)CLAMP((int)((bbox[3] + 0.001f) * grid_f), 0, (int)grid_max) + 1;
        
		for(register unsigned int xx = coords[0]; xx < coords[2]; ++xx)
		{
			for(register unsigned int yy = coords[1]; yy < coords[3]; ++yy)
			{
				mGrid[xx + yy * mGridSize].push_back(i);
			}
		}
	}
}

CGrid2DAS::~CGrid2DAS()
{ }

bool CGrid2DAS::intersect(const AtVector2& pt, AtVector2& bary, unsigned int& elem) const
{
	const unsigned int grid_max = mGridSize - 1;
	const float grid_f = (float)mGridSize;
	const int grid_coord =  CLAMP((int)(pt.x * grid_f), 0, (int)grid_max)
    + CLAMP((int)(pt.y * grid_f), 0, (int)grid_max) * mGridSize;
	for(std::vector<unsigned int>::const_iterator it = mGrid[grid_coord].begin(); it != mGrid[grid_coord].end(); ++it)
	{
		if(is->intersect(*it, pt, bary))
		{
			elem = *it;
			return true;
		}
	}
	return false;
}
