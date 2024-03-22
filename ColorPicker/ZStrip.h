#pragma once

#include "ColorPickerDoc.h"

class CZStrip {

public:

    CZStrip( ) = delete;

    CZStrip( CColorPickerDoc const* pDoc, CBitmap* pBitmap ):
        m_pDoc    { pDoc    },
        m_pBitmap { pBitmap }
    {
        /*empty*/
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

    void SetChannel( AllChannels const channel ) {
        m_channel = channel;
    }

    void Update( );

    int const static ImageWidth  {  20 };
    int const static ImageHeight { 256 };

private:

    void _UpdateLabL( );
    void _UpdateLabA( );
    void _UpdateLabB( );

    void _UpdateSrgbR( );
    void _UpdateSrgbG( );
    void _UpdateSrgbB( );

    CColorPickerDoc const* m_pDoc;
    CBitmap*               m_pBitmap;

    AllChannels            m_channel     { AllChannels::unknown };

    RawLabValueT*          m_RawLabImage { new RawLabValueT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbValueT*            m_SrgbImage   { new   SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
