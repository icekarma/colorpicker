#pragma once

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

    void _AdjustPosition( CWnd* ctrl, SIZE const& adjust );
    void _AdjustSize( CWnd* ctrl, SIZE const& adjust );

    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
    virtual void OnInitialUpdate( );

    afx_msg void OnPaint( );
    afx_msg void OnCloseButtonClicked( );

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

    CStatic  m_XyGrid;
    CStatic  m_ZStrip;

    CButton  m_buttonClose;

    int      m_nLabLValue  { };
    int      m_nLabAValue  { };
    int      m_nLabBValue  { };

    int      m_nSrgbRValue { };
    int      m_nSrgbGValue { };
    int      m_nSrgbBValue { };

    CButton* m_buttonLabChannels[3]  { &m_buttonLabLChannel,  &m_buttonLabAChannel,  &m_buttonLabBChannel  };
    CEdit*   m_editLabValues[3]      { &m_editLabLValue,      &m_editLabAValue,      &m_editLabBValue      };

    int*     m_nLabValues[3]         { &m_nLabLValue,         &m_nLabAValue,         &m_nLabBValue         };

    CButton* m_buttonSrgbChannels[3] { &m_buttonSrgbRChannel, &m_buttonSrgbGChannel, &m_buttonSrgbBChannel };
    CEdit*   m_editSrgbValues[3]     { &m_editSrgbRValue,     &m_editSrgbGValue,     &m_editSrgbBValue     };

    int*     m_nSrgbValues[3]        { &m_nSrgbRValue,        &m_nSrgbGValue,        &m_nSrgbBValue        };

};
