#pragma once

class CChildView:
    public CFormView
{

    DECLARE_DYNCREATE( CChildView )
    DECLARE_MESSAGE_MAP( )

public:

#if defined AFX_DESIGN_TIME
    enum { IDD = IDD_NEWCHILDVIEW };
#endif

protected:

    CChildView( ): CFormView { IDD_NEWCHILDVIEW } { /*empty*/ }

    virtual ~CChildView( ) { /*empty*/ }

protected:

    //virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

    //afx_msg void OnPaint( );

};
