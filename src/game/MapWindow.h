/*
===============================================================================

	MapWindow.h

	The members of MapWindow that the SWIG wrappers MapWindow_* call, and the
	constructor Game's constructor (0x405C10) calls. No virtual functions
	are declared yet.

	0x68 bytes. Evidence: the constructor (0x4FE560) calls MainWindow's
	with MWT_TACTICAL_MAP (1.0 by 1.0), constructs the member at 0x58,
	stores the vtable 0x889C4C, adds the MapWindow:: broadcast handlers
	and builds the "Sensor Interference" pane; it clears 0x50, 0x54 and
	0x64.

===============================================================================
*/

#ifndef __MAPWINDOW_H__
#define __MAPWINDOW_H__

#include "MainWindow.h"
#include "NiAVObject.h"
#include "NiPointer.h"
#include "NiPoint3.h"
#include "NiColor.h"
#include "NiTList.h"

class TGObject;
class NiLines;
class ObjectClass;
class ShipClass;
class TGEvent;
class TGStream;

// GetObjType's value for a MapWindow (the type its Cast checks).
#define MAPWINDOW_TYPE		0x8116

// The event of the map's periodic update timer (App's ET_MAP_WINDOW_PERIODIC_UPDATE).
#define ET_MAP_WINDOW_PERIODIC_UPDATE		0x8000A4

// The navigation point changed (the SWIG constant of the App module).
#define ET_NAV_POINT_CHANGED		0x8000C1

// A ship's stalk in the map: the line from the ship's position dropped onto the player's plane up
// to the ship, with a colour at each end (invented name; Unknown500BA0 builds them, Unknown500F50
// turns them into line segments).
struct MapStalk {
	NiPoint3	base;				// 0x00 the ship's position dropped onto the player's plane
	NiPoint3	tip;				// 0x0C the ship's position
	NiColorA	baseColor;			// 0x18
	NiColorA	tipColor;			// 0x28
	float		baseWidth;			// 0x38 (the line width factor; not read by Unknown500F50)
	float		tipWidth;			// 0x3C

				MapStalk( const NiPoint3 &base_, const NiPoint3 &tip_, const NiColorA &baseColor_, const NiColorA &tipColor_, float baseWidth_, float tipWidth_ )
					: base( base_ ), tip( tip_ ), baseColor( baseColor_ ), tipColor( tipColor_ ) {
					baseWidth = baseWidth_;
					tipWidth = tipWidth_;
				}
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// Vtable 0x889C4C: MainWindow's 84 slots. GetObjType, IsTypeOf, the class
// names, GetClassHandlers and the destructor are header inlines whose
// copies the class factory's unit keeps (0x43EB10-0x43EBF0).
// FUNCTION: STBC 0x0043ebc0 ??_GMapWindow@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x0043ebf0 ??1MapWindow@@UAE@XZ @game/UtopiaModule
class MapWindow : public MainWindow {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// FUNCTION: STBC 0x0043eae0 ??0MapWindow@@QAE@J@Z @game/UtopiaModule
					MapWindow( long id ) : MainWindow( id ) {}	// the loader's: the stream fills the members
					MapWindow( void );

	// TGObject
	// FUNCTION: STBC 0x0043eb10 ?GetObjType@MapWindow@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return MAPWINDOW_TYPE; }
	// FUNCTION: STBC 0x0043eb20 ?IsTypeOf@MapWindow@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) { if ( type == MAPWINDOW_TYPE ) return true; return MainWindow::IsTypeOf( type ); }
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0043eb80 ?GetClassName@MapWindow@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "MapWindow"; }
	// FUNCTION: STBC 0x0043eb90 ?GetSwigTypeName@MapWindow@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_MapWindow"; }
	// FUNCTION: STBC 0x0043eba0 ?GetPythonClassName@MapWindow@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "MapWindowPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0043ebb0 ?GetClassHandlers@MapWindow@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );
	virtual void	LostFocus( bool layout = true );

	// MainWindow
	virtual void	Initialize( void );
	virtual void	Update( float time );

	static MapWindow *	Cast( TGObject * );
	void	UpdatePlayerOrientationGraphics( void );
	void	RemovePlayerOrientationGraphics( void );
	// Builds the player's orientation graphics at 0x58: a node of four meshes, the horizontal and the vertical ring
	// of the player's sensor range, their tick marks and the disc inside (0x4FF0F0; invented name).
	void	Unknown4FF0F0( ShipClass *player );
	// Gives the graphic the vertex colour, depth, white texture and texture mode properties of the map's meshes
	// (0x500990; invented name).
	void	Unknown500990( NiAVObject *object );
	// Builds the stalks of the ships the player can target within range and attaches them to the
	// set's scene (0x500BA0; invented name: the second argument is not read).
	void	Unknown500BA0( ShipClass *player, ObjectClass *target, int count, ObjectClass **ships );
	// Turns the stalks (emptying the list) into one line graphic (0x500F50; invented name).
	static NiLines *	Unknown500F50( NiTList<MapStalk *> *stalks );

	// Register the handler methods by name, and add the class handlers.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			GroupChangeHandler( TGEvent *event );
	void			AddShipHandler( TGEvent *event );
	void			NavPointChanged( TGEvent *event );
	void			PeriodicUpdateHandler( TGEvent *event );
	void			TargetChangedHandler( TGEvent *event );

	// Gives the object a reticle unless it has one (invented name).
	void			AddReticle( ObjectClass *object );
	// Gives each ship the player's sensors see, the player's target and the
	// visible placements a reticle (invented name).
	void			AddReticles( void );

	// Shows the "Sensor Interference" pane, laid out (invented name).
	void			Unknown4FEE00( void );
	// TODO identity: hides the first child pane while it is shown
	void			Unknown4FEF30( void );
	// TODO identity: detaches the graphic at 0x5C from its parent and drops it
	void			Unknown501380( void );

protected:
	float			unknown50;				// 0x50 the time of the last Update; 0 at first
	float			unknown54;				// 0x54 time since the first child's icon last turned
	NiPointer<NiNode>	unknown58;		// 0x58 a player orientation graphic, detached by RemovePlayerOrientationGraphics
	NiPointer<NiAVObject>	unknown5C;		// 0x5C a graphic Unknown501380 detaches
	NiPointer<NiAVObject>	unknown60;		// 0x60 a player orientation graphic, detached by RemovePlayerOrientationGraphics
	long			unknown64;				// 0x64 the periodic update timer's ID

	static TGEventHandlerTable	classHandlers;		// invented name
	static const float	TWO_PI;						// 0x88E5E4 (invented name)
	static const float	STALK_RANGE;				// 250.0: the rings' radius too (invented name)
	static const float	STALK_WIDTH;				// 1.25: the rings' thickness too (invented name)
	static const float	PERIODIC_UPDATE_INTERVAL;	// seconds between the map's periodic updates (invented name)
	static const float	RING_STEP;					// 0x88E5F8: one segment of a ring, as a fraction of the turn (invented name)
};

#endif /* !__MAPWINDOW_H__ */
