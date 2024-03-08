#pragma once

#include <lcms2.h>

//================================================
// Preprocessor macros
//================================================

#undef  LAB_COLOR_USE_FLOAT
#define LAB_COLOR_USE_DOUBLE

#if ( !defined LAB_COLOR_USE_FLOAT && !defined LAB_COLOR_USE_DOUBLE ) || ( defined LAB_COLOR_USE_FLOAT && defined LAB_COLOR_USE_DOUBLE )
#   error Must define only *ONE* of LAB_COLOR_USE_FLOAT or LAB_COLOR_USE_DOUBLE.
#endif

//================================================
// Type aliases
//================================================

#if defined LAB_COLOR_USE_FLOAT
using FloatT = float;
#elif defined LAB_COLOR_USE_DOUBLE
using FloatT = double;
#endif

//================================================
// Constants
//================================================

#if defined LAB_COLOR_USE_FLOAT
cmsUInt32Number constexpr LabPixelFormat { TYPE_Lab_FLT };
#elif defined LAB_COLOR_USE_DOUBLE
cmsUInt32Number constexpr LabPixelFormat { TYPE_Lab_DBL };
#endif

FloatT constexpr LabMinimumL {    0.0 }; FloatT constexpr LabMaximumL {  100.0 };
FloatT constexpr LabMinimumA { -128.0 }; FloatT constexpr LabMaximumA {  127.0 };
FloatT constexpr LabMinimumB { -128.0 }; FloatT constexpr LabMaximumB {  127.0 };

//================================================
// Class TransformsManager
//================================================

class TransformsManager {

public:

    TransformsManager( ) {
        cmsHPROFILE hLabProfile { cmsCreateLab4Profile( cmsD50_xyY( ) ) };
        cmsHPROFILE hRgbProfile { cmsCreate_sRGBProfile( )              };

        _hLabToSrgbTransform = cmsCreateTransform( hLabProfile, LabPixelFormat, hRgbProfile, TYPE_RGB_8,     INTENT_PERCEPTUAL, 0 );
        _hSrgbToLabTransform = cmsCreateTransform( hRgbProfile, TYPE_RGB_8,     hLabProfile, LabPixelFormat, INTENT_PERCEPTUAL, 0 );

        cmsCloseProfile( hLabProfile );
        cmsCloseProfile( hRgbProfile );
    }

    ~TransformsManager( ) {
        if ( _hLabToSrgbTransform ) {
            cmsDeleteTransform( _hLabToSrgbTransform );
            _hLabToSrgbTransform = nullptr;
        }
        if ( _hSrgbToLabTransform ) {
            cmsDeleteTransform( _hSrgbToLabTransform );
            _hSrgbToLabTransform = nullptr;
        }
    }

    cmsHTRANSFORM GetLabToSrgbTransform( ) const { return _hLabToSrgbTransform; }
    cmsHTRANSFORM GetSrgbToLabTransform( ) const { return _hSrgbToLabTransform; }

protected:

    cmsHTRANSFORM _hLabToSrgbTransform { };
    cmsHTRANSFORM _hSrgbToLabTransform { };

};

extern TransformsManager Transforms;
