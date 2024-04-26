#pragma once

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

template<Enumeration T>
T constexpr operator++( T& rhs ) {
    rhs = static_cast<T>( +rhs + +static_cast<T>( 1 ) );
    return rhs;
}

template<Enumeration T>
T constexpr operator++( T& rhs, int ) {
    T old = rhs;
    ++rhs;
    return old;
}

template<Enumeration T>
T constexpr operator--( T& rhs ) {
    rhs = static_cast<T>( +rhs - +static_cast<T>( 1 ) );
    return rhs;
}

template<Enumeration T>
T constexpr operator--( T& rhs, int ) {
    T old = rhs;
    --rhs;
    return old;
}
