#pragma once

#include "ColorPickerDoc.h"

class CSwatch {

public:

    CSwatch( ) = delete;

    CSwatch( CColorPickerDoc* pDoc, CStatic* pStatic ):
        m_pDoc { pDoc }
    {
        if ( m_bitmap.CreateBitmap( ImageWidth, ImageHeight, 1, 32, nullptr ) ) {
            pStatic->SetBitmap( m_bitmap );
        } else {
            debug( "CSwatch::`ctor: CreateBitmap failed\n" );
        }
    }

    ~CSwatch( ) {
        if ( m_Image ) {
            delete[] m_Image;
            m_Image = nullptr;
        }
    }

    void Update( );

private:

    int const              ImageWidth  { 99 };
    int const              ImageHeight { 52 };

    CColorPickerDoc const* m_pDoc;
    CBitmap                m_bitmap;

    SrgbValueT*            m_Image     { new SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
