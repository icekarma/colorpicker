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
        if ( m_SrgbImage ) {
            delete[] m_SrgbImage;
            m_SrgbImage = nullptr;
        }
    }

    void Update( );

private:

    void _UpdateSrgb( );

    int const              ImageWidth  { 99 };
    int const              ImageHeight { 52 };

    CColorPickerDoc const* m_pDoc;
    CBitmap                m_bitmap;

    SrgbValueT*            m_SrgbImage { new SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
