#pragma once

#include "XyGrid.h"
#include "ZStrip.h"

class CChildView:
    public CFormView
{

    DECLARE_DYNCREATE( CChildView )
    DECLARE_MESSAGE_MAP( )

public:

#if defined AFX_DESIGN_TIME
    enum { IDD = IDD_CHILDVIEW };
#endif

protected:

    CChildView( ):
        CFormView { IDD_CHILDVIEW }
    {
        /*empty*/
    }

    virtual ~CChildView( ) {
        if ( m_pXyGrid ) {
            delete m_pXyGrid;
            m_pXyGrid = nullptr;
        }

        if ( m_pZStrip ) {
            delete m_pZStrip;
            m_pZStrip = nullptr;
        }
    }

    //============================================
    // Methods
    //============================================

    //
    // Overrides
    //

    virtual void DoDataExchange( CDataExchange* pDX ) override;
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs )  override;
    virtual void OnInitialUpdate( )                   override;

    //
    // Event handlers
    //

    afx_msg void OnCloseButtonClicked( );
    afx_msg void OnChannelButtonClicked( UINT uId );
    afx_msg void OnColorValueChange( UINT uId );
    afx_msg void OnZStripClicked( );
    afx_msg void OnXyGridClicked( );

    //
    // Own methods
    //

    void UpdateBitmaps( );
    void FetchLab( );
    void FetchSrgb( );
    void SetChannelValue( AllChannels channel, int value );

    //============================================
    // Member variables
    //============================================

    //
    // Controls
    //

    CStatic     m_groupBoxLab;
    CButton     m_buttonLabLChannel;
    CButton     m_buttonLabAChannel;
    CButton     m_buttonLabBChannel;
    CEdit       m_editLabLValue;
    CEdit       m_editLabAValue;
    CEdit       m_editLabBValue;

    CStatic     m_groupBoxSrgb;
    CButton     m_buttonSrgbRChannel;
    CButton     m_buttonSrgbGChannel;
    CButton     m_buttonSrgbBChannel;
    CEdit       m_editSrgbRValue;
    CEdit       m_editSrgbGValue;
    CEdit       m_editSrgbBValue;

    CStatic     m_staticXyGrid;
    CStatic     m_staticZStrip;

    CButton     m_buttonClose;

    //
    // Values
    //

    int         m_nSelectedChannel { };

    int         m_nLabLValue       { };
    int         m_nLabAValue       { };
    int         m_nLabBValue       { };

    int         m_nSrgbRValue      { };
    int         m_nSrgbGValue      { };
    int         m_nSrgbBValue      { };

    //
    // Control arrays
    //

    // Lab color

    CButton*    m_buttonLabChannels[3]  { &m_buttonLabLChannel,  &m_buttonLabAChannel,  &m_buttonLabBChannel  };
    CEdit*      m_editLabValues[3]      { &m_editLabLValue,      &m_editLabAValue,      &m_editLabBValue      };
    int*        m_nLabValues[3]         { &m_nLabLValue,         &m_nLabAValue,         &m_nLabBValue         };

    // sRGB color

    CButton*    m_buttonSrgbChannels[3] { &m_buttonSrgbRChannel, &m_buttonSrgbGChannel, &m_buttonSrgbBChannel };
    CEdit*      m_editSrgbValues[3]     { &m_editSrgbRValue,     &m_editSrgbGValue,     &m_editSrgbBValue     };
    int*        m_nSrgbValues[3]        { &m_nSrgbRValue,        &m_nSrgbGValue,        &m_nSrgbBValue        };

    // All

    CButton*    m_buttonAllChannels[6]  { &m_buttonLabLChannel,  &m_buttonLabAChannel,  &m_buttonLabBChannel,
                                          &m_buttonSrgbRChannel, &m_buttonSrgbGChannel, &m_buttonSrgbBChannel };
    CEdit*      m_editAllValues[6]      { &m_editLabLValue,      &m_editLabAValue,      &m_editLabBValue,
                                          &m_editSrgbRValue,     &m_editSrgbGValue,     &m_editSrgbBValue     };
    int*        m_nAllValues[6]         { &m_nLabLValue,         &m_nLabAValue,         &m_nLabBValue,
                                          &m_nSrgbRValue,        &m_nSrgbGValue,        &m_nSrgbBValue        };

    //
    // Channel mapping
    //

    AllChannels m_channelX { AllChannels::LabA };
    AllChannels m_channelY { AllChannels::LabB };
    AllChannels m_channelZ { AllChannels::LabL };

    //
    // Bitmaps
    //

    CBitmap     m_bitmapXyGrid;
    CBitmap     m_bitmapZStrip;

    //
    // Bitmap generators
    //

    CXyGrid*    m_pXyGrid { };
    CZStrip*    m_pZStrip { };

};
