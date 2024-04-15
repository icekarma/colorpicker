#pragma once

#include "ColorPickerDoc.h"
#include "StaticBrush.h"

class CSwatch {

public:

    CSwatch( ) = delete;

    CSwatch( CColorPickerDoc* pDoc, CStaticBrush* pStatic ):
        m_pDoc         { pDoc    },
        m_pStaticBrush { pStatic },
        m_color        { m_pDoc->GetSrgbColor( ) }
    {
        m_pStaticBrush->SetColor( m_color );
    }

    void Update( ) {
        if ( SrgbColor newColor { m_pDoc->GetSrgbColor( ) }; m_color != newColor ) {
            m_color = newColor;
            m_pStaticBrush->SetColor( m_color );
        }
    }

private:

    CColorPickerDoc const* m_pDoc;
    CStaticBrush*          m_pStaticBrush;

    SrgbColor              m_color;

};
