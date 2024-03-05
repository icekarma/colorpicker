#include "pch.h"

#include "ColorPicker.h"
#include "ChildView.h"

//CChildView::CChildView( ) {
//    /*empty*/
//}

//CChildView::~CChildView( ) {
//    /*empty*/
//}

BEGIN_MESSAGE_MAP( CChildView, CWnd )
    ON_WM_PAINT( )
END_MESSAGE_MAP( )

BOOL CChildView::PreCreateWindow( CREATESTRUCT& cs ) {
    if ( !CWnd::PreCreateWindow( cs ) ) {
        return FALSE;
    }

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style     &= ~WS_BORDER;
    cs.lpszClass  = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor( nullptr, IDC_ARROW ), reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), nullptr );

    return TRUE;
}

void CChildView::OnPaint( ) {
    CPaintDC dc( this );

    // Do not call CWnd::OnPaint() for painting messages
}

