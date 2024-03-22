#pragma once

#include "ColorPickerDoc.h"

class CXyGrid {

public:

    CXyGrid( ) = delete;

    CXyGrid( CColorPickerDoc* pDoc, CBitmap* pBitmap ):
        m_pDoc    { pDoc    },
        m_pBitmap { pBitmap }
    {
        /*empty*/
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

    int const static ImageWidth  { 256 };
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

    AllChannels            m_channelX    { AllChannels::unknown };
    AllChannels            m_channelY    { AllChannels::unknown };
    AllChannels            m_channelZ    { AllChannels::unknown };

    RawLabValueT*          m_RawLabImage { new RawLabValueT[ImageWidth * ImageHeight *  ImageLabValuesPerPixel] };
    SrgbValueT*            m_SrgbImage   { new   SrgbValueT[ImageWidth * ImageHeight * ImageSrgbValuesPerPixel] };

};
