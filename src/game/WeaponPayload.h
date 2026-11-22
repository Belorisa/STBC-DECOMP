/*
===============================================================================

	WeaponPayload.h

	What a weapon delivers: a torpedo, or the beam of a phaser bank or a
	tractor beam projector. Each payload takes a new ID, so that the hits of
	one shot can be told apart.

	0x10 bytes. Evidence: the constructor (0x5B95A0) stores the vtable
	0x893234 (three slots), takes the next ID from the counter at 0x98EED4
	and sets the payload type to WP_DEFAULT and the damage radius factor to
	1. The SWIG wrappers WeaponPayload_* read the ID (0x4), the payload
	type (0x8) and the damage radius factor (0xC) inline, set the factor,
	and give the payload the next ID (NextID). Torpedo derives from it
	(App.pyc); PhaserBank and TractorBeamProjector hold one.

===============================================================================
*/

#ifndef __WEAPONPAYLOAD_H__
#define __WEAPONPAYLOAD_H__

class TGStream;

class WeaponPayload {
public:
	// The SWIG constants WeaponPayload_WP_*.
	enum payloadType_t {
		WP_DEFAULT,
		WP_PHASER,
		WP_TORPEDO,
		WP_PULSE,
		WP_TRACTOR
	};

					WeaponPayload( void );

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual			~WeaponPayload( void );

	long			GetID( void ) { return id; }
	void			NextID( void ) { id = GetNextID(); }
	// Takes the next payload ID (invented name).
	static long		GetNextID( void ) { return nextID++; }
	int				GetPayloadType( void ) { return payloadType; }
	float			GetDamageRadiusFactor( void ) { return damageRadiusFactor; }
	void			SetDamageRadiusFactor( float damageRadiusFactor_ ) { damageRadiusFactor = damageRadiusFactor_; }

protected:
	long			id;						// 0x04 from nextID
	int				payloadType;			// 0x08 a payloadType_t
	float			damageRadiusFactor;		// 0x0C 1 at first

	static long		nextID;					// the ID the next payload takes (invented name)
};

/*
================
WeaponPayload::~WeaponPayload
================
*/
// FUNCTION: STBC 0x005731c0 ??_GWeaponPayload@@UAEPAXI@Z
inline WeaponPayload::~WeaponPayload( void ) {
}

#endif /* !__WEAPONPAYLOAD_H__ */
