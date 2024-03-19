#include "pch.h"

#include "XyGrid.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#undef TIMING

void CXyGrid::Update( ) {
#if defined TIMING
    uint64_t start, stop;
    GetSystemTimeAsFileTime( reinterpret_cast<LPFILETIME>( &start ) );
#endif // TIMING

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

    m_pBitmap->SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbValuesPerPixel, m_SrgbImage );

#if defined TIMING
    GetSystemTimeAsFileTime( reinterpret_cast<LPFILETIME>( &stop ) );
    uint64_t delta = stop - start;
    debug( "CXyGrid::Update: elapsed time %.4f ms; rate %.2f Hz\n", delta / 10'000.0, 1.0 / ( delta / 10'000'000.0 ) );
#endif // TIMING
}

void CXyGrid::_UpdateLabL( ) {
    LabPixelT* ptr { m_LabImage };
    LabPixelT  z   { m_pDoc->GetLabColor( ).GetChannelValue( LabChannels::L ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<LabPixelT>(               z );
            *ptr++ = static_cast<LabPixelT>( LabMinimumA + x );
            *ptr++ = static_cast<LabPixelT>( LabMinimumB + y );
        }
    }

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), m_LabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CXyGrid::_UpdateLabA( ) {
    LabPixelT* ptr { m_LabImage };
    LabPixelT  z   { m_pDoc->GetLabColor( ).GetChannelValue( LabChannels::a ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<LabPixelT>( LabMinimumL + y * LabRanges[+LabChannels::L] / ImageHeight );
            *ptr++ = static_cast<LabPixelT>(               z );
            *ptr++ = static_cast<LabPixelT>( LabMinimumB + x );
        }
    }

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), m_LabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CXyGrid::_UpdateLabB( ) {
    LabPixelT* ptr { m_LabImage };
    LabPixelT  z   { m_pDoc->GetLabColor( ).GetChannelValue( LabChannels::b ) };

    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<LabPixelT>( LabMinimumL + y * LabRanges[+LabChannels::L] / ImageHeight );
            *ptr++ = static_cast<LabPixelT>( LabMinimumA + x );
            *ptr++ = static_cast<LabPixelT>(               z );
        }
    }

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), m_LabImage, m_SrgbImage, ImageWidth * ImageHeight );
}

void CXyGrid::_UpdateSrgbR( ) {
}

void CXyGrid::_UpdateSrgbG( ) {
}

void CXyGrid::_UpdateSrgbB( ) {
}
