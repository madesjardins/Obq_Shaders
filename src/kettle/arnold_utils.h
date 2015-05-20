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

#pragma once

#include <ai.h>

#include <vector>

template <typename T> 
inline T readArnoldArrayElement(const AtArray* ain, const AtUInt64& i)
{}

template <>
inline AtVector readArnoldArrayElement<AtVector>(const AtArray* ain, const AtUInt64& i)
{
	return AiArrayGetVec(ain, i);
}

template <>
inline AtVector2 readArnoldArrayElement<AtVector2>(const AtArray* ain, const AtUInt64& i)
{
	return AiArrayGetPnt2(ain, i);
}

template <>
inline AtUInt32 readArnoldArrayElement<AtUInt32>(const AtArray* ain, const AtUInt64& i)
{
	return AiArrayGetUInt(ain, i);
}

template <typename T>
inline void readArnoldArray(std::vector<T>& vout, const AtArray* ain)
{
	if(ain->nelements != 0)
	{
		vout.resize(ain->nelements);
		for(register AtUInt64 i = 0; i < ain->nelements; ++i)
			vout[i] = readArnoldArrayElement<T>(ain, i);
	}
	else
		AiMsgWarning("[readArnoldArray] Trying to read an empty vector!");
}