#pragma once

#undef DEBUG_WINDOW_SIZE

class CMainFrame:
    public CFrameWnd
{

    DECLARE_DYNCREATE( CMainFrame )
    DECLARE_MESSAGE_MAP( )

public:

    virtual ~CMainFrame( ) override = default;

protected:

    virtual BOOL PreCreateWindow( CREATESTRUCT& cs ) override;

#if defined DEBUG_WINDOW_SIZE
    afx_msg void OnSizing( UINT fwSide, LPRECT pRect );
    afx_msg void OnSize( UINT nType, int cx, int cy );
#endif // defined DEBUG_WINDOW_SIZE

};
