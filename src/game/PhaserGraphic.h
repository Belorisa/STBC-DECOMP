/*
===============================================================================

	PhaserGraphic.h

	The node that draws a phaser beam, and the two pieces it is built
	from: the beam's tube and the glow at either end.

	The unit (0x4E3C40-0x4E7B10) holds, in order: the static initialisers
	of the NetImmerse headers it includes, PhaserGraphic's NiRTTI (built at
	0x4E4050), the beam (0x4E40A0-0x4E6050), the glow (0x4E60C0-0x4E6A80),
	PhaserGraphic (0x4E6AB0 on), PhaserProperty's orientation getters and
	NiColorA's copy constructor kept as COMDAT copies.

	PhaserBeam: 0x138 bytes. Evidence: PhaserGraphic's constructor
	allocates 0x138 bytes and calls the constructor (0x4E40A0, ret 0x3C)
	with the property's four colours, its number of sides, radii, scale,
	taper, tiling, texture speed and texture name; the constructor calls
	NiTriShape's over arrays it allocates and stores the vtable 0x88D410
	(NiTriShape's 51 slots with the destructor 0x4E4E50 and slot 35,
	0x4E5040, replaced; slot 0 is NiTriShape's own GetRTTI, kept at
	0x4E7D10: the class has no NiRTTI). The destructor (0x4E4E70) stores
	the vtable and frees the arrays at 0x118, 0x11C and 0x120.

	PhaserGlow: 0x144 bytes. Evidence: PhaserGraphic's constructor
	allocates 0x144 bytes twice and calls the constructor (0x4E60C0, ret
	0x14) with the ship's set, the ship, 1.0 and two of the property's
	colours; it builds NiBillboardNode's (inlined: vtable 0x88CD48) and a
	NiPick at 0xF4, then stores the vtable 0x88D4F0 (NiBillboardNode's 44
	slots with the destructor 0x4E67A0, slot 25 0x4E6060 and slot 35
	0x4E6810 replaced; no NiRTTI of its own). It draws a quad of
	"Data/Textures/Tactical/PhaserLights.tga" (0x8E0FF0). The destructor
	is the implicit one (0x4E67C0: it only destroys the NiPick).

	PhaserGraphic: 0xF8 bytes. Evidence: PhaserBank allocates that much
	when it starts firing; the constructor (0x4E6AB0) calls NiNode's,
	keeps the phaser property (0xE8) and the ship (0xEC), stores the
	vtable 0x88D5A4 (NiNode's 44 slots with GetRTTI and the destructor
	replaced), whose GetRTTI returns the NiRTTI named "PhaserGraphic"
	(0x986368, built at 0x4E4050), and attaches five children: the beam
	(0), two NiPointLights (1, 2) and two glows (3, 4). The destructor is
	the implicit one (0x4E6E80 jumps to NiNode's).

	The class names other than PhaserGraphic, and the member and function
	names, are invented, after their use.

===============================================================================
*/

#ifndef __PHASERGRAPHIC_H__
#define __PHASERGRAPHIC_H__

#include "NiNode.h"
#include "NiTriShape.h"
#include "NiBillboardNode.h"
#include "NiPick.h"
#include "NiColor.h"

class NiCamera;

// the rings of vertices of each of the beam's two tubes, the shell and the core
#define PHASER_BEAM_RINGS		6
class NiPointLight;
class PhaserProperty;
class SetClass;
class ShipClass;

/*
================
PhaserBeam

The beam's tube: rings of numSides vertices along its length, its
texture scrolled at the property's texture speed.
================
*/
// FUNCTION: STBC 0x004e4e50 ??_GPhaserBeam@@UAEPAXI@Z
class PhaserBeam : public NiTriShape {
public:
							PhaserBeam( const NiColorA &outerShellColor, const NiColorA &innerShellColor, const NiColorA &outerCoreColor, const NiColorA &innerCoreColor,
										int numSides_, float mainRadius_, float taperRadius_, float coreScale_, float taperRatio_,
										float taperMinLength_, float taperMaxLength_, float lengthTextureTilePerUnit_, float perimeterTile_,
										float textureSpeed_, char *textureName );
	virtual					~PhaserBeam();

	// rebuilds the tube for the camera before the base's pass
	virtual void			VirtualSlot35( NiCamera *camera );

	// points the beam along direction, length long
	void					SetDirection( const NiPoint3 &direction_, float length_, bool unknown );
	// the texture starts unscrolled
	void					ResetTextureOffset();
	// scrolls the texture for deltaTime
	void					ScrollTexture( float deltaTime );

protected:
	float					unknownE4;					// 0xE4 0.7 at first
	float					unknownE8;					// 0xE8 1.0 at first
	float					mainRadius;					// 0xEC
	float					taperRadius;				// 0xF0
	float					coreScale;					// 0xF4
	float					taperMinLength;				// 0xF8
	float					taperRatio;					// 0xFC
	float					taperMaxLength;				// 0x100
	float					lengthTextureTilePerUnit;	// 0x104
	float					perimeterTile;				// 0x108
	float					textureSpeed;				// 0x10C texture lengths a second
	bool					unknown110;					// 0x110
	int						numSides;					// 0x114
	float *					sideAlphas;					// 0x118 owned: each vertex's alpha, the sine of its side's angle
	float *					sideTextureT;				// 0x11C owned: each vertex's texture t coordinate
	NiPoint3 *				sideDirections;				// 0x120 owned: each side's direction from the axis
	float					textureOffset;				// 0x124 how far the texture has scrolled
	NiPoint3				direction;					// 0x128
	float					length;						// 0x134
};

/*
================
PhaserGlow

A camera-facing quad of the phaser lights texture at one end of the beam,
coloured from the property's colours, that picks against the ship's set.
================
*/
// The implicit destructor only destroys the pick; the constructor emits it
// with the vtable.
// FUNCTION: STBC 0x004e67a0 ??_GPhaserGlow@@UAEPAXI@Z
// FUNCTION: STBC 0x004e67c0 ??1PhaserGlow@@UAE@XZ
class PhaserGlow : public NiBillboardNode {
public:
							PhaserGlow( SetClass *set_, ShipClass *ship_, float scale, const NiColorA &innerColor, const NiColorA &outerColor );

	// keeps the time since the glow was reset
	virtual void			UpdateDownwardPass( float time );
	virtual void			VirtualSlot35( NiCamera *camera );

	// restarts the clock and clears the quad's alpha
	void					Reset();
	void					SetSet( SetClass *set_ ) { set = set_; }

protected:
	SetClass *				set;				// 0xEC the set picked against
	ShipClass *				ship;				// 0xF0
	NiPick					pick;				// 0xF4
	bool					reset;				// 0x12C the next update restarts the clock
	float					unknown130;			// 0x130 1.0 at first
	float					lastTime;			// 0x134 the time of the last update
	float					elapsedTime;		// 0x138 since the reset
	NiColorA *				colors;				// 0x13C the quad's eight corner colours (the shape's)
	NiTriShape *			shape;				// 0x140 the quad, the only child
};

/*
================
PhaserGraphic

The beam, the lights at its ends and their glows, kept on the ship that
fires them.
================
*/
// FUNCTION: STBC 0x004e6e60 ??_GPhaserGraphic@@UAEPAXI@Z
// FUNCTION: STBC 0x004e6e80 ??1PhaserGraphic@@UAE@XZ
class PhaserGraphic : public NiNode {
public:
	// FUNCTION: STBC 0x004e6e50 ?GetRTTI@PhaserGraphic@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

							PhaserGraphic( PhaserProperty *property_, ShipClass *ship_ );

	// takes both lights off the ship
	void					RemoveLights();
	// whether the beam has been shown for its whole duration
	bool					IsFinished();
	// lights the start of the beam at time, for duration seconds
	void					Start( float time, float duration_ );
	// places the beam for time, growing over the duration
	void					Update( const NiPoint3 &position, const NiPoint3 &direction, float length, float time );
	// takes the start light off the ship
	void					RemoveStartLight();

	NiPointLight *			GetEndLight();

	// TODO identity: starts the charge-up at time for duration seconds: both
	// lights are lit in the inner shell colour and put on the ship, the beam
	// is hidden and the glows are reset onto the ship's set (0x4E6E90;
	// PhaserBank's beam creation calls it with 0.35 seconds).
	void					Unknown4E6E90( float time, float duration_ );
	// TODO identity: moves the two lights and glows of a charging beam
	// together along the arc, from either side of the width angle, as time
	// passes; true once they meet (0x4E7200; PhaserBank's beam update then
	// starts the beam).
	bool					Unknown4E7200( float time, float widthAngle );

	static const NiRTTI		rtti;

protected:
	NiPointLight *			GetStartLightNode();
	NiPointLight *			GetEndLightNode();
	PhaserGlow *			GetStartGlow();
	PhaserGlow *			GetEndGlow();
	PhaserBeam *			GetBeam();

	PhaserProperty *		property;			// 0xE8
	ShipClass *				ship;				// 0xEC
	float					startTime;			// 0xF0 game time the beam started
	float					duration;			// 0xF4 seconds
};

/*
================
PhaserGlow::Reset
================
*/
inline void PhaserGlow::Reset() {
	reset = true;
	for ( int i = 0; i < 8; i++ ) {
		colors[i].a = 0.0f;
	}
	shape->GetShapeData()->IncRevisionID();
}

#endif /* !__PHASERGRAPHIC_H__ */
