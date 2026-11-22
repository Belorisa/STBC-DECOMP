/*
===============================================================================

	WeaponSystemTarget.h

	A target a weapon system or a single weapon aims at: the target
	object's ID and the point aimed at (invented name).

	0x10 bytes. Evidence: WeaponSystem::ReadFromStream (0x585B80) allocates
	0x10 bytes per entry of its target list, stores 0 and NiPoint3::ZERO
	inline (the constructor), and reads each with 0x585F00; the write side
	(0x585EC0) writes the ID with WriteID and the point's coordinates.
	Weapon holds one at 0x8C and saves it with the same two functions.

===============================================================================
*/

#ifndef __WEAPONSYSTEMTARGET_H__
#define __WEAPONSYSTEMTARGET_H__

#include "NiPoint3.h"

class TGStream;

struct WeaponSystemTarget {
					WeaponSystemTarget( void ) : targetID( 0 ), offset( NiPoint3::ZERO ) {}

	bool			WriteToStream( TGStream *stream );		// invented name
	bool			ReadFromStream( TGStream *stream );		// invented name

	long			targetID;				// 0x00 the target object's ID
	NiPoint3		offset;					// 0x04 the point aimed at, in the target's frame
};

#endif /* !__WEAPONSYSTEMTARGET_H__ */
