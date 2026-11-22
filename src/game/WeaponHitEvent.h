/*
===============================================================================

	WeaponHitEvent.h

	The event sent to an object a weapon hits (ET_WEAPON_HIT): where it was
	hit, in the object's model space, and how hard.

	0x60 bytes. Evidence: the constructor (0x5B8750) runs TGEvent's, stores
	the vtable 0x8944E0 (TGEvent's 15 slots) and sets the event type
	0x800064; the deleting destructor (0x5B8840) frees 0x60 bytes to the
	pool, as the factory 0x595500 allocates. The class strings are
	"WeaponHitEvent", "_p_WeaponHitEvent" and "WeaponHitEventPtr";
	GetObjType returns 0x8127. The SWIG wrappers read GetObjectHitNormal at
	0x30, GetObjectHitPoint at 0x3C, GetWeaponInstanceID at 0x48, GetRadius
	at 0x4C, GetCondition at 0x50, GetDamage at 0x54, GetWeaponType at 0x2C,
	IsHullHit at 0x58 and Get/SetFiringPlayerID at 0x5C.

===============================================================================
*/

#ifndef __WEAPONHITEVENT_H__
#define __WEAPONHITEVENT_H__

#include "TGEvent.h"
#include "NiPoint3.h"

class ObjectClass;

// GetObjType's value for a WeaponHitEvent (the slot 1 at 0x5B87D0).
#define WEAPONHITEVENT_TYPE		0x8127

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005b8840 ??_GWeaponHitEvent@@UAEPAXI@Z
// FUNCTION: STBC 0x005b8870 ??1WeaponHitEvent@@UAE@XZ
class WeaponHitEvent : public TGEvent {
public:
	// The weapon types (the SWIG constants WeaponHitEvent_PHASER and so on).
	enum weaponType_t {
		PHASER,
		TORPEDO,
		TRACTOR_BEAM
	};

					WeaponHitEvent( long id = 0 );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	ObjectClass *	GetFiringObject( void );
	// Records the firing object by its ID; NULL clears it (invented name).
	void			SetFiringObject( ObjectClass *object );
	ObjectClass *	GetTargetObject( void );
	// The hit point and normal moved from the target's model space into
	// world space; unchanged when the target is not an object.
	NiPoint3		GetWorldHitPoint( void );
	NiPoint3		GetWorldHitNormal( void );

	int				GetWeaponType( void ) { return weaponType; }
	// FUNCTION: STBC 0x005af410 ?GetObjectHitNormal@WeaponHitEvent@@QAEABVNiPoint3@@XZ @game/ShipClass
	const NiPoint3 &	GetObjectHitNormal( void ) { return objectHitNormal; }
	float			GetCondition( void ) { return condition; }
	const NiPoint3 &	GetObjectHitPoint( void ) { return objectHitPoint; }
	float			GetRadius( void ) { return radius; }
	float			GetDamage( void ) { return damage; }
	bool			IsHullHit( void ) { return isHullHit; }
	long			GetWeaponInstanceID( void ) { return weaponInstanceID; }
	int				GetFiringPlayerID( void ) { return firingPlayerID; }
	void			SetObjectHitPoint( const NiPoint3 &point ) { objectHitPoint = point; }
	void			SetObjectHitNormal( const NiPoint3 &normal ) { objectHitNormal = normal; }
	void			SetWeaponInstanceID( long id ) { weaponInstanceID = id; }
	void			SetRadius( float radius_ ) { radius = radius_; }
	void			SetCondition( float condition_ ) { condition = condition_; }
	void			SetDamage( float damage_ ) { damage = damage_; }
	void			SetWeaponType( int type ) { weaponType = type; }
	void			SetHullHit( bool hullHit ) { isHullHit = hullHit; }
	void			SetFiringPlayerID( int id ) { firingPlayerID = id; }

protected:
	long			firingObjectID;		// 0x28 the firing object's ID; 0 for none (invented name)
	int				weaponType;			// 0x2C (GetWeaponType)
	NiPoint3		objectHitNormal;	// 0x30 in the target's model space (GetObjectHitNormal)
	NiPoint3		objectHitPoint;		// 0x3C in the target's model space (GetObjectHitPoint)
	long			weaponInstanceID;	// 0x48 (GetWeaponInstanceID)
	float			radius;				// 0x4C (GetRadius)
	float			condition;			// 0x50 (GetCondition)
	float			damage;				// 0x54 (GetDamage)
	bool			isHullHit;			// 0x58 (IsHullHit)
	int				firingPlayerID;		// 0x5C the firing player's network ID (GetFiringPlayerID)
};

#endif /* !__WEAPONHITEVENT_H__ */
