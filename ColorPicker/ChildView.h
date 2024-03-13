#pragma once

#include "XyGrid.h"
#include "ZStrip.h"

class CXyGrid;
class CZStrip;

class CChildView:
    public CFormView
{

    DECLARE_DYNCREATE( CChildView )
    DECLARE_MESSAGE_MAP( )

public:

#if defined AFX_DESIGN_TIME
    enum { IDD = IDD_CHILDVIEW_2 };
#endif

protected:

    CChildView( ): CFormView { IDD_CHILDVIEW_2 } { /*empty*/ }

    virtual ~CChildView( ) { /*empty*/ }

protected:

    //============================================
    // Methods
    //============================================

    //
    // Class methods
    //

    void _AdjustPosition( CWnd* ctrl, SIZE const& adjust );
    void _AdjustSize( CWnd* ctrl, SIZE const& adjust );
    void _SetPosition( CWnd* ctrl, POINT const& pt );
    void _SetSize( CWnd* ctrl, SIZE const& size );

    //
    // Overrides
    //

    virtual void DoDataExchange( CDataExchange* pDX ) override;
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs )  override;
    virtual void OnInitialUpdate( )                   override;

    //
    // Event handlers
    //

    //afx_msg void OnPaint( );
    afx_msg void OnCloseButtonClicked( );

    //============================================
    // Member variables
    //============================================

    //
    // Controls
    //

    CStatic  m_groupBoxLab;
    CButton  m_buttonLabLChannel;
    CButton  m_buttonLabAChannel;
    CButton  m_buttonLabBChannel;
    CEdit    m_editLabLValue;
    CEdit    m_editLabAValue;
    CEdit    m_editLabBValue;

    CStatic  m_groupBoxSrgb;
    CButton  m_buttonSrgbRChannel;
    CButton  m_buttonSrgbGChannel;
    CButton  m_buttonSrgbBChannel;
    CEdit    m_editSrgbRValue;
    CEdit    m_editSrgbGValue;
    CEdit    m_editSrgbBValue;

    CStatic  m_staticXyGrid;
    CStatic  m_staticZStrip;

    CButton  m_buttonClose;

    //
    // Values
    //

    int      m_nLabLValue  { };
    int      m_nLabAValue  { };
    int      m_nLabBValue  { };

    int      m_nSrgbRValue { };
    int      m_nSrgbGValue { };
    int      m_nSrgbBValue { };

    //
    // Control arrays
    //

    // Lab color

    CButton* m_buttonLabChannels[3]  { &m_buttonLabLChannel,  &m_buttonLabAChannel,  &m_buttonLabBChannel  };
    CEdit*   m_editLabValues[3]      { &m_editLabLValue,      &m_editLabAValue,      &m_editLabBValue      };

    int*     m_nLabValues[3]         { &m_nLabLValue,         &m_nLabAValue,         &m_nLabBValue         };

    // sRGB color

    CButton* m_buttonSrgbChannels[3] { &m_buttonSrgbRChannel, &m_buttonSrgbGChannel, &m_buttonSrgbBChannel };
    CEdit*   m_editSrgbValues[3]     { &m_editSrgbRValue,     &m_editSrgbGValue,     &m_editSrgbBValue     };

    int*     m_nSrgbValues[3]        { &m_nSrgbRValue,        &m_nSrgbGValue,        &m_nSrgbBValue        };

    // All

    CButton* m_buttonAllChannels[6]  { &m_buttonLabLChannel,  &m_buttonLabAChannel,  &m_buttonLabBChannel,
                                       &m_buttonSrgbRChannel, &m_buttonSrgbGChannel, &m_buttonSrgbBChannel };
    CEdit*   m_editAllValues[6]      { &m_editLabLValue,      &m_editLabAValue,      &m_editLabBValue,
                                       &m_editSrgbRValue,     &m_editSrgbGValue,     &m_editSrgbBValue     };

    int*     m_nAllValues[6]         { &m_nLabLValue,         &m_nLabAValue,         &m_nLabBValue,
                                       &m_nSrgbRValue,        &m_nSrgbGValue,        &m_nSrgbBValue        };

    //
    // Channel mapping
    //

    int      m_nXChannel { 0 };
    int      m_nYChannel { 1 };
    int      m_nZChannel { 2 };

    //
    // Bitmaps
    //

    CBitmap  m_bitmapXyGrid;
    CBitmap  m_bitmapZStrip;

    //
    // Bitmap generators
    //

    CXyGrid* m_pXyGrid { };
    CZStrip* m_pZStrip { };

};
