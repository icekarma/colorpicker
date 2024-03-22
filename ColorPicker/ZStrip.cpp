#include "pch.h"

#include "ZStrip.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#define TIMING

#include "Timing.h"

void CZStrip::Update( ) {
#if defined TIMING
    Timing timing( "CZStrip::Update", true );
#endif // defined TIMING

    switch ( m_channel ) {
        case AllChannels::LabL:  _UpdateLabL( );  break;
        case AllChannels::LabA:  _UpdateLabA( );  break;
        case AllChannels::LabB:  _UpdateLabB( );  break;
        case AllChannels::SrgbR: _UpdateSrgbR( ); break;
        case AllChannels::SrgbG: _UpdateSrgbG( ); break;
        case AllChannels::SrgbB: _UpdateSrgbB( ); break;

        default:
            debug( "CZStrip::Update: Unknown value %d for m_channel\n", +m_channel );
            return;
    }

    m_pBitmap->SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_SrgbImage );

#if defined TIMING
    timing.Stop( );
#endif // defined TIMING
}

void CZStrip::_UpdateLabL( ) {
    RawLabValueT*         ptr      { m_RawLabImage };
    Triplet<RawLabValueT> channels { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<RawLabValueT>( y           );
            *ptr++ = static_cast<RawLabValueT>( channels[1] );
            *ptr++ = static_cast<RawLabValueT>( channels[2] );
        }
    }

    Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CZStrip::_UpdateLabA( ) {
    RawLabValueT*         ptr      { m_RawLabImage };
    Triplet<RawLabValueT> channels { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<RawLabValueT>( channels[0] );
            *ptr++ = static_cast<RawLabValueT>( y           );
            *ptr++ = static_cast<RawLabValueT>( channels[2] );
        }
    }

    Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CZStrip::_UpdateLabB( ) {
    RawLabValueT*         ptr      { m_RawLabImage };
    Triplet<RawLabValueT> channels { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<RawLabValueT>( channels[0] );
            *ptr++ = static_cast<RawLabValueT>( channels[1] );
            *ptr++ = static_cast<RawLabValueT>( y           );
        }
    }

    Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CZStrip::_UpdateSrgbR( ) {
}

void CZStrip::_UpdateSrgbG( ) {
}

void CZStrip::_UpdateSrgbB( ) {
}
