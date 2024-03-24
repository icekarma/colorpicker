#include "pch.h"

#include "XyGrid.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#define TIMING

#include "Timing.h"

void CXyGrid::Update( ) {
#if defined TIMING
    Timing timing( "CXyGrid::Update", true );
#endif // defined TIMING

    switch ( m_channelZ ) {
        case AllChannels::LabL:  _UpdateLabL( );  break;
        case AllChannels::LabA:  _UpdateLabA( );  break;
        case AllChannels::LabB:  _UpdateLabB( );  break;
        case AllChannels::SrgbR: _UpdateSrgbR( ); break;
        case AllChannels::SrgbG: _UpdateSrgbG( ); break;
        case AllChannels::SrgbB: _UpdateSrgbB( ); break;

        default:
            debug( "CXyGrid::Update: Unknown value %d for m_channelZ\n", +m_channelZ );
            return;
    }

    m_bitmap.SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_SrgbImage );

#if defined TIMING
    timing.Stop( );
#endif // defined TIMING
}

void CXyGrid::_UpdateLabL( ) {
    RawLabValueT* ptr { m_RawLabImage };
    RawLabValueT  z   { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) )[+LabChannels::L] };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<RawLabValueT>( z );
            *ptr++ = static_cast<RawLabValueT>( x );
            *ptr++ = static_cast<RawLabValueT>( y );
        }
    }

    Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CXyGrid::_UpdateLabA( ) {
    RawLabValueT* ptr { m_RawLabImage };
    RawLabValueT  z   { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) )[+LabChannels::a] };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<RawLabValueT>( y );
            *ptr++ = static_cast<RawLabValueT>( z );
            *ptr++ = static_cast<RawLabValueT>( x );
        }
    }

    Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CXyGrid::_UpdateLabB( ) {
    RawLabValueT* ptr { m_RawLabImage };
    RawLabValueT  z   { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) )[+LabChannels::b] };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<RawLabValueT>( y );
            *ptr++ = static_cast<RawLabValueT>( x );
            *ptr++ = static_cast<RawLabValueT>( z );
        }
    }

    Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CXyGrid::_UpdateSrgbR( ) {
}

void CXyGrid::_UpdateSrgbG( ) {
}

void CXyGrid::_UpdateSrgbB( ) {
}
