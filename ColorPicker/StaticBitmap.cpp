#include "pch.h"

#include "StaticBitmap.h"

namespace {

    [[nodiscard]] CPoint ClipPointToRect( CPoint const& point, CRect const& rect ) {
        return {
            std::min( std::max( point.x, rect.left ), rect.right  - 1 ),
            std::min( std::max( point.y, rect.top  ), rect.bottom - 1 )
        };
    }

}

void CStaticBitmap::NotifyPosition( CPoint const& point ) {
    if ( CPoint pt { ClipPointToRect( point, m_rcClient ) }; pt != m_ptLast ) {
        m_ptLast = pt;

        if ( m_fInverted ) {
            pt.y = m_rcClient.Height( ) - pt.y - 1;
        }

        UINT_PTR const uControlId { static_cast<UINT_PTR>( m_nControlId ) };
        ZSB_MOUSEMOVE mm { { GetSafeHwnd( ), uControlId, ZSBN_MOUSEMOVE }, pt };
        GetOwner( )->SendMessage( WM_NOTIFY, uControlId, reinterpret_cast<LPARAM>( &mm ) );
    }
}

BOOL CStaticBitmap::OnEraseBkgnd( CDC* /*pDC*/ ) {
    return TRUE;
}

void CStaticBitmap::OnLButtonDown( UINT nFlags, CPoint point ) {
    if ( m_nControlId ) {
        m_fLButtonDown = true;
        SetCapture( );

        NotifyPosition( point );
    }

    CStatic::OnLButtonDown( nFlags, point );
}

void CStaticBitmap::OnLButtonUp( UINT nFlags, CPoint point ) {
    if ( m_fLButtonDown && m_nControlId ) {
        NotifyPosition( point );

        m_fLButtonDown = false;
        ReleaseCapture( );
    }

    CStatic::OnLButtonUp( nFlags, point );
}

void CStaticBitmap::OnMouseMove( UINT nFlags, CPoint point ) {
    if ( m_fLButtonDown && m_nControlId ) {
        NotifyPosition( point );
    }

    CStatic::OnMouseMove( nFlags, point );
}

void CStaticBitmap::OnSize( UINT nType, int cx, int cy ) {
    CStatic::OnSize( nType, cx, cy );

    GetClientRect( m_rcClient );

    if ( !m_nControlId ) {
        m_nControlId = ::GetDlgCtrlID( GetSafeHwnd( ) );
        if ( !m_nControlId ) {
            debug( L"CStaticBitmap::OnSize: Couldn't get control ID for our window handle: %lu\n", ::GetLastError( ) );
        }
    }
}
