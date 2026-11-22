/*
===============================================================================

	OBBRoot.cpp

	The OBB trees of a model's triangle shapes (OBBRoot) and the model that
	gathers them for its tests (OBBModel).

===============================================================================
*/

#include <iostream>
#include "OBBRoot.h"
#include "OBBModel.h"
#include "NiMain.h"
#include "NiNode.h"
#include "NiPick.h"
#include "NiPlane.h"
#include "NiTriShapeData.h"
#include "NiTriShape.h"
#include "NiMath.h"

#include <string.h>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00467a30 _$E32
// FUNCTION: STBC 0x00467a40 _$E29
// FUNCTION: STBC 0x00467a50 _$E31
// FUNCTION: STBC 0x00467a60 _$E30
// FUNCTION: STBC 0x00467a70 _$E38
// FUNCTION: STBC 0x00467a80 _$E35
// FUNCTION: STBC 0x00467a90 _$E37
// FUNCTION: STBC 0x00467aa0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0046ac40 _$E45
// FUNCTION: STBC 0x0046ac50 _$E44
// FUNCTION: STBC 0x0046b000 _$E56
// FUNCTION: STBC 0x0046b010 _$E55
// FUNCTION: STBC 0x0046b560 _$E67
// FUNCTION: STBC 0x0046b570 _$E66
// FUNCTION: STBC 0x0046b580 _$E71
// FUNCTION: STBC 0x0046b590 _$E70
// FUNCTION: STBC 0x0046c210 _$E75
// FUNCTION: STBC 0x0046c220 _$E74
// FUNCTION: STBC 0x0046c230 _$E192
// FUNCTION: STBC 0x0046c240 _$E191
// FUNCTION: STBC 0x0046c2c0 _$E200
// FUNCTION: STBC 0x0046c2d0 _$E199
// FUNCTION: STBC 0x0046c2e0 _$E207
// FUNCTION: STBC 0x0046c2f0 _$E206
// FUNCTION: STBC 0x0046c300 _$E211
// FUNCTION: STBC 0x0046c310 _$E210
// FUNCTION: STBC 0x0046c320 _$E215
// FUNCTION: STBC 0x0046c330 _$E214
// FUNCTION: STBC 0x0046c340 _$E219
// FUNCTION: STBC 0x0046c350 _$E218
// FUNCTION: STBC 0x0046c360 _$E225
// FUNCTION: STBC 0x0046c370 _$E224
// FUNCTION: STBC 0x0046c420 _$E236
// FUNCTION: STBC 0x0046c430 _$E235
// FUNCTION: STBC 0x0046c520 _$E245
// FUNCTION: STBC 0x0046c530 _$E244
// FUNCTION: STBC 0x0046c5f0 _$E182
// FUNCTION: STBC 0x0046c600 _$E181
// FUNCTION: STBC 0x0046c660 _$E177
// FUNCTION: STBC 0x0046c670 _$E176
// FUNCTION: STBC 0x0046c6b0 _$E117
// FUNCTION: STBC 0x0046c6c0 _$E116

/*
================
Unknown4635B0

A green wireframe shape of the box's eight corners and twelve triangles.
TODO identity: nothing calls it, and it has external linkage (a static
copy would not be emitted); its body is CollisionTestDisplay's static
CreateBoxShape (0x5888C0) to the byte, so the two units each carry one
under different names.
================
*/
// FUNCTION: STBC 0x004635b0
// DRAFT: inline budget: retail expands the green colour's constructor (budget 35, cost 47: own cost 6 tuples more, c2trace --whatif own=+6 flips only that site); the rest agrees
NiPointer<NiTriShape> Unknown4635B0( const NiBox &box ) {
	NiPoint3 *corners = new NiPoint3[8];
	corners[0] = box.center + box.axis[0] * box.extent[0] - box.axis[1] * box.extent[1] - box.axis[2] * box.extent[2];
	corners[1] = box.center + box.axis[0] * box.extent[0] + box.axis[1] * box.extent[1] - box.axis[2] * box.extent[2];
	corners[2] = box.center - box.axis[0] * box.extent[0] + box.axis[1] * box.extent[1] - box.axis[2] * box.extent[2];
	corners[3] = box.center - box.axis[0] * box.extent[0] - box.axis[1] * box.extent[1] - box.axis[2] * box.extent[2];
	corners[4] = box.center + box.axis[0] * box.extent[0] - box.axis[1] * box.extent[1] + box.axis[2] * box.extent[2];
	corners[5] = box.center + box.axis[0] * box.extent[0] + box.axis[1] * box.extent[1] + box.axis[2] * box.extent[2];
	corners[6] = box.center - box.axis[0] * box.extent[0] + box.axis[1] * box.extent[1] + box.axis[2] * box.extent[2];
	corners[7] = box.center - box.axis[0] * box.extent[0] - box.axis[1] * box.extent[1] + box.axis[2] * box.extent[2];

	unsigned short *triangles = new unsigned short[36];
	triangles[0] = 3;	triangles[1] = 2;	triangles[2] = 1;
	triangles[3] = 3;	triangles[4] = 1;	triangles[5] = 0;
	triangles[6] = 0;	triangles[7] = 1;	triangles[8] = 5;
	triangles[9] = 0;	triangles[10] = 5;	triangles[11] = 4;
	triangles[12] = 4;	triangles[13] = 5;	triangles[14] = 6;
	triangles[15] = 4;	triangles[16] = 6;	triangles[17] = 7;
	triangles[18] = 7;	triangles[19] = 6;	triangles[20] = 2;
	triangles[21] = 7;	triangles[22] = 2;	triangles[23] = 3;
	triangles[24] = 1;	triangles[25] = 2;	triangles[26] = 6;
	triangles[27] = 1;	triangles[28] = 6;	triangles[29] = 5;
	triangles[30] = 0;	triangles[31] = 4;	triangles[32] = 7;
	triangles[33] = 0;	triangles[34] = 7;	triangles[35] = 3;

	NiTriShape *shape = new NiTriShape( 8, corners, NULL, NULL, NULL, 12, triangles );

	NiWireframeProperty *wireframe = new NiWireframeProperty;
	wireframe->SetWireframe( true );
	shape->AttachProperty( wireframe );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	shape->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor( 0.0f, 1.0f, 0.0f ) );
	shape->AttachProperty( material );

	shape->UpdateProperties();
	return shape;
}

// The run-time type's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00463f60 _$E408
// FUNCTION: STBC 0x00463f70 _$E404
// FUNCTION: STBC 0x00463f90 _$E407
// FUNCTION: STBC 0x00463fa0 _$E405

// DATA: STBC 0x00981330
const NiRTTI OBBRoot::rtti( "OBBRoot", &NiExtraData::rtti );

/*
================
OBBModel::OBBModel
================
*/
// FUNCTION: STBC 0x00463fc0
OBBModel::OBBModel( NiAVObject *root_, unsigned int leafSize ) : callback( NULL ) {
	root = root_;
	AddShapes( root_, leafSize );
	ComputeBound( root_ );
}

/*
================
OBBModel::HasOBBRoots
================
*/
// FUNCTION: STBC 0x00464080
bool OBBModel::HasOBBRoots( NiAVObject *object ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		for ( NiExtraData *extra = shape->GetExtraData(); extra != NULL; extra = extra->GetNext() ) {
			if ( NiIsExactKindOf( OBBRoot, extra ) ) {
				return true;
			}
		}
		return false;
	}

	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child != NULL && !HasOBBRoots( child ) ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
OBBModel::AddShapes
================
*/
// FUNCTION: STBC 0x00464140
void OBBModel::AddShapes( NiAVObject *object, unsigned int leafSize ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		NiExtraData *extra = shape->GetExtraData();
		if ( extra != NULL ) {
			while ( true ) {
				if ( NiIsExactKindOf( OBBRoot, extra ) ) {
					shapes.Add( shape );
					return;
				}
				extra = extra->GetNext();
				if ( extra == NULL ) {
					break;
				}
			}
		}
		if ( leafSize > 0 ) {
			shape->AddExtraData( new OBBRoot( shape, leafSize ) );
			shapes.Add( shape );
		}
	} else {
		NiNode *node = NiDynamicCast( NiNode, object );
		if ( node != NULL ) {
			for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
				NiAVObject *child = node->GetAt( i );
				if ( child != NULL ) {
					AddShapes( child, leafSize );
				}
			}
		}
	}
}

/*
================
OBBModel::RefreshShapes
================
*/
// FUNCTION: STBC 0x00464300
void OBBModel::RefreshShapes() {
	shapes.RemoveAll();
	AddShapes( root, 0 );
}

/*
================
OBBModel::ComputeBound
================
*/
// FUNCTION: STBC 0x00464330
void OBBModel::ComputeBound( NiAVObject *root_ ) {
	halfExtent = NiPoint3::ZERO;
	center = NiPoint3::ZERO;

	unsigned int total = 0;
	unsigned int i;
	for ( i = 0; i < shapes.GetSize(); i++ ) {
		total += shapes.GetAt( i )->GetVertexCount();
	}
	if ( total == 0 ) {
		return;
	}

	// gather the world vertices of every shape
	NiPoint3 *vertices = new NiPoint3[total];
	NiPoint3 *next = vertices;
	for ( i = 0; i < shapes.GetSize(); i++ ) {
		NiTriShape *shape = shapes.GetAt( i );
		shape->CreateWorldVertices();
		shape->UpdateWorldVertices();
		// BUG: the shape is tested for NULL only after its world vertices were made
		if ( shape != NULL ) {
			unsigned short count = shape->GetVertexCount();
			memcpy( next, shape->GetWorldVertices(), count * sizeof( NiPoint3 ) );
			next += count;
		}
		shape->DestroyWorldVertices();
	}

	// take them into the model's space
	float invScale = 1.0f / root_->GetWorldScale();
	for ( i = 0; i < total; i++ ) {
		vertices[i] = ( vertices[i] - root_->GetWorldTranslate() ) * root_->GetWorldRotate() * invScale;
	}

	box.CreateFromData( total, vertices );

	NiPoint3 min( NI_INFINITY, NI_INFINITY, NI_INFINITY );
	NiPoint3 max( -NI_INFINITY, -NI_INFINITY, -NI_INFINITY );
	for ( i = 0; i < total; i++ ) {
		if ( vertices[i].x < min.x ) {
			min.x = vertices[i].x;
		}
		if ( vertices[i].x > max.x ) {
			max.x = vertices[i].x;
		}
		if ( vertices[i].y < min.y ) {
			min.y = vertices[i].y;
		}
		if ( vertices[i].y > max.y ) {
			max.y = vertices[i].y;
		}
		if ( vertices[i].z < min.z ) {
			min.z = vertices[i].z;
		}
		if ( vertices[i].z > max.z ) {
			max.z = vertices[i].z;
		}
	}
	halfExtent = ( max - min ) * 0.5f;
	center = ( max + min ) * 0.5f;

	delete [] vertices;
}

/*
================
OBBModel::FindRayIntersections
================
*/
// FUNCTION: STBC 0x00464650
bool OBBModel::FindRayIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick ) {
	bool hit = false;
	for ( unsigned int i = 0; i < shapes.GetSize(); i++ ) {
		NiTriShape *shape = shapes.GetAt( i );
		NiExtraData *extra = shape->GetExtraData();
		if ( extra != NULL ) {
			while ( true ) {
				if ( extra->GetRTTI() == &OBBRoot::rtti ) {
					hit |= static_cast<OBBRoot *>( extra )->FindRayIntersections( shape, origin, direction, pick, callback );
					break;
				}
				extra = extra->GetNext();
				if ( extra == NULL ) {
					break;
				}
			}
		}
	}
	if ( hit ) {
		SortResults( pick );
	}
	return hit;
}

/*
================
OBBModel::FindSegmentIntersections
================
*/
// FUNCTION: STBC 0x004646e0
bool OBBModel::FindSegmentIntersections( const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick ) {
	bool hit = false;
	for ( unsigned int i = 0; i < shapes.GetSize(); i++ ) {
		NiTriShape *shape = shapes.GetAt( i );
		NiExtraData *extra = shape->GetExtraData();
		if ( extra != NULL ) {
			while ( true ) {
				if ( extra->GetRTTI() == &OBBRoot::rtti ) {
					hit |= static_cast<OBBRoot *>( extra )->FindSegmentIntersections( shape, origin, direction, pick, callback );
					break;
				}
				extra = extra->GetNext();
				if ( extra == NULL ) {
					break;
				}
			}
		}
	}
	if ( hit ) {
		SortResults( pick );
	}
	return hit;
}

/*
================
OBBModel::FindSphereIntersections
================
*/
// FUNCTION: STBC 0x00464770
bool OBBModel::FindSphereIntersections( const NiPoint3 &center, float radius, NiTArray<OBBTriangleList *> *lists ) {
	bool hit = false;
	for ( unsigned int i = 0; i < shapes.GetSize(); i++ ) {
		NiTriShape *shape = shapes.GetAt( i );
		NiExtraData *extra = shape->GetExtraData();
		if ( extra != NULL ) {
			while ( true ) {
				if ( extra->GetRTTI() == &OBBRoot::rtti ) {
					hit |= static_cast<OBBRoot *>( extra )->Unknown465150( shape, center, radius, lists );
					break;
				}
				extra = extra->GetNext();
				if ( extra == NULL ) {
					break;
				}
			}
		}
	}
	return hit;
}

/*
================
OBBModel::FindRoughOBBIntersections

Runs OBBRoot's pair test over every pair of a shape of this model and a
shape of other that both have an OBBRoot; true when any pair touched.
Resets the leaf pair counter first.
================
*/
// FUNCTION: STBC 0x004647e0
bool OBBModel::FindRoughOBBIntersections( OBBModel *other, CollisionRecorder *recorder ) {
	obbLeafTests = 0;
	bool hit = false;
	for ( unsigned int i = 0; i < shapes.GetSize(); i++ ) {
		NiTriShape *shape = shapes.GetAt( i );
		NiExtraData *extra = shape->GetExtraData();
		if ( extra != NULL ) {
			while ( true ) {
				if ( extra->GetRTTI() == &OBBRoot::rtti ) {
					for ( unsigned int j = 0; j < other->shapes.GetSize(); j++ ) {
						NiTriShape *otherShape = other->shapes.GetAt( j );
						NiExtraData *otherExtra = otherShape->GetExtraData();
						if ( otherExtra != NULL ) {
							while ( true ) {
								if ( otherExtra->GetRTTI() == &OBBRoot::rtti ) {
									hit |= static_cast<OBBRoot *>( extra )->FindRoughOBBIntersections( shape, otherShape, static_cast<OBBRoot *>( otherExtra ), recorder, callback, other->callback );
									break;
								}
								otherExtra = otherExtra->GetNext();
								if ( otherExtra == NULL ) {
									break;
								}
							}
						}
					}
					break;
				}
				extra = extra->GetNext();
				if ( extra == NULL ) {
					break;
				}
			}
		}
	}
	return hit;
}

/*
================
OBBModel::FindBoxIntersections

Gathers the triangles of each shape that meet the world box into the lists
(OBBRoot's box test, one level down); nothing below depth 1.
================
*/
// FUNCTION: STBC 0x004648e0
bool OBBModel::FindBoxIntersections( const NiBox &box, NiTArray<OBBTriangleList *> *lists, int depth ) {
	if ( depth <= 0 ) {
		return false;
	}
	bool hit = false;
	for ( unsigned int i = 0; i < shapes.GetSize(); i++ ) {
		NiTriShape *shape = shapes.GetAt( i );
		NiExtraData *extra = shape->GetExtraData();
		if ( extra != NULL ) {
			while ( true ) {
				if ( extra->GetRTTI() == &OBBRoot::rtti ) {
					hit |= static_cast<OBBRoot *>( extra )->Unknown465250( shape, box, lists, depth - 1, callback );
					break;
				}
				extra = extra->GetNext();
				if ( extra == NULL ) {
					break;
				}
			}
		}
	}
	return hit;
}

/*
================
OBBModel::SortResults
================
*/
// FUNCTION: STBC 0x00464970
void OBBModel::SortResults( NiPick &pick ) {
	if ( pick.GetSortType() == NiPick::NO_SORT ) {
		return;
	}
	NiPick::Results &results = pick.GetResults();
	if ( results.GetEffectiveSize() <= 1 ) {
		return;
	}

	unsigned int size = results.GetSize();
	unsigned int i;
	NiPick::Record *record0;
	NiPick::Record *record1;
	NiPick::Record *save;
	if ( pick.GetPickType() == NiPick::FIND_ALL ) {
		// a bubble sort: there are few hits
		for ( i = 0; i < size; i++ ) {
			record0 = results.GetAt( i );
			if ( !record0 ) {
				continue;
			}
			for ( unsigned int j = i + 1; j < size; j++ ) {
				record1 = results.GetAt( j );
				if ( !record1 ) {
					continue;
				}
				if ( record1->GetDistance() < record0->GetDistance() ) {
					save = record0;
					record0 = record1;
					record1 = save;
					results.SetAt( i, record0 );
					results.SetAt( j, record1 );
				}
			}
		}
	} else {
		record0 = results.GetAt( 0 );
		for ( i = 1; i < size; i++ ) {
			record1 = results.RemoveAt( i );
			if ( record1 ) {
				if ( record1->GetDistance() < record0->GetDistance() ) {
					save = record0;
					record0 = record1;
					delete save;
				} else {
					delete record1;
				}
			}
		}
		results.SetAt( 0, record0 );
	}
}

/*
================
OBBRootData::OBBRootData

Allocates one index and one plane per triangle for the nodes to share.
================
*/
// FUNCTION: STBC 0x00464bd0 ??0OBBRootData@@QAE@PAVNiTriShape@@I@Z
OBBRootData::OBBRootData( NiTriShape *shape, unsigned int leafSize ) {
	indices = new unsigned short[shape->GetShapeData()->GetTriangleCount()];
	planes = new NiPlane[shape->GetShapeData()->GetTriangleCount()];
	root = new OBBNode( shape, leafSize, indices, planes );
}

/*
================
OBBRootData::~OBBRootData
================
*/
// FUNCTION: STBC 0x00464cf0
// FUNCTION: STBC 0x00464cd0 ??_GOBBRootData@@UAEPAXI@Z
OBBRootData::~OBBRootData() {
	delete [] indices;
	delete [] planes;
	delete root;
}

/*
================
OBBRoot::OBBRoot
================
*/
// FUNCTION: STBC 0x00464d60
OBBRoot::OBBRoot( NiTriShape *shape, unsigned int leafSize ) : tree( new OBBRootData( shape, leafSize ) ) {
}

/*
================
OBBRoot::CreateClone
================
*/
// FUNCTION: STBC 0x00464e80
NiObject *OBBRoot::CreateClone() {
	OBBRoot *clone = new OBBRoot;
	CopyMembers( clone );
	return clone;
}

/*
================
OBBRoot::CopyMembers

The clone shares the tree.
================
*/
// FUNCTION: STBC 0x00464f00
void OBBRoot::CopyMembers( OBBRoot *dest ) {
	NiExtraData::CopyMembers( dest );
	dest->tree = tree;
}

/*
================
OBBRoot::FindRayIntersections

The tree is in the shape's model space: the ray is taken into it, and
the world ray is passed on for the hits' records.
================
*/
// FUNCTION: STBC 0x00464f50
bool OBBRoot::FindRayIntersections( NiTriShape *shape, const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback ) {
	NiPoint3 localOrigin = ( origin - shape->GetWorldTranslate() ) * shape->GetWorldRotate() / shape->GetWorldScale();
	NiPoint3 localDirection = direction * shape->GetWorldRotate() / shape->GetWorldScale();
	VisibleDamagePiece *piece = NULL;
	return tree->GetRoot()->FindRayIntersections( shape, localOrigin, localDirection, origin, direction, pick, callback, piece );
}

/*
================
OBBRoot::FindSegmentIntersections
================
*/
// FUNCTION: STBC 0x00465050
bool OBBRoot::FindSegmentIntersections( NiTriShape *shape, const NiPoint3 &origin, const NiPoint3 &direction, NiPick &pick, VisibleDamage *callback ) {
	NiPoint3 localOrigin = ( origin - shape->GetWorldTranslate() ) * shape->GetWorldRotate() / shape->GetWorldScale();
	NiPoint3 localDirection = direction * shape->GetWorldRotate() / shape->GetWorldScale();
	VisibleDamagePiece *piece = NULL;
	return tree->GetRoot()->FindSegmentIntersections( shape, localOrigin, localDirection, origin, direction, pick, callback, piece );
}

/*
================
OBBRoot::Unknown465150
================
*/
// FUNCTION: STBC 0x00465150
bool OBBRoot::Unknown465150( NiTriShape *shape, const NiPoint3 &center, float radius, NiTArray<OBBTriangleList *> *lists ) {
	NiPoint3 offset = center - shape->GetWorldTranslate();
	float invScale = 1.0f / shape->GetWorldScale();
	NiPoint3 localCenter = offset * shape->GetWorldRotate() * invScale;
	float localRadius = invScale * radius;
	return tree->GetRoot()->FindSphereIntersections( shape, localCenter, localRadius, localRadius * localRadius, lists );
}

/*
================
OBBRoot::FindRoughOBBIntersections
================
*/
// FUNCTION: STBC 0x00465210
bool OBBRoot::FindRoughOBBIntersections( NiTriShape *shape, NiTriShape *otherShape, OBBRoot *other, CollisionRecorder *recorder, VisibleDamage *damage, VisibleDamage *otherDamage ) {
	return tree->GetRoot()->FindRoughOBBIntersections( shape, otherShape, other->tree->GetRoot(), recorder, damage, otherDamage );
}

/*
================
OBBRoot::Unknown465250
================
*/
// FUNCTION: STBC 0x00465250
bool OBBRoot::Unknown465250( NiTriShape *shape, const NiBox &box, NiTArray<OBBTriangleList *> *lists, int depth,
							 VisibleDamage *callback ) {
	if ( depth <= 0 ) {
		if ( shape->GetShapeData()->GetTriangleCount() != 0 ) {
			OBBTriangleList *list = new OBBTriangleList( shape );
			list->shape = shape;	// BUG: redundant: the constructor stored the shape
			memcpy( list->triangles, tree->GetIndices(), shape->GetShapeData()->GetTriangleCount() * sizeof( unsigned short ) );
			list->count = shape->GetShapeData()->GetTriangleCount();
			lists->Add( list );
			return true;
		}
		return false;
	}
	float invScale = 1.0f / shape->GetWorldScale();
	NiBox localBox;
	localBox.center = invScale * ( ( box.center - shape->GetWorldTranslate() ) * shape->GetWorldRotate() );
	localBox.extent[0] = invScale * box.extent[0];
	localBox.extent[1] = invScale * box.extent[1];
	localBox.extent[2] = invScale * box.extent[2];
	localBox.axis[0] = box.axis[0] * shape->GetWorldRotate();
	localBox.axis[1] = box.axis[1] * shape->GetWorldRotate();
	localBox.axis[2] = box.axis[2] * shape->GetWorldRotate();
	VisibleDamagePiece *piece = NULL;
	return tree->GetRoot()->FindBoxIntersections( shape, localBox, lists, depth - 1, callback, piece );
}
