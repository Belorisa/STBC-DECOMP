/*
===============================================================================

	STComponentMenuItem.h

	A button of a component menu that stands for one subsystem of a ship:
	its name is the subsystem's display name, and its body is a fill
	gauge of the subsystem's condition.

	0x118 bytes (its scalar deleting destructor frees 0x118).

	Evidence: the constructor (0x536C80, called by STComponentMenu's item
	builder 0x5367C0 with the subsystem and an event) builds the button,
	stores the vtable 0x890FCC (STButton's 91 slots), keeps the
	subsystem's ID (0x114), names the button and puts a fill gauge in place
	of the body. GetObjType returns 0x80F5. The SWIG wrapper
	STComponentMenuItem_GetSubsystem looks the subsystem up by the ID.

===============================================================================
*/

#ifndef __STCOMPONENTMENUITEM_H__
#define __STCOMPONENTMENUITEM_H__

#include "STButton.h"

class ShipSubsystem;
class TGObject;

// GetObjType's value for an STComponentMenuItem.
#define STCOMPONENTMENUITEM_TYPE	0x80F5

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00536ef0 ??_GSTComponentMenuItem@@UAEPAXI@Z
class STComponentMenuItem : public STButton {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00557d90 ??0STComponentMenuItem@@QAE@J@Z @game/InterfaceModule
					STComponentMenuItem( long id ) : STButton( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					STComponentMenuItem( ShipSubsystem *subsystem, TGEvent *event );
	// FUNCTION: STBC 0x00536f20 ??1STComponentMenuItem@@UAE@XZ
	virtual			~STComponentMenuItem( void ) {}

	// TGObject
	// FUNCTION: STBC 0x00536e30 ?GetObjType@STComponentMenuItem@@UAEHXZ
	virtual int		GetObjType( void ) { return STCOMPONENTMENUITEM_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00536eb0 ?GetClassName@STComponentMenuItem@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STComponentMenuItem"; }
	// FUNCTION: STBC 0x00536ec0 ?GetSwigTypeName@STComponentMenuItem@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STComponentMenuItem"; }
	// FUNCTION: STBC 0x00536ed0 ?GetPythonClassName@STComponentMenuItem@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STComponentMenuItemPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00536ee0 ?GetClassHandlers@STComponentMenuItem@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static STComponentMenuItem *	Cast( TGObject * );
	ShipSubsystem *	GetSubsystem( void );

protected:
	long			subsystemID;			// 0x114

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STComponentMenuItem::IsTypeOf
================
*/
// FUNCTION: STBC 0x00536e40
inline bool STComponentMenuItem::IsTypeOf( int type ) {
	if ( type == STCOMPONENTMENUITEM_TYPE ) {
		return true;
	}
	return STButton::IsTypeOf( type );
}

#endif /* !__STCOMPONENTMENUITEM_H__ */
