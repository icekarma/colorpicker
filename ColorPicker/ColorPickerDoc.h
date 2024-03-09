#pragma once

#include "Color.h"

class CColorPickerDoc:
    public CDocument
{

    DECLARE_DYNCREATE( CColorPickerDoc )
    DECLARE_MESSAGE_MAP( )

protected:

    CColorPickerDoc( ) noexcept { /*empty*/ }

    virtual ~CColorPickerDoc( ) { /*empty*/ }

    virtual BOOL OnNewDocument( );

public:

    // If you change LabColor, you must change SrgbColor too!
    LabColorValue  LabColor  { };

    // If you change SrgbColor, you must change LabColor too!
    SrgbColorValue SrgbColor { };

};
