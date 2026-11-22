/*
===============================================================================

	AsteroidFieldPlacement.h

	The members of AsteroidFieldPlacement that the SWIG wrappers AsteroidFieldPlacement_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __ASTEROIDFIELDPLACEMENT_H__
#define __ASTEROIDFIELDPLACEMENT_H__

#include "PlacementObject.h"
#include <stdio.h>

class AsteroidField;
class SetClass;
class TGObject;

// The SWIG inline accessors read GetFieldRadius 0x70, GetNumTilesPerAxis
// 0x74, GetNumAsteroidsPerTile 0x78 and GetAsteroidSizeFactor 0x7C; the
// setters (0x494760-0x4949E0) write them.
// GetObjType's value for an AsteroidFieldPlacement (the type its Cast checks).
#define ASTEROIDFIELDPLACEMENT_TYPE		0x80DF

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004415c0 ??_GAsteroidFieldPlacement@@UAEPAXI@Z
class NiLines;

class AsteroidFieldPlacement : public PlacementObject {
public:
	// FUNCTION: STBC 0x004414f0 ??0AsteroidFieldPlacement@@QAE@J@Z @game/UtopiaModule
					AsteroidFieldPlacement( long id ) : PlacementObject( id ) {}	// the loader's: the stream fills the members
	// The constructor (0x493CF0) stores the vtable 0x88AE50 after
	// PlacementObject's constructor, sets the field's defaults (radius 1000,
	// 3 tiles per axis, 2 asteroids per tile, size factor 10) and gives the
	// node its model.
					AsteroidFieldPlacement( char *name, char *setName, char *modelName );
	static AsteroidFieldPlacement *	Create( char *name, char *setName, char *modelName ) { return new AsteroidFieldPlacement( name, setName, modelName ); }	// the SWIG wrapper AsteroidFieldPlacement_Create expands it

	virtual			~AsteroidFieldPlacement( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// PlacementObject's virtuals that the class overrides
	virtual void	Update( float time );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	VirtualSlot68( FILE *file, char *setName );
	virtual bool	VirtualSlot69( FILE *file );
	virtual bool	VirtualSlot70( FILE *file );

	void	SetFieldRadius( float );
	void	SetNumTilesPerAxis( int );
	void	SetNumAsteroidsPerTile( int );
	void	SetAsteroidSizeFactor( float );
	static AsteroidFieldPlacement *	Cast( TGObject * );
	AsteroidField *	GetField( void );
	void	ConfigField( void );
	static AsteroidFieldPlacement *	GetObject( SetClass *, char * );
	static AsteroidFieldPlacement *	GetObjectBySetName( char *, char * );
	// The placement with the ID in the set (name invented, after its family's
	// GetObjectByID); nothing in retail calls it.
	static AsteroidFieldPlacement *	GetObjectByID( SetClass *set, long id );

	float			GetFieldRadius( void ) { return fieldRadius; }
	int				GetNumTilesPerAxis( void ) { return numTilesPerAxis; }
	int				GetNumAsteroidsPerTile( void ) { return numAsteroidsPerTile; }
	float			GetAsteroidSizeFactor( void ) { return asteroidSizeFactor; }

protected:
	// The second child of the node: the rings that show the field's extent
	// (0x494700; invented name).
	NiAVObject *	GetFieldModel( void );
	// Makes the node of those rings for the radius: 40 circles across the
	// sphere along each axis (0x493DD0; invented name).
	NiNode *		CreateFieldModel( void );
	// A circle of the radius in the XY plane, as 40 lines (0x493FD0; the
	// name is a placeholder).
	NiLines *		Unknown493FD0( float radius );

	float			fieldRadius;			// 0x70
	int				numTilesPerAxis;		// 0x74
	int				numAsteroidsPerTile;	// 0x78
	float			asteroidSizeFactor;		// 0x7C
	static TGEventHandlerTable	classHandlers;		// invented name

	long			fieldID;				// 0x80 the field object's ID, which GetField looks up in the set (invented name, as LightPlacement's lightID)
};

/*
================
AsteroidFieldPlacement::GetObjType
================
*/
// FUNCTION: STBC 0x00441510
inline int AsteroidFieldPlacement::GetObjType( void ) {
	return ASTEROIDFIELDPLACEMENT_TYPE;
}

/*
================
AsteroidFieldPlacement::IsTypeOf
================
*/
// FUNCTION: STBC 0x00441520
inline bool AsteroidFieldPlacement::IsTypeOf( int type ) {
	if ( type == ASTEROIDFIELDPLACEMENT_TYPE ) {
		return true;
	}
	return PlacementObject::IsTypeOf( type );
}

/*
================
AsteroidFieldPlacement::GetClassName
================
*/
// FUNCTION: STBC 0x00441580
inline char *AsteroidFieldPlacement::GetClassName( void ) {
	return "AsteroidFieldPlacement";
}

/*
================
AsteroidFieldPlacement::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00441590
inline char *AsteroidFieldPlacement::GetSwigTypeName( void ) {
	return "_p_AsteroidFieldPlacement";
}

/*
================
AsteroidFieldPlacement::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004415a0
inline char *AsteroidFieldPlacement::GetPythonClassName( void ) {
	return "AsteroidFieldPlacementPtr";
}

/*
================
AsteroidFieldPlacement::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004415b0
inline TGEventHandlerTable *AsteroidFieldPlacement::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__ASTEROIDFIELDPLACEMENT_H__ */
