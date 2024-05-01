#pragma once

#include "ColorPickerDoc.h"
#include "StaticBitmap.h"
#include "StaticBrush.h"
#include "XyGrid.h"
#include "ZStrip.h"

extern std::unordered_map<UINT, AllChannels> const g_mapLabelControlIdToChannel;
extern std::unordered_map<UINT, AllChannels> const g_mapValueControlIdToChannel;

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

    CChildView( );
    virtual ~CChildView( ) override;

    //============================================
    // Methods
    //============================================

    //
    // Static methods
    //

    LRESULT static _EditWndProc( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam );

    //
    // Own methods
    //

    LRESULT EditWndProc( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam );

    CButton* MapChannelToButtonControl( AllChannels const channel ) const {
        return ( channel <= AllChannels::Max ) ? m_mapChannelToButtonControl[+channel] : nullptr;
    }

    AllChannels MapHwndToChannel( HWND const hwnd ) const {
        return _MapImpl( m_mapHwndToChannel, hwnd, AllChannels::unknown );
    }

    WNDPROC MapHwndToWndProc( HWND const hwnd ) const {
        return _MapImpl( m_mapHwndToWndProc, hwnd );
    }

    AllChannels MapLabelControlIdToChannel( UINT const uId ) const {
        return _MapImpl( g_mapLabelControlIdToChannel, uId, AllChannels::unknown );
    }

    AllChannels MapValueControlIdToChannel( UINT const uId ) const {
        return _MapImpl( g_mapValueControlIdToChannel, uId, AllChannels::unknown );
    }

    CEdit* MapValueControlIdToEditControl( UINT const uId ) const {
        return _MapImpl( m_mapValueControlIdToEditControl, uId );
    }

    void UpdateBitmaps( ) {
        UpdateBitmaps( true, true );
    }

    void UpdateBitmaps( bool const fUpdateZ, bool const fUpdateXy );

    void SubclassEditControl( CEdit& pEdit, WNDPROC const wndProc );
    void UnSubclassEditControl( CEdit& pEdit );

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

    afx_msg void OnValueEditGotFocus( UINT uId );
    afx_msg void OnValueEditLostFocus( UINT uId );
    afx_msg void OnCloseButtonClicked( );
    afx_msg bool EditControl_OnKeyDown( AllChannels const channel, UINT const nChar, UINT const nRepCnt, UINT const nFlags );
    afx_msg void OnChannelRadioClicked( UINT uId );
    afx_msg void OnValueEditUpdate( UINT uId );
    afx_msg void OnHexColorUpdate( );
    afx_msg void OnZStripMouseMove( NMHDR* pNotifyStruct, LRESULT* result );
    afx_msg void OnXyGridMouseMove( NMHDR* pNotifyStruct, LRESULT* result );

    //============================================
    // Member variables
    //============================================

    CButton* const m_mapChannelToButtonControl[6] {
        &m_radioLabL,
        &m_radioLabA,
        &m_radioLabB,
        &m_radioSrgbR,
        &m_radioSrgbG,
        &m_radioSrgbB,
    };

    std::unordered_map<UINT, CEdit*> const m_mapValueControlIdToEditControl {
        {     IDC_LAB_L_VALUE, &m_editLabL     },
        {     IDC_LAB_A_VALUE, &m_editLabA     },
        {     IDC_LAB_B_VALUE, &m_editLabB     },
        {    IDC_SRGB_R_VALUE, &m_editSrgbR    },
        {    IDC_SRGB_G_VALUE, &m_editSrgbG    },
        {    IDC_SRGB_B_VALUE, &m_editSrgbB    },
        { IDC_HEX_COLOR_VALUE, &m_editHexColor },
    };

    std::unordered_map<HWND, AllChannels> m_mapHwndToChannel;
    std::unordered_map<HWND, WNDPROC>     m_mapHwndToWndProc;

    CColorPickerDoc* m_pDoc                { };
    CEdit*           m_pCurrentEdit        { };
    bool             m_fBlockBitmapUpdates { };

    //
    // Controls
    //

    CButton      m_groupBoxLab;
    CButton      m_radioLabL;
    CButton      m_radioLabA;
    CButton      m_radioLabB;
    CEdit        m_editLabL;
    CEdit        m_editLabA;
    CEdit        m_editLabB;

    CButton      m_groupBoxSrgb;
    CButton      m_radioSrgbR;
    CButton      m_radioSrgbG;
    CButton      m_radioSrgbB;
    CEdit        m_editSrgbR;
    CEdit        m_editSrgbG;
    CEdit        m_editSrgbB;

    CStatic      m_labelHexColor;
    CEdit        m_editHexColor;

    CStaticBrush m_staticSwatch;
    CZStrip      m_staticZStrip;
    CXyGrid      m_staticXyGrid;

    CButton      m_buttonClose;

    //
    // Channel mapping
    //

    AllChannels   m_channelX { AllChannels::LabA };
    AllChannels   m_channelY { AllChannels::LabB };
    AllChannels   m_channelZ { AllChannels::LabL };

};
