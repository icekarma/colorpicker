#pragma once

#include "ChildView.h"
#include "ColorPickerDoc.h"

class CChildView;

class CZStrip {

public:

    CZStrip( ) = delete;

    CZStrip( CColorPickerDoc* pDoc, CBitmap* pBitmap ):
        m_pDoc    { pDoc    },
        m_pBitmap { pBitmap }
    {
        /*empty*/
    }

    void Update( );

private:

    CColorPickerDoc const* m_pDoc          { };
    CBitmap*               m_pBitmap       { };

    int                    m_nLastChannel  { -1 };
    LabColorValue          m_LastLabValue;
    SrgbColorValue         m_lastSrgbValue;

};
