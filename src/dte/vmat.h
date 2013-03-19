
#ifndef DS_VMAT_H
#define DS_VMAT_H

//
// Initial code derived from:
//
// Graphics Gems IV
// C++ Vector and Matrix Algebra routines
// Author: Jean-Francois Doue
// Version 3.1 --- October 1993
//
// Modified for our purposes.
//

#include <math.h>
#include "misc.h"

namespace DSmath
{

// Typedef for apply() function pointer argument.
// The signature is a function which takes a single float as an
// argument and returns a float.
//-------------------------------------------------------------
typedef float (*V_FCT_PTR)(float);

enum AXIS    {VX=0, VY=1, VZ=2, VW=3};              // Vector axes and matrix columns.
enum RGBA    {VRED=0, VGREEN=1, VBLUE=2, VALPHA=3}; // Color vector indecies.
enum HSV     {VHUE=0, VSAT=1, VVAL=2};              // HSV volor vector indicies.

// Forward declarations.
//----------------------
class Vec2;
class Vec3;
class Vec4;
class Mat3;
class Mat4;

/***************************************************************************sc**
    A 2D vector class with basic overloaded operations.

    History:
        09/10/07 (Aug) - Added clamp().
        08/01/07 (Aug) - Added mix().
        12/03/04 (Aug) - Made more functions inline plus general cleanup.        
        03/20/96 (Aug) - Made operator[] inline and added a const version of the operator.
        10/02/95 (Aug/Sky) - Initial version.                           
---------------------------------------------------------------------------*ec*/
class Vec2
{                                                                         /*sc*/
    private:
        float n[2];

    public:

    // Constructors
    //-------------
    Vec2()                                      // default constructor
    {
        n[VX] = 0.0f;
        n[VY] = 0.0f;
    }

    Vec2(const float x, const float y)          // initialize to values
    {
        n[VX] = x;
        n[VY] = y;
    }

    Vec2(const float d)                         // initialize to constant
    {
        n[VX] = n[VY] = d;
    }

    Vec2(const Vec2& v)                         // copy constructor
    {
        n[VX] = v.n[VX];
        n[VY] = v.n[VY];
    }

    Vec2(const Vec3& v);                        // cast v3 to v2
    Vec2(const Vec3& v, AXIS dropAxis);         // cast v3 to v2

    // Assignment operators
    //---------------------
    Vec2& operator  = (const Vec2& v)           // assignment of a Vec2
    {
        n[VX] = v.n[VX];
        n[VY] = v.n[VY];
        return *this;
    }

    Vec2& operator += (const Vec2& v)           // incrementation by a Vec2
    {
        n[VX] += v.n[VX];
        n[VY] += v.n[VY];
        return *this;
    }

    Vec2& operator -= (const Vec2& v)           // decrementation by a Vec2
    {
        n[VX] -= v.n[VX];
        n[VY] -= v.n[VY];
        return *this;
    }

    Vec2& operator *= (const float d)           // multiplication by a constant
    {
        n[VX] *= d;
        n[VY] *= d;
        return *this;
    }

    Vec2& operator /= (const float d)           // division by a constant
    {
        float d_inv = 1.0f / d;

        n[VX] *= d_inv;
        n[VY] *= d_inv;
        return *this;
    }

    // Indexing
    //---------
    float& operator [] (int i){ return n[i]; }
    float  operator [] (int i) const { return n[i]; }

    // Functions
    //----------
    float length() const            // length of a Vec2
    {
        return static_cast<float>(sqrt( static_cast<double>(length2()) ));
    }

    float length2() const           // squared length of a vec2
    {
        return n[VX]*n[VX] + n[VY]*n[VY];
    }

    Vec2& clamp(float lower, float upper) // clamp to range.
    {
        n[VX] = DSmath::clamp(lower, upper, n[VX]);
        n[VY] = DSmath::clamp(lower, upper, n[VY]);

        return *this;
    }

    Vec2& normalize()               // normalize a vec2
    {
        // It is up to the caller to avoid divide-by-zero
        *this /= length();
        return *this;
    }

    bool safe_normalize()           // check and normalize a vec2
    {
        float len2 = length2();
        if (len2 > 0.0f)
            *this /= static_cast<float>(sqrt( static_cast<double>(len2) ));
        else
            return false;

        return true;
    }

    Vec2& apply(V_FCT_PTR fct)      // apply a function to each component
    {
        n[VX] = (*fct)(n[VX]);
        n[VY] = (*fct)(n[VY]);
        return *this;
    }

    // Friends
    //--------
    friend Vec2 operator - (const Vec2& v);                 // -v1
    friend Vec2 operator + (const Vec2& a, const Vec2& b);  // v1 + v2
    friend Vec2 operator - (const Vec2& a, const Vec2& b);  // v1 - v2
    friend Vec2 operator * (const Vec2& a, const float d);  // v1 * 3.0
    friend Vec2 operator * (const float d, const Vec2& a);  // 3.0 * v1
    friend Vec2 operator * (const Mat3& a, const Vec2& v);  // M . v
    friend Vec2 operator * (const Vec2& v, const Mat3& a);  // v . M
    friend float operator * (const Vec2& a, const Vec2& b); // dot product
    friend Vec2 operator / (const Vec2& a, const float d);  // v1 / 3.0
    friend Vec3 operator ^ (const Vec2& a, const Vec2& b);  // cross product
    friend int operator == (const Vec2& a, const Vec2& b);  // v1 == v2 ?
    friend int operator != (const Vec2& a, const Vec2& b);  // v1 != v2 ?
    friend void swap(Vec2& a, Vec2& b);                     // swap v1 & v2
    friend Vec2 vmin(const Vec2& a, const Vec2& b);         // min(v1, v2)
    friend Vec2 vmax(const Vec2& a, const Vec2& b);         // max(v1, v2)
    friend Vec2 prod(const Vec2& a, const Vec2& b);         // term by term *
    friend Vec2 mix(const Vec2& a, const Vec2& b, float mixval); // mix vectors

    // Necessary friend declarations
    //------------------------------
    friend class Vec3;
};                                                                        /*ec*/

/***************************************************************************sc**
    A 3D vector class with basic overloaded operations.

    History:
        09/10/07 (Aug) - Added clamp().
        12/03/04 (Aug) - Moved friend functions into cpp file because MS .NET
            compiler is broken.  Also, general cleanup.
        10/28/97 (Aug) - Updated blank CTOR to init members to 0.0
        08/27/96 (Aug) - Inlined some of the constructors and operators to help
            increase DarkTree performance.
        03/20/96 (Aug) - Made operator[] inline and added a const version of
            the operator.
        10/02/95 (Aug/Sky) - Initial version.                           
---------------------------------------------------------------------------*ec*/
class Vec3
{                                                                         /*sc*/
    protected:
        float n[3];

    public:

    // Constructors
    //-------------
    Vec3()                                  // default constructor
    {
        n[VX] = 0.0f;
        n[VY] = 0.0f;
        n[VZ] = 0.0f;
    }
    
                                            // initialize to values
    Vec3(const float x, const float y, const float z)
    {
        n[VX] = x;
        n[VY] = y;
        n[VZ] = z;
    }

    Vec3(const float d)                     // initialize to constant
    {
        n[VX] = n[VY] = n[VZ] = d;
    }

    Vec3(const Vec3& v)                     // copy constructor
    {
        n[VX] = v.n[VX];
        n[VY] = v.n[VY];
        n[VZ] = v.n[VZ];
    }

    Vec3(const Vec2& v);                    // cast v2 to v3
    Vec3(const Vec2& v, float d);           // cast v2 to v3
    Vec3(const Vec4& v);                    // cast v4 to v3
    Vec3(const Vec4& v, AXIS dropAxis);     // cast v4 to v3

    // Assignment operators
    //---------------------
    Vec3& operator     = (const Vec3& v)    // assignment of a Vec3
    {
        n[VX] = v.n[VX];
        n[VY] = v.n[VY];
        n[VZ] = v.n[VZ];
        return *this;
    }

    Vec3& operator += (const Vec3& v)       // incrementation by a Vec3
    {
        n[VX] += v.n[VX];
        n[VY] += v.n[VY];
        n[VZ] += v.n[VZ];
        return *this;
    }

    Vec3& operator -= (const Vec3& v)       // decrementation by a Vec3
    {
        n[VX] -= v.n[VX];
        n[VY] -= v.n[VY];
        n[VZ] -= v.n[VZ];
        return *this;
    }

    Vec3& operator *= (const float d)       // multiplication by a constant
    {
        n[VX] *= d;
        n[VY] *= d;
        n[VZ] *= d;
        return *this;
    }

    Vec3& operator /= (const float d)       // division by a constant
    {
        float d_inv = 1.0f / d;
        n[VX] *= d_inv;
        n[VY] *= d_inv;
        n[VZ] *= d_inv;
        return *this;
    }

    // Indexing
    //---------
    float& operator [] (int i)       { return n[i]; }
    float  operator [] (int i) const { return n[i]; }

    // Functions
    //----------
    float length2() const
    {
        return n[VX]*n[VX] + n[VY]*n[VY] + n[VZ]*n[VZ];
    }

    float length() const
    {
        return (float)sqrt( (double)length2() );
    }

    Vec3& clamp(float lower, float upper) // clamp to range.
    {
        n[VX] = DSmath::clamp(lower, upper, n[VX]);
        n[VY] = DSmath::clamp(lower, upper, n[VY]);
        n[VZ] = DSmath::clamp(lower, upper, n[VZ]);

        return *this;
    }

    Vec3& normalize()
    {
        *this /= length();
        return *this;
    }

    bool safe_normalize()
    {
        float len2 = length2();

        if (len2 > 0.0f)
            *this /= static_cast<float>(sqrt( static_cast<double>(len2) ));
        else
            return false;

        return true;
    }

    Vec3& apply(V_FCT_PTR fct)     // Apply a function to each component
    {
        n[VX] = (*fct)(n[VX]);
        n[VY] = (*fct)(n[VY]);
        n[VZ] = (*fct)(n[VZ]);
        return *this;
    }

    // Friends
    //--------
    friend Vec3 operator - (const Vec3& v);                 // -v1
    friend Vec3 operator + (const Vec3& a, const Vec3& b);  // v1 + v2
    friend Vec3 operator - (const Vec3& a, const Vec3& b);  // v1 - v2
    friend Vec3 operator * (const Vec3& a, const float d);  // v1 * 3.0
    friend Vec3 operator * (const float d, const Vec3& a);  // 3.0 * v1
    friend Vec3 operator * (const Mat4& a, const Vec3& v);  // M . v
    friend Vec3 operator * (const Vec3& v, Mat4& a);        // v . M
    friend float operator * (const Vec3& a, const Vec3& b); // dot product
    friend Vec3 operator / (const Vec3& a, const float d);  // v1 / 3.0
    friend Vec3 operator ^ (const Vec3& a, const Vec3& b);  // cross product
    friend int operator == (const Vec3& a, const Vec3& b);  // v1 == v2 ?
    friend int operator != (const Vec3& a, const Vec3& b);  // v1 != v2 ?
    friend void swap(Vec3& a, Vec3& b);                     // swap v1 & v2
    friend Vec3 vmin(const Vec3& a, const Vec3& b);         // min(v1, v2)
    friend Vec3 vmax(const Vec3& a, const Vec3& b);         // max(v1, v2)
    friend Vec3 prod(const Vec3& a, const Vec3& b);         // term by term *
    friend Vec3 mix(const Vec3& a, const Vec3& b, float mixval); // mix vectors

    // Necessary friend declarations
    //------------------------------
    friend class Vec2;
    friend class Vec4;
    friend class Mat3;
    friend Vec2 operator * (const Mat3& a, const Vec2& v);  // linear transform
    friend Mat3 operator * (Mat3& a, Mat3& b);              // matrix 3 product
};                                                                        /*ec*/

/***************************************************************************sc**
    A 4D vector class with basic overloaded operations.

    History:
        09/10/07 (Aug) - Added clamp().
        08/01/07 (Aug) - Added mix().
        12/03/04 (Aug) - Moved friend functions into cpp file because MS .NET
            compiler is broken.  Also, general cleanup.
        03/20/96 (Aug) - Made operator[] inline and added a const version of
            the operator.
        10/02/95 (Aug/Sky) - Initial version.                           
---------------------------------------------------------------------------*ec*/
class Vec4
{                                                                         /*sc*/
    protected:
        float n[4];

    public:

    // Constructors
    //-------------
    Vec4()                                  // default constructor
    {
        n[VX] = 0.0f;
        n[VY] = 0.0f;
        n[VZ] = 0.0f;
        n[VW] = 0.0f;
    }

                                            // initialize to values
    Vec4(const float x, const float y, const float z, const float w)
    {
        n[VX] = x;
        n[VY] = y;
        n[VZ] = z;
        n[VW] = w;
    }

    Vec4(const float d)                     // initialize to constant
    {
        n[VX] = n[VY] = n[VZ] = n[VW] = d;
    }

    Vec4(const Vec4& v)                     // copy constructor
    {
        n[VX] = v.n[VX];
        n[VY] = v.n[VY];
        n[VZ] = v.n[VZ];
        n[VW] = v.n[VW];
    }

    Vec4(const Vec3& v)                     // cast Vec3 to Vec4
    {
        n[VX] = v.n[VX];
        n[VY] = v.n[VY];
        n[VZ] = v.n[VZ];
        n[VW] = (float)1.0;
    }

    Vec4(const Vec3& v, const float d)      // cast Vec3 to Vec4
    {
        n[VX] = v.n[VX];
        n[VY] = v.n[VY];
        n[VZ] = v.n[VZ];
        n[VW] = d;
    }


    // Assignment operators
    //---------------------
    Vec4& operator     = (const Vec4& v)    // assignment of a Vec4
    {
        n[VX] = v.n[VX];
        n[VY] = v.n[VY];
        n[VZ] = v.n[VZ];
        n[VW] = v.n[VW];
        return *this;
    }

    Vec4& operator += (const Vec4& v)       // incrementation by a Vec4
    {
        n[VX] += v.n[VX];
        n[VY] += v.n[VY];
        n[VZ] += v.n[VZ];
        n[VW] += v.n[VW];
        return *this;
    }

    Vec4& operator -= (const Vec4& v)       // decrementation by a Vec4
    {
        n[VX] -= v.n[VX];
        n[VY] -= v.n[VY];
        n[VZ] -= v.n[VZ];
        n[VW] -= v.n[VW];
        return *this;
    }

    Vec4& operator *= (const float d)       // multiplication by a constant
    {
        n[VX] *= d;
        n[VY] *= d;
        n[VZ] *= d;
        n[VW] *= d;
        return *this;
    }

    Vec4& operator /= (const float d)       // division by a constant
    {
        float di = 1.0f / d;
       n[VX] *= di;
       n[VY] *= di;
       n[VZ] *= di;
       n[VW] *= di;
       return *this;
    }

    // Indexing
    //---------
    float& operator [] (int i){ return n[i]; }
    float  operator [] (int i) const { return n[i]; } 

    Vec4& clamp(float lower, float upper) // clamp to range.
    {
        n[VX] = DSmath::clamp(lower, upper, n[VX]);
        n[VY] = DSmath::clamp(lower, upper, n[VY]);
        n[VZ] = DSmath::clamp(lower, upper, n[VZ]);
        n[VW] = DSmath::clamp(lower, upper, n[VW]);

        return *this;
    }

    // Length/normalize functions.
    //----------------------------
    float length2() const
    {
        return n[VX]*n[VX] + n[VY]*n[VY] + n[VZ]*n[VZ] + n[VW]*n[VW];
    }
    
    float length()  const
    {
        return (float)sqrt( (double)length2() );
    }
    
    Vec4& normalize()
    {
        *this /= length();
        return *this;
    }
    
    bool safe_normalize()
    {
        float len2 = length2();
        if(len2 > 0.0f)
            *this /= static_cast<float>(sqrt( static_cast<double>(len2) ));
        else
            return false;
        
        return true;
    }

    Vec4& apply(V_FCT_PTR fct)      // Apply a function to each component
    {
        n[VX] = (*fct)(n[VX]);
        n[VY] = (*fct)(n[VY]);
        n[VZ] = (*fct)(n[VZ]);
        n[VW] = (*fct)(n[VW]);
        return *this;
    }

    // Friends
    //--------
    friend Vec4 operator - (const Vec4& v);                 // -v1
    friend Vec4 operator + (const Vec4& a, const Vec4& b);  // v1 + v2
    friend Vec4 operator - (const Vec4& a, const Vec4& b);  // v1 - v2
    friend Vec4 operator * (const Vec4& a, const float d);  // v1 * 3.0
    friend Vec4 operator * (const float d, const Vec4& a);  // 3.0 * v1
    friend Vec4 operator * (const Mat4& a, const Vec4& v);  // M . v
    friend Vec4 operator * (const Vec4& v, Mat4& a);        // v . M
    friend float operator * (const Vec4& a, const Vec4& b); // dot product
    friend Vec4 operator / (const Vec4& a, const float d);  // v1 / 3.0
    friend int operator == (const Vec4& a, const Vec4& b);  // v1 == v2 ?
    friend int operator != (const Vec4& a, const Vec4& b);  // v1 != v2 ?
    friend void swap(Vec4& a, Vec4& b);                     // swap v1 & v2
    friend Vec4 vmin(const Vec4& a, const Vec4& b);         // min(v1, v2)
    friend Vec4 vmax(const Vec4& a, const Vec4& b);         // max(v1, v2)
    friend Vec4 prod(const Vec4& a, const Vec4& b);         // term by term *
    friend Vec4 mix(const Vec4& a, const Vec4& b, float mixval); // mix vectors

    // Necessary friend declarations
    //------------------------------
    friend class Vec3;
    friend class Mat4;
    friend Vec3 operator * (const Mat4& a, const Vec3& v);  // linear transform
    friend Mat4 operator * (Mat4& a, Mat4& b);              // matrix 4 product
};                                                                        /*ec*/

/***************************************************************************sc**
    A 3D matrix class with basic overloaded operations.

    History:
        12/03/04 (Aug) - Moved friend functions into cpp file because MS .NET
            compiler is broken.  Also, general cleanup.
        03/20/96 (Aug) - Made operator[] inline and added a const version of 
            the operator.
        10/02/95 (Aug) - Initial version.                           
---------------------------------------------------------------------------*ec*/
class Mat3
{                                                                         /*sc*/
    protected:
        Vec3 v[3];

    public:

    // Constructors
    //-------------
    Mat3();
    Mat3(const Vec3& v0, const Vec3& v1, const Vec3& v2);
    Mat3(const float d);
    Mat3(const Mat3& m);

    // Assignment operators
    //---------------------
    Mat3& operator  = (const Mat3& m);          // assignment of a Mat3
    Mat3& operator += (const Mat3& m);          // incrementation by a Mat3
    Mat3& operator -= (const Mat3& m);          // decrementation by a Mat3
    Mat3& operator *= (const float d);          // multiplication by a constant
    Mat3& operator /= (const float d);          // division by a constant
    
    // Indexing
    //---------
    Vec3& operator [] (int i){ return v[i]; }
    Vec3  operator [] (int i) const { return v[i]; }

    // Functions
    //----------
    Mat3 transpose();                           // transpose
    Mat3 inverse();                             // inverse
    Mat3& apply(V_FCT_PTR fct);                 // Apply a function to each element

    // Friends
    //--------
    friend Mat3 operator - (const Mat3& a);                 // -m1
    friend Mat3 operator + (const Mat3& a, const Mat3& b);  // m1 + m2
    friend Mat3 operator - (const Mat3& a, const Mat3& b);  // m1 - m2
    friend Mat3 operator * (Mat3& a, Mat3& b);              // m1 * m2
    friend Mat3 operator * (const Mat3& a, const float d);  // m1 * 3.0
    friend Mat3 operator * (const float d, const Mat3& a);  // 3.0 * m1
    friend Mat3 operator / (const Mat3& a, const float d);  // m1 / 3.0
    friend int operator == (const Mat3& a, const Mat3& b);  // m1 == m2 ?
    friend int operator != (const Mat3& a, const Mat3& b);  // m1 != m2 ?
    friend void swap(Mat3& a, Mat3& b);                     // swap m1 & m2

    // Necessary friend declarations
    //------------------------------
    /*TODO - implement later if needed.
    friend Vec3 operator * (const Mat3& a, const Vec3& v);  // linear transform
    */
    friend Vec2 operator * (const Mat3& a, const Vec2& v);  // linear transform
};                                                                        /*ec*/

/***************************************************************************sc**
    A 4D matrix class with basic overloaded operations.

    History:
        12/03/04 (Aug) - Moved friend functions into cpp file because MS .NET 
            compiler is broken.  Also, general cleanup.
        03/20/96 (Aug) - Made operator[] inline and added a const version of 
            the operator.
        10/02/95 (Aug/Sky) - Initial version.                           
---------------------------------------------------------------------------*ec*/
class Mat4
{                                                                         /*sc*/
    protected:
        Vec4 v[4];

    public:

    // Constructors
    //-------------
    Mat4(){}

    Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        v[3] = v3;
    }

    Mat4(const float d)
    {
        v[0] = v[1] = v[2] = v[3] = Vec4(d);
    }

    Mat4(const Mat4& m)
    {
        v[0] = m.v[0];
        v[1] = m.v[1];
        v[2] = m.v[2];
        v[3] = m.v[3];
    }

    // Assignment operators
    //---------------------
    Mat4& operator  = (const Mat4& m)       // assignment of a Mat4
    {
        v[0] = m.v[0];
        v[1] = m.v[1];
        v[2] = m.v[2];
        v[3] = m.v[3];
        return *this;
    }

    Mat4& operator += (const Mat4& m)       // incrementation by a Mat4
    {
        v[0] += m.v[0];
        v[1] += m.v[1];
        v[2] += m.v[2];
        v[3] += m.v[3];
        return *this;
    }

    Mat4& operator -= (const Mat4& m)       // decrementation by a Mat4
    {
        v[0] -= m.v[0];
        v[1] -= m.v[1];
        v[2] -= m.v[2];
        v[3] -= m.v[3];
        return *this;
    }

    Mat4& operator *= (const float d)       // multiplication by a constant
    {
        v[0] *= d;
        v[1] *= d;
        v[2] *= d;
        v[3] *= d;
        return *this;
    }

    Mat4& operator /= (const float d)       // division by a constant
    {
        v[0] /= d;
        v[1] /= d;
        v[2] /= d;
        v[3] /= d;
        return *this;
    }

    Vec4& operator [] (int i){ return v[i]; }   // indexing
    Vec4  operator [] (int i) const { return v[i]; }


    // Functions
    //----------
    Mat4 transpose()                    // transpose
    {
        return Mat4(Vec4(v[0][0], v[1][0], v[2][0], v[3][0]),
                    Vec4(v[0][1], v[1][1], v[2][1], v[3][1]),
                    Vec4(v[0][2], v[1][2], v[2][2], v[3][2]),
                    Vec4(v[0][3], v[1][3], v[2][3], v[3][3]));
    }

    Mat4 inverse();                     // inverse

    Mat4& apply(V_FCT_PTR fct)          // apply a function to each element
    {
        v[VX].apply(fct);
        v[VY].apply(fct);
        v[VZ].apply(fct);
        v[VW].apply(fct);
        return *this;
    }

    // Friends
    //--------
    friend Mat4 operator - (const Mat4& a);                 // -m1
    friend Mat4 operator + (const Mat4& a, const Mat4& b);  // m1 + m2
    friend Mat4 operator - (const Mat4& a, const Mat4& b);  // m1 - m2
    friend Mat4 operator * (Mat4& a, Mat4& b);              // m1 * m2
    friend Mat4 operator * (const Mat4& a, const float d);  // m1 * 4.0
    friend Mat4 operator * (const float d, const Mat4& a);  // 4.0 * m1
    friend Mat4 operator / (const Mat4& a, const float d);  // m1 / 3.0
    friend int operator == (const Mat4& a, const Mat4& b);  // m1 == m2 ?
    friend int operator != (const Mat4& a, const Mat4& b);  // m1 != m2 ?
    friend void swap(Mat4& a, Mat4& b);                     // swap m1 & m2

    // Necessary friend declarations
    //------------------------------
    friend Vec4 operator * (const Mat4& a, const Vec4& v);  // linear transform
    friend Vec4 operator * (const Vec4& v, Mat4& a);        // v . M
    friend Vec3 operator * (const Mat4& a, const Vec3& v);  // linear transform
};                                                                        /*ec*/

/***************************************************************************sc**
    General matrix and vector functions.

    History:
        03/05/04 (Aug) - Made Vec arguments const& where possible.
        10/11/95 (Aug/Sky) - Initial version.                           
---------------------------------------------------------------------------*ec*/
Vec3 HSVtoRGB(const Vec3& HSVvals);                         // convert HSV to RGB
Vec3 RGBtoHSV(const Vec3& RGBvals);                         // convert RGB to HSV
Vec3 HSVtoRGBnorm(const Vec3& HSVvals);                     // convert HSV to RGB
Vec3 RGBtoHSVnorm(const Vec3& RGBvals);                     // convert RGB to HSV
Mat3 identity2D(void);                                      // identity 2D
Mat3 translation2D(const Vec2& v);                          // translation 2D
Mat3 rotation2D(const Vec2& Center, const float angleRad);  // rotation 2D
Mat3 scaling2D(const Vec2& scaleVector);                    // scaling 2D
Mat4 identity3D(void);                                      // identity 3D
Mat4 translation3D(const Vec3& v);                          // translation 3D
Mat4 rotation3D(Vec3 Axis, const float angleRad);           // rotation 3D
Mat4 scaling3D(const Vec3& scaleVector);                    // scaling 3D
Mat4 perspective3D(const float d);                          // perspective 3D

} //namespace DSmath

#endif //DS_VMAT_H
