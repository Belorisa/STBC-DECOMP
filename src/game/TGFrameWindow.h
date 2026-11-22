/*
===============================================================================

	TGFrameWindow.h

	A window drawn with a frame (TGFrameWindow_GetFramePane returns it).

	0x4C bytes, the size of a TGWindow: it adds no members (the scalar
	deleting destructor 0x725EB0 frees through the pool of that size).

	Evidence: the vtable 0x896D14 (83 slots, none new) overrides the
	destructor (0x7451A0), the type members and Resize (0x7451D0).
	GetObjType returns 0x20D. The load constructor and the type's inline
	members are emitted with the UI module's class factories (0x725DE0 to
	0x725EB0).

===============================================================================
*/

#ifndef __TGFRAMEWINDOW_H__
#define __TGFRAMEWINDOW_H__

#include "TGWindow.h"

class TGFrame;
class TGPane;

// GetObjType's value for a TGFrameWindow.
#define TGFRAMEWINDOW_TYPE		0x20D

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00725eb0 ??_GTGFrameWindow@@UAEPAXI@Z
class TGFrameWindow : public TGWindow {
public:
	// FUNCTION: STBC 0x00725de0 ??0TGFrameWindow@@QAE@J@Z
					TGFrameWindow( long id = 0 ) : TGWindow( id ) {}
					TGFrameWindow( float, float, char *, int, float, int, TGPane * );
	virtual			~TGFrameWindow( void );

	// TGObject
	// FUNCTION: STBC 0x00725e00 ?GetObjType@TGFrameWindow@@UAEHXZ
	virtual int		GetObjType( void ) { return TGFRAMEWINDOW_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00725e70 ?GetClassName@TGFrameWindow@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGFrameWindow"; }
	// FUNCTION: STBC 0x00725e80 ?GetSwigTypeName@TGFrameWindow@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGFrameWindow"; }
	// FUNCTION: STBC 0x00725e90 ?GetPythonClassName@TGFrameWindow@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGFrameWindowPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00725ea0 ?GetClassHandlers@TGFrameWindow@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );

	// Converts without a type test: the SWIG wrapper TGFrameWindow_Cast passes the
	// pointer through unchanged.
	static TGFrameWindow *	Cast( TGObject *object ) { return (TGFrameWindow *)object; }
	static TGFrameWindow *	CreateFromTheme( float, float, TGPane * );
	static TGFrameWindow *	Create( float, float, char *, int, float, int, TGPane * );
	TGFrame *	GetFramePane( void );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGFrameWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x00725e10
inline bool TGFrameWindow::IsTypeOf( int type ) {
	if ( type == TGFRAMEWINDOW_TYPE ) {
		return true;
	}
	return TGWindow::IsTypeOf( type );
}

#endif /* !__TGFRAMEWINDOW_H__ */
