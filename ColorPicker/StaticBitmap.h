#pragma once

UINT const ZSBN_MOUSEMOVE { 17 };

struct ZSB_MOUSEMOVE: NMHDR {
    CPoint point;
};

class CStaticBitmap: public CStatic {

public:

    virtual ~CStaticBitmap( ) override = default;

    bool GetInverted( ) const {
        return !m_fInverted;
    }

    void SetInverted( bool const fInverted ) {
        m_fInverted = !fInverted;
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
    bool   m_fInverted    { true };
    bool   m_fLButtonDown { };
    CPoint m_ptLast       { -1, -1 };

};
