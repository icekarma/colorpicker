#pragma once

//================================================
// Preprocessor macros
//================================================

#undef  LAB_COLOR_USE_FLOAT
#undef  LAB_COLOR_USE_DOUBLE
#define LAB_COLOR_USE_UINT8

#if ( !defined LAB_COLOR_USE_FLOAT && !defined LAB_COLOR_USE_DOUBLE && !defined LAB_COLOR_USE_UINT8 ) || ( ( defined LAB_COLOR_USE_FLOAT && defined LAB_COLOR_USE_DOUBLE ) || ( defined LAB_COLOR_USE_FLOAT && defined LAB_COLOR_USE_UINT8 ) || ( defined LAB_COLOR_USE_DOUBLE && defined LAB_COLOR_USE_UINT8 ) )
#   error Must define only ONE of LAB_COLOR_USE_FLOAT, LAB_COLOR_USE_DOUBLE, or LAB_COLOR_USE_UINT8.
#endif

#if defined LAB_COLOR_USE_FLOAT || defined LAB_COLOR_USE_DOUBLE
#   define FLOATING_POINT_LAB_COLOR
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
using LabValueT = float;
#elif defined LAB_COLOR_USE_DOUBLE
using LabValueT = double;
#elif defined LAB_COLOR_USE_UINT8
using LabValueT = uint8_t;
#endif
using SrgbValueT = uint8_t;

using LabColorValue  = LabColor<LabValueT>;
using SrgbColorValue = SrgbColor<uint8_t>;

template<typename T>
using Triplet = std::array<T, 3>;

//================================================
// Constants
//================================================

int constexpr ImageLabValuesPerPixel  { 3 };
int constexpr ImageSrgbValuesPerPixel { 4 };

cmsUInt32Number constexpr SrgbPixelFormat { TYPE_BGRA_8 };
cmsUInt32Number constexpr  LabPixelFormat {
#if defined LAB_COLOR_USE_FLOAT
    TYPE_Lab_FLT
#elif defined LAB_COLOR_USE_DOUBLE
    TYPE_Lab_DBL
#elif defined LAB_COLOR_USE_UINT8
    TYPE_Lab_8
#endif
};

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
// Function prototypes
//================================================

LabColorValue  ConvertColor( SrgbColorValue const& color );
SrgbColorValue ConvertColor( LabColorValue  const& color );

//================================================
// Methods
//================================================

LabChannels inline constexpr AllChannelsToLabChannels( AllChannels const channel ) {
    return (LabChannels) ( +channel - +AllChannels::LabMin );
}

SrgbChannels inline constexpr AllChannelsToSrgbChannels( AllChannels const channel ) {
    return (SrgbChannels) ( +channel - +AllChannels::SrgbMin );
}

AllChannels inline constexpr LabChannelsToAllChannels( LabChannels const channel ) {
    return (AllChannels) ( +channel + +AllChannels::LabMin );
}

AllChannels inline constexpr SrgbChannelsToAllChannels( SrgbChannels const channel ) {
    return (AllChannels) ( +channel + +AllChannels::SrgbMin );
}

//================================================
// Classes
//================================================

//
// Class TransformsManager
//

class TransformsManager {

public:

    TransformsManager( ) {
        cmsHPROFILE hLabProfile { cmsCreateLab4Profile( cmsD50_xyY( ) ) };
        cmsHPROFILE hRgbProfile { cmsCreate_sRGBProfile( )              };

        _hLabToSrgbTransform = cmsCreateTransform( hLabProfile, LabPixelFormat,  hRgbProfile, SrgbPixelFormat, INTENT_PERCEPTUAL, 0 );
        _hSrgbToLabTransform = cmsCreateTransform( hRgbProfile, SrgbPixelFormat, hLabProfile, LabPixelFormat,  INTENT_PERCEPTUAL, 0 );

        cmsCloseProfile( hLabProfile );
        cmsCloseProfile( hRgbProfile );
    }

    ~TransformsManager( ) {
        if ( _hLabToSrgbTransform ) {
            cmsDeleteTransform( _hLabToSrgbTransform );
            _hLabToSrgbTransform = nullptr;
        }
        if ( _hSrgbToLabTransform ) {
            cmsDeleteTransform( _hSrgbToLabTransform );
            _hSrgbToLabTransform = nullptr;
        }
    }

    cmsHTRANSFORM GetLabToSrgbTransform( ) const { return _hLabToSrgbTransform; }
    cmsHTRANSFORM GetSrgbToLabTransform( ) const { return _hSrgbToLabTransform; }

protected:

    cmsHTRANSFORM _hLabToSrgbTransform { };
    cmsHTRANSFORM _hSrgbToLabTransform { };

};

//
// Template class Color
//

template<typename ValueT>
class Color {

public:

    virtual constexpr ColorSpace      GetColorSpace( )                                                 const noexcept = 0;
    virtual constexpr int             GetChannelCount( )                                               const noexcept = 0;

    virtual constexpr ValueT          GetChannelValue( AllChannels const channel )                     const noexcept = 0;
    virtual constexpr void            SetChannelValue( AllChannels const channel, ValueT const value )       noexcept = 0;

    virtual constexpr Triplet<ValueT> GetChannelValues( )                                              const noexcept = 0;
    virtual constexpr void            SetChannelValues( Triplet<ValueT> const& values )                      noexcept = 0;

};

//
// Template class LabColor
//

template<typename ValueT>
class LabColor:
    public Color<ValueT>
{

public:

    constexpr LabColor( ) noexcept:
        LabColor { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    constexpr LabColor( LabColor const& rhs ) noexcept:
        LabColor { rhs._values }
    {
        /*empty*/
    }

    constexpr LabColor( LabColor&& rhs ) noexcept:
        LabColor { std::move( rhs._values ) }
    {
        /*empty*/
    }

    constexpr LabColor( ValueT const L, ValueT const a, ValueT const b ) noexcept:
        _values { L, a, b }
    {
        /*empty*/
    }

    constexpr LabColor( Triplet<ValueT> const channels ) noexcept:
        LabColor { channels[+LabChannels::L], channels[+LabChannels::a], channels[+LabChannels::b] }
    {
        /*empty*/
    }

    constexpr LabColor& operator=( LabColor const& rhs ) noexcept {
        _values = rhs._values;

        return *this;
    }

    constexpr LabColor& operator=( LabColor&& rhs ) noexcept {
        _values = std::move( rhs._values );

        return *this;
    }

    virtual ColorSpace constexpr GetColorSpace( ) const noexcept override {
        return ColorSpace::Lab;
    }

    virtual int constexpr GetChannelCount( ) const noexcept override {
        return 3;
    }

    constexpr ValueT GetChannelValue( LabChannels const channel ) const noexcept {
        assert( ( channel >= LabChannels::L ) && ( channel <= LabChannels::b ) );
        return _values[+channel];
    }

    constexpr void SetChannelValue( LabChannels const channel, ValueT const value ) noexcept {
        assert( ( channel >= LabChannels::L ) && ( channel <= LabChannels::b ) );
        _values[+channel] = value;
    }

    virtual constexpr ValueT GetChannelValue( AllChannels const channel ) const noexcept override {
        assert( ( channel >= AllChannels::LabL ) && ( channel <= AllChannels::LabB ) );
        return _values[+AllChannelsToLabChannels( channel )];
    }

    virtual constexpr void SetChannelValue( AllChannels const channel, ValueT const value ) noexcept override {
        assert( ( channel >= AllChannels::LabL ) && ( channel <= AllChannels::LabB ) );
        _values[+AllChannelsToLabChannels( channel )] = value;
    }

    virtual constexpr Triplet<ValueT> GetChannelValues( ) const noexcept override {
        return _values;
    }

    virtual constexpr void SetChannelValues( Triplet<ValueT> const& values ) noexcept override {
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

    constexpr SrgbColor( ) noexcept:
        SrgbColor { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    constexpr SrgbColor( SrgbColor const& rhs ) noexcept:
        _values ( rhs._values )
    {
        /*empty*/
    }

    constexpr SrgbColor( SrgbColor&& rhs ) noexcept:
        _values ( std::move( rhs._values ) )
    {
        /*empty*/
    }

    constexpr SrgbColor( ValueT const R, ValueT const G, ValueT const B ) noexcept:
        _values { R, G, B }
    {
        /*empty*/
    }

    constexpr SrgbColor( Triplet<ValueT> const channels ) noexcept:
        SrgbColor { channels[+SrgbChannels::R], channels[+SrgbChannels::G], channels[+SrgbChannels::B] }
    {
        /*empty*/
    }

    constexpr SrgbColor& operator=( SrgbColor const& rhs ) noexcept {
        _values = rhs._values;

        return *this;
    }

    constexpr SrgbColor& operator=( SrgbColor&& rhs ) noexcept {
        _values = std::move( rhs._values );

        return *this;
    }

    virtual ColorSpace constexpr GetColorSpace( ) const noexcept override {
        return ColorSpace::sRGB;
    }

    virtual int constexpr GetChannelCount( ) const noexcept override {
        return 3;
    }

    constexpr ValueT GetChannelValue( SrgbChannels const channel ) const noexcept {
        assert( ( channel >= SrgbChannels::R ) && ( channel <= SrgbChannels::B ) );
        return _values[+channel];
    }

    constexpr void SetChannelValue( SrgbChannels const channel, ValueT const value ) noexcept {
        assert( ( channel >= SrgbChannels::R ) && ( channel <= SrgbChannels::B ) );
        _values[+channel] = value;
    }

    virtual constexpr ValueT GetChannelValue( AllChannels const channel ) const noexcept override {
        assert( ( channel >= AllChannels::SrgbR ) && ( channel <= AllChannels::SrgbB ) );
        return _values[+AllChannelsToSrgbChannels( channel )];
    }

    virtual constexpr void SetChannelValue( AllChannels const channel, ValueT const value ) noexcept override {
        assert( ( channel >= AllChannels::SrgbR ) && ( channel <= AllChannels::SrgbB ) );
        _values[+AllChannelsToSrgbChannels( channel )] = value;
    }

    virtual constexpr Triplet<ValueT> GetChannelValues( ) const noexcept override {
        return _values;
    }

    virtual constexpr void SetChannelValues( Triplet<ValueT> const& values ) noexcept override {
        _values = values;
    }

protected:

    Triplet<ValueT> _values;

};

//================================================
// External data
//================================================

extern TransformsManager Transforms;
