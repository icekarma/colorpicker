#pragma once

#include "Resource.h"

class CColorPickerApp:
    public CWinApp
 {

    DECLARE_MESSAGE_MAP( )

public:

    CColorPickerApp( ) noexcept;


    // Overrides
public:

    virtual BOOL InitInstance( );
    virtual int ExitInstance( );

    // Implementation
public:

    afx_msg void OnAppAbout( );

};

extern CColorPickerApp theApp;
