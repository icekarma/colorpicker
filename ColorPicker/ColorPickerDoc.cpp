#include "pch.h"

#include "ColorPicker.h"
#include "ColorPickerDoc.h"

IMPLEMENT_DYNCREATE( CColorPickerDoc, CDocument )

BEGIN_MESSAGE_MAP( CColorPickerDoc, CDocument )
END_MESSAGE_MAP( )

BOOL CColorPickerDoc::OnNewDocument( ) {
	if ( !CDocument::OnNewDocument( ) ) {
		return FALSE;
	}

	// Reinitialization (SDI applications will reuse this document)

	LabColor  = { };
	SrgbColor = { };

	return TRUE;
}
