#pragma once

#include "NewChildView.h"

class CMainFrame:
    public CFrameWnd
{

    DECLARE_DYNCREATE( CMainFrame )
    DECLARE_MESSAGE_MAP( )

    // Construction
public:

    //CMainFrame( ) noexcept;
    //virtual ~CMainFrame( );

    // Attributes
public:

    // Operations
public:

    // Overrides
public:

    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
    //virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

    // Implementation
public:

    // Generated message map functions
protected:

    //afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    //afx_msg void OnSetFocus( CWnd* pOldWnd );

};
