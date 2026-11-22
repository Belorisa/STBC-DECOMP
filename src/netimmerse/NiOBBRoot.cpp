// NiOBBRoot.cpp -- the root of a shape's bounding box tree

#include "NiRTLib.h"
#include "NiOBBRoot.h"
#include "NiOBBLeaf.h"
#include "NiNode.h"
#include "NiDynamicEffect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00823950 _$E37
// FUNCTION: STBC 0x00823960 _$E48
// FUNCTION: STBC 0x00823970 _$E52
// FUNCTION: STBC 0x00823980 _$E56
// FUNCTION: STBC 0x00823990 _$E60
// FUNCTION: STBC 0x008239a0 _$E177
// FUNCTION: STBC 0x008239b0 _$E184
// FUNCTION: STBC 0x008239c0 _$E188

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x008238d0 _$E195
// FUNCTION: STBC 0x008238f0 _$E17
// FUNCTION: STBC 0x00823910 _$E15
// FUNCTION: STBC 0x00823920 _$E23
// FUNCTION: STBC 0x00823940 _$E21

/*
================
NiOBBRoot::NiOBBRoot

A single triangle is a leaf; more are split into a tree of nodes.
================
*/
// FUNCTION: STBC 0x8235F0
NiOBBRoot::NiOBBRoot( unsigned short triangleCount, const unsigned short *triList, const NiPoint3 *vertices,
					  const NiPoint3 *worldVertices, int binSize ) {
	if ( triangleCount > 1 ) {
		tree = new NiOBBNode( this, triangleCount, triList, vertices, worldVertices, binSize );
	} else {
		tree = new NiOBBLeaf( this, triList, vertices, worldVertices, 0 );
	}
	deltaTime = 0.0f;
	velocity = NiPoint3::ZERO;
	rotate = NiMatrix3::IDENTITY;
	translate = NiPoint3::ZERO;
	scale = 1.0f;
	transformChanged = false;
	changeStamp = 0;
}

/*
================
NiOBBRoot::~NiOBBRoot
================
*/
// FUNCTION: STBC 0x823740
NiOBBRoot::~NiOBBRoot() {
	delete tree;
}

/*
================
NiOBBRoot::Transform

Nothing changes when the transform is the one the root holds.
================
*/
// FUNCTION: STBC 0x823760
void NiOBBRoot::Transform( const NiMatrix3 &rotate_, const NiPoint3 &translate_, float scale_ ) {
	if ( translate == translate_ && scale == scale_ && rotate == rotate_ ) {
		return;
	}
	rotate = rotate_;
	translate = translate_;
	scale = scale_;
	transformChanged = true;
	changeStamp++;
}

/*
================
NiOBBRoot::UpdateTree
================
*/
inline void NiOBBRoot::UpdateTree() {
	if ( transformChanged ) {
		transformChanged = false;
		tree->Transform( rotate, translate, scale );
	}
}

/*
================
NiOBBRoot::TestCollision

A shape does not collide with itself.
================
*/
// FUNCTION: STBC 0x8237E0
bool NiOBBRoot::TestCollision( NiOBBRoot *other, int depth, int otherDepth ) {
	if ( this == other ) {
		return false;
	}
	other->UpdateTree();
	UpdateTree();
	return tree->TestCollision( other->tree, depth, otherDepth );
}

/*
================
NiOBBRoot::FindCollisions

The nodes move their boxes themselves, each when its root's stamp differs
from the one it last moved for.
================
*/
// FUNCTION: STBC 0x823850
// Gamebryo 1.2.2: NiOBBRoot::FindCollisions
int NiOBBRoot::FindCollisions( NiOBBRoot *other, NiAVObject *root0, NiAVObject *root1, NiAVObject *object0,
							   NiAVObject *object1, int depth, int otherDepth ) {
	if ( this == other ) {
		return 0;
	}
	return tree->FindCollisions( other->tree, root0, root1, object0, object1, depth, otherDepth, changeStamp,
								 other->GetChangeStamp() );
}

/*
================
NiOBBRoot::FindIntersections
================
*/
// FUNCTION: STBC 0x8238A0
int NiOBBRoot::FindIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, bool &found,
								  NiAVObject *object, unsigned int stamp ) {
	return tree->FindIntersections( origin, direction, pick, found, object, stamp );
}
