#pragma once

#include "XyGrid.h"
#include "ZStrip.h"
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

    virtual ~CChildView( ) override = default;

    //============================================
    // Methods
    //============================================

    //
    // Own methods
    //

    CEdit* MapControlIdToPointer( UINT const uId );
    void UpdateBitmaps( bool const fUpdateZ, bool const fUpdateXy );

    void UpdateBitmaps( ) {
        UpdateBitmaps( true, true );
    }

    //
    // Overrides
    //

    virtual void DoDataExchange( CDataExchange* pDX ) override;
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs )  override;
    virtual void OnInitialUpdate( )                   override;

    //
    // Event handlers
    //

    afx_msg void OnUpdateEditCut( CCmdUI* pCmdUI );
    afx_msg void OnUpdateEditCopy( CCmdUI* pCmdUI );
    afx_msg void OnUpdateEditPaste( CCmdUI* pCmdUI );
    afx_msg void OnUpdateEditClear( CCmdUI* pCmdUI );
    afx_msg void OnUpdateEditUndo( CCmdUI* pCmdUI );
    afx_msg void OnUpdateEditSelectAll( CCmdUI* pCmdUI );
    afx_msg void OnUpdateViewInvert( CCmdUI* pCmdUI );

    afx_msg void OnEditCut( );
    afx_msg void OnEditCopy( );
    afx_msg void OnEditPaste( );
    afx_msg void OnEditClear( );
    afx_msg void OnEditUndo( );
    afx_msg void OnEditSelectAll( );
    afx_msg void OnViewInvert( );

    afx_msg void OnEditGotFocus( UINT uId );
    afx_msg void OnEditLostFocus( UINT uId );
    afx_msg void OnCloseButtonClicked( );
    afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
    afx_msg void OnChannelButtonClicked( UINT uId );
    afx_msg void OnColorValueUpdate( UINT uId );
    afx_msg void OnHexColorUpdate( );
    afx_msg void OnZStripMouseMove( NMHDR* pNotifyStruct, LRESULT* result );
    afx_msg void OnXyGridMouseMove( NMHDR* pNotifyStruct, LRESULT* result );

    //============================================
    // Member variables
    //============================================

    std::unordered_map<AllChannels, CButton*> m_mapRadioButtons;
    std::unordered_map<unsigned,    CEdit*>   m_mapEditControls;

    CColorPickerDoc* m_pDoc                { };
    CEdit*           m_pCurrentEdit        { };
    bool             m_fBlockBitmapUpdates { };

    //
    // Controls
    //

    CButton       m_groupBoxLab;
    CButton       m_radioLabL;
    CButton       m_radioLabA;
    CButton       m_radioLabB;
    CEdit         m_editLabL;
    CEdit         m_editLabA;
    CEdit         m_editLabB;

    CButton       m_groupBoxSrgb;
    CButton       m_radioSrgbR;
    CButton       m_radioSrgbG;
    CButton       m_radioSrgbB;
    CEdit         m_editSrgbR;
    CEdit         m_editSrgbG;
    CEdit         m_editSrgbB;

    CStatic       m_labelHexColor;
    CEdit         m_editHexColor;

    CStaticBrush  m_staticSwatch;
    CZStrip       m_staticZStrip;
    CXyGrid       m_staticXyGrid;

    CButton       m_buttonClose;

    //
    // Channel mapping
    //

    AllChannels   m_channelX { AllChannels::LabA };
    AllChannels   m_channelY { AllChannels::LabB };
    AllChannels   m_channelZ { AllChannels::LabL };

};
