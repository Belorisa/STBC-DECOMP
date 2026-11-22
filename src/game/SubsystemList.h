/*
===============================================================================

	SubsystemList.h

	The SWIG interface's name for a ship's list of subsystems: scripts walk
	ShipClass's subsystem list through it with NiTList's own members.

	Evidence: the SWIG wrappers SubsystemList_GetHeadPos, GetTailPos,
	GetSize, GetHead, GetTail, GetNext and GetPrev expand NiTList's inline
	members on it (count at 0x00, head at 0x04, tail at 0x08); the wrapper
	SubsystemList_Cast calls the static Cast out of line (0x5BC180, in the SWIG
	unit's code), which returns its argument. The class adds no members.

===============================================================================
*/

#ifndef __SUBSYSTEMLIST_H__
#define __SUBSYSTEMLIST_H__

#include "NiTList.h"

class ShipSubsystem;

class SubsystemList : public NiTList<ShipSubsystem *> {
public:
	// Returns the list as a SubsystemList (0x5BC180, among the SWIG
	// unit's own code).
	static SubsystemList *	Cast( NiTList<ShipSubsystem *> *list );
};

#endif /* !__SUBSYSTEMLIST_H__ */
