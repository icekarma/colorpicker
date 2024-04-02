#pragma once

class CMainFrame:
    public CFrameWnd
{

    DECLARE_DYNCREATE( CMainFrame )
    DECLARE_MESSAGE_MAP( )

public:

    virtual ~CMainFrame( ) { /*empty*/ }

protected:

    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

#if defined DEBUG_WINDOW_SIZE
    afx_msg void OnSizing( UINT fwSide, LPRECT pRect );
    afx_msg void OnSize( UINT nType, int cx, int cy );
#endif // defined DEBUG_WINDOW_SIZE

};
