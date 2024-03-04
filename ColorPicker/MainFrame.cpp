#include "pch.h"

#include "ColorPicker.h"
#include "MainFrame.h"

IMPLEMENT_DYNAMIC( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
    ON_WM_CREATE( )
    ON_WM_SETFOCUS( )
END_MESSAGE_MAP( )

CMainFrame::CMainFrame( ) noexcept:
    m_pwndView { }
{
    /*empty*/
}

CMainFrame::~CMainFrame( ) {
    /*empty*/
}

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct ) {
    if ( CFrameWnd::OnCreate( lpCreateStruct ) == -1 ) {
        return -1;
    }

    m_pwndView = DYNAMIC_DOWNCAST( CNewChildView, RUNTIME_CLASS( CNewChildView )->CreateObject( ) );
    if ( !m_pwndView ) {
        TRACE0( "Failed to create view window\n" );
        return -1;
    }

    // now we need to figure out how to get it to create its Win32 HWND window.
    // this code would do it, if CFormView::Create() weren't protected:
    //if ( !m_pwndView->Create( nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect( 0, 0, 0, 0 ), this, AFX_IDW_PANE_FIRST, nullptr ) ) {
    //    TRACE0("Failed to create view window\n");
    //    return -1;
    //}

    return 0;
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs ) {
    if ( !CFrameWnd::PreCreateWindow( cs ) ) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.lpszClass  = AfxRegisterWndClass( 0 );

    return TRUE;
}

void CMainFrame::OnSetFocus( CWnd* /*pOldWnd*/ ) {
    // forward focus to the view window
    if ( m_pwndView->GetSafeHwnd( ) ) {
        m_pwndView->SetFocus( );
    }
}

BOOL CMainFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo ) {
    // let the view have first crack at the command
    if ( m_pwndView->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) {
        return TRUE;
    }

    // otherwise, do default handling
    return CFrameWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}
