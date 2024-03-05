#include "pch.h"

#include "ColorPicker.h"
#include "AboutDlg.h"
#include "MainFrame.h"

BEGIN_MESSAGE_MAP( CColorPickerApp, CWinApp )
    ON_COMMAND( ID_APP_ABOUT, &CColorPickerApp::OnAppAbout )
END_MESSAGE_MAP( )

CColorPickerApp::CColorPickerApp( ) noexcept {
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

    // Format: CompanyName.ProductName.SubProduct.VersionInformation
    SetAppID( L"ZiveTechnologyResearch.ColorPicker.AppID.NoVersion" );

    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

CColorPickerApp theApp;

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

    //====vvvv====0001====TODO==TODO==TODO==TODO==TODO====

    CFrameWnd* pFrame = new CMainFrame;
    if ( !pFrame ) {
        return FALSE;
    }
    m_pMainWnd = pFrame;
    pFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr, nullptr );

    //====^^^^====0001====TODO==TODO==TODO==TODO==TODO====

    pFrame->ShowWindow( SW_SHOW );
    pFrame->UpdateWindow( );
    return TRUE;
}

//int CColorPickerApp::ExitInstance( ) {
//    return CWinApp::ExitInstance( );
//}

void CColorPickerApp::OnAppAbout( ) {
    CAboutDlg { }.DoModal( );
}
