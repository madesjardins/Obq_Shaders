#ifndef _aitag_Obq_StereoLens_h_
#define _aitag_Obq_StereoLens_h_

enum
{
   C4DAIP_OBQ_STEREOLENS_MAIN_GRP                      = 2001,
   C4DAIP_OBQ_STEREOLENS_MAIN_ATTRIBUTES_GRP          = 3001,
   C4DAIP_OBQ_STEREOLENS_VIEWMODE_GRP                 = 4001,
   C4DAIP_OBQ_STEREOLENS_CAMERAS_GRP                  = 5001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_OVERSCAN_GRP       = 6001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_TARGET_RESOLUTION_GRP = 7001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_RENDER_RESOLUTION_GRP = 8001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_NUKE_INFO_GRP      = 9001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_DEPTH_OF_FIELD_GRP = 10001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_FOCUS_DISTANCE_GRP = 11001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_APERTURE_GRP       = 12001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_BOKEH_QUALITY_GRP  = 13001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_DEBUG_GRP          = 14001,
   C4DAIP_OBQ_STEREOLENS_AUTOMATIC_OPTIONS_GRP        = 15001,
   
   C4DAIP_OBQ_STEREOLENS_VIEWMODE                     = 2011803800,
   C4DAIP_OBQ_STEREOLENS_LEFTCAMERA                   = 197392140,
   C4DAIP_OBQ_STEREOLENS_RIGHTCAMERA                  = 1961510561,
   C4DAIP_OBQ_STEREOLENS_FILMBACKX                    = 844175529,
   C4DAIP_OBQ_STEREOLENS_LEFTCENTEROFFSET             = 1747026187,
   C4DAIP_OBQ_STEREOLENS_RIGHTCENTEROFFSET            = 144804450,
   C4DAIP_OBQ_STEREOLENS_RECALCULATEDISTANCEFORSIDECAMERAS = 31262512,
   C4DAIP_OBQ_STEREOLENS_TOTALOVERSCANPIXELS          = 1019770638,
   C4DAIP_OBQ_STEREOLENS_USEDOF                       = 1114014882,
   C4DAIP_OBQ_STEREOLENS_FOCUSDISTANCE                = 576484419,
   C4DAIP_OBQ_STEREOLENS_APERTURESIZE                 = 132167995,
   C4DAIP_OBQ_STEREOLENS_APERTUREASPECTRATIO          = 2041964385,
   C4DAIP_OBQ_STEREOLENS_USEPOLYGONALAPERTURE         = 742294878,
   C4DAIP_OBQ_STEREOLENS_APERTUREBLADES               = 1534385803,
   C4DAIP_OBQ_STEREOLENS_APERTUREBLADECURVATURE       = 1032978937,
   C4DAIP_OBQ_STEREOLENS_APERTUREROTATION             = 480867856,
   C4DAIP_OBQ_STEREOLENS_FOCUSPLANEISPLANE            = 1258784012,
   C4DAIP_OBQ_STEREOLENS_BOKEHINVERT                  = 840279495,
   C4DAIP_OBQ_STEREOLENS_BOKEHBIAS                    = 1969062976,
   C4DAIP_OBQ_STEREOLENS_BOKEHGAIN                    = 1968891744,
   C4DAIP_OBQ_STEREOLENS_FOV                          = 1891353277,
   C4DAIP_OBQ_STEREOLENS_POSITION                     = 604222387,
   C4DAIP_OBQ_STEREOLENS_LOOK_AT                      = 1284360863,
   C4DAIP_OBQ_STEREOLENS_UP                           = 202987805,
   C4DAIP_OBQ_STEREOLENS_MATRIX                       = 1447855635,
   C4DAIP_OBQ_STEREOLENS_NEAR_CLIP                    = 505319067,
   C4DAIP_OBQ_STEREOLENS_FAR_CLIP                     = 1755792600,
   C4DAIP_OBQ_STEREOLENS_SHUTTER_START                = 1625782988,
   C4DAIP_OBQ_STEREOLENS_SHUTTER_END                  = 825795011,
   C4DAIP_OBQ_STEREOLENS_SHUTTER_TYPE                 = 1480880056,
   C4DAIP_OBQ_STEREOLENS_SHUTTER_CURVE                = 1644703157,
   C4DAIP_OBQ_STEREOLENS_ROLLING_SHUTTER              = 1102174883,
   C4DAIP_OBQ_STEREOLENS_ROLLING_SHUTTER_DURATION     = 2016027490,
   C4DAIP_OBQ_STEREOLENS_FILTERMAP                    = 1122302556,
   C4DAIP_OBQ_STEREOLENS_HANDEDNESS                   = 194577653,
   C4DAIP_OBQ_STEREOLENS_TIME_SAMPLES                 = 403214619,
   C4DAIP_OBQ_STEREOLENS_SCREEN_WINDOW_MIN            = 1930850958,
   C4DAIP_OBQ_STEREOLENS_SCREEN_WINDOW_MAX            = 1930851212,
   C4DAIP_OBQ_STEREOLENS_EXPOSURE                     = 1667303827,
   C4DAIP_OBQ_STEREOLENS_NAME                         = 2010123353,

   C4DAIP_OBQ_STEREOLENS_SHUTTER_TYPE__BOX            = 0,
   C4DAIP_OBQ_STEREOLENS_SHUTTER_TYPE__TRIANGLE       = 1,
   C4DAIP_OBQ_STEREOLENS_SHUTTER_TYPE__CURVE          = 2,

   C4DAIP_OBQ_STEREOLENS_ROLLING_SHUTTER__OFF         = 0,
   C4DAIP_OBQ_STEREOLENS_ROLLING_SHUTTER__TOP         = 1,
   C4DAIP_OBQ_STEREOLENS_ROLLING_SHUTTER__BOTTOM      = 2,
   C4DAIP_OBQ_STEREOLENS_ROLLING_SHUTTER__LEFT        = 3,
   C4DAIP_OBQ_STEREOLENS_ROLLING_SHUTTER__RIGHT       = 4,

   C4DAIP_OBQ_STEREOLENS_HANDEDNESS__RIGHT            = 0,
   C4DAIP_OBQ_STEREOLENS_HANDEDNESS__LEFT             = 1,
};

#endif

