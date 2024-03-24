#include "pch.h"

#include "Swatch.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#define TIMING

#include "Timing.h"

void CSwatch::Update( ) {
#if defined TIMING
    Timing timing( "CXyGrid::Update", true );
#endif // defined TIMING

    SrgbValueT*         ptr   { m_Image };
    Triplet<SrgbValueT> color { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = color[+SrgbChannels::B];
            *ptr++ = color[+SrgbChannels::G];
            *ptr++ = color[+SrgbChannels::R];
            *ptr++ = 0;
        }
    }

    m_pBitmap->SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_Image );

#if defined TIMING
    timing.Stop( );
#endif // defined TIMING
}
