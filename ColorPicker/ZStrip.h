#pragma once

#include "ColorPickerDoc.h"

class CZStrip {

public:

    CZStrip( ) = delete;

    CZStrip( CColorPickerDoc const* pDoc, CStatic* pStatic ):
        m_pDoc    { pDoc    },
        m_pStatic { pStatic }
    {
        if ( m_bitmap.CreateBitmap( ImageWidth, ImageHeight, 1, 32, nullptr ) ) {
            m_pStatic->SetBitmap( m_bitmap );
        } else {
            debug( L"CZStrip::`ctor: CreateBitmap failed\n" );
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

    bool SetChannel( AllChannels const channelZ ) {
        bool changed = m_channelZ != channelZ;

        m_channelZ = channelZ;

        return changed;
    }

    void Update( );

private:

    void _UpdateLab( );
    void _UpdateSrgb( );

    int const static       ImageWidth    {  20 };
    int const static       ImageHeight   { 256 };

    CColorPickerDoc const* m_pDoc;
    CBitmap                m_bitmap;
    CStatic*               m_pStatic;

    AllChannels            m_channelZ    { AllChannels::unknown };

    RawLabValueT*          m_RawLabImage { new RawLabValueT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbValueT*            m_SrgbImage   { new   SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
