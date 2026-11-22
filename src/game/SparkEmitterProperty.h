/*
===============================================================================

	SparkEmitterProperty.h

	The members of SparkEmitterProperty that the SWIG wrappers SparkEmitterProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __SPARKEMITTERPROPERTY_H__
#define __SPARKEMITTERPROPERTY_H__

#include "EffectEmitterProperty.h"

class TGStream;

class TGModelProperty;

// The type SparkEmitterProperty::Cast (0x67F640) checks.
#define SPARKEMITTERPROPERTY_TYPE		0x8143

// FUNCTION: STBC 0x0067f860 ??_GSparkEmitterProperty@@UAEPAXI@Z
// FUNCTION: STBC 0x0067f890 ??1SparkEmitterProperty@@UAE@XZ
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class SparkEmitterProperty : public EffectEmitterProperty {
public:
					SparkEmitterProperty( long id = 0 );

	// TGObject
	// FUNCTION: STBC 0x0067f6e0 ?GetObjType@SparkEmitterProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return SPARKEMITTERPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0067f730 ?GetClassName@SparkEmitterProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "SparkEmitterProperty"; }
	// FUNCTION: STBC 0x0067f740 ?GetSwigTypeName@SparkEmitterProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_SparkEmitterProperty"; }
	// FUNCTION: STBC 0x0067f750 ?GetPythonClassName@SparkEmitterProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "SparkEmitterPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );

	static SparkEmitterProperty *	Cast( TGModelProperty * );
	static SparkEmitterProperty *	Create( char * );
};

/*
================
SparkEmitterProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0067f6f0
inline bool SparkEmitterProperty::IsTypeOf( int type ) {
	if ( type == SPARKEMITTERPROPERTY_TYPE ) {
		return true;
	}
	return EffectEmitterProperty::IsTypeOf( type );
}

#endif /* !__SPARKEMITTERPROPERTY_H__ */
