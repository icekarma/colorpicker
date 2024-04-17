#include "pch.h"

#include "ZStrip.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#undef TIMING

#include "Timing.h"

void CZStrip::Update( ) {
#if defined TIMING
    Timing timing( L"CZStrip::Update", true );
#endif // defined TIMING

    switch ( m_channelZ ) {
        case AllChannels::LabL:
        case AllChannels::LabA:
        case AllChannels::LabB:
            _UpdateLab( );
            break;

        case AllChannels::SrgbR:
        case AllChannels::SrgbG:
        case AllChannels::SrgbB:
            _UpdateSrgb( );
            break;

        default:
            debug( L"CZStrip::Update: Unknown value %d for m_channelZ\n", +m_channelZ );
            return;
    }

    m_bitmap.SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_SrgbImage );

#if defined TIMING
    timing.Stop( );
#endif // defined TIMING
}

void CZStrip::_UpdateLab( ) {
    RawLabValueT* ptr      { m_RawLabImage };
    RawLabTriplet channels { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) ) };
    LabChannels   channel  { AllChannelsToLabChannels( m_channelZ ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        channels[+channel] = static_cast<RawLabValueT>( y );
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::L] );
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::a] );
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::b] );
        }
    }

    Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CZStrip::_UpdateSrgb( ) {
    SrgbValueT*  ptr      { m_SrgbImage };
    SrgbTriplet  channels { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };
    SrgbChannels channel  { AllChannelsToSrgbChannels( m_channelZ ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        channels[+channel] = static_cast<SrgbValueT>( y );
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::B] );
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::G] );
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::R] );
            *ptr++ = 0;
        }
    }
}
