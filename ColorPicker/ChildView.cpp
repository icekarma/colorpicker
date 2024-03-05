#include "pch.h"

#include "ColorPicker.h"
#include "ChildView.h"

IMPLEMENT_DYNCREATE( CChildView, CFormView )

BEGIN_MESSAGE_MAP( CChildView, CFormView )
    ON_WM_PAINT( )
END_MESSAGE_MAP( )

//void CChildView::DoDataExchange( CDataExchange* pDX ) {
//    CFormView::DoDataExchange( pDX );
//}

BOOL CChildView::PreCreateWindow( CREATESTRUCT& cs ) {
    if ( !CWnd::PreCreateWindow( cs ) ) {
        return FALSE;
    }

    cs.style     &= ~( WS_BORDER | WS_DLGFRAME | WS_THICKFRAME );
    cs.dwExStyle &= ~( WS_EX_CLIENTEDGE );
    cs.lpszClass  = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor( nullptr, IDC_ARROW ), reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), nullptr );

    return TRUE;
}

void CChildView::OnPaint( ) {
    // Do not call CWnd::OnPaint() for painting messages
    CPaintDC dc { this };

    // TODO
}
