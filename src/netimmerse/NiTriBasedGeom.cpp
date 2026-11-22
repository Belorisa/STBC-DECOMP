// NiTriBasedGeom.cpp -- geometry objects drawn as triangles

#include "NiRTLib.h"
#include <assert.h>
#include <math.h>

#include "NiCollisionRecord.h"
#include "NiNode.h"
#include "NiOBBRoot.h"
#include "NiPick.h"
#include "NiStream.h"
#include "NiTriBasedGeom.h"
#include "NiTriIntersect.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007f1110 _$E35
// FUNCTION: STBC 0x007f1120 _$E46
// FUNCTION: STBC 0x007f1130 _$E50
// FUNCTION: STBC 0x007f1140 _$E54
// FUNCTION: STBC 0x007f1150 _$E58
// FUNCTION: STBC 0x007f1160 _$E177
// FUNCTION: STBC 0x007f1170 _$E182
// FUNCTION: STBC 0x007f1180 _$E186
// FUNCTION: STBC 0x007f11f0 _$E194
// FUNCTION: STBC 0x007f1200 _$E201
// FUNCTION: STBC 0x007f1210 _$E205
// FUNCTION: STBC 0x007f1250 _$E209

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007f10f0 _$E223
// FUNCTION: STBC 0x007f0f90 _$E17
// FUNCTION: STBC 0x007f0fb0 _$E15
// FUNCTION: STBC 0x007f0fc0 _$E23
// FUNCTION: STBC 0x007f0fe0 _$E21

// FUNCTION: STBC 0x007ef0a0 _$E216
// FUNCTION: STBC 0x007ef0c0 _$E213
// DATA: STBC 0x9A1AF8 ?rtti@NiTriBasedGeom@@2VNiRTTI@@B
const NiRTTI NiTriBasedGeom::rtti( "NiTriBasedGeom", &NiGeometry::rtti );

/*
================
NiTriBasedGeom::Register
================
*/
// FUNCTION: STBC 0x7EF0E0 ?Register@NiTriBasedGeom@@SAHXZ
NiImplementRegister( NiTriBasedGeom )

/*
================
NiTriBasedGeom::NiTriBasedGeom
================
*/
// FUNCTION: STBC 0x7EF230 ??0NiTriBasedGeom@@QAE@PAVNiTriBasedGeomData@@@Z
NiTriBasedGeom::NiTriBasedGeom( NiTriBasedGeomData *data ) : NiGeometry( data ) {
	obbRoot = 0;
}

/*
================
NiTriBasedGeom::NiTriBasedGeom
================
*/
// FUNCTION: STBC 0x7EF260 ??0NiTriBasedGeom@@QAE@XZ
NiTriBasedGeom::NiTriBasedGeom() {
	obbRoot = 0;
}

/*
================
NiTriBasedGeom::~NiTriBasedGeom
================
*/
// FUNCTION: STBC 0x7EF280 ??1NiTriBasedGeom@@UAE@XZ
NiTriBasedGeom::~NiTriBasedGeom() {
	delete obbRoot;
}

/*
================
NiTriBasedGeom::SetModelData

The bounding box tree belongs to the old data.
================
*/
// FUNCTION: STBC 0x7EF2E0
void NiTriBasedGeom::SetModelData( NiGeometryData *data ) {
	NiGeometry::SetModelData( data );
	VirtualSlot24();
}

/*
================
NiTriBasedGeom::IntersectTriangle

Where a ray meets a triangle: the ray meets the triangle's plane at
distance along it (only from the front when frontOnly is set), and the
point is inside when its coordinates u and v along the two edges from
vertex0, scaled by the edges' Gram determinant, are not negative and sum
to no more than it. False when the ray runs along the plane, meets it
behind the origin, or the triangle is degenerate.
================
*/
// FUNCTION: STBC 0x7EF350 ?IntersectTriangle@NiTriBasedGeom@@SA_NABVNiPoint3@@0000_NAAV2@AAM33@Z
bool NiTriBasedGeom::IntersectTriangle( const NiPoint3 &origin, const NiPoint3 &dir, const NiPoint3 &vertex0,
										const NiPoint3 &vertex1, const NiPoint3 &vertex2, bool frontOnly,
										NiPoint3 &intersect, float &distance, float &u, float &v ) {
	const float tolerance = 1e-05f;

	NiPlane plane( vertex0, vertex1, vertex2 );
	float denom = plane.GetNormal() * dir;
	if ( frontOnly && denom > tolerance ) {
		return false;
	}
	if ( fabsf( denom ) <= tolerance ) {
		return false;
	}
	distance = -plane.Distance( origin ) / denom;
	if ( distance < 0.0f ) {
		return false;
	}
	intersect = distance * dir + origin;

	NiPoint3 edge1 = vertex1 - vertex0;
	NiPoint3 edge2 = vertex2 - vertex0;
	float edgeDot = edge2 * edge1;
	float invLength1 = 1.0f / ( edge1 * edge1 );
	float invLength2 = 1.0f / ( edge2 * edge2 );
	float ratio1 = invLength1 * edgeDot;
	float ratio2 = invLength2 * edgeDot;
	float det = fabsf( 1.0f - ratio2 * ratio1 );
	if ( fabsf( det ) < tolerance ) {
		return false;
	}

	NiPoint3 diff = vertex0 - intersect;
	float along1 = ( diff * edge1 ) * invLength1;
	float along2 = ( edge2 * diff ) * invLength2;
	u = along2 * ratio1 - along1;
	v = along1 * ratio2 - along2;
	if ( u < 0.0f || v < 0.0f || v + u > det ) {
		return false;
	}
	float invDet = 1.0f / det;
	u *= invDet;
	v *= invDet;
	return true;
}

/*
================
NiTriBasedGeom::FindIntersections

Records each triangle the ray hits, in model space, with the interpolated
texture coordinates, normal and color the pick asks for; a bound pick
records the shape itself. A pick that asks for it goes through the
bounding box tree of a shape that propagates through its boxes.
Gamebryo 1.2.2: FindIntersectionsTriBasedGeom.
================
*/
// FUNCTION: STBC 0x7EF600 ?FindIntersections@NiTriBasedGeom@@UAE_NABVNiPoint3@@0AAVNiPick@@@Z
bool NiTriBasedGeom::FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick ) {
	if ( pick.GetObserveAppCullFlag() && GetAppCulled() ) {
		return false;
	}
	if ( !TestRayBound( origin, dir ) ) {
		return false;
	}

	// return if bound intersect
	if ( pick.GetIntersectType() == NiPick::BOUND_INTERSECT ) {
		NiPick::Record *record = new NiPick::Record( this );
		assert( record );
		pick.Add( record );
		return true;
	}

	if ( pick.GetUnknown12() && GetPropagationMode() == PROPAGATE_USE_OBBS ) {
		VirtualSlot23( 1 );
		if ( !obbRoot ) {
			return false;
		}
		obbRoot->Transform( worldRotate, worldTranslate, worldScale );
		bool found;
		obbRoot->FindIntersections( origin, dir, pick, found, this, obbRoot->GetChangeStamp() );
		return found;
	}

	// find the ray's model space origin and direction
	NiPoint3 diff = origin - worldTranslate;
	float invWorldScale = 1.0f / worldScale;
	NiPoint3 modelOrigin = diff * worldRotate * invWorldScale;
	NiPoint3 modelDir = dir * worldRotate * invWorldScale;

	bool found = false;
	unsigned short triangles = ( (NiTriBasedGeomData *)GetModelData() )->GetActiveTriangleCount();

	for ( unsigned short i = 0; i < triangles; i++ ) {
		unsigned short i0, i1, i2;
		GetTriangleIndices( i, i0, i1, i2 );

		NiPoint3 intersect;
		NiPoint3 *vertices = GetPackedVertices();
		float lineParam, triParam1, triParam2;

		bool intersects = IntersectTriangle( modelOrigin, modelDir, vertices[i0], vertices[i1], vertices[i2],
											 pick.GetFrontOnly(), intersect, lineParam, triParam1, triParam2 );

		if ( intersects ) {
			found = true;
			NiPick::Record *record = new NiPick::Record( this );

			if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
				intersect = worldScale * ( worldRotate * intersect ) + worldTranslate;
			}
			record->SetIntersection( intersect );

			record->SetDistance( lineParam );
			record->SetTriangleIndex( i );
			record->SetVertexIndices( i0, i1, i2 );

			float triParam0 = 1.0f - ( triParam1 + triParam2 );

			if ( pick.GetReturnTexture() ) {
				NiPoint2 *textures = GetPackedTextures();
				if ( textures ) {
					NiPoint2 texture = triParam0 * textures[i0] + triParam1 * textures[i1] + triParam2 * textures[i2];
					record->SetTexture( texture );
				} else {
					record->SetTexture( NiPoint2::ZERO );
				}
			} else {
				record->SetTexture( NiPoint2::ZERO );
			}

			if ( pick.GetReturnNormal() ) {
				NiPoint3 normal;
				NiPoint3 *normals = GetPackedNormals();

				if ( pick.GetReturnSmoothNormal() && normals ) {
					normal = triParam0 * normals[i0] + triParam1 * normals[i1] + triParam2 * normals[i2];
				} else {
					NiPoint3 edge0, edge1;

					edge0 = vertices[i1] - vertices[i0];
					edge1 = vertices[i2] - vertices[i0];
					normal = edge0.Cross( edge1 );
				}
				normal.Unitize();

				if ( pick.GetCoordinateType() == NiPick::WORLD_COORDINATES ) {
					normal = worldRotate * normal;
				}

				record->SetNormal( normal );
			} else {
				record->SetNormal( NiPoint3::ZERO );
			}

			if ( pick.GetReturnColor() ) {
				NiColorA *colors = GetPackedColors();
				if ( colors ) {
					NiColorA color = triParam0 * colors[i0] + triParam1 * colors[i1] + triParam2 * colors[i2];
					record->SetColor( color );
				} else {
					record->SetColor( NiColorA::WHITE );
				}
			} else {
				record->SetColor( NiColorA::WHITE );
			}

			pick.Add( record );

			if ( pick.GetPickType() == NiPick::FIND_FIRST && pick.GetSortType() == NiPick::NO_SORT ) {
				return true;
			}
		}
	}

	return found;
}

/*
================
NiTriBasedGeom::VirtualSlot24
================
*/
// FUNCTION: STBC 0x7F0090
void NiTriBasedGeom::VirtualSlot24() {
	delete obbRoot;
	obbRoot = 0;
}

/*
================
NiTriBasedGeom::TestTriangles

Every world triangle of this object against every world triangle of the
other, both moving.
================
*/
// FUNCTION: STBC 0x7F00B0
bool NiTriBasedGeom::TestTriangles( float deltaTime, NiTriBasedGeom *other ) {
	// thisTriCount, otherTriCount, thisTriangle, otherTriangle: (names invented; fit the /Od frame order)
	unsigned short thisTriCount = GetTriangleCount();
	unsigned short otherTriCount = other->GetTriangleCount();

	for ( unsigned short i = 0; i < thisTriCount; i++ ) {
		for ( unsigned short j = 0; j < otherTriCount; j++ ) {
			NiPoint3 *thisTriangle[3], *otherTriangle[3];
			GetWorldTriangle( i, thisTriangle[0], thisTriangle[1], thisTriangle[2] );
			other->GetWorldTriangle( j, otherTriangle[0], otherTriangle[1], otherTriangle[2] );
			NiTriIntersect intersect( thisTriangle, worldVelocity, otherTriangle, other->worldVelocity );
			if ( intersect.Test( deltaTime ) ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
NiTriBasedGeom::FindTriangles

Reports each pair of world triangles that meet; a callback's nonzero
return stops the search.
================
*/
// FUNCTION: STBC 0x7F01B0
int NiTriBasedGeom::FindTriangles( float deltaTime, NiTriBasedGeom *other, bool calcNormals,
								   NiCollisionIntersect &intersect ) {
	// thisTriCount, otherTriCount, thisTriangle, otherTriangle, triangleIntersection: (names invented; fit the /Od frame order)
	unsigned short thisTriCount = GetTriangleCount();
	unsigned short otherTriCount = other->GetTriangleCount();

	for ( unsigned short i = 0; i < thisTriCount; i++ ) {
		for ( unsigned short j = 0; j < otherTriCount; j++ ) {
			NiPoint3 *thisTriangle[3], *otherTriangle[3];
			GetWorldTriangle( i, thisTriangle[0], thisTriangle[1], thisTriangle[2] );
			other->GetWorldTriangle( j, otherTriangle[0], otherTriangle[1], otherTriangle[2] );
			NiTriIntersect triangleIntersection( thisTriangle, worldVelocity, otherTriangle, other->worldVelocity );
			if ( triangleIntersection.Find( deltaTime, intersect.time, intersect.point, calcNormals, &intersect.normal0,
									&intersect.normal1 ) ) {
				int result = ReportCollision( intersect );
				if ( result == TERMINATE_COLLISIONS ) {
					return TERMINATE_COLLISIONS;
				}
			}
		}
	}
	return CONTINUE_COLLISIONS;
}

/*
================
NiTriBasedGeom::TestVolumeTriangles

Every world triangle of this object against the other object's world
volume; an object without a model volume never meets the triangles.
================
*/
// FUNCTION: STBC 0x7F0300
bool NiTriBasedGeom::TestVolumeTriangles( float deltaTime, NiAVObject *other ) {
	// triCount, pos0, pos1, pos2: (names invented; fit the /Od frame order)
	if ( !other->GetModelABV() ) {
		return false;
	}

	NiBoundingVolume *volume = other->GetWorldABV();
	assert( volume );
	volume->UpdateWorldData( *other->GetModelABV(), other->GetWorldRotate(), other->GetWorldTranslate(),
							 other->GetWorldScale() );

	unsigned short triCount = GetTriangleCount();
	for ( unsigned short i = 0; i < triCount; i++ ) {
		NiPoint3 *pos0, *pos1, *pos2;
		GetWorldTriangle( i, pos0, pos1, pos2 );
		if ( NiBoundingVolume::TestTriIntersect( deltaTime, *volume, other->GetWorldVelocity(), *pos0, *pos1,
												 *pos2, worldVelocity ) ) {
			return true;
		}
	}
	return false;
}

/*
================
NiTriBasedGeom::FindVolumeTriangles

Reports each world triangle that meets the other object's world volume,
the volume's normal first.
================
*/
// FUNCTION: STBC 0x7F03C0
int NiTriBasedGeom::FindVolumeTriangles( float deltaTime, NiAVObject *other, bool calcNormals,
										 NiCollisionIntersect &intersect ) {
	// triCount, pos0, pos1, pos2: (names invented; fit the /Od frame order)
	if ( !other->GetModelABV() ) {
		return CONTINUE_COLLISIONS;
	}

	NiBoundingVolume *volume = other->GetWorldABV();
	assert( volume );
	volume->UpdateWorldData( *other->GetModelABV(), other->GetWorldRotate(), other->GetWorldTranslate(),
							 other->GetWorldScale() );

	unsigned short triCount = GetTriangleCount();
	for ( unsigned short i = 0; i < triCount; i++ ) {
		NiPoint3 *pos0, *pos1, *pos2;
		GetWorldTriangle( i, pos0, pos1, pos2 );
		if ( NiBoundingVolume::FindTriIntersect( deltaTime, *volume, other->GetWorldVelocity(), *pos0, *pos1,
												 *pos2, worldVelocity, intersect.time, intersect.point, calcNormals,
												 intersect.normal1, intersect.normal0 ) ) {
			int result = ReportCollision( intersect );
			if ( result == TERMINATE_COLLISIONS ) {
				return TERMINATE_COLLISIONS;
			}
		}
	}
	return CONTINUE_COLLISIONS;
}

/*
================
NiTriBasedGeom::TestCollisions

The bounds first; then by the two objects' propagation modes: a node
that propagates is descended, triangles meet triangles or volumes, and
two bounding box trees are tested against each other.
================
*/
// FUNCTION: STBC 0x7F04B0
bool NiTriBasedGeom::TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	// AnimOnly.exe 0x431052 (NiTriBasedGeom.cpp): each case returns its
	// own result and creates a missing model volume in place
	// (its GetVertices is NiGeometry's non-virtual inline, a sibling of the
	// virtual DamageTool.exe names GetPackedVertices; until that rename the
	// model data's array is read directly, the same release code)
	assert( record0.GetAVObject() == this );
	NiAVObject *other = record1.GetAVObject();
	if ( !TestBoundIntersect( deltaTime, other ) ) {
		return false;
	}

	if ( NiIsKindOf( NiNode, other ) ) {
		NiNode *node = (NiNode *)other;
		if ( node->GetPropagationMode() != PROPAGATE_NONE ) {
			return other->TestCollisions( deltaTime, record1, record0 );
		}
		switch ( GetPropagationMode() ) {
		case PROPAGATE_NONE:
			return true;
		case PROPAGATE_USE_TRIANGLES:
			UpdateWorldVertices();
			return TestVolumeTriangles( deltaTime, node );
		case PROPAGATE_USE_OBBS:
			if ( !GetModelABV() ) {
				NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( GetVertexCount(), GetVertices() );
				if ( volume ) {
					SetModelABV( volume );
					return TestBoundIntersect( deltaTime, other );
				}
			}
			return true;
		}
		return true;
	}

	NiTriBasedGeom *otherGeom = NiDynamicCast( NiTriBasedGeom, other );
	if ( otherGeom ) {
		switch ( GetPropagationMode() ) {
		case PROPAGATE_NONE:
			switch ( otherGeom->GetPropagationMode() ) {
			case PROPAGATE_NONE:
				return true;
			case PROPAGATE_USE_TRIANGLES:
				otherGeom->UpdateWorldVertices();
				return otherGeom->TestVolumeTriangles( deltaTime, this );
			case PROPAGATE_USE_OBBS:
				if ( !otherGeom->GetModelABV() ) {
					NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( otherGeom->GetVertexCount(),
																				 otherGeom->GetVertices() );
					if ( volume ) {
						otherGeom->SetModelABV( volume );
						return TestBoundIntersect( deltaTime, other );
					}
				}
				return true;
			}
			return true;
		case PROPAGATE_USE_TRIANGLES:
			UpdateWorldVertices();
			switch ( otherGeom->GetPropagationMode() ) {
			case PROPAGATE_NONE:
				return TestVolumeTriangles( deltaTime, otherGeom );
			case PROPAGATE_USE_TRIANGLES:
				otherGeom->UpdateWorldVertices();
				return TestTriangles( deltaTime, otherGeom );
			case PROPAGATE_USE_OBBS:
				if ( !otherGeom->GetModelABV() ) {
					NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( otherGeom->GetVertexCount(),
																				 otherGeom->GetVertices() );
					if ( volume ) {
						otherGeom->SetModelABV( volume );
					}
				}
				return TestVolumeTriangles( deltaTime, otherGeom );
			}
			return true;
		case PROPAGATE_USE_OBBS:
			switch ( otherGeom->GetPropagationMode() ) {
			case PROPAGATE_NONE:
				if ( !GetModelABV() ) {
					NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( GetVertexCount(), GetVertices() );
					if ( volume ) {
						SetModelABV( volume );
						return TestBoundIntersect( deltaTime, other );
					}
				}
				return true;
			case PROPAGATE_USE_TRIANGLES:
				otherGeom->UpdateWorldVertices();
				if ( !GetModelABV() ) {
					NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( GetVertexCount(), GetVertices() );
					if ( volume ) {
						SetModelABV( volume );
					}
				}
				return otherGeom->TestVolumeTriangles( deltaTime, this );
			case PROPAGATE_USE_OBBS: {
				VirtualSlot23( record0.GetUnknown0C() );
				otherGeom->VirtualSlot23( record1.GetUnknown0C() );
				if ( !obbRoot ) {
					return false;
				}
				obbRoot->SetDeltaTime( deltaTime );
				obbRoot->SetVelocity( worldVelocity );
				obbRoot->Transform( worldRotate, worldTranslate, worldScale );
				NiOBBRoot *otherRoot = otherGeom->obbRoot;
				if ( !otherRoot ) {
					return false;
				}
				otherRoot->SetDeltaTime( deltaTime );
				otherRoot->SetVelocity( otherGeom->worldVelocity );
				otherRoot->Transform( otherGeom->worldRotate, otherGeom->worldTranslate, otherGeom->worldScale );
				UpdateWorldVertices();
				otherGeom->UpdateWorldVertices();
				return obbRoot->TestCollision( otherRoot, record0.GetUnknown08(), record1.GetUnknown08() );
			}
			}
			return true;
		}
		return true;
	}

	assert( false );
	return true;
}

/*
================
NiTriBasedGeom::FindCollisions

As TestCollisions, reporting what meets to the roots' callbacks.
================
*/
// FUNCTION: STBC 0x7F0820
int NiTriBasedGeom::FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 ) {
	// collision: (name invented; fits the /Od frame order)
	// AnimOnly.exe 0x431592 (NiTriBasedGeom.cpp), written as TestCollisions
	assert( record0.GetAVObject() == this );
	NiAVObject *other = record1.GetAVObject();
	if ( !TestBoundIntersect( deltaTime, other ) ) {
		return CONTINUE_COLLISIONS;
	}

	NiCollisionIntersect collision;
	collision.root0 = record0.GetRoot();
	collision.object0 = this;
	collision.root1 = record1.GetRoot();
	collision.object1 = other;

	NiTriBasedGeom *otherGeom = NULL;
	if ( NiIsKindOf( NiNode, other ) ) {
		NiNode *node = (NiNode *)other;
		if ( node->GetPropagationMode() != PROPAGATE_NONE ) {
			return other->FindCollisions( deltaTime, record1, record0 );
		}
		switch ( GetPropagationMode() ) {
		case PROPAGATE_NONE:
			if ( FindBoundIntersect( deltaTime, other, collision.time, collision.point, true, collision.normal0,
									 collision.normal1 ) ) {
				return ReportCollision( collision );
			}
			return CONTINUE_COLLISIONS;
		case PROPAGATE_USE_TRIANGLES:
			UpdateWorldVertices();
			return FindVolumeTriangles( deltaTime, node, true, collision );
		case PROPAGATE_USE_OBBS:
			if ( !GetModelABV() ) {
				NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( GetVertexCount(), GetVertices() );
				if ( volume ) {
					SetModelABV( volume );
				}
			}
			if ( FindBoundIntersect( deltaTime, other, collision.time, collision.point, true, collision.normal0,
									 collision.normal1 ) ) {
				return ReportCollision( collision );
			}
			return CONTINUE_COLLISIONS;
		}
		return CONTINUE_COLLISIONS;
	}

	otherGeom = NiDynamicCast( NiTriBasedGeom, other );
	if ( otherGeom ) {
		switch ( GetPropagationMode() ) {
		case PROPAGATE_NONE:
			switch ( otherGeom->GetPropagationMode() ) {
			case PROPAGATE_NONE:
				if ( FindBoundIntersect( deltaTime, other, collision.time, collision.point, true, collision.normal0,
										 collision.normal1 ) ) {
					return ReportCollision( collision );
				}
				return CONTINUE_COLLISIONS;
			case PROPAGATE_USE_TRIANGLES:
				otherGeom->UpdateWorldVertices();
				return otherGeom->FindVolumeTriangles( deltaTime, this, true, collision );
			case PROPAGATE_USE_OBBS:
				if ( !otherGeom->GetModelABV() ) {
					NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( otherGeom->GetVertexCount(),
																				 otherGeom->GetVertices() );
					if ( volume ) {
						otherGeom->SetModelABV( volume );
					}
				}
				if ( FindBoundIntersect( deltaTime, other, collision.time, collision.point, true, collision.normal0,
										 collision.normal1 ) ) {
					return ReportCollision( collision );
				}
				return CONTINUE_COLLISIONS;
			}
			break;
		case PROPAGATE_USE_TRIANGLES:
			UpdateWorldVertices();
			switch ( otherGeom->GetPropagationMode() ) {
			case PROPAGATE_NONE:
				return FindVolumeTriangles( deltaTime, otherGeom, true, collision );
			case PROPAGATE_USE_TRIANGLES:
				otherGeom->UpdateWorldVertices();
				return FindTriangles( deltaTime, otherGeom, true, collision );
			case PROPAGATE_USE_OBBS:
				if ( !otherGeom->GetModelABV() ) {
					NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( otherGeom->GetVertexCount(),
																				 otherGeom->GetVertices() );
					if ( volume ) {
						otherGeom->SetModelABV( volume );
					}
				}
				return FindVolumeTriangles( deltaTime, otherGeom, true, collision );
			}
			break;
		case PROPAGATE_USE_OBBS:
			switch ( otherGeom->GetPropagationMode() ) {
			case PROPAGATE_NONE:
				if ( !GetModelABV() ) {
					NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( GetVertexCount(), GetVertices() );
					if ( volume ) {
						SetModelABV( volume );
					}
				}
				if ( FindBoundIntersect( deltaTime, other, collision.time, collision.point, true, collision.normal0,
										 collision.normal1 ) ) {
					return ReportCollision( collision );
				}
				return CONTINUE_COLLISIONS;
			case PROPAGATE_USE_TRIANGLES:
				otherGeom->UpdateWorldVertices();
				if ( !GetModelABV() ) {
					NiBoundingVolume *volume = NiBoundingVolume::CreateFromData( GetVertexCount(), GetVertices() );
					if ( volume ) {
						SetModelABV( volume );
					}
				}
				return otherGeom->FindVolumeTriangles( deltaTime, this, true, collision );
			case PROPAGATE_USE_OBBS: {
				VirtualSlot23( record0.GetUnknown0C() );
				otherGeom->VirtualSlot23( record1.GetUnknown0C() );
				if ( !obbRoot ) {
					return CONTINUE_COLLISIONS;
				}
				obbRoot->SetDeltaTime( deltaTime );
				obbRoot->SetVelocity( worldVelocity );
				obbRoot->Transform( worldRotate, worldTranslate, worldScale );
				NiOBBRoot *otherRoot = otherGeom->obbRoot;
				if ( !otherRoot ) {
					return CONTINUE_COLLISIONS;
				}
				otherRoot->SetDeltaTime( deltaTime );
				otherRoot->SetVelocity( otherGeom->worldVelocity );
				otherRoot->Transform( otherGeom->worldRotate, otherGeom->worldTranslate, otherGeom->worldScale );
				UpdateWorldVertices();
				otherGeom->UpdateWorldVertices();
				if ( obbRoot->FindCollisions( otherRoot, record0.GetRoot(), record1.GetRoot(), record0.GetAVObject(),
											  record1.GetAVObject(), record0.GetUnknown08(),
											  record1.GetUnknown08() ) == TERMINATE_COLLISIONS ) {
					return TERMINATE_COLLISIONS;
				}
				break;
			}
			}
			break;
		}
		return CONTINUE_COLLISIONS;
	}

	assert( false );
	return CONTINUE_COLLISIONS;
}

/*
================
NiTriBasedGeom::CreateClone

NiTriBasedGeom itself is never cloned; the derived classes create the
clone.
================
*/
// FUNCTION: STBC 0x7F0D00
NiObject *NiTriBasedGeom::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiTriBasedGeom::CopyMembers

A clone of an object with a bounding box tree builds its own.
================
*/
// FUNCTION: STBC 0x7F0D10
void NiTriBasedGeom::CopyMembers( NiTriBasedGeom *dest ) {
	NiGeometry::CopyMembers( dest );
	if ( obbRoot ) {
		dest->VirtualSlot23( 1 );
	}
}

/*
================
NiTriBasedGeom::ProcessClone
================
*/
// FUNCTION: STBC 0x7F0D40
void NiTriBasedGeom::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiTriBasedGeom, clone ) );
	NiGeometry::ProcessClone( clone );
}

/*
================
NiTriBasedGeom::CreateObject

The class is abstract: the stream creates nothing for it.
================
*/
// FUNCTION: STBC 0x7F0D50
void NiTriBasedGeom::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiTriBasedGeom::LoadBinary
================
*/
// FUNCTION: STBC 0x7F0D60
void NiTriBasedGeom::LoadBinary( NiStream &stream ) {
	NiGeometry::LoadBinary( stream );
}

/*
================
NiTriBasedGeom::LinkObject
================
*/
// FUNCTION: STBC 0x7F0D70
void NiTriBasedGeom::LinkObject( NiStream &stream ) {
	NiGeometry::LinkObject( stream );
}

/*
================
NiTriBasedGeom::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7F0D80
bool NiTriBasedGeom::RegisterStreamables( NiStream &stream ) {
	return NiGeometry::RegisterStreamables( stream );
}

/*
================
NiTriBasedGeom::SaveBinary
================
*/
// FUNCTION: STBC 0x7F0D90
void NiTriBasedGeom::SaveBinary( NiStream &stream ) {
	NiGeometry::SaveBinary( stream );
}

/*
================
NiTriBasedGeom::IsEqual
================
*/
// FUNCTION: STBC 0x7F0DA0
bool NiTriBasedGeom::IsEqual( NiObject *object ) {
	return NiGeometry::IsEqual( object );
}

/*
================
NiTriBasedGeom::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7F0DB0
void NiTriBasedGeom::GetViewerStrings( NiTArray<char *> *strings ) {
	NiGeometry::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_pOBBRoot", obbRoot ) );
}
