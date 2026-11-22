/*
===============================================================================

	TGStringDialog.h

	A dialog window that asks for a string in a paragraph.

	0x68 bytes, allocated from the pools (the scalar deleting destructor
	0x726310 frees through the pool of that size).

	Evidence: the vtable 0x897124 (83 slots, none new) overrides the
	destructor (0x737E30, which only restores the vtable before
	TGDialogWindow's), the type members, the stream slots (0x737FA0,
	0x738000, 0x738040, 0x738080: the IDs of the objects at 0x60 and 0x64)
	and GotFocus (0x737F00, which hands the focus to the paragraph at
	0x60). 0x737F20 and 0x737F30 return the paragraph's string (its slots
	91 and 82). The handlers are named by "TGDialogWindow::MouseHandler" and
	"TGStringDialog::DialogEventHandler". GetObjType returns 0x20F. No SWIG
	wrapper names the class's members. The load constructor and the type's
	inline members are emitted with the UI module's class factories
	(0x726220 to 0x726310).

===============================================================================
*/

#ifndef __TGSTRINGDIALOG_H__
#define __TGSTRINGDIALOG_H__

#include "TGDialogWindow.h"
#include "TGParagraph.h"

class TGParagraph;
class TGString;

// The flags of a string dialog's window: the OK and Cancel buttons, and
// draggable (invented name).
#define TGSTRINGDIALOG_DIALOG_FLAGS	( TGDialogWindow::DFT_OKCANCEL | TGDialogWindow::DFT_DRAGGABLE )

// GetObjType's value for a TGStringDialog.
#define TGSTRINGDIALOG_TYPE		0x20F

// FUNCTION: STBC 0x00726310 ??_GTGStringDialog@@UAEPAXI@Z
class TGStringDialog : public TGDialogWindow {
public:
	// FUNCTION: STBC 0x00726220 ??0TGStringDialog@@QAE@J@Z
					TGStringDialog( long id = 0 ) : TGDialogWindow( id ) {}
					TGStringDialog( float width, float height, char *message, char *frameName, float frameSize,
						char *buttonFrameName, float buttonFrameSize, char *label, char *string,
						TGEventHandlerObject *destination, TGEventHandlerObject *stringDestination );
	virtual			~TGStringDialog( void );

	// TGObject
	// FUNCTION: STBC 0x00726240 ?GetObjType@TGStringDialog@@UAEHXZ
	virtual int		GetObjType( void ) { return TGSTRINGDIALOG_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x007262d0 ?GetClassName@TGStringDialog@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGStringDialog"; }
	// FUNCTION: STBC 0x007262e0 ?GetSwigTypeName@TGStringDialog@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGStringDialog"; }
	// FUNCTION: STBC 0x007262f0 ?GetPythonClassName@TGStringDialog@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGStringDialogPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00726300 ?GetClassHandlers@TGStringDialog@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );

	// A new string dialog (invented name; CreateFromTheme calls it with the
	// current theme's frames).
	static TGStringDialog *	CreateFromTheme( float width, float height, char *message, char *label, char *string,
						TGEventHandlerObject *destination, TGEventHandlerObject *stringDestination );	// invented name, as TGDialogWindow's
	static TGStringDialog *	Create( float width, float height, char *message, char *frameName, float frameSize,
						char *buttonFrameName, float buttonFrameSize, char *label, char *string,
						TGEventHandlerObject *destination, TGEventHandlerObject *stringDestination );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			DialogEventHandler( TGEvent *event );

	// The paragraph's string (invented names).
	char *			GetCString( void );
	void			GetString( TGString *string );
	// Puts the string in the paragraph with the cursor after it (invented
	// name; the placement editor's name and save dialogs, 0x49C670 and
	// 0x49C9D0, expand it).
	void			SetString( char *string ) {
		paragraph->SetString( string );
		paragraph->MoveCursorToEnd();
	}

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	TGParagraph *	paragraph;				// 0x60 holds the string (invented name)
	TGEventHandlerObject *	unknown64;		// 0x64 the dialog itself when none is given; an ID between ReadFromStream and PostFixUp

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGStringDialog::IsTypeOf
================
*/
// FUNCTION: STBC 0x00726250
inline bool TGStringDialog::IsTypeOf( int type ) {
	if ( type == TGSTRINGDIALOG_TYPE ) {
		return true;
	}
	return TGDialogWindow::IsTypeOf( type );
}

#endif /* !__TGSTRINGDIALOG_H__ */
