/*
===============================================================================

	NiStreamMacros.h

	The registration half of a streamable class's header: every unit that
	includes the header registers the class's loader from a static
	initializer.

	Evidence: retail carries, in every unit that includes NetImmerse headers,
	one `jmp; call NiXxx::Register; mov [unit static], eax; ret` pair per
	streamable class whose header the unit sees, in the order the headers
	complete (a base class before the classes that include its header), each
	storing into a static of its own unit (NiObject's in 528 units).
	Gamebryo 1.1.2's NiAnimationKeyMacros.h keeps the same form for the key
	classes ("Insert in header for class": `static int
	classname##registerloaderreturn = classname::RegisterLoader()`); the
	macro and static names here are invented after it.

===============================================================================
*/

#ifndef __NISTREAMMACROS_H__
#define __NISTREAMMACROS_H__

// Insert in the header after the class; NiImplementRegister (NiStream.h)
// defines the Register function in the class's unit.
#define NiRegisterStream( classname ) \
	static int classname##RegisterReturn = classname::Register()

#endif /* !__NISTREAMMACROS_H__ */
