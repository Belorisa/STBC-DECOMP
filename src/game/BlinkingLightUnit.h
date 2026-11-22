/*
===============================================================================

	BlinkingLightUnit.h

	The builders of the camera-facing light sprites ShipClass hangs on its
	model for each BlinkingLightProperty (ShipClass::Unknown5B5280 calls
	Unknown4DB1D0). Their unit, at 0x4DA130-0x4DB260 between
	AnimatedTriShapeParticles and BridgeEffectAction, has no name in retail;
	the unit's and the functions' names are invented.

===============================================================================
*/

#ifndef __BLINKINGLIGHTUNIT_H__
#define __BLINKINGLIGHTUNIT_H__

class NiNode;
class NiPoint3;
class NiColor;

// A billboard holding a unit quad at position, scaled by radius, textured,
// lit by color, its alpha animated by numKeys linear keys (times, values)
// at frequency.
NiNode *	Unknown4DABA0( const NiPoint3 &position, const NiColor &color, float radius, float frequency, unsigned int numKeys, float *times, float *values, char *textureName );

// The blinking sprite: off, a fade in over a tenth of the lit time, lit, a
// fade out over the last tenth, at period seconds a cycle.
NiNode *	Unknown4DB1D0( const NiPoint3 &position, const NiColor &color, float radius, float period, float duration, char *textureName );

#endif /* !__BLINKINGLIGHTUNIT_H__ */
