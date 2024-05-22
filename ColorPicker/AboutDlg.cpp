#include "pch.h"

#include "AboutDlg.h"

#include "Utils.h"

BEGIN_MESSAGE_MAP( CAboutDlg, CDialogEx )
END_MESSAGE_MAP( )

//
// Global variables
//

std::unique_ptr<CImage> g_pImage;

//
// Private items

namespace {

    //
    // Private constants
    //

    SIZE constexpr ButtonDefaultSize  { 75, 23 };
    LONG constexpr ButtonPaddedHeight { 16 + ButtonDefaultSize.cy + 16 };

    //
    // Private functions
    //

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

//
// Implementation of CAboutDlg
//

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

    SIZE const desiredSize { g_pImage ? SIZE { g_pImage->GetWidth( ), g_pImage->GetHeight( ) } : SIZE { 466L, ButtonPaddedHeight } };
    CRect adjustedRect     { ::GetWindowRect( *this ).TopLeft( ), desiredSize };
    DWORD dwWindowStyle    { static_cast<DWORD>( ::GetWindowLongPtr( GetSafeHwnd( ), GWL_STYLE   ) ) };
    DWORD dwWindowStyleEx  { static_cast<DWORD>( ::GetWindowLongPtr( GetSafeHwnd( ), GWL_EXSTYLE ) ) };
    if ( ::AdjustWindowRectEx( adjustedRect, dwWindowStyle, FALSE, dwWindowStyleEx ) ) {
        SetWindowPos( nullptr, 0, 0, adjustedRect.Width( ), adjustedRect.Height( ), SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER );
    }

    //
    // Set up some local variables
    //

    CRect  rectClient { ::GetClientRect( this ) };
    CFont* pFont      { GetFont( ) };

    //
    // Display the image
    //

    if ( g_pImage ) {
        m_pBitmap = new CStatic;
        if ( !m_pBitmap->Create( nullptr, WS_VISIBLE | SS_BITMAP, CRect { { }, desiredSize }, this ) ) {
            DWORD dwError { GetLastError( ) };
            debug( L"CAboutDlg::OnInitDialog: m_pBitmap->Create failed: %lu\n", dwError );
        }
        m_pBitmap->SetFont( pFont );
        m_pBitmap->SetBitmap( *g_pImage );
    }

    //
    // Create a static text control for the copyright message and icon attribution
    //

    CRect rectStaticText { POINT { 0, rectClient.Height( ) - ButtonPaddedHeight }, SIZE { rectClient.Width( ) - 16 - ButtonDefaultSize.cx, ButtonPaddedHeight } };
    rectStaticText.DeflateRect( 8, 16 );

    m_pStaticText = new CStatic;
    if ( !m_pStaticText->Create( _GetResourceString( IDS_ABOUT_COPYRIGHT ) + L" " + _GetResourceString( IDS_ABOUT_ICON_ATTRIBUTION ), WS_VISIBLE | SS_CENTERIMAGE | SS_NOPREFIX | SS_CENTER, rectStaticText, this, IDC_ABOUT_COPYRIGHT ) ) {
        DWORD dwError { GetLastError( ) };
        debug( L"CAboutDlg::OnInitDialog: m_pStaticText->Create failed: %lu\n", dwError );
    }
    m_pStaticText->SetFont( pFont );

    //
    // Create the OK button control
    //

    CRect rectButtonOK { POINT { rectClient.Width() - ButtonDefaultSize.cx - 16, rectClient.Height( ) - ButtonDefaultSize.cy - 16 }, ButtonDefaultSize };
    m_pButtonOK = new CButton;

    SetLastError( ERROR_SUCCESS );
    if ( !m_pButtonOK->Create( L"OK", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_GROUP | WS_TABSTOP, rectButtonOK, this, IDOK ) ) {
        DWORD dwError { GetLastError( ) };
        debug( L"CAboutDlg::OnInitDialog: m_pButtonOK->Create failed: %lu\n", dwError );
    }
    m_pButtonOK->SetFont( pFont );

    // return TRUE unless you set the focus to a control
    return TRUE;
}
