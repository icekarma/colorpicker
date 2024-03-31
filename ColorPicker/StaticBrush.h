#pragma once

#include "Color.h"

class CStaticBrush: public CStatic {

    DECLARE_MESSAGE_MAP( )

public:

    void SetColor( SrgbColor const& color );

};
