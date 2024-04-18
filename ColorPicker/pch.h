#pragma once

#if defined _UNICODE
#   if defined _M_IX86
#       pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#   elif defined _M_X64
#       pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#   else
#       pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#   endif
#endif

#define _AFX_ALL_WARNINGS
#define _AFX_NO_AFXCMN_SUPPORT
#define _AFX_NO_OLE_SUPPORT
#define  AFX_NO_CTLBARS_SUPPORT
#define  AFX_NO_SOCKET_SUPPORT
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define  NOMINMAX
#define  VC_EXTRALEAN
#define  WIN32_LEAN_AND_MEAN

#include "targetver.h"

#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstdint>

#include <algorithm>
#include <array>
#include <concepts>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

using std::max;
using std::min;

#include <afxwin.h>
#include <afxext.h>

#if !defined _AFX_NO_OLE_SUPPORT
#   include <afxdtctl.h>
#endif
#if !defined _AFX_NO_AFXCMN_SUPPORT
#   include <afxcmn.h>
#endif
#if !defined AFX_NO_CTLBARS_SUPPORT
#   include <afxcontrolbars.h>
#endif
#if !defined AFX_NO_SOCKET_SUPPORT
#   include <afxsock.h>
#endif

#include <afxwinappex.h>
#include <afxdialogex.h>

#include <lcms2.h>

#include "Resource.h"

#if defined _DEBUG

#   if defined DEBUG_NEW
#       if defined new
#           undef new
#       endif // defined new
#       define new DEBUG_NEW
#   endif // defined DEBUG_NEW

// A wprintf()-like function to record a message to the debugging facility.
template<typename... ArgsT>
void debug( wchar_t const* format, ArgsT... args ) {
    CString message;
    message.Format( format, args... );
    ::OutputDebugStringW( message );
}

#else // !defined _DEBUG

// A wprintf()-like function to record a message to the debugging facility.
template<typename... ArgsT>
void debug( wchar_t const*, ArgsT... ) {
    /*empty*/
}

#endif // defined _DEBUG

#include "Early.h"
#include "Color.h"

//================================================
// Constants
//================================================

LONG  constexpr         WindowStylesToRemove { WS_BORDER        | WS_DLGFRAME         | WS_THICKFRAME       | WS_HSCROLL           | WS_VSCROLL       | WS_MINIMIZEBOX   | WS_MAXIMIZEBOX };
DWORD constexpr ExtendedWindowStylesToRemove { WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME | WS_EX_LEFTSCROLLBAR | WS_EX_RIGHTSCROLLBAR | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE                  };

//================================================
// Pseudo-cast operators
//================================================

template<typename T>
concept IsCObject = std::derived_from<T, CObject>;

template<IsCObject Class>
inline CRuntimeClass* runtime_class( ) {
    return Class::GetThisClass( );
}

template<IsCObject Class, typename ValueT>
inline Class* dynamic_downcast( ValueT* p ) {
    return reinterpret_cast<Class*>( AfxDynamicDownCast( runtime_class<Class>( ), p ) );
}

template<IsCObject Class, IsCObject ValueT>
inline Class* static_downcast( ValueT* p ) {
#if defined _DEBUG
    return static_cast<Class*>( AfxStaticDownCast( runtime_class<Class>( ), p ) );
#else // !defined _DEBUG
    return static_cast<Class*>( p );
#endif // defined _DEBUG
}
