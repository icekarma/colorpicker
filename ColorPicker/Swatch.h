#pragma once

#include "ColorPickerDoc.h"
#include "StaticBrush.h"

class CSwatch {

public:

    CSwatch( ) = delete;

    CSwatch( CColorPickerDoc* pDoc, CStaticBrush* pStatic ):
        m_pDoc         { pDoc    },
        m_pStaticBrush { pStatic }
    {
        Update( );
    }

    void Update( ) {
        m_pStaticBrush->SetColor( m_pDoc->GetSrgbColor( ) );
    }

private:

    CColorPickerDoc const* m_pDoc;
    CStaticBrush*          m_pStaticBrush;

};
