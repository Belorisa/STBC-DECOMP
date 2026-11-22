/*
===============================================================================

	ChangeRenderedSetAction.cpp

	The unit lays out its code, then its <iostream> quads and NetImmerse
	registrations (0x401450-0x40276F, with the template copies it emits
	among them), then TGAction's inline virtuals (0x402770): the first
	object of the link emits those copies, and LoadEpisodeAction's run
	follows LoadEpisodeAction's code (0x402920), as LoadMissionAction's
	follows its own ahead of Episode's quad (0x403FE0).

===============================================================================
*/

#include <string.h>

#include "ChangeRenderedSetAction.h"
#include "PyEmbed.h"
#include "SetManager.h"
#include "TGProfilingInfo.h"
#include "TGStream.h"
#include "TGPlayerList.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale id pair.
// FUNCTION: STBC 0x00401450 _$E245
// FUNCTION: STBC 0x00401460 _$E242
// FUNCTION: STBC 0x00401470 _$E244
// FUNCTION: STBC 0x00401480 _$E243
// FUNCTION: STBC 0x00401490 _$E251
// FUNCTION: STBC 0x004014a0 _$E248
// FUNCTION: STBC 0x004014b0 _$E250
// FUNCTION: STBC 0x004014c0 _$E249
// FUNCTION: STBC 0x00402300 _$E468
// FUNCTION: STBC 0x00402320 _$E467

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004014d0 _$E75
// FUNCTION: STBC 0x004014e0 _$E74
// FUNCTION: STBC 0x004014f0 _$E255
// FUNCTION: STBC 0x00401500 _$E254
// FUNCTION: STBC 0x00401510 _$E86
// FUNCTION: STBC 0x00401520 _$E85
// FUNCTION: STBC 0x00401530 _$E90
// FUNCTION: STBC 0x00401540 _$E89
// FUNCTION: STBC 0x00401550 _$E94
// FUNCTION: STBC 0x00401560 _$E93
// FUNCTION: STBC 0x00401570 _$E98
// FUNCTION: STBC 0x00401580 _$E97
// FUNCTION: STBC 0x00401590 _$E215
// FUNCTION: STBC 0x004015a0 _$E214
// FUNCTION: STBC 0x004015b0 _$E157
// FUNCTION: STBC 0x004015c0 _$E156
// FUNCTION: STBC 0x004015d0 _$E170
// FUNCTION: STBC 0x004015e0 _$E169
// FUNCTION: STBC 0x004015f0 _$E220
// FUNCTION: STBC 0x00401600 _$E219
// FUNCTION: STBC 0x00401610 _$E260
// FUNCTION: STBC 0x00401620 _$E259
// FUNCTION: STBC 0x00401760 _$E224
// FUNCTION: STBC 0x00401770 _$E223
// FUNCTION: STBC 0x00401d50 _$E264
// FUNCTION: STBC 0x00401d60 _$E263
// FUNCTION: STBC 0x00401df0 _$E271
// FUNCTION: STBC 0x00401e00 _$E270
// FUNCTION: STBC 0x00401e10 _$E293
// FUNCTION: STBC 0x00401e20 _$E292
// FUNCTION: STBC 0x00401f50 _$E297
// FUNCTION: STBC 0x00401f60 _$E296
// FUNCTION: STBC 0x00401f70 _$E115
// FUNCTION: STBC 0x00401f80 _$E114
// FUNCTION: STBC 0x00401f90 _$E279
// FUNCTION: STBC 0x00401fa0 _$E278
// FUNCTION: STBC 0x00402040 _$E286
// FUNCTION: STBC 0x00402050 _$E285
// FUNCTION: STBC 0x00402060 _$E309
// FUNCTION: STBC 0x00402070 _$E308
// FUNCTION: STBC 0x00402080 _$E313
// FUNCTION: STBC 0x00402090 _$E312
// FUNCTION: STBC 0x004020a0 _$E353
// FUNCTION: STBC 0x004020b0 _$E352
// FUNCTION: STBC 0x004020c0 _$E359
// FUNCTION: STBC 0x004020d0 _$E358
// FUNCTION: STBC 0x004020e0 _$E301
// FUNCTION: STBC 0x004020f0 _$E300
// FUNCTION: STBC 0x00402100 _$E305
// FUNCTION: STBC 0x00402110 _$E304
// FUNCTION: STBC 0x00402120 _$E317
// FUNCTION: STBC 0x00402130 _$E316
// FUNCTION: STBC 0x00402140 _$E321
// FUNCTION: STBC 0x00402150 _$E320
// FUNCTION: STBC 0x00402160 _$E325
// FUNCTION: STBC 0x00402170 _$E324
// FUNCTION: STBC 0x00402180 _$E329
// FUNCTION: STBC 0x00402190 _$E328
// FUNCTION: STBC 0x004021a0 _$E333
// FUNCTION: STBC 0x004021b0 _$E332
// FUNCTION: STBC 0x004021c0 _$E337
// FUNCTION: STBC 0x004021d0 _$E336
// FUNCTION: STBC 0x004021e0 _$E341
// FUNCTION: STBC 0x004021f0 _$E340
// FUNCTION: STBC 0x00402200 _$E345
// FUNCTION: STBC 0x00402210 _$E344
// FUNCTION: STBC 0x00402220 _$E120
// FUNCTION: STBC 0x00402230 _$E119
// FUNCTION: STBC 0x00402240 _$E349
// FUNCTION: STBC 0x00402250 _$E348
// FUNCTION: STBC 0x00402260 _$E125
// FUNCTION: STBC 0x00402270 _$E124
// FUNCTION: STBC 0x00402280 _$E363
// FUNCTION: STBC 0x00402290 _$E362
// FUNCTION: STBC 0x004022a0 _$E367
// FUNCTION: STBC 0x004022b0 _$E366
// FUNCTION: STBC 0x004022c0 _$E371
// FUNCTION: STBC 0x004022d0 _$E370
// FUNCTION: STBC 0x004022e0 _$E375
// FUNCTION: STBC 0x004022f0 _$E374
// FUNCTION: STBC 0x00402340 _$E134
// FUNCTION: STBC 0x00402350 _$E133
// FUNCTION: STBC 0x00402360 _$E379
// FUNCTION: STBC 0x00402370 _$E378
// FUNCTION: STBC 0x00402380 _$E383
// FUNCTION: STBC 0x00402390 _$E382
// FUNCTION: STBC 0x004023a0 _$E387
// FUNCTION: STBC 0x004023b0 _$E386
// FUNCTION: STBC 0x004023c0 _$E140
// FUNCTION: STBC 0x004023d0 _$E139
// FUNCTION: STBC 0x004023e0 _$E145
// FUNCTION: STBC 0x004023f0 _$E144
// FUNCTION: STBC 0x00402400 _$E175
// FUNCTION: STBC 0x00402410 _$E174
// FUNCTION: STBC 0x00402420 _$E391
// FUNCTION: STBC 0x00402430 _$E390
// FUNCTION: STBC 0x00402440 _$E180
// FUNCTION: STBC 0x00402450 _$E179
// FUNCTION: STBC 0x00402460 _$E395
// FUNCTION: STBC 0x00402470 _$E394
// FUNCTION: STBC 0x00402480 _$E399
// FUNCTION: STBC 0x00402490 _$E398
// FUNCTION: STBC 0x004024a0 _$E185
// FUNCTION: STBC 0x004024b0 _$E184
// FUNCTION: STBC 0x004024c0 _$E403
// FUNCTION: STBC 0x004024d0 _$E402
// FUNCTION: STBC 0x004024e0 _$E150
// FUNCTION: STBC 0x004024f0 _$E149
// FUNCTION: STBC 0x00402500 _$E407
// FUNCTION: STBC 0x00402510 _$E406
// FUNCTION: STBC 0x00402520 _$E411
// FUNCTION: STBC 0x00402530 _$E410
// FUNCTION: STBC 0x00402540 _$E190
// FUNCTION: STBC 0x00402550 _$E189
// FUNCTION: STBC 0x00402560 _$E195
// FUNCTION: STBC 0x00402570 _$E194
// FUNCTION: STBC 0x00402580 _$E415
// FUNCTION: STBC 0x00402590 _$E414
// FUNCTION: STBC 0x004025a0 _$E419
// FUNCTION: STBC 0x004025b0 _$E418
// FUNCTION: STBC 0x004025c0 _$E423
// FUNCTION: STBC 0x004025d0 _$E422
// FUNCTION: STBC 0x004025e0 _$E427
// FUNCTION: STBC 0x004025f0 _$E426
// FUNCTION: STBC 0x00402600 _$E431
// FUNCTION: STBC 0x00402610 _$E430
// FUNCTION: STBC 0x00402620 _$E435
// FUNCTION: STBC 0x00402630 _$E434
// FUNCTION: STBC 0x00402640 _$E439
// FUNCTION: STBC 0x00402650 _$E438
// FUNCTION: STBC 0x00402660 _$E443
// FUNCTION: STBC 0x00402670 _$E442
// FUNCTION: STBC 0x00402680 _$E200
// FUNCTION: STBC 0x00402690 _$E199
// FUNCTION: STBC 0x004026a0 _$E447
// FUNCTION: STBC 0x004026b0 _$E446
// FUNCTION: STBC 0x004026c0 _$E205
// FUNCTION: STBC 0x004026d0 _$E204
// FUNCTION: STBC 0x004026e0 _$E210
// FUNCTION: STBC 0x004026f0 _$E209
// FUNCTION: STBC 0x00402710 _$E451
// FUNCTION: STBC 0x00402720 _$E450
// FUNCTION: STBC 0x00402730 _$E456
// FUNCTION: STBC 0x00402740 _$E455
// FUNCTION: STBC 0x00402750 _$E460
// FUNCTION: STBC 0x00402760 _$E459

/*
================
ChangeRenderedSetAction::ChangeRenderedSetAction

Renders the set given: keeps its name.
================
*/
// FUNCTION: STBC 0x00401000 ??0ChangeRenderedSetAction@@QAE@PAVSetClass@@@Z
ChangeRenderedSetAction::ChangeRenderedSetAction( SetClass *set ) {
	setName = NULL;
	SetName( set->GetName() );
	moduleName = NULL;
}

/*
================
ChangeRenderedSetAction::GetObjType
================
*/
// FUNCTION: STBC 0x00401070
int ChangeRenderedSetAction::GetObjType( void ) {
	return CHANGERENDEREDSETACTION_TYPE;
}

/*
================
ChangeRenderedSetAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x00401080
bool ChangeRenderedSetAction::IsTypeOf( int type ) {
	if ( type == CHANGERENDEREDSETACTION_TYPE ) {
		return true;
	}
	return TGAction::IsTypeOf( type );
}

/*
================
ChangeRenderedSetAction::GetClassName
================
*/
// FUNCTION: STBC 0x004010b0
char *ChangeRenderedSetAction::GetClassName( void ) {
	return "ChangeRenderedSetAction";
}

/*
================
ChangeRenderedSetAction::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004010c0
char *ChangeRenderedSetAction::GetSwigTypeName( void ) {
	return "_p_ChangeRenderedSetAction";
}

/*
================
ChangeRenderedSetAction::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004010d0
char *ChangeRenderedSetAction::GetPythonClassName( void ) {
	return "ChangeRenderedSetActionPtr";
}

/*
================
ChangeRenderedSetAction::ChangeRenderedSetAction

Renders the set the module's GetSet returns: keeps the module's name.
================
*/
// FUNCTION: STBC 0x00401110 ??0ChangeRenderedSetAction@@QAE@PAD@Z
ChangeRenderedSetAction::ChangeRenderedSetAction( char *module ) : setName( NULL ) {
	moduleName = new char[strlen( module ) + 1];
	strcpy( moduleName, module );
}

/*
================
ChangeRenderedSetAction::ChangeRenderedSetAction

The action the loader makes before reading it from the stream.
================
*/
// FUNCTION: STBC 0x004011a0 ??0ChangeRenderedSetAction@@QAE@J@Z
ChangeRenderedSetAction::ChangeRenderedSetAction( long id ) : TGAction( id ) {
	setName = NULL;
	moduleName = NULL;
}

/*
================
ChangeRenderedSetAction::~ChangeRenderedSetAction
================
*/
// FUNCTION: STBC 0x004011d0
ChangeRenderedSetAction::~ChangeRenderedSetAction( void ) {
	delete moduleName;
	delete setName;
}

/*
================
ChangeRenderedSetAction::SetName

Keeps a copy of the name of the set to render.
================
*/
// FUNCTION: STBC 0x00401200
void ChangeRenderedSetAction::SetName( char *name ) {
	delete setName;
	if ( name != NULL ) {
		setName = new char[strlen( name ) + 1];
		strcpy( setName, name );
	} else {
		setName = NULL;
	}
}

/*
================
ChangeRenderedSetAction::Play

Renders the set of the name, else the set the module's GetSet returns,
and completes.
================
*/
// FUNCTION: STBC 0x00401270
// PARKED: register allocation: retail keeps this in EDI and the set in ESI, this build the reverse (an if/else for the set, the PyObject local at the top, SetManager::GetSet against sets.Get tried); calls, literals and control flow match
void ChangeRenderedSetAction::Play( void ) {
	TGProfilingInfo profile( "ChangeRenderedSetAction::Play()" );

	SetClass *set = NULL;
	if ( setName != NULL ) {
		set = g_kSetManager.GetSet( setName );
	}
	if ( set == NULL && moduleName != NULL ) {
		PyObject *result;
		PyEmbed::CallFunction( moduleName, "GetSet", "O", &result, "" );
		set = (SetClass *)PyEmbed::GetObjectPointer( result );
		Py_DECREF( result );
	}
	if ( set != NULL ) {
		g_kSetManager.MakeRenderedSet( set );
	}

	TGAction::Play();
	Completed();
}

/*
================
ChangeRenderedSetAction::WriteToStream
================
*/
// FUNCTION: STBC 0x004013b0
bool ChangeRenderedSetAction::WriteToStream( TGStream *stream ) {
	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( setName );
	stream->VirtualSlot11( moduleName );
	return true;
}

/*
================
ChangeRenderedSetAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x004013f0
bool ChangeRenderedSetAction::ReadFromStream( TGStream *stream ) {
	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}
	setName = stream->VirtualSlot10( -1 );
	moduleName = stream->VirtualSlot10( -1 );
	return true;
}

/*
	SortedArray<TGNetPlayer, int>'s explicit instantiation. Retail keeps every
	member of it out of line, unreferenced ones included, among this object's
	static initialisers (0x401630-0x401DB2), which only an explicit
	instantiation emits; that retail kept this unit's copies (the link's first
	object) is the evidence for placing it here. Which file of the original held
	it is not known (user's ruling, 2026-11-22: written here with this comment).
*/
template class SortedArray<TGNetPlayer, int>;
