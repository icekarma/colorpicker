#pragma once

#include "ColorPickerDoc.h"
#include "StaticBitmap.h"

class CXyGrid: public CStaticBitmap {

    DECLARE_DYNCREATE( CXyGrid )
    DECLARE_MESSAGE_MAP( )

public:

    virtual ~CXyGrid( ) override {
        if ( m_RawLabImage ) {
            delete[] m_RawLabImage;
            m_RawLabImage = nullptr;
        }

        if ( m_SrgbImage ) {
            delete[] m_SrgbImage;
            m_SrgbImage = nullptr;
        }
    }

    void SetDocument( CColorPickerDoc const* pDoc ) {
        m_pDoc = pDoc;
    }

    bool SetChannels( AllChannels const channelX, AllChannels const channelY, AllChannels const channelZ ) {
        bool changed = m_channelZ != channelZ;

        m_channelX = channelX;
        m_channelY = channelY;
        m_channelZ = channelZ;

        return changed;
    }

    void UpdateBitmap( );

private:

    void _UpdateLab( );
    void _UpdateSrgb( );

    afx_msg void OnSize( UINT nType, int cx, int cy );

    int const static       ImageWidth    { 256 };
    int const static       ImageHeight   { 256 };

    CColorPickerDoc const* m_pDoc        { };
    CBitmap                m_bitmap;

    AllChannels            m_channelX    { AllChannels::unknown };
    AllChannels            m_channelY    { AllChannels::unknown };
    AllChannels            m_channelZ    { AllChannels::unknown };

    RawLabValueT*          m_RawLabImage { new RawLabValueT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbValueT*            m_SrgbImage   { new   SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
