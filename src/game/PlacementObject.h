/*
===============================================================================

	PlacementObject.h

	The members of PlacementObject that the SWIG wrappers PlacementObject_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __PLACEMENTOBJECT_H__
#define __PLACEMENTOBJECT_H__

#include <stdio.h>
#include "ObjectClass.h"
#include "NiTStringMap.h"
#include "SortedArray.h"

class SetClass;
class TGEventHandlerTable;
class TGObject;

// GetObjType's value for a PlacementObject (the vtable slot 1 at 0x440D60).
#define PLACEMENTOBJECT_TYPE		0x80DE

// The SWIG inline accessors read GetModelName 0x68, IsStatic 0x6C and
// IsNavPoint 0x6D.
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00440e20 ??_GPlacementObject@@UAEPAXI@Z
class NiColorA;
class NiTriangles;
class PlacementObject;

typedef SortedArray<PlacementObject, long>	PlacementList;

class PlacementObject : public ObjectClass {
public:
					// FUNCTION: STBC 0x00440d40 ??0PlacementObject@@QAE@J@Z @game/UtopiaModule
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
					PlacementObject( long id ) : ObjectClass( id ) {}	// the loader's: the stream fills the members
	// The constructor (0x4A1280) stores the vtable 0x88A6E8 after
	// ObjectClass's constructor, sets the name, files the object under the
	// set's name and sets the model; the destructor (0x4A14D0) takes it out
	// again.
					PlacementObject( char *name, char *setName, char *modelName );
	static PlacementObject *	Create( char *name, char *setName, char *modelName ) { return new PlacementObject( name, setName, modelName ); }	// the SWIG wrapper PlacementObject_Create expands it
	virtual			~PlacementObject( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );		// 0x4A2F30
	virtual bool	ReadFromStream( TGStream *stream );		// 0x4A3000
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// ObjectClass
	virtual void	Update( float time );					// 0x4A1730
	virtual void	AddToSet( SetClass *set );				// 0x4A2610
	virtual void	RemoveFromSet( SetClass *set );			// 0x4A2640

	// Takes the placements out of their sets and deletes every one in the
	// class's map: the editor's "Delete Placements", and the game's end
	// (Game::ReallyTerminate; 0x4A2950, invented name).
	static void	DeleteAll( void );
	// Empties the set's list before the set is deleted (SetManager::DeleteSet,
	// 0x4A2A00; invented name).
	static void	DeleteSetPlacements( SetClass *set );
	// The editor's "Togl Plcmnts (p)": puts every placement into its set and
	// shows it, or takes them out again; nav points stay in their sets,
	// hidden (0x4A2790, 0x4A2890, 0x4A2930; invented names).
	static void	ShowPlacements( void );
	static void	HidePlacements( void );
	static void	TogglePlacements( void );
	static PlacementObject *	Cast( TGObject * );
	void	SetModel( char *name );
	// Builds the placement's arrow: six triangles coloured by its kind
	// (0x4A1A60; invented name).
	NiTriangles *	CreateModel( void );
	bool	SaveObject( FILE *, char * );
	void	SetStatic( bool );
	void	SetNavPoint( bool );
	SetClass *	FindContainingSet( void );
	static PlacementObject *	GetObject( SetClass *, char * );
	static PlacementObject *	GetObjectBySetName( char *, char * );
	// the placement with the ID in the set (invented name, after
	// ObjectClass::GetObjectByID)
	static PlacementObject *	GetObjectByID( SetClass *set, long id );
	// the set's placements and their number (invented name, as
	// Backdrop::GetClassObjects)
	static PlacementObject **	GetClassObjects( SetClass *set, int *count );
	// The saved game's statics: the lists and the placements in no set
	// (UtopiaModule::ProcessSave and ProcessInternalLoad; invented names).
	static bool	SaveStatics( TGStream *stream );
	static bool	LoadStatics( TGStream *stream );
	static bool	FixUpStatics( void );

	// PlacementObject's own virtual slots (vtable 0x88A6E8, 71 slots; the
	// next vtable, LightPlacement's, starts after slot 70). Slot 67 is the
	// SWIG wrapper PlacementObject_SaveObjectSecondPass; Waypoint's override
	// (0x4A46B0) writes the links between waypoints to the file.
	// FUNCTION: STBC 0x00440e10 ?SaveObjectSecondPass@PlacementObject@@UAE_NPAU_iobuf@@PAD@Z @game/UtopiaModule
	virtual bool	SaveObjectSecondPass( FILE *file, char *setName ) { return true; }
	virtual bool	VirtualSlot68( FILE *file, char *setName );		// writes the name and the model name (0x4A1890)
	virtual bool	VirtualSlot69( FILE *file );					// writes the static and nav point flags (0x4A18F0)
	virtual bool	VirtualSlot70( FILE *file );					// writes the rest (0x4A1A20)

	char *			GetModelName( void ) { return modelName; }
	bool			IsStatic( void ) { return isStatic; }
	bool			IsNavPoint( void ) { return isNavPoint; }

protected:
	// recolours the model's parts (invented names); ApplyColors writes a
	// part's colour to each of its vertices
	void			SetColors( const NiColorA *partColors );
	void			ApplyColors( NiColorA *colors, const NiColorA *partColors );

	char *			modelName;				// 0x68 SetModel's name
	bool			isStatic;				// 0x6C SetStatic
	bool			isNavPoint;				// 0x6D SetNavPoint

	static bool		placementsShown;		// ShowPlacements was last (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name

	// The placed objects of every set, by the set's name (invented name):
	// each list is sorted by object ID (GetObjectByID's binary search
	// compares through BaseObjectClass::CompareKey, 0x431030). A class
	// static: its exit-time destructor is guarded (0x4A10B0), as only a
	// class static's is.
	static NiTStringMap<PlacementList *>	placementsBySet;
};

/*
================
PlacementObject::GetObjType
================
*/
// FUNCTION: STBC 0x00440d60
inline int PlacementObject::GetObjType( void ) {
	return PLACEMENTOBJECT_TYPE;
}

/*
================
PlacementObject::IsTypeOf
================
*/
// FUNCTION: STBC 0x00440d70
inline bool PlacementObject::IsTypeOf( int type ) {
	if ( type == PLACEMENTOBJECT_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

/*
================
PlacementObject::GetClassName
================
*/
// FUNCTION: STBC 0x00440dd0
inline char *PlacementObject::GetClassName( void ) {
	return "PlacementObject";
}

/*
================
PlacementObject::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00440de0
inline char *PlacementObject::GetSwigTypeName( void ) {
	return "_p_PlacementObject";
}

/*
================
PlacementObject::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00440df0
inline char *PlacementObject::GetPythonClassName( void ) {
	return "PlacementObjectPtr";
}

/*
================
PlacementObject::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00440e00
inline TGEventHandlerTable *PlacementObject::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__PLACEMENTOBJECT_H__ */
