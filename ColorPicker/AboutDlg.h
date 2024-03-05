#pragma once

class CAboutDlg:
    public CDialogEx
{

    DECLARE_MESSAGE_MAP( )

public:

#if defined AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

    CAboutDlg( ) noexcept: CDialogEx { IDD_ABOUTBOX } { /*empty*/ }

    virtual ~CAboutDlg( ) { /*empty*/ }

protected:

    //virtual void DoDataExchange( CDataExchange* pDX );

};
