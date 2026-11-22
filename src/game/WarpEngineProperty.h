/*
===============================================================================

	WarpEngineProperty.h

	The members of WarpEngineProperty that the SWIG wrappers WarpEngineProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __WARPENGINEPROPERTY_H__
#define __WARPENGINEPROPERTY_H__

#include "PoweredSubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type WarpEngineProperty::Cast (0x6993E0) checks.
#define WARPENGINEPROPERTY_TYPE		0x813B

// FUNCTION: STBC 0x00699600 ??_GWarpEngineProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class WarpEngineProperty : public PoweredSubsystemProperty {
public:
					WarpEngineProperty( long id = 0 );
	virtual			~WarpEngineProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00699480 ?GetObjType@WarpEngineProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return WARPENGINEPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x006994d0 ?GetClassName@WarpEngineProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "WarpEngineProperty"; }
	// FUNCTION: STBC 0x006994e0 ?GetSwigTypeName@WarpEngineProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_WarpEngineProperty"; }
	// FUNCTION: STBC 0x006994f0 ?GetPythonClassName@WarpEngineProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "WarpEnginePropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static WarpEngineProperty *	Cast( TGModelProperty * );
	static WarpEngineProperty *	Create( char * );
};

/*
================
WarpEngineProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00699490
inline bool WarpEngineProperty::IsTypeOf( int type ) {
	if ( type == WARPENGINEPROPERTY_TYPE ) {
		return true;
	}
	return PoweredSubsystemProperty::IsTypeOf( type );
}

#endif /* !__WARPENGINEPROPERTY_H__ */
