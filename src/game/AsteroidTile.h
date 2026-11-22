/*
===============================================================================

	AsteroidTile.h

	One tile of an asteroid field, an object of its own. Only what the
	registration needs is declared: no data members and no virtual functions
	yet.

	Evidence: the class strings "AsteroidTile", "_p_AsteroidTile" and
	"AsteroidTilePtr" (getters 0x41C100-0x41C120) in the vtable the loader
	0x41BFD0 stores; UtopiaModule's creator for type 0x8015 (0x43EA70)
	allocates 0x84 bytes from the pool and calls that loader. The handler
	table at 0x97ECE8 (its getter 0x41C130, its initialisers 0x41BE00-0x41BE30)
	takes ObjectClass's as its parent in the registration.

===============================================================================
*/

#ifndef __ASTEROIDTILE_H__
#define __ASTEROIDTILE_H__

#include "ObjectClass.h"

// GetObjType's value for an AsteroidTile (its creator's type in the registration).
#define ASTEROIDTILE_TYPE		0x8015

class AsteroidField;
class SetClass;
class TGStream;
class TGStringEvent;
class TGPoint3;

// FUNCTION: STBC 0x0041c160 ??_GAsteroidTile@@UAEPAXI@Z @game/AsteroidTile
class AsteroidTile : public ObjectClass {
public:
					AsteroidTile( long id );	// the loader's: the stream fills the members
					AsteroidTile( int asteroidsPerTile_, float asteroidSizeFactor_, long id );
	virtual			~AsteroidTile( void );

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

	// BaseObjectClass
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// The set's tiles and their number, their number alone, the tile of the
	// ID or name in the set or anywhere (invented names, as the other
	// object classes').
	static AsteroidTile **	GetClassObjects( SetClass *set, int *count );
	static int		GetNumClassObjects( SetClass *set );
	static AsteroidTile *	GetObjectByID( SetClass *set, long id );
	static AsteroidTile *	GetObject( SetClass *set, char *name_ );

	// The tile number the asteroids' names count with, saved with the game
	// (invented names).
	static bool		WriteStaticData( TGStream *stream );
	static bool		ReadStaticData( TGStream *stream );

	// The asteroids come when the tile enters a set and go when it leaves
	// (the handlers' names are the handler strings').
	void			HandleEnterSet( TGEvent *event );
	void			HandleExitSet( TGStringEvent *event );

	AsteroidField *	GetField( void ) { return field; }		// invented name
	void			SetField( AsteroidField *field_ ) { field = field_; }		// invented name
	void			SetTileSize( float size ) { tileSize = size; }		// invented name
	void			SetRandomSeed( int seed ) { randomSeed = seed; }	// invented name

protected:
	// Whether an asteroid of the radius fits at the location: nothing of the
	// set is there and no placement of types 0x80DE or 0x80E0 is near
	// (invented name).
	bool			IsAsteroidPlaceFree( NiPoint3 *location, float radius );
	// A new asteroid of one of the four models, at a free random place of the
	// tile; loads the models the first time (invented name).
	ObjectClass *	CreateAsteroid( void );

	AsteroidField *	field;					// 0x68 the field the tile belongs to (invented name)
	float			tileSize;				// 0x6C the tile's width (invented name)
	float			asteroidSizeFactor;		// 0x70 scales the asteroids' random sizes (invented name)
	ObjectClass **	asteroids;				// 0x74 while the tile is in a set (invented name)
	int				numAsteroids;			// 0x78 (invented name)
	int				randomSeed;				// 0x7C the asteroids' places and turns come from it (invented name)
	int				asteroidsPerTile;		// 0x80 (invented name)

	static int		asteroidNumber;			// numbers the asteroids' names (invented name)
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__ASTEROIDTILE_H__ */
