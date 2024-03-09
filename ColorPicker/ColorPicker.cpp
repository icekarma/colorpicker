#include "pch.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"
#include "MainFrame.h"
#include "ChildView.h"
#include "AboutDlg.h"

BEGIN_MESSAGE_MAP( CColorPickerApp, CWinApp )
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

BOOL CColorPickerApp::InitInstance( ) {
    INITCOMMONCONTROLSEX InitCtrls { sizeof InitCtrls, 0xFFFF };
    InitCommonControlsEx( &InitCtrls );

    CWinApp::InitInstance( );

    if ( !AfxSocketInit( ) ) {
        AfxMessageBox( IDP_SOCKETS_INIT_FAILED );
        return FALSE;
    }

    EnableTaskbarInteraction( FALSE );

    AfxInitRichEdit2();

    SetRegistryKey( L"Zive Technology Research\\ColorPicker" );
    LoadStdProfileSettings( 4 );

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

//int CColorPickerApp::ExitInstance( ) {
//    return CWinApp::ExitInstance( );
//}

void CColorPickerApp::OnAppAbout( ) {
    CAboutDlg { }.DoModal( );
}
