/*
===============================================================================

	TGWindow.h

	A pane that holds one sub pane, indented inside it, and hands the
	child calls on to that sub pane: what is inserted into a window goes
	into its sub pane.

	0x4C bytes (TGWindow_Create allocates 0x4C): no member of its own.

	Evidence: the constructor (0x73E6A0) builds the pane at the window's
	size, makes a sub pane of the same size when none is given, marks it
	to skip its parent (slot 48), inserts it as the window's own child and
	indents it (SetIndent, 0x73EB30); it stores the vtable 0x892AA4 (83
	slots; 82 is new). Slot 82 is the window's first child itself
	(TGPane::GetFirstChild, 0x72ED60): TGWindow_GetSubPane calls it. The
	overrides of slots 64 to 81 forward to the sub pane; SetFocus keeps
	the focus calls for the sub pane itself on the window. Resize (slot
	24) resizes the sub pane by as much as the window unless
	TGWINDOW_SIMPLE_RESIZE is set. The destructor (0x73E7D0) clears the
	focus and kills the children through TGPane's own functions. The
	inline functions the vtable points at were kept from another unit
	(0x5596B0 to 0x559760).

===============================================================================
*/

#ifndef __TGWINDOW_H__
#define __TGWINDOW_H__

#include "TGPane.h"

class TGObject;

// GetObjType's value for a TGWindow.
#define TGWINDOW_TYPE			0x20C

// A flag of the window (the SWIG constant TGWINDOW_SIMPLE_RESIZE): Resize
// leaves the sub pane alone.
#define TGWINDOW_SIMPLE_RESIZE	0x1000

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00559760 ??_GTGWindow@@UAEPAXI@Z
class TGWindow : public TGPane {
public:
	// FUNCTION: STBC 0x00725d90 ??0TGWindow@@QAE@J@Z
					TGWindow( long id = 0 ) : TGPane( id ) {}
					TGWindow( float width, float height, float indentX, float indentY, TGPane *subPane );
	virtual			~TGWindow( void );

	// TGObject
	// FUNCTION: STBC 0x005596b0 ?GetObjType@TGWindow@@UAEHXZ
	virtual int		GetObjType( void ) { return TGWINDOW_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00559720 ?GetClassName@TGWindow@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGWindow"; }
	// FUNCTION: STBC 0x00559730 ?GetSwigTypeName@TGWindow@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGWindow"; }
	// FUNCTION: STBC 0x00559740 ?GetPythonClassName@TGWindow@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGWindowPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00559750 ?GetClassHandlers@TGWindow@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );
	virtual void	SetExclusive( bool layout = true );
	virtual void	SetNotExclusive( bool layout = true );

	// TGPane
	virtual void	InsertChild( int index, TGUIObject *child, float x = 0.0f, float y = 0.0f, bool layout = true );
	virtual void	RemoveChild( TGUIObject *child, bool layout = true );
	virtual void	DeleteChild( TGUIObject *child, bool layout = true );
	virtual int		GetNumChildren( void );
	virtual TGUIObject *	GetFirstChild( void );
	virtual TGUIObject *	GetLastChild( void );
	virtual TGUIObject *	GetNthChild( int index );
	virtual TGUIObject *	GetNextChild( TGUIObject *child );
	virtual TGUIObject *	GetPrevChild( TGUIObject *child );
	virtual void	KillChildren( void );
	virtual void	MoveToFront( TGUIObject *child, bool layout = true );
	virtual void	MoveToBack( TGUIObject *child, bool layout = true );
	virtual void	SetFocus( TGUIObject *child, bool layout = true );
	virtual TGUIObject *	GetFocus( void );
	virtual void	MoveFocus( int direction, bool wrap = false, bool layout = true );

	// FUNCTION: STBC 0x00518af0 ?GetSubPane@TGWindow@@UAEPAVTGPane@@XZ
	virtual TGPane *	GetSubPane( void ) { return (TGPane *)TGPane::GetFirstChild(); }

	static TGWindow *	Create( float width, float height, float indentX, float indentY, TGPane *subPane );
	static TGWindow *	Cast( TGObject *object );
	static void		RegisterHandlers( void );			// invented name; the window adds none
	void	SetIndent( float x, float y );
	bool	IsSimpleResize( void ) { return ( flags & TGWINDOW_SIMPLE_RESIZE ) != 0; }		// invented name
	void	SetSimpleResize( void ) { flags |= TGWINDOW_SIMPLE_RESIZE; }		// invented name

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x005596c0
inline bool TGWindow::IsTypeOf( int type ) {
	if ( type == TGWINDOW_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__TGWINDOW_H__ */
