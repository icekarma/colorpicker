#pragma once

UINT const ZSBN_MOUSEMOVE { WM_USER + 0x7FE };

class CStaticBitmap: public CStatic {

    DECLARE_MESSAGE_MAP( )

public:

    void SetCustomMessageTargetWindow( HWND htarget ) {
        m_hWndTarget = htarget;
    }

protected:

    void _Notify( CPoint point );

    afx_msg BOOL OnEraseBkgnd( CDC* pDC );
    afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
    afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
    afx_msg void OnMouseMove( UINT nFlags, CPoint point );
    afx_msg void OnSize( UINT nType, int cx, int cy );

    CRect  m_rcClient;
    int    m_nControlId   { };
    HWND   m_hWndTarget   { };
    bool   m_fLButtonDown { };
    CPoint m_ptLast       { -1, -1 };

};
