/*
===============================================================================

	STFileDialog.h

	The members of STFileDialog that the SWIG wrappers STFileDialog_* call,
	and its vtable. No data members yet.

	Evidence: the constructor (0x51F5D0) runs TGPane's and stores the vtable
	0x88FC90 (TGPane's 82 slots: the destructor, GetObjType, IsTypeOf, the
	class strings and GetClassHandlers overridden); the destructor is the
	implicit one. The copies of the inline virtuals and of the deleting
	destructor follow the constructor (0x51F630-0x51F6E0). GetObjType
	returns 0x80E7, the SWIG constant CT_ST_FILE_DIALOG. STLoadDialog and
	STSaveDialog derive from it.

===============================================================================
*/

#ifndef __STFILEDIALOG_H__
#define __STFILEDIALOG_H__

#include "TGPane.h"

class STFileMenu;
class STStylizedWindow;
class TGEventHandlerTable;

// GetObjType's value for an STFileDialog (the SWIG constant CT_ST_FILE_DIALOG).
#define STFILEDIALOG_TYPE		0x80E7

// Sent by a file button when it is chosen (the SWIG constant ET_ST_FILE_DIALOG_FILE_CHOSEN).
#define ET_ST_FILE_DIALOG_FILE_CHOSEN	0x80009A
// Sent by the Delete button, and by the confirmation of the deletion (the SWIG constants ET_ST_FILE_DIALOG_DELETE, _DELETE2).
#define ET_ST_FILE_DIALOG_DELETE		0x80009B
#define ET_ST_FILE_DIALOG_DELETE2		0x80009C

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0051f6e0 ??_GSTFileDialog@@UAEPAXI@Z @game/STFileDialog
// FUNCTION: STBC 0x0051f710 ??1STFileDialog@@UAE@XZ @game/STFileDialog
class STFileDialog : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558100 ??0STFileDialog@@QAE@J@Z @game/InterfaceModule
					STFileDialog( long id ) : TGPane( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// TGObject
	// FUNCTION: STBC 0x0051f630 ?GetObjType@STFileDialog@@UAEHXZ @game/STFileDialog
	virtual int		GetObjType( void ) { return STFILEDIALOG_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0051f6a0 ?GetClassName@STFileDialog@@UAEPADXZ @game/STFileDialog
	virtual char *	GetClassName( void ) { return "STFileDialog"; }
	// FUNCTION: STBC 0x0051f6b0 ?GetSwigTypeName@STFileDialog@@UAEPADXZ @game/STFileDialog
	virtual char *	GetSwigTypeName( void ) { return "_p_STFileDialog"; }
	// FUNCTION: STBC 0x0051f6c0 ?GetPythonClassName@STFileDialog@@UAEPADXZ @game/STFileDialog
	virtual char *	GetPythonClassName( void ) { return "STFileDialogPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0051f6d0 ?GetClassHandlers@STFileDialog@@UAEPAVTGEventHandlerTable@@XZ @game/STFileDialog
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	STFileMenu *	GetFileMenu( void );
	void	RefreshFileList( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// Declared for the SWIG wrappers that call them.
public:
					STFileDialog( float width, float height );
	static STFileDialog *	Create( float width, float height ) { return new STFileDialog( width, height ); }	// the SWIG wrapper STFileDialog_Create expands it

protected:
	void	CreateControls( void );		// invented name
	// The four areas CreateControls puts together (invented names), each from the lcars group's Python values.
	TGPane *	CreateTopArea( char *lcars );
	TGPane *	CreateContentArea( char *lcars );
	STStylizedWindow *	CreateFileListWindow( char *lcars );		// 0x51FC60
	TGPane *	CreateScreenshotArea( char *lcars );		// 0x51FE50

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STFileDialog::IsTypeOf
================
*/
// FUNCTION: STBC 0x0051f640 @game/STFileDialog
inline bool STFileDialog::IsTypeOf( int type ) {
	if ( type == STFILEDIALOG_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__STFILEDIALOG_H__ */
