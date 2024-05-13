#pragma once

class CColorPickerDoc:
    public CDocument
{

    DECLARE_DYNCREATE( CColorPickerDoc )
    DECLARE_MESSAGE_MAP( )

protected:

    virtual ~CColorPickerDoc( ) override = default;

public:

    //
    // Color stuff
    //

    void SetColor( LabColor const& color ) {
        m_LabColor  = color;
        m_SrgbColor = Transforms.ConvertColor( color );
    }

    void SetColor( SrgbColor const& color ) {
        m_LabColor  = Transforms.ConvertColor( color );
        m_SrgbColor = color;
    }

    [[nodiscard]] LabColor GetLabColor( ) const {
        return m_LabColor;
    }

    [[nodiscard]] SrgbColor GetSrgbColor( ) const {
        return m_SrgbColor;
    }

    [[nodiscard]] int GetChannelValue( AllChannels const channel ) const {
        if ( IsLabChannel( channel ) ) {
            return m_LabColor.GetChannelValue( channel );
        } else if ( IsSrgbChannel( channel ) ) {
            return m_SrgbColor.GetChannelValue( channel );
        } else {
            return INT_MIN;
        }
    }

    void SetChannelValue( AllChannels const channel, int const nValue ) {
        _SetChannelValueImpl( channel, nValue );

        if ( IsLabChannel( channel ) ) {
            SetColor( m_LabColor );
        } else if ( IsSrgbChannel( channel ) ) {
            SetColor( m_SrgbColor );
        }
    }

    void SetChannelValues( std::initializer_list<std::pair<AllChannels, int>> const values );

    [[nodiscard]] AllChannels GetSelectedChannel( ) const {
        return m_SelectedChannel;
    }

    void SetSelectedChannel( AllChannels const channel ) {
        m_SelectedChannel = channel;
    }

    //
    // Registry stuff
    //

    void LoadFromRegistry( );
    void SaveToRegistry( );

protected:

    void _SetChannelValueImpl( AllChannels const channel, int const nValue ) {
        if ( IsLabChannel( channel ) ) {
            m_LabColor.SetChannelValue( channel, static_cast<LabValueT>( nValue ) );
        } else if ( IsSrgbChannel( channel ) ) {
            m_SrgbColor.SetChannelValue( channel, static_cast<SrgbValueT>( nValue ) );
        }
    }

    virtual BOOL OnNewDocument( ) override;

    // Use SetColor or SetChannelValue to change these, because it keeps their values synchronized!
    LabColor    m_LabColor        { };
    SrgbColor   m_SrgbColor       { };

    AllChannels m_SelectedChannel { AllChannels::unknown };

};
