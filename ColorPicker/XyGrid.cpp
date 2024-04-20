#include "pch.h"

#include "XyGrid.h"

#include "ColorPicker.h"

#undef TIMING

#include "Timing.h"

IMPLEMENT_DYNCREATE( CXyGrid, CStaticBitmap )

BEGIN_MESSAGE_MAP( CXyGrid, CStaticBitmap )
    ON_WM_ERASEBKGND( )
    ON_WM_LBUTTONDOWN( )
    ON_WM_LBUTTONUP( )
    ON_WM_MOUSEMOVE( )
    ON_WM_SIZE( )
END_MESSAGE_MAP( )

void CXyGrid::UpdateBitmap( ) {
#if defined TIMING
    Timing timing( L"CXyGrid::UpdateBitmap", true );
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
            debug( L"CXyGrid::UpdateBitmap: Unknown value %d for m_channelZ\n", +m_channelZ );
            return;
    }

#if defined TIMING
    timing.Stop( );
#endif // defined TIMING

    m_bitmap.SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_SrgbImage );
    Invalidate( FALSE );
}

void CXyGrid::_UpdateLab( ) {
    RawLabValueT* ptr      { m_RawLabImage };
    RawLabTriplet channels { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) ) };
    LabChannels   channelX { AllChannelsToLabChannels( m_channelX ) };
    LabChannels   channelY { AllChannelsToLabChannels( m_channelY ) };

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
    SrgbValueT*  ptr      { m_SrgbImage };
    SrgbTriplet  channels { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };
    SrgbChannels channelX { AllChannelsToSrgbChannels( m_channelX ) };
    SrgbChannels channelY { AllChannelsToSrgbChannels( m_channelY ) };

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

void CXyGrid::OnSize( UINT nType, int cx, int cy ) {
    CStaticBitmap::OnSize( nType, cx, cy );

    if ( !m_bitmap.CreateBitmap( ImageWidth, ImageHeight, 1, 32, nullptr ) ) {
        debug( L"CXyGrid::OnSize: CreateBitmap failed\n" );
        return;
    }
    SetBitmap( m_bitmap );
}
