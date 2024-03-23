#pragma once

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

using LabValueT      =  int8_t;
using RawLabValueT   = uint8_t;
using SrgbValueT     = uint8_t;

using LabColorValue  = LabColor<LabValueT>;
using SrgbColorValue = SrgbColor<SrgbValueT>;

template<typename T>
using Triplet        = std::array<T, 3>;

//================================================
// Constants
//================================================

int             constexpr  ImageLabValuesPerPixel { 3 };
int             constexpr ImageSrgbValuesPerPixel { 4 };

cmsUInt32Number constexpr  LabPixelFormat         { TYPE_Lab_8  };
cmsUInt32Number constexpr SrgbPixelFormat         { TYPE_BGRA_8 };

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
    LabL    =  0, LabA    =  1, LabB    =  2,
    SrgbR   =  3, SrgbG   =  4, SrgbB   =  5,

    unknown = -1,
    LabMin  = AllChannels::LabL,
    LabMax  = AllChannels::LabB,
    SrgbMin = AllChannels::SrgbR,
    SrgbMax = AllChannels::SrgbB,
};

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

Triplet<RawLabValueT> inline constexpr ScaleLabColor( Triplet<LabValueT> const& channels ) {
    return {
        static_cast<RawLabValueT>( static_cast<int>( channels[0] ) * 255 / 100 ),
        static_cast<RawLabValueT>( static_cast<int>( channels[1] ) + 128       ),
        static_cast<RawLabValueT>( static_cast<int>( channels[2] ) + 128       )
    };
}

Triplet<LabValueT> inline constexpr ScaleLabColor( Triplet<RawLabValueT> const& channels ) {
    return {
        static_cast<LabValueT>( static_cast<int>( static_cast<unsigned>( channels[0] ) ) * 100 / 255 ),
        static_cast<LabValueT>( static_cast<int>( static_cast<unsigned>( channels[1] ) ) - 128       ),
        static_cast<LabValueT>( static_cast<int>( static_cast<unsigned>( channels[2] ) ) - 128       )
    };
}

//================================================
// Classes
//================================================

//
// Abstract base template class Color
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

    constexpr LabColor( int const L, int const a, int const b ) noexcept:
        _values { static_cast<LabValueT>( L ), static_cast<LabValueT>( a ), static_cast<LabValueT>( b ) }
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

    constexpr SrgbColor( int const R, int const G, int const B ) noexcept:
        _values { static_cast<SrgbValueT>( R ), static_cast<SrgbValueT>( G ), static_cast<SrgbValueT>( B ) }
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

    void TransformLabToSrgb( void const* pInputBuffer, void* pOutputBuffer, unsigned cPixels ) {
        cmsDoTransform( _hLabToSrgbTransform, pInputBuffer, pOutputBuffer, cPixels );
    }

    void TransformSrgbToLab( void const* pInputBuffer, void* pOutputBuffer, unsigned cPixels ) {
        cmsDoTransform( _hSrgbToLabTransform, pInputBuffer, pOutputBuffer, cPixels );
    }

    SrgbColorValue ConvertColor( LabColorValue const& color ) {
        SrgbValueT            srgbValues[ImageSrgbValuesPerPixel];
        Triplet<RawLabValueT> labValues { ScaleLabColor( color.GetChannelValues( ) ) };

        TransformLabToSrgb( labValues.data( ), srgbValues, 1 );
        return { srgbValues[2], srgbValues[1], srgbValues[0] };
    }

    LabColorValue ConvertColor( SrgbColorValue const& color ) {
        RawLabValueT        labValues[ImageLabValuesPerPixel];
        Triplet<SrgbValueT> srgbValues { color.GetChannelValues( ) };

        TransformSrgbToLab( srgbValues.data( ), labValues, 1 );
        return ScaleLabColor( Triplet<RawLabValueT> { labValues[0], labValues[1], labValues[2] } );
    }

protected:

    cmsHTRANSFORM _hLabToSrgbTransform { };
    cmsHTRANSFORM _hSrgbToLabTransform { };

};

//================================================
// External data
//================================================

extern TransformsManager Transforms;
