/*
===============================================================================

	SpecularPass.cpp

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <string.h>

#include "SpecularPass.h"
#include "DamageBitArray.h"
#include "Glow.h"
#include "LODModelManager.h"
#include "NiAlphaProperty.h"
#include "NiCamera.h"
#include "NiDirectionalLight.h"
#include "NiFogProperty.h"
#include "NiMultiTextureProperty.h"
#include "NiTList.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004efac0 _$E239
// FUNCTION: STBC 0x004efad0 _$E238
// FUNCTION: STBC 0x004efbc0 _$E197
// FUNCTION: STBC 0x004efbd0 _$E196
// FUNCTION: STBC 0x004efbe0 _$E245
// FUNCTION: STBC 0x004efbf0 _$E244
// FUNCTION: STBC 0x004efc20 _$E272
// FUNCTION: STBC 0x004efc30 _$E271
// FUNCTION: STBC 0x004efc80 _$E279
// FUNCTION: STBC 0x004efc90 _$E278
// FUNCTION: STBC 0x004efca0 _$E293
// FUNCTION: STBC 0x004efcb0 _$E292
// FUNCTION: STBC 0x004efcc0 _$E297
// FUNCTION: STBC 0x004efcd0 _$E296
// FUNCTION: STBC 0x004efd00 _$E209
// FUNCTION: STBC 0x004efd10 _$E208
// FUNCTION: STBC 0x004efd20 _$E216
// FUNCTION: STBC 0x004efd30 _$E215
// FUNCTION: STBC 0x004efd40 _$E220
// FUNCTION: STBC 0x004efd50 _$E219
// FUNCTION: STBC 0x004efd60 _$E224
// FUNCTION: STBC 0x004efd70 _$E223
// FUNCTION: STBC 0x004efd80 _$E228
// FUNCTION: STBC 0x004efd90 _$E227
// FUNCTION: STBC 0x004efda0 _$E234
// FUNCTION: STBC 0x004efdb0 _$E233
// FUNCTION: STBC 0x004efe80 _$E264
// FUNCTION: STBC 0x004efe90 _$E263
// FUNCTION: STBC 0x004efea0 _$E301
// FUNCTION: STBC 0x004efeb0 _$E300
// FUNCTION: STBC 0x004eff10 _$E305
// FUNCTION: STBC 0x004eff20 _$E304
// FUNCTION: STBC 0x004eff40 _$E309
// FUNCTION: STBC 0x004eff50 _$E308
// FUNCTION: STBC 0x004eff60 _$E313
// FUNCTION: STBC 0x004eff70 _$E312
// FUNCTION: STBC 0x004eff80 _$E317
// FUNCTION: STBC 0x004eff90 _$E316
// FUNCTION: STBC 0x004effa0 _$E321
// FUNCTION: STBC 0x004effb0 _$E320
// FUNCTION: STBC 0x004effc0 _$E325
// FUNCTION: STBC 0x004effd0 _$E324
// FUNCTION: STBC 0x004effe0 _$E329
// FUNCTION: STBC 0x004efff0 _$E328
// FUNCTION: STBC 0x004f0000 _$E333
// FUNCTION: STBC 0x004f0010 _$E332
// FUNCTION: STBC 0x004f0020 _$E97
// FUNCTION: STBC 0x004f0030 _$E96
// FUNCTION: STBC 0x004f00c0 _$E283
// FUNCTION: STBC 0x004f00d0 _$E282
// FUNCTION: STBC 0x004f00e0 _$E102
// FUNCTION: STBC 0x004f00f0 _$E101
// FUNCTION: STBC 0x004f0100 _$E337
// FUNCTION: STBC 0x004f0110 _$E336
// FUNCTION: STBC 0x004f0120 _$E341
// FUNCTION: STBC 0x004f0130 _$E340
// FUNCTION: STBC 0x004f0140 _$E345
// FUNCTION: STBC 0x004f0150 _$E344
// FUNCTION: STBC 0x004f0160 _$E349
// FUNCTION: STBC 0x004f0170 _$E348
// FUNCTION: STBC 0x004f0180 _$E111
// FUNCTION: STBC 0x004f0190 _$E110
// FUNCTION: STBC 0x004f01a0 _$E353
// FUNCTION: STBC 0x004f01b0 _$E352
// FUNCTION: STBC 0x004f01c0 _$E357
// FUNCTION: STBC 0x004f01d0 _$E356
// FUNCTION: STBC 0x004f01e0 _$E361
// FUNCTION: STBC 0x004f01f0 _$E360
// FUNCTION: STBC 0x004f0340 _$E152
// FUNCTION: STBC 0x004f0350 _$E151
// FUNCTION: STBC 0x004f0390 _$E365
// FUNCTION: STBC 0x004f03a0 _$E364
// FUNCTION: STBC 0x004f03b0 _$E157
// FUNCTION: STBC 0x004f03c0 _$E156
// FUNCTION: STBC 0x004f03d0 _$E369
// FUNCTION: STBC 0x004f03e0 _$E368
// FUNCTION: STBC 0x004f03f0 _$E373
// FUNCTION: STBC 0x004f0400 _$E372
// FUNCTION: STBC 0x004f0410 _$E162
// FUNCTION: STBC 0x004f0420 _$E161
// FUNCTION: STBC 0x004f0430 _$E377
// FUNCTION: STBC 0x004f0440 _$E376
// FUNCTION: STBC 0x004f0450 _$E127
// FUNCTION: STBC 0x004f0460 _$E126
// FUNCTION: STBC 0x004f0470 _$E381
// FUNCTION: STBC 0x004f0480 _$E380
// FUNCTION: STBC 0x004f0490 _$E385
// FUNCTION: STBC 0x004f04a0 _$E384
// FUNCTION: STBC 0x004f04d0 _$E172
// FUNCTION: STBC 0x004f04e0 _$E171
// FUNCTION: STBC 0x004f04f0 _$E389
// FUNCTION: STBC 0x004f0500 _$E388
// FUNCTION: STBC 0x004f0510 _$E393
// FUNCTION: STBC 0x004f0520 _$E392
// FUNCTION: STBC 0x004f0530 _$E397
// FUNCTION: STBC 0x004f0540 _$E396
// FUNCTION: STBC 0x004f0550 _$E401
// FUNCTION: STBC 0x004f0560 _$E400
// FUNCTION: STBC 0x004f0570 _$E405
// FUNCTION: STBC 0x004f0580 _$E404
// FUNCTION: STBC 0x004f0590 _$E409
// FUNCTION: STBC 0x004f05a0 _$E408
// FUNCTION: STBC 0x004f05b0 _$E413
// FUNCTION: STBC 0x004f05c0 _$E412
// FUNCTION: STBC 0x004f05d0 _$E417
// FUNCTION: STBC 0x004f05e0 _$E416
// FUNCTION: STBC 0x004f0610 _$E421
// FUNCTION: STBC 0x004f0620 _$E420
// FUNCTION: STBC 0x004f0630 _$E182
// FUNCTION: STBC 0x004f0640 _$E181
// FUNCTION: STBC 0x004f0670 _$E425
// FUNCTION: STBC 0x004f0680 _$E424
// FUNCTION: STBC 0x004f0690 _$E430
// FUNCTION: STBC 0x004f06a0 _$E429
// FUNCTION: STBC 0x004f06b0 _$E434
// FUNCTION: STBC 0x004f06c0 _$E433
// FUNCTION: STBC 0x004efaa0 _$E45
// FUNCTION: STBC 0x004efab0 _$E44
// FUNCTION: STBC 0x004efae0 _$E56
// FUNCTION: STBC 0x004efaf0 _$E55
// FUNCTION: STBC 0x004efb00 _$E60
// FUNCTION: STBC 0x004efb10 _$E59
// FUNCTION: STBC 0x004efb20 _$E64
// FUNCTION: STBC 0x004efb30 _$E63
// FUNCTION: STBC 0x004efb40 _$E68
// FUNCTION: STBC 0x004efb50 _$E67
// FUNCTION: STBC 0x004efb60 _$E192
// FUNCTION: STBC 0x004efb70 _$E191
// FUNCTION: STBC 0x004efb80 _$E134
// FUNCTION: STBC 0x004efb90 _$E133
// FUNCTION: STBC 0x004efba0 _$E147
// FUNCTION: STBC 0x004efbb0 _$E146
// FUNCTION: STBC 0x004efc00 _$E201
// FUNCTION: STBC 0x004efc10 _$E200
// FUNCTION: STBC 0x004efce0 _$E92
// FUNCTION: STBC 0x004efcf0 _$E91
// FUNCTION: STBC 0x004f0200 _$E117
// FUNCTION: STBC 0x004f0210 _$E116
// FUNCTION: STBC 0x004f0220 _$E122
// FUNCTION: STBC 0x004f0230 _$E121
// FUNCTION: STBC 0x004f04b0 _$E167
// FUNCTION: STBC 0x004f04c0 _$E166
// FUNCTION: STBC 0x004f05f0 _$E177
// FUNCTION: STBC 0x004f0600 _$E176
// FUNCTION: STBC 0x004f0650 _$E187
// FUNCTION: STBC 0x004f0660 _$E186

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004efa20 _$E32
// FUNCTION: STBC 0x004efa30 _$E29
// FUNCTION: STBC 0x004efa40 _$E31
// FUNCTION: STBC 0x004efa50 _$E30
// FUNCTION: STBC 0x004efa60 _$E38
// FUNCTION: STBC 0x004efa70 _$E35
// FUNCTION: STBC 0x004efa80 _$E37
// FUNCTION: STBC 0x004efa90 _$E36

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x004f0360 _$E455
// FUNCTION: STBC 0x004f0380 _$E454

/*
================
ModelPass::CreateClone

A pass is not cloned with its parent.
================
*/
// FUNCTION: STBC 0x004ec9e0
NiObject *ModelPass::CreateClone() {
	return NULL;
}

/*
================
ModelPass::CopyMembers

The copy watches no model.
================
*/
// FUNCTION: STBC 0x004ec9f0
void ModelPass::CopyMembers( ModelPass *dest ) {
	NiNode::CopyMembers( dest );
	dest->model = NULL;
}

/*
================
ModelPass::~ModelPass
================
*/
// FUNCTION: STBC 0x004eca30
ModelPass::~ModelPass() {
	NiTMapIterator pos = shapes.GetFirstPos();
	while ( pos ) {
		NiTriShape *shape;
		PassShape *passShape;
		shapes.GetNext( pos, shape, passShape );
		delete passShape;
	}
}

/*
================
ModelPass::CreatePassShape
================
*/
// FUNCTION: STBC 0x004ecb90
PassShape *ModelPass::CreatePassShape( NiTriShape *shape ) {
	return NULL;
}

/*
================
ModelPass::UpdatePassShape
================
*/
// FUNCTION: STBC 0x004ecba0
void ModelPass::UpdatePassShape( PassShape *passShape ) {
}

/*
================
ModelPass::FindShapes

Marks the record of every triangle shape under object as still in the
model, and makes a record for each shape that has none.
================
*/
// FUNCTION: STBC 0x004ecbd0
void ModelPass::FindShapes( NiAVObject *object ) {
	if ( object == NULL ) {
		return;
	}
	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			FindShapes( node->GetAt( i ) );
		}
		return;
	}
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		PassShape *passShape;
		if ( shapes.GetAt( shape, passShape ) ) {
			passShape->inModel = true;
			return;
		}
		passShape = CreatePassShape( shape );
		if ( passShape != NULL ) {
			shapes.SetAt( shape, passShape );
		}
	}
}

// The NiRTTI's static initialiser, its exit-time thunk, the exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004ecd40 _$E440
// FUNCTION: STBC 0x004ecd50 _$E436
// FUNCTION: STBC 0x004ecd70 _$E439
// FUNCTION: STBC 0x004ecd80 _$E437
// DATA: STBC 0x986628 ?rtti@SpecularPass@@2VNiRTTI@@B
const NiRTTI SpecularPass::rtti( "SpecularPass", &NiNode::rtti );

/*
================
SpecularPass::SetSpecularKs

Sets the specular Ks of every SpecularPass in the tree.
================
*/
// FUNCTION: STBC 0x004ecda0
void SpecularPass::SetSpecularKs( NiAVObject *object, float ks ) {
	SpecularPass *pass = NiDynamicCast( SpecularPass, object );
	if ( pass != NULL ) {
		pass->specularKs = ks;
		return;
	}
	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			SetSpecularKs( node->GetAt( i ), ks );
		}
	}
}

/*
================
SpecularPassShape::~SpecularPassShape

Takes the pass's shape out of the pass.
================
*/
// FUNCTION: STBC 0x004ece40
SpecularPassShape::~SpecularPassShape() {
	if ( passShape != NULL && passShape->GetParent() != NULL ) {
		passShape->GetParent()->DetachChild( passShape );
	}
}

/*
================
SpecularPass::CreateClone
================
*/
// FUNCTION: STBC 0x004ecf80
NiObject *SpecularPass::CreateClone() {
	SpecularPass *clone = new SpecularPass( NULL, 1.0f, 4.0f );
	CopyMembers( clone );
	return clone;
}

/*
================
SpecularPass::CopyMembers
================
*/
// FUNCTION: STBC 0x004ecff0
void SpecularPass::CopyMembers( SpecularPass *dest ) {
	ModelPass::CopyMembers( dest );
	dest->specularKs = specularKs;
	dest->specularPower = specularPower;
}

/*
================
SpecularPass::SpecularPass

The pass's shapes are added to what is already drawn: trilinear filtering,
additive blending with no sorting, depth tested but not written, their
own vertex colours as the only lighting, and no fog.
================
*/
// FUNCTION: STBC 0x004ed020
SpecularPass::SpecularPass( NiAVObject *model_, float ks, float power ) : ModelPass( model_ ), specularKs( ks ), specularPower( power ) {
	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_TRILERP );
	AttachProperty( textureMode );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_ONE );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	alpha->SetNoSorter( true );
	AttachProperty( alpha );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	AttachProperty( zBuffer );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	AttachProperty( vertexColor );

	NiFogProperty *fog = new NiFogProperty;
	fog->SetFog( false );
	AttachProperty( fog );
}

/*
================
SpecularPass::CreateShape

A shape for the pass to draw over the model's shape: copies of its
vertices, texture coordinates and triangles, vertex colours for Display to
fill in, and the shape's gloss map as its texture. NULL when the shape has
no gloss map. The new shape is left out of the glow.
================
*/
// FUNCTION: STBC 0x004ed3e0
NiTriShape *SpecularPass::CreateShape( NiTriShape *shape ) {
	if ( shape->GetParent() == NULL ) {
		return NULL;
	}
	NiMultiTextureProperty *multiTexture = NiDynamicCast( NiMultiTextureProperty, shape->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( multiTexture == NULL ) {
		return NULL;
	}
	NiMultiTextureProperty::Map *gloss = multiTexture->GetMap( NiMultiTextureProperty::GLOSS_INDEX );
	if ( gloss == NULL ) {
		return NULL;
	}
	NiImage *image = gloss->image;
	if ( image == NULL ) {
		return NULL;
	}

	unsigned short vertexCount = shape->GetVertexCount();
	NiPoint3 *vertices = new NiPoint3[vertexCount];
	memcpy( vertices, shape->GetShapeData()->GetVertices(), vertexCount * sizeof( NiPoint3 ) );
	NiColorA *colors = new NiColorA[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount];
	memcpy( textures, shape->GetShapeData()->GetTextures(), vertexCount * sizeof( NiPoint2 ) );
	unsigned short triangleCount = shape->GetShapeData()->GetTriangleCount();
	unsigned short *triList = new unsigned short[triangleCount * 3];
	memcpy( triList, shape->GetTriList(), triangleCount * 3 * sizeof( unsigned short ) );
	NiTriShape *passShape = new NiTriShape( vertexCount, vertices, NULL, colors, textures, triangleCount, triList );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( image );
	passShape->AttachProperty( texture );
	AttachChild( passShape, true );

	passShape->AddExtraData( new GlowPass );
	passShape->UpdateProperties();
	passShape->Update( 0.0f );
	return passShape;
}

/*
================
SpecularPass::CreatePassShape
================
*/
// FUNCTION: STBC 0x004ed780
PassShape *SpecularPass::CreatePassShape( NiTriShape *shape ) {
	NiTriShape *newShape = CreateShape( shape );
	if ( newShape != NULL ) {
		SpecularPassShape *passShape = new SpecularPassShape( shape );
		passShape->passShape = newShape;
		return passShape;
	}
	return NULL;
}

/*
================
SpecularPass::UpdatePassShape

Copies the changed shape again: in place while its vertex and triangle
counts are the pass shape's, into a new pass shape otherwise.
================
*/
// FUNCTION: STBC 0x004ed830
void SpecularPass::UpdatePassShape( PassShape *passShape ) {
	SpecularPassShape *specular = (SpecularPassShape *)passShape;
	NiTriShape *shape = specular->shape;
	unsigned short vertexCount = shape->GetVertexCount();
	unsigned short triangleCount = shape->GetShapeData()->GetTriangleCount();
	NiTriShape *drawn = specular->passShape;
	if ( drawn == NULL ) {
		return;
	}
	if ( drawn->GetVertexCount() == vertexCount && drawn->GetShapeData()->GetTriangleCount() == triangleCount ) {
		memcpy( drawn->GetShapeData()->GetVertices(), shape->GetShapeData()->GetVertices(), vertexCount * sizeof( NiPoint3 ) );
		memcpy( drawn->GetShapeData()->GetTextures(), shape->GetShapeData()->GetTextures(), vertexCount * sizeof( NiPoint2 ) );
		memcpy( drawn->GetTriList(), shape->GetTriList(), triangleCount * 3 * sizeof( unsigned short ) );
		drawn->GetShapeData()->IncRevisionID();
		return;
	}
	if ( drawn->GetParent() != NULL ) {
		drawn->GetParent()->DetachChild( drawn );
	}
	specular->passShape = CreateShape( shape );
}

/*
================
SpecularPass::UpdateShapes

Brings the records up to date with the model: a record whose shape has
left the model is deleted, one whose shape data has changed is copied
again, and the others take the shape's triangles (which Display leaves
alone, but the level of detail may have reordered).
================
*/
// FUNCTION: STBC 0x004ed990
void SpecularPass::UpdateShapes() {
	if ( model == NULL ) {
		return;
	}

	NiTriShape *shape;
	PassShape *passShape;
	NiTMapIterator pos = shapes.GetFirstPos();
	while ( pos ) {
		shapes.GetNext( pos, shape, passShape );
		if ( passShape != NULL ) {
			passShape->inModel = false;
		}
	}
	FindShapes( model );

	pos = shapes.GetFirstPos();
	NiTList<void *> gone( 2 );
	while ( pos ) {
		shapes.GetNext( pos, shape, passShape );
		if ( !passShape->inModel ) {
			delete passShape;
			gone.AddTail( shape );
		} else if ( passShape->revisionID != passShape->shape->GetShapeData()->GetRevisionID() ) {
			UpdatePassShape( passShape );
			passShape->revisionID = passShape->shape->GetShapeData()->GetRevisionID();
		} else {
			memcpy( ( (SpecularPassShape *)passShape )->passShape->GetTriList(),
				passShape->shape->GetTriList(),
				passShape->shape->GetShapeData()->GetTriangleCount() * 3 * sizeof( unsigned short ) );
		}
	}

	NiTListIterator gonePos = gone.GetHeadPos();
	while ( gonePos ) {
		shapes.RemoveAt( (NiTriShape *)gone.GetNext( gonePos ) );
	}
}

/*
================
SpecularPass::UpdateDownwardPass
================
*/
// FUNCTION: STBC 0x004edca0
void SpecularPass::UpdateDownwardPass( float time ) {
	NiNode::UpdateDownwardPass( time );
	if ( LODModelManager::specularMapsEnabled ) {
		UpdateShapes();
	}
}

/*
================
SpecularPass::GetEffectState
================
*/
// FUNCTION: STBC 0x004edcd0
NiPointer<NiDynamicEffectState> SpecularPass::GetEffectState() {
	return effectState;
}

/*
================
SpecularPass::UpdateEffectsDownward

Keeps the parent's effect state for Display, which lights the pass's
shapes itself: the children are given none.
================
*/
// FUNCTION: STBC 0x004edcf0
void SpecularPass::UpdateEffectsDownward( NiDynamicEffectState *parentState ) {
	if ( parentState != NULL ) {
		NiDynamicEffectState *lastState = NiDynamicEffectState::GetCachedState();
		if ( parentState->Equal( lastState ) ) {
			effectState = lastState;
		} else {
			effectState = parentState;
			NiDynamicEffectState::SetCachedState( parentState );
		}
	} else {
		effectState = NULL;
	}
}

/*
================
SpecularPass::Display

Lights the pass's shapes and draws them. Each directional light bright
enough adds its highlight to the vertex colours: the sixteenth power of
the cosine between the vertex normal and the halfway vector of the light
and the view direction, scaled by the light's dimmer and the pass's Ks, in
the light's diffuse colour. The sum is then clamped and multiplied by the
model shape's own vertex colours. Nothing is drawn without such a light.
================
*/
// FUNCTION: STBC 0x004edde0
// PARKED: three residues in a body that otherwise matches: retail branches on the flags of the light cast (this build tests the pointer again), copies the diffuse colour member by member (this build through its address), and loads z*nz first in the first highlight loop (this build x*nx; the second loop agrees)
void SpecularPass::Display( NiCamera *camera ) {
	if ( !LODModelManager::specularMapsEnabled ) {
		return;
	}
	if ( model == NULL ) {
		return;
	}

	unsigned int lightCount = 0;
	NiTMapIterator pos;
	NiTriShape *shape;
	PassShape *passShape;

	NiDynamicEffectState::ListItem *lightPos = effectState->GetLightHeadPos();
	while ( lightPos ) {
		NiDynamicEffect *effect = effectState->GetNextLight( lightPos );
		NiDirectionalLight *light = NiDynamicCast( NiDirectionalLight, effect );
		if ( light == NULL ) {
			continue;
		}
		NiColor color = light->GetDiffuseColor();
		float intensity = light->GetDimmer() * specularKs;
		if ( intensity <= 8.0f / 255.0f ) {
			continue;
		}
		lightCount++;

		NiPoint3 view = -camera->GetWorldDirection();
		NiPoint3 halfway = -( light->GetWorldRotate() * NiPoint3::UNIT_X ) + view;
		float length = sqrtf( halfway.SqrLength() );
		if ( length <= 0.0001f ) {
			continue;
		}
		halfway = halfway / length;

		pos = shapes.GetFirstPos();
		while ( pos ) {
			shapes.GetNext( pos, shape, passShape );
			NiTriShape *drawn = ( (SpecularPassShape *)passShape )->passShape;
			if ( drawn == NULL ) {
				continue;
			}
			NiColorA *colors = drawn->GetShapeData()->GetColors();
			NiPoint3 *normals = passShape->shape->GetShapeData()->GetNormals();
			unsigned short vertexCount = drawn->GetVertexCount();
			if ( colors == NULL || normals == NULL ) {
				continue;
			}
			NiPoint3 localHalfway = halfway * drawn->GetWorldRotate();
			unsigned short i;
			if ( lightCount == 1 ) {
				// the first light sets the colours
				for ( i = 0; i < vertexCount; i++ ) {
					float highlight = normals[i].Dot( localHalfway );
					if ( highlight > 0.0f ) {
						highlight *= highlight;
						highlight *= highlight;
						highlight *= highlight;
						highlight *= highlight;
						highlight *= intensity;
						if ( highlight > 1.0f ) {
							highlight = 1.0f;
						}
						colors[i].r = color.r * highlight;
						colors[i].g = color.g * highlight;
						colors[i].b = color.b * highlight;
					} else {
						colors[i].r = 0.0f;
						colors[i].g = 0.0f;
						colors[i].b = 0.0f;
					}
				}
			} else {
				// the others add to them
				for ( i = 0; i < vertexCount; i++ ) {
					float highlight = normals[i].Dot( localHalfway );
					if ( highlight > 0.0f ) {
						highlight *= highlight;
						highlight *= highlight;
						highlight *= highlight;
						highlight *= highlight;
						highlight *= intensity;
						if ( highlight > 1.0f ) {
							highlight = 1.0f;
						}
						colors[i].r += highlight * color.r;
						colors[i].g += highlight * color.g;
						colors[i].b += highlight * color.b;
					}
				}
			}
		}
	}
	if ( lightCount <= 0 ) {
		return;
	}

	pos = shapes.GetFirstPos();
	while ( pos ) {
		shapes.GetNext( pos, shape, passShape );
		NiColorA *shapeColors = passShape->shape->GetShapeData()->GetColors();
		NiColorA *colors = ( (SpecularPassShape *)passShape )->passShape->GetShapeData()->GetColors();
		unsigned short vertexCount = passShape->shape->GetVertexCount();
		unsigned short i;
		if ( shapeColors != NULL ) {
			if ( lightCount > 1 ) {
				for ( i = 0; i < vertexCount; i++ ) {
					if ( colors[i].r > 1.0f ) {
						colors[i].r = 1.0f;
					}
					if ( colors[i].g > 1.0f ) {
						colors[i].g = 1.0f;
					}
					if ( colors[i].b > 1.0f ) {
						colors[i].b = 1.0f;
					}
					colors[i].r *= shapeColors[i].r;
					colors[i].g *= shapeColors[i].g;
					colors[i].b *= shapeColors[i].b;
				}
			} else {
				for ( i = 0; i < vertexCount; i++ ) {
					colors[i].r *= shapeColors[i].r;
					colors[i].g *= shapeColors[i].g;
					colors[i].b *= shapeColors[i].b;
				}
			}
		} else if ( lightCount > 1 ) {
			for ( i = 0; i < vertexCount; i++ ) {
				if ( colors[i].r > 1.0f ) {
					colors[i].r = 1.0f;
				}
				if ( colors[i].g > 1.0f ) {
					colors[i].g = 1.0f;
				}
				if ( colors[i].b > 1.0f ) {
					colors[i].b = 1.0f;
				}
			}
		}
		( (SpecularPassShape *)passShape )->passShape->GetShapeData()->IncRevisionID();
	}
	NiNode::Display( camera );
}

/*
================
MotionBlurPass::CreateClone
================
*/
// FUNCTION: STBC 0x004ee3c0
NiObject *MotionBlurPass::CreateClone() {
	MotionBlurPass *clone = new MotionBlurPass( NULL );
	CopyMembers( clone );
	return clone;
}

/*
================
MotionBlurPass::CopyMembers
================
*/
// FUNCTION: STBC 0x004ee430
void MotionBlurPass::CopyMembers( MotionBlurPass *dest ) {
	dest->time = time;
	ModelPass::CopyMembers( dest );
}

/*
================
MotionBlurPassShape::~MotionBlurPassShape

Takes the pass's shape out of the pass.
================
*/
// FUNCTION: STBC 0x004ee450
MotionBlurPassShape::~MotionBlurPassShape() {
	if ( passShape != NULL && passShape->GetParent() != NULL ) {
		passShape->GetParent()->DetachChild( passShape );
	}
}

/*
================
MotionBlurPass::MotionBlurPass
================
*/
// FUNCTION: STBC 0x004ee540
MotionBlurPass::MotionBlurPass( NiAVObject *model_ ) : ModelPass( model_ ) {
}

/*
================
MotionBlurPass::UpdateDownwardPass
================
*/
// FUNCTION: STBC 0x004ee5f0
void MotionBlurPass::UpdateDownwardPass( float time_ ) {
	time = time_;
	NiNode::UpdateDownwardPass( time_ );
}

/*
================
MotionBlurPass::Display

While the model moves faster than 12 units, draws a translucent copy of
each of its shapes with the vertices that face away from the motion pulled
back along it: a smear behind the model. Only the triangles with a vertex
pulled back are drawn; those vertices are fully transparent, the others
30% opaque. The length of the smear pulses with time.
================
*/
// FUNCTION: STBC 0x004ee640
// PARKED: inline budget: retail calls DamageBitArray::IsSet (0x4BB2F0) three times in the triangle loop; this build expands them, and the register allocation follows from there (the statements and their order agree)
void MotionBlurPass::Display( NiCamera *camera ) {
	if ( !LODModelManager::motionBlurEnabled ) {
		return;
	}
	if ( model == NULL ) {
		return;
	}
	UpdateShapes();

	NiPoint3 velocity = model->GetWorldVelocity();
	float speed = velocity.Length();
	if ( speed <= 12.0f ) {
		return;
	}
	model->GetWorldBound();	// BUG: redundant: the bound is not used
	float pulse = sinf( time * 5.0f ) * 0.1f;

	NiTMapIterator pos = shapes.GetFirstPos();
	while ( pos ) {
		NiTriShape *key;
		PassShape *passShape;
		shapes.GetNext( pos, key, passShape );
		NiTriShape *shape = passShape->shape;
		NiTriShape *drawn = ( (MotionBlurPassShape *)passShape )->passShape;
		if ( shape == NULL ) {
			continue;
		}

		// the smear, in the shape's own space
		NiPoint3 localVelocity = TGModelUtils::WorldToLocalVector( shape, velocity );
		NiPoint3 direction = localVelocity / speed;
		NiPoint3 offset = ( localVelocity - direction * ( 1.0f / 3.0f ) ) / 20.0f;
		offset = offset + offset * pulse;

		NiColorA *drawnColors = drawn->GetShapeData()->GetColors();
		NiPoint3 *drawnVertices = drawn->GetShapeData()->GetVertices();
		NiColorA *colors = shape->GetShapeData()->GetColors();
		NiPoint3 *vertices = shape->GetShapeData()->GetVertices();
		NiPoint3 *normals = shape->GetShapeData()->GetNormals();
		unsigned short vertexCount = shape->GetVertexCount();
		DamageBitArray pulled( vertexCount );
		pulled.ClearAll();
		unsigned short i;
		if ( colors != NULL ) {
			for ( i = 0; i < vertexCount; i++ ) {
				if ( localVelocity.Dot( normals[i] ) < 0.0f ) {
					drawnVertices[i] = vertices[i] - offset;
					drawnColors[i] = colors[i];
					drawnColors[i].a = 0.0f;
					pulled.Set( i );
				} else {
					drawnVertices[i] = vertices[i];
					drawnColors[i] = colors[i];
					drawnColors[i].a = 0.3f;
				}
			}
		} else {
			for ( i = 0; i < vertexCount; i++ ) {
				if ( localVelocity.Dot( normals[i] ) < 0.0f ) {
					drawnVertices[i] = vertices[i] - offset;
					drawnColors[i] = NiColorA( 1.0f, 1.0f, 1.0f, 0.0f );
					pulled.Set( i );
				} else {
					drawnVertices[i] = vertices[i];
					drawnColors[i] = NiColorA::WHITE;
					drawnColors[i].a = 0.3f;
				}
			}
		}

		// the triangles with a vertex pulled back
		unsigned short triangleCount = shape->GetShapeData()->GetTriangleCount();
		DamageBitArray unused( triangleCount );	// BUG: redundant: never read
		unused.ClearAll();
		unsigned short *triList = shape->GetShapeData()->GetTriList();
		unsigned short *drawnTriList = drawn->GetShapeData()->GetTriList();
		unsigned short drawnIndices = 0;
		for ( i = 0; i < triangleCount; i++ ) {
			unsigned short index = i * 3;
			if ( triList[index] != triList[index + 1] ) {
				if ( pulled.IsSet( triList[index] ) || pulled.IsSet( triList[index + 1] ) || pulled.IsSet( triList[index + 2] ) ) {
					drawnTriList[drawnIndices] = triList[index];
					drawnTriList[drawnIndices + 1] = triList[index + 1];
					drawnTriList[drawnIndices + 2] = triList[index + 2];
					drawnIndices += 3;
				}
			}
		}
		drawn->GetShapeData()->SetTriangleCount( drawnIndices / 3 );
		drawn->UpdateEffects();
		drawn->GetShapeData()->IncRevisionID();
	}
	NiNode::Display( camera );
}

/*
================
MotionBlurPass::CreatePassShape

Only a shape with normals gets a record: Display needs them.
================
*/
// FUNCTION: STBC 0x004eee90
PassShape *MotionBlurPass::CreatePassShape( NiTriShape *shape ) {
	if ( shape != NULL && shape->GetShapeData()->GetNormals() != NULL ) {
		MotionBlurPassShape *passShape = new MotionBlurPassShape( shape );
		passShape->passShape = CreateShape( shape );
		return passShape;
	}
	return NULL;
}

/*
================
MotionBlurPass::UpdatePassShape

Copies the changed shape's vertices and texture coordinates again: in
place while its vertex count is the pass shape's, into a new pass shape
otherwise.
================
*/
// FUNCTION: STBC 0x004eef50
void MotionBlurPass::UpdatePassShape( PassShape *passShape ) {
	MotionBlurPassShape *blur = (MotionBlurPassShape *)passShape;
	NiTriShape *shape = blur->shape;
	unsigned short vertexCount = shape->GetVertexCount();
	NiTriShape *drawn = blur->passShape;
	if ( drawn == NULL ) {
		return;
	}
	if ( drawn->GetVertexCount() == vertexCount ) {
		memcpy( drawn->GetShapeData()->GetVertices(), shape->GetShapeData()->GetVertices(), vertexCount * sizeof( NiPoint3 ) );
		memcpy( drawn->GetShapeData()->GetTextures(), shape->GetShapeData()->GetTextures(), vertexCount * sizeof( NiPoint2 ) );
		return;
	}
	if ( drawn->GetParent() != NULL ) {
		drawn->GetParent()->DetachChild( drawn );
	}
	blur->passShape = CreateShape( shape );
}

/*
================
MotionBlurPass::UpdateShapes

Brings the records up to date with the model: a record whose shape has
left the model is deleted, and one whose shape data has changed is copied
again.
================
*/
// FUNCTION: STBC 0x004ef050
void MotionBlurPass::UpdateShapes() {
	NiTriShape *shape;
	PassShape *passShape;
	NiTMapIterator pos = shapes.GetFirstPos();
	while ( pos ) {
		shapes.GetNext( pos, shape, passShape );
		if ( passShape != NULL ) {
			passShape->inModel = false;
		}
	}
	FindShapes( model );

	pos = shapes.GetFirstPos();
	NiTList<void *> gone( 2 );
	while ( pos ) {
		shapes.GetNext( pos, shape, passShape );
		if ( !passShape->inModel ) {
			delete passShape;
			gone.AddTail( shape );
		} else if ( passShape->revisionID != passShape->shape->GetShapeData()->GetRevisionID() ) {
			UpdatePassShape( passShape );
			passShape->revisionID = passShape->shape->GetShapeData()->GetRevisionID();
		}
	}

	NiTListIterator gonePos = gone.GetHeadPos();
	while ( gonePos ) {
		shapes.RemoveAt( (NiTriShape *)gone.GetNext( gonePos ) );
	}
}

/*
================
MotionBlurPass::CreateShape

A shape for the pass to draw behind the model's shape: copies of its
vertices, normals and texture coordinates, vertex colours and a triangle
list for Display to fill in, and clones of its texture properties. It is
blended over what is drawn, modulated by its vertex colours, depth tested
but not written, and left out of the glow.
================
*/
// FUNCTION: STBC 0x004ef310
// PARKED: inline budget: retail calls the alpha property's NiProperty constructor (c2trace [34.1], share 61 = cost 61); own cost -2 flips it but expands the z-buffer test's SetBit [49.1], and no single extra site keeps both
NiTriShape *MotionBlurPass::CreateShape( NiTriShape *shape ) {
	if ( shape->GetParent() == NULL ) {
		return NULL;
	}

	unsigned short vertexCount = shape->GetVertexCount();
	NiPoint3 *vertices = new NiPoint3[vertexCount];
	memcpy( vertices, shape->GetShapeData()->GetVertices(), vertexCount * sizeof( NiPoint3 ) );
	NiPoint3 *normals = new NiPoint3[vertexCount];
	memcpy( normals, shape->GetShapeData()->GetNormals(), vertexCount * sizeof( NiPoint3 ) );
	NiColorA *colors = new NiColorA[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount];
	memcpy( textures, shape->GetShapeData()->GetTextures(), vertexCount * sizeof( NiPoint2 ) );
	unsigned short triangleCount = shape->GetShapeData()->GetTriangleCount();
	unsigned short *triList = new unsigned short[triangleCount * 3];
	memset( triList, 0, triangleCount * 3 * sizeof( unsigned short ) );
	NiTriShape *passShape = new NiTriShape( vertexCount, vertices, normals, colors, textures, triangleCount, triList );

	NiPointer<NiTextureProperty> texture = NiDynamicCast( NiTextureProperty, shape->GetProperty( NiTextureProperty::TEXTURE ) );
	if ( texture != NULL ) {
		passShape->AttachProperty( (NiProperty *)texture->Clone() );
	}
	NiPointer<NiMultiTextureProperty> multiTexture = NiDynamicCast( NiMultiTextureProperty, shape->GetProperty( NiMultiTextureProperty::MULTI_TEXTURE ) );
	if ( multiTexture != NULL ) {
		passShape->AttachProperty( (NiProperty *)multiTexture->Clone() );
	}
	GlowPass *glowPass = new GlowPass;
	passShape->AddExtraData( glowPass );
	AttachChild( passShape, false );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	alpha->SetNoSorter( true );
	passShape->AttachProperty( alpha );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE_VERTEX );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_TRILERP );
	passShape->AttachProperty( textureMode );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_AMB_DIFF );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E_A_D );
	passShape->AttachProperty( vertexColor );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	passShape->AttachProperty( zBuffer );

	// the fog property goes to the pass itself, once for every shape made
	NiFogProperty *fog = new NiFogProperty;
	fog->SetFog( false );
	AttachProperty( fog );

	passShape->UpdateProperties();
	passShape->Update( 0.0f );
	return passShape;
}
