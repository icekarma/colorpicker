#pragma once

#include "ColorPickerDoc.h"
#include "StaticBitmap.h"

class CZStrip: public CStaticBitmap {

    DECLARE_DYNCREATE( CZStrip )
    DECLARE_MESSAGE_MAP( )

public:

    virtual ~CZStrip( ) override {
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

    bool SetChannel( AllChannels const channelZ ) {
        bool changed = m_channelZ != channelZ;
        m_channelZ = channelZ;
        return changed;
    }

    void UpdateBitmap( );

private:

    void _UpdateLab( );
    void _UpdateSrgb( );

    afx_msg void OnSize( UINT nType, int cx, int cy );

    int const static       ImageWidth    {  20 };
    int const static       ImageHeight   { 256 };

    CColorPickerDoc const* m_pDoc        { };
    CBitmap                m_bitmap;

    AllChannels            m_channelZ    { AllChannels::unknown };

    RawLabValueT*          m_RawLabImage { new RawLabValueT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbValueT*            m_SrgbImage   { new   SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
