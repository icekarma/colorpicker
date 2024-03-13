#include "pch.h"

#include "ColorConverter.h"

//================================================
// Global variables
//================================================

TransformsManager Transforms;

//================================================
// Methods
//================================================

SrgbColorValue ConvertColor( LabColorValue const& color ) {
    FloatT  labValues[ImageLabValuesPerPixel];
    uint8_t srgbValues[ImageSrgbBytesPerPixel];

    color.GetChannelValues( labValues );
    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), labValues, srgbValues, 1 );
    return { srgbValues[2], srgbValues[1], srgbValues[0] };
}

LabColorValue ConvertColor( SrgbColorValue const& color ) {
    uint8_t srgbValues[ImageSrgbBytesPerPixel];
    FloatT  labValues[ImageLabValuesPerPixel];

    color.GetChannelValues( srgbValues );
    cmsDoTransform( Transforms.GetSrgbToLabTransform( ), srgbValues, labValues, 1 );
    return { labValues[0], labValues[1], labValues[2] };
}
