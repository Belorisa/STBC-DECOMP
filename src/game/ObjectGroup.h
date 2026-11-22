/*
===============================================================================

	ObjectGroup.h

	A named group of objects: the names it is given, and the objects of
	those names that exist now. The members of ObjectGroup that the SWIG
	wrappers ObjectGroup_* call, the constructor and the layout.

	0x4C bytes, allocated from the pools. Evidence: the constructor
	(0x59C7B0) calls TGEventHandlerObject's, builds the arrays at 0x14
	(vtable 0x889F44) and 0x2C (vtable 0x889F40), each NiTArray( 1, 1 ),
	stores the vtable 0x889EE4 (23 slots), sets 0x44 to -1 and clears the
	event flags (0x48), and adds two broadcast handlers. Game's
	constructor (0x405C10) allocates 0x4C bytes for it. The SWIG wrappers
	name the new slots 21 (RemoveName) and 22 (RemoveAllNames) and the
	inline accessors IsEventFlagSet 0x48 and GetNumActiveObjects 0x38 (the
	size of the second array); SetEventFlag (0x59C900) sets bits of the
	byte at 0x48.

===============================================================================
*/

#ifndef __OBJECTGROUP_H__
#define __OBJECTGROUP_H__

#include "NiTArray.h"
#include "TGEventHandlerObject.h"

class TGEventHandlerTable;
class TGStream;

class ObjectClass;
class SetClass;
class TGEvent;
struct _object;
typedef _object PyObject;

// GetObjType's value for an ObjectGroup.
#define OBJECTGROUP_TYPE			0x8011

// An event telling the group's present objects changed (the App constant).
#define ET_OBJECT_GROUP_CHANGED		0x800010
// An object of the group entered the set (the App constant).
#define ET_OBJECT_GROUP_OBJECT_ENTERED_SET	0x800011
// An object of the group left a set (the App constant).
#define ET_OBJECT_GROUP_OBJECT_EXITED_SET	0x800012
// An object of the group was destroyed (the App constant).
#define ET_OBJECT_GROUP_OBJECT_DESTROYED	0x800013

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// UtopiaModule's loader creator (0x43EF40) expands the loading constructor, so
// UtopiaModule's unit keeps the inline virtuals below and the deleting destructor.
// FUNCTION: STBC 0x0043f0a0 ??_GObjectGroup@@UAEPAXI@Z @game/UtopiaModule
class ObjectGroup : public TGEventHandlerObject {
public:
					ObjectGroup( void );
	// FUNCTION: STBC 0x0043ef70 ??0ObjectGroup@@QAE@J@Z @game/UtopiaModule
					ObjectGroup( long id ) : TGEventHandlerObject( id ) {}	// the loader's: the stream sets the rest
	virtual			~ObjectGroup( void );

	// TGObject
	// FUNCTION: STBC 0x0043f010 ?GetObjType@ObjectGroup@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return OBJECTGROUP_TYPE; }
	// FUNCTION: STBC 0x0043f020 ?IsTypeOf@ObjectGroup@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) {
		if ( type == OBJECTGROUP_TYPE ) {
			return true;
		}
		return TGEventHandlerObject::IsTypeOf( type );
	}
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0043f060 ?GetClassName@ObjectGroup@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "ObjectGroup"; }
	// FUNCTION: STBC 0x0043f070 ?GetSwigTypeName@ObjectGroup@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_ObjectGroup"; }
	// FUNCTION: STBC 0x0043f080 ?GetPythonClassName@ObjectGroup@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "ObjectGroupPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0043f090 ?GetClassHandlers@ObjectGroup@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	virtual bool	RemoveName( char *name );
	virtual bool	RemoveAllNames( void );

	// Register the class's handler methods by name, and add them to its table.
	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// Handlers: an object entered a set, left one, was destroyed, or is
	// being deleted.
	void			EnteredSet( TGEvent *event );
	void			ExitedSet( TGEvent *event );
	void			ObjectDestroyed( TGEvent *event );
	void			ObjectDeleted( TGEvent *event );

	void	SetEventFlag( int );
	void	ClearEventFlag( int );
	void	AddName( char * );
	bool	IsNameInGroup( char * );
	static ObjectGroup *	FromModule( char *, char * );
	// The saved game's groups (UtopiaModule::ProcessSave and
	// ProcessInternalLoad; invented names).
	static bool		SaveGroups( TGStream *stream );
	static bool		LoadGroups( TGStream *stream );
	static void		DeleteAllGroups( void );

	// Adds the object, when it is not in the group yet (name invented; 0x59D4B0).
	void			AddActiveObject( ObjectClass *object );
	// Posts the event that tells the group's count of present objects has
	// changed, once per change stamp (name invented; 0x59D440).
	void			PostCountChanged( void );

	// New tuples of the names, of the objects present and of those present
	// in the set (the Python methods below return them).
	PyObject *		GetNameTuple( void );
	PyObject *		GetActiveObjectTuple( void );
	PyObject *		GetActiveObjectTupleInSet( SetClass *set );

	char *			GetNthName( int index ) { return names.GetAt( index ); }	// invented name
	int				GetNumNames( void ) { return names.GetSize(); }			// invented name
	int				GetNumActiveObjects( void ) { return activeObjects.GetSize(); }
	bool			IsEventFlagSet( int flag ) { return ( eventFlags & flag ) != 0; }

protected:
	NiTArray<char *>	names;			// 0x14 the names given (element type inferred from AddName)
	NiTArray<ObjectClass *>	activeObjects;	// 0x2C the objects present (GetNumActiveObjects; AddActiveObject; name invented)
	long			unknown44;			// 0x44 -1 after construction
	unsigned char	eventFlags;			// 0x48 SetEventFlag's bits

	static TGEventHandlerTable	classHandlers;		// invented name
};

// The Python methods ObjectGroup.GetNameTuple, GetActiveObjectTuple and
// GetActiveObjectTupleInSet (added to App's shadow class by the objects module).
PyObject *		ObjectGroup_GetNameTuple( PyObject *self, PyObject *args );
PyObject *		ObjectGroup_GetActiveObjectTuple( PyObject *self, PyObject *args );
PyObject *		ObjectGroup_GetActiveObjectTupleInSet( PyObject *self, PyObject *args );

#endif /* !__OBJECTGROUP_H__ */
