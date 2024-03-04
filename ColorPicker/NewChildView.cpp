#include "pch.h"

#include "ColorPicker.h"
#include "NewChildView.h"

IMPLEMENT_DYNCREATE( CNewChildView, CFormView )

BEGIN_MESSAGE_MAP( CNewChildView, CFormView )
    ON_WM_PAINT( )
END_MESSAGE_MAP( )

CNewChildView::CNewChildView( ):
    CFormView { IDD_NEWCHILDVIEW }
{
    /*empty*/
}

CNewChildView::~CNewChildView( ) {
    /*empty*/
}

void CNewChildView::DoDataExchange( CDataExchange* pDX ) {
    CFormView::DoDataExchange( pDX );
}

BOOL CNewChildView::PreCreateWindow( CREATESTRUCT& cs ) {
    if ( !CWnd::PreCreateWindow( cs ) ) {
        return FALSE;
    }

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style     &= ~WS_BORDER;
    cs.lpszClass  = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor( nullptr, IDC_ARROW ), reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), nullptr );

    return TRUE;
}

void CNewChildView::OnPaint( ) {
    // Do not call CWnd::OnPaint() for painting messages

    // TODO

    //CPaintDC dc { this };
}
