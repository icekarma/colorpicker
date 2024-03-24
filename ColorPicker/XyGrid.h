#pragma once

#include "ColorPickerDoc.h"

class CXyGrid {

public:

    CXyGrid( ) = delete;

    CXyGrid( CColorPickerDoc* pDoc, CStatic* pStatic ):
        m_pDoc { pDoc }
    {
        if ( m_bitmap.CreateBitmap( ImageWidth, ImageHeight, 1, 32, nullptr ) ) {
            pStatic->SetBitmap( m_bitmap );
        } else {
            debug( "CXyGrid::`ctor: CreateBitmap failed\n" );
        }
    }

    ~CXyGrid( ) {
        if ( m_RawLabImage ) {
            delete[] m_RawLabImage;
            m_RawLabImage = nullptr;
        }

        if ( m_SrgbImage ) {
            delete[] m_SrgbImage;
            m_SrgbImage = nullptr;
        }
    }

    void SetChannels( AllChannels const channelX, AllChannels const channelY, AllChannels const channelZ ) {
        m_channelX = channelX;
        m_channelY = channelY;
        m_channelZ = channelZ;
    }

    void Update( );

private:

    void _UpdateLabL( );
    void _UpdateLabA( );
    void _UpdateLabB( );

    void _UpdateSrgbR( );
    void _UpdateSrgbG( );
    void _UpdateSrgbB( );

    int const              ImageWidth    { 256 };
    int const              ImageHeight   { 256 };

    CColorPickerDoc const* m_pDoc;
    CBitmap                m_bitmap;

    AllChannels            m_channelX    { AllChannels::unknown };
    AllChannels            m_channelY    { AllChannels::unknown };
    AllChannels            m_channelZ    { AllChannels::unknown };

    RawLabValueT*          m_RawLabImage { new RawLabValueT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbValueT*            m_SrgbImage   { new   SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
