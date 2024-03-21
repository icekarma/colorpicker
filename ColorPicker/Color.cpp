#include "pch.h"

#include "Color.h"

//================================================
// Global variables
//================================================

TransformsManager Transforms;

//================================================
// Methods
//================================================

SrgbColorValue ConvertColor( LabColorValue const& color ) {
    Triplet<FloatT> labValues { };
    uint8_t         srgbValues[ImageSrgbValuesPerPixel];

    color.GetChannelValues( labValues );
    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), labValues.data( ), srgbValues, 1 );
    return { srgbValues[2], srgbValues[1], srgbValues[0] };
}

LabColorValue ConvertColor( SrgbColorValue const& color ) {
    Triplet<uint8_t> srgbValues { };
    FloatT           labValues[ImageLabValuesPerPixel];

    color.GetChannelValues( srgbValues );
    cmsDoTransform( Transforms.GetSrgbToLabTransform( ), srgbValues.data( ), labValues, 1 );
    return { labValues[0], labValues[1], labValues[2] };
}
