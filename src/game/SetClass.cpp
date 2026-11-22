/*
===============================================================================

	SetClass.cpp

===============================================================================
*/

#include <iostream>
#include "SetClass.h"
#include "TGTrace.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "ObjectClass.h"
#include "CameraObjectClass.h"
#include "LightObjectClass.h"
#include "TGEventHandlerTable.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGModelManager.h"
#include "ProximityManager.h"
#include "ProximityCheck.h"
#include "Nebula.h"
#include "StarSphere.h"
#include "Backdrop.h"
#include "TGStream.h"
#include "TGLocalizationManager.h"
#include "TGLocalizationDatabase.h"
#include "UtopiaApp.h"
#include "BridgeSet.h"
#include "SetManager.h"
#include "ViewScreenObject.h"
#include "TGIconManager.h"
#include "NiCamera.h"
#include "ImageManagerClass.h"
#include "TGSoundRegion.h"
#include "TGSoundManager.h"
#include "TGSound.h"
#include "ImpulseEngineSubsystem.h"
#include "ImpulseEngineProperty.h"
#include "DamageableObject.h"
#include "PlacementObject.h"
#include "Torpedo.h"
#include "TGSystemWrapperClass.h"
#include "NiZBufferProperty.h"
#include "NiAlphaProperty.h"
#include "NiDitherProperty.h"
#include "ShipClass.h"
#include "TGStringEvent.h"
#include "NiTList.h"
#include "CameraMode.h"
#include "WeaponSystem.h"
#include "PhaserSystem.h"
#include "TractorBeamSystem.h"
#include "PyEmbed.h"
#include "TGPointerMap.h"
#include <math.h>
#include "Glow.h"
#include "renderer/NiDDImage.h"
#include "TGMovieManager.h"
#include <stdlib.h>
#include "NiMath.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0040c9f0 _$E32
// FUNCTION: STBC 0x0040ca00 _$E29
// FUNCTION: STBC 0x0040ca10 _$E31
// FUNCTION: STBC 0x0040ca20 _$E30
// FUNCTION: STBC 0x0040ca30 _$E38
// FUNCTION: STBC 0x0040ca40 _$E35
// FUNCTION: STBC 0x0040ca50 _$E37
// FUNCTION: STBC 0x0040ca60 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0040ca70 _$E66
// FUNCTION: STBC 0x0040ca80 _$E65
// FUNCTION: STBC 0x0040ca90 _$E230
// FUNCTION: STBC 0x0040caa0 _$E229
// FUNCTION: STBC 0x0040cab0 _$E77
// FUNCTION: STBC 0x0040cac0 _$E76
// FUNCTION: STBC 0x0040cad0 _$E81
// FUNCTION: STBC 0x0040cae0 _$E80
// FUNCTION: STBC 0x0040caf0 _$E85
// FUNCTION: STBC 0x0040cb00 _$E84
// FUNCTION: STBC 0x0040cb10 _$E89
// FUNCTION: STBC 0x0040cb20 _$E88
// FUNCTION: STBC 0x0040cb30 _$E206
// FUNCTION: STBC 0x0040cb40 _$E205
// FUNCTION: STBC 0x0040cb50 _$E148
// FUNCTION: STBC 0x0040cb60 _$E147
// FUNCTION: STBC 0x0040cb70 _$E161
// FUNCTION: STBC 0x0040cb80 _$E160
// FUNCTION: STBC 0x0040cb90 _$E211
// FUNCTION: STBC 0x0040cba0 _$E210
// FUNCTION: STBC 0x0040cbb0 _$E235
// FUNCTION: STBC 0x0040cbc0 _$E234
// FUNCTION: STBC 0x0040cbd0 _$E215
// FUNCTION: STBC 0x0040cbe0 _$E214
// FUNCTION: STBC 0x0040cbf0 _$E239
// FUNCTION: STBC 0x0040cc00 _$E238
// FUNCTION: STBC 0x0040cc10 _$E246
// FUNCTION: STBC 0x0040cc20 _$E245
// FUNCTION: STBC 0x0040cd40 _$E268
// FUNCTION: STBC 0x0040cd50 _$E267
// FUNCTION: STBC 0x0040d000 _$E272
// FUNCTION: STBC 0x0040d010 _$E271
// FUNCTION: STBC 0x0040d130 _$E106
// FUNCTION: STBC 0x0040d140 _$E105
// FUNCTION: STBC 0x0040dbb0 _$E254
// FUNCTION: STBC 0x0040dbc0 _$E253
// FUNCTION: STBC 0x0040df60 _$E261
// FUNCTION: STBC 0x0040df70 _$E260
// FUNCTION: STBC 0x0040e510 _$E284
// FUNCTION: STBC 0x0040e520 _$E283
// FUNCTION: STBC 0x0040e7f0 _$E288
// FUNCTION: STBC 0x0040e800 _$E287
// FUNCTION: STBC 0x0040ec70 _$E328
// FUNCTION: STBC 0x0040ec80 _$E327
// FUNCTION: STBC 0x0040f050 _$E334
// FUNCTION: STBC 0x0040f060 _$E333
// FUNCTION: STBC 0x0040f690 _$E276
// FUNCTION: STBC 0x0040f6a0 _$E275
// FUNCTION: STBC 0x0040f820 _$E280
// FUNCTION: STBC 0x0040f830 _$E279
// FUNCTION: STBC 0x0040f8a0 _$E292
// FUNCTION: STBC 0x0040f8b0 _$E291
// FUNCTION: STBC 0x0040fb50 _$E296
// FUNCTION: STBC 0x0040fb60 _$E295
// FUNCTION: STBC 0x0040fcb0 _$E300
// FUNCTION: STBC 0x0040fcc0 _$E299
// FUNCTION: STBC 0x0040fe60 _$E304
// FUNCTION: STBC 0x0040fe70 _$E303
// FUNCTION: STBC 0x0040ff80 _$E308
// FUNCTION: STBC 0x0040ff90 _$E307
// FUNCTION: STBC 0x00410350 _$E312
// FUNCTION: STBC 0x00410360 _$E311
// FUNCTION: STBC 0x00410370 _$E316
// FUNCTION: STBC 0x00410380 _$E315
// FUNCTION: STBC 0x004106a0 _$E320
// FUNCTION: STBC 0x004106b0 _$E319
// FUNCTION: STBC 0x004107b0 _$E111
// FUNCTION: STBC 0x004107c0 _$E110
// FUNCTION: STBC 0x004126a0 _$E324
// FUNCTION: STBC 0x004128a0 _$E561
// FUNCTION: STBC 0x004126b0 _$E323
// FUNCTION: STBC 0x00412700 _$E116
// FUNCTION: STBC 0x00412710 _$E115
// FUNCTION: STBC 0x00412730 _$E338
// FUNCTION: STBC 0x00412740 _$E337
// FUNCTION: STBC 0x00412750 _$E342
// FUNCTION: STBC 0x00412760 _$E341
// FUNCTION: STBC 0x00412770 _$E346
// FUNCTION: STBC 0x00412780 _$E345
// FUNCTION: STBC 0x00412790 _$E350
// FUNCTION: STBC 0x004127a0 _$E349
// FUNCTION: STBC 0x004127f0 _$E125
// FUNCTION: STBC 0x00412800 _$E124
// FUNCTION: STBC 0x00412820 _$E354
// FUNCTION: STBC 0x00412830 _$E353
// FUNCTION: STBC 0x00412840 _$E358
// FUNCTION: STBC 0x00412850 _$E357
// FUNCTION: STBC 0x00412860 _$E362
// FUNCTION: STBC 0x00412870 _$E361
// FUNCTION: STBC 0x00412880 _$E131
// FUNCTION: STBC 0x00412890 _$E130
// FUNCTION: STBC 0x00412af0 _$E136
// FUNCTION: STBC 0x00412b00 _$E135
// FUNCTION: STBC 0x00412fb0 _$E166
// FUNCTION: STBC 0x00412fc0 _$E165
// FUNCTION: STBC 0x00414e70 _$E366
// FUNCTION: STBC 0x00414e80 _$E365
// FUNCTION: STBC 0x00415a90 _$E171
// FUNCTION: STBC 0x00415aa0 _$E170
// FUNCTION: STBC 0x00415ab0 _$E370
// FUNCTION: STBC 0x00415ac0 _$E369
// FUNCTION: STBC 0x00415c40 _$E374
// FUNCTION: STBC 0x00415c50 _$E373
// FUNCTION: STBC 0x00415eb0 _$E176
// FUNCTION: STBC 0x00415ec0 _$E175
// FUNCTION: STBC 0x00415ed0 _$E378
// FUNCTION: STBC 0x00415ee0 _$E377
// FUNCTION: STBC 0x00416120 _$E141
// FUNCTION: STBC 0x00416130 _$E140
// FUNCTION: STBC 0x004162e0 _$E382
// FUNCTION: STBC 0x004162f0 _$E381
// FUNCTION: STBC 0x00416300 _$E386
// FUNCTION: STBC 0x00416310 _$E385
// FUNCTION: STBC 0x00416320 _$E181
// FUNCTION: STBC 0x00416330 _$E180
// FUNCTION: STBC 0x00416340 _$E186
// FUNCTION: STBC 0x00416350 _$E185
// FUNCTION: STBC 0x00416360 _$E390
// FUNCTION: STBC 0x00416370 _$E389
// FUNCTION: STBC 0x00416380 _$E394
// FUNCTION: STBC 0x00416390 _$E393
// FUNCTION: STBC 0x004163a0 _$E398
// FUNCTION: STBC 0x004163b0 _$E397
// FUNCTION: STBC 0x004163c0 _$E402
// FUNCTION: STBC 0x004163d0 _$E401
// FUNCTION: STBC 0x004163e0 _$E406
// FUNCTION: STBC 0x004163f0 _$E405
// FUNCTION: STBC 0x00416400 _$E410
// FUNCTION: STBC 0x00416410 _$E409
// FUNCTION: STBC 0x00416420 _$E414
// FUNCTION: STBC 0x00416430 _$E413
// FUNCTION: STBC 0x00416440 _$E418
// FUNCTION: STBC 0x00416450 _$E417
// FUNCTION: STBC 0x00416460 _$E191
// FUNCTION: STBC 0x00416470 _$E190
// FUNCTION: STBC 0x00416480 _$E422
// FUNCTION: STBC 0x00416490 _$E421
// FUNCTION: STBC 0x004164e0 _$E196
// FUNCTION: STBC 0x004164f0 _$E195
// FUNCTION: STBC 0x00416550 _$E201
// FUNCTION: STBC 0x00416560 _$E200
// FUNCTION: STBC 0x004174d0 _$E426
// FUNCTION: STBC 0x004174e0 _$E425
// FUNCTION: STBC 0x004174f0 _$E431
// FUNCTION: STBC 0x00417500 _$E430
// FUNCTION: STBC 0x00417510 _$E435
// FUNCTION: STBC 0x00417520 _$E434

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0040cc30 _$E550
// FUNCTION: STBC 0x0040cc40 _$E546
// FUNCTION: STBC 0x0040cc50 _$E549
// FUNCTION: STBC 0x0040cc60 _$E547
// DATA: STBC 0x0097e7c0
TGEventHandlerTable SetClass::classHandlers;

// unknown108's first value (invented name)
static const float UNKNOWN108_DEFAULT = 200.0f;

// How far from the camera a ship's engine sound is heard (invented name)
static const float ENGINE_SOUND_RANGE = 35.0f;

/*
================
SetClass::Cast

The object as a SetClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0040cc80
SetClass *SetClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( SETCLASS_TYPE ) ) {
		return (SetClass *)object;
	}
	return NULL;
}

// FUNCTION: STBC 0x0040ccb0 _$E555
// FUNCTION: STBC 0x0040ccc0 _$E552
// FUNCTION: STBC 0x0040ccd0 _$E554
// FUNCTION: STBC 0x0040cce0 _$E553
// DATA: STBC 0x0097e91c
// The glow while it is enabled and the card can draw it. A file-scope static:
// its exit destructor (0x40CCE0) has no guard, which a class static's has.
NiPointer<GlowRenderer> glowRenderer;

// DATA: STBC 0x0097e990
bool SetClass::glowChecked;

// DATA: STBC 0x0097e991
bool SetClass::glowSupported;

// DATA: STBC 0x0097e998
bool SetClass::glowEnabled;

// DATA: STBC 0x008d8b30
bool SetClass::splitLargeZRenders = true;

// The nearest an engine sound is at full volume: an eighth of its range (invented name).
// FUNCTION: STBC 0x0040cd10 _$E557
// FUNCTION: STBC 0x0040cd00 _$E558
// DATA: STBC 0x0097e780
static float engineSoundMinDistance = ENGINE_SOUND_RANGE * 0.125f;

/*
================
SetClass::ShutdownGlow

Releases the glow at shutdown.
================
*/
// FUNCTION: STBC 0x0040cd30
void SetClass::ShutdownGlow( void ) {
	ResetGlow( NULL );
}

/*
================
SetClass::ResetGlow

Releases the glow renderer and forgets whether the card can glow. Given a
renderer, glow that is enabled is made again for it.
================
*/
// FUNCTION: STBC 0x0040cd60
void SetClass::ResetGlow( NiRender *renderer ) {
	glowRenderer = NULL;
	glowChecked = false;
	if ( renderer != NULL && glowEnabled ) {
		SetGlowEnabled( true );
	}
}

/*
================
SetClass::CanGlowBeEnabled

Whether the card can draw the enhanced glow: the card's script
(MainMenu.CardSpecificSetup's CanDoEnhancedGlows) says so and a glow can be
made for the renderer. The answer is kept until the next ResetGlow; without
a renderer or its device the answer is no, and asked again next time.
================
*/
// FUNCTION: STBC 0x0040cdb0
bool SetClass::CanGlowBeEnabled( void ) {
	if ( !glowChecked ) {
		int canDoGlows = 1;
		NiRender *renderer = UtopiaApp::GetRenderer();
		if ( renderer != NULL ) {
			Ni3DDeviceDesc *device = ( (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer() )->Unknown7C2EE0();
			if ( device != NULL && device->GetName() != NULL ) {
				PyEmbed::CallFunction( "MainMenu.CardSpecificSetup", "CanDoEnhancedGlows", "i", &canDoGlows, "(si)", device->GetName(),
									   UtopiaApp::GetRenderer()->VirtualSlot21() > 16 );
				if ( canDoGlows == 0 ) {
					glowSupported = false;
				} else {
					glowRenderer = GlowRenderer::Create( UtopiaApp::GetRenderer() );
					glowSupported = glowRenderer != NULL;
					glowRenderer = NULL;
				}
				glowChecked = true;
				return glowSupported;
			}
		}
		return false;
	}
	return glowSupported;
}

/*
================
SetClass::SetGlowEnabled

Turns the enhanced glow on or off. Turning it on makes the glow when the
card can draw it and there is none yet; turning it off releases it.
================
*/
// FUNCTION: STBC 0x0040d020
void SetClass::SetGlowEnabled( bool enabled ) {
	if ( CanGlowBeEnabled() ) {
		if ( enabled == true ) {
			if ( !glowEnabled || glowRenderer == NULL ) {
				glowRenderer = GlowRenderer::Create( UtopiaApp::GetRenderer() );
			}
		} else if ( !enabled ) {
			glowRenderer = NULL;
		}
	}
	glowEnabled = enabled;
}

/*
================
SetClass::SetClass

Builds the scene graph: under the top node the scene, effect, backdrop
and foreground roots, with the depth-buffer, alpha, dither, fog and
wireframe properties the roots draw with; and the pick the set picks
objects with.
================
*/
// FUNCTION: STBC 0x0040d150
SetClass::SetClass( long id ) : TGEventHandlerObject( id ) {
	objectsLocked = false;
	name = NULL;
	regionModule = NULL;
	unknown10C = true;
	unknown90 = 0.001f;
	// The random number is unsigned in retail (GetRandomNumber divides
	// unsigned); the shared declaration says int.
	unknown94 = (float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 100 ) * 0.01f;
	unknown60 = true;

	unknown98 = new NiNode;
	unknown9C = new NiNode;
	rendered = false;
	sceneRoot = new NiNode;
	effectRoot = new NiNode;
	backdropRoot = new NiNode;
	foregroundRoot = new NiNode;
	unknown98->AttachChild( backdropRoot, false );
	unknown98->AttachChild( sceneRoot, false );
	unknown98->AttachChild( effectRoot, false );
	unknown98->AttachChild( foregroundRoot, false );

	// the backdrops and the foreground draw without the depth buffer
	NiZBufferProperty *noDepth = new NiZBufferProperty;
	noDepth->SetZBufferTest( false );
	noDepth->SetZBufferWrite( false );
	backdropRoot->AttachProperty( noDepth );
	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetNoSorter( true );
	backdropRoot->AttachProperty( alpha );
	backdropRoot->SetAppCulled( true );
	foregroundRoot->AttachProperty( noDepth );

	NiDitherProperty *dither = new NiDitherProperty;
	dither->SetDithering( true );
	unknown98->AttachProperty( dither );

	NiZBufferProperty *depth = new NiZBufferProperty;
	depth->SetZBufferTest( true );
	depth->SetZBufferWrite( true );
	sceneRoot->AttachProperty( depth );

	// the effects test the depth buffer without writing it
	NiZBufferProperty *effectDepth = new NiZBufferProperty;
	effectDepth->SetZBufferTest( true );
	effectDepth->SetZBufferWrite( false );
	effectRoot->AttachProperty( effectDepth );

	fog = new NiFogProperty;
	fog->SetFog( false );
	fog->SetFogFunction( NiFogProperty::FOG_Z_LINEAR );
	fog->SetDepth( 1.0f );
	sceneRoot->AttachProperty( fog );

	wireframe = new NiWireframeProperty;
	wireframe->SetWireframe( false );
	unknown98->AttachProperty( wireframe );
	unknown98->UpdateProperties();

	pick = new NiPick;
	pick->SetPickType( NiPick::FIND_FIRST );
	pick->SetSortType( NiPick::NO_SORT );
	pick->SetIntersectType( NiPick::BOUND_INTERSECT );
	pick->SetFrontOnly( true );
	pick->SetReturnTexture( false );
	pick->SetReturnNormal( false );
	pick->SetReturnColor( false );
	pickOrigin = NiPoint3::ZERO;
	pickDirection = NiPoint3::ZERO;
	pickIndex = 0;
	unknownD8 = false;

	fogColor = NiColor::BLACK;
	starSphere = NULL;
	nebula = NULL;
	sceneChanged = true;
	unknownF1 = true;
	proximityManager = NULL;
	numDistanceModifiers = 0;
	distanceModifiers = NULL;
	backgroundModel = NULL;
	glowIntensity = 0.7f;
	unknown108 = UNKNOWN108_DEFAULT;
	numSounds = 0;
	memset( sounds, 0, sizeof( sounds ) );
}

/*
================
SetClass::GetObjType
================
*/
// FUNCTION: STBC 0x0040d970
int SetClass::GetObjType( void ) {
	return SETCLASS_TYPE;
}

/*
================
SetClass::GetClassName
================
*/
// FUNCTION: STBC 0x0040d9c0
char *SetClass::GetClassName( void ) {
	return "SetClass";
}

/*
================
SetClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0040d9d0
char *SetClass::GetSwigTypeName( void ) {
	return "_p_SetClass";
}

/*
================
SetClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0040d9e0
char *SetClass::GetPythonClassName( void ) {
	return "SetClassPtr";
}

/*
================
SetClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0040d9f0
TGEventHandlerTable *SetClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
SetClass::Compare

The order of the sets by name.
================
*/
// FUNCTION: STBC 0x0040da00
int SetClass::Compare( SetClass *other ) {
	return CompareKey( other->GetName() );
}

/*
================
SetClass::CompareKey

Zero when the set has the name; positive when the name sorts after the
set's, negative when before.
================
*/
// FUNCTION: STBC 0x0040da10
int SetClass::CompareKey( char *name_ ) {
	return strcmp( name_, name );
}

/*
================
SetClass::~SetClass

Deletes the objects and the cameras, the set's sound region, its pick,
proximity manager and distance modifiers, and lets go of the scene graph.
================
*/
// FUNCTION: STBC 0x0040dc00
SetClass::~SetClass( void ) {
	if ( backgroundModel != NULL ) {
		RemoveBackgroundModel();
	}
	TGSoundRegion *region = TGSoundRegion::GetRegion( name );
	if ( region != NULL ) {
		delete region;
	}

	int count = objects.GetCount();
	ObjectClass **items = objects.GetItems();
	LockObjects();
	while ( --count >= 0 ) {
		DeleteObjectFromSet( items[count]->GetObjID() );
	}
	UnlockObjects();
	count = cameras.GetCount();
	CameraObjectClass **cameraItems = cameras.GetItems();
	for ( count--; count >= 0; count-- ) {
		DeleteCameraFromSet( cameraItems[count]->GetName() );
	}

	SetName( NULL );
	SetRegionModule( NULL );
	unknown98 = NULL;
	unknown9C = NULL;
	wireframe = NULL;
	fog = NULL;
	delete pick;
	if ( proximityManager != NULL ) {
		delete proximityManager;
	}
	if ( distanceModifiers != NULL ) {
		for ( int i = 0; i < numDistanceModifiers; i++ ) {
			delete distanceModifiers[i];
		}
		delete [] distanceModifiers;
		distanceModifiers = NULL;
	}
	rendered = false;
	Unknown413CB0();
}

/*
================
SetClass::SetName

Keeps a copy of the name, and makes the display name from it.
================
*/
// FUNCTION: STBC 0x0040df80
void SetClass::SetName( char *name_ ) {
	if ( name != NULL ) {
		if ( name == name_ ) {
			return;
		}
		delete [] name;
		name = NULL;
	}
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
		displayName = MakeDisplayName( name );
	} else {
		displayName.SetString( "" );
	}
}

/*
================
SetClass::MakeDisplayName

The set name's entry in the systems database. A name without one is
looked up with its trailing digits and spaces cut off, and those are
appended after a space to what is found ("Vesuvi 4" from "Vesuvi4"); a
name without either entry is its own display name.
================
*/
// FUNCTION: STBC 0x0040e060
TGString SetClass::MakeDisplayName( char *name_ ) {
	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Systems.TGL", true );
	TGString displayName;
	if ( database->HasString( name_ ) ) {
		displayName = *database->GetString( name_ );
	} else {
		char *baseName = new char[strlen( name_ ) + 1];
		strcpy( baseName, name_ );
		// BUG: the cut starts at the terminator, which strchr finds as well
		for ( int i = strlen( baseName ); i >= 0 && strchr( "0123456789 ", baseName[i] ) != NULL; i-- ) {
			baseName[i] = '\0';
		}
		if ( database->HasString( baseName ) ) {
			displayName = *database->GetString( baseName );
			char *suffix = name_ + strlen( baseName );
			if ( *suffix != '\0' ) {
				TGString number( suffix );
				displayName.Unknown6F4970( &( TGString( " " ) + number ) );
			}
		} else {
			displayName.AssignC( name_ );
		}
		delete baseName;
	}
	g_kLocalizationManager.Unload( database );
	return displayName;
}

/*
================
SetClass::SetRegionModule

Keeps a copy of the name of the region's script module.
================
*/
// FUNCTION: STBC 0x0040e3b0
void SetClass::SetRegionModule( char *module ) {
	if ( regionModule != NULL ) {
		delete [] regionModule;
		regionModule = NULL;
	}
	if ( module != NULL ) {
		regionModule = new char[strlen( module ) + 1];
		strcpy( regionModule, module );
	}
}

/*
================
SetClass::SetProximityManagerActive

Creates the set's proximity manager, with the set's objects in it, or
deletes it.
================
*/
// FUNCTION: STBC 0x0040e420
void SetClass::SetProximityManagerActive( bool active ) {
	if ( active ) {
		if ( proximityManager == NULL ) {
			proximityManager = new ProximityManager;
			proximityManager->set = this;
			ObjectClass **items = objects.GetItems();
			for ( int i = 0; i < objects.GetCount(); i++ ) {
				proximityManager->AddObject( items[i] );
			}
		}
	} else if ( proximityManager != NULL ) {
		delete proximityManager;
		proximityManager = NULL;
	}
}

/*
================
SetClass::LockObjects

From now on the objects added wait until UnlockObjects.
================
*/
// FUNCTION: STBC 0x0040e500
void SetClass::LockObjects( void ) {
	if ( !objectsLocked ) {
		objectsLocked = true;
	}
}

/*
================
SetClass::UnlockObjects

Moves the objects added while the objects were locked into the set.
================
*/
// FUNCTION: STBC 0x0040e530
void SetClass::UnlockObjects( void ) {
	if ( objectsLocked ) {
		objectsLocked = false;
		int count = pendingObjects.GetCount();
		ObjectClass **items = pendingObjects.GetItems();
		int i;
		for ( i = 0; i < count; i++ ) {
			objects.Insert( items[i] );
			objectTable.AddObject( items[i], this );
		}
		for ( i = count - 1; i >= 0; i-- ) {
			pendingObjects.RemoveAt( i );
		}
	}
}

/*
================
SetClass::Unknown40E6A0

The backdrop's node changes places with the one before it among the
backdrop root's children.
================
*/
// FUNCTION: STBC 0x0040e6a0
void SetClass::Unknown40E6A0( Backdrop *backdrop ) {
	NiNode *node = backdrop->GetNode();
	if ( node != NULL ) {
		NiNode *parent = node->GetParent();
		for ( unsigned int i = 1; i < parent->GetChildCount(); i++ ) {
			if ( parent->GetAt( i ) == node ) {
				NiAVObject *previous = parent->GetAt( i - 1 );
				parent->SetAt( i - 1, node );
				parent->SetAt( i, previous );
				return;
			}
		}
	}
}

/*
================
SetClass::Unknown40E810

The backdrop's node changes places with the one after it among the
backdrop root's children.
================
*/
// FUNCTION: STBC 0x0040e810
void SetClass::Unknown40E810( Backdrop *backdrop ) {
	NiNode *node = backdrop->GetNode();
	if ( node != NULL ) {
		NiNode *parent = node->GetParent();
		for ( unsigned int i = 0; i < parent->GetChildCount() - 1; i++ ) {
			if ( parent->GetAt( i ) == node ) {
				NiAVObject *next = parent->GetAt( i + 1 );
				parent->SetAt( i + 1, node );
				parent->SetAt( i, next );
				return;
			}
		}
	}
}

/*
================
SetClass::AddBackdropToSet

Names the backdrop, files it by name and ID and attaches it under the
backdrop root; a star sphere becomes the set's. False when the name is
taken.
================
*/
// FUNCTION: STBC 0x0040e980
bool SetClass::AddBackdropToSet( Backdrop *backdrop, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	backdrop->SetName( name_ );
	if ( name_ != NULL ) {
		objectIDs.SetAt( name_, backdrop->GetObjID() );
	}
	if ( objectsLocked ) {
		pendingObjects.Insert( backdrop );
	} else {
		objects.Insert( backdrop );
		objectTable.AddObject( backdrop, this );
	}
	backdrop->SetContainingSet( this );
	backdropRoot->SetAppCulled( false );
	backdropRoot->AttachChild( backdrop->GetNode(), false );
	backdropRoot->Update( 0.0f );
	if ( StarSphere::Cast( backdrop ) != NULL ) {
		starSphere = StarSphere::Cast( backdrop );
	}
	sceneChanged = true;
	return true;
}

/*
================
SetClass::AddObjectToSet

Names the object and adds it to the set, unless another object has the
name; while the objects are locked it waits among the pending objects.
Sends ET_TORPEDO_ENTERED_SET for a torpedo, ET_ENTERED_SET for anything
else.
================
*/
// FUNCTION: STBC 0x0040ec90
bool SetClass::AddObjectToSet( ObjectClass *object, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	object->SetName( name_ );
	if ( name_ != NULL ) {
		objectIDs.SetAt( name_, object->GetObjID() );
	}
	if ( objectsLocked ) {
		pendingObjects.Insert( object );
	} else {
		objects.Insert( object );
		objectTable.AddObject( object, this );
	}
	if ( Nebula::Cast( object ) != NULL ) {
		nebula = Nebula::Cast( object );
	}
	object->SetContainingSet( this );
	sceneChanged = true;
	object->GetNode()->UpdateProperties();
	object->GetNode()->Update( UtopiaApp::GetApp()->GetGameTime() );
	if ( proximityManager != NULL ) {
		proximityManager->AddObject( object );
	}

	TGEvent *event;
	if ( object->IsTypeOf( TORPEDO_TYPE ) ) {
		event = new TGEvent;
		event->SetEventType( ET_TORPEDO_ENTERED_SET );
		event->SetDestination( object );
	} else {
		event = new TGEvent;
		event->SetEventType( ET_ENTERED_SET );
		event->SetDestination( object );
		// an object without a LOD model is not saved with the event
		if ( object->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) && ( (DamageableObject *)object )->GetLODModelName() == NULL ) {
			event->SetNotSaved( true );
		}
	}
	g_kEventManager.AddEvent( event );
	return true;
}

/*
================
SetClass::RemoveObjectFromSet

Takes the object with the ID out of the set, its node out of the scene
graph and out of the lights' illuminated objects, and sends
ET_TORPEDO_EXITED_SET for a torpedo, ET_EXITED_SET for anything else,
with the set's name. The object is not deleted; NULL when the set has no
object with the ID.
================
*/
// FUNCTION: STBC 0x0040f070 ?RemoveObjectFromSet@SetClass@@UAEPAVObjectClass@@J@Z
ObjectClass *SetClass::RemoveObjectFromSet( long id ) {
	ObjectClass *object = GetObjectByID( id );
	if ( object != NULL ) {
		TGStringEvent *event;
		if ( object->IsTypeOf( TORPEDO_TYPE ) ) {
			event = new TGStringEvent;
			event->SetEventType( ET_TORPEDO_EXITED_SET );
			event->SetDestination( object );
			event->SetString( name );
		} else {
			event = new TGStringEvent;
			event->SetEventType( ET_EXITED_SET );
			event->SetDestination( object );
			event->SetString( name );
		}
		g_kEventManager.AddEvent( event );

		if ( object->GetName() != NULL ) {
			objectIDs.RemoveAt( object->GetName() );
		}
		NiNode *node = object->GetNode();
		if ( node != NULL ) {
			if ( node->GetParent() == sceneRoot ) {
				sceneRoot->DetachChild( node );
			} else if ( node->GetParent() == backdropRoot ) {
				backdropRoot->DetachChild( node );
				if ( backdropRoot->GetChildCount() == 0 ) {
					backdropRoot->SetAppCulled( true );
				}
			}
			sceneChanged = true;
		}
		if ( object == starSphere ) {
			starSphere = NULL;
		}
		if ( object == nebula ) {
			nebula = NULL;
		}
		object->SetContainingSet( NULL );
		if ( objectsLocked ) {
			pendingObjects.Remove( id );
		}
		objects.Remove( id );
		objectTable.RemoveObject( object, this );
		if ( proximityManager != NULL ) {
			proximityManager->RemoveObject( object );
		}

		int count = lights.GetCount();
		LightObjectClass **lightItems = lights.GetItems();
		for ( int i = 0; i < count; i++ ) {
			if ( lightItems[i] != NULL ) {
				lightItems[i]->RemoveIlluminatedObject( object );
			}
		}
	}
	return object;
}

/*
================
SetClass::RemoveObjectFromSet

Takes the object with the name out of the set; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0040f400 ?RemoveObjectFromSet@SetClass@@QAEPAVObjectClass@@PAD@Z
ObjectClass *SetClass::RemoveObjectFromSet( char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return RemoveObjectFromSet( id );
		}
	}
	return NULL;
}

/*
================
SetClass::RemoveBackdropFromSet

Takes the backdrop out of the set without deleting it; the backdrop root
is culled once it has no children. False when the backdrop is not in the
set.
================
*/
// FUNCTION: STBC 0x0040f460
bool SetClass::RemoveBackdropFromSet( Backdrop *backdrop ) {
	if ( backdrop == NULL || backdrop->GetContainingSet() != this ) {
		return false;
	}
	char *name_ = backdrop->GetName();
	if ( name_ != NULL ) {
		objectIDs.RemoveAt( name_ );
	}
	if ( backdrop->GetNode() != NULL ) {
		backdropRoot->DetachChild( backdrop->GetNode() );
		if ( backdropRoot->GetChildCount() == 0 ) {
			backdropRoot->SetAppCulled( true );
		}
		sceneChanged = true;
	}
	if ( backdrop == starSphere ) {
		starSphere = NULL;
	}
	backdrop->SetContainingSet( NULL );
	if ( objectsLocked ) {
		pendingObjects.Remove( backdrop->GetObjID() );
	}
	objects.Remove( backdrop->GetObjID() );
	objectTable.RemoveObject( backdrop, this );
	return true;
}

// The texture directories for the image details, by ImageManagerClass
// detail (invented name).
static char *detailDirectories[] = { "Low/", "Medium/", "High/" };

/*
================
GetDetailTexturePath

The directory of the model file with the directory of the image detail
appended: where the model's textures are. The path is a static buffer
(invented name).
================
*/
// FUNCTION: STBC 0x0040f6b0
char *GetDetailTexturePath( char *file ) {
	static char path[MAX_PATH];
	strcpy( path, file );
	char *directory = detailDirectories[g_kImageManager.GetImageDetail()];
	strrchr( path, '/' )[1] = '\0';
	strcat( path, directory );
	return path;
}

/*
================
SetClass::SetBackgroundModel

Loads the model file, unless it is loaded, and adds it to the set at the
position as the background model, in place of the old one; false when
the old one cannot be removed or the model cannot be loaded.
================
*/
// FUNCTION: STBC 0x0040f730
bool SetClass::SetBackgroundModel( char *file, float x, float y, float z ) {
	if ( backgroundModel != NULL && !RemoveBackgroundModel() ) {
		return false;
	}
	char *texturePath = GetDetailTexturePath( file );
	if ( !g_kModelManager.IsModelLoaded( file ) ) {
		if ( !g_kModelManager.LoadModel( file, NULL, texturePath ) ) {
			return false;
		}
	} else {
		g_kModelManager.Refer( file );
	}
	ObjectClass *object = ObjectClass::Create( g_kModelManager.CloneModel( file ) );
	object->SetTranslateXYZ( x, y, z );
	AddObjectToSet( object, "BackgroundModel" );
	backgroundModel = new char[strlen( file ) + 1];
	strcpy( backgroundModel, file );
	unknown10C = false;
	glowIntensity = 0.0f;
	return true;
}

/*
================
SetClass::RemoveBackgroundModel

Releases the background model and deletes its object; false when there
is none.
================
*/
// FUNCTION: STBC 0x0040f840
bool SetClass::RemoveBackgroundModel( void ) {
	if ( backgroundModel == NULL ) {
		return false;
	}
	g_kModelManager.Unrefer( backgroundModel );
	delete backgroundModel;
	backgroundModel = NULL;
	if ( GetObject( "BackgroundModel" ) == NULL ) {
		return false;
	}
	DeleteObjectFromSet( "BackgroundModel" );
	return true;
}

/*
================
SetClass::DeleteObjectFromSet

Takes the object with the ID out of the set and has it delete itself
through the event queue.
================
*/
// FUNCTION: STBC 0x0040f8c0 ?DeleteObjectFromSet@SetClass@@UAEXJ@Z
void SetClass::DeleteObjectFromSet( long id ) {
	ObjectClass *object = RemoveObjectFromSet( id );
	TGEvent *event = new TGEvent;
	event->SetEventType( ET_DELETE_ME_STAGE_1 );
	event->SetDestination( object );
	g_kEventManager.AddEvent( event );
}

/*
================
SetClass::DeleteObjectFromSet

Deletes the object with the name, when there is one.
================
*/
// FUNCTION: STBC 0x0040f920 ?DeleteObjectFromSet@SetClass@@QAEXPAD@Z
void SetClass::DeleteObjectFromSet( char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			DeleteObjectFromSet( id );
		}
	}
}

/*
================
SetClass::GetTargetableObjects

A new array of the set's targetable objects, and their count: with
targeter, those it can target; with aliveOnly, none that is dead or dying.
The caller frees the array.
================
*/
// FUNCTION: STBC 0x0040f980
ObjectClass **SetClass::GetTargetableObjects( int *count, ShipClass *targeter, bool aliveOnly ) {
	*count = 0;
	int numObjects = 0;
	ObjectClass **all = ObjectClass::GetClassObjects( this, &numObjects );

	NiTList<ObjectClass *> targetable( 2 );
	for ( int i = 0; i < numObjects; i++ ) {
		ObjectClass *object = all[i];
		if ( !object->IsTargetable() ) {
			continue;
		}
		if ( aliveOnly ) {
			DamageableObject *damageable = DamageableObject::Cast( object );
			if ( damageable != NULL && ( damageable->IsDead() || damageable->GetLifeTime() < INFINITE_LIFE ) ) {
				continue;
			}
		}
		if ( targeter != NULL && !targeter->CanTargetObject( object ) ) {
			continue;
		}
		targetable.AddTail( object );
	}

	ObjectClass **result = NULL;
	if ( targetable.GetSize() != 0 ) {
		*count = targetable.GetSize();
		result = new ObjectClass *[targetable.GetSize()];
		int i = 0;
		while ( targetable.GetSize() != 0 ) {
			result[i++] = targetable.RemoveHead();
		}
	}
	return result;
}

/*
================
SetClass_GetTargetableObjects

The Python method SetClass.GetTargetableObjects( set[, targeter[,
aliveOnly]] ): a tuple of new shadow instances of the set's targetable
objects; None when the first argument is not a set.
================
*/
// FUNCTION: STBC 0x0040fb70
PyObject *SetClass_GetTargetableObjects( PyObject *self, PyObject *args ) {
	ShipClass *targeter = NULL;
	bool aliveOnly = true;
	SetClass *set = NULL;
	if ( PyTuple_Size( args ) >= 2 ) {
		PyObject *targeterObject = PyTuple_GetItem( args, 1 );
		PyEmbed::Unknown6F98E0( targeterObject, (void **)&targeter, "ObjectClass" );
		if ( PyTuple_Size( args ) >= 3 ) {
			aliveOnly = PyInt_AsLong( PyTuple_GetItem( args, 2 ) ) != 0;
		}
	}
	PyEmbed::Unknown6F98E0( PyTuple_GetItem( args, 0 ), (void **)&set, "SetClass" );
	if ( set != NULL ) {
		int count;
		ObjectClass **objects;
		if ( PyTuple_Size( args ) == 3 ) {
			objects = set->GetTargetableObjects( &count, targeter, aliveOnly );
		} else if ( PyTuple_Size( args ) == 2 ) {
			objects = set->GetTargetableObjects( &count, targeter, true );
		} else {
			objects = set->GetTargetableObjects( &count, NULL, true );
		}
		PyObject *tuple = PyTuple_New( count );
		while ( --count >= 0 ) {
			PyTuple_SetItem( tuple, count, objects[count]->VirtualSlot08() );
		}
		delete [] objects;
		return tuple;
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
SetClass::GetObjectByID

The set's object with the ID; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0040fcd0
ObjectClass *SetClass::GetObjectByID( long id ) {
	return objects.Get( id );
}

/*
================
SetClass::GetObject

The set's object with the name; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0040fd30
ObjectClass *SetClass::GetObject( char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return objects.Get( id );
		}
	}
	return NULL;
}

/*
================
SetClass::GetClassObject

The set's object of the class type with the ID; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0040fde0 ?GetClassObject@SetClass@@QAEPAVBaseObjectClass@@HJ@Z
BaseObjectClass *SetClass::GetClassObject( int type, long id ) {
	return objectTable.GetObject( type, id );
}

/*
================
SetClass::GetClassObject

The set's object of the class type with the name; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0040fe00 ?GetClassObject@SetClass@@QAEPAVBaseObjectClass@@HPAD@Z
BaseObjectClass *SetClass::GetClassObject( int type, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return GetClassObject( type, id );
		}
	}
	return NULL;
}

/*
================
SetClass::GetNextObject

The object after the one with the ID, in ID order, wrapping round to the
first; the first object when there is none with the ID, NULL when the set
is empty.
================
*/
// FUNCTION: STBC 0x0040fe80
ObjectClass *SetClass::GetNextObject( long id ) {
	ObjectClass *object = NULL;
	int count = objects.GetCount();
	if ( count > 0 ) {
		ObjectClass **items = objects.GetItems();
		int index = objects.Find( id );
		index++;
		if ( index >= count ) {
			index = 0;
		}
		object = items[index];
	}
	return object;
}

/*
================
SetClass::GetPreviousObject

The object before the one with the ID, in ID order, wrapping round to the
last; the last object when there is none with the ID, NULL when the set
is empty.
================
*/
// FUNCTION: STBC 0x0040ff00
ObjectClass *SetClass::GetPreviousObject( long id ) {
	ObjectClass *object = NULL;
	int count = objects.GetCount();
	if ( count > 0 ) {
		ObjectClass **items = objects.GetItems();
		int index = objects.Find( id );
		if ( index == -1 ) {
			index = count;
		}
		index--;
		if ( index < 0 ) {
			index = count - 1;
		}
		object = items[index];
	}
	return object;
}

/*
================
SetClass::GetFirstObject

The object with the lowest ID.
================
*/
// FUNCTION: STBC 0x0040ffa0
ObjectClass *SetClass::GetFirstObject( void ) {
	return objects.GetAt( 0 );
}

/*
================
SetClass::Update

Steps the set by the time, at most once every unknown90 seconds: each
object once, including those added meanwhile, the ships' weapons, the
proximity manager, the cameras and the lights, deleting those marked for
deletion; then moves the set's sound region with the active camera.
================
*/
// FUNCTION: STBC 0x0040ffb0
// PARKED: register: in the cameras' inline DeleteAt retail loads the count into EDX and adds it to the shifted index in ECX (ebp + count); this build adds the index to the count in ECX, as retail's lights loop does; the rest is exact
void SetClass::Update( float time ) {
	float elapsed = time - unknown94;
	float step = UtopiaApp::GetApp()->GetGameFrameStep();
	if ( step < 0.001f ) {
		step = 0.001f;
	}
	CalcIsRendered();
	if ( elapsed < unknown90 || !unknownF1 ) {
		return;
	}
	unknown94 = time - (float)fmod( time - unknown94, unknown90 );
	Unknown413C60();

	int i;
	int count;
	for ( ;; ) {
		count = objects.GetCount();
		ObjectClass **items = objects.GetItems();
		LockObjects();
		for ( i = 0; i < count; i++ ) {
			if ( !items[i]->GetUnknown25() ) {
				items[i]->Update( time );
				items[i]->SetUnknown25( true );
				if ( items[i]->IsDeleteMe() ) {
					DeleteObjectFromSet( items[i]->GetObjID() );
					i--;
					count--;
				}
			}
		}
		if ( pendingObjects.GetCount() == 0 ) {
			break;
		}
		UnlockObjects();
	}
	UnlockObjects();

	ShipClass **ships = ShipClass::GetClassObjects( this, &count );
	for ( i = 0; i < count; i++ ) {
		WeaponSystem::Unknown5856D0( ships[i] );
		if ( ships[i]->GetPhaserSystem() != NULL ) {
			ships[i]->GetPhaserSystem()->Update( step );
		}
		if ( ships[i]->GetTractorBeamSystem() != NULL ) {
			ships[i]->GetTractorBeamSystem()->Update( step );
		}
	}
	if ( proximityManager != NULL ) {
		proximityManager->Update();
	}

	count = cameras.GetCount();
	CameraObjectClass **cameraItems = cameras.GetItems();
	for ( i = 0; i < count; i++ ) {
		cameraItems[i]->Update( time );
		if ( cameraItems[i]->IsDeleteMe() ) {
			sceneRoot->DetachChild( cameraItems[i]->GetNode() );
			cameras.DeleteAt( i );
			i--;
			count--;
		}
	}
	count = lights.GetCount();
	LightObjectClass **lightItems = lights.GetItems();
	for ( i = 0; i < count; i++ ) {
		lightItems[i]->Update( time );
		if ( lightItems[i]->IsDeleteMe() ) {
			sceneRoot->DetachChild( lightItems[i]->GetNode() );
			lights.DeleteAt( i );
			i--;
			count--;
			sceneChanged = true;
		}
	}

	Unknown413CB0();
	TGSoundRegion *region = TGSoundRegion::GetRegion( GetName() );
	if ( region != NULL ) {
		CameraObjectClass *camera = GetActiveCamera();
		if ( camera != NULL ) {
			region->SetTransform( camera->GetWorldLocation(), camera->GetWorldForward(), camera->GetWorldUp() );
		}
	}
}

/*
================
SetClass::Unknown410390

Centres the backdrops and the foreground on the camera and updates them
to the frame's time; the nebula measures the camera's distance (thrown
away) and the star sphere rescales its texture to the camera. Nothing in
retail calls it.
================
*/
// FUNCTION: STBC 0x00410390
void SetClass::Unknown410390( NiCamera *camera ) {
	if ( camera != NULL ) {
		NiPoint3 location = camera->GetWorldTranslate();
		if ( nebula != NULL ) {
			nebula->VirtualSlot68( camera );
		}
		backdropRoot->SetTranslate( location );
		backdropRoot->Update( g_kSystemWrapper.GetTimeInSeconds() );
		if ( starSphere != NULL ) {
			starSphere->UpdateTextureScale( camera );
		}
		foregroundRoot->SetTranslate( location );
		foregroundRoot->Update( g_kSystemWrapper.GetTimeInSeconds() );
	}
}

/*
================
SetClass::Unknown410460

Gives the set, its objects, lights and cameras a last update time of up to
a second before the time, each by its own random draw.
================
*/
// FUNCTION: STBC 0x00410460
void SetClass::Unknown410460( float time ) {
	unknown94 = time - (float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 100 ) * 0.01f;

	ObjectClass **items = objects.GetItems();
	int i;
	int count = objects.GetCount();
	for ( i = 0; i < count; i++ ) {
		items[i]->SetLastUpdateTime( time - (float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 100 ) * 0.01f );
	}
	LightObjectClass **lightItems = lights.GetItems();
	count = lights.GetCount();
	for ( i = 0; i < count; i++ ) {
		lightItems[i]->SetLastUpdateTime( time - (float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 100 ) * 0.01f );
	}
	count = cameras.GetCount();
	CameraObjectClass **cameraItems = cameras.GetItems();
	for ( i = 0; i < count; i++ ) {
		cameraItems[i]->SetLastUpdateTime( time - (float)(unsigned int)g_kSystemWrapper.GetRandomNumber( 100 ) * 0.01f );
	}
}

/*
================
SetClass::GetDistance

The straight distance between the points, plus each distance modifier's
change to it.
================
*/
// FUNCTION: STBC 0x00410570
float SetClass::GetDistance( NiPoint3 *from, NiPoint3 *to ) {
	float modified = ( *to - *from ).Length();
	float distance = modified;
	for ( int i = 0; i < numDistanceModifiers; i++ ) {
		modified += distanceModifiers[i]->ModifyDistance( distance, from, to ) - distance;
	}
	return modified;
}

/*
================
SetClass::IsLocationEmpty

Whether no object that collides is within the radius of the location;
proximity checks never count, and nebulas only when not ignored. Without
a proximity manager every location is empty.
================
*/
// FUNCTION: STBC 0x00410600
bool SetClass::IsLocationEmpty( NiPoint3 *location, float radius, bool ignoreNebulas ) {
	int occupied = 0;
	ProximityManager *manager = proximityManager;
	if ( manager != NULL ) {
		ProximityManager::ObjectIterator *iterator = manager->GetNearObjects( *location, radius, false );
		ObjectClass *object;
		while ( ( object = manager->GetNextObject( iterator ) ) != NULL ) {
			if ( !object->IsTypeOf( PROXIMITYCHECK_TYPE ) && object->GetCollisionFlags() != 0 &&
				 ( !ignoreNebulas || !object->IsTypeOf( NEBULA_TYPE ) ) ) {
				occupied = 1;
				break;
			}
		}
		manager->EndObjectIteration( iterator );
	}
	return occupied ? false : true;
}

/*
================
SetClass::AddDistanceModifier

Appends the modifier to the set's list, which grows by one each time.
================
*/
// FUNCTION: STBC 0x004106c0
void SetClass::AddDistanceModifier( DistanceModifier *modifier ) {
	DistanceModifier **modifiers = new DistanceModifier *[numDistanceModifiers + 1];
	if ( numDistanceModifiers != 0 ) {
		memcpy( modifiers, distanceModifiers, numDistanceModifiers * sizeof( DistanceModifier * ) );
		delete [] distanceModifiers;
	}
	distanceModifiers = modifiers;
	distanceModifiers[numDistanceModifiers] = modifier;
	numDistanceModifiers++;
}

/*
================
SetClass::VirtualSlot26

Brings the scene up to the game time when it has changed, and renders
the set through its active camera.
================
*/
// FUNCTION: STBC 0x00410730
void SetClass::VirtualSlot26( void ) {
	if ( unknown60 ) {
		sceneRoot->Update( UtopiaApp::GetApp()->GetGameTime() );
		unknown60 = false;
	}
	CameraObjectClass *camera = GetActiveCamera();
	if ( camera != NULL ) {
		VirtualSlot25( camera->GetNiCamera(), true, true, true );
	}
}

/*
================
DepthEdge

One end of an object's depth interval along the view direction, for
VirtualSlot25's split of a deep scene into two renders (invented names).
================
*/
struct DepthEdge {
	float			depth;			// along the camera's view direction
	bool			start;			// the near end; false for the far end
};

// The edges of the objects in view, two per object, kept from render to
// render; the block holds depthEdgeCapacity objects' edges (invented names).
// DATA: STBC 0x0097e99c
static DepthEdge *		depthEdges;
// DATA: STBC 0x0097e9a0
static unsigned int		depthEdgeCapacity;

/*
================
CompareDepthEdges

qsort's order of the depth edges: nearest first (invented name).
================
*/
// FUNCTION: STBC 0x00410780
int CompareDepthEdges( const void *first, const void *second ) {
	if ( ( (const DepthEdge *)first )->depth < ( (const DepthEdge *)second )->depth ) {
		return -1;
	}
	if ( ( (const DepthEdge *)first )->depth > ( (const DepthEdge *)second )->depth ) {
		return 1;
	}
	return 0;
}

/*
================
ReadRenderStats

Asks the renderer for its counts of the frame drawn, and drops them
(invented name; VirtualSlot25 does it after each Click and after the icons).
================
*/
static inline void ReadRenderStats( void ) {
	unsigned int unused1, unused2, unused3, unused4, unused5;
	UtopiaApp::GetRenderer()->VirtualSlot32( &unused1, &unused2, &unused3, &unused4, &unused5 );
}

/*
================
SetClass::VirtualSlot25

Renders the set through the camera. The backdrops follow the eye; out of
a nebula's view only what the nebula leaves visible is drawn. The scene
and effect children outside the view, or beyond the nebula's view
distance, are detached for the render and attached again after it. When
the scene is too deep for one depth range it is split in two: the far
part first, then the near part over it. With glow, the same passes go to
the glow renderer first. clearBackBuffer waits for the back buffer and
has the first pass clear it (invented name, from the clear flags);
drawIcons draws the icons over the scene (invented name).
================
*/
// FUNCTION: STBC 0x004107d0
// DRAFT: inline budget: retail destroys the detached child of the split loops through NiObject::DecRefCount (0x40CFE0) and inlines it in the plain loops, this build the reverse; the back buffer Lock loop is peeled once here; game time is copied through the FPU in retail
void SetClass::VirtualSlot25( NiCamera *camera, bool clearBackBuffer, bool unknown3, bool drawIcons ) {
	if ( g_kMovieManager.IsForcedMoviePlaying() ) {
		return;
	}

	if ( unknown60 ) {
		sceneRoot->Update( UtopiaApp::GetApp()->GetGameTime() );
		unknown60 = false;
	}
	camera->SetScene( unknown98 );
	if ( sceneChanged ) {
		unknown98->UpdateEffects();
		sceneChanged = false;
	}

	NiFrustum savedFrustum = camera->GetViewFrustum();
	NiRect<float> savedPort = camera->GetViewPort();
	NiPoint3 cameraLoc = camera->GetWorldLocation();
	// DATA: STBC 0x0097e758 _?wholeScreen@?L@??VirtualSlot25@SetClass@@UAEXPAVNiCamera@@_N11@Z@4V?$NiRect@M@@A
	static NiRect<float> wholeScreen( 0.0f, 1.0f, 1.0f, 0.0f );
	float time = UtopiaApp::GetApp()->GetGameTime();

	// the backdrops are centred on the eye
	if ( backdropRoot != NULL ) {
		backdropRoot->SetTranslate( cameraLoc );
		backdropRoot->Update( time );
	}
	float backdropDepth = backdropRoot->GetWorldBound().radius * 1.05f;

	// out of a nebula's view, what it hides is detached
	float nebulaDistance = NI_INFINITY;
	bool inNebula = false;
	NiTList<NiPointer<NiAVObject> > hiddenScene( 2 );
	NiTList<NiPointer<NiAVObject> > hiddenEffects( 2 );
	bool nearNebula = false;
	if ( nebula != NULL ) {
		nebulaDistance = nebula->VirtualSlot68( camera );
		inNebula = nebulaDistance <= nebula->GetVisibility();
		nearNebula = nebulaDistance < nebula->GetVisibility() * 4.0f;
		if ( nebulaDistance > nebula->GetVisibility() ) {
			unsigned int count = sceneRoot->GetArrayCount();
			unsigned int i;
			for ( i = 0; i < count; i++ ) {
				NiAVObject *child = sceneRoot->GetAt( i );
				if ( child != NULL && child != nebula->GetNode() ) {
					NiPoint3 center = child->GetWorldBound().center;
					NiPoint3 direction = cameraLoc - center;
					direction.Unitize();
					NiPoint3 nearest = direction * child->GetWorldBound().radius + center;
					if ( !nebula->VirtualSlot69( &nearest, &cameraLoc ) ) {
						hiddenScene.AddTail( child );
						sceneRoot->DetachChildAt( i );
					}
				}
			}
			count = effectRoot->GetArrayCount();
			for ( i = 0; i < count; i++ ) {
				NiAVObject *child = effectRoot->GetAt( i );
				if ( child != NULL ) {
					if ( !nebula->VirtualSlot69( &child->GetWorldBound().center, &cameraLoc ) ) {
						hiddenEffects.AddTail( child );
						effectRoot->DetachChildAt( i );
					}
				}
			}
		}
	}

	if ( starSphere != NULL ) {
		starSphere->UpdateTextureScale( camera );
	}
	if ( foregroundRoot != NULL ) {
		foregroundRoot->SetTranslate( cameraLoc );
		foregroundRoot->Update( time );
	}
	effectRoot->Update( time );

	bool split = false;
	bool glow = glowEnabled && glowIntensity > 0.0f && glowRenderer != NULL;
	NiTList<NiPointer<NiAVObject> > culledScene( 40 );
	NiTList<NiPointer<NiAVObject> > culledEffects( 40 );
	NiFrustum frustum = camera->GetViewFrustum();
	float farthest = -NI_INFINITY;
	float splitDepth = unknown108;

	if ( !nearNebula ) {
		if ( !unknown10C || !splitLargeZRenders ) {
			unsigned int count = sceneRoot->GetArrayCount();
			unsigned int i;
			for ( i = 0; i < count; i++ ) {
				NiAVObject *child = sceneRoot->GetAt( i );
				if ( child != NULL ) {
					NiBound &bound = child->GetWorldBound();
					bool visible = bound.WhichSide( camera->GetPlane( NiCamera::NEAR_PLANE ) ) != NiPlane::NEGATIVE_SIDE;
					for ( int plane = NiCamera::LEFT_PLANE; visible && plane <= NiCamera::BOTTOM_PLANE; plane++ ) {
						visible = bound.WhichSide( camera->GetPlane( plane ) ) != NiPlane::NEGATIVE_SIDE;
					}
					if ( visible ) {
						float radius = bound.radius * 1.05f;
						float depth = ( bound.center - cameraLoc ) * camera->GetWorldDirection();
						if ( depth - radius < nebulaDistance ) {
							if ( depth + radius > farthest ) {
								farthest = depth + radius;
							}
						} else {
							culledScene.AddTail( child );
							sceneRoot->DetachChildAt( i );
						}
					} else {
						culledScene.AddTail( child );
						sceneRoot->DetachChildAt( i );
					}
				}
			}
			count = effectRoot->GetArrayCount();
			for ( i = 0; i < count; i++ ) {
				NiAVObject *child = effectRoot->GetAt( i );
				if ( child != NULL ) {
					NiBound &bound = child->GetWorldBound();
					bool visible = bound.WhichSide( camera->GetPlane( NiCamera::NEAR_PLANE ) ) != NiPlane::NEGATIVE_SIDE;
					for ( int plane = NiCamera::LEFT_PLANE; visible && plane < NiCamera::MAX_PLANES; plane++ ) {
						visible = bound.WhichSide( camera->GetPlane( plane ) ) != NiPlane::NEGATIVE_SIDE;
					}
					if ( visible ) {
						float radius = bound.radius * 1.05f;
						float depth = ( bound.center - cameraLoc ) * camera->GetWorldDirection();
						if ( depth - radius < nebulaDistance ) {
							if ( depth + radius > farthest ) {
								farthest = depth + radius;
							}
						} else {
							culledEffects.AddTail( child );
							effectRoot->DetachChildAt( i );
						}
					} else {
						culledEffects.AddTail( child );
						effectRoot->DetachChildAt( i );
					}
				}
			}
		} else {
			// the depth interval of each object in view
			unsigned int numVisible = 0;
			unsigned int count = sceneRoot->GetArrayCount();
			unsigned int i;
			for ( i = 0; i < count; i++ ) {
				if ( sceneRoot->GetAt( i ) != NULL ) {
					numVisible++;
				}
			}
			count = effectRoot->GetArrayCount();
			for ( i = 0; i < count; i++ ) {
				if ( effectRoot->GetAt( i ) != NULL ) {
					numVisible++;
				}
			}
			if ( depthEdgeCapacity < numVisible ) {
				delete [] depthEdges;
				depthEdges = new DepthEdge[numVisible * 2];
				depthEdgeCapacity = numVisible;
			}

			numVisible = 0;
			count = sceneRoot->GetArrayCount();
			for ( i = 0; i < count; i++ ) {
				NiAVObject *child = sceneRoot->GetAt( i );
				if ( child != NULL ) {
					NiBound &bound = child->GetWorldBound();
					bool visible = bound.WhichSide( camera->GetPlane( NiCamera::NEAR_PLANE ) ) != NiPlane::NEGATIVE_SIDE;
					for ( int plane = NiCamera::LEFT_PLANE; visible && plane <= NiCamera::BOTTOM_PLANE; plane++ ) {
						visible = bound.WhichSide( camera->GetPlane( plane ) ) != NiPlane::NEGATIVE_SIDE;
					}
					if ( visible ) {
						float radius = bound.radius * 1.05f;
						float depth = ( bound.center - cameraLoc ) * camera->GetWorldDirection();
						if ( depth - radius < nebulaDistance ) {
							depthEdges[numVisible * 2].depth = depth - radius;
							depthEdges[numVisible * 2].start = true;
							depthEdges[numVisible * 2 + 1].depth = depth + radius;
							depthEdges[numVisible * 2 + 1].start = false;
							numVisible++;
						} else {
							culledScene.AddTail( child );
							sceneRoot->DetachChildAt( i );
						}
					} else {
						culledScene.AddTail( child );
						sceneRoot->DetachChildAt( i );
					}
				}
			}
			count = effectRoot->GetArrayCount();
			for ( i = 0; i < count; i++ ) {
				NiAVObject *child = effectRoot->GetAt( i );
				if ( child != NULL ) {
					NiBound &bound = child->GetWorldBound();
					bool visible = bound.WhichSide( camera->GetPlane( NiCamera::NEAR_PLANE ) ) != NiPlane::NEGATIVE_SIDE;
					for ( int plane = NiCamera::LEFT_PLANE; visible && plane < NiCamera::MAX_PLANES; plane++ ) {
						visible = bound.WhichSide( camera->GetPlane( plane ) ) != NiPlane::NEGATIVE_SIDE;
					}
					if ( visible ) {
						float radius = bound.radius * 1.05f;
						float depth = ( bound.center - cameraLoc ) * camera->GetWorldDirection();
						if ( depth - radius < nebulaDistance ) {
							depthEdges[numVisible * 2].depth = depth - radius;
							depthEdges[numVisible * 2].start = true;
							depthEdges[numVisible * 2 + 1].depth = depth + radius;
							depthEdges[numVisible * 2 + 1].start = false;
							numVisible++;
						} else {
							culledEffects.AddTail( child );
							effectRoot->DetachChildAt( i );
						}
					} else {
						culledEffects.AddTail( child );
						effectRoot->DetachChildAt( i );
					}
				}
			}

			// split where no interval is open beyond unknown108, when the
			// whole is more than twice as deep as the near part
			if ( numVisible == 0 ) {
				farthest = backdropDepth;
			} else {
				qsort( depthEdges, numVisible * 2, sizeof( DepthEdge ), CompareDepthEdges );
				farthest = depthEdges[numVisible * 2 - 1].depth;
				if ( farthest >= unknown108 + unknown108 ) {
					bool found = false;
					int open = 0;
					DepthEdge *edge = depthEdges;
					for ( unsigned int k = 0; k < numVisible * 2; k++, edge++ ) {
						if ( edge->start ) {
							open++;
						} else {
							open--;
						}
						if ( open <= 0 && ( edge->depth > unknown108 || ( k < numVisible * 2 - 2 && edge[1].depth > unknown108 ) ) ) {
							splitDepth = edge->depth;
							found = true;
						}
						if ( found ) {
							break;
						}
					}
					if ( splitDepth + splitDepth < farthest ) {
						split = true;
					} else {
						split = false;
					}
				} else {
					split = false;
				}
			}
		}
		if ( farthest > nebulaDistance ) {
			farthest = nebulaDistance;
		}
	} else {
		farthest = nebulaDistance;
	}
	if ( !split && farthest > 10000.0f ) {
		farthest = 10000.0f;
	}

	// wait until the back buffer is free
	if ( clearBackBuffer && camera->GetRenderer() != NULL ) {
		NiPointer<NiImage> image = (NiImage *)camera->GetRenderer()->VirtualSlot22( 1 );
		NiPointer<NiDDBufferImage> bufferImage = NiDynamicCast( NiDDBufferImage, image );
		IDirectDrawSurface7 *surface;
		bufferImage->Unknown7C70C0( &surface );
		DDSURFACEDESC2 desc;
		desc.dwSize = sizeof( desc );
		for ( ;; ) {
			HRESULT result = surface->Lock( NULL, &desc, 0, NULL );
			if ( result == DDERR_SURFACELOST ) {
				break;
			}
			if ( result == DDERR_WASSTILLDRAWING || result == DDERR_SURFACEBUSY || result == DDERR_SURFACEISOBSCURED ) {
				continue;
			}
			surface->Unlock( NULL );
			break;
		}
	}

	NiPointer<NiNode> detachedBackdrops;
	if ( glow ) {
		NiFogProperty *fogProperty = fog;
		bool fogOn = fogProperty->GetFog();
		fogProperty->SetFog( false );
		camera->SetViewPort( wholeScreen );
		glowRenderer->SetIntensity( glowIntensity );
		glowRenderer->BeginPass();
		bool first = true;
		if ( split ) {
			if ( !inNebula ) {
				detachedBackdrops = backdropRoot;
				unknown98->DetachChild( backdropRoot );
				camera->SetViewFrustum( NiFrustum( frustum.left, frustum.right, frustum.top, frustum.bottom, 1.0f, backdropDepth ) );
				camera->SetScene( backdropRoot );
				camera->Update( time );
				glowRenderer->RenderPass( camera, true );
				first = false;
			}
			camera->SetScene( unknown98 );
			camera->SetViewFrustum( NiFrustum( frustum.left, frustum.right, frustum.top, frustum.bottom, splitDepth, farthest ) );
			camera->Update( time );
			glowRenderer->RenderPass( camera, first );
			first = false;
		} else if ( !inNebula && farthest < backdropDepth ) {
			farthest = backdropDepth;
		}
		NiFrustum nearFrustum = frustum;
		nearFrustum.nearDistance = 1.0f;
		nearFrustum.farDistance = split ? splitDepth : farthest;
		camera->SetViewFrustum( nearFrustum );
		camera->Update( time );
		glowRenderer->RenderPass( camera, first );
		glowRenderer->EndPass();
		camera->SetViewPort( savedPort );
		fog->SetFog( fogOn );
	}

	NiFrustum nearFrustum = frustum;
	float nearFar;
	if ( split ) {
		if ( !inNebula ) {
			camera->SetViewFrustum( NiFrustum( frustum.left, frustum.right, frustum.top, frustum.bottom, 1.0f, backdropDepth ) );
			camera->SetScene( backdropRoot );
			camera->Update( time );
			detachedBackdrops = backdropRoot;
			unknown98->DetachChild( backdropRoot );
		}
		camera->GetRenderer()->SetBackgroundColor( fogColor );
		camera->SetViewPort( wholeScreen );
		camera->Clear( clearBackBuffer ? 5 : 4 );
		camera->SetViewPort( savedPort );
		if ( !inNebula ) {
			camera->Update( time );
			camera->Click();
			ReadRenderStats();
		}
		camera->SetScene( unknown98 );
		camera->SetViewFrustum( NiFrustum( frustum.left, frustum.right, frustum.top, frustum.bottom, splitDepth, farthest ) );
		camera->Update( time );
		camera->Click();
		ReadRenderStats();
		clearBackBuffer = false;
		nearFar = splitDepth;
	} else {
		if ( !inNebula && farthest < backdropDepth ) {
			farthest = backdropDepth;
		}
		camera->GetRenderer()->SetBackgroundColor( fogColor );
		nearFar = farthest;
	}
	nearFrustum.nearDistance = 1.0f;
	nearFrustum.farDistance = nearFar;
	camera->SetViewFrustum( nearFrustum );
	unsigned int clearFlags = clearBackBuffer ? 5 : 4;
	camera->SetViewPort( wholeScreen );
	camera->Update( time );
	camera->Clear( clearFlags );
	camera->SetViewPort( savedPort );
	camera->SetScene( unknown98 );
	camera->Update( time );
	camera->Click();
	ReadRenderStats();

	// put back what was detached
	if ( split ) {
		unknown98->AttachChild( backdropRoot, true );
	}
	NiTListIterator pos = culledScene.GetHeadPos();
	while ( pos != NULL ) {
		sceneRoot->AttachChild( culledScene.GetNext( pos ), true );
	}
	pos = culledEffects.GetHeadPos();
	while ( pos != NULL ) {
		effectRoot->AttachChild( culledEffects.GetNext( pos ), true );
	}
	if ( nebula != NULL ) {
		pos = hiddenScene.GetHeadPos();
		while ( pos != NULL ) {
			sceneRoot->AttachChild( hiddenScene.GetNext( pos ), true );
		}
		pos = hiddenEffects.GetHeadPos();
		while ( pos != NULL ) {
			effectRoot->AttachChild( hiddenEffects.GetNext( pos ), true );
		}
	}

	if ( glow ) {
		glowRenderer->Composite();
	}
	if ( drawIcons ) {
		g_kIconManager.Draw();
		ReadRenderStats();
	}
	if ( unknown3 ) {
		camera->Unknown7E81B0();
	}
	camera->SetViewFrustum( savedFrustum );
}

/*
================
SetClass::AddCameraToSet

Names the camera, files it by name and ID and attaches it to the scene;
the first camera becomes the active one, and the icon manager's camera
when the set is rendered. False when the name is taken.
================
*/
// FUNCTION: STBC 0x004128b0
bool SetClass::AddCameraToSet( CameraObjectClass *camera, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	camera->SetName( name_ );
	if ( name_ != NULL ) {
		objectIDs.SetAt( name_, camera->GetObjID() );
	}
	cameras.Insert( camera );
	objectTable.AddObject( camera, this );
	camera->SetContainingSet( this );
	camera->GetNiCamera()->SetScene( unknown98 );
	sceneRoot->AttachChild( camera->GetNode(), true );
	if ( cameras.GetCount() == 1 ) {
		camera->SetActive( true );
		CalcIsRendered();
		if ( rendered ) {
			g_kIconManager.SetCamera( camera->GetNiCamera() );
		}
	}
	return true;
}

/*
================
SetClass::CreateCamera

Creates a camera at the position under the name, adds it to the set and
gives the icon manager its NetImmerse camera; false when the name is
taken.
================
*/
// FUNCTION: STBC 0x00412b10
bool SetClass::CreateCamera( float x, float y, float z, float unknown4, float unknown5, float unknown6, float unknown7, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	CameraObjectClass *camera = CameraObjectClass::Create( x, y, z, unknown4, unknown5, unknown6, unknown7, name_ );
	AddCameraToSet( camera, name_ );
	g_kIconManager.SetDisplayInfo( camera->GetNiCamera(), 1.0f );
	return true;
}

/*
================
SetClass::GetCamera

The set's camera with the name; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00412bb0
CameraObjectClass *SetClass::GetCamera( char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return cameras.Get( id );
		}
	}
	return NULL;
}

/*
================
SetClass::RemoveCameraFromSet

Takes the named camera out of the set, without deleting it; when it was
the active camera, another one becomes active.
================
*/
// FUNCTION: STBC 0x00412c60
void SetClass::RemoveCameraFromSet( char *name_ ) {
	CameraObjectClass *camera = GetCamera( name_ );
	if ( camera == NULL ) {
		return;
	}
	if ( camera->IsActive() ) {
		int count = cameras.GetCount();
		CameraObjectClass **items = cameras.GetItems();
		for ( int i = 0; i < count; i++ ) {
			if ( items[i] != camera ) {
				SetActiveCamera( items[i]->GetName() );
				break;
			}
		}
		camera->SetActive( false );
	}
	if ( name_ != NULL ) {
		objectIDs.RemoveAt( name_ );
	}
	sceneRoot->DetachChild( camera->GetNode() );
	camera->GetNiCamera()->SetScene( NULL );
	camera->SetContainingSet( NULL );
	objectTable.RemoveObject( camera, this );
	cameras.Remove( camera );
}

/*
================
SetClass::DeleteCameraFromSet

Takes the named camera out of the set and has it delete itself through
the event queue.
================
*/
// FUNCTION: STBC 0x00412e50
void SetClass::DeleteCameraFromSet( char *name_ ) {
	CameraObjectClass *camera = GetCamera( name_ );
	if ( camera != NULL ) {
		RemoveCameraFromSet( name_ );
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_DELETE_ME_STAGE_1 );
		event->SetDestination( camera );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
SetClass::SetActiveCamera

Makes the named camera the set's active one, and the icon manager's camera
when the set is rendered; false when the set has no object of the name.
================
*/
// FUNCTION: STBC 0x00412ec0
bool SetClass::SetActiveCamera( char *name_ ) {
	if ( name_ == NULL ) {
		return false;
	}
	long id;
	if ( !objectIDs.GetAt( name_, id ) ) {
		return false;
	}
	CameraObjectClass *camera = cameras.Get( id );
	if ( camera != NULL ) {
		camera->SetActive( true );
		CalcIsRendered();
		if ( rendered ) {
			NiCamera *niCamera = camera->GetNiCamera();
			if ( niCamera != NULL ) {
				g_kIconManager.SetCamera( niCamera );
			}
		}
		int count = cameras.GetCount();
		CameraObjectClass **items = cameras.GetItems();
		for ( int i = 0; i < count; i++ ) {
			CameraObjectClass *other = items[i];
			if ( other != camera && other->IsActive() ) {
				other->SetActive( false );
			}
		}
	}
	return true;
}

/*
================
SetClass::GetActiveCamera

The first of the set's cameras that is active; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00412fd0
CameraObjectClass *SetClass::GetActiveCamera( void ) {
	int count = cameras.GetCount();
	CameraObjectClass **items = cameras.GetItems();
	for ( int i = 0; i < count; i++ ) {
		if ( items[i]->IsActive() ) {
			return items[i];
		}
	}
	return NULL;
}

/*
================
SetClass::GetActiveNiCamera

Returns the NetImmerse camera of the active camera, or NULL if there is no active camera.
================
*/
// FUNCTION: STBC 0x00413010
NiCamera *SetClass::GetActiveNiCamera( void ) {
	CameraObjectClass *camera = GetActiveCamera();
	if ( camera != NULL ) {
		return camera->GetNiCamera();
	}
	return NULL;
}

/*
================
SetClass::AddLightToSet

Names the light, files it by name and ID and puts it among the set's
lights. False when the name is taken.
================
*/
// FUNCTION: STBC 0x00413030
bool SetClass::AddLightToSet( LightObjectClass *light, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	light->SetName( name_ );
	if ( name_ != NULL ) {
		objectIDs.SetAt( name_, light->GetObjID() );
	}
	lights.Insert( light );
	objectTable.AddObject( light, this );
	light->SetContainingSet( this );
	sceneChanged = true;
	return true;
}

/*
================
SetClass::CreatePointLight

Creates a point light at the location under the name and adds it to the
set; false when the name is taken.
================
*/
// FUNCTION: STBC 0x00413200
bool SetClass::CreatePointLight( float x, float y, float z, float red, float green, float blue, float dimmer,
								 float linearAttenuation, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	LightObjectClass *light = LightObjectClass::Create( LightObjectClass::CreateNiPointLight( red, green, blue, dimmer,
																							  linearAttenuation, name_ ) );
	light->SetTranslateXYZ( x, y, z );
	AddLightToSet( light, name_ );
	return true;
}

/*
================
SetClass::CreateSpotLight

Creates a spot light at the location under the name and adds it to the
set; false when the name is taken. The twelfth argument is not used.
================
*/
// FUNCTION: STBC 0x004132a0
bool SetClass::CreateSpotLight( float x, float y, float z, float red, float green, float blue, float dimmer, float directionX,
								float directionY, float directionZ, float angle, float unknown12, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	LightObjectClass *light = LightObjectClass::Create( LightObjectClass::CreateNiSpotLight( red, green, blue, dimmer, directionX,
																							 directionY, directionZ, angle, name_ ) );
	light->SetTranslateXYZ( x, y, z );
	AddLightToSet( light, name_ );
	return true;
}

/*
================
SetClass::CreateAmbientLight

Creates an ambient light under the name and adds it to the set; false
when the name is taken.
================
*/
// FUNCTION: STBC 0x00413350
bool SetClass::CreateAmbientLight( float red, float green, float blue, float dimmer, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	LightObjectClass *light = LightObjectClass::Create( LightObjectClass::CreateNiAmbientLight( red, green, blue, dimmer, name_ ) );
	AddLightToSet( light, name_ );
	return true;
}

/*
================
SetClass::CreateDirectionalLight

Creates a directional light under the name and adds it to the set; false
when the name is taken.
================
*/
// FUNCTION: STBC 0x004133d0
bool SetClass::CreateDirectionalLight( float red, float green, float blue, float dimmer, float directionX, float directionY,
									   float directionZ, char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return false;
		}
	}
	LightObjectClass *light = LightObjectClass::Create( LightObjectClass::CreateNiDirectionalLight( red, green, blue, dimmer, directionX,
																									directionY, directionZ, name_ ) );
	AddLightToSet( light, name_ );
	return true;
}

/*
================
SetClass::GetLight

The set's light with the name; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00413460
LightObjectClass *SetClass::GetLight( char *name_ ) {
	if ( name_ != NULL ) {
		long id;
		if ( objectIDs.GetAt( name_, id ) ) {
			return lights.Get( id );
		}
	}
	return NULL;
}

/*
================
SetClass::GetLightByID

The set's light with the ID; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00413510
LightObjectClass *SetClass::GetLightByID( long id ) {
	return lights.Get( id );
}

/*
================
SetClass::DeleteLightFromSet

Takes the named light out of the set and deletes it.
================
*/
// FUNCTION: STBC 0x00413570
void SetClass::DeleteLightFromSet( char *name_ ) {
	LightObjectClass *light = GetLight( name_ );
	if ( light == NULL ) {
		return;
	}
	if ( name_ != NULL ) {
		objectIDs.RemoveAt( name_ );
	}
	sceneRoot->DetachChild( light->GetNode() );
	sceneChanged = true;
	objectTable.RemoveObject( light, this );
	lights.Delete( light );
}

/*
================
SetClass::DeleteLightByID

DeleteLightFromSet for the light with the ID.
================
*/
// FUNCTION: STBC 0x00413710
void SetClass::DeleteLightByID( long id ) {
	LightObjectClass *light = GetLightByID( id );
	if ( light == NULL ) {
		return;
	}
	char *lightName = light->GetName();
	if ( lightName != NULL ) {
		objectIDs.RemoveAt( lightName );
	}
	sceneRoot->DetachChild( light->GetNode() );
	sceneChanged = true;
	objectTable.RemoveObject( light, this );
	lights.Delete( light );
}

/*
================
SetClass::SetWireframe
================
*/
// FUNCTION: STBC 0x004138b0
void SetClass::SetWireframe( bool wireframe_ ) {
	wireframe->SetWireframe( wireframe_ );
}

/*
================
SetClass::GetWireframe
================
*/
// FUNCTION: STBC 0x004138d0
bool SetClass::GetWireframe( void ) {
	return wireframe->GetWireframe();
}

/*
================
SetClass::StartPick

Picks along the ray: through the objects' bounds only, or down to their
triangles.
================
*/
// FUNCTION: STBC 0x004138e0 ?StartPick@SetClass@@QAEXPAVNiPoint3@@0_N@Z
void SetClass::StartPick( NiPoint3 *origin, NiPoint3 *direction, bool boundsOnly ) {
	if ( boundsOnly ) {
		pick->SetIntersectType( NiPick::BOUND_INTERSECT );
	} else {
		pick->SetIntersectType( NiPick::TRIANGLE_INTERSECT );
	}
	pickOrigin = *origin;
	pickDirection = *direction;
	pickIndex = 0;
	unknownD8 = true;
}

/*
================
SetClass::StartPick

Starts a pick along the ray through the point of the window, given as
fractions of the renderer's width and height, from the active camera.
================
*/
// FUNCTION: STBC 0x00413950 ?StartPick@SetClass@@QAEXMM_N@Z
void SetClass::StartPick( float x, float y, bool boundsOnly ) {
	CameraObjectClass *camera = GetActiveCamera();
	if ( camera == NULL ) {
		unknownD8 = false;
		return;
	}
	NiCamera *niCamera = camera->GetNiCamera();
	NiRender *renderer = niCamera->GetRenderer();
	int width = renderer->VirtualSlot19();
	int height = renderer->VirtualSlot20();
	long windowX = (long)( x * width );
	long windowY = (long)( y * height );
	niCamera->WindowPointToRay( windowX, windowY, pickOrigin, pickDirection );
	StartPick( &pickOrigin, &pickDirection, boundsOnly );
}

/*
================
SetClass::GetDotProductToObject

How squarely the active camera looks at the object: the cosine of the
angle between the camera's direction and the way to the object; 0 without
an active camera or an object.
================
*/
// FUNCTION: STBC 0x004139e0
float SetClass::GetDotProductToObject( ObjectClass *object ) {
	CameraObjectClass *camera = GetActiveCamera();
	if ( camera == NULL || object == NULL ) {
		return 0.0f;
	}
	NiCamera *niCamera = camera->GetNiCamera();
	NiPoint3 direction = object->GetWorldLocation() - niCamera->GetWorldLocation();
	direction.Unitize();
	return direction.Dot( niCamera->GetWorldDirection() );
}

/*
================
SetClass::GetDotProductToVector

How squarely the active camera looks along the vector: the cosine of the
angle between the camera's direction and the vector; 0 without an active
camera.
================
*/
// FUNCTION: STBC 0x00413ac0
float SetClass::GetDotProductToVector( TGPoint3 vector ) {
	CameraObjectClass *camera = GetActiveCamera();
	if ( camera == NULL ) {
		unknownD8 = false;
		return 0.0f;
	}
	NiCamera *niCamera = camera->GetNiCamera();
	vector.Unitize();
	return niCamera->GetWorldDirection().Dot( vector );
}

/*
================
SetClass::GetPickedObject

Returns the object picked by the last pick operation, or NULL if none.
================
*/
// FUNCTION: STBC 0x00413b70 ?GetPickedObject@SetClass@@QAEPAVObjectClass@@XZ
ObjectClass *SetClass::GetPickedObject( void ) {
	NiPick::Record *record;
	return GetPickedObject( &record );
}

/*
================
SetClass::GetPickedObject

The next object, from pickIndex on, that the pick's ray hits, with the
record of the first hit; NULL when no other object is hit, which ends the
pick.
================
*/
// FUNCTION: STBC 0x00413b80 ?GetPickedObject@SetClass@@QAEPAVObjectClass@@PAPAVRecord@NiPick@@@Z
ObjectClass *SetClass::GetPickedObject( NiPick::Record **record ) {
	if ( !unknownD8 ) {
		return NULL;
	}
	int count = objects.GetCount();
	ObjectClass **items = objects.GetItems();
	for ( ; pickIndex < count; pickIndex++ ) {
		pick->SetTarget( items[pickIndex]->GetNode() );
		if ( pick->PickObjects( pickOrigin, pickDirection, false ) ) {
			ObjectClass *object = items[pickIndex++];
			*record = pick->GetRecord( 0 );
			return object;
		}
	}
	unknownD8 = false;
	return NULL;
}

/*
================
SetClass::Unknown413C60

Marks every object, camera and light not yet stepped.
================
*/
// FUNCTION: STBC 0x00413c60
void SetClass::Unknown413C60( void ) {
	int i;
	int count = objects.GetCount();
	ObjectClass **items = objects.GetItems();
	for ( i = 0; i < count; i++ ) {
		items[i]->SetUnknown25( false );
	}
	count = cameras.GetCount();
	CameraObjectClass **cameraItems = cameras.GetItems();
	for ( i = 0; i < count; i++ ) {
		cameraItems[i]->SetUnknown25( false );
	}
	count = lights.GetCount();
	LightObjectClass **lightItems = lights.GetItems();
	for ( i = 0; i < count; i++ ) {
		lightItems[i]->SetUnknown25( false );
	}
}

/*
================
SetClass::Unknown413CB0

While this is the rendered set, plays the engine sounds of the four ships
nearest the active camera (those within 35 of it) and stops the sounds of
the ships that have left that group; any other set stops its sounds.
================
*/
// FUNCTION: STBC 0x00413cb0
// DRAFT: call set as retail's (AddTail on an empty list, InsertBefore, AddTail after the walk: three NewItem 0x417BF0 calls); left: retail keeps the insertion walk's pos in memory (so GetNext's null test stays at the loop top: one branch more), this and i in other registers
void SetClass::Unknown413CB0( void ) {
	int i;
	if ( g_kSetManager.GetRenderedSet() != this ) {
		for ( i = 0; i < numSounds; i++ ) {
			g_kSoundManager.StopSound( sounds[i].soundID );
		}
		numSounds = 0;
		return;
	}

	NiTList<ShipClass *> nearShips( 2 );
	if ( proximityManager != NULL ) {
		CameraObjectClass *camera = GetActiveCamera();
		if ( camera != NULL ) {
			ProximityManager::ObjectIterator *iterator = proximityManager->GetNearObjects( camera->GetWorldLocation(), ENGINE_SOUND_RANGE, false );
			ObjectClass *object = proximityManager->GetNextObject( iterator );
			while ( object != NULL ) {
				ShipClass *ship = (ShipClass *)object;
				if ( object->IsTypeOf( SHIPCLASS_TYPE ) && ship->GetImpulseEngineSubsystem() != NULL ) {
					// the list keeps the ships nearest first
					NiTListIterator pos = nearShips.GetHeadPos();
					if ( pos == NULL ) {
						nearShips.AddTail( ship );
					} else {
						for ( ;; ) {
							NiTListIterator listed = pos;
							ShipClass *other = nearShips.GetNext( pos );
							NiPoint3 otherOffset = other->GetWorldLocation() - camera->GetWorldLocation();
							if ( ( ship->GetWorldLocation() - camera->GetWorldLocation() ).SqrLength() <= otherOffset.SqrLength() ) {
								nearShips.InsertBefore( listed, ship );
								break;
							}
							if ( pos == NULL ) {
								nearShips.AddTail( ship );
								break;
							}
						}
					}
				}
				object = proximityManager->GetNextObject( iterator );
			}
			proximityManager->EndObjectIteration( iterator );
		}
		while ( nearShips.GetSize() > 4 ) {
			nearShips.RemoveTail();
		}
	}

	// the sounds of ships still among the nearest go on; the others stop
	for ( i = 0; i < numSounds; i++ ) {
		NiTListIterator pos = nearShips.GetHeadPos();
		bool found = false;
		while ( pos != NULL ) {
			NiTListIterator listed = pos;
			ShipClass *ship = nearShips.GetNext( pos );
			if ( ship->GetObjID() == sounds[i].objectID ) {
				nearShips.RemovePos( listed );
				found = true;
				break;
			}
		}
		if ( !found ) {
			g_kSoundManager.StopSound( sounds[i].soundID );
			memmove( &sounds[i], &sounds[numSounds - 1], sizeof( ObjectSound ) );
			numSounds--;
			i--;
		}
	}

	// a sound for each of the others
	while ( nearShips.GetSize() != 0 ) {
		ShipClass *ship = nearShips.RemoveHead();
		if ( numSounds < 4 && ship->GetImpulseEngineSubsystem() != NULL ) {
			ImpulseEngineProperty *property = ship->GetImpulseEngineSubsystem()->GetProperty();
			if ( property != NULL && property->GetEngineSound() != NULL ) {
				TGSoundRegion *region = TGSoundRegion::GetRegion( name );
				if ( region != NULL ) {
					TGSound *sound = g_kSoundManager.GetSound( property->GetEngineSound() );
					if ( sound != NULL ) {
						region->AddSound( sound );
						sound->AttachToNode( ship->GetNode() );
						sound->SetLooping( true );
						sound->SetMinMaxDistance( engineSoundMinDistance, ENGINE_SOUND_RANGE );
						sounds[numSounds].objectID = ship->GetObjID();
						sounds[numSounds].soundID = sound->Play( true );
						numSounds++;
					}
				}
			}
		}
	}
}

/*
================
SetClass::Unknown414270

Turns the active camera to look at the object, from where the camera is
or, in a place-by-direction mode, from where the mode puts it.
================
*/
// FUNCTION: STBC 0x00414270
// PARKED: inline budget: retail expands the first UnitCross's cross product and Length inline and calls its operator/ and zero constructor out of line, and the second UnitCross's constructor, Length, operator/ and the assignment all out of line (a frame 12 bytes larger); this build spends the budget differently
void SetClass::Unknown414270( ObjectClass *object ) {
	NiBound bound;
	CameraObjectClass *camera = GetActiveCamera();
	NiPoint3 target = object->GetNiObject()->GetWorldTranslate();
	// BUG: redundant: the object's bound is copied but never read
	bound = object->GetWorldBound();
	NiPoint3 position = camera->GetWorldLocation();
	PlaceByDirectionMode *mode = PlaceByDirectionMode::Cast( camera->GetCurrentCameraMode( true ) );
	if ( mode != NULL ) {
		position = mode->Unknown427E50( target );
	}
	NiPoint3 forward = target - position;
	forward.Unitize();
	NiPoint3 right;
	right = forward.UnitCross( NiPoint3::UNIT_Z );
	NiPoint3 up;
	up = right.UnitCross( forward );
	camera->LookToward( &forward, &up, 1.0f );
}

/*
================
SetClass::Unknown4145E0

Moves the ID of the object known as oldName to newName in the name map
(BaseObjectClass::SetName calls it with the set the object is in).
================
*/
// FUNCTION: STBC 0x004145e0
void SetClass::Unknown4145E0( char *oldName, char *newName ) {
	long id;
	if ( oldName != NULL && objectIDs.GetAt( oldName, id ) ) {
		objectIDs.RemoveAt( oldName );
	}
	if ( newName != NULL ) {
		// BUG: id is unset when oldName is not in the map
		objectIDs.SetAt( newName, id );
	}
}

/*
================
SetClass::WriteToStream

Writes the set: its names and flags, the background model, the objects
(the backdrops by the order of their nodes under the backdrop root), the
objects still waiting to join, the cameras and lights, and what the set
needs to put itself together again.
================
*/
// FUNCTION: STBC 0x00414750
// DRAFT: frame and calls as retail's; left: retail lays the objects loop's non-backdrop part after the loop's back edge (the AddTail falls into the increment), and keeps the child in EBX in the backdrop walk where this build keeps pos
bool SetClass::WriteToStream( TGStream *stream ) {
	char text[256];
	int i;

	if ( !TGEventHandlerObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( name );
	sprintf( text, "Saving set %s\n", name );
	TGObject::Log( text );
	stream->WriteBool( objectsLocked );
	displayName.WriteToStream( stream );
	stream->VirtualSlot11( backgroundModel );
	if ( backgroundModel != NULL ) {
		ObjectClass *model = GetObject( "BackgroundModel" );
		if ( model != NULL ) {
			model->VirtualSlot64( stream );
		} else {
			stream->WriteInt( 0 );
		}
	}

	NiTList<ObjectClass *> backdrops( 2 );
	int count = objects.GetCount();
	sprintf( text, "Saving %d Objects\n", count );
	TGObject::Log( text );
	ObjectClass **items = objects.GetItems();
	int saved = 0;
	for ( i = 0; i < count; i++, items++ ) {
		if ( *items == NULL ) {
			continue;
		}
		if ( (*items)->IsTypeOf( BACKDROP_TYPE ) ) {
			backdrops.AddTail( *items );
			continue;
		}
		if ( (*items)->GetName() != NULL && strcmp( (*items)->GetName(), "BackgroundModel" ) == 0 ) {
			continue;
		}
		if ( !(*items)->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) || ( (DamageableObject *)*items )->GetLODModelName() != NULL ) {
			stream->WriteBool( true );
			saved++;
			if ( !(*items)->WriteToStream( stream ) ) {
				return false;
			}
		}
	}
	sprintf( text, "Saved %d Objects\n", saved );
	TGObject::Log( text );

	sprintf( text, "Saving %d backdrops\n", backdropRoot->GetChildCount() );
	TGObject::Log( text );
	for ( i = backdropRoot->GetChildCount() - 1; i >= 0; i-- ) {
		NiAVObject *child = backdropRoot->GetAt( i );
		NiTListIterator pos = backdrops.GetHeadPos();
		if ( child != NULL ) {
			while ( pos != NULL ) {
				Backdrop *backdrop = (Backdrop *)backdrops.GetNext( pos );
				if ( backdrop->GetNode() == child ) {
					sprintf( text, "Saving backdrop type %s, ID %d\n", backdrop->GetClassName(), backdrop->GetObjID() );
					TGObject::Log( text );
					stream->WriteBool( true );
					if ( !backdrop->WriteToStream( stream ) ) {
						return false;
					}
				}
			}
		}
	}
	stream->WriteBool( false );

	count = pendingObjects.GetCount();
	sprintf( text, "Saving %d New Objects\n", count );
	TGObject::Log( text );
	items = pendingObjects.GetItems();
	stream->WriteLong( count );
	for ( i = 0; i < count; i++ ) {
		if ( !items[i]->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) || ( (DamageableObject *)items[i] )->GetLODModelName() != NULL ) {
			stream->WriteBool( true );
			if ( !items[i]->WriteToStream( stream ) ) {
				return false;
			}
		}
	}

	count = cameras.GetCount();
	sprintf( text, "Saving %d Cameras\n", count );
	TGObject::Log( text );
	CameraObjectClass **cameraItems = cameras.GetItems();
	stream->WriteLong( count );
	for ( i = 0; i < count; i++ ) {
		if ( !cameraItems[i]->WriteToStream( stream ) ) {
			return false;
		}
	}

	count = lights.GetCount();
	sprintf( text, "Saving %d Lights\n", count );
	TGObject::Log( text );
	LightObjectClass **lightItems = lights.GetItems();
	stream->WriteLong( count );
	for ( i = 0; i < count; i++ ) {
		if ( !lightItems[i]->WriteToStream( stream ) ) {
			return false;
		}
	}

	stream->VirtualSlot11( regionModule );
	stream->WriteFloat( unknown90 );
	stream->WriteFloat( unknown94 );
	stream->VirtualSlot17( (long)(NiNode *)unknown98 );
	stream->VirtualSlot17( (long)sceneRoot );
	stream->VirtualSlot17( (long)effectRoot );
	stream->VirtualSlot17( (long)backdropRoot );
	stream->VirtualSlot17( (long)foregroundRoot );
	stream->WriteBool( wireframe->GetWireframe() );
	stream->WriteFloat( glowIntensity );
	stream->WriteFloat( fogColor.r );
	stream->WriteFloat( fogColor.g );
	stream->WriteFloat( fogColor.b );
	stream->WriteID( TGObject::GetSavedID( starSphere ) );
	stream->WriteBool( sceneChanged == true );
	stream->WriteBool( unknownF1 == true );
	if ( proximityManager != NULL ) {
		stream->WriteBool( true );
		proximityManager->VirtualSlot00( stream );
	} else {
		stream->WriteBool( false );
	}
	CameraObjectClass *camera = GetActiveCamera();
	stream->VirtualSlot11( camera != NULL ? camera->GetName() : NULL );
	stream->WriteInt( numSounds );
	for ( i = 0; i < numSounds; i++ ) {
		stream->WriteID( sounds[i].objectID );
		stream->WriteInt( sounds[i].soundID );
	}
	return true;
}

/*
================
SetClass::ReadFromStream

Reads what WriteToStream wrote: the name, the objects (each filed by name
and ID as it loads), the new objects, the cameras and the lights, the
region module, the saved addresses of the nodes (filed in the pointer map
for the fix-up), the wireframe flag, the glow intensity and fog colour, the
star sphere's ID, the proximity manager's objects, the active camera's name
and the sounds. False when an object fails to load.
================
*/
// FUNCTION: STBC 0x00414e90
bool SetClass::ReadFromStream( TGStream *stream ) {
	char text[256];
	int i;
	int count;

	if ( !TGEventHandlerObject::ReadFromStream( stream ) ) {
		return false;
	}
	name = stream->VirtualSlot10( -1 );
	sprintf( text, "Loading set %s\n", name );
	TGObject::Log( text );
	objectsLocked = stream->ReadBool();
	displayName.ReadFromStream( stream );
	unknown138 = NULL;
	unknown134 = 0;
	backgroundModel = stream->VirtualSlot10( -1 );
	if ( backgroundModel != NULL ) {
		VirtualSlot29( stream );
	}
	unknown60 = true;

	sprintf( text, "Loading Objects\n" );
	TGObject::Log( text );
	while ( stream->ReadBool() == true ) {
		ObjectClass *object = (ObjectClass *)TGObject::LoadObject( stream, 0 );
		if ( object == NULL ) {
			return false;
		}
		if ( object->GetName() != NULL ) {
			objectIDs.SetAt( object->GetName(), object->GetObjID() );
		}
		objects.Insert( object );
		objectTable.AddObject( object, this );
		if ( Nebula::Cast( object ) != NULL ) {
			nebula = Nebula::Cast( object );
		}
	}

	count = stream->ReadLong();
	sprintf( text, "Loading %d New Objects\n", count );
	TGObject::Log( text );
	for ( i = 0; i < count; i++ ) {
		ObjectClass *object = (ObjectClass *)TGObject::LoadObject( stream, 0 );
		if ( object == NULL ) {
			return false;
		}
		if ( object->GetName() != NULL ) {
			objectIDs.SetAt( object->GetName(), object->GetObjID() );
		}
		pendingObjects.Insert( object );
		if ( Nebula::Cast( object ) != NULL ) {
			nebula = Nebula::Cast( object );
		}
	}

	count = stream->ReadLong();
	sprintf( text, "Loading %d Cameras\n", count );
	TGObject::Log( text );
	for ( i = 0; i < count; i++ ) {
		CameraObjectClass *camera = (CameraObjectClass *)TGObject::LoadObject( stream, 0 );
		if ( camera == NULL ) {
			return false;
		}
		if ( camera->GetName() != NULL ) {
			objectIDs.SetAt( camera->GetName(), camera->GetObjID() );
		}
		cameras.Insert( camera );
		objectTable.AddObject( camera, this );
	}

	count = stream->ReadLong();
	sprintf( text, "Loading %d Lights\n", count );
	TGObject::Log( text );
	for ( i = 0; i < count; i++ ) {
		LightObjectClass *light = (LightObjectClass *)TGObject::LoadObject( stream, 0 );
		if ( light == NULL ) {
			return false;
		}
		if ( light->GetName() != NULL ) {
			objectIDs.SetAt( light->GetName(), light->GetObjID() );
		}
		lights.Insert( light );
		objectTable.AddObject( light, this );
	}

	regionModule = stream->VirtualSlot10( -1 );
	unknown90 = stream->ReadFloat();
	unknown94 = stream->ReadFloat();
	long savedAddress = stream->VirtualSlot16();
	pointerMap.Add( savedAddress, unknown98 );
	savedAddress = stream->VirtualSlot16();
	pointerMap.Add( savedAddress, sceneRoot );
	savedAddress = stream->VirtualSlot16();
	pointerMap.Add( savedAddress, effectRoot );
	savedAddress = stream->VirtualSlot16();
	pointerMap.Add( savedAddress, backdropRoot );
	savedAddress = stream->VirtualSlot16();
	pointerMap.Add( savedAddress, foregroundRoot );
	wireframe->SetWireframe( stream->ReadBool() );
	glowIntensity = stream->ReadFloat();
	fogColor.r = stream->ReadFloat();
	fogColor.g = stream->ReadFloat();
	fogColor.b = stream->ReadFloat();
	starSphere = (StarSphere *)stream->ReadID();		// the saved ID until FixUpPointers
	sceneChanged = stream->ReadBool();
	unknownF1 = stream->ReadBool();
	if ( stream->ReadBool() == true ) {
		proximityManager = new ProximityManager;
		while ( stream->ReadBool() == true ) {
			proximityManager->AddObject( (ObjectClass *)TGObject::LoadObject( stream, 0 ) );
		}
	} else {
		proximityManager = NULL;
	}
	activeCameraName = stream->VirtualSlot10( -1 );
	numSounds = stream->ReadInt();
	for ( i = 0; i < numSounds; i++ ) {
		sounds[i].objectID = stream->ReadID();
		sounds[i].soundID = stream->ReadInt();
	}
	return true;
}

/*
================
SetClass::VirtualSlot29

Reads the list of strings at 0x134: the count, then each string's length
and bytes.
================
*/
// FUNCTION: STBC 0x00415ad0
bool SetClass::VirtualSlot29( TGStream *stream ) {
	unknown134 = stream->ReadInt();
	if ( unknown134 > 0 ) {
		unknown138 = (char **)malloc( unknown134 * sizeof( char * ) );
	}
	for ( int i = 0; i < unknown134; i++ ) {
		long length = stream->ReadLong();
		unknown138[i] = (char *)malloc( length );
		stream->Read( unknown138[i], length );
	}
	return true;
}

/*
================
SetClass::FixUpPointers

False while the star sphere's ID names no object yet.
================
*/
// FUNCTION: STBC 0x00415b70
bool SetClass::FixUpPointers( void ) {
	if ( !TGEventHandlerObject::FixUpPointers() ) {
		return false;
	}
	if ( starSphere != NULL && GetTGObjectPtr( (long)starSphere ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
SetClass::PostFixUp

Turns the star sphere's ID back into the object, and gives the objects to
the proximity manager.
================
*/
// FUNCTION: STBC 0x00415ba0
void SetClass::PostFixUp( void ) {
	TGEventHandlerObject::PostFixUp();
	starSphere = (StarSphere *)GetSavedObject( (long)starSphere );

	int count = objects.GetCount();
	ObjectClass **items = objects.GetItems();
	int i;
	for ( i = 0; i < count; i++ ) {
		if ( proximityManager != NULL ) {
			proximityManager->AddObject( items[i] );
		}
	}
	count = pendingObjects.GetCount();
	items = pendingObjects.GetItems();
	for ( i = 0; i < count; i++ ) {
		if ( proximityManager != NULL ) {
			proximityManager->AddObject( items[i] );
		}
	}
	// The traces are compiled out of the release build, leaving the counts.
	count = cameras.GetCount();
	for ( i = 0; i < count; i++ ) {
		TGTRACE( ( TGTRACE_TYPE_DEBUG, "SetClass::PostFixUp: camera %d of %d\n", i, count ) );
	}
	count = lights.GetCount();
	for ( i = 0; i < count; i++ ) {
		TGTRACE( ( TGTRACE_TYPE_DEBUG, "SetClass::PostFixUp: light %d of %d\n", i, count ) );
	}
}

/*
================
SetClass::SetFog
================
*/
// FUNCTION: STBC 0x00415c60
void SetClass::SetFog( bool fog_ ) {
	fog->SetFog( fog_ );
}

/*
================
SetClass::SetFogColor

Takes the colour for the fog and passes it to the fog property.
================
*/
// FUNCTION: STBC 0x00415c80
void SetClass::SetFogColor( NiColor *color ) {
	fogColor = *color;
	fog->SetFogColor( fogColor );
}

/*
================
SetClass::VirtualSlot30

After the set is loaded: makes the camera named at activeCameraName
active, brings the scene graph up to date, hands every object, camera and
light the set again, and reloads the background model with the texture
replacements read with it.
================
*/
// FUNCTION: STBC 0x00415cd0
void SetClass::VirtualSlot30( void ) {
	if ( activeCameraName != NULL ) {
		SetActiveCamera( activeCameraName );
		delete [] activeCameraName;
		activeCameraName = NULL;
	}
	unknown98->UpdateEffects();
	unknown98->UpdateProperties();
	unknown98->Update( 0.0f );

	// BUG: redundant: an empty list is built here and destroyed on return without ever being used
	NiTList<ObjectClass *> unused( 2 );

	int i;
	int count = objects.GetCount();
	ObjectClass **items = objects.GetItems();
	for ( i = 0; i < count; i++ ) {
		if ( items[i] != NULL ) {
			items[i]->SetContainingSet( this );
			items[i]->VirtualSlot66();
		}
	}
	count = pendingObjects.GetCount();
	items = pendingObjects.GetItems();
	for ( i = 0; i < count; i++ ) {
		if ( items[i] != NULL ) {
			items[i]->SetContainingSet( this );
			items[i]->VirtualSlot66();
		}
	}
	count = cameras.GetCount();
	CameraObjectClass **cameraItems = cameras.GetItems();
	for ( i = 0; i < count; i++ ) {
		if ( cameraItems[i] != NULL ) {
			cameraItems[i]->SetContainingSet( this );
		}
	}
	count = lights.GetCount();
	LightObjectClass **lightItems = lights.GetItems();
	for ( i = 0; i < count; i++ ) {
		if ( lightItems[i] != NULL ) {
			lightItems[i]->SetContainingSet( this );
			lightItems[i]->VirtualSlot50();
		}
	}

	if ( backgroundModel != NULL ) {
		char *model = backgroundModel;
		backgroundModel = NULL;
		SetBackgroundModel( model, 0.0f, 0.0f, 0.0f );
		delete [] model;
		unknown10C = false;
		// the strings come in pairs: a texture and, after its end, its replacement
		ObjectClass *background = GetObject( "BackgroundModel" );
		for ( i = 0; i < unknown134; i++ ) {
			char *texture = unknown138[i];
			if ( background != NULL ) {
				background->ReplaceTexture( texture, texture + strlen( texture ) + 1 );
			}
			delete [] unknown138[i];
		}
		if ( unknown138 != NULL ) {
			delete [] unknown138;
		}
		unknown138 = NULL;
		unknown134 = 0;
	}
	sceneChanged = true;
}

/*
================
SetClass_GetClassObjectList

The Python method SetClass.GetClassObjectList( set[, type] ): a tuple of
new shadow instances of the set's objects of the class type (1 by
default), leaving out those whose instance is None.
================
*/
// FUNCTION: STBC 0x00415ef0
PyObject *SetClass_GetClassObjectList( PyObject *self, PyObject *args ) {
	PyObject *tuple = NULL;
	int type = 1;
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "Oi", &instance, &type ) != -1 ) {
		SetClass *set = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&set, "SetClass" );
		if ( set != NULL ) {
			int count = 0;
			BaseObjectClass **objects = set->objectTable.GetObjects( type, &count );
			NiTList<PyObject *> instances( 2 );
			for ( int i = 0; i < count; i++ ) {
				PyObject *object = objects[i]->VirtualSlot08();
				if ( object != NULL ) {
					if ( object != Py_None ) {
						instances.AddTail( object );
					} else {
						Py_DECREF( object );
					}
				}
			}
			tuple = PyTuple_New( instances.GetSize() );
			int index = 0;
			while ( !instances.IsEmpty() ) {
				PyTuple_SetItem( tuple, index++, instances.RemoveHead() );
			}
		}
	}
	return tuple;
}

/*
================
SetClass_GetNavPoints

The Python method SetClass.GetNavPoints( set ): a tuple of new shadow
instances of the set's placements that are nav points; None when the
argument is not a set.
================
*/
// FUNCTION: STBC 0x00416140
PyObject *SetClass_GetNavPoints( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	if ( PyArg_ParseTuple( args, "O", &instance ) != -1 ) {
		SetClass *set = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&set, "SetClass" );
		if ( set != NULL ) {
			int count = 0;
			PlacementObject **placements = (PlacementObject **)set->objectTable.GetObjects( PLACEMENTOBJECT_TYPE, &count );
			int numNavPoints = 0;
			int i;
			for ( i = 0; i < count; i++ ) {
				if ( placements[i]->IsNavPoint() ) {
					numNavPoints++;
				}
			}
			PyObject *tuple = PyTuple_New( numNavPoints );
			int index = 0;
			for ( i = 0; i < count; i++ ) {
				if ( placements[i]->IsNavPoint() ) {
					PyTuple_SetItem( tuple, index++, placements[i]->VirtualSlot08() );
				}
			}
			return tuple;
		}
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
SetClass::CalcIsRendered

The set is rendered when it is the rendered set, or when the bridge is and
the bridge's view screen shows the set through its remote camera.
================
*/
// FUNCTION: STBC 0x00416230
void SetClass::CalcIsRendered( void ) {
	if ( g_kSetManager.GetRenderedSet() == this ) {
		rendered = true;
		return;
	}
	BridgeSet *bridge = (BridgeSet *)g_kSetManager.GetSet( "bridge" );
	if ( bridge != NULL && g_kSetManager.GetRenderedSet() == bridge ) {
		ViewScreenObject *viewScreen = bridge->GetViewScreen();
		if ( viewScreen != NULL ) {
			CameraObjectClass *camera = viewScreen->GetRemoteCam();
			if ( camera != NULL && camera->GetContainingSet() == this ) {
				rendered = true;
				return;
			}
		}
	}
	rendered = false;
}

