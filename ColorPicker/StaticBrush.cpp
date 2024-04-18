#include "pch.h"

#include "StaticBrush.h"

BEGIN_MESSAGE_MAP( CStaticBrush, CStatic )
    ON_WM_ERASEBKGND( )
    ON_WM_PAINT( )
END_MESSAGE_MAP( )

void CStaticBrush::SetColor( SrgbColor const& color ) {
    if ( m_color == color ) {
        return;
    }
    m_color = color;

    m_brush.DeleteObject( );
    m_brush.CreateSolidBrush( (COLORREF) m_color );
    Invalidate( FALSE );
}

BOOL CStaticBrush::OnEraseBkgnd( CDC* /*pDC*/ ) {
    return TRUE;
}

void CStaticBrush::OnPaint( ) {
    CRect rcClient;
    GetClientRect( rcClient );

    CPaintDC dc { this };
    dc.FillRect( rcClient, &m_brush );

    // Do not call CStatic::OnPaint() for painting messages
}
