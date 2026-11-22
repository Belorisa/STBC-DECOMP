/*
===============================================================================

	NiPick.h

	A ray pick through the scene graph: how the objects hit are found and
	reported, and the results. The class and member names follow the
	Gamebryo lineage, which keeps the class (NiPick: m_ePickType,
	m_eSortType, m_eIntersectType, m_eCoordinateType, m_bFrontOnly,
	m_bObserveAppCullFlag); no string in this binary names it.

	Evidence: the scene-graph objects' slot 34 takes the ray's origin and
	direction and the pick. NiNode's (0x7E4D30) and NiCollisionSwitch's
	(0x7F8EB0) return false at once when the byte at 0x11 is set and the
	object is app-culled; NiSwitchNode's (0x7FC5A0) adds a 0x44-byte result
	for the node itself when the word at 8 is 0 and the word at 0 is 1,
	into the array at 0x18, keeping it at 0x30, and stops early when the
	words at 0 and 4 are 1 and the array holds a result.

	The result (0x44 bytes; Gamebryo's NiPick::Record, whose layout it
	keeps) as NiNode's slot 34 (0x7E4D30) and NiTriBasedGeom's (0x7EF600)
	build it: smart pointers at 0x00 and 0x04 (see Record), and at 0x34 a
	colour built through NiColorA's constructor (0x46C280) with four zeros.
	The results array adds through SetAtGrow (0x7E6560) and AddFirstEmpty;
	the ray first meets the world bound in NiAVObject::TestRayBound
	(0x7DD170). The byte at 0x12 selects the collision data test in
	NiTriBasedGeom's. NiOBBLeaf's ray test (0x827C50) passes the byte at
	0x10 to the ray-triangle test, stores the hit's point at 0x08 and its
	distance at 0x14 of the result, and its face normal at 0x28 when the
	byte at 0x35 is set.

===============================================================================
*/

#ifndef __NIPICK_H__
#define __NIPICK_H__

#include "NiTArray.h"
#include "NiPointer.h"
#include "NiPoint2.h"
#include "NiPoint3.h"
#include "NiColor.h"

class NiAVObject;
class NiNode;

class NiPick {
public:
	enum PickType {
		FIND_ALL,
		FIND_FIRST
	};

	enum SortType {
		SORT,
		NO_SORT
	};

	enum IntersectType {
		BOUND_INTERSECT,
		TRIANGLE_INTERSECT
	};

	enum CoordinateType {
		MODEL_COORDINATES,
		WORLD_COORDINATES
	};

	/*
	================
	NiPick::Record

	One object the ray hit. The two smart pointers have distinct types
	(each has its own out-of-line constructor and assignment copies): the
	first is NiPointer<NiAVObject>, whose constructor (0x46C4C0) NiNode's
	children array also uses (NiBSPNode::CreateClone's inlined SetSize);
	the second's assignment (0x416FA0) is what SetClass uses to hand its
	NiPointer<NiNode> root to a camera, so it is taken as NiPointer<NiNode>.
	================
	*/
	// FUNCTION: STBC 0x464B50 ??1Record@NiPick@@QAE@XZ @game/OBBRoot
	class Record {
	public:
								Record( NiAVObject *object_ ) { object = object_; proxyParent = 0; }

		void					SetIntersection( const NiPoint3 &intersect ) { intersection = intersect; }
		void					SetDistance( float distance_ ) { distance = distance_; }
		float					GetDistance() const { return distance; }
		void					SetNormal( const NiPoint3 &normal_ ) { normal = normal_; }
		// Gamebryo 1.2.2: NiPick::Record's setters
		void					SetTriangleIndex( unsigned short index ) { triangleIndex = index; }
		void					SetVertexIndices( unsigned short index0, unsigned short index1, unsigned short index2 ) {
									vertexIndex[0] = index0;
									vertexIndex[1] = index1;
									vertexIndex[2] = index2;
								}
		void					SetTexture( const NiPoint2 &texture_ ) { texture = texture_; }
		void					SetColor( const NiColorA &color_ ) { color = color_; }

		NiPointer<NiAVObject>	object;			// 0x00 the object hit
		NiPointer<NiNode>		proxyParent;	// 0x04 NULL from the scene-graph objects
		NiPoint3				intersection;	// 0x08
		float					distance;		// 0x14 along the ray, in ray direction lengths
		unsigned short			triangleIndex;	// 0x18
		unsigned short			vertexIndex[3];	// 0x1A
		NiPoint2				texture;		// 0x20
		NiPoint3				normal;			// 0x28
		NiColorA				color;			// 0x34
	};

	typedef NiTArray<Record *>	Results;

							NiPick();
							~NiPick();

	// deletes every record and empties the results (Gamebryo lineage name)
	void					ClearResultsArray();

	// picks along the ray from origin in direction dir through the root's
	// subtree; the results are sorted nearest first, and a FIND_FIRST pick
	// keeps only the nearest. False when nothing was hit (Gamebryo lineage
	// name).
	bool					PickObjects( const NiPoint3 &origin, const NiPoint3 &dir, bool append );

	PickType				GetPickType() const { return pickType; }
	SortType				GetSortType() const { return sortType; }
	IntersectType			GetIntersectType() const { return intersectType; }
	bool					GetObserveAppCullFlag() const { return observeAppCullFlag; }
	bool					GetFrontOnly() const { return frontOnly; }
	bool					GetReturnNormal() const { return returnNormal; }
	CoordinateType			GetCoordinateType() const { return coordinateType; }
	bool					GetReturnTexture() const { return returnTexture; }
	bool					GetReturnSmoothNormal() const { return returnSmoothNormal; }
	bool					GetReturnColor() const { return returnColor; }
	// TODO identity: NiTriBasedGeom's slot 34 picks through the bounding box
	// tree when this is set
	bool					GetUnknown12() const { return unknown12; }
	// the hits (Gamebryo 1.1.2: NiPick::GetResults)
	Results &				GetResults() { return results; }
	// the hit at the index, nearest first when the pick sorts (Gamebryo lineage name)
	Record *				GetRecord( unsigned int index ) const { return results.GetAt( index ); }

	// setters (Gamebryo lineage names)
	void					SetPickType( PickType type ) { pickType = type; }
	void					SetSortType( SortType type ) { sortType = type; }
	void					SetIntersectType( IntersectType type ) { intersectType = type; }
	void					SetCoordinateType( CoordinateType type ) { coordinateType = type; }
	void					SetFrontOnly( bool front ) { frontOnly = front; }
	void					SetObserveAppCullFlag( bool observe ) { observeAppCullFlag = observe; }
	void					SetReturnTexture( bool texture ) { returnTexture = texture; }
	void					SetReturnNormal( bool normal ) { returnNormal = normal; }
	void					SetReturnSmoothNormal( bool smooth ) { returnSmoothNormal = smooth; }
	void					SetReturnColor( bool color ) { returnColor = color; }
	void					SetTarget( NiAVObject *root_ ) { root = root_; }

	// the number of hits (Gamebryo 1.1.2: NIPICK.INL, NiPick::GetSize)
	unsigned int			GetSize() const { return results.GetEffectiveSize(); }

	// files a hit; the caller creates the record (VC6 does not inline a
	// function whose new-expression needs an unwind state, and retail
	// inlines this one)
	void					Add( Record *record ) {
								lastAdded = record;
								results.AddFirstEmpty( record );
							}

protected:
	PickType				pickType;			// 0x00
	SortType				sortType;			// 0x04
	IntersectType			intersectType;		// 0x08
	CoordinateType			coordinateType;		// 0x0C
	bool					frontOnly;			// 0x10
	bool					observeAppCullFlag;	// 0x11 culled objects are not picked
	bool					unknown12;			// 0x12 selects NiTriBasedGeom's collision data test
	NiPointer<NiAVObject>	root;				// 0x14 the picked subtree (Gamebryo's m_spRoot)
	NiTArray<Record *>		results;			// 0x18
	Record *				lastAdded;			// 0x30
	bool					returnTexture;		// 0x34
	bool					returnNormal;		// 0x35 the triangle hits record the face normal
	bool					returnSmoothNormal;	// 0x36 (Gamebryo lineage name)
	bool					returnColor;		// 0x37 (Gamebryo lineage name)
};

#endif /* !__NIPICK_H__ */
