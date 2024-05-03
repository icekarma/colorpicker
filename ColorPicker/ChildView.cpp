#include "pch.h"

#include "ChildView.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"
#include "ChannelInformation.h"
#include "Utils.h"

IMPLEMENT_DYNCREATE( CChildView, CFormView )

BEGIN_MESSAGE_MAP( CChildView, CFormView )
    ON_BN_CLICKED        (                 IDCLOSE,                           &CChildView::OnCloseButtonClicked   )
    ON_EN_UPDATE         (                 IDC_HEX_COLOR_VALUE,               &CChildView::OnHexColorUpdate       )

    ON_CONTROL_RANGE     ( BN_CLICKED,     IDC_LAB_L_LABEL, IDC_SRGB_B_LABEL, &CChildView::OnChannelRadioClicked  )
    ON_CONTROL_RANGE     ( EN_KILLFOCUS,   IDC_LAB_L_VALUE, IDC_SRGB_B_LABEL, &CChildView::OnValueEditLostFocus   )
    ON_CONTROL_RANGE     ( EN_SETFOCUS,    IDC_LAB_L_VALUE, IDC_SRGB_B_LABEL, &CChildView::OnValueEditGotFocus    )
    ON_CONTROL_RANGE     ( EN_UPDATE,      IDC_LAB_L_VALUE, IDC_SRGB_B_VALUE, &CChildView::OnValueEditUpdate      )

    ON_UPDATE_COMMAND_UI (                 ID_EDIT_CUT,                       &CChildView::OnUpdateEditCut        )
    ON_UPDATE_COMMAND_UI (                 ID_EDIT_COPY,                      &CChildView::OnUpdateEditCopy       )
    ON_UPDATE_COMMAND_UI (                 ID_EDIT_PASTE,                     &CChildView::OnUpdateEditPaste      )
    ON_UPDATE_COMMAND_UI (                 ID_EDIT_CLEAR,                     &CChildView::OnUpdateEditClear      )
    ON_UPDATE_COMMAND_UI (                 ID_EDIT_UNDO,                      &CChildView::OnUpdateEditUndo       )
    ON_UPDATE_COMMAND_UI (                 ID_EDIT_SELECT_ALL,                &CChildView::OnUpdateEditSelectAll  )
    ON_UPDATE_COMMAND_UI (                 ID_VIEW_INVERT,                    &CChildView::OnUpdateViewInvert     )

    ON_COMMAND           (                 ID_EDIT_CUT,                       &CChildView::OnEditCut              )
    ON_COMMAND           (                 ID_EDIT_COPY,                      &CChildView::OnEditCopy             )
    ON_COMMAND           (                 ID_EDIT_PASTE,                     &CChildView::OnEditPaste            )
    ON_COMMAND           (                 ID_EDIT_CLEAR,                     &CChildView::OnEditClear            )
    ON_COMMAND           (                 ID_EDIT_UNDO,                      &CChildView::OnEditUndo             )
    ON_COMMAND           (                 ID_EDIT_SELECT_ALL,                &CChildView::OnEditSelectAll        )
    ON_COMMAND           (                 ID_VIEW_INVERT,                    &CChildView::OnViewInvert           )

    ON_NOTIFY            ( ZSBN_MOUSEMOVE, IDC_Z_STRIP,                       &CChildView::OnZStripMouseMove      )
    ON_NOTIFY            ( ZSBN_MOUSEMOVE, IDC_XY_GRID,                       &CChildView::OnXyGridMouseMove      )
END_MESSAGE_MAP( )

//
// Global constants
//

std::unordered_map<UINT, AllChannels> const g_mapLabelControlIdToChannel {
    { IDC_LAB_L_LABEL,  AllChannels::LabL  },
    { IDC_LAB_A_LABEL,  AllChannels::LabA  },
    { IDC_LAB_B_LABEL,  AllChannels::LabB  },
    { IDC_SRGB_R_LABEL, AllChannels::SrgbR },
    { IDC_SRGB_G_LABEL, AllChannels::SrgbG },
    { IDC_SRGB_B_LABEL, AllChannels::SrgbB }
};

std::unordered_map<UINT, AllChannels> const g_mapValueControlIdToChannel {
    { IDC_LAB_L_VALUE,  AllChannels::LabL  },
    { IDC_LAB_A_VALUE,  AllChannels::LabA  },
    { IDC_LAB_B_VALUE,  AllChannels::LabB  },
    { IDC_SRGB_R_VALUE, AllChannels::SrgbR },
    { IDC_SRGB_G_VALUE, AllChannels::SrgbG },
    { IDC_SRGB_B_VALUE, AllChannels::SrgbB }
};

namespace {

    //
    // Private constants
    //

    AllChannelsTriplet const _ChannelXyzTriplets[6] {
        { AllChannels::LabA,  AllChannels::LabB,  AllChannels::LabL  },
        { AllChannels::LabB,  AllChannels::LabL,  AllChannels::LabA  },
        { AllChannels::LabA,  AllChannels::LabL,  AllChannels::LabB  },
        { AllChannels::SrgbB, AllChannels::SrgbG, AllChannels::SrgbR },
        { AllChannels::SrgbB, AllChannels::SrgbR, AllChannels::SrgbG },
        { AllChannels::SrgbR, AllChannels::SrgbG, AllChannels::SrgbB }
    };

    //
    // Private variables
    //

    constinit CChildView* g_pChildView { };

    //
    // Private functions
    //

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

    [[nodiscard]] bool _IsTextSelected( CEdit const* pEdit ) {
        int nStartIndex, nEndIndex;

        pEdit->GetSel( nStartIndex, nEndIndex );
        return nStartIndex != nEndIndex;
    }

    [[nodiscard]] CString _SafeGetWindowText( CEdit const& edit ) {
        int cbText { edit.GetWindowTextLength( ) };
        if ( cbText < 1 ) {
            debug( L"_SafeGetWindowText: no text in control\n" );
            return { };
        }

        CString str;
        wchar_t* pwszStr { str.GetBufferSetLength( cbText + 1 ) };
        if ( edit.GetWindowText( pwszStr, cbText + 1 ) < 1 ) {
            debug( L"_SafeGetWindowText: GetWindowText failed\n" );
            return { };
        }
        str.ReleaseBuffer( cbText );

        return str.Trim( );
    }

    [[nodiscard]] bool _GetValueFromEdit( CEdit const& edit, int& nValue ) {
        CString strText { _SafeGetWindowText( edit ) };
        if ( strText.IsEmpty( ) ) {
            return false;
        }

        wchar_t* pwszEnd { };
        long tmp { wcstol( strText, &pwszEnd, 10 ) };
        if ( !pwszEnd || *pwszEnd ) {
            debug( L"_GetValueFromEdit: garbage in number: '%s'\n", pwszEnd );
            return false;
        }
        if constexpr ( sizeof( long ) > sizeof( int ) ) {
            if ( ( tmp < static_cast<long>( INT_MIN ) ) || ( tmp > static_cast<long>( INT_MAX ) ) ) {
                debug( L"_GetValueFromEdit: number out of range: %ld\n", tmp );
                return false;
            }
        }

        nValue = static_cast<int>( tmp );
        return true;
    }

    [[nodiscard]] bool _GetValueAndChangedFromEdit( CEdit const& edit, int& nValue, bool& fChanged ) {
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

    [[nodiscard]] bool _GetHexColorFromEdit( CEdit const& edit, SrgbTriplet& values ) {
        CString strText { _SafeGetWindowText( edit ) };
        if ( strText.IsEmpty( ) ) {
            debug( L"_GetHexColorFromEdit: no text in edit control\n" );
            return false;
        }
        if ( strText[0] == '#' ) {
            strText.Delete( 0, 1 );
        }

        wchar_t* pwszEnd { };
        long tmp { wcstol( strText, &pwszEnd, 16 ) };
        if ( !pwszEnd || *pwszEnd ) {
            debug( L"_GetHexColorFromEdit: garbage in number: '%s'\n", pwszEnd );
            return false;
        } else if ( ( tmp < 0 ) || ( tmp > 0xFFFFFF ) ) {
            debug( L"_GetHexColorFromEdit: number out of range: %ld\n", tmp );
            return false;
        }

        int r { static_cast<int>( ( tmp >> 16L ) & 0xFFL ) };
        int g { static_cast<int>( ( tmp >>  8L ) & 0xFFL ) };
        int b { static_cast<int>(   tmp          & 0xFFL ) };

        values = { { static_cast<SrgbValueT>( r ), static_cast<SrgbValueT>( g ), static_cast<SrgbValueT>( b ) } };
        return true;
    }

    [[nodiscard]] bool _GetHexColorAndChangedFromEdit( CEdit const& edit, SrgbTriplet& values, bool& fChanged ) {
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

    [[nodiscard]] DWORD _SetWindowProcedure( HWND const hWnd, WNDPROC const newWndProc, WNDPROC& oldWndProc ) {
        SetLastError( ERROR_SUCCESS );
        oldWndProc = reinterpret_cast<WNDPROC>( SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( newWndProc ) ) );
        return GetLastError( );
    }

    [[nodiscard]] int _ClipToChannelRange( AllChannels const channel, int const value ) {
        ChannelInformation const& channelInfo { AllChannelsInformation[+channel] };
        return std::min( std::max( value, channelInfo.m_minimumValue ), channelInfo.m_maximumValue );
    }

}

CChildView::CChildView( ):
    CFormView { IDD_CHILDVIEW }
{
#if defined _DEBUG
    if ( g_pChildView ) {
        debug( L"CChildView::`ctor: WARNING: On entry, g_pChildView is not nullptr! (0x%p)\n", g_pChildView );
        DebugBreak( );
    }
#endif // defined _DEBUG

    g_pChildView = this;
}

CChildView::~CChildView( ) {
#if defined _DEBUG
    if ( !g_pChildView ) {
        debug( L"CChildView::`dtor: WARNING: On entry, g_pChildView is nullptr!\n" );
        DebugBreak( );
    }
#endif // defined _DEBUG

    g_pChildView = nullptr;
}

LRESULT CChildView::_EditWndProc( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam ) {
#if defined _DEBUG
    if ( !g_pChildView ) {
        debug( L"CChildView::_EditWndProc: WARNING: On entry, g_pChildView is nullptr!\n" );
        DebugBreak( );
    }
#endif // defined _DEBUG

    return g_pChildView->EditWndProc( hwnd, uMessage, wParam, lParam );
}

LRESULT CChildView::EditWndProc( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam ) {
    //debug( L"CChildView::EditWndProc: hwnd: 0x%p, uMessage: 0x%04X, wParam: 0x%08lX, lParam: 0x%08lX; %s\n", hwnd, uMessage, wParam, lParam, static_cast<LPCWSTR>( GetNameForWindowsMessage( uMessage ) ) );

    WNDPROC wndProc { MapHwndToWndProc( hwnd ) };
    if ( !wndProc ) {
        debug( L"CChildView::EditWndProc: Uh oh: Window handle didn't map to a WNDPROC\n" );
        DebugBreak( );
        return 0;
    }

    AllChannels channel { MapHwndToChannel( hwnd ) };
    if ( channel != AllChannels::unknown ) {
        if ( ( uMessage == WM_KEYDOWN ) && EditControl_OnKeyDown( channel, static_cast<UINT>( wParam ), LOWORD( lParam ), HIWORD( lParam ) ) ) {
            return 0;
        }
    }

    return CallWindowProc( wndProc, hwnd, uMessage, wParam, lParam );
}

void CChildView::UpdateBitmaps( bool const fUpdateZ, bool const fUpdateXy ) {
    if ( m_nBlockBitmapUpdates ) {
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

void CChildView::SubclassEditControl( CEdit& pEdit, WNDPROC const newWndProc ) {
    HWND    hwndEdit { pEdit.GetSafeHwnd( ) };
    WNDPROC wndProc;

    if ( DWORD dwError { _SetWindowProcedure( hwndEdit, newWndProc, wndProc ) }; !dwError ) {
        m_mapHwndToWndProc[hwndEdit] = wndProc;
    } else {
        debug( L"CChildView::SubclassEditControl: pEdit 0x%p, hwnd 0x%p: SetWindowLongPtr failed: error %lu\n", &pEdit, hwndEdit, dwError );
        DebugBreak( );
    }
}

void CChildView::UnSubclassEditControl( CEdit& pEdit ) {
    HWND hwndEdit { pEdit.GetSafeHwnd( ) };
    if ( !m_mapHwndToWndProc.contains( hwndEdit ) ) {
        debug( L"CChildView::UnSubclassEditControl: pEdit 0x%p, hwnd 0x%p: Control isn't subclassed\n", &pEdit, hwndEdit );
        DebugBreak( );
        return;
    }

    WNDPROC wndProc { m_mapHwndToWndProc.at( hwndEdit ) };
    WNDPROC oldWndProc;
    if ( DWORD dwError { _SetWindowProcedure( hwndEdit, wndProc, oldWndProc ) }; !dwError ) {
        m_mapHwndToWndProc.erase( hwndEdit );
    } else {
        debug( L"CChildView::UnSubclassEditControl: pEdit 0x%p, hwnd 0x%p: SetWindowLongPtr failed: error %lu\n", &pEdit, hwndEdit, dwError );
        DebugBreak( );
    }
}

void CChildView::UpdateLabEditsIfValuesChanged( LabTriplet const& oldValues, LabTriplet const& newValues ) {
    _UpdateEditIfValueChanged( m_editLabL, oldValues[+LabChannels::L], newValues[+LabChannels::L] );
    _UpdateEditIfValueChanged( m_editLabA, oldValues[+LabChannels::a], newValues[+LabChannels::a] );
    _UpdateEditIfValueChanged( m_editLabB, oldValues[+LabChannels::b], newValues[+LabChannels::b] );
}

void CChildView::UpdateSrgbEditsIfValuesChanged( SrgbTriplet const& oldValues, SrgbTriplet const& newValues ) {
    _UpdateEditIfValueChanged( m_editSrgbR, oldValues[+SrgbChannels::R], newValues[+SrgbChannels::R] );
    _UpdateEditIfValueChanged( m_editSrgbG, oldValues[+SrgbChannels::G], newValues[+SrgbChannels::G] );
    _UpdateEditIfValueChanged( m_editSrgbB, oldValues[+SrgbChannels::B], newValues[+SrgbChannels::B] );
    if ( newValues != oldValues ) {
        _PutHexColorToEdit( m_editHexColor, newValues );
    }
}

void CChildView::DoDataExchange( CDataExchange* pDX ) {
    CFormView::DoDataExchange( pDX );

    DDX_Control( pDX, IDC_GROUPBOX_LAB,    m_groupBoxLab   );
    DDX_Control( pDX, IDC_LAB_L_LABEL,     m_radioLabL     );
    DDX_Control( pDX, IDC_LAB_A_LABEL,     m_radioLabA     );
    DDX_Control( pDX, IDC_LAB_B_LABEL,     m_radioLabB     );
    DDX_Control( pDX, IDC_LAB_L_VALUE,     m_editLabL      );
    DDX_Control( pDX, IDC_LAB_A_VALUE,     m_editLabA      );
    DDX_Control( pDX, IDC_LAB_B_VALUE,     m_editLabB      );

    DDX_Control( pDX, IDC_GROUPBOX_SRGB,   m_groupBoxSrgb  );
    DDX_Control( pDX, IDC_SRGB_R_LABEL,    m_radioSrgbR    );
    DDX_Control( pDX, IDC_SRGB_G_LABEL,    m_radioSrgbG    );
    DDX_Control( pDX, IDC_SRGB_B_LABEL,    m_radioSrgbB    );
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

void CChildView::InitializeMemberVariables( ) {
    m_mapHwndToChannel = {
        { m_editLabL .GetSafeHwnd( ), AllChannels::LabL  },
        { m_editLabA .GetSafeHwnd( ), AllChannels::LabA  },
        { m_editLabB .GetSafeHwnd( ), AllChannels::LabB  },
        { m_editSrgbR.GetSafeHwnd( ), AllChannels::SrgbR },
        { m_editSrgbG.GetSafeHwnd( ), AllChannels::SrgbG },
        { m_editSrgbB.GetSafeHwnd( ), AllChannels::SrgbB }
    };

    m_pDoc = static_downcast<CColorPickerDoc>( GetDocument( ) );
}

void CChildView::AdjustUIControls( ) {
    SIZE constexpr adjustUp2   { 0, -2 }; SIZE constexpr adjustLeft2  { -2, 0 };
    SIZE constexpr adjustUp1   { 0, -1 }; SIZE constexpr adjustLeft1  { -1, 0 };
    SIZE constexpr adjustDown1 { 0,  1 }; SIZE constexpr adjustRight1 {  1, 0 };
    SIZE constexpr adjustDown2 { 0,  2 }; SIZE constexpr adjustRight2 {  2, 0 };

    _AdjustSize    ( &m_groupBoxLab,   adjustUp1    );
    _AdjustPosition( &m_radioLabL,     adjustDown1  );
    _AdjustPosition( &m_radioLabA,     adjustUp1    );
    _AdjustPosition( &m_radioLabB,     adjustUp2    );
    //djustPosition( &m_editLabL,                   );
    //djustPosition( &m_editLabA,                   );
    _AdjustPosition( &m_editLabB,      adjustUp2    );

    _AdjustPosition( &m_groupBoxSrgb,  adjustDown2  );
    _AdjustSize    ( &m_groupBoxSrgb,  adjustUp1    );
    _AdjustPosition( &m_radioSrgbR,    adjustDown2  );
    //djustPosition( &m_radioSrgbG,                 );
    _AdjustPosition( &m_radioSrgbB,    adjustUp2    );
    _AdjustPosition( &m_editSrgbR,     adjustDown2  + adjustRight1 );
    _AdjustPosition( &m_editSrgbG,     adjustDown2  + adjustRight1 );
    _AdjustPosition( &m_editSrgbB,     adjustUp1    + adjustRight1 );

    _AdjustPosition( &m_editHexColor,  adjustUp1    + SIZE { -7, 0 } );
    _AdjustSize    ( &m_editHexColor,  {   8,   0 } );

    _AdjustSize    ( &m_staticSwatch,  {  -1,  -1 } );

    _SetSize       ( &m_staticZStrip,  {  20, 256 } );

    _AdjustPosition( &m_staticXyGrid,  adjustLeft1  );
    _SetSize       ( &m_staticXyGrid,  { 256, 256 } );

    _AdjustPosition( &m_buttonClose,   {  -1,  -2 } );
}

void CChildView::OnInitialUpdate( ) {
    CFormView::OnInitialUpdate( );

    InitializeMemberVariables( );
    AdjustUIControls( );

    //
    // Load settings from registry
    //

    m_pDoc->LoadFromRegistry( );

    //
    // Apply them to the controls
    //

    AllChannels        const  selectedChannel { m_pDoc->GetSelectedChannel( ) };
    AllChannelsTriplet const& channels        { _ChannelXyzTriplets[+selectedChannel] };

    m_channelX = channels[0];
    m_channelY = channels[1];
    m_channelZ = channels[2];

    for ( AllChannels channel { }; channel <= AllChannels::Max; ++channel ) {
        m_mapChannelToButtonControl[+channel]->SetCheck( ( channel == selectedChannel ) ? BST_CHECKED : BST_UNCHECKED );
    }

    bool const fInverted { m_pDoc->IsInverted( ) };

    m_staticZStrip.SetDocument( m_pDoc );
    m_staticZStrip.SetChannel( m_channelZ );
    m_staticZStrip.SetInverted( fInverted );

    m_staticXyGrid.SetDocument( m_pDoc );
    m_staticXyGrid.SetChannels( m_channelX, m_channelY, m_channelZ );
    m_staticXyGrid.SetInverted( fInverted );

    LabTriplet  const  labValues { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet const srgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    ++m_nBlockBitmapUpdates;

    _PutValueToEdit   ( m_editLabL,      labValues[ +LabChannels::L] );
    _PutValueToEdit   ( m_editLabA,      labValues[ +LabChannels::a] );
    _PutValueToEdit   ( m_editLabB,      labValues[ +LabChannels::b] );
    _PutValueToEdit   ( m_editSrgbR,    srgbValues[+SrgbChannels::R] );
    _PutValueToEdit   ( m_editSrgbG,    srgbValues[+SrgbChannels::G] );
    _PutValueToEdit   ( m_editSrgbB,    srgbValues[+SrgbChannels::B] );
    _PutHexColorToEdit( m_editHexColor, srgbValues );

    --m_nBlockBitmapUpdates;
    UpdateBitmaps( );

    //
    // Subclass edit controls
    //

    SubclassEditControl( m_editLabL,  _EditWndProc );
    SubclassEditControl( m_editLabA,  _EditWndProc );
    SubclassEditControl( m_editLabB,  _EditWndProc );
    SubclassEditControl( m_editSrgbR, _EditWndProc );
    SubclassEditControl( m_editSrgbG, _EditWndProc );
    SubclassEditControl( m_editSrgbB, _EditWndProc );
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
    bool const fInverted { !m_pDoc->IsInverted( ) };
    m_pDoc       ->SetInverted( fInverted );
    m_staticZStrip.SetInverted( fInverted );
    m_staticXyGrid.SetInverted( fInverted );
    UpdateBitmaps( );
}

void CChildView::OnValueEditGotFocus( UINT uId ) {
#if defined _DEBUG
    if ( m_pCurrentEdit ) {
        debug( L"CChildView::OnValueEditGotFocus: Uh oh: m_pCurrentEdit not nullptr on entry\n" );
        DebugBreak( );
    }
#endif // defined _DEBUG

    m_pCurrentEdit = MapValueControlIdToEditControl( uId );
}

void CChildView::OnValueEditLostFocus( UINT uId ) {
    if ( !m_pCurrentEdit ) {
        // this signifies it's not of the L*a*b* or RGB edit controls
        return;
    }

#if defined _DEBUG
    if ( CEdit* pEdit { MapValueControlIdToEditControl( uId ) }; !pEdit ) {
        debug( L"CChildView::OnValueEditGotFocus: Couldn't map control ID to edit control\n" );
        DebugBreak( );
    } else if ( m_pCurrentEdit != pEdit ) {
        debug( L"CChildView::OnValueEditGotFocus: Uh oh: Control ID doesn't map to same object as m_pCurrentEdit: pEdit 0x%p vs. m_pCurrentEdit 0x%p\n", pEdit, m_pCurrentEdit );
        DebugBreak( );
    }
#endif // defined _DEBUG

    if ( int n; _GetValueFromEdit( *m_pCurrentEdit, n ) ) {
        // TODO range check
    } else {
        // TODO complain
    }

    m_pCurrentEdit = nullptr;
}

void CChildView::OnCloseButtonClicked( ) {
    m_pDoc->SaveToRegistry( );

    UnSubclassEditControl( m_editSrgbB );
    UnSubclassEditControl( m_editSrgbG );
    UnSubclassEditControl( m_editSrgbR );
    UnSubclassEditControl( m_editLabB  );
    UnSubclassEditControl( m_editLabA  );
    UnSubclassEditControl( m_editLabL  );

    ::PostQuitMessage( 0 );
}

bool CChildView::EditControl_OnKeyDown( AllChannels const channel, UINT const nChar, UINT const nRepCnt, UINT const nFlags ) {
    debug( L"CChildView::EditControl_OnKeyDown: channel: %s, nChar: %u, nRepCnt: %u, nFlags: 0x%08X\n", ToString( channel ), nChar, nRepCnt, nFlags );

    int adjust { };
    if ( nChar == VK_UP ) {
        adjust = 1;
    } else if ( nChar == VK_DOWN ) {
        adjust = -1;
    } else {
        return false;
    }

    LabTriplet  oldLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet oldSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    m_pDoc->SetChannelValue( channel, _ClipToChannelRange( channel, m_pDoc->GetChannelValue( channel ) + adjust ) );

    LabTriplet  newLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet newSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    ++m_nBlockBitmapUpdates;

    UpdateLabEditsIfValuesChanged (  oldLabValues,  newLabValues );
    UpdateSrgbEditsIfValuesChanged( oldSrgbValues, newSrgbValues );

    --m_nBlockBitmapUpdates;
    UpdateBitmaps( );

    return true;
}

void CChildView::OnChannelRadioClicked( UINT const uId ) {
    AllChannels channel { MapLabelControlIdToChannel( uId ) };
    if ( channel == AllChannels::unknown ) {
        return;
    }

    if ( AllChannelsTriplet const& channels { _ChannelXyzTriplets[+channel] }; channels != AllChannelsTriplet { { m_channelX, m_channelY, m_channelZ } } ) {
        m_channelX = channels[0];
        m_channelY = channels[1];
        m_channelZ = channels[2];

        m_pDoc->SetSelectedChannel( m_channelZ );

        m_staticZStrip.SetChannel( m_channelZ );
        m_staticXyGrid.SetChannels( m_channelX, m_channelY, m_channelZ );

        UpdateBitmaps( );
    }
}

void CChildView::OnValueEditUpdate( UINT const uId ) {
    LabTriplet  oldLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet oldSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };
    LabTriplet  newLabValues  {  oldLabValues };
    SrgbTriplet newSrgbValues { oldSrgbValues };
    AllChannels channel       { MapValueControlIdToChannel( uId ) };
    CEdit*      pEdit         { MapValueControlIdToEditControl( uId ) };
    bool        fChanged      { };

    ++m_nBlockBitmapUpdates;

    if ( IsLabChannel( channel ) ) {
        LabValueT oldValue { static_cast<LabValueT>( m_pDoc->GetChannelValue( channel ) ) };
        LabValueT newValue { oldValue };

        fChanged = _UpdateValueIfEditChanged( *pEdit, oldValue, newValue );
        if ( fChanged ) {
            newLabValues[+AllChannelsToLabChannels( channel )] = newValue;
            m_pDoc->SetChannelValue( channel, newValue );

            newSrgbValues = m_pDoc->GetSrgbColor( );
            UpdateSrgbEditsIfValuesChanged( oldSrgbValues, newSrgbValues );
        }
    } else if ( IsSrgbChannel( channel ) ) {
        SrgbValueT oldValue { static_cast<SrgbValueT>( m_pDoc->GetChannelValue( channel ) ) };
        SrgbValueT newValue { oldValue };

        fChanged = _UpdateValueIfEditChanged( *pEdit, oldValue, newValue );
        if ( fChanged ) {
            newSrgbValues[+AllChannelsToSrgbChannels( channel )] = newValue;
            m_pDoc->SetChannelValue( channel, newValue );
            if ( newSrgbValues != oldSrgbValues ) {
                _PutHexColorToEdit( m_editHexColor, newSrgbValues );
            }

            newLabValues = m_pDoc->GetLabColor( );
            UpdateLabEditsIfValuesChanged( oldLabValues, newLabValues );
        }
    }

    --m_nBlockBitmapUpdates;
    if ( fChanged ) {
        UpdateBitmaps( );
    }
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
        m_pDoc->SetColor( SrgbColor { newSrgbValues } );
        newLabValues = m_pDoc->GetLabColor( ).GetChannelValues( );

        ++m_nBlockBitmapUpdates;

        UpdateLabEditsIfValuesChanged (  oldLabValues,  newLabValues );
        UpdateSrgbEditsIfValuesChanged( oldSrgbValues, newSrgbValues );

        --m_nBlockBitmapUpdates;
        UpdateBitmaps( );
    }
}

void CChildView::OnZStripMouseMove( NMHDR* pNotifyStruct, LRESULT* result ) {
    ZSB_MOUSEMOVE* mm { static_cast<ZSB_MOUSEMOVE*>( pNotifyStruct ) };

    LabTriplet  oldLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet oldSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    m_pDoc->SetChannelValue( m_channelZ, ScaleLabColor( m_channelZ, mm->point.y ) );

    LabTriplet  newLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet newSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    ++m_nBlockBitmapUpdates;

    UpdateLabEditsIfValuesChanged (  oldLabValues,  newLabValues );
    UpdateSrgbEditsIfValuesChanged( oldSrgbValues, newSrgbValues );

    --m_nBlockBitmapUpdates;
    UpdateBitmaps( false, true );

    *result = 0;
}

void CChildView::OnXyGridMouseMove( NMHDR* pNotifyStruct, LRESULT* result ) {
    ZSB_MOUSEMOVE* mm { static_cast<ZSB_MOUSEMOVE*>( pNotifyStruct ) };

    LabTriplet  oldLabValues  { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet oldSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    m_pDoc->SetChannelValues( {
        { m_channelX, ScaleLabColor( m_channelX, mm->point.x ) },
        { m_channelY, ScaleLabColor( m_channelY, mm->point.y ) }
    } );

    LabTriplet   newLabValues { m_pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet newSrgbValues { m_pDoc->GetSrgbColor( ).GetChannelValues( ) };

    ++m_nBlockBitmapUpdates;

    UpdateLabEditsIfValuesChanged( oldLabValues, newLabValues );
    UpdateSrgbEditsIfValuesChanged( oldSrgbValues, newSrgbValues );

    --m_nBlockBitmapUpdates;
    UpdateBitmaps( true, false );

    *result = 0;
}
