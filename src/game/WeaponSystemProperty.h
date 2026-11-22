/*
===============================================================================

	WeaponSystemProperty.h

	The property a WeaponSystem is built from: the kind of weapon system,
	whether it fires one weapon at a time, whether its weapons are aimed,
	and the string that describes its firing chains.

	Evidence: the SWIG wrappers WeaponSystemProperty_* read and write the
	weapon system type (0x4C), single fire (0x50) and aimed weapon (0x51)
	inline; GetFiringChainString copies the string at 0x54 (its out-of-line
	copy is 0x5B4EC0), and SetFiringChainString assigns it through TGString's
	operator=. WeaponSystem::IsSingleFire and ShouldBeAimed read 0x50
	and 0x51.

===============================================================================
*/

#ifndef __WEAPONSYSTEMPROPERTY_H__
#define __WEAPONSYSTEMPROPERTY_H__

#include "PoweredSubsystemProperty.h"
#include "TGString.h"

class TGStream;

class TGModelProperty;

// The type WeaponSystemProperty::Cast (0x69AF60) checks.
#define WEAPONSYSTEMPROPERTY_TYPE		0x812F

// FUNCTION: STBC 0x0069b1e0 ??_GWeaponSystemProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class WeaponSystemProperty : public PoweredSubsystemProperty {
public:
					WeaponSystemProperty( long id = 0 );
	virtual			~WeaponSystemProperty( void );

	// TGObject
	// FUNCTION: STBC 0x0069b060 ?GetObjType@WeaponSystemProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return WEAPONSYSTEMPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0069b0b0 ?GetClassName@WeaponSystemProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "WeaponSystemProperty"; }
	// FUNCTION: STBC 0x0069b0c0 ?GetSwigTypeName@WeaponSystemProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_WeaponSystemProperty"; }
	// FUNCTION: STBC 0x0069b0d0 ?GetPythonClassName@WeaponSystemProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "WeaponSystemPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	virtual bool	Copy( TGModelProperty *property );

	static WeaponSystemProperty *	Cast( TGModelProperty * );
	static WeaponSystemProperty *	Create( char * );

	// The kind of weapon system (the SWIG constants
	// WeaponSystemProperty.WST_*, in value order).
	enum weaponSystemType_t {
		WST_UNKNOWN,
		WST_PHASER,
		WST_TORPEDO,
		WST_PULSE,
		WST_TRACTOR
	};

	int				GetWeaponSystemType( void ) { return weaponSystemType; }
	void			SetWeaponSystemType( int type ) { weaponSystemType = type; }
	bool			IsSingleFire( void ) { return singleFire; }
	void			SetSingleFire( bool singleFire_ ) { singleFire = singleFire_; }
	bool			IsAimedWeapon( void ) { return aimedWeapon; }
	void			SetAimedWeapon( bool aimedWeapon_ ) { aimedWeapon = aimedWeapon_; }
	TGString		GetFiringChainString( void );
	void			SetFiringChainString( TGString *string ) { firingChainString = *string; }

protected:
	int				weaponSystemType;		// 0x4C a weaponSystemType_t
	bool			singleFire;				// 0x50
	bool			aimedWeapon;			// 0x51
	TGString		firingChainString;		// 0x54
};

/*
================
WeaponSystemProperty::GetFiringChainString
================
*/
// FUNCTION: STBC 0x005b4ec0
inline TGString WeaponSystemProperty::GetFiringChainString( void ) {
	return firingChainString;
}

/*
================
WeaponSystemProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0069b070
inline bool WeaponSystemProperty::IsTypeOf( int type ) {
	if ( type == WEAPONSYSTEMPROPERTY_TYPE ) {
		return true;
	}
	return PoweredSubsystemProperty::IsTypeOf( type );
}

#endif /* !__WEAPONSYSTEMPROPERTY_H__ */
