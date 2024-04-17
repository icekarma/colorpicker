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
            debug( L"CXyGrid::`ctor: CreateBitmap failed\n" );
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

    bool SetChannels( AllChannels const channelX, AllChannels const channelY, AllChannels const channelZ ) {
        bool changed = m_channelZ != channelZ;

        m_channelX = channelX;
        m_channelY = channelY;
        m_channelZ = channelZ;

        return changed;
    }

    void Update( );

private:

    void _UpdateLab( );
    void _UpdateSrgb( );

    int const static       ImageWidth    { 256 };
    int const static       ImageHeight   { 256 };

    CColorPickerDoc const* m_pDoc;
    CBitmap                m_bitmap;

    AllChannels            m_channelX    { AllChannels::unknown };
    AllChannels            m_channelY    { AllChannels::unknown };
    AllChannels            m_channelZ    { AllChannels::unknown };

    RawLabValueT*          m_RawLabImage { new RawLabValueT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbValueT*            m_SrgbImage   { new   SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
