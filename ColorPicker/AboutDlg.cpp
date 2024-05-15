#include "pch.h"

#include "AboutDlg.h"

#include "Utils.h"

BEGIN_MESSAGE_MAP( CAboutDlg, CDialogEx )
    ON_WM_CLOSE( )
END_MESSAGE_MAP( )

void CAboutDlg::OnClose( ) {
    if ( m_pOkButton ) {
        m_pOkButton->Detach( );
        delete m_pOkButton;
        m_pOkButton = nullptr;
    }

    CDialogEx::OnClose( );
}

BOOL CAboutDlg::OnInitDialog( ) {
    CDialogEx::OnInitDialog( );

    //
    // Load our image
    //

    if ( !m_pImage ) {
        m_pImage = new CImage( );

        CString strFileName { GetExecutablePath( ) };
        if ( strFileName.IsEmpty( ) ) {
            debug( L"CAboutDlg::OnInitDialog: Executable path is empty\n" );
            return TRUE;
        }

        int index { strFileName.ReverseFind( L'\\' ) };
        if ( index < 0 ) {
            debug( L"CAboutDlg::OnInitDialog: Bad executable path\n" );
            return TRUE;
        }
        strFileName = strFileName.Left( index ) + L"\\About.png";

        HRESULT hr { m_pImage->Load( strFileName ) };
        if ( FAILED( hr ) ) {
            debug( L"CAboutDlg::OnInitDialog: Loading image failed\n" );
            return TRUE;
        }
    }

    //
    // Resize the window to fit
    //

    SIZE const desiredSize { m_pImage->GetWidth( ), m_pImage->GetHeight( ) };

    CRect windowRect;
    GetWindowRect( windowRect );

    CRect adjustedRect { windowRect.TopLeft( ), desiredSize };
    if ( ::AdjustWindowRectEx( adjustedRect, (DWORD) ::GetWindowLongPtr( GetSafeHwnd( ), GWL_STYLE ), FALSE, (DWORD) ::GetWindowLongPtr( GetSafeHwnd( ), GWL_EXSTYLE ) ) ) {
        SetWindowPos( nullptr, 0, 0, adjustedRect.Width( ), adjustedRect.Height( ), SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER );
    }

    //
    // Display the image
    //

    m_pBitmap = new CStatic( );
    m_pBitmap->Create( nullptr, SS_BITMAP, CRect { { }, desiredSize }, this );
    m_pBitmap->SetBitmap( *m_pImage );
    m_pBitmap->ShowWindow( SW_SHOW );

    //
    // Attach our own object to the OK button
    //

    SetLastError( ERROR_SUCCESS );
    HWND hwndButton { ::GetDlgItem( GetSafeHwnd( ), IDOK ) };
    DWORD dwError { GetLastError( ) };
    if ( dwError ) {
        debug( L"CAboutDlg::OnInitDialog: GetDlgItem failed: %lu\n", dwError );
    } else {
        m_pOkButton = new CButton( );
        m_pOkButton->Attach( hwndButton );

        CRect rectClient { ::GetClientRect( this ) };
        CRect rectButton { ::GetWindowRect( m_pOkButton ) };
        SetPosition( m_pOkButton, SIZE { 16, rectClient.Height( ) - 16 - rectButton.Height( ) } );
    }

    // return TRUE unless you set the focus to a control
    return TRUE;
}
