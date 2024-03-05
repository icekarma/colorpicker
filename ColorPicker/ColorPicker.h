#pragma once

#include "Resource.h"

class CColorPickerApp:
    public CWinApp
{

    DECLARE_MESSAGE_MAP( )

public:

    CColorPickerApp( ) noexcept;

    virtual ~CColorPickerApp( ) { /*empty*/ }

    virtual BOOL InitInstance( );
    //virtual int ExitInstance( );

protected:

    afx_msg void OnAppAbout( );

};

extern CColorPickerApp theApp;
