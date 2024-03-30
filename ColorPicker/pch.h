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
#include <stdexcept>
#include <string>
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

#include <afxcontrolbars.h>
#include <afxsock.h>

#include <afxwinappex.h>
#include <afxdialogex.h>

#include <lcms2.h>

#include "Resource.h"

#if defined _DEBUG && defined DEBUG_NEW
#   if defined new
#       undef new
#   endif
#   define new DEBUG_NEW
#endif

#include "Debug.h"

//================================================
// Constants
//================================================

LONG  constexpr         WindowStylesToRemove { WS_BORDER        | WS_DLGFRAME         | WS_THICKFRAME       | WS_HSCROLL           | WS_VSCROLL       | WS_MINIMIZEBOX   | WS_MAXIMIZEBOX };
DWORD constexpr ExtendedWindowStylesToRemove { WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME | WS_EX_LEFTSCROLLBAR | WS_EX_RIGHTSCROLLBAR | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE                  };

//================================================
// Concepts
//================================================

template<typename T> concept Enumeration = std::is_enum_v<T>;

//================================================
// Overloaded operators
//================================================

template<Enumeration T>
std::underlying_type_t<T> constexpr operator+( T const rhs ) {
    return static_cast<std::underlying_type_t<T>>( rhs );
}

//================================================
// Pseudo-"cast operator"s
//================================================

template<typename ReturnT, typename ValueT>
inline ReturnT* dynamic_downcast( ValueT* p ) {
    return (ReturnT*) AfxDynamicDownCast( ReturnT::GetThisClass( ), p );
}

//================================================
// Functions
//================================================

