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

#pragma warning( disable: 4061 4191 4514 4625 4626 4710 4711 4820 5027 5045 5245 )
// C4061: enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
// C4191: 'operator/operation' : unsafe conversion from 'type of expression' to 'type required'
// C4514: 'function' : unreferenced inline function has been removed
// C4625: 'derived class': copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
// C4626: 'derived class': assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
// C4710: 'function' : function not inlined
// C4711: function 'function' selected for inline expansion
// C4820: 'bytes' bytes padding added after construct 'member_name'
// C5027: 'type': move assignment operator was implicitly defined as deleted
// C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
// C5245: unreferenced function with internal linkage has been removed

#pragma warning( push )
#pragma warning( disable: 4081 4263 4264 4266 4365 4946 5026 5204 5220 5240 5246 )
// C4081: expected 'token1'; found 'token2'
// C4263: 'function' : member function does not override any base class virtual member function
// C4264: 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
// C4266: 'function' : no override available for virtual member function from base 'type'; function is hidden
// C4365: 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
// C4946: reinterpret_cast used between related classes: 'class1' and 'class2'
// C5026: 'type': move constructor was implicitly defined as deleted
// C5204: 'type-name': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
// C5220: 'name': a non-static data member with a volatile qualified type no longer implies that compiler generated copy/move constructors and copy/move assignment operators are not trivial
// C5240: 'attribute-name': attribute is ignored in this syntactic position
// C5246: 'member': the initialization of a subobject should be wrapped in braces

#include "targetver.h"

#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <array>
#include <concepts>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <vector>

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

#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable: 4081 4365 4946 5204 5240 5246 )
// C4081: expected 'token1'; found 'token2'
// C4365: 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
// C4946: reinterpret_cast used between related classes: 'class1' and 'class2'
// C5204: 'type-name': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
// C5240: 'attribute-name': attribute is ignored in this syntactic position
// C5246: 'member': the initialization of a subobject should be wrapped in braces

#include <lcms2.h>

#pragma warning( pop )

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

//================================================
// Global template functions
//================================================

template<typename T>
inline T constexpr typed_nullptr( ) {
    return static_cast<T>( nullptr );
}

template<typename KeyT, typename ValueT, typename HasherT, typename KeyEqT, typename AllocT>
ValueT _MapImpl( std::unordered_map<KeyT, ValueT, HasherT, KeyEqT, AllocT> const& map, KeyT const& key, ValueT const defaultValue = { } ) {
    if ( map.contains( key ) ) {
        return map.at( key );
    } else {
        return defaultValue;
    }
}
