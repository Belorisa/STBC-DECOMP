/*
===============================================================================

	StaticsUnit4B3510.cpp

	A unit retail keeps with no code of its own: the locale-id pair and its
	<iostream> quads, at 0x4B3510-0x4B35BF, their statics the two bytes at
	.bss 0x983B38 (after DamageMesh's, before MetaSurfacePolygonizer's). The
	unit's name is invented: nothing in retail names it. It registers no
	NetImmerse class and holds no other .bss or .rdata, so it includes no
	NetImmerse header, and none of MetaSurfacePolygonizer's code (which
	follows at 0x4B35C0) can be its: that code's statics, vtable and
	template copies all sit in MetaSurfacePolygonizer's blocks.

===============================================================================
*/

#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004b3510 _$E45
// FUNCTION: STBC 0x004b3530 _$E44

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004b3540 _$E32
// FUNCTION: STBC 0x004b3550 _$E29
// FUNCTION: STBC 0x004b3560 _$E31
// FUNCTION: STBC 0x004b3570 _$E30
// FUNCTION: STBC 0x004b3580 _$E38
// FUNCTION: STBC 0x004b3590 _$E35
// FUNCTION: STBC 0x004b35a0 _$E37
// FUNCTION: STBC 0x004b35b0 _$E36
