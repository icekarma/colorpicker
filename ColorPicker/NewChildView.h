#pragma once

class CNewChildView:
    public CFormView
{

    DECLARE_DYNCREATE( CNewChildView )
    DECLARE_MESSAGE_MAP( )

    // Construction
protected:

    CNewChildView( ):
        CFormView { IDD_NEWCHILDVIEW }
    {
        /*empty*/
    }

    virtual ~CNewChildView( ) {
        /*empty*/
    }

    // Dialog Data
public:

#if defined AFX_DESIGN_TIME
    enum { IDD = IDD_NEWCHILDVIEW };
#endif

    // Overrides
protected:

    //virtual void DoDataExchange( CDataExchange* pDX );
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

    // Generated message map functions
protected:

    //afx_msg void OnPaint( );

};
