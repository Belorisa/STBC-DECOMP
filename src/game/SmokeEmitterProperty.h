/*
===============================================================================

	SmokeEmitterProperty.h

	The members of SmokeEmitterProperty that the SWIG wrappers SmokeEmitterProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __SMOKEEMITTERPROPERTY_H__
#define __SMOKEEMITTERPROPERTY_H__

#include "EffectEmitterProperty.h"

class TGStream;

class TGModelProperty;

// The type SmokeEmitterProperty::Cast (0x67EA10) checks.
#define SMOKEEMITTERPROPERTY_TYPE		0x8142

// FUNCTION: STBC 0x0067ec30 ??_GSmokeEmitterProperty@@UAEPAXI@Z
// FUNCTION: STBC 0x0067ec60 ??1SmokeEmitterProperty@@UAE@XZ
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class SmokeEmitterProperty : public EffectEmitterProperty {
public:
					SmokeEmitterProperty( long id = 0 );

	// TGObject
	// FUNCTION: STBC 0x0067eab0 ?GetObjType@SmokeEmitterProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return SMOKEEMITTERPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0067eb00 ?GetClassName@SmokeEmitterProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "SmokeEmitterProperty"; }
	// FUNCTION: STBC 0x0067eb10 ?GetSwigTypeName@SmokeEmitterProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_SmokeEmitterProperty"; }
	// FUNCTION: STBC 0x0067eb20 ?GetPythonClassName@SmokeEmitterProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "SmokeEmitterPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );

	static SmokeEmitterProperty *	Cast( TGModelProperty * );
	static SmokeEmitterProperty *	Create( char * );
};

/*
================
SmokeEmitterProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0067eac0
inline bool SmokeEmitterProperty::IsTypeOf( int type ) {
	if ( type == SMOKEEMITTERPROPERTY_TYPE ) {
		return true;
	}
	return EffectEmitterProperty::IsTypeOf( type );
}

#endif /* !__SMOKEEMITTERPROPERTY_H__ */
