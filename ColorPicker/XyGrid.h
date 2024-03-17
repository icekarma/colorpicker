#pragma once

#include "ChildView.h"
#include "ColorPickerDoc.h"

class CChildView;

class CXyGrid {

public:

    CXyGrid( ) = delete;

    CXyGrid( CColorPickerDoc* pDoc, CBitmap* pBitmap ):
        m_pDoc    { pDoc    },
        m_pBitmap { pBitmap }
    {
        /*empty*/
    }

    void Update( );

private:

    CColorPickerDoc const* m_pDoc             { };
    CBitmap*               m_pBitmap          { };

    int const ImageWidth  { 256 };
    int const ImageHeight { 256 };

    int                    m_nLastChannels[3] { -1, -1, -1 };
    LabColorValue          m_LastLabValue;
    SrgbColorValue         m_lastSrgbValue;

};
