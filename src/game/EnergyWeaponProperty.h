/*
===============================================================================

	EnergyWeaponProperty.h

	The property of a weapon that fires from a charge: how the charge
	builds and drains, the damage, the firing sound and the charge
	indicator's icon.

	0x90 bytes: the SWIG wrapper new_EnergyWeaponProperty allocates that
	much.

	Evidence: the SWIG wrappers EnergyWeaponProperty_* read and write the
	max charge (0x68), the recharge rate (0x6C), the normal discharge rate
	(0x70), the min firing charge (0x74), the max damage (0x78) and its
	distance (0x7C), the indicator icon number (0x84) and position (0x88,
	0x8C) inline, and read the fire sound's name (0x80). The virtual
	functions are not declared yet.

===============================================================================
*/

#ifndef __ENERGYWEAPONPROPERTY_H__
#define __ENERGYWEAPONPROPERTY_H__

#include "WeaponProperty.h"

class TGStream;

class TGModelProperty;

// The type EnergyWeaponProperty::Cast (0x681150) checks.
#define ENERGYWEAPONPROPERTY_TYPE		0x8131

// FUNCTION: STBC 0x006813c0 ??_GEnergyWeaponProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class EnergyWeaponProperty : public WeaponProperty {
public:
					EnergyWeaponProperty( long id = 0 );
	virtual			~EnergyWeaponProperty( void );

	// TGObject
	// FUNCTION: STBC 0x00681240 ?GetObjType@EnergyWeaponProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return ENERGYWEAPONPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00681290 ?GetClassName@EnergyWeaponProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "EnergyWeaponProperty"; }
	// FUNCTION: STBC 0x006812a0 ?GetSwigTypeName@EnergyWeaponProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_EnergyWeaponProperty"; }
	// FUNCTION: STBC 0x006812b0 ?GetPythonClassName@EnergyWeaponProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "EnergyWeaponPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual bool	Copy( TGModelProperty *property );

	static EnergyWeaponProperty *	Cast( TGModelProperty * );
	static EnergyWeaponProperty *	Create( char * );
	void	SetFireSound( char * );

	float			GetRechargeRate( void ) { return rechargeRate; }
	float			GetNormalDischargeRate( void ) { return normalDischargeRate; }
	float			GetMaxCharge( void ) { return maxCharge; }
	float			GetMinFiringCharge( void ) { return minFiringCharge; }
	char *			GetFireSound( void ) { return fireSound; }
	float			GetMaxDamage( void ) { return maxDamage; }
	float			GetMaxDamageDistance( void ) { return maxDamageDistance; }
	int				GetIndicatorIconNum( void ) { return indicatorIconNum; }
	float			GetIndicatorIconPositionX( void ) { return indicatorIconPositionX; }
	float			GetIndicatorIconPositionY( void ) { return indicatorIconPositionY; }
	void			SetMaxDamage( float damage ) { maxDamage = damage; }
	void			SetMaxDamageDistance( float distance ) { maxDamageDistance = distance; }
	void			SetMinFiringCharge( float charge ) { minFiringCharge = charge; }
	void			SetMaxCharge( float charge ) { maxCharge = charge; }
	void			SetNormalDischargeRate( float rate ) { normalDischargeRate = rate; }
	void			SetRechargeRate( float rate ) { rechargeRate = rate; }
	void			SetIndicatorIconNum( int num ) { indicatorIconNum = num; }
	void			SetIndicatorIconPositionX( float x ) { indicatorIconPositionX = x; }
	void			SetIndicatorIconPositionY( float y ) { indicatorIconPositionY = y; }

protected:
	float			maxCharge;				// 0x68
	float			rechargeRate;			// 0x6C
	float			normalDischargeRate;	// 0x70
	float			minFiringCharge;		// 0x74
	float			maxDamage;				// 0x78
	float			maxDamageDistance;		// 0x7C
	char *			fireSound;				// 0x80 the sound's name (SetFireSound sets it)
	int				indicatorIconNum;		// 0x84
	float			indicatorIconPositionX;	// 0x88
	float			indicatorIconPositionY;	// 0x8C
};

/*
================
EnergyWeaponProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x00681250
inline bool EnergyWeaponProperty::IsTypeOf( int type ) {
	if ( type == ENERGYWEAPONPROPERTY_TYPE ) {
		return true;
	}
	return WeaponProperty::IsTypeOf( type );
}

#endif /* !__ENERGYWEAPONPROPERTY_H__ */
