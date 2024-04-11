#pragma once

class CStaticBrush: public CStatic {

    DECLARE_MESSAGE_MAP( )

public:

    ~CStaticBrush( ) {
        m_brush.DeleteObject( );
    }

    void SetColor( SrgbColor const& color );

protected:

    afx_msg BOOL OnEraseBkgnd( CDC* pDC );
    afx_msg void OnPaint( );

    SrgbColor m_color { };
    CBrush    m_brush { (COLORREF) m_color };

};
