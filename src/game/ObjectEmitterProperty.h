/*
===============================================================================

	ObjectEmitterProperty.h

	The members of ObjectEmitterProperty that the SWIG wrappers ObjectEmitterProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __OBJECTEMITTERPROPERTY_H__
#define __OBJECTEMITTERPROPERTY_H__

#include "PositionOrientationProperty.h"

class TGStream;

class TGModelProperty;

// The type ObjectEmitterProperty::Cast (0x684A80) checks.
#define OBJECTEMITTERPROPERTY_TYPE		0x8146

// FUNCTION: STBC 0x00684ca0 ??_GObjectEmitterProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ObjectEmitterProperty : public PositionOrientationProperty {
public:
					ObjectEmitterProperty( long id = 0 );
	virtual			~ObjectEmitterProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00684b20 ?GetObjType@ObjectEmitterProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return OBJECTEMITTERPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00684b70 ?GetClassName@ObjectEmitterProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ObjectEmitterProperty"; }
	// FUNCTION: STBC 0x00684b80 ?GetSwigTypeName@ObjectEmitterProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ObjectEmitterProperty"; }
	// FUNCTION: STBC 0x00684b90 ?GetPythonClassName@ObjectEmitterProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ObjectEmitterPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual bool	Copy( TGModelProperty *property );

	static ObjectEmitterProperty *	Cast( TGModelProperty * );
	static ObjectEmitterProperty *	Create( char * );

	// What the emitter launches (the SWIG constants
	// ObjectEmitterProperty.OEP_*, in value order).
	enum emittedObjectType_t {
		OEP_UNKNOWN,
		OEP_SHUTTLE,
		OEP_PROBE,
		OEP_DECOY
	};

	int				GetEmittedObjectType( void ) { return emittedObjectType; }
	void			SetEmittedObjectType( int type ) { emittedObjectType = type; }

protected:
	int				emittedObjectType;	// 0x50 an emittedObjectType_t
};

/*
================
ObjectEmitterProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00684b30
inline bool ObjectEmitterProperty::IsTypeOf( int type ) {
	if ( type == OBJECTEMITTERPROPERTY_TYPE ) {
		return true;
	}
	return PositionOrientationProperty::IsTypeOf( type );
}

#endif /* !__OBJECTEMITTERPROPERTY_H__ */
