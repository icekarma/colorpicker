#include "pch.h"

#include "MainFrame.h"

#include "ColorPicker.h"

#if defined _DEBUG
#undef POSITION_RIGHT_HALF_CENTERED
#endif // defined _DEBUG

IMPLEMENT_DYNCREATE( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
#if defined _DEBUG && defined DEBUG_WINDOW_SIZE
    ON_WM_SIZING( )
    ON_WM_SIZE( )
#endif // defined _DEBUG && defined DEBUG_WINDOW_SIZE
    ON_WM_CREATE( )
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

#if defined _DEBUG && defined POSITION_RIGHT_HALF_CENTERED
    int constexpr W = 1920 / 2;
    int constexpr H = 1080;
    cs.x = ( W - cs.cx ) / 2 + W;
    cs.y = ( H - cs.cy ) / 2;
#else // !( defined _DEBUG && defined POSITION_RIGHT_HALF_CENTERED )
    int x { static_cast<int>( theApp.GetProfileInt( L"Settings", L"X", -1'000'000 ) ) };
    int y { static_cast<int>( theApp.GetProfileInt( L"Settings", L"Y", -1'000'000 ) ) };
    if ( ( x != -1'000'000 ) && ( y != -1'000'000 ) ) {
        cs.x = x;
        cs.y = y;
    }
#endif // defined _DEBUG && defined POSITION_RIGHT_HALF_CENTERED

    return TRUE;
}

#if defined _DEBUG && defined DEBUG_WINDOW_SIZE
void CMainFrame::OnSizing( UINT fwSide, LPRECT pRect ) {
    CFrameWnd::OnSizing( fwSide, pRect );

    CRect rect( pRect );
    debug( L"CMainFrame::OnSizing: %dx%d (%d,%d)-(%d,%d)\n", rect.Width( ), rect.Height( ), rect.left, rect.top, rect.right, rect.bottom );
}

void CMainFrame::OnSize( UINT nType, int cx, int cy ) {
    CFrameWnd::OnSize( nType, cx, cy );

    debug( L"CMainFrame::OnSize: %dx%d\n", cx, cy );
}
#endif // defined _DEBUG && defined DEBUG_WINDOW_SIZE

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct ) {
    if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 ) {
        return -1;
    }

    HICON hIcon = theApp.LoadIcon( IDR_MAINFRAME );
    if ( hIcon ) {
        SetIcon( hIcon, TRUE  );
        SetIcon( hIcon, FALSE );
    }

    return 0;
}
