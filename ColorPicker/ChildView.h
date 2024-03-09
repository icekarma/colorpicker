#pragma once

class CChildView:
    public CFormView
{

    DECLARE_DYNCREATE( CChildView )
    DECLARE_MESSAGE_MAP( )

public:

#if defined AFX_DESIGN_TIME
    enum { IDD = IDD_CHILDVIEW_2 };
#endif

protected:

    CChildView( ): CFormView { IDD_CHILDVIEW_2 } { /*empty*/ }

    virtual ~CChildView( ) { /*empty*/ }

protected:

    void _AdjustPosition( CWnd* ctrl, SIZE const& adjust );
    void _AdjustSize( CWnd* ctrl, SIZE const& adjust );

    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

    afx_msg void OnPaint( );
    afx_msg void OnCloseButtonClicked( );

};
