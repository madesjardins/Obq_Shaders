//------------------------------------------------------------------------------
// Copyright 2002-2008 Darkling Simulations, LLC
//------------------------------------------------------------------------------

#ifndef DS_DTE_DTENGINE_H
#define DS_DTE_DTENGINE_H

#include <cstddef>
#include "dte\vmat.h"

//Export specification.
#ifdef _MSC_VER
#ifdef DLL_BUILD
#define DTE_EXPORT extern "C" __declspec(dllexport)
#else
#define DTE_EXPORT extern "C" __declspec(dllimport)
#endif
#else
#define DTE_EXPORT extern "C"
#endif

//The API for the DarkTree Engine (DTE).
namespace DSdte
{

/***************************************************************************sc**
    Provides C structures with the same data profile as the math vector, matrix,
    and transform classes in order to keep the API functions C only. The minimum
    set of explicit conversion functions needed is also required.

    History:
        09/29/04 (JL) - Added Vec4ToStruct() and Mat4ToStruct().
        09/21/04 (JL) - Added Vec4 and Mat4 conversion functions.
        03/16/04 (AAS) - Made Vec conversion arguments const.
        01/14/03 (AAS) - Initial version.
---------------------------------------------------------------------------*ec*/
struct Vec3_struct
{                                                                         /*sc*/
    float   n[3];
};                                                                        /*ec*/

inline Vec3_struct Vec3ToStruct(const DSmath::Vec3 &v)
{                                                                         /*sc*/
    Vec3_struct    tmp;

    tmp.n[0] = v[0];
    tmp.n[1] = v[1];
    tmp.n[2] = v[2];

    return tmp;
}                                                                         /*ec*/

inline DSmath::Vec3 StructToVec3(const Vec3_struct &s)
{                                                                         /*sc*/
    return DSmath::Vec3(s.n[0], s.n[1], s.n[2]);
}                                                                         /*ec*/

struct Vec4_struct
{                                                                         /*sc*/
    float   n[4];
};                                                                        /*ec*/

inline Vec4_struct Vec4ToStruct(const DSmath::Vec4 &v)
{                                                                         /*sc*/
    Vec4_struct    tmp;

    tmp.n[0] = v[0];
    tmp.n[1] = v[1];
    tmp.n[2] = v[2];
    tmp.n[3] = v[3];

    return tmp;
}                                                                        /*ec*/

inline DSmath::Vec4 StructToVec4(const Vec4_struct &s)
{                                                                         /*sc*/
    return DSmath::Vec4(s.n[0], s.n[1], s.n[2], s.n[3]);
}                                                                         /*ec*/

struct Mat4_struct
{                                                                         /*sc*/
    Vec4_struct     v[4];
};                                                                        /*ec*/

inline Mat4_struct Mat4ToStruct(const DSmath::Mat4 &m)
{                                                                         /*sc*/
    Mat4_struct    tmp;

    tmp.v[0] = Vec4ToStruct(m[0]);
    tmp.v[1] = Vec4ToStruct(m[1]);
    tmp.v[2] = Vec4ToStruct(m[2]);
    tmp.v[3] = Vec4ToStruct(m[3]);

    return tmp;
}                                                                        /*ec*/

inline DSmath::Mat4 StructToMat4(const Mat4_struct &s)
{                                                                         /*sc*/
    return DSmath::Mat4(StructToVec4(s.v[0]), StructToVec4(s.v[1]),
                        StructToVec4(s.v[2]), StructToVec4(s.v[3]));
}                                                                         /*ec*/

/***************************************************************************sc**
    DarkTree texture types currently supported by the engine. 

    Input:

    Output:

    History:
        08/14/02 (AAS) - Initial version.
---------------------------------------------------------------------------*ec*/
enum DTE_TYPE
{                                                                         /*sc*/
    DTE_EMPTY   = 0,
    DTE_COLOR   = 1, 
    DTE_PERCENT = 2,
    DTE_BUMP    = 3,
    DTE_SHADED  = 4
};                                                                        /*ec*/

/***************************************************************************sc**
    Surface data types that can be returned by the engine. If the loaded
    DarkTree type is a shader, all of these types will be available. If the
    loaded DarkTree is color or percent, only the COLOR and PERCENT types will
    be available. If a bump DarkTree is loaded, both BUMP and ELEVATION will be
    available. If a shaded DarkTree is loaded, evaluating percent will return
    the diffuse level.

    History:
        08/14/02 (AAS) - Initial version.
---------------------------------------------------------------------------*ec*/
enum DTE_EVALUATE
{                                                                         /*sc*/
    DTE_EVAL_COLOR          = 0, 
    DTE_EVAL_PERCENT        = 1, 
    DTE_EVAL_ELEVATION      = 2,
    DTE_EVAL_DIFFUSE_LEVEL  = 3,
    DTE_EVAL_DIFFUSE_FUNC   = 4,
    DTE_EVAL_LUMINOSITY     = 5,
    DTE_EVAL_SPECULAR_LEVEL = 6,
    DTE_EVAL_SPECULAR_FUNC  = 7,
    DTE_EVAL_SPECULAR_COLOR = 8,
    DTE_EVAL_GLOSSINESS     = 9,
    DTE_EVAL_METAL_LEVEL    = 10,
    DTE_EVAL_ANISOTROPY     = 11,
    DTE_EVAL_ANISO_DIR      = 12,
    DTE_EVAL_REFLECTIVITY   = 13,
    DTE_EVAL_ENVIRONMENT    = 14,
    DTE_EVAL_TRANSPARENCY   = 15,
    DTE_EVAL_REFRACTION     = 16,
    DTE_EVAL_CC_LEVEL       = 17,
    DTE_EVAL_CC_GLOSSINESS  = 18,
    DTE_EVAL_CC_THICKNESS   = 19,
    DTE_EVAL_CC_SMOOTHNESS  = 20,
    DTE_EVAL_BUMP           = 21,
    DTE_EVAL_ALPHA          = 22
};                                                                        /*ec*/

/***************************************************************************sc**
    Currently supported tweak control types. Mapping and sampling types are
    represented as unsigned integers and defined as follows.

     Mapping    0 - Planar 2D
                1 - Cylindrical 2D
                2 - Spherical 2D
                3 - Planar 3D

     Sampling   0 - Normal
                1 - Recenter
                2 - Single Sample

    History:
        08/14/02 (AAS) - Initial version.
---------------------------------------------------------------------------*ec*/
enum DTE_TWEAK_TYPE
{                                                                         /*sc*/
    DTE_TWEAK_UNKNOWN   = 0,
    DTE_TWEAK_FLOAT     = 1,
    DTE_TWEAK_INT       = 2,
    DTE_TWEAK_BOOL      = 3,
    DTE_TWEAK_STRING    = 4,
    DTE_TWEAK_COLOR     = 5,
    DTE_TWEAK_PERCENT   = 6,
    DTE_TWEAK_BUMP      = 7,
    DTE_TWEAK_MAPPING   = 8,
    DTE_TWEAK_SAMPLING  = 9
};                                                                        /*ec*/

/***************************************************************************sc**
    Scale units defined for the texture. For reference, the preview in the
    DarkTree application is a -1.0 to 1.0 X/Y window.

    History:
        08/14/02 (AAS) - Initial version.
---------------------------------------------------------------------------*ec*/
enum DTE_SCALE_UNIT
{                                                                         /*sc*/
    DTE_INCH        = 0,
    DTE_FOOT        = 1,
    DTE_MILE        = 2,
    DTE_MILLIMETER  = 3,
    DTE_CENTIMETER  = 4,
    DTE_METER       = 5,
    DTE_KILOMETER   = 6
};                                                                        /*ec*/

/***************************************************************************sc**
    Provides a structure with the same profile as the RenderState class in order
    to keep the API functions C only.

     NOTE - the data profile must match RenderState exactly.

    Input:

    Output:

    History:
        09/21/04 (JL) - Initial version.
---------------------------------------------------------------------------*ec*/
struct RenderState_struct
{                                                                         /*sc*/
        //Required
        //--------
        Vec3_struct     point; 
        Vec3_struct     normal;      
        float           sampleSize;     
        float           bumpScale;   
        float           frameNumber;    
        Vec3_struct     incident;      
        float           cameraDistanceP; 
        DTE_EVALUATE    evaluateAs; 

        //Optional
        //--------
        float           continuousRep;  //Continuous rep value for function eval.
        unsigned int    regionRep;      //Regional rep for control component eval.
};                                                                        /*ec*/

/***************************************************************************sc**
    Render state information structure for texture evaluation.

    History:
        08/14/02 (AAS) - Initial version.
---------------------------------------------------------------------------*ec*/
class RenderState
{                                                                         /*sc*/
    public:
        RenderState(): point(0.0f, 0.0f, 0.0f),
                       normal(0.0f, 0.0f, -1.0f),
                       sampleSize(0.00001f),
                       bumpScale(1.0f),
                       frameNumber(1.0f),
                       incident(0.0f, 0.0f, 1.0f),
                       cameraDistanceP(0.0f),
                       evaluateAs(DTE_EVAL_COLOR),
                       continuousRep(0.0f),
                       regionRep(0)
                       {}

        //Required
        //--------
        DSmath::Vec3    point;              //Transformed local point.
        DSmath::Vec3    normal;             //Surface unit normal.
        float           sampleSize;         //Rough measure of sample size.
        float           bumpScale;          //Bump scaling factor.
        float           frameNumber;        //Frame number.
        DSmath::Vec3    incident;           //Surface incident unit vector.
        float           cameraDistanceP;    //Camera distance (percent).
        DTE_EVALUATE    evaluateAs;         //Type of evaluation.

        //Optional
        //--------
        float           continuousRep;      //Continuous rep value for function eval.
        unsigned int    regionRep;          //Regional rep for control component eval.
};                                                                        /*ec*/

/***************************************************************************sc**
    Provides a structure with the same profile as the DynamicTweakValue class in
    order to keep the API functions C only.

     NOTE - the data profile must match DynamicTweakValue exactly.

    Input:

    Output:

    History:
        09/21/04 (JL) - Initial version.
---------------------------------------------------------------------------*ec*/
struct DynamicTweakValue_struct
{                                                                         /*sc*/
        int   Tag; 
        void  *pValue;
};                                                                        /*ec*/

/***************************************************************************sc**
    Holds a tweak access tag/value pair for setting tweak values on a per-pixel
    basis via Evaluate(). This structure is defined for efficiency and pValue
    -must- point to the tweak type associated with the access tag.

    History:
        08/14/02 (AAS) - Initial version.
---------------------------------------------------------------------------*ec*/
class DynamicTweakValue
{                                                                         /*sc*/
    public:
        DynamicTweakValue(): Tag(-1),
                             pValue(NULL)
                             {}

        //Public members.
        //---------------
        int   Tag ;     //The access tag of the tweak.
        void  *pValue;  //The value of the tweak.
};                                                                        /*ec*/

/***************************************************************************sc**
    This defines a function pointer that can be used for message handling.
    Messages internal to the instance, either due to an error or generated by a
    component, will call the function with the following arguments.

      NOTE - The message handler function is shared between all DTE instances
             and may need to be thread safe, depending on what you are doing.

    Input:
        instance - the instance pointer.  Can be used to identify the originator
                   of the message in cases where the message function pointer
                   is shared.  Some internal messages and component messages
                   will set this value to NULL.
        severity - the severity of the message.  Supported values are in the 
                   range 0 through 3 (0=fatal, 1=error, 2=warning, 3=info).
        source   - text describing the source of the message.
        message  - the message itself.

    Output:

    History:
        04/07/03 (AAS) - Initial version.
---------------------------------------------------------------------------*ec*/
typedef void (*FN_POSTMESSAGE)(void *instance,
                               int severity,
                               const char* const source,
                               const char* const message);

//DarkTreeEngine function interface.
//-------------------------------------------------------------
//-------------------------------------------------------------

//Versioning.
//-----------
DTE_EXPORT unsigned int    Version(void);

//Initialization.
//---------------
DTE_EXPORT bool            Initialize(void);

//Component management.
//---------------------
DTE_EXPORT bool            ComponentLibsInUse(void);
DTE_EXPORT void            ComponentLibsClear(void);
DTE_EXPORT bool            ComponentLibsAdd(const char* const path);
DTE_EXPORT bool            ComponentLibsAddSingle(const char* const fn);
DTE_EXPORT void            ComponentLibsRebuild(void);

//Construction, Destruction, Assignment.
//--------------------------------------
DTE_EXPORT void*           Create(unsigned int numCPU, unsigned int numLock);
DTE_EXPORT void*           CreateCopy(void *dteSource);
DTE_EXPORT void            Destroy(void *dte);
DTE_EXPORT void            Assign(void *dteDestination, void *dteSource);
        
//Debugging.
//----------
DTE_EXPORT void            SetMessageHandler(FN_POSTMESSAGE handler);
DTE_EXPORT bool            CpuLimitReached(void *dte);

//Multithreading.
//---------------
DTE_EXPORT unsigned int    NumCpuSupported(void *dte);
DTE_EXPORT unsigned int    NumLockSupported(void *dte);
DTE_EXPORT int             GetUniqueLock(void *dte);
DTE_EXPORT void            ReleaseUniqueLock(void *dte, unsigned int lockID);

//Query.
//------
DTE_EXPORT bool            ComponentExists(void *dte,
                                const char* const classification,
                                const char* const name);
DTE_EXPORT bool            IsConstant(void *dte, DTE_EVALUATE evalType);
DTE_EXPORT bool            IsLinked(void *dte, DTE_EVALUATE evalType);
DTE_EXPORT DTE_TYPE        Type(void *dte);
DTE_EXPORT const char*     Name(void *dte);
DTE_EXPORT const char*     Description(void *dte);
DTE_EXPORT DTE_SCALE_UNIT  ScaleUnit(void *dte);
DTE_EXPORT float           Scale(void *dte);
DTE_EXPORT float           StartFrame(void *dte);
DTE_EXPORT float           EndFrame(void *dte);

//Operations.
//-----------
DTE_EXPORT bool             Load(void *dte, const char* const filename);
DTE_EXPORT void             Clear(void *dte);
DTE_EXPORT bool             ConvertTo(void *dte, DTE_TYPE convertType);
DTE_EXPORT void             SetTransform(void *dte, Mat4_struct &transform);

//Evaluate.
//---------
DTE_EXPORT Vec3_struct      Evaluate(void *dte, const RenderState_struct *state,
                                const DynamicTweakValue_struct *dynTweak=NULL,
                                unsigned int numDynTweak=0);

//Tweaks.
//-------
DTE_EXPORT unsigned int     Tweak_Num(void *dte);
DTE_EXPORT unsigned int     Tweak_Num_Type(void *dte, DTE_TWEAK_TYPE type);
DTE_EXPORT bool             Tweak_Exists(void *dte, const char* const name);

DTE_EXPORT int              Tweak_AccessTag(void *dte, const char* const name);
DTE_EXPORT DTE_TWEAK_TYPE   Tweak_Type(void *dte, const char* const name);

DTE_EXPORT const char*      Tweak_First(void *dte);
DTE_EXPORT const char*      Tweak_First_Type(void *dte, DTE_TWEAK_TYPE type);
DTE_EXPORT const char*      Tweak_Last(void *dte);
DTE_EXPORT const char*      Tweak_Last_Type(void *dte, DTE_TWEAK_TYPE type);
DTE_EXPORT const char*      Tweak_Prev(void *dte, const char* const currentName);
DTE_EXPORT const char*      Tweak_Prev_Type(void *dte, const char* const currentName, DTE_TWEAK_TYPE type);
DTE_EXPORT const char*      Tweak_Next(void *dte, const char* const currentName);
DTE_EXPORT const char*      Tweak_Next_Type(void *dte, const char* const currentName, DTE_TWEAK_TYPE type);

DTE_EXPORT void             Tweak_Set_Float(void *dte, unsigned int tag, float value);
DTE_EXPORT float            Tweak_Get_Float(void *dte, unsigned int tag);

DTE_EXPORT void             Tweak_Set_Int(void *dte, unsigned int tag, int value);
DTE_EXPORT int              Tweak_Get_Int(void *dte, unsigned int tag);

DTE_EXPORT void             Tweak_Set_Bool(void *dte, unsigned int tag, bool value);
DTE_EXPORT bool             Tweak_Get_Bool(void *dte, unsigned int tag);

DTE_EXPORT void             Tweak_Set_String(void *dte, unsigned int tag, const char *value);
DTE_EXPORT const char*      Tweak_Get_String(void *dte, unsigned int tag);

DTE_EXPORT void             Tweak_Set_Color(void *dte, unsigned int tag, const Vec3_struct &value);
DTE_EXPORT Vec3_struct      Tweak_Get_Color(void *dte, unsigned int tag);

} //End of "dte" namespace.

#endif //DS_DTE_DTENGINE_H
