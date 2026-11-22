/*
===============================================================================

	STWarpButton.h

	The warp button of the helm menu: an STButton that remembers where it
	warps to and keeps five sequences of actions that run around the warp
	(before it, before and during it, during it, after the start and after
	it).

	Evidence: GetObjType returns 0x80FD, the type STWarpButton::Cast
	(0x53E5B0) checks. The SWIG wrappers STWarpButton_* name Cast,
	SetLocation, SetPlacementName, ClearBDASequences and the five
	AddAction* functions, which add to the sequences at 0x140 to 0x150 in
	the order of their names, making a sequence when there is none. Only
	what these functions prove is laid out; the members between are
	unknown, and so are the rest of the class's virtual functions.

===============================================================================
*/

#ifndef __STWARPBUTTON_H__
#define __STWARPBUTTON_H__

#include "STButton.h"
#include "TGEventHandlerTable.h"

class TGAction;
class TGObject;
class SetClass;
class TGSequence;
class TGString;
class TGEvent;
class WarpSequence;
class TGStream;

// GetObjType's value for an STWarpButton.
#define STWARPBUTTON_TYPE			0x80FD

// The event the button sends itself (invented name; the SWIG constant is not
// known). It also listens for ET_EXITED_WARP (ShipClass.h).
#define ET_ST_WARP_BUTTON			0x8000C7

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0053e7d0 ??_GSTWarpButton@@UAEPAXI@Z
class STWarpButton : public STButton {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00557f70 ??0STWarpButton@@QAE@J@Z @game/InterfaceModule
					STWarpButton( long id ) : STButton( id ) {}
					STWarpButton( char *name );
					STWarpButton( TGString *name );
	static STWarpButton *	Create( char *name ) { return new STWarpButton( name ); }	// the SWIG wrapper STWarpButton_Create expands it
	static STWarpButton *	CreateW( TGString *name ) { return new STWarpButton( name ); }	// the SWIG wrapper STWarpButton_CreateW expands it
	// The course menu and the ship (the SWIG wrappers SetCourseMenu and SetShip store them at 0x134 and 0x138).
	void			SetCourseMenu( TGObject *menu ) { courseMenu = menu; }
	void			SetShip( TGObject *ship ) { this->ship = ship; }
	virtual			~STWarpButton( void );

	// TGObject
	// FUNCTION: STBC 0x0053e710 ?GetObjType@STWarpButton@@UAEHXZ
	virtual int		GetObjType( void ) { return STWARPBUTTON_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0053e790 ?GetClassName@STWarpButton@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STWarpButton"; }
	// FUNCTION: STBC 0x0053e7a0 ?GetSwigTypeName@STWarpButton@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STWarpButton"; }
	// FUNCTION: STBC 0x0053e7b0 ?GetPythonClassName@STWarpButton@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STWarpButtonPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0053e7c0 ?GetClassHandlers@STWarpButton@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static STWarpButton *	Cast( TGObject *object );
	void			AddActionBeforeWarp( TGAction *action, float delay );
	void			AddActionBeforeDuringWarp( TGAction *action, float delay );
	void			AddActionDuringWarp( TGAction *action, float delay );
	void			AddActionAfterDuringWarp( TGAction *action, float delay );
	void			AddActionAfterWarp( TGAction *action, float delay );
	void			ClearBDASequences( void );
	void			SetLocation( char *location );
	void			SetPlacementName( char *placementName );
	// Sets where the button warps to (invented name).
	// The SWIG wrapper STWarpButton_SetDestination calls it.
	void			SetDestination( char *newDestination, char *name, char *placement, char *otherName, long id );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	char *	GetLocation( void ) { return location; }	// the region the button is in (invented name; SortedRegionMenu::SendActivationEvent compares it)
	char *	GetDestination( void ) { return destination; }
	long	GetDestinationButtonID( void ) { return destinationButtonID; }
	void	SetUseLargeLoadingScreen( bool useLargeLoadingScreen_ ) { useLargeLoadingScreen = useLargeLoadingScreen_; }
	float	GetWarpTime( void ) { return warpTime; }
	void	SetWarpTime( float warpTime_ ) { warpTime = warpTime_; }

	// Python helpers of the handlers (invented names; they use no member).
	SetClass *		GetSetFromModule( char *moduleName );
	SetClass *		InitializeSetModule( char *moduleName );
	void			Unknown53ECB0( TGString *name );

	void			ExitedWarpHandler( TGEvent *event );	// registered under "STWarpButton::ExitedWarpHandler"
	void			HandleWarpEvent( TGEvent *event );		// registered under "STWarpButton::HandleWarpEvent"

protected:
	char *			unknown114;				// 0x114 a copy, or NULL
	char *			unknown118;				// 0x118 a copy, or NULL
	char *			location;				// 0x11C a copy, or NULL
	char *			destination;				// 0x120 a copy, or NULL
	char *			placementName;			// 0x124 a copy, or NULL
	bool			useLargeLoadingScreen;				// 0x128
	long			destinationButtonID;				// 0x12C an ID
	WarpSequence *	warpSequence;			// 0x130 the warp HandleWarpEvent started (invented name); an ID between ReadFromStream and PostFixUp
	TGObject *		courseMenu;				// 0x134 the same
	TGObject *		ship;					// 0x138 the same
	float			warpTime;				// 0x13C
	TGSequence *	beforeWarpSequence;			// 0x140 the sequences are made by the first action added to them
	TGSequence *	beforeDuringWarpSequence;	// 0x144
	TGSequence *	duringWarpSequence;			// 0x148
	TGSequence *	afterDuringWarpSequence;	// 0x14C
	TGSequence *	afterWarpSequence;			// 0x150

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
};

/*
================
STWarpButton::IsTypeOf
================
*/
// FUNCTION: STBC 0x0053e720
inline bool STWarpButton::IsTypeOf( int type ) {
	if ( type == STWARPBUTTON_TYPE ) {
		return true;
	}
	return STButton::IsTypeOf( type );
}

#endif /* !__STWARPBUTTON_H__ */
