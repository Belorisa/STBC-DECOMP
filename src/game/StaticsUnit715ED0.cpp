/*
===============================================================================

	StaticsUnit715ED0.cpp

	A unit retail keeps with no code of its own: the <iostream> quads and
	the locale-id pair, at 0x715ED0-0x715F7F, their statics the two bytes at
	.bss 0x99BF18 (after TGMusic's data, before TGSystemModule's). It sits
	between TGMusicFadeEvent's code (to 0x715ECF), whose own quads are at
	0x715190, and TGSystemModule's object quad (0x715F80), whose own quads
	are at 0x7160A0. The unit's name is invented: nothing in retail names
	it. It registers no NetImmerse class, so it includes no NetImmerse
	header.

===============================================================================
*/

#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00715ed0 _$E32
// FUNCTION: STBC 0x00715ee0 _$E29
// FUNCTION: STBC 0x00715ef0 _$E31
// FUNCTION: STBC 0x00715f00 _$E30
// FUNCTION: STBC 0x00715f10 _$E38
// FUNCTION: STBC 0x00715f20 _$E35
// FUNCTION: STBC 0x00715f30 _$E37
// FUNCTION: STBC 0x00715f40 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00715f50 _$E45
// FUNCTION: STBC 0x00715f70 _$E44
