/*
===============================================================================

	EngPowerCtrl.h

	The engineering power control: a pane of 0x60 bytes holding a bar per
	powered subsystem. The vtable 0x892548 is TGPane's 82 slots with the
	destructor, the type, name and class handler slots, the stream, fix-up and
	post fix-up slots and LostTrueFocusPath overridden. The one control that
	exists is kept in a static (0x98AEF8): the constructor sets it, the
	destructor clears it, and the SWIG wrapper EngPowerCtrl_GetPowerCtrl
	returns it.

===============================================================================
*/

#ifndef __ENGPOWERCTRL_H__
#define __ENGPOWERCTRL_H__

#include "TGPane.h"
#include "NiTMap.h"

class PoweredSubsystem;
class STNumericBar;
class ShipClass;
class ShipSubsystem;
class TGStream;
class TGKeyboardEvent;

// The event type of a subsystem's wanted power changing (the SWIG constant).
#define ET_SUBSYSTEM_POWER_CHANGED	0x80008C

// The event the control's timer sends it (invented name).
#define ET_ENG_POWER_REFRESH		0x80008D

// GetObjType's value for an EngPowerCtrl (its GetObjType 0x54D140; class
// strings "EngPowerCtrl", "_p_EngPowerCtrl", "EngPowerCtrlPtr").
#define ENGPOWERCTRL_TYPE		0x8107

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0054d1f0 ??_GEngPowerCtrl@@UAEPAXI@Z
class EngPowerCtrl : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558e30 ??0EngPowerCtrl@@QAE@J@Z @game/InterfaceModule
					EngPowerCtrl( long id ) : TGPane( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					EngPowerCtrl( float width );
	virtual			~EngPowerCtrl( void );

	// TGObject
	// FUNCTION: STBC 0x0054d140 ?GetObjType@EngPowerCtrl@@UAEHXZ
	virtual int		GetObjType( void ) { return ENGPOWERCTRL_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0054d1b0 ?GetClassName@EngPowerCtrl@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "EngPowerCtrl"; }
	// FUNCTION: STBC 0x0054d1c0 ?GetSwigTypeName@EngPowerCtrl@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_EngPowerCtrl"; }
	// FUNCTION: STBC 0x0054d1d0 ?GetPythonClassName@EngPowerCtrl@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "EngPowerCtrlPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0054d1e0 ?GetClassHandlers@EngPowerCtrl@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	LostTrueFocusPath( bool layout = true );

	// The control; NULL until one is made (the SWIG wrapper EngPowerCtrl_GetPowerCtrl).
	static EngPowerCtrl *	GetPowerCtrl( void ) { return instance; }

	void	Refresh( void );
	STNumericBar *	GetBarForSubsystem( ShipSubsystem * );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	HandleSetPlayer( TGEvent *event );			// handler "EngPowerCtrl::HandleSetPlayer"
	void	HandlePowerChange( TGEvent *event );		// handler "EngPowerCtrl::HandlePowerChange"
	void	HandlePeriodicRefresh( TGEvent *event );	// handler "EngPowerCtrl::HandlePeriodicRefresh"
	void	HandleKeyboard( TGKeyboardEvent *event );			// handler "EngPowerCtrl::HandleKeyboard"
	void	HandleSelectOption( TGEvent *event );		// handler "EngPowerCtrl::HandleSelectOption"

protected:
	// Fills the control with a bar for each powered subsystem of the ship (invented name).
	void	Unknown54D410( ShipClass *ship );
	// A row with a power bar for the subsystem in the colour (invented name).
	TGPane *	CreateBar( PoweredSubsystem *subsystem, NiColorA color );
	// Focuses the nearest focusable bar before (previous) or after the
	// focus; past the end the parent moves its focus (invented name).
	void	MoveSelection( bool previous );

	static EngPowerCtrl *	instance;				// 0x98AEF8 the one control (invented name)
	static TGEventHandlerTable	classHandlers;		// invented name

	// Tells the subsystem's ship that its wanted power changed (invented name).
	void	SendPowerChangedEvent( PoweredSubsystem *subsystem );

	NiTMap<STNumericBar *, long>	barSubsystemIDs;	// 0x4C (invented name) 37 buckets; the bars by the ID of the subsystem each shows (GetBarForSubsystem, 0x54DC90)
	long			timerID;				// 0x5C the timer that refreshes the control (invented name)

	// Declared for the SWIG wrappers that call them.
public:
	static EngPowerCtrl *	Create( float width ) { return new EngPowerCtrl( width ); }	// the SWIG wrapper EngPowerCtrl_Create expands it
};

/*
================
EngPowerCtrl::IsTypeOf
================
*/
// FUNCTION: STBC 0x0054d150 ?IsTypeOf@EngPowerCtrl@@UAE_NH@Z
inline bool EngPowerCtrl::IsTypeOf( int type ) {
	if ( type == ENGPOWERCTRL_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__ENGPOWERCTRL_H__ */
