#pragma once

class CChildView:
    public CWnd
{

    DECLARE_MESSAGE_MAP( )

    // Construction
public:

    //CChildView( );

    virtual ~CChildView( ) {
        /*empty*/
    }

    // Attributes
public:

    // Operations
public:

    // Overrides
protected:

    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

    // Implementation
public:

    // Generated message map functions
protected:

    afx_msg void OnPaint( );

};

