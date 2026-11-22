/*
===============================================================================

	UnknownClass6A8C50.h

	The values of PhysicsObjectClass's map at 0x68, one per player the
	object's state goes to: PhysicsObjectClass::VirtualSlot79 (0x5A22A0)
	creates the player's record on first use and keeps in its first member
	the game time it last let an update through. Nothing names the class or
	reads its other members.

	0x50 bytes, no vtable. Evidence: the constructor (0x6A8C50) builds a
	NiTMap of 0x25 buckets at 0x40 (vtable 0x895660) and clears the
	members from 0x00 to 0x3C except 0x08; the destructor (0x6A8CE0)
	deletes the map's values with operator delete (no destructor runs) and
	then destroys the map. The unit holds only the two and the map's COMDAT
	copies.

===============================================================================
*/

#ifndef __UNKNOWNCLASS6A8C50_H__
#define __UNKNOWNCLASS6A8C50_H__

#include "NiTMap.h"
#include "NiTList.h"

// The values of the map at 0x40, one per powered subsystem: the power
// percentage ShipClass::VirtualSlot72 last sent for it (1 byte; it creates
// them with 0xCC and compares the byte; the name is invented, deleted
// without a destructor).
struct UnknownClass6A8C50Item {
	unsigned char	unknown00;			// 0x00 the percentage, scaled to 0-255

					UnknownClass6A8C50Item( void ) : unknown00( 0xCC ) {}
};

class UnknownClass6A8C50 {
public:
					UnknownClass6A8C50( void );
					~UnknownClass6A8C50( void );

	float			unknown00;			// 0x00 game time of the last update let through to the player
	float			unknown04;			// 0x04 game time of the last update that sent every part (PhysicsObjectClass::VirtualSlot72)
	int				unknown08;			// 0x08 not set by the constructor
	float			unknown0C;			// 0x0C the speed last sent (compared by VirtualSlot72)
	float			unknown10;			// 0x10 the position last sent whole: x
	float			unknown14;			// 0x14 y
	float			unknown18;			// 0x18 z
	float			unknown1C;			// 0x1C the packed length of the offset from it last sent
	char			unknown20;			// 0x20 the packed direction of that offset
	char			unknown21;			// 0x21
	char			unknown22;			// 0x22
	float			unknown24;			// 0x24 game time the position was last sent whole
	char			unknown28;			// 0x28 the packed forward axis last sent
	char			unknown29;			// 0x29
	char			unknown2A;			// 0x2A
	char			unknown2B;			// 0x2B the packed up axis last sent
	char			unknown2C;			// 0x2C
	char			unknown2D;			// 0x2D
	unsigned char	unknown2E;			// 0x2E
	NiTListIterator	unknown30;			// 0x30 ShipClass::VirtualSlot72: the next of the ship's subsystems whose state it sends
	int				unknown34;			// 0x34 that subsystem's index in the ship's list
	NiTListIterator	unknown38;			// 0x38 the next subsystem whose power it sends
	int				unknown3C;			// 0x3C that subsystem's index
	NiTMap<unsigned int, UnknownClass6A8C50Item *>	unknown40;	// 0x40 owns its values; the key type is unproven
};

#endif /* !__UNKNOWNCLASS6A8C50_H__ */
