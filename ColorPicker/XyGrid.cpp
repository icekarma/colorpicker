#include "pch.h"

#include "XyGrid.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

void CXyGrid::Update( ) {
    uint8_t* rgbImage { new uint8_t[ImageWidth * ImageHeight * ImageSrgbBytesPerPixel] };
    FloatT*  labImage { new  FloatT[ImageWidth * ImageHeight * ImageLabValuesPerPixel] };
    FloatT   LabL     { m_pDoc->GetColorAsLab( ).GetChannelValue( +LabChannels::L )    };

    FloatT* ptr { labImage };
    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < ImageWidth; ++x ) {
            *ptr++ = static_cast<FloatT>( LabL            );
            *ptr++ = static_cast<FloatT>( LabMinimumA + x );
            *ptr++ = static_cast<FloatT>( LabMinimumB + y );
        }
    }

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), labImage, rgbImage, ImageWidth * ImageHeight );
    m_pBitmap->SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbBytesPerPixel, rgbImage );

    delete[] labImage;
    delete[] rgbImage;
}
