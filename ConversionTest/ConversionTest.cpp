#include <lcms2.h>

#include <cstdint>
#include <cstdio>
#include <cstring>

double* GenerateLabImage( double* image, double const L ) {
    double* ptr = image;

    for ( int y { }; y < 200; ++y ) {
        for ( int x { }; x < 200; ++x ) {
            *ptr++ = L;
            *ptr++ = x - 100.0;
            *ptr++ = y - 100.0;
        }
    }

    return image;
}

bool WriteImageToFile( char const* fileName, uint8_t const* image, size_t const bytesToWrite, int const width, int const height ) {
    FILE* f { fopen( fileName, "wb" ) };
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

int main( ) {
    cmsHTRANSFORM hTransform;
    {
        cmsHPROFILE hInProfile  { cmsCreateLab4Profile( cmsD50_xyY( ) ) };
        cmsHPROFILE hOutProfile { cmsCreate_sRGBProfile( )              };

        hTransform = cmsCreateTransform( hInProfile, TYPE_Lab_DBL, hOutProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0 );

        cmsCloseProfile( hInProfile );
        cmsCloseProfile( hOutProfile );
    }


    double*  inputBuffer  { new double[3 * 200 * 200]  };
    uint8_t* outputBuffer { new uint8_t[3 * 200 * 200] };
    char fileName[64];

    for ( int L = 0; L <= 100; L += 5 ) {
        GenerateLabImage( inputBuffer, L );
        cmsDoTransform( hTransform, inputBuffer, outputBuffer, 200 * 200 );
        sprintf( fileName, "test-%03d.ppm", L );
        WriteImageToFile( fileName, outputBuffer, sizeof( uint8_t ) * 3 * 200 * 200, 200, 200 );
    }

    delete[] outputBuffer;
    delete[] inputBuffer;
    cmsDeleteTransform( hTransform );
    return 0;
}
