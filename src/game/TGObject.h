/*
===============================================================================

	TGObject.h

	The root of the game's object hierarchy: an object with a vtable and a
	game-wide ID. Every object is registered under its ID in a table of
	2039 buckets, so that IDs written to a saved game or sent over the
	network can be turned back into pointers (GetTGObjectPtr).

	8 bytes, allocated from the pools (TGPOOL_ALLOCATION): the scalar
	deleting destructor (0x6F0B70) frees through the pool of the object's
	size.

	Evidence: the constructor (0x6F0A70) stores the vtable 0x896278, takes
	the next free ID or the one given, creates the table on first use and
	registers the object; the destructor (0x6F0BA0) unregisters it. The
	SWIG wrappers TGObject_* name GetObjType, IsTypeOf, Print, Destroy,
	GetObjID, GetIndentString, SetIncrementID, IsIncrementID and
	GetTGObjectPtr; the other names are invented and say so. Slots 5-7 are
	pure (__purecall in the vtable).

===============================================================================
*/

#ifndef __TGOBJECT_H__
#define __TGOBJECT_H__

#include "Python.h"
#include "TGPoolManager.h"
#include "NiTMap.h"
#include "NiTList.h"

class TGStream;
class TGObject;

// What a class registers to be created by type while a game loads.
typedef TGObject *	( *TGObjectCreator )( long id );		// invented name

// A call made once a loaded game's pointers are fixed up
// (AddFixUpCallback); the arguments are the caller's.
struct TGFixUpCallback {									// invented name
	bool			( *function )( void *argument1, void *argument2 );
	void *			argument1;
	void *			argument2;
};

// What the loader records per type for CloseLogFile's report.
struct TGLoadSizes {										// invented name
	long			largestID;				// the biggest object of the type
	int				largestSize;			// its size in the stream
	int				totalSize;				// all the type's objects together
};

// Items per block of the lists the loader keeps.
#define LOAD_LIST_BLOCK			2

// GetObjType's value for a plain TGObject.
#define TGOBJECT_TYPE			2

// Buckets in the ID table.
#define OBJECT_TABLE_SIZE		2039

// GetIndentString: four spaces per level, at most this many levels.
#define INDENT_WIDTH			4
#define MAX_INDENT_LEVEL		64

// FUNCTION: STBC 0x006f0b70 ??_GTGObject@@UAEPAXI@Z
class TGObject {
public:
	TGPOOL_ALLOCATION

					TGObject( long id = 0 );
	virtual			~TGObject( void );

	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual void	Print( int indent = 0, int depth = -1 );
	virtual bool	WriteToStream( TGStream *stream );		// invented name
	virtual bool	ReadFromStream( TGStream *stream ) = 0;	// invented name
	virtual bool	FixUpPointers( void ) = 0;				// invented name
	virtual void	PostFixUp( void ) = 0;					// invented name
	virtual PyObject *	VirtualSlot08( void );		// a new Python shadow instance of the object
	virtual char *	GetClassName( void );					// invented name
	virtual char *	GetSwigTypeName( void );				// invented name
	virtual char *	GetPythonClassName( void );				// invented name

	void			Destroy( void ) { delete this; }
	long			GetObjID( void ) { return id; }
	void			SetObjID( long id );					// invented name

	void			FormatAddress( char *buffer );			// invented name
	void			GetSwigPointer( char *buffer );			// invented name

	static char *	GetIndentString( int indent );
	static TGObject *	GetTGObjectPtr( long id );
	// A pointer member saved as its object's ID, 0 for none, and the ID
	// read back into the object after the load (invented names). The
	// early returns are what the subsystems' stream functions inline.
	static long		GetSavedID( TGObject *object ) { if ( object == NULL ) return 0; return object->GetObjID(); }
	static TGObject *	GetSavedObject( long id ) { if ( id == 0 ) return NULL; return GetTGObjectPtr( id ); }
	static void		SetIncrementID( bool increment ) { incrementID = increment; }
	static bool		IsIncrementID( void ) { return incrementID; }

	static bool		AddObject( TGObject *object );			// invented name
	static bool		RemoveObject( TGObject *object );		// invented name
	static void		ReleaseObjectTable( void );				// invented name
	static void		RemoveAllObjects( void );				// invented name
	static int		GetNumObjects( void );					// invented name
	static void		ResetNextID( void );					// invented name
	static void		OpenLogFile( const char *fileName );	// invented name
	static void		CloseLogFile( void );					// invented name
	static void		Log( const char *text );				// invented name

	// Saved games: an object of the given type from its registered creator,
	// and an object read back from a stream with its ID moved by idOffset.
	static TGObject *	CreateObject( int type, long id );			// invented name
	// Files the creator of a type for CreateObject (invented name).
	static void		RegisterCreator( int type, TGObjectCreator creator ) { creators.SetAt( type, creator ); }
	static TGObject *	LoadObject( TGStream *stream, long idOffset );	// invented name
	static bool		FixUpObject( TGObject *object );				// invented name
	static bool		FixUpLoadedObjects( void );						// invented name
	static bool		AddFixUpCallback( bool ( *function )( void *, void * ), void *argument1, void *argument2 );	// invented name

	// Every live object, by ID.
	static NiTMap<long, TGObject *> *	objectTable;
	// The objects LoadObject read, waiting for the fix-up pass.
	static NiTList<TGObject *>	pendingObjects;					// invented name
	// The calls AddFixUpCallback queued for the end of the load.
	static NiTList<TGFixUpCallback *>	fixUpCallbacks;			// invented name
	// The creator of each type, by GetObjType's value.
	static NiTMap<int, TGObjectCreator>	creators;					// invented name

protected:
	long			id;						// 0x04

	static long		nextID;					// the ID the next object without one gets
	static bool		incrementID;			// an object created with an ID raises nextID past it
	static bool		keepObjectTable;		// cleared at shutdown: the table is deleted once empty
};

/*
================
TGObject::IsTypeOf

Derived classes inline it into their own IsTypeOf. The out-of-line copy is
folded by the linker with an identical body at 0x518AB0.
================
*/
// FUNCTION: STBC 0x00518ab0
inline bool TGObject::IsTypeOf( int type ) {
	if ( type == TGOBJECT_TYPE ) {
		return true;
	}
	return false;
}

#endif /* !__TGOBJECT_H__ */
