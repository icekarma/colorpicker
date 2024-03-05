#pragma once

#include "ChildView.h"

class CMainFrame:
    public CFrameWnd
{

    DECLARE_DYNCREATE( CMainFrame )
    DECLARE_MESSAGE_MAP( )

public:

    //CMainFrame( ) noexcept { /*empty*/ }

    virtual ~CMainFrame( ) { /*empty*/ }

protected:

    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

    //virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
    //afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    //afx_msg void OnSetFocus( CWnd* pOldWnd );

};
