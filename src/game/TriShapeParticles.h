/*
===============================================================================

	TriShapeParticles.h

	Particles drawn as camera-facing quads: an NiTriShape of four vertices
	and two triangles per particle, whose quads are rebuilt around each
	particle's position before it is drawn. The particle controllers write
	the positions, sizes, colours and normals one per particle; the shape
	spreads them to the quads' corners.

	TriShapeParticlesData: 0x5C bytes. Evidence: the NiRTTI at 0x986B50 is
	built with the name "TriShapeParticlesData" (0x8E10B0) and
	NiTriShapeDynamicData's NiRTTI as its base (0x4F9400); the constructor
	(0x4F9630) builds NiTriShapeDynamicData's over four new vertices and two
	triangles per particle and stores the vtable 0x88E084 (18 slots,
	NiTriShapeDynamicData's with GetRTTI and the destructor replaced); the
	destructor (0x4F97D0) frees the four per-particle arrays at 0x4C-0x58.

	TriShapeParticles: 0xE8 bytes. Evidence: the NiRTTI at 0x986B30 is built
	with the name "TriShapeParticles" (0x8E10C8) and NiTriShape's NiRTTI as
	its base (0x4F9460); the constructors (0x4F94B0, 0x4F9830) call
	NiTriShape's and store the vtable 0x88DFA4 (NiTriShape's 51 slots and
	five of its own); CreateClone (0x4F9890) allocates 0xE8 bytes. The
	destructor is the implicit one: its copy (0x4FB530, kept with GetRTTI's
	at 0x4FB500 by the next unit) jumps straight to NiTriBasedGeom's.
	PointParticleController::CreateTarget builds one; the controllers read
	the per-particle arrays through the getters (0x4F9C40-0x4F9C70) and set
	the live count through slot 52.

	The member and function names are invented, after their use.

===============================================================================
*/

#ifndef __TRISHAPEPARTICLES_H__
#define __TRISHAPEPARTICLES_H__

#include "NiTriShape.h"
#include "NiTriShapeDynamicData.h"

class NiCamera;

/*
================
TriShapeParticlesData

The quads' geometry, and the per-particle arrays the quads are built from.
Each array holds one element per particle and is owned.
================
*/
// FUNCTION: STBC 0x004f97b0 ??_GTriShapeParticlesData@@UAEPAXI@Z @game/TriShapeParticles
class TriShapeParticlesData : public NiTriShapeDynamicData {
public:
	// FUNCTION: STBC 0x004f97a0 ?GetRTTI@TriShapeParticlesData@@UBEPBVNiRTTI@@XZ @game/TriShapeParticles
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual					~TriShapeParticlesData();

							TriShapeParticlesData( unsigned short particleCount, NiPoint3 *particleNormals_, NiColorA *particleColors_, NiPoint2 *textures_, float *sizes_ );

	static const NiRTTI		rtti;

	float *					sizes;				// 0x4C or NULL: the shape's one size then
	NiPoint3 *				positions;			// 0x50 the quads' centres
	NiColorA *				particleColors;		// 0x54 or NULL
	NiPoint3 *				particleNormals;	// 0x58 or NULL
};

/*
================
TriShapeParticlesData::~TriShapeParticlesData

Inline: TriShapeOrientedParticlesData's destructor (0x4FA6B0) expands it.
================
*/
// FUNCTION: STBC 0x004f97d0 ??1TriShapeParticlesData@@UAE@XZ @game/TriShapeParticles
inline TriShapeParticlesData::~TriShapeParticlesData() {
	delete [] sizes;
	delete [] positions;
	delete [] particleColors;
	delete [] particleNormals;
}

// FUNCTION: STBC 0x004fb510 ??_GTriShapeParticles@@UAEPAXI@Z
// FUNCTION: STBC 0x004fb530 ??1TriShapeParticles@@UAE@XZ
class TriShapeParticles : public NiTriShape {
public:
	// FUNCTION: STBC 0x004fb500 ?GetRTTI@TriShapeParticles@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// builds the quads for the camera before the base's pass
	virtual void			VirtualSlot35( NiCamera *camera );
	// the model bound is taken over the particles' positions
	virtual void			UpdateWorldBound();

	// the model bound is recomputed at the next update
	virtual void			InvalidateBound();
	virtual void			SetActiveParticleCount( unsigned short count );
	virtual unsigned short	GetActiveParticleCount();
	// new data for as many particles, with arrays where this shape's data
	// has them
	virtual TriShapeParticlesData *	CreateData();
	// places each live particle's quad facing the camera, and copies the
	// particle's colour and normal to its corners
	virtual void			UpdateQuads( NiCamera *camera );

							TriShapeParticles( unsigned short particleCount, bool hasNormals, bool hasColors, bool hasSizes, bool hasTextures );
							TriShapeParticles( TriShapeParticlesData *data );

	unsigned int			GetParticleCount();
	// maps the whole texture onto each quad
	void					SetTextureCoordinates();

	NiPoint3 *				GetPositions();
	float *					GetSizes();
	NiColorA *				GetParticleColors();
	NiPoint3 *				GetParticleNormals();

	static const NiRTTI		rtti;

protected:
	// the model data as what it is
	TriShapeParticlesData *	GetParticleData() const { return (TriShapeParticlesData *)(NiGeometryData *)modelData; }

	void					CopyMembers( TriShapeParticles *dest );
	// two triangles per quad, over the quad's four vertices
	void					BuildTriangleList();

	float					size;				// 0xE4 every particle's size, when the data has no sizes
};

#endif /* !__TRISHAPEPARTICLES_H__ */
