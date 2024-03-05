#pragma once

#include "Resource.h"

class CColorPickerApp:
    public CWinApp
{

    DECLARE_MESSAGE_MAP( )

    // Construction
public:

    CColorPickerApp( ) noexcept;

    virtual ~CColorPickerApp( ) {
        /*empty*/
    }

    // Overrides
public:

    virtual BOOL InitInstance( );
    //virtual int ExitInstance( );

    // Implementation
public:

    afx_msg void OnAppAbout( );

};

extern CColorPickerApp theApp;
