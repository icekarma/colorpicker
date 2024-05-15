#pragma once

//
// Function prototypes
//

CString GetExecutablePath( );
CString GetWindowsMessageName( UINT const uMessage );

void    _AdjustPosition( CWnd* ctrl, SIZE const& adjust );
void    _AdjustSize( CWnd* ctrl, SIZE const& adjust );
int     _BoolToChecked( bool const fValue );
int     _ClipToChannelRange( AllChannels const channel, int const value );
void    _ComplainAboutBadValue( HWND hwnd, CEdit* pEdit, int const nNewValue, CString const& strMessage );
bool    _GetValueAndChangedFromEdit( CEdit const& edit, int& nValue, bool& fChanged );
bool    _GetValueFromEdit( CEdit const& edit, int& nValue );
bool    _IsTextSelected( CEdit const* pEdit );
bool    _PutTextOnClipboard( const CString& str );
void    _PutValueToEdit( CEdit& edit, int const nValue );
CString _SafeGetWindowText( CEdit const& edit );
void    _SetPosition( CWnd* ctrl, POINT const& position );
void    _SetPosition( CWnd* ctrl, SIZE const& adjust );
void    _SetSize( CWnd* ctrl, SIZE const& size );
DWORD   _SetWindowProcedure( HWND const hWnd, WNDPROC const newWndProc, WNDPROC& oldWndProc );

template<typename T>
[[nodiscard]] bool _UpdateValueIfEditChanged( CEdit const& edit, T const oldValue, T& newValue ) {
    bool fChanged { };

    if ( int value { static_cast<int>( oldValue ) }; _GetValueAndChangedFromEdit( edit, value, fChanged ) && fChanged ) {
        newValue = static_cast<T>( value );
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
