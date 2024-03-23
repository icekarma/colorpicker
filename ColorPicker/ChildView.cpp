#include "pch.h"

#include "ChildView.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

#include "Color.h"

//==============================================================//
// TODO * TODO * TODO * TODO * TODO * TODO * TODO * TODO * TODO //
//                                                              //
// * Add a hexadecimal representation of the sRGB colour value. //
// * Option for 0..255 vs 0..100                                //
// * Option for floating point with 1dp                         //
// * Per-channel gradients, à la https://imgur.com/a/BWOAht9 ?  //
// * Color library                                              //
//                                                              //
// TODO * TODO * TODO * TODO * TODO * TODO * TODO * TODO * TODO //
//==============================================================//

IMPLEMENT_DYNCREATE( CChildView, CFormView )

BEGIN_MESSAGE_MAP( CChildView, CFormView )
    ON_BN_CLICKED   ( IDCLOSE,        &CChildView::OnCloseButtonClicked )
    ON_MESSAGE      ( ZSBN_MOUSEMOVE, &CChildView::OnZsbnMouseMove      )

    ON_CONTROL_RANGE( BN_CLICKED, IDC_LAB_L_CHANNEL, IDC_SRGB_B_CHANNEL, &CChildView::OnChannelButtonClicked )
    ON_CONTROL_RANGE( EN_CHANGE,  IDC_LAB_L_VALUE,   IDC_SRGB_B_VALUE,   &CChildView::OnColorValueChange     )
END_MESSAGE_MAP( )

namespace {

    //
    // Constants
    //

    std::unordered_map<int, Triplet<AllChannels>> const _ControlIdToChannels {
        { IDC_LAB_L_CHANNEL,  { AllChannels::LabA,  AllChannels::LabB,  AllChannels::LabL  } },
        { IDC_LAB_A_CHANNEL,  { AllChannels::LabB,  AllChannels::LabL,  AllChannels::LabA  } },
        { IDC_LAB_B_CHANNEL,  { AllChannels::LabA,  AllChannels::LabL,  AllChannels::LabB  } },
        { IDC_SRGB_R_CHANNEL, { AllChannels::SrgbB, AllChannels::SrgbG, AllChannels::SrgbR } },
        { IDC_SRGB_G_CHANNEL, { AllChannels::SrgbB, AllChannels::SrgbR, AllChannels::SrgbG } },
        { IDC_SRGB_B_CHANNEL, { AllChannels::SrgbR, AllChannels::SrgbG, AllChannels::SrgbB } },
    };

    //
    // Private functions
    //

    void _AdjustPosition( CWnd* ctrl, SIZE const& adjust ) {
        WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

        ctrl->GetWindowPlacement( &wp );
        wp.rcNormalPosition.left   += adjust.cx;
        wp.rcNormalPosition.top    += adjust.cy;
        wp.rcNormalPosition.right  += adjust.cx;
        wp.rcNormalPosition.bottom += adjust.cy;
        ctrl->SetWindowPlacement( &wp );
    }

    void _AdjustSize( CWnd* ctrl, SIZE const& adjust ) {
        WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

        ctrl->GetWindowPlacement( &wp );
        wp.rcNormalPosition.right  += adjust.cx;
        wp.rcNormalPosition.bottom += adjust.cy;
        ctrl->SetWindowPlacement( &wp );
    }

    void _SetPosition( CWnd* ctrl, SIZE const& adjust ) {
        WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

        ctrl->GetWindowPlacement( &wp );
        CRect rect { wp.rcNormalPosition };
        rect.MoveToXY( adjust.cx, adjust.cy );
        wp.rcNormalPosition = rect;
        ctrl->SetWindowPlacement( &wp );
    }

    void _SetSize( CWnd* ctrl, SIZE const& size ) {
        WINDOWPLACEMENT wp { sizeof WINDOWPLACEMENT, };

        ctrl->GetWindowPlacement( &wp );
        wp.rcNormalPosition.right  = wp.rcNormalPosition.left + size.cx;
        wp.rcNormalPosition.bottom = wp.rcNormalPosition.top  + size.cy;
        ctrl->SetWindowPlacement( &wp );
    }

}

void CChildView::DoDataExchange( CDataExchange* pDX ) {
    debug( "CChildView::DoDataExchange\n" );

    CFormView::DoDataExchange( pDX );

    // Do data exchange for controls

    if ( !m_groupBoxLab.GetSafeHwnd( ) ) {
        DDX_Control( pDX, IDC_GROUPBOX_LAB,   m_groupBoxLab        );
        DDX_Control( pDX, IDC_LAB_L_CHANNEL,  m_buttonLabLChannel  );
        DDX_Control( pDX, IDC_LAB_A_CHANNEL,  m_buttonLabAChannel  );
        DDX_Control( pDX, IDC_LAB_B_CHANNEL,  m_buttonLabBChannel  );
        DDX_Control( pDX, IDC_LAB_L_VALUE,    m_editLabLValue      );
        DDX_Control( pDX, IDC_LAB_A_VALUE,    m_editLabAValue      );
        DDX_Control( pDX, IDC_LAB_B_VALUE,    m_editLabBValue      );

        DDX_Control( pDX, IDC_GROUPBOX_SRGB,  m_groupBoxSrgb       );
        DDX_Control( pDX, IDC_SRGB_R_CHANNEL, m_buttonSrgbRChannel );
        DDX_Control( pDX, IDC_SRGB_G_CHANNEL, m_buttonSrgbGChannel );
        DDX_Control( pDX, IDC_SRGB_B_CHANNEL, m_buttonSrgbBChannel );
        DDX_Control( pDX, IDC_SRGB_R_VALUE,   m_editSrgbRValue     );
        DDX_Control( pDX, IDC_SRGB_G_VALUE,   m_editSrgbGValue     );
        DDX_Control( pDX, IDC_SRGB_B_VALUE,   m_editSrgbBValue     );

        DDX_Control( pDX, IDC_XY_GRID,        m_staticXyGrid       );
        DDX_Control( pDX, IDC_Z_STRIP,        m_staticZStrip       );

        DDX_Control( pDX, IDCLOSE,            m_buttonClose        );
    }

    // Do data exchange for values

    DDX_Radio  ( pDX, IDC_LAB_L_CHANNEL,  m_nSelectedChannel   );

    DDX_Text   ( pDX, IDC_LAB_L_VALUE,    m_nLabLValue         );
    DDX_Text   ( pDX, IDC_LAB_A_VALUE,    m_nLabAValue         );
    DDX_Text   ( pDX, IDC_LAB_B_VALUE,    m_nLabBValue         );

    DDX_Text   ( pDX, IDC_SRGB_R_VALUE,   m_nSrgbRValue        );
    DDX_Text   ( pDX, IDC_SRGB_G_VALUE,   m_nSrgbGValue        );
    DDX_Text   ( pDX, IDC_SRGB_B_VALUE,   m_nSrgbBValue        );

    debug(
        "==================================================\n"
        "Selected channel: %d\n"
        "L*a*b*:           (%4d, %4d, %4d)\n"
        "RGB:              (%4d, %4d, %4d)\n"
        "",
        m_nSelectedChannel,
        m_nLabLValue,  m_nLabAValue,  m_nLabBValue,
        m_nSrgbRValue, m_nSrgbGValue, m_nSrgbBValue
    );
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

    CColorPickerDoc* pDoc { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };

    m_staticZStrip.SetCustomMessageTargetWindow( GetSafeHwnd( ) );
    m_staticXyGrid.SetCustomMessageTargetWindow( GetSafeHwnd( ) );

    //ZE adjust0   { 0,  0 };
    SIZE adjustUp1 { 0, -1 }; SIZE adjustDn1 { 0, 1 };
    SIZE adjustUp2 { 0, -2 }; SIZE adjustDn2 { 0, 2 };

    _AdjustPosition( &m_editLabBValue,      adjustUp2 );
    //djustPosition( &m_editLabLValue,      adjust0   );
    //djustPosition( &m_editLabAValue,      adjust0   );
    //djustPosition( &m_editLabBValue,      adjust0   );
    _AdjustPosition( &m_buttonLabLChannel,  adjustDn1 );
    _AdjustPosition( &m_buttonLabAChannel,  adjustUp1 );
    _AdjustPosition( &m_buttonLabBChannel,  adjustUp2 );
    _AdjustSize    ( &m_groupBoxLab,        adjustUp1 );

    _AdjustPosition( &m_groupBoxSrgb,       adjustDn2 );
    _AdjustPosition( &m_editSrgbRValue,     adjustDn2 );
    _AdjustPosition( &m_editSrgbGValue,     adjustDn2 );
    _AdjustPosition( &m_editSrgbBValue,     adjustUp1 );
    _AdjustPosition( &m_buttonSrgbRChannel, adjustDn2 );
    //djustPosition( &m_buttonSrgbGChannel, adjust0   );
    _AdjustPosition( &m_buttonSrgbBChannel, adjustUp2 );
    _AdjustSize    ( &m_groupBoxSrgb,       adjustUp1 );

    _SetSize       ( &m_staticZStrip,       {  20, 256 } );

    _AdjustPosition( &m_staticXyGrid,       {  -1,   0 } );
    _SetSize       ( &m_staticXyGrid,       { 256, 256 } );

    _AdjustPosition( &m_buttonClose,        {  -1,  -2 } );

    m_buttonLabLChannel.SetCheck( BST_CHECKED );
    m_editLabLValue.SetFocus( );

    if ( m_bitmapZStrip.CreateBitmap( CZStrip::ImageWidth, CZStrip::ImageHeight, 1, 32, nullptr ) ) {
        m_staticZStrip.SetBitmap( m_bitmapZStrip );
    } else {
        debug( "CChildView::OnInitialUpdate: m_bitmapZStrip.CreateBitmap failed\n" );
    }
    m_pZStrip = new CZStrip { pDoc, &m_bitmapZStrip };
    m_pZStrip->SetChannel( m_channelZ );
    m_pZStrip->Update( );

    if ( m_bitmapXyGrid.CreateBitmap( CXyGrid::ImageWidth, CXyGrid::ImageHeight, 1, 32, nullptr ) ) {
        m_staticXyGrid.SetBitmap( m_bitmapXyGrid );
    } else {
        debug( "CChildView::OnInitialUpdate: m_bitmapXyGrid.CreateBitmap failed\n" );
    }
    m_pXyGrid = new CXyGrid { pDoc, &m_bitmapXyGrid };
    m_pXyGrid->SetChannels( m_channelX, m_channelY, m_channelZ );
    m_pXyGrid->Update( );
}

void CChildView::OnCloseButtonClicked( ) {
    ::PostQuitMessage( 0 );
}

void CChildView::OnChannelButtonClicked( UINT const uId ) {
    debug( "CChildView::OnChannelButtonClicked: uId: %u\n", uId );

    Triplet<AllChannels> const& channels { _ControlIdToChannels.at( uId ) };
    m_channelX = channels[0];
    m_channelY = channels[1];
    m_channelZ = channels[2];

    UpdateBitmaps( );
}

void CChildView::UpdateBitmaps( ) {
    if ( m_pZStrip ) {
        m_pZStrip->SetChannel( m_channelZ );
        m_pZStrip->Update( );
        m_staticZStrip.Invalidate( );
    }

    if ( m_pXyGrid ) {
        m_pXyGrid->SetChannels( m_channelX, m_channelY, m_channelZ );
        m_pXyGrid->Update( );
        m_staticXyGrid.Invalidate( );
    }
}

bool CChildView::GetValueFromEdit( CEdit const& edit, int& result ) {
    int cbText { edit.GetWindowTextLengthW( ) };
    if ( cbText < 1 ) {
        debug( "CChildView::GetValueFromEdit: bail 1\n" );
        return false;
    }
    ++cbText;

    wchar_t* pwszText { new wchar_t[cbText + 1] };
    if ( !pwszText ) {
        debug( "CChildView::GetValueFromEdit: bail 2\n" );
        return false;
    }
    if ( edit.GetWindowTextW( pwszText, cbText ) < 1 ) {
        delete[] pwszText;
        debug( "CChildView::GetValueFromEdit: bail 3\n" );
        return false;
    }

    for ( int index { cbText - 1 }; index >= 0; --index ) {
        if ( !iswspace( pwszText[index] ) ) {
            break;
        }
        pwszText[index] = '\0';
    }

    errno = 0;
    wchar_t* pwszEnd { };
    long tmp = wcstol( pwszText, &pwszEnd, 10 );
    if ( !pwszEnd || *pwszEnd || ( tmp < static_cast<long>( INT_MIN ) ) || ( tmp > static_cast<long>( INT_MAX ) ) ) {
        delete[] pwszText;
        debug( "CChildView::GetValueFromEdit: bail 4\n" );
        return false;
    }

    result = static_cast<int>( tmp );
    delete[] pwszText;
    return true;
}

bool CChildView::GetValueAndChangeStateFromEdit( CEdit const& edit, int& nValue, bool& fChanged ) {
    int oldValue { nValue };

    if ( !GetValueFromEdit( edit, nValue ) ) {
        return false;
    }

    fChanged = oldValue != nValue;
    return true;
}

void CChildView::PutValueToEdit( CEdit& edit, int const value ) const {
    edit.SetWindowTextW( std::to_wstring( value ).c_str( ) );
}

void CChildView::SetChannelValue( AllChannels channel, int value ) {
    CColorPickerDoc* pDoc { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };

    switch ( channel ) {
        case AllChannels::LabL:
        case AllChannels::LabA:
        case AllChannels::LabB: {
            auto color { pDoc->GetLabColor( ) };
            color.SetChannelValue( AllChannelsToLabChannels( channel ), static_cast<LabValueT>( value ) );
            pDoc->SetColor( color );
            break;
        }

        case AllChannels::SrgbR:
        case AllChannels::SrgbG:
        case AllChannels::SrgbB: {
            auto color { pDoc->GetSrgbColor( ) };
            color.SetChannelValue( AllChannelsToSrgbChannels( channel ), static_cast<SrgbValueT>( value ) );
            pDoc->SetColor( color );
            break;
        }

        default:
            break;
    }
}

void CChildView::OnColorValueChange( UINT const uId ) {
    static unsigned busy { };
    if ( InterlockedExchange( &busy, 1 ) ) {
        return;
    }

    CColorPickerDoc* pDoc { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };

    switch ( uId ) {
        case IDC_LAB_L_VALUE:
        case IDC_LAB_A_VALUE:
        case IDC_LAB_B_VALUE: {
            Triplet<LabValueT>  oldLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
            Triplet<SrgbValueT> oldSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };
            bool                fChanged      { };

            debug( "CChildView::OnColorValueChange: L*a*b*, before update: (%4d, %4d, %4d)\n", m_nLabLValue, m_nLabAValue, m_nLabBValue );
            if ( uId == IDC_LAB_L_VALUE ) { GetValueAndChangeStateFromEdit( m_editLabLValue, m_nLabLValue, fChanged ); }
            if ( uId == IDC_LAB_A_VALUE ) { GetValueAndChangeStateFromEdit( m_editLabAValue, m_nLabAValue, fChanged ); }
            if ( uId == IDC_LAB_B_VALUE ) { GetValueAndChangeStateFromEdit( m_editLabBValue, m_nLabBValue, fChanged ); }
            debug( "CChildView::OnColorValueChange: fChanged: %s\n", fChanged ? "true" : "false" );

            if ( fChanged ) {
                pDoc->SetColor( LabColorValue { m_nLabLValue, m_nLabAValue, m_nLabBValue } );
                debug( "CChildView::OnColorValueChange: L*a*b*, after update:  (%4d, %4d, %4d)\n", m_nLabLValue,  m_nLabAValue,  m_nLabBValue  );
                debug( "CChildView::OnColorValueChange: sRGB,   after update:  (%4d, %4d, %4d)\n", m_nSrgbRValue, m_nSrgbGValue, m_nSrgbBValue );

                Triplet<SrgbValueT> srgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };
                if ( srgbValues[+SrgbChannels::R] != oldSrgbValues[+SrgbChannels::R] ) { m_nSrgbRValue = srgbValues[+SrgbChannels::R]; PutValueToEdit( m_editSrgbRValue, m_nSrgbRValue ); }
                if ( srgbValues[+SrgbChannels::G] != oldSrgbValues[+SrgbChannels::G] ) { m_nSrgbGValue = srgbValues[+SrgbChannels::G]; PutValueToEdit( m_editSrgbGValue, m_nSrgbGValue ); }
                if ( srgbValues[+SrgbChannels::B] != oldSrgbValues[+SrgbChannels::B] ) { m_nSrgbBValue = srgbValues[+SrgbChannels::B]; PutValueToEdit( m_editSrgbBValue, m_nSrgbBValue ); }
            }
            break;
        }

        case IDC_SRGB_R_VALUE:
        case IDC_SRGB_G_VALUE:
        case IDC_SRGB_B_VALUE: {
            Triplet<LabValueT>  oldLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
            Triplet<SrgbValueT> oldSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };
            bool                fChanged      { };

            debug( "CChildView::OnColorValueChange: sRGB, before update: (%4d, %4d, %4d)\n", m_nSrgbRValue, m_nSrgbGValue, m_nSrgbBValue );
            if ( uId == IDC_SRGB_R_VALUE ) { GetValueAndChangeStateFromEdit( m_editSrgbRValue, m_nSrgbRValue, fChanged ); }
            if ( uId == IDC_SRGB_G_VALUE ) { GetValueAndChangeStateFromEdit( m_editSrgbGValue, m_nSrgbGValue, fChanged ); }
            if ( uId == IDC_SRGB_B_VALUE ) { GetValueAndChangeStateFromEdit( m_editSrgbBValue, m_nSrgbBValue, fChanged ); }
            debug( "CChildView::OnColorValueChange: fChanged: %s\n", fChanged ? "true" : "false" );

            if ( fChanged ) {
                pDoc->SetColor( SrgbColorValue { m_nSrgbRValue, m_nSrgbGValue, m_nSrgbBValue } );
                debug( "CChildView::OnColorValueChange: sRGB,   after update:  (%4d, %4d, %4d)\n", m_nSrgbRValue, m_nSrgbGValue, m_nSrgbBValue );
                debug( "CChildView::OnColorValueChange: L*a*b*, after update:  (%4d, %4d, %4d)\n", m_nLabLValue,  m_nLabAValue,  m_nLabBValue  );

                Triplet<LabValueT> labValues { pDoc->GetLabColor( ).GetChannelValues( ) };
                if ( labValues[+LabChannels::L] != oldLabValues[+LabChannels::L] ) { m_nLabLValue = labValues[+LabChannels::L]; PutValueToEdit( m_editLabLValue, m_nLabLValue ); }
                if ( labValues[+LabChannels::a] != oldLabValues[+LabChannels::a] ) { m_nLabAValue = labValues[+LabChannels::a]; PutValueToEdit( m_editLabAValue, m_nLabAValue ); }
                if ( labValues[+LabChannels::b] != oldLabValues[+LabChannels::b] ) { m_nLabBValue = labValues[+LabChannels::b]; PutValueToEdit( m_editLabBValue, m_nLabBValue ); }
            }
            break;
        }
    }

    UpdateBitmaps( );

    InterlockedExchange( &busy, 0 );
}

afx_msg LRESULT CChildView::OnZsbnMouseMove( WPARAM wParam, LPARAM lParam ) {
    if ( wParam == IDC_Z_STRIP ) {
        OnZStripMouseMove( LOWORD( lParam ), HIWORD( lParam ) );
    } else if ( wParam == IDC_XY_GRID ) {
        OnXyGridMouseMove( LOWORD( lParam ), HIWORD( lParam ) );
    }

    return 0;
}

void CChildView::OnZStripMouseMove( int x, int y ) {
    debug( "CChildView::OnZStripMouseMove: point: (%d,%d)\n", x, y );
    SetChannelValue( m_channelZ, y );
}

void CChildView::OnXyGridMouseMove( int x, int y ) {
    debug( "CChildView::OnXyGridMouseMove: point: (%d,%d)\n", x, y );
    SetChannelValue( m_channelX, x );
    SetChannelValue( m_channelY, y );
}
