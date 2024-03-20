#include "pch.h"

#include "StaticBitmap.h"

BEGIN_MESSAGE_MAP( CStaticBitmap, CStatic )
	ON_WM_MOUSEMOVE( )
	ON_WM_LBUTTONDOWN( )
	ON_WM_LBUTTONUP( )
	ON_WM_SIZE( )
END_MESSAGE_MAP( )

void CStaticBitmap::OnLButtonDown( UINT nFlags, CPoint point ) {
	m_fLButtonDown = true;
	SetCapture( );
	if ( m_hWndTarget ) {
		::PostMessage( m_hWndTarget, ZSBN_MOUSEMOVE, reinterpret_cast<WPARAM>( GetSafeHwnd( ) ), reinterpret_cast<LPARAM>( new StaticBitmapMouseMoveArgs { nFlags, point } ) );
	}

	CStatic::OnLButtonDown( nFlags, point );
}

void CStaticBitmap::OnLButtonUp( UINT nFlags, CPoint point ) {
	m_fLButtonDown = false;
	ReleaseCapture( );

	CStatic::OnLButtonUp( nFlags, point );
}

void CStaticBitmap::OnMouseMove( UINT nFlags, CPoint point ) {
	if ( m_hWndTarget && m_fLButtonDown && m_rcClient.PtInRect( point ) ) {
		::PostMessage( m_hWndTarget, ZSBN_MOUSEMOVE, reinterpret_cast<WPARAM>( GetSafeHwnd( ) ), reinterpret_cast<LPARAM>( new StaticBitmapMouseMoveArgs { nFlags, point } ) );
	}

	CStatic::OnMouseMove( nFlags, point );
}

void CStaticBitmap::OnSize( UINT nType, int cx, int cy ) {
	CStatic::OnSize( nType, cx, cy );

	GetClientRect( m_rcClient );
}
