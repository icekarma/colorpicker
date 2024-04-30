#include "pch.h"

#include "ColorPickerDoc.h"

#include "ColorPicker.h"

IMPLEMENT_DYNCREATE( CColorPickerDoc, CDocument )

BEGIN_MESSAGE_MAP( CColorPickerDoc, CDocument )
END_MESSAGE_MAP( )

namespace {

    RawLabTriplet const RawLabDefaultColor { { 255, 128, 128 } };
    SrgbTriplet   const   SrgbDefaultColor { { 255, 255, 255 } };

}

BOOL CColorPickerDoc::OnNewDocument( ) {
    if ( !CDocument::OnNewDocument( ) ) {
        return FALSE;
    }

    // Reinitialization (SDI applications will reuse this document)

    m_LabColor  = RawLabDefaultColor;
    m_SrgbColor =   SrgbDefaultColor;

    return TRUE;
}

int CColorPickerDoc::GetChannelValue( AllChannels const channel ) {
    if ( IsLabChannel( channel ) ) {
        return m_LabColor.GetChannelValue( channel );
    } else {
        return m_SrgbColor.GetChannelValue( channel );
    }
}

void CColorPickerDoc::_SetChannelValueImpl( AllChannels const channel, int const nValue ) {
    if ( IsLabChannel( channel ) ) {
        // TODO should this really be rescaling the value? What guarantees that callers only pass values effectively of type RawLabValueT?
        if ( channel == AllChannels::LabL ) {
            m_LabColor.SetChannelValue( channel, static_cast<LabValueT>( nValue * 100 / 255 ) );
        } else {
            m_LabColor.SetChannelValue( channel, static_cast<LabValueT>( nValue - 128 ) );
        }
    } else {
        m_SrgbColor.SetChannelValue( channel, static_cast<SrgbValueT>( nValue ) );
    }
}

void CColorPickerDoc::SetChannelValue( AllChannels const channel, int const nValue ) {
    _SetChannelValueImpl( channel, nValue );

    if ( IsLabChannel( channel ) ) {
        SetColor( m_LabColor );
    } else {
        SetColor( m_SrgbColor );
    }
}

void CColorPickerDoc::SetChannelValues( std::initializer_list<std::pair<AllChannels, int>> const values ) {
#if defined _DEBUG
    if ( values.size( ) == 0 ) {
        debug( L"CColorPickerDoc::SetChannelValues: Warning: No channels passed!\n" );
        DebugBreak( );
    }
#endif // defined _DEBUG

    bool fLab  { };
    bool fSrgb { };

    for ( auto const& value : values ) {
        fLab  |= IsLabChannel ( value.first );
        fSrgb |= IsSrgbChannel( value.first );

        _SetChannelValueImpl( value.first, value.second );
    }

#if defined _DEBUG
    if ( fLab && fSrgb ) {
        debug( L"CColorPickerDoc::SetChannelValues: Warning: Both L*a*b* and sRGB channels passed!\n" );
        DebugBreak( );
    }
#endif // defined _DEBUG

    if ( fLab ) {
        SetColor( m_LabColor );
    } else if ( fSrgb ) {
        SetColor( m_SrgbColor );
    }
}

void CColorPickerDoc::LoadFromRegistry( ) {
    m_LabColor = RawLabTriplet { {
        static_cast<RawLabValueT>( theApp.GetProfileIntW( L"Settings\\Saved Values", L"LabL", RawLabDefaultColor[+LabChannels::L] ) ),
        static_cast<RawLabValueT>( theApp.GetProfileIntW( L"Settings\\Saved Values", L"LabA", RawLabDefaultColor[+LabChannels::a] ) ),
        static_cast<RawLabValueT>( theApp.GetProfileIntW( L"Settings\\Saved Values", L"LabB", RawLabDefaultColor[+LabChannels::b] ) )
    } };

    m_SrgbColor = SrgbTriplet { {
        static_cast<SrgbValueT>( theApp.GetProfileIntW( L"Settings\\Saved Values", L"SrgbR", SrgbDefaultColor[+SrgbChannels::R] ) ),
        static_cast<SrgbValueT>( theApp.GetProfileIntW( L"Settings\\Saved Values", L"SrgbG", SrgbDefaultColor[+SrgbChannels::G] ) ),
        static_cast<SrgbValueT>( theApp.GetProfileIntW( L"Settings\\Saved Values", L"SrgbB", SrgbDefaultColor[+SrgbChannels::B] ) )
    } };

    m_fInverted = !!theApp.GetProfileIntW( L"Settings", L"Inverted", 0 );

    m_SelectedChannel = static_cast<AllChannels>( theApp.GetProfileIntW( L"Settings", L"Selected channel", static_cast<int>( AllChannels::LabL ) ) );
    if ( m_SelectedChannel > AllChannels::Max ) {
        m_SelectedChannel = AllChannels::LabL;
    }
}

void CColorPickerDoc::SaveToRegistry( ) {
    RawLabTriplet labValues { ScaleLabColor( m_LabColor.GetChannelValues( ) ) };
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"LabL", labValues[+LabChannels::L] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"LabA", labValues[+LabChannels::a] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"LabB", labValues[+LabChannels::b] );

    SrgbTriplet srgbValues { m_SrgbColor.GetChannelValues( ) };
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbR", srgbValues[+SrgbChannels::R] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbG", srgbValues[+SrgbChannels::G] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbB", srgbValues[+SrgbChannels::B] );

    theApp.WriteProfileInt( L"Settings", L"Inverted",         m_fInverted ? 1 : 0 );
    theApp.WriteProfileInt( L"Settings", L"Selected channel", static_cast<int>( m_SelectedChannel ) );
}
