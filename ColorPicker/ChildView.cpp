#include "pch.h"

#include "ChildView.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#include "ChannelInformation.h"

//===================//
// Hair space: -> <- //
//===================//

IMPLEMENT_DYNCREATE( CChildView, CFormView )

BEGIN_MESSAGE_MAP( CChildView, CFormView )
    ON_BN_CLICKED( IDCLOSE,             &CChildView::OnCloseButtonClicked )
    ON_EN_UPDATE ( IDC_HEX_COLOR_VALUE, &CChildView::OnHexColorUpdate     )

    ON_CONTROL_RANGE( BN_CLICKED,   IDC_LAB_L_CHANNEL, IDC_SRGB_B_CHANNEL,  &CChildView::OnChannelButtonClicked )
    ON_CONTROL_RANGE( EN_KILLFOCUS, IDC_LAB_L_VALUE,   IDC_HEX_COLOR_VALUE, &CChildView::OnEditLostFocus        )
    ON_CONTROL_RANGE( EN_SETFOCUS,  IDC_LAB_L_VALUE,   IDC_HEX_COLOR_VALUE, &CChildView::OnEditGotFocus         )
    ON_CONTROL_RANGE( EN_UPDATE,    IDC_LAB_L_VALUE,   IDC_SRGB_B_VALUE,    &CChildView::OnColorValueUpdate     )

    ON_UPDATE_COMMAND_UI( ID_EDIT_CUT,        &CChildView::OnUpdateEditCut       )
    ON_UPDATE_COMMAND_UI( ID_EDIT_COPY,       &CChildView::OnUpdateEditCopy      )
    ON_UPDATE_COMMAND_UI( ID_EDIT_PASTE,      &CChildView::OnUpdateEditPaste     )
    ON_UPDATE_COMMAND_UI( ID_EDIT_CLEAR,      &CChildView::OnUpdateEditClear     )
    ON_UPDATE_COMMAND_UI( ID_EDIT_UNDO,       &CChildView::OnUpdateEditUndo      )
    ON_UPDATE_COMMAND_UI( ID_EDIT_SELECT_ALL, &CChildView::OnUpdateEditSelectAll )
    ON_UPDATE_COMMAND_UI( ID_VIEW_INVERT,     &CChildView::OnUpdateViewInvert    )

    ON_COMMAND( ID_EDIT_CUT,        &CChildView::OnEditCut       )
    ON_COMMAND( ID_EDIT_COPY,       &CChildView::OnEditCopy      )
    ON_COMMAND( ID_EDIT_PASTE,      &CChildView::OnEditPaste     )
    ON_COMMAND( ID_EDIT_CLEAR,      &CChildView::OnEditClear     )
    ON_COMMAND( ID_EDIT_UNDO,       &CChildView::OnEditUndo      )
    ON_COMMAND( ID_EDIT_SELECT_ALL, &CChildView::OnEditSelectAll )
    ON_COMMAND( ID_VIEW_INVERT,     &CChildView::OnViewInvert    )

    ON_NOTIFY( ZSBN_MOUSEMOVE, IDC_Z_STRIP, &CChildView::OnZStripMouseMove )
    ON_NOTIFY( ZSBN_MOUSEMOVE, IDC_XY_GRID, &CChildView::OnXyGridMouseMove )
END_MESSAGE_MAP( )

namespace {

    //
    // Constants
    //

    std::unordered_map<AllChannels, AllChannelsTriplet> const _ChannelXyzTriplets {
        { AllChannels::LabL,  { { AllChannels::LabA,  AllChannels::LabB,  AllChannels::LabL  } } },
        { AllChannels::LabA,  { { AllChannels::LabB,  AllChannels::LabL,  AllChannels::LabA  } } },
        { AllChannels::LabB,  { { AllChannels::LabA,  AllChannels::LabL,  AllChannels::LabB  } } },
        { AllChannels::SrgbR, { { AllChannels::SrgbB, AllChannels::SrgbG, AllChannels::SrgbR } } },
        { AllChannels::SrgbG, { { AllChannels::SrgbB, AllChannels::SrgbR, AllChannels::SrgbG } } },
        { AllChannels::SrgbB, { { AllChannels::SrgbR, AllChannels::SrgbG, AllChannels::SrgbB } } }
    };

    std::unordered_map<UINT, AllChannels> const _ControlIdToChannel {
        { IDC_LAB_L_CHANNEL,  AllChannels::LabL  },
        { IDC_LAB_A_CHANNEL,  AllChannels::LabA  },
        { IDC_LAB_B_CHANNEL,  AllChannels::LabB  },
        { IDC_SRGB_R_CHANNEL, AllChannels::SrgbR },
        { IDC_SRGB_G_CHANNEL, AllChannels::SrgbG },
        { IDC_SRGB_B_CHANNEL, AllChannels::SrgbB }
    };

    //
    // Private functions
    //

    AllChannels _MapControlIdToChannel( UINT const uId ) {
        try {
            return _ControlIdToChannel.at( uId );
        }
        catch ( ... ) {
            /*empty*/
        }
        return AllChannels::unknown;
    }

    void _AdjustPosition( CWnd* ctrl, SIZE const& adjust ) {
        WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

        ctrl->GetWindowPlacement( &wp );
        wp.rcNormalPosition.left   += adjust.cx;
        wp.rcNormalPosition.top    += adjust.cy;
        wp.rcNormalPosition.right  += adjust.cx;
        wp.rcNormalPosition.bottom += adjust.cy;
        ctrl->SetWindowPlacement( &wp );
    }

    void _AdjustSize( CWnd* ctrl, SIZE const& adjust ) {
        WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

        ctrl->GetWindowPlacement( &wp );
        wp.rcNormalPosition.right  += adjust.cx;
        wp.rcNormalPosition.bottom += adjust.cy;
        ctrl->SetWindowPlacement( &wp );
    }

    void _SetPosition( CWnd* ctrl, SIZE const& adjust ) {
        WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

        ctrl->GetWindowPlacement( &wp );
        CRect rect { wp.rcNormalPosition };
        rect.MoveToXY( adjust.cx, adjust.cy );
        wp.rcNormalPosition = rect;
        ctrl->SetWindowPlacement( &wp );
    }

    void _SetSize( CWnd* ctrl, SIZE const& size ) {
        WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

        ctrl->GetWindowPlacement( &wp );
        wp.rcNormalPosition.right  = wp.rcNormalPosition.left + size.cx;
        wp.rcNormalPosition.bottom = wp.rcNormalPosition.top  + size.cy;
        ctrl->SetWindowPlacement( &wp );
    }

    bool _IsTextSelected( CEdit const* pEdit ) {
        int nStartIndex, nEndIndex;

        pEdit->GetSel( nStartIndex, nEndIndex );
        return nStartIndex != nEndIndex;
    }

    wchar_t* _SafeGetWindowText( CEdit const& edit ) {
        int cbText { edit.GetWindowTextLength( ) };
        if ( cbText < 1 ) {
            debug( L"_SafeGetWindowText: bail 1: no text in control\n" );
            return nullptr;
        }
        ++cbText;

        wchar_t* pwszText { new wchar_t[cbText + 1u] { } };
        if ( !pwszText ) {
            debug( L"_SafeGetWindowText: bail 2: memory allocation failure\n" );
            return nullptr;
        }
        if ( edit.GetWindowText( pwszText, cbText ) < 1 ) {
            delete[] pwszText;
            debug( L"_SafeGetWindowText: bail 3: GetWindowText failed\n" );
            return nullptr;
        }

        for ( int index { cbText - 2 }; index >= 0; --index ) {
            if ( !iswspace( pwszText[index] ) ) {
                break;
            }
            pwszText[index] = '\0';
        }

        return pwszText;
    }

    bool _GetValueFromEdit( CEdit const& edit, int& nValue ) {
        wchar_t* pwszText { _SafeGetWindowText( edit ) };
        if ( !pwszText ) {
            debug( L"_GetValueFromEdit: bail 1: _SafeGetWindowText returned nullptr\n" );
            return false;
        }

        wchar_t* pwszEnd { };
        long tmp { wcstol( pwszText, &pwszEnd, 10 ) };
        if ( !pwszEnd || *pwszEnd || ( tmp < static_cast<long>( INT_MIN ) ) || ( tmp > static_cast<long>( INT_MAX ) ) ) {
            delete[] pwszText;
            debug( L"_GetValueFromEdit: bail 2: garbage in number\n" );
            return false;
        }

        nValue = static_cast<int>( tmp );
        delete[] pwszText;
        return true;
    }

    bool _GetValueAndChangedFromEdit( CEdit const& edit, int& nValue, bool& fChanged ) {
        int nOldValue { nValue };

        if ( _GetValueFromEdit( edit, nValue ) ) {
            fChanged = nOldValue != nValue;
            return true;
        } else {
            return false;
        }
    }

    void _PutValueToEdit( CEdit& edit, int const nValue ) {
        CString str;
        str.Format( L"%d", nValue );
        edit.SetWindowText( str );
    }

    template<typename T>
    [[nodiscard]] bool _UpdateValueIfEditChanged( CEdit const& edit, T const oldValue, T& newValue ) {
        int n { static_cast<int>( oldValue ) };
        bool fChanged { };

        if ( _GetValueAndChangedFromEdit( edit, n, fChanged ) && fChanged ) {
            newValue = static_cast<T>( n );
            return true;
        } else {
            return false;
        }
    }

    template<typename T>
    void _UpdateEditIfValueChanged( CEdit& edit, T const oldValue, T const newValue ) {
        if ( newValue != oldValue ) {
            _PutValueToEdit( edit, static_cast<int>( newValue ) );
        }
    }

    bool _GetHexColorFromEdit( CEdit const& edit, SrgbTriplet& values ) {
        wchar_t* pwszText { _SafeGetWindowText( edit ) };
        if ( !pwszText ) {
            debug( L"_GetHexColorFromEdit: bail 1: _SafeGetWindowText returned nullptr\n" );
            return false;
        }

        wchar_t* pwszEnd { };
        long tmp { wcstol( pwszText, &pwszEnd, 16 ) };
        if ( !pwszEnd || *pwszEnd || ( tmp < 0 ) || ( tmp > 0xFFFFFF ) ) {
            delete[] pwszText;
            debug( L"_GetHexColorFromEdit: bail 2: garbage in number\n" );
            return false;
        }

        int r {   tmp >> 16          };
        int g { ( tmp >>  8 ) & 0xFF };
        int b {   tmp         & 0xFF };

        values = { { static_cast<SrgbValueT>( r ), static_cast<SrgbValueT>( g ), static_cast<SrgbValueT>( b ) } };
        delete[] pwszText;
        return true;
    }

    bool _GetHexColorAndChangedFromEdit( CEdit const& edit, SrgbTriplet& values, bool& fChanged ) {
        SrgbTriplet oldValues { values };

        if ( _GetHexColorFromEdit( edit, values ) ) {
            fChanged = oldValues != values;
            return true;
        } else {
            return false;
        }
    }

    void _PutHexColorToEdit( CEdit& edit, SrgbTriplet const& values ) {
        unsigned r { values[+SrgbChannels::R] };
        unsigned g { values[+SrgbChannels::G] };
        unsigned b { values[+SrgbChannels::B] };

        CString str;
        str.Format( L"%06.6X", static_cast<int>( ( r << 16u ) | ( g << 8u ) | b ) );
        edit.SetWindowText( str );
    }

}

CEdit* CChildView::MapControlIdToPointer( UINT const uId ) {
    try {
        return m_mapEditControls.at( uId );
    }
    catch ( ... ) {
        return nullptr;
    }
}

void CChildView::UpdateBitmaps( bool const fUpdateZ, bool const fUpdateXy ) {
    if ( m_fBlockBitmapUpdates ) {
        return;
    }

    m_staticSwatch.SetColor( m_pDoc->GetSrgbColor( ) );

    if ( fUpdateZ ) {
        m_staticZStrip.UpdateBitmap( );
    }

    if ( fUpdateXy ) {
        m_staticXyGrid.UpdateBitmap( );
    }
}

void CChildView::DoDataExchange( CDataExchange* pDX ) {
    CFormView::DoDataExchange( pDX );

    DDX_Control( pDX, IDC_GROUPBOX_LAB,    m_groupBoxLab   );
    DDX_Control( pDX, IDC_LAB_L_CHANNEL,   m_radioLabL     );
    DDX_Control( pDX, IDC_LAB_A_CHANNEL,   m_radioLabA     );
    DDX_Control( pDX, IDC_LAB_B_CHANNEL,   m_radioLabB     );
    DDX_Control( pDX, IDC_LAB_L_VALUE,     m_editLabL      );
    DDX_Control( pDX, IDC_LAB_A_VALUE,     m_editLabA      );
    DDX_Control( pDX, IDC_LAB_B_VALUE,     m_editLabB      );

    DDX_Control( pDX, IDC_GROUPBOX_SRGB,   m_groupBoxSrgb  );
    DDX_Control( pDX, IDC_SRGB_R_CHANNEL,  m_radioSrgbR    );
    DDX_Control( pDX, IDC_SRGB_G_CHANNEL,  m_radioSrgbG    );
    DDX_Control( pDX, IDC_SRGB_B_CHANNEL,  m_radioSrgbB    );
    DDX_Control( pDX, IDC_SRGB_R_VALUE,    m_editSrgbR     );
    DDX_Control( pDX, IDC_SRGB_G_VALUE,    m_editSrgbG     );
    DDX_Control( pDX, IDC_SRGB_B_VALUE,    m_editSrgbB     );

    DDX_Control( pDX, IDC_HEX_COLOR_LABEL, m_labelHexColor );
    DDX_Control( pDX, IDC_HEX_COLOR_VALUE, m_editHexColor  );

    DDX_Control( pDX, IDC_SWATCH,          m_staticSwatch  );
    DDX_Control( pDX, IDC_Z_STRIP,         m_staticZStrip  );
    DDX_Control( pDX, IDC_XY_GRID,         m_staticXyGrid  );

    DDX_Control( pDX, IDCLOSE,             m_buttonClose   );
}

BOOL CChildView::PreCreateWindow( CREATESTRUCT& cs ) {
    if ( !CWnd::PreCreateWindow( cs ) ) {
        return FALSE;
    }

    cs.style     &= ~WindowStylesToRemove;
    cs.dwExStyle &= ~ExtendedWindowStylesToRemove;
    cs.lpszClass  = AfxRegisterWndClass( CS_DROPSHADOW | CS_SAVEBITS, ::LoadCursor( nullptr, IDC_ARROW ), reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), nullptr );

    return TRUE;
}

void CChildView::OnInitialUpdate( ) {
    CFormView::OnInitialUpdate( );

    //
    // Initialize member variables
    //

    m_mapRadioButtons = {
        { AllChannels::LabL,   &m_radioLabL  },
        { AllChannels::LabA,   &m_radioLabA  },
        { AllChannels::LabB,   &m_radioLabB  },
        { AllChannels::SrgbR,  &m_radioSrgbR },
        { AllChannels::SrgbG,  &m_radioSrgbG },
        { AllChannels::SrgbB,  &m_radioSrgbB }
    };
    m_mapEditControls = {
        { IDC_LAB_L_VALUE,     &m_editLabL     },
        { IDC_LAB_A_VALUE,     &m_editLabA     },
        { IDC_LAB_B_VALUE,     &m_editLabB     },
        { IDC_SRGB_R_VALUE,    &m_editSrgbR    },
        { IDC_SRGB_G_VALUE,    &m_editSrgbG    },
        { IDC_SRGB_B_VALUE,    &m_editSrgbB    },
        { IDC_HEX_COLOR_VALUE, &m_editHexColor },
    };

    m_pDoc = static_downcast<CColorPickerDoc>( GetDocument( ) );

    //
    // Adjust UI controls
    //

    SIZE constexpr adjustUp2   { 0, -2 }; SIZE constexpr adjustLeft2  { -2, 0 };
    SIZE constexpr adjustUp1   { 0, -1 }; SIZE constexpr adjustLeft1  { -1, 0 };
    SIZE constexpr adjustDown1 { 0,  1 }; SIZE constexpr adjustRight1 {  1, 0 };
    SIZE constexpr adjustDown2 { 0,  2 }; SIZE constexpr adjustRight2 {  2, 0 };

    _AdjustSize    ( &m_groupBoxLab,   adjustUp1    );
    _AdjustPosition( &m_radioLabL,     adjustDown1  );
    _AdjustPosition( &m_radioLabA,     adjustUp1    );
    _AdjustPosition( &m_radioLabB,     adjustUp2    );
    // m_editLabL
    // m_editLabA
    _AdjustPosition( &m_editLabB,      adjustUp2    );

    _AdjustPosition( &m_groupBoxSrgb,  adjustDown2  );
    _AdjustSize    ( &m_groupBoxSrgb,  adjustUp1    );
    _AdjustPosition( &m_radioSrgbR,    adjustDown2  );
    // m_radioSrgbG
    _AdjustPosition( &m_radioSrgbB,    adjustUp2    );
    _AdjustPosition( &m_editSrgbR,     adjustDown2  );
    _AdjustPosition( &m_editSrgbG,     adjustDown2  );
    _AdjustPosition( &m_editSrgbB,     adjustUp1    );

    _AdjustPosition( &m_editHexColor,  adjustUp1    );
    _AdjustSize    ( &m_editHexColor,  {   1,   0 } );

    _AdjustSize    ( &m_staticSwatch,  {  -1,  -1 } );

    _SetSize       ( &m_staticZStrip,  {  20, 256 } );

    _AdjustPosition( &m_staticXyGrid,  adjustLeft1  );
    _SetSize       ( &m_staticXyGrid,  { 256, 256 } );

    _AdjustPosition( &m_buttonClose,   {  -1,  -2 } );

    //
    // Load settings from registry
    //

    m_fBlockBitmapUpdates = true;
    m_pDoc->LoadFromRegistry( );

    bool               const  fGuiInverted     { !m_pDoc->IsInverted( ) };

    LabTriplet         const   labValues       { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet        const  srgbValues       { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    AllChannels        const  selectedChannel  { m_pDoc->GetSelectedChannel( ) };
    AllChannelsTriplet const& selectedChannels { _ChannelXyzTriplets.at( selectedChannel ) };

    m_channelX = selectedChannels[0];
    m_channelY = selectedChannels[1];
    m_channelZ = selectedChannels[2];

    //
    // Apply them to the controls
    //

    for ( AllChannels channel { AllChannels::Min }; channel <= AllChannels::Max; ++channel ) {
        m_mapRadioButtons.at( channel )->SetCheck( ( channel == selectedChannel ) ? BST_CHECKED : BST_UNCHECKED );
    }

    m_staticZStrip.SetDocument( m_pDoc );
    m_staticZStrip.SetChannel( m_channelZ );
    m_staticZStrip.SetInverted( fGuiInverted );

    m_staticXyGrid.SetDocument( m_pDoc );
    m_staticXyGrid.SetChannels( m_channelX, m_channelY, m_channelZ );
    m_staticXyGrid.SetInverted( fGuiInverted );

    _PutValueToEdit   ( m_editLabL,      labValues[ +LabChannels::L] );
    _PutValueToEdit   ( m_editLabA,      labValues[ +LabChannels::a] );
    _PutValueToEdit   ( m_editLabB,      labValues[ +LabChannels::b] );
    _PutValueToEdit   ( m_editSrgbR,    srgbValues[+SrgbChannels::R] );
    _PutValueToEdit   ( m_editSrgbG,    srgbValues[+SrgbChannels::G] );
    _PutValueToEdit   ( m_editSrgbB,    srgbValues[+SrgbChannels::B] );
    _PutHexColorToEdit( m_editHexColor, srgbValues );

    m_fBlockBitmapUpdates = false;
    UpdateBitmaps( );
}

void CChildView::OnUpdateEditCut( CCmdUI* pCmdUI ) {
    pCmdUI->Enable( m_pCurrentEdit && _IsTextSelected( m_pCurrentEdit ) );
}

void CChildView::OnUpdateEditCopy( CCmdUI* pCmdUI ) {
    pCmdUI->Enable( m_pCurrentEdit && _IsTextSelected( m_pCurrentEdit ) );
}

void CChildView::OnUpdateEditPaste( CCmdUI* pCmdUI ) {
    pCmdUI->Enable( m_pCurrentEdit && ::IsClipboardFormatAvailable( CF_TEXT ) );
}

void CChildView::OnUpdateEditClear( CCmdUI* pCmdUI ) {
    pCmdUI->Enable( m_pCurrentEdit && _IsTextSelected( m_pCurrentEdit ) );
}

void CChildView::OnUpdateEditUndo( CCmdUI* pCmdUI ) {
    pCmdUI->Enable( m_pCurrentEdit && m_pCurrentEdit->CanUndo( ) );
}

void CChildView::OnUpdateViewInvert( CCmdUI* pCmdUI ) {
    pCmdUI->Enable( TRUE );
    pCmdUI->SetCheck( m_pDoc->IsInverted( ) ? 1 : 0 );
}

void CChildView::OnUpdateEditSelectAll( CCmdUI* pCmdUI ) {
    pCmdUI->Enable( m_pCurrentEdit && _IsTextSelected( m_pCurrentEdit ) );
}

void CChildView::OnEditCut( ) {
    m_pCurrentEdit->Cut( );
}

void CChildView::OnEditCopy( ) {
    m_pCurrentEdit->Copy( );
}

void CChildView::OnEditPaste( ) {
    m_pCurrentEdit->Paste( );
}

void CChildView::OnEditClear( ) {
    m_pCurrentEdit->Clear( );
}

void CChildView::OnEditUndo( ) {
    m_pCurrentEdit->Undo( );
}

void CChildView::OnEditSelectAll( ) {
    m_pCurrentEdit->SetSel( 0, -1 );
}

void CChildView::OnViewInvert( ) {
    m_pDoc->ToggleInverted( );

    bool const fGuiInverted { !m_pDoc->IsInverted( ) };
    m_staticZStrip.SetInverted( fGuiInverted );
    m_staticXyGrid.SetInverted( fGuiInverted );

    UpdateBitmaps( );
}

void CChildView::OnEditGotFocus( UINT uId ) {
    m_pCurrentEdit = MapControlIdToPointer( uId );
}

void CChildView::OnEditLostFocus( UINT uId ) {
    if ( !m_pCurrentEdit ) {
        debug( L"CChildView::OnEditLostFocus: uId %d: weird, lost focus with m_pCurrentEdit==nullptr\n" );
        return;
    }

    if ( int n; _GetValueFromEdit( *m_pCurrentEdit, n ) ) {
        // TODO range check
    } else {
        wchar_t* pwszText { _SafeGetWindowText( *m_pCurrentEdit ) };
        debug( L"CChildView::OnEditLostFocus: garbage in edit control %u: '%s'\n", uId, pwszText );
        delete[] pwszText;
    }

    m_pCurrentEdit = nullptr;
}

void CChildView::OnCloseButtonClicked( ) {
    m_pDoc->SaveToRegistry( );

    ::PostQuitMessage( 0 );
}

void CChildView::OnChannelButtonClicked( UINT const uId ) {
    AllChannels channel { _MapControlIdToChannel( uId ) };
    if ( channel == AllChannels::unknown ) {
        return;
    }

    AllChannelsTriplet const  oldChannels { m_channelX, m_channelY, m_channelZ };
    AllChannelsTriplet const&    channels { _ChannelXyzTriplets.at( channel )  };
    if ( oldChannels != channels ) {
        m_channelX = channels[0];
        m_channelY = channels[1];
        m_channelZ = channels[2];

        m_staticZStrip.SetChannel( m_channelZ );
        m_staticXyGrid.SetChannels( m_channelX, m_channelY, m_channelZ );

        m_pDoc->SetSelectedChannel( m_channelZ );

        UpdateBitmaps( );
    }
}

void CChildView::OnColorValueUpdate( UINT const uId ) {
    LabTriplet  oldLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet oldSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };
    LabTriplet  newLabValues  {  oldLabValues };
    SrgbTriplet newSrgbValues { oldSrgbValues };
    bool        fChanged      { };

    switch ( uId ) {
        case IDC_LAB_L_VALUE: fChanged = _UpdateValueIfEditChanged( m_editLabL, oldLabValues[+LabChannels::L], newLabValues[+LabChannels::L] ); goto Lab;
        case IDC_LAB_A_VALUE: fChanged = _UpdateValueIfEditChanged( m_editLabA, oldLabValues[+LabChannels::a], newLabValues[+LabChannels::a] ); goto Lab;
        case IDC_LAB_B_VALUE: fChanged = _UpdateValueIfEditChanged( m_editLabB, oldLabValues[+LabChannels::b], newLabValues[+LabChannels::b] );
        {
Lab:
            if ( fChanged ) {
                m_pDoc->SetColor( LabColor { newLabValues[+LabChannels::L], newLabValues[+LabChannels::a], newLabValues[+LabChannels::b] } );
                newSrgbValues = m_pDoc->GetSrgbColor( ).GetChannelValues( );

                _UpdateEditIfValueChanged( m_editSrgbR, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] );
                _UpdateEditIfValueChanged( m_editSrgbG, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] );
                _UpdateEditIfValueChanged( m_editSrgbB, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
                _PutHexColorToEdit( m_editHexColor, newSrgbValues );
            }
            break;
        }

        case IDC_SRGB_R_VALUE: fChanged = _UpdateValueIfEditChanged( m_editSrgbR, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] ); goto sRGB;
        case IDC_SRGB_G_VALUE: fChanged = _UpdateValueIfEditChanged( m_editSrgbG, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] ); goto sRGB;
        case IDC_SRGB_B_VALUE: fChanged = _UpdateValueIfEditChanged( m_editSrgbB, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
        {
sRGB:
            if ( fChanged ) {
                m_pDoc->SetColor( SrgbColor { newSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::B] } );
                newLabValues = m_pDoc->GetLabColor( ).GetChannelValues( );

                _UpdateEditIfValueChanged( m_editLabL, oldLabValues[+LabChannels::L], newLabValues[+LabChannels::L] );
                _UpdateEditIfValueChanged( m_editLabA, oldLabValues[+LabChannels::a], newLabValues[+LabChannels::a] );
                _UpdateEditIfValueChanged( m_editLabB, oldLabValues[+LabChannels::b], newLabValues[+LabChannels::b] );
                _PutHexColorToEdit( m_editHexColor, newSrgbValues );
            }
            break;
        }
    }

    UpdateBitmaps( );
}

void CChildView::OnHexColorUpdate( ) {
    LabTriplet  oldLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet oldSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };
    LabTriplet  newLabValues  {  oldLabValues };
    SrgbTriplet newSrgbValues { oldSrgbValues };
    bool        fChanged      { };

    if ( !_GetHexColorAndChangedFromEdit( m_editHexColor, newSrgbValues, fChanged ) ) {
        return;
    }

    if ( fChanged ) {
        m_pDoc->SetColor( SrgbColor { newSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::B] } );
        newLabValues = m_pDoc->GetLabColor( ).GetChannelValues( );

        _UpdateEditIfValueChanged( m_editLabL,   oldLabValues[ +LabChannels::L],  newLabValues[ +LabChannels::L] );
        _UpdateEditIfValueChanged( m_editLabA,   oldLabValues[ +LabChannels::a],  newLabValues[ +LabChannels::a] );
        _UpdateEditIfValueChanged( m_editLabB,   oldLabValues[ +LabChannels::b],  newLabValues[ +LabChannels::b] );
        _UpdateEditIfValueChanged( m_editSrgbR, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] );
        _UpdateEditIfValueChanged( m_editSrgbG, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] );
        _UpdateEditIfValueChanged( m_editSrgbB, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
    }

    UpdateBitmaps( );
}

void CChildView::OnZStripMouseMove( NMHDR* pNotifyStruct, LRESULT* result ) {
    ZSB_MOUSEMOVE* mm { static_cast<ZSB_MOUSEMOVE*>( pNotifyStruct ) };
    int y { mm->point.y };

    m_fBlockBitmapUpdates = true;

    LabTriplet  oldLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet oldSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    m_pDoc->SetChannelValue( m_channelZ, y );

    LabTriplet  newLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet newSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    _UpdateEditIfValueChanged(  m_editLabL,  oldLabValues[ +LabChannels::L],  newLabValues[ +LabChannels::L] );
    _UpdateEditIfValueChanged(  m_editLabA,  oldLabValues[ +LabChannels::a],  newLabValues[ +LabChannels::a] );
    _UpdateEditIfValueChanged(  m_editLabB,  oldLabValues[ +LabChannels::b],  newLabValues[ +LabChannels::b] );
    _UpdateEditIfValueChanged( m_editSrgbR, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] );
    _UpdateEditIfValueChanged( m_editSrgbG, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] );
    _UpdateEditIfValueChanged( m_editSrgbB, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
    if ( oldSrgbValues != newSrgbValues ) {
        _PutHexColorToEdit( m_editHexColor, newSrgbValues );
    }

    m_fBlockBitmapUpdates = false;
    UpdateBitmaps( false, true );

    *result = 0;
}

void CChildView::OnXyGridMouseMove( NMHDR* pNotifyStruct, LRESULT* result ) {
    ZSB_MOUSEMOVE* mm { static_cast<ZSB_MOUSEMOVE*>( pNotifyStruct ) };
    int x { mm->point.x };
    int y { mm->point.y };

    m_fBlockBitmapUpdates = true;

    LabTriplet  oldLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet oldSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    m_pDoc->SetChannelValue( m_channelX, x );
    m_pDoc->SetChannelValue( m_channelY, y );

    LabTriplet  newLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet newSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    _UpdateEditIfValueChanged(  m_editLabL,  oldLabValues[ +LabChannels::L],  newLabValues[ +LabChannels::L] );
    _UpdateEditIfValueChanged(  m_editLabA,  oldLabValues[ +LabChannels::a],  newLabValues[ +LabChannels::a] );
    _UpdateEditIfValueChanged(  m_editLabB,  oldLabValues[ +LabChannels::b],  newLabValues[ +LabChannels::b] );
    _UpdateEditIfValueChanged( m_editSrgbR, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] );
    _UpdateEditIfValueChanged( m_editSrgbG, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] );
    _UpdateEditIfValueChanged( m_editSrgbB, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
    if ( oldSrgbValues != newSrgbValues ) {
        _PutHexColorToEdit( m_editHexColor, newSrgbValues );
    }

    m_fBlockBitmapUpdates = false;
    UpdateBitmaps( true, false );

    *result = 0;
}
