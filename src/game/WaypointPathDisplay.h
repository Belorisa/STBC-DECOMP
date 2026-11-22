/*
===============================================================================

	WaypointPathDisplay.h

	The line drawn along a waypoint's path to the next waypoint while its
	set is shown: nine points of the waypoint's curve joined by an NiLines
	under the set's node, white, textured with data/Textures/White.tga and
	drawn with the depth buffer. No string names the class or its methods:
	every name is invented.

	0x18 bytes. Evidence: Waypoint::UpdateCurve (0x4A4910) allocates it
	and calls the constructor (0x58F760) with a node and the waypoint's
	curve; Waypoint::SetContainingSet (0x4A4380) and UpdateCurve call the
	destructor (0x58F7C0). The constructor stores the node at 0, copies the
	curve at 4 (BezierCurve's copy constructor) and builds the lines at
	0x14 (0x58F890). The class has a unit of its own between StarSphere's
	and DamageableObject's: a NiMain registration run (0x58EE10-0x58F750)
	comes before its code.

===============================================================================
*/

#ifndef __WAYPOINTPATHDISPLAY_H__
#define __WAYPOINTPATHDISPLAY_H__

#include "BezierCurve.h"
#include "NiPointer.h"

class NiNode;
class NiLines;

class WaypointPathDisplay {
public:
							WaypointPathDisplay( NiNode *parent_, const BezierCurve &curve_ );
							~WaypointPathDisplay();

	// replaces the lines with new ones along the curve, attached under
	// the parent (invented name)
	void					Build();

	// the copy of the curve, whose points Waypoint::UpdateCurve updates in
	// place before a Build (invented name)
	BezierCurve &			GetCurve() { return curve; }

	enum {
		NUM_POINTS = 9		// the points of the curve the lines join
	};

protected:
	NiNode *				parent;		// 0x00 the lines are attached under it
	BezierCurve				curve;		// 0x04 a copy of the waypoint's curve
	NiPointer<NiLines>		lines;		// 0x14
};

#endif /* !__WAYPOINTPATHDISPLAY_H__ */
