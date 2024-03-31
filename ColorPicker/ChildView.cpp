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
    ON_BN_CLICKED( IDCLOSE, &CChildView::OnCloseButtonClicked )

    ON_NOTIFY( ZSBN_MOUSEMOVE, IDC_Z_STRIP, &CChildView::OnZStripMouseMove )
    ON_NOTIFY( ZSBN_MOUSEMOVE, IDC_XY_GRID, &CChildView::OnXyGridMouseMove )

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
    CFormView::DoDataExchange( pDX );

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

    DDX_Control( pDX, IDC_SWATCH,         m_staticSwatch       );
    DDX_Control( pDX, IDC_Z_STRIP,        m_staticZStrip       );
    DDX_Control( pDX, IDC_XY_GRID,        m_staticXyGrid       );

    DDX_Control( pDX, IDCLOSE,            m_buttonClose        );
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

    //djustPosition( &m_staticSwatch,       adjust0      );

    _SetSize       ( &m_staticZStrip,       {  20, 256 } );

    _AdjustPosition( &m_staticXyGrid,       {  -1,   0 } );
    _SetSize       ( &m_staticXyGrid,       { 256, 256 } );

    _AdjustPosition( &m_buttonClose,        {  -1,  -2 } );

    m_buttonLabLChannel.SetCheck( BST_CHECKED );
    m_editLabLValue.SetFocus( );

    m_pSwatch = new CSwatch { pDoc, &m_staticSwatch };
    m_pSwatch->Update( );

    m_pZStrip = new CZStrip { pDoc, &m_staticZStrip };
    m_pZStrip->SetChannel( m_channelZ );
    m_pZStrip->Update( );

    m_pXyGrid = new CXyGrid { pDoc, &m_staticXyGrid };
    m_pXyGrid->SetChannels( m_channelX, m_channelY, m_channelZ );
    m_pXyGrid->Update( );

    // Set m_uBusy to prevent OnColorValueChange from interfering
    InterlockedExchange( &m_uBusy, 1 );

    Triplet<LabValueT> labValues { pDoc->GetLabColor( ).GetChannelValues( ) };
    PutValueToEdit( m_editLabLValue, labValues[+LabChannels::L] );
    PutValueToEdit( m_editLabAValue, labValues[+LabChannels::a] );
    PutValueToEdit( m_editLabBValue, labValues[+LabChannels::b] );

    Triplet<SrgbValueT> srgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };
    PutValueToEdit( m_editSrgbRValue, srgbValues[+SrgbChannels::R] );
    PutValueToEdit( m_editSrgbGValue, srgbValues[+SrgbChannels::G] );
    PutValueToEdit( m_editSrgbBValue, srgbValues[+SrgbChannels::B] );

    InterlockedExchange( &m_uBusy, 0 );
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
    if ( m_pSwatch ) {
        m_pSwatch->Update( );
        m_staticSwatch.Invalidate( FALSE );
    }

    if ( m_pZStrip ) {
        m_pZStrip->SetChannel( m_channelZ );
        m_pZStrip->Update( );
        m_staticZStrip.Invalidate( FALSE );
    }

    if ( m_pXyGrid ) {
        m_pXyGrid->SetChannels( m_channelX, m_channelY, m_channelZ );
        m_pXyGrid->Update( );
        m_staticXyGrid.Invalidate( FALSE );
    }
}

bool CChildView::GetValueFromEdit( CEdit const& edit, int& result ) {
    int cbText { edit.GetWindowTextLengthW( ) };
    if ( cbText < 1 ) {
        debug( "CChildView::GetValueFromEdit: bail 1: no text in control\n" );
        return false;
    }
    ++cbText;

    wchar_t* pwszText { new wchar_t[cbText + 1] };
    if ( !pwszText ) {
        debug( "CChildView::GetValueFromEdit: bail 2: memory allocation failure\n" );
        return false;
    }
    if ( edit.GetWindowTextW( pwszText, cbText ) < 1 ) {
        delete[] pwszText;
        debug( "CChildView::GetValueFromEdit: bail 3: GetWindowText failed\n" );
        return false;
    }

    for ( int index { cbText - 2 }; index >= 0; --index ) {
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
        debug( "CChildView::GetValueFromEdit: bail 4: garbage in number\n" );
        return false;
    }

    result = static_cast<int>( tmp );
    delete[] pwszText;
    return true;
}

bool CChildView::GetValueAndChangedFromEdit( CEdit const& edit, int& nValue, bool& fChanged ) {
    int nOldValue { nValue };

    if ( !GetValueFromEdit( edit, nValue ) ) {
        return false;
    }

    fChanged = nOldValue != nValue;
    return true;
}

void CChildView::PutValueToEdit( CEdit& edit, int const nValue ) const {
    edit.SetWindowTextW( std::to_wstring( nValue ).c_str( ) );
}

void CChildView::SetChannelValue( AllChannels channel, int nValue ) {
    CColorPickerDoc* pDoc { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };

    switch ( channel ) {
        case AllChannels::LabL:
        case AllChannels::LabA:
        case AllChannels::LabB: {
            LabColor color { pDoc->GetLabColor( ) };
            if ( channel == AllChannels::LabL ) {
                nValue = nValue * 100 / 255;
            } else {
                nValue -= 128;
            }
            color.SetChannelValue( channel, static_cast<LabValueT>( nValue ) );
            pDoc->SetColor( color );
            break;
        }

        case AllChannels::SrgbR:
        case AllChannels::SrgbG:
        case AllChannels::SrgbB: {
            SrgbColor color { pDoc->GetSrgbColor( ) };
            color.SetChannelValue( channel, static_cast<SrgbValueT>( nValue ) );
            pDoc->SetColor( color );
            break;
        }

        default:
            break;
    }
}

template<typename T>
bool CChildView::UpdateValueIfEditChanged( CEdit const& edit, T const oldValue, T& newValue ) {
    int n { static_cast<int>( oldValue ) };
    bool fChanged { };

    if ( GetValueAndChangedFromEdit( edit, n, fChanged ) && fChanged ) {
        newValue = static_cast<T>( n );
        return true;
    } else {
        return false;
    }
}

template<typename T>
void CChildView::UpdateEditIfValueChanged( CEdit& edit, T const oldValue, T const newValue ) {
    if ( newValue != oldValue ) {
        PutValueToEdit( edit, static_cast<int>( newValue ) );
    }
}

void CChildView::OnColorValueChange( UINT const uId ) {
    if ( InterlockedExchange( &m_uBusy, 1 ) ) {
        return;
    }

    CColorPickerDoc*    pDoc          { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };
    Triplet<LabValueT>  oldLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    Triplet<SrgbValueT> oldSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };
    Triplet<LabValueT>  newLabValues  {  oldLabValues };
    Triplet<SrgbValueT> newSrgbValues { oldSrgbValues };
    bool fChanged { };

    debug( "CChildView::OnColorValueChange: L*a*b*, before update: (%4d, %4d, %4d)\n",  oldLabValues[ +LabChannels::L],  oldLabValues[ +LabChannels::a],  oldLabValues[ +LabChannels::b] );
    debug( "CChildView::OnColorValueChange: sRGB,   before update: (%4d, %4d, %4d)\n", oldSrgbValues[+SrgbChannels::R], oldSrgbValues[+SrgbChannels::G], oldSrgbValues[+SrgbChannels::B] );

    switch ( uId ) {
        case IDC_LAB_L_VALUE:
        case IDC_LAB_A_VALUE:
        case IDC_LAB_B_VALUE: {
            if ( uId == IDC_LAB_L_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editLabLValue, oldLabValues[+LabChannels::L], newLabValues[+LabChannels::L] ); }
            if ( uId == IDC_LAB_A_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editLabAValue, oldLabValues[+LabChannels::a], newLabValues[+LabChannels::a] ); }
            if ( uId == IDC_LAB_B_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editLabBValue, oldLabValues[+LabChannels::b], newLabValues[+LabChannels::b] ); }
            debug( "CChildView::OnColorValueChange: fChanged: %s\n", fChanged ? "true" : "false" );

            if ( fChanged ) {
                pDoc->SetColor( LabColor { newLabValues[+LabChannels::L], newLabValues[+LabChannels::a], newLabValues[+LabChannels::b] } );

                newSrgbValues = pDoc->GetSrgbColor( ).GetChannelValues( );
                UpdateEditIfValueChanged( m_editSrgbRValue, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] );
                UpdateEditIfValueChanged( m_editSrgbGValue, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] );
                UpdateEditIfValueChanged( m_editSrgbBValue, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
            }
            break;
        }

        case IDC_SRGB_R_VALUE:
        case IDC_SRGB_G_VALUE:
        case IDC_SRGB_B_VALUE: {
            if ( uId == IDC_SRGB_R_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editSrgbRValue, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] ); }
            if ( uId == IDC_SRGB_G_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editSrgbGValue, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] ); }
            if ( uId == IDC_SRGB_B_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editSrgbBValue, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] ); }
            debug( "CChildView::OnColorValueChange: fChanged: %s\n", fChanged ? "true" : "false" );

            if ( fChanged ) {
                pDoc->SetColor( SrgbColor { newSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::B] } );

                newLabValues = pDoc->GetLabColor( ).GetChannelValues( );
                UpdateEditIfValueChanged( m_editLabLValue, oldLabValues[+LabChannels::L], newLabValues[+LabChannels::L] );
                UpdateEditIfValueChanged( m_editLabAValue, oldLabValues[+LabChannels::a], newLabValues[+LabChannels::a] );
                UpdateEditIfValueChanged( m_editLabBValue, oldLabValues[+LabChannels::b], newLabValues[+LabChannels::b] );
            }
            break;
        }
    }

    debug( "CChildView::OnColorValueChange: L*a*b*, after update:  (%4d, %4d, %4d)\n",  newLabValues[ +LabChannels::L],  newLabValues[ +LabChannels::a],  newLabValues[ +LabChannels::b] );
    debug( "CChildView::OnColorValueChange: sRGB,   after update:  (%4d, %4d, %4d)\n", newSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::B] );

    UpdateBitmaps( );

    InterlockedExchange( &m_uBusy, 0 );
}

void CChildView::OnZStripMouseMove( NMHDR* pNotifyStruct, LRESULT* result ) {
    ZSB_MOUSEMOVE* mm { reinterpret_cast<ZSB_MOUSEMOVE*>( pNotifyStruct ) };
    int x { mm->point.x };
    int y { mm->point.y };

    debug( "CChildView::OnZStripMouseMove: point: (%d,%d)\n", x, y );

    CColorPickerDoc*    pDoc          { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };
    Triplet<LabValueT>  oldLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    Triplet<SrgbValueT> oldSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    SetChannelValue( m_channelZ, y );

    Triplet<LabValueT>  newLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    Triplet<SrgbValueT> newSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    InterlockedExchange( &m_uBusy, 1 );
    if (  newLabValues[ +LabChannels::L] !=  oldLabValues[ +LabChannels::L] ) { PutValueToEdit( m_editLabLValue,   newLabValues[ +LabChannels::L] ); }
    if (  newLabValues[ +LabChannels::a] !=  oldLabValues[ +LabChannels::a] ) { PutValueToEdit( m_editLabAValue,   newLabValues[ +LabChannels::a] ); }
    if (  newLabValues[ +LabChannels::b] !=  oldLabValues[ +LabChannels::b] ) { PutValueToEdit( m_editLabBValue,   newLabValues[ +LabChannels::b] ); }
    if ( newSrgbValues[+SrgbChannels::R] != oldSrgbValues[+SrgbChannels::R] ) { PutValueToEdit( m_editSrgbRValue, newSrgbValues[+SrgbChannels::R] ); }
    if ( newSrgbValues[+SrgbChannels::G] != oldSrgbValues[+SrgbChannels::G] ) { PutValueToEdit( m_editSrgbGValue, newSrgbValues[+SrgbChannels::G] ); }
    if ( newSrgbValues[+SrgbChannels::B] != oldSrgbValues[+SrgbChannels::B] ) { PutValueToEdit( m_editSrgbBValue, newSrgbValues[+SrgbChannels::B] ); }
    InterlockedExchange( &m_uBusy, 0 );

    UpdateBitmaps( );

    *result = 0;
}

void CChildView::OnXyGridMouseMove( NMHDR* pNotifyStruct, LRESULT* result ) {
    ZSB_MOUSEMOVE* mm { reinterpret_cast<ZSB_MOUSEMOVE*>( pNotifyStruct ) };
    int x { mm->point.x };
    int y { mm->point.y };

    debug( "CChildView::OnXyGridMouseMove: point: (%d,%d)\n", x, y );

    CColorPickerDoc*    pDoc          { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };
    Triplet<LabValueT>  oldLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    Triplet<SrgbValueT> oldSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    SetChannelValue( m_channelX, x );
    SetChannelValue( m_channelY, y );

    Triplet<LabValueT>  newLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    Triplet<SrgbValueT> newSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    InterlockedExchange( &m_uBusy, 1 );
    if (  newLabValues[ +LabChannels::L] !=  oldLabValues[ +LabChannels::L] ) { PutValueToEdit( m_editLabLValue,   newLabValues[ +LabChannels::L] ); }
    if (  newLabValues[ +LabChannels::a] !=  oldLabValues[ +LabChannels::a] ) { PutValueToEdit( m_editLabAValue,   newLabValues[ +LabChannels::a] ); }
    if (  newLabValues[ +LabChannels::b] !=  oldLabValues[ +LabChannels::b] ) { PutValueToEdit( m_editLabBValue,   newLabValues[ +LabChannels::b] ); }
    if ( newSrgbValues[+SrgbChannels::R] != oldSrgbValues[+SrgbChannels::R] ) { PutValueToEdit( m_editSrgbRValue, newSrgbValues[+SrgbChannels::R] ); }
    if ( newSrgbValues[+SrgbChannels::G] != oldSrgbValues[+SrgbChannels::G] ) { PutValueToEdit( m_editSrgbGValue, newSrgbValues[+SrgbChannels::G] ); }
    if ( newSrgbValues[+SrgbChannels::B] != oldSrgbValues[+SrgbChannels::B] ) { PutValueToEdit( m_editSrgbBValue, newSrgbValues[+SrgbChannels::B] ); }
    InterlockedExchange( &m_uBusy, 0 );

    UpdateBitmaps( );

    *result = 0;
}
