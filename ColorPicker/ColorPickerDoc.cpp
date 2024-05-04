#include "pch.h"

#include "ColorPickerDoc.h"
#include "ChannelInformation.h"

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

void CColorPickerDoc::SetChannelValues( std::initializer_list<std::pair<AllChannels, int>> const values ) {
#if defined _DEBUG
    if ( values.size( ) == 0 ) {
        debug( L"CColorPickerDoc::SetChannelValues: Warning: No channels passed!\n" );
        DebugBreak( );
    }
#endif // defined _DEBUG

    bool fLab  { };
    bool fSrgb { };

    for ( auto const& [channel, value] : values ) {
        fLab  |= IsLabChannel ( channel );
        fSrgb |= IsSrgbChannel( channel );

        _SetChannelValueImpl( channel, value );
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
    //
    // Get values from registry into local variables, ensuring all values are within
    // acceptable ranges and resetting them to default values if they are not.
    //

    UINTTriplet labValues { {
        theApp.GetProfileInt( L"Settings\\Saved Values", L"LabL", RawLabDefaultColor[+LabChannels::L] ),
        theApp.GetProfileInt( L"Settings\\Saved Values", L"LabA", RawLabDefaultColor[+LabChannels::a] ),
        theApp.GetProfileInt( L"Settings\\Saved Values", L"LabB", RawLabDefaultColor[+LabChannels::b] )
    } };
    UINTTriplet srgbValues { {
        theApp.GetProfileInt( L"Settings\\Saved Values", L"SrgbR", SrgbDefaultColor[+SrgbChannels::R] ),
        theApp.GetProfileInt( L"Settings\\Saved Values", L"SrgbG", SrgbDefaultColor[+SrgbChannels::G] ),
        theApp.GetProfileInt( L"Settings\\Saved Values", L"SrgbB", SrgbDefaultColor[+SrgbChannels::B] )
    } };

    if (
        ( (  labValues[ +LabChannels::L] > 255 ) || (  labValues[ +LabChannels::a] > 255 ) || (  labValues[ +LabChannels::b] > 255 ) ) ||
        ( ( srgbValues[+SrgbChannels::R] > 255 ) || ( srgbValues[+SrgbChannels::G] > 255 ) || ( srgbValues[+SrgbChannels::B] > 255 ) )
    ) {
        // restore defaults
        labValues  = triplet_cast<UINT>( RawLabDefaultColor );
        srgbValues = triplet_cast<UINT>(   SrgbDefaultColor );
    }

    bool fInverted = theApp.GetProfileInt( L"Settings", L"Inverted", 0 ) ? true : false;

    AllChannels selectedChannel { static_cast<AllChannels>( theApp.GetProfileInt( L"Settings", L"Selected channel", static_cast<int>( AllChannels::LabL ) ) ) };
    if ( selectedChannel > AllChannels::Max ) {
        selectedChannel = AllChannels::LabL;
    }

    //
    // Store the possibly-corrected values in our member variables.
    //

    m_LabColor        = triplet_cast<RawLabValueT>(  labValues );
    m_SrgbColor       = triplet_cast<  SrgbValueT>( srgbValues );
    m_fInverted       = fInverted;
    m_SelectedChannel = selectedChannel;
}

void CColorPickerDoc::SaveToRegistry( ) {
    RawLabTriplet labValues { ScaleLabColor( m_LabColor.GetChannelValues( ) ) };
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"LabL",             labValues[+LabChannels::L] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"LabA",             labValues[+LabChannels::a] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"LabB",             labValues[+LabChannels::b] );

    SrgbTriplet srgbValues { m_SrgbColor.GetChannelValues( ) };
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbR",            srgbValues[+SrgbChannels::R] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbG",            srgbValues[+SrgbChannels::G] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbB",            srgbValues[+SrgbChannels::B] );

    theApp.WriteProfileInt( L"Settings",               L"Inverted",         m_fInverted ? 1 : 0 );
    theApp.WriteProfileInt( L"Settings",               L"Selected channel", static_cast<int>( m_SelectedChannel ) );
}
