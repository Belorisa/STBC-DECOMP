/*
===============================================================================

	ObjectGroupWithInfo.h

	An object group that keeps a Python object for each name: the class,
	and the Python methods that reach it (ObjectGroupWithInfo_*).

===============================================================================
*/

#ifndef __OBJECTGROUPWITHINFO_H__
#define __OBJECTGROUPWITHINFO_H__

#include "ObjectGroup.h"
#include "NiTStringMap.h"

class TGObject;
class TGStream;

struct _object;
typedef _object PyObject;

// GetObjType's value for an ObjectGroupWithInfo (the type its Cast checks).
#define OBJECTGROUPWITHINFO_TYPE		0x8012

// An object group that keeps a Python object for each of its names.
// Evidence: the vtable 0x889F48 (ObjectGroup's 23 slots: RemoveName,
// WriteToStream and ReadFromStream replaced); the loader's creator
// (UtopiaModule::CreateObjectGroupWithInfo, 0x43F0D0) allocates 0x5C bytes
// and calls the loading constructor (0x43F100), which builds a string map of 37
// buckets at 0x4C (vtables 0x889FB8, then 0x889FA4); the destructor
// (0x59E920) releases each value and empties the map; the methods hold a
// reference to each value (Py_INCREF on SetInfoForName, Py_DECREF on
// replacing or removing it). The bases are the ones the SWIG interface
// declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0043f280 ??_GObjectGroupWithInfo@@UAEPAXI@Z @game/UtopiaModule
class ObjectGroupWithInfo : public ObjectGroup {
public:
					ObjectGroupWithInfo( void );
					// FUNCTION: STBC 0x0043f100 ??0ObjectGroupWithInfo@@QAE@J@Z @game/UtopiaModule
					ObjectGroupWithInfo( long id ) : ObjectGroup( id ) {}	// the loader's: the stream sets the rest
	virtual			~ObjectGroupWithInfo( void );

	// TGObject
	// FUNCTION: STBC 0x0043f1e0 ?GetObjType@ObjectGroupWithInfo@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return OBJECTGROUPWITHINFO_TYPE; }
	// FUNCTION: STBC 0x0043f1f0 ?IsTypeOf@ObjectGroupWithInfo@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) {
		if ( type == OBJECTGROUPWITHINFO_TYPE ) {
			return true;
		}
		return ObjectGroup::IsTypeOf( type );
	}
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0043f240 ?GetClassName@ObjectGroupWithInfo@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "ObjectGroupWithInfo"; }
	// FUNCTION: STBC 0x0043f250 ?GetSwigTypeName@ObjectGroupWithInfo@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_ObjectGroupWithInfo"; }
	// FUNCTION: STBC 0x0043f260 ?GetPythonClassName@ObjectGroupWithInfo@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "ObjectGroupWithInfoPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0043f270 ?GetClassHandlers@ObjectGroupWithInfo@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ObjectGroup: the name's object is released with it
	virtual bool	RemoveName( char *name );

	static ObjectGroupWithInfo *	Cast( TGObject * );

	// Adds the name and keeps the object for it.
	void			AddNameAndInfo( char *name, PyObject *object );
	// Keeps the object for a name of the group, releasing the one it replaces.
	void			SetInfoForName( char *name, PyObject *object );
	// The object kept for the name, or NULL.
	PyObject *		GetInfo( char *name );

protected:
	NiTStringMap<PyObject *>	info;	// 0x4C the object of each name, one reference held (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

// The Python methods ObjectGroupWithInfo.AddNameAndInfo, ObjectGroupWithInfo.SetInfoForName, ObjectGroupWithInfo.GetInfo (UtopiaModule::VirtualSlot00 adds them to the class).
PyObject *		ObjectGroupWithInfo_AddNameAndInfo( PyObject *self, PyObject *args );
PyObject *		ObjectGroupWithInfo_SetInfoForName( PyObject *self, PyObject *args );
PyObject *		ObjectGroupWithInfo_GetInfo( PyObject *self, PyObject *args );

#endif /* !__OBJECTGROUPWITHINFO_H__ */
