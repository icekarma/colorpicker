#pragma once

#include "Color.h"

class CStaticBrush: public CStatic {

    DECLARE_MESSAGE_MAP( )

public:

    ~CStaticBrush( ) {
        m_brush.DeleteObject( );
    }

    void SetColor( SrgbColor const& color );

protected:

    SrgbColor m_color { };
    CBrush    m_brush { (COLORREF) m_color };

    afx_msg BOOL OnEraseBkgnd( CDC* pDC );
    afx_msg void OnPaint( );

};
