/*
===============================================================================

	EngineGlowProperty.h

	The members of EngineGlowProperty that the SWIG wrappers EngineGlowProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __ENGINEGLOWPROPERTY_H__
#define __ENGINEGLOWPROPERTY_H__

#include "TGModelProperty.h"

class TGStream;

// The type EngineGlowProperty::Cast (0x67D1A0) checks.
#define ENGINEGLOWPROPERTY_TYPE		0x8140

// FUNCTION: STBC 0x0067d3b0 ??_GEngineGlowProperty@@UAEPAXI@Z
// FUNCTION: STBC 0x0067d3e0 ??1EngineGlowProperty@@UAE@XZ
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class EngineGlowProperty : public TGModelProperty {
public:
					EngineGlowProperty( long id = 0 );

	// TGObject
	// FUNCTION: STBC 0x0067d240 ?GetObjType@EngineGlowProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return ENGINEGLOWPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x0067d280 ?GetClassName@EngineGlowProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "EngineGlowProperty"; }
	// FUNCTION: STBC 0x0067d290 ?GetSwigTypeName@EngineGlowProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_EngineGlowProperty"; }
	// FUNCTION: STBC 0x0067d2a0 ?GetPythonClassName@EngineGlowProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "EngineGlowPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	// FUNCTION: STBC 0x0067d3f0 ?VirtualSlot15@EngineGlowProperty@@UAEX_NPAVModelPropertyViewer@@PAVNiNode@@@Z
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {}

	static EngineGlowProperty *	Cast( TGModelProperty * );
	static EngineGlowProperty *	Create( char * );
};

/*
================
EngineGlowProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0067d250
inline bool EngineGlowProperty::IsTypeOf( int type ) {
	if ( type == ENGINEGLOWPROPERTY_TYPE ) {
		return true;
	}
	return TGModelProperty::IsTypeOf( type );
}

#endif /* !__ENGINEGLOWPROPERTY_H__ */
