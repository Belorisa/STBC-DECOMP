/*
===============================================================================

	RadarBlip.h

	A radar scope's icon for one ship: Layout sizes it to its artwork and
	then turns the icon's poly to point the way the ship lies from the
	player.

	0x54 bytes, allocated from the pools.

	Evidence: the constructor (0x5431D0) calls TGIcon's with no color,
	stores the vtable 0x891DBC (TGIcon's 64 slots; the type and name slots,
	the stream slots, GetClassHandlers and Layout overridden) and the ship's
	ID at 0x50, which the SWIG wrappers RadarBlip_SetShipID and GetShipID
	write and read; RadarBlip_Create passes 0 for it. GetObjType (0x543200)
	returns 0x8105; the class strings are "RadarBlip", "_p_RadarBlip" and
	"RadarBlipPtr". The destructor (0x5432E0) stores the vtable before
	TGIcon's. The code lies in the radar scope's unit.

===============================================================================
*/

#ifndef __RADARBLIP_H__
#define __RADARBLIP_H__

#include "TGIcon.h"

// GetObjType's value for a RadarBlip.
#define RADARBLIP_TYPE			0x8105

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005432b0 ??_GRadarBlip@@UAEPAXI@Z @game/RadarScope
class RadarBlip : public TGIcon {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558ca0 ??0RadarBlip@@QAE@J@Z @game/InterfaceModule
					RadarBlip( long id ) : TGIcon( id ) {}
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					RadarBlip( char *groupName, int iconNum, long shipID );
	virtual			~RadarBlip( void );

	static RadarBlip *	Create( char *groupName, int iconNum ) { return new RadarBlip( groupName, iconNum, 0 ); }

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// TGUIObject
	virtual void	Layout( void );

	void			SetShipID( long id ) { shipID = id; }
	long			GetShipID( void ) { return shipID; }

protected:
	// Turns the poly to point the way the ship lies from the player
	// (invented name).
	void			UpdateDirection( void );

	long			shipID;					// 0x50 the ship the blip shows

	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__RADARBLIP_H__ */
