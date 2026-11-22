/*
===============================================================================

	SetObjectTable.h

	The objects of a set by class type: one list per GetObjType value,
	sorted by type, each holding the set's objects of that type sorted by
	ID. An object is in the list of its own type and in those of its base
	classes: AddToSet and RemoveFromSet of each class add the object under
	the class's type and call the base class's.

	The table remembers the last type it was asked for, so that walking the
	objects of one type does not search the type lists again.

	Evidence: SetClass's constructor (0x40D150) constructs the table at
	0x14 (0x40AF40: vtable 0x888944, items 0x4, count 0x8, capacity 0xC,
	the remembered type 0x10, -1 at first, and its list 0x14). The type
	list (0x40BF40: vtable 0x888948, the type at 0x10) is created by Add
	(0x40B240) on the first object of its type. The callers pass the
	object and the class type (BaseObjectClass::AddToSet 0x4306A0,
	ObjectClass::AddToSet 0x434D50). The names are invented.

===============================================================================
*/

#ifndef __SETOBJECTTABLE_H__
#define __SETOBJECTTABLE_H__

#include "SortedArray.h"
#include "BaseObjectClass.h"

class SetClass;

// The set's objects of one class type, sorted by ID. The objects belong to
// the set, not to the list.
// FUNCTION: STBC 0x0040BF70 ??_GSetObjectTypeList@@UAEPAXI@Z
class SetObjectTypeList {
public:
					SetObjectTypeList( void );
	virtual			~SetObjectTypeList( void );

	// Compare the list's type with a type, or with another list's.
	virtual int		CompareKey( int type_ );
	virtual int		Compare( SetObjectTypeList *other );

	SortedArray<BaseObjectClass, long>	objects;	// 0x04
	int				GetType( void ) { return type; }

	int				type;			// 0x10 GetObjType's value of the objects
};

// The type lists of a set, sorted by type. The table owns the lists.
// FUNCTION: STBC 0x0040AF60 ??_GSetObjectTable@@UAEPAXI@Z
class SetObjectTable {
public:
					SetObjectTable( void );
	virtual			~SetObjectTable( void );

	void			AddObject( BaseObjectClass *object, SetClass *set );
	void			RemoveObject( BaseObjectClass *object, SetClass *set );

	void			Add( BaseObjectClass *object, int type );
	void			Remove( BaseObjectClass *object, int type );

	BaseObjectClass **	GetObjects( int type, int *count_ );
	BaseObjectClass **	GetObjects( int type );
	int				GetNumObjects( int type );
	BaseObjectClass *	GetObject( int type, long id );

private:
	SortedArray<SetObjectTypeList, int>	lists;	// 0x04
	int				lastType;		// 0x10 the type last asked for; -1 for none
	SetObjectTypeList *	lastList;	// 0x14 its list
};

#endif /* !__SETOBJECTTABLE_H__ */
