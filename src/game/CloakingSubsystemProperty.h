/*
===============================================================================

	CloakingSubsystemProperty.h

	The members of CloakingSubsystemProperty that the SWIG wrappers CloakingSubsystemProperty_* call. Only what the
	wrappers prove is declared: no data members yet.

===============================================================================
*/

#ifndef __CLOAKINGSUBSYSTEMPROPERTY_H__
#define __CLOAKINGSUBSYSTEMPROPERTY_H__

#include "PoweredSubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type CloakingSubsystemProperty::Cast (0x680270) checks.
#define CLOAKINGSUBSYSTEMPROPERTY_TYPE		0x813A

// FUNCTION: STBC 0x00680490 ??_GCloakingSubsystemProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class CloakingSubsystemProperty : public PoweredSubsystemProperty {
public:
					CloakingSubsystemProperty( long id = 0 );
	virtual			~CloakingSubsystemProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00680310 ?GetObjType@CloakingSubsystemProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return CLOAKINGSUBSYSTEMPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00680360 ?GetClassName@CloakingSubsystemProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "CloakingSubsystemProperty"; }
	// FUNCTION: STBC 0x00680370 ?GetSwigTypeName@CloakingSubsystemProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_CloakingSubsystemProperty"; }
	// FUNCTION: STBC 0x00680380 ?GetPythonClassName@CloakingSubsystemProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "CloakingSubsystemPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static CloakingSubsystemProperty *	Cast( TGModelProperty * );
	static CloakingSubsystemProperty *	Create( char * );

	float			GetCloakStrength( void ) { return cloakStrength; }
	void			SetCloakStrength( float strength ) { cloakStrength = strength; }

protected:
	float			cloakStrength;			// 0x4C
};

/*
================
CloakingSubsystemProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00680320
inline bool CloakingSubsystemProperty::IsTypeOf( int type ) {
	if ( type == CLOAKINGSUBSYSTEMPROPERTY_TYPE ) {
		return true;
	}
	return PoweredSubsystemProperty::IsTypeOf( type );
}

#endif /* !__CLOAKINGSUBSYSTEMPROPERTY_H__ */
