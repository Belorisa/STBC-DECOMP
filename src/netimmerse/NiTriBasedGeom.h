/*
===============================================================================

	NiTriBasedGeom.h

	A geometry object drawn as triangles, with an optional oriented
	bounding box tree for collision tests.

	Evidence: the vtable at 0x899264 (49 slots: NiGeometry's 46, then 3 new,
	two of them _purecall); the constructors (0x7EF230, 0x7EF260) call
	NiGeometry's and clear the tree at 0xE0; the destructor (0x7EF280)
	deletes the tree through its slot 0; the NiRTTI (0x9A1AF8) is named
	"NiTriBasedGeom" with NiGeometry's as its base; the viewer strings name
	the tree ("m_pOBBRoot"). NiTriShape's overrides give the new slots their
	meaning: 46 and 47 return a triangle's three model or world vertices,
	48 its three vertex indices. Slot 24 deletes the tree; NiTriShape's slot
	23 builds it. The names follow the Gamebryo lineage.

===============================================================================
*/

#ifndef __NITRIBASEDGEOM_H__
#define __NITRIBASEDGEOM_H__

#include "NiGeometry.h"
#include "NiTriBasedGeomData.h"
#include "NiStreamMacros.h"

class NiOBBRoot;
struct NiCollisionIntersect;

/*
================
NiTriBasedGeomLoadRecord

The record level of the triangle geometry classes: NiGeometry's, adding
nothing. The class name is invented.

Evidence: the load records mirror the object classes, and the creation
functions of NiTriShape's and NiParticles's subclasses expand their records'
constructors as deep as a record two levels under NiGeometry's allows
(NiEnvMappedTriShape::CreateObject, 0x7FB610, NiAutoNormalParticles's,
0x7F60F0); the vtable stores of this level are dead in every inlined
constructor.
================
*/
class NiTriBasedGeomLoadRecord : public NiGeometryLoadRecord {
};

// FUNCTION: STBC 0x007F1230 ??_GNiTriBasedGeom@@UAEPAXI@Z
class NiTriBasedGeom : public NiGeometry {
public:
	// FUNCTION: STBC 0x007F1220 ?GetRTTI@NiTriBasedGeom@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiTriBasedGeom();

	// deletes the bounding box tree
	virtual void			VirtualSlot24();

	// TODO identity: the collision tests against another object, through
	// the bounding box trees or the triangles
	virtual bool			TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual int				FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual bool			FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick );

	// new data deletes the bounding box tree
	virtual void			SetModelData( NiGeometryData *data );

	// a triangle's three vertices, in model or world space
	virtual void			GetModelTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) = 0;
	virtual void			GetWorldTriangle( unsigned short index, NiPoint3 *&vertex0, NiPoint3 *&vertex1, NiPoint3 *&vertex2 ) = 0;
	// a triangle's three vertex indices. DamageTool.exe declares it
	// non-const (GetPickInfo, UAE), its NiTriStrips copy byte-identical to
	// retail's 0x80EF80; non-const here costs that copy its match, so the
	// body's spelling differs from the original somewhere and const stays
	// FUNCTION: STBC 0x007F1260 ?GetTriangleIndices@NiTriBasedGeom@@UBEXGAAG00@Z
	virtual void			GetTriangleIndices( unsigned short index, unsigned short &index0, unsigned short &index1, unsigned short &index2 ) const {}

							NiTriBasedGeom( NiTriBasedGeomData *data );
							NiTriBasedGeom();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// whether a ray meets a triangle (only its front face when frontOnly is
	// set): the point, the distance along the ray and the barycentric
	// coordinates of the hit (name invented)
	static bool				IntersectTriangle( const NiPoint3 &origin, const NiPoint3 &dir, const NiPoint3 &vertex0,
											   const NiPoint3 &vertex1, const NiPoint3 &vertex2, bool frontOnly,
											   NiPoint3 &intersect, float &distance, float &u, float &v );

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiTriBasedGeom *dest );

	NiTriBasedGeomData *	GetTriData() const { return (NiTriBasedGeomData *)(NiGeometryData *)modelData; }

	// the model data's triangle count, through a local as Gamebryo 1.1.2's
	// NITRIBASEDGEOM.INL writes it and the SDK's debug NetImmerse 3.1
	// compiles it (AnimOnly.exe 0x432510)
	unsigned short			GetTriangleCount() const {
								NiTriBasedGeomData *data = NiSmartPointerCast( NiTriBasedGeomData, modelData );
								return data->GetTriangleCount();
							}
	// the active counts, written the same way (Gamebryo lineage names;
	// AnimOnly.exe 0x451210, 0x451240)
	unsigned short			GetActiveTriangleCount() const {
								NiTriBasedGeomData *data = NiSmartPointerCast( NiTriBasedGeomData, modelData );
								return data->GetActiveTriangleCount();
							}
	unsigned short			GetActiveVertexCount() const {
								NiTriBasedGeomData *data = NiSmartPointerCast( NiTriBasedGeomData, modelData );
								return data->GetActiveVertexCount();
							}

	// the triangle collision tests (names invented): every world triangle
	// against the other object's triangles, or against its world volume
	bool					TestTriangles( float deltaTime, NiTriBasedGeom *other );
	int						FindTriangles( float deltaTime, NiTriBasedGeom *other, bool calcNormals,
										   NiCollisionIntersect &intersect );
	bool					TestVolumeTriangles( float deltaTime, NiAVObject *other );
	int						FindVolumeTriangles( float deltaTime, NiAVObject *other, bool calcNormals,
												 NiCollisionIntersect &intersect );

	NiOBBRoot *				obbRoot;		// owned, or NULL
};

NiRegisterStream( NiTriBasedGeom );

#endif /* !__NITRIBASEDGEOM_H__ */
