/*
===============================================================================

	ImpulseEngineProperty.h

	0x60 bytes: the SWIG wrapper new_ImpulseEngineProperty allocates that much.

	Evidence: the SWIG wrappers ImpulseEngineProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __IMPULSEENGINEPROPERTY_H__
#define __IMPULSEENGINEPROPERTY_H__

#include "PoweredSubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type ImpulseEngineProperty::Cast (0x683BC0) checks.
#define IMPULSEENGINEPROPERTY_TYPE		0x813C

// FUNCTION: STBC 0x00683e00 ??_GImpulseEngineProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ImpulseEngineProperty : public PoweredSubsystemProperty {
public:
					ImpulseEngineProperty( long id = 0 );
	virtual			~ImpulseEngineProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00683c80 ?GetObjType@ImpulseEngineProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return IMPULSEENGINEPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00683cd0 ?GetClassName@ImpulseEngineProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ImpulseEngineProperty"; }
	// FUNCTION: STBC 0x00683ce0 ?GetSwigTypeName@ImpulseEngineProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ImpulseEngineProperty"; }
	// FUNCTION: STBC 0x00683cf0 ?GetPythonClassName@ImpulseEngineProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ImpulseEnginePropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual bool	Copy( TGModelProperty *property );

	static ImpulseEngineProperty *	Cast( TGModelProperty * );
	static ImpulseEngineProperty *	Create( char * );
	void	SetEngineSound( char * );

	// The named local is (invented): retail's callers copy the speed through
	// fld/fstp, which needs a second copy level here; an inner inline, a named
	// local or a value wrapper give the same code, so its spelling is unknown.
	float			GetMaxSpeed( void ) { float speed = maxSpeed; return speed; }
	void			SetMaxSpeed( float maxSpeed_ ) { maxSpeed = maxSpeed_; }
	float			GetMaxAccel( void ) { return maxAccel; }
	void			SetMaxAccel( float maxAccel_ ) { maxAccel = maxAccel_; }
	float			GetMaxAngularVelocity( void ) { return maxAngularVelocity; }
	void			SetMaxAngularVelocity( float maxAngularVelocity_ ) { maxAngularVelocity = maxAngularVelocity_; }
	float			GetMaxAngularAccel( void ) { return maxAngularAccel; }
	void			SetMaxAngularAccel( float maxAngularAccel_ ) { maxAngularAccel = maxAngularAccel_; }
	char *			GetEngineSound( void ) { return engineSound; }

protected:
	float			maxSpeed;				// 0x4C
	float			maxAccel;				// 0x50
	float			maxAngularVelocity;		// 0x54
	float			maxAngularAccel;		// 0x58
	char *			engineSound;			// 0x5C the sound's name (SetEngineSound sets it)
};

/*
================
ImpulseEngineProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00683c90
inline bool ImpulseEngineProperty::IsTypeOf( int type ) {
	if ( type == IMPULSEENGINEPROPERTY_TYPE ) {
		return true;
	}
	return PoweredSubsystemProperty::IsTypeOf( type );
}

#endif /* !__IMPULSEENGINEPROPERTY_H__ */
