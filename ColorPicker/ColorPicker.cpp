#include "pch.h"

#include "ColorPicker.h"

#include "ColorPickerDoc.h"
#include "Settings.h"
#include "MainFrame.h"
#include "ChildView.h"
#include "AboutDlg.h"

BEGIN_MESSAGE_MAP( CColorPickerApp, CWinAppEx )
    ON_COMMAND( ID_APP_ABOUT, &CColorPickerApp::OnAppAbout )
END_MESSAGE_MAP( )

CColorPickerApp theApp;

CColorPickerApp::CColorPickerApp( ) noexcept {
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

    // Format: CompanyName.ProductName.SubProduct.VersionInformation
    SetAppID( L"ZiveTechnologyResearch.ColorPicker.AppID.NoVersion" );

    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

CColorPickerApp::~CColorPickerApp( ) {
    if ( g_pSettings ) {
        delete g_pSettings;
        g_pSettings = nullptr;
    }
}

BOOL CColorPickerApp::InitInstance( ) {
    INITCOMMONCONTROLSEX InitCtrls { sizeof InitCtrls, 0xFFFF };
    InitCommonControlsEx( &InitCtrls );

    CWinApp::InitInstance( );

    EnableTaskbarInteraction( TRUE );

    SetRegistryKey( L"Zive Technology Research" );
    LoadStdProfileSettings( );
    g_pSettings = new CSettings;

    CSingleDocTemplate* pDocTemplate {
        new CSingleDocTemplate(
            IDR_MAINFRAME,
            RUNTIME_CLASS( CColorPickerDoc ),
            RUNTIME_CLASS( CMainFrame ),
            RUNTIME_CLASS( CChildView )
        )
    };
    if ( !pDocTemplate ) {
        return FALSE;
    }
    AddDocTemplate( pDocTemplate );

    CCommandLineInfo cmdInfo;
    ParseCommandLine( cmdInfo );
    if ( !ProcessShellCommand( cmdInfo ) ) {
        return FALSE;
    }

    m_pMainWnd->ShowWindow( SW_SHOW );
    m_pMainWnd->UpdateWindow( );
    return TRUE;
}

void CColorPickerApp::OnAppAbout( ) {
    CAboutDlg { }.DoModal( );
}
