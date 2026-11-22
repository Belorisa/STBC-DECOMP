/*
===============================================================================

	TGTemplatedAttrObject.h

	An attribute object with a template: an attribute it does not set
	itself is looked up in the template object. Streams store the template
	by ID; ReadFromStream keeps the ID in the pointer until the fix-up pass
	turns it back into the object.

	0x10 bytes. Evidence: the constructor (0x6F2590) calls TGAttrObject's
	and stores the vtable 0x8962A8 (19 slots); GetObjType returns 4. The
	SWIG wrapper TGTemplatedAttrObject_LookupLocalAttrValue names the
	lookup that skips the template.

===============================================================================
*/

#ifndef __TGTEMPLATEDATTROBJECT_H__
#define __TGTEMPLATEDATTROBJECT_H__

#include "TGAttrObject.h"

// GetObjType's value for a TGTemplatedAttrObject.
#define TGTEMPLATEDATTROBJECT_TYPE	4

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006f2620 ??_GTGTemplatedAttrObject@@UAEPAXI@Z
class TGTemplatedAttrObject : public TGAttrObject {
public:
					TGTemplatedAttrObject( long id = 0 );
	virtual			~TGTemplatedAttrObject( void );

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

	// TGAttrObject
	virtual bool	LookupAttrValue( AttrType type, float *value );
	virtual float	GetAttrValue( AttrType type );
	virtual bool	LookupAttrInt( AttrType type, int *value );
	virtual int		GetAttrInt( AttrType type );
	virtual bool	SetAttrInt( AttrType type, int value );

	bool			LookupLocalAttrValue( AttrType type, float *value );

protected:
	TGAttrObject *	templateObject;			// 0x0C the object's ID between ReadFromStream and PostFixUp
};

/*
================
TGTemplatedAttrObject::IsTypeOf

TGEventHandlerObject inlines it into its own.
================
*/
// FUNCTION: STBC 0x006f25c0
inline bool TGTemplatedAttrObject::IsTypeOf( int type ) {
	if ( type == TGTEMPLATEDATTROBJECT_TYPE ) {
		return true;
	}
	return TGAttrObject::IsTypeOf( type );
}

#endif /* !__TGTEMPLATEDATTROBJECT_H__ */
