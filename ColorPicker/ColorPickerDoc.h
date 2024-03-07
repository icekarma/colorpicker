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

    LabColor<double>   LabColor  { };
    SrgbColor<uint8_t> SrgbColor { };

};
