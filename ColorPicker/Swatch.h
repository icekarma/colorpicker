#pragma once

#include "ColorPickerDoc.h"

class CSwatch {

public:

    CSwatch( ) = delete;

    CSwatch( CColorPickerDoc* pDoc, CBitmap* pBitmap ):
        m_pDoc { pDoc },
        m_pBitmap { pBitmap } {
        /*empty*/
    }

    ~CSwatch( ) {
        if ( m_Image ) {
            delete[] m_Image;
            m_Image = nullptr;
        }
    }

    void Update( );

    int const static ImageWidth  { 99 };
    int const static ImageHeight { 52 };

private:

    CColorPickerDoc const* m_pDoc;
    CBitmap*               m_pBitmap;

    SrgbValueT*            m_Image   { new SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
