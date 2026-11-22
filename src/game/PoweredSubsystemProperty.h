/*
===============================================================================

	PoweredSubsystemProperty.h

	The members of PoweredSubsystemProperty that the SWIG wrappers PoweredSubsystemProperty_* call.

	0x4C bytes (Create, 0x68A5B0, allocates that much from the pools). The
	wrappers GetNormalPowerPerSecond and SetNormalPowerPerSecond read and
	write 0x48 inline. The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __POWEREDSUBSYSTEMPROPERTY_H__
#define __POWEREDSUBSYSTEMPROPERTY_H__

#include "SubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type PoweredSubsystemProperty::Cast (0x68A580) checks.
#define POWEREDSUBSYSTEMPROPERTY_TYPE		0x812C

// FUNCTION: STBC 0x0068a7a0 ??_GPoweredSubsystemProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class PoweredSubsystemProperty : public SubsystemProperty {
public:
					PoweredSubsystemProperty( long id = 0 );
	virtual			~PoweredSubsystemProperty( void );

	// TGObject
	// FUNCTION: STBC 0x0068a620 ?GetObjType@PoweredSubsystemProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return POWEREDSUBSYSTEMPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0068a670 ?GetClassName@PoweredSubsystemProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "PoweredSubsystemProperty"; }
	// FUNCTION: STBC 0x0068a680 ?GetSwigTypeName@PoweredSubsystemProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_PoweredSubsystemProperty"; }
	// FUNCTION: STBC 0x0068a690 ?GetPythonClassName@PoweredSubsystemProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "PoweredSubsystemPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual bool	Copy( TGModelProperty *property );

	static PoweredSubsystemProperty *	Cast( TGModelProperty * );
	static PoweredSubsystemProperty *	Create( char * );

	float			GetNormalPowerPerSecond( void ) { return normalPowerPerSecond; }
	void			SetNormalPowerPerSecond( float power ) { normalPowerPerSecond = power; }

private:
	float			normalPowerPerSecond;	// 0x48
};

/*
================
PoweredSubsystemProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0068a630
inline bool PoweredSubsystemProperty::IsTypeOf( int type ) {
	if ( type == POWEREDSUBSYSTEMPROPERTY_TYPE ) {
		return true;
	}
	return SubsystemProperty::IsTypeOf( type );
}

#endif /* !__POWEREDSUBSYSTEMPROPERTY_H__ */
