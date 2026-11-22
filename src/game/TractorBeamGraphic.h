/*
===============================================================================

	TractorBeamGraphic.h

	The node that draws a tractor beam, and the two pieces it is built
	from: the beam's tube and the glow where it hits.

	The unit (from 0x4F5930 to 0x4F94B0) holds, in order: the static
	initialisers of the NetImmerse headers it includes, TractorBeamGraphic's
	NiRTTI (built at 0x4F5DE0), the beam (0x4F5E30-0x4F7AC0), the glow
	(0x4F7BA0-0x4F84B0), then TractorBeamGraphic (0x4F84C0 on).

	TractorBeamShape: 0x134 bytes. Evidence: TractorBeamGraphic's
	constructor allocates 0x134 bytes and calls the constructor (0x4F5E30,
	ret 0x40) with one colour four times, twelve sides, the property's beam
	width and constants; the constructor calls NiTriShape's and stores the
	vtable 0x88DD74 (NiTriShape's 51 slots with the destructor 0x4F6BC0 and
	slot 35, 0x4F6EA0, replaced; slot 0 is NiTriShape's own GetRTTI: the
	class has no NiRTTI). The destructor (0x4F6BE0) stores the vtable and
	frees the arrays at 0x118 and 0x11C.

	TractorBeamGlow: 0x138 bytes. Evidence: TractorBeamGraphic's
	constructor allocates 0x138 bytes and calls the constructor (0x4F7BA0)
	with the ship's set, the ship and two colours; it stores the vtable
	0x88DE40 (NiBillboardNode's 44 slots with the destructor 0x4F8270 and
	slot 35, 0x4F82E0, replaced; no NiRTTI of its own). The destructor is
	the implicit one (0x4F8290: it only destroys the NiPick at 0xF4).

	TractorBeamGraphic: 0xF8 bytes. Evidence: TractorBeamProjector
	allocates that much when it starts firing (0x580D98) and calls the
	constructor (0x4F84C0) with its property and its ship; the constructor
	calls NiNode's, keeps the ship (0xE8) and the property's beam width
	(0xEC), stores the vtable 0x88DEF4 (NiNode's 44 slots with GetRTTI and
	the destructor replaced), whose GetRTTI returns the NiRTTI named
	"TractorBeamGraphic" (0x986A10, built at 0x4F5DE0), and attaches four
	children: the beam (0), two NiPointLights (1, 2) and the glow (3). The
	destructor is the implicit one (0x4F8850 jumps to NiNode's).

	The class names other than TractorBeamGraphic, and the member and
	function names, are invented, after their use.

===============================================================================
*/

#ifndef __TRACTORBEAMGRAPHIC_H__
#define __TRACTORBEAMGRAPHIC_H__

#include "NiNode.h"
#include "NiTriShape.h"
#include "NiBillboardNode.h"
#include "NiPick.h"
#include "NiColor.h"

class NiCamera;

// the rings of vertices of each of the beam's two tubes, the shell and the core
#define TRACTOR_BEAM_RINGS		6
class NiPointLight;
class SetClass;
class ShipClass;
class TractorBeamProperty;

/*
================
TractorBeamShape

The beam's tube, its texture scrolled with time.
================
*/
// FUNCTION: STBC 0x004f6bc0 ??_GTractorBeamShape@@UAEPAXI@Z
class TractorBeamShape : public NiTriShape {
public:
							TractorBeamShape( const NiColorA &color0, const NiColorA &color1, const NiColorA &color2, const NiColorA &color3,
											  int numSides_, float radius, float unknown6, float unknown7, float unknown8, float unknown9,
											  float unknown10, float unknown11, float unknown12, float unknown13, float textureSpeed_,
											  char *textureName );
	virtual					~TractorBeamShape();

	// rebuilds the tube for the camera before the base's pass
	virtual void			VirtualSlot35( NiCamera *camera );

	// points the beam along direction, length long
	void					SetDirection( const NiPoint3 &direction_, float length_ );
	// scrolls the texture to where it is time seconds after the start
	void					SetTextureTime( float time );

protected:
	bool					unknownE4;			// 0xE4 true at first
	float					unknownE8;			// 0xE8 0.7 at first
	float					unknownEC;			// 0xEC 1.0 at first
	float					unknownF0;			// 0xF0
	float					unknownF4;			// 0xF4 the constructor's sixth argument: the radius
	float					unknownF8;			// 0xF8
	float					unknownFC;			// 0xFC
	float					unknown100;			// 0x100
	float					unknown104;			// 0x104
	float					unknown108;			// 0x108
	float					unknown10C;			// 0x10C
	float					textureSpeed;		// 0x110 texture lengths a second
	int						numSides;			// 0x114
	float *					sideAlphas;			// 0x118 owned: each vertex's alpha, the sine of its side's angle, less on the last two rings
	float *					sideTextureT;		// 0x11C owned: each vertex's texture t coordinate
	float					textureOffset;		// 0x120 how far the texture has scrolled
	NiPoint3				direction;			// 0x124
	float					length;				// 0x130
};

/*
================
TractorBeamGlow

A camera-facing quad where the beam hits, that picks against the ship's
set.
================
*/
// The implicit destructor only destroys the pick; the constructor emits it
// with the vtable.
// FUNCTION: STBC 0x004f8270 ??_GTractorBeamGlow@@UAEPAXI@Z
// FUNCTION: STBC 0x004f8290 ??1TractorBeamGlow@@UAE@XZ
class TractorBeamGlow : public NiBillboardNode {
public:
							TractorBeamGlow( SetClass *set_, ShipClass *ship_, const NiColorA &innerColor, const NiColorA &outerColor );

	virtual void			VirtualSlot35( NiCamera *camera );

	void					SetSet( SetClass *set_ ) { set = set_; }
	void					SetUnknown12C( float value ) { unknown12C = value; }

protected:
	SetClass *				set;				// 0xEC the set picked against
	ShipClass *				ship;				// 0xF0
	NiPick					pick;				// 0xF4
	float					unknown12C;			// 0x12C 1.0 when the beam starts
	NiColorA *				colors;				// 0x130 the quad's eight corner colours (the shape's)
	NiTriShape *			shape;				// 0x134 the quad, the only child
};

/*
================
TractorBeamGraphic

The beam, the lights at its ends and the glow where it hits, kept on the
ship that fires them.
================
*/
// FUNCTION: STBC 0x004f8830 ??_GTractorBeamGraphic@@UAEPAXI@Z
// FUNCTION: STBC 0x004f8850 ??1TractorBeamGraphic@@UAE@XZ
class TractorBeamGraphic : public NiNode {
public:
	// FUNCTION: STBC 0x004f8800 ?GetRTTI@TractorBeamGraphic@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

							TractorBeamGraphic( TractorBeamProperty *property, ShipClass *ship_ );

	NiPointLight *			GetEndLight();

	// shows the beam from time, growing over duration seconds
	void					Start( float time, float duration_ );
	// places the beam for time, growing over the duration
	void					Update( const NiPoint3 &position, const NiPoint3 &direction, float length, float time );
	// takes the start light off the ship and hides the beam
	void					Stop();

	static const NiRTTI		rtti;

protected:
	NiPointLight *			GetStartLight();
	TractorBeamGlow *		GetGlow();
	TractorBeamShape *		GetBeam();

	ShipClass *				ship;				// 0xE8
	float					width;				// 0xEC the property's beam width
	float					startTime;			// 0xF0 game time the beam started
	float					duration;			// 0xF4 seconds
};

#endif /* !__TRACTORBEAMGRAPHIC_H__ */
