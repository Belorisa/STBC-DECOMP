/*
===============================================================================

	TGFuzzyClusterGeom.h

	The shapes a nebula is drawn with.

	TGFuzzyClusterGeom: one sphere of a MetaNebula, fading out as the
	camera comes near. 0xF4 bytes. Evidence: the NiRTTI at 0x98DE98 is built
	with the name "TGFuzzyClusterGeom" (0x8E5D88) and TGFuzzyTriShape's
	NiRTTI as its base (0x597480); the constructor (0x5992E0) calls
	TGFuzzyTriShape's and stores the vtable 0x894058: TGFuzzyTriShape's 51
	slots with GetRTTI (0x599320), the destructor (0x599330) and slot 35
	(0x599400) replaced. MetaNebula::AddSphere (0x59B520) allocates 0xF4
	bytes for one.

	TGFuzzyClusterInnerGeom: the overlay the camera sees from inside a
	nebula. 0xE8 bytes. Evidence: the NiRTTI at 0x98DE48 is built with the
	name "TGFuzzyClusterInnerGeom" (0x8E5D70) and NiTriShape's NiRTTI as its
	base (0x597420); the vtable 0x893F80 is NiTriShape's 51 slots with
	GetRTTI (0x598080), the destructor (0x598090) and slot 35 (0x599630)
	replaced; the destructor (0x5980B0) releases the pointer at 0xE4.

	TGFuzzyClusterInnerController, the overlay's controller, is defined in
	Nebula.cpp, its only user.

	The classes' names are their NiRTTI's; the member and function names are
	invented.

===============================================================================
*/

#ifndef __TGFUZZYCLUSTERGEOM_H__
#define __TGFUZZYCLUSTERGEOM_H__

#include "TGFuzzyTriShape.h"
#include "NiColor.h"
#include "NiMath.h"

class NiNode;


// FUNCTION: STBC 0x00599330 ??_GTGFuzzyClusterGeom@@UAEPAXI@Z
class TGFuzzyClusterGeom : public TGFuzzyTriShape {
public:
	// FUNCTION: STBC 0x00599320 ?GetRTTI@TGFuzzyClusterGeom@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }
	virtual					~TGFuzzyClusterGeom();

	// sets the vertices' fuzziness for the camera before the base's pass
	virtual void			VirtualSlot35( NiCamera *camera );

							TGFuzzyClusterGeom( NiTriShape *shape );

	// frees the colours and allocates one per vertex, black each
	void					AllocateColors();
	void					SetFadeDistance( float distance ) { fadeDistance = distance; }

	static const NiRTTI		rtti;

protected:
	NiColor *				colors;				// 0xE8 the vertices' colours without their alpha, or NULL; owned
	float					fadeDistance;		// 0xEC how near the surface the camera fades the vertices; a tenth of the radius when it is not positive
	bool					opaque;				// 0xF0 every vertex has the fuzziness 1
};

// FUNCTION: STBC 0x00598090 ??_GTGFuzzyClusterInnerGeom@@UAEPAXI@Z
// FUNCTION: STBC 0x005980b0 ??1TGFuzzyClusterInnerGeom@@UAE@XZ
class TGFuzzyClusterInnerGeom : public NiTriShape {
public:
	// FUNCTION: STBC 0x00598080 ?GetRTTI@TGFuzzyClusterInnerGeom@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual void			VirtualSlot35( NiCamera *camera );

							TGFuzzyClusterInnerGeom( NiTriShapeData *data, NiNode *clusters_ ) : NiTriShape( data ), clusters( clusters_ ) {}

	void					SetClusters( NiNode *clusters_ ) { clusters = clusters_; }

	static const NiRTTI		rtti;

protected:
	NiPointer<NiNode>		clusters;			// 0xE4 the nebula's object: the node of its spheres (VirtualSlot35 reads its children; MetaNebula::CreateExterior stores it through NiPointer<NiNode>'s constructor, 0x46C560)
};

#endif /* !__TGFUZZYCLUSTERGEOM_H__ */
