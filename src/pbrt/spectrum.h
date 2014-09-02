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

#include "Obq_Common.h"
#include <vector>

template <int nSamples> class CoefficientSpectrum;
class RGBSpectrum;
class SampledSpectrum;
typedef RGBSpectrum Spectrum;


// Global Inline Functions
inline float Lerp(float t, float v1, float v2) {
    return (1.f - t) * v1 + t * v2;
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

 //CIE RGB
 inline void XYZToRGB(const float xyz[3], float rgb[3]) {
    rgb[0] =  2.3706743f*xyz[0] - 0.9000405f*xyz[1] - 0.4706338f*xyz[2];
    rgb[1] = -0.5138850f*xyz[0] + 1.4253036f*xyz[1] + 0.0885814f*xyz[2];
    rgb[2] =  0.0052982f*xyz[0] - 0.0146949f*xyz[1] + 1.0093968f*xyz[2];
}

//inline void XYZToSRGBD65(const float xyz[3], float rgb[3]) {
//    rgb[0] =  3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
//    rgb[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
//    rgb[2] =  0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];
//}

// inverse CIE RGB
//
inline void RGBToXYZ(const float rgb[3], float xyz[3]) {
    xyz[0] = 0.4887180f*rgb[0] + 0.3106803f*rgb[1] + 0.2006017f*rgb[2];
    xyz[1] = 0.1762044f*rgb[0] + 0.8129847f*rgb[1] + 0.0108109f*rgb[2];
    xyz[2] = 0.0000000f*rgb[0] + 0.0102048f*rgb[1] + 0.9897952f*rgb[2];
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
    RGBSpectrum(const RGBSpectrum &s, SpectrumType type = SPECTRUM_REFLECTANCE) {
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
        rgb[0] = c[0];
        rgb[1] = c[1];
        rgb[2] = c[2];
    }
    const RGBSpectrum &ToRGBSpectrum() const {
        return *this;
    }
    void ToXYZ(float xyz[3]) const {
        RGBToXYZ(c, xyz);
    }
    static RGBSpectrum FromXYZ(const float xyz[3], SpectrumType type = SPECTRUM_REFLECTANCE) {
        RGBSpectrum r;
		XYZToRGB(xyz, r.c);
        return r;
    }
    float y() const {
        const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
        return YWeight[0] * c[0] + YWeight[1] * c[1] + YWeight[2] * c[2];
    }
    static RGBSpectrum FromSampled(const float *lambda, const float *v,
                                   int n) {
        // Sort samples if unordered, use sorted for returned spectrum
        if (!SpectrumSamplesSorted(lambda, v, n)) {
            std::vector<float> slambda(&lambda[0], &lambda[n]);
            std::vector<float> sv(&v[0], &v[n]);
            SortSpectrumSamples(&slambda[0], &sv[0], n);
            return FromSampled(&slambda[0], &sv[0], n);
        }
        float xyz[3] = { 0, 0, 0 };
        float yint = 0.f;
        for (int i = 0; i < nCIESamples; ++i) {
            yint += CIE_Y[i];
            float val = InterpolateSpectrumSamples(lambda, v, n,
                                                   CIE_lambda[i]);
            xyz[0] += val * CIE_X[i];
            xyz[1] += val * CIE_Y[i];
            xyz[2] += val * CIE_Z[i];
        }
        xyz[0] /= yint;
        xyz[1] /= yint;
        xyz[2] /= yint;
        return FromXYZ(xyz);
    }
};

inline Spectrum Lerp(float t, const Spectrum &s1, const Spectrum &s2) {
    return (1.f - t) * s1 + t * s2;
}


#endif // SPECTRUM_H
