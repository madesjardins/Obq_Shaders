
#ifndef DS_MISC_H
#define DS_MISC_H

#include <limits>

namespace DSmath
{
    
const float epsilon = std::numeric_limits<float>::epsilon() * 10;

// Non-inline functions.
//----------------------
float smoothstep(float lower, float upper, float value);
float gain(float gain, float value);
float bias(float bias, float value);
long  ftol(float value);
long  iceiling(float value);
float ceiling (float value);
long  ifloor(float value);
float floor (float value);

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: swap
--------------------------------------------------------------------------------

    Description: Swaps two values without using a temp variable.

    Algorithms:

    Input Parameters:

    Output Values:

    Revision History:
        Rev.002 08/29/96 (Aug) - Added unsigned short version.
        Rev.001 03/21/95 (Aug) - Initial version.

**ec***************************************************************************/
inline void swap(int &a, int &b)
{                                                                         /*sc*/
	a = a^b;
	b = b^a;
	a = a^b;
}                                                                         /*ec*/

inline void swap(unsigned char &a, unsigned char &b)
{                                                                         /*sc*/

	a = static_cast<unsigned char>(a^b);
	b = static_cast<unsigned char>(b^a);
	a = static_cast<unsigned char>(a^b);
}                                                                         /*ec*/

inline void swap(unsigned short &a, unsigned short &b)
{                                                                         /*sc*/

	a = static_cast<unsigned short>(a^b);
	b = static_cast<unsigned short>(b^a);
	a = static_cast<unsigned short>(a^b);
}                                                                         /*ec*/

inline void swap(float &a, float &b)
{                                                                         /*sc*/
	float   temp = a;
	a = b;
	b = temp;
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: mix
--------------------------------------------------------------------------------

    Description: Returns a mix of the two input values based on a mix
     value between 0.0 and 1.0.  The resulting value is a linear
     interpolation between the two input values.  A mix value of 0.0 will
     result in a return value of "value0" and a mix value of 1.0 will
     result in a return value of "value1".

     NOTE: The bounds of the mix value are not checked for performance
     reasons.  Behavior for values outside the range 0.0 - 1.0 is
     undefined.

    Algorithms: Linear interpolation between values.

    Input Parameters:
     value0, value1 - any pair of float, unsigned char, or int values.
     mixval - a float value in the range 0.0 to 1.0.

    Output Values:
     A value consisting of a linear interpolation between the two input
     values based on the mix value.

    Revision History:
        Rev.003 02/09/98 (Random) - Changed from 2 *'s to 1.
        Rev.002 05/27/96 (Aug) - Fixed type warnings.
        Rev.001 03/21/95 (Aug) - Initial version.

**ec***************************************************************************/
inline int mix(int value0, int value1, float mixval)
{                                                                         /*sc*/
	return static_cast<int>( value0 + mixval*(value1 - value0) );
}                                                                         /*ec*/

inline unsigned char mix(unsigned char value0, unsigned char value1, float mixval)
{                                                                         /*sc*/
	return static_cast<unsigned char>( value0 + mixval*(value1 - value0) );
}                                                                         /*ec*/

inline float mix(float value0, float value1, float mixval)
{                                                                         /*sc*/
	return static_cast<float>( value0 + mixval*(value1 - value0) );
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: step
--------------------------------------------------------------------------------

    Description: A simple step function.  Returns 0 when "value" is less than
     "step" and returns 1 otherwise.

    Algorithms:

    Input Parameters:
     step - the point at which the function transitions from 0 to 1.
     value - value compared against the step point.

    Output Values: 0 if (value < step); 1 if (value >= step).

    Revision History:
        Rev.001 03/21/95 (Aug) - Initial version.

**ec***************************************************************************/
inline int step(int step, int value)
{                                                                         /*sc*/
	return static_cast<int>(value >= step);
}                                                                         /*ec*/

inline unsigned char step(unsigned char step, unsigned char value)
{                                                                         /*sc*/
	return static_cast<unsigned char>(value >= step);
}                                                                         /*ec*/

inline float step(float step, float value)
{                                                                         /*sc*/
	return static_cast<float>(value >= step);
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: pulse
--------------------------------------------------------------------------------

    Description: A simple pulse function.  Returns 0 when "value" is less than
     "start" or greater than "end".  Returns 1 otherwise.

    Algorithms:

    Input Parameters:
     start - the transition point of the start of the pulse.
     end - the transition point of the end of the pulse.
     value - the value compared against start/end.

    Output Values: 0 if (value < start) or (value > end).
                   1 if (value >= start) or (value <= end).

    Revision History:
        Rev.001 03/21/95 (Aug) - Initial version.

**ec***************************************************************************/
inline int pulse(int start, int end, int value)
{                                                                         /*sc*/
	return step(start, value) - step(end, value);
}                                                                         /*ec*/

inline unsigned char pulse(unsigned char start, unsigned char end, unsigned char value)
{                                                                         /*sc*/
	return step(start, value) - step(end, value);
}                                                                         /*ec*/

inline float pulse(float start, float end, float value)
{                                                                         /*sc*/
	return step(start, value) - step(end, value);
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: scurve
--------------------------------------------------------------------------------

    Description: A simple s-curve function.

    Algorithms:    
     The equation is 3t^2 - 2t^3.
     It is 0 at 0, 0.5 at 0.5, and 1 at 1.

    Input Parameters:
     t - the input value.

    Output Values: The value of the s-curve (0 - 1).

    Revision History:
        Rev.001 03/26/99 (Random) - Initial version.

**ec***************************************************************************/
inline float scurve(float t)
{                                                                         /*sc*/
    return 2.0f * t * t * (1.5f - t);
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: smoothcurve
--------------------------------------------------------------------------------

    Description: A smoother s-curve function.

    Algorithms:
     The equation is 6t^5 - 15t^4 + 10t^3.
     Rewrite as ((6t - 15)t + 10)t^3 so only 5 *'s.
     It is 0 at 0, 0.5 at 0.5, and 1 at 1.
     It has a smooth second derivative.

    Input Parameters:
     t - the input value.

    Output Values: The value of the s-curve (0 - 1).

    Revision History:
        Rev.001 11/01/00 (Random) - Initial version.

**ec***************************************************************************/
inline float smoothcurve(float t)
{                                                                         /*sc*/
    return (t * t * t * ( 10.0f + (t * (6.0f * t - 15.0f))));
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: clamp
--------------------------------------------------------------------------------

    Description: Clamps "value" between provided upper and lower bounds.
     The function returns the lower bound when the value is less than the
     lower bound, it returns the upper bound when the function is greater
     than the upper bound, and it returns the value itself when it is
     between the two bounding values.

    Algorithms:

    Input Parameters:
     lower - the lower bound/clamping value.
     upper - the upper bound/clamping value.
     value - value to compare against the clamping bounds.

    Output Values: returns lower if (value < lower)
                           upper if (value > upper)
                           value if (value >= lower) and (value <= upper)

    Revision History:
        Rev.003 10/07/96 (Aug) - Added unsigned int version.
        Rev.002 04/08/96 (SkylerS) - Added double version to avoid overflow.
        Rev.001 03/21/95 (Aug) - Initial version.

**ec***************************************************************************/
inline unsigned int clamp(unsigned int lower, unsigned int upper, unsigned int value)
{                                                                         /*sc*/
	return (value < lower ? lower : (value > upper ? upper : value));
}                                                                         /*ec*/

inline int clamp(int lower, int upper, int value)
{                                                                         /*sc*/
	return (value < lower ? lower : (value > upper ? upper : value));
}                                                                         /*ec*/

inline unsigned char clamp(unsigned char lower, unsigned char upper, unsigned char value)
{                                                                         /*sc*/
	return (value < lower ? lower : (value > upper ? upper : value));
}                                                                         /*ec*/

inline float clamp(float lower, float upper, float value)
{                                                                         /*sc*/
	return (value < lower ? lower : (value > upper ? upper : value));
}                                                                         /*ec*/

inline double clamp( double lower, double upper, double value )
{                                                                         /*sc*/
    return (value < lower ? lower : (value > upper ? upper : value) );
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: DSmin
--------------------------------------------------------------------------------

    Description: Returns the smaller of the two input values.  min() is such a
     commonly #defined function that we need the 'DS', even with namespaces.

    Algorithms:

    Input Parameters:
     a - first input value.
     b - second input value.

    Output Values: the smaller of the two values.

    Revision History:
        Rev.003 10/17/96 (Aug) - Added unsigned int version.
        Rev.002 09/16/95 (SkylerS) - Added double version.
        Rev.001 03/21/95 (Aug) - Initial version.

**ec***************************************************************************/
inline int DSmin(int a, int b)
{                                                                         /*sc*/
	return a < b ? a : b;
}                                                                         /*ec*/

inline unsigned int DSmin(unsigned int a, unsigned int b)
{                                                                         /*sc*/
	return a < b ? a : b;
}                                                                         /*ec*/

inline unsigned char DSmin(unsigned char a, unsigned char b)
{                                                                         /*sc*/
	return a < b ? a : b;
}                                                                         /*ec*/

inline float DSmin(float a, float b)
{                                                                         /*sc*/
	return a < b ? a : b;
}                                                                         /*ec*/

inline double DSmin(double a, double b)
{                                                                         /*sc*/
	return a < b ? a : b;
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: DSmax
--------------------------------------------------------------------------------

    Description: Returns the larger of the two input values.  max() is such a
     commonly #defined function that we need the 'DS', even with namespaces.

    Algorithms:

    Input Parameters:

    Output Values: the larger of the two values.

    Revision History:
        Rev.003 10/17/96 (Aug) - Added unsigned int version.
        Rev.002 09/16/95 (SkylerS) - Added double version.
        Rev.001 03/21/95 (Aug) - Initial version.

**ec***************************************************************************/
inline int DSmax(int a, int b)
{                                                                         /*sc*/
	return a < b ? b : a;
}                                                                         /*ec*/

inline unsigned int DSmax(unsigned int a, unsigned int b)
{                                                                         /*sc*/
	return a < b ? b : a;
}                                                                         /*ec*/

inline unsigned char DSmax(unsigned char a, unsigned char b)
{                                                                         /*sc*/
	return a < b ? b : a;
}                                                                         /*ec*/

inline float DSmax(float a, float b)
{                                                                         /*sc*/
	return a < b ? b : a;
}                                                                         /*ec*/

inline double DSmax(double a, double b)
{                                                                         /*sc*/
	return a < b ? b : a;
}                                                                         /*ec*/

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: DSabs
--------------------------------------------------------------------------------

    Description: Returns the absolute value of the input.  abs() is such a
     commonly #defined function that we need the 'DS', even with namespaces.

    Algorithms:

    Input Parameters:
     a - the input.     

    Output Values: the absolute value of the input.

    Revision History:
        Rev.003 11/01/00 (Random) - Use faster bit-clear for floats(IEEE).
        Rev.002 10/06/95 (Aug) - Updated unsigned char version to do
         nothing. DOH! Also added double version.
        Rev.001 03/21/95 (Aug) - Initial version.

**ec***************************************************************************/
inline int DSabs(int a)
{                                                                         /*sc*/
	return a < 0 ? -a : a;
}                                                                         /*ec*/

inline unsigned char DSabs(unsigned char a)
{                                                                         /*sc*/
	return a;
}                                                                         /*ec*/

inline float DSabs(float a)
{                                                                         /*sc*/
    //  Single-precision IEEE floating point is 32 bits, clear
	//  the MSbit to make sure it is positive.
    //-------------------------------------------------------
    unsigned long floatBits = *(reinterpret_cast<unsigned long*>(&a));
    floatBits &= 0x7fffffff;

    return *(reinterpret_cast<float*>(&floatBits));
}                                                                         /*ec*/

inline double DSabs(double a)
{                                                                         /*sc*/
	return a < 0 ? -a : a;
}                                                                         /*ec*/

} //namespace DSmath

#endif //DS_MISC_H
