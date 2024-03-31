#pragma once

UINT const ZSBN_MOUSEMOVE { 17 };

struct ZSB_MOUSEMOVE {
    NMHDR  hdr;
    CPoint point;
};

class CStaticBitmap: public CStatic {

    DECLARE_MESSAGE_MAP( )

public:

    void SetCustomMessageTarget( CWnd* pWndTarget ) {
        m_pWndTarget = pWndTarget;
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
    CWnd*  m_pWndTarget   { };
    bool   m_fLButtonDown { };
    CPoint m_ptLast       { -1, -1 };

};
