/*
===============================================================================

	STCharacterMenu.h

	The menu of a bridge character: an STMenu that catches its buttons'
	clicks and passes them up its parents itself.

	0x5C bytes, no member of its own (its scalar deleting destructor frees
	0x5C).

	Evidence: the constructors (0x51B4A0 with a C string and 0x51B5C0 with
	a TGString, called by STCharacterMenu_Create and _CreateW with two
	optional colours) call STMenu's with STMF_DEFAULTS, no event and no
	sub pane and store the vtable 0x88F97C (STMenu's 115 slots); the
	colours are not used. GetObjType returns 0x80E5. The handler name is
	the string "STCharacterMenu::ButtonClickedHandler", registered for
	ET_ST_BUTTON_CLICKED. The destructor is implicit; the copy retail kept
	(0x516EC0) is another unit's.

===============================================================================
*/

#ifndef __STCHARACTERMENU_H__
#define __STCHARACTERMENU_H__

#include "STMenu.h"

class NiColorA;
class TGBoolEvent;

// GetObjType's value for an STCharacterMenu.
#define STCHARACTERMENU_TYPE	0x80E5

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0051b590 ??_GSTCharacterMenu@@UAEPAXI@Z
// FUNCTION: STBC 0x00516ec0 ??1STCharacterMenu@@UAE@XZ
class STCharacterMenu : public STMenu {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00557bd0 ??0STCharacterMenu@@QAE@J@Z @game/InterfaceModule
					STCharacterMenu( long id ) : STMenu( id ) {}
					STCharacterMenu( char *name, NiColorA *color1 = NULL, NiColorA *color2 = NULL );
					STCharacterMenu( TGString *name, NiColorA *color1 = NULL, NiColorA *color2 = NULL );
	static STCharacterMenu *	Create( char *name, NiColorA *color1, NiColorA *color2 ) { return new STCharacterMenu( name, color1, color2 ); }	// the SWIG wrapper STCharacterMenu_Create expands it
	static STCharacterMenu *	CreateW( TGString *name, NiColorA *color1, NiColorA *color2 ) { return new STCharacterMenu( name, color1, color2 ); }	// the SWIG wrapper STCharacterMenu_CreateW expands it

	// TGObject
	// FUNCTION: STBC 0x0051b4d0 ?GetObjType@STCharacterMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return STCHARACTERMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0051b550 ?GetClassName@STCharacterMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STCharacterMenu"; }
	// FUNCTION: STBC 0x0051b560 ?GetSwigTypeName@STCharacterMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STCharacterMenu"; }
	// FUNCTION: STBC 0x0051b570 ?GetPythonClassName@STCharacterMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STCharacterMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0051b580 ?GetClassHandlers@STCharacterMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			ButtonClickedHandler( TGBoolEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STCharacterMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x0051b4e0
inline bool STCharacterMenu::IsTypeOf( int type ) {
	if ( type == STCHARACTERMENU_TYPE ) {
		return true;
	}
	return STMenu::IsTypeOf( type );
}

#endif /* !__STCHARACTERMENU_H__ */
