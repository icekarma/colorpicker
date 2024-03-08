#pragma once

#include "Color.h"
#include "TransformsManager.h"

LabColorValue  ConvertColor( SrgbColorValue const& color );
SrgbColorValue ConvertColor( LabColorValue  const& color );
