/*
===============================================================================

	NiParticles.h

	A particle geometry object: an NiTriBasedGeom drawn from
	NiParticlesData, one billboard per active vertex.

	Evidence: the NiRTTI (0x9A1CB0) is named "NiParticles" with
	NiTriBasedGeom's as its base; the vtable at 0x8995D8 has NiTriBasedGeom's
	49 slots and adds none (the load record's follows at 0x89969C); the
	constructors (0x7F4F50, 0x7F4FD0, 0x7F4FF0) call NiTriBasedGeom's and add
	no members, the first creating the data from four arguments; CreateClone
	(0x7F5250) allocates 0xE4 bytes. The world vertex, normal and bound
	updates cover only the data's active particles; slot 31 (0x7F50C0) hands
	the renderer at the argument's 0x188 the particles through its slot 49.

===============================================================================
*/

#ifndef __NIPARTICLES_H__
#define __NIPARTICLES_H__

#include "NiTriBasedGeom.h"
#include "NiParticlesData.h"
#include "NiStreamMacros.h"

/*
================
NiParticlesLoadRecord

The record CreateObject hands the stream. The class name is invented.

Evidence: CreateObject (0x7F52D0) allocates 0x2C bytes and stores the vptr
0x89969C after the link IDs, the property link ID array at 0x10 and the
model data's link ID at 0x28.
================
*/
// FUNCTION: STBC 0x7F53B0 ??_GNiParticlesLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7F53D0 ??1NiParticlesLoadRecord@@UAE@XZ
class NiParticlesLoadRecord : public NiTriBasedGeomLoadRecord {
};

// FUNCTION: STBC 0x007F5700 ??_GNiParticles@@UAEPAXI@Z
class NiParticles : public NiTriBasedGeom {
public:
	// FUNCTION: STBC 0x7F56F0 ?GetRTTI@NiParticles@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	// the world copies and bound of the active particles only
	virtual void			UpdateWorldVertices();
	virtual void			UpdateWorldNormals();

	// TODO identity: slot 31 draws the particles through the renderer at the
	// argument's 0x188 and counts the shapes, triangles and vertices drawn
	virtual void			Display( NiCamera *camera );

	// particles take no part in NiTriBasedGeom's slot 34
	virtual bool			FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick );

	virtual void			UpdateWorldBound();

	virtual void			GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );
	virtual void			GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 );

							NiParticles( unsigned short vertexCount_, NiPoint3 *vertices_, NiPoint3 *normals_, NiColorA *colors_ );
							NiParticles( NiParticlesData *data );
							NiParticles();

	// the model data as what it is (name invented)
	NiParticlesData *		GetParticlesData() const { return (NiParticlesData *)(NiGeometryData *)modelData; }
	// the model data's active particle count (name invented; AnimOnly.exe
	// 0x4381C0)
	unsigned short			GetActiveParticleCount() const;
	// and its setter, held to the vertex count (name invented; AnimOnly.exe
	// 0x4A44F0)
	void					SetActiveParticleCount( unsigned short count );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiParticles *dest );

};

NiRegisterStream( NiParticles );

/*
================
NiParticles::GetActiveParticleCount
================
*/
inline unsigned short NiParticles::GetActiveParticleCount() const {
	NiParticlesData *data = NiSmartPointerCast( NiParticlesData, modelData );
	return data->GetActiveParticleCount();
}

/*
================
NiParticles::SetActiveParticleCount
================
*/
inline void NiParticles::SetActiveParticleCount( unsigned short count ) {
	NiParticlesData *data = NiSmartPointerCast( NiParticlesData, modelData );
	data->SetActiveParticleCount( count );
}

#endif /* !__NIPARTICLES_H__ */
