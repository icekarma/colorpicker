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

    virtual ~CAboutDlg( ) override {
        if ( m_pBitmap ) {
            delete m_pBitmap;
            m_pBitmap = nullptr;
        }

        if ( m_pImage ) {
            delete m_pImage;
            m_pImage = nullptr;
        }
    }

protected:

    CImage*  m_pImage       { };
    CStatic* m_pBitmap      { };
    CButton* m_pOkButton    { };

    virtual BOOL OnInitDialog( );

};
