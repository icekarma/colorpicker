#pragma once

//================================================
// Forward declarations
//================================================

template<typename ValueT> class  LabColorTemplate;
template<typename ValueT> class SrgbColorTemplate;

//================================================
// Type aliases
//================================================

using    LabValueT =  int8_t;
using RawLabValueT = uint8_t;
using   SrgbValueT = uint8_t;

template<typename T>
using      Triplet = std::array<T, 3>;

using    LabColor  =  LabColorTemplate<   LabValueT>;
using RawLabColor  =  LabColorTemplate<RawLabValueT>;
using   SrgbColor  = SrgbColorTemplate<  SrgbValueT>;

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
// Function prototypes
//================================================

LabChannels           inline constexpr  AllChannelsToLabChannels ( AllChannels const channel );
SrgbChannels          inline constexpr  AllChannelsToSrgbChannels( AllChannels const channel );

AllChannels           inline constexpr  LabChannelsToAllChannels(  LabChannels const channel );
AllChannels           inline constexpr SrgbChannelsToAllChannels( SrgbChannels const channel );

Triplet<RawLabValueT> inline constexpr ScaleLabColor( Triplet<   LabValueT> const& channels );
Triplet<LabValueT>    inline constexpr ScaleLabColor( Triplet<RawLabValueT> const& channels );

RawLabColor           inline constexpr ScaleLabColor(    LabColor const& color );
LabColor              inline constexpr ScaleLabColor( RawLabColor const& color );

//================================================
// Classes
//================================================

//
// Abstract base template class Color
//

template<typename ValueT>
class ColorTemplate {

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
class LabColorTemplate:
    public ColorTemplate<ValueT>
{

public:

    constexpr LabColorTemplate( ) noexcept:
        LabColorTemplate { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    constexpr LabColorTemplate( LabColorTemplate const& rhs ) noexcept:
        LabColorTemplate { rhs._values }
    {
        /*empty*/
    }

    constexpr LabColorTemplate( LabColorTemplate&& rhs ) noexcept:
        LabColorTemplate { std::move( rhs._values ) }
    {
        /*empty*/
    }

    constexpr LabColorTemplate( ValueT const L, ValueT const a, ValueT const b ) noexcept:
        _values { L, a, b }
    {
        /*empty*/
    }

    constexpr LabColorTemplate( int const L, int const a, int const b ) noexcept:
        _values { static_cast<LabValueT>( L ), static_cast<LabValueT>( a ), static_cast<LabValueT>( b ) }
    {
        /*empty*/
    }

    constexpr LabColorTemplate( Triplet<ValueT> const channels ) noexcept:
        LabColorTemplate { channels[+LabChannels::L], channels[+LabChannels::a], channels[+LabChannels::b] }
    {
        /*empty*/
    }

    constexpr LabColorTemplate& operator=( LabColorTemplate const& rhs ) noexcept {
        _values = rhs._values;

        return *this;
    }

    constexpr LabColorTemplate& operator=( LabColorTemplate&& rhs ) noexcept {
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
class SrgbColorTemplate:
    public ColorTemplate<ValueT>
{

public:

    constexpr SrgbColorTemplate( ) noexcept:
        SrgbColorTemplate { static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ), static_cast<ValueT>( 0 ) }
    {
        /*empty*/
    }

    constexpr SrgbColorTemplate( SrgbColorTemplate const& rhs ) noexcept:
        _values ( rhs._values )
    {
        /*empty*/
    }

    constexpr SrgbColorTemplate( SrgbColorTemplate&& rhs ) noexcept:
        _values ( std::move( rhs._values ) )
    {
        /*empty*/
    }

    constexpr SrgbColorTemplate( ValueT const R, ValueT const G, ValueT const B ) noexcept:
        _values { R, G, B }
    {
        /*empty*/
    }

    constexpr SrgbColorTemplate( int const R, int const G, int const B ) noexcept:
        _values { static_cast<SrgbValueT>( R ), static_cast<SrgbValueT>( G ), static_cast<SrgbValueT>( B ) }
    {
        /*empty*/
    }

    constexpr SrgbColorTemplate( Triplet<ValueT> const channels ) noexcept:
        SrgbColorTemplate { channels[+SrgbChannels::R], channels[+SrgbChannels::G], channels[+SrgbChannels::B] }
    {
        /*empty*/
    }

    constexpr SrgbColorTemplate& operator=( SrgbColorTemplate const& rhs ) noexcept {
        _values = rhs._values;

        return *this;
    }

    constexpr SrgbColorTemplate& operator=( SrgbColorTemplate&& rhs ) noexcept {
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

    SrgbColor ConvertColor( LabColor const& color ) {
        SrgbValueT            srgbValues[ImageSrgbValuesPerPixel];
        Triplet<RawLabValueT> labValues { ScaleLabColor( color.GetChannelValues( ) ) };

        TransformLabToSrgb( labValues.data( ), srgbValues, 1 );
        return { srgbValues[2], srgbValues[1], srgbValues[0] };
    }

    LabColor ConvertColor( SrgbColor const& color ) {
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
// Function definitions
//================================================

LabChannels inline constexpr AllChannelsToLabChannels( AllChannels const channel ) {
    return static_cast<LabChannels>( +channel - +AllChannels::LabMin );
}

SrgbChannels inline constexpr AllChannelsToSrgbChannels( AllChannels const channel ) {
    return static_cast<SrgbChannels>( +channel - +AllChannels::SrgbMin );
}

AllChannels inline constexpr LabChannelsToAllChannels( LabChannels const channel ) {
    return static_cast<AllChannels>( +channel + +AllChannels::LabMin );
}

AllChannels inline constexpr SrgbChannelsToAllChannels( SrgbChannels const channel ) {
    return static_cast<AllChannels>( +channel + +AllChannels::SrgbMin );
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

RawLabColor inline constexpr ScaleLabColor( LabColor const& color ) {
    return RawLabColor { ScaleLabColor( color.GetChannelValues( ) ) };
}

LabColor inline constexpr ScaleLabColor( RawLabColor const& color ) {
    return LabColor { ScaleLabColor( color.GetChannelValues( ) ) };
}

//================================================
// External data
//================================================

extern TransformsManager Transforms;
