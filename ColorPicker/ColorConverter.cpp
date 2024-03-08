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
    double  inputValues[3];
    uint8_t outputValues[3];

    color.GetChannelValues( inputValues );

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), inputValues, outputValues, 1 );

    return SrgbColorValue { outputValues[0], outputValues[1], outputValues[2] };
}

LabColorValue ConvertColor( SrgbColorValue const& color ) {
    uint8_t inputValues[3];
    double  outputValues[3];

    color.GetChannelValues( inputValues );

    cmsDoTransform( Transforms.GetSrgbToLabTransform( ), inputValues, outputValues, 1 );

    return LabColorValue { outputValues[0], outputValues[1], outputValues[2] };
}
