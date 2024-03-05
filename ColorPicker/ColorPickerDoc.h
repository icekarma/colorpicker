#pragma once

class CColorPickerDoc:
    public CDocument
{

    DECLARE_DYNCREATE( CColorPickerDoc )
    DECLARE_MESSAGE_MAP( )

protected:

    CColorPickerDoc( ) noexcept { /*empty*/ }

    virtual ~CColorPickerDoc( ) { /*empty*/ }

    virtual BOOL OnNewDocument( );
    //virtual void Serialize( CArchive& ar );

};
