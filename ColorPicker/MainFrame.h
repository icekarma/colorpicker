#pragma once

#if defined _DEBUG
#undef DEBUG_WINDOW_SIZE
#endif // defined _DEBUG

class CMainFrame:
    public CFrameWnd
{

    DECLARE_DYNCREATE( CMainFrame )
    DECLARE_MESSAGE_MAP( )

public:

    virtual ~CMainFrame( ) override = default;

protected:

    virtual BOOL PreCreateWindow( CREATESTRUCT& cs ) override;

    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
#if defined _DEBUG && defined DEBUG_WINDOW_SIZE
    afx_msg void OnSizing( UINT fwSide, LPRECT pRect );
    afx_msg void OnSize( UINT nType, int cx, int cy );
#endif // defined _DEBUG && defined DEBUG_WINDOW_SIZE

};
