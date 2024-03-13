#include "pch.h"

#include "ZStrip.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

void CZStrip::Update( ) {
    uint8_t* rgbImage { new uint8_t[ImageWidth * ImageHeight * ImageSrgbBytesPerPixel] };
    FloatT*  labImage { new  FloatT[ImageWidth * ImageHeight * ImageLabValuesPerPixel] };

    FloatT channels[3];
    m_pDoc->GetColorAsLab( ).GetChannelValues( channels );

    FloatT* ptr { labImage };
    for ( int y { }; y < ImageHeight; ++y ) {
        for ( int x { }; x < 20; ++x ) {
            *ptr++ = static_cast<FloatT>( y / 2.55    ) + static_cast<FloatT>( LabMinimumL );
            *ptr++ = static_cast<FloatT>( channels[1] );
            *ptr++ = static_cast<FloatT>( channels[2] );
        }
    }

    cmsDoTransform( Transforms.GetLabToSrgbTransform( ), labImage, rgbImage, ImageWidth * ImageHeight );
    m_pBitmap->SetBitmapBits( ImageWidth * ImageHeight * ImageSrgbBytesPerPixel, rgbImage );

    delete[] labImage;
    delete[] rgbImage;
}
