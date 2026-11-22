/*
===============================================================================

	TGAttrObject.h

	An object with named numeric attributes: floats keyed by an AttrType,
	kept in a TGAttrTable created on the first SetAttrValue. The Int
	accessors store an int's bits in the float.

	0x0C bytes. Evidence: the constructor (0x6F31A0) calls TGObject's and
	stores the vtable 0x8962F4 (19 slots); GetObjType returns 3. The
	attribute method names are those of the TGAttrObject_* wrappers, which
	call slots 12-18.

===============================================================================
*/

#ifndef __TGATTROBJECT_H__
#define __TGATTROBJECT_H__

#include "TGObject.h"
#include "TGAttrTable.h"

// GetObjType's value for a TGAttrObject.
#define TGATTROBJECT_TYPE		3

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006f3240 ??_GTGAttrObject@@UAEPAXI@Z @game/TGAttrObject
class TGAttrObject : public TGObject {
public:
					TGAttrObject( long id = 0 );
	virtual			~TGAttrObject( void );

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

	virtual bool	LookupAttrValue( AttrType type, float *value );
	virtual float	GetAttrValue( AttrType type );
	virtual bool	SetAttrValue( AttrType type, float value );
	virtual bool	RemoveAttr( AttrType type );
	virtual bool	LookupAttrInt( AttrType type, int *value );
	virtual int		GetAttrInt( AttrType type );
	virtual bool	SetAttrInt( AttrType type, int value );

protected:
	TGAttrTable *	attributes;				// 0x08 NULL until the first SetAttrValue or ReadFromStream
};

/*
================
TGAttrObject::IsTypeOf

TGTemplatedAttrObject inlines it into its own.
================
*/
// FUNCTION: STBC 0x006f31d0
inline bool TGAttrObject::IsTypeOf( int type ) {
	if ( type == TGATTROBJECT_TYPE ) {
		return true;
	}
	return TGObject::IsTypeOf( type );
}

#endif /* !__TGATTROBJECT_H__ */
