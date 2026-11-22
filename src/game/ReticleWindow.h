/*
===============================================================================

	ReticleWindow.h

	The target reticles of the tactical view: ReticleWindow, the reticle
	with a name (NamedReticleWindow) and the player's own
	(PlayerReticleWindow). No SWIG wrappers: only the constructors and the
	sizes the game's code needs are declared.

	Evidence:
	- ReticleWindow, 0x7C bytes: the constructor (0x514F00) calls TGPane's
	  (1.0 by 1.0), constructs the TGRect at 0x6C, stores the vtable
	  0x88F4D0 and fills 0x4C-0x60 with the icons of the "ReticleTextures"
	  group. The vtable has TGPane's 82 slots and three of the class's own;
	  GetObjType returns 0x810B (App's CT_RETICLE_WINDOW). Slot 83
	  (0x515D50) keeps the object's ID at 0x4C while the object exists and
	  hides the reticle otherwise. The reticle manager calls the class's
	  slots and members on each of its reticle children.
	- NamedReticleWindow, 0x80 bytes: the constructor (0x50FEA0) calls
	  ReticleWindow's, stores the vtable 0x88F1E8 and keeps a pane at 0x7C;
	  the class strings are "NamedReticleWindow", "_p_NamedReticleWindow"
	  and "NamedReticleWindowPtr".
	- PlayerReticleWindow, 0xA0 bytes: the constructor (0x511680) calls
	  NamedReticleWindow's, stores the vtable 0x88F360 and builds its panes
	  and icons at 0x80-0x9C; the class strings are "PlayerReticleWindow",
	  "_p_PlayerReticleWindow" and "PlayerReticleWindowPtr". The game's
	  start (0x4060C0) allocates 0xA0 bytes for it.

===============================================================================
*/

#ifndef __RETICLEWINDOW_H__
#define __RETICLEWINDOW_H__

#include "TGPane.h"
#include "TGRect.h"

class NiColorA;
class TGEvent;
class TGObjPtrEvent;
class ObjectClass;
class ShipClass;
class TGEventHandlerTable;
class TGStream;
class TGIcon;
class NiCamera;
class NiPoint3;

// GetObjType's values of the reticles (App's CT_* constants).
#define RETICLEWINDOW_TYPE				0x810B
#define PLAYERRETICLEWINDOW_TYPE		0x810C
#define NAMEDRETICLEWINDOW_TYPE			0x810D

// The constructor's unwind in NamedReticleWindow's unit keeps a copy of the
// compiler's destructor.
// FUNCTION: STBC 0x00510170 ??1ReticleWindow@@UAE@XZ @game/NamedReticleWindow
// FUNCTION: STBC 0x00515250 ??_GReticleWindow@@UAEPAXI@Z @game/ReticleWindow
class ReticleWindow : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558f40 ??0ReticleWindow@@QAE@J@Z @game/InterfaceModule
					ReticleWindow( long id ) : TGPane( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					ReticleWindow( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );		// 0x515DA0
	virtual bool	ReadFromStream( TGStream *stream );		// 0x515E30
	virtual bool	FixUpPointers( void );					// 0x515EA0
	virtual void	PostFixUp( void );						// 0x515F00
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// The reticle's own slots (invented names).
	virtual void	VirtualSlot82( float time );		// 0x5152B0: follows the object
	virtual void	SetTargetID( long id );				// 0x515D50: the object to follow; hidden when it is gone
	virtual void	VirtualSlot84( void );				// 0x515D90: nothing in the class

	// Register the handler methods by name; the class has none.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	long			GetTargetID( void ) { return targetID; }	// invented name
	// Colours the reticle's five icons (invented name).
	void			SetColor( NiColorA *color );

	// TODO identity: empty in the class; the manager calls it on each reticle
	void			Unknown5152A0( void );
	// Fits the target arrow's pane to the tactical view's empty area while
	// the tactical window has the focus, to the whole screen otherwise; the
	// manager calls it on each reticle when the map window gets the focus.
	void			Unknown515F70( void );

	// Places the reticle on the object (invented name).
	void			Unknown515310( ObjectClass *object );
	// Projects a world point through the camera into its view port.
	void			Unknown515BC0( NiCamera *camera, const NiPoint3 &point, float &x, float &y );

protected:
	long			targetID;				// 0x4C the object the reticle follows; 0 for none (invented name)
	TGIcon *		corners[4];				// 0x50 "ReticleTextures" icons 0 to 3, one in each corner (invented name)
	TGIcon *		targetArrow;			// 0x60 the "TargetArrow" icon, alone in a pane (invented name)
	float			unknown64;				// 0x64
	float			unknown68;				// 0x68
	TGRect			unknown6C;				// 0x6C

	static TGEventHandlerTable	classHandlers;		// invented name
};

// The destructor is the compiler's; the unit that builds the class keeps
// it.
// FUNCTION: STBC 0x00510270 ??_GNamedReticleWindow@@UAEPAXI@Z @game/NamedReticleWindow
// FUNCTION: STBC 0x005102a0 ??1NamedReticleWindow@@UAE@XZ @game/NamedReticleWindow
// FUNCTION: STBC 0x00511440 ??_9@$BBFM@AE @game/NamedReticleWindow
// FUNCTION: STBC 0x00511450 ??_9@$BBGA@AE @game/NamedReticleWindow
// FUNCTION: STBC 0x00511460 ??_9@$BBFI@AE @game/NamedReticleWindow
class NamedReticleWindow : public ReticleWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558fe0 ??0NamedReticleWindow@@QAE@J@Z @game/InterfaceModule
					NamedReticleWindow( long id ) : ReticleWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					NamedReticleWindow( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );		// 0x510AC0
	virtual bool	ReadFromStream( TGStream *stream );		// 0x510B10
	virtual bool	FixUpPointers( void );					// 0x510B60
	virtual void	PostFixUp( void );						// 0x510B90
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// ReticleWindow
	virtual void	VirtualSlot82( float time );			// 0x510360
	virtual void	SetTargetID( long id );					// 0x510870
	virtual void	VirtualSlot84( void );					// 0x510980

	// The class's own slots: the broadcast handlers. RegisterHandlerNames
	// registers pointers to these virtuals, so retail keeps the compiler's
	// vcall thunks for them (0x511440-0x511460).
	// Colours the name with the object's affiliation (invented name).
	virtual void	VirtualSlot85( void );
	// The ship the event names was identified.
	virtual void	HandleShipIdentified( TGObjPtrEvent *event );
	// The object's group changed.
	virtual void	HandleObjectGroupChanged( TGEvent *event );
	// The event's destination changed its name.
	virtual void	HandleNameChange( TGEvent *event );

	// Register the handler methods by name; the class adds no handlers.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// Places the reticle on the object, then the name and the bars
	// around it (invented name).
	void			Unknown5103D0( ObjectClass *object );
	// TODO identity: lays the name and the bars out around the object
	void			Unknown5103F0( ObjectClass *object );


protected:
	TGPane *		unknown7C;				// 0x7C

	static TGEventHandlerTable	classHandlers;		// invented name
};

// The destructor is the compiler's; the unit that builds the class keeps
// it.
// FUNCTION: STBC 0x00511d00 ??_GPlayerReticleWindow@@UAEPAXI@Z @game/PlayerReticleWindow
// FUNCTION: STBC 0x00511d30 ??1PlayerReticleWindow@@UAE@XZ @game/PlayerReticleWindow
class PlayerReticleWindow : public NamedReticleWindow {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00559080 ??0PlayerReticleWindow@@QAE@J@Z @game/InterfaceModule
					PlayerReticleWindow( long id ) : NamedReticleWindow( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					PlayerReticleWindow( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );		// 0x513250
	virtual bool	ReadFromStream( TGStream *stream );		// 0x513320
	virtual bool	FixUpPointers( void );					// 0x5133C0
	virtual void	PostFixUp( void );						// 0x513440
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// ReticleWindow
	virtual void	VirtualSlot82( float time );			// 0x511DA0: follows the player's target
	virtual void	VirtualSlot84( void );					// 0x513020

	// Register the handler methods by name; the class has none.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// TODO identity: places the reticle on the player's target, with the
	// subsystem marker, the lead marker and the blinking lock bars
	void			Unknown511E70( ObjectClass *target, float time );
	// TODO identity: lays out the lock bars around the target
	void			Unknown512310( ShipClass *player, ObjectClass *target, float lock );
	// TODO identity: places the marker on the player's target subsystem
	void			Unknown513050( ShipClass *player, ObjectClass *target );

protected:
	TGPane *		unknown80;				// 0x80 a pane in front of the reticle: the name, the frame and the bars
	TGIcon *		subsystemMarker;		// 0x84 "ReticleTextures" icon 4, on the target subsystem (invented name)
	TGPane *		unknown88;				// 0x88 the bars' icons
	TGIcon *		unknown8C;				// 0x8C the frame icon
	bool			unknown90;				// 0x90
	float			unknown94;				// 0x94 the game time of the last blink
	float			unknown98;				// 0x98 seconds between blinks, 0.5 after construction
	bool			unknown9C;				// 0x9C the bars are lit

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
ReticleWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x005151b0 @game/ReticleWindow
inline bool ReticleWindow::IsTypeOf( int type ) {
	if ( type == RETICLEWINDOW_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

/*
================
NamedReticleWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x005101d0 @game/NamedReticleWindow
inline bool NamedReticleWindow::IsTypeOf( int type ) {
	if ( type == NAMEDRETICLEWINDOW_TYPE ) {
		return true;
	}
	return ReticleWindow::IsTypeOf( type );
}

#endif /* !__RETICLEWINDOW_H__ */
