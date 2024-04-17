#pragma once

UINT const ZSBN_MOUSEMOVE { 17 };

struct ZSB_MOUSEMOVE: NMHDR {
    CPoint point;
};

class CStaticBitmap: public CStatic {

    DECLARE_MESSAGE_MAP( )

public:

    virtual ~CStaticBitmap( ) override {
        /*empty*/
    }

protected:

    void NotifyPosition( CPoint const& point );

    afx_msg BOOL OnEraseBkgnd( CDC* pDC );
    afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
    afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
    afx_msg void OnMouseMove( UINT nFlags, CPoint point );
    afx_msg void OnSize( UINT nType, int cx, int cy );

    CRect  m_rcClient;
    int    m_nControlId   { };
    bool   m_fLButtonDown { };
    CPoint m_ptLast       { -1, -1 };

};
