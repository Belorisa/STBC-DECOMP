/*
===============================================================================

	ZoomCameraObjectClass.cpp

===============================================================================
*/

#include <iostream>
#include "NiMain.h"
#include "ZoomCameraObjectClass.h"
#include "NiCamera.h"
#include "NiNode.h"
#include "SetClass.h"
#include "SetManager.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "TGSystemWrapperClass.h"
#include "NiFloatKey.h"
#include "NiKeyframeController.h"
#include "NiKeyframeData.h"
#include "NiListener.h"
#include "NiPosKey.h"
#include "NiRotKey.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGAnimNode.h"
#include "NiMath.h"
#include <math.h>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0067b1a0 _$E491
// FUNCTION: STBC 0x0067b1c0 _$E490

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0067ab40 _$E32
// FUNCTION: STBC 0x0067ab50 _$E29
// FUNCTION: STBC 0x0067ab60 _$E31
// FUNCTION: STBC 0x0067ab70 _$E30
// FUNCTION: STBC 0x0067ab80 _$E38
// FUNCTION: STBC 0x0067ab90 _$E35
// FUNCTION: STBC 0x0067aba0 _$E37
// FUNCTION: STBC 0x0067abb0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00679bb0 _$E456
// FUNCTION: STBC 0x00679bc0 _$E455
// FUNCTION: STBC 0x00679bd0 _$E447
// FUNCTION: STBC 0x00679be0 _$E446
// FUNCTION: STBC 0x00679bf0 _$E433
// FUNCTION: STBC 0x00679c00 _$E432
// FUNCTION: STBC 0x00679c10 _$E460
// FUNCTION: STBC 0x00679c20 _$E459
// FUNCTION: STBC 0x00679c30 _$E464
// FUNCTION: STBC 0x00679c40 _$E463
// FUNCTION: STBC 0x0067abc0 _$E45
// FUNCTION: STBC 0x0067abd0 _$E44
// FUNCTION: STBC 0x0067abe0 _$E49
// FUNCTION: STBC 0x0067abf0 _$E48
// FUNCTION: STBC 0x0067ac00 _$E60
// FUNCTION: STBC 0x0067ac10 _$E59
// FUNCTION: STBC 0x0067ac20 _$E64
// FUNCTION: STBC 0x0067ac30 _$E63
// FUNCTION: STBC 0x0067ac40 _$E68
// FUNCTION: STBC 0x0067ac50 _$E67
// FUNCTION: STBC 0x0067ac60 _$E72
// FUNCTION: STBC 0x0067ac70 _$E71
// FUNCTION: STBC 0x0067ac80 _$E196
// FUNCTION: STBC 0x0067ac90 _$E195
// FUNCTION: STBC 0x0067aca0 _$E138
// FUNCTION: STBC 0x0067acb0 _$E137
// FUNCTION: STBC 0x0067acc0 _$E151
// FUNCTION: STBC 0x0067acd0 _$E150
// FUNCTION: STBC 0x0067ace0 _$E201
// FUNCTION: STBC 0x0067acf0 _$E200
// FUNCTION: STBC 0x0067ad00 _$E206
// FUNCTION: STBC 0x0067ad10 _$E205
// FUNCTION: STBC 0x0067ad20 _$E210
// FUNCTION: STBC 0x0067ad30 _$E209
// FUNCTION: STBC 0x0067ad40 _$E214
// FUNCTION: STBC 0x0067ad50 _$E213
// FUNCTION: STBC 0x0067ad60 _$E221
// FUNCTION: STBC 0x0067ad70 _$E220
// FUNCTION: STBC 0x0067ad80 _$E246
// FUNCTION: STBC 0x0067ad90 _$E245
// FUNCTION: STBC 0x0067ada0 _$E250
// FUNCTION: STBC 0x0067adb0 _$E249
// FUNCTION: STBC 0x0067adc0 _$E96
// FUNCTION: STBC 0x0067add0 _$E95
// FUNCTION: STBC 0x0067ade0 _$E232
// FUNCTION: STBC 0x0067adf0 _$E231
// FUNCTION: STBC 0x0067ae00 _$E239
// FUNCTION: STBC 0x0067ae10 _$E238
// FUNCTION: STBC 0x0067ae20 _$E262
// FUNCTION: STBC 0x0067ae30 _$E261
// FUNCTION: STBC 0x0067ae40 _$E266
// FUNCTION: STBC 0x0067ae50 _$E265
// FUNCTION: STBC 0x0067ae60 _$E306
// FUNCTION: STBC 0x0067ae70 _$E305
// FUNCTION: STBC 0x0067ae80 _$E312
// FUNCTION: STBC 0x0067ae90 _$E311
// FUNCTION: STBC 0x0067aea0 _$E254
// FUNCTION: STBC 0x0067aeb0 _$E253
// FUNCTION: STBC 0x0067aec0 _$E258
// FUNCTION: STBC 0x0067aed0 _$E257
// FUNCTION: STBC 0x0067aee0 _$E270
// FUNCTION: STBC 0x0067aef0 _$E269
// FUNCTION: STBC 0x0067af00 _$E274
// FUNCTION: STBC 0x0067af10 _$E273
// FUNCTION: STBC 0x0067af20 _$E278
// FUNCTION: STBC 0x0067af30 _$E277
// FUNCTION: STBC 0x0067af40 _$E282
// FUNCTION: STBC 0x0067af50 _$E281
// FUNCTION: STBC 0x0067af60 _$E286
// FUNCTION: STBC 0x0067af70 _$E285
// FUNCTION: STBC 0x0067af80 _$E290
// FUNCTION: STBC 0x0067af90 _$E289
// FUNCTION: STBC 0x0067afa0 _$E294
// FUNCTION: STBC 0x0067afb0 _$E293
// FUNCTION: STBC 0x0067afc0 _$E298
// FUNCTION: STBC 0x0067afd0 _$E297
// FUNCTION: STBC 0x0067afe0 _$E101
// FUNCTION: STBC 0x0067aff0 _$E100
// FUNCTION: STBC 0x0067b000 _$E302
// FUNCTION: STBC 0x0067b010 _$E301
// FUNCTION: STBC 0x0067b020 _$E106
// FUNCTION: STBC 0x0067b030 _$E105
// FUNCTION: STBC 0x0067b040 _$E316
// FUNCTION: STBC 0x0067b050 _$E315
// FUNCTION: STBC 0x0067b060 _$E320
// FUNCTION: STBC 0x0067b070 _$E319
// FUNCTION: STBC 0x0067b080 _$E324
// FUNCTION: STBC 0x0067b090 _$E323
// FUNCTION: STBC 0x0067b0a0 _$E328
// FUNCTION: STBC 0x0067b0b0 _$E327
// FUNCTION: STBC 0x0067b0c0 _$E115
// FUNCTION: STBC 0x0067b0d0 _$E114
// FUNCTION: STBC 0x0067b0e0 _$E332
// FUNCTION: STBC 0x0067b0f0 _$E331
// FUNCTION: STBC 0x0067b100 _$E336
// FUNCTION: STBC 0x0067b110 _$E335
// FUNCTION: STBC 0x0067b120 _$E340
// FUNCTION: STBC 0x0067b130 _$E339
// FUNCTION: STBC 0x0067b140 _$E121
// FUNCTION: STBC 0x0067b150 _$E120
// FUNCTION: STBC 0x0067b160 _$E126
// FUNCTION: STBC 0x0067b170 _$E125
// FUNCTION: STBC 0x0067b180 _$E156
// FUNCTION: STBC 0x0067b190 _$E155
// FUNCTION: STBC 0x0067b1d0 _$E344
// FUNCTION: STBC 0x0067b1e0 _$E343
// FUNCTION: STBC 0x0067b1f0 _$E161
// FUNCTION: STBC 0x0067b200 _$E160
// FUNCTION: STBC 0x0067b210 _$E348
// FUNCTION: STBC 0x0067b220 _$E347
// FUNCTION: STBC 0x0067b230 _$E352
// FUNCTION: STBC 0x0067b240 _$E351
// FUNCTION: STBC 0x0067b250 _$E166
// FUNCTION: STBC 0x0067b260 _$E165
// FUNCTION: STBC 0x0067b270 _$E356
// FUNCTION: STBC 0x0067b280 _$E355
// FUNCTION: STBC 0x0067b290 _$E131
// FUNCTION: STBC 0x0067b2a0 _$E130
// FUNCTION: STBC 0x0067b2b0 _$E360
// FUNCTION: STBC 0x0067b2c0 _$E359
// FUNCTION: STBC 0x0067b2d0 _$E364
// FUNCTION: STBC 0x0067b2e0 _$E363
// FUNCTION: STBC 0x0067b2f0 _$E171
// FUNCTION: STBC 0x0067b300 _$E170
// FUNCTION: STBC 0x0067b310 _$E176
// FUNCTION: STBC 0x0067b320 _$E175
// FUNCTION: STBC 0x0067b330 _$E368
// FUNCTION: STBC 0x0067b340 _$E367
// FUNCTION: STBC 0x0067b350 _$E372
// FUNCTION: STBC 0x0067b360 _$E371
// FUNCTION: STBC 0x0067b370 _$E376
// FUNCTION: STBC 0x0067b380 _$E375
// FUNCTION: STBC 0x0067b390 _$E380
// FUNCTION: STBC 0x0067b3a0 _$E379
// FUNCTION: STBC 0x0067b3b0 _$E384
// FUNCTION: STBC 0x0067b3c0 _$E383
// FUNCTION: STBC 0x0067b3d0 _$E388
// FUNCTION: STBC 0x0067b3e0 _$E387
// FUNCTION: STBC 0x0067b3f0 _$E392
// FUNCTION: STBC 0x0067b400 _$E391
// FUNCTION: STBC 0x0067b410 _$E396
// FUNCTION: STBC 0x0067b420 _$E395
// FUNCTION: STBC 0x0067b430 _$E181
// FUNCTION: STBC 0x0067b440 _$E180
// FUNCTION: STBC 0x0067b450 _$E400
// FUNCTION: STBC 0x0067b460 _$E399
// FUNCTION: STBC 0x0067b470 _$E186
// FUNCTION: STBC 0x0067b480 _$E185
// FUNCTION: STBC 0x0067b490 _$E191
// FUNCTION: STBC 0x0067b4a0 _$E190
// FUNCTION: STBC 0x0067b4b0 _$E468
// FUNCTION: STBC 0x0067b4c0 _$E467
// FUNCTION: STBC 0x0067b4d0 _$E473
// FUNCTION: STBC 0x0067b4e0 _$E472
// FUNCTION: STBC 0x0067b4f0 _$E477
// FUNCTION: STBC 0x0067b500 _$E476

// The handler table's dynamic initialiser and its exit-time destructor.
// FUNCTION: STBC 0x00679c50 _$E483
// FUNCTION: STBC 0x00679c60 _$E479
// FUNCTION: STBC 0x00679c70 _$E482
// FUNCTION: STBC 0x00679c80 _$E480
// DATA: STBC 0x00992468
TGEventHandlerTable ZoomCameraObjectClass::classHandlers;

/*
================
ZoomCameraObjectClass::AddToSet

Adds the camera to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00679ca0
void ZoomCameraObjectClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, ZOOMCAMERAOBJECTCLASS_TYPE );
		CameraObjectClass::AddToSet( set );
	}
}

/*
================
ZoomCameraObjectClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00679cd0
void ZoomCameraObjectClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, ZOOMCAMERAOBJECTCLASS_TYPE );
		CameraObjectClass::RemoveFromSet( set );
	}
}

/*
================
ZoomCameraObjectClass::GetClassObjects

The set's zoom cameras and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x00679d00
ZoomCameraObjectClass **ZoomCameraObjectClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (ZoomCameraObjectClass **)set->objectTable.GetObjects( ZOOMCAMERAOBJECTCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
ZoomCameraObjectClass::GetNumClassObjects

The number of the set's zoom cameras.
================
*/
// FUNCTION: STBC 0x00679d30
int ZoomCameraObjectClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( ZOOMCAMERAOBJECTCLASS_TYPE );
	}
	return 0;
}

/*
================
ZoomCameraObjectClass::GetObjectByID

The zoom camera with the given ID in the set, or among every object when
the set is NULL; NULL when it is none or of another class (invented name:
no SWIG wrapper calls it).
================
*/
// FUNCTION: STBC 0x00679d50
ZoomCameraObjectClass *ZoomCameraObjectClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( ZOOMCAMERAOBJECTCLASS_TYPE ) ) {
			return (ZoomCameraObjectClass *)object;
		}
		return NULL;
	}
	return (ZoomCameraObjectClass *)set->GetClassObject( ZOOMCAMERAOBJECTCLASS_TYPE, id );
}

/*
================
ZoomCameraObjectClass::GetObject

The zoom camera of the name in the set, or in any set when the set is
NULL, the last set first.
================
*/
// FUNCTION: STBC 0x00679dc0
ZoomCameraObjectClass *ZoomCameraObjectClass::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( ZOOMCAMERAOBJECTCLASS_TYPE, name );
		}
		return (ZoomCameraObjectClass *)object;
	}
	return (ZoomCameraObjectClass *)set->GetClassObject( ZOOMCAMERAOBJECTCLASS_TYPE, name );
}

/*
================
ZoomCameraObjectClass::Cast

The object as a ZoomCameraObjectClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00679e20
ZoomCameraObjectClass *ZoomCameraObjectClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( ZOOMCAMERAOBJECTCLASS_TYPE ) ) {
		return (ZoomCameraObjectClass *)object;
	}
	return NULL;
}

/*
================
ZoomCameraObjectClass::ZoomCameraObjectClass

Not zoomed, looking along -Y with Z up.
================
*/
// FUNCTION: STBC 0x00679e50 ??0ZoomCameraObjectClass@@QAE@J@Z
ZoomCameraObjectClass::ZoomCameraObjectClass( long id ) : CameraObjectClass( id ) {
	zoomed = false;
	zooming = false;
	shake = 0.0f;
	timeToNextShake = 0.0f;
	minZoom = 0.7f;
	maxZoom = 1.0f;
	zoomTime = 1.0f;
	zoomStartTime = -1.0f;
	forward.x = 0.0f;
	forward.y = -1.0f;
	forward.z = 0.0f;
	up.x = 0.0f;
	up.y = 0.0f;
	up.z = 1.0f;
}

/*
================
ZoomCameraObjectClass::GetObjType
================
*/
// FUNCTION: STBC 0x00679ed0
int ZoomCameraObjectClass::GetObjType( void ) {
	return ZOOMCAMERAOBJECTCLASS_TYPE;
}

/*
================
ZoomCameraObjectClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x00679ee0
bool ZoomCameraObjectClass::IsTypeOf( int type ) {
	if ( type == ZOOMCAMERAOBJECTCLASS_TYPE ) {
		return true;
	}
	return CameraObjectClass::IsTypeOf( type );
}

/*
================
ZoomCameraObjectClass::GetClassName
================
*/
// FUNCTION: STBC 0x00679f40
char *ZoomCameraObjectClass::GetClassName( void ) {
	return "ZoomCameraObjectClass";
}

/*
================
ZoomCameraObjectClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00679f50
char *ZoomCameraObjectClass::GetSwigTypeName( void ) {
	return "_p_ZoomCameraObjectClass";
}

/*
================
ZoomCameraObjectClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00679f60
char *ZoomCameraObjectClass::GetPythonClassName( void ) {
	return "ZoomCameraObjectClassPtr";
}

/*
================
ZoomCameraObjectClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00679f70
TGEventHandlerTable *ZoomCameraObjectClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ZoomCameraObjectClass::Create

A zoom camera with a new NetImmerse camera, "Camera01", 4:3 as wide as the
unzoomed scale makes it out to 600 units, with the system renderer, at x, y,
z, its up vector turned onto the direction and then rolled by the angle
about it; a keyframe controller without data, and a new animation node of
the name above it, which becomes the object's NetImmerse object.
================
*/
// FUNCTION: STBC 0x00679fc0
ZoomCameraObjectClass *ZoomCameraObjectClass::Create( float x, float y, float z, float angle, float dx, float dy, float dz, char *name_ ) {
	ZoomCameraObjectClass *zoomCamera = new ZoomCameraObjectClass;
	NiCamera *niCamera = new NiCamera;
	NiRect<float> port( 0.0f, 1.0f, 1.0f, 0.0f );
	NiFrustum frustum;
	frustum.left = zoomCamera->maxZoom * -0.5f;
	frustum.right = zoomCamera->maxZoom * 0.5f;
	frustum.top = zoomCamera->maxZoom * 0.375f;
	frustum.bottom = zoomCamera->maxZoom * -0.375f;
	frustum.nearDistance = 1.0f;
	frustum.farDistance = 600.0f;
	niCamera->SetViewFrustum( frustum );
	niCamera->SetViewPort( port );
	niCamera->SetRenderer( g_kSystemWrapper.GetRenderer() );
	niCamera->SetName( "Camera01" );
	niCamera->SetTranslate( NiPoint3( x, y, z ) );

	NiPoint3 direction( dx, dy, dz );
	NiPoint3 up = niCamera->GetWorldUpVector();
	NiPoint3 axis;
	axis = up.Cross( direction );
	axis.Unitize();
	if ( axis == NiPoint3::ZERO ) {
		axis = niCamera->GetWorldRightVector();
	}
	float cosine = up.Dot( direction );
	float turn;
	if ( -1.0f < cosine ) {
		if ( cosine < 1.0f ) {
			turn = (float)acos( cosine );
		} else {
			turn = 0.0f;
		}
	} else {
		turn = NI_PI;
	}
	NiMatrix3 rotation;
	rotation.MakeRotation( -turn, axis );
	niCamera->SetRotate( niCamera->GetRotate() * rotation );
	niCamera->Update( 0.0f );

	NiPoint3 rollAxis = niCamera->GetWorldUpVector();
	NiMatrix3 roll;
	roll.MakeRotation( angle, rollAxis );
	niCamera->SetRotate( roll * niCamera->GetRotate() );

	NiKeyframeController *controller = new NiKeyframeController;
	controller->SetData( NULL );
	controller->SetTarget( niCamera );
	TGAnimNode *node = new TGAnimNode;
	node->SetName( name_ );
	node->AttachChild( niCamera, false );
	zoomCamera->SetName( name_ );
	zoomCamera->SetNiObject( node );
	zoomCamera->camera = niCamera;
	return zoomCamera;
}

/*
================
ZoomCameraObjectClass::Update

Moves the zoom on toward its end and sets the frustum for it, shakes the
node while a shake lasts, then updates the camera.
================
*/
// FUNCTION: STBC 0x0067a490
void ZoomCameraObjectClass::Update( float time ) {
	float elapsed;
	if ( lastUpdateTime < 0.0f ) {
		elapsed = 0.001f;
	} else {
		elapsed = time - lastUpdateTime;
		if ( elapsed > 1.0f ) {
			elapsed = 1.0f;
		}
	}

	if ( zooming ) {
		float fraction = ( time - zoomStartTime ) / zoomTime;
		if ( fraction > 1.0f ) {
			fraction = 1.0f;
			zooming = false;
		}
		float scale;
		if ( zoomed ) {
			scale = ( maxZoom - minZoom ) * ( 1.0f - fraction ) + minZoom;
		} else {
			scale = ( maxZoom - minZoom ) * fraction + minZoom;
		}
		camera->SetViewFrustum( NiFrustum( scale * -0.5f, scale * 0.5f, scale * 0.375f, scale * -0.375f, 1.0f, 600.0f ) );
	}

	if ( shake != 0.0f ) {
		timeToNextShake -= elapsed;
		if ( timeToNextShake <= 0.0f ) {
			shakeTime -= 0.1f - timeToNextShake;
			timeToNextShake = 0.1f;
			if ( shakeTime < 0.0f ) {
				SetShake( 0.0f, 0.0f );
			} else {
				NiNode *shaken = node;
				if ( shaken != NULL ) {
					NiPoint3 offset;
					// BUG: the unsigned random number less 500 wraps instead of going negative
					offset.x = ( (unsigned int)g_kSystemWrapper.GetRandomNumber( 1000 ) - 500 ) * 0.002f;
					offset.y = ( (unsigned int)g_kSystemWrapper.GetRandomNumber( 1000 ) - 500 ) * 0.002f;
					offset.z = ( (unsigned int)g_kSystemWrapper.GetRandomNumber( 1000 ) - 500 ) * 0.002f;
					offset.Unitize();
					shaken->SetTranslate( offset * shake );
				}
			}
		}
	}
	CameraObjectClass::Update( time );
}

/*
================
ZoomCameraObjectClass::ToggleZoom

Switches the zoom on or off at the time (the scripts pass the game time);
a zoom still running turns back from where it is.
================
*/
// FUNCTION: STBC 0x0067a7b0
void ZoomCameraObjectClass::ToggleZoom( float time ) {
	float remaining = zoomTime + zoomStartTime;
	zoomed = !zoomed;
	remaining -= time;
	if ( remaining < 0.0f ) {
		remaining = 0.0f;
	}
	zooming = true;
	zoomStartTime = time - remaining;
}

/*
================
ZoomCameraObjectClass::UpdateViewFrustum

Gives the NetImmerse camera a frustum 4:3 in proportion, as wide as the
zoom makes it, from the near limit to 600 units.
================
*/
// FUNCTION: STBC 0x0067a800
void ZoomCameraObjectClass::UpdateViewFrustum( void ) {
	float scale;
	if ( !zoomed ) {
		scale = maxZoom;
	} else {
		scale = minZoom;
	}
	camera->SetViewFrustum( NiFrustum( scale * -0.5f, scale * 0.5f, scale * 0.375f, scale * -0.375f, 1.0f, 600.0f ) );
}

/*
================
ZoomCameraObjectClass::SetShake

The camera shakes by the amount for the time; without either, it stops
and its node returns to the origin.
================
*/
// FUNCTION: STBC 0x0067a8d0
void ZoomCameraObjectClass::SetShake( float amount, float time ) {
	if ( amount == 0.0f || time <= 0.0f ) {
		if ( node != NULL ) {
			node->SetTranslate( NiPoint3( 0.0f, 0.0f, 0.0f ) );
		}
	}
	shake = amount;
	shakeTime = time;
	timeToNextShake = 0;
}

/*
================
ZoomCameraObjectClass::LookForward

Points the camera along its forward direction, with its up vector.
================
*/
// FUNCTION: STBC 0x0067a930
void ZoomCameraObjectClass::LookForward( void ) {
	LookToward( &forward, &up, 1.0f );
}

/*
================
ZoomCameraObjectClass::WriteToStream
================
*/
// FUNCTION: STBC 0x0067a950
bool ZoomCameraObjectClass::WriteToStream( TGStream *stream ) {
	if ( !CameraObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( forward.x );
	stream->WriteFloat( forward.y );
	stream->WriteFloat( forward.z );
	stream->WriteFloat( up.x );
	stream->WriteFloat( up.y );
	stream->WriteFloat( up.z );
	stream->WriteFloat( minZoom );
	stream->WriteFloat( maxZoom );
	stream->WriteFloat( zoomTime );
	stream->WriteFloat( zoomStartTime );
	stream->WriteFloat( shake );
	stream->WriteFloat( shakeTime );
	stream->WriteFloat( timeToNextShake );
	stream->WriteBool( zoomed );
	stream->WriteBool( zooming );
	return true;
}

/*
================
ZoomCameraObjectClass::ReadFromStream

Reads what WriteToStream wrote, names the NetImmerse camera and sets its
frustum for the zoom read.
================
*/
// FUNCTION: STBC 0x0067aa40
bool ZoomCameraObjectClass::ReadFromStream( TGStream *stream ) {
	if ( !CameraObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	camera->SetName( "Camera01" );
	forward.x = stream->ReadFloat();
	forward.y = stream->ReadFloat();
	forward.z = stream->ReadFloat();
	up.x = stream->ReadFloat();
	up.y = stream->ReadFloat();
	up.z = stream->ReadFloat();
	minZoom = stream->ReadFloat();
	maxZoom = stream->ReadFloat();
	zoomTime = stream->ReadFloat();
	zoomStartTime = stream->ReadFloat();
	shake = stream->ReadFloat();
	shakeTime = stream->ReadFloat();
	timeToNextShake = stream->ReadFloat();
	zoomed = stream->ReadBool();
	zooming = stream->ReadBool();
	UpdateViewFrustum();
	return true;
}
