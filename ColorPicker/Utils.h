#pragma once

//
// Function prototypes
//

void                  AdjustPosition( CWnd* pWnd, SIZE const& adjust );
void                  AdjustPositionAndSize( CWnd* pWnd, SIZE const& adjustPosition, SIZE const& adjustSize );
void                  AdjustSize( CWnd* pWnd, SIZE const& adjust );
[[nodiscard]] int     BoolToChecked( bool const fValue );
[[nodiscard]] int     ClipToChannelRange( AllChannels const channel, int const value );
void                  ComplainAboutBadValue( HWND hwnd, CEdit* pEdit, int const nNewValue, CString const& strMessage );
[[nodiscard]] CRect   GetClientRect( CWnd* const pWnd );
[[nodiscard]] CRect   GetClientRect( HWND const hwnd );
[[nodiscard]] CString GetExecutablePath( );
[[nodiscard]] CRect   GetScreenRect( CWnd* const pWnd );
[[nodiscard]] CRect   GetScreenRect( HWND const hwnd );
[[nodiscard]] CRect   GetScreenRect( CWnd* const pParent,    CWnd* const pWnd );
[[nodiscard]] CRect   GetScreenRect( CWnd* const pParent,    HWND  const hwnd );
[[nodiscard]] CRect   GetScreenRect( HWND  const hwndParent, CWnd* const pWnd );
[[nodiscard]] CRect   GetScreenRect( HWND  const hwndParent, HWND  const hwnd );
[[nodiscard]] bool    GetValueAndChangedFromEdit( CEdit const& edit, int& nValue, bool& fChanged );
[[nodiscard]] bool    GetValueFromEdit( CEdit const& edit, int& nValue );
[[nodiscard]] CRect   GetWindowRect( CWnd* const pWnd );
[[nodiscard]] CRect   GetWindowRect( HWND const hwnd );
[[nodiscard]] CString GetWindowsMessageName( UINT const uMessage );
[[nodiscard]] bool    IsTextSelected( CEdit const* pEdit );
void                  PutTextOnClipboard( CString const& str );
void                  PutValueToEdit( CEdit& edit, int const nValue );
[[nodiscard]] CString GetWindowText( CEdit const* pEdit );
void                  SetPosition( CWnd* pWnd, POINT const& position );
void                  SetPosition( CWnd* pWnd, SIZE const& adjust );
void                  SetPositionAndSize( CWnd* pWnd, RECT const& rect );
void                  SetSize( CWnd* pWnd, SIZE const& size );
[[nodiscard]] DWORD   SetWindowProcedure( HWND const hWnd, WNDPROC const newWndProc, WNDPROC& oldWndProc );

template<typename T>
void DeleteIfSet( T*& ptr ) {
    if ( ptr ) {
        delete ptr;
        ptr = nullptr;
    }
}

template<typename T>
[[nodiscard]] bool UpdateValueIfEditChanged( CEdit const& edit, T const oldValue, T& newValue ) {
    bool fChanged { };

    if ( int value { static_cast<int>( oldValue ) }; GetValueAndChangedFromEdit( edit, value, fChanged ) && fChanged ) {
        newValue = static_cast<T>( value );
        return true;
    } else {
        return false;
    }
}

template<typename T>
void UpdateEditIfValueChanged( CEdit& edit, T const oldValue, T const newValue ) {
    if ( newValue != oldValue ) {
        PutValueToEdit( edit, static_cast<int>( newValue ) );
    }
}
