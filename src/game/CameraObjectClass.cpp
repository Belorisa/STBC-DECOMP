/*
===============================================================================

	CameraObjectClass.cpp

===============================================================================
*/

#include <iostream>
#include "CameraObjectClass.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiCamera.h"
#include "CameraMode.h"
#include "PyEmbed.h"
#include "TGSystemWrapperClass.h"
#include "renderer/NiRender.h"
#include "NiKeyframeController.h"
#include "TGAnimNode.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGStream.h"
#include "TGPointerMap.h"
#include "NiMath.h"
#include <math.h>
#include "TGAnimBlender.h"
#include "BezierCurve.h"
#include <string.h>
#include "NiApplication.h"
#include "UtopiaModule.h"
#include "TGIconManager.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0042e660 _$E546
// FUNCTION: STBC 0x0042e680 _$E545

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00429720 _$E32
// FUNCTION: STBC 0x00429730 _$E29
// FUNCTION: STBC 0x00429740 _$E31
// FUNCTION: STBC 0x00429750 _$E30
// FUNCTION: STBC 0x00429760 _$E38
// FUNCTION: STBC 0x00429770 _$E35
// FUNCTION: STBC 0x00429780 _$E37
// FUNCTION: STBC 0x00429790 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00429680 _$E314
// FUNCTION: STBC 0x00429690 _$E313
// FUNCTION: STBC 0x004296a0 _$E296
// FUNCTION: STBC 0x004296b0 _$E295
// FUNCTION: STBC 0x004296c0 _$E305
// FUNCTION: STBC 0x004296d0 _$E304
// FUNCTION: STBC 0x004296e0 _$E318
// FUNCTION: STBC 0x004296f0 _$E317
// FUNCTION: STBC 0x00429700 _$E322
// FUNCTION: STBC 0x00429710 _$E321
// FUNCTION: STBC 0x0042a9a0 _$E66
// FUNCTION: STBC 0x0042a9b0 _$E65
// FUNCTION: STBC 0x0042a9c0 _$E231
// FUNCTION: STBC 0x0042a9d0 _$E230
// FUNCTION: STBC 0x0042c1b0 _$E77
// FUNCTION: STBC 0x0042c1c0 _$E76
// FUNCTION: STBC 0x0042c8c0 _$E81
// FUNCTION: STBC 0x0042c8d0 _$E80
// FUNCTION: STBC 0x0042c8e0 _$E85
// FUNCTION: STBC 0x0042c8f0 _$E84
// FUNCTION: STBC 0x0042d0b0 _$E89
// FUNCTION: STBC 0x0042d0c0 _$E88
// FUNCTION: STBC 0x0042d450 _$E206
// FUNCTION: STBC 0x0042d460 _$E205
// FUNCTION: STBC 0x0042d470 _$E148
// FUNCTION: STBC 0x0042d480 _$E147
// FUNCTION: STBC 0x0042d490 _$E161
// FUNCTION: STBC 0x0042d4a0 _$E160
// FUNCTION: STBC 0x0042d4b0 _$E211
// FUNCTION: STBC 0x0042d4c0 _$E210
// FUNCTION: STBC 0x0042d4d0 _$E236
// FUNCTION: STBC 0x0042d4e0 _$E235
// FUNCTION: STBC 0x0042d4f0 _$E215
// FUNCTION: STBC 0x0042d500 _$E214
// FUNCTION: STBC 0x0042d510 _$E240
// FUNCTION: STBC 0x0042d520 _$E239
// FUNCTION: STBC 0x0042d530 _$E247
// FUNCTION: STBC 0x0042d540 _$E246
// FUNCTION: STBC 0x0042d550 _$E357
// FUNCTION: STBC 0x0042d560 _$E356
// FUNCTION: STBC 0x0042d570 _$E361
// FUNCTION: STBC 0x0042d580 _$E360
// FUNCTION: STBC 0x0042d590 _$E106
// FUNCTION: STBC 0x0042d5a0 _$E105
// FUNCTION: STBC 0x0042d5f0 _$E343
// FUNCTION: STBC 0x0042d600 _$E342
// FUNCTION: STBC 0x0042d610 _$E350
// FUNCTION: STBC 0x0042d620 _$E349
// FUNCTION: STBC 0x0042d630 _$E373
// FUNCTION: STBC 0x0042d640 _$E372
// FUNCTION: STBC 0x0042d650 _$E377
// FUNCTION: STBC 0x0042d660 _$E376
// FUNCTION: STBC 0x0042d670 _$E417
// FUNCTION: STBC 0x0042d680 _$E416
// FUNCTION: STBC 0x0042d690 _$E423
// FUNCTION: STBC 0x0042d6a0 _$E422
// FUNCTION: STBC 0x0042d6b0 _$E365
// FUNCTION: STBC 0x0042d6c0 _$E364
// FUNCTION: STBC 0x0042d6d0 _$E369
// FUNCTION: STBC 0x0042d6e0 _$E368
// FUNCTION: STBC 0x0042d6f0 _$E381
// FUNCTION: STBC 0x0042d700 _$E380
// FUNCTION: STBC 0x0042d710 _$E385
// FUNCTION: STBC 0x0042d720 _$E384
// FUNCTION: STBC 0x0042d730 _$E389
// FUNCTION: STBC 0x0042d740 _$E388
// FUNCTION: STBC 0x0042d750 _$E393
// FUNCTION: STBC 0x0042d760 _$E392
// FUNCTION: STBC 0x0042d770 _$E397
// FUNCTION: STBC 0x0042d780 _$E396
// FUNCTION: STBC 0x0042d790 _$E401
// FUNCTION: STBC 0x0042d7a0 _$E400
// FUNCTION: STBC 0x0042d7b0 _$E405
// FUNCTION: STBC 0x0042d7c0 _$E404
// FUNCTION: STBC 0x0042d7d0 _$E409
// FUNCTION: STBC 0x0042d7e0 _$E408
// FUNCTION: STBC 0x0042d7f0 _$E111
// FUNCTION: STBC 0x0042d800 _$E110
// FUNCTION: STBC 0x0042d810 _$E413
// FUNCTION: STBC 0x0042d820 _$E412
// FUNCTION: STBC 0x0042d830 _$E116
// FUNCTION: STBC 0x0042d840 _$E115
// FUNCTION: STBC 0x0042d850 _$E427
// FUNCTION: STBC 0x0042d860 _$E426
// FUNCTION: STBC 0x0042d870 _$E431
// FUNCTION: STBC 0x0042d880 _$E430
// FUNCTION: STBC 0x0042d890 _$E435
// FUNCTION: STBC 0x0042d8a0 _$E434
// FUNCTION: STBC 0x0042d910 _$E439
// FUNCTION: STBC 0x0042d920 _$E438
// FUNCTION: STBC 0x0042da60 _$E125
// FUNCTION: STBC 0x0042da70 _$E124
// FUNCTION: STBC 0x0042db40 _$E443
// FUNCTION: STBC 0x0042db50 _$E442
// FUNCTION: STBC 0x0042db60 _$E447
// FUNCTION: STBC 0x0042db70 _$E446
// FUNCTION: STBC 0x0042db80 _$E451
// FUNCTION: STBC 0x0042db90 _$E450
// FUNCTION: STBC 0x0042dba0 _$E131
// FUNCTION: STBC 0x0042dbb0 _$E130
// FUNCTION: STBC 0x0042dbc0 _$E136
// FUNCTION: STBC 0x0042dbd0 _$E135
// FUNCTION: STBC 0x0042dbe0 _$E166
// FUNCTION: STBC 0x0042dbf0 _$E165
// FUNCTION: STBC 0x0042dc00 _$E455
// FUNCTION: STBC 0x0042dc10 _$E454
// FUNCTION: STBC 0x0042dc20 _$E171
// FUNCTION: STBC 0x0042dc30 _$E170
// FUNCTION: STBC 0x0042dc80 _$E459
// FUNCTION: STBC 0x0042dc90 _$E458
// FUNCTION: STBC 0x0042dd30 _$E463
// FUNCTION: STBC 0x0042dd40 _$E462
// FUNCTION: STBC 0x0042df30 _$E176
// FUNCTION: STBC 0x0042df40 _$E175
// FUNCTION: STBC 0x0042df60 _$E467
// FUNCTION: STBC 0x0042df70 _$E466
// FUNCTION: STBC 0x0042df80 _$E141
// FUNCTION: STBC 0x0042df90 _$E140
// FUNCTION: STBC 0x0042dfa0 _$E471
// FUNCTION: STBC 0x0042dfb0 _$E470
// FUNCTION: STBC 0x0042dfc0 _$E475
// FUNCTION: STBC 0x0042dfd0 _$E474
// FUNCTION: STBC 0x0042dfe0 _$E181
// FUNCTION: STBC 0x0042dff0 _$E180
// FUNCTION: STBC 0x0042e000 _$E186
// FUNCTION: STBC 0x0042e010 _$E185
// FUNCTION: STBC 0x0042e020 _$E479
// FUNCTION: STBC 0x0042e030 _$E478
// FUNCTION: STBC 0x0042e040 _$E483
// FUNCTION: STBC 0x0042e050 _$E482
// FUNCTION: STBC 0x0042e060 _$E487
// FUNCTION: STBC 0x0042e070 _$E486
// FUNCTION: STBC 0x0042e080 _$E491
// FUNCTION: STBC 0x0042e090 _$E490
// FUNCTION: STBC 0x0042e0a0 _$E495
// FUNCTION: STBC 0x0042e0b0 _$E494
// FUNCTION: STBC 0x0042e0c0 _$E499
// FUNCTION: STBC 0x0042e0d0 _$E498
// FUNCTION: STBC 0x0042e0e0 _$E503
// FUNCTION: STBC 0x0042e0f0 _$E502
// FUNCTION: STBC 0x0042e100 _$E507
// FUNCTION: STBC 0x0042e110 _$E506
// FUNCTION: STBC 0x0042e1a0 _$E191
// FUNCTION: STBC 0x0042e1b0 _$E190
// FUNCTION: STBC 0x0042e1e0 _$E511
// FUNCTION: STBC 0x0042e1f0 _$E510
// FUNCTION: STBC 0x0042e200 _$E196
// FUNCTION: STBC 0x0042e210 _$E195
// FUNCTION: STBC 0x0042e2c0 _$E201
// FUNCTION: STBC 0x0042e2d0 _$E200
// FUNCTION: STBC 0x0042e6e0 _$E515
// FUNCTION: STBC 0x0042e6f0 _$E514
// FUNCTION: STBC 0x0042e700 _$E520
// FUNCTION: STBC 0x0042e710 _$E519
// FUNCTION: STBC 0x0042e720 _$E524
// FUNCTION: STBC 0x0042e730 _$E523

// The dynamic initialisers of the list of cameras and of the handler
// table, and their exit-time destructors.
// FUNCTION: STBC 0x004297a0 _$E530
// FUNCTION: STBC 0x004297b0 _$E526
// FUNCTION: STBC 0x004297e0 _$E529
// FUNCTION: STBC 0x004297f0 _$E527
// FUNCTION: STBC 0x00429860 _$E535
// FUNCTION: STBC 0x00429870 _$E532
// FUNCTION: STBC 0x00429880 _$E534
// FUNCTION: STBC 0x00429890 _$E533
// DATA: STBC 0x0097f038
NiTList<CameraObjectClass *> CameraObjectClass::cameras( 2 );

// DATA: STBC 0x0097f010
TGEventHandlerTable CameraObjectClass::classHandlers;

// The half width of the default frustum at unit distance (invented name).
// DATA: STBC 0x00889478
static const float DEFAULT_HALF_WIDTH = 0.37125f;

/*
================
CameraObjectClass::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x004298b0
void CameraObjectClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, CAMERAOBJECTCLASS_TYPE );
		BaseObjectClass::AddToSet( set );
	}
}

/*
================
CameraObjectClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x004298e0
void CameraObjectClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, CAMERAOBJECTCLASS_TYPE );
		BaseObjectClass::RemoveFromSet( set );
	}
}

/*
================
CameraObjectClass::GetClassObjects

The set's cameras and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x00429910
CameraObjectClass **CameraObjectClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (CameraObjectClass **)set->objectTable.GetObjects( CAMERAOBJECTCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
CameraObjectClass::GetNumClassObjects

The number of the set's cameras.
================
*/
// FUNCTION: STBC 0x00429940
int CameraObjectClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( CAMERAOBJECTCLASS_TYPE );
	}
	return 0;
}

/*
================
CameraObjectClass::GetObjectByID

The camera with the given ID in the set, or among every object when the
set is NULL; NULL when it is none or not a CameraObjectClass.
================
*/
// FUNCTION: STBC 0x00429960
CameraObjectClass *CameraObjectClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( CAMERAOBJECTCLASS_TYPE ) ) {
			return (CameraObjectClass *)object;
		}
		return NULL;
	}
	return (CameraObjectClass *)set->GetClassObject( CAMERAOBJECTCLASS_TYPE, id );
}

/*
================
CameraObjectClass::GetObject

The camera with the name in the set, or in the last set that has one when
the set is NULL; NULL when there is none.
================
*/
// FUNCTION: STBC 0x004299d0
CameraObjectClass *CameraObjectClass::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( CAMERAOBJECTCLASS_TYPE, name_ );
		}
		return (CameraObjectClass *)object;
	}
	return (CameraObjectClass *)set->GetClassObject( CAMERAOBJECTCLASS_TYPE, name_ );
}

/*
================
CameraObjectClass::Cast

The object as a CameraObjectClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00429a30
CameraObjectClass *CameraObjectClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( CAMERAOBJECTCLASS_TYPE ) ) {
		return (CameraObjectClass *)object;
	}
	return NULL;
}

/*
================
CameraObjectClass::CameraObjectClass

A camera without a NetImmerse camera yet. A new camera (no ID: not one
being loaded) starts in the default mode. Every camera joins the list of
cameras.
================
*/
// FUNCTION: STBC 0x00429a60 ??0CameraObjectClass@@QAE@J@Z
CameraObjectClass::CameraObjectClass( long id ) : BaseObjectClass( id ), modeStack( 2 ), namedModes( 37 ), modeHierarchy( 37 ) {
	camera = NULL;
	sweepTime = 0;
	sweepDuration = 0;
	sweepLoops = false;
	sweep = NULL;
	sweepObjectID = 0;
	turnFromForward = NiPoint3::ZERO;
	turnFromUp = NiPoint3::ZERO;
	turnToForward = NiPoint3::ZERO;
	turnToUp = NiPoint3::ZERO;
	turnAxis = NiPoint3::ZERO;
	turnStart = NiMatrix3::IDENTITY;
	turnAngle = 0;
	rollAngle = 0;
	turnTime = 0;
	turnDuration = 0;
	unknownC0 = 0;
	turning = false;
	lookAtPoint = NiPoint3::ZERO;
	lookAtUp = NiPoint3::ZERO;
	lookingAt = false;
	active = false;
	unknownE4 = 5;
	currentMode = NULL;
	lastModeID = 0;
	if ( id == 0 ) {
		PushCameraMode( new CameraMode( this ) );
	}
	cameras.AddTail( this );
}

/*
================
CameraObjectClass::GetObjType
================
*/
// FUNCTION: STBC 0x00429d40
int CameraObjectClass::GetObjType( void ) {
	return CAMERAOBJECTCLASS_TYPE;
}

/*
================
CameraObjectClass::GetClassName
================
*/
// FUNCTION: STBC 0x00429da0
char *CameraObjectClass::GetClassName( void ) {
	return "CameraObjectClass";
}

/*
================
CameraObjectClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00429db0
char *CameraObjectClass::GetSwigTypeName( void ) {
	return "_p_CameraObjectClass";
}

/*
================
CameraObjectClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00429dc0
char *CameraObjectClass::GetPythonClassName( void ) {
	return "CameraObjectClassPtr";
}

/*
================
CameraObjectClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00429dd0
TGEventHandlerTable *CameraObjectClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
CameraObjectClass::~CameraObjectClass

Leaves the list of cameras, deletes the sweep, takes the scene from the
NetImmerse camera and releases the modes the stack and the name map hold.
================
*/
// FUNCTION: STBC 0x00429e30 ??1CameraObjectClass@@UAE@XZ
CameraObjectClass::~CameraObjectClass( void ) {
	cameras.Remove( this );
	if ( sweep != NULL ) {
		delete sweep;
		sweep = NULL;
	}
	if ( camera != NULL ) {
		camera->SetScene( NULL );
	}
	while ( !modeStack.IsEmpty() ) {
		modeStack.RemoveHead()->RemoveReference();
	}

	const char *name_;
	NiTMapIterator item = namedModes.GetFirstPos();
	while ( item != NULL ) {
		CameraMode *mode;
		namedModes.GetNext( item, name_, mode );
		mode->RemoveReference();
	}
	namedModes.RemoveAll();

	item = modeHierarchy.GetFirstPos();
	while ( item != NULL ) {
		char *fallbackName;
		modeHierarchy.GetNext( item, name_, fallbackName );
		delete [] fallbackName;
	}
	modeHierarchy.RemoveAll();
}

/*
================
CameraObjectClass::Create

A new camera at x, y, z with its NetImmerse camera, named.
================
*/
// FUNCTION: STBC 0x0042a0b0
CameraObjectClass *CameraObjectClass::Create( float x, float y, float z, float unknown4, float unknown5, float unknown6, float unknown7, char *name_ ) {
	CameraObjectClass *camera = new CameraObjectClass;
	camera->Unknown42A290( x, y, z, unknown4, unknown5, unknown6, unknown7, name_ );
	return camera;
}

/*
================
CameraObjectClass::CreateFromNiCamera

A camera around an existing NetImmerse camera: the camera gets the name,
a keyframe controller without data, the system renderer and a new
animation node above it, which becomes the object's NetImmerse object.
================
*/
// FUNCTION: STBC 0x0042a120
CameraObjectClass *CameraObjectClass::CreateFromNiCamera( NiCamera *niCamera, char *name_ ) {
	CameraObjectClass *camera = new CameraObjectClass;
	camera->camera = niCamera;
	niCamera->SetName( name_ );
	NiKeyframeController *controller = new NiKeyframeController;
	controller->SetData( NULL );
	controller->SetTarget( niCamera );
	niCamera->SetRenderer( g_kSystemWrapper.GetRenderer() );
	TGAnimNode *node = new TGAnimNode;
	node->AttachChild( niCamera, false );
	node->SetName( name_ );
	camera->SetName( name_ );
	camera->SetNiObject( node );
	return camera;
}

/*
================
CameraObjectClass::Unknown42A290

Gives the camera a new NetImmerse camera with the system renderer and the
name, at x, y, z, its up vector turned onto the direction and then rolled
by the angle about it; a keyframe controller without data, and a new
animation node above it, which becomes the object's NetImmerse object;
then the default frustum.
================
*/
// FUNCTION: STBC 0x0042a290
void CameraObjectClass::Unknown42A290( float x, float y, float z, float angle, float dx, float dy, float dz, char *name_ ) {
	NiCamera *niCamera = new NiCamera;
	camera = niCamera;
	NiRender *renderer = g_kSystemWrapper.GetRenderer();
	niCamera->SetViewPort( NiRect<float>( 0.0f, 1.0f, 1.0f, 0.0f ) );
	niCamera->SetRenderer( renderer );
	niCamera->SetName( name_ );
	niCamera->SetTranslate( x, y, z );

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
	SetName( name_ );
	SetNiObject( node );
	SetFrustumValues( DEFAULT_HALF_WIDTH, 1.0f, 2000.0f );
}

/*
================
CameraObjectClass::SetFrustumValues

Gives the NetImmerse camera a frustum halfWidth to each side at unit
distance, as high as the renderer's screen proportions make it.
================
*/
// FUNCTION: STBC 0x0042a690
void CameraObjectClass::SetFrustumValues( float halfWidth, float nearDistance, float farDistance ) {
	NiPointer<NiRender> renderer = g_kSystemWrapper.GetRenderer();
	float aspect = (float)renderer->VirtualSlot20() / (float)renderer->VirtualSlot19();
	camera->SetViewFrustum( NiFrustum( -halfWidth, halfWidth, aspect * halfWidth, -( aspect * halfWidth ), nearDistance, farDistance ) );
}

/*
================
CameraObjectClass::Update

Moves the camera along its sweep, follows the current mode's direction,
or looks at its point, then lets the current mode update.
================
*/
// FUNCTION: STBC 0x0042a7c0
void CameraObjectClass::Update( float time ) {
	float elapsed;
	if ( lastUpdateTime < 0.0f ) {
		elapsed = 0.001f;
	} else {
		elapsed = time - lastUpdateTime;
	}
	if ( sweep != NULL ) {
		VirtualSlot52( sweep );
		UpdateSweep( elapsed );
	}
	if ( turning ) {
		NiPoint3 forward = turnToForward;
		NiPoint3 up = turnToUp;
		VirtualSlot53( &forward, &up );
		if ( forward != turnToForward || up != turnToUp ) {
			NiPoint3 start = turnFromForward;
			NiPoint3 end = turnFromUp;
			StartTurn( &start, &end, &forward, &up, &turnStart, turnTime, turnDuration );
		}
		UpdateTurn( elapsed );
	} else if ( lookingAt ) {
		NiCamera *niCamera = camera;
		niCamera->Update( 0.0f );
		niCamera->LookAtWorldPoint( lookAtPoint, lookAtUp );
	}

	bool hadSweep = sweep != NULL;
	CameraMode *mode = GetCurrentCameraMode( true );
	if ( mode != NULL && mode->IsValid() ) {
		mode->UpdateLagTrackers();
		mode->Update();
		if ( !hadSweep && sweep != NULL ) {
			UpdateSweep( 0.0f );
		}
	}
	BaseObjectClass::Update( time );
}

/*
================
CameraObjectClass::UpdateNodeOnly
================
*/
// FUNCTION: STBC 0x0042a9e0
void CameraObjectClass::UpdateNodeOnly( void ) {
	if ( camera != NULL ) {
		camera->Update( 0.0f );
	}
}

/*
================
CameraObjectClass::IsAnimating

True while an animation blender among the controllers of the camera's
animation node is playing.
================
*/
// FUNCTION: STBC 0x0042aa00
bool CameraObjectClass::IsAnimating( void ) {
	TGAnimNode *node = GetAnimNode();
	if ( node != NULL ) {
		for ( NiTimeController *controller = node->GetControllers(); controller != NULL; controller = controller->GetNext() ) {
			if ( controller->IsKindOf( &TGAnimBlender::rtti ) && ( (TGAnimBlender *)controller )->IsAnimating() ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
CameraObjectClass::GetWorldForward

The NetImmerse camera's view direction.
================
*/
// FUNCTION: STBC 0x0042aa50
NiPoint3 CameraObjectClass::GetWorldForward( void ) {
	return camera->GetWorldDirection();
}

/*
================
CameraObjectClass::GetWorldUp
================
*/
// FUNCTION: STBC 0x0042aa80
NiPoint3 CameraObjectClass::GetWorldUp( void ) {
	return camera->GetWorldUpVector();
}

/*
================
CameraObjectClass::GetWorldRight
================
*/
// FUNCTION: STBC 0x0042aab0
NiPoint3 CameraObjectClass::GetWorldRight( void ) {
	return camera->GetWorldRightVector();
}

/*
================
CameraObjectClass::SetFrustum

Gives the NetImmerse camera the view frustum, its near distance kept in
the camera's limits.
================
*/
// FUNCTION: STBC 0x0042aae0
void CameraObjectClass::SetFrustum( float left, float right, float top, float bottom, float nearDistance, float farDistance ) {
	if ( camera != NULL ) {
		camera->SetViewFrustum( NiFrustum( left, right, top, bottom, nearDistance, farDistance ) );
	}
}

// What GetNiFrustum returns for a camera without a NetImmerse camera:
// nothing sets it (invented name). TODO structure: NiFrustum's empty
// default constructor gives it a dynamic initialiser (_$E531/_$E532)
// that retail does not have.
// DATA: STBC 0x0097efe8
static NiFrustum noFrustum;

/*
================
CameraObjectClass::GetNiFrustum

A copy of the NetImmerse camera's view frustum.
================
*/
// FUNCTION: STBC 0x0042ab70
NiFrustum CameraObjectClass::GetNiFrustum( void ) {
	if ( camera == NULL ) {
		return noFrustum;
	}
	return camera->GetViewFrustum();
}

/*
================
CameraObjectClass::SetNiFrustum

Gives the NetImmerse camera a copy of the frustum, its near distance kept
in the camera's limits.
================
*/
// FUNCTION: STBC 0x0042abb0
void CameraObjectClass::SetNiFrustum( NiFrustum *frustum ) {
	if ( camera != NULL ) {
		camera->SetViewFrustum( *frustum );
	}
}

/*
================
CameraObjectClass::SetNearAndFarDistance

Sets the distances of the near and far planes of the camera's view
frustum.
================
*/
// FUNCTION: STBC 0x0042ac30
void CameraObjectClass::SetNearAndFarDistance( float nearDistance, float farDistance ) {
	if ( camera != NULL ) {
		NiFrustum frustum = camera->GetViewFrustum();
		frustum.nearDistance = nearDistance;
		frustum.farDistance = farDistance;
		camera->SetViewFrustum( frustum );
	}
}

/*
================
CameraObjectClass::PushCameraMode

Leaves the current mode and makes the mode current, on top of the stack;
the stack holds a reference to it.
================
*/
// FUNCTION: STBC 0x0042acd0
void CameraObjectClass::PushCameraMode( CameraMode *mode ) {
	if ( mode != NULL ) {
		if ( currentMode != NULL ) {
			currentMode->VirtualSlot29();
			lastModeID = currentMode->GetObjID();
		}
		mode->AddReference();
		modeStack.AddHead( mode );
		mode->VirtualSlot28();
		currentMode = mode;
	}
}

/*
================
CameraObjectClass::GetCurrentCameraMode

The mode on top of the stack; NULL when the stack is empty. With validate,
a mode that is not valid gives way to its fallback in the hierarchy, as
far as the fallbacks go, and the mode found becomes the current one.
================
*/
// FUNCTION: STBC 0x0042ad70
CameraMode *CameraObjectClass::GetCurrentCameraMode( bool validate ) {
	if ( !modeStack.IsEmpty() ) {
		CameraMode *mode = modeStack.GetHead();
		if ( validate ) {
			while ( !mode->IsValid() ) {
				char *name_ = mode->GetName();
				if ( name_ == NULL ) {
					break;
				}
				char *fallbackName;
				if ( !modeHierarchy.GetAt( name_, fallbackName ) ) {
					break;
				}
				CameraMode *fallback = GetNamedCameraMode( fallbackName );
				if ( fallback == NULL ) {
					break;
				}
				mode = fallback;
			}
			if ( mode != currentMode ) {
				if ( currentMode != NULL ) {
					currentMode->VirtualSlot29();
					lastModeID = currentMode->GetObjID();
				}
				currentMode = mode;
				if ( mode != NULL ) {
					mode->VirtualSlot28();
				}
			}
		}
		return mode;
	}
	return NULL;
}

/*
================
CameraObjectClass::PopCameraMode

Takes the named mode, or the top one when the name is NULL, off the stack
and releases the stack's reference; when it was the current mode, the new
top becomes current.
================
*/
// FUNCTION: STBC 0x0042ae50
void CameraObjectClass::PopCameraMode( char *name_ ) {
	CameraMode *mode = NULL;
	if ( name_ != NULL ) {
		NiTListIterator pos = modeStack.GetHeadPos();
		while ( pos != NULL ) {
			CameraMode *stacked = modeStack.GetNext( pos );
			if ( strcmp( stacked->GetName(), name_ ) == 0 ) {
				mode = stacked;
				break;
			}
		}
	} else if ( !modeStack.IsEmpty() ) {
		mode = modeStack.GetHead();
	}
	if ( mode == NULL ) {
		return;
	}
	if ( mode == currentMode ) {
		currentMode->VirtualSlot29();
		lastModeID = currentMode->GetObjID();
		currentMode = NULL;
	}
	modeStack.Remove( mode );
	mode->RemoveReference();
	if ( currentMode == NULL && !modeStack.IsEmpty() ) {
		currentMode = modeStack.GetHead();
		currentMode->VirtualSlot28();
	}
}

/*
================
CameraObjectClass::AddModeHierarchy

Files a copy of the fallback's name under the mode's name.
================
*/
// FUNCTION: STBC 0x0042af90
void CameraObjectClass::AddModeHierarchy( char *modeName, char *fallbackName ) {
	if ( modeName != NULL && fallbackName != NULL ) {
		char *copy = new char[strlen( fallbackName ) + 1];
		strcpy( copy, fallbackName );
		modeHierarchy.SetAt( modeName, copy );
	}
}

/*
================
CameraObjectClass::GetNamedCameraMode

The mode filed under the name; when there is none, the function of that
name in the CameraModes script makes it from the camera, and it is filed.
NULL when the script makes none.
================
*/
// FUNCTION: STBC 0x0042b070
CameraMode *CameraObjectClass::GetNamedCameraMode( char *name_ ) {
	CameraMode *mode = NULL;
	if ( namedModes.GetAt( name_, mode ) ) {
		return mode;
	}
	PyObject *result = NULL;
	PyObject *self = VirtualSlot08();
	int status = PyEmbed::CallFunction( "CameraModes", name_, "O", &result, "(O)", self );
	Py_DECREF( self );
	if ( status == -1 ) {
		return mode;
	}
	PyEmbed::Unknown6F98E0( result, (void **)&mode, "CameraMode" );
	Py_DECREF( result );
	if ( mode != NULL ) {
		namedModes.SetAt( name_, mode );
		mode->AddReference();
		mode->SetName( name_ );
	}
	return mode;
}

/*
================
CameraObjectClass::AddNamedCameraMode

Files the mode under the name, releasing the mode filed there before; the
map holds a reference, and the mode takes the name.
================
*/
// FUNCTION: STBC 0x0042b1f0
void CameraObjectClass::AddNamedCameraMode( char *name_, CameraMode *mode ) {
	CameraMode *old;
	if ( namedModes.GetAt( name_, old ) ) {
		old->RemoveReference();
	}
	namedModes.SetAt( name_, mode );
	mode->AddReference();
	mode->SetName( name_ );
}

/*
================
CameraObjectClass::StartSweep

Sends the camera along a new sweep through the points, over the time;
looping, it starts over at the end. The ID names the object the sweep
starts from.
================
*/
// FUNCTION: STBC 0x0042b2e0
void CameraObjectClass::StartSweep( int count, NiPoint3 *points, float time, bool loop, long objectID ) {
	StopSweep();
	sweep = new BezierCurve( count, points );
	sweepTime = 0.0f;
	sweepDuration = time;
	sweepLoops = loop;
	sweepObjectID = objectID;
}

/*
================
CameraObjectClass::SweepTo

Sweeps the camera from where its NetImmerse camera is to the position,
over the time.
================
*/
// FUNCTION: STBC 0x0042b360
void CameraObjectClass::SweepTo( NiPoint3 *position, float time ) {
	NiPoint3 points[4];
	points[0] = points[1] = camera->GetTranslate();
	points[2] = points[3] = *position;
	StartSweep( 4, points, time, false, 0 );
}

/*
================
CameraObjectClass::LookAt

Has the camera look at the point, with the up vector, while it updates.
================
*/
// FUNCTION: STBC 0x0042b3d0
void CameraObjectClass::LookAt( NiPoint3 *point, NiPoint3 *up ) {
	lookAtPoint = *point;
	lookAtUp = *up;
	lookingAt = true;
}

/*
================
CameraObjectClass::VirtualSlot52
================
*/
// FUNCTION: STBC 0x0042b420
void CameraObjectClass::VirtualSlot52( BezierCurve *sweep ) {
	CameraMode *mode = GetCurrentCameraMode( true );
	if ( mode != NULL ) {
		mode->VirtualSlot30( sweep );
	}
}

/*
================
CameraObjectClass::UpdateSweep

Moves the camera along its sweep by the elapsed time; at the end a looping
sweep starts over, and another one is deleted.
================
*/
// FUNCTION: STBC 0x0042b440
void CameraObjectClass::UpdateSweep( float elapsed ) {
	BezierCurve *curve = sweep;
	if ( curve != NULL ) {
		NiCamera *niCamera = camera;
		elapsed += sweepTime;
		sweepTime = elapsed;
		if ( elapsed >= sweepDuration ) {
			if ( sweepLoops ) {
				sweepTime = elapsed - (int)( elapsed / sweepDuration ) * sweepDuration;
			} else {
				sweepTime = sweepDuration;
			}
		}
		niCamera->SetTranslate( curve->Evaluate( sweepTime / sweepDuration ) );
		if ( sweepTime == sweepDuration ) {
			delete sweep;
			sweep = NULL;
		}
	}
}

/*
================
CameraObjectClass::StopSweep
================
*/
// FUNCTION: STBC 0x0042b500
void CameraObjectClass::StopSweep( void ) {
	if ( sweep != NULL ) {
		delete sweep;
		sweep = NULL;
	}
}

/*
================
CameraObjectClass::VirtualSlot53
================
*/
// FUNCTION: STBC 0x0042b520
void CameraObjectClass::VirtualSlot53( NiPoint3 *forward, NiPoint3 *up ) {
	CameraMode *mode = GetCurrentCameraMode( true );
	if ( mode != NULL ) {
		mode->VirtualSlot31( forward, up );
	}
}

// the slack a turn allows in the length of its unit vectors
#define TURN_MIN_UNIT_LENGTH	0.99f
#define TURN_MAX_UNIT_LENGTH	1.01f

/*
================
CameraObjectClass::StartTurn

Starts turning the camera from the direction and up vector to the others,
from the rotation, the elapsed time into the duration. The four vectors
must be of unit length and each up vector at right angles to its direction;
otherwise nothing happens. The turn is kept in the rotation's frame: an
angle about an axis that takes one direction to the other, then a roll that
takes the turned up vector to the new one.
================
*/
// FUNCTION: STBC 0x0042b540
// DRAFT: inline budget: retail copies the turned up vector inline (ours calls NiPoint3::operator=), and sums the last cross length y, x, z; the turned direction (computed, never used) shares its slot with the cross temporary
void CameraObjectClass::StartTurn( const NiPoint3 *fromForward, const NiPoint3 *fromUp, const NiPoint3 *forward, const NiPoint3 *up, const NiMatrix3 *rotation, float elapsed, float duration ) {
	NiCamera *niCamera = camera;
	if ( !turning ) {
		unknownC0++;
	}
	if ( fromForward->SqrLength() >= TURN_MIN_UNIT_LENGTH && fromForward->SqrLength() <= TURN_MAX_UNIT_LENGTH
		&& fromUp->SqrLength() >= TURN_MIN_UNIT_LENGTH && fromUp->SqrLength() <= TURN_MAX_UNIT_LENGTH
		&& forward->SqrLength() >= TURN_MIN_UNIT_LENGTH && forward->SqrLength() <= TURN_MAX_UNIT_LENGTH
		&& up->SqrLength() >= TURN_MIN_UNIT_LENGTH && up->SqrLength() <= TURN_MAX_UNIT_LENGTH
		&& fromForward->Cross( *fromUp ).Length() >= TURN_MIN_UNIT_LENGTH && fromForward->Cross( *fromUp ).Length() <= TURN_MAX_UNIT_LENGTH
		&& forward->Cross( *up ).Length() >= TURN_MIN_UNIT_LENGTH && forward->Cross( *up ).Length() <= TURN_MAX_UNIT_LENGTH ) {
		turning = true;
		turnStart = *rotation;

		// the vectors in the frame of the rotation the turn starts from
		turnFromForward = *fromForward * niCamera->GetWorldRotate().Inverse() * niCamera->GetRotate() * turnStart.Inverse();
		turnFromUp = *fromUp * niCamera->GetWorldRotate().Inverse() * niCamera->GetRotate() * turnStart.Inverse();
		turnToForward = *forward * niCamera->GetWorldRotate().Inverse() * niCamera->GetRotate() * turnStart.Inverse();
		turnToUp = *up * niCamera->GetWorldRotate().Inverse() * niCamera->GetRotate() * turnStart.Inverse();

		// the turn that takes one direction to the other
		turnAxis = turnToForward.Cross( turnFromForward );
		turnAxis.Unitize();
		if ( turnAxis == NiPoint3::ZERO ) {
			turnAxis = turnFromUp;
		}
		turnAngle = NiACos( turnToForward.Dot( turnFromForward ) );

		// the roll that then takes the turned up vector to the new one
		NiMatrix3 turn;
		NiPoint3 turnedForward;
		NiPoint3 turnedUp;
		turn.MakeRotation( -turnAngle, turnAxis );
		turnedForward = turnFromForward * turn;	// BUG: redundant: never used
		turnedUp = turnFromUp * turn;
		rollAngle = NiACos( turnedUp.Dot( turnToUp ) );
		NiPoint3 right;
		right = turnToForward.Cross( turnToUp );
		if ( right.Dot( turnedUp ) < 0.0f ) {
			rollAngle = -rollAngle;
		}

		turnAxis = turnAxis * turnStart;
		turnDuration = duration;
		turnTime = elapsed;
		turnFromForward = *fromForward;
		turnFromUp = *fromUp;
		turnToForward = *forward;
		turnToUp = *up;
	}
}

/*
================
CameraObjectClass::LookToward

Turns the camera from its NetImmerse camera's direction and up vector to
the given ones, over the time; not while an animation moves it.
================
*/
// FUNCTION: STBC 0x0042bcb0
void CameraObjectClass::LookToward( NiPoint3 *forward, NiPoint3 *up, float time ) {
	if ( !IsAnimating() ) {
		NiCamera *niCamera = camera;
		StartTurn( &niCamera->GetWorldDirection(), &niCamera->GetWorldUpVector(), forward, up, &niCamera->GetRotate(), 0.0f, time );
	}
}

/*
================
CameraObjectClass::UpdateTurn

Turns the camera by the elapsed time, eased in and out: about the turn's
axis from the rotation it started from, then about the X axis.
================
*/
// FUNCTION: STBC 0x0042bcf0
void CameraObjectClass::UpdateTurn( float elapsed ) {
	NiCamera *niCamera = camera;
	turnTime += elapsed;
	if ( turnTime > turnDuration ) {
		turning = false;
		turnTime = turnDuration;
	}
	float fraction = 1.0f - ( (float)cos( turnTime / turnDuration * NI_PI ) + 1.0f ) * 0.5f;
	NiMatrix3 rotation;
	rotation.MakeRotation( fraction * turnAngle, turnAxis );
	niCamera->SetRotate( rotation * turnStart );
	rotation.MakeXRotation( fraction * rollAngle );
	niCamera->SetRotate( niCamera->GetRotate() * rotation );
	if ( !turning ) {
		Reorthogonalize();
	}
}

/*
================
CameraObjectClass::StopTurn
================
*/
// FUNCTION: STBC 0x0042bde0
void CameraObjectClass::StopTurn( void ) {
	turning = false;
	Reorthogonalize();
}

/*
================
CameraObjectClass::Unknown42BDF0

The object of the ID at 0x4C while 0x48 is set; NULL otherwise.
================
*/
// FUNCTION: STBC 0x0042bdf0
TGObject *CameraObjectClass::Unknown42BDF0( void ) {
	if ( sweep == NULL ) {
		return NULL;
	}
	return TGObject::GetSavedObject( sweepObjectID );
}

/*
================
CameraObjectClass::Reorthogonalize

Makes the NetImmerse camera's rotation orthogonal again, once in eight.
================
*/
// FUNCTION: STBC 0x0042be10
void CameraObjectClass::Reorthogonalize( void ) {
	NiCamera *niCamera = camera;
	if ( unknownC0 % 8 == 0 ) {
		NiMatrix3 rotation( niCamera->GetRotate() );
		rotation.Reorthogonalize();
		niCamera->SetRotate( rotation );
	}
}

/*
================
CameraObjectClass::Rotate

Turns the camera by the world-space rotation through the current mode;
not while an animation moves it.
================
*/
// FUNCTION: STBC 0x0042be60 ?Rotate@CameraObjectClass@@UAEXABVNiMatrix3@@@Z
void CameraObjectClass::Rotate( const NiMatrix3 &matrix ) {
	if ( !IsAnimating() ) {
		CameraMode *mode = GetCurrentCameraMode( true );
		if ( mode != NULL ) {
			mode->VirtualSlot32( matrix );
		}
	}
}

/*
================
CameraObjectClass::Unknown42BE90

Turns the camera about the axis that mixes its up axis by the first angle
and its right axis against the second, by the length of that axis.
================
*/
// FUNCTION: STBC 0x0042be90
// DRAFT: x87: retail also copies the scaled up vector into the axis slots before the right term (a dead store ours lacks) and keeps one more value on the FPU through Length
void CameraObjectClass::Unknown42BE90( float upAngle, float rightAngle ) {
	NiPoint3 up = camera->GetWorldUpVector() * upAngle;
	NiPoint3 right = camera->GetWorldRightVector() * -rightAngle;
	NiPoint3 axis = up + right;
	float angle = axis.Length();
	if ( angle != 0.0f ) {
		axis /= angle;
		NiMatrix3 rotation;
		rotation.MakeRotation( angle, axis );
		Rotate( rotation );
	}
}

/*
================
CameraObjectClass::SetTranslateXYZ
================
*/
// FUNCTION: STBC 0x0042bfa0
void CameraObjectClass::SetTranslateXYZ( float x, float y, float z ) {
	if ( camera != NULL ) {
		camera->SetTranslate( NiPoint3( x, y, z ) );
	}
}

/*
================
CameraObjectClass::GetTranslate

The NetImmerse camera's translation; the origin when there is none.
================
*/
// FUNCTION: STBC 0x0042bfd0
const TGPoint3 &CameraObjectClass::GetTranslate( void ) {
	if ( camera != NULL ) {
		return static_cast<const TGPoint3 &>( camera->GetTranslate() );
	}
	return TGPoint3::ZERO;
}

/*
================
CameraObjectClass::GetWorldLocation

The NetImmerse camera's world translation; the origin when there is none.
================
*/
// FUNCTION: STBC 0x0042bff0
const TGPoint3 &CameraObjectClass::GetWorldLocation( void ) {
	if ( camera != NULL ) {
		return static_cast<const TGPoint3 &>( camera->GetWorldLocation() );
	}
	return TGPoint3::ZERO;
}

/*
================
CameraObjectClass::Zoom

Passes the zoom to the current mode.
================
*/
// FUNCTION: STBC 0x0042c010
void CameraObjectClass::Zoom( float zoom ) {
	CameraMode *mode = GetCurrentCameraMode( true );
	if ( mode != NULL ) {
		mode->Zoom( zoom );
	}
}

/*
================
CameraObjectClass::Unknown42C030
================
*/
// FUNCTION: STBC 0x0042c030
NiMatrix3 CameraObjectClass::Unknown42C030( const NiMatrix3 &rotation ) {
	NiPoint3 col[3];
	rotation.GetRow( 2, col[0] );
	rotation.GetRow( 0, col[1] );
	rotation.GetRow( 1, col[2] );
	NiMatrix3 result( col[0], col[1], col[2] );
	return result;
}

/*
================
CameraObjectClass::Unknown42C0B0
================
*/
// FUNCTION: STBC 0x0042c0b0
NiMatrix3 CameraObjectClass::Unknown42C0B0( const NiMatrix3 &rotation ) {
	NiPoint3 col[3];
	rotation.GetRow( 1, col[0] );
	rotation.GetRow( 2, col[1] );
	rotation.GetRow( 0, col[2] );
	NiMatrix3 result( col[0], col[1], col[2] );
	return result;
}

/*
================
CameraObjectClass::WorldPtToScreenPt
================
*/
// FUNCTION: STBC 0x0042c130
bool CameraObjectClass::WorldPtToScreenPt( const NiPoint3 &point, float &x, float &y ) {
	if ( camera != NULL ) {
		return camera->WorldPtToScreenPt( point, x, y );
	}
	return false;
}

/*
================
CameraObjectClass::SetAngleAxisRotation
================
*/
// FUNCTION: STBC 0x0042c160
void CameraObjectClass::SetAngleAxisRotation( float angle, float x, float y, float z ) {
	NiCamera *niCamera = camera;
	if ( niCamera != NULL ) {
		NiMatrix3 rotation;
		rotation.MakeRotation( angle, NiPoint3( x, y, z ) );
		niCamera->SetRotate( rotation );
	}
}

/*
================
CameraObjectClass::SetMatrixRotation
================
*/
// FUNCTION: STBC 0x0042c1d0 ?SetMatrixRotation@CameraObjectClass@@UAEXABVNiMatrix3@@@Z
void CameraObjectClass::SetMatrixRotation( const NiMatrix3 &matrix ) {
	if ( camera != NULL ) {
		camera->SetRotate( matrix );
	}
}

/*
================
CameraObjectClass::GetRotation

The NetImmerse camera's rotation; the identity when there is none.
================
*/
// FUNCTION: STBC 0x0042c1f0
const TGMatrix3 &CameraObjectClass::GetRotation( void ) {
	if ( camera != NULL ) {
		return static_cast<const TGMatrix3 &>( camera->GetRotate() );
	}
	return static_cast<const TGMatrix3 &>( NiMatrix3::IDENTITY );
}

/*
================
CameraObjectClass::SetAllRenderers

Gives every camera there is the renderer to draw with: each CameraObjectClass
and the NetImmerse cameras of the module, the application and the icon manager,
then resets the glow for the renderer.
================
*/
// FUNCTION: STBC 0x0042c210
void CameraObjectClass::SetAllRenderers( NiRender *renderer ) {
	NiTListIterator pos = cameras.GetHeadPos();
	while ( pos != NULL ) {
		( cameras.GetNext( pos ) )->SetRenderer( renderer );
	}

	g_kUtopiaModule.GetCamera()->SetRenderer( renderer );

	NiCamera *applicationCamera = NiApplication::application->GetCamera();
	applicationCamera->SetRenderer( renderer );

	NiCamera *iconCamera = g_kIconManager.GetCamera();
	iconCamera->SetRenderer( renderer );

	SetClass::ResetGlow( renderer );
}

/*
================
CameraObjectClass::SetRenderer

Gives the NetImmerse camera the renderer to draw with.
================
*/
// FUNCTION: STBC 0x0042c330
void CameraObjectClass::SetRenderer( NiRender *renderer ) {
	if ( camera != NULL ) {
		camera->SetRenderer( renderer );
	}
}

/*
================
CameraObjectClass::RegisterHandlerNames

The camera's handler methods by their class-qualified names, then the camera
modes' names.
================
*/
// FUNCTION: STBC 0x0042c380
void CameraObjectClass::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CameraObjectClass::AnimationDoneHandler, "CameraObjectClass::AnimationDoneHandler" );

	CameraMode::RegisterHandlerNames();
}

/*
================
CameraObjectClass::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0042c3a0
void CameraObjectClass::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_CAMERA_ANIMATION_DONE, "CameraObjectClass::AnimationDoneHandler" );

	CameraMode::RegisterHandlers();
}

/*
================
CameraObjectClass::AnimationDoneHandler

The camera's animation has played out: every controller of the object the
camera shows, and of the objects under it, is stopped.
================
*/
// FUNCTION: STBC 0x0042c3c0
void CameraObjectClass::AnimationDoneHandler( TGEvent *event ) {
	NiTimeController::StopAnimations( niObject );
}

/*
================
CameraObjectClass::WriteToStream

The camera's state, its NetImmerse camera's transform, the IDs of the
stacked modes, and the names of the named modes and of the fallbacks.
================
*/
// FUNCTION: STBC 0x0042c3d0
bool CameraObjectClass::WriteToStream( TGStream *stream ) {
	if ( !BaseObjectClass::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteFloat( sweepTime );
	stream->WriteFloat( sweepDuration );
	stream->WriteBool( sweepLoops );
	stream->WriteBool( sweep != NULL );
	if ( sweep != NULL ) {
		sweep->WriteToStream( stream );
	}
	stream->WriteFloat( turnFromForward.x );
	stream->WriteFloat( turnFromForward.y );
	stream->WriteFloat( turnFromForward.z );
	stream->WriteFloat( turnFromUp.x );
	stream->WriteFloat( turnFromUp.y );
	stream->WriteFloat( turnFromUp.z );
	stream->WriteFloat( turnToForward.x );
	stream->WriteFloat( turnToForward.y );
	stream->WriteFloat( turnToForward.z );
	stream->WriteFloat( turnToUp.x );
	stream->WriteFloat( turnToUp.y );
	stream->WriteFloat( turnToUp.z );
	stream->WriteFloat( turnAxis.x );
	stream->WriteFloat( turnAxis.y );
	stream->WriteFloat( turnAxis.z );
	stream->WriteFloat( turnStart.GetEntry( 0, 0 ) );
	stream->WriteFloat( turnStart.GetEntry( 0, 1 ) );
	stream->WriteFloat( turnStart.GetEntry( 0, 2 ) );
	stream->WriteFloat( turnStart.GetEntry( 1, 0 ) );
	stream->WriteFloat( turnStart.GetEntry( 1, 1 ) );
	stream->WriteFloat( turnStart.GetEntry( 1, 2 ) );
	stream->WriteFloat( turnStart.GetEntry( 2, 0 ) );
	stream->WriteFloat( turnStart.GetEntry( 2, 1 ) );
	stream->WriteFloat( turnStart.GetEntry( 2, 2 ) );
	stream->WriteID( sweepObjectID );
	stream->WriteFloat( turnAngle );
	stream->WriteFloat( rollAngle );
	stream->WriteFloat( turnTime );
	stream->WriteFloat( turnDuration );
	stream->WriteLong( unknownC0 );
	stream->WriteBool( turning );
	stream->WriteFloat( lookAtPoint.x );
	stream->WriteFloat( lookAtPoint.y );
	stream->WriteFloat( lookAtPoint.z );
	stream->WriteFloat( lookAtUp.x );
	stream->WriteFloat( lookAtUp.y );
	stream->WriteFloat( lookAtUp.z );
	stream->WriteBool( lookingAt );
	stream->WriteBool( active );
	stream->WriteLong( unknownE4 );

	NiCamera *niCamera = camera;
	stream->VirtualSlot17( (long)niCamera );
	if ( niCamera != NULL ) {
		const NiPoint3 &translate = niCamera->GetTranslate();
		stream->WriteFloat( translate.x );
		stream->WriteFloat( translate.y );
		stream->WriteFloat( translate.z );
		const NiMatrix3 &rotate = niCamera->GetRotate();
		stream->WriteFloat( rotate.GetEntry( 0, 0 ) );
		stream->WriteFloat( rotate.GetEntry( 0, 1 ) );
		stream->WriteFloat( rotate.GetEntry( 0, 2 ) );
		stream->WriteFloat( rotate.GetEntry( 1, 0 ) );
		stream->WriteFloat( rotate.GetEntry( 1, 1 ) );
		stream->WriteFloat( rotate.GetEntry( 1, 2 ) );
		stream->WriteFloat( rotate.GetEntry( 2, 0 ) );
		stream->WriteFloat( rotate.GetEntry( 2, 1 ) );
		stream->WriteFloat( rotate.GetEntry( 2, 2 ) );
		stream->VirtualSlot17( (long)GetAnimNode() );
	}

	stream->WriteInt( modeStack.GetSize() );
	NiTListIterator pos = modeStack.GetHeadPos();
	while ( pos != NULL ) {
		stream->WriteID( modeStack.GetNext( pos )->GetObjID() );
	}
	stream->WriteID( lastModeID );

	const char *name_;
	NiTMapIterator item = namedModes.GetFirstPos();
	while ( item != NULL ) {
		CameraMode *mode;
		namedModes.GetNext( item, name_, mode );
		if ( name_ != NULL && mode != NULL ) {
			stream->WriteBool( true );
			stream->VirtualSlot11( (char *)name_ );
			stream->WriteID( mode->GetObjID() );
		}
	}
	stream->WriteBool( false );

	item = modeHierarchy.GetFirstPos();
	while ( item != NULL ) {
		char *fallbackName;
		modeHierarchy.GetNext( item, name_, fallbackName );
		if ( name_ != NULL && fallbackName != NULL ) {
			stream->WriteBool( true );
			stream->VirtualSlot11( (char *)name_ );
			stream->VirtualSlot11( fallbackName );
		}
	}
	stream->WriteBool( false );
	return true;
}

/*
================
CameraObjectClass::ReadFromStream

Reads what WriteToStream wrote. A saved NetImmerse camera is made again
at its saved place and filed under its saved address with the animation
node; the modes stay IDs until PostFixUp.
================
*/
// FUNCTION: STBC 0x0042c900
bool CameraObjectClass::ReadFromStream( TGStream *stream ) {
	if ( !BaseObjectClass::ReadFromStream( stream ) ) {
		return false;
	}

	sweepTime = stream->ReadFloat();
	sweepDuration = stream->ReadFloat();
	sweepLoops = stream->ReadBool();
	if ( stream->ReadBool() ) {
		sweep = new BezierCurve;
		sweep->ReadFromStream( stream );
	}
	turnFromForward.x = stream->ReadFloat();
	turnFromForward.y = stream->ReadFloat();
	turnFromForward.z = stream->ReadFloat();
	turnFromUp.x = stream->ReadFloat();
	turnFromUp.y = stream->ReadFloat();
	turnFromUp.z = stream->ReadFloat();
	turnToForward.x = stream->ReadFloat();
	turnToForward.y = stream->ReadFloat();
	turnToForward.z = stream->ReadFloat();
	turnToUp.x = stream->ReadFloat();
	turnToUp.y = stream->ReadFloat();
	turnToUp.z = stream->ReadFloat();
	turnAxis.x = stream->ReadFloat();
	turnAxis.y = stream->ReadFloat();
	turnAxis.z = stream->ReadFloat();
	turnStart.SetEntry( 0, 0, stream->ReadFloat() );
	turnStart.SetEntry( 0, 1, stream->ReadFloat() );
	turnStart.SetEntry( 0, 2, stream->ReadFloat() );
	turnStart.SetEntry( 1, 0, stream->ReadFloat() );
	turnStart.SetEntry( 1, 1, stream->ReadFloat() );
	turnStart.SetEntry( 1, 2, stream->ReadFloat() );
	turnStart.SetEntry( 2, 0, stream->ReadFloat() );
	turnStart.SetEntry( 2, 1, stream->ReadFloat() );
	turnStart.SetEntry( 2, 2, stream->ReadFloat() );
	sweepObjectID = stream->ReadID();
	turnAngle = stream->ReadFloat();
	rollAngle = stream->ReadFloat();
	turnTime = stream->ReadFloat();
	turnDuration = stream->ReadFloat();
	unknownC0 = stream->ReadLong();
	turning = stream->ReadBool();
	lookAtPoint.x = stream->ReadFloat();
	lookAtPoint.y = stream->ReadFloat();
	lookAtPoint.z = stream->ReadFloat();
	lookAtUp.x = stream->ReadFloat();
	lookAtUp.y = stream->ReadFloat();
	lookAtUp.z = stream->ReadFloat();
	lookingAt = stream->ReadBool();
	active = stream->ReadBool();
	unknownE4 = stream->ReadLong();

	long savedCamera = stream->VirtualSlot16();
	if ( savedCamera != 0 ) {
		NiPoint3 position;
		position.x = stream->ReadFloat();
		position.y = stream->ReadFloat();
		position.z = stream->ReadFloat();
		// the set-up names the new camera and the object again
		char *savedName = name;
		name = NULL;
		Unknown42A290( position.x, position.y, position.z, 0.0f, 0.0f, 1.0f, 0.0f, savedName );
		NiMatrix3 rotation;
		rotation.SetEntry( 0, 0, stream->ReadFloat() );
		rotation.SetEntry( 0, 1, stream->ReadFloat() );
		rotation.SetEntry( 0, 2, stream->ReadFloat() );
		rotation.SetEntry( 1, 0, stream->ReadFloat() );
		rotation.SetEntry( 1, 1, stream->ReadFloat() );
		rotation.SetEntry( 1, 2, stream->ReadFloat() );
		rotation.SetEntry( 2, 0, stream->ReadFloat() );
		rotation.SetEntry( 2, 1, stream->ReadFloat() );
		rotation.SetEntry( 2, 2, stream->ReadFloat() );
		camera->SetRotate( rotation );
		delete savedName;
		pointerMap.Add( savedCamera, camera );
		pointerMap.Add( stream->VirtualSlot16(), GetAnimNode() );
	}

	for ( int i = stream->ReadInt() - 1; i >= 0; i-- ) {
		modeStack.AddTail( (CameraMode *)stream->ReadID() );
	}
	lastModeID = stream->ReadID();

	while ( stream->ReadBool() == true ) {
		char *modeName = stream->VirtualSlot10( -1 );
		namedModes.SetAt( modeName, (CameraMode *)stream->ReadID() );
		delete modeName;
	}
	while ( stream->ReadBool() ) {
		// BUG: the map files a copy of the key; the read key leaks
		char *modeName = stream->VirtualSlot10( -1 );
		modeHierarchy.SetAt( modeName, stream->VirtualSlot10( -1 ) );
	}
	return true;
}

/*
================
CameraObjectClass::FixUpPointers

Fails until every stacked and every named mode is loaded.
================
*/
// FUNCTION: STBC 0x0042d0d0
bool CameraObjectClass::FixUpPointers( void ) {
	if ( !BaseObjectClass::FixUpPointers() ) {
		return false;
	}
	NiTListIterator pos = modeStack.GetHeadPos();
	while ( pos != NULL ) {
		if ( TGObject::GetTGObjectPtr( (long)modeStack.GetNext( pos ) ) == NULL ) {
			return false;
		}
	}
	NiTMapIterator item = namedModes.GetFirstPos();
	while ( item != NULL ) {
		const char *name_;
		CameraMode *mode;
		namedModes.GetNext( item, name_, mode );
		if ( TGObject::GetTGObjectPtr( (long)mode ) == NULL ) {
			return false;
		}
	}
	return true;
}

/*
================
CameraObjectClass::PostFixUp

Turns the saved IDs of the stacked and the named modes back into the
modes; the last mode restored, the bottom of the stack, becomes the
current mode.
================
*/
// FUNCTION: STBC 0x0042d190
void CameraObjectClass::PostFixUp( void ) {
	BaseObjectClass::PostFixUp();

	NiTList<CameraMode *> savedIDs( 2 );
	while ( !modeStack.IsEmpty() ) {
		savedIDs.AddTail( modeStack.RemoveHead() );
	}
	CameraMode *mode = NULL;
	while ( !savedIDs.IsEmpty() ) {
		mode = (CameraMode *)TGObject::GetTGObjectPtr( (long)savedIDs.RemoveHead() );
		modeStack.AddTail( mode );
	}
	currentMode = mode;
	if ( mode != NULL ) {
		mode->VirtualSlot28();
	}

	// BUG: redundant: a second list of modes, never used
	NiTList<CameraMode *> unused( 2 );
	NiTMapIterator item = namedModes.GetFirstPos();
	while ( item != NULL ) {
		const char *name_;
		CameraMode *savedID;
		namedModes.GetNext( item, name_, savedID );
		char *key = new char[strlen( name_ ) + 1];
		strcpy( key, name_ );
		namedModes.SetAt( key, (CameraMode *)TGObject::GetTGObjectPtr( (long)savedID ) );
		delete [] key;
	}
}
