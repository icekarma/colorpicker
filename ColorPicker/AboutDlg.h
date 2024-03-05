#pragma once

class CAboutDlg: public CDialogEx {

    DECLARE_MESSAGE_MAP( )

public:

    CAboutDlg( ) noexcept:
        CDialogEx( IDD_ABOUTBOX )
    {
        /*empty*/
    }

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:

    virtual void DoDataExchange( CDataExchange* pDX );

    // Implementation
protected:

};
