#pragma once

#include <lcms2.h>

//================================================
// Preprocessor macros
//================================================

#undef  LAB_COLOR_USE_FLOAT
#define LAB_COLOR_USE_DOUBLE

#if ( !defined LAB_COLOR_USE_FLOAT && !defined LAB_COLOR_USE_DOUBLE ) || ( defined LAB_COLOR_USE_FLOAT && defined LAB_COLOR_USE_DOUBLE )
#   error Must define only ONE of LAB_COLOR_USE_FLOAT or LAB_COLOR_USE_DOUBLE.
#endif

//================================================
// Forward declarations
//================================================

template<typename T>
class LabColor;

template<typename T>
class SrgbColor;

//================================================
// Type aliases
//================================================

#if defined LAB_COLOR_USE_FLOAT
using FloatT = float;
#elif defined LAB_COLOR_USE_DOUBLE
using FloatT = double;
#endif

using LabColorValue  = LabColor<FloatT>;
using SrgbColorValue = SrgbColor<uint8_t>;

//================================================
// Constants
//================================================

cmsUInt32Number constexpr LabPixelFormat {
#if defined LAB_COLOR_USE_FLOAT
    TYPE_Lab_FLT
#elif defined LAB_COLOR_USE_DOUBLE
    TYPE_Lab_DBL
#endif
};

FloatT constexpr LabMinimumL {    0.0 }; FloatT constexpr LabMaximumL {  100.0 };
FloatT constexpr LabMinimumA { -128.0 }; FloatT constexpr LabMaximumA {  127.0 };
FloatT constexpr LabMinimumB { -128.0 }; FloatT constexpr LabMaximumB {  127.0 };

//================================================
// Late #include
//================================================

#include "TransformsManager.h"

//================================================
// Types
//================================================

enum class ColorSpace {
    unknown = 0,
    Lab     = 1,
    sRGB    = 2,
};

template<typename ValueT>
class Color {

public:

    virtual ColorSpace constexpr GetColorSpace( ) = 0;
    virtual int        constexpr GetChannelCount( ) = 0;
    virtual ValueT               GetChannelValue( int const channel ) = 0;
    virtual void                 SetChannelValue( int const channel, ValueT const value ) = 0;

};

template<typename ValueT>
class LabColor:
    public Color<ValueT>
{

public:

    int const ChannelL = 0;
    int const ChannelA = 1;
    int const ChannelB = 2;

    LabColor( ):
        LabColor { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    LabColor( ValueT const L, ValueT const a, ValueT const b ):
        _values { L, a, b }
    {
        /*empty*/
    }

    LabColor( LabColor const& rhs ):
        _values { rhs._values[0], rhs._values[1], rhs._values[2] }
    {
        /*empty*/
    }

    LabColor( LabColor&& rhs ) noexcept:
        _values { rhs._values[0], rhs._values[1], rhs._values[2] }
    {
        rhs._values[0] = static_cast<ValueT>( 0 );
        rhs._values[1] = static_cast<ValueT>( 0 );
        rhs._values[2] = static_cast<ValueT>( 0 );
    }

    LabColor& operator=( LabColor const& rhs ) {
        _values[0] = rhs._values[0];
        _values[1] = rhs._values[1];
        _values[2] = rhs._values[2];

        return *this;
    }

    LabColor& operator=( LabColor&& rhs ) noexcept {
        _values[0] = rhs._values[0];
        _values[1] = rhs._values[1];
        _values[2] = rhs._values[2];

        rhs._values[0] = static_cast<ValueT>( 0 );
        rhs._values[1] = static_cast<ValueT>( 0 );
        rhs._values[2] = static_cast<ValueT>( 0 );

        return *this;
    }

    virtual ColorSpace constexpr GetColorSpace( ) {
        return ColorSpace::Lab;
    }

    virtual int constexpr GetChannelCount( ) {
        return 3;
    }

    virtual ValueT GetChannelValue( int const channel ) {
        assert( ( channel >= 0 ) && ( channel < 3 ) );
        return _values[channel];
    }

    virtual void SetChannelValue( int const channel, ValueT const value ) {
        assert( ( channel >= 0 ) && ( channel < 3 ) );
        _values[channel] = value;
    }

protected:

    ValueT _values[3];

};

template<typename ValueT>
class SrgbColor:
    public Color<ValueT>
{

public:

    int const ChannelR = 0;
    int const ChannelG = 1;
    int const ChannelB = 2;

    SrgbColor( ):
        SrgbColor { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    SrgbColor( ValueT const R, ValueT const G, ValueT const B ):
        _values { R, G, B }
    {
        /*empty*/
    }

    SrgbColor( SrgbColor const& rhs ):
        _values { rhs._values[0], rhs._values[1], rhs._values[2] }
    {
        /*empty*/
    }

    SrgbColor( SrgbColor&& rhs ) noexcept:
        _values { rhs._values[0], rhs._values[1], rhs._values[2] }
    {
        rhs._values[0] = static_cast<ValueT>( 0 );
        rhs._values[1] = static_cast<ValueT>( 0 );
        rhs._values[2] = static_cast<ValueT>( 0 );
    }

    SrgbColor& operator=( SrgbColor const& rhs ) {
        _values[0] = rhs._values[0];
        _values[1] = rhs._values[1];
        _values[2] = rhs._values[2];

        return *this;
    }

    SrgbColor& operator=( SrgbColor&& rhs ) noexcept {
        _values[0] = rhs._values[0];
        _values[1] = rhs._values[1];
        _values[2] = rhs._values[2];

        rhs._values[0] = static_cast<ValueT>( 0 );
        rhs._values[1] = static_cast<ValueT>( 0 );
        rhs._values[2] = static_cast<ValueT>( 0 );

        return *this;
    }

    virtual ColorSpace constexpr GetColorSpace( ) {
        return ColorSpace::sRGB;
    }

    virtual int constexpr GetChannelCount( ) {
        return 3;
    }

    virtual void GetChannelValues( ValueT values[3] ) {
        values[0] = _values[0];
        values[1] = _values[1];
        values[2] = _values[2];
    }

    virtual void SetChannelValues( ValueT values[3] ) {
        _values[0] = values[0];
        _values[1] = values[1];
        _values[2] = values[2];
    }

    virtual ValueT GetChannelValue( int const channel ) {
        assert( ( channel >= 0 ) && ( channel < 3 ) );

        return _values[channel];
    }

    virtual void SetChannelValue( int const channel, ValueT const value ) {
        assert( ( channel >= 0 ) && ( channel < 3 ) );

        _values[channel] = value;
    }

protected:

    ValueT _values[3];

};
