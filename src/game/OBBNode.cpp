/*
===============================================================================

	OBBNode.cpp

===============================================================================
*/

#include "OBBNode.h"
#include "OBBLeaf.h"
#include "NiPoint3.h"
#include "TGModelUtils.h"
#include "NiPick.h"
#include "NiPlane.h"
#include "NiTriShapeData.h"
#include "ObjectClass.h"
#include "VisibleDamage.h"
#include "CollisionRecorder.h"
#include "DamageBitArray.h"
#include "TGModelUtils.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

// the tolerance of the leaf's ray and segment tests (invented name)
#define EPSILON		1e-5f

// the leaf pairs the tree tests have reached (invented name; nothing reads it)
// DATA: STBC 0x00981374
int obbLeafTests;

/*
================
OBBNode::OBBNode

Measures the area and centroid of every triangle of the shape and builds
the tree over all of them.
================
*/
// FUNCTION: STBC 0x004654f0 ??0OBBNode@@QAE@PAVNiTriShape@@HPAGPAVNiPlane@@@Z
OBBNode::OBBNode( NiTriShape *shape, int leafSize, unsigned short *indices, NiPlane *planes ) {
	left = NULL;
	right = NULL;

	unsigned short triangleCount = shape->GetShapeData()->GetTriangleCount();
	float *areas = new float[triangleCount];
	NiPoint3 *centroids = new NiPoint3[triangleCount];

	const unsigned short *triList = shape->GetTriList();
	const NiPoint3 *vertices = shape->GetShapeData()->GetVertices();
	unsigned short i, j;
	for ( i = 0, j = 0; i < triangleCount; i++ ) {
		unsigned short index0 = triList[j++];
		unsigned short index1 = triList[j++];
		unsigned short index2 = triList[j++];
		NiPoint3 edge1 = vertices[index1] - vertices[index0];
		NiPoint3 edge2 = vertices[index2] - vertices[index0];
		NiPoint3 cross = edge1.Cross( edge2 );
		areas[i] = cross.Length() * 0.5f;
		centroids[i] = ( vertices[index0] + vertices[index1] + vertices[index2] ) * ( 1.0f / 3.0f );
	}

	unsigned short *order = new unsigned short[triangleCount];
	for ( int k = 0; k < triangleCount; k++ ) {
		order[k] = k;
	}
	unsigned short *scratch = new unsigned short[triangleCount];
	ProjectionInfo *keys = new ProjectionInfo[triangleCount];

	CreateRecursive( shape->GetTriList(), shape->GetShapeData()->GetVertices(), areas, centroids, 0,
					 triangleCount - 1, order, scratch, leafSize, indices, planes, keys );

	delete [] keys;
	delete [] scratch;
	delete [] order;
	delete [] centroids;
	delete [] areas;
}

/*
================
OBBNode::OBBNode
================
*/
// FUNCTION: STBC 0x004657d0 ??0OBBNode@@QAE@XZ
OBBNode::OBBNode() : left( NULL ), right( NULL ) {
}

/*
================
OBBNode::~OBBNode
================
*/
// FUNCTION: STBC 0x004657f0
// FUNCTION: STBC 0x004657b0 ??_GOBBNode@@UAEPAXI@Z
OBBNode::~OBBNode() {
	delete left;
	delete right;
}

/*
================
OBBNode::CreateRecursive

Fits the node's box, splits its triangles in two halves and gives each
half a subnode, or a leaf when it is shorter than the leaf size. The
leaves take the tree's index and plane arrays in order.
================
*/
// FUNCTION: STBC 0x00465820
void OBBNode::CreateRecursive( const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
							   const NiPoint3 *centroids, int i0, int i1, unsigned short *order, unsigned short *scratch,
							   int leafSize, unsigned short *indices, NiPlane *planes, ProjectionInfo *keys ) {
	ComputeOBB( triList, vertices, areas, centroids, i0, i1, order );

	int j0, j1;
	DivideTriangles( centroids, i0, i1, order, j0, j1, scratch, keys );

	int count = j0 - i0 + 1;
	if ( count >= leafSize ) {
		left = new OBBNode;
		left->CreateRecursive( triList, vertices, areas, centroids, i0, j0, scratch, order, leafSize, indices, planes, keys );
	} else if ( count > 0 ) {
		left = new OBBLeaf( triList, vertices, areas, centroids, i0, j0, scratch, indices, planes );
	}
	indices += count;
	planes += count;

	count = i1 - j1 + 1;
	if ( count >= leafSize ) {
		right = new OBBNode;
		right->CreateRecursive( triList, vertices, areas, centroids, j1, i1, scratch, order, leafSize, indices, planes, keys );
	} else if ( count > 0 ) {
		right = new OBBLeaf( triList, vertices, areas, centroids, j1, i1, scratch, indices, planes );
	}
}

/*
================
OBBNode::ComputeOBB

The box's axes are the eigenvectors of the triangles' covariance; the
extents are those of the vertices projected on them, and the center moves
to the middle of the projections.
================
*/
// FUNCTION: STBC 0x00465a30
void OBBNode::ComputeOBB( const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
						  const NiPoint3 *centroids, int i0, int i1, const unsigned short *order ) {
	NiPoint3 &boxCenter = center;
	NiPoint3 *boxAxis = axis;
	float *boxExtent = extent;

	NiMatrix3 covariance = ComputeCovarianceAndMean( triList, vertices, areas, centroids, i0, i1, order );
	float eigenvalue[3];
	covariance.EigenSolveSymmetric( eigenvalue, boxAxis );

	NiPoint3 lower( 0.0f, 0.0f, 0.0f );
	NiPoint3 upper( 0.0f, 0.0f, 0.0f );
	// RULE-EXCEPTION: R35, one-line extent updates as in NiOBBNode::ComputeOBB
	for ( unsigned short i = i0; i <= i1; i++ ) {
		int triangle = order[i];
		int j = 3 * triangle;
		const NiPoint3 &v0 = vertices[triList[j++]];
		const NiPoint3 &v1 = vertices[triList[j++]];
		const NiPoint3 &v2 = vertices[triList[j++]];

		NiPoint3 diff = v0 - boxCenter;
		float x, y, z;

		x = boxAxis[0].Dot( diff );
		y = boxAxis[1].Dot( diff );
		z = boxAxis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;

		diff = v1 - boxCenter;
		x = boxAxis[0].Dot( diff );
		y = boxAxis[1].Dot( diff );
		z = boxAxis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;

		diff = v2 - boxCenter;
		x = boxAxis[0].Dot( diff );
		y = boxAxis[1].Dot( diff );
		z = boxAxis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;
	}

	NiPoint3 halfExtent = 0.5f * ( upper - lower );
	boxExtent[0] = halfExtent.x;
	boxExtent[1] = halfExtent.y;
	boxExtent[2] = halfExtent.z;

	NiPoint3 middle = 0.5f * ( upper + lower );
	boxCenter += boxAxis[0] * middle.x + boxAxis[1] * middle.y + boxAxis[2] * middle.z;
}

/*
================
OBBNode::ComputeCovarianceAndMean

The covariance of each triangle is taken over its vertices and centroid,
weighted by its area; the mean becomes the box's center.
================
*/
// FUNCTION: STBC 0x00465f60
NiMatrix3 OBBNode::ComputeCovarianceAndMean( const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
											 const NiPoint3 *centroids, int i0, int i1, const unsigned short *order ) {
	NiPoint3 sum( 0.0f, 0.0f, 0.0f );
	float totalArea = 0.0f;
	float sumProducts[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };	// xx, xy, xz, yy, yz, zz

	for ( unsigned short i = i0; i <= i1; i++ ) {
		int triangle = order[i];
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

	NiPoint3 mean = invTotalArea * sum;
	center = mean;

	NiMatrix3 covariance;
	// BUG: the second row takes yy, yz and zz and the third copies the first row's xy and xz, so the
	// matrix is not the covariance (Gamebryo fills the upper triangle and mirrors it)
	covariance.SetEntry( 0, 0, sumProducts[0] * scale - mean.x * mean.x );
	float xy = sumProducts[1] * scale - mean.x * mean.y;
	covariance.SetEntry( 0, 1, xy );
	float xz = sumProducts[2] * scale - mean.x * mean.z;
	covariance.SetEntry( 0, 2, xz );
	covariance.SetEntry( 1, 0, sumProducts[3] * scale - mean.y * mean.y );
	covariance.SetEntry( 1, 1, sumProducts[4] * scale - mean.y * mean.z );
	float zz = sumProducts[5] * scale - mean.z * mean.z;
	covariance.SetEntry( 1, 2, zz );
	covariance.SetEntry( 2, 0, xy );
	covariance.SetEntry( 2, 1, xz );
	covariance.SetEntry( 2, 2, zz );
	return covariance;
}

/*
================
OBBNode::RayIntersectsBox

The ray misses the box when its origin lies outside a pair of faces and
it points away from them, or when the box's projection on an axis that
crosses the ray's direction with a box axis does not reach the ray.
This is the ray-box test of Eberly's Magic Software library, written as
it is there: each dot product, its absolute value and the right-hand
side of each cross test kept in its own array element or local.
================
*/
// FUNCTION: STBC 0x004662c0
bool OBBNode::RayIntersectsBox( const NiPoint3 &localOrigin, const NiPoint3 &localDirection ) {
	float dirDot[3], absDirDot[3], diffDot[3], absDiffDot[3], absCrossDot[3], rhs;

	NiPoint3 diff = localOrigin - center;

	dirDot[0] = localDirection.Dot( axis[0] );
	absDirDot[0] = (float)fabs( dirDot[0] );
	diffDot[0] = diff.Dot( axis[0] );
	absDiffDot[0] = (float)fabs( diffDot[0] );
	if ( absDiffDot[0] > extent[0] && diffDot[0] * dirDot[0] >= 0.0f ) {
		return false;
	}

	dirDot[1] = localDirection.Dot( axis[1] );
	absDirDot[1] = (float)fabs( dirDot[1] );
	diffDot[1] = diff.Dot( axis[1] );
	absDiffDot[1] = (float)fabs( diffDot[1] );
	if ( absDiffDot[1] > extent[1] && diffDot[1] * dirDot[1] >= 0.0f ) {
		return false;
	}

	dirDot[2] = localDirection.Dot( axis[2] );
	absDirDot[2] = (float)fabs( dirDot[2] );
	diffDot[2] = diff.Dot( axis[2] );
	absDiffDot[2] = (float)fabs( diffDot[2] );
	if ( absDiffDot[2] > extent[2] && diffDot[2] * dirDot[2] >= 0.0f ) {
		return false;
	}

	NiPoint3 cross = localDirection.Cross( diff );

	absCrossDot[0] = (float)fabs( cross.Dot( axis[0] ) );
	rhs = extent[1] * absDirDot[2] + extent[2] * absDirDot[1];
	if ( absCrossDot[0] > rhs ) {
		return false;
	}

	absCrossDot[1] = (float)fabs( cross.Dot( axis[1] ) );
	rhs = extent[0] * absDirDot[2] + extent[2] * absDirDot[0];
	if ( absCrossDot[1] > rhs ) {
		return false;
	}

	absCrossDot[2] = (float)fabs( cross.Dot( axis[2] ) );
	rhs = extent[0] * absDirDot[1] + extent[1] * absDirDot[0];
	if ( absCrossDot[2] > rhs ) {
		return false;
	}

	return true;
}

/*
================
OBBNode::SegmentIntersectsBox

The segment is taken as its midpoint and half its direction: it misses
the box when the midpoint lies farther from a pair of faces than the half
segment reaches, or when the box's projection on an axis that crosses the
segment with a box axis does not reach it. This is the segment-box test
of Eberly's Magic Software library, its face tests a loop as there.
================
*/
// FUNCTION: STBC 0x00466510
bool OBBNode::SegmentIntersectsBox( const NiPoint3 &localOrigin, const NiPoint3 &localDirection ) {
	NiPoint3 halfDirection = localDirection * 0.5f;
	NiPoint3 diff = localOrigin + halfDirection - center;
	float absDirDot[3];
	float absDiffDot[3];
	float rhs;

	for ( int i = 0; i < 3; i++ ) {
		absDirDot[i] = (float)fabs( halfDirection.Dot( axis[i] ) );
		absDiffDot[i] = (float)fabs( diff.Dot( axis[i] ) );
		rhs = extent[i] + absDirDot[i];
		if ( absDiffDot[i] > rhs ) {
			return false;
		}
	}

	NiPoint3 cross = halfDirection.Cross( diff );
	float absCrossDot[3];

	absCrossDot[0] = (float)fabs( cross.Dot( axis[0] ) );
	rhs = extent[1] * absDirDot[2] + extent[2] * absDirDot[1];
	if ( absCrossDot[0] > rhs ) {
		return false;
	}

	absCrossDot[1] = (float)fabs( cross.Dot( axis[1] ) );
	rhs = extent[0] * absDirDot[2] + extent[2] * absDirDot[0];
	if ( absCrossDot[1] > rhs ) {
		return false;
	}

	absCrossDot[2] = (float)fabs( cross.Dot( axis[2] ) );
	rhs = extent[0] * absDirDot[1] + extent[1] * absDirDot[0];
	if ( absCrossDot[2] > rhs ) {
		return false;
	}

	return true;
}

/*
================
OBBNode::Unknown4666E0
================
*/
// FUNCTION: STBC 0x004666e0
bool OBBNode::Unknown4666E0( const NiPoint3 &localCenter, float radiusSquared ) {
	return false;
}

/*
================
OBBNode::SphereIntersectsBox

The squared distance from the center to the box grows on each axis the
center lies beyond; the sphere misses as soon as it passes radiusSquared.
================
*/
// FUNCTION: STBC 0x004666f0
bool OBBNode::SphereIntersectsBox( const NiPoint3 &localCenter, float radiusSquared ) {
	NiPoint3 diff = localCenter - center;
	float distanceSquared = 0.0f;
	float delta;
	float lower;

	delta = diff.Dot( axis[0] );
	lower = -extent[0];
	if ( delta < lower ) {
		delta -= lower;
		distanceSquared += delta * delta;
	} else if ( delta > extent[0] ) {
		delta -= extent[0];
		distanceSquared += delta * delta;
	}
	if ( distanceSquared > radiusSquared ) {
		return false;
	}

	delta = diff.Dot( axis[1] );
	lower = -extent[1];
	if ( delta < lower ) {
		delta -= lower;
		distanceSquared += delta * delta;
	} else if ( delta > extent[1] ) {
		delta -= extent[1];
		distanceSquared += delta * delta;
	}
	if ( distanceSquared > radiusSquared ) {
		return false;
	}

	delta = diff.Dot( axis[2] );
	lower = -extent[2];
	if ( delta < lower ) {
		delta -= lower;
		distanceSquared += delta * delta;
	} else if ( delta > extent[2] ) {
		delta -= extent[2];
		distanceSquared += delta * delta;
	}
	if ( distanceSquared > radiusSquared ) {
		return false;
	}
	return true;
}

/*
================
OBBNode::BoxIntersectsBox

The fifteen separating axes of two boxes: the three axes of each and the
nine cross products of an axis of one with an axis of the other: Eberly's
box-box test (Magic Software), its arrays kept: c the cosines between the
axes, absC their absolute values, ad the offset of the box's center along
the node's axes (retail's 0x60 frame is these three arrays and diff).
================
*/
// FUNCTION: STBC 0x00466850 ?BoxIntersectsBox@OBBNode@@IAE_NABVNiBox@@@Z
// DRAFT: complete; x87 operand order: the dot products load their operands in mixed orders (the commutative sort's symbol numbers); the frame is retail's
bool OBBNode::BoxIntersectsBox( const NiBox &box ) {
	NiPoint3 diff = box.center - center;
	float c[3][3];
	float absC[3][3];
	float ad[3];

	c[0][0] = axis[0].Dot( box.axis[0] );
	c[0][1] = axis[0].Dot( box.axis[1] );
	c[0][2] = axis[0].Dot( box.axis[2] );
	ad[0] = diff.Dot( axis[0] );
	absC[0][0] = NiAbs( c[0][0] );
	absC[0][1] = NiAbs( c[0][1] );
	absC[0][2] = NiAbs( c[0][2] );
	if ( NiAbs( box.extent[0] * absC[0][0] + box.extent[1] * absC[0][1] + box.extent[2] * absC[0][2] + extent[0] ) < ad[0] ) {
		return false;
	}

	c[1][0] = axis[1].Dot( box.axis[0] );
	c[1][1] = axis[1].Dot( box.axis[1] );
	c[1][2] = axis[1].Dot( box.axis[2] );
	ad[1] = diff.Dot( axis[1] );
	absC[1][0] = NiAbs( c[1][0] );
	absC[1][1] = NiAbs( c[1][1] );
	absC[1][2] = NiAbs( c[1][2] );
	if ( NiAbs( box.extent[0] * absC[1][0] + box.extent[1] * absC[1][1] + box.extent[2] * absC[1][2] + extent[1] ) < ad[1] ) {
		return false;
	}

	c[2][0] = axis[2].Dot( box.axis[0] );
	c[2][1] = axis[2].Dot( box.axis[1] );
	c[2][2] = axis[2].Dot( box.axis[2] );
	ad[2] = diff.Dot( axis[2] );
	absC[2][0] = NiAbs( c[2][0] );
	absC[2][1] = NiAbs( c[2][1] );
	absC[2][2] = NiAbs( c[2][2] );
	if ( NiAbs( box.extent[0] * absC[2][0] + box.extent[1] * absC[2][1] + box.extent[2] * absC[2][2] + extent[2] ) < ad[2] ) {
		return false;
	}

	if ( NiAbs( extent[0] * absC[0][0] + extent[1] * absC[1][0] + extent[2] * absC[2][0] + box.extent[0] ) < diff.Dot( box.axis[0] ) ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[0][1] + extent[1] * absC[1][1] + extent[2] * absC[2][1] + box.extent[1] ) < diff.Dot( box.axis[1] ) ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[0][2] + extent[1] * absC[1][2] + extent[2] * absC[2][2] + box.extent[2] ) < diff.Dot( box.axis[2] ) ) {
		return false;
	}

	if ( NiAbs( extent[1] * absC[2][0] + extent[2] * absC[1][0] + box.extent[1] * absC[0][2] + box.extent[2] * absC[0][1] ) < ad[2] * c[1][0] - ad[1] * c[2][0] ) {
		return false;
	}
	if ( NiAbs( extent[1] * absC[2][1] + extent[2] * absC[1][1] + box.extent[0] * absC[0][2] + box.extent[2] * absC[0][0] ) < ad[2] * c[1][1] - ad[1] * c[2][1] ) {
		return false;
	}
	if ( NiAbs( extent[1] * absC[2][2] + extent[2] * absC[1][2] + box.extent[0] * absC[0][1] + box.extent[1] * absC[0][0] ) < ad[2] * c[1][2] - ad[1] * c[2][2] ) {
		return false;
	}

	if ( NiAbs( extent[0] * absC[2][0] + extent[2] * absC[0][0] + box.extent[1] * absC[1][2] + box.extent[2] * absC[1][1] ) < ad[0] * c[2][0] - ad[2] * c[0][0] ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[2][1] + extent[2] * absC[0][1] + box.extent[0] * absC[1][2] + box.extent[2] * absC[1][0] ) < ad[0] * c[2][1] - ad[2] * c[0][1] ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[2][2] + extent[2] * absC[0][2] + box.extent[0] * absC[1][1] + box.extent[1] * absC[1][0] ) < ad[0] * c[2][2] - ad[2] * c[0][2] ) {
		return false;
	}

	if ( NiAbs( extent[0] * absC[1][0] + extent[1] * absC[0][0] + box.extent[1] * absC[2][2] + box.extent[2] * absC[2][1] ) < ad[1] * c[0][0] - ad[0] * c[1][0] ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[1][1] + extent[1] * absC[0][1] + box.extent[0] * absC[2][2] + box.extent[2] * absC[2][0] ) < ad[1] * c[0][1] - ad[0] * c[1][1] ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[1][2] + extent[1] * absC[0][2] + box.extent[0] * absC[2][1] + box.extent[1] * absC[2][0] ) < ad[1] * c[0][2] - ad[0] * c[1][2] ) {
		return false;
	}
	return true;
}

/*
================
OBBNode::BoxIntersectsBox

The test of BoxIntersectsBox against another node's box: its axes and
center are carried from otherShape's model space through the world into
shape's, and its extents scaled by the ratio of the shapes' scales.
================
*/
// FUNCTION: STBC 0x00466e40 ?BoxIntersectsBox@OBBNode@@IAE_NPAVNiAVObject@@0PAV1@@Z
// DRAFT: x87 order of the dot products as in BoxIntersectsBox; the call set is retail's (the other box's centre through TGModelUtils::LocalToLocalPoint, whose WorldToLocalPoint is the out-of-line 0x435960)
bool OBBNode::BoxIntersectsBox( NiAVObject *shape, NiAVObject *otherShape, OBBNode *other ) {
	const NiMatrix3 &rotate = shape->GetWorldRotate();
	const NiMatrix3 &otherRotate = otherShape->GetWorldRotate();
	NiPoint3 otherAxis0 = ( otherRotate * other->axis[0] ) * rotate;
	NiPoint3 otherAxis1 = ( otherRotate * other->axis[1] ) * rotate;
	NiPoint3 otherAxis2 = ( otherRotate * other->axis[2] ) * rotate;
	float scale = otherShape->GetWorldScale() / shape->GetWorldScale();
	float otherExtent0 = scale * other->extent[0];
	float otherExtent1 = scale * other->extent[1];
	float otherExtent2 = scale * other->extent[2];
	NiPoint3 diff = TGModelUtils::LocalToLocalPoint( otherShape, shape, other->center );
	diff -= center;
	float c[3][3];
	float absC[3][3];
	float ad[3];

	c[0][0] = axis[0].Dot( otherAxis0 );
	c[0][1] = axis[0].Dot( otherAxis1 );
	c[0][2] = axis[0].Dot( otherAxis2 );
	ad[0] = diff.Dot( axis[0] );
	absC[0][0] = NiAbs( c[0][0] );
	absC[0][1] = NiAbs( c[0][1] );
	absC[0][2] = NiAbs( c[0][2] );
	if ( NiAbs( otherExtent0 * absC[0][0] + otherExtent1 * absC[0][1] + otherExtent2 * absC[0][2] + extent[0] ) < ad[0] ) {
		return false;
	}

	c[1][0] = axis[1].Dot( otherAxis0 );
	c[1][1] = axis[1].Dot( otherAxis1 );
	c[1][2] = axis[1].Dot( otherAxis2 );
	ad[1] = diff.Dot( axis[1] );
	absC[1][0] = NiAbs( c[1][0] );
	absC[1][1] = NiAbs( c[1][1] );
	absC[1][2] = NiAbs( c[1][2] );
	if ( NiAbs( otherExtent0 * absC[1][0] + otherExtent1 * absC[1][1] + otherExtent2 * absC[1][2] + extent[1] ) < ad[1] ) {
		return false;
	}

	c[2][0] = axis[2].Dot( otherAxis0 );
	c[2][1] = axis[2].Dot( otherAxis1 );
	c[2][2] = axis[2].Dot( otherAxis2 );
	ad[2] = diff.Dot( axis[2] );
	absC[2][0] = NiAbs( c[2][0] );
	absC[2][1] = NiAbs( c[2][1] );
	absC[2][2] = NiAbs( c[2][2] );
	if ( NiAbs( otherExtent0 * absC[2][0] + otherExtent1 * absC[2][1] + otherExtent2 * absC[2][2] + extent[2] ) < ad[2] ) {
		return false;
	}

	if ( NiAbs( extent[0] * absC[0][0] + extent[1] * absC[1][0] + extent[2] * absC[2][0] + otherExtent0 ) < diff.Dot( otherAxis0 ) ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[0][1] + extent[1] * absC[1][1] + extent[2] * absC[2][1] + otherExtent1 ) < diff.Dot( otherAxis1 ) ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[0][2] + extent[1] * absC[1][2] + extent[2] * absC[2][2] + otherExtent2 ) < diff.Dot( otherAxis2 ) ) {
		return false;
	}

	if ( NiAbs( extent[1] * absC[2][0] + extent[2] * absC[1][0] + otherExtent1 * absC[0][2] + otherExtent2 * absC[0][1] ) < ad[2] * c[1][0] - ad[1] * c[2][0] ) {
		return false;
	}
	if ( NiAbs( extent[1] * absC[2][1] + extent[2] * absC[1][1] + otherExtent0 * absC[0][2] + otherExtent2 * absC[0][0] ) < ad[2] * c[1][1] - ad[1] * c[2][1] ) {
		return false;
	}
	if ( NiAbs( extent[1] * absC[2][2] + extent[2] * absC[1][2] + otherExtent0 * absC[0][1] + otherExtent1 * absC[0][0] ) < ad[2] * c[1][2] - ad[1] * c[2][2] ) {
		return false;
	}

	if ( NiAbs( extent[0] * absC[2][0] + extent[2] * absC[0][0] + otherExtent1 * absC[1][2] + otherExtent2 * absC[1][1] ) < ad[0] * c[2][0] - ad[2] * c[0][0] ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[2][1] + extent[2] * absC[0][1] + otherExtent0 * absC[1][2] + otherExtent2 * absC[1][0] ) < ad[0] * c[2][1] - ad[2] * c[0][1] ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[2][2] + extent[2] * absC[0][2] + otherExtent0 * absC[1][1] + otherExtent1 * absC[1][0] ) < ad[0] * c[2][2] - ad[2] * c[0][2] ) {
		return false;
	}

	if ( NiAbs( extent[0] * absC[1][0] + extent[1] * absC[0][0] + otherExtent1 * absC[2][2] + otherExtent2 * absC[2][1] ) < ad[1] * c[0][0] - ad[0] * c[1][0] ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[1][1] + extent[1] * absC[0][1] + otherExtent0 * absC[2][2] + otherExtent2 * absC[2][0] ) < ad[1] * c[0][1] - ad[0] * c[1][1] ) {
		return false;
	}
	if ( NiAbs( extent[0] * absC[1][2] + extent[1] * absC[0][2] + otherExtent0 * absC[2][1] + otherExtent1 * absC[2][0] ) < ad[1] * c[0][2] - ad[0] * c[1][2] ) {
		return false;
	}
	return true;
}

/*
================
OBBNode::TriIntersectsTri
================
*/
// FUNCTION: STBC 0x00467630
bool OBBNode::TriIntersectsTri( NiPoint3 *triangle0[3], NiPoint3 *triangle1[3] ) {
	NiTriIntersect intersect( triangle0, NiPoint3::ZERO, triangle1, NiPoint3::ZERO );
	return intersect.Test( 0.0f );
}

/*
================
OBBNode::FindRayIntersections

Descends into both subnodes when the ray passes the box.
================
*/
// FUNCTION: STBC 0x00467670
bool OBBNode::FindRayIntersections( NiTriShape *shape, const NiPoint3 &localOrigin, const NiPoint3 &localDirection,
							 const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback, VisibleDamagePiece *&piece ) {
	if ( RayIntersectsBox( localOrigin, localDirection ) ) {
		bool hitLeft = false;
		if ( left != NULL ) {
			hitLeft = left->FindRayIntersections( shape, localOrigin, localDirection, origin, direction, pick, callback, piece );
		}
		bool hitRight = false;
		if ( right != NULL ) {
			hitRight = right->FindRayIntersections( shape, localOrigin, localDirection, origin, direction, pick, callback, piece );
		}
		return hitLeft || hitRight;
	}
	return false;
}

/*
================
OBBNode::FindSegmentIntersections
================
*/
// FUNCTION: STBC 0x00467730
bool OBBNode::FindSegmentIntersections( NiTriShape *shape, const NiPoint3 &localOrigin, const NiPoint3 &localDirection,
							 const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback, VisibleDamagePiece *&piece ) {
	if ( SegmentIntersectsBox( localOrigin, localDirection ) ) {
		bool hitLeft = false;
		if ( left != NULL ) {
			hitLeft = left->FindSegmentIntersections( shape, localOrigin, localDirection, origin, direction, pick, callback, piece );
		}
		bool hitRight = false;
		if ( right != NULL ) {
			hitRight = right->FindSegmentIntersections( shape, localOrigin, localDirection, origin, direction, pick, callback, piece );
		}
		return hitLeft || hitRight;
	}
	return false;
}

/*
================
OBBNode::VirtualSlot04
================
*/
// FUNCTION: STBC 0x004677f0
void OBBNode::VirtualSlot04( OBBTriangleList *list ) {
	if ( left != NULL ) {
		left->VirtualSlot04( list );
	}
	if ( right != NULL ) {
		right->VirtualSlot04( list );
	}
}

/*
================
OBBNode::FindSphereIntersections

A box inside the sphere gives every triangle under the node to the
shape's list; otherwise the test descends into both subnodes.
================
*/
// FUNCTION: STBC 0x00467820
bool OBBNode::FindSphereIntersections( NiTriShape *shape, const NiPoint3 &localCenter, float radius, float radiusSquared,
							 NiTArray<OBBTriangleList *> *lists ) {
	bool hit = false;
	if ( SphereIntersectsBox( localCenter, radiusSquared ) ) {
		if ( Unknown4666E0( localCenter, radiusSquared ) ) {
			OBBTriangleList *list = OBBTriangleList::Find( lists, shape );
			if ( list == NULL ) {
				list = new OBBTriangleList( shape );
				lists->Add( list );
			}
			if ( left != NULL ) {
				left->VirtualSlot04( list );
			}
			if ( right != NULL ) {
				// BUG: the left subnode's triangles are added a second time and the right's never
				left->VirtualSlot04( list );
			}
			hit = true;
		} else {
			if ( left != NULL ) {
				hit = left->FindSphereIntersections( shape, localCenter, radius, radiusSquared, lists );
			}
			if ( right != NULL ) {
				hit |= right->FindSphereIntersections( shape, localCenter, radius, radiusSquared, lists );
			}
		}
	}
	return hit;
}

/*
================
OBBNode::FindRoughOBBIntersections

Tests the node's tree against another node's. A node with one subnode
passes the test on to it; when the boxes meet, the larger of two split
nodes is divided further.
================
*/
// FUNCTION: STBC 0x00467ab0
bool OBBNode::FindRoughOBBIntersections( NiTriShape *shape, NiTriShape *otherShape, OBBNode *other, CollisionRecorder *recorder, VisibleDamage *damage, VisibleDamage *otherDamage ) {
	bool hit = false;
	if ( left == NULL && right != NULL ) {
		return right->FindRoughOBBIntersections( shape, otherShape, other, recorder, damage, otherDamage );
	}
	if ( left != NULL && right == NULL ) {
		return left->FindRoughOBBIntersections( shape, otherShape, other, recorder, damage, otherDamage );
	}
	if ( other->left == NULL && other->right != NULL ) {
		return FindRoughOBBIntersections( shape, otherShape, other->right, recorder, damage, otherDamage );
	}
	if ( other->left != NULL && other->right == NULL ) {
		return FindRoughOBBIntersections( shape, otherShape, other->left, recorder, damage, otherDamage );
	}
	if ( BoxIntersectsBox( shape, otherShape, other ) ) {
		// BUG: the box is never filled in, so the volume compared is garbage
		NiBox box;
		float volume = box.extent[0] * box.extent[1] * box.extent[2];
		float otherVolume = other->extent[0] * other->extent[1] * other->extent[2];
		bool otherSplit = other->left != NULL && other->right != NULL;
		if ( volume < otherVolume && otherSplit ) {
			if ( other->left != NULL && other->right != NULL ) {
				hit = FindRoughOBBIntersections( shape, otherShape, other->left, recorder, damage, otherDamage );
				hit |= FindRoughOBBIntersections( shape, otherShape, other->right, recorder, damage, otherDamage );
			}
		} else if ( left != NULL && right != NULL ) {
			hit = left->FindRoughOBBIntersections( shape, otherShape, other, recorder, damage, otherDamage );
			hit |= right->FindRoughOBBIntersections( shape, otherShape, other, recorder, damage, otherDamage );
		}
	}
	return hit;
}

/*
================
OBBNode::FindBoxIntersections
================
*/
// FUNCTION: STBC 0x00467c90
bool OBBNode::FindBoxIntersections( NiTriShape *shape, const NiBox &box, NiTArray<OBBTriangleList *> *lists, int depth,
							 VisibleDamage *callback, VisibleDamagePiece *&piece ) {
	if ( depth > 0 && !BoxIntersectsBox( box ) ) {
		return false;
	}
	bool hitLeft = false;
	if ( left != NULL ) {
		hitLeft = left->FindBoxIntersections( shape, box, lists, depth - 1, callback, piece );
	}
	bool hitRight = false;
	if ( right != NULL ) {
		hitRight = right->FindBoxIntersections( shape, box, lists, depth - 1, callback, piece );
	}
	return hitLeft || hitRight;
}

/*
================
OBBNode::Compare

qsort's order for ProjectionInfo: nearest first.
================
*/
// FUNCTION: STBC 0x00467d40
int OBBNode::Compare( const void *a, const void *b ) {
	const ProjectionInfo *key0 = (const ProjectionInfo *)a;
	const ProjectionInfo *key1 = (const ProjectionInfo *)b;
	if ( key0->distance < key1->distance ) {
		return -1;
	}
	if ( key0->distance > key1->distance ) {
		return 1;
	}
	return 0;
}

/*
================
OBBNode::DivideTriangles

Sorts the range's triangles by the distance of their centroids along the
box's longest axis and gives the nearer half to the first range, the rest
to the second.
================
*/
// FUNCTION: STBC 0x00467d70
// PARKED: complete; register allocation: retail keeps i0 in memory and counts the axis loop down in EBX, this build keeps i0 in EBP; unsigned short and unsigned int axis counters tried
void OBBNode::DivideTriangles( const NiPoint3 *centroids, int i0, int i1, unsigned short *order,
							   int &j0, int &j1, unsigned short *scratch, ProjectionInfo *keys ) {
	unsigned short count = i1 - i0 + 1;

	int longest = 0;
	float maxExtent = -1.0f;
	for ( int axisIndex = 0; axisIndex < 3; axisIndex++ ) {
		if ( extent[axisIndex] > maxExtent ) {
			maxExtent = extent[axisIndex];
			longest = axisIndex;
		}
	}

	unsigned short i;
	unsigned short k = 0;
	for ( i = i0; i <= i1; i++ ) {
		unsigned short triangle = order[i];
		NiPoint3 diff = centroids[triangle] - center;
		keys[k].triangle = triangle;
		keys[k].distance = axis[longest].Dot( diff );
		k++;
	}

	qsort( keys, count, sizeof( ProjectionInfo ), Compare );

	unsigned short next = i0;
	int half = count / 2;
	for ( k = 0; k < half; k++ ) {
		scratch[next++] = keys[k].triangle;
	}
	for ( ; k < count; k++ ) {
		scratch[next++] = keys[k].triangle;
	}

	j0 = i0 + half - 1;
	j1 = j0 + 1;
}

/*
================
OBBLeaf::OBBLeaf

Takes its run of the tree's arrays, copies its triangles into the index
run and fits the box: a lone triangle's box has its first axis along the
first edge, the second in the triangle's plane and the third along its
normal; more triangles go through ComputeOBB. Then each triangle's plane
is stored.
================
*/
// FUNCTION: STBC 0x00467ee0
// PARKED: inline budget: retail also expands the first Unitize's Length; c2trace wants about 30 more tuples of own cost (Length 57 at a share of 55, the final += at exactly 54 left), not a site change
OBBLeaf::OBBLeaf( const unsigned short *triList, const NiPoint3 *vertices, const float *areas,
				  const NiPoint3 *centroids, int i0, int i1, const unsigned short *order,
				  unsigned short *indices_, NiPlane *planes_ ) : indices( indices_ ), planes( planes_ ) {
	count = i1 - i0 + 1;
	int k = 0;
	for ( int i = i0; i <= i1; i++ ) {
		indices[k++] = order[i];
	}

	if ( count > 1 ) {
		ComputeOBB( triList, vertices, areas, centroids, i0, i1, order );
	} else {
		unsigned short start = 3 * indices[0];
		const NiPoint3 &v0 = vertices[triList[start++]];
		const NiPoint3 &v1 = vertices[triList[start++]];
		const NiPoint3 &v2 = vertices[triList[start]];

		NiPoint3 &boxCenter = center;
		NiPoint3 *boxAxis = axis;
		float *boxExtent = extent;

		boxCenter = ( v0 + v1 + v2 ) / 3.0f;

		boxAxis[0] = v1 - v0;
		boxAxis[0].Unitize();

		boxAxis[1] = v2 - v0;
		float projection = boxAxis[0].Dot( boxAxis[1] );
		boxAxis[1] -= projection * boxAxis[0];
		boxAxis[1].Unitize();

		boxAxis[2] = boxAxis[0].Cross( boxAxis[1] );

		NiPoint3 lower( 0.0f, 0.0f, 0.0f );
		NiPoint3 upper( 0.0f, 0.0f, 0.0f );

		// RULE-EXCEPTION: R35, one-line extent updates as in NiOBBLeaf's constructor
		NiPoint3 diff;
		float x, y, z;

		diff = v0 - boxCenter;
		x = boxAxis[0].Dot( diff );
		y = boxAxis[1].Dot( diff );
		z = boxAxis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;

		diff = v1 - boxCenter;
		x = boxAxis[0].Dot( diff );
		y = boxAxis[1].Dot( diff );
		z = boxAxis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;

		diff = v2 - boxCenter;
		x = boxAxis[0].Dot( diff );
		y = boxAxis[1].Dot( diff );
		z = boxAxis[2].Dot( diff );
		if ( x < lower.x ) lower.x = x; else if ( x > upper.x ) upper.x = x;
		if ( y < lower.y ) lower.y = y; else if ( y > upper.y ) upper.y = y;
		if ( z < lower.z ) lower.z = z; else if ( z > upper.z ) upper.z = z;

		NiPoint3 halfExtent = 0.5f * ( upper - lower );
		boxExtent[0] = halfExtent.x;
		boxExtent[1] = halfExtent.y;
		boxExtent[2] = halfExtent.z;

		NiPoint3 middle = 0.5f * ( upper + lower );
		boxCenter += boxAxis[0] * middle.x + boxAxis[1] * middle.y + boxAxis[2] * middle.z;
	}

	for ( k = 0; k < (int)count; k++ ) {
		const unsigned short *corner = &triList[3 * indices[k]];
		planes[k] = NiPlane( vertices[corner[0]], vertices[corner[1]], vertices[corner[2]] );
	}
}


/*
================
OBBLeaf::RayIntersectsTriangle
================
*/
// FUNCTION: STBC 0x00468770
// DamageTool.exe 0x41BC40
bool OBBLeaf::RayIntersectsTriangle( const NiPoint3 &origin, const NiPoint3 &direction, const NiPlane &plane,
							 const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2, bool frontOnly,
							 NiPoint3 &point, float &distance, float &s, float &t ) {
	float cosine = direction * plane.GetNormal();
	if ( frontOnly && cosine > EPSILON ) {
		return false;
	}
	if ( (float)fabs( cosine ) > EPSILON ) {
		distance = -( plane.Distance( origin ) / cosine );
		if ( distance >= 0.0f ) {
			point = origin + distance * direction;
			NiPoint3 edge0 = v1 - v0;
			NiPoint3 edge1 = v2 - v0;
			float edgeDot = edge1.Dot( edge0 );
			float scale0 = 1.0f / ( edge0 * edge0 );
			float scale1 = 1.0f / ( edge1 * edge1 );
			float a = scale0 * edgeDot;
			float b = scale1 * edgeDot;
			float det = (float)fabs( 1.0f - b * a );
			if ( (float)fabs( det ) >= EPSILON ) {
				NiPoint3 diff = v0 - point;
				float p0 = diff.Dot( edge0 ) * scale0;
				float p1 = ( edge1 * diff ) * scale1;
				s = p1 * a - p0;
				t = p0 * b - p1;
				if ( s >= 0.0f && t >= 0.0f && t + s <= det ) {
					float invDet = 1.0f / det;
					s = invDet * s;
					t = invDet * t;
					return true;
				}
			}
		}
	}
	return false;
}

/*
================
OBBLeaf::SegmentIntersectsTriangle
================
*/
// FUNCTION: STBC 0x00468a00
// DamageTool.exe 0x41BED0
bool OBBLeaf::SegmentIntersectsTriangle( const NiPoint3 &origin, const NiPoint3 &direction, const NiPlane &plane,
							 const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2, bool frontOnly,
							 NiPoint3 &point, float &distance, float &s, float &t ) {
	float cosine = direction * plane.GetNormal();
	if ( frontOnly && cosine > EPSILON ) {
		return false;
	}
	if ( (float)fabs( cosine ) > EPSILON ) {
		distance = -( plane.Distance( origin ) / cosine );
		if ( distance >= 0.0f && distance <= 1.0f ) {
			point = origin + distance * direction;
			NiPoint3 edge0 = v1 - v0;
			NiPoint3 edge1 = v2 - v0;
			float edgeDot = edge1.Dot( edge0 );
			float scale0 = 1.0f / ( edge0 * edge0 );
			float scale1 = 1.0f / ( edge1 * edge1 );
			float a = scale0 * edgeDot;
			float b = scale1 * edgeDot;
			float det = (float)fabs( 1.0f - b * a );
			if ( (float)fabs( det ) >= EPSILON ) {
				NiPoint3 diff = v0 - point;
				float p0 = diff.Dot( edge0 ) * scale0;
				float p1 = ( edge1 * diff ) * scale1;
				s = p1 * a - p0;
				t = p0 * b - p1;
				if ( s >= 0.0f && t >= 0.0f && t + s <= det ) {
					float invDet = 1.0f / det;
					s = invDet * s;
					t = invDet * t;
					return true;
				}
			}
		}
	}
	return false;
}

/*
================
OBBLeaf::FindRayIntersections

When the ray passes the leaf's box, each of its triangles the ray crosses
is filed in the pick (NiTriBasedGeom's ray pick, per triangle). A callback
may renumber the triangle's corners first; when it takes the triangle, it
is asked about the hit, and it may skip it (0), report it on a shape and
with a normal of its own (2), or leave it to the leaf (1). A pick that
keeps the first hit stops at it.
================
*/
// FUNCTION: STBC 0x00468cb0
// DRAFT: calls and branches agree; registers: retail keeps this in ESI and the pick in EBP, and loads the callback's vtable and piece before its pushes
bool OBBLeaf::FindRayIntersections( NiTriShape *shape, const NiPoint3 &localOrigin, const NiPoint3 &localDirection,
							 const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback, VisibleDamagePiece *&piece ) {
	if ( !RayIntersectsBox( localOrigin, localDirection ) ) {
		return false;
	}

	bool hit = false;
	const NiPoint3 *vertices = shape->GetShapeData()->GetVertices();
	const unsigned short *triList = shape->GetTriList();
	bool firstOnly = pick.GetPickType() == NiPick::FIND_FIRST && pick.GetSortType() == NiPick::NO_SORT;
	for ( unsigned int i = 0; i < count; i++ ) {
		unsigned int corner = 3 * indices[i];
		unsigned short corner0 = triList[corner++];
		unsigned short corner1 = triList[corner++];
		unsigned short corner2 = triList[corner];
		bool taken = false;
		if ( callback != NULL ) {
			taken = callback->VirtualSlot00( shape, indices[i], &corner0, &corner1, &corner2, &piece );
		}

		NiPoint3 point;
		float distance;
		float s;
		float t;
		if ( !RayIntersectsTriangle( localOrigin, localDirection, planes[i], vertices[corner0], vertices[corner1], vertices[corner2],
							 pick.GetFrontOnly(), point, distance, s, t ) ) {
			continue;
		}

		NiPick::Record *record = NULL;
		if ( taken ) {
			// identity-TODO: VisibleDamage.h declares slot 1's shape, origin,
			// normal and triangle parameters as int (slot2's header): they
			// are the addresses of the hit shape, the ray in model space, the
			// normal and the triangle index, which the callback fills
			NiTriShape *hitShape = shape;
			NiPoint3 normal;
			unsigned int triangle = 0;
			int result = callback->VirtualSlot01( (int)&hitShape, (int)&localOrigin, (int)&localDirection, &origin, &direction,
												  &point, &distance, (int)&normal, (int)&triangle, piece );
			if ( result == 0 ) {
				continue;
			}
			if ( result == 2 ) {
				record = new NiPick::Record( hitShape );
				if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
					point = hitShape->GetWorldScale() * ( hitShape->GetWorldRotate() * point ) + hitShape->GetWorldTranslate();
				}
				record->SetIntersection( point );
				record->SetDistance( distance );
				record->SetTriangleIndex( triangle );
				if ( pick.GetReturnNormal() ) {
					if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
						normal = hitShape->GetWorldRotate() * normal;
					}
					record->SetNormal( normal );
				}
			}
		}

		hit = true;
		if ( record == NULL ) {
			record = new NiPick::Record( shape );
			if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
				point = shape->GetWorldScale() * ( shape->GetWorldRotate() * point ) + shape->GetWorldTranslate();
			}
			record->SetIntersection( point );
			record->SetDistance( distance );
			record->SetTriangleIndex( indices[i] );

			float w = 1.0f - ( s + t );
			if ( pick.GetReturnTexture() ) {
				NiPoint2 *textures = shape->GetShapeData()->GetTextures();
				if ( textures != NULL ) {
					record->SetTexture( w * textures[corner0] + s * textures[corner1] + t * textures[corner2] );
				} else {
					record->SetTexture( NiPoint2::ZERO );
				}
			}

			if ( pick.GetReturnNormal() ) {
				NiPoint3 normal;
				NiPoint3 *normals = shape->GetShapeData()->GetNormals();
				if ( pick.GetReturnSmoothNormal() && normals != NULL ) {
					normal = w * normals[corner0] + s * normals[corner1] + t * normals[corner2];
					normal.Unitize();
				} else {
					normal = planes[i].GetNormal();
				}
				if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
					normal = shape->GetWorldRotate() * normal;
				}
				record->SetNormal( normal );
			}
		}

		pick.Add( record );
		if ( firstOnly ) {
			return true;
		}
	}
	return hit;
}

/*
================
OBBLeaf::FindSegmentIntersections

Slot 1 for the segment from localOrigin to localOrigin + localDirection:
the box and triangle tests are the segment's, and the callback is asked
through its slot 2.
================
*/
// FUNCTION: STBC 0x00469640
// DRAFT: calls and branches agree; registers: retail keeps this in ESI and the pick in EBP, and loads the callback's vtable and piece before its pushes
bool OBBLeaf::FindSegmentIntersections( NiTriShape *shape, const NiPoint3 &localOrigin, const NiPoint3 &localDirection,
							 const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback, VisibleDamagePiece *&piece ) {
	if ( !SegmentIntersectsBox( localOrigin, localDirection ) ) {
		return false;
	}

	bool hit = false;
	const NiPoint3 *vertices = shape->GetShapeData()->GetVertices();
	const unsigned short *triList = shape->GetTriList();
	bool firstOnly = pick.GetPickType() == NiPick::FIND_FIRST && pick.GetSortType() == NiPick::NO_SORT;
	for ( unsigned int i = 0; i < count; i++ ) {
		unsigned int corner = 3 * indices[i];
		unsigned short corner0 = triList[corner++];
		unsigned short corner1 = triList[corner++];
		unsigned short corner2 = triList[corner];
		bool taken = false;
		if ( callback != NULL ) {
			taken = callback->VirtualSlot00( shape, indices[i], &corner0, &corner1, &corner2, &piece );
		}

		NiPoint3 point;
		float distance;
		float s;
		float t;
		if ( !SegmentIntersectsTriangle( localOrigin, localDirection, planes[i], vertices[corner0], vertices[corner1], vertices[corner2],
							 pick.GetFrontOnly(), point, distance, s, t ) ) {
			continue;
		}

		NiPick::Record *record = NULL;
		if ( taken ) {
			// identity-TODO: as slot 1's call
			NiTriShape *hitShape = shape;
			NiPoint3 normal;
			unsigned int triangle = 0;
			int result = callback->VirtualSlot02( (int)&hitShape, (int)&localOrigin, (int)&localDirection, &origin, &direction,
												  &point, &distance, (int)&normal, (int)&triangle, piece );
			if ( result == 0 ) {
				continue;
			}
			if ( result == 2 ) {
				record = new NiPick::Record( hitShape );
				if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
					point = hitShape->GetWorldScale() * ( hitShape->GetWorldRotate() * point ) + hitShape->GetWorldTranslate();
				}
				record->SetIntersection( point );
				record->SetDistance( distance );
				record->SetTriangleIndex( triangle );
				if ( pick.GetReturnNormal() ) {
					if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
						normal = hitShape->GetWorldRotate() * normal;
					}
					record->SetNormal( normal );
				}
			}
		}

		hit = true;
		if ( record == NULL ) {
			record = new NiPick::Record( shape );
			if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
				point = shape->GetWorldScale() * ( shape->GetWorldRotate() * point ) + shape->GetWorldTranslate();
			}
			record->SetIntersection( point );
			record->SetDistance( distance );
			record->SetTriangleIndex( indices[i] );

			float w = 1.0f - ( s + t );
			if ( pick.GetReturnTexture() ) {
				NiPoint2 *textures = shape->GetShapeData()->GetTextures();
				if ( textures != NULL ) {
					record->SetTexture( w * textures[corner0] + s * textures[corner1] + t * textures[corner2] );
				} else {
					record->SetTexture( NiPoint2::ZERO );
				}
			}

			if ( pick.GetReturnNormal() ) {
				NiPoint3 normal;
				NiPoint3 *normals = shape->GetShapeData()->GetNormals();
				if ( pick.GetReturnSmoothNormal() && normals != NULL ) {
					normal = w * normals[corner0] + s * normals[corner1] + t * normals[corner2];
					normal.Unitize();
				} else {
					normal = planes[i].GetNormal();
				}
				if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
					normal = shape->GetWorldRotate() * normal;
				}
				record->SetNormal( normal );
			}
		}

		pick.Add( record );
		if ( firstOnly ) {
			return true;
		}
	}
	return hit;
}

/*
================
OBBLeaf::VirtualSlot04
================
*/
// FUNCTION: STBC 0x00469f80
void OBBLeaf::VirtualSlot04( OBBTriangleList *list ) {
	if ( count > 0 ) {
		memcpy( &list->triangles[list->count], indices, count * sizeof( unsigned short ) );
		list->count += count;
	}
}
/*
================
OBBLeaf::FindSphereIntersections

A box inside the sphere gives all of the leaf's triangles to the shape's
list. Otherwise each triangle whose plane passes within radius of the
center is kept when a corner, or the triangle's closest point, lies
inside the sphere.
================
*/
// FUNCTION: STBC 0x00469fc0
// DRAFT: inline budget: this build runs out before the loop's list (NiPointer's constructor, SetAtGrow and DecRefCount out of line), where retail expands everything but the first site's SetSize and SetAt
bool OBBLeaf::FindSphereIntersections( NiTriShape *shape, const NiPoint3 &localCenter, float radius, float radiusSquared,
							 NiTArray<OBBTriangleList *> *lists ) {
	bool hit = false;
	if ( SphereIntersectsBox( localCenter, radiusSquared ) ) {
		if ( Unknown4666E0( localCenter, radiusSquared ) ) {
			OBBTriangleList *list = OBBTriangleList::Find( lists, shape );
			if ( list == NULL ) {
				list = new OBBTriangleList( shape );
				list->shape = shape;	// BUG: redundant: the constructor stored the shape
				lists->Add( list );
			}
			OBBLeaf::VirtualSlot04( list );
		} else {
			NiTriShapeData *data = shape->GetShapeData();
			const NiPoint3 *vertices = data->GetVertices();
			const unsigned short *triList = data->GetTriList();
			OBBTriangleList *list = NULL;
			for ( unsigned int i = 0; i < count; i++ ) {
				if ( (float)fabs( planes[i].Distance( localCenter ) ) <= radius ) {
					unsigned int first = 3 * indices[i];
					const NiPoint3 &v0 = vertices[triList[first]];
					const NiPoint3 &v1 = vertices[triList[first + 1]];
					const NiPoint3 &v2 = vertices[triList[first + 2]];
					float s;
					float t;
					if ( ( v0 - localCenter ).SqrLength() <= radiusSquared || ( v1 - localCenter ).SqrLength() <= radiusSquared
						 || ( v2 - localCenter ).SqrLength() <= radiusSquared
						 || SquaredDistanceToTri( localCenter, v0, v1, v2, s, t ) <= radiusSquared ) {
						hit = true;
						if ( list == NULL ) {
							list = OBBTriangleList::Find( lists, shape );
							if ( list == NULL ) {
								list = new OBBTriangleList( shape );
								lists->Add( list );
							}
						}
						list->triangles[list->count++] = indices[i];
					}
				}
			}
		}
	}
	return hit;
}

/*
================
OBBLeaf::FindRoughOBBIntersections

Tests the leaf against another node's tree. When the other side is a leaf
too, each of its triangles (moved into this shape's frame) is tested
against each of this leaf's; triangles the visible damage has removed, or
with a removed corner, are skipped. The first pair that meets adds the
centre of the six corners, in world space, to the recorder.
================
*/
// FUNCTION: STBC 0x0046a420
// DRAFT: inline budget: the call set is retail's but for one NiPoint3 constructor (0x414540): the hit point's LocalToWorldPoint expands its operator*'s constructor with a share of 50 where retail's is under 47 (about 19 fewer units left before that site); frame 0xC0 against 0x124
bool OBBLeaf::FindRoughOBBIntersections( NiTriShape *shape, NiTriShape *otherShape, OBBNode *other, CollisionRecorder *recorder, VisibleDamage *damage, VisibleDamage *otherDamage ) {
	bool hit = false;
	if ( other->GetLeft() == NULL && other->GetRight() != NULL ) {
		return FindRoughOBBIntersections( shape, otherShape, other->GetRight(), recorder, damage, otherDamage );
	}
	if ( other->GetLeft() != NULL && other->GetRight() == NULL ) {
		return FindRoughOBBIntersections( shape, otherShape, other->GetLeft(), recorder, damage, otherDamage );
	}
	if ( BoxIntersectsBox( shape, otherShape, other ) ) {
		if ( other->GetLeft() != NULL && other->GetRight() != NULL ) {
			hit = FindRoughOBBIntersections( shape, otherShape, other->GetLeft(), recorder, damage, otherDamage );
			hit |= FindRoughOBBIntersections( shape, otherShape, other->GetRight(), recorder, damage, otherDamage );
		} else {
			OBBLeaf *otherLeaf = static_cast<OBBLeaf *>( other );
			obbLeafTests++;

			NiPoint3 corners[3];
			NiPoint3 *triangle[3];
			triangle[0] = &corners[0];
			triangle[1] = &corners[1];
			triangle[2] = &corners[2];
			DamageBitArray *removedTriangles = NULL;
			DamageBitArray *removedVertices = NULL;
			const NiPoint3 *vertices = shape->GetShapeData()->GetVertices();
			const unsigned short *triList;
			if ( damage != NULL ) {
				triList = damage->VirtualSlot05( shape );
				removedTriangles = damage->VirtualSlot03( shape );
				removedVertices = damage->VirtualSlot04( shape );
			} else {
				triList = shape->GetTriList();
			}

			NiPoint3 otherCorners[3];
			NiPoint3 *otherTriangle[3];
			otherTriangle[0] = &otherCorners[0];
			otherTriangle[1] = &otherCorners[1];
			otherTriangle[2] = &otherCorners[2];
			const NiPoint3 *otherVertices = otherShape->GetShapeData()->GetVertices();
			DamageBitArray *otherRemovedTriangles = NULL;
			DamageBitArray *otherRemovedVertices = NULL;
			const unsigned short *otherTriList;
			if ( otherDamage != NULL ) {
				otherTriList = otherDamage->VirtualSlot05( otherShape );
				otherRemovedTriangles = otherDamage->VirtualSlot03( otherShape );
				otherRemovedVertices = otherDamage->VirtualSlot04( otherShape );
			} else {
				otherTriList = otherShape->GetTriList();
			}

			for ( unsigned int i = 0; i < otherLeaf->count; i++ ) {
				if ( otherRemovedTriangles != NULL && otherRemovedTriangles->IsSet( otherLeaf->indices[i] ) ) {
					continue;
				}
				const unsigned short *otherCorner = &otherTriList[3 * otherLeaf->indices[i]];
				if ( otherRemovedVertices != NULL && ( otherRemovedVertices->IsSet( otherCorner[0] ) || otherRemovedVertices->IsSet( otherCorner[1] )
													   || otherRemovedVertices->IsSet( otherCorner[2] ) ) ) {
					continue;
				}
				otherCorners[0] = otherVertices[otherCorner[0]];
				otherCorners[1] = otherVertices[otherCorner[1]];
				otherCorners[2] = otherVertices[otherCorner[2]];
				otherCorners[0] = TGModelUtils::LocalToLocalPoint( otherShape, shape, otherCorners[0] );
				otherCorners[1] = TGModelUtils::LocalToLocalPoint( otherShape, shape, otherCorners[1] );
				otherCorners[2] = TGModelUtils::LocalToLocalPoint( otherShape, shape, otherCorners[2] );

				for ( unsigned int j = 0; j < count; j++ ) {
					if ( removedTriangles != NULL && removedTriangles->IsSet( indices[j] ) ) {
						continue;
					}
					const unsigned short *corner = &triList[3 * indices[j]];
					if ( removedVertices != NULL && ( removedVertices->IsSet( corner[0] ) || removedVertices->IsSet( corner[1] )
													  || removedVertices->IsSet( corner[2] ) ) ) {
						continue;
					}
					corners[0] = vertices[corner[0]];
					corners[1] = vertices[corner[1]];
					corners[2] = vertices[corner[2]];
					if ( TriIntersectsTri( triangle, otherTriangle ) ) {
						NiPoint3 center = ( otherCorners[0] + otherCorners[1] + otherCorners[2] + corners[0] + corners[1] + corners[2] ) * ( 1.0f / 6.0f );
						NiPoint3 point = TGModelUtils::LocalToWorldPoint( shape, center );
						recorder->AddPoint( new NiPoint3( point ) );
						return true;
					}
				}
			}
		}
	}
	return hit;
}

/*
================
OBBLeaf::FindBoxIntersections

At depth 0 every triangle of the leaf goes to the shape's list; else the
leaf's box is tested first, then each triangle, which the callback may
renumber, against the box.
================
*/
// FUNCTION: STBC 0x0046ac60
// DRAFT: inline budget: retail expands the second site's SetAt inline (the first calls SetSize and SetAt out of line, as here); the corner reads and the callback's argument pushes are scheduled differently
bool OBBLeaf::FindBoxIntersections( NiTriShape *shape, const NiBox &box, NiTArray<OBBTriangleList *> *lists, int depth,
							 VisibleDamage *callback, VisibleDamagePiece *&piece ) {
	bool hit = false;
	if ( depth <= 0 ) {
		OBBTriangleList *list = OBBTriangleList::Find( lists, shape );
		if ( list == NULL ) {
			list = new OBBTriangleList( shape );
			lists->Add( list );
		}
		OBBLeaf::VirtualSlot04( list );
		return count != 0;
	}
	if ( BoxIntersectsBox( box ) ) {
		NiTriShapeData *data = shape->GetShapeData();
		const NiPoint3 *vertices = data->GetVertices();
		const unsigned short *triList = data->GetTriList();
		OBBTriangleList *list = NULL;
		for ( unsigned int i = 0; i < count; i++ ) {
			unsigned int first = 3 * indices[i];
			unsigned short corner0 = triList[first];
			unsigned short corner1 = triList[first + 1];
			unsigned short corner2 = triList[first + 2];
			if ( callback != NULL ) {
				callback->VirtualSlot00( shape, indices[i], &corner0, &corner1, &corner2, &piece );
			}
			if ( BoxIntersectsTri( box, vertices[corner0], vertices[corner1], vertices[corner2] ) ) {
				if ( list == NULL ) {
					hit = true;
					list = OBBTriangleList::Find( lists, shape );
					if ( list == NULL ) {
						list = new OBBTriangleList( shape );
						lists->Add( list );
					}
				}
				list->triangles[list->count++] = indices[i];
			}
		}
	}
	return hit;
}

/*
================
OBBLeaf::SquaredDistanceToTri

The squared distance from point to the triangle v0 + s * edge0 + t * edge1
(Eberly's point-triangle distance): the quadratic in s and t is minimised
over the triangle by the region of the plane its free minimum falls in.
================
*/
// FUNCTION: STBC 0x0046b020
float OBBLeaf::SquaredDistanceToTri( const NiPoint3 &point, const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2,
							  float &s, float &t ) {
	NiPoint3 edge0;
	NiPoint3 edge1;
	NiPoint3 diff;
	edge0 = v1 - v0;
	edge1 = v2 - v0;
	diff = v0 - point;
	float a00 = edge0.SqrLength();
	float a01 = edge1.Dot( edge0 );
	float a11 = edge1.SqrLength();
	float b0 = diff.Dot( edge0 );
	float b1 = diff.Dot( edge1 );
	float c = diff.SqrLength();
	float det = NiAbs( a00 * a11 - a01 * a01 );
	s = a01 * b1 - a11 * b0;
	t = a01 * b0 - a00 * b1;
	float numer;
	float denom;
	float tmp0;
	float tmp1;

	if ( s + t <= det ) {
		if ( s < 0.0f ) {
			if ( t < 0.0f ) {
				// region 4
				if ( b0 < 0.0f ) {
					t = 0.0f;
					if ( -b0 >= a00 ) {
						s = 1.0f;
						return NiAbs( a00 + 2.0f * b0 + c );
					}
					s = -b0 / a00;
					return NiAbs( b0 * s + c );
				}
				s = 0.0f;
				if ( b1 >= 0.0f ) {
					t = 0.0f;
					return NiAbs( c );
				}
				if ( -b1 >= a11 ) {
					t = 1.0f;
					return NiAbs( a11 + 2.0f * b1 + c );
				}
				t = -b1 / a11;
				return NiAbs( b1 * t + c );
			}
			// region 3
			s = 0.0f;
			if ( b1 >= 0.0f ) {
				t = 0.0f;
				return NiAbs( c );
			}
			if ( -b1 >= a11 ) {
				t = 1.0f;
				return NiAbs( a11 + 2.0f * b1 + c );
			}
			t = -b1 / a11;
			return NiAbs( b1 * t + c );
		}
		if ( t < 0.0f ) {
			// region 5
			t = 0.0f;
			if ( b0 >= 0.0f ) {
				s = 0.0f;
				return NiAbs( c );
			}
			if ( -b0 >= a00 ) {
				s = 1.0f;
				return NiAbs( a00 + 2.0f * b0 + c );
			}
			s = -b0 / a00;
			return NiAbs( b0 * s + c );
		}
		// region 0
		float invDet = 1.0f / det;
		s *= invDet;
		t *= invDet;
		return NiAbs( s * ( a00 * s + a01 * t + 2.0f * b0 ) + t * ( a01 * s + a11 * t + 2.0f * b1 ) + c );
	}

	if ( s < 0.0f ) {
		// region 2
		tmp0 = a01 + b0;
		tmp1 = a11 + b1;
		if ( tmp1 > tmp0 ) {
			numer = tmp1 - tmp0;
			denom = a00 - 2.0f * a01 + a11;
			if ( numer >= denom ) {
				s = 1.0f;
				t = 0.0f;
				return NiAbs( a00 + 2.0f * b0 + c );
			}
			s = numer / denom;
			t = 1.0f - s;
			return NiAbs( a11 + 2.0f * b1 + c - numer * s );
		}
		s = 0.0f;
		if ( tmp1 <= 0.0f ) {
			t = 1.0f;
			return NiAbs( a11 + 2.0f * b1 + c );
		}
		if ( b1 >= 0.0f ) {
			t = 0.0f;
			return NiAbs( c );
		}
		t = -b1 / a11;
		return NiAbs( b1 * t + c );
	}
	if ( t < 0.0f ) {
		// region 6
		tmp0 = a01 + b1;
		tmp1 = a00 + b0;
		if ( tmp1 > tmp0 ) {
			numer = tmp1 - tmp0;
			denom = a00 - 2.0f * a01 + a11;
			if ( numer >= denom ) {
				t = 1.0f;
				s = 0.0f;
				return NiAbs( a11 + 2.0f * b1 + c );
			}
			t = numer / denom;
			s = 1.0f - t;
			return NiAbs( a00 + 2.0f * b0 + c - numer * t );
		}
		t = 0.0f;
		if ( tmp1 <= 0.0f ) {
			s = 1.0f;
			return NiAbs( a00 + 2.0f * b0 + c );
		}
		if ( b0 >= 0.0f ) {
			s = 0.0f;
			return NiAbs( c );
		}
		s = -b0 / a00;
		return NiAbs( b0 * s + c );
	}
	// region 1
	tmp0 = a01 + b0;
	tmp1 = a11 + b1;
	numer = tmp1 - tmp0;
	if ( numer <= 0.0f ) {
		s = 0.0f;
		t = 1.0f;
		return NiAbs( a11 + 2.0f * b1 + c );
	}
	denom = a00 - 2.0f * a01 + a11;
	if ( numer >= denom ) {
		s = 1.0f;
		t = 0.0f;
		return NiAbs( a00 + 2.0f * b0 + c );
	}
	s = numer / denom;
	t = 1.0f - s;
	return NiAbs( a11 + 2.0f * b1 + c - numer * s );
}

/*
================
OBBLeaf::BoxIntersectsTri

The separating axis test of the triangle v0 v1 v2 against box: the
triangle's normal, the three box axes and the nine products of a box axis
with an edge.
================
*/
// FUNCTION: STBC 0x0046b5a0
// DRAFT: branches and returns close (retail shares three of the return-false exits); x87: retail keeps the normal's third projection on the FPU and sums the radius from it, and projects diff on the normal z first
bool OBBLeaf::BoxIntersectsTri( const NiBox &box, const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2 ) {
	NiPoint3 edge0 = v1 - v0;
	NiPoint3 edge1 = v2 - v0;
	NiPoint3 normal = edge0.Cross( edge1 );
	NiPoint3 diff = v0 - box.center;

	// the normal
	float normal0 = normal.Dot( box.axis[0] );
	float normal1 = normal.Dot( box.axis[1] );
	float normal2 = normal.Dot( box.axis[2] );
	float radius = NiAbs( normal0 * box.extent[0] ) + NiAbs( normal1 * box.extent[1] )
				   + NiAbs( normal2 * box.extent[2] );
	float distance = normal.Dot( diff );
	if ( distance > radius ) {
		return false;
	}
	if ( distance < -radius ) {
		return false;
	}

	// the box axes: the first corner's projection, relative to the box's
	// center, and the other corners' relative to it
	float diff0 = diff.Dot( box.axis[0] );
	float edge00 = edge0.Dot( box.axis[0] );
	float edge10 = edge1.Dot( box.axis[0] );
	if ( diff0 > box.extent[0] ) {
		float least;
		if ( edge00 < 0.0f ) {
			least = ( edge10 <= edge00 ) ? edge10 : edge00;
		} else if ( edge10 < 0.0f ) {
			least = edge10;
		} else {
			return false;
		}
		if ( diff0 + least > box.extent[0] ) {
			return false;
		}
	} else if ( diff0 < -box.extent[0] ) {
		float most;
		if ( edge00 > 0.0f ) {
			most = ( edge10 < edge00 ) ? edge00 : edge10;
		} else if ( edge10 > 0.0f ) {
			most = edge10;
		} else {
			return false;
		}
		if ( diff0 + most < -box.extent[0] ) {
			return false;
		}
	}
	float diff1 = diff.Dot( box.axis[1] );
	float edge01 = edge0.Dot( box.axis[1] );
	float edge11 = edge1.Dot( box.axis[1] );
	if ( diff1 > box.extent[1] ) {
		float least;
		if ( edge01 < 0.0f ) {
			least = ( edge11 <= edge01 ) ? edge11 : edge01;
		} else if ( edge11 < 0.0f ) {
			least = edge11;
		} else {
			return false;
		}
		if ( diff1 + least > box.extent[1] ) {
			return false;
		}
	} else if ( diff1 < -box.extent[1] ) {
		float most;
		if ( edge01 > 0.0f ) {
			most = ( edge11 < edge01 ) ? edge01 : edge11;
		} else if ( edge11 > 0.0f ) {
			most = edge11;
		} else {
			return false;
		}
		if ( diff1 + most < -box.extent[1] ) {
			return false;
		}
	}
	float diff2 = diff.Dot( box.axis[2] );
	float edge02 = edge0.Dot( box.axis[2] );
	float edge12 = edge1.Dot( box.axis[2] );
	if ( diff2 > box.extent[2] ) {
		float least;
		if ( edge02 < 0.0f ) {
			least = ( edge12 <= edge02 ) ? edge12 : edge02;
		} else if ( edge12 < 0.0f ) {
			least = edge12;
		} else {
			return false;
		}
		if ( diff2 + least > box.extent[2] ) {
			return false;
		}
	} else if ( diff2 < -box.extent[2] ) {
		float most;
		if ( edge02 > 0.0f ) {
			most = ( edge12 < edge02 ) ? edge02 : edge12;
		} else if ( edge12 > 0.0f ) {
			most = edge12;
		} else {
			return false;
		}
		if ( diff2 + most < -box.extent[2] ) {
			return false;
		}
	}

	// the box axes crossed with the edges: two corners project to the same
	// point, the third is off it by the normal's projection on the axis
	float cross0;
	float cross1;
	float crossRadius;
	cross0 = diff.Dot( box.axis[0].Cross( edge0 ) );
	crossRadius = NiAbs( edge02 * box.extent[1] ) + NiAbs( edge01 * box.extent[2] );
	if ( cross0 > crossRadius ) {
		if ( normal0 >= 0.0f ) {
			return false;
		}
		if ( cross0 + normal0 > crossRadius ) {
			return false;
		}
	} else if ( cross0 < -crossRadius ) {
		if ( normal0 <= 0.0f ) {
			return false;
		}
		if ( cross0 + normal0 < -crossRadius ) {
			return false;
		}
	}
	cross1 = diff.Dot( box.axis[0].Cross( edge1 ) );
	crossRadius = NiAbs( edge12 * box.extent[1] ) + NiAbs( edge11 * box.extent[2] );
	if ( cross1 > crossRadius ) {
		if ( -normal0 >= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal0 > crossRadius ) {
			return false;
		}
	} else if ( cross1 < -crossRadius ) {
		if ( -normal0 <= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal0 < -crossRadius ) {
			return false;
		}
	}
	cross1 -= cross0;
	crossRadius = NiAbs( ( edge12 - edge02 ) * box.extent[1] ) + NiAbs( ( edge11 - edge01 ) * box.extent[2] );
	if ( cross1 > crossRadius ) {
		if ( -normal0 >= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal0 > crossRadius ) {
			return false;
		}
	} else if ( cross1 < -crossRadius ) {
		if ( -normal0 <= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal0 < -crossRadius ) {
			return false;
		}
	}
	cross0 = diff.Dot( box.axis[1].Cross( edge0 ) );
	crossRadius = NiAbs( edge02 * box.extent[0] ) + NiAbs( edge00 * box.extent[2] );
	if ( cross0 > crossRadius ) {
		if ( normal1 >= 0.0f ) {
			return false;
		}
		if ( cross0 + normal1 > crossRadius ) {
			return false;
		}
	} else if ( cross0 < -crossRadius ) {
		if ( normal1 <= 0.0f ) {
			return false;
		}
		if ( cross0 + normal1 < -crossRadius ) {
			return false;
		}
	}
	cross1 = diff.Dot( box.axis[1].Cross( edge1 ) );
	crossRadius = NiAbs( edge12 * box.extent[0] ) + NiAbs( edge10 * box.extent[2] );
	if ( cross1 > crossRadius ) {
		if ( -normal1 >= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal1 > crossRadius ) {
			return false;
		}
	} else if ( cross1 < -crossRadius ) {
		if ( -normal1 <= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal1 < -crossRadius ) {
			return false;
		}
	}
	cross1 -= cross0;
	crossRadius = NiAbs( ( edge12 - edge02 ) * box.extent[0] ) + NiAbs( ( edge10 - edge00 ) * box.extent[2] );
	if ( cross1 > crossRadius ) {
		if ( -normal1 >= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal1 > crossRadius ) {
			return false;
		}
	} else if ( cross1 < -crossRadius ) {
		if ( -normal1 <= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal1 < -crossRadius ) {
			return false;
		}
	}
	cross0 = diff.Dot( box.axis[2].Cross( edge0 ) );
	crossRadius = NiAbs( edge01 * box.extent[0] ) + NiAbs( edge00 * box.extent[1] );
	if ( cross0 > crossRadius ) {
		if ( normal2 >= 0.0f ) {
			return false;
		}
		if ( cross0 + normal2 > crossRadius ) {
			return false;
		}
	} else if ( cross0 < -crossRadius ) {
		if ( normal2 <= 0.0f ) {
			return false;
		}
		if ( cross0 + normal2 < -crossRadius ) {
			return false;
		}
	}
	cross1 = diff.Dot( box.axis[2].Cross( edge1 ) );
	crossRadius = NiAbs( edge11 * box.extent[0] ) + NiAbs( edge10 * box.extent[1] );
	if ( cross1 > crossRadius ) {
		if ( -normal2 >= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal2 > crossRadius ) {
			return false;
		}
	} else if ( cross1 < -crossRadius ) {
		if ( -normal2 <= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal2 < -crossRadius ) {
			return false;
		}
	}
	cross1 -= cross0;
	crossRadius = NiAbs( ( edge11 - edge01 ) * box.extent[0] ) + NiAbs( ( edge10 - edge00 ) * box.extent[1] );
	if ( cross1 > crossRadius ) {
		if ( -normal2 >= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal2 > crossRadius ) {
			return false;
		}
	} else if ( cross1 < -crossRadius ) {
		if ( -normal2 <= 0.0f ) {
			return false;
		}
		if ( cross1 + -normal2 < -crossRadius ) {
			return false;
		}
	}
	return true;
}

