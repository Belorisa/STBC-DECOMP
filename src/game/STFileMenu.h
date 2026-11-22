/*
===============================================================================

	STFileMenu.h

	The members of STFileMenu that the SWIG wrappers STFileMenu_* call. Only what the
	wrappers prove is declared: the two strings they replace, and no virtual
	functions yet.

	Evidence: SetDir (0x521110) and SetFilter (0x521180) free and replace the
	strings at 0x64 and 0x68, the first members after STTopLevelMenu's 0x64
	bytes; Refresh (0x5211F0) lists the directory into the menu with them.
	The type STFileMenu_Cast tests for is 0x80E6.

===============================================================================
*/

#ifndef __STFILEMENU_H__
#define __STFILEMENU_H__

#include "STTopLevelMenu.h"

class TGObject;

// GetObjType's value for an STFileMenu.
#define STFILEMENU_TYPE			0x80E6

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TGString;
// FUNCTION: STBC 0x00520ff0 ??_GSTFileMenu@@UAEPAXI@Z
// FUNCTION: STBC 0x00521020 ??1STFileMenu@@UAE@XZ
class STFileMenu : public STTopLevelMenu {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x005580b0 ??0STFileMenu@@QAE@J@Z @game/InterfaceModule
					STFileMenu( long id ) : STTopLevelMenu( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// TGObject
	// FUNCTION: STBC 0x00520f20 ?GetObjType@STFileMenu@@UAEHXZ
	virtual int		GetObjType( void ) { return STFILEMENU_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00520fb0 ?GetClassName@STFileMenu@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STFileMenu"; }
	// FUNCTION: STBC 0x00520fc0 ?GetSwigTypeName@STFileMenu@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STFileMenu"; }
	// FUNCTION: STBC 0x00520fd0 ?GetPythonClassName@STFileMenu@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STFileMenuPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00520fe0 ?GetClassHandlers@STFileMenu@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// STMenu
	virtual void	SetFocus( TGUIObject *child, bool layout = true );

	static STFileMenu *	Cast( TGObject * );
	char *	GetDir( void ) { return dir; }
	char *	GetFilter( void ) { return filter; }
	void	SetDir( char * );
	void	SetFilter( char * );
	void	Refresh( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	HandleKeyboard( TGEvent *event );		// handler "STFileMenu::HandleKeyboard"

protected:
	static TGEventHandlerTable	classHandlers;	// invented name
	char *			dir;					// 0x64 the directory listed (owned copy; invented name)
	char *			filter;					// 0x68 the file name filter (owned copy; invented name)

	// Declared for the SWIG wrappers that call them.
public:
					STFileMenu( TGString *name, char *dir, char *filter );
					STFileMenu( char *name, char *dir, char *filter );
	static STFileMenu *	Create( char *name, char *dir, char *filter ) { return new STFileMenu( name, dir, filter ); }	// the SWIG wrapper STFileMenu_Create expands it
	static STFileMenu *	CreateW( TGString *name, char *dir, char *filter ) { return new STFileMenu( name, dir, filter ); }	// the SWIG wrapper STFileMenu_CreateW expands it
};

/*
================
STFileMenu::IsTypeOf
================
*/
// FUNCTION: STBC 0x00520f30 ?IsTypeOf@STFileMenu@@UAE_NH@Z
inline bool STFileMenu::IsTypeOf( int type ) {
	if ( type == STFILEMENU_TYPE ) {
		return true;
	}
	return STTopLevelMenu::IsTypeOf( type );
}

#endif /* !__STFILEMENU_H__ */
