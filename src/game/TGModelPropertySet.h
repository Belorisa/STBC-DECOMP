/*
===============================================================================

	TGModelPropertySet.h

	The properties of a model, in lists by name.

	0x0C bytes, allocated from the pools. Evidence: the inline constructor
	(an out-of-line copy at 0x6EB040; DamageableObject's constructors
	inline it) calls TGObject's, stores the vtable 0x893D58 and allocates
	the list at 0x08 (a list of 2-item blocks); the destructor is
	0x6EB0D0. GetObjType returns 0x602. The inline virtuals of slots 0-2 and
	9-11 are kept from DamageableObject's unit (0x5963A0-0x596400); slots
	4-7 are in the class's unit (0x6EB730-0x6EB940).

===============================================================================
*/

#ifndef __TGMODELPROPERTYSET_H__
#define __TGMODELPROPERTYSET_H__

#include <string.h>

#include "TGObject.h"
#include "NiTList.h"
#include "TGModelPropertyList.h"

class TGModelProperty;
class TGStream;

// GetObjType's value for a TGModelPropertySet.
#define TGMODELPROPERTYSET_TYPE		0x602

// A property list with a name: the set's lists (invented name). 0x24 bytes,
// allocated with operator new. Evidence: AddToSet (0x6EB220) and
// ReadFromStream (0x6EB780) build it inline: TGModelPropertyList's
// constructor, the vtable 0x8961A8 (one slot, the destructor 0x6EC570) and
// the name at 0x20, which they free and copy; GetPropertyList (0x6EB4F0)
// compares the name. The destructor (0x6EC590) frees the name.
// FUNCTION: STBC 0x006ec570 ??_GTGNamedModelPropertyList@@UAEPAXI@Z @game/TGModelPropertySet
class TGNamedModelPropertyList : public TGModelPropertyList {
public:
					TGNamedModelPropertyList( void ) { name = NULL; }
	// FUNCTION: STBC 0x006ec590 ??1TGNamedModelPropertyList@@UAE@XZ @game/TGModelPropertySet
	virtual			~TGNamedModelPropertyList( void ) { if ( name != NULL ) { delete[] name; } name = NULL; }

	char *			GetName( void ) { return name; }
	// Copies the name (invented name; AddToSet inlines it).
	void			SetName( char *name_ ) {
						if ( name != NULL ) { delete[] name; }
						name = NULL;
						name = new char[strlen( name_ ) + 1];
						strcpy( name, name_ );
					}

	// The name, then each instance after a true (invented names; the set's
	// stream functions call them).
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );

protected:
	char *			name;					// 0x20 owned
};

// FUNCTION: STBC 0x00596400 ??_GTGModelPropertySet@@UAEPAXI@Z @game/DamageableObject
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TGModelPropertySet : public TGObject {
public:
	// The SWIG wrapper new_TGModelPropertySet calls the out-of-line copy (it
	// returns with a plain ret, the loader's with ret 4).
	// FUNCTION: STBC 0x005dea10 ??0TGModelPropertySet@@QAE@XZ @game/AppWrap
					TGModelPropertySet( void ) { lists = new NiTList<TGNamedModelPropertyList *>( 2 ); }
	// Create calls the out-of-line copy.
	// FUNCTION: STBC 0x006eb040 ??0TGModelPropertySet@@QAE@J@Z @game/TGModelPropertySet
	// DRAFT: retail places the NULL-new branch last with its own epilogue; ours joins it with a jmp
					TGModelPropertySet( long id ) : TGObject( id ) { lists = new NiTList<TGNamedModelPropertyList *>( 2 ); }
	virtual			~TGModelPropertySet( void );

	// TGObject
	// FUNCTION: STBC 0x005963a0 ?GetObjType@TGModelPropertySet@@UAEHXZ
	virtual int		GetObjType( void ) { return TGMODELPROPERTYSET_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x005963d0 ?GetClassName@TGModelPropertySet@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGModelPropertySet"; }
	// FUNCTION: STBC 0x005963e0 ?GetSwigTypeName@TGModelPropertySet@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGModelPropertySet"; }
	// FUNCTION: STBC 0x005963f0 ?GetPythonClassName@TGModelPropertySet@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGModelPropertySetPtr"; }

	// The lists themselves (invented name); DamageableObject::SetupProperties
	// walks them. Out of line (0x6EB4E0), as retail calls it.
	NiTList<TGNamedModelPropertyList *> *	GetLists( void );
	TGModelPropertyList *	GetPropertyList( char * );
	TGModelPropertyList *	GetPropertiesByType( int );
	bool	AddToSet( char *, TGModelProperty * );

protected:
	NiTList<TGNamedModelPropertyList *> *	lists;		// 0x08 owned, with its lists (invented name)

	// Declared for the SWIG wrappers that call them.
public:
	static TGModelPropertySet *	Create( long id = 0 );	// retail 0x6EB560: the SWIG wrapper TGModelPropertySet_Create calls it with the default ID
};

/*
================
TGModelPropertySet::IsTypeOf
================
*/
// FUNCTION: STBC 0x005963b0 @game/DamageableObject
inline bool TGModelPropertySet::IsTypeOf( int type ) {
	if ( type == TGMODELPROPERTYSET_TYPE ) {
		return true;
	}
	return TGObject::IsTypeOf( type );
}

#endif /* !__TGMODELPROPERTYSET_H__ */
