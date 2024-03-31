#include "pch.h"

#include "StaticBitmap.h"

BEGIN_MESSAGE_MAP( CStaticBitmap, CStatic )
    ON_WM_ERASEBKGND( )
    ON_WM_LBUTTONDOWN( )
    ON_WM_LBUTTONUP( )
    ON_WM_MOUSEMOVE( )
    ON_WM_SIZE( )
END_MESSAGE_MAP( )

void CStaticBitmap::_Notify( CPoint point ) {
    CPoint ptNew {
        std::min( std::max( point.x, m_rcClient.left ), m_rcClient.right  - 1 ),
        std::min( std::max( point.y, m_rcClient.top  ), m_rcClient.bottom - 1 )
    };
    if ( ptNew != m_ptLast ) {
        if ( !m_nControlId ) {
            ::SetLastError( 0 );
            m_nControlId = ::GetDlgCtrlID( GetSafeHwnd( ) );
            if ( !m_nControlId ) {
                debug( "CStaticBitmap::_Notify: Couldn't get control ID for our window handle: %lu\n", ::GetLastError( ) );
                return;
            }
        }

        m_ptLast = ptNew;
        ::PostMessage( m_hWndTarget, ZSBN_MOUSEMOVE, m_nControlId, MAKELPARAM( ptNew.x, ptNew.y ) );
    }
}

BOOL CStaticBitmap::OnEraseBackground( CDC* /*pDC*/ ) {
    return TRUE;
}

void CStaticBitmap::OnLButtonDown( UINT nFlags, CPoint point ) {
    m_fLButtonDown = true;
    SetCapture( );

    if ( m_hWndTarget ) {
        _Notify( point );
    }

    CStatic::OnLButtonDown( nFlags, point );
}

void CStaticBitmap::OnLButtonUp( UINT nFlags, CPoint point ) {
    if ( m_hWndTarget && m_fLButtonDown ) {
        _Notify( point );
    }

    m_fLButtonDown = false;
    ReleaseCapture( );

    CStatic::OnLButtonUp( nFlags, point );
}

void CStaticBitmap::OnMouseMove( UINT nFlags, CPoint point ) {
    if ( m_hWndTarget && m_fLButtonDown ) {
        _Notify( point );
    }

    CStatic::OnMouseMove( nFlags, point );
}

void CStaticBitmap::OnSize( UINT nType, int cx, int cy ) {
    CStatic::OnSize( nType, cx, cy );

    GetClientRect( m_rcClient );
}
