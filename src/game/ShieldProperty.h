/*
===============================================================================

	ShieldProperty.h

	0x90 bytes: the SWIG wrapper new_ShieldProperty allocates that much.

	Evidence: the SWIG wrappers ShieldProperty_* read and write the members inline
	(offsets at the members). The virtual functions are not declared yet.

===============================================================================
*/

#ifndef __SHIELDPROPERTY_H__
#define __SHIELDPROPERTY_H__

#include "PoweredSubsystemProperty.h"
#include "TGColorA.h"

class TGStream;

class TGModelProperty;

// The type ShieldProperty::Cast (0x6900F0) checks.
#define SHIELDPROPERTY_TYPE		0x8137

// FUNCTION: STBC 0x00690360 ??_GShieldProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ShieldProperty : public PoweredSubsystemProperty {
public:
					ShieldProperty( long id = 0 );
	virtual			~ShieldProperty( void );

	// TGObject
	// FUNCTION: STBC 0x006901e0 ?GetObjType@ShieldProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return SHIELDPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00690230 ?GetClassName@ShieldProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ShieldProperty"; }
	// FUNCTION: STBC 0x00690240 ?GetSwigTypeName@ShieldProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ShieldProperty"; }
	// FUNCTION: STBC 0x00690250 ?GetPythonClassName@ShieldProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ShieldPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual bool	Copy( TGModelProperty *property );

	static ShieldProperty *	Cast( TGModelProperty * );
	static ShieldProperty *	Create( char * );

	float			GetMaxShields( int shield ) { return maxShields[shield]; }
	void			SetMaxShields( int shield, float maxShields_ ) { maxShields[shield] = maxShields_; }
	float			GetShieldChargePerSecond( int shield ) { return shieldChargePerSecond[shield]; }
	void			SetShieldChargePerSecond( int shield, float chargePerSecond ) { shieldChargePerSecond[shield] = chargePerSecond; }
	// The copies kept out of line are the SWIG wrappers' (Copy calls them).
	// FUNCTION: STBC 0x005af3d0 ?GetShieldGlowColor@ShieldProperty@@QAE?AVTGColorA@@XZ
	TGColorA		GetShieldGlowColor( void ) { return shieldGlowColor; }
	// FUNCTION: STBC 0x00643030 ?SetShieldGlowColor@ShieldProperty@@QAEXVTGColorA@@@Z
	void			SetShieldGlowColor( TGColorA color ) { shieldGlowColor = color; }
	float			GetShieldGlowDecay( void ) { return shieldGlowDecay; }
	void			SetShieldGlowDecay( float shieldGlowDecay_ ) { shieldGlowDecay = shieldGlowDecay_; }

protected:
	TGColorA		shieldGlowColor;		// 0x4C
	float			shieldGlowDecay;		// 0x5C
	float			maxShields[6];			// 0x60 per facing (ShieldClass::NUM_SHIELDS)
	float			shieldChargePerSecond[6];	// 0x78 per facing
};

/*
================
ShieldProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x006901f0
inline bool ShieldProperty::IsTypeOf( int type ) {
	if ( type == SHIELDPROPERTY_TYPE ) {
		return true;
	}
	return PoweredSubsystemProperty::IsTypeOf( type );
}

#endif /* !__SHIELDPROPERTY_H__ */
