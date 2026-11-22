/*
===============================================================================

	LensFlare.cpp

===============================================================================
*/

#include <iostream>
#include "LensFlare.h"
#include "ImageManagerClass.h"
#include "ObjectClass.h"
#include "SetClass.h"
#include "Backdrop.h"
#include "NiCamera.h"
#include "TGStream.h"
#include "renderer/NiImage.h"
#include "NiAlphaProperty.h"
#include "NiColor.h"
#include "NiMath.h"
#include "NiTextureModeProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004e34d0 _$E45
// FUNCTION: STBC 0x004e34e0 _$E44
// FUNCTION: STBC 0x004e3510 _$E56
// FUNCTION: STBC 0x004e3520 _$E55
// FUNCTION: STBC 0x004e3540 _$E60
// FUNCTION: STBC 0x004e3550 _$E59
// FUNCTION: STBC 0x004e3560 _$E64
// FUNCTION: STBC 0x004e3570 _$E63
// FUNCTION: STBC 0x004e3580 _$E68
// FUNCTION: STBC 0x004e3590 _$E67
// FUNCTION: STBC 0x004e34f0 _$E234
// FUNCTION: STBC 0x004e3500 _$E233
// FUNCTION: STBC 0x004e35a0 _$E192
// FUNCTION: STBC 0x004e35b0 _$E191
// FUNCTION: STBC 0x004e35c0 _$E134
// FUNCTION: STBC 0x004e35d0 _$E133
// FUNCTION: STBC 0x004e35e0 _$E147
// FUNCTION: STBC 0x004e35f0 _$E146
// FUNCTION: STBC 0x004e3600 _$E197
// FUNCTION: STBC 0x004e3610 _$E196
// FUNCTION: STBC 0x004e3620 _$E239
// FUNCTION: STBC 0x004e3630 _$E238
// FUNCTION: STBC 0x004e3650 _$E201
// FUNCTION: STBC 0x004e3660 _$E200
// FUNCTION: STBC 0x004e3670 _$E243
// FUNCTION: STBC 0x004e3680 _$E242
// FUNCTION: STBC 0x004e3690 _$E250
// FUNCTION: STBC 0x004e36a0 _$E249
// FUNCTION: STBC 0x004e36b0 _$E272
// FUNCTION: STBC 0x004e36c0 _$E271
// FUNCTION: STBC 0x004e36d0 _$E276
// FUNCTION: STBC 0x004e36e0 _$E275
// FUNCTION: STBC 0x004e36f0 _$E92
// FUNCTION: STBC 0x004e3700 _$E91
// FUNCTION: STBC 0x004e3710 _$E258
// FUNCTION: STBC 0x004e3720 _$E257
// FUNCTION: STBC 0x004e3730 _$E265
// FUNCTION: STBC 0x004e3740 _$E264
// FUNCTION: STBC 0x004e3750 _$E288
// FUNCTION: STBC 0x004e3760 _$E287
// FUNCTION: STBC 0x004e3770 _$E292
// FUNCTION: STBC 0x004e3780 _$E291
// FUNCTION: STBC 0x004e3790 _$E332
// FUNCTION: STBC 0x004e37a0 _$E331
// FUNCTION: STBC 0x004e37b0 _$E338
// FUNCTION: STBC 0x004e37c0 _$E337
// FUNCTION: STBC 0x004e3810 _$E280
// FUNCTION: STBC 0x004e3820 _$E279
// FUNCTION: STBC 0x004e3830 _$E284
// FUNCTION: STBC 0x004e3840 _$E283
// FUNCTION: STBC 0x004e3850 _$E296
// FUNCTION: STBC 0x004e3860 _$E295
// FUNCTION: STBC 0x004e3870 _$E300
// FUNCTION: STBC 0x004e3880 _$E299
// FUNCTION: STBC 0x004e3890 _$E304
// FUNCTION: STBC 0x004e38a0 _$E303
// FUNCTION: STBC 0x004e38b0 _$E308
// FUNCTION: STBC 0x004e38c0 _$E307
// FUNCTION: STBC 0x004e38d0 _$E312
// FUNCTION: STBC 0x004e38e0 _$E311
// FUNCTION: STBC 0x004e38f0 _$E316
// FUNCTION: STBC 0x004e3900 _$E315
// FUNCTION: STBC 0x004e3910 _$E320
// FUNCTION: STBC 0x004e3920 _$E319
// FUNCTION: STBC 0x004e3930 _$E324
// FUNCTION: STBC 0x004e3940 _$E323
// FUNCTION: STBC 0x004e3950 _$E97
// FUNCTION: STBC 0x004e3960 _$E96
// FUNCTION: STBC 0x004e3970 _$E328
// FUNCTION: STBC 0x004e3980 _$E327
// FUNCTION: STBC 0x004e3990 _$E102
// FUNCTION: STBC 0x004e39a0 _$E101
// FUNCTION: STBC 0x004e39b0 _$E342
// FUNCTION: STBC 0x004e39c0 _$E341
// FUNCTION: STBC 0x004e39d0 _$E346
// FUNCTION: STBC 0x004e39e0 _$E345
// FUNCTION: STBC 0x004e39f0 _$E350
// FUNCTION: STBC 0x004e3a00 _$E349
// FUNCTION: STBC 0x004e3a10 _$E354
// FUNCTION: STBC 0x004e3a20 _$E353
// FUNCTION: STBC 0x004e3a30 _$E111
// FUNCTION: STBC 0x004e3a40 _$E110
// FUNCTION: STBC 0x004e3a50 _$E358
// FUNCTION: STBC 0x004e3a60 _$E357
// FUNCTION: STBC 0x004e3a70 _$E362
// FUNCTION: STBC 0x004e3a80 _$E361
// FUNCTION: STBC 0x004e3a90 _$E366
// FUNCTION: STBC 0x004e3aa0 _$E365
// FUNCTION: STBC 0x004e3ab0 _$E117
// FUNCTION: STBC 0x004e3ac0 _$E116
// FUNCTION: STBC 0x004e3ad0 _$E122
// FUNCTION: STBC 0x004e3ae0 _$E121
// FUNCTION: STBC 0x004e3af0 _$E152
// FUNCTION: STBC 0x004e3b00 _$E151
// FUNCTION: STBC 0x004e3c40 _$E370
// FUNCTION: STBC 0x004e3c50 _$E369
// FUNCTION: STBC 0x004e3c60 _$E157
// FUNCTION: STBC 0x004e3c70 _$E156
// FUNCTION: STBC 0x004e3c80 _$E374
// FUNCTION: STBC 0x004e3c90 _$E373
// FUNCTION: STBC 0x004e3ca0 _$E378
// FUNCTION: STBC 0x004e3cb0 _$E377
// FUNCTION: STBC 0x004e3cc0 _$E162
// FUNCTION: STBC 0x004e3cd0 _$E161
// FUNCTION: STBC 0x004e3ce0 _$E382
// FUNCTION: STBC 0x004e3cf0 _$E381
// FUNCTION: STBC 0x004e3d00 _$E127
// FUNCTION: STBC 0x004e3d10 _$E126
// FUNCTION: STBC 0x004e3d20 _$E386
// FUNCTION: STBC 0x004e3d30 _$E385
// FUNCTION: STBC 0x004e3d40 _$E390
// FUNCTION: STBC 0x004e3d50 _$E389
// FUNCTION: STBC 0x004e3d60 _$E167
// FUNCTION: STBC 0x004e3d70 _$E166
// FUNCTION: STBC 0x004e3d80 _$E172
// FUNCTION: STBC 0x004e3d90 _$E171
// FUNCTION: STBC 0x004e3da0 _$E394
// FUNCTION: STBC 0x004e3db0 _$E393
// FUNCTION: STBC 0x004e3dc0 _$E398
// FUNCTION: STBC 0x004e3dd0 _$E397
// FUNCTION: STBC 0x004e3de0 _$E402
// FUNCTION: STBC 0x004e3df0 _$E401
// FUNCTION: STBC 0x004e3e00 _$E406
// FUNCTION: STBC 0x004e3e10 _$E405
// FUNCTION: STBC 0x004e3e20 _$E410
// FUNCTION: STBC 0x004e3e30 _$E409
// FUNCTION: STBC 0x004e3e40 _$E414
// FUNCTION: STBC 0x004e3e50 _$E413
// FUNCTION: STBC 0x004e3e60 _$E418
// FUNCTION: STBC 0x004e3e70 _$E417
// FUNCTION: STBC 0x004e3e80 _$E422
// FUNCTION: STBC 0x004e3e90 _$E421
// FUNCTION: STBC 0x004e3ea0 _$E177
// FUNCTION: STBC 0x004e3eb0 _$E176
// FUNCTION: STBC 0x004e3ec0 _$E426
// FUNCTION: STBC 0x004e3ed0 _$E425
// FUNCTION: STBC 0x004e3ee0 _$E182
// FUNCTION: STBC 0x004e3ef0 _$E181
// FUNCTION: STBC 0x004e3f00 _$E187
// FUNCTION: STBC 0x004e3f10 _$E186
// FUNCTION: STBC 0x004e3f20 _$E430
// FUNCTION: STBC 0x004e3f30 _$E429
// FUNCTION: STBC 0x004e3f40 _$E435
// FUNCTION: STBC 0x004e3f50 _$E434
// FUNCTION: STBC 0x004e3f60 _$E439
// FUNCTION: STBC 0x004e3f70 _$E438

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x004e3c10 _$E454
// FUNCTION: STBC 0x004e3c30 _$E453

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004e3450 _$E32
// FUNCTION: STBC 0x004e3460 _$E29
// FUNCTION: STBC 0x004e3470 _$E31
// FUNCTION: STBC 0x004e3480 _$E30
// FUNCTION: STBC 0x004e3490 _$E38
// FUNCTION: STBC 0x004e34a0 _$E35
// FUNCTION: STBC 0x004e34b0 _$E37
// FUNCTION: STBC 0x004e34c0 _$E36

// Every lens flare, in creation order (invented name). A file-scope static,
// not a class member: its exit-time destructor has no guard, which a class
// static's has. Above it: the dynamic initialiser, the construction, the
// registration of the destructor and the destructor.
// FUNCTION: STBC 0x004e1b10 _$E444
// FUNCTION: STBC 0x004e1b20 _$E441
// FUNCTION: STBC 0x004e1b50 _$E443
// FUNCTION: STBC 0x004e1b60 _$E442
// DATA: STBC 0x009861e0
static NiTList<LensFlare *> lensFlares( 2 );

/*
================
LensFlare::DeleteAll

Deletes every lens flare; each one takes itself out of the list.
================
*/
// FUNCTION: STBC 0x004e1bb0 ?DeleteAll@LensFlare@@SAXXZ
void LensFlare::DeleteAll( void ) {
	NiTListIterator pos = lensFlares.GetHeadPos();
	while ( pos ) {
		LensFlare *flare = lensFlares.GetNext( pos );
		delete flare;
	}
}

/*
================
LensFlare::SaveStatics

Writes the number of lens flares, then each one.
================
*/
// FUNCTION: STBC 0x004e1be0 ?SaveStatics@LensFlare@@SA_NPAVTGStream@@@Z
bool LensFlare::SaveStatics( TGStream *stream ) {
	NiTListIterator pos = lensFlares.GetHeadPos();
	stream->WriteInt( lensFlares.GetSize() );
	while ( pos ) {
		if ( !lensFlares.GetNext( pos )->WriteToStream( stream ) ) {
			return false;
		}
	}
	return true;
}

/*
================
LensFlare::LoadStatics

Replaces the lens flares with the ones the stream holds.
================
*/
// FUNCTION: STBC 0x004e1c30 ?LoadStatics@LensFlare@@SA_NPAVTGStream@@@Z
bool LensFlare::LoadStatics( TGStream *stream ) {
	DeleteAll();
	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		LensFlare *flare = new LensFlare;
		if ( !flare->ReadFromStream( stream ) ) {
			return false;
		}
	}
	return true;
}

/*
================
LensFlare::FixUpStatics
================
*/
// FUNCTION: STBC 0x004e1ca0 ?FixUpStatics@LensFlare@@SA_NXZ
bool LensFlare::FixUpStatics( void ) {
	NiTListIterator pos = lensFlares.GetHeadPos();
	while ( pos ) {
		if ( !lensFlares.GetNext( pos )->FixUp() ) {
			return false;
		}
	}
	return true;
}

/*
================
LensFlare::Create
================
*/
// FUNCTION: STBC 0x004e1cd0 ?Create@LensFlare@@SAPAV1@PAVSetClass@@@Z
LensFlare *LensFlare::Create( SetClass *set ) {
	return new LensFlare( set );
}

/*
================
LensFlareController::Update

Moves the fan's rim: each pair of rim vertices at an angle sits at the
radius the wave gives that angle at the time.
================
*/
// FUNCTION: STBC 0x004e1d30
// DRAFT: register allocation: retail copies the rate and amplitude to the frame and walks the vertices in ECX without saving EDI; the arithmetic and loop match
void LensFlareController::Update( float time ) {
	if ( lastUpdate != time ) {
		lastUpdate = time;
		NiTriShape *shape = (NiTriShape *)target;
		NiPoint3 *vertices = shape->GetShapeData()->GetVertices();
		int sides = ( shape->GetShapeData()->GetVertexCount() - 1 ) / 2;
		float step = NI_TWO_PI / sides;
		float angle = 0.0f;
		float phase = rate * time;
		float scale = (float)sin( phase + angle ) * amplitude + 1.0f;
		float s = (float)sin( angle );
		float c = (float)cos( angle );
		for ( int i = 0; i < sides; i++ ) {
			vertices[i * 2 + 1].x = c * scale;
			vertices[i * 2 + 1].y = s * scale;
			angle += step;
			s = (float)sin( angle );
			c = (float)cos( angle );
			scale = (float)sin( phase + angle ) * amplitude + 1.0f;
			vertices[i * 2 + 2].x = c * scale;
			vertices[i * 2 + 2].y = s * scale;
		}
		shape->GetShapeData()->IncRevisionID();
	}
}

/*
================
LensFlareNode::UpdateDownwardPass

Keeps the time and puts the bound back to everywhere.
================
*/
// FUNCTION: STBC 0x004e1e40
void LensFlareNode::UpdateDownwardPass( float time ) {
	unknown12C = time;
	NiNode::UpdateDownwardPass( time );
	UpdateWorldBound();
}

/*
================
LensFlareNode::UpdateWorldBound

The flare is never culled: its bound is infinite.
================
*/
// FUNCTION: STBC 0x004e1e70
void LensFlareNode::UpdateWorldBound() {
	worldBound.SetCenterAndRadius( NiPoint3::ZERO, NI_INFINITY );
}

/*
================
LensFlareNode::LensFlareNode

Attaches the material, a z-buffer property that neither tests nor writes,
additive alpha blending, emissive-only vertex colours and bilinear clamped
texturing, and sets the pick up for the occlusion test.
================
*/
// FUNCTION: STBC 0x004e1ea0
LensFlareNode::LensFlareNode( SetClass *set_ ) {
	unknown130 = 0;
	set = set_;
	unknown128 = 1.0f;

	material = new NiMaterialProperty;
	material->SetAlpha( unknown128 );
	material->SetEmittance( NiColor( 1.0f, 1.0f, 1.0f ) );
	AttachProperty( material );

	NiZBufferProperty *zbuffer = new NiZBufferProperty;
	zbuffer->SetZBufferWrite( false );
	zbuffer->SetZBufferTest( false );
	AttachProperty( zbuffer );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	alpha->SetAlphaTesting( false );
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_ONE );
	AttachProperty( alpha );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	AttachProperty( vertexColor );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	AttachProperty( textureMode );

	worldBound.SetCenterAndRadius( NiPoint3::ZERO, NI_INFINITY );

	pick.SetFrontOnly( true );
	pick.SetIntersectType( NiPick::TRIANGLE_INTERSECT );
	pick.SetReturnTexture( false );
	pick.SetReturnNormal( false );
	pick.SetReturnSmoothNormal( false );
	pick.SetReturnColor( false );
	pick.SetObserveAppCullFlag( true );
	pick.SetSortType( NiPick::NO_SORT );
}

/*
================
LensFlareNode::~LensFlareNode
================
*/
// FUNCTION: STBC 0x004e2250
LensFlareNode::~LensFlareNode() {
	delete [] unknown138;
	delete [] unknown13C;
}

/*
================
LensFlareNode::VirtualSlot35

Places the flare for the camera: unknown130 picks the direction of the
flare's source (0 to 5: along the source's +x, +y, +z, -x, -y or -z axis;
6: toward the source); the node sits five units from the eye that way.
While the source is in front of the camera the flare fades out a step
when it is off the screen or something in the set is in the way, in
otherwise; its images then face the camera, scaled and spread along the
screen with the angle.
================
*/
// FUNCTION: STBC 0x004e22f0
// DRAFT: control flow and frame: the call set and statements agree; retail's tail merge keeps the first positive and negative axis case's copy (ours the last), both pick loops end in je exit / jmp top with the step block after both, and the temporaries sit 4 bytes apart
void LensFlareNode::VirtualSlot35( NiCamera *camera ) {
	ObjectClass *source = set->GetObjectByID( sourceID );
	if ( source != NULL ) {
		NiPoint3 direction;
		switch ( unknown130 ) {
			case 0:
				direction = source->GetNode()->GetWorldRotate() * NiPoint3::UNIT_X;
				break;
			case 1:
				direction = source->GetNode()->GetWorldRotate() * NiPoint3::UNIT_Y;
				break;
			case 2:
				direction = source->GetNode()->GetWorldRotate() * NiPoint3::UNIT_Z;
				break;
			case 3:
				direction = source->GetNode()->GetWorldRotate() * -NiPoint3::UNIT_X;
				break;
			case 4:
				direction = source->GetNode()->GetWorldRotate() * -NiPoint3::UNIT_Y;
				break;
			case 5:
				direction = source->GetNode()->GetWorldRotate() * -NiPoint3::UNIT_Z;
				break;
			case 6:
				direction = source->GetNode()->GetWorldTranslate() - camera->GetWorldTranslate();
				direction.Unitize();
				break;
		}

		float facing = camera->GetWorldDirection() * direction;
		NiPoint3 offset = direction * 5.0f;
		NiPoint3 position;
		position = offset + camera->GetWorldLocation();
		SetTranslate( offset );

		if ( facing > 0.0f ) {
			float step = -0.15f;
			float x;
			float y;
			if ( camera->WorldPtToScreenPt( position, x, y ) ) {
				set->StartPick( (NiPoint3 *)&camera->GetWorldLocation(), &direction, true );
				ObjectClass *object = set->GetPickedObject();
				if ( unknown130 != 6 ) {
					while ( object != NULL ) {
						if ( !object->IsTypeOf( BACKDROP_TYPE ) && object->VirtualSlot63( &camera->GetWorldLocation(), direction, &pick ) ) {
							step = 0.15f;
							break;
						}
						object = set->GetPickedObject();
					}
				} else {
					NiPoint3 toSource = source->GetNode()->GetWorldTranslate() - camera->GetWorldTranslate();
					while ( object != NULL ) {
						if ( !object->IsTypeOf( BACKDROP_TYPE ) && object != source
							&& object->VirtualSlot63( &camera->GetWorldLocation(), toSource, &pick )
							&& pick.GetRecord( 0 )->GetDistance() <= 1.0f ) {
							step = 0.15f;
							break;
						}
						object = set->GetPickedObject();
					}
				}
			} else {
				step = 0.15f;
			}

			unknown128 += step;
			if ( unknown128 > 1.0f ) {
				unknown128 = 1.0f;
			} else if ( unknown128 < 0.0f ) {
				unknown128 = 0.0f;
			}
			material->SetAlpha( 1.0f - unknown128 );

			if ( unknown128 < 1.0f ) {
				NiMatrix3 rotation;
				NiPoint3 view = camera->GetWorldDirection();
				NiPoint3 up = camera->GetWorldUpVector();
				NiPoint3 right = camera->GetWorldRightVector();
				rotation.SetCol( 0, right.x, right.y, right.z );
				rotation.SetCol( 1, up.x, up.y, up.z );
				rotation.SetCol( 2, view.x, view.y, view.z );
				for ( unsigned int i = 0; i < GetArrayCount(); i++ ) {
					NiAVObject *child = children.GetAt( i );
					if ( child != NULL ) {
						float scale = facing * unknown13C[i] * 5.0f;
						if ( scale <= 0.0f ) {
							scale = -scale;
						}
						child->SetScale( scale );
						if ( unknown138[i] != 0.0f ) {
							child->SetTranslate( NiPoint3( 0.0f, camera->GetWorldRightVector() * direction * unknown138[i] * 5.0f,
								camera->GetWorldUpVector() * direction * unknown138[i] * -5.0f ) );
						}
						child->SetRotate( rotation );
						child->UpdateDownwardPass( unknown12C );
					}
				}
			}
		}
	}
	NiAVObject::VirtualSlot35( camera );
}

/*
================
CreateFan

A unit disc of sides triangles around the centre, each with its own two
rim vertices: the centre is textured at ( 0.5, 1 ), each triangle's rim
at ( 1, 0 ) and ( 0, 0 ). The name is invented.
================
*/
// FUNCTION: STBC 0x004e2970
// PARKED: schedule tie in the loop: the strength-reduced steps (vertices #23, textures #24, triangles #25, all 0x8000) go in their input order, so the textures step fills c1 V; retail's picks need the triangles step first and vertices before textures in the input (the order the loop optimiser created the induction pointers), which no store order of the body gives
NiTriShape *CreateFan( int sides ) {
	float step = NI_TWO_PI / sides;
	int vertexCount = sides * 2 + 1;
	NiPoint3 *vertices = new NiPoint3[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount];
	unsigned short *triangles = new unsigned short[sides * 3];

	vertices[0].x = vertices[0].y = vertices[0].z = 0.0f;
	textures[0].x = 0.5f;
	textures[0].y = 1.0f;

	float angle = 0.0f;
	float s = 0.0f;
	float c = 1.0f;
	for ( int i = 0; i < sides; i++ ) {
		vertices[i * 2 + 1].x = c;
		vertices[i * 2 + 1].y = s;
		vertices[i * 2 + 1].z = 0.0f;
		textures[i * 2 + 1].x = 1.0f;
		textures[i * 2 + 1].y = 0.0f;
		angle += step;
		s = (float)sin( angle );
		c = (float)cos( angle );
		vertices[i * 2 + 2].x = c;
		vertices[i * 2 + 2].y = s;
		vertices[i * 2 + 2].z = 0.0f;
		textures[i * 2 + 2].x = 0.0f;
		textures[i * 2 + 2].y = 0.0f;
		triangles[i * 3] = 0;
		triangles[i * 3 + 1] = i * 2 + 1;
		triangles[i * 3 + 2] = i * 2 + 2;
	}
	return new NiTriShape( vertexCount, vertices, NULL, NULL, textures, sides, triangles );
}

/*
================
LensFlare::LensFlare

Joins the list of lens flares.
================
*/
// FUNCTION: STBC 0x004e2b10
LensFlare::LensFlare( SetClass *set_ ) : images( 2 ) {
	lensFlares.AddTail( this );
	set = set_;
}

/*
================
LensFlare::~LensFlare

Leaves the list of lens flares, deletes the flares and takes the node out
of the scene.
================
*/
// FUNCTION: STBC 0x004e2c30 ??1LensFlare@@QAE@XZ
LensFlare::~LensFlare( void ) {
	lensFlares.Remove( this );
	NiTListIterator pos = images.GetHeadPos();
	while ( pos ) {
		delete images.GetNext( pos );
	}
	if ( node && node->GetParent() ) {
		node->GetParent()->DetachChild( node );
	}
}

/*
================
LensFlare::SetSource

Keeps the source object's ID and the second argument, and passes both on
to the node once it is built.
================
*/
// FUNCTION: STBC 0x004e2d50 ?SetSource@LensFlare@@QAEXPAVObjectClass@@H@Z
void LensFlare::SetSource( ObjectClass *source, int unknown1C_ ) {
	unknown1C = unknown1C_;
	sourceID = source->GetObjID();
	if ( node != NULL ) {
		node->SetSource( sourceID, unknown1C );
	}
}

/*
================
LensFlare::WriteToStream

Writes the set's and the source's IDs, the second argument of SetSource
and each flare.
================
*/
// FUNCTION: STBC 0x004e2d80 ?WriteToStream@LensFlare@@QAE_NPAVTGStream@@@Z
bool LensFlare::WriteToStream( TGStream *stream ) {
	stream->WriteID( set->GetObjID() );
	stream->WriteID( sourceID );
	stream->WriteInt( unknown1C );
	stream->WriteInt( images.GetSize() );
	NiTListIterator pos = images.GetHeadPos();
	while ( pos ) {
		LensFlareImage *flare = images.GetNext( pos );
		stream->WriteFloat( flare->unknown08 );
		stream->WriteFloat( flare->unknown0C );
		stream->WriteInt( flare->unknown04 );
		if ( flare->image ) {
			stream->VirtualSlot11( flare->image->GetURL() );
		} else {
			stream->VirtualSlot11( NULL );
		}
		stream->WriteFloat( flare->unknown10 );
		stream->WriteFloat( flare->unknown14 );
	}
	return true;
}

/*
================
LensFlare::ReadFromStream

Reads what WriteToStream wrote; the set stays an ID until FixUp.
================
*/
// FUNCTION: STBC 0x004e2e30 ?ReadFromStream@LensFlare@@QAE_NPAVTGStream@@@Z
// PARKED: register: the inlined list block constructor (shared NiTList.h) addresses the last item as [eax+ecx-8] where retail has [ecx+eax-8]; CameraMode::ReadFromStream has the same wall the other way round
bool LensFlare::ReadFromStream( TGStream *stream ) {
	set = (SetClass *)stream->ReadID();
	sourceID = stream->ReadID();
	unknown1C = stream->ReadInt();
	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		LensFlareImage *flare = new LensFlareImage;
		flare->unknown08 = stream->ReadFloat();
		flare->unknown0C = stream->ReadFloat();
		flare->unknown04 = stream->ReadInt();
		char *imageName = stream->VirtualSlot10( -1 );
		if ( imageName ) {
			flare->image = g_kImageManager.GetImage( imageName, true, true );
			delete [] imageName;
		}
		flare->unknown10 = stream->ReadFloat();
		flare->unknown14 = stream->ReadFloat();
		images.AddTail( flare );
	}
	return true;
}

/*
================
LensFlare::FixUp

Turns the set's ID back into the set and builds the flare.
================
*/
// FUNCTION: STBC 0x004e3030 ?FixUp@LensFlare@@QAE_NXZ
bool LensFlare::FixUp( void ) {
	set = (SetClass *)TGObject::GetTGObjectPtr( (long)set );
	Build();
	return true;
}

/*
================
LensFlare::AddFlare

Appends a flare to the list; its image comes from the image manager by name.
================
*/
// FUNCTION: STBC 0x4E3050 ?AddFlare@LensFlare@@QAEXHPADMMMM@Z
void LensFlare::AddFlare( int arg1, char *imageName, float arg3, float arg4, float arg5, float arg6 ) {
	LensFlareImage *flare = new LensFlareImage;

	flare->unknown04 = arg1;
	flare->unknown08 = arg3;
	flare->unknown0C = arg4;
	flare->unknown10 = arg5;
	flare->unknown14 = arg6;
	if ( imageName ) {
		flare->image = g_kImageManager.GetImage( imageName, true, true );
	}
	images.AddTail( flare );
}

/*
================
LensFlare::Build

Makes the node: a fan for each image, textured with it, wobbling when the
image has an amplitude, under a LensFlareNode that the set's foreground
draws.
================
*/
// FUNCTION: STBC 0x004e31b0
// DRAFT: inline budget: retail releases the old node through DecRefCount out of line; the per-image loop walks the two value arrays with one pointer difference
void LensFlare::Build( void ) {
	LensFlareNode *flareNode = new LensFlareNode( set );
	node = flareNode;
	flareNode->SetSource( sourceID, unknown1C );

	NiTListIterator pos = images.GetHeadPos();
	float *values138 = new float[images.GetSize()];
	float *values13C = new float[images.GetSize()];
	flareNode->unknown138 = values138;
	flareNode->unknown13C = values13C;
	int i = 0;
	while ( pos ) {
		LensFlareImage *flare = images.GetNext( pos );
		values138[i] = flare->unknown08;
		values13C[i] = flare->unknown0C;
		i++;
		NiTriShape *shape = CreateFan( flare->unknown04 );
		if ( flare->image ) {
			NiTextureProperty *texture = new NiTextureProperty;
			texture->SetImage( flare->image );
			shape->AttachProperty( texture );
		}
		if ( flare->unknown14 != 0.0f ) {
			LensFlareController *controller = new LensFlareController( flare->unknown10, flare->unknown14 );
			controller->SetTarget( shape );
		}
		flareNode->AttachChild( shape, false );
	}
	set->GetForegroundRoot()->AttachChild( flareNode, false );
	flareNode->UpdateProperties();
}
