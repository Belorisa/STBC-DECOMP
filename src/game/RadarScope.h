/*
===============================================================================

	RadarScope.h

	The tactical radar's scope: a pane of 0x70 bytes whose children the
	script Tactical.Interface.RadarScope.Create builds (the ship icon, the
	radar ring, the bracket, target bracket, blip, phaser line and
	background panes, in the order of the SWIG constants RadarScope_*).
	Update puts a RadarBlip in the blip pane for each object of the
	player's set the sensors show; a weapon hit on the player puts a
	bracket blip on the attacker for BRACKET_TIME seconds.

	Evidence: the constructor (0x543670) calls TGPane's with 0, builds the
	list at 0x4C with 2 items per block, stores the vtable 0x891EBC
	(TGPane's 82 slots and no more; the destructor, the type, name, class
	handler and stream slots overridden), sets 0x68 and clears 0x6C, and
	adds the broadcast handlers named "RadarScope::GroupChangeHandler",
	"RadarScope::SetObjectUpdate" and "RadarScope::SetPlayerHandler".
	GetObjType (0x5437B0) returns 0x8104; the class strings are
	"RadarScope", "_p_RadarScope" and "RadarScopePtr". The scalar deleting
	destructor (0x543860) frees 0x70 bytes through the pools.

===============================================================================
*/

#ifndef __RADARSCOPE_H__
#define __RADARSCOPE_H__

#include "TGPane.h"
#include "NiTList.h"

class NiColorA;
class ObjectClass;
class RadarBlip;
class TGEvent;
class TGIcon;
class TGObject;
class TGObjPtrEvent;
class TGStream;
class WeaponHitEvent;

// GetObjType's value for a RadarScope.
#define RADARSCOPE_TYPE			0x8104

// The scope's events (the SWIG constants of the same names).
#define ET_RADAR_DISPLAY_OBJECT_UPDATE		0x8000A8
#define ET_RADAR_DISPLAY_BRACKET_EXPIRE		0x8000AA

/*
================
RadarBracket

A bracket blip the scope shows on a ship that hit the player, with the
timer that takes it down (invented name; the 12-byte record the weapon hit
handler allocates).
================
*/
struct RadarBracket {
	RadarBlip *		blip;					// 0x00 in the bracket pane
	long			timerID;				// 0x04 sends the expire event
	long			eventID;				// 0x08 the expire event
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00543860 ??_GRadarScope@@UAEPAXI@Z
class RadarScope : public TGPane {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558d00 ??0RadarScope@@QAE@J@Z @game/InterfaceModule
					RadarScope( long id ) : TGPane( id ), brackets( 2 ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// The children the script builds (the SWIG constants RadarScope_*).
	enum {
		SHIP_ICON,
		RADAR_RING,
		BRACKET_PANE,
		TARGET_BRACKET,
		BLIP_PANE,
		PHASER_LINE_PANE,
		BACKGROUND_PANE
	};

					RadarScope( float width, float height );
	virtual			~RadarScope( void );

	// TGObject
	// FUNCTION: STBC 0x005437b0 ?GetObjType@RadarScope@@UAEHXZ
	virtual int		GetObjType( void ) { return RADARSCOPE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00543820 ?GetClassName@RadarScope@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "RadarScope"; }
	// FUNCTION: STBC 0x00543830 ?GetSwigTypeName@RadarScope@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_RadarScope"; }
	// FUNCTION: STBC 0x00543840 ?GetPythonClassName@RadarScope@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "RadarScopePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00543850 ?GetClassHandlers@RadarScope@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	static RadarScope *	Create( float width, float height ) { return new RadarScope( width, height ); }	// the SWIG wrapper RadarScope_Create expands it
	void	Update( void );
	TGIcon *	CreateShipIcon( void );
	void	SetTargetBracket( TGUIObject *bracket ) { targetBracket = bracket; }

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void	GroupChangeHandler( TGEvent *event );
	void	SetObjectUpdate( TGEvent *event );
	void	SetPlayerHandler( TGObjPtrEvent *event );
	void	PlayerDamagedHandler( WeaponHitEvent *event );
	void	BracketExpireHandler( TGObjPtrEvent *event );

protected:
	// Takes the children and the brackets down and has the script build
	// the children again (invented name).
	void		Rebuild( void );
	// The range the scope shows: the given one, or the player's sensor
	// range for 0, at most MAX_LIMITED_RANGE when limitRange is set
	// (invented name).
	float		GetRange( float range );
	// The colour of an object's blip: an incoming torpedo's, or its
	// affiliation's (invented name).
	NiColorA	GetBlipColor( ObjectClass *object );
	// The bracket blip on the object; NULL for none (invented name).
	RadarBlip *	FindBracketBlip( ObjectClass *object );
	// Whether the object is a torpedo homing on the player (invented name).
	bool		IsIncomingTorpedo( ObjectClass *object );
	// Takes the blips down (invented name).
	void		ClearBlips( void );
	// Takes the brackets down, with their timers (invented name).
	void		ClearBrackets( void );

	NiTList<RadarBracket *>	brackets;		// 0x4C (invented name)
	TGUIObject *	targetBracket;			// 0x64
	bool			limitRange;				// 0x68 true from the constructor (invented name)
	long			playerID;				// 0x6C the ship whose weapon hits the scope listens to (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
	static float	unknown8E3E44;			// saved and loaded with the scope; nothing else reads it
};

/*
================
RadarScope::IsTypeOf
================
*/
// FUNCTION: STBC 0x005437c0 ?IsTypeOf@RadarScope@@UAE_NH@Z
inline bool RadarScope::IsTypeOf( int type ) {
	if ( type == RADARSCOPE_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__RADARSCOPE_H__ */
