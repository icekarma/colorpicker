#include <lcms2.h>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <string>

namespace {

    std::string OutputFilePath { "TestSet4" };

    template<typename T>
    T constexpr _ce_abs( T value ) {
        return ( value < static_cast<T>( 0 ) ) ? -value : value;
    }

    double constexpr LabMinimumA { -128.0 };
    double constexpr LabMaximumA {  128.0 };
    double constexpr LabMinimumB { -128.0 };
    double constexpr LabMaximumB {  128.0 };

    int    constexpr ImageWidth  { _ce_abs( static_cast<int>( LabMinimumA ) ) + _ce_abs( static_cast<int>( LabMaximumA ) ) };
    int    constexpr ImageHeight { _ce_abs( static_cast<int>( LabMinimumB ) ) + _ce_abs( static_cast<int>( LabMaximumB ) ) };

    double* GenerateLabImage( double* image, double const L ) {
        int constexpr minA { static_cast<int>( LabMinimumA ) };
        int constexpr minB { static_cast<int>( LabMinimumB ) };
        double*       ptr  { image };

        for ( int y { }; y < ImageHeight; ++y ) {
            for ( int x { }; x < ImageWidth; ++x ) {
                *ptr++ = L;
                *ptr++ = x + minA;
                *ptr++ = y + minB;
            }
        }

        return image;
    }

    bool WriteImageToFile( std::string fileName, uint8_t const* image, size_t const bytesToWrite, int const width, int const height ) {
        FILE* f { fopen( fileName.c_str( ), "wb" ) };
        if ( !f ) {
            fprintf( stderr, "Couldn't create output file\n" );
            return false;
        }

        fprintf( f, "P6\n%d %d 255\n", width, height );
        if ( fwrite( image, 1, bytesToWrite, f ) != bytesToWrite ) {
            fprintf( stderr, "Couldn't write image to file\n" );
            fclose( f );
            return false;
        }

        fclose( f );
        return true;
    }

    std::string PadLeft( std::string input, char const padChar, size_t const minWidth ) {
        return ( input.length( ) > minWidth ) ? input : std::string( std::max( minWidth - input.length( ), 0ULL ), padChar ) + input;
    }

}

int main( ) {
    cmsHTRANSFORM hTransform;
    {
        cmsHPROFILE hInProfile  { cmsCreateLab4Profile( cmsD50_xyY( ) ) };
        cmsHPROFILE hOutProfile { cmsCreate_sRGBProfile( )              };

        hTransform = cmsCreateTransform( hInProfile, TYPE_Lab_DBL, hOutProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0 );

        cmsCloseProfile( hInProfile );
        cmsCloseProfile( hOutProfile );
    }


    double*  inputBuffer  { new  double[3 * ImageWidth * ImageHeight] };
    uint8_t* outputBuffer { new uint8_t[3 * ImageWidth * ImageHeight] };

    for ( int L = 0; L <= 100; L += 5 ) {
        GenerateLabImage( inputBuffer, L );
        cmsDoTransform( hTransform, inputBuffer, outputBuffer, ImageWidth * ImageHeight );

        std::string fileName { ( OutputFilePath.empty( ) ? std::string { } : OutputFilePath + '\\' ) + "test-" + PadLeft( std::to_string( L ), '0', 3 ) + ".ppm" };
        if ( !WriteImageToFile( fileName, outputBuffer, sizeof uint8_t * 3 * ImageWidth * ImageHeight, ImageWidth, ImageHeight ) ) {
            break;
        }
    }

    delete[] outputBuffer;
    delete[] inputBuffer;
    cmsDeleteTransform( hTransform );
    return 0;
}
