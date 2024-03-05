#pragma once

class CNewChildView:
    public CFormView
{

    DECLARE_DYNCREATE( CNewChildView )
    DECLARE_MESSAGE_MAP( )

    // Construction
protected:

    CNewChildView( );
    virtual ~CNewChildView( );

#if defined AFX_DESIGN_TIME
public:

    enum { IDD = IDD_NEWCHILDVIEW };
#endif

    // Overrides
protected:

    virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

    // Generated message map functions
protected:

    afx_msg void OnPaint( );

};
