// NiOBBLeaf.cpp -- a leaf of a shape's bounding box tree

#include "NiRTLib.h"
#include <float.h>
#include "NiOBBLeaf.h"
#include "NiOBBRoot.h"
#include "NiNode.h"
#include "NiCollisionRecord.h"
#include "NiPick.h"
#include "NiTriBasedGeom.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00827f50 _$E29
// FUNCTION: STBC 0x00827f60 _$E40
// FUNCTION: STBC 0x00827f70 _$E44
// FUNCTION: STBC 0x00827f80 _$E48
// FUNCTION: STBC 0x00827f90 _$E52
// FUNCTION: STBC 0x00827fa0 _$E176
// FUNCTION: STBC 0x00827fb0 _$E183
// FUNCTION: STBC 0x00827fc0 _$E187
// FUNCTION: STBC 0x00827ff0 _$E195
// FUNCTION: STBC 0x00828000 _$E202
// FUNCTION: STBC 0x00828010 _$E206
// FUNCTION: STBC 0x00828020 _$E210

/*
================
NiOBBLeaf::NiOBBLeaf

The box of a triangle: the first axis along its first edge, the second in
its plane, the third along its normal, centered on the triangle.
================
*/
// FUNCTION: STBC 0x825D60
// Gamebryo 1.2.2: NiOBBLeaf::NiOBBLeaf
NiOBBLeaf::NiOBBLeaf( NiOBBRoot *root, const unsigned short *triList, const NiPoint3 *vertices,
					  const NiPoint3 *worldVertices, unsigned short triangle ) {
	left = 0;
	right = 0;

	unsigned short start = 3 * triangle;

	unsigned short index = triList[start];
	vertex[0] = &worldVertices[index];
	const NiPoint3 &v0 = vertices[index];
	start++;

	index = triList[start];
	vertex[1] = &worldVertices[index];
	const NiPoint3 &v1 = vertices[index];
	start++;

	index = triList[start];
	vertex[2] = &worldVertices[index];
	const NiPoint3 &v2 = vertices[index];

	NiPoint3 &center = box.center;
	NiPoint3 *axis = box.axis;
	float *extent = box.extent;

	center = ( v0 + v1 + v2 ) / 3.0f;

	axis[0] = v1 - v0;
	axis[0].Unitize();

	axis[1] = v2 - v0;
	float projection = axis[0].Dot( axis[1] );
	axis[1] -= projection * axis[0];
	axis[1].Unitize();

	axis[2] = axis[0].Cross( axis[1] );

	NiPoint3 lower( 0.0f, 0.0f, 0.0f );
	NiPoint3 upper( 0.0f, 0.0f, 0.0f );

	// RULE-EXCEPTION: R35, one-line extent updates as in NiOBBNode::ComputeOBB (line breaks change the codegen)
	NiPoint3 diff;
	float x, y, z;

	diff = v0 - center;
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
ComputeBarycenter

The barycentric coordinates of a point in the plane of a triangle.
================
*/
// FUNCTION: STBC 0x826560
void NiOBBLeaf::ComputeBarycenter( const NiPoint3 &v0, const NiPoint3 &v1, const NiPoint3 &v2, const NiPoint3 &point,
							   float barycenter[3] ) {
	NiPoint3 edge0 = v0 - v2;
	NiPoint3 edge1 = v1 - v2;
	NiPoint3 diff = point - v2;
	float a00 = edge0.Dot( edge0 );
	float a01 = edge0.Dot( edge1 );
	float a11 = edge1.Dot( edge1 );
	float b0 = diff.Dot( edge0 );
	float b1 = diff.Dot( edge1 );
	float invDet = 1.0f / ( a00 * a11 - a01 * a01 );
	barycenter[0] = ( a11 * b0 - a01 * b1 ) * invDet;
	barycenter[1] = ( a00 * b1 - a01 * b0 ) * invDet;
	barycenter[2] = 1.0f - barycenter[0] - barycenter[1];
}

/*
================
TentativeCollision

Where a segment, moving by the velocity over the time step, meets a plane
(the points whose dot product with the normal is the constant): the first
and last times of contact and the points touching then. A segment that lies
in the plane gives both its ends.
================
*/
// FUNCTION: STBC 0x8266B0
int NiOBBLeaf::TentativeCollision( float deltaTime, const NiPoint3 &normal, float constant, const NiPoint3 &velocity,
							   const NiPoint3 &end0, const NiPoint3 &end1, float &time0, NiPoint3 &point0,
							   float &time1, NiPoint3 &point1 ) {
	const float minTime = 0.0f;
	const float maxTime = deltaTime;
	const float spanTime = maxTime - minTime;

	// the signed distances of the ends from the plane, at the start and the
	// end of the step
	float distance00 = normal.Dot( end0 + minTime * velocity ) - constant;
	float distance10 = normal.Dot( end1 + minTime * velocity ) - constant;
	float distance01 = normal.Dot( end0 + maxTime * velocity ) - constant;
	float distance11 = normal.Dot( end1 + maxTime * velocity ) - constant;

	float s, t;
	NiPoint3 point;

	int found = 0;

	// the segment at the start of the step
	if ( distance00 * distance10 <= 0.0f ) {
		if ( distance00 == 0.0f && distance10 == 0.0f ) {
			time0 = minTime;
			time1 = minTime;
			point0 = end0 + time0 * velocity;
			point1 = end1 + time1 * velocity;
			return 1;
		}
		time0 = minTime;
		s = distance00 / ( distance00 - distance10 );
		point0 = ( 1.0f - s ) * end0 + s * end1 + time0 * velocity;
		found = 1;
	}

	// the first end's path
	if ( distance00 * distance01 <= 0.0f ) {
		if ( distance00 == 0.0f && distance01 == 0.0f ) {
			time0 = minTime;
			time1 = maxTime;
			point0 = end0 + time0 * velocity;
			point1 = end0 + time1 * velocity;
			return 1;
		}
		t = minTime + spanTime * distance00 / ( distance00 - distance01 );
		point = end0 + t * velocity;
		if ( found ) {
			time1 = t;
			point1 = point;
			if ( time0 > time1 ) {
				t = time0;
				time0 = time1;
				time1 = t;
				point = point0;
				point0 = point1;
				point1 = point;
			}
			return 1;
		}
		time0 = t;
		point0 = point;
		found = 1;
	}

	// the second end's path
	if ( distance10 * distance11 <= 0.0f ) {
		if ( distance10 == 0.0f && distance11 == 0.0f ) {
			time0 = minTime;
			time1 = maxTime;
			point0 = end1 + time0 * velocity;
			point1 = end1 + time1 * velocity;
			return 1;
		}
		t = minTime + spanTime * distance10 / ( distance10 - distance11 );
		point = end1 + t * velocity;
		if ( found ) {
			time1 = t;
			point1 = point;
			if ( time0 > time1 ) {
				t = time0;
				time0 = time1;
				time1 = t;
				point = point0;
				point0 = point1;
				point1 = point;
			}
			return 1;
		}
		time0 = t;
		point0 = point;
		found = 1;
	}

	// the segment at the end of the step
	if ( distance01 * distance11 <= 0.0f ) {
		if ( distance01 == 0.0f && distance11 == 0.0f ) {
			time0 = maxTime;
			time1 = maxTime;
			point0 = end0 + time0 * velocity;
			point1 = end1 + time1 * velocity;
			return 1;
		}
		t = maxTime;
		s = distance01 / ( distance01 - distance11 );
		point = ( 1.0f - s ) * end0 + s * end1 + maxTime * velocity;
		if ( found ) {
			time1 = t;
			point1 = point;
			if ( time0 > time1 ) {
				t = time0;
				time0 = time1;
				time1 = t;
				point = point0;
				point0 = point1;
				point1 = point;
			}
			return 1;
		}
		time0 = t;
		point0 = point;
		found = 1;
	}

	if ( found ) {
		time1 = time0;
		point1 = point0;
		return 1;
	}

	return 0;
}

/*
================
TestEdgeTri

The first contact, earlier than time, of a moving edge with a triangle:
where the part of the edge's sweep that crosses the triangle's plane enters
the triangle. Returns nonzero when the edge meets the triangle, and lowers
time and moves point when the contact is earlier.
================
*/
// FUNCTION: STBC 0x8270E0
int NiOBBLeaf::TestEdgeTri( const NiPoint3 &end0, const NiPoint3 &end1, const NiPoint3 *const *triangle,
							const NiPoint3 &normal, float constant, const NiPoint3 &velocity, float deltaTime,
							float &time, NiPoint3 &point ) {
	float time0, time1;
	NiPoint3 point0, point1;
	if ( TentativeCollision( deltaTime, normal, constant, velocity, end0, end1, time0, point0, time1, point1 ) ) {
		float barycenter0[3], barycenter1[3];
		ComputeBarycenter( *triangle[0], *triangle[1], *triangle[2], point0, barycenter0 );
		ComputeBarycenter( *triangle[0], *triangle[1], *triangle[2], point1, barycenter1 );

		// a contact outside the triangle moves along the sweep, to where the
		// edge crosses into the triangle
		float s, t, a0, a1, a2;

		if ( barycenter0[0] < 0.0f ) {
			if ( barycenter1[0] < 0.0f ) {
				return 0;
			}

			s = barycenter0[0] / ( barycenter0[0] - barycenter1[0] );
			a1 = ( 1 - s ) * barycenter0[1] + s * barycenter1[1];
			a2 = ( 1 - s ) * barycenter0[2] + s * barycenter1[2];
			if ( a1 >= 0.0f && a2 >= 0.0f ) {
				t = time0 * ( 1 - s ) + time1 * s;
				if ( t < time ) {
					time = t;
					point = point0 * ( 1 - s ) + point1 * s;
				}
				return 1;
			} else {
				return 0;
			}
		}

		if ( barycenter0[1] < 0.0f ) {
			if ( barycenter1[1] < 0.0f ) {
				return 0;
			}

			s = barycenter0[1] / ( barycenter0[1] - barycenter1[1] );
			a0 = ( 1 - s ) * barycenter0[0] + s * barycenter1[0];
			a2 = ( 1 - s ) * barycenter0[2] + s * barycenter1[2];
			if ( a0 >= 0.0f && a2 >= 0.0f ) {
				t = time0 * ( 1 - s ) + time1 * s;
				if ( t < time ) {
					time = t;
					point = point0 * ( 1 - s ) + point1 * s;
				}
				return 1;
			} else {
				return 0;
			}
		}

		if ( barycenter0[2] < 0.0f ) {
			if ( barycenter1[2] < 0.0f ) {
				return 0;
			}

			s = barycenter0[2] / ( barycenter0[2] - barycenter1[2] );
			a0 = ( 1 - s ) * barycenter0[0] + s * barycenter1[0];
			a1 = ( 1 - s ) * barycenter0[1] + s * barycenter1[1];
			if ( a0 >= 0.0f && a1 >= 0.0f ) {
				t = time0 * ( 1 - s ) + time1 * s;
				if ( t < time ) {
					time = t;
					point = point0 * ( 1 - s ) + point1 * s;
				}
				return 1;
			} else {
				return 0;
			}
		}

		if ( time0 < time ) {
			time = time0;
			point = point0;
		}

		return 1;
	}

	return 0;
}

/*
================
FindIntersection

The first contact of the edges of one triangle, moving by the velocity
relative to the other, with the other triangle.
================
*/
// FUNCTION: STBC 0x827520
int NiOBBLeaf::FindIntersection( const NiPoint3 *const *edges, const NiPoint3 *const *triangle,
								const NiPoint3 &velocity, float deltaTime, float &time, NiPoint3 &point ) {
	NiPoint3 edge1 = *triangle[1] - *triangle[0];
	NiPoint3 edge2 = *triangle[2] - *triangle[0];
	NiPoint3 normal = edge1.Cross( edge2 );
	float constant = normal.Dot( *triangle[0] );
	int found = TestEdgeTri( *edges[0], *edges[1], triangle, normal, constant, velocity, deltaTime, time, point );
	found |= TestEdgeTri( *edges[0], *edges[2], triangle, normal, constant, velocity, deltaTime, time, point );
	found |= TestEdgeTri( *edges[1], *edges[2], triangle, normal, constant, velocity, deltaTime, time, point );
	return found;
}

/*
================
NiOBBLeaf::TestTriangles

Whether two leaves' triangles meet while their shapes move over the step:
each triangle's edges are swept against the other triangle, and the earlier
contact gives the time and the point, moved with the shape whose triangle
was hit.
================
*/
// FUNCTION: STBC 0x827650
int NiOBBLeaf::TestTriangles( NiOBBLeaf *leaf0, NiOBBLeaf *leaf1, float &time, NiPoint3 &point ) {
	NiOBBRoot *root0 = leaf0->box.root;
	float deltaTime = root0->deltaTime;
	const NiPoint3 &velocity1 = leaf1->box.root->velocity;

	NiPoint3 velocity;
	float time0 = FLT_MAX;
	velocity = root0->velocity - velocity1;
	NiPoint3 point0;
	int found0 = FindIntersection( leaf0->vertex, leaf1->vertex, velocity, deltaTime, time0, point0 );

	float time1 = FLT_MAX;
	velocity = velocity1 - root0->velocity;
	NiPoint3 point1;
	int found1 = FindIntersection( leaf1->vertex, leaf0->vertex, velocity, deltaTime, time1, point1 );

	if ( found0 ) {
		if ( found1 ) {
			if ( time0 <= time1 ) {
				time = time0;
				point = point0 + time0 * velocity1;
				return 1;
			}
			time = time1;
			point = point1 + time1 * root0->velocity;
			return 1;
		}
		time = time0;
		point = point0 + time0 * velocity1;
		return 1;
	}
	if ( found1 ) {
		time = time1;
		point = point1 + time1 * root0->velocity;
		return 1;
	}
	return 0;
}

/*
================
NiOBBLeaf::FaceFaceCollision

Two leaves' triangles, as moving: when they meet, the contact's time,
point and the boxes' normals there go to the collision callback.
================
*/
// FUNCTION: STBC 0x827930
// Gamebryo 1.2.2: NiOBBLeaf::FaceFaceCollision, without the bCollision
// result and the triangle pointers. Nothing in retail calls it: FindCollisions
// has the same body written in place
int NiOBBLeaf::FaceFaceCollision( NiOBBLeaf *other, NiAVObject *root0, NiAVObject *root1, NiAVObject *object0,
								  NiAVObject *object1 ) {
	NiCollisionIntersect intersect;
	intersect.root0 = root0;
	intersect.root1 = root1;
	intersect.object0 = object0;
	intersect.object1 = object1;

	int intersects = TestTriangles( this, other, intersect.time, intersect.point );
	if ( intersects ) {
		box.GetNormal( intersect.point, intersect.normal0 );
		other->box.GetNormal( intersect.point, intersect.normal1 );
		return CollisionCallback( intersect );
	} else {
		return CONTINUE_COLLISIONS;
	}
}

/*
================
NiOBBLeaf::TestCollision

A leaf descends only the other tree.
================
*/
// FUNCTION: STBC 0x8279C0
// Gamebryo 1.2.2: NiOBBLeaf::TestCollision
bool NiOBBLeaf::TestCollision( NiOBBNode *other, int depth, int otherDepth ) {
	if ( !box.TestIntersection( other->box ) ) {
		return false;
	}
	if ( !other->IsLeaf() ) {
		if ( otherDepth == 0 || !other->HasChildren() ) {
			return true;
		} else {
			int otherDepthLess = otherDepth - 1;
			return TestCollision( other->GetLeft(), depth, otherDepthLess ) || TestCollision( other->GetRight(), depth, otherDepthLess );
		}
	}

	return true;
}

/*
================
NiOBBLeaf::FindCollisions

Two leaves past both depth limits (negative depths) test their triangles
exactly; otherwise the boxes are the contact.
================
*/
// FUNCTION: STBC 0x827A50
// Gamebryo 1.2.2: NiOBBLeaf::FindCollisions (FaceFaceCollision's body written in place: the
// unit is /Ob1, and FaceFaceCollision's own copy, 0x827930, has no caller)
int NiOBBLeaf::FindCollisions( NiOBBNode *other, NiAVObject *root0, NiAVObject *root1, NiAVObject *object0,
							   NiAVObject *object1, int depth, int otherDepth, unsigned int stamp_,
							   unsigned int otherStamp ) {
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

	if ( !other->IsLeaf() ) {
		if ( otherDepth == 0 || !other->HasChildren() ) {
			return BoxBoxCollision( other, root0, root1, object0, object1 );
		} else {
			int otherDepthLess = otherDepth - 1;
			int result = FindCollisions( other->GetLeft(), root0, root1, object0, object1, depth, otherDepthLess, stamp_, otherStamp );
			if ( result == TERMINATE_COLLISIONS ) {
				return result;
			}

			result = FindCollisions( other->GetRight(), root0, root1, object0, object1, depth, otherDepthLess, stamp_, otherStamp );
			if ( result == TERMINATE_COLLISIONS ) {
				return result;
			}
		}
		return CONTINUE_COLLISIONS;
	}

	if ( depth >= 0 || otherDepth >= 0 ) {
		return BoxBoxCollision( other, root0, root1, object0, object1 );
	}

	// both past their depth limits: the triangles themselves, as moving
	NiCollisionIntersect intersect;
	intersect.root0 = root0;
	intersect.root1 = root1;
	intersect.object0 = object0;
	intersect.object1 = object1;

	int intersects = TestTriangles( this, (NiOBBLeaf *)other, intersect.time, intersect.point );
	if ( intersects ) {
		box.GetNormal( intersect.point, intersect.normal0 );
		other->box.GetNormal( intersect.point, intersect.normal1 );
		return CollisionCallback( intersect );
	} else {
		return CONTINUE_COLLISIONS;
	}
}

/*
================
NiOBBLeaf::FindIntersections

Records a hit of the triangle in the pick; the search goes on.
================
*/
// FUNCTION: STBC 0x827C50
// Gamebryo 1.2.2: NiOBBLeaf::FindIntersections
int NiOBBLeaf::FindIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, bool &found,
								  NiAVObject *object, unsigned int stamp_ ) {
	if ( stamp_ != stamp ) {
		box.Transform( object->GetWorldRotate(), object->GetWorldTranslate(), object->GetWorldScale() );
		stamp = stamp_;
	}

	NiPoint3 intersect;
	float distance, u, v;
	found = NiTriBasedGeom::IntersectTriangle( origin, direction, *vertex[0], *vertex[1], *vertex[2], pick.GetFrontOnly(),
											   intersect, distance, u, v );
	if ( found ) {
		NiPick::Record *record = new NiPick::Record( object );
		record->SetIntersection( intersect );
		record->SetDistance( distance );
		if ( pick.GetReturnNormal() ) {
			NiPoint3 edge0 = *vertex[1] - *vertex[0];
			NiPoint3 edge1 = *vertex[2] - *vertex[0];
			NiPoint3 normal = edge0.Cross( edge1 );
			normal.Unitize();
			record->SetNormal( normal );
		}
		pick.Add( record );
	}

	return CONTINUE_COLLISIONS;
}
