/*
===============================================================================

	Backdrop.h

	The members of Backdrop that the SWIG wrappers Backdrop_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __BACKDROP_H__
#define __BACKDROP_H__

#include <stdio.h>
#include "ObjectClass.h"
#include "NiTArray.h"
#include "NiTStringMap.h"

class SetClass;
class TGObject;

// GetObjType's value for a Backdrop (the type its Cast checks).
#define BACKDROP_TYPE		0x800B

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0058b5a0 ??_GBackdrop@@UAEPAXI@Z @game/Backdrop
class Backdrop : public ObjectClass {
public:
					Backdrop( long id = 0 );
	virtual			~Backdrop( void );

	static Backdrop *	Create( void );
	static Backdrop *	Cast( TGObject * );
	void	SetModelName( char * );
	char *	GetModelName( void ) { return modelName; }
	static Backdrop *	GetObject( SetClass *, char * );
	static int	GetNumClassObjects( SetClass * );
	static Backdrop *	GetObjectByID( SetClass *set, long id );			// invented name
	static Backdrop **	GetClassObjects( SetClass *set, int *count );		// invented name

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
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// BaseObjectClass
	virtual void	SetContainingSet( SetClass *set );
	virtual void	SetName( char *name );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// Backdrop's own slots: SaveToFile calls them in turn with the file,
	// each writing part of the backdrop's script lines.
	virtual bool	VirtualSlot67( FILE *file );
	virtual bool	VirtualSlot68( FILE *file );
	virtual bool	VirtualSlot69( FILE *file );

	// The backdrop of the name, through the class's map of every backdrop
	// by name (0x58C2E0; invented name).
	static Backdrop *	GetByName( char *name );
	// Takes every backdrop out of its set and deletes it (invented name;
	// the game's end, Game::ReallyTerminate, calls it).
	static void	DeleteAll( void );

	// The backdrop's lines of a LoadBackdrops script: its slots 67 to 69 in
	// turn, while each succeeds (0x58BF10; invented name).
	bool	SaveToFile( FILE *file );
	// Writes a LoadBackdrops script of every backdrop, or of the set's, to
	// the file (0x58C1C0, 0x58C230; invented names).
	static bool	SaveAll( FILE *file );
	static bool	SaveSet( SetClass *set, FILE *file );

	// The background editor's "Move Frwd" and "Move Back": the backdrop
	// changes places with the one before or after it, in its set's scene
	// and in the drawing order (0x58BCF0, 0x58BDF0; invented names).
	void	MoveForward( void );
	void	MoveBack( void );

protected:
	// Takes the backdrop out of the by-name map and the array of every
	// backdrop (0x58B990; the destructor calls it; invented name).
	void	RemoveFromLists( void );
	// Sets the no-sorter flag of the alpha properties of the object (the
	// backdrop's own when it is NULL) and of the objects under it (0x58B7E0;
	// invented name).
	void	SetNoSorterOnTree( NiAVObject *object = NULL );
	// Takes every backdrop out of its set (invented name; 0x58C150).
	static void	RemoveAllFromSets( void );
	// Moves every backdrop into the set, out of any other set that holds it
	// (invented name; 0x58C100, which nothing calls).
	static void	MoveAllToSet( SetClass *set );

	// Every backdrop there is, in drawing order (invented name; 0x98D8D8,
	// built by the unit's static initializer 0x58B280 with NiTArray's
	// defaults): the constructor adds to the array, DeleteAll walks it.
	static NiTArray<Backdrop *>	backdrops;
	// Every backdrop by name (invented name; 0x98D868, 37 buckets, built by
	// the unit's static initializer 0x58B310).
	static NiTStringMap<Backdrop *>	backdropsByName;
	static TGEventHandlerTable	classHandlers;		// invented name; 0x98D7E8

	// Evidence: the constructor 0x58B380 clears the word at 0x68, after
	// ObjectClass's 0x68 bytes; the subclass constructor 0x58D2A0 starts at 0x6C.
	char *			modelName;			// 0x68
};

/*
================
Backdrop::IsTypeOf
================
*/
// FUNCTION: STBC 0x0058b500
inline bool Backdrop::IsTypeOf( int type ) {
	if ( type == BACKDROP_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

#endif /* !__BACKDROP_H__ */
