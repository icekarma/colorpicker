#include "pch.h"

#include "ZStrip.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#undef TIMING

void CZStrip::Update( ) {
#if defined TIMING
    uint64_t start, stop;
    GetSystemTimeAsFileTime( reinterpret_cast<LPFILETIME>( &start ) );
#endif // TIMING

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
    GetSystemTimeAsFileTime( reinterpret_cast<LPFILETIME>( &stop ) );
    uint64_t delta = stop - start;
    debug( "CZStrip::Update: elapsed time %.4f ms; rate %.2f Hz\n", delta / 10'000.0, 1.0 / ( delta / 10'000'000.0 ) );
#endif // TIMING
}

void CZStrip::_UpdateLabL( ) {
    LabPixelT* ptr { m_LabImage };

    Triplet<LabPixelT> channels;
    m_pDoc->GetLabColor( ).GetChannelValues( channels );

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<LabPixelT>( LabMinimumL + y * LabRanges[+LabChannels::L] / ImageHeight );
            *ptr++ = static_cast<LabPixelT>( channels[1] );
            *ptr++ = static_cast<LabPixelT>( channels[2] );
        }
    }

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), m_LabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CZStrip::_UpdateLabA( ) {
    LabPixelT* ptr { m_LabImage };

    Triplet<LabPixelT> channels;
    m_pDoc->GetLabColor( ).GetChannelValues( channels );

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<LabPixelT>( channels[0] );
            *ptr++ = static_cast<LabPixelT>( LabMinimumA + y * LabRanges[+LabChannels::a] / ImageHeight );
            *ptr++ = static_cast<LabPixelT>( channels[2] );
        }
    }

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), m_LabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CZStrip::_UpdateLabB( ) {
    LabPixelT* ptr { m_LabImage };

    Triplet<LabPixelT> channels;
    m_pDoc->GetLabColor( ).GetChannelValues( channels );

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<LabPixelT>( channels[0] );
            *ptr++ = static_cast<LabPixelT>( channels[1] );
            *ptr++ = static_cast<LabPixelT>( LabMinimumB + y * LabRanges[+LabChannels::b] / ImageHeight );
        }
    }

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), m_LabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CZStrip::_UpdateSrgbR( ) {
}

void CZStrip::_UpdateSrgbG( ) {
}

void CZStrip::_UpdateSrgbB( ) {
}
