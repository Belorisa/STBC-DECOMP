/*
===============================================================================

	WeaponProperty.h

	The property a Weapon is built from: its ID and firing groups, the
	damage radius, and the icon the tactical display draws for it.

	0x68 bytes: the SWIG wrapper new_WeaponProperty allocates that much.

	Evidence: the SWIG wrappers WeaponProperty_* read and write dumbfire
	(0x48), the weapon ID (0x4C), the groups (0x50), the damage radius
	factor (0x54), the icon number (0x58), the icon position (0x5C, 0x60)
	and icon above ship (0x64) inline. The virtual functions are not
	declared yet.

===============================================================================
*/

#ifndef __WEAPONPROPERTY_H__
#define __WEAPONPROPERTY_H__

#include "SubsystemProperty.h"

class TGStream;

class TGModelProperty;

// The type WeaponProperty::Cast (0x69A070) checks.
#define WEAPONPROPERTY_TYPE		0x8130

// FUNCTION: STBC 0x0069a2b0 ??_GWeaponProperty@@UAEPAXI@Z
// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class WeaponProperty : public SubsystemProperty {
public:
					WeaponProperty( long id = 0 );
	virtual			~WeaponProperty( void );

	// TGObject
	// FUNCTION: STBC 0x0069a130 ?GetObjType@WeaponProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return WEAPONPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0069a180 ?GetClassName@WeaponProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "WeaponProperty"; }
	// FUNCTION: STBC 0x0069a190 ?GetSwigTypeName@WeaponProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_WeaponProperty"; }
	// FUNCTION: STBC 0x0069a1a0 ?GetPythonClassName@WeaponProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "WeaponPropertyPtr"; }

	// TGModelProperty
	virtual void	WriteScript( TGStream *stream );
	virtual void	WriteInit( TGStream *stream, char *variableName );
	virtual bool	Copy( TGModelProperty *property );

	static WeaponProperty *	Cast( TGModelProperty * );
	static WeaponProperty *	Create( char * );
	bool	IsMemberOfGroup( int );

	bool			IsDumbfire( void ) { return dumbfire; }
	void			SetDumbfire( bool dumbfire_ ) { dumbfire = dumbfire_; }
	int				GetWeaponID( void ) { return weaponID; }
	void			SetWeaponID( int id ) { weaponID = id; }
	int				GetGroups( void ) { return groups; }
	void			SetGroups( int groups_ ) { groups = groups_; }
	float			GetDamageRadiusFactor( void ) { return damageRadiusFactor; }
	void			SetDamageRadiusFactor( float factor ) { damageRadiusFactor = factor; }
	int				GetIconNum( void ) { return iconNum; }
	void			SetIconNum( int num ) { iconNum = num; }
	float			GetIconPositionX( void ) { return iconPositionX; }
	void			SetIconPositionX( float x ) { iconPositionX = x; }
	float			GetIconPositionY( void ) { return iconPositionY; }
	void			SetIconPositionY( float y ) { iconPositionY = y; }
	bool			IsIconAboveShip( void ) { return iconAboveShip; }
	void			SetIconAboveShip( bool above ) { iconAboveShip = above; }

protected:
	bool			dumbfire;				// 0x48
	int				weaponID;				// 0x4C
	int				groups;					// 0x50 the firing groups the weapon is a member of
	float			damageRadiusFactor;		// 0x54
	int				iconNum;				// 0x58
	float			iconPositionX;			// 0x5C
	float			iconPositionY;			// 0x60
	bool			iconAboveShip;			// 0x64
};

/*
================
WeaponProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x0069a140
inline bool WeaponProperty::IsTypeOf( int type ) {
	if ( type == WEAPONPROPERTY_TYPE ) {
		return true;
	}
	return SubsystemProperty::IsTypeOf( type );
}

#endif /* !__WEAPONPROPERTY_H__ */
