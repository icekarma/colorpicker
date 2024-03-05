#pragma once

class CAboutDlg:
    public CDialogEx
{

    DECLARE_MESSAGE_MAP( )

    // Construction
public:

    CAboutDlg( ) noexcept:
        CDialogEx( IDD_ABOUTBOX )
    {
        /*empty*/
    }

    // Dialog Data
public:

#if defined AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

    // Overrides
protected:

    //virtual void DoDataExchange( CDataExchange* pDX );

    // Implementation
protected:

};
