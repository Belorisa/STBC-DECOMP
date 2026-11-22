/*
===============================================================================

	TGModelUtils.h

	The scripting helpers that carry a point or a vector between the frame
	of a scene node and the world, or between two nodes' frames: the App
	methods TGModelUtils_LocalToWorldPoint and its eight siblings. A point
	is scaled, rotated and moved by the node's world transform; a vector is
	scaled and rotated only; a unit vector is only rotated. The inverse
	rotates by the transposed matrix (point * matrix) and divides by the
	scale. The class is the SWIG name's prefix; its members are inline in
	the wrapper unit, as the wrappers' bytes show.

===============================================================================
*/

#ifndef __TGMODELUTILS_H__
#define __TGMODELUTILS_H__

#include "NiAVObject.h"

class NiNode;

class TGModelUtils {
public:
	// The node as a scene object (the SWIG wrapper
	// TGModelUtils_CastNodeToAVObject); out of line, among the model helpers.
	static NiAVObject *	CastNodeToAVObject( NiNode *node );
	// Out of line in retail after ObjectClass::GetRandomPointOnModel, its first caller; OBBNode and OBBLeaf call
	// the second copy too.
	// FUNCTION: STBC 0x004358f0 ?LocalToWorldPoint@TGModelUtils@@SA?AVNiPoint3@@PAVNiAVObject@@ABV2@@Z @game/ObjectClass
	static NiPoint3	LocalToWorldPoint( NiAVObject *node, const NiPoint3 &point ) {
		return node->GetWorldRotate() * ( point * node->GetWorldScale() ) + node->GetWorldTranslate();
	}
	// FUNCTION: STBC 0x00435960 ?WorldToLocalPoint@TGModelUtils@@SA?AVNiPoint3@@PAVNiAVObject@@ABV2@@Z @game/ObjectClass
	static NiPoint3	WorldToLocalPoint( NiAVObject *node, const NiPoint3 &point ) {
		return ( ( point - node->GetWorldTranslate() ) * node->GetWorldRotate() ) * ( 1.0f / node->GetWorldScale() );
	}
	static NiPoint3	LocalToLocalPoint( NiAVObject *from, NiAVObject *to, const NiPoint3 &point ) {
		return WorldToLocalPoint( to, LocalToWorldPoint( from, point ) );
	}
	static NiPoint3	LocalToWorldVector( NiAVObject *node, const NiPoint3 &vector ) {
		return node->GetWorldRotate() * ( vector * node->GetWorldScale() );
	}
	static NiPoint3	WorldToLocalVector( NiAVObject *node, const NiPoint3 &vector ) {
		return ( vector * node->GetWorldRotate() ) * ( 1.0f / node->GetWorldScale() );
	}
	static NiPoint3	LocalToLocalVector( NiAVObject *from, NiAVObject *to, const NiPoint3 &vector ) {
		return WorldToLocalVector( to, LocalToWorldVector( from, vector ) );
	}
	static NiPoint3	LocalToWorldUnitVector( NiAVObject *node, const NiPoint3 &vector ) {
		return node->GetWorldRotate() * vector;
	}
	static NiPoint3	WorldToLocalUnitVector( NiAVObject *node, const NiPoint3 &vector ) {
		return vector * node->GetWorldRotate();
	}
	static NiPoint3	LocalToLocalUnitVector( NiAVObject *from, NiAVObject *to, const NiPoint3 &vector ) {
		return ( from->GetWorldRotate() * vector ) * to->GetWorldRotate();
	}
};

#endif /* !__TGMODELUTILS_H__ */
