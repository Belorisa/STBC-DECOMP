/*
===============================================================================

	ExplodeEmitterProperty.h

	The members of ExplodeEmitterProperty that the SWIG wrappers ExplodeEmitterProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __EXPLODEEMITTERPROPERTY_H__
#define __EXPLODEEMITTERPROPERTY_H__

#include "EffectEmitterProperty.h"

class TGStream;

class TGModelProperty;

// The type ExplodeEmitterProperty::Cast (0x67DDE0) checks.
#define EXPLODEEMITTERPROPERTY_TYPE		0x8144

// FUNCTION: STBC 0x0067e000 ??_GExplodeEmitterProperty@@UAEPAXI@Z
// FUNCTION: STBC 0x0067e030 ??1ExplodeEmitterProperty@@UAE@XZ
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ExplodeEmitterProperty : public EffectEmitterProperty {
public:
					ExplodeEmitterProperty( long id = 0 );

	// TGObject
	// FUNCTION: STBC 0x0067de80 ?GetObjType@ExplodeEmitterProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return EXPLODEEMITTERPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0067ded0 ?GetClassName@ExplodeEmitterProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ExplodeEmitterProperty"; }
	// FUNCTION: STBC 0x0067dee0 ?GetSwigTypeName@ExplodeEmitterProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ExplodeEmitterProperty"; }
	// FUNCTION: STBC 0x0067def0 ?GetPythonClassName@ExplodeEmitterProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ExplodeEmitterPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );

	static ExplodeEmitterProperty *	Cast( TGModelProperty * );
	static ExplodeEmitterProperty *	Create( char * );
};

/*
================
ExplodeEmitterProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0067de90
inline bool ExplodeEmitterProperty::IsTypeOf( int type ) {
	if ( type == EXPLODEEMITTERPROPERTY_TYPE ) {
		return true;
	}
	return EffectEmitterProperty::IsTypeOf( type );
}

#endif /* !__EXPLODEEMITTERPROPERTY_H__ */
