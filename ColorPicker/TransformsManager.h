#pragma once

#include <lcms2.h>

#include "Color.h"

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
