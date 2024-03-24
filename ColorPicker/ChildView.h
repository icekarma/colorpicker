#pragma once

#include "XyGrid.h"
#include "ZStrip.h"
#include "Swatch.h"
#include "StaticBitmap.h"

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

        if ( m_pSwatch ) {
            delete m_pSwatch;
            m_pSwatch = nullptr;
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
    afx_msg LRESULT OnZsbnMouseMove( WPARAM wParam, LPARAM lParam );
    afx_msg void OnZStripMouseMove( int x, int y );
    afx_msg void OnXyGridMouseMove( int x, int y );

    //
    // Own methods
    //

    void UpdateBitmaps( );
    bool GetValueFromEdit( CEdit const& edit, int& result );
    bool GetValueAndChangeStateFromEdit( CEdit const& edit, int& nValue, bool& fChanged );
    void PutValueToEdit( CEdit& edit, int const value ) const;
    void SetChannelValue( AllChannels channel, int value );

    //============================================
    // Member variables
    //============================================

    volatile unsigned m_uBusy { };

    //
    // Controls
    //

    CStatic       m_groupBoxLab;
    CButton       m_buttonLabLChannel;
    CButton       m_buttonLabAChannel;
    CButton       m_buttonLabBChannel;
    CEdit         m_editLabLValue;
    CEdit         m_editLabAValue;
    CEdit         m_editLabBValue;

    CStatic       m_groupBoxSrgb;
    CButton       m_buttonSrgbRChannel;
    CButton       m_buttonSrgbGChannel;
    CButton       m_buttonSrgbBChannel;
    CEdit         m_editSrgbRValue;
    CEdit         m_editSrgbGValue;
    CEdit         m_editSrgbBValue;

    CStaticBitmap m_staticXyGrid;
    CStaticBitmap m_staticZStrip;
    CStatic       m_staticSwatch;

    CButton       m_buttonClose;

    //
    // Values
    //

    int           m_nSelectedChannel     { };

    //
    // Bitmaps
    //

    CBitmap       m_bitmapXyGrid;
    CBitmap       m_bitmapZStrip;
    CBitmap       m_bitmapSwatch;

    //
    // Bitmap generators
    //

    CXyGrid*      m_pXyGrid              { };
    CZStrip*      m_pZStrip              { };
    CSwatch*      m_pSwatch              { };

    //
    // Channel mapping
    //

    AllChannels   m_channelX             { AllChannels::LabA };
    AllChannels   m_channelY             { AllChannels::LabB };
    AllChannels   m_channelZ             { AllChannels::LabL };

};
