/*
===============================================================================

	SpaceCamera.cpp

===============================================================================
*/

#include <iostream>
#include <stdlib.h>
#include "SpaceCamera.h"
#include "BridgeSet.h"
#include "ViewScreenObject.h"
#include "TGAnimNode.h"
#include "TGIconManager.h"
#include "TGIconGroup.h"
#include "TGSystemWrapperClass.h"
#include "TGStream.h"
#include "NiCamera.h"
#include "NiPlane.h"
#include "NiAlphaProperty.h"
#include "NiMaterialProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "NiVertexColorProperty.h"
#include "NiZBufferProperty.h"
#include "TGEventHandlerTable.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiFlipController.h"
#include "NiFloatKey.h"
#include "NiKeyframeController.h"
#include "NiKeyframeData.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiPosKey.h"
#include "NiRotKey.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00430370 _$E507
// FUNCTION: STBC 0x00430390 _$E506

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0042fba0 _$E32
// FUNCTION: STBC 0x0042fbb0 _$E29
// FUNCTION: STBC 0x0042fbc0 _$E31
// FUNCTION: STBC 0x0042fbd0 _$E30
// FUNCTION: STBC 0x0042fbe0 _$E38
// FUNCTION: STBC 0x0042fbf0 _$E35
// FUNCTION: STBC 0x0042fc00 _$E37
// FUNCTION: STBC 0x0042fc10 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0042e740 _$E290
// FUNCTION: STBC 0x0042e750 _$E289
// FUNCTION: STBC 0x0042e760 _$E322
// FUNCTION: STBC 0x0042e770 _$E321
// FUNCTION: STBC 0x0042e780 _$E313
// FUNCTION: STBC 0x0042e790 _$E312
// FUNCTION: STBC 0x0042e7a0 _$E299
// FUNCTION: STBC 0x0042e7b0 _$E298
// FUNCTION: STBC 0x0042e7c0 _$E326
// FUNCTION: STBC 0x0042e7d0 _$E325
// FUNCTION: STBC 0x0042e7e0 _$E330
// FUNCTION: STBC 0x0042e7f0 _$E329
// FUNCTION: STBC 0x0042fc20 _$E66
// FUNCTION: STBC 0x0042fc30 _$E65
// FUNCTION: STBC 0x0042fc40 _$E269
// FUNCTION: STBC 0x0042fc50 _$E268
// FUNCTION: STBC 0x0042fc60 _$E77
// FUNCTION: STBC 0x0042fc70 _$E76
// FUNCTION: STBC 0x0042fc80 _$E81
// FUNCTION: STBC 0x0042fc90 _$E80
// FUNCTION: STBC 0x0042fca0 _$E85
// FUNCTION: STBC 0x0042fcb0 _$E84
// FUNCTION: STBC 0x0042fcc0 _$E89
// FUNCTION: STBC 0x0042fcd0 _$E88
// FUNCTION: STBC 0x0042fce0 _$E206
// FUNCTION: STBC 0x0042fcf0 _$E205
// FUNCTION: STBC 0x0042fd20 _$E148
// FUNCTION: STBC 0x0042fd30 _$E147
// FUNCTION: STBC 0x0042fd40 _$E161
// FUNCTION: STBC 0x0042fd50 _$E160
// FUNCTION: STBC 0x0042fd60 _$E211
// FUNCTION: STBC 0x0042fd70 _$E210
// FUNCTION: STBC 0x0042fd80 _$E274
// FUNCTION: STBC 0x0042fd90 _$E273
// FUNCTION: STBC 0x0042fda0 _$E215
// FUNCTION: STBC 0x0042fdb0 _$E214
// FUNCTION: STBC 0x0042fdc0 _$E278
// FUNCTION: STBC 0x0042fdd0 _$E277
// FUNCTION: STBC 0x0042fde0 _$E285
// FUNCTION: STBC 0x0042fdf0 _$E284
// FUNCTION: STBC 0x0042fe00 _$E340
// FUNCTION: STBC 0x0042fe10 _$E339
// FUNCTION: STBC 0x0042fe20 _$E344
// FUNCTION: STBC 0x0042fe30 _$E343
// FUNCTION: STBC 0x0042fe40 _$E106
// FUNCTION: STBC 0x0042fe50 _$E105
// FUNCTION: STBC 0x0042fe60 _$E227
// FUNCTION: STBC 0x0042fe70 _$E226
// FUNCTION: STBC 0x0042fe80 _$E234
// FUNCTION: STBC 0x0042fe90 _$E233
// FUNCTION: STBC 0x0042fea0 _$E238
// FUNCTION: STBC 0x0042feb0 _$E237
// FUNCTION: STBC 0x0042fec0 _$E242
// FUNCTION: STBC 0x0042fed0 _$E241
// FUNCTION: STBC 0x0042fee0 _$E384
// FUNCTION: STBC 0x0042fef0 _$E383
// FUNCTION: STBC 0x0042ff00 _$E390
// FUNCTION: STBC 0x0042ff10 _$E389
// FUNCTION: STBC 0x0042ff20 _$E348
// FUNCTION: STBC 0x0042ff30 _$E347
// FUNCTION: STBC 0x0042ff40 _$E352
// FUNCTION: STBC 0x0042ff50 _$E351
// FUNCTION: STBC 0x0042ff60 _$E246
// FUNCTION: STBC 0x0042ff70 _$E245
// FUNCTION: STBC 0x0042ff80 _$E250
// FUNCTION: STBC 0x0042ff90 _$E249
// FUNCTION: STBC 0x0042ffa0 _$E356
// FUNCTION: STBC 0x0042ffb0 _$E355
// FUNCTION: STBC 0x0042ffc0 _$E360
// FUNCTION: STBC 0x0042ffd0 _$E359
// FUNCTION: STBC 0x0042ffe0 _$E364
// FUNCTION: STBC 0x0042fff0 _$E363
// FUNCTION: STBC 0x00430000 _$E368
// FUNCTION: STBC 0x00430010 _$E367
// FUNCTION: STBC 0x00430020 _$E372
// FUNCTION: STBC 0x00430030 _$E371
// FUNCTION: STBC 0x00430040 _$E376
// FUNCTION: STBC 0x00430050 _$E375
// FUNCTION: STBC 0x00430080 _$E111
// FUNCTION: STBC 0x00430090 _$E110
// FUNCTION: STBC 0x004300a0 _$E380
// FUNCTION: STBC 0x004300b0 _$E379
// FUNCTION: STBC 0x004300c0 _$E116
// FUNCTION: STBC 0x004300d0 _$E115
// FUNCTION: STBC 0x004300e0 _$E394
// FUNCTION: STBC 0x004300f0 _$E393
// FUNCTION: STBC 0x00430100 _$E398
// FUNCTION: STBC 0x00430110 _$E397
// FUNCTION: STBC 0x00430120 _$E402
// FUNCTION: STBC 0x00430130 _$E401
// FUNCTION: STBC 0x00430140 _$E406
// FUNCTION: STBC 0x00430150 _$E405
// FUNCTION: STBC 0x00430160 _$E125
// FUNCTION: STBC 0x00430170 _$E124
// FUNCTION: STBC 0x00430180 _$E410
// FUNCTION: STBC 0x00430190 _$E409
// FUNCTION: STBC 0x004301a0 _$E414
// FUNCTION: STBC 0x004301b0 _$E413
// FUNCTION: STBC 0x004301c0 _$E418
// FUNCTION: STBC 0x004301d0 _$E417
// FUNCTION: STBC 0x004301e0 _$E131
// FUNCTION: STBC 0x004301f0 _$E130
// FUNCTION: STBC 0x00430240 _$E136
// FUNCTION: STBC 0x00430250 _$E135
// FUNCTION: STBC 0x00430270 _$E166
// FUNCTION: STBC 0x00430280 _$E165
// FUNCTION: STBC 0x00430290 _$E422
// FUNCTION: STBC 0x004302a0 _$E421
// FUNCTION: STBC 0x004302b0 _$E171
// FUNCTION: STBC 0x004302c0 _$E170
// FUNCTION: STBC 0x004302d0 _$E426
// FUNCTION: STBC 0x004302e0 _$E425
// FUNCTION: STBC 0x004302f0 _$E430
// FUNCTION: STBC 0x00430300 _$E429
// FUNCTION: STBC 0x00430310 _$E176
// FUNCTION: STBC 0x00430320 _$E175
// FUNCTION: STBC 0x00430330 _$E434
// FUNCTION: STBC 0x00430340 _$E433
// FUNCTION: STBC 0x00430350 _$E141
// FUNCTION: STBC 0x00430360 _$E140
// FUNCTION: STBC 0x004303a0 _$E438
// FUNCTION: STBC 0x004303b0 _$E437
// FUNCTION: STBC 0x004303c0 _$E442
// FUNCTION: STBC 0x004303d0 _$E441
// FUNCTION: STBC 0x004303e0 _$E181
// FUNCTION: STBC 0x004303f0 _$E180
// FUNCTION: STBC 0x00430400 _$E186
// FUNCTION: STBC 0x00430410 _$E185
// FUNCTION: STBC 0x00430420 _$E446
// FUNCTION: STBC 0x00430430 _$E445
// FUNCTION: STBC 0x00430440 _$E450
// FUNCTION: STBC 0x00430450 _$E449
// FUNCTION: STBC 0x00430460 _$E454
// FUNCTION: STBC 0x00430470 _$E453
// FUNCTION: STBC 0x00430480 _$E458
// FUNCTION: STBC 0x00430490 _$E457
// FUNCTION: STBC 0x004304a0 _$E462
// FUNCTION: STBC 0x004304b0 _$E461
// FUNCTION: STBC 0x004304c0 _$E466
// FUNCTION: STBC 0x004304d0 _$E465
// FUNCTION: STBC 0x004304e0 _$E470
// FUNCTION: STBC 0x004304f0 _$E469
// FUNCTION: STBC 0x00430500 _$E474
// FUNCTION: STBC 0x00430510 _$E473
// FUNCTION: STBC 0x00430560 _$E191
// FUNCTION: STBC 0x00430570 _$E190
// FUNCTION: STBC 0x00430590 _$E478
// FUNCTION: STBC 0x004305a0 _$E477
// FUNCTION: STBC 0x004305b0 _$E196
// FUNCTION: STBC 0x004305c0 _$E195
// FUNCTION: STBC 0x004305d0 _$E201
// FUNCTION: STBC 0x004305e0 _$E200
// FUNCTION: STBC 0x004305f0 _$E482
// FUNCTION: STBC 0x00430600 _$E481
// FUNCTION: STBC 0x00430610 _$E487
// FUNCTION: STBC 0x00430620 _$E486
// FUNCTION: STBC 0x00430630 _$E491
// FUNCTION: STBC 0x00430640 _$E490

// DATA: STBC 0x008d96f4
bool SpaceCamera::spaceDustInGame = true;

// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x0042e800 _$E497
// FUNCTION: STBC 0x0042e810 _$E493
// FUNCTION: STBC 0x0042e820 _$E496
// FUNCTION: STBC 0x0042e830 _$E494
// DATA: STBC 0x0097f228
TGEventHandlerTable SpaceCamera::classHandlers;

// DATA: STBC 0x008895ac
const float SpaceCamera::DUST_MAX_DISTANCE = 40.0f;
// DATA: STBC 0x008895b0
const float SpaceCamera::DUST_MIN_DISTANCE = 8.0f;
// DATA: STBC 0x008895b4
const float SpaceCamera::DUST_SIZE = 0.01f;
// DATA: STBC 0x008895b8
const float SpaceCamera::DUST_HALF_WIDTH = 0.25f;

/*
================
SpaceCamera::AddToSet

Adds the camera to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x0042e850
void SpaceCamera::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, SPACECAMERA_TYPE );
		CameraObjectClass::AddToSet( set );
	}
}

/*
================
SpaceCamera::RemoveFromSet
================
*/
// FUNCTION: STBC 0x0042e880
void SpaceCamera::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, SPACECAMERA_TYPE );
		CameraObjectClass::RemoveFromSet( set );
	}
}

/*
================
SpaceCamera::GetClassObjects

The set's space cameras and their number.
================
*/
// FUNCTION: STBC 0x0042e8b0
SpaceCamera **SpaceCamera::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (SpaceCamera **)set->objectTable.GetObjects( SPACECAMERA_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
SpaceCamera::GetNumClassObjects

The number of the set's space cameras.
================
*/
// FUNCTION: STBC 0x0042e8e0
int SpaceCamera::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( SPACECAMERA_TYPE );
	}
	return 0;
}

/*
================
SpaceCamera::GetObjectByID

The space camera with the given ID in the set, or among every object when
the set is NULL; NULL when it is none or not a space camera.
================
*/
// FUNCTION: STBC 0x0042e900
SpaceCamera *SpaceCamera::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( SPACECAMERA_TYPE ) ) {
			return (SpaceCamera *)object;
		}
		return NULL;
	}
	return (SpaceCamera *)set->GetClassObject( SPACECAMERA_TYPE, id );
}

/*
================
SpaceCamera::GetObject

The space camera of the name in the set, or in any set when the set is
NULL, the last set first.
================
*/
// FUNCTION: STBC 0x0042e970
SpaceCamera *SpaceCamera::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( SPACECAMERA_TYPE, name );
		}
		return (SpaceCamera *)object;
	}
	return (SpaceCamera *)set->GetClassObject( SPACECAMERA_TYPE, name );
}

/*
================
SpaceCamera::SpaceCamera

The loader's constructor: the stream fills the members. A camera made
without an ID is set up at once, without a name.
================
*/
// FUNCTION: STBC 0x0042e9d0 ??0SpaceCamera@@QAE@J@Z
SpaceCamera::SpaceCamera( long id ) : CameraObjectClass( id ) {
	if ( id == 0 ) {
		Initialize( "" );
	}
}

/*
================
SpaceCamera::GetObjType
================
*/
// FUNCTION: STBC 0x0042ea40
int SpaceCamera::GetObjType( void ) {
	return SPACECAMERA_TYPE;
}

/*
================
SpaceCamera::IsTypeOf
================
*/
// FUNCTION: STBC 0x0042ea50
bool SpaceCamera::IsTypeOf( int type ) {
	if ( type == SPACECAMERA_TYPE ) {
		return true;
	}
	return CameraObjectClass::IsTypeOf( type );
}

/*
================
SpaceCamera::GetClassName
================
*/
// FUNCTION: STBC 0x0042eab0
char *SpaceCamera::GetClassName( void ) {
	return "SpaceCamera";
}

/*
================
SpaceCamera::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0042eac0
char *SpaceCamera::GetSwigTypeName( void ) {
	return "_p_SpaceCamera";
}

/*
================
SpaceCamera::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0042ead0
char *SpaceCamera::GetPythonClassName( void ) {
	return "SpaceCameraPtr";
}

/*
================
SpaceCamera::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0042eae0
TGEventHandlerTable *SpaceCamera::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
SpaceCamera::SpaceCamera

A new camera of the name.
================
*/
// FUNCTION: STBC 0x0042eb20 ??0SpaceCamera@@QAE@PAD@Z
SpaceCamera::SpaceCamera( char *name_ ) {
	Initialize( name_ );
}

/*
================
SpaceCamera::Initialize

Shows the dust, makes the NetImmerse camera with the system renderer and
the name and gives it the dust frustum; a keyframe controller without
data, and a new animation node above it, which becomes the object's
NetImmerse object.
================
*/
// FUNCTION: STBC 0x0042eb90
void SpaceCamera::Initialize( char *name_ ) {
	spaceDustForCamera = true;
	NiRender *renderer = g_kSystemWrapper.GetRenderer();
	NiRect<float> port( 0.0f, 1.0f, 1.0f, 0.0f );
	NiCamera *niCamera = new NiCamera;
	camera = niCamera;
	niCamera->SetViewPort( port );
	niCamera->SetRenderer( renderer );
	niCamera->SetName( name_ );
	SetDustFrustum( niCamera );
	niCamera->Update( 0.0f );
	NiKeyframeController *controller = new NiKeyframeController;
	controller->SetData( NULL );
	controller->SetTarget( niCamera );
	TGAnimNode *node = new TGAnimNode;
	node->SetName( name_ );
	node->AttachChild( niCamera, false );
	SetName( name_ );
	SetNiObject( node );
}

/*
================
SpaceCamera::~SpaceCamera
================
*/
// FUNCTION: STBC 0x0042ed60
SpaceCamera::~SpaceCamera( void ) {
	DeleteSpaceDust();
}

/*
================
SpaceCamera::IsRenderedCamera
================
*/
// FUNCTION: STBC 0x0042ede0
bool SpaceCamera::IsRenderedCamera( void ) {
	SetClass *set = g_kSetManager.GetRenderedSet();
	bool onViewScreen = false;
	BridgeSet *bridge = BridgeSet::Cast( set );
	if ( bridge != NULL && bridge->GetViewScreen() != NULL && bridge->GetViewScreen()->GetRemoteCam() == this ) {
		onViewScreen = true;
	}
	if ( set != NULL && set->GetActiveCamera() == this ) {
		return true;
	}
	return onViewScreen;
}

/*
================
SpaceCamera::Update

Makes the dust when it should be shown and is not, and moves it along with
the camera while the camera is the one drawn.
================
*/
// FUNCTION: STBC 0x0042ee30
void SpaceCamera::Update( float time ) {
	CameraObjectClass::Update( time );
	if ( dust == NULL && spaceDustForCamera && spaceDustInGame ) {
		StartSpaceDust();
	}
	if ( IsRenderedCamera() == true && spaceDustForCamera && spaceDustInGame && dust != NULL ) {
		UpdateSpaceDust( time );
	}
}

/*
================
SpaceCamera::UpdateSpaceDust

Puts back in front of the camera each particle behind it, outside the
side planes of its view or farther than DUST_MAX_DISTANCE.
================
*/
// FUNCTION: STBC 0x0042eec0
// PARKED: x87 operand order: the call set agrees (the near plane's normal copy expands once the camera's location and direction are named); the setup dot and the near-plane distance load their products in another order (z, y, x and y, x, z in retail)
void SpaceCamera::UpdateSpaceDust( float time ) {
	float maxDistanceSquared = DUST_MAX_DISTANCE * DUST_MAX_DISTANCE;
	NiCamera *niCamera = camera;
	NiPoint3 *positions = dust->GetParticlesData()->GetVertices();
	NiPlane **planes = niCamera->GetPlanes();
	NiPlane nearPlane;
	const NiPoint3 &direction = niCamera->GetWorldDirection();
	const NiPoint3 &location = niCamera->GetWorldLocation();
	nearPlane.SetNormal( direction );
	nearPlane.SetConstant( direction * location );
	for ( int i = 0; i < DUST_COUNT; i++ ) {
		if ( nearPlane.Distance( positions[i] ) < 0.0f
			|| planes[NiCamera::LEFT_PLANE]->Distance( positions[i] ) < 0.0f
			|| planes[NiCamera::RIGHT_PLANE]->Distance( positions[i] ) < 0.0f
			|| planes[NiCamera::TOP_PLANE]->Distance( positions[i] ) < 0.0f
			|| planes[NiCamera::BOTTOM_PLANE]->Distance( positions[i] ) < 0.0f
			|| ( positions[i] - location ).SqrLength() > maxDistanceSquared ) {
			PlaceDustParticle( i );
		}
	}
	dust->GetParticlesData()->GetBound().unknown10 = true;
	dust->GetParticlesData()->IncRevisionID();
}

/*
================
SpaceCamera::PlaceDustParticle

Puts the particle at a random place in front of the camera, between
DUST_MIN_DISTANCE and DUST_MAX_DISTANCE away and inside its view.
================
*/
// FUNCTION: STBC 0x0042f0c0
// DRAFT: instructions identical; only the frame slots of the offsets and the three points differ (retail puts the up offset at 0x20, this build at 0x14)
void SpaceCamera::PlaceDustParticle( int index ) {
	NiPoint3 *positions = dust->GetParticlesData()->GetVertices();
	float distance = rand() * ( DUST_MAX_DISTANCE - DUST_MIN_DISTANCE ) * ( 1.0f / RAND_MAX ) + DUST_MIN_DISTANCE;
	float across = NiSymmetricRandom();
	float up = NiSymmetricRandom();
	NiCamera *niCamera = camera;
	const NiFrustum &frustum = niCamera->GetViewFrustum();
	NiPoint3 upOffset = niCamera->GetWorldUpVector() * distance * up * frustum.top;
	NiPoint3 rightOffset = niCamera->GetWorldRightVector() * distance * across * frustum.right;
	positions[index] = niCamera->GetWorldLocation() + niCamera->GetWorldDirection() * distance + upOffset + rightOffset;
}

/*
================
SpaceCamera::SetSpaceDustInGame

Shows or hides the dust of every space camera that shows it.
================
*/
// FUNCTION: STBC 0x0042f2d0
void SpaceCamera::SetSpaceDustInGame( bool enable ) {
	if ( enable != spaceDustInGame ) {
		int count;
		SpaceCamera **cameras = GetAllSpaceCameras( &count );
		for ( int i = 0; i < count; i++ ) {
			SpaceCamera *camera = cameras[i];
			if ( camera->spaceDustForCamera == true ) {
				if ( !enable ) {
					camera->DeleteSpaceDust();
				} else {
					camera->StartSpaceDust();
				}
			}
		}
		delete [] cameras;
		spaceDustInGame = enable;
	}
}

/*
================
SpaceCamera::CreateSpaceDust

Makes the dust: count particles of DUST_SIZE with the tenth icon of the
effect textures, blended over the scene without writing the depth, under
the set's effect root, each placed in front of the camera. Without the
icon the game shows no dust.
================
*/
// FUNCTION: STBC 0x0042f380
void SpaceCamera::CreateSpaceDust( int count ) {
	NiPoint3 *positions = new NiPoint3[count];
	dust = new NiParticles( count, positions, NULL, NULL );
	dust->GetParticlesData()->SetRadius( DUST_SIZE );

	TGIconGroup *effectTextures = g_kIconManager.GetIconGroup( "EffectTextures" );
	NiTextureProperty *texture = effectTextures->GetIconTexture( 10 );
	if ( texture == NULL ) {
		spaceDustInGame = false;
		return;
	}
	dust->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	textureMode->SetClampMode( NiTextureModeProperty::CLAMP_S_CLAMP_T );
	textureMode->SetFilterMode( NiTextureModeProperty::FILTER_BILERP );
	dust->AttachProperty( textureMode );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	dust->AttachProperty( vertexColor );

	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetEmittance( NiColor::WHITE );
	dust->AttachProperty( material );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	dust->AttachProperty( zBuffer );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetSrcBlendMode( NiAlphaProperty::ALPHA_SRCALPHA );
	alpha->SetDestBlendMode( NiAlphaProperty::ALPHA_INVSRCALPHA );
	alpha->SetAlphaBlending( true );
	dust->AttachProperty( alpha );

	if ( GetContainingSet() ) {
		GetContainingSet()->GetEffectRoot()->AttachChild( dust, false );
	}
	dust->UpdateProperties();
	for ( int i = 0; i < count; i++ ) {
		PlaceDustParticle( i );
	}
	dust->GetParticlesData()->GetBound().unknown10 = true;
}

/*
================
SpaceCamera::DeleteSpaceDust

Takes the dust out of the scene and lets it go.
================
*/
// FUNCTION: STBC 0x0042f780
void SpaceCamera::DeleteSpaceDust( void ) {
	if ( dust != NULL ) {
		if ( dust->GetParent() != NULL ) {
			dust->GetParent()->DetachChild( dust );
		}
		dust = NULL;
	}
}

/*
================
SpaceCamera::SetSpaceDustForCamera

Shows or hides the camera's dust, while the game shows dust.
================
*/
// FUNCTION: STBC 0x0042f7f0
void SpaceCamera::SetSpaceDustForCamera( bool enable ) {
	if ( spaceDustForCamera != enable ) {
		if ( !enable ) {
			if ( spaceDustInGame == true ) {
				DeleteSpaceDust();
			}
		} else if ( spaceDustInGame == true ) {
			StartSpaceDust();
		}
		spaceDustForCamera = enable;
	}
}

/*
================
SpaceCamera::GetAllSpaceCameras

Every set's space cameras in one new array, and their number; NULL when
there are none.
================
*/
// FUNCTION: STBC 0x0042f860
SpaceCamera **SpaceCamera::GetAllSpaceCameras( int *count ) {
	*count = 0;
	int numSets = g_kSetManager.GetNumSets();
	SetClass **sets = g_kSetManager.GetSets();
	if ( numSets == 0 ) {
		return NULL;
	}
	SpaceCamera ***lists = new SpaceCamera **[numSets];
	int *counts = new int[numSets];
	int total = 0;
	int i;
	for ( i = 0; i < numSets; i++ ) {
		lists[i] = GetClassObjects( sets[i], &counts[i] );
		total += counts[i];
	}
	if ( total == 0 ) {
		// BUG: the two lists are not freed
		return NULL;
	}
	SpaceCamera **cameras = new SpaceCamera *[total];
	int n = 0;
	for ( i = 0; i < numSets; i++ ) {
		for ( int j = 0; j < counts[i]; j++ ) {
			cameras[n++] = lists[i][j];
		}
	}
	delete [] counts;
	delete [] lists;
	*count = total;
	return cameras;
}

/*
================
SpaceCamera::SetContainingSet

The dust goes with the camera into the new set.
================
*/
// FUNCTION: STBC 0x0042f980
void SpaceCamera::SetContainingSet( SetClass *set ) {
	BaseObjectClass::SetContainingSet( set );
	DeleteSpaceDust();
	if ( spaceDustForCamera && spaceDustInGame && set != NULL ) {
		StartSpaceDust();
	}
}

/*
================
SpaceCamera::WriteToStream
================
*/
// FUNCTION: STBC 0x0042f9e0
bool SpaceCamera::WriteToStream( TGStream *stream ) {
	if ( !CameraObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( spaceDustForCamera );
	return true;
}

/*
================
SpaceCamera::ReadFromStream
================
*/
// FUNCTION: STBC 0x0042fa10
bool SpaceCamera::ReadFromStream( TGStream *stream ) {
	if ( !CameraObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	spaceDustForCamera = stream->ReadBool();
	SetDustFrustum( camera );
	return true;
}

/*
================
SpaceCamera::PostFixUp
================
*/
// FUNCTION: STBC 0x0042fa50
void SpaceCamera::PostFixUp( void ) {
	CameraObjectClass::PostFixUp();
	DeleteSpaceDust();
	if ( spaceDustForCamera && spaceDustInGame && containingSet != NULL ) {
		StartSpaceDust();
	}
}

/*
================
SpaceCamera::SetDustFrustum

Narrows the camera to the dust's view: DUST_HALF_WIDTH to each side, scaled
by the screen's aspect vertically, from 1 to 5000.
================
*/
// FUNCTION: STBC 0x0042fab0
void SpaceCamera::SetDustFrustum( NiCamera *niCamera ) {
	NiRender *renderer = g_kSystemWrapper.GetRenderer();
	float aspect = (float)renderer->VirtualSlot20() / (float)renderer->VirtualSlot19();
	NiFrustum frustum;
	frustum.left = -DUST_HALF_WIDTH;
	frustum.right = DUST_HALF_WIDTH;
	frustum.top = aspect * DUST_HALF_WIDTH;
	frustum.bottom = -( aspect * DUST_HALF_WIDTH );
	frustum.nearDistance = 1.0f;
	frustum.farDistance = 5000.0f;
	niCamera->SetViewFrustum( frustum );
}
