/*
===============================================================================

	TGOverlayAction.h

	A timed action that shows an overlay on one of a character's overlay
	controllers for its duration: Play adds the overlay, and the
	destructor takes it back.

	0x38 bytes, allocated from the pools.

	Evidence: the constructor (0x709140) calls TGTimedAction's, holds a
	reference to the overlay manager, copies the controller's and the
	overlay's names and sets the duration; the load's constructor
	(0x6FD8C0) and the creator (0x6FD890, type 0x70E) are the actions
	module's. The destructor (0x709240) removes the overlay when the
	manager and both names are set. The vtable 0x896658 has TGAction's 24
	slots; GetObjType returns 0x70E and IsTypeOf accepts TGTimedAction's
	and TGAction's types too. The class strings are "TGOverlayAction",
	"_p_TGOverlayAction", "TGOverlayActionPtr" and "CT_TG_OVERLAY_ACTION".
	The manager is saved by its address and found again through the
	pointer map after a load.

===============================================================================
*/

#ifndef __TGOVERLAYACTION_H__
#define __TGOVERLAYACTION_H__

#include "TGTimedAction.h"
#include "TGOverlayManager.h"
#include "NiPointer.h"

// GetObjType's value for a TGOverlayAction.
#define TGOVERLAYACTION_TYPE	0x70E

// FUNCTION: STBC 0x006fd9c0 ??_GTGOverlayAction@@UAEPAXI@Z @game/TGActionModule
class TGOverlayAction : public TGTimedAction {
public:
					TGOverlayAction( TGOverlayManager *manager_, char *controller_, char *overlay_, float duration_ );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006fd8c0 ??0TGOverlayAction@@QAE@J@Z @game/TGActionModule
					TGOverlayAction( long id ) : TGTimedAction( id ) { manager = NULL; }
	virtual			~TGOverlayAction( void );

	// TGObject
	// FUNCTION: STBC 0x006fd940 ?GetObjType@TGOverlayAction@@UAEHXZ @game/TGActionModule
	virtual int		GetObjType( void ) { return TGOVERLAYACTION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x006fd990 ?GetClassName@TGOverlayAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetClassName( void ) { return "TGOverlayAction"; }
	// FUNCTION: STBC 0x006fd9a0 ?GetSwigTypeName@TGOverlayAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGOverlayAction"; }
	// FUNCTION: STBC 0x006fd9b0 ?GetPythonClassName@TGOverlayAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetPythonClassName( void ) { return "TGOverlayActionPtr"; }

	// TGAction
	virtual void	Play( void );

protected:
	NiPointer<TGOverlayManager>	manager;	// 0x28
	long			savedManager;			// 0x2C the manager's address when saved, until PostFixUp (invented name)
	char *			controller;				// 0x30 a copy of the controller's name
	char *			overlay;				// 0x34 a copy of the overlay's name
};

/*
================
TGOverlayAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fd950 @game/TGActionModule
inline bool TGOverlayAction::IsTypeOf( int type ) {
	if ( type == TGOVERLAYACTION_TYPE ) {
		return true;
	}
	return TGTimedAction::IsTypeOf( type );
}

#endif /* !__TGOVERLAYACTION_H__ */
