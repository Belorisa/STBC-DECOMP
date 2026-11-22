/*
===============================================================================

	MetaVolume.h

	A sphere of damage on a DamageableObject: its center, its radius and
	the strength the damage is drawn with.

	0x38 bytes. Evidence: the NiRTTI at 0x983C10 is named "MetaVolume"
	(0x8DD344) with NiObject's as its base; slot 0 of the vtable 0x88C6C4
	(0x4BCA90) returns it, and slot 10 is the scalar deleting destructor
	0x4BBE60; the other slots are NiObject's. The constructor (0x4BBDE0)
	calls NiObject's, copies the center (0x08), stores the radius (0x14),
	its square (0x18) and the strength (0x1C), then UpdateBounds (0x4BBEC0)
	sets the bounding box (0x20, 0x2C) from them.
	DamageableObject::AddMetaVolume (0x593E50) scales the radius and the
	strength by the object's visible damage radius and strength modifiers
	through the two setters (0x4BBE90, 0x4BBEB0).
	DamageableObject::ReadFromStream (0x595890) builds one from five
	streamed floats, and WriteToStream (0x595650) writes the same five.

	The unit also keeps a sphere model (0x983C28) for drawing a volume:
	an icosahedron that 0x4BBFD0 builds, or a subdivided one that 0x4BC390
	builds with a wireframe property. CreateDebugNode (0x4BBF20) hangs a
	clone of it, scaled to the radius, under a new node. Nothing in the
	retail game calls the three. The method names are invented.

===============================================================================
*/

#ifndef __METAVOLUME_H__
#define __METAVOLUME_H__

#include "NiObject.h"
#include "NiPoint3.h"
#include "NiPointer.h"
#include "NiTArray.h"

class NiNode;
class NiTriShape;

// FUNCTION: STBC 0x004bbe60 ??_GMetaVolume@@UAEPAXI@Z
class MetaVolume : public NiObject {
public:
	// FUNCTION: STBC 0x004bca90 ?GetRTTI@MetaVolume@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual					~MetaVolume();

							MetaVolume( const NiPoint3 &center_, float radius_, float strength_ );

	const NiPoint3 &		GetCenter( void ) const { return center; }
	float					GetRadius( void ) const { return radius; }
	float					GetStrength( void ) const { return strength; }
	const NiPoint3 &		GetBoxMin( void ) const { return boxMin; }
	const NiPoint3 &		GetBoxMax( void ) const { return boxMax; }

	// the volume's strength at a point: strength * ( 1 - d^2 / r^2 )^2
	// within the radius, 0 outside (DamageTool.exe's
	// ?GetSampleAt@MetaVolume@@QBEMABVNiPoint3@@@Z, 0x4120B0, has the
	// instructions of retail's out-of-line copy 0x4BAED0, which
	// MetaSurfacePolygonizer::Unknown4B5ED0 calls; VisibleDamageBuilder's
	// copy, which this build expands)
	// FUNCTION: STBC 0x004baed0 ?GetSampleAt@MetaVolume@@QBEMABVNiPoint3@@@Z @game/VisibleDamageBuilder
	float					GetSampleAt( const NiPoint3 &point ) const {
								float distance = ( point - center ).SqrLength();
								if ( distance < radiusSquared ) {
									float falloff = 1.0f - distance / radiusSquared;
									return falloff * strength * falloff;
								}
								return 0.0f;
							}

	// the volumes' strengths at a point, added up (invented name)
	static float			GetTotalStrength( const NiPoint3 *point, NiTArray< NiPointer<MetaVolume> > *volumes );
	// the same sum, adding each volume's offset to the point, scaled by its
	// strength there, to direction (placeholder name; nothing in retail calls it)
	static float			Unknown4BC7C0( const NiPoint3 *point, NiTArray< NiPointer<MetaVolume> > *volumes, NiPoint3 *direction );

	void					SetRadius( float radius_ );
	void					SetStrength( float strength_ );

	// a node holding a copy of the sphere model scaled to the radius
	NiNode *				CreateDebugNode( void );

	static const NiRTTI		rtti;

protected:
	friend class MetaSurfacePolygonizer;	// weighs the volumes from the members

	// the bounding box: the center less and plus the radius on each axis
	void					UpdateBounds( void );

	// build the unit's sphere model (0x983C28)
	static void				BuildSphereModel( void );
	static void				BuildSubdividedSphereModel( void );

	// the sphere a volume is drawn with, built on first use (invented name;
	// a class static: its exit-time release, 0x4BBDA0, takes the guard
	// byte's second bit after the NiRTTI's)
	static NiPointer<NiTriShape>	sphereModel;

	NiPoint3				center;				// 0x08
	float					radius;				// 0x14
	float					radiusSquared;		// 0x18
	float					strength;			// 0x1C
	NiPoint3				boxMin;				// 0x20
	NiPoint3				boxMax;				// 0x2C
};

/*
================
MetaVolumePool

A DamageableObject's damage volumes. Evidence: a NiObject of 0x20 bytes
(vtable 0x893D28: NiObject's slots, its own destructor 0x590E40) holding
an array of volumes at 0x08 (vtable 0x88C760); DamageableObject's
InitObject (0x590CB0) builds it inline, and its box query (0x4BC890)
lives in this unit. The name is DamageTool.exe (the BC SDK's damage tool, built with RTTI and a COFF symbol table):
its RTTI names MetaVolumePool : NiObject with the same vtable (NiObject's
slots and its destructor), and its ??1MetaVolumePool (0x403B40) and
GetInfluencing (0x4138C0) have this class's destructor's and box query's
instructions.
================
*/
class MetaVolumePool : public NiObject {
public:
					MetaVolumePool( void ) {}
	// FUNCTION: STBC 0x00590e40 ??1MetaVolumePool@@UAE@XZ
	// FUNCTION: STBC 0x00590e20 ??_GMetaVolumePool@@UAEPAXI@Z
	virtual			~MetaVolumePool( void ) {}

	// adds to volumes, with a reference each, the volumes whose box meets
	// the box from boxMin to boxMax (invented name); the list must not be
	// empty
	// takes a reference to the volume (invented name)
	// FUNCTION: STBC 0x00593ee0 ?AddVolume@MetaVolumePool@@QAEXPAVMetaVolume@@@Z @game/DamageableObject
	void			AddVolume( MetaVolume *volume ) { objects.Add( volume ); }

	void			GetVolumesInBox( const NiPoint3 *boxMin, const NiPoint3 *boxMax, NiTArray< NiPointer<MetaVolume> > *volumes );

	// DamageableObject's WriteToStream (0x595650) writes the elements as
	// MetaVolumes (center, radius and strength), and ReadFromStream
	// (0x595890) adds MetaVolumes it builds from the stream.
	NiTArray< NiPointer<MetaVolume> >	objects;	// 0x08 (invented name)
};

#endif /* !__METAVOLUME_H__ */
