#pragma once

#include "Color.h"

class CColorPickerDoc:
    public CDocument
{

    DECLARE_DYNCREATE( CColorPickerDoc )
    DECLARE_MESSAGE_MAP( )

public:

    void SetColor( LabColor const& color ) {
        m_LabColor  = color;
        m_SrgbColor = Transforms.ConvertColor( color );
    }

    void SetColor( SrgbColor const& color ) {
        m_LabColor  = Transforms.ConvertColor( color );
        m_SrgbColor = color;
    }

    LabColor GetLabColor( ) const {
        return m_LabColor;
    }

    SrgbColor GetSrgbColor( ) const {
        return m_SrgbColor;
    }

    void SetChannels( int const x, int const y, int const z ) {
        m_nChannelX = x;
        m_nChannelY = y;
        m_nChannelZ = z;
    }

    void GetChannels( int& x, int& y, int& z ) const {
        x = m_nChannelX;
        y = m_nChannelY;
        z = m_nChannelZ;
    }

    int GetChannelX( ) const { return m_nChannelX; }
    int GetChannelY( ) const { return m_nChannelY; }
    int GetChannelZ( ) const { return m_nChannelZ; }

protected:

    CColorPickerDoc( ) noexcept { /*empty*/ }

    virtual ~CColorPickerDoc( ) { /*empty*/ }

    virtual BOOL OnNewDocument( );

    // Use SetColor to change these, because it keeps their values synchronized!
    LabColor    m_LabColor;
    SrgbColor   m_SrgbColor;

    int m_nChannelX { 0 };
    int m_nChannelY { 1 };
    int m_nChannelZ { 2 };

};
