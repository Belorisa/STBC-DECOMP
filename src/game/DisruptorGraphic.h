/*
===============================================================================

	DisruptorGraphic.h

	The shape that draws a disruptor bolt: an outer shell and an inner core,
	each a ring of segments around the bolt's axis. 0xE8 bytes.

	Evidence: the NiRTTI at 0x985F18 is built (0x4DD200) with the name
	"DisruptorGraphic" (0x8E0F58) and TGFuzzyTriShape's NiRTTI (0x980AE8)
	as its base; the vtable 0x88D138 returns it from GetRTTI (0x4DDDE0).
	Torpedo::CreateDisruptorModel allocates 0xE8 bytes and calls the
	constructor (0x4DD250, ret 0x1C) with the script's two colours (the
	scripts' kOuterShellColor and kOuterCoreColor), the segment count and
	four sizes. The constructor builds the vertices, colours and triangles,
	then adds the shape's properties. The vertex profile (four rings between
	a tip and a tail) is measured in 55ths of the length and 13ths of the
	radius. The vtable replaces only GetRTTI (0x4DDDE0) and the destructor
	(0x4DDB60). The parameter names are invented, after their use.

===============================================================================
*/

#ifndef __DISRUPTORGRAPHIC_H__
#define __DISRUPTORGRAPHIC_H__

#include "TGFuzzyTriShape.h"

class TGColorA;

/*
================
DisruptorGraphic

A disruptor bolt's shell and core.
================
*/
// FUNCTION: STBC 0x004ddb60 ??_GDisruptorGraphic@@UAEPAXI@Z
class DisruptorGraphic : public TGFuzzyTriShape {
public:
	// FUNCTION: STBC 0x004ddde0 ?GetRTTI@DisruptorGraphic@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual					~DisruptorGraphic();

	// The shell spans length along the bolt's axis and radius around it, in
	// the given number of segments; the core is the shell scaled by
	// coreRadiusScale around the axis and coreLengthScale along it.
							DisruptorGraphic( TGColorA *shellColor, TGColorA *coreColor, int segments, float length,
											  float radius, float coreRadiusScale, float coreLengthScale );

	static const NiRTTI		rtti;
};

#endif /* !__DISRUPTORGRAPHIC_H__ */
