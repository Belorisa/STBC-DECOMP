/*
===============================================================================

	TGConfigUnit.cpp

	A unit of which retail keeps the static initialisers of <iostream> and
	the NetImmerse map templates of the configuration classes, between
	TGConfigMapping's code and TGModule's: its ios_base::Init and _Winit
	objects are the two bytes of .bss at 0x99C870-0x99C871, after
	TGConfigMapping's last datum (0x99C860) and before TGModule's own stream
	objects (0x99C884). The NiTMap and NiTStringMap copies of TGConfigValue
	and TGConfigSection that follow the quads (0x71DD60-0x71E2AF) are its
	too by their place, but they stay annotated to TGConfigMapping, whose
	object compiles the same bytes, until the unit's code is known. The
	unit's name is invented.

===============================================================================
*/

#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0071dce0 _$E32
// FUNCTION: STBC 0x0071dcf0 _$E29
// FUNCTION: STBC 0x0071dd00 _$E31
// FUNCTION: STBC 0x0071dd10 _$E30
// FUNCTION: STBC 0x0071dd20 _$E38
// FUNCTION: STBC 0x0071dd30 _$E35
// FUNCTION: STBC 0x0071dd40 _$E37
// FUNCTION: STBC 0x0071dd50 _$E36
