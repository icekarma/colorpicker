#pragma once

class CColorPickerDoc:
    public CDocument
{

    DECLARE_DYNCREATE( CColorPickerDoc )
    DECLARE_MESSAGE_MAP( )

protected:

    CColorPickerDoc( ) = default;

    virtual ~CColorPickerDoc( ) override = default;

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

    void SetChannelValue( AllChannels const channel, int const nValue );

protected:

    virtual BOOL OnNewDocument( ) override;

    // Use SetColor or SetChannelValue to change these, because it keeps their values synchronized!
    LabColor  m_LabColor  { };
    SrgbColor m_SrgbColor { };

};
