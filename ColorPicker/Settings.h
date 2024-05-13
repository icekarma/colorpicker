#pragma once

#include "ColorPicker.h"

class CSettings {

public:

    //
    // Object life cycle management
    //

    CSettings( ) {
        m_selectedChannel    = static_cast<AllChannels>( std::min( +AllChannels::Max, theApp.GetProfileInt( L"Settings", L"Selected Channel", static_cast<int>( AllChannels::LabL ) ) ) );

        m_fInverted          = theApp.GetProfileInt   ( L"Settings", L"Inverted",  0    ) ? true : false;
        m_strSeparator       = theApp.GetProfileString( L"Settings", L"Separator", L"," );
        m_strQuoting         = theApp.GetProfileString( L"Settings", L"Quoting",   L""  );

        m_ptWindowPosition.x = static_cast<int>( theApp.GetProfileInt( L"Settings", L"X", -1'000'000 ) );
        m_ptWindowPosition.y = static_cast<int>( theApp.GetProfileInt( L"Settings", L"Y", -1'000'000 ) );
    }

    CSettings( CSettings const& ) = delete;
    CSettings( CSettings&& )      = delete;

    CSettings& operator=( CSettings const& ) = delete;
    CSettings& operator=( CSettings&& )      = delete;

    //
    // Accessors
    //

    [[nodiscard]] AllChannels GetSelectedChannel( ) const {
        return m_selectedChannel;
    }

    [[nodiscard]] bool IsInverted( ) const {
        return m_fInverted;
    }

    [[nodiscard]] CString const& GetSeparator( ) const {
        return m_strSeparator;
    }

    [[nodiscard]] CString const& GetQuoting( ) const {
        return m_strQuoting;
    }

    [[nodiscard]] CPoint const& GetWindowPosition( ) const {
        return m_ptWindowPosition;
    }

    //
    // Mutators
    //

    void SetSelectedChannel( AllChannels const value ) {
        m_selectedChannel = value;
        theApp.WriteProfileInt( L"Settings", L"Selected Channel", static_cast<int>( m_selectedChannel ) );
    }

    void SetInverted( bool const value ) {
        m_fInverted = value;
        theApp.WriteProfileInt( L"Settings", L"Inverted", m_fInverted ? 1 : 0 );
    }

    void SetSeparator( CString const& value ) {
        m_strSeparator = value;
        theApp.WriteProfileString( L"Settings", L"Separator", m_strSeparator );
    }

    void SetQuoting( CString const& value ) {
        m_strQuoting = value;
        theApp.WriteProfileString( L"Settings", L"Quoting", m_strQuoting );
    }

    void SetWindowPosition( CPoint const& value ) {
        m_ptWindowPosition = value;
        theApp.WriteProfileInt( L"Settings", L"X", m_ptWindowPosition.x );
        theApp.WriteProfileInt( L"Settings", L"Y", m_ptWindowPosition.y );
    }

    void SetWindowPosition( CRect const& value ) {
        SetWindowPosition( value.TopLeft( ) );
    }

protected:

    AllChannels m_selectedChannel  { AllChannels::unknown };
    bool        m_fInverted        { };
    CString     m_strSeparator     { };
    CString     m_strQuoting       { };
    CPoint      m_ptWindowPosition { };

};

extern CSettings g_settings;
