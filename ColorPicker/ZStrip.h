#pragma once

#include "ColorPickerDoc.h"

class CZStrip {

public:

    CZStrip( ) = delete;

    CZStrip( CColorPickerDoc const* pDoc, CStatic* pStatic ):
        m_pDoc { pDoc }
    {
        if ( m_bitmap.CreateBitmap( ImageWidth, ImageHeight, 1, 32, nullptr ) ) {
            pStatic->SetBitmap( m_bitmap );
        } else {
            debug( "CZStrip::`ctor: CreateBitmap failed\n" );
        }
    }

    ~CZStrip( ) {
        if ( m_RawLabImage ) {
            delete[] m_RawLabImage;
            m_RawLabImage = nullptr;
        }

        if ( m_SrgbImage ) {
            delete[] m_SrgbImage;
            m_SrgbImage = nullptr;
        }
    }

    void SetChannel( AllChannels const channelZ ) {
        m_channelZ = channelZ;
    }

    void Update( );

private:

    void _UpdateLab( );
    void _UpdateSrgb( );

    int const static       ImageWidth    {  20 };
    int const static       ImageHeight   { 256 };

    CColorPickerDoc const* m_pDoc;
    CBitmap                m_bitmap;

    AllChannels            m_channelZ    { AllChannels::unknown };

    RawLabValueT*          m_RawLabImage { new RawLabValueT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbValueT*            m_SrgbImage   { new   SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
