
#ifndef DS_TFORM_H
#define DS_TFORM_H

#include "vmat.h"

namespace DSmath
{

/*sc****************************************************************************
--------------------------------------------------------------------------------
    Name: Tform
--------------------------------------------------------------------------------

    Description: This class is a transformation container that allows the user
     to set and get the transformation state, concatenate transforms together,
     and apply those transformations to points and vectors.  Each transform has
     a globally unique ID number.  Any function that changes the transform
     should update this number by calling the NewID() function.  The copy CTOR
     and assignment operator SHOULD NOT update the ID since the new or copied
     transform technically contains the same data.

    Revision History:
        Rev.006 08/28/02 (Aug) - Removed get(scale, rotate, translate) as
         it is no longer in use.
        Rev.005 02/11/98 (Random) - Added a flag for keeping the internal
          inverse matrix up to date.  This should be used by all code except the
          local transforms for the DarkTrees.
        Rev.004 07/25/97 (SkylerS) - Added point transform optimizations
         and added permanent inverse transform matrix.  This should help our
         caching optimizations.
        Rev.003 07/23/97 (Aug) - Added global unique ID.
        Rev.002 05/26/97 (SkylerS) - Removed optimizations and went to
         plain old 4x4 matricies.
        Rev.001 04/07/96 (SkylerS) - Initial version.

**ec***************************************************************************/
class  Tform
{                                                                         /*sc*/
    private:
        Mat4                MT, MT_I;   // The transform matrix and its inverse.
        static unsigned int _GlobalID;  // Global unique transform identifier.
        unsigned int        _ID;        // Local identifier.

        void NewID(void){ _ID=_GlobalID; _GlobalID++; }

    protected:
    public:

        // Types of transformations that can be done.
        enum ttype { RX, RY, RZ, SX, SY, SZ, TX, TY, TZ };

        // Initialization        
        Tform();
        void clear();

        // Transform access functions.
        void set( Vec3 scale, Vec3 rotate, Vec3 translate );
        void set( Mat4 transform, bool invertFlag = true )
            { MT = transform; if (invertFlag == true) MT_I = MT.inverse(); NewID(); }

        Mat4 get( void ){ return MT; }

        // Transform inversion.
        void invert( void ){ swap(MT, MT_I); NewID(); }

        // Transformations about a given axis.
        void vector_rotate( Vec3 &axis, float radians );
        void vector_scale( Vec3 &scale );
        void vector_translate( Vec3 &dir );

        // Transform concatenation functions.
        void cat( ttype which, float amount, bool invertFlag = true );

        void cat( Mat4 matrix, bool invertFlag = true )
            { MT = matrix * MT; if (invertFlag == true) MT_I = MT.inverse(); NewID(); }

        void cat( Tform transform, bool invertFlag = true )
            { MT = transform.MT * MT; if (invertFlag == true) MT_I = MT.inverse(); NewID(); }

        // Transform application functions.
        void transform( Vec3 *point );
        void inverse_transform( Vec3 *point );
        void vector_transform( Vec3 *vector );

        // Transform identification retrieval.
        unsigned int GetID(void){ return (_ID); }
};                                                                        /*ec*/

} //namespace DSmath

#endif //DS_TFORM_H
