// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011 - Science-D-Visions. Current version: 1.1


#ifndef ldpk_power_ct_sdv
#define ldpk_power_ct_sdv

namespace ldpk
	{
// Position des hoechsten gesetzten Bits in N.
// Das ist eine Art Zweierlogarithmus von N.
//
//! @brief Compile-Time computations, needed by ldpk::power_ct
	template <int N> class highest_bit_ct
		{
	public:
		const static int r = highest_bit_ct<(N >> 1)>::r + 1;
		};
// Das ist der niedrigste Wert, fuer den die Operation definiert ist.
// Was wir bei Null machen, entscheiden wir spaeter.
	template <> class highest_bit_ct<1>
		{
	public:
		const static int r = 0;
		};

// Berechnung der Potenzen 0 bis N-1 einer T-wertigen Zahl,
// wobei N eine CT-Konstante ist. Hierfuer werden N - 2
// RT-Multiplikationen benoetigt. Fuer das n-te Monom werden
// ceil(log2(n)) Multiplikationen verwendet, um die Genauigkeit
// zu optimieren. Der Sinn der Uebung besteht darin, eine Polynomauswertung
// zu bauen, die nicht ungenauer ist als Clenshaw (allerdings etwas langsamer),
// aber moeglicherweise einfacher, weil man keine Chebyshev-Entwicklung benoetigt.
//
//! Computing N powers of a given number, where N is a compile-time constant.
	template <class T,int N>
	class power_ct
		{
	public:
		void build(T x,T* p)
			{
			power_ct<T,N - 1>().build(x,p);
			p[N - 1] = p[1 << (highest_bit_ct<N - 1>::r - 1)] * p[N - 1 - (1 << (highest_bit_ct<N - 1>::r - 1))];
			}
		};
	template <class T>
	class power_ct<T,1>
		{
	public:
		void build(T x,T* p)
			{
			p[0] = T(1);
			}
		};
	template <class T>
	class power_ct<T,2>
		{
	public:
		void build(T x,T* p)
			{
			p[0] = T(1);
			p[1] = x;
			}
		};
	}

#endif
