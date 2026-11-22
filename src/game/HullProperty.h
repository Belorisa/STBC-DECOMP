/*
===============================================================================

	HullProperty.h

	The members of HullProperty that the SWIG wrappers HullProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __HULLPROPERTY_H__
#define __HULLPROPERTY_H__

#include "SubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type HullProperty::Cast (0x682F30) checks.
#define HULLPROPERTY_TYPE		0x8138

// FUNCTION: STBC 0x00683150 ??_GHullProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class HullProperty : public SubsystemProperty {
public:
					HullProperty( long id = 0 );
	virtual			~HullProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00682fd0 ?GetObjType@HullProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return HULLPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00683020 ?GetClassName@HullProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "HullProperty"; }
	// FUNCTION: STBC 0x00683030 ?GetSwigTypeName@HullProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_HullProperty"; }
	// FUNCTION: STBC 0x00683040 ?GetPythonClassName@HullProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "HullPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static HullProperty *	Cast( TGModelProperty * );
	static HullProperty *	Create( char * );
};

/*
================
HullProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00682fe0
inline bool HullProperty::IsTypeOf( int type ) {
	if ( type == HULLPROPERTY_TYPE ) {
		return true;
	}
	return SubsystemProperty::IsTypeOf( type );
}

#endif /* !__HULLPROPERTY_H__ */
