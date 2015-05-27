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

#include "KettleBaker.h"

#include "Grid2DAS.h"

CKettleBaker::CKettleBaker(AtNode* node, AtNode* camera_node, const char* uvspace)
{
	if(AiNodeIs(node, "polymesh"))
	{
		std::vector<AtUInt32> nsides;
		readArnoldArray(nsides, AiNodeGetArray(node, "nsides"));

		mTriangleCount = 0;
		for(std::vector<AtUInt32>::iterator it = nsides.begin(); it != nsides.end(); ++it)
			mTriangleCount += *it - 2;		

		//readArnoldArray(mVList, AiNodeGetArray(node, "vlist")); // baaaah...
		AtArray* vlist = AiNodeGetArray(node, "vlist");
		mVList.resize(vlist->nelements);
		for(register unsigned int i = 0; i < vlist->nelements; ++i)
			mVList[i] = AiArrayGetPnt(vlist, i); 

		AtArray* nlist = AiNodeGetArray(node, "nlist");
		const bool calculate_normals = nlist->nelements == 0;
		if(!calculate_normals)
			readArnoldArray(mNList, nlist);			
		else
			mNList.reserve(mTriangleCount);

		bool useDefaultUV = (std::strcmp(uvspace, "DefaultUV") == 0);

		const char* uvspace_list = (useDefaultUV? "uvlist" : uvspace); // multi-uv support
		AiMsgInfo("[Obq_KettleUVStereoLens] <%s> is using uv = %s",AiNodeGetName(node), uvspace_list);

		AtArray* uvlist = AiNodeGetArray(node, uvspace_list);
		if(uvlist)
			readArnoldArray(mUVList, uvlist);
		else
			throw(std::runtime_error("No UV is available!"));

		// Doing the triangulation		

		std::vector<AtUInt32> vidxs;
		readArnoldArray(vidxs, AiNodeGetArray(node, "vidxs"));

		std::vector<AtUInt32> nidxs;
		readArnoldArray(nidxs, AiNodeGetArray(node, "nidxs"));

		char uvspace_idxs[256]; // multi-uv support
		std::sprintf(uvspace_idxs,"%sidxs",(useDefaultUV?"uv":uvspace));
		AiMsgInfo("[Obq_KettleUVStereoLens] <%s> is using uvidxs = %s",AiNodeGetName(node), uvspace_idxs);

		std::vector<AtUInt32> uvidxs;
		readArnoldArray(uvidxs, AiNodeGetArray(node, uvspace_idxs));

		AtUInt32 last_id = 0;
		// count the exact count, so no constant vector reallocation, 
		// in general it should be faster than guess the number of triangles, and resize the vectors

		mVIdxs.reserve(mTriangleCount);
		mNIdxs.reserve(mTriangleCount);
		mUVIdxs.reserve(mTriangleCount);

		for(std::vector<AtUInt32>::const_iterator it = nsides.begin(); it != nsides.end(); ++it)
		{
			const AtUInt32 ns = *it;

			for(register unsigned int j = 0; j < (ns - 2); ++j)
			{
				STriangleIndex ti;
				ti.x = vidxs[last_id];
				ti.y = vidxs[last_id + j + 1];
				ti.z = vidxs[last_id + j + 2];
				mVIdxs.push_back(ti);
				if(calculate_normals)
				{
					const AtVector& p0 = mVList[ti.x];
					const AtVector& p1 = mVList[ti.y];
					const AtVector& p2 = mVList[ti.z];
					ti.x = ti.y = ti.z = (AtUInt32)mNIdxs.size();
					AtVector n;
					AiV3Cross(n, p1 - p0, p2 - p1);
					AiV3Normalize(n, n);
					mNList.push_back(n);
					mNIdxs.push_back(ti);
				}
				else
				{
					ti.x = nidxs[last_id];
					ti.y = nidxs[last_id + j + 1];
					ti.z = nidxs[last_id + j + 2];
					mNIdxs.push_back(ti);
				}
				ti.x = uvidxs[last_id];
				ti.y = uvidxs[last_id + j + 1];
				ti.z = uvidxs[last_id + j + 2];
				mUVIdxs.push_back(ti);				
			}

			last_id += ns;
		}
		
		// OBQ EDIT :: At Render Time
		//AtArray* mat_list = AiNodeGetArray(node, "matrix");
		//if(mat_list != 0)
		//{
		//	AtMatrix mat;
		//	if(mat_list->nelements != 0)
		//	{
		//		AiArrayGetMtx(mat_list, 0, mat);
		//		if(!AiM4IsIdentity(mat))
		//		{
		//			for(std::vector<AtVector>::iterator it = mVList.begin(); it != mVList.end(); ++it)
		//				AiM4PointByMatrixMult(&(*it), mat, &(*it));
		//			for(std::vector<AtVector>::iterator it = mNList.begin(); it != mNList.end(); ++it)
		//			{
		//				AiM4VectorByMatrixMult(&(*it), mat, &(*it));
		//				AiV3Normalize(*it, *it);
		//			}
		//		}
		//	}
		//}

		//AtArray* marr = AiNodeGetArray(node, "matrix"); // objects should have one matrix, no motion blur when baking :)
		//AiArrayGetMtx(marr, 0, mTMat);

		//AtArray* cmarr = AiNodeGetArray(camera_node, "matrix");// Old: AtArray* cmarr = AiNodeGetArray(node, "matrix");
		//AtMatrix cmat;
		//AiArrayGetMtx(cmarr, 0, cmat);
		//AiM4Invert(cmat, mCMat);

        pAS = new CGrid2DAS(this, AiNodeGetInt(camera_node, "grid_size"));// Old : AiNodeGetInt(node, "grid_size"))
	}
	else
		throw(std::runtime_error("Object is not a polymesh"));
}

CKettleBaker::~CKettleBaker()
{
}

inline bool findBarycentric(const AtPoint2& P, const AtPoint2& A, const AtPoint2& B, const AtPoint2& C, AtPoint2& bary)
{
	const AtVector2 v0 = {C.x - A.x, C.y - A.y};
	const AtVector2 v1 = {B.x - A.x, B.y - A.y};
	const AtVector2 v2 = {P.x - A.x, P.y - A.y};

	const float dot00 = AiV2Dot(v0, v0);
	const float dot01 = AiV2Dot(v0, v1);
	const float dot02 = AiV2Dot(v0, v2);
	const float dot11 = AiV2Dot(v1, v1);
	const float dot12 = AiV2Dot(v1, v2);
	
	const float denom = (dot00 * dot11 - dot01 * dot01);
	if(denom == 0.0f)
		return false;
	
	const float invDenom = 1.0f / denom;

	bary.x = (dot11 * dot02 - dot01 * dot12) * invDenom;
	bary.y = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (bary.x >= -0.00001f) && (bary.y >= -0.00001f) && ((bary.x + bary.y) <= 1.00001f);
}

bool CKettleBaker::findSurfacePoint(const AtPoint2& uv, AtVector& pout, AtVector& nout)
{ // stealing is good : http://www.blackpawn.com/texts/pointinpoly/default.html
	pout = AI_V3_ZERO;
	nout = AI_V3_ZERO;

	AtPoint2 bary;

	unsigned int i = 0;

	if(pAS->intersect(uv, bary, i))
	{
		const STriangleIndex& tip = mVIdxs[i];
		const AtVector& va = mVList[tip.x];
		const AtVector& vb = mVList[tip.y];
		const AtVector& vc = mVList[tip.z];
		const STriangleIndex& tin = mNIdxs[i];
		const AtVector& na = mNList[tin.x];
		const AtVector& nb = mNList[tin.y];
		const AtVector& nc = mNList[tin.z];
		const float one_minus_u_v = 1.0f - bary.x - bary.y;
		pout = va * one_minus_u_v + vb * bary.y + vc * bary.x;
		nout = na * one_minus_u_v + nb * bary.y + nc * bary.x;
		// OBQ EDIT :: At Render Time
		//AiM4PointByMatrixMult(&pout, mTMat, &pout);
		//AiM4PointByMatrixMult(&pout, mCMat, &pout);
		//AiM4VectorByMatrixMult(&nout, mTMat, &nout);
		//AiM4VectorByMatrixMult(&nout, mCMat, &nout);
		AiV3Normalize(nout, nout);
		return true;
	}
	return false;
}

unsigned int CKettleBaker::numElements() const
{
	return mTriangleCount;
}

void CKettleBaker::boundingBox(const unsigned int& element, float bbox[4]) const
{
	const STriangleIndex& id = mUVIdxs[element];
	const AtVector2& uv0 = mUVList[id.x];
	const AtVector2& uv1 = mUVList[id.y];
	const AtVector2& uv2 = mUVList[id.z];

	bbox[0] = MIN(uv0.x, MIN(uv1.x, uv2.x));
	bbox[1] = MIN(uv0.y, MIN(uv1.y, uv2.y));
	bbox[2] = MAX(uv0.x, MAX(uv1.x, uv2.x));
	bbox[3] = MAX(uv0.y, MAX(uv1.y, uv2.y));
}

bool CKettleBaker::intersect(const unsigned int& element, 
		const AtVector2& pt, AtVector2& bary) const
{
	const STriangleIndex& ti = mUVIdxs[element];
	const AtPoint2& pa = mUVList[ti.x];
	const AtPoint2& pb = mUVList[ti.y];
	const AtPoint2& pc = mUVList[ti.z];
	return findBarycentric(pt, pa, pb, pc, bary);
}
