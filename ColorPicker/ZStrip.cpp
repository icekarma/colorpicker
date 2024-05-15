#include "pch.h"

#include "ZStrip.h"

#include "ColorPicker.h"

#undef TIMING

#include "Timing.h"

IMPLEMENT_DYNCREATE( CZStrip, CStaticBitmap )

BEGIN_MESSAGE_MAP( CZStrip, CStaticBitmap )
    ON_WM_ERASEBKGND( )
    ON_WM_LBUTTONDOWN( )
    ON_WM_LBUTTONUP( )
    ON_WM_MOUSEMOVE( )
    ON_WM_SIZE( )
END_MESSAGE_MAP( )

void CZStrip::UpdateBitmap( ) {
#if defined TIMING
    Timing timing( L"CZStrip::UpdateBitmap", true );
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
            debug( L"CZStrip::UpdateBitmap: Unknown value %d for m_channelZ\n", +m_channelZ );
            return;
    }

#if defined TIMING
    timing.Stop( );
#endif // defined TIMING

    m_bitmap.SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_SrgbImage );
    Invalidate( FALSE );
}

void CZStrip::_UpdateLab( ) {
    RawLabValueT* ptr      { m_RawLabImage };
    RawLabTriplet channels { ScaleLabColor( m_pDoc->GetLabColor( ).GetChannelValues( ) ) };
    LabChannels   channel  { AllChannelsToLabChannels( m_channelZ ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        channels[+channel] = static_cast<RawLabValueT>( m_fInverted ? ImageHeight - y - 1 : y );

        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::L] );
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::a] );
            *ptr++ = static_cast<RawLabValueT>( channels[+LabChannels::b] );
        }
    }

    g_Transforms.TransformLabToSrgb( m_RawLabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CZStrip::_UpdateSrgb( ) {
    SrgbValueT*  ptr      { m_SrgbImage };
    SrgbTriplet  channels { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };
    SrgbChannels channel  { AllChannelsToSrgbChannels( m_channelZ ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        channels[+channel] = static_cast<SrgbValueT>( m_fInverted ? ImageHeight - y - 1 : y );

        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::B] );
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::G] );
            *ptr++ = static_cast<SrgbValueT>( channels[+SrgbChannels::R] );
            *ptr++ = 0;
        }
    }
}

void CZStrip::OnSize( UINT nType, int cx, int cy ) {
    CStaticBitmap::OnSize( nType, cx, cy );

    if ( !m_bitmap.CreateBitmap( ImageWidth, ImageHeight, 1, 32, nullptr ) ) {
        debug( L"CZStrip::OnSize: CreateBitmap failed\n" );
        return;
    }
    SetBitmap( m_bitmap );
}
