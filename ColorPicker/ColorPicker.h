#pragma once

class CColorPickerApp:
    public CWinAppEx
{

    DECLARE_MESSAGE_MAP( )

public:

    CColorPickerApp( ) noexcept;

    virtual ~CColorPickerApp( ) override;

    virtual BOOL InitInstance( ) override;

protected:

    afx_msg void OnAppAbout( );

};

extern CColorPickerApp theApp;
