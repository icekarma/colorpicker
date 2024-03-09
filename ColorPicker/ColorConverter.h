#pragma once

#include "Color.h"

LabColorValue  ConvertColor( SrgbColorValue const& color );
SrgbColorValue ConvertColor( LabColorValue  const& color );
