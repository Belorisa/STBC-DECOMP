/*
===============================================================================

	WarpSet.cpp

===============================================================================
*/

#include <iostream>
#include <stdlib.h>

#include "WarpSet.h"
#include "TGEventHandlerTable.h"
#include "BridgeSet.h"
#include "CameraObjectClass.h"
#include "Game.h"
#include "MainWindow.h"
#include "SetManager.h"
#include "ShipClass.h"
#include "StarSphere.h"
#include "TGStream.h"
#include "TopWindow.h"
#include "UtopiaApp.h"
#include "ViewScreenObject.h"
#include "WarpTrail.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004d8850 _$E489
// FUNCTION: STBC 0x004d8870 _$E488

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004d7630 _$E32
// FUNCTION: STBC 0x004d7640 _$E29
// FUNCTION: STBC 0x004d7650 _$E31
// FUNCTION: STBC 0x004d7660 _$E30
// FUNCTION: STBC 0x004d7670 _$E38
// FUNCTION: STBC 0x004d7680 _$E35
// FUNCTION: STBC 0x004d7690 _$E37
// FUNCTION: STBC 0x004d76a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004d76b0 _$E66
// FUNCTION: STBC 0x004d76c0 _$E65
// FUNCTION: STBC 0x004d76d0 _$E241
// FUNCTION: STBC 0x004d76e0 _$E240
// FUNCTION: STBC 0x004d76f0 _$E77
// FUNCTION: STBC 0x004d7700 _$E76
// FUNCTION: STBC 0x004d7ab0 _$E81
// FUNCTION: STBC 0x004d7ac0 _$E80
// FUNCTION: STBC 0x004d7f20 _$E85
// FUNCTION: STBC 0x004d7f30 _$E84
// FUNCTION: STBC 0x004d8030 _$E89
// FUNCTION: STBC 0x004d8040 _$E88
// FUNCTION: STBC 0x004d8050 _$E206
// FUNCTION: STBC 0x004d8060 _$E205
// FUNCTION: STBC 0x004d8070 _$E148
// FUNCTION: STBC 0x004d8080 _$E147
// FUNCTION: STBC 0x004d8090 _$E161
// FUNCTION: STBC 0x004d80a0 _$E160
// FUNCTION: STBC 0x004d80b0 _$E211
// FUNCTION: STBC 0x004d80c0 _$E210
// FUNCTION: STBC 0x004d80d0 _$E246
// FUNCTION: STBC 0x004d80e0 _$E245
// FUNCTION: STBC 0x004d80f0 _$E215
// FUNCTION: STBC 0x004d8100 _$E214
// FUNCTION: STBC 0x004d8110 _$E250
// FUNCTION: STBC 0x004d8120 _$E249
// FUNCTION: STBC 0x004d8130 _$E257
// FUNCTION: STBC 0x004d8140 _$E256
// FUNCTION: STBC 0x004d8150 _$E329
// FUNCTION: STBC 0x004d8160 _$E328
// FUNCTION: STBC 0x004d8170 _$E333
// FUNCTION: STBC 0x004d8180 _$E332
// FUNCTION: STBC 0x004d8190 _$E106
// FUNCTION: STBC 0x004d81a0 _$E105
// FUNCTION: STBC 0x004d81b0 _$E274
// FUNCTION: STBC 0x004d81c0 _$E273
// FUNCTION: STBC 0x004d81d0 _$E281
// FUNCTION: STBC 0x004d81e0 _$E280
// FUNCTION: STBC 0x004d81f0 _$E285
// FUNCTION: STBC 0x004d8200 _$E284
// FUNCTION: STBC 0x004d8210 _$E289
// FUNCTION: STBC 0x004d8220 _$E288
// FUNCTION: STBC 0x004d8230 _$E293
// FUNCTION: STBC 0x004d8240 _$E292
// FUNCTION: STBC 0x004d8250 _$E299
// FUNCTION: STBC 0x004d8260 _$E298
// FUNCTION: STBC 0x004d8270 _$E337
// FUNCTION: STBC 0x004d8280 _$E336
// FUNCTION: STBC 0x004d8290 _$E341
// FUNCTION: STBC 0x004d82a0 _$E340
// FUNCTION: STBC 0x004d82b0 _$E345
// FUNCTION: STBC 0x004d82c0 _$E344
// FUNCTION: STBC 0x004d82d0 _$E349
// FUNCTION: STBC 0x004d82e0 _$E348
// FUNCTION: STBC 0x004d82f0 _$E353
// FUNCTION: STBC 0x004d8300 _$E352
// FUNCTION: STBC 0x004d8310 _$E357
// FUNCTION: STBC 0x004d8320 _$E356
// FUNCTION: STBC 0x004d8330 _$E319
// FUNCTION: STBC 0x004d8340 _$E318
// FUNCTION: STBC 0x004d8350 _$E361
// FUNCTION: STBC 0x004d8360 _$E360
// FUNCTION: STBC 0x004d8370 _$E365
// FUNCTION: STBC 0x004d8380 _$E364
// FUNCTION: STBC 0x004d8390 _$E369
// FUNCTION: STBC 0x004d83a0 _$E368
// FUNCTION: STBC 0x004d83b0 _$E111
// FUNCTION: STBC 0x004d83c0 _$E110
// FUNCTION: STBC 0x004d83d0 _$E373
// FUNCTION: STBC 0x004d83e0 _$E372
// FUNCTION: STBC 0x004d83f0 _$E116
// FUNCTION: STBC 0x004d8400 _$E115
// FUNCTION: STBC 0x004d8410 _$E377
// FUNCTION: STBC 0x004d8420 _$E376
// FUNCTION: STBC 0x004d8430 _$E381
// FUNCTION: STBC 0x004d8440 _$E380
// FUNCTION: STBC 0x004d8450 _$E385
// FUNCTION: STBC 0x004d8460 _$E384
// FUNCTION: STBC 0x004d8470 _$E389
// FUNCTION: STBC 0x004d8480 _$E388
// FUNCTION: STBC 0x004d8490 _$E125
// FUNCTION: STBC 0x004d84a0 _$E124
// FUNCTION: STBC 0x004d84b0 _$E393
// FUNCTION: STBC 0x004d84c0 _$E392
// FUNCTION: STBC 0x004d84d0 _$E397
// FUNCTION: STBC 0x004d84e0 _$E396
// FUNCTION: STBC 0x004d84f0 _$E401
// FUNCTION: STBC 0x004d8500 _$E400
// FUNCTION: STBC 0x004d8510 _$E131
// FUNCTION: STBC 0x004d8520 _$E130
// FUNCTION: STBC 0x004d8530 _$E136
// FUNCTION: STBC 0x004d8540 _$E135
// FUNCTION: STBC 0x004d8550 _$E166
// FUNCTION: STBC 0x004d8560 _$E165
// FUNCTION: STBC 0x004d8570 _$E405
// FUNCTION: STBC 0x004d8580 _$E404
// FUNCTION: STBC 0x004d8590 _$E171
// FUNCTION: STBC 0x004d85a0 _$E170
// FUNCTION: STBC 0x004d85b0 _$E409
// FUNCTION: STBC 0x004d85c0 _$E408
// FUNCTION: STBC 0x004d85d0 _$E413
// FUNCTION: STBC 0x004d85e0 _$E412
// FUNCTION: STBC 0x004d85f0 _$E176
// FUNCTION: STBC 0x004d8600 _$E175
// FUNCTION: STBC 0x004d8610 _$E417
// FUNCTION: STBC 0x004d8620 _$E416
// FUNCTION: STBC 0x004d8630 _$E141
// FUNCTION: STBC 0x004d8640 _$E140
// FUNCTION: STBC 0x004d8650 _$E421
// FUNCTION: STBC 0x004d8660 _$E420
// FUNCTION: STBC 0x004d8670 _$E425
// FUNCTION: STBC 0x004d8680 _$E424
// FUNCTION: STBC 0x004d8690 _$E181
// FUNCTION: STBC 0x004d86a0 _$E180
// FUNCTION: STBC 0x004d86b0 _$E186
// FUNCTION: STBC 0x004d86c0 _$E185
// FUNCTION: STBC 0x004d86d0 _$E429
// FUNCTION: STBC 0x004d86e0 _$E428
// FUNCTION: STBC 0x004d86f0 _$E433
// FUNCTION: STBC 0x004d8700 _$E432
// FUNCTION: STBC 0x004d8710 _$E437
// FUNCTION: STBC 0x004d8720 _$E436
// FUNCTION: STBC 0x004d8730 _$E441
// FUNCTION: STBC 0x004d8740 _$E440
// FUNCTION: STBC 0x004d8750 _$E445
// FUNCTION: STBC 0x004d8760 _$E444
// FUNCTION: STBC 0x004d8770 _$E449
// FUNCTION: STBC 0x004d8780 _$E448
// FUNCTION: STBC 0x004d8790 _$E453
// FUNCTION: STBC 0x004d87a0 _$E452
// FUNCTION: STBC 0x004d87b0 _$E457
// FUNCTION: STBC 0x004d87c0 _$E456
// FUNCTION: STBC 0x004d87d0 _$E191
// FUNCTION: STBC 0x004d87e0 _$E190
// FUNCTION: STBC 0x004d87f0 _$E461
// FUNCTION: STBC 0x004d8800 _$E460
// FUNCTION: STBC 0x004d8810 _$E196
// FUNCTION: STBC 0x004d8820 _$E195
// FUNCTION: STBC 0x004d8830 _$E201
// FUNCTION: STBC 0x004d8840 _$E200
// FUNCTION: STBC 0x004d8880 _$E465
// FUNCTION: STBC 0x004d8890 _$E464
// FUNCTION: STBC 0x004d88a0 _$E470
// FUNCTION: STBC 0x004d88b0 _$E469
// FUNCTION: STBC 0x004d88c0 _$E474
// FUNCTION: STBC 0x004d88d0 _$E473

// The handler table's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004d7710 _$E480
// FUNCTION: STBC 0x004d7720 _$E476
// FUNCTION: STBC 0x004d7730 _$E479
// FUNCTION: STBC 0x004d7740 _$E477
// DATA: STBC 0x00985870
TGEventHandlerTable WarpSet::classHandlers;

/*
================
WarpSet::Cast

The object as a WarpSet; NULL when it is none.
================
*/
// FUNCTION: STBC 0x004d7760
WarpSet *WarpSet::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( WARPSET_TYPE ) ) {
		return (WarpSet *)object;
	}
	return NULL;
}

/*
================
WarpSet::WarpSet

A warp tunnel of the given number of trails (or the default), round a star
sphere backdrop.
================
*/
// FUNCTION: STBC 0x004d7790 ??0WarpSet@@QAE@H@Z
WarpSet::WarpSet( int numTrails_ ) {
	trails = NULL;
	SetNumTrails( numTrails_ != 0 ? numTrails_ : defaultNumTrails, true );

	StarSphere *stars = new StarSphere;
	stars->SetName( "Star Backdrop" );
	stars->SetTranslate( &NiPoint3( 0.0f, 0.0f, 0.0f ) );
	stars->AlignToVectors( NiPoint3( MODEL_FORWARD ), NiPoint3( MODEL_UP ) );
	stars->SetTextureFileName( "data/stars.tga" );
	stars->SetTargetPolyCount( 256 );
	stars->SetHorizontalSpan( 1.0f );
	stars->SetVerticalSpan( 1.0f );
	stars->SetSphereRadius( 300.0f );
	stars->SetTextureHTile( 22.0f );
	stars->SetTextureVTile( 11.0f );
	stars->Rebuild();
	AddBackdropToSet( stars, "Star Backdrop" );
	stars->Update( 0.0f );
	forceUpdate = false;
	// BUG: redundant: AddBackdropToSet already keeps the star sphere
	starSphere = stars;
}

/*
================
WarpSet::GetObjType
================
*/
// FUNCTION: STBC 0x004d7930
int WarpSet::GetObjType( void ) {
	return WARPSET_TYPE;
}

/*
================
WarpSet::IsTypeOf
================
*/
// FUNCTION: STBC 0x004d7940
bool WarpSet::IsTypeOf( int type ) {
	if ( type == WARPSET_TYPE ) {
		return true;
	}
	return SetClass::IsTypeOf( type );
}

/*
================
WarpSet::GetClassName
================
*/
// FUNCTION: STBC 0x004d7990
char *WarpSet::GetClassName( void ) {
	return "WarpSet";
}

/*
================
WarpSet::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004d79a0
char *WarpSet::GetSwigTypeName( void ) {
	return "_p_WarpSet";
}

/*
================
WarpSet::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004d79b0
char *WarpSet::GetPythonClassName( void ) {
	return "WarpSetPtr";
}

/*
================
WarpSet::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004d79c0
TGEventHandlerTable *WarpSet::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
WarpSet::WarpSet

The ID constructor of the stream loader: no trails yet.
================
*/
// FUNCTION: STBC 0x004d7a00 ??0WarpSet@@QAE@J@Z
WarpSet::WarpSet( long id ) : SetClass( id ) {
	numTrails = 0;
	trails = NULL;
}

/*
================
WarpSet::~WarpSet
================
*/
// FUNCTION: STBC 0x004d7a30
WarpSet::~WarpSet( void ) {
	DeleteTrails();
}

/*
================
WarpSet::Create
================
*/
// FUNCTION: STBC 0x004d7a80
WarpSet *WarpSet::Create( void ) {
	return new WarpSet;
}

// DATA: STBC 0x008e0e74
int WarpSet::defaultNumTrails = 400;

// The tunnel the trails fly through, in the set's frame (invented names):
// they spread across x and z, come from y = 100 and leave at y = -100.
#define TUNNEL_HALF_WIDTH		30.0f
#define TUNNEL_START			100.0f
#define TUNNEL_END				-100.0f
// The depth of the stretch a trail starts in (invented name).
#define TRAIL_START_DEPTH		10.0f
// The trails fly towards the camera at this many units a second (invented name).
#define TRAIL_SPEED				30.0f
// At most this many trails go back to the start in one update (invented name).
#define MAX_TRAILS_PLACED		50

/*
================
WarpSet::PlaceTrail
================
*/
// FUNCTION: STBC 0x004d7ad0
void WarpSet::PlaceTrail( int index, bool anywhere ) {
	float x = rand() * ( 2.0f * TUNNEL_HALF_WIDTH / RAND_MAX ) - TUNNEL_HALF_WIDTH;
	float y = rand() * ( 2.0f * TRAIL_START_DEPTH / RAND_MAX ) - TRAIL_START_DEPTH;
	if ( !anywhere ) {
		y += TUNNEL_START;
	} else {
		y += rand() * ( 2.0f * TUNNEL_START / RAND_MAX ) - TUNNEL_START;
	}
	float z = rand() * ( 2.0f * TUNNEL_HALF_WIDTH / RAND_MAX ) - TUNNEL_HALF_WIDTH;
	trails[index]->SetTranslate( NiPoint3( x, y, z ) );
}

/*
================
WarpSet::Update

The set's objects step; then, while someone sees the set, the trails fly
towards the camera by the game's frame step, and those past the tunnel's
end go back to its start. A set whose ships are all gone stops the trails
unless it is told to keep updating. The frame step is the real frame time
while the options window covers a paused game, or while forced.
================
*/
// FUNCTION: STBC 0x004d7b90
void WarpSet::Update( float time ) {
	float step = UtopiaApp::GetApp()->GetGameFrameStep();
	if ( TopWindow::GetTopWindow() != NULL ) {
		MainWindow *options = TopWindow::GetTopWindow()->FindMainWindow( MWT_OPTIONS );
		if ( ( options != NULL && UtopiaApp::GetApp()->IsGamePaused() && !options->IsCompletelyVisible() ) || forceUpdate ) {
			step = UtopiaApp::GetApp()->GetFrameTime();
		}
	}

	SetClass::Update( time );

	if ( !forceUpdate ) {
		// BUG: redundant: the ships are fetched and dropped
		objectTable.GetObjects( SHIPCLASS_TYPE );
		if ( objectTable.GetNumObjects( SHIPCLASS_TYPE ) == 0 ) {
			return;
		}
	}
	if ( GetViewingCamera() == NULL ) {
		return;
	}

	int placed = 0;
	for ( int i = 0; i < numTrails; i++ ) {
		NiPoint3 position = trails[i]->GetTranslate();
		position.y -= step * TRAIL_SPEED;
		trails[i]->SetTranslate( position );
		if ( !IsTrailInTunnel( i ) && placed < MAX_TRAILS_PLACED ) {
			PlaceTrail( i, false );
			placed++;
		}
	}
}

/*
================
WarpSet::IsTrailInTunnel
================
*/
// FUNCTION: STBC 0x004d7cd0
bool WarpSet::IsTrailInTunnel( int index ) {
	if ( trails[index]->GetTranslate().y > TUNNEL_END ) {
		return true;
	}
	return false;
}

/*
================
WarpSet::GetViewingCamera
================
*/
// FUNCTION: STBC 0x004d7d00
CameraObjectClass *WarpSet::GetViewingCamera( void ) {
	SetClass *rendered = g_kSetManager.GetRenderedSet();
	BridgeSet *bridge = BridgeSet::Cast( rendered );
	if ( bridge != NULL && bridge->GetViewScreen() != NULL ) {
		CameraObjectClass *camera = bridge->GetViewScreen()->GetRemoteCam();
		if ( camera != NULL && camera->GetContainingSet() == this ) {
			return camera;
		}
	}
	if ( rendered == this ) {
		return rendered->GetActiveCamera();
	}
	return NULL;
}

/*
================
WarpSet::SetNumTrails

Replaces the trails with count new ones, placed anywhere along the tunnel
or at its start, under the effect root.
================
*/
// FUNCTION: STBC 0x004d7d50
void WarpSet::SetNumTrails( int count, bool anywhere ) {
	DeleteTrails();
	numTrails = count;
	if ( count > 0 ) {
		trails = new WarpTrail *[count];
		for ( int i = 0; i < numTrails; i++ ) {
			trails[i] = new WarpTrail;
			PlaceTrail( i, anywhere );
			effectRoot->AttachChild( trails[i], false );
		}
	}
}

/*
================
WarpSet::DeleteTrails
================
*/
// FUNCTION: STBC 0x004d7e20
void WarpSet::DeleteTrails( void ) {
	if ( trails != NULL ) {
		for ( int i = 0; i < numTrails; i++ ) {
			NiNode *parent = trails[i]->GetParent();
			if ( parent != NULL ) {
				parent->DetachChild( trails[i] );
			}
		}
		delete [] trails;
		trails = NULL;
	}
}

/*
================
WarpSet::AddObjectToSet

A ship coming in is hidden at the set's origin, unrotated; the player's
ship brings a new tunnel of trails.
================
*/
// FUNCTION: STBC 0x004d7eb0
bool WarpSet::AddObjectToSet( ObjectClass *object, char *name_ ) {
	ShipClass *ship = ShipClass::Cast( object );
	if ( ship != NULL ) {
		ship->SetHidden( true );
		ship->SetTranslate( &NiPoint3::ZERO );
		ship->SetMatrixRotation( NiMatrix3::IDENTITY );
		if ( ship == Game::GetCurrentPlayer() ) {
			SetNumTrails( numTrails, true );
		}
	}
	return SetClass::AddObjectToSet( object, name_ );
}

/*
================
WarpSet::RemoveObjectFromSet

A ship leaving is shown again, unless it is cloaked and not the player's.
================
*/
// FUNCTION: STBC 0x004d7f40
ObjectClass *WarpSet::RemoveObjectFromSet( long id ) {
	ShipClass *ship = ShipClass::Cast( GetObjectByID( id ) );
	if ( ship != NULL ) {
		if ( !ship->IsCloaked() || ( ship->IsCloaked() == true && ship == Game::GetCurrentPlayer() ) ) {
			ship->GetNode()->SetAppCulled( false );
		}
	}
	return SetClass::RemoveObjectFromSet( id );
}

/*
================
WarpSet::WriteToStream
================
*/
// FUNCTION: STBC 0x004d7fa0
bool WarpSet::WriteToStream( TGStream *stream ) {
	if ( !SetClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteInt( numTrails );
	return true;
}

/*
================
WarpSet::ReadFromStream
================
*/
// FUNCTION: STBC 0x004d7fd0
bool WarpSet::ReadFromStream( TGStream *stream ) {
	if ( !SetClass::ReadFromStream( stream ) ) {
		return false;
	}
	if ( trails != NULL ) {
		DeleteTrails();
	}
	trails = NULL;
	numTrails = 0;
	forceUpdate = false;
	int count = stream->ReadInt();
	SetNumTrails( count, true );
	return true;
}
