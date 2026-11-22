/*
===============================================================================

	AsteroidField.h

	The members of AsteroidField that the SWIG wrappers AsteroidField_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __ASTEROIDFIELD_H__
#define __ASTEROIDFIELD_H__

#include "ObjectClass.h"
#include "NiTArray.h"
#include "NiTList.h"

class AsteroidTile;
class SetClass;
class ProximityCheck;
class ProximityEvent;
class ShipClass;
class TGObject;
class TGStringEvent;
struct AsteroidTileSlot;

// One tile of asteroids the field has made (invented name). 0x0C bytes from
// operator new (CreateTile, 0x41A760, which makes the tile); the field's tile
// list at 0x78 owns them.
struct AsteroidTileEntry {
	AsteroidTile *		tile;		// 0x00 the pooled tile object (0x84 bytes, CreateTile's)
	AsteroidTileSlot *	slot;		// 0x04 the slot it is placed in, or NULL
	bool				unknown08;	// 0x08
};

// A place around the field's centre that a tile can occupy (invented name);
// the field's slot list at 0x90 owns them.
struct AsteroidTileSlot {
	AsteroidTileEntry *	entry;		// 0x00 the tile placed here, or NULL
	bool				unknown04;	// 0x04
	NiPoint3			offset;		// 0x08 from the field's centre
};

// The vtable 0x888B84 keeps ObjectClass's 67 slots; the two words after it
// (0x888C90, 0x888C94) are the vtables of the slot and tile arrays.
// The SWIG inline accessors read GetOuterFieldRadius 0x6C, GetInnerFieldRadius
// and GetTileSize 0x70, GetNumAsteroidsPerTile 0xC8 and
// GetAsteroidSizeFactor 0xCC; the setters (0x41A9D0-0x41AA20) write them.
// GetObjType's value for an AsteroidField (the type its Cast checks).
#define ASTEROIDFIELD_TYPE		0x8014

// The event type that fires the field's proximity handler (the SWIG table's name).
#define ET_PROXIMITY_ASTEROID_FIELD		0x800062

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00419a30 ??_GAsteroidField@@UAEPAXI@Z @game/AsteroidField
class AsteroidField : public ObjectClass {
public:
					AsteroidField( long id, int tilesAcross_ );
	virtual			~AsteroidField( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	// The class's handler methods by name, and in the class's table (invented names).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );

	void HandleEnterSet( TGEvent *event );
	void HandleExitSet( TGEvent *event );
	void HandleShipEnterSet( TGEvent *event );
	void HandleShipExitSet( TGStringEvent *event );
	void HandleProximityEvent( ProximityEvent *event );

	// BaseObjectClass
	virtual void	Update( float time );

	// ObjectClass
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// a new field (the SWIG wrapper's default: 3 tiles across)
	static AsteroidField *	Create( int tilesAcross_ = 3 ) { return new AsteroidField( 0, tilesAcross_ ); }
	static AsteroidField *	Cast( TGObject * );
	void	SetOuterFieldRadius( float );
	void	SetInnerFieldRadius( float );
	void	SetNumAsteroidsPerTile( int );
	void	SetAsteroidSizeFactor( float );
	bool	IsShipInside( ShipClass * );
	static AsteroidField *	GetObject( SetClass *, char * );
	static AsteroidField *	GetObjectByID( SetClass *, long id );
	static int	GetNumClassObjects( SetClass * );

	float			GetOuterFieldRadius( void ) { return outerFieldRadius; }
	float			GetInnerFieldRadius( void ) { return innerFieldRadius; }
	float			GetTileSize( void ) { return innerFieldRadius * 2.0f / tilesAcross; }
	int				GetNumAsteroidsPerTile( void ) { return numAsteroidsPerTile; }
	float			GetAsteroidSizeFactor( void ) { return asteroidSizeFactor; }

protected:
	float			unknown68;				// 0x68 added to the outer radius for the proximity check
	float			outerFieldRadius;		// 0x6C
	float			innerFieldRadius;		// 0x70
	ProximityCheck *	proximityCheck;		// 0x74 its radius follows the outer radius; built by the event handler 0x419F00 (invented name)
	NiTArray<AsteroidTileEntry *>	tiles;	// 0x78 every tile made, placed or not (invented name)
	NiTArray<AsteroidTileSlot *>	slots;	// 0x90 (invented name)
	NiTList<ShipClass *>	unknownA8;		// 0xA8 the ships inside the proximity check (HandleProximityEvent); each is tested against the slots (IsShipNearSlot). Its AddTail/AddHead call NewItem 0x417BF0, SetClass::Unknown413CB0's NiTList<ShipClass *> copy
	int				unknownC0;				// 0xC0
	int				tilesAcross;			// 0xC4 tiles across the inner field: GetTileSize divides its width by it (invented name)
	int				numAsteroidsPerTile;	// 0xC8
	float			asteroidSizeFactor;		// 0xCC
	int				tileCount;				// 0xD0 numbers the tiles' names (invented name)

	void			UpdateSlot( AsteroidTileSlot *slot, unsigned int index );
	// Whether the ship is within a tile's diagonal (plus 0x68) of the slot (invented name).
	bool			IsShipNearSlot( AsteroidTileSlot *slot, ShipClass *ship );
	// Makes a tile, names it after the field and files it (invented name).
	AsteroidTileEntry *	CreateTile( void );
	// Lays the slots out on the tile grid around the centre (invented name).
	void			CreateSlots( void );
	// The first tile no slot holds that is not marked, or NULL (0x41A910; UpdateSlot
	// takes it before making a new one; invented name).
	AsteroidTileEntry *	FindUnplacedTile( void );
	void			DeleteSlots( void );	// invented name
	void			DeleteTiles( void );	// invented name

	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__ASTEROIDFIELD_H__ */
