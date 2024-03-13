#pragma once

#include <lcms2.h>

#include <cstdint>

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

cmsUInt32Number constexpr SrgbPixelFormat { TYPE_BGRA_8 };
cmsUInt32Number constexpr  LabPixelFormat {
#if defined LAB_COLOR_USE_FLOAT
    TYPE_Lab_FLT
#elif defined LAB_COLOR_USE_DOUBLE
    TYPE_Lab_DBL
#endif
};

int constexpr LabMinimumL {    0 }; int constexpr LabMaximumL {  100 };
int constexpr LabMinimumA { -128 }; int constexpr LabMaximumA {  127 };
int constexpr LabMinimumB { -128 }; int constexpr LabMaximumB {  127 };

int constexpr LabMinima[3] {
    LabMinimumL,
    LabMinimumA,
    LabMinimumB,
};

int constexpr LabMaxima[3] {
    LabMaximumL,
    LabMaximumA,
    LabMaximumB,
};

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

enum class LabChannels {
    L = 0,
    a = 1,
    b = 2,
};

enum class SrgbChannels {
    R = 0,
    G = 1,
    B = 2,
};

//================================================
// Classes
//================================================

//
// Template class Color
//

template<typename ValueT>
class Color {

public:

    Color( )                                          noexcept { /*empty*/ }
    Color( ValueT const, ValueT const, ValueT const ) noexcept { /*empty*/ }
    Color( ValueT const[3] )                          noexcept { /*empty*/ }

    virtual ColorSpace constexpr GetColorSpace( )                                         const noexcept = 0;
    virtual int        constexpr GetChannelCount( )                                       const noexcept = 0;

    virtual ValueT               GetChannelValue( int const channel )                     const noexcept = 0;
    virtual void                 SetChannelValue( int const channel, ValueT const value )       noexcept = 0;

    virtual void                 GetChannelValues( ValueT values[3] )                     const noexcept = 0;
    virtual void                 SetChannelValues( ValueT const values[3] )                     noexcept = 0;

};

//
// Template class LabColor
//

template<typename ValueT>
class LabColor:
    public Color<ValueT>
{

public:

    LabColor( ) noexcept:
        LabColor { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    LabColor( LabColor const& rhs ) noexcept:
        LabColor { rhs._values }
    {
        /*empty*/
    }

    LabColor( LabColor&& rhs ) noexcept:
        LabColor { rhs._values }
    {
        /*empty*/
    }

    LabColor( ValueT const L, ValueT const a, ValueT const b ) noexcept:
        _values { L, a, b }
    {
        /*empty*/
    }

    LabColor( ValueT const channels[3] ) noexcept:
        LabColor { channels[+LabChannels::L], channels[+LabChannels::a], channels[+LabChannels::b] }
    {
        /*empty*/
    }

    LabColor& operator=( LabColor const& rhs ) noexcept {
        _values[+LabChannels::L] = rhs._values[+LabChannels::L];
        _values[+LabChannels::a] = rhs._values[+LabChannels::a];
        _values[+LabChannels::b] = rhs._values[+LabChannels::b];

        return *this;
    }

    LabColor& operator=( LabColor&& rhs ) noexcept {
        _values[+LabChannels::L] = rhs._values[+LabChannels::L];
        _values[+LabChannels::a] = rhs._values[+LabChannels::a];
        _values[+LabChannels::b] = rhs._values[+LabChannels::b];

        return *this;
    }

    virtual ColorSpace constexpr GetColorSpace( ) const noexcept {
        return ColorSpace::Lab;
    }

    virtual int constexpr GetChannelCount( ) const noexcept {
        return 3;
    }

    virtual ValueT GetChannelValue( int const channel ) const noexcept {
        assert( ( channel >= 0 ) && ( channel < 3 ) );
        return _values[channel];
    }

    virtual void SetChannelValue( int const channel, ValueT const value ) noexcept {
        assert( ( channel >= 0 ) && ( channel < 3 ) );
        _values[channel] = value;
    }

    virtual void GetChannelValues( ValueT values[3] ) const noexcept {
        values[+LabChannels::L] = _values[+LabChannels::L];
        values[+LabChannels::a] = _values[+LabChannels::a];
        values[+LabChannels::b] = _values[+LabChannels::b];
    }

    virtual void SetChannelValues( ValueT const values[3] ) noexcept {
        _values[+LabChannels::L] = values[+LabChannels::L];
        _values[+LabChannels::a] = values[+LabChannels::a];
        _values[+LabChannels::b] = values[+LabChannels::b];
    }

protected:

    ValueT _values[3];

};

//
// Template class SrgbColor
//

template<typename ValueT>
class SrgbColor:
    public Color<ValueT>
{

public:

    SrgbColor( ) noexcept:
        SrgbColor { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    SrgbColor( SrgbColor const& rhs ) noexcept:
        SrgbColor { rhs._values }
    {
        /*empty*/
    }

    SrgbColor( SrgbColor&& rhs ) noexcept:
        SrgbColor { rhs._values }
    {
        /*empty*/
    }

    SrgbColor( ValueT const R, ValueT const G, ValueT const B ) noexcept:
        _values { R, G, B }
    {
        /*empty*/
    }

    SrgbColor( ValueT const channels[3] ) noexcept:
        SrgbColor { channels[+SrgbChannels::R], channels[+SrgbChannels::G], channels[+SrgbChannels::B] }
    {
        /*empty*/
    }

    SrgbColor& operator=( SrgbColor const& rhs ) noexcept {
        _values[+SrgbChannels::R] = rhs._values[+SrgbChannels::R];
        _values[+SrgbChannels::G] = rhs._values[+SrgbChannels::G];
        _values[+SrgbChannels::B] = rhs._values[+SrgbChannels::B];

        return *this;
    }

    SrgbColor& operator=( SrgbColor&& rhs ) noexcept {
        _values[+SrgbChannels::R] = rhs._values[+SrgbChannels::R];
        _values[+SrgbChannels::G] = rhs._values[+SrgbChannels::G];
        _values[+SrgbChannels::B] = rhs._values[+SrgbChannels::B];

        return *this;
    }

    virtual ColorSpace constexpr GetColorSpace( ) const noexcept {
        return ColorSpace::sRGB;
    }

    virtual int constexpr GetChannelCount( ) const noexcept {
        return 3;
    }

    virtual ValueT GetChannelValue( int const channel ) const noexcept {
        assert( ( channel >= 0 ) && ( channel < 3 ) );

        return _values[channel];
    }

    virtual void SetChannelValue( int const channel, ValueT const value ) noexcept {
        assert( ( channel >= 0 ) && ( channel < 3 ) );

        _values[channel] = value;
    }

    virtual void GetChannelValues( ValueT values[3] ) const noexcept {
        values[+SrgbChannels::R] = _values[+SrgbChannels::R];
        values[+SrgbChannels::G] = _values[+SrgbChannels::G];
        values[+SrgbChannels::B] = _values[+SrgbChannels::B];
    }

    virtual void SetChannelValues( ValueT const values[3] ) noexcept {
        _values[+SrgbChannels::R] = values[+SrgbChannels::R];
        _values[+SrgbChannels::G] = values[+SrgbChannels::G];
        _values[+SrgbChannels::B] = values[+SrgbChannels::B];
    }

protected:

    ValueT _values[3];

};
