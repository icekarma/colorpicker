#pragma once

class CColorPickerApp:
    public CWinApp
{

    DECLARE_MESSAGE_MAP( )

public:

    CColorPickerApp( ) noexcept;

    virtual ~CColorPickerApp( ) { /*empty*/ }

    virtual BOOL InitInstance( );

protected:

    afx_msg void OnAppAbout( );

};

extern CColorPickerApp theApp;
