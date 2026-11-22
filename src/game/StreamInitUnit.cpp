/*
===============================================================================

	StreamInitUnit.cpp

	A unit of which retail keeps only the static initialisers of the
	standard headers, between BezierCurve's code and ChatObjectClass's: their
	ios_base::Init and _Winit objects are the two bytes of .bss at
	0x9810B8-0x9810B9, after BezierCurve's last datum (0x9810B4) and before
	ChatObjectClass's own stream objects (0x981121). Whatever else the
	original file held compiled to nothing retail kept. The unit's name is
	invented.

===============================================================================
*/

#include <iostream>

// The locale id's guard (std::ctype<unsigned short>::id) and its exit-time
// registration.
// FUNCTION: STBC 0x00461120 _$E45
// FUNCTION: STBC 0x00461140 _$E44

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00461150 _$E32
// FUNCTION: STBC 0x00461160 _$E29
// FUNCTION: STBC 0x00461170 _$E31
// FUNCTION: STBC 0x00461180 _$E30
// FUNCTION: STBC 0x00461190 _$E38
// FUNCTION: STBC 0x004611a0 _$E35
// FUNCTION: STBC 0x004611b0 _$E37
// FUNCTION: STBC 0x004611c0 _$E36
