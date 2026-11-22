/*
===============================================================================

	CollisionTestDisplay.cpp

===============================================================================
*/

#include <iostream>
#include "CollisionTestDisplay.h"
#include "CollisionRecorder.h"
#include "DamageableObject.h"
#include "VisibleDamage.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiNode.h"
#include "NiAutoNormalParticles.h"
#include "NiAutoNormalParticlesData.h"
#include "OBBModel.h"
#include "NiTriShape.h"
#include "NiPointer.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "OBBNode.h"
#include "NiTriShapeData.h"
#include "NiWireframeProperty.h"
#include "NiVertexColorProperty.h"
#include "NiMaterialProperty.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00589f50 _$E468
// FUNCTION: STBC 0x00589f70 _$E467

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00589730 _$E32
// FUNCTION: STBC 0x00589740 _$E29
// FUNCTION: STBC 0x00589750 _$E31
// FUNCTION: STBC 0x00589760 _$E30
// FUNCTION: STBC 0x00589770 _$E38
// FUNCTION: STBC 0x00589780 _$E35
// FUNCTION: STBC 0x00589790 _$E37
// FUNCTION: STBC 0x005897a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x005897b0 _$E66
// FUNCTION: STBC 0x005897c0 _$E65
// FUNCTION: STBC 0x005897d0 _$E288
// FUNCTION: STBC 0x005897e0 _$E287
// FUNCTION: STBC 0x005897f0 _$E77
// FUNCTION: STBC 0x00589800 _$E76
// FUNCTION: STBC 0x00589810 _$E81
// FUNCTION: STBC 0x00589820 _$E80
// FUNCTION: STBC 0x00589830 _$E85
// FUNCTION: STBC 0x00589840 _$E84
// FUNCTION: STBC 0x00589850 _$E89
// FUNCTION: STBC 0x00589860 _$E88
// FUNCTION: STBC 0x00589870 _$E206
// FUNCTION: STBC 0x00589880 _$E205
// FUNCTION: STBC 0x00589930 _$E148
// FUNCTION: STBC 0x00589940 _$E147
// FUNCTION: STBC 0x00589950 _$E161
// FUNCTION: STBC 0x00589960 _$E160
// FUNCTION: STBC 0x00589970 _$E211
// FUNCTION: STBC 0x00589980 _$E210
// FUNCTION: STBC 0x00589990 _$E293
// FUNCTION: STBC 0x005899a0 _$E292
// FUNCTION: STBC 0x005899b0 _$E215
// FUNCTION: STBC 0x005899c0 _$E214
// FUNCTION: STBC 0x005899d0 _$E297
// FUNCTION: STBC 0x005899e0 _$E296
// FUNCTION: STBC 0x005899f0 _$E304
// FUNCTION: STBC 0x00589a00 _$E303
// FUNCTION: STBC 0x00589a10 _$E314
// FUNCTION: STBC 0x00589a20 _$E313
// FUNCTION: STBC 0x00589a30 _$E318
// FUNCTION: STBC 0x00589a40 _$E317
// FUNCTION: STBC 0x00589a50 _$E106
// FUNCTION: STBC 0x00589a60 _$E105
// FUNCTION: STBC 0x00589a70 _$E234
// FUNCTION: STBC 0x00589a80 _$E233
// FUNCTION: STBC 0x00589a90 _$E241
// FUNCTION: STBC 0x00589aa0 _$E240
// FUNCTION: STBC 0x00589ab0 _$E245
// FUNCTION: STBC 0x00589ac0 _$E244
// FUNCTION: STBC 0x00589ad0 _$E249
// FUNCTION: STBC 0x00589ae0 _$E248
// FUNCTION: STBC 0x00589af0 _$E253
// FUNCTION: STBC 0x00589b00 _$E252
// FUNCTION: STBC 0x00589b10 _$E259
// FUNCTION: STBC 0x00589b20 _$E258
// FUNCTION: STBC 0x00589b30 _$E322
// FUNCTION: STBC 0x00589b40 _$E321
// FUNCTION: STBC 0x00589b50 _$E326
// FUNCTION: STBC 0x00589b60 _$E325
// FUNCTION: STBC 0x00589b70 _$E272
// FUNCTION: STBC 0x00589b80 _$E271
// FUNCTION: STBC 0x00589b90 _$E276
// FUNCTION: STBC 0x00589ba0 _$E275
// FUNCTION: STBC 0x00589bb0 _$E280
// FUNCTION: STBC 0x00589bc0 _$E279
// FUNCTION: STBC 0x00589bd0 _$E284
// FUNCTION: STBC 0x00589be0 _$E283
// FUNCTION: STBC 0x00589bf0 _$E330
// FUNCTION: STBC 0x00589c00 _$E329
// FUNCTION: STBC 0x00589c10 _$E334
// FUNCTION: STBC 0x00589c20 _$E333
// FUNCTION: STBC 0x00589c30 _$E338
// FUNCTION: STBC 0x00589c40 _$E337
// FUNCTION: STBC 0x00589c50 _$E342
// FUNCTION: STBC 0x00589c60 _$E341
// FUNCTION: STBC 0x00589c70 _$E111
// FUNCTION: STBC 0x00589c80 _$E110
// FUNCTION: STBC 0x00589c90 _$E346
// FUNCTION: STBC 0x00589ca0 _$E345
// FUNCTION: STBC 0x00589cb0 _$E116
// FUNCTION: STBC 0x00589cc0 _$E115
// FUNCTION: STBC 0x00589cd0 _$E350
// FUNCTION: STBC 0x00589ce0 _$E349
// FUNCTION: STBC 0x00589cf0 _$E354
// FUNCTION: STBC 0x00589d00 _$E353
// FUNCTION: STBC 0x00589d10 _$E358
// FUNCTION: STBC 0x00589d20 _$E357
// FUNCTION: STBC 0x00589d30 _$E362
// FUNCTION: STBC 0x00589d40 _$E361
// FUNCTION: STBC 0x00589d50 _$E125
// FUNCTION: STBC 0x00589d60 _$E124
// FUNCTION: STBC 0x00589d70 _$E366
// FUNCTION: STBC 0x00589d80 _$E365
// FUNCTION: STBC 0x00589d90 _$E370
// FUNCTION: STBC 0x00589da0 _$E369
// FUNCTION: STBC 0x00589db0 _$E374
// FUNCTION: STBC 0x00589dc0 _$E373
// FUNCTION: STBC 0x00589dd0 _$E131
// FUNCTION: STBC 0x00589de0 _$E130
// FUNCTION: STBC 0x00589df0 _$E136
// FUNCTION: STBC 0x00589e00 _$E135
// FUNCTION: STBC 0x00589e10 _$E166
// FUNCTION: STBC 0x00589e20 _$E165
// FUNCTION: STBC 0x00589e30 _$E378
// FUNCTION: STBC 0x00589e40 _$E377
// FUNCTION: STBC 0x00589e50 _$E171
// FUNCTION: STBC 0x00589e60 _$E170
// FUNCTION: STBC 0x00589e70 _$E382
// FUNCTION: STBC 0x00589e80 _$E381
// FUNCTION: STBC 0x00589e90 _$E386
// FUNCTION: STBC 0x00589ea0 _$E385
// FUNCTION: STBC 0x00589eb0 _$E176
// FUNCTION: STBC 0x00589ec0 _$E175
// FUNCTION: STBC 0x00589ed0 _$E390
// FUNCTION: STBC 0x00589ee0 _$E389
// FUNCTION: STBC 0x00589ef0 _$E141
// FUNCTION: STBC 0x00589f00 _$E140
// FUNCTION: STBC 0x00589f10 _$E394
// FUNCTION: STBC 0x00589f20 _$E393
// FUNCTION: STBC 0x00589f30 _$E398
// FUNCTION: STBC 0x00589f40 _$E397
// FUNCTION: STBC 0x00589f80 _$E181
// FUNCTION: STBC 0x00589f90 _$E180
// FUNCTION: STBC 0x00589fa0 _$E186
// FUNCTION: STBC 0x00589fb0 _$E185
// FUNCTION: STBC 0x00589fc0 _$E402
// FUNCTION: STBC 0x00589fd0 _$E401
// FUNCTION: STBC 0x00589fe0 _$E406
// FUNCTION: STBC 0x00589ff0 _$E405
// FUNCTION: STBC 0x0058a000 _$E410
// FUNCTION: STBC 0x0058a010 _$E409
// FUNCTION: STBC 0x0058a020 _$E414
// FUNCTION: STBC 0x0058a030 _$E413
// FUNCTION: STBC 0x0058a040 _$E418
// FUNCTION: STBC 0x0058a050 _$E417
// FUNCTION: STBC 0x0058a060 _$E422
// FUNCTION: STBC 0x0058a070 _$E421
// FUNCTION: STBC 0x0058a080 _$E426
// FUNCTION: STBC 0x0058a090 _$E425
// FUNCTION: STBC 0x0058a0a0 _$E430
// FUNCTION: STBC 0x0058a0b0 _$E429
// FUNCTION: STBC 0x0058a0c0 _$E191
// FUNCTION: STBC 0x0058a0d0 _$E190
// FUNCTION: STBC 0x0058a0e0 _$E434
// FUNCTION: STBC 0x0058a0f0 _$E433
// FUNCTION: STBC 0x0058a100 _$E196
// FUNCTION: STBC 0x0058a110 _$E195
// FUNCTION: STBC 0x0058a120 _$E201
// FUNCTION: STBC 0x0058a130 _$E200
// FUNCTION: STBC 0x0058a140 _$E438
// FUNCTION: STBC 0x0058a150 _$E437
// FUNCTION: STBC 0x0058a160 _$E443
// FUNCTION: STBC 0x0058a170 _$E442
// FUNCTION: STBC 0x0058a180 _$E447
// FUNCTION: STBC 0x0058a190 _$E446

// DATA: STBC 0x008939a0
static const float	maxEdgeLength = 1.0f;	// invented name: the longest triangle edge tested by its center, in world units

// FUNCTION: STBC 0x00587f70 _$E451
// FUNCTION: STBC 0x00587f80 _$E450
// DATA: STBC 0x0098d59c
static float	maxEdgeLengthSquared = maxEdgeLength * maxEdgeLength;	// invented name

// FUNCTION: STBC 0x00587fa0 _$E456
// FUNCTION: STBC 0x00587fb0 _$E453
// FUNCTION: STBC 0x00587fc0 _$E455
// FUNCTION: STBC 0x00587fd0 _$E454
// DATA: STBC 0x0098d5e4
static NiPointer<NiNode>	testDisplayRoot;	// invented name: the node the tested points hang under

// DATA: STBC 0x0098d5c8
static int	trianglesSplit;		// invented name: the triangles the current test of two sides split

// A triangle of a shape's model-space vertices (invented name).
struct ShapeTriangle {
	NiPoint3 *	corner[3];
};

static NiPointer<NiTriShape>	CreateBoxShape( const NiBox &box );
bool						TestTriangle( NiTriShape *shape, const ShapeTriangle *triangle, DamageableObject *object, VisibleDamage *damage, CollisionRecorder *recorder );

// DATA: STBC 0x0098d674
static int	pointsTested;		// invented name: the points the current test of two sides tried

/*
================
DisplayCollisionTests

Creates the display's node under the rendered set's scene root, or
detaches and releases it.
================
*/
// FUNCTION: STBC 0x00588000
void DisplayCollisionTests( bool show ) {
	if ( show ) {
		if ( testDisplayRoot == NULL ) {
			SetClass *set = g_kSetManager.GetRenderedSet();
			if ( set != NULL ) {
				testDisplayRoot = new NiNode;
				set->GetSceneRoot()->AttachChild( testDisplayRoot, true );
			}
		}
	} else if ( testDisplayRoot != NULL ) {
		testDisplayRoot->GetParent()->DetachChild( testDisplayRoot );
		testDisplayRoot = NULL;
	}
}

/*
================
DamageCollisionTest::VirtualSlot01

The damage that is set up is tested: when both sides have it, the
smaller object's model is tested against the larger one's damage.
================
*/
// FUNCTION: STBC 0x00588150
bool DamageCollisionTest::VirtualSlot01( DamageCollisionTest *other, CollisionRecorder *recorder ) {
	bool hit = false;
	trianglesSplit = 0;
	pointsTested = 0;
	recorder->BeginRecord( 2, object, other->object );

	// the model tested and the damage it is tested against
	DamageableObject *modelObject = NULL;
	OBBModel *model = NULL;
	DamageCollisionTest *modelSide = NULL;
	DamageableObject *damagedObject = NULL;
	OBBModel *damagedModel = NULL;
	VisibleDamage *targetDamage = NULL;
	if ( damage != NULL && damage->Unknown4B1B50() && other->damage != NULL && other->damage->Unknown4B1B50() ) {
		if ( object->GetWorldBound().GetRadius() > other->object->GetWorldBound().GetRadius() ) {
			modelObject = other->object;
			model = other->obbModel;
			modelSide = other;
			damagedObject = object;
			damagedModel = obbModel;
			targetDamage = damage;
		} else {
			modelObject = object;
			model = obbModel;
			modelSide = this;
			damagedObject = other->object;
			damagedModel = other->obbModel;
			targetDamage = other->damage;
		}
	} else if ( damage != NULL && damage->Unknown4B1B50() ) {
		modelObject = other->object;
		model = other->obbModel;
		modelSide = other;
		damagedObject = object;
		damagedModel = obbModel;
		targetDamage = damage;
	} else if ( other->damage != NULL && other->damage->Unknown4B1B50() ) {
		modelObject = object;
		model = obbModel;
		modelSide = this;
		damagedObject = other->object;
		damagedModel = other->obbModel;
		targetDamage = other->damage;
	}

	if ( modelObject != NULL ) {
		if ( modelSide->unknown10 == 0 ) {
			hit = TestVertices( modelObject, model, modelSide, damagedObject, damagedModel, targetDamage, recorder );
		} else {
			hit = Unknown5884A0( modelObject, model, modelSide, damagedObject, damagedModel, targetDamage, recorder );
		}
		if ( testDisplayRoot != NULL ) {
			testDisplayRoot->UpdateProperties();
			testDisplayRoot->Update( 0.0f );
		}
	}
	recorder->EndRecord();
	return hit;
}

/*
================
TestPoint

Tests the world point against the damage: it is shown in the display when
there is one, and a point that hits is recorded (invented name).
================
*/
// FUNCTION: STBC 0x00588380
static bool TestPoint( VisibleDamage *damage, NiPoint3 *point, CollisionRecorder *recorder ) {
	NiAutoNormalParticles *particles;
	if ( testDisplayRoot != NULL ) {
		NiPoint3 *vertex = new NiPoint3;
		*vertex = *point;
		particles = new NiAutoNormalParticles( 1, vertex, NULL );
		particles->GetParticlesData()->SetRadius( 0.05f );
		testDisplayRoot->AttachChild( particles, false );
	}
	pointsTested++;
	if ( damage->Unknown4B1940( point, false ) ) {
		NiPoint3 *hitPoint = new NiPoint3( *point );
		recorder->AddPoint( hitPoint );
		return true;
	}
	return false;
}

/*
================
DamageCollisionTest::TestVertices

Tests the world position of each vertex of the model's shapes against the
damage; true at the first that hits.
================
*/
// FUNCTION: STBC 0x00588280
bool DamageCollisionTest::TestVertices( DamageableObject *modelObject, OBBModel *model, DamageCollisionTest *modelSide, DamageableObject *damagedObject, OBBModel *damagedModel, VisibleDamage *damage, CollisionRecorder *recorder ) {
	bool hit = false;
	for ( unsigned int i = 0; i < model->shapes.GetSize(); i++ ) {
		NiTriShape *shape = model->shapes.GetAt( i );
		if ( shape != NULL ) {
			NiGeometryData *data = shape->modelData;
			unsigned short vertexCount = data->GetVertexCount();
			NiPoint3 *vertices = data->GetVertices();
			for ( unsigned short v = 0; v < vertexCount; v++ ) {
				NiPoint3 point = shape->GetWorldRotate() * ( shape->GetWorldScale() * vertices[v] ) + shape->GetWorldTranslate();
				hit = TestPoint( damage, &point, recorder );
				if ( hit ) {
					return hit;
				}
			}
		}
	}
	return hit;
}

/*
================
DamageCollisionTest::Unknown5884A0

Tests the model's triangles that meet the damaged model's world box
against the damage: the model's box test gathers the triangles, each is
handed to the model's callback, and split until it is small enough to be
tested by its center. The box is shown in the display when there is one.
================
*/
// FUNCTION: STBC 0x005884a0
bool DamageCollisionTest::Unknown5884A0( DamageableObject *modelObject, OBBModel *model, DamageCollisionTest *modelSide, DamageableObject *damagedObject, OBBModel *damagedModel, VisibleDamage *damage, CollisionRecorder *recorder ) {
	bool hit = false;

	// the damaged model's box in the world
	NiBox box = damagedModel->box;
	box.center = TGModelUtils::LocalToWorldPoint( damagedObject->GetNiObject(), box.center );
	box.axis[0] = TGModelUtils::LocalToWorldVector( damagedObject->GetNode(), box.axis[0] );
	box.axis[1] = TGModelUtils::LocalToWorldVector( damagedObject->GetNode(), box.axis[1] );
	box.axis[2] = TGModelUtils::LocalToWorldVector( damagedObject->GetNode(), box.axis[2] );
	box.extent[0] = damagedObject->GetNiObject()->GetWorldScale() * box.extent[0];
	box.extent[1] = damagedObject->GetNiObject()->GetWorldScale() * box.extent[1];
	box.extent[2] = damagedObject->GetNiObject()->GetWorldScale() * box.extent[2];

	// only the array is freed: the lists it holds are never deleted
	NiTArray<OBBTriangleList *> lists;
	model->FindBoxIntersections( box, &lists, modelSide->unknown10 );

	VisibleDamage *callback = model->callback;
	VisibleDamagePiece *piece = NULL;
	if ( testDisplayRoot != NULL ) {
		testDisplayRoot->RemoveAllChildren();
		testDisplayRoot->AttachChild( CreateBoxShape( box ), false );
	}

	for ( unsigned int i = 0; i < lists.GetSize(); i++ ) {
		OBBTriangleList *list = lists.GetAt( i );
		if ( list != NULL ) {
			NiTriShapeData *data = list->shape->GetShapeData();
			unsigned short *triList = data->GetTriList();
			NiPoint3 *vertices = data->GetVertices();
			for ( unsigned int j = 0; j < list->count; j++ ) {
				unsigned short triangle = list->triangles[j];
				unsigned short corners[3];
				corners[0] = triList[3 * triangle];
				corners[1] = triList[3 * triangle + 1];
				corners[2] = triList[3 * triangle + 2];
				if ( callback != NULL ) {
					callback->VirtualSlot00( list->shape, triangle, &corners[0], &corners[1], &corners[2], &piece );
				}
				ShapeTriangle points;
				points.corner[0] = &vertices[corners[0]];
				points.corner[1] = &vertices[corners[1]];
				points.corner[2] = &vertices[corners[2]];
				if ( points.corner[0] != NULL ) {
					hit |= TestTriangle( list->shape, &points, modelObject, damage, recorder );
				}
			}
		}
	}
	return hit;
}

/*
================
CreateBoxShape

A green wireframe shape of the box's eight corners and twelve
triangles, for the display (invented name).
================
*/
// FUNCTION: STBC 0x005888c0
// DRAFT: complete; the green emittance passed as a temporary gives retail's 0x58 frame (as its twin OBBRoot's Unknown4635B0); left: NiColor's constructor goes out of line where retail expands it (needs own cost +6, c2trace --whatif own=+6)
static NiPointer<NiTriShape> CreateBoxShape( const NiBox &box ) {
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

/*
================
IsTriangleLarge

True when an edge of the triangle is longer in the world than
maxEdgeLength (invented name).
================
*/
// FUNCTION: STBC 0x00589270
bool IsTriangleLarge( const ShapeTriangle *triangle, float scale ) {
	float limit = maxEdgeLengthSquared / ( scale * scale );
	if ( ( *triangle->corner[1] - *triangle->corner[0] ).SqrLength() > limit ) {
		return true;
	}
	if ( ( *triangle->corner[2] - *triangle->corner[0] ).SqrLength() > limit ) {
		return true;
	}
	if ( ( *triangle->corner[2] - *triangle->corner[1] ).SqrLength() > limit ) {
		return true;
	}
	return false;
}

/*
================
TestTriangle

Tests a small triangle by its center in the world; a large one is split
at the middle of its longest edge and both halves are tested (invented
name).
================
*/
// FUNCTION: STBC 0x00589340
// DRAFT: complete; inline budget: retail calls the centre's first operator+ constructor out of line (share 48 here, retail's under 47: one more inline site after it, or about 20 tuples less own cost)
bool TestTriangle( NiTriShape *shape, const ShapeTriangle *triangle, DamageableObject *object, VisibleDamage *damage, CollisionRecorder *recorder ) {
	if ( !IsTriangleLarge( triangle, shape->GetWorldScale() ) ) {
		NiPoint3 center = ( *triangle->corner[0] + *triangle->corner[1] + *triangle->corner[2] ) / 3.0f;
		center = TGModelUtils::LocalToWorldPoint( shape, center );
		return TestPoint( damage, &center, recorder );
	} else {
		// split the longest edge
		ShapeTriangle first = { triangle->corner[0], triangle->corner[1], triangle->corner[2] };
		ShapeTriangle second = first;
		NiPoint3 middle;
		float edge01 = ( *triangle->corner[1] - *triangle->corner[0] ).SqrLength();
		float edge02 = ( *triangle->corner[2] - *triangle->corner[0] ).SqrLength();
		float edge12 = ( *triangle->corner[2] - *triangle->corner[1] ).SqrLength();
		if ( edge02 < edge01 ) {
			if ( edge12 < edge01 ) {
				middle = ( *triangle->corner[1] + *triangle->corner[0] ) / 2.0f;
				first.corner[0] = &middle;
				second.corner[1] = &middle;
			} else {
				middle = ( *triangle->corner[2] + *triangle->corner[1] ) / 2.0f;
				first.corner[2] = &middle;
				second.corner[1] = &middle;
			}
		} else {
			middle = ( *triangle->corner[2] + *triangle->corner[0] ) / 2.0f;
			first.corner[2] = &middle;
			second.corner[0] = &middle;
		}
		trianglesSplit++;
		if ( TestTriangle( shape, &first, object, damage, recorder ) ) {
			return true;
		}
		return TestTriangle( shape, &second, object, damage, recorder ) == true;
	}
}

/*
================
DamageCollisionTest::VirtualSlot02
================
*/
// FUNCTION: STBC 0x00589710
bool DamageCollisionTest::VirtualSlot02( DamageCollisionTest *other, CollisionRecorder *recorder ) {
	return false;
}
