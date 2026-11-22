/*
===============================================================================

	AnimatedTriShapeParticles.h

	Particles drawn as camera-facing quads whose texture coordinates play a
	texture animation track: an NiTriShape of four vertices and two
	triangles per particle over arrays its controller owns. Before each
	draw the quads are rebuilt around the particles' positions and each
	quad takes its particle's frame of its track.

	0xF8 bytes. Evidence: the NiRTTI at 0x985AA8 is built with the name
	"AnimatedTriShapeParticles" (0x8E0F04) and NiTriShape's NiRTTI as its
	base (0x4D9790); the constructor (0x4D97E0) builds an
	NiTriShapeDynamicData over four vertices and two triangles per particle
	from the arrays it is given, keeps the per-particle arrays at
	0xE4-0xF4 and stores the vtable 0x88CF40 (NiTriShape's 51 slots with
	GetRTTI, the destructor, 35 and 38 replaced, and three of its own);
	AnimTSParticleController::CreateTarget (0x4C7360) allocates 0xF8 bytes
	and passes its own arrays. The destructor is the implicit one (0x4D98E0
	jumps to NiTriBasedGeom's): the arrays belong to the controller.

	The member and function names are invented, after their use.

===============================================================================
*/

#ifndef __ANIMATEDTRISHAPEPARTICLES_H__
#define __ANIMATEDTRISHAPEPARTICLES_H__

#include "NiTriShape.h"
#include "NiTriShapeDynamicData.h"

class NiCamera;
class TextureAnimTrack;

// FUNCTION: STBC 0x004d98c0 ??_GAnimatedTriShapeParticles@@UAEPAXI@Z
// FUNCTION: STBC 0x004d98e0 ??1AnimatedTriShapeParticles@@UAE@XZ
class AnimatedTriShapeParticles : public NiTriShape {
public:
	// FUNCTION: STBC 0x004d98b0 ?GetRTTI@AnimatedTriShapeParticles@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// builds the quads for the camera after the base's pass
	virtual void			VirtualSlot35( NiCamera *camera );
	// the model bound is taken over the particles' positions
	virtual void			UpdateWorldBound();

	// places each live particle's quad facing the camera, and gives it its
	// particle's colour and its frame of its track
	virtual void			UpdateQuads( NiCamera *camera );
	virtual void			SetActiveParticleCount( unsigned short count );
	virtual unsigned short	GetActiveParticleCount();

							AnimatedTriShapeParticles( unsigned short particleCount, NiPoint3 *vertices, NiPoint3 *normals, NiColorA *colors, NiPoint2 *textures, unsigned short *triList, NiPoint3 *positions_, float *sizes_, NiColorA *particleColors_, unsigned short *frames_, TextureAnimTrack **tracks_ );

	NiPoint3 *				GetPositions() { return positions; }
	// the model data as what it is
	NiTriShapeDynamicData *	GetDynamicData() const { return (NiTriShapeDynamicData *)(NiGeometryData *)modelData; }

	static const NiRTTI		rtti;

protected:
	NiPoint3 *				positions;			// 0xE4 the quads' centres, one per particle
	float *					sizes;				// 0xE8 one per particle
	NiColorA *				particleColors;		// 0xEC one per particle, or NULL
	unsigned short *		frames;				// 0xF0 each particle's frame of its track
	TextureAnimTrack **		tracks;				// 0xF4 each particle's track
};

#endif /* !__ANIMATEDTRISHAPEPARTICLES_H__ */
