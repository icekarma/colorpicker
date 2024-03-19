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
        if ( m_LabImage ) {
            delete[] m_LabImage;
            m_LabImage = nullptr;
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

    using  LabPixelT =  LabColorValue::PixelT;
    using SrgbPixelT = SrgbColorValue::PixelT;

    void _UpdateLabL( );
    void _UpdateLabA( );
    void _UpdateLabB( );

    void _UpdateSrgbR( );
    void _UpdateSrgbG( );
    void _UpdateSrgbB( );

    CColorPickerDoc const* m_pDoc;
    CBitmap*               m_pBitmap;

    AllChannels            m_channel   { AllChannels::unknown };

    LabPixelT*             m_LabImage  { new  LabPixelT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbPixelT*            m_SrgbImage { new SrgbPixelT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
