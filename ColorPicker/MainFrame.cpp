#include "pch.h"

#include "MainFrame.h"

#include "ColorPicker.h"
#include "ChildView.h"
#include "Settings.h"
#include "Utils.h"

IMPLEMENT_DYNCREATE( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
    ON_WM_CLOSE( )
    ON_WM_CREATE( )
#if defined _DEBUG && defined DEBUG_WINDOW_SIZE
    ON_WM_SIZE( )
    ON_WM_SIZING( )
#endif // defined _DEBUG && defined DEBUG_WINDOW_SIZE
END_MESSAGE_MAP( )

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs ) {
    if ( !CFrameWnd::PreCreateWindow( cs ) ) {
        return FALSE;
    }

    cs.style     &= ~WindowStylesToRemove;
    cs.dwExStyle &= ~ExtendedWindowStylesToRemove;
    cs.lpszClass  = AfxRegisterWndClass( 0 );
    cs.cx         = 432;
    cs.cy         = 396;

    CPoint topLeft { g_pSettings->GetWindowPosition( ) };
    if ( ( topLeft.x != -1'000'000 ) && ( topLeft.y != -1'000'000 ) ) {
        cs.x = topLeft.x;
        cs.y = topLeft.y;
    }

    return TRUE;
}

void CMainFrame::OnClose( ) {
    extern CChildView* g_pChildView;
    g_pChildView->Close( );

    CFrameWnd::OnClose( );
}

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct ) {
    if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 ) {
        return -1;
    }

    HICON hIcon = theApp.LoadIcon( IDR_MAINFRAME );
    if ( hIcon ) {
        SetIcon( hIcon, TRUE  );
        SetIcon( hIcon, FALSE );
    }

    debug( L"CMainFrame::OnCreate: GetExecutablePath(): \"%s\"\n", (LPCWSTR) GetExecutablePath( ) );

    return 0;
}

#if defined _DEBUG && defined DEBUG_WINDOW_SIZE
void CMainFrame::OnSize( UINT nType, int cx, int cy ) {
    CFrameWnd::OnSize( nType, cx, cy );

    debug( L"CMainFrame::OnSize: %dx%d\n", cx, cy );
}

void CMainFrame::OnSizing( UINT fwSide, LPRECT pRect ) {
    CFrameWnd::OnSizing( fwSide, pRect );

    CRect rect( pRect );
    debug( L"CMainFrame::OnSizing: %dx%d (%d,%d)-(%d,%d)\n", rect.Width( ), rect.Height( ), rect.left, rect.top, rect.right, rect.bottom );
}
#endif // defined _DEBUG && defined DEBUG_WINDOW_SIZE
