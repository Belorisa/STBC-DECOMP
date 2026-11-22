/*
===============================================================================

	NiRTLib.h

	The C++ runtime headers NetImmerse's units share. The name is invented for
	this binary, after Gamebryo 1.2.2's NiRTLib.h (reached there through
	NiSystem.h and the precompiled header), which plays the same part.

	No surviving NetImmerse or Gamebryo header includes <iostream>, yet every
	NetImmerse unit of the retail image carries its static initialisers: the
	ios_base::Init and _Winit objects (two init/exit pairs, each writing its own
	pair of .bss bytes) and the locale-id guard. The shared runtime header the
	original included first is the one place that explains all of them.

===============================================================================
*/

#ifndef __NIRTLIB_H__
#define __NIRTLIB_H__

#include <iostream>

#endif /* !__NIRTLIB_H__ */
