#include <lcms2.h>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <string>

namespace {

    using FloatT = float;
    uint32_t const LabPixelFormat = TYPE_Lab_FLT;

    std::string const OutputFilePath { "TestSet5" };

    FloatT constexpr LabMinimumA { -128.0 };
    FloatT constexpr LabMaximumA {  127.0 };
    FloatT constexpr LabMinimumB { -128.0 };
    FloatT constexpr LabMaximumB {  127.0 };

    int    constexpr ImageWidth  { static_cast<int>( LabMaximumA - LabMinimumA ) + 1 };
    int    constexpr ImageHeight { static_cast<int>( LabMaximumB - LabMinimumB ) + 1 };

    FloatT* GenerateLabImage( FloatT* image, FloatT const L ) {
        FloatT* ptr { image };

        for ( int y { }; y < ImageHeight; ++y ) {
            for ( int x { }; x < ImageWidth; ++x ) {
                *ptr++ = L;
                *ptr++ = LabMinimumA + static_cast<FloatT>( x );
                *ptr++ = LabMinimumB + static_cast<FloatT>( y );
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
    cmsHTRANSFORM hLabToSrgbTransform;
    cmsHTRANSFORM hSrgbToLabTransform;

    {
        cmsHPROFILE hLabProfile { cmsCreateLab4Profile( cmsD50_xyY( ) ) };
        cmsHPROFILE hRgbProfile { cmsCreate_sRGBProfile( )              };

        hLabToSrgbTransform = cmsCreateTransform( hLabProfile, LabPixelFormat, hRgbProfile, TYPE_RGB_8,     INTENT_PERCEPTUAL, 0 );
        hSrgbToLabTransform = cmsCreateTransform( hRgbProfile, TYPE_RGB_8,     hLabProfile, LabPixelFormat, INTENT_PERCEPTUAL, 0 );

        cmsCloseProfile( hLabProfile );
        cmsCloseProfile( hRgbProfile );
    }

    FloatT*  inputBuffer  { new  FloatT[3 * ImageWidth * ImageHeight] };
    uint8_t* outputBuffer { new uint8_t[3 * ImageWidth * ImageHeight] };

    for ( int L = 0; L <= 100; L += 5 ) {
        GenerateLabImage( inputBuffer, static_cast<FloatT>( L ) );
        cmsDoTransform( hLabToSrgbTransform, inputBuffer, outputBuffer, ImageWidth * ImageHeight );

        std::string fileName { ( OutputFilePath.empty( ) ? std::string { } : OutputFilePath + '\\' ) + "test-" + PadLeft( std::to_string( L ), '0', 3 ) + ".ppm" };
        if ( !WriteImageToFile( fileName, outputBuffer, sizeof uint8_t * 3 * ImageWidth * ImageHeight, ImageWidth, ImageHeight ) ) {
            break;
        }
    }

    delete[] outputBuffer;
    delete[] inputBuffer;

    cmsDeleteTransform( hLabToSrgbTransform );
    cmsDeleteTransform( hSrgbToLabTransform );
    return 0;
}
