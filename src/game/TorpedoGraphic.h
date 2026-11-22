/*
===============================================================================

	TorpedoGraphic.h

	The node that draws a torpedo in flight, and the controller that spins
	its core, pulses its glow and flickers its flares.

	The unit (from 0x4F4430 to 0x4F5930) holds, in order: the static
	initialisers of the NetImmerse headers it includes, TorpedoGraphic's
	NiRTTI (built at 0x4F45E0), the constructor (0x4F4630), the controller
	(0x4F5520-0x4F5780), and GetLight (0x4F58D0).

	TorpedoGraphic: 0xE8 bytes, NiNode's: no members of its own. Evidence:
	Torpedo::CreateTorpedoModel allocates 0xE8 bytes and calls the
	constructor (0x4F4630, fourteen arguments) with its core, glow and flare
	textures, colours and sizes; the constructor calls NiNode's, stores the
	vtable 0x88DC70 (NiNode's 44 slots with GetRTTI 0x4F5B90 and the
	destructor 0x4F54F0 replaced), whose GetRTTI returns the NiRTTI named
	"TorpedoGraphic" (0x986970, built at 0x4F45E0), and attaches a
	NiPointLight first. The destructor is the implicit one (0x4F5510 jumps
	to NiNode's). CreateTorpedoModel keeps GetLight's result (0x4F58D0).

	TorpedoController: 0x64 bytes. Evidence: the constructor (0x4F5520, ret
	0x28) calls NiTimeController's, stores the vtable 0x88DD20
	(NiTimeController's 20 slots with the destructor 0x4F5BB0, Update
	0x4F5780 and TargetIsRequiredType 0x4F5BA0 replaced; no NiRTTI of its
	own) and allocates one float per flare. Update turns the object at 0x38
	about y, scales the object at 0x4C by a sine between two scales, and
	fades the flares' quads in the shape at 0x60 (0x4F5600). The destructor
	(0x4F5BD0) stores the vtable, frees the flare times and releases the
	shape.

	The controller's name, and the member and function names, are
	invented, after their use.

===============================================================================
*/

#ifndef __TORPEDOGRAPHIC_H__
#define __TORPEDOGRAPHIC_H__

#include "NiNode.h"
#include "NiTimeController.h"
#include "NiTriShape.h"
#include "NiPointer.h"

class NiPointLight;
class TGColorA;

/*
================
TorpedoController

Spins the torpedo's core, pulses its glow and replays its flares.
================
*/
// FUNCTION: STBC 0x004f5bb0 ??_GTorpedoController@@UAEPAXI@Z
class TorpedoController : public NiTimeController {
public:
							TorpedoController( float rotationSpeed_, NiAVObject *spinObject_, float minScale_, float maxScale_,
											   float pulseRate_, NiAVObject *pulseObject_, float flareLife_, float flareLifeVariance_,
											   int numFlares_, NiTriShape *flareShape_ );
	virtual					~TorpedoController();

	virtual void			Update( float time );
	// FUNCTION: STBC 0x004f5ba0 ?TargetIsRequiredType@TorpedoController@@UBE_NXZ
	virtual bool			TargetIsRequiredType() const { return true; }

protected:
	// fades each flare's quad in and out over its life, restarting it
	// when the life is over
	void					UpdateFlares( float time );

	float					rotationSpeed;		// 0x30 radians a second about y
	float					angle;				// 0x34 radians
	NiAVObject *			spinObject;			// 0x38 turned about y
	float					minScale;			// 0x3C
	float					maxScale;			// 0x40
	float					pulsePhase;			// 0x44 radians
	float					pulseRate;			// 0x48 radians a second
	NiAVObject *			pulseObject;		// 0x4C scaled by the pulse
	float					flareLife;			// 0x50 seconds
	float					flareLifeVariance;	// 0x54 seconds
	int						numFlares;			// 0x58
	float *					flareTimes;			// 0x5C owned: when each flare started
	NiPointer<NiTriShape>	flareShape;			// 0x60 the flares' quads, four vertices each
};

/*
================
TorpedoGraphic

The torpedo's light, core, glow and flares.
================
*/
// GetRTTI (0x4F5B90), the scalar deleting destructor (0x4F54F0) and the
// implicit destructor (0x4F5510) are emitted with the vtable, by the
// constructor.
// FUNCTION: STBC 0x004f54f0 ??_GTorpedoGraphic@@UAEPAXI@Z
// FUNCTION: STBC 0x004f5510 ??1TorpedoGraphic@@UAE@XZ
class TorpedoGraphic : public NiNode {
public:
	// The arguments of Torpedo::CreateTorpedoModel, in its order: the core's
	// texture, colour, size and spin; the glow's texture, colour, pulse rate
	// and scales; the flares' texture, colour, count, length and life. The
	// names are invented, after the torpedo scripts' values and
	// TorpedoController's members.
							TorpedoGraphic( char *coreTexture, TGColorA *coreColor, float coreSize, float coreRotationSpeed,
											char *glowTexture, TGColorA *glowColor, float glowPulseRate, float glowMinScale,
											float glowMaxScale, char *flareTexture, TGColorA *flareColor, int numFlares,
											float flareLength, float flareLife );

	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// the light, the first child
	NiPointLight *			GetLight();

	static const NiRTTI		rtti;
};

#endif /* !__TORPEDOGRAPHIC_H__ */
