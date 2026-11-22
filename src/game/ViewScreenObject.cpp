/*
===============================================================================

	ViewScreenObject.cpp

===============================================================================
*/

#include "NiMain.h"
#include "ViewScreenObject.h"
#include "ViewScreenNode.h"
#include "SetClass.h"
#include "SetManager.h"
#include "Game.h"
#include "TGPane.h"
#include "TopWindow.h"
#include "TacticalControlWindow.h"
#include "TGModelManager.h"
#include "TGBoolEvent.h"
#include "TGEventManager.h"
#include "TGEventHandlerObject.h"
#include "CharacterClass.h"
#include "CameraObjectClass.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "STTopLevelMenu.h"
#include "PyEmbed.h"
#include "TGString.h"
#include "TGSoundAction.h"
#include "UtopiaModule.h"
#include "ZoomCameraObjectClass.h"
#include "ReticleManagerWindow.h"
#include <string.h>
#include <iostream>

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00679b20 _$E525
// FUNCTION: STBC 0x00679b40 _$E524

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00678250 _$E21
// FUNCTION: STBC 0x00678260 _$E20
// FUNCTION: STBC 0x00678270 _$E25
// FUNCTION: STBC 0x00678280 _$E24
// FUNCTION: STBC 0x00678290 _$E36
// FUNCTION: STBC 0x006782a0 _$E35
// FUNCTION: STBC 0x006782b0 _$E40
// FUNCTION: STBC 0x006782c0 _$E39
// FUNCTION: STBC 0x006782d0 _$E44
// FUNCTION: STBC 0x006782e0 _$E43
// FUNCTION: STBC 0x00678460 _$E48
// FUNCTION: STBC 0x00678470 _$E47
// FUNCTION: STBC 0x00678970 _$E172
// FUNCTION: STBC 0x00678980 _$E171
// FUNCTION: STBC 0x00679340 _$E114
// FUNCTION: STBC 0x00679350 _$E113
// FUNCTION: STBC 0x00679360 _$E127
// FUNCTION: STBC 0x00679370 _$E126
// FUNCTION: STBC 0x00679380 _$E177
// FUNCTION: STBC 0x00679390 _$E176
// FUNCTION: STBC 0x006793a0 _$E182
// FUNCTION: STBC 0x006793b0 _$E181
// FUNCTION: STBC 0x006793c0 _$E186
// FUNCTION: STBC 0x006793d0 _$E185
// FUNCTION: STBC 0x006793e0 _$E190
// FUNCTION: STBC 0x006793f0 _$E189
// FUNCTION: STBC 0x00679400 _$E197
// FUNCTION: STBC 0x00679410 _$E196
// FUNCTION: STBC 0x00679420 _$E222
// FUNCTION: STBC 0x00679430 _$E221
// FUNCTION: STBC 0x00679440 _$E226
// FUNCTION: STBC 0x00679450 _$E225
// FUNCTION: STBC 0x00679460 _$E72
// FUNCTION: STBC 0x00679470 _$E71
// FUNCTION: STBC 0x00679480 _$E208
// FUNCTION: STBC 0x00679490 _$E207
// FUNCTION: STBC 0x006794a0 _$E215
// FUNCTION: STBC 0x006794b0 _$E214
// FUNCTION: STBC 0x006794c0 _$E238
// FUNCTION: STBC 0x006794d0 _$E237
// FUNCTION: STBC 0x006794e0 _$E242
// FUNCTION: STBC 0x006794f0 _$E241
// FUNCTION: STBC 0x00679500 _$E282
// FUNCTION: STBC 0x00679510 _$E281
// FUNCTION: STBC 0x00679520 _$E288
// FUNCTION: STBC 0x00679530 _$E287
// FUNCTION: STBC 0x00679540 _$E230
// FUNCTION: STBC 0x00679550 _$E229
// FUNCTION: STBC 0x00679560 _$E234
// FUNCTION: STBC 0x00679570 _$E233
// FUNCTION: STBC 0x00679580 _$E246
// FUNCTION: STBC 0x00679590 _$E245
// FUNCTION: STBC 0x006795a0 _$E250
// FUNCTION: STBC 0x006795b0 _$E249
// FUNCTION: STBC 0x006795c0 _$E254
// FUNCTION: STBC 0x006795d0 _$E253
// FUNCTION: STBC 0x006795e0 _$E258
// FUNCTION: STBC 0x006795f0 _$E257
// FUNCTION: STBC 0x00679600 _$E262
// FUNCTION: STBC 0x00679610 _$E261
// FUNCTION: STBC 0x00679620 _$E266
// FUNCTION: STBC 0x00679630 _$E265
// FUNCTION: STBC 0x00679640 _$E270
// FUNCTION: STBC 0x00679650 _$E269
// FUNCTION: STBC 0x00679660 _$E274
// FUNCTION: STBC 0x00679670 _$E273
// FUNCTION: STBC 0x00679680 _$E77
// FUNCTION: STBC 0x00679690 _$E76
// FUNCTION: STBC 0x006796a0 _$E278
// FUNCTION: STBC 0x006796b0 _$E277
// FUNCTION: STBC 0x006796c0 _$E82
// FUNCTION: STBC 0x006796d0 _$E81
// FUNCTION: STBC 0x006796e0 _$E292
// FUNCTION: STBC 0x006796f0 _$E291
// FUNCTION: STBC 0x00679700 _$E296
// FUNCTION: STBC 0x00679710 _$E295
// FUNCTION: STBC 0x00679720 _$E300
// FUNCTION: STBC 0x00679730 _$E299
// FUNCTION: STBC 0x00679740 _$E304
// FUNCTION: STBC 0x00679750 _$E303
// FUNCTION: STBC 0x00679760 _$E91
// FUNCTION: STBC 0x00679770 _$E90
// FUNCTION: STBC 0x00679780 _$E308
// FUNCTION: STBC 0x00679790 _$E307
// FUNCTION: STBC 0x006797a0 _$E312
// FUNCTION: STBC 0x006797b0 _$E311
// FUNCTION: STBC 0x006797c0 _$E316
// FUNCTION: STBC 0x006797d0 _$E315
// FUNCTION: STBC 0x006797e0 _$E97
// FUNCTION: STBC 0x006797f0 _$E96
// FUNCTION: STBC 0x00679800 _$E102
// FUNCTION: STBC 0x00679810 _$E101
// FUNCTION: STBC 0x00679820 _$E132
// FUNCTION: STBC 0x00679830 _$E131
// FUNCTION: STBC 0x00679840 _$E320
// FUNCTION: STBC 0x00679850 _$E319
// FUNCTION: STBC 0x00679860 _$E137
// FUNCTION: STBC 0x00679870 _$E136
// FUNCTION: STBC 0x00679880 _$E324
// FUNCTION: STBC 0x00679890 _$E323
// FUNCTION: STBC 0x006798a0 _$E328
// FUNCTION: STBC 0x006798b0 _$E327
// FUNCTION: STBC 0x006798c0 _$E142
// FUNCTION: STBC 0x006798d0 _$E141
// FUNCTION: STBC 0x006798e0 _$E332
// FUNCTION: STBC 0x006798f0 _$E331
// FUNCTION: STBC 0x00679900 _$E107
// FUNCTION: STBC 0x00679910 _$E106
// FUNCTION: STBC 0x00679920 _$E336
// FUNCTION: STBC 0x00679930 _$E335
// FUNCTION: STBC 0x00679940 _$E340
// FUNCTION: STBC 0x00679950 _$E339
// FUNCTION: STBC 0x00679960 _$E147
// FUNCTION: STBC 0x00679970 _$E146
// FUNCTION: STBC 0x00679980 _$E152
// FUNCTION: STBC 0x00679990 _$E151
// FUNCTION: STBC 0x006799a0 _$E344
// FUNCTION: STBC 0x006799b0 _$E343
// FUNCTION: STBC 0x006799c0 _$E348
// FUNCTION: STBC 0x006799d0 _$E347
// FUNCTION: STBC 0x006799e0 _$E352
// FUNCTION: STBC 0x006799f0 _$E351
// FUNCTION: STBC 0x00679a00 _$E356
// FUNCTION: STBC 0x00679a10 _$E355
// FUNCTION: STBC 0x00679a20 _$E360
// FUNCTION: STBC 0x00679a30 _$E359
// FUNCTION: STBC 0x00679a40 _$E364
// FUNCTION: STBC 0x00679a50 _$E363
// FUNCTION: STBC 0x00679a60 _$E368
// FUNCTION: STBC 0x00679a70 _$E367
// FUNCTION: STBC 0x00679a80 _$E372
// FUNCTION: STBC 0x00679a90 _$E371
// FUNCTION: STBC 0x00679aa0 _$E157
// FUNCTION: STBC 0x00679ab0 _$E156
// FUNCTION: STBC 0x00679ac0 _$E376
// FUNCTION: STBC 0x00679ad0 _$E375
// FUNCTION: STBC 0x00679ae0 _$E162
// FUNCTION: STBC 0x00679af0 _$E161
// FUNCTION: STBC 0x00679b00 _$E167
// FUNCTION: STBC 0x00679b10 _$E166
// FUNCTION: STBC 0x00679b50 _$E448
// FUNCTION: STBC 0x00679b60 _$E447
// FUNCTION: STBC 0x00679b70 _$E453
// FUNCTION: STBC 0x00679b80 _$E452
// FUNCTION: STBC 0x00679b90 _$E457
// FUNCTION: STBC 0x00679ba0 _$E456

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006781d0 _$E505
// FUNCTION: STBC 0x006781e0 _$E502
// FUNCTION: STBC 0x006781f0 _$E504
// FUNCTION: STBC 0x00678200 _$E503
// FUNCTION: STBC 0x00678210 _$E511
// FUNCTION: STBC 0x00678220 _$E508
// FUNCTION: STBC 0x00678230 _$E510
// FUNCTION: STBC 0x00678240 _$E509

// The handler table's dynamic initialiser and its exit-time destructor.
// FUNCTION: STBC 0x006782f0 _$E517
// FUNCTION: STBC 0x00678300 _$E513
// FUNCTION: STBC 0x00678310 _$E516
// FUNCTION: STBC 0x00678320 _$E514
// DATA: STBC 0x009922e0
TGEventHandlerTable ViewScreenObject::classHandlers;

/*
================
ViewScreenObject::AddToSet

Adds the object to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00678340
void ViewScreenObject::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, VIEWSCREENOBJECT_TYPE );
		ObjectClass::AddToSet( set );
	}
}

/*
================
ViewScreenObject::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00678370
void ViewScreenObject::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, VIEWSCREENOBJECT_TYPE );
		ObjectClass::RemoveFromSet( set );
	}
}

/*
================
ViewScreenObject::GetClassObjects

The set's view screens and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x006783a0
ViewScreenObject **ViewScreenObject::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (ViewScreenObject **)set->objectTable.GetObjects( VIEWSCREENOBJECT_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
ViewScreenObject::GetNumClassObjects

The number of the set's objects of the class.
================
*/
// FUNCTION: STBC 0x006783d0
int ViewScreenObject::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( VIEWSCREENOBJECT_TYPE );
	}
	return 0;
}

/*
================
ViewScreenObject::GetObjectByID

The object of the class with the given ID in the set, or among every
object when the set is NULL; NULL when it is none or of another class.
================
*/
// FUNCTION: STBC 0x006783f0
ViewScreenObject *ViewScreenObject::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( VIEWSCREENOBJECT_TYPE ) ) {
			return (ViewScreenObject *)object;
		}
		return NULL;
	}
	return (ViewScreenObject *)set->GetClassObject( VIEWSCREENOBJECT_TYPE, id );
}

/*
================
ViewScreenObject::GetObject

The object of the class and name in the set, or in any set when the set
is NULL, the last set first.
================
*/
// FUNCTION: STBC 0x00678480
ViewScreenObject *ViewScreenObject::GetObject( SetClass *set, char *name ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( VIEWSCREENOBJECT_TYPE, name );
		}
		return (ViewScreenObject *)object;
	}
	return (ViewScreenObject *)set->GetClassObject( VIEWSCREENOBJECT_TYPE, name );
}

/*
================
ViewScreenObject::Cast

The object as a ViewScreenObject; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006784e0
ViewScreenObject *ViewScreenObject::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( VIEWSCREENOBJECT_TYPE ) ) {
		return (ViewScreenObject *)object;
	}
	return NULL;
}

/*
================
ViewScreenObject::Create

A new view screen showing the named model.
================
*/
// FUNCTION: STBC 0x00678510
ViewScreenObject *ViewScreenObject::Create( char *name ) {
	return new ViewScreenObject( name );
}

/*
================
ViewScreenObject::ViewScreenObject
================
*/
// FUNCTION: STBC 0x00678540 ??0ViewScreenObject@@QAE@PAD@Z
ViewScreenObject::ViewScreenObject( char *name ) {
	Init( name );
}

/*
================
ViewScreenObject::GetObjType
================
*/
// FUNCTION: STBC 0x006785a0
int ViewScreenObject::GetObjType( void ) {
	return VIEWSCREENOBJECT_TYPE;
}

/*
================
ViewScreenObject::IsTypeOf
================
*/
// FUNCTION: STBC 0x006785b0
bool ViewScreenObject::IsTypeOf( int type ) {
	if ( type == VIEWSCREENOBJECT_TYPE ) {
		return true;
	}
	return ObjectClass::IsTypeOf( type );
}

/*
================
ViewScreenObject::GetClassName
================
*/
// FUNCTION: STBC 0x00678610
char *ViewScreenObject::GetClassName( void ) {
	return "ViewScreenObject";
}

/*
================
ViewScreenObject::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00678620
char *ViewScreenObject::GetSwigTypeName( void ) {
	return "_p_ViewScreenObject";
}

/*
================
ViewScreenObject::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00678630
char *ViewScreenObject::GetPythonClassName( void ) {
	return "ViewScreenObjectPtr";
}

/*
================
ViewScreenObject::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00678640
TGEventHandlerTable *ViewScreenObject::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ViewScreenObject::ViewScreenObject

The load's constructor: the model comes with the stream.
================
*/
// FUNCTION: STBC 0x00678680 ??0ViewScreenObject@@QAE@J@Z
ViewScreenObject::ViewScreenObject( long id ) : ObjectClass( id ) {
	Init( NULL );
}

/*
================
ViewScreenObject::~ViewScreenObject
================
*/
// FUNCTION: STBC 0x006786e0
ViewScreenObject::~ViewScreenObject( void ) {
	if ( modelName != NULL ) {
		g_kModelManager.Unrefer( modelName );
		delete [] modelName;
	}
}

/*
================
ViewScreenObject::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00678750
void ViewScreenObject::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ViewScreenObject::MenuEventHandler, "ViewScreenObject::MenuEventHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ViewScreenObject::CameraDeletedHandler, "ViewScreenObject::CameraDeletedHandler" );
}

/*
================
ViewScreenObject::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00678780
void ViewScreenObject::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_CHARACTER_MENU, "ViewScreenObject::MenuEventHandler" );
}

/*
================
ViewScreenObject::Init

Makes the view screen node and shows the named model on it.
================
*/
// FUNCTION: STBC 0x006787a0
void ViewScreenObject::Init( char *name ) {
	modelName = NULL;
	remoteCam = NULL;
	node = new ViewScreenNode;
	if ( name != NULL ) {
		SetModel( name );
	}
	menu = NULL;
	useRemoteCam = true;
}

/*
================
ViewScreenObject::SetModel

Releases the old model and shows a clone of the named one, loading it first
when it is not loaded.
================
*/
// FUNCTION: STBC 0x00678850
void ViewScreenObject::SetModel( char *modelName_ ) {
	if ( modelName != NULL ) {
		g_kModelManager.Unrefer( modelName );
		delete [] modelName;
		modelName = NULL;
	}
	if ( modelName_ != NULL ) {
		modelName = new char[strlen( modelName_ ) + 1];
		strcpy( modelName, modelName_ );
		if ( !g_kModelManager.IsModelLoaded( modelName ) ) {
			g_kModelManager.LoadModel( modelName, NULL, NULL );
		}
		SetNiObject( g_kModelManager.CloneModel( modelName ) );
	}
}

/*
================
ViewScreenObject::GetViewScreenNode
================
*/
// FUNCTION: STBC 0x00678900
ViewScreenNode *ViewScreenObject::GetViewScreenNode( void ) {
	return (ViewScreenNode *)GetNode();
}

/*
================
ViewScreenObject::SetRemoteCam

Watches the new camera for its deletion instead of the old one. Without a
camera the screen goes off.
================
*/
// FUNCTION: STBC 0x00678910
void ViewScreenObject::SetRemoteCam( CameraObjectClass *camera ) {
	if ( remoteCam != NULL ) {
		g_kEventManager.RemoveBroadcastHandler( ET_DELETE_ME_STAGE_1, this, "ViewScreenObject::CameraDeletedHandler", remoteCam );
	}
	remoteCam = camera;
	if ( camera == NULL ) {
		SetIsOn( false );
		return;
	}
	useRemoteCam = true;
	g_kEventManager.AddBroadcastMethodHandler( ET_DELETE_ME_STAGE_1, this, "ViewScreenObject::CameraDeletedHandler", camera );
}

/*
================
ViewScreenObject::GetRemoteCam

The remote camera when it is in use, else the player's camera.
================
*/
// FUNCTION: STBC 0x00678990
CameraObjectClass *ViewScreenObject::GetRemoteCam( void ) {
	Game *game = Game::GetCurrentGame();
	CameraObjectClass *camera = remoteCam;
	if ( !useRemoteCam ) {
		camera = game->GetPlayerCamera();
	}
	return camera;
}

/*
================
ViewScreenObject::ToggleRemoteCam
================
*/
// FUNCTION: STBC 0x006789b0
void ViewScreenObject::ToggleRemoteCam( bool on ) {
	useRemoteCam = on;
}

/*
================
ViewScreenObject::SetIsOn
================
*/
// FUNCTION: STBC 0x006789c0
void ViewScreenObject::SetIsOn( bool on ) {
	GetViewScreenNode()->SetIsOn( on );
}

/*
================
ViewScreenObject::IsOn
================
*/
// FUNCTION: STBC 0x006789e0
bool ViewScreenObject::IsOn( void ) {
	return GetViewScreenNode()->IsOn();
}

/*
================
ViewScreenObject::SetStaticIsOn
================
*/
// FUNCTION: STBC 0x006789f0
void ViewScreenObject::SetStaticIsOn( bool on ) {
	GetViewScreenNode()->SetStaticIsOn( on );
}

/*
================
ViewScreenObject::SetStaticVariation
================
*/
// FUNCTION: STBC 0x00678a10
void ViewScreenObject::SetStaticVariation( float low, float high ) {
	GetViewScreenNode()->SetStaticVariation( low, high );
}

/*
================
ViewScreenObject::IsStaticOn
================
*/
// FUNCTION: STBC 0x00678a30
bool ViewScreenObject::IsStaticOn( void ) {
	// BUG: answers whether the screen is on, not whether its static is
	return GetViewScreenNode()->IsOn();
}

/*
================
ViewScreenObject::SetOffTexture
================
*/
// FUNCTION: STBC 0x00678a40
void ViewScreenObject::SetOffTexture( char *name ) {
	GetViewScreenNode()->SetOffTexture( name );
}

/*
================
ViewScreenObject::SetStaticTextureIconGroup
================
*/
// FUNCTION: STBC 0x00678a60
void ViewScreenObject::SetStaticTextureIconGroup( char *name ) {
	GetViewScreenNode()->SetStaticTextureIconGroup( name );
}

/*
================
ViewScreenObject::Unknown678A80

Draws what the remote camera sees into the screen's rect of the camera's
view, the reticles laid out for it, and returns true; false while the
screen is off or out of view.
================
*/
// FUNCTION: STBC 0x00678a80
bool ViewScreenObject::Unknown678A80( CameraObjectClass *camera ) {
	NiPointer<NiCamera> screenCamera = CreateScreenCamera( camera );
	if ( screenCamera == NULL ) {
		return false;
	}
	// MATCH-PENDING: R17, GetRemoteCam's body (0x678990, defined earlier in the unit) written out by hand where the original most likely called GetRemoteCam() and VC6 expanded it; reviewer's verdict at merge, open for helper recovery
	Game *game = Game::GetCurrentGame();
	CameraObjectClass *remote = remoteCam;
	if ( !useRemoteCam ) {
		remote = game->GetPlayerCamera();
	}
	ViewScreenNode *node = GetViewScreenNode();
	float left, right, top, bottom;
	NiRect<float> port;
	node->GetScreenRect( left, right, top, bottom, port );
	NiCamera *viewCamera = camera->GetNiCamera();
	ReticleManagerWindow *reticles = ReticleManagerWindow::GetReticleManagerWindow();
	reticles->SetZoom( viewCamera, screenCamera, left, right, top, bottom, port );
	node->FitScreenCamera( viewCamera, screenCamera );
	SetClass *set = remote->GetContainingSet();
	if ( set != NULL ) {
		set->VirtualSlot25( screenCamera, true, false, false );
		NiRender *renderer = screenCamera->GetRenderer();
		renderer->VirtualSlot38( screenCamera );
		renderer->VirtualSlot43( true );
		reticles->VirtualSlot63( renderer );
		renderer->VirtualSlot44();
		renderer->VirtualSlot39();
		reticles->RestoreZoom();
	}
	return true;
}

/*
================
ViewScreenObject::CreateScreenCamera

The remote camera's copy: its scene, place and frustum, with the camera's
renderer, its port the screen's rect in the camera's view.
================
*/
// FUNCTION: STBC 0x00678c30
NiCamera *ViewScreenObject::CreateScreenCamera( CameraObjectClass *camera ) {
	if ( !IsOn() || camera == NULL || GetRemoteCam() == NULL ) {
		return NULL;
	}
	ViewScreenNode *node = GetViewScreenNode();
	if ( !node->IsInView( camera ) ) {
		return NULL;
	}
	// MATCH-PENDING: R17, GetRemoteCam's body (0x678990, defined earlier in the unit) written out by hand after a real GetRemoteCam() call above; the original most likely called it both times and VC6 expanded this one; reviewer's verdict at merge, open for helper recovery
	Game *game = Game::GetCurrentGame();
	CameraObjectClass *remote = remoteCam;
	if ( !useRemoteCam ) {
		remote = game->GetPlayerCamera();
	}
	NiCamera *viewCamera = camera->GetNiCamera();
	NiCamera *remoteCamera = remote->GetNiCamera();
	NiCamera *screenCamera = new NiCamera;
	screenCamera->SetRenderer( viewCamera->GetRenderer() );
	screenCamera->SetScene( remoteCamera->GetScene() );
	screenCamera->SetTranslate( remoteCamera->GetTranslate() );
	screenCamera->SetRotate( remoteCamera->GetRotate() );
	screenCamera->SetViewFrustum( remoteCamera->GetViewFrustum() );
	if ( !node->FindScreenPort( viewCamera, screenCamera ) ) {
		delete screenCamera;
		return NULL;
	}
	screenCamera->Update( 0.0f );
	return screenCamera;
}

/*
================
ViewScreenObject::MenuUp

Asks the screen's menu to open, through an event to itself.
================
*/
// FUNCTION: STBC 0x00678e20
void ViewScreenObject::MenuUp( void ) {
	TGBoolEvent *event = new TGBoolEvent;
	event->SetDestination( this );
	event->SetBool( true );
	event->SetEventType( ET_CHARACTER_MENU );
	g_kEventManager.AddEvent( event );
}

/*
================
ViewScreenObject::MenuDown

Asks the screen's menu to close.
================
*/
// FUNCTION: STBC 0x00678e80
void ViewScreenObject::MenuDown( void ) {
	TGBoolEvent *event = new TGBoolEvent;
	event->SetDestination( this );
	event->SetBool( false );
	event->SetEventType( ET_CHARACTER_MENU );
	g_kEventManager.AddEvent( event );
}

/*
================
ViewScreenObject::MenuEventHandler

The menu comes up or goes down (ET_CHARACTER_MENU, as MenuUp and MenuDown send
it). Coming up, the bridge's camera looks at the screen's menu pane and
zooms in on it. Going down, the zoom ends, the screen switches off with its
sound, shows space again and lets go of the menu.
================
*/
// FUNCTION: STBC 0x00678ee0
// PARKED: schedule: SetOwner's inlined owner store lands two instructions later in retail (after LookToward's second push and `mov ecx, edi`); SetOwner after the NiPoint3s, the menu re-read and a camera local all move it further
void ViewScreenObject::MenuEventHandler( TGEvent *event ) {
	if ( menu == NULL ) {
		return;
	}
	SetClass *bridge = g_kSetManager.GetSet( "bridge" );
	TacticalControlWindow *window = TacticalControlWindow::GetTacticalControlWindow();
	TGString name( "" );
	// the menu is a character menu (STTopLevelMenu), whose owner is this screen
	( (STTopLevelMenu *)menu )->GetName( &name );
	window->GetMenuParentPane( &name );
	if ( ( (TGBoolEvent *)event )->GetBool() ) {
		PyEmbed::CallFunction( "BridgeHandlers", "DropMenusTurnBack", NULL, NULL, NULL );
		STTopLevelMenu *characterMenu = (STTopLevelMenu *)menu;
		characterMenu->SetVisible( true );
		characterMenu->SetOwner( this );
		NiPoint3 up( 0.0f, 0.0f, 1.0f );
		NiPoint3 forward( 0.0f, -1.0f, 0.0f );
		bridge->GetActiveCamera()->LookToward( &forward, &up, 1.0f );
		ZoomCameraObjectClass *camera = ZoomCameraObjectClass::GetObject( bridge, "maincamera" );
		if ( camera == NULL ) {
			return;
		}
		if ( camera->IsZoomed() ) {
			camera->ToggleZoom( g_kUtopiaModule.GetGameTime() );
		}
		camera->SetZoom( 0.7f );
		camera->UpdateViewFrustum();
		camera->ToggleZoom( g_kUtopiaModule.GetGameTime() );
	} else {
		( (STTopLevelMenu *)menu )->SetNotVisible( true );
		ZoomCameraObjectClass *camera = ZoomCameraObjectClass::GetObject( bridge, "maincamera" );
		if ( camera != NULL && camera->IsZoomed() ) {
			camera->ToggleZoom( g_kUtopiaModule.GetGameTime() );
		}
		TGSoundAction *sound = new TGSoundAction( "ViewOff", 3, NULL, 1.0f );
		sound->Play();
		LookTowardsSpace();
		ClearMenu();
		if ( PyEmbed::CallFunction( "MissionLib", "ResetViewscreen", NULL, NULL, NULL ) == -1 ) {
			PyErr_Print();
		}
	}
}

/*
================
ViewScreenObject::CameraDeletedHandler

Lets go of the remote camera when it is the one being deleted.
================
*/
// FUNCTION: STBC 0x00679120
void ViewScreenObject::CameraDeletedHandler( TGEvent *event ) {
	if ( remoteCam == event->GetDestination() ) {
		SetRemoteCam( NULL );
	}
}

/*
================
ViewScreenObject::IsMenuUp

Whether the menu's top-level pane is visible: the pane just below the
tactical control window, or the one below the top window's own child.
================
*/
// FUNCTION: STBC 0x00679140
int ViewScreenObject::IsMenuUp( void ) {
	if ( menu == NULL ) {
		return 0;
	}
	TGPane *pane = menu->GetParent();
	TGPane *child = NULL;
	TGPane *tacticalControl = (TGPane *)TacticalControlWindow::GetTacticalControlWindow();
	TGPane *top = (TGPane *)TopWindow::GetTopWindow();
	while ( pane->GetParent() != NULL && pane->GetParent() != tacticalControl && pane->GetParent() != top ) {
		child = pane;
		pane = pane->GetParent();
	}
	if ( pane->GetParent() == top ) {
		pane = child;
	}
	return pane->IsVisible();
}

/*
================
ViewScreenObject::LookTowardsSpace

Switches the screen on and shows the player's camera.
================
*/
// FUNCTION: STBC 0x00679190
void ViewScreenObject::LookTowardsSpace( void ) {
	SetIsOn( true );
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		SetRemoteCam( game->GetPlayerCamera() );
	}
}

/*
================
ViewScreenObject::WriteToStream

The remote camera's ID, the model's name, the menu's ID, and whether the
screen and the remote camera are on.
================
*/
// FUNCTION: STBC 0x006791c0
bool ViewScreenObject::WriteToStream( TGStream *stream ) {
	if ( !ObjectClass::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteID( TGObject::GetSavedID( remoteCam ) );
	stream->VirtualSlot11( modelName );
	stream->WriteID( TGObject::GetSavedID( menu ) );
	bool on = GetViewScreenNode()->IsOn();
	stream->WriteBool( on );
	stream->WriteBool( useRemoteCam );
	return true;
}

/*
================
ViewScreenObject::ReadFromStream

Reads what WriteToStream wrote; the camera and the menu stay IDs until
FixUpPointers.
================
*/
// FUNCTION: STBC 0x00679240
bool ViewScreenObject::ReadFromStream( TGStream *stream ) {
	if ( !ObjectClass::ReadFromStream( stream ) ) {
		return false;
	}

	remoteCam = (CameraObjectClass *)stream->ReadID();
	char *name = stream->VirtualSlot10( -1 );
	SetModel( name );
	delete [] name;
	menu = (TGPane *)stream->ReadID();
	if ( stream->ReadBool() ) {
		GetViewScreenNode()->SetIsOn( true );
	}
	useRemoteCam = stream->ReadBool();
	return true;
}

/*
================
ViewScreenObject::FixUpPointers

False while the remote camera or the menu is not loaded yet.
================
*/
// FUNCTION: STBC 0x006792c0
bool ViewScreenObject::FixUpPointers( void ) {
	if ( !ObjectClass::FixUpPointers() ) {
		return false;
	}

	if ( remoteCam != NULL && GetTGObjectPtr( (long)remoteCam ) == NULL ) {
		return false;
	}
	if ( menu != NULL && GetTGObjectPtr( (long)menu ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ViewScreenObject::PostFixUp

Turns the saved IDs back into pointers.
================
*/
// FUNCTION: STBC 0x00679310
void ViewScreenObject::PostFixUp( void ) {
	ObjectClass::PostFixUp();

	remoteCam = (CameraObjectClass *)GetTGObjectPtr( (long)remoteCam );
	menu = (TGPane *)GetTGObjectPtr( (long)menu );
}
