/*
===============================================================================

	EffectEmitterProperty.h

	The members of EffectEmitterProperty that the SWIG wrappers EffectEmitterProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __EFFECTEMITTERPROPERTY_H__
#define __EFFECTEMITTERPROPERTY_H__

#include "PositionOrientationProperty.h"

class TGStream;

class TGModelProperty;

// The type EffectEmitterProperty::Cast (0x67C580) checks.
#define EFFECTEMITTERPROPERTY_TYPE		0x8141

// FUNCTION: STBC 0x0067c7a0 ??_GEffectEmitterProperty@@UAEPAXI@Z
// The implicit destructor: retail kept the copy in BlinkingLightProperty's unit range, which this
// build does not emit (BlinkingLightProperty's destructor inlines it); EffectEmitterProperty's copy is identical.
// FUNCTION: STBC 0x0067b810 ??1EffectEmitterProperty@@UAE@XZ @game/EffectEmitterProperty
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class EffectEmitterProperty : public PositionOrientationProperty {
public:
					EffectEmitterProperty( long id = 0 );

	// TGObject
	// FUNCTION: STBC 0x0067c620 ?GetObjType@EffectEmitterProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return EFFECTEMITTERPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0067c670 ?GetClassName@EffectEmitterProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "EffectEmitterProperty"; }
	// FUNCTION: STBC 0x0067c680 ?GetSwigTypeName@EffectEmitterProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_EffectEmitterProperty"; }
	// FUNCTION: STBC 0x0067c690 ?GetPythonClassName@EffectEmitterProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "EffectEmitterPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );

	static EffectEmitterProperty *	Cast( TGModelProperty * );
	static EffectEmitterProperty *	Create( char * );
};

/*
================
EffectEmitterProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0067c630
inline bool EffectEmitterProperty::IsTypeOf( int type ) {
	if ( type == EFFECTEMITTERPROPERTY_TYPE ) {
		return true;
	}
	return PositionOrientationProperty::IsTypeOf( type );
}

#endif /* !__EFFECTEMITTERPROPERTY_H__ */
