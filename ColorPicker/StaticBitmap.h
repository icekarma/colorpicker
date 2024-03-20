#pragma once

UINT const ZSBN_MOUSEMOVE { WM_USER + 0x801 };

struct StaticBitmapMouseMoveArgs {
    UINT   nFlags;
    CPoint point;
};

class CStaticBitmap: public CStatic {

    DECLARE_MESSAGE_MAP( )

public:

    afx_msg void OnLButtonDown ( UINT nFlags, CPoint point );
    afx_msg void OnLButtonUp   ( UINT nFlags, CPoint point );
    afx_msg void OnMouseMove   ( UINT nFlags, CPoint point );
    afx_msg void OnSize        ( UINT nType, int cx, int cy );

    void SetCustomMessageTargetWindow( HWND htarget ) {
        m_hWndTarget = htarget;
    }

protected:

    CRect m_rcClient;
    HWND  m_hWndTarget   { };
    bool  m_fLButtonDown { };

};
