#pragma once

class CNewChildView:
    public CFormView
{

    DECLARE_DYNCREATE( CNewChildView )
    DECLARE_MESSAGE_MAP( )

protected:

    CNewChildView( );
    virtual ~CNewChildView( );

#ifdef AFX_DESIGN_TIME
public:

    enum { IDD = IDD_NEWCHILDVIEW };
#endif

protected:

    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

    afx_msg void OnPaint( );

};
