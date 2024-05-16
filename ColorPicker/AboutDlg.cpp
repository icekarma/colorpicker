#include "pch.h"

#include "AboutDlg.h"

#include "Utils.h"

BEGIN_MESSAGE_MAP( CAboutDlg, CDialogEx )
END_MESSAGE_MAP( )

namespace {

    constinit SIZE const ButtonDefaultSize { 75, 23 };

}

CAboutDlg::~CAboutDlg( ) {
    if ( m_pBitmap ) {
        delete m_pBitmap;
        m_pBitmap = nullptr;
    }

    if ( m_pImage ) {
        delete m_pImage;
        m_pImage = nullptr;
    }

    if ( m_pButtonOK ) {
        delete m_pButtonOK;
        m_pButtonOK = nullptr;
    }
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
    // Create the OK button
    //

    CRect rectClient { ::GetClientRect( this ) };
    CRect rectButton { ::GetWindowRect( m_pButtonOK ) };

    m_pButtonOK = new CButton( );
    SetLastError( ERROR_SUCCESS );
    if ( !m_pButtonOK->Create( L"OK", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_GROUP | WS_TABSTOP, CRect { { 16, rectClient.Height( ) - ButtonDefaultSize.cy - 16 }, ButtonDefaultSize }, this, IDOK ) ) {
        DWORD dwError { GetLastError( ) };
        debug( L"CAboutDlg::OnInitDialog: m_pButtonOK->Create failed: %lu\n", dwError );
    }
    m_pButtonOK->SetFont( GetFont( ) );

    // return TRUE unless you set the focus to a control
    return TRUE;
}
