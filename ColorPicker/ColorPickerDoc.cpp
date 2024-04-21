#include "pch.h"

#include "ColorPickerDoc.h"

#include "ColorPicker.h"

IMPLEMENT_DYNCREATE( CColorPickerDoc, CDocument )

BEGIN_MESSAGE_MAP( CColorPickerDoc, CDocument )
END_MESSAGE_MAP( )

BOOL CColorPickerDoc::OnNewDocument( ) {
    if ( !CDocument::OnNewDocument( ) ) {
        return FALSE;
    }

    // Reinitialization (SDI applications will reuse this document)

    m_LabColor  = { 100,   0,   0 };
    m_SrgbColor = { 255, 255, 255 };

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
