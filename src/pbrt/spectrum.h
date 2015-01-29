/*
	This is a modified version of pbrt's spectrum.h file.
	------------------------------------------------------

    pbrt source code Copyright(c) 1998-2010 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    pbrt is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.  Note that the text contents of
    the book "Physically Based Rendering" are *not* licensed under the
    GNU GPL.

    pbrt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "O_Common.h"

enum ObqXYZ2RGB { SRGB_ILLUM_E, SRGB_ILLUM_D65_BRADFORD_E, SRGB_ILLUM_D65, KEEP_XYZ };

// enum for backfacemode
enum {OUTPUT_LINEAR, OUTPUT_SRGB};

// enum for fresnel equations
//
enum {INPUT_RGB, INPUT_STRING};

template <int nSamples> class CoefficientSpectrum;
class RGBSpectrum;
class SampledSpectrum;
typedef RGBSpectrum Spectrum;


// Global Inline Functions
inline float Lerp(float t, float v1, float v2) {
    return (1.f - t) * v1 + t * v2;
}

inline double Lerp(double t, double v1, double v2) {
    return (1.0 - t) * v1 + t * v2;
}

inline float Clamp(float val, float low, float high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}


inline int Clamp(int val, int low, int high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}


// Spectrum Utility Declarations
static const int sampledLambdaStart = 400;
static const int sampledLambdaEnd = 700;
static const int nSpectralSamples = 30;
extern bool SpectrumSamplesSorted(const float *lambda, const float *vals, int n);
extern void SortSpectrumSamples(float *lambda, float *vals, int n);


 //sRGB illuminant E
inline void XYZToSRGB_E(const float xyz[3], float rgb[3]) {
    rgb[0] =  2.689656f*xyz[0] - 1.275862f*xyz[1] - 0.413793f*xyz[2];
    rgb[1] = -1.022108f*xyz[0] + 1.978287f*xyz[1] + 0.043822f*xyz[2];
    rgb[2] =  0.061224f*xyz[0] - 0.224490f*xyz[1] + 1.163265f*xyz[2];
}

//sRGB illuminant D65 Bradford adapted from XYZ Illuminant E
inline void XYZToSRGB_D65_BRAD_E(const float xyz[3], float rgb[3]) {
    rgb[0] =  3.146252f*xyz[0] - 1.666125f*xyz[1] - 0.480127f*xyz[2];
    rgb[1] = -0.995535f*xyz[0] + 1.955764f*xyz[1] + 0.039771f*xyz[2];
    rgb[2] =  0.063598f*xyz[0] - 0.214596f*xyz[1] + 1.150999f*xyz[2];
}

//sRGB illuminant D65
inline void XYZToSRGB_D65(const float xyz[3], float rgb[3]) {
    rgb[0] =  3.24081278f*xyz[0] - 1.53730845f*xyz[1] - 0.49858657f*xyz[2];
    rgb[1] = -0.96924323f*xyz[0] + 1.87596655f*xyz[1] + 0.04155507f*xyz[2];
    rgb[2] =  0.05563836f*xyz[0] - 0.20400739f*xyz[1] + 1.05712962f*xyz[2];
}

// Inverse sRGB illuminant E
inline void RGBToXYZ_E(const float rgb[3], float xyz[3]) 
{
	xyz[0] = 0.496921f*rgb[0] + 0.339090f*rgb[1] + 0.163989f*rgb[2];
	xyz[1] = 0.256225f*rgb[0] + 0.678179f*rgb[1] + 0.065596f*rgb[2];
	xyz[2] = 0.023293f*rgb[0] + 0.113030f*rgb[1] + 0.863677f*rgb[2];
}

//Inverse sRGB illuminant D65 Bradford adapted XYZ illuminant E
inline void RGBToXYZ_D65_BRAD_E(const float rgb[3], float xyz[3]) 
{
	xyz[0] = 0.438496f*rgb[0] + 0.392140f*rgb[1] + 0.169364f*rgb[2];
	xyz[1] = 0.222854f*rgb[0] + 0.708672f*rgb[1] + 0.068474f*rgb[2];
	xyz[2] = 0.017321f*rgb[0] + 0.110460f*rgb[1] + 0.872219f*rgb[2];
}

// Inverse sRGB illuminant D65
inline void RGBToXYZ_D65(const float rgb[3], float xyz[3]) 
{
	xyz[0] = 0.412410794743812f*rgb[0] + 0.35758446988800086f*rgb[1] + 0.180453780024802f*rgb[2];
	xyz[1] = 0.2126493343769654f*rgb[0] + 0.7151690302407988f*rgb[1] + 0.07218150138135924f*rgb[2];
	xyz[2] = 0.019331759359499993f*rgb[0] + 0.11919480016293431f*rgb[1] + 0.9503899884355825f*rgb[2];
}

enum SpectrumType { SPECTRUM_REFLECTANCE, SPECTRUM_ILLUMINANT };
extern float InterpolateSpectrumSamples(const float *lambda, const float *vals, int n, float l);

// Spectral Data Declarations
static const int nCIESamples = 471;
extern const float CIE_X[nCIESamples];
extern const float CIE_Y[nCIESamples];
extern const float CIE_Z[nCIESamples];
extern const float CIE_lambda[nCIESamples];
static const int nRGB2SpectSamples = 32;

// Spectrum Declarations
template <int nSamples> class CoefficientSpectrum {
public:
    // CoefficientSpectrum Public Methods
    CoefficientSpectrum(float v = 0.f) {
        for (int i = 0; i < nSamples; ++i)
            c[i] = v;
    }

    CoefficientSpectrum operator+(const CoefficientSpectrum &s2) const {
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] += s2.c[i];
        return ret;
    }
 
    CoefficientSpectrum operator*(const CoefficientSpectrum &sp) const {
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] *= sp.c[i];
        return ret;
    }

    CoefficientSpectrum operator*(float a) const {
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] *= a;
        return ret;
    }
    CoefficientSpectrum &operator*=(float a) {
        for (int i = 0; i < nSamples; ++i)
            c[i] *= a;
        return *this;
    }
    friend inline
    CoefficientSpectrum operator*(float a, const CoefficientSpectrum &s) {
        return s * a;
    }
 
    CoefficientSpectrum Clamp(float low = 0, float high = AI_INFINITE) const {
        CoefficientSpectrum ret;
        for (int i = 0; i < nSamples; ++i)
            ret.c[i] = ::Clamp(c[i], low, high);
        return ret;
    }
    bool HasNaNs() const {
        for (int i = 0; i < nSamples; ++i)
            if (isnan(c[i])) return true;
        return false;
    }

protected:
    // CoefficientSpectrum Protected Data
    float c[nSamples];
};


class RGBSpectrum : public CoefficientSpectrum<3> {
    using CoefficientSpectrum<3>::c;
public:
    // RGBSpectrum Public Methods
    RGBSpectrum(float v = 0.f) : CoefficientSpectrum<3>(v) { }
    RGBSpectrum(const CoefficientSpectrum<3> &v)
        : CoefficientSpectrum<3>(v) { }
    RGBSpectrum(const RGBSpectrum &s, SpectrumType type = SPECTRUM_REFLECTANCE) : CoefficientSpectrum<3>()  {
        *this = s;
    }
    static RGBSpectrum FromRGB(const float rgb[3],
            SpectrumType type = SPECTRUM_REFLECTANCE) {
        RGBSpectrum s;
        s.c[0] = rgb[0];
        s.c[1] = rgb[1];
        s.c[2] = rgb[2];
        return s;
    }

	void ToRGB(float *rgb) const {
        rgb[0] = c[0];
        rgb[1] = c[1];
        rgb[2] = c[2];
    }
	void ToRGB(double *rgb) const {
        rgb[0] = static_cast<float>(c[0]);
        rgb[1] = static_cast<float>(c[1]);
        rgb[2] = static_cast<float>(c[2]);
    }
    const RGBSpectrum &ToRGBSpectrum() const {
        return *this;
    }
    void ToXYZ(float xyz[3],/*ObqXYZ2RGB*/int op) const {
		switch(op)
		{
		case SRGB_ILLUM_E:
			RGBToXYZ_E(c, xyz);
			break;
		case SRGB_ILLUM_D65_BRADFORD_E:
			RGBToXYZ_D65_BRAD_E(c, xyz);
			break;
		case SRGB_ILLUM_D65:
			RGBToXYZ_D65(c, xyz);
			break;
		default: // copy aka keep rgb
			 xyz[0] = c[0];			
			 xyz[1] = c[1];
			 xyz[2] = c[2];
			break;
		}
    }

    static RGBSpectrum FromXYZ(const float xyz[3], /*ObqXYZ2RGB*/int op, SpectrumType type = SPECTRUM_REFLECTANCE) {
        RGBSpectrum r;
		//AiMsgInfo("RGBSpectrum : (X,Y,Z) = (%f, %f, %f)",xyz[0],xyz[1],xyz[2]);
		switch(op)
		{
		case SRGB_ILLUM_E:
			XYZToSRGB_E(xyz, r.c);
			break;
		case SRGB_ILLUM_D65_BRADFORD_E:
			XYZToSRGB_D65_BRAD_E(xyz, r.c);
			break;
		case SRGB_ILLUM_D65:
			XYZToSRGB_D65(xyz, r.c);
			break;
		default: // copy aka keep xyz
			r.c[0] = xyz[0];			
			r.c[1] = xyz[1];
			r.c[2] = xyz[2];
			break;
		}
        return r;
    }
   
	float y() const {
        const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
        return YWeight[0] * c[0] + YWeight[1] * c[1] + YWeight[2] * c[2];
    }

    static RGBSpectrum FromSampled(const float *lambda, const float *v,
                                   int n,/*ObqXYZ2RGB*/int op) {
        // Sort samples if unordered, use sorted for returned spectrum
        if (!SpectrumSamplesSorted(lambda, v, n)) {
            std::vector<float> slambda(&lambda[0], &lambda[n]);
            std::vector<float> sv(&v[0], &v[n]);
            SortSpectrumSamples(&slambda[0], &sv[0], n);
            return FromSampled(&slambda[0], &sv[0], n,op);
        }
        float xyz[3] = { 0, 0, 0 };
        float yint = 0.0;
        for (int i = 0; i < nCIESamples; ++i) {
            yint += CIE_Y[i]; // original pbrt
			
            float val = InterpolateSpectrumSamples(lambda, v, n,
                                                   CIE_lambda[i]);
            xyz[0] += val * CIE_X[i];
            xyz[1] += val * CIE_Y[i];
            xyz[2] += val * CIE_Z[i];
        }
		
        xyz[0] /= yint;
        xyz[1] /= yint;
        xyz[2] /= yint;
        return FromXYZ(xyz,op);
    }
};

inline Spectrum Lerp(float t, const Spectrum &s1, const Spectrum &s2) {
    return (1.f - t) * s1 + t * s2;
}




#endif // SPECTRUM_H
