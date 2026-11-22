/*
===============================================================================

	STLoadDialog.h

	The file dialog that lists the saved games to load.

	0x4C bytes. Evidence: the SWIG wrapper STLoadDialog_Create ("ff") allocates
	0x4C bytes from the pool and calls the constructor 0x523E20, which runs
	STFileDialog's, stores the vtable 0x8900F0 (TGPane's 82 slots: the
	destructor, GetObjType, IsTypeOf, the class strings and GetClassHandlers
	overridden) and calls 0x524040. The destructor is the implicit one (0x523F60
	only jumps to TGPane's). The copies of the inline virtuals and of the
	deleting destructor follow the constructor (0x523E80-0x523F30). GetObjType
	returns 0x80E8, the SWIG constant CT_ST_LOAD_DIALOG.

===============================================================================
*/

#ifndef __STLOADDIALOG_H__
#define __STLOADDIALOG_H__

#include "STFileDialog.h"

class TGEventHandlerTable;
class TGEvent;
class TGKeyboardEvent;

// GetObjType's value for an STLoadDialog (the SWIG constant CT_ST_LOAD_DIALOG).
#define STLOADDIALOG_TYPE		0x80E8

// Sent to the dialog by its Load Game button (the SWIG constant ET_ST_LOAD_DIALOG_LOAD).
#define ET_ST_LOAD_DIALOG_LOAD	0x80009E

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00523f30 ??_GSTLoadDialog@@UAEPAXI@Z
// FUNCTION: STBC 0x00523f60 ??1STLoadDialog@@UAE@XZ
class STLoadDialog : public STFileDialog {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558150 ??0STLoadDialog@@QAE@J@Z @game/InterfaceModule
					STLoadDialog( long id ) : STFileDialog( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					STLoadDialog( float width, float height );

	// TGObject
	// FUNCTION: STBC 0x00523e80 ?GetObjType@STLoadDialog@@UAEHXZ
	virtual int		GetObjType( void ) { return STLOADDIALOG_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00523ef0 ?GetClassName@STLoadDialog@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STLoadDialog"; }
	// FUNCTION: STBC 0x00523f00 ?GetSwigTypeName@STLoadDialog@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STLoadDialog"; }
	// FUNCTION: STBC 0x00523f10 ?GetPythonClassName@STLoadDialog@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STLoadDialogPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00523f20 ?GetClassHandlers@STLoadDialog@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			KeyboardHandler( TGKeyboardEvent *event );
	void			HandleFileSelection( TGEvent *event );
	void			HandleLoadButton( TGEvent *event );
	void			HandleDeleteButton( TGEvent *event );

	// Declared for the SWIG wrapper that calls it.
	static STLoadDialog *	Create( float width, float height ) { return new STLoadDialog( width, height ); }	// the SWIG wrapper STLoadDialog_Create expands it

protected:
	// Adds the Load Game and Delete buttons (invented name).
	void			CreateButtons( void );

	static TGEventHandlerTable	classHandlers;		// 0x9890A0 (invented name)
};

/*
================
STLoadDialog::IsTypeOf
================
*/
// FUNCTION: STBC 0x00523e90 ?IsTypeOf@STLoadDialog@@UAE_NH@Z
inline bool STLoadDialog::IsTypeOf( int type ) {
	if ( type == STLOADDIALOG_TYPE ) {
		return true;
	}
	return STFileDialog::IsTypeOf( type );
}

#endif /* !__STLOADDIALOG_H__ */
