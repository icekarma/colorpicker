#include "pch.h"

#include "ColorPicker.h"
#include "ChildView.h"

// XY color grid: 256x256
// Z color strip: 19x256

// Selection  Z   X   Y
// -- ColorSpace::Lab:
// L*         L*  a*  b*
// a*         a*  b*  L*
// b*         b*  a*  L*
// -- ColorSpace::sRGB:
// R          R   B   G
// G          G   B   R
// B          B   R   G

IMPLEMENT_DYNCREATE( CChildView, CFormView )

BEGIN_MESSAGE_MAP( CChildView, CFormView )
    ON_WM_PAINT( )
    ON_BN_CLICKED( IDCLOSE, &CChildView::OnCloseButtonClicked )
END_MESSAGE_MAP( )

void CChildView::_AdjustPosition( CWnd* ctrl, SIZE const& adjust ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    ctrl->GetWindowPlacement( &wp );
    CRect rect { wp.rcNormalPosition };
    rect.OffsetRect( adjust );
    wp.rcNormalPosition = rect;
    ctrl->SetWindowPlacement( &wp );
}

void CChildView::_AdjustSize( CWnd* ctrl, SIZE const& adjust ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    ctrl->GetWindowPlacement( &wp );
    wp.rcNormalPosition.right  += adjust.cx;
    wp.rcNormalPosition.bottom += adjust.cy;
    ctrl->SetWindowPlacement( &wp );
}

void CChildView::DoDataExchange( CDataExchange* pDX ) {
    CFormView::DoDataExchange( pDX );

    DDX_Control( pDX, IDC_GROUPBOX_LAB,   m_groupBoxLab );
    DDX_Control( pDX, IDC_LAB_L_VALUE,    m_editLabLValue );
    DDX_Control( pDX, IDC_LAB_A_VALUE,    m_editLabAValue );
    DDX_Control( pDX, IDC_LAB_B_VALUE,    m_editLabBValue );
    DDX_Control( pDX, IDC_LAB_L_CHANNEL,  m_buttonLabLChannel );
    DDX_Control( pDX, IDC_LAB_A_CHANNEL,  m_buttonLabAChannel );
    DDX_Control( pDX, IDC_LAB_B_CHANNEL,  m_buttonLabBChannel );

    DDX_Control( pDX, IDC_GROUPBOX_SRGB,  m_groupBoxSrgb );
    DDX_Control( pDX, IDC_SRGB_R_VALUE,   m_editSrgbRValue );
    DDX_Control( pDX, IDC_SRGB_G_VALUE,   m_editSrgbGValue );
    DDX_Control( pDX, IDC_SRGB_B_VALUE,   m_editSrgbBValue );
    DDX_Control( pDX, IDC_SRGB_R_CHANNEL, m_buttonSrgbRChannel );
    DDX_Control( pDX, IDC_SRGB_G_CHANNEL, m_buttonSrgbGChannel );
    DDX_Control( pDX, IDC_SRGB_B_CHANNEL, m_buttonSrgbBChannel );

    DDX_Text( pDX, IDC_LAB_L_VALUE,  m_nLabLValue );
    DDX_Text( pDX, IDC_LAB_A_VALUE,  m_nLabAValue );
    DDX_Text( pDX, IDC_LAB_B_VALUE,  m_nLabBValue );

    DDX_Text( pDX, IDC_SRGB_R_VALUE, m_nSrgbRValue );
    DDX_Text( pDX, IDC_SRGB_G_VALUE, m_nSrgbGValue );
    DDX_Text( pDX, IDC_SRGB_B_VALUE, m_nSrgbBValue );
}

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

void CChildView::OnCloseButtonClicked( ) {
    ::PostQuitMessage( 0 );
}
