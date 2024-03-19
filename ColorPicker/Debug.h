#pragma once

#include <stdexcept>

#undef DEBUG_TO_STDOUT
#undef DEBUG_TO_STDERR

template<typename T> concept ExceptionType = std::is_base_of_v<std::exception, T>;

template<typename... Args>
std::string FormatString( std::string const& format, Args... args ) {
    using CharT = std::string::value_type;

    size_t bufferSize { 4096 };
    auto   buffer     { static_cast<CharT*>( malloc( bufferSize * sizeof CharT ) ) };
    if ( !buffer ) {
        throw std::runtime_error( "FormatString ran out of memory" );
    }

    do {
        __pragma( warning( disable: 4774 ) )
        if ( int const ret { _snprintf_s( buffer, bufferSize, _TRUNCATE, format.c_str( ), args... ) }; ret > -1 ) {
            std::string tmp { buffer, static_cast<size_t>( ret ) };
            free( buffer );
            return tmp;
        }

        bufferSize <<= 1;
        auto const tmp { static_cast<CharT*>( realloc( buffer, bufferSize ) ) };
        if ( !tmp ) {
            throw std::runtime_error( "FormatString ran out of memory" );
        }
        buffer = tmp;
    } while ( true );
}

template<typename... Args>
std::wstring FormatString( std::wstring const& format, Args... args ) {
    using CharT = std::wstring::value_type;

    size_t bufferSize { 4096 };
    auto   buffer     { static_cast<CharT*>( malloc( bufferSize * sizeof CharT ) ) };

    do {
        __pragma( warning( disable: 4774 ) )
        if ( int const ret { _snwprintf_s( buffer, bufferSize, _TRUNCATE, format.c_str( ), args... ) }; ret > -1 ) {
            std::wstring tmp { buffer, static_cast<size_t>( ret ) };
            free( buffer );
            return tmp;
        }

        bufferSize <<= 1;
        auto const tmp { static_cast<CharT*>( realloc( buffer, bufferSize ) ) };
        if ( !tmp ) {
            throw std::runtime_error( "FormatString ran out of memory" );
        }
        buffer = tmp;
    } while ( true );
}

inline void _LogDebugMessage( std::string const& message ) {
#if defined DEBUG_TO_STDOUT
    fwrite( message.c_str( ), 1, message.length( ), stdout );
#endif // defined DEBUG_TO_STDOUT
#if defined DEBUG_TO_STDERR
    fwrite( message.c_str( ), 1, message.length( ), stderr );
#endif // defined DEBUG_TO_STDERR

    OutputDebugStringA( message.c_str( ) );
}

inline void _LogDebugMessage( std::wstring const& message ) {
#if defined DEBUG_TO_STDOUT
    fwrite( message.c_str( ), 2, message.length( ), stdout );
#endif // defined DEBUG_TO_STDOUT
#if defined DEBUG_TO_STDERR
    fwrite( message.c_str( ), 2, message.length( ), stderr );
#endif // defined DEBUG_TO_STDERR

    OutputDebugStringW( message.c_str( ) );
}

template<typename... ArgsT>
void _LogDebugMessage( char const* format, ArgsT... args ) {
    _LogDebugMessage( FormatString( format, args... ) );
}

template<typename... ArgsT>
void _LogDebugMessage( wchar_t const* format, ArgsT... args ) {
    _LogDebugMessage( FormatString( format, args... ) );
}

// A printf()-like function to record a message to the debugging facility.
template<typename... ArgsT>
void debug( char const* format, ArgsT... args ) {
    _LogDebugMessage( format, args... );
}

// A wprintf()-like function to record a message to the debugging facility.
template<typename... ArgsT>
void debug( wchar_t const* format, ArgsT... args ) {
    _LogDebugMessage( format, args... );
}

// Records a message to the debugging facility, like debug(), then throws an exception of the provided type with the same message.
template<ExceptionType ExceptionT, typename... ArgsT>
[[noreturn]] void die( char const* format, ArgsT... args ) {
    std::string const buf { FormatString( format, args... ) };
    _LogDebugMessage( buf );
    throw ExceptionT { buf };
}

// Records a message to the debugging facility, like debug(), then throws an exception of the provided type with the same message.
template<ExceptionType ExceptionT, typename... ArgsT>
[[noreturn]] void die( wchar_t const* format, ArgsT... args ) {
    std::wstring const buf { FormatString( format, args... ) };
    _LogDebugMessage( buf );
    throw ExceptionT { buf };
}

// Records a message to the debugging facility, like debug(), then throws an exception of type std::runtime_error with the same message.
template<typename... ArgsT>
[[noreturn]] void die( char const* format, ArgsT... args ) {
    die<std::runtime_error>( format, args... );
}

// Records a message to the debugging facility, like debug(), then throws an exception of type std::runtime_error with the same message.
template<typename... ArgsT>
[[noreturn]] void die( wchar_t const* format, ArgsT... args ) {
    die<std::runtime_error>( format, args... );
}
