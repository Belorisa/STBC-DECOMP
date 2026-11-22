/*
===============================================================================

	BridgeSet.cpp

===============================================================================
*/

#include <iostream>

#include <string.h>

#include "BridgeSet.h"
#include "ViewScreenObject.h"
#include "CameraObjectClass.h"
#include "TGStream.h"
#include "VarManagerClass.h"
#include "TGEventHandlerTable.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00666130 _$E66
// FUNCTION: STBC 0x00666140 _$E65
// FUNCTION: STBC 0x00666150 _$E236
// FUNCTION: STBC 0x00666160 _$E235
// FUNCTION: STBC 0x00666170 _$E77
// FUNCTION: STBC 0x00666180 _$E76
// FUNCTION: STBC 0x00666190 _$E81
// FUNCTION: STBC 0x006661a0 _$E80
// FUNCTION: STBC 0x006661b0 _$E85
// FUNCTION: STBC 0x006661c0 _$E84
// FUNCTION: STBC 0x006661d0 _$E89
// FUNCTION: STBC 0x006661e0 _$E88
// FUNCTION: STBC 0x006661f0 _$E206
// FUNCTION: STBC 0x00666200 _$E205
// FUNCTION: STBC 0x00666210 _$E148
// FUNCTION: STBC 0x00666220 _$E147
// FUNCTION: STBC 0x00666230 _$E161
// FUNCTION: STBC 0x00666240 _$E160
// FUNCTION: STBC 0x00666250 _$E211
// FUNCTION: STBC 0x00666260 _$E210
// FUNCTION: STBC 0x00666270 _$E241
// FUNCTION: STBC 0x00666280 _$E240
// FUNCTION: STBC 0x00666290 _$E215
// FUNCTION: STBC 0x006662a0 _$E214
// FUNCTION: STBC 0x006662b0 _$E245
// FUNCTION: STBC 0x006662c0 _$E244
// FUNCTION: STBC 0x006662d0 _$E252
// FUNCTION: STBC 0x006662e0 _$E251
// FUNCTION: STBC 0x006662f0 _$E274
// FUNCTION: STBC 0x00666300 _$E273
// FUNCTION: STBC 0x00666310 _$E278
// FUNCTION: STBC 0x00666320 _$E277
// FUNCTION: STBC 0x00666330 _$E106
// FUNCTION: STBC 0x00666340 _$E105
// FUNCTION: STBC 0x00666350 _$E260
// FUNCTION: STBC 0x00666360 _$E259
// FUNCTION: STBC 0x00666370 _$E267
// FUNCTION: STBC 0x00666380 _$E266
// FUNCTION: STBC 0x00666390 _$E290
// FUNCTION: STBC 0x006663a0 _$E289
// FUNCTION: STBC 0x006663b0 _$E294
// FUNCTION: STBC 0x006663c0 _$E293
// FUNCTION: STBC 0x006663d0 _$E334
// FUNCTION: STBC 0x006663e0 _$E333
// FUNCTION: STBC 0x006663f0 _$E340
// FUNCTION: STBC 0x00666400 _$E339
// FUNCTION: STBC 0x00666410 _$E282
// FUNCTION: STBC 0x00666420 _$E281
// FUNCTION: STBC 0x00666430 _$E286
// FUNCTION: STBC 0x00666440 _$E285
// FUNCTION: STBC 0x00666450 _$E298
// FUNCTION: STBC 0x00666460 _$E297
// FUNCTION: STBC 0x00666470 _$E302
// FUNCTION: STBC 0x00666480 _$E301
// FUNCTION: STBC 0x00666490 _$E306
// FUNCTION: STBC 0x006664a0 _$E305
// FUNCTION: STBC 0x006664b0 _$E310
// FUNCTION: STBC 0x006664c0 _$E309
// FUNCTION: STBC 0x006664d0 _$E314
// FUNCTION: STBC 0x006664e0 _$E313
// FUNCTION: STBC 0x006664f0 _$E318
// FUNCTION: STBC 0x00666500 _$E317
// FUNCTION: STBC 0x00666510 _$E322
// FUNCTION: STBC 0x00666520 _$E321
// FUNCTION: STBC 0x00666530 _$E326
// FUNCTION: STBC 0x00666540 _$E325
// FUNCTION: STBC 0x00666550 _$E111
// FUNCTION: STBC 0x00666560 _$E110
// FUNCTION: STBC 0x00666570 _$E330
// FUNCTION: STBC 0x00666580 _$E329
// FUNCTION: STBC 0x00666590 _$E116
// FUNCTION: STBC 0x006665a0 _$E115
// FUNCTION: STBC 0x006665b0 _$E344
// FUNCTION: STBC 0x006665c0 _$E343
// FUNCTION: STBC 0x006665d0 _$E348
// FUNCTION: STBC 0x006665e0 _$E347
// FUNCTION: STBC 0x006665f0 _$E352
// FUNCTION: STBC 0x00666600 _$E351
// FUNCTION: STBC 0x00666610 _$E356
// FUNCTION: STBC 0x00666620 _$E355
// FUNCTION: STBC 0x00666630 _$E125
// FUNCTION: STBC 0x00666640 _$E124
// FUNCTION: STBC 0x00666650 _$E360
// FUNCTION: STBC 0x00666660 _$E359
// FUNCTION: STBC 0x00666670 _$E364
// FUNCTION: STBC 0x00666680 _$E363
// FUNCTION: STBC 0x00666690 _$E368
// FUNCTION: STBC 0x006666a0 _$E367
// FUNCTION: STBC 0x006666b0 _$E131
// FUNCTION: STBC 0x006666c0 _$E130
// FUNCTION: STBC 0x006666d0 _$E136
// FUNCTION: STBC 0x006666e0 _$E135
// FUNCTION: STBC 0x006666f0 _$E166
// FUNCTION: STBC 0x00666700 _$E165
// FUNCTION: STBC 0x00666710 _$E372
// FUNCTION: STBC 0x00666720 _$E371
// FUNCTION: STBC 0x00666730 _$E171
// FUNCTION: STBC 0x00666740 _$E170
// FUNCTION: STBC 0x00666750 _$E376
// FUNCTION: STBC 0x00666760 _$E375
// FUNCTION: STBC 0x00666770 _$E380
// FUNCTION: STBC 0x00666780 _$E379
// FUNCTION: STBC 0x006667c0 _$E176
// FUNCTION: STBC 0x006667d0 _$E175
// FUNCTION: STBC 0x006667e0 _$E384
// FUNCTION: STBC 0x006667f0 _$E383
// FUNCTION: STBC 0x00666800 _$E141
// FUNCTION: STBC 0x00666810 _$E140
// FUNCTION: STBC 0x00666820 _$E388
// FUNCTION: STBC 0x00666830 _$E387
// FUNCTION: STBC 0x00666840 _$E392
// FUNCTION: STBC 0x00666850 _$E391
// FUNCTION: STBC 0x00666860 _$E181
// FUNCTION: STBC 0x00666870 _$E180
// FUNCTION: STBC 0x00666880 _$E186
// FUNCTION: STBC 0x00666890 _$E185
// FUNCTION: STBC 0x006668a0 _$E396
// FUNCTION: STBC 0x006668b0 _$E395
// FUNCTION: STBC 0x006668c0 _$E400
// FUNCTION: STBC 0x006668d0 _$E399
// FUNCTION: STBC 0x006668e0 _$E404
// FUNCTION: STBC 0x006668f0 _$E403
// FUNCTION: STBC 0x00666900 _$E408
// FUNCTION: STBC 0x00666910 _$E407
// FUNCTION: STBC 0x00666920 _$E412
// FUNCTION: STBC 0x00666930 _$E411
// FUNCTION: STBC 0x00666940 _$E416
// FUNCTION: STBC 0x00666950 _$E415
// FUNCTION: STBC 0x00666960 _$E420
// FUNCTION: STBC 0x00666970 _$E419
// FUNCTION: STBC 0x00666980 _$E424
// FUNCTION: STBC 0x00666990 _$E423
// FUNCTION: STBC 0x006669a0 _$E191
// FUNCTION: STBC 0x006669b0 _$E190
// FUNCTION: STBC 0x006669c0 _$E428
// FUNCTION: STBC 0x006669d0 _$E427
// FUNCTION: STBC 0x006669e0 _$E196
// FUNCTION: STBC 0x006669f0 _$E195
// FUNCTION: STBC 0x00666a00 _$E201
// FUNCTION: STBC 0x00666a10 _$E200
// FUNCTION: STBC 0x00666a20 _$E432
// FUNCTION: STBC 0x00666a30 _$E431
// FUNCTION: STBC 0x00666a40 _$E437
// FUNCTION: STBC 0x00666a50 _$E436
// FUNCTION: STBC 0x00666a60 _$E441
// FUNCTION: STBC 0x00666a70 _$E440

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006660b0 _$E32
// FUNCTION: STBC 0x006660c0 _$E29
// FUNCTION: STBC 0x006660d0 _$E31
// FUNCTION: STBC 0x006660e0 _$E30
// FUNCTION: STBC 0x006660f0 _$E38
// FUNCTION: STBC 0x00666100 _$E35
// FUNCTION: STBC 0x00666110 _$E37
// FUNCTION: STBC 0x00666120 _$E36
// FUNCTION: STBC 0x00666790 _$E455
// FUNCTION: STBC 0x006667b0 _$E454

// The camera mode VirtualSlot26 puts the active camera in while the view
// screen shows it (the value of CameraObjectClass's 0xE4; the meaning is open).
#define VIEWSCREEN_CAMERA_MODE	4

// The class handler table's construction and its destruction at exit.
// FUNCTION: STBC 0x00665c70 _$E447
// FUNCTION: STBC 0x00665c80 _$E443
// FUNCTION: STBC 0x00665c90 _$E446
// DATA: STBC 0x00991ad8
TGEventHandlerTable BridgeSet::classHandlers;

/*
================
BridgeSet::BridgeSet
================
*/
// FUNCTION: STBC 0x00665cc0
BridgeSet::BridgeSet( long id ) : SetClass( id ) {
	unknown10C = false;
	glowIntensity = 0.0f;
	viewScreen = NULL;
	config = NULL;
}

/*
================
BridgeSet::GetObjType
================
*/
// FUNCTION: STBC 0x00665d00
int BridgeSet::GetObjType( void ) {
	return BRIDGESET_TYPE;
}

/*
================
BridgeSet::IsTypeOf
================
*/
// FUNCTION: STBC 0x00665d10
bool BridgeSet::IsTypeOf( int type ) {
	if ( type == BRIDGESET_TYPE ) {
		return true;
	}
	return SetClass::IsTypeOf( type );
}

/*
================
BridgeSet::GetClassName
================
*/
// FUNCTION: STBC 0x00665d60
char *BridgeSet::GetClassName( void ) {
	return "BridgeSet";
}

/*
================
BridgeSet::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00665d70
char *BridgeSet::GetSwigTypeName( void ) {
	return "_p_BridgeSet";
}

/*
================
BridgeSet::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00665d80
char *BridgeSet::GetPythonClassName( void ) {
	return "BridgeSetPtr";
}

/*
================
BridgeSet::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00665d90
TGEventHandlerTable *BridgeSet::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
BridgeSet::~BridgeSet
================
*/
// FUNCTION: STBC 0x00665dd0
BridgeSet::~BridgeSet( void ) {
	if ( config != NULL ) {
		delete config;
	}
}

/*
================
BridgeSet::Cast

The set as a BridgeSet; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00665e00 ?Cast@BridgeSet@@SAPAV1@PAVSetClass@@@Z
BridgeSet *BridgeSet::Cast( SetClass *set ) {
	if ( set != NULL && set->IsTypeOf( BRIDGESET_TYPE ) ) {
		return (BridgeSet *)set;
	}
	return NULL;
}

/*
================
BridgeSet::SetConfig

Keeps a copy of the configuration name.
================
*/
// FUNCTION: STBC 0x00665e30 ?SetConfig@BridgeSet@@QAEXPAD@Z
void BridgeSet::SetConfig( char *config_ ) {
	if ( config != NULL ) {
		delete [] config;
	}
	config = new char[strlen( config_ ) + 1];
	strcpy( config, config_ );
}

/*
================
BridgeSet::IsSameConfig
================
*/
// FUNCTION: STBC 0x00665e90 ?IsSameConfig@BridgeSet@@QAE_NPAD@Z
bool BridgeSet::IsSameConfig( char *config_ ) {
	if ( config_ != NULL && config != NULL ) {
		return strcmp( config_, config ) == 0;
	}
	return false;
}

/*
================
BridgeSet::SetViewScreen

Puts the view screen in the set under the name.
================
*/
// FUNCTION: STBC 0x00665f00 ?SetViewScreen@BridgeSet@@QAEXPAVViewScreenObject@@PAD@Z
void BridgeSet::SetViewScreen( ViewScreenObject *viewScreen_, char *name_ ) {
	viewScreen = viewScreen_;
	AddObjectToSet( viewScreen_, name_ );
}

/*
================
BridgeSet::VirtualSlot26

Renders the set through its active camera, in the view screen's camera mode
while the view screen shows that camera.
================
*/
// FUNCTION: STBC 0x00665f20
void BridgeSet::VirtualSlot26( void ) {
	CameraObjectClass *camera = GetActiveCamera();
	if ( camera != NULL ) {
		bool onViewScreen = viewScreen != NULL && viewScreen->Unknown678A80( camera );
		int mode = camera->unknownE4;
		if ( onViewScreen ) {
			camera->unknownE4 = VIEWSCREEN_CAMERA_MODE;
		}
		SetClass::VirtualSlot25( camera->GetNiCamera(), false, true, true );
		camera->unknownE4 = mode;
	}
}

/*
================
BridgeSet::WriteToStream

The set, then the view screen's ID, the configuration name and the mouse
lock setting.
================
*/
// FUNCTION: STBC 0x00665f80
bool BridgeSet::WriteToStream( TGStream *stream ) {
	if ( !SetClass::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( GetSavedID( viewScreen ) );
	stream->VirtualSlot11( config );
	float lockMouse = g_kVarManager.GetFloatVariable( "global", "fLockMouse" );
	stream->WriteFloat( lockMouse );
	return true;
}

/*
================
BridgeSet::ReadFromStream

Reads what WriteToStream wrote; the view screen stays an ID until PostFixUp.
================
*/
// FUNCTION: STBC 0x00665ff0
bool BridgeSet::ReadFromStream( TGStream *stream ) {
	config = NULL;
	if ( !SetClass::ReadFromStream( stream ) ) {
		return false;
	}
	viewScreen = (ViewScreenObject *)stream->ReadID();
	config = stream->VirtualSlot10( -1 );
	g_kVarManager.SetFloatVariable( "global", "fLockMouse", stream->ReadFloat() );
	return true;
}

/*
================
BridgeSet::FixUpPointers

False while the view screen's ID names no object yet.
================
*/
// FUNCTION: STBC 0x00666060
bool BridgeSet::FixUpPointers( void ) {
	if ( !SetClass::FixUpPointers() ) {
		return false;
	}
	if ( viewScreen != NULL && GetTGObjectPtr( (long)viewScreen ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
BridgeSet::PostFixUp

Turns the view screen's ID back into the object.
================
*/
// FUNCTION: STBC 0x00666090
void BridgeSet::PostFixUp( void ) {
	SetClass::PostFixUp();
	viewScreen = (ViewScreenObject *)GetTGObjectPtr( (long)viewScreen );
}
