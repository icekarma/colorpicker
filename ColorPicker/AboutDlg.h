#pragma once

class CAboutDlg:
    public CDialogEx
{

    DECLARE_MESSAGE_MAP( )

public:

#if defined AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

    CAboutDlg( ): CDialogEx { IDD_ABOUTBOX } { /*empty*/ }

    virtual ~CAboutDlg( ) override;

protected:

    CStatic* m_pBitmap     { };
    CButton* m_pButtonOK   { };
    CStatic* m_pStaticText { };

    virtual BOOL OnInitDialog( );

};
