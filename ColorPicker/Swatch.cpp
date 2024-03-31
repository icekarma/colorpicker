#include "pch.h"

#include "Swatch.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#undef TIMING

#include "Timing.h"

void CSwatch::Update( ) {
#if defined TIMING
    Timing timing( "CXyGrid::Update", true );
#endif // defined TIMING

    _UpdateSrgb( );

    m_bitmap.SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_SrgbImage );

#if defined TIMING
    timing.Stop( );
#endif // defined TIMING
}

void CSwatch::_UpdateSrgb( ) {
    SrgbValueT*         ptr      { m_SrgbImage };
    Triplet<SrgbValueT> channels { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = channels[+SrgbChannels::B];
            *ptr++ = channels[+SrgbChannels::G];
            *ptr++ = channels[+SrgbChannels::R];
            *ptr++ = 0;
        }
    }
}
