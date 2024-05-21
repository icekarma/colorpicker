#include "pch.h"

#include "AboutDlg.h"

#include "Utils.h"

BEGIN_MESSAGE_MAP( CAboutDlg, CDialogEx )
END_MESSAGE_MAP( )

namespace {

    SIZE constexpr ButtonDefaultSize { 75, 23 };

    std::unique_ptr<CImage> LoadAboutPng( ) {
        CString strFileName { GetExecutablePath( ) };
        if ( strFileName.IsEmpty( ) ) {
            debug( L"LoadAboutPng: Executable path is empty\n" );
            return nullptr;
        }

        int index { strFileName.ReverseFind( L'\\' ) };
        if ( index < 0 ) {
            debug( L"LoadAboutPng: Executable path contains no backslashes\n" );
            return nullptr;
        }
        strFileName = strFileName.Left( index ) + L"\\About.png";

        CImage* pImage { new CImage };
        if ( HRESULT hr { pImage->Load( strFileName ) }; FAILED( hr ) ) {
            debug( L"LoadAboutPng: Loading image failed: hr: 0x%08lX\n", hr );
            delete pImage;
            return nullptr;
        }

        return std::unique_ptr<CImage> { pImage };
    }

}

CAboutDlg::~CAboutDlg( ) {
    DeleteIfSet( m_pBitmap     );
    DeleteIfSet( m_pButtonOK   );
    DeleteIfSet( m_pStaticText );
}

BOOL CAboutDlg::OnInitDialog( ) {
    CDialogEx::OnInitDialog( );

    //
    // Load our image
    //

    if ( !g_pImage ) {
        g_pImage = LoadAboutPng( );
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
