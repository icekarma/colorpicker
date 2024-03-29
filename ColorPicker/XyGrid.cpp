#include "pch.h"

#include "XyGrid.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#undef TIMING

#include "Timing.h"

void CXyGrid::Update( ) {
#if defined TIMING
    Timing timing( "CXyGrid::Update", true );
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
            debug( "CXyGrid::Update: Unknown value %d for m_channelZ\n", +m_channelZ );
            return;
    }

    m_bitmap.SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_SrgbImage );

#if defined TIMING
    timing.Stop( );
#endif // defined TIMING
}

void CXyGrid::_UpdateLab( ) {
    RawLabValueT*         ptr      { m_RawLabImage };
    Triplet<RawLabValueT> channels { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) ) };
    LabChannels           channelX { AllChannelsToLabChannels( m_channelX ) };
    LabChannels           channelY { AllChannelsToLabChannels( m_channelY ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        channels[+channelY] = static_cast<RawLabValueT>( y );
        for ( int x { }; x < ImageWidth; ++x ) {
            channels[+channelX] = static_cast<RawLabValueT>( x );
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::L] );
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::a] );
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::b] );
        }
    }

    Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CXyGrid::_UpdateSrgb( ) {
    SrgbValueT*         ptr      { m_SrgbImage };
    Triplet<SrgbValueT> channels { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };
    SrgbChannels        channelX { AllChannelsToSrgbChannels( m_channelX ) };
    SrgbChannels        channelY { AllChannelsToSrgbChannels( m_channelY ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        channels[+channelY] = static_cast<SrgbValueT>( y );
        for ( int x { }; x < ImageWidth; ++x ) {
            channels[+channelX] = static_cast<SrgbValueT>( x );
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::B] );
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::G] );
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::R] );
            *ptr++ = 0;
        }
    }
}
