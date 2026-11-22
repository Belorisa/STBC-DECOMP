/*
===============================================================================

	STSaveDialog.h

	The file dialog that names the game to save.

	0x4C bytes. Evidence: the SWIG wrapper STSaveDialog_Create ("ff") allocates
	0x4C bytes from the pool and calls the constructor 0x52F610, which runs
	STFileDialog's, stores the vtable 0x89082C (TGPane's 82 slots: the
	destructor, GetObjType, IsTypeOf, the class strings and GetClassHandlers
	overridden) and calls 0x52F800. The destructor is the implicit one (0x52F750
	only jumps to TGPane's). The copies of the inline virtuals and of the
	deleting destructor follow the constructor (0x52F670-0x52F720). GetObjType
	returns 0x80E9, the SWIG constant CT_ST_SAVE_DIALOG.

===============================================================================
*/

#ifndef __STSAVEDIALOG_H__
#define __STSAVEDIALOG_H__

#include "STFileDialog.h"

class TGEvent;
class TGEventHandlerObject;
class TGEventHandlerTable;

// GetObjType's value for an STSaveDialog (the SWIG constant CT_ST_SAVE_DIALOG).
#define STSAVEDIALOG_TYPE		0x80E9

// Sent to the dialog by its Save button (the SWIG constant ET_ST_SAVE_DIALOG_SAVE).
#define ET_ST_SAVE_DIALOG_SAVE	0x80009D

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0052f720 ??_GSTSaveDialog@@UAEPAXI@Z
// FUNCTION: STBC 0x0052f750 ??1STSaveDialog@@UAE@XZ
class STSaveDialog : public STFileDialog {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005581a0 ??0STSaveDialog@@QAE@J@Z @game/InterfaceModule
					STSaveDialog( long id ) : STFileDialog( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					STSaveDialog( float width, float height );

	// TGObject
	// FUNCTION: STBC 0x0052f670 ?GetObjType@STSaveDialog@@UAEHXZ
	virtual int		GetObjType( void ) { return STSAVEDIALOG_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0052f6e0 ?GetClassName@STSaveDialog@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STSaveDialog"; }
	// FUNCTION: STBC 0x0052f6f0 ?GetSwigTypeName@STSaveDialog@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STSaveDialog"; }
	// FUNCTION: STBC 0x0052f700 ?GetPythonClassName@STSaveDialog@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STSaveDialogPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0052f710 ?GetClassHandlers@STSaveDialog@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			HandleFileSelection( TGEvent *event );
	void			HandleSaveButton( TGEvent *event );
	void			HandleDeleteButton( TGEvent *event );
	// The save name's text field's mouse handler, added to the field itself
	// (0x5302D0; its name is the binary's).
	static void		HandleSaveNameMouse( TGEventHandlerObject *object, TGEvent *event );

	// Declared for the SWIG wrapper that calls it.
	static STSaveDialog *	Create( float width, float height ) { return new STSaveDialog( width, height ); }	// the SWIG wrapper STSaveDialog_Create expands it

protected:
	// TODO identity: builds the save name's text field and the Save and
	// Delete buttons in the dialog's panes (0x52F800, 1.4 KB; the
	// constructor calls it; name invented).
	void			CreateControls( void );

	static TGEventHandlerTable	classHandlers;		// 0x989820 (invented name)
};

/*
================
STSaveDialog::IsTypeOf
================
*/
// FUNCTION: STBC 0x0052f680 ?IsTypeOf@STSaveDialog@@UAE_NH@Z
inline bool STSaveDialog::IsTypeOf( int type ) {
	if ( type == STSAVEDIALOG_TYPE ) {
		return true;
	}
	return STFileDialog::IsTypeOf( type );
}

#endif /* !__STSAVEDIALOG_H__ */
