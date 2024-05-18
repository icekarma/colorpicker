#pragma once

//
// Function prototypes
//

void                  AdjustPosition( _In_ CWnd* pWnd, _In_ SIZE const& adjust );
void                  AdjustPositionAndSize( _In_ CWnd* pWnd, _In_ SIZE const& adjustPosition, _In_ SIZE const& adjustSize );
void                  AdjustSize( _In_ CWnd* pWnd, _In_ SIZE const& adjust );
[[nodiscard]] int     BoolToChecked( _In_ bool const fValue );
[[nodiscard]] int     ClipToChannelRange( _In_ AllChannels const channel, _In_ int const value );
void                  ComplainAboutBadValue( _In_ HWND hwnd, _In_ CEdit* pEdit, _In_ int const nNewValue, _In_ CString const& strMessage );
[[nodiscard]] CRect   GetClientRect( _In_ CWnd* const pWnd );
[[nodiscard]] CRect   GetClientRect( _In_ HWND const hwnd );
[[nodiscard]] CString GetExecutablePath( );
[[nodiscard]] bool    GetValueAndChangedFromEdit( _In_ CEdit const& edit, _Out_ int& nValue, _Out_ bool& fChanged );
[[nodiscard]] bool    GetValueFromEdit( _In_ CEdit const& edit, _Out_ int& nValue );
[[nodiscard]] CRect   GetWindowRect( _In_ CWnd* const pWnd );
[[nodiscard]] CRect   GetWindowRect( _In_ HWND const hwnd );
[[nodiscard]] CString GetWindowsMessageName( _In_ UINT const uMessage );
[[nodiscard]] bool    IsTextSelected( _In_ CEdit const* pEdit );
void                  PutTextOnClipboard( _In_ CString const& str );
void                  PutValueToEdit( _In_ CEdit& edit, _In_ int const nValue );
[[nodiscard]] CString SafeGetWindowText( _In_ CEdit const& edit );
void                  SetPosition( _In_ CWnd* pWnd, _In_ POINT const& position );
void                  SetPosition( _In_ CWnd* pWnd, _In_ SIZE const& adjust );
void                  SetSize( _In_ CWnd* pWnd, _In_ SIZE const& size );
[[nodiscard]] DWORD   SetWindowProcedure( _In_ HWND const hWnd, _In_ WNDPROC const newWndProc, _Out_ WNDPROC& oldWndProc );

template<typename T>
[[nodiscard]] bool UpdateValueIfEditChanged( _In_ CEdit const& edit, _In_ T const oldValue, _Out_ T& newValue ) {
    bool fChanged { };

    if ( int value { static_cast<int>( oldValue ) }; GetValueAndChangedFromEdit( edit, value, fChanged ) && fChanged ) {
        newValue = static_cast<T>( value );
        return true;
    } else {
        return false;
    }
}

template<typename T>
void UpdateEditIfValueChanged( _In_ CEdit& edit, _In_ T const oldValue, _In_ T const newValue ) {
    if ( newValue != oldValue ) {
        PutValueToEdit( edit, static_cast<int>( newValue ) );
    }
}
