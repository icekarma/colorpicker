#include "pch.h"

#include "ChildView.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#include "Color.h"
#include "TransformsManager.h"

//==============================================================//
// TODO * TODO * TODO * TODO * TODO * TODO * TODO * TODO * TODO //
//                                                              //
//                                                              //
// * Add a hexadecimal representation of the sRGB colour value. //
// * Option for 0..255 vs 0..100                                //
// * Option for floating point with 1dp                         //
// * Per-channel gradients, à la https://imgur.com/a/BWOAht9 ?  //
// * Color library                                              //
//                                                              //
//                                                              //
// TODO * TODO * TODO * TODO * TODO * TODO * TODO * TODO * TODO //
//==============================================================//

// XY color grid: 256x256
// Z color strip: 20x256

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
    //ON_WM_PAINT( )
    ON_BN_CLICKED( IDCLOSE, &CChildView::OnCloseButtonClicked )
END_MESSAGE_MAP( )

void CChildView::_AdjustPosition( CWnd* ctrl, SIZE const& adjust ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    ctrl->GetWindowPlacement( &wp );
    wp.rcNormalPosition.left   += adjust.cx;
    wp.rcNormalPosition.top    += adjust.cy;
    wp.rcNormalPosition.right  += adjust.cx;
    wp.rcNormalPosition.bottom += adjust.cy;
    ctrl->SetWindowPlacement( &wp );
}

void CChildView::_AdjustSize( CWnd* ctrl, SIZE const& adjust ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    ctrl->GetWindowPlacement( &wp );
    wp.rcNormalPosition.right  += adjust.cx;
    wp.rcNormalPosition.bottom += adjust.cy;
    ctrl->SetWindowPlacement( &wp );
}

void CChildView::_SetPosition( CWnd* ctrl, POINT const& pt ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    ctrl->GetWindowPlacement( &wp );
    CRect rect { wp.rcNormalPosition };
    rect.MoveToXY( pt );
    wp.rcNormalPosition = rect;
    ctrl->SetWindowPlacement( &wp );
}

void CChildView::_SetSize( CWnd* ctrl, SIZE const& size ) {
    WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

    ctrl->GetWindowPlacement( &wp );
    wp.rcNormalPosition.right  = wp.rcNormalPosition.left + size.cx;
    wp.rcNormalPosition.bottom = wp.rcNormalPosition.top  + size.cy;
    ctrl->SetWindowPlacement( &wp );
}

void CChildView::DoDataExchange( CDataExchange* pDX ) {
    CFormView::DoDataExchange( pDX );

    // Do data exchange for controls

    DDX_Control( pDX, IDC_GROUPBOX_LAB,   m_groupBoxLab        );
    DDX_Control( pDX, IDC_LAB_L_VALUE,    m_editLabLValue      );
    DDX_Control( pDX, IDC_LAB_A_VALUE,    m_editLabAValue      );
    DDX_Control( pDX, IDC_LAB_B_VALUE,    m_editLabBValue      );
    DDX_Control( pDX, IDC_LAB_L_CHANNEL,  m_buttonLabLChannel  );
    DDX_Control( pDX, IDC_LAB_A_CHANNEL,  m_buttonLabAChannel  );
    DDX_Control( pDX, IDC_LAB_B_CHANNEL,  m_buttonLabBChannel  );

    DDX_Control( pDX, IDC_GROUPBOX_SRGB,  m_groupBoxSrgb       );
    DDX_Control( pDX, IDC_SRGB_R_VALUE,   m_editSrgbRValue     );
    DDX_Control( pDX, IDC_SRGB_G_VALUE,   m_editSrgbGValue     );
    DDX_Control( pDX, IDC_SRGB_B_VALUE,   m_editSrgbBValue     );
    DDX_Control( pDX, IDC_SRGB_R_CHANNEL, m_buttonSrgbRChannel );
    DDX_Control( pDX, IDC_SRGB_G_CHANNEL, m_buttonSrgbGChannel );
    DDX_Control( pDX, IDC_SRGB_B_CHANNEL, m_buttonSrgbBChannel );

    DDX_Control( pDX, IDC_COLOR_XY_GRID,  m_staticXyGrid       );
    DDX_Control( pDX, IDC_COLOR_Z_STRIP,  m_staticZStrip       );

    DDX_Control( pDX, IDCLOSE,            m_buttonClose        );

    // Do data exchange for values

    DDX_Text( pDX,    IDC_LAB_L_VALUE,    m_nLabLValue         );
    DDX_Text( pDX,    IDC_LAB_A_VALUE,    m_nLabAValue         );
    DDX_Text( pDX,    IDC_LAB_B_VALUE,    m_nLabBValue         );

    DDX_Text( pDX,    IDC_SRGB_R_VALUE,   m_nSrgbRValue        );
    DDX_Text( pDX,    IDC_SRGB_G_VALUE,   m_nSrgbGValue        );
    DDX_Text( pDX,    IDC_SRGB_B_VALUE,   m_nSrgbBValue        );
}

BOOL CChildView::PreCreateWindow( CREATESTRUCT& cs ) {
    if ( !CWnd::PreCreateWindow( cs ) ) {
        return FALSE;
    }

    cs.style     &= ~WindowStylesToRemove;
    cs.dwExStyle &= ~ExtendedWindowStylesToRemove;
    cs.lpszClass  = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor( nullptr, IDC_ARROW ), reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), nullptr );

    return TRUE;
}

void CChildView::OnInitialUpdate( ) {
    CFormView::OnInitialUpdate( );

    //ZE adjust0   { 0,  0 };
    SIZE adjustUp1 { 0, -1 }; SIZE adjustDn1 { 0, 1 };
    SIZE adjustUp2 { 0, -2 }; SIZE adjustDn2 { 0, 2 };

    _AdjustPosition( &m_editLabBValue,      adjustUp2    );
    //djustPosition( &m_editLabLValue,      adjust0      );
    //djustPosition( &m_editLabAValue,      adjust0      );
    //djustPosition( &m_editLabBValue,      adjust0      );
    _AdjustPosition( &m_buttonLabLChannel,  adjustDn1    );
    _AdjustPosition( &m_buttonLabAChannel,  adjustUp1    );
    _AdjustPosition( &m_buttonLabBChannel,  adjustUp2    );
    _AdjustSize    ( &m_groupBoxLab,        adjustUp1    );

    _AdjustPosition( &m_groupBoxSrgb,       adjustDn2    );
    _AdjustPosition( &m_editSrgbRValue,     adjustDn2    );
    _AdjustPosition( &m_editSrgbGValue,     adjustDn2    );
    _AdjustPosition( &m_editSrgbBValue,     adjustUp1    );
    _AdjustPosition( &m_buttonSrgbRChannel, adjustDn2    );
    //djustPosition( &m_buttonSrgbGChannel, adjust0      );
    _AdjustPosition( &m_buttonSrgbBChannel, adjustUp2    );
    _AdjustSize    ( &m_groupBoxSrgb,       adjustUp1    );

    _SetSize       ( &m_staticZStrip,       {  20, 256 } );

    _AdjustPosition( &m_staticXyGrid,       {  -1,   0 } );
    _SetSize       ( &m_staticXyGrid,       { 256, 256 } );

    _AdjustPosition( &m_buttonClose,        {  -1,  -2 } );

    m_editLabLValue.SetFocus( );

    if ( m_bitmapZStrip.CreateBitmap( 20, 256, 1, 32, nullptr ) ) {
        m_staticZStrip.SetBitmap( m_bitmapZStrip );
    } else {
        debug( "CChildView::OnInitialUpdate: m_bitmapZStrip.CreateBitmap failed\n" );
    }

    if ( m_bitmapXyGrid.CreateBitmap( 256, 256, 1, 32, nullptr ) ) {
        m_staticXyGrid.SetBitmap( m_bitmapXyGrid );
    } else {
        debug( "CChildView::OnInitialUpdate: m_bitmapXyGrid.CreateBitmap failed\n" );
    }

    CColorPickerDoc* pDoc { DYNAMIC_DOWNCAST( CColorPickerDoc, GetDocument( ) ) };
    m_pZStrip = new CZStrip { pDoc, &m_bitmapZStrip };
    m_pZStrip->Update( );
    m_pXyGrid = new CXyGrid { pDoc, &m_bitmapXyGrid };
    m_pXyGrid->Update( );
}

//void CChildView::OnPaint( ) {
//    // Do not call CWnd::OnPaint() for painting messages
//    CPaintDC dc { this };
//
//    // TODO
//}

void CChildView::OnCloseButtonClicked( ) {
    ::PostQuitMessage( 0 );
}
