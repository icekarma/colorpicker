#pragma once

//
// Function prototypes
//

CString GetExecutablePath( );
CString GetWindowsMessageName( UINT const uMessage );

void    AdjustPosition( CWnd* ctrl, SIZE const& adjust );
void    AdjustPositionAndSize( CWnd* ctrl, SIZE const& adjustPosition, SIZE const& adjustSize );
void    AdjustSize( CWnd* ctrl, SIZE const& adjust );
int     BoolToChecked( bool const fValue );
int     ClipToChannelRange( AllChannels const channel, int const value );
void    ComplainAboutBadValue( HWND hwnd, CEdit* pEdit, int const nNewValue, CString const& strMessage );
bool    GetValueAndChangedFromEdit( CEdit const& edit, int& nValue, bool& fChanged );
bool    GetValueFromEdit( CEdit const& edit, int& nValue );
bool    IsTextSelected( CEdit const* pEdit );
bool    PutTextOnClipboard( const CString& str );
void    PutValueToEdit( CEdit& edit, int const nValue );
CString SafeGetWindowText( CEdit const& edit );
void    SetPosition( CWnd* ctrl, POINT const& position );
void    SetPosition( CWnd* ctrl, SIZE const& adjust );
void    SetSize( CWnd* ctrl, SIZE const& size );
DWORD   SetWindowProcedure( HWND const hWnd, WNDPROC const newWndProc, WNDPROC& oldWndProc );

template<typename T>
[[nodiscard]] bool _UpdateValueIfEditChanged( CEdit const& edit, T const oldValue, T& newValue ) {
    bool fChanged { };

    if ( int value { static_cast<int>( oldValue ) }; GetValueAndChangedFromEdit( edit, value, fChanged ) && fChanged ) {
        newValue = static_cast<T>( value );
        return true;
    } else {
        return false;
    }
}

template<typename T>
void _UpdateEditIfValueChanged( CEdit& edit, T const oldValue, T const newValue ) {
    if ( newValue != oldValue ) {
        PutValueToEdit( edit, static_cast<int>( newValue ) );
    }
}

[[nodiscard]] CRect inline GetClientRect( HWND const hwnd ) {
    CRect rect;
    return ::GetClientRect( hwnd, rect ) ? rect : CRect { { -65536, -65536 }, SIZE { -1, -1 } };
}

[[nodiscard]] CRect inline GetClientRect( CWnd* const pWnd ) {
    return GetClientRect( pWnd->GetSafeHwnd( ) );
}

[[nodiscard]] CRect inline GetWindowRect( HWND const hwnd ) {
    CRect rect;
    return ::GetWindowRect( hwnd, rect ) ? rect : CRect { { -65536, -65536 }, SIZE { -1, -1 } };
}

[[nodiscard]] CRect inline GetWindowRect( CWnd* const pWnd ) {
    return GetWindowRect( pWnd->GetSafeHwnd( ) );
}
