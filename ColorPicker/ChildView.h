#pragma once

#include "XyGrid.h"
#include "ZStrip.h"
#include "Swatch.h"
#include "StaticBitmap.h"
#include "StaticBrush.h"

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
        if ( m_pSwatch ) {
            delete m_pSwatch;
            m_pSwatch = nullptr;
        }

        if ( m_pZStrip ) {
            delete m_pZStrip;
            m_pZStrip = nullptr;
        }

        if ( m_pXyGrid ) {
            delete m_pXyGrid;
            m_pXyGrid = nullptr;
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
    afx_msg void OnZStripMouseMove( NMHDR* pNotifyStruct, LRESULT* result );
    afx_msg void OnXyGridMouseMove( NMHDR* pNotifyStruct, LRESULT* result );

    //
    // Own methods
    //

    void UpdateBitmaps( );
    bool GetValueFromEdit( CEdit const& edit, int& result );
    bool GetValueAndChangedFromEdit( CEdit const& edit, int& nValue, bool& fChanged );
    void PutValueToEdit( CEdit& edit, int const nValue ) const;
    template<typename T>
    bool UpdateValueIfEditChanged( CEdit const& edit, T const oldValue, T& newValue );
    template<typename T>
    void UpdateEditIfValueChanged( CEdit& edit, T const oldValue, T const newValue );

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

    CStaticBrush  m_staticSwatch;
    CStaticBitmap m_staticZStrip;
    CStaticBitmap m_staticXyGrid;

    CButton       m_buttonClose;

    //
    // Bitmap generators
    //

    CSwatch*      m_pSwatch              { };
    CZStrip*      m_pZStrip              { };
    CXyGrid*      m_pXyGrid              { };

    //
    // Channel mapping
    //

    AllChannels   m_channelX             { AllChannels::LabA };
    AllChannels   m_channelY             { AllChannels::LabB };
    AllChannels   m_channelZ             { AllChannels::LabL };

};
