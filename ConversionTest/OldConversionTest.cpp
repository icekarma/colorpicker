#include <cstdio>

#include <array>

// D50 and D65 white points
double const D50_X = 0.9649;
double const D50_Y = 1.0000;
double const D50_Z = 0.8251;

double const D65_X = 0.9505;
double const D65_Y = 1.0000;
double const D65_Z = 1.0890;

// Conversion constants
double const epsilon = 21.0 / 256.0;
double const kappa = 24.0 / 116.0;
double const t0 = 0.008856;
double const g = 4.0;

// Types
using PixelValue = std::array<double, 3>;

// Prototypes
PixelValue LabToXYZ( PixelValue const& lab );
PixelValue XYZToLab( PixelValue const& xyz );
PixelValue SRGBToXYZ( PixelValue const& srgb );
PixelValue XYZToSRGB( PixelValue const& xyz );
PixelValue XYZAdaptD50ToD65( PixelValue const& xyz );
PixelValue XYZAdaptD65ToD50( PixelValue const& xyz );

PixelValue LabToSRGB( PixelValue const& lab );
PixelValue SRGBToLab( PixelValue const& srgb );

// Function to convert CIELAB to XYZ
PixelValue LabToXYZ( PixelValue const& lab ) {
    double y = ( lab[0] + 16.0 ) / 116.0;
    double x = lab[1] / 500.0 + y;
    double z = y - lab[2] / 200.0;

    x = ( x > t0 ) ? pow( ( x + kappa ), g ) : ( x / kappa );
    y = ( y > t0 ) ? pow( ( y + kappa ), g ) : ( y / kappa );
    z = ( z > t0 ) ? pow( ( z + kappa ), g ) : ( z / kappa );

    return { D50_X * x, D50_Y * y, D50_Z * z };
}

// Function to convert XYZ to CIELAB
PixelValue XYZToLab( PixelValue const& xyz ) {
    // Convert XYZ to CIELAB using D50 white point
    double x = xyz[0] / D50_X;
    double y = xyz[1] / D50_Y;
    double z = xyz[2] / D50_Z;

    x = ( x > 0.008856 ) ? pow( x, 1.0 / 3.0 ) : ( 7.787 * x + 16.0 / 116.0 );
    y = ( y > 0.008856 ) ? pow( y, 1.0 / 3.0 ) : ( 7.787 * y + 16.0 / 116.0 );
    z = ( z > 0.008856 ) ? pow( z, 1.0 / 3.0 ) : ( 7.787 * z + 16.0 / 116.0 );

    double L = 116.0 * y - 16.0;
    double a = 500.0 * ( x - y );
    double b = 200.0 * ( y - z );

    return { L, a, b };
}

#pragma warning( push )
#pragma warning( disable: 4459 )
// Function to convert sRGB to XYZ (inverse of XYZToSRGB)
PixelValue SRGBToXYZ( PixelValue const& srgb ) {
    double r = srgb[0] / 255.0;
    double g = srgb[1] / 255.0;
    double b = srgb[2] / 255.0;

    r = ( r <= 0.04045 ) ? r / 12.92 : pow( ( r + 0.055 ) / 1.055, 2.4 );
    g = ( g <= 0.04045 ) ? g / 12.92 : pow( ( g + 0.055 ) / 1.055, 2.4 );
    b = ( b <= 0.04045 ) ? b / 12.92 : pow( ( b + 0.055 ) / 1.055, 2.4 );

    return { D65_X * r, D65_Y * g, D65_Z * b };
}

// Function to convert XYZ to sRGB
PixelValue XYZToSRGB( PixelValue const& xyz ) {
    double r =  3.2404542 * xyz[0] - 1.5371385 * xyz[1] - 0.4985356 * xyz[2];
    double g = -0.9692562 * xyz[0] + 1.8759616 * xyz[1] + 0.0915525 * xyz[2];
    double b =  0.0556434 * xyz[0] - 0.2040211 * xyz[1] + 1.1496244 * xyz[2];

    r = ( r > 0.0 ) ? pow( r, 1.0 / 2.2 ) : 0.0;
    g = ( g > 0.0 ) ? pow( g, 1.0 / 2.2 ) : 0.0;
    b = ( b > 0.0 ) ? pow( b, 1.0 / 2.2 ) : 0.0;

    return { r * 255.0, g * 255.0, b * 255.0 };
}
#pragma warning( pop )

// Function to adapt XYZ from D50 to D65
PixelValue XYZAdaptD50ToD65( PixelValue const& xyz ) {
    // Bradford adaptation matrix (D50 to D65)
    double static const M[3][3] = {
        { 0.9525, -0.02497,  0.0688 },
        { 0.3138,  0.9502,  -0.1129 },
        { 0.1805,  0.0177,   0.8018 }
    };

    PixelValue result;
    for ( int i = 0; i < 3; ++i ) {
        result[i] = 0.0;
        for ( int j = 0; j < 3; ++j ) {
            result[i] += M[i][j] * xyz[j];
        }
    }

    return result;
}

// Function to adapt XYZ from D65 to D50
PixelValue XYZAdaptD65ToD50( PixelValue const& xyz ) {
    // Inverse Bradford adaptation matrix (D65 to D50)
    double static const M[3][3] = {
        {  1.0479,   0.02296, -0.01488 },
        { -0.3545,   1.1117,   0.1471  },
        {  0.08813, -0.09504,  1.008   }
    };

    PixelValue result;
    for ( int i = 0; i < 3; ++i ) {
        result[i] = 0.0;
        for ( int j = 0; j < 3; ++j ) {
            result[i] += M[i][j] * xyz[j];
        }
    }
    return result;
}

// Function to convert D50 Lab to D65 sRGB
PixelValue LabToSRGB( PixelValue const& lab ) {
    // Convert CIELAB to XYZ
    PixelValue xyz_D50 = LabToXYZ( lab );

    // Adapt XYZ from D50 to D65
    PixelValue xyz_D65 = XYZAdaptD50ToD65( xyz_D50 );

    // Convert XYZ to sRGB
    return XYZToSRGB( xyz_D65 );
}

// Function to convert D65 sRGB to D50 Lab
PixelValue SRGBToLab( PixelValue const& srgb ) {
    // Convert SRGB to XYZ
    PixelValue xyz_D65 = SRGBToXYZ( srgb );

    // Adapt XYZ from D65 to D50
    PixelValue xyz_D50 = XYZAdaptD65ToD50( xyz_D65 );

    // Convert XYZ to CIELAB
    return XYZToLab( xyz_D50 );
}

int main( ) {
    //PixelValue srgb = LabToSRGB( { 50.0, 10.0, 20.0 } );
    //printf( "sRGB: (%d, %d, %d)\n",
    //    static_cast<int>( round( srgb[0] ) ),
    //    static_cast<int>( round( srgb[1] ) ),
    //    static_cast<int>( round( srgb[2] ) )
    //);

    PixelValue lab, srgb;

    lab = SRGBToLab( { 255.0, 255.0, 255.0 } );
    printf( "Lab:  (%d, %d, %d)\n", static_cast<int>( round( lab[0] ) ), static_cast<int>( round( lab[1] ) ), static_cast<int>( round( lab[2] ) ) );

    lab = SRGBToLab( { 0.0, 0.0, 0.0 } );
    printf( "Lab:  (%d, %d, %d)\n", static_cast<int>( round( lab[0] ) ), static_cast<int>( round( lab[1] ) ), static_cast<int>( round( lab[2] ) ) );

    srgb = LabToSRGB( { 100.0, 0.0, 0.0 } );
    printf( "sRGB: (%d, %d, %d)\n", static_cast<int>( round( srgb[0] ) ), static_cast<int>( round( srgb[1] ) ), static_cast<int>( round( srgb[2] ) ) );

    srgb = LabToSRGB( { 0.0, 0.0, 0.0 } );
    printf( "sRGB: (%d, %d, %d)\n", static_cast<int>( round( srgb[0] ) ), static_cast<int>( round( srgb[1] ) ), static_cast<int>( round( srgb[2] ) ) );

    srgb = LabToSRGB( { 100.0, 100.0, 0.0 } );
    printf( "sRGB: (%d, %d, %d)\n", static_cast<int>( round( srgb[0] ) ), static_cast<int>( round( srgb[1] ) ), static_cast<int>( round( srgb[2] ) ) );

    srgb = LabToSRGB( { 100.0, -100.0, 0.0 } );
    printf( "sRGB: (%d, %d, %d)\n", static_cast<int>( round( srgb[0] ) ), static_cast<int>( round( srgb[1] ) ), static_cast<int>( round( srgb[2] ) ) );

    srgb = LabToSRGB( { 100.0, 0.0, 100.0 } );
    printf( "sRGB: (%d, %d, %d)\n", static_cast<int>( round( srgb[0] ) ), static_cast<int>( round( srgb[1] ) ), static_cast<int>( round( srgb[2] ) ) );

    srgb = LabToSRGB( { 100.0, 0.0, -100.0 } );
    printf( "sRGB: (%d, %d, %d)\n", static_cast<int>( round( srgb[0] ) ), static_cast<int>( round( srgb[1] ) ), static_cast<int>( round( srgb[2] ) ) );
}
