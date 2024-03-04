#pragma once

#include "NewChildView.h"

class CMainFrame: public CFrameWnd {

    DECLARE_DYNAMIC( CMainFrame )
    DECLARE_MESSAGE_MAP( )

public:

    CMainFrame( ) noexcept;

    // Attributes
public:

    // Operations
public:

    // Overrides
public:

    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
    virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

    // Implementation
public:

    virtual ~CMainFrame( );

protected:
    CNewChildView* m_pwndView;

    // Generated message map functions
protected:

    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    afx_msg void OnSetFocus( CWnd* pOldWnd );

};