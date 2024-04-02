#include "pch.h"

#include "MainFrame.h"

#include "ColorPicker.h"

IMPLEMENT_DYNCREATE( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
#if defined DEBUG_WINDOW_SIZE
    ON_WM_SIZING( )
    ON_WM_SIZE( )
#endif // defined DEBUG_WINDOW_SIZE
END_MESSAGE_MAP( )

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs ) {
    if ( !CFrameWnd::PreCreateWindow( cs ) ) {
        return FALSE;
    }

    cs.style     &= ~WindowStylesToRemove;
    cs.dwExStyle &= ~ExtendedWindowStylesToRemove;
    cs.lpszClass  = AfxRegisterWndClass( 0 );
    cs.cx         = 432;
    cs.cy         = 371;

    return TRUE;
}

#if defined DEBUG_WINDOW_SIZE
void CMainFrame::OnSizing( UINT fwSide, LPRECT pRect ) {
    CFrameWnd::OnSizing( fwSide, pRect );

    CRect rect( pRect );
    debug( "CMainFrame::OnSizing: %dx%d (%d,%d)-(%d,%d)\n", rect.Width( ), rect.Height( ), rect.left, rect.top, rect.right, rect.bottom );
}

void CMainFrame::OnSize( UINT nType, int cx, int cy ) {
    CFrameWnd::OnSize( nType, cx, cy );

    debug( "CMainFrame::OnSize: %dx%d\n", cx, cy );
}
#endif // defined DEBUG_WINDOW_SIZE
