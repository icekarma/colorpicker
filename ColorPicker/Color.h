#pragma once

#include <lcms2.h>

#include <cstdint>

#include <array>

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

template<typename T>
using Triplet = std::array<T, 3>;

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

int constexpr LabMinimumL   {    0 }; int constexpr LabMaximumL  { 100 }; int constexpr LabRangeL  { LabMaximumL  - LabMinimumL  + 1 };
int constexpr LabMinimumA   { -128 }; int constexpr LabMaximumA  { 127 }; int constexpr LabRangeA  { LabMaximumA  - LabMinimumA  + 1 };
int constexpr LabMinimumB   { -128 }; int constexpr LabMaximumB  { 127 }; int constexpr LabRangeB  { LabMaximumB  - LabMinimumB  + 1 };

int constexpr SrgbMinimumR  {    0 }; int constexpr SrgbMaximumR { 255 }; int constexpr SrgbRangeR { SrgbMaximumR - SrgbMinimumR + 1 };
int constexpr SrgbMinimumG  {    0 }; int constexpr SrgbMaximumG { 255 }; int constexpr SrgbRangeG { SrgbMaximumG - SrgbMinimumG + 1 };
int constexpr SrgbMinimumB  {    0 }; int constexpr SrgbMaximumB { 255 }; int constexpr SrgbRangeB { SrgbMaximumB - SrgbMinimumB + 1 };

int constexpr LabMinima[3]  { LabMinimumL,  LabMinimumA,  LabMinimumB  };
int constexpr LabMaxima[3]  { LabMaximumL,  LabMaximumA,  LabMaximumB  };
int constexpr LabRanges[3]  { LabRangeL,    LabRangeA,    LabRangeB    };

int constexpr SrgbMinima[3] { SrgbMinimumR, SrgbMinimumG, SrgbMinimumB };
int constexpr SrgbMaxima[3] { SrgbMaximumR, SrgbMaximumG, SrgbMaximumB };
int constexpr SrgbRanges[3] { SrgbRangeR,   SrgbRangeG,   SrgbRangeB   };

//================================================
// Late #include
//================================================

#include "TransformsManager.h"

//================================================
// Types
//================================================

enum class ColorSpace {
    unknown = -1,
    Lab     =  0,
    sRGB    =  1,
};

enum class LabChannels {
    unknown = -1,
    L       =  0,
    a       =  1,
    b       =  2,
};

enum class SrgbChannels {
    unknown = -1,
    R       =  0,
    G       =  1,
    B       =  2,
};

enum class AllChannels {
    unknown = -1,
    LabL    =  0,
    LabA    =  1,
    LabB    =  2,
    SrgbR   =  3,
    SrgbG   =  4,
    SrgbB   =  5,

    LabMin  = AllChannels::LabL,
    LabMax  = AllChannels::LabB,
    SrgbMin = AllChannels::SrgbR,
    SrgbMax = AllChannels::SrgbB,
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
    Color( Triplet<ValueT> const )                    noexcept { /*empty*/ }

    virtual ColorSpace constexpr GetColorSpace( )   const noexcept = 0;
    virtual int        constexpr GetChannelCount( ) const noexcept = 0;

    virtual void                 GetChannelValues( Triplet<ValueT>& values ) const noexcept = 0;
    virtual void                 SetChannelValues( Triplet<ValueT> const values )  noexcept = 0;

};

//
// Template class LabColor
//

template<typename ValueT>
class LabColor:
    public Color<ValueT>
{

public:

    using PixelT = FloatT;

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
        LabColor { std::move( rhs._values ) }
    {
        /*empty*/
    }

    LabColor( ValueT const L, ValueT const a, ValueT const b ) noexcept:
        _values { L, a, b }
    {
        /*empty*/
    }

    LabColor( Triplet<ValueT> const channels ) noexcept:
        LabColor { channels[+LabChannels::L], channels[+LabChannels::a], channels[+LabChannels::b] }
    {
        /*empty*/
    }

    LabColor& operator=( LabColor const& rhs ) noexcept {
        _values = rhs._values;

        return *this;
    }

    LabColor& operator=( LabColor&& rhs ) noexcept {
        _values = std::move( rhs._values );

        return *this;
    }

    virtual ColorSpace constexpr GetColorSpace( ) const noexcept {
        return ColorSpace::Lab;
    }

    virtual int constexpr GetChannelCount( ) const noexcept {
        return 3;
    }

    virtual ValueT GetChannelValue( LabChannels const channel ) const noexcept {
        assert( ( channel >= LabChannels::L ) && ( channel <= LabChannels::b ) );
        return _values[+channel];
    }

    virtual void SetChannelValue( LabChannels const channel, ValueT const value ) noexcept {
        assert( ( channel >= LabChannels::L ) && ( channel <= LabChannels::b ) );
        _values[+channel] = value;
    }

    virtual void GetChannelValues( Triplet<ValueT>& values ) const noexcept {
        values = _values;
    }

    virtual void SetChannelValues( Triplet<ValueT> const values ) noexcept {
        _values = values;
    }

protected:

    Triplet<ValueT> _values;

};

//
// Template class SrgbColor
//

template<typename ValueT>
class SrgbColor:
    public Color<ValueT>
{

public:

    using PixelT = uint8_t;

    SrgbColor( ) noexcept:
        SrgbColor { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    SrgbColor( SrgbColor const& rhs ) noexcept:
        _values ( rhs._values )
    {
        /*empty*/
    }

    SrgbColor( SrgbColor&& rhs ) noexcept:
        _values ( std::move( rhs._values ) )
    {
        /*empty*/
    }

    SrgbColor( ValueT const R, ValueT const G, ValueT const B ) noexcept:
        _values { R, G, B }
    {
        /*empty*/
    }

    SrgbColor( Triplet<ValueT> const channels ) noexcept:
        SrgbColor { channels[+SrgbChannels::R], channels[+SrgbChannels::G], channels[+SrgbChannels::B] }
    {
        /*empty*/
    }

    SrgbColor& operator=( SrgbColor const& rhs ) noexcept {
        _values = rhs._values;

        return *this;
    }

    SrgbColor& operator=( SrgbColor&& rhs ) noexcept {
        _values = std::move( rhs._values );

        return *this;
    }

    virtual ColorSpace constexpr GetColorSpace( ) const noexcept {
        return ColorSpace::sRGB;
    }

    virtual int constexpr GetChannelCount( ) const noexcept {
        return 3;
    }

    virtual ValueT GetChannelValue( SrgbChannels const channel ) const noexcept {
        assert( ( channel >= SrgbChannels::R ) && ( channel <= SrgbChannels::B ) );
        return _values[+channel];
    }

    virtual void SetChannelValue( SrgbChannels const channel, ValueT const value ) noexcept {
        assert( ( channel >= SrgbChannels::R ) && ( channel <= SrgbChannels::B ) );
        _values[+channel] = value;
    }

    virtual void GetChannelValues( Triplet<ValueT>& values ) const noexcept {
        values = _values;
    }

    virtual void SetChannelValues( Triplet<ValueT> const values ) noexcept {
        _values = values;
    }

protected:

    Triplet<ValueT> _values;

};
