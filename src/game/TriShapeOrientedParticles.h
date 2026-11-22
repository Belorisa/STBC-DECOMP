/*
===============================================================================

	TriShapeOrientedParticles.h

	Particles drawn as quads that lie along a direction of their own: each
	particle's quad is stretched along its direction and turned about it to
	face the camera.

	TriShapeOrientedParticlesData: 0x64 bytes. Evidence: the NiRTTI at
	0x986B40 is built with the name "TriShapeOrientedParticlesData"
	(0x8E10DC) and TriShapeParticlesData's NiRTTI as its base (0x4FA3B0);
	the constructor (0x4FA5F0) builds TriShapeParticlesData's without sizes,
	stores the dimensions at 0x5C and a new NiPoint3 per particle at 0x60,
	and stores the vtable 0x88E1AC (TriShapeParticlesData's 18 slots with
	GetRTTI and the destructor replaced); the destructor (0x4FA6B0) frees
	both arrays.

	TriShapeOrientedParticles: 0xF0 bytes. Evidence: the NiRTTI at
	0x986B08 is built with the name "TriShapeOrientedParticles" (0x8E10FC)
	and TriShapeParticles' NiRTTI as its base (0x4FA410); the constructors
	(0x4FA460, 0x4FA710) call TriShapeParticles' and store the vtable
	0x88E0CC (TriShapeParticles' 56 slots with GetRTTI, CreateClone, the
	destructor, CreateData and UpdateQuads replaced); CreateClone (0x4FA740)
	allocates 0xF0 bytes. The quads take the width at 0xE8 and the length at
	0xEC when the data holds no dimensions (UpdateQuads, 0x4FA960).

	The member and function names are invented, after their use.

===============================================================================
*/

#ifndef __TRISHAPEORIENTEDPARTICLES_H__
#define __TRISHAPEORIENTEDPARTICLES_H__

#include "TriShapeParticles.h"

/*
================
TriShapeOrientedParticlesData

TriShapeParticlesData with a direction per particle and, optionally, a
width and length per particle. Both arrays are owned.
================
*/
// FUNCTION: STBC 0x004fa690 ??_GTriShapeOrientedParticlesData@@UAEPAXI@Z
class TriShapeOrientedParticlesData : public TriShapeParticlesData {
public:
	// FUNCTION: STBC 0x004fa680 ?GetRTTI@TriShapeOrientedParticlesData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual					~TriShapeOrientedParticlesData();

							TriShapeOrientedParticlesData( unsigned short particleCount, NiPoint3 *particleNormals_, NiColorA *particleColors_, NiPoint2 *textures_, NiPoint2 *dimensions_ );

	static const NiRTTI		rtti;

	NiPoint2 *				dimensions;			// 0x5C or NULL: x the width, y the length of each quad
	NiPoint3 *				directions;			// 0x60 the direction each quad lies along
};

/*
================
TriShapeOrientedParticlesData::~TriShapeOrientedParticlesData
================
*/
// FUNCTION: STBC 0x004fa6b0 ??1TriShapeOrientedParticlesData@@UAE@XZ
inline TriShapeOrientedParticlesData::~TriShapeOrientedParticlesData() {
	delete [] dimensions;
	delete [] directions;
}

/*
================
TriShapeOrientedParticles
================
*/
// FUNCTION: STBC 0x004fb550 ??_GTriShapeOrientedParticles@@UAEPAXI@Z
// FUNCTION: STBC 0x004fb570 ??1TriShapeOrientedParticles@@UAE@XZ
class TriShapeOrientedParticles : public TriShapeParticles {
public:
	// FUNCTION: STBC 0x004fb540 ?GetRTTI@TriShapeOrientedParticles@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// new data for as many particles, with arrays where this shape's data
	// has them
	virtual TriShapeParticlesData *	CreateData();
	// places each live particle's quad along its direction, turned to face
	// the camera, and copies the particle's colour and normal to its corners
	virtual void			UpdateQuads( NiCamera *camera );

							TriShapeOrientedParticles( unsigned short particleCount, bool hasNormals, bool hasColors, bool hasDimensions, bool hasTextures );
							TriShapeOrientedParticles( TriShapeOrientedParticlesData *data );

	NiPoint3 *				GetDirections();
	NiPoint2 *				GetDimensions();
	// every quad's size, when the data has no dimensions (name invented)
	void					SetDefaultDimensions( float width_, float length_ ) { width = width_; length = length_; }

	static const NiRTTI		rtti;

protected:
	// the model data as what it is
	TriShapeOrientedParticlesData *	GetOrientedData() const { return (TriShapeOrientedParticlesData *)(NiGeometryData *)modelData; }

	void					CopyMembers( TriShapeOrientedParticles *dest );

	float					width;				// 0xE8 every quad's width, when the data has no dimensions
	float					length;				// 0xEC every quad's length, when the data has no dimensions
};

#endif /* !__TRISHAPEORIENTEDPARTICLES_H__ */
