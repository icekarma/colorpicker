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

CColorPickerDoc::CColorPickerDoc( ) {
    m_LabColor  = RawLabDefaultColor;
    m_SrgbColor =   SrgbDefaultColor;
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

void CColorPickerDoc::SetChannelValue( AllChannels const channel, int const nValue ) {
    switch ( channel ) {
        case AllChannels::LabL:
        case AllChannels::LabA:
        case AllChannels::LabB: {
            LabColor color { m_LabColor };
            if ( channel == AllChannels::LabL ) {
                color.SetChannelValue( channel, static_cast<LabValueT>( nValue * 100 / 255 ) );
            } else {
                color.SetChannelValue( channel, static_cast<LabValueT>( nValue - 128 ) );
            }
            SetColor( color );
            break;
        }

        case AllChannels::SrgbR:
        case AllChannels::SrgbG:
        case AllChannels::SrgbB: {
            SrgbColor color { m_SrgbColor };
            color.SetChannelValue( channel, static_cast<SrgbValueT>( nValue ) );
            SetColor( color );
            break;
        }

        default:
            break;
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

    m_SelectedChannel = static_cast<AllChannels>( theApp.GetProfileIntW( L"Settings", L"Selected channel", +AllChannels::LabL ) );
}

void CColorPickerDoc::SaveToRegistry( ) {
    RawLabTriplet labValues { ScaleLabColor( m_LabColor.GetChannelValues( ) ) };
    theApp.WriteProfileInt( L"Settings\\Saved Values",  L"LabL",  labValues[ +LabChannels::L] );
    theApp.WriteProfileInt( L"Settings\\Saved Values",  L"LabA",  labValues[ +LabChannels::a] );
    theApp.WriteProfileInt( L"Settings\\Saved Values",  L"LabB",  labValues[ +LabChannels::b] );

    SrgbTriplet srgbValues { m_SrgbColor.GetChannelValues( ) };
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbR", srgbValues[+SrgbChannels::R] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbG", srgbValues[+SrgbChannels::G] );
    theApp.WriteProfileInt( L"Settings\\Saved Values", L"SrgbB", srgbValues[+SrgbChannels::B] );

    theApp.WriteProfileInt( L"Settings", L"Inverted", m_fInverted ? 1 : 0 );

    theApp.WriteProfileInt( L"Settings", L"Selected channel", +m_SelectedChannel );
}
