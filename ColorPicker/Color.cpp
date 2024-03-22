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
    SrgbValueT         srgbValues[ImageSrgbValuesPerPixel];
    Triplet<LabValueT> labValues { color.GetChannelValues( ) };

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), labValues.data( ), srgbValues, 1 );
    return { srgbValues[2], srgbValues[1], srgbValues[0] };
}

LabColorValue ConvertColor( SrgbColorValue const& color ) {
    LabValueT           labValues[ImageLabValuesPerPixel];
    Triplet<SrgbValueT> srgbValues { color.GetChannelValues( ) };

    cmsDoTransform( Transforms.GetSrgbToLabTransform( ), srgbValues.data( ), labValues, 1 );
    return { labValues[0], labValues[1], labValues[2] };
}
