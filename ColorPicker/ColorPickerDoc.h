#pragma once

class CColorPickerDoc:
    public CDocument
{

    DECLARE_DYNCREATE( CColorPickerDoc )
    DECLARE_MESSAGE_MAP( )

    // Construction
protected:

    CColorPickerDoc( ) noexcept { /*empty*/ }

    virtual ~CColorPickerDoc( ) { /*empty*/ }

    // Attributes
public:

    // Operations
public:

    // Overrides
public:

    virtual BOOL OnNewDocument( );
    //virtual void Serialize( CArchive& ar );

    // Implementation
public:

    // Generated message map functions
protected:

};
