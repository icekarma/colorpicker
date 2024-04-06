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
    ON_CONTROL_RANGE( EN_UPDATE,  IDC_LAB_L_VALUE,   IDC_SRGB_B_VALUE,   &CChildView::OnColorValueUpdate     )
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

    DDX_Control( pDX, IDC_GROUPBOX_LAB,    m_groupBoxLab        );
    DDX_Control( pDX, IDC_LAB_L_CHANNEL,   m_buttonLabLChannel  );
    DDX_Control( pDX, IDC_LAB_A_CHANNEL,   m_buttonLabAChannel  );
    DDX_Control( pDX, IDC_LAB_B_CHANNEL,   m_buttonLabBChannel  );
    DDX_Control( pDX, IDC_LAB_L_VALUE,     m_editLabLValue      );
    DDX_Control( pDX, IDC_LAB_A_VALUE,     m_editLabAValue      );
    DDX_Control( pDX, IDC_LAB_B_VALUE,     m_editLabBValue      );

    DDX_Control( pDX, IDC_GROUPBOX_SRGB,   m_groupBoxSrgb       );
    DDX_Control( pDX, IDC_SRGB_R_CHANNEL,  m_buttonSrgbRChannel );
    DDX_Control( pDX, IDC_SRGB_G_CHANNEL,  m_buttonSrgbGChannel );
    DDX_Control( pDX, IDC_SRGB_B_CHANNEL,  m_buttonSrgbBChannel );
    DDX_Control( pDX, IDC_SRGB_R_VALUE,    m_editSrgbRValue     );
    DDX_Control( pDX, IDC_SRGB_G_VALUE,    m_editSrgbGValue     );
    DDX_Control( pDX, IDC_SRGB_B_VALUE,    m_editSrgbBValue     );

    DDX_Control( pDX, IDC_LABEL_HEX_COLOR, m_labelHexColor      );
    DDX_Control( pDX, IDC_EDIT_HEX_COLOR,  m_editHexColor       );

    DDX_Control( pDX, IDC_SWATCH,          m_staticSwatch       );
    DDX_Control( pDX, IDC_Z_STRIP,         m_staticZStrip       );
    DDX_Control( pDX, IDC_XY_GRID,         m_staticXyGrid       );

    DDX_Control( pDX, IDCLOSE,             m_buttonClose        );
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

    SIZE constexpr adjustUp2 { 0, -2 };
    SIZE constexpr adjustUp1 { 0, -1 };
    SIZE constexpr adjustDn1 { 0,  1 };
    SIZE constexpr adjustDn2 { 0,  2 };

    _AdjustSize    ( &m_groupBoxLab,        adjustUp1    );
    _AdjustPosition( &m_buttonLabLChannel,  adjustDn1    );
    _AdjustPosition( &m_buttonLabAChannel,  adjustUp1    );
    _AdjustPosition( &m_buttonLabBChannel,  adjustUp2    );
    // m_editLabLValue
    // m_editLabAValue
    _AdjustPosition( &m_editLabBValue,      adjustUp2    );

    _AdjustPosition( &m_groupBoxSrgb,       adjustDn2    );
    _AdjustSize    ( &m_groupBoxSrgb,       adjustUp1    );
    _AdjustPosition( &m_buttonSrgbRChannel, adjustDn2    );
    // m_buttonSrgbGChannel
    _AdjustPosition( &m_buttonSrgbBChannel, adjustUp2    );
    _AdjustPosition( &m_editSrgbRValue,     adjustDn2    );
    _AdjustPosition( &m_editSrgbGValue,     adjustDn2    );
    _AdjustPosition( &m_editSrgbBValue,     adjustUp1    );

    _AdjustPosition( &m_editHexColor,       {   1,  -1 } );

    _AdjustSize    ( &m_staticSwatch,       adjustUp1    );

    _SetSize       ( &m_staticZStrip,       {  20, 256 } );

    _AdjustPosition( &m_staticXyGrid,       {  -1,   0 } );
    _SetSize       ( &m_staticXyGrid,       { 256, 256 } );

    _AdjustPosition( &m_buttonClose,        {  -1,  -2 } );

    m_buttonLabLChannel.SetCheck( BST_CHECKED );

    m_pSwatch = new CSwatch { pDoc, &m_staticSwatch };
    m_pSwatch->Update( );

    m_pZStrip = new CZStrip { pDoc, &m_staticZStrip };
    m_pZStrip->SetChannel( m_channelZ );
    m_pZStrip->Update( );

    m_pXyGrid = new CXyGrid { pDoc, &m_staticXyGrid };
    m_pXyGrid->SetChannels( m_channelX, m_channelY, m_channelZ );
    m_pXyGrid->Update( );

    LabTriplet  labValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet srgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    PutValueToEdit   ( m_editLabLValue,   labValues[ +LabChannels::L] );
    PutValueToEdit   ( m_editLabAValue,   labValues[ +LabChannels::a] );
    PutValueToEdit   ( m_editLabBValue,   labValues[ +LabChannels::b] );
    PutValueToEdit   ( m_editSrgbRValue, srgbValues[+SrgbChannels::R] );
    PutValueToEdit   ( m_editSrgbGValue, srgbValues[+SrgbChannels::G] );
    PutValueToEdit   ( m_editSrgbBValue, srgbValues[+SrgbChannels::B] );
    PutHexColorToEdit( m_editHexColor,   srgbValues );
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

wchar_t* CChildView::SafeGetWindowText( CEdit const& edit ) {
    int cbText { edit.GetWindowTextLength( ) };
    if ( cbText < 1 ) {
        debug( "CChildView::SafeGetWindowText: bail 1: no text in control\n" );
        return nullptr;
    }
    ++cbText;

    wchar_t* pwszText { new wchar_t[cbText + 1] { } };
    if ( !pwszText ) {
        debug( "CChildView::SafeGetWindowText: bail 2: memory allocation failure\n" );
        return nullptr;
    }
    if ( edit.GetWindowText( pwszText, cbText ) < 1 ) {
        delete[] pwszText;
        debug( "CChildView::SafeGetWindowText: bail 3: GetWindowText failed\n" );
        return nullptr;
    }

    for ( int index { cbText - 2 }; index >= 0; --index ) {
        if ( !iswspace( pwszText[index] ) ) {
            break;
        }
        pwszText[index] = '\0';
    }

    return pwszText;
}

bool CChildView::GetValueFromEdit( CEdit const& edit, int& result ) {
    wchar_t* pwszText { SafeGetWindowText( edit ) };
    if ( !pwszText ) {
        debug( "CChildView::GetValueFromEdit: bail 1: SafeGetWindowText returned nullptr\n" );
        return false;
    }

    wchar_t* pwszEnd { };
    long tmp = wcstol( pwszText, &pwszEnd, 10 );
    if ( !pwszEnd || *pwszEnd || ( tmp < static_cast<long>( INT_MIN ) ) || ( tmp > static_cast<long>( INT_MAX ) ) ) {
        delete[] pwszText;
        debug( "CChildView::GetValueFromEdit: bail 2: garbage in number\n" );
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
    CString str;

    str.Format( L"%d", nValue );
    edit.SetWindowText( str );
}

void CChildView::PutHexColorToEdit( CEdit& edit, SrgbTriplet const& values ) const {
    unsigned r { values[+SrgbChannels::R] };
    unsigned g { values[+SrgbChannels::G] };
    unsigned b { values[+SrgbChannels::B] };

    CString str;
    str.Format( L"%06.6X", static_cast<int>( ( r << 16u ) | ( g << 8u ) | b ) );
    edit.SetWindowText( str );
}

template<typename T>
[[nodiscard]] bool CChildView::UpdateValueIfEditChanged( CEdit const& edit, T const oldValue, T& newValue ) {
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

void CChildView::OnColorValueUpdate( UINT const uId ) {
    debug( "CChildView::OnColorValueUpdate(uId=%u):\n", uId );

    CColorPickerDoc* pDoc          { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };
    LabTriplet       oldLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet      oldSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };
    LabTriplet       newLabValues  {  oldLabValues };
    SrgbTriplet      newSrgbValues { oldSrgbValues };
    bool             fChanged      { };

    //debug( "CChildView::OnColorValueUpdate: L*a*b*, before update: (%4d, %4d, %4d)\n",  oldLabValues[ +LabChannels::L],  oldLabValues[ +LabChannels::a],  oldLabValues[ +LabChannels::b] );
    //debug( "CChildView::OnColorValueUpdate: sRGB,   before update: (%4d, %4d, %4d)\n", oldSrgbValues[+SrgbChannels::R], oldSrgbValues[+SrgbChannels::G], oldSrgbValues[+SrgbChannels::B] );

    switch ( uId ) {
        case IDC_LAB_L_VALUE:
        case IDC_LAB_A_VALUE:
        case IDC_LAB_B_VALUE: {
            if ( uId == IDC_LAB_L_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editLabLValue, oldLabValues[+LabChannels::L], newLabValues[+LabChannels::L] ); }
            if ( uId == IDC_LAB_A_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editLabAValue, oldLabValues[+LabChannels::a], newLabValues[+LabChannels::a] ); }
            if ( uId == IDC_LAB_B_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editLabBValue, oldLabValues[+LabChannels::b], newLabValues[+LabChannels::b] ); }
            //debug( "CChildView::OnColorValueUpdate: fChanged: %s\n", fChanged ? "true" : "false" );

            if ( fChanged ) {
                pDoc->SetColor( LabColor { newLabValues[+LabChannels::L], newLabValues[+LabChannels::a], newLabValues[+LabChannels::b] } );

                newSrgbValues = pDoc->GetSrgbColor( ).GetChannelValues( );
                UpdateEditIfValueChanged( m_editSrgbRValue, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] );
                UpdateEditIfValueChanged( m_editSrgbGValue, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] );
                UpdateEditIfValueChanged( m_editSrgbBValue, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
                PutHexColorToEdit( m_editHexColor, newSrgbValues );
            }
            break;
        }

        case IDC_SRGB_R_VALUE:
        case IDC_SRGB_G_VALUE:
        case IDC_SRGB_B_VALUE: {
            if ( uId == IDC_SRGB_R_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editSrgbRValue, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] ); }
            if ( uId == IDC_SRGB_G_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editSrgbGValue, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] ); }
            if ( uId == IDC_SRGB_B_VALUE ) { fChanged = UpdateValueIfEditChanged( m_editSrgbBValue, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] ); }
            //debug( "CChildView::OnColorValueUpdate: fChanged: %s\n", fChanged ? "true" : "false" );

            if ( fChanged ) {
                pDoc->SetColor( SrgbColor { newSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::B] } );

                newLabValues = pDoc->GetLabColor( ).GetChannelValues( );
                UpdateEditIfValueChanged( m_editLabLValue, oldLabValues[+LabChannels::L], newLabValues[+LabChannels::L] );
                UpdateEditIfValueChanged( m_editLabAValue, oldLabValues[+LabChannels::a], newLabValues[+LabChannels::a] );
                UpdateEditIfValueChanged( m_editLabBValue, oldLabValues[+LabChannels::b], newLabValues[+LabChannels::b] );
                PutHexColorToEdit( m_editHexColor, newSrgbValues );
            }
            break;
        }
    }

    //debug( "CChildView::OnColorValueUpdate: L*a*b*, after update:  (%4d, %4d, %4d)\n",  newLabValues[ +LabChannels::L],  newLabValues[ +LabChannels::a],  newLabValues[ +LabChannels::b] );
    //debug( "CChildView::OnColorValueUpdate: sRGB,   after update:  (%4d, %4d, %4d)\n", newSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::B] );

    UpdateBitmaps( );
}

void CChildView::OnZStripMouseMove( NMHDR* pNotifyStruct, LRESULT* result ) {
    ZSB_MOUSEMOVE* mm { static_cast<ZSB_MOUSEMOVE*>( pNotifyStruct ) };
    int x { mm->point.x };
    int y { mm->point.y };

    debug( "CChildView::OnZStripMouseMove: point: (%d,%d)\n", x, y );

    CColorPickerDoc* pDoc          { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };
    LabTriplet       oldLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet      oldSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    pDoc->SetChannelValue( m_channelZ, y );

    LabTriplet       newLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet      newSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    UpdateEditIfValueChanged(  m_editLabLValue,  oldLabValues[ +LabChannels::L],  newLabValues[ +LabChannels::L] );
    UpdateEditIfValueChanged(  m_editLabAValue,  oldLabValues[ +LabChannels::a],  newLabValues[ +LabChannels::a] );
    UpdateEditIfValueChanged(  m_editLabBValue,  oldLabValues[ +LabChannels::b],  newLabValues[ +LabChannels::b] );
    UpdateEditIfValueChanged( m_editSrgbRValue, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] );
    UpdateEditIfValueChanged( m_editSrgbGValue, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] );
    UpdateEditIfValueChanged( m_editSrgbBValue, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
    if ( oldSrgbValues != newSrgbValues ) {
        PutHexColorToEdit( m_editHexColor, newSrgbValues );
    }

    UpdateBitmaps( );

    *result = 0;
}

void CChildView::OnXyGridMouseMove( NMHDR* pNotifyStruct, LRESULT* result ) {
    ZSB_MOUSEMOVE* mm { static_cast<ZSB_MOUSEMOVE*>( pNotifyStruct ) };
    int x { mm->point.x };
    int y { mm->point.y };

    debug( "CChildView::OnXyGridMouseMove: point: (%d,%d)\n", x, y );

    CColorPickerDoc* pDoc          { dynamic_downcast<CColorPickerDoc>( GetDocument( ) ) };
    LabTriplet       oldLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet      oldSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    pDoc->SetChannelValue( m_channelX, x );
    pDoc->SetChannelValue( m_channelY, y );

    LabTriplet       newLabValues  { pDoc-> GetLabColor( ).GetChannelValues( ) };
    SrgbTriplet      newSrgbValues { pDoc->GetSrgbColor( ).GetChannelValues( ) };

    UpdateEditIfValueChanged(  m_editLabLValue,  oldLabValues[ +LabChannels::L],  newLabValues[ +LabChannels::L] );
    UpdateEditIfValueChanged(  m_editLabAValue,  oldLabValues[ +LabChannels::a],  newLabValues[ +LabChannels::a] );
    UpdateEditIfValueChanged(  m_editLabBValue,  oldLabValues[ +LabChannels::b],  newLabValues[ +LabChannels::b] );
    UpdateEditIfValueChanged( m_editSrgbRValue, oldSrgbValues[+SrgbChannels::R], newSrgbValues[+SrgbChannels::R] );
    UpdateEditIfValueChanged( m_editSrgbGValue, oldSrgbValues[+SrgbChannels::G], newSrgbValues[+SrgbChannels::G] );
    UpdateEditIfValueChanged( m_editSrgbBValue, oldSrgbValues[+SrgbChannels::B], newSrgbValues[+SrgbChannels::B] );
    if ( oldSrgbValues != newSrgbValues ) {
        PutHexColorToEdit( m_editHexColor, newSrgbValues );
    }

    UpdateBitmaps( );

    *result = 0;
}
