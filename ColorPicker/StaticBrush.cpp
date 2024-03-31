#include "pch.h"

#include "StaticBrush.h"

BEGIN_MESSAGE_MAP( CStaticBrush, CStatic )
END_MESSAGE_MAP( )

void CStaticBrush::SetColor( SrgbColor const& color ) {
    HBRUSH hNewBrush { ::CreateSolidBrush( (COLORREF) color ) };
    if ( !hNewBrush ) {
        debug( "CStaticBrush::SetColor: CreateSolidBrush failed\n" );
        return;
    }

    HBRUSH hOldBrush { reinterpret_cast<HBRUSH>( ::SetClassLongPtr( GetSafeHwnd( ), GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>( hNewBrush ) ) ) };
    if ( hOldBrush ) {
        DeleteObject( hOldBrush );
    }
}
