// NiOBBNode.cpp -- a node of a shape's bounding box tree

#include "NiRTLib.h"
#include <math.h>
#include "NiOBBNode.h"
#include "NiOBBLeaf.h"
#include "NiOBBRoot.h"
#include "NiAVObject.h"
#include "NiCollisionRecord.h"
#include "NiDynamicEffect.h"
#include "NiGeometry.h"
#include "NiGeometryData.h"
#include "NiNode.h"
#include "NiTriBasedGeom.h"
#include "NiTriBasedGeomData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00829680 _$E219
// FUNCTION: STBC 0x00829620 _$E17
// FUNCTION: STBC 0x00829640 _$E15
// FUNCTION: STBC 0x00829650 _$E23
// FUNCTION: STBC 0x00829670 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x008296a0 _$E30
// FUNCTION: STBC 0x008296b0 _$E41
// FUNCTION: STBC 0x008296c0 _$E45
// FUNCTION: STBC 0x008296d0 _$E49
// FUNCTION: STBC 0x008296e0 _$E53
// FUNCTION: STBC 0x008296f0 _$E177
// FUNCTION: STBC 0x00829700 _$E184
// FUNCTION: STBC 0x00829710 _$E203

/*
================
NiOBBNode::NiOBBNode

Measures the area and centroid of every triangle and builds the tree over
all of them.
================
*/
// FUNCTION: STBC 0x828030
// Gamebryo 1.2.2: NiOBBNode::NiOBBNode
NiOBBNode::NiOBBNode( NiOBBRoot *root, unsigned short triangleCount, const unsigned short *triList,
					  const NiPoint3 *vertices, const NiPoint3 *worldVertices, int binSize ) {
	float *areas = new float[triangleCount];
	NiPoint3 *centroids = new NiPoint3[triangleCount];

	unsigned short i, j;
	for ( i = 0, j = 0; i < triangleCount; i++ ) {
		unsigned short index0 = triList[j++];
		unsigned short index1 = triList[j++];
		unsigned short index2 = triList[j++];
		NiPoint3 edge1 = vertices[index1] - vertices[index0];
		NiPoint3 edge2 = vertices[index2] - vertices[index0];
		NiPoint3 cross = edge1.Cross( edge2 );
		areas[i] = 0.5f * cross.Length();
		centroids[i] = ( vertices[index0] + vertices[index1] + vertices[index2] ) / 3.0f;
	}

	int *indices = new int[triangleCount];
	for ( int k = 0; k < triangleCount; k++ ) {
		indices[k] = k;
	}
	int *scratch = new int[triangleCount];

	CreateRecursive( root, triList, vertices, worldVertices, areas, centroids, 0, triangleCount - 1, indices, scratch, binSize );

	delete [] scratch;
	delete [] indices;
	delete [] centroids;
	delete [] areas;
	stamp = 0;
}

/*
================
NiOBBNode::~NiOBBNode

A node has both subtrees or neither.
================
*/
// FUNCTION: STBC 0x8282C0
NiOBBNode::~NiOBBNode() {
	if ( left ) {
		delete left;
		delete right;
	}
}

/*
================
NiOBBNode::CreateRecursive

A range smaller than the bin size ends the tree; a half of one triangle is
a leaf.
================
*/
// FUNCTION: STBC 0x8282F0
void NiOBBNode::CreateRecursive( NiOBBRoot *root, const unsigned short *triList, const NiPoint3 *vertices,
					   const NiPoint3 *worldVertices, const float *areas, const NiPoint3 *centroids, int i0, int i1,
					   int *indices, int *scratch, int binSize ) {
	ComputeOBB( root, triList, vertices, areas, centroids, i0, i1, indices );

	if ( i1 - i0 < binSize ) {
		left = 0;
		right = 0;
		return;
	}

	int j0, j1;
	DivideTriangles( centroids, i0, i1, indices, j0, j1, scratch );

	if ( i0 < j0 ) {
		left = new NiOBBNode;
		left->CreateRecursive( root, triList, vertices, worldVertices, areas, centroids, i0, j0, scratch, indices, binSize );
	} else if ( i0 == j0 ) {
		left = new NiOBBLeaf( root, triList, vertices, worldVertices, scratch[i0] );
	}

	if ( j1 < i1 ) {
		right = new NiOBBNode;
		right->CreateRecursive( root, triList, vertices, worldVertices, areas, centroids, j1, i1, scratch, indices, binSize );
	} else if ( j1 == i1 ) {
		right = new NiOBBLeaf( root, triList, vertices, worldVertices, scratch[i1] );
	}
}

/*
================
NiOBBNode::ComputeOBB
================
*/
// FUNCTION: STBC 0x828500
// Gamebryo 1.2.2: NiOBBNode::ComputeOBB
void NiOBBNode::ComputeOBB( NiOBBRoot *root, const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
						const NiPoint3 *centroids, int i0, int i1, const int *indices ) {
	NiPoint3 &center = box.center;
	NiPoint3 *axis = box.axis;
	float *extent = box.extent;

	NiMatrix3 covariance = ComputeCovarianceAndMean( triList, vertices, areas, centroids, i0, i1, indices );
	float eigenvalue[3];
	covariance.EigenSolveSymmetric( eigenvalue, axis );

	NiPoint3 lower( 0.0f, 0.0f, 0.0f );
	NiPoint3 upper( 0.0f, 0.0f, 0.0f );
	// RULE-EXCEPTION: R35, the extents' if/else bodies stay without braces (Gamebryo 1.2.2's form): with
	// braces VC6 leaves the axis terms of the center offset to an out-of-line NiPoint3 constructor (89.0);
	// line breaks alone change nothing
	for ( unsigned short i = i0; i <= i1; i++ ) {
		int triangle = indices[i];
		int j = 3 * triangle;
		const NiPoint3 &v0 = vertices[triList[j++]];
		const NiPoint3 &v1 = vertices[triList[j++]];
		const NiPoint3 &v2 = vertices[triList[j++]];

		NiPoint3 diff = v0 - center;
		float x, y, z;

		x = axis[0].Dot( diff );
		y = axis[1].Dot( diff );
		z = axis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;

		diff = v1 - center;
		x = axis[0].Dot( diff );
		y = axis[1].Dot( diff );
		z = axis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;

		diff = v2 - center;
		x = axis[0].Dot( diff );
		y = axis[1].Dot( diff );
		z = axis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;
	}

	NiPoint3 halfExtent = 0.5f * ( upper - lower );
	extent[0] = halfExtent.x;
	extent[1] = halfExtent.y;
	extent[2] = halfExtent.z;

	NiPoint3 middle = 0.5f * ( upper + lower );
	center += axis[0] * middle.x + axis[1] * middle.y + axis[2] * middle.z;

	box.Transform( NiMatrix3::IDENTITY, NiPoint3::ZERO, 1.0f );
	box.SetRoot( root );
}

/*
================
NiOBBNode::ComputeCovarianceAndMean

The covariance of each triangle is taken over its vertices and centroid.
================
*/
// FUNCTION: STBC 0x828A40
// Gamebryo 1.2.2: NiOBBNode::ComputeCovarianceAndMean
NiMatrix3 NiOBBNode::ComputeCovarianceAndMean( const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
										const NiPoint3 *centroids, int i0, int i1, const int *indices ) {
	NiPoint3 sum( 0.0f, 0.0f, 0.0f );
	float totalArea = 0.0f;
	float sumProducts[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };	// xx, xy, xz, yy, yz, zz

	for ( unsigned short i = i0; i <= i1; i++ ) {
		int triangle = indices[i];
		int j = 3 * triangle;
		const NiPoint3 &v0 = vertices[triList[j++]];
		const NiPoint3 &v1 = vertices[triList[j++]];
		const NiPoint3 &v2 = vertices[triList[j++]];
		const NiPoint3 &centroid = centroids[triangle];
		float area = areas[triangle];

		totalArea += area;
		sum += area * centroid;
		sumProducts[0] += area * ( centroid.x * centroid.x + v0.x * v0.x + v1.x * v1.x + v2.x * v2.x );
		sumProducts[1] += area * ( centroid.x * centroid.y + v0.x * v0.y + v1.x * v1.y + v2.x * v2.y );
		sumProducts[2] += area * ( centroid.x * centroid.z + v0.x * v0.z + v1.x * v1.z + v2.x * v2.z );
		sumProducts[3] += area * ( centroid.y * centroid.y + v0.y * v0.y + v1.y * v1.y + v2.y * v2.y );
		sumProducts[4] += area * ( centroid.y * centroid.z + v0.y * v0.z + v1.y * v1.z + v2.y * v2.z );
		sumProducts[5] += area * ( centroid.z * centroid.z + v0.z * v0.z + v1.z * v1.z + v2.z * v2.z );
	}

	float invTotalArea = 1.0f / totalArea;
	float scale = 0.25f * invTotalArea;

	NiPoint3 &center = box.center;
	center = invTotalArea * sum;

	NiMatrix3 covariance;
	covariance.SetEntry( 0, 0, sumProducts[0] * scale - center.x * center.x );
	covariance.SetEntry( 0, 1, sumProducts[1] * scale - center.x * center.y );
	covariance.SetEntry( 0, 2, sumProducts[2] * scale - center.x * center.z );
	covariance.SetEntry( 1, 1, sumProducts[3] * scale - center.y * center.y );
	covariance.SetEntry( 1, 2, sumProducts[4] * scale - center.y * center.z );
	covariance.SetEntry( 2, 2, sumProducts[5] * scale - center.z * center.z );
	covariance.SetEntry( 1, 0, covariance.GetEntry( 0, 1 ) );
	covariance.SetEntry( 2, 0, covariance.GetEntry( 0, 2 ) );
	covariance.SetEntry( 2, 1, covariance.GetEntry( 1, 2 ) );
	return covariance;
}

/*
================
NiOBBNode::DivideTriangles

A centroid on the center's plane goes to the half its position in the
range falls in. The name is Gamebryo 1.1.2's (NiOBBNode.h).
================
*/
// FUNCTION: STBC 0x828D90
// Gamebryo 1.2.2: NiOBBNode::DivideTriangles
void NiOBBNode::DivideTriangles( const NiPoint3 *centroids, int i0, int i1, const int *indices, int &j0, int &j1, int *scratch ) {
	int i;
	int middle = ( i0 + i1 ) / 2;

	j0 = i0 - 1;
	j1 = i1 + 1;

	const NiPoint3 &center = box.center;
	NiPoint3 *axis = box.axis;
	int axisIndex;
	for ( axisIndex = 0; axisIndex < 3 && ( j0 < i0 || j1 > i1 ); axisIndex++ ) {
		// try splitting along this axis
		j0 = i0 - 1;
		j1 = i1 + 1;

		for ( i = i0; i <= i1; i++ ) {
			int triangle = indices[i];

			float distance = axis[axisIndex].Dot( centroids[triangle] - center );
			if ( distance > 0.0f ) {
				scratch[++j0] = triangle;
			} else if ( distance < 0.0f ) {
				scratch[--j1] = triangle;
			} else {
				// on the splitting plane: by its place in the range
				if ( i <= middle ) {
					scratch[++j0] = triangle;
				} else {
					scratch[--j1] = triangle;
				}
			}
		}
	}

	if ( axisIndex == 3 && ( j0 < i0 || j1 > i1 ) ) {
		// no axis separates the triangles: halve the range
		j0 = middle;
		j1 = middle + 1;
		for ( i = i0; i <= i1; i++ ) {
			scratch[i] = indices[i];
		}
	}
}

/*
================
NiOBBNode::Transform
================
*/
// FUNCTION: STBC 0x828EF0
void NiOBBNode::Transform( const NiMatrix3 &rotate, const NiPoint3 &translate, float scale ) {
	box.Transform( rotate, translate, scale );
}

/*
================
NiOBBNode::CollisionCallback

Each object's callback sees the collision from its own side.
================
*/
// FUNCTION: STBC 0x828F10
int NiOBBNode::CollisionCallback( NiCollisionIntersect &intersect ) {
	int result0 = 0;
	int result1 = 0;

	if ( intersect.root0 && intersect.root0->GetCollideCallback() ) {
		result0 = intersect.root0->GetCollideCallback()( intersect );
	}

	if ( intersect.root1 ) {
		NiAVObject::CollideCallback callback = intersect.root1->GetCollideCallback();
		if ( callback ) {
			NiAVObject *root = intersect.root0;
			intersect.root0 = intersect.root1;
			intersect.root1 = root;
			NiPoint3 normal = intersect.normal0;
			intersect.normal0 = intersect.normal1;
			intersect.normal1 = normal;
			result1 = callback( intersect );
		}
	}

	if ( result0 || result1 ) {
		return 1;
	}
	return 0;
}

/*
================
NiOBBNode::BoxBoxCollision

Boxes at rest meet at the middle of their centers.
================
*/
// FUNCTION: STBC 0x828FB0
// Gamebryo 1.2.2: NiOBBNode::BoxBoxCollision
int NiOBBNode::BoxBoxCollision( NiOBBNode *other, NiAVObject *root0, NiAVObject *root1, NiAVObject *object0,
						   NiAVObject *object1 ) {
	NiCollisionIntersect intersect;
	intersect.root0 = root0;
	intersect.root1 = root1;
	intersect.object0 = object0;
	intersect.object1 = object1;

	NiPoint3 velocity0 = box.GetRoot()->GetVelocity();
	NiPoint3 velocity1 = other->box.GetRoot()->GetVelocity();
	int intersects = 0;
	if ( velocity0 == NiPoint3::ZERO && velocity1 == NiPoint3::ZERO ) {
		// boxes at rest
		const NiPoint3 &center0 = box.worldCenter;
		const NiPoint3 &center1 = other->box.worldCenter;
		intersects = 1;
		intersect.time = 0.0f;
		intersect.point = 0.5f * ( center0 + center1 );
	} else {
		intersects = box.FindIntersection( other->box, intersect.time, intersect.point );
	}

	if ( intersects ) {
		box.GetNormal( intersect.point, intersect.normal0 );
		other->box.GetNormal( intersect.point, intersect.normal1 );
		return CollisionCallback( intersect );
	}
	return 0;
}

/*
================
NiOBBNode::TestCollision
================
*/
// FUNCTION: STBC 0x829130
// Gamebryo 1.2.2: NiOBBNode::TestCollision
bool NiOBBNode::TestCollision( NiOBBNode *other, int depth, int otherDepth ) {
	if ( !other ) {
		return false;
	}
	if ( !box.TestIntersection( other->box ) ) {
		return false;
	}
	if ( !depth || !otherDepth ) {
		return true;
	}

	if ( IsLeaf() ) {
		if ( other->IsLeaf() || !other->HasChildren() ) {
			return true;
		} else {
			int otherDepthLess = otherDepth - 1;
			return TestCollision( other->left, depth, otherDepthLess ) || TestCollision( other->right, depth, otherDepthLess );
		}
	}

	if ( other->IsLeaf() ) {
		if ( left && right ) {
			int depthLess = depth - 1;
			return left->TestCollision( other, depthLess, otherDepth ) || right->TestCollision( other, depthLess, otherDepth );
		}
	} else {
		if ( left && right ) {
			int depthLess = depth - 1;
			int otherDepthLess = otherDepth - 1;
			return left->TestCollision( other->left, depthLess, otherDepthLess )
				|| left->TestCollision( other->right, depthLess, otherDepthLess )
				|| right->TestCollision( other->left, depthLess, otherDepthLess )
				|| right->TestCollision( other->right, depthLess, otherDepthLess );
		}
	}

	return true;
}

/*
================
NiOBBNode::FindCollisions

Descends the tree that is not yet a leaf; at the depth limits the boxes
themselves are the contact.
================
*/
// FUNCTION: STBC 0x8292B0
// Gamebryo 1.2.2: NiOBBNode::FindCollisions
int NiOBBNode::FindCollisions( NiOBBNode *other, NiAVObject *root0, NiAVObject *root1, NiAVObject *object0,
							   NiAVObject *object1, int depth, int otherDepth, unsigned int stamp_,
							   unsigned int otherStamp ) {
	if ( !other ) {
		return CONTINUE_COLLISIONS;
	}

	if ( stamp_ != stamp ) {
		box.Transform( object0->GetWorldRotate(), object0->GetWorldTranslate(), object0->GetWorldScale() );
		stamp = stamp_;
	}

	if ( otherStamp != other->GetChangeStamp() ) {
		other->TransformBox( object1->GetWorldRotate(), object1->GetWorldTranslate(), object1->GetWorldScale() );
		other->SetChangeStamp( otherStamp );
	}

	if ( !box.TestIntersection( other->box ) ) {
		return CONTINUE_COLLISIONS;
	}
	if ( depth == 0 || otherDepth == 0 ) {
		return BoxBoxCollision( other, root0, root1, object0, object1 );
	}

	int result;

	if ( IsLeaf() ) {
		if ( other->IsLeaf() || !other->HasChildren() ) {
			result = BoxBoxCollision( other, root0, root1, object0, object1 );
			if ( result == TERMINATE_COLLISIONS ) {
				return result;
			}
		} else {
			int otherDepthLess = otherDepth - 1;
			result = FindCollisions( other->left, root0, root1, object0, object1, depth, otherDepthLess, stamp_, otherStamp );
			if ( result == TERMINATE_COLLISIONS ) {
				return result;
			}
			result = FindCollisions( other->right, root0, root1, object0, object1, depth, otherDepthLess, stamp_, otherStamp );
			if ( result == TERMINATE_COLLISIONS ) {
				return result;
			}
		}
	} else {
		if ( other->IsLeaf() ) {
			if ( left && right ) {
				int depthLess = depth - 1;
				result = left->FindCollisions( other, root0, root1, object0, object1, depthLess, otherDepth, stamp_, otherStamp );
				if ( result == TERMINATE_COLLISIONS ) {
					return result;
				}
				result = right->FindCollisions( other, root0, root1, object0, object1, depthLess, otherDepth, stamp_, otherStamp );
				if ( result == TERMINATE_COLLISIONS ) {
					return result;
				}
			} else {
				return BoxBoxCollision( other, root0, root1, object0, object1 );
			}
		} else {
			// descend this tree to its leaves before the other
			int depthLess = depth - 1;
			if ( left && right ) {
				result = left->FindCollisions( other, root0, root1, object0, object1, depthLess, otherDepth, stamp_, otherStamp );
				if ( result == TERMINATE_COLLISIONS ) {
					return result;
				}
				result = right->FindCollisions( other, root0, root1, object0, object1, depthLess, otherDepth, stamp_, otherStamp );
				if ( result == TERMINATE_COLLISIONS ) {
					return result;
				}
			} else {
				return BoxBoxCollision( other, root0, root1, object0, object1 );
			}
		}
	}

	return CONTINUE_COLLISIONS;
}

/*
================
NiOBBNode::FindIntersections

Stops when a subtree asks to.
================
*/
// FUNCTION: STBC 0x829560
// Gamebryo 1.2.2: NiOBBNode::FindIntersections
int NiOBBNode::FindIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, bool &found,
								  NiAVObject *object, unsigned int stamp_ ) {
	if ( stamp_ != stamp ) {
		box.Transform( object->GetWorldRotate(), object->GetWorldTranslate(), object->GetWorldScale() );
		stamp = stamp_;
	}

	if ( !box.TestRay( origin, direction ) ) {
		return CONTINUE_COLLISIONS;
	}

	int result;

	if ( left ) {
		result = left->FindIntersections( origin, direction, pick, found, object, stamp_ );
		if ( result == TERMINATE_COLLISIONS ) {
			return result;
		}
	}
	if ( right ) {
		result = right->FindIntersections( origin, direction, pick, found, object, stamp_ );
		if ( result == TERMINATE_COLLISIONS ) {
			return result;
		}
	}

	return CONTINUE_COLLISIONS;
}
