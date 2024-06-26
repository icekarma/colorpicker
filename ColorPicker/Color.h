#pragma once

//================================================
// Forward declarations
//================================================

template<typename ValueT> class  LabColorTemplate;
template<typename ValueT> class SrgbColorTemplate;

//================================================
// Types
//================================================

enum class LabChannels: unsigned {
    L, a, b,

    unknown = ~0u,
    Min     =   L,
    Max     =   b
};

enum class SrgbChannels: unsigned {
    R, G, B,

    unknown = ~0u,
    Min     =   R,
    Max     =   B
};

enum class AllChannels: unsigned {
     LabL,  LabA,  LabB,
    SrgbR, SrgbG, SrgbB,

    unknown =   ~0u,
    Min     =  LabL,
    Max     = SrgbB,
    LabMin  =  LabL,
    LabMax  =  LabB,
    SrgbMin = SrgbR,
    SrgbMax = SrgbB
};

//================================================
// Type aliases
//================================================

using          LabValueT =  int8_t;
using       RawLabValueT = uint8_t;
using         SrgbValueT = uint8_t;

template<typename T>
using            Triplet = std::array<T, 3>;

using         IntTriplet = Triplet<         int>;
using        UINTTriplet = Triplet<        UINT>;
using         LabTriplet = Triplet<   LabValueT>;
using      RawLabTriplet = Triplet<RawLabValueT>;
using        SrgbTriplet = Triplet<  SrgbValueT>;
using AllChannelsTriplet = Triplet< AllChannels>;

using           LabColor =  LabColorTemplate<   LabValueT>;
using        RawLabColor =  LabColorTemplate<RawLabValueT>;
using          SrgbColor = SrgbColorTemplate<  SrgbValueT>;

//================================================
// Constants
//================================================

int             constexpr  ImageLabValuesPerPixel { 3 };
int             constexpr ImageSrgbValuesPerPixel { 4 };

cmsUInt32Number constexpr  LabPixelFormat         { TYPE_Lab_8  };
cmsUInt32Number constexpr SrgbPixelFormat         { TYPE_BGRA_8 };

LPCWSTR         constexpr  LabChannelNames[3]     { L"L*", L"a*", L"b*" };
LPCWSTR         constexpr SrgbChannelNames[3]     { L"R",  L"G",  L"B"  };
LPCWSTR         constexpr  AllChannelNames[6]     { L"L*", L"a*", L"b*", L"R", L"G", L"B" };

//================================================
// Function prototypes
//================================================

[[nodiscard]] inline constexpr bool           IsLabChannel(  AllChannels const channel );
[[nodiscard]] inline constexpr bool           IsLabChannel(  LabChannels const channel );
[[nodiscard]] inline constexpr bool          IsSrgbChannel(  AllChannels const channel );
[[nodiscard]] inline constexpr bool          IsSrgbChannel( SrgbChannels const channel );
[[nodiscard]] inline constexpr bool           IsAllChannel(  AllChannels const channel );

[[nodiscard]] inline constexpr LabChannels    AllChannelsToLabChannels (  AllChannels const channel );
[[nodiscard]] inline constexpr SrgbChannels   AllChannelsToSrgbChannels(  AllChannels const channel );
[[nodiscard]] inline constexpr AllChannels    LabChannelsToAllChannels (  LabChannels const channel );
[[nodiscard]] inline constexpr AllChannels   SrgbChannelsToAllChannels ( SrgbChannels const channel );

[[nodiscard]] inline constexpr RawLabTriplet ScaleLabColor(    LabTriplet const& values );
[[nodiscard]] inline constexpr LabTriplet    ScaleLabColor( RawLabTriplet const& values );
[[nodiscard]] inline constexpr RawLabColor   ScaleLabColor(      LabColor const& color  );
[[nodiscard]] inline constexpr LabColor      ScaleLabColor(   RawLabColor const& color  );

[[nodiscard]] inline constexpr int           ScaleLabColor( AllChannels const channel, int const value );

[[nodiscard]] inline constexpr LPCWSTR       ToString(  LabChannels const channel );
[[nodiscard]] inline constexpr LPCWSTR       ToString( SrgbChannels const channel );
[[nodiscard]] inline constexpr LPCWSTR       ToString(  AllChannels const channel );

//================================================
// Classes
//================================================

//
// Abstract base template class Color
//

template<typename ValueT>
class ColorTemplate {

public:

                  virtual constexpr                 ~ColorTemplate( )                                                      noexcept = default;

                  virtual constexpr bool            operator==( ColorTemplate<ValueT> const& rhs )                   const noexcept = 0;
                  virtual constexpr bool            operator!=( ColorTemplate<ValueT> const& rhs )                   const noexcept = 0;

    [[nodiscard]] virtual constexpr                 operator Triplet<ValueT>( )                                      const noexcept = 0;

    [[nodiscard]] virtual constexpr int             GetChannelCount( )                                               const noexcept = 0;

    [[nodiscard]] virtual constexpr ValueT          GetChannelValue( AllChannels const channel )                     const noexcept = 0;
                  virtual constexpr void            SetChannelValue( AllChannels const channel, ValueT const value )       noexcept = 0;

    [[nodiscard]] virtual constexpr Triplet<ValueT> GetChannelValues( )                                              const noexcept = 0;
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

    template<typename ForeignT>
    constexpr LabColorTemplate( LabColorTemplate<ForeignT> const& rhs ) noexcept:
        LabColorTemplate { ScaleLabColor( rhs ) }
    {
        /*empty*/
    }

    constexpr LabColorTemplate( Triplet<ValueT> const& values ) noexcept:
        LabColorTemplate { values[+LabChannels::L], values[+LabChannels::a], values[+LabChannels::b] }
    {
        /*empty*/
    }

    template<typename ForeignT>
    constexpr LabColorTemplate( Triplet<ForeignT> const& values ) noexcept:
        LabColorTemplate { ScaleLabColor( values ) }
    {
        /*empty*/
    }

    constexpr LabColorTemplate( ValueT const L, ValueT const a, ValueT const b ) noexcept:
        _values { { L, a, b } }
    {
        /*empty*/
    }

    constexpr LabColorTemplate( int const L, int const a, int const b ) noexcept:
        _values { { static_cast<ValueT>( L ), static_cast<ValueT>( a ), static_cast<ValueT>( b ) } }
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

    template<typename ForeignT>
    constexpr LabColorTemplate& operator=( LabColorTemplate<ForeignT> const& rhs ) noexcept {
        _values = ScaleLabColor( rhs._values );

        return *this;
    }

    constexpr LabColorTemplate& operator=( Triplet<ValueT> const& values ) noexcept {
        _values = values;

        return *this;
    }

    template<typename ForeignT>
    constexpr LabColorTemplate& operator=( Triplet<ForeignT> const& values ) noexcept {
        _values = ScaleLabColor( values );

        return *this;
    }

    virtual constexpr bool operator==( ColorTemplate<ValueT> const& rhs ) const noexcept override {
        return
            ( GetChannelCount( ) == rhs.GetChannelCount( )  ) &&
            ( _values            == rhs.GetChannelValues( ) )
            ;
    }

    virtual constexpr bool operator!=( ColorTemplate<ValueT> const& rhs ) const noexcept override {
        return
            ( GetChannelCount( ) != rhs.GetChannelCount( )  ) ||
            ( _values            != rhs.GetChannelValues( ) )
            ;
    }

    constexpr bool operator==( LabColorTemplate<ValueT> const& rhs ) const noexcept {
        return _values == rhs._values;
    }

    constexpr bool operator!=( LabColorTemplate<ValueT> const& rhs ) const noexcept {
        return _values != rhs._values;
    }

    virtual constexpr operator Triplet<ValueT>( ) const noexcept override {
        return _values;
    }

    template<typename ForeignT>
    constexpr operator LabColorTemplate<ForeignT>( ) const noexcept {
        return { ScaleLabColor( _values ) };
    }

    template<typename ForeignT>
    constexpr operator Triplet<ForeignT>( ) const noexcept {
        return ScaleLabColor( _values );
    }

    [[nodiscard]] virtual int constexpr GetChannelCount( ) const noexcept override {
        return 3;
    }

    [[nodiscard]] constexpr ValueT GetChannelValue( LabChannels const channel ) const noexcept {
        assert( IsLabChannel( channel ) );
        return _values[+channel];
    }

    constexpr void SetChannelValue( LabChannels const channel, ValueT const value ) noexcept {
        assert( IsLabChannel( channel ) );
        _values[+channel] = value;
    }

    [[nodiscard]] virtual constexpr ValueT GetChannelValue( AllChannels const channel ) const noexcept override {
        assert( IsLabChannel( channel ) );
        return _values[+AllChannelsToLabChannels( channel )];
    }

    virtual constexpr void SetChannelValue( AllChannels const channel, ValueT const value ) noexcept override {
        assert( IsLabChannel( channel ) );
        _values[+AllChannelsToLabChannels( channel )] = value;
    }

    [[nodiscard]] virtual constexpr Triplet<ValueT> GetChannelValues( ) const noexcept override {
        return _values;
    }

    template<typename ForeignT>
    [[nodiscard]] constexpr Triplet<ForeignT> GetChannelValues( ) const noexcept {
        return ScaleLabColor<ForeignT>( _values );
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
        _values { { R, G, B } }
    {
        /*empty*/
    }

    constexpr SrgbColorTemplate( int const R, int const G, int const B ) noexcept:
        _values { { static_cast<SrgbValueT>( R ), static_cast<SrgbValueT>( G ), static_cast<SrgbValueT>( B ) } }
    {
        /*empty*/
    }

    constexpr SrgbColorTemplate( Triplet<ValueT> const values ) noexcept:
        SrgbColorTemplate { values[+SrgbChannels::R], values[+SrgbChannels::G], values[+SrgbChannels::B] }
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

    virtual constexpr bool operator==( ColorTemplate<ValueT> const& rhs ) const noexcept override {
        return
            ( GetChannelCount( ) == rhs.GetChannelCount( )  ) &&
            ( _values            == rhs.GetChannelValues( ) )
            ;
    }

    virtual constexpr bool operator!=( ColorTemplate<ValueT> const& rhs ) const noexcept override {
        return
            ( GetChannelCount( ) != rhs.GetChannelCount( )  ) ||
            ( _values            != rhs.GetChannelValues( ) )
            ;
    }

    constexpr bool operator==( SrgbColorTemplate<ValueT> const& rhs ) const noexcept {
        return _values == rhs._values;
    }

    constexpr bool operator!=( SrgbColorTemplate<ValueT> const& rhs ) const noexcept {
        return _values != rhs._values;
    }

    virtual constexpr operator Triplet<ValueT>( ) const noexcept override {
        return _values;
    }

    constexpr explicit operator COLORREF( ) const noexcept {
        return RGB( _values[+SrgbChannels::R], _values[+SrgbChannels::G], _values[+SrgbChannels::B] );
    }

    virtual int constexpr GetChannelCount( ) const noexcept override {
        return 3;
    }

    [[nodiscard]] constexpr ValueT GetChannelValue( SrgbChannels const channel ) const noexcept {
        assert( IsSrgbChannel( channel ) );
        return _values[+channel];
    }

    [[nodiscard]] constexpr void SetChannelValue( SrgbChannels const channel, ValueT const value ) noexcept {
        assert( IsSrgbChannel( channel ) );
        _values[+channel] = value;
    }

    [[nodiscard]] virtual constexpr ValueT GetChannelValue( AllChannels const channel ) const noexcept override {
        assert( IsSrgbChannel( channel ) );
        return _values[+AllChannelsToSrgbChannels( channel )];
    }

    [[nodiscard]] virtual constexpr void SetChannelValue( AllChannels const channel, ValueT const value ) noexcept override {
        assert( IsSrgbChannel( channel ) );
        _values[+AllChannelsToSrgbChannels( channel )] = value;
    }

    [[nodiscard]] virtual constexpr Triplet<ValueT> GetChannelValues( ) const noexcept override {
        return _values;
    }

    [[nodiscard]] virtual constexpr void SetChannelValues( Triplet<ValueT> const& values ) noexcept override {
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

    [[nodiscard]] cmsHTRANSFORM GetLabToSrgbTransform( ) const { return _hLabToSrgbTransform; }
    [[nodiscard]] cmsHTRANSFORM GetSrgbToLabTransform( ) const { return _hSrgbToLabTransform; }

    void TransformLabToSrgb( void const* pInputBuffer, void* pOutputBuffer, unsigned cPixels ) {
        cmsDoTransform( _hLabToSrgbTransform, pInputBuffer, pOutputBuffer, cPixels );
    }

    void TransformSrgbToLab( void const* pInputBuffer, void* pOutputBuffer, unsigned cPixels ) {
        cmsDoTransform( _hSrgbToLabTransform, pInputBuffer, pOutputBuffer, cPixels );
    }

    [[nodiscard]] SrgbColor ConvertColor( LabColor const& color ) {
        SrgbValueT    srgbValues[ImageSrgbValuesPerPixel];
        RawLabTriplet labValues { ScaleLabColor( color.GetChannelValues( ) ) };

        TransformLabToSrgb( labValues.data( ), srgbValues, 1 );
        return { srgbValues[2], srgbValues[1], srgbValues[0] };
    }

    [[nodiscard]] LabColor ConvertColor( SrgbColor const& color ) {
        RawLabValueT labValues[ImageLabValuesPerPixel];
        SrgbTriplet  srgbValues { color.GetChannelValues( ) };

        TransformSrgbToLab( srgbValues.data( ), labValues, 1 );
        return ScaleLabColor( RawLabTriplet { { labValues[0], labValues[1], labValues[2] } } );
    }

protected:

    cmsHTRANSFORM _hLabToSrgbTransform;
    cmsHTRANSFORM _hSrgbToLabTransform;

};

//================================================
// Function definitions
//================================================

[[nodiscard]] inline constexpr bool IsLabChannel( AllChannels const channel ) {
    return /*( channel >= AllChannels::LabMin ) &&*/ ( channel <= AllChannels::LabMax );
}

[[nodiscard]] inline constexpr bool IsLabChannel( LabChannels const channel ) {
    return /*( channel >= LabChannels::Min ) &&*/ ( channel <= LabChannels::Max );
}

[[nodiscard]] inline constexpr bool IsSrgbChannel( AllChannels const channel ) {
    return ( channel >= AllChannels::SrgbMin ) && ( channel <= AllChannels::SrgbMax );
}

[[nodiscard]] inline constexpr bool IsSrgbChannel( SrgbChannels const channel ) {
    return /*( channel >= SrgbChannels::Min ) &&*/ ( channel <= SrgbChannels::Max );
}

[[nodiscard]] inline constexpr bool IsAllChannel( AllChannels const channel ) {
    return /*( channel >= AllChannels::Min ) &&*/ ( channel <= AllChannels::Max );
}

[[nodiscard]] inline constexpr LabChannels AllChannelsToLabChannels( AllChannels const channel ) {
    return IsLabChannel( channel ) ? static_cast<LabChannels>( +channel /*- +AllChannels::LabMin*/ ) : LabChannels::unknown;
}

[[nodiscard]] inline constexpr SrgbChannels AllChannelsToSrgbChannels( AllChannels const channel ) {
    return IsSrgbChannel( channel ) ? static_cast<SrgbChannels>( +channel - +AllChannels::SrgbMin ) : SrgbChannels::unknown;
}

[[nodiscard]] inline constexpr AllChannels LabChannelsToAllChannels( LabChannels const channel ) {
    return IsLabChannel( channel ) ? static_cast<AllChannels>( +channel /*+ +AllChannels::LabMin*/ ) : AllChannels::unknown;
}

[[nodiscard]] inline constexpr AllChannels SrgbChannelsToAllChannels( SrgbChannels const channel ) {
    return IsSrgbChannel( channel ) ? static_cast<AllChannels>( +channel + +AllChannels::SrgbMin ) : AllChannels::unknown;
}

[[nodiscard]] inline constexpr RawLabTriplet ScaleLabColor( LabTriplet const& values ) {
    return { {
        static_cast<RawLabValueT>( static_cast<int>( values[+LabChannels::L] ) * 255 / 100 ),
        static_cast<RawLabValueT>( static_cast<int>( values[+LabChannels::a] ) + 128       ),
        static_cast<RawLabValueT>( static_cast<int>( values[+LabChannels::b] ) + 128       )
    } };
}

[[nodiscard]] inline constexpr LabTriplet ScaleLabColor( RawLabTriplet const& values ) {
    return { {
        static_cast<LabValueT>( static_cast<int>( static_cast<unsigned>( values[+LabChannels::L] ) ) * 100 / 255 ),
        static_cast<LabValueT>( static_cast<int>( static_cast<unsigned>( values[+LabChannels::a] ) ) - 128       ),
        static_cast<LabValueT>( static_cast<int>( static_cast<unsigned>( values[+LabChannels::b] ) ) - 128       )
    } };
}

[[nodiscard]] inline constexpr RawLabColor ScaleLabColor( LabColor const& color ) {
    return RawLabColor { ScaleLabColor( color.GetChannelValues( ) ) };
}

[[nodiscard]] inline constexpr LabColor ScaleLabColor( RawLabColor const& color ) {
    return LabColor { ScaleLabColor( color.GetChannelValues( ) ) };
}

[[nodiscard]] inline constexpr int ScaleLabColor( AllChannels const channel, int const value ) {
    switch ( channel ) {
        case AllChannels::LabL:
            return value * 100 / 255;

        case AllChannels::LabA:
        case AllChannels::LabB:
            return value - 128;

        default:
            return value;
    }
}

[[nodiscard]] inline constexpr LPCWSTR ToString( LabChannels const channel ) {
    return
        ( channel == LabChannels::unknown ) ? L"unknown"                :
        IsLabChannel( channel )             ? LabChannelNames[+channel] :
        nullptr;
}

[[nodiscard]] inline constexpr LPCWSTR ToString( SrgbChannels const channel ) {
    return
        ( channel == SrgbChannels::unknown ) ? L"unknown"                 :
        IsSrgbChannel( channel )             ? SrgbChannelNames[+channel] :
        nullptr;
}

[[nodiscard]] inline constexpr LPCWSTR ToString( AllChannels const channel ) {
    return
        ( channel == AllChannels::unknown ) ? L"unknown"                :
        IsAllChannel( channel )             ? AllChannelNames[+channel] :
        nullptr;
}

//================================================
// External data
//================================================

extern TransformsManager g_Transforms;
