/*
===============================================================================

	TGTimedAction.cpp

===============================================================================
*/

#include <windows.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGTimedAction.h"
#include "TGActionManager.h"
#include "TGObjPtrEvent.h"
#include "TGStream.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "UtopiaModule.h"
#include <iostream>
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x007018e0 _$E241
// FUNCTION: STBC 0x007018f0 _$E238
// FUNCTION: STBC 0x00701900 _$E240
// FUNCTION: STBC 0x00701910 _$E239
// FUNCTION: STBC 0x00701920 _$E247
// FUNCTION: STBC 0x00701930 _$E244
// FUNCTION: STBC 0x00701940 _$E246
// FUNCTION: STBC 0x00701950 _$E245

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00701960 _$E56
// FUNCTION: STBC 0x00701970 _$E55
// FUNCTION: STBC 0x00701980 _$E212
// FUNCTION: STBC 0x00701990 _$E211
// FUNCTION: STBC 0x007019a0 _$E67
// FUNCTION: STBC 0x007019b0 _$E66
// FUNCTION: STBC 0x007019c0 _$E71
// FUNCTION: STBC 0x007019d0 _$E70
// FUNCTION: STBC 0x007019e0 _$E75
// FUNCTION: STBC 0x007019f0 _$E74
// FUNCTION: STBC 0x00701a00 _$E79
// FUNCTION: STBC 0x00701a10 _$E78
// FUNCTION: STBC 0x00701a20 _$E199
// FUNCTION: STBC 0x00701a30 _$E198
// FUNCTION: STBC 0x00701a40 _$E141
// FUNCTION: STBC 0x00701a50 _$E140
// FUNCTION: STBC 0x00701a60 _$E154
// FUNCTION: STBC 0x00701a70 _$E153
// FUNCTION: STBC 0x00701a80 _$E204
// FUNCTION: STBC 0x00701a90 _$E203
// FUNCTION: STBC 0x00701aa0 _$E217
// FUNCTION: STBC 0x00701ab0 _$E216
// FUNCTION: STBC 0x00701ac0 _$E208
// FUNCTION: STBC 0x00701ad0 _$E207
// FUNCTION: STBC 0x00701ae0 _$E221
// FUNCTION: STBC 0x00701af0 _$E220
// FUNCTION: STBC 0x00701b00 _$E228
// FUNCTION: STBC 0x00701b10 _$E227
// FUNCTION: STBC 0x00701b20 _$E272
// FUNCTION: STBC 0x00701b30 _$E271
// FUNCTION: STBC 0x00701b40 _$E276
// FUNCTION: STBC 0x00701b50 _$E275
// FUNCTION: STBC 0x00701b60 _$E103
// FUNCTION: STBC 0x00701b70 _$E102
// FUNCTION: STBC 0x00701b80 _$E258
// FUNCTION: STBC 0x00701b90 _$E257
// FUNCTION: STBC 0x00701ba0 _$E265
// FUNCTION: STBC 0x00701bb0 _$E264
// FUNCTION: STBC 0x00701bc0 _$E288
// FUNCTION: STBC 0x00701bd0 _$E287
// FUNCTION: STBC 0x00701be0 _$E292
// FUNCTION: STBC 0x00701bf0 _$E291
// FUNCTION: STBC 0x00701c00 _$E332
// FUNCTION: STBC 0x00701c10 _$E331
// FUNCTION: STBC 0x00701c20 _$E338
// FUNCTION: STBC 0x00701c30 _$E337
// FUNCTION: STBC 0x00701c40 _$E280
// FUNCTION: STBC 0x00701c50 _$E279
// FUNCTION: STBC 0x00701c60 _$E284
// FUNCTION: STBC 0x00701c70 _$E283
// FUNCTION: STBC 0x00701c80 _$E296
// FUNCTION: STBC 0x00701c90 _$E295
// FUNCTION: STBC 0x00701ca0 _$E300
// FUNCTION: STBC 0x00701cb0 _$E299
// FUNCTION: STBC 0x00701cc0 _$E304
// FUNCTION: STBC 0x00701cd0 _$E303
// FUNCTION: STBC 0x00701ce0 _$E308
// FUNCTION: STBC 0x00701cf0 _$E307
// FUNCTION: STBC 0x00701d00 _$E312
// FUNCTION: STBC 0x00701d10 _$E311
// FUNCTION: STBC 0x00701d20 _$E316
// FUNCTION: STBC 0x00701d30 _$E315
// FUNCTION: STBC 0x00701d40 _$E320
// FUNCTION: STBC 0x00701d50 _$E319
// FUNCTION: STBC 0x00701d60 _$E324
// FUNCTION: STBC 0x00701d70 _$E323
// FUNCTION: STBC 0x00701d80 _$E108
// FUNCTION: STBC 0x00701d90 _$E107
// FUNCTION: STBC 0x00701da0 _$E328
// FUNCTION: STBC 0x00701db0 _$E327
// FUNCTION: STBC 0x00701dc0 _$E113
// FUNCTION: STBC 0x00701dd0 _$E112
// FUNCTION: STBC 0x00701de0 _$E342
// FUNCTION: STBC 0x00701df0 _$E341
// FUNCTION: STBC 0x00701e00 _$E346
// FUNCTION: STBC 0x00701e10 _$E345
// FUNCTION: STBC 0x00701e20 _$E350
// FUNCTION: STBC 0x00701e30 _$E349
// FUNCTION: STBC 0x00701e40 _$E354
// FUNCTION: STBC 0x00701e50 _$E353
// FUNCTION: STBC 0x00701e60 _$E118
// FUNCTION: STBC 0x00701e70 _$E117
// FUNCTION: STBC 0x00701e80 _$E358
// FUNCTION: STBC 0x00701e90 _$E357
// FUNCTION: STBC 0x00701ea0 _$E362
// FUNCTION: STBC 0x00701eb0 _$E361
// FUNCTION: STBC 0x00701ec0 _$E366
// FUNCTION: STBC 0x00701ed0 _$E365
// FUNCTION: STBC 0x00701ee0 _$E124
// FUNCTION: STBC 0x00701ef0 _$E123
// FUNCTION: STBC 0x00701f00 _$E129
// FUNCTION: STBC 0x00701f10 _$E128
// FUNCTION: STBC 0x00701f20 _$E159
// FUNCTION: STBC 0x00701f30 _$E158
// FUNCTION: STBC 0x00701f40 _$E370
// FUNCTION: STBC 0x00701f50 _$E369
// FUNCTION: STBC 0x00701f60 _$E164
// FUNCTION: STBC 0x00701f70 _$E163
// FUNCTION: STBC 0x00701f80 _$E374
// FUNCTION: STBC 0x00701f90 _$E373
// FUNCTION: STBC 0x00701fa0 _$E378
// FUNCTION: STBC 0x00701fb0 _$E377
// FUNCTION: STBC 0x00701fc0 _$E169
// FUNCTION: STBC 0x00701fd0 _$E168
// FUNCTION: STBC 0x00701fe0 _$E382
// FUNCTION: STBC 0x00701ff0 _$E381
// FUNCTION: STBC 0x00702000 _$E134
// FUNCTION: STBC 0x00702010 _$E133
// FUNCTION: STBC 0x00702020 _$E386
// FUNCTION: STBC 0x00702030 _$E385
// FUNCTION: STBC 0x00702040 _$E390
// FUNCTION: STBC 0x00702050 _$E389
// FUNCTION: STBC 0x00702060 _$E174
// FUNCTION: STBC 0x00702070 _$E173
// FUNCTION: STBC 0x00702080 _$E179
// FUNCTION: STBC 0x00702090 _$E178
// FUNCTION: STBC 0x007020a0 _$E394
// FUNCTION: STBC 0x007020b0 _$E393
// FUNCTION: STBC 0x007020c0 _$E398
// FUNCTION: STBC 0x007020d0 _$E397
// FUNCTION: STBC 0x007020e0 _$E402
// FUNCTION: STBC 0x007020f0 _$E401
// FUNCTION: STBC 0x00702100 _$E406
// FUNCTION: STBC 0x00702110 _$E405
// FUNCTION: STBC 0x00702120 _$E410
// FUNCTION: STBC 0x00702130 _$E409
// FUNCTION: STBC 0x00702140 _$E414
// FUNCTION: STBC 0x00702150 _$E413
// FUNCTION: STBC 0x00702160 _$E418
// FUNCTION: STBC 0x00702170 _$E417
// FUNCTION: STBC 0x00702180 _$E422
// FUNCTION: STBC 0x00702190 _$E421
// FUNCTION: STBC 0x007021a0 _$E184
// FUNCTION: STBC 0x007021b0 _$E183
// FUNCTION: STBC 0x007021c0 _$E426
// FUNCTION: STBC 0x007021d0 _$E425
// FUNCTION: STBC 0x007021e0 _$E189
// FUNCTION: STBC 0x007021f0 _$E188
// FUNCTION: STBC 0x00702200 _$E194
// FUNCTION: STBC 0x00702210 _$E193

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).

// FUNCTION: STBC 0x007025d0 _$E434
// FUNCTION: STBC 0x007025f0 _$E433

/*
================
TGTimedAction::TGTimedAction
================
*/
// FUNCTION: STBC 0x00702220
TGTimedAction::TGTimedAction( long id ) : TGAction( id ) {
	timerID = 0;
	duration = -1.0f;
}

/*
================
TGTimedAction::~TGTimedAction
================
*/
// FUNCTION: STBC 0x00702280
// FUNCTION: STBC 0x00702250 ??_GTGTimedAction@@UAEPAXI@Z
TGTimedAction::~TGTimedAction( void ) {
	if ( timerID != 0 ) {
		if ( useRealTime ) {
			g_kRealtimeTimerManager.DeleteTimer( timerID );
		} else {
			g_kTimerManager.DeleteTimer( timerID );
		}
	}
}

/*
================
TGTimedAction::Play

Starts the timer that completes the action after the duration.
================
*/
// FUNCTION: STBC 0x007022f0
void TGTimedAction::Play( void ) {
	if ( duration < 0.0f ) {
		Completed();
		return;
	}

	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetEventType( ET_ACTION_COMPLETED );
	event->SetDestination( &g_kTGActionManager );
	event->SetObjPtr( this );

	TGTimer *timer = new TGTimer;
	if ( useRealTime ) {
		timer->SetTimerStart( g_kUtopiaModule.GetRealTime() + duration );
	} else {
		timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + duration );
	}
	timer->SetEvent( event );
	timerID = timer->GetObjID();
	if ( useRealTime ) {
		g_kRealtimeTimerManager.AddTimer( timer );
	} else {
		g_kTimerManager.AddTimer( timer );
	}

	TGAction::Play();
}

/*
================
TGTimedAction::Restart

As Play, as if started delay seconds ago; the duration is not checked.
================
*/
// FUNCTION: STBC 0x007023e0
void TGTimedAction::Restart( float delay ) {
	TGObjPtrEvent *event = new TGObjPtrEvent;
	event->SetEventType( ET_ACTION_COMPLETED );
	event->SetDestination( &g_kTGActionManager );
	event->SetObjPtr( this );

	TGTimer *timer = new TGTimer;
	if ( useRealTime ) {
		timer->SetTimerStart( g_kUtopiaModule.GetRealTime() + ( duration - delay ) );
	} else {
		timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + ( duration - delay ) );
	}
	timer->SetEvent( event );
	timerID = timer->GetObjID();
	if ( useRealTime ) {
		g_kRealtimeTimerManager.AddTimer( timer );
	} else {
		g_kTimerManager.AddTimer( timer );
	}

	TGAction::Play();
}

/*
================
TGTimedAction::Completed
================
*/
// FUNCTION: STBC 0x007024c0
void TGTimedAction::Completed( void ) {
	if ( timerID != 0 ) {
		if ( useRealTime ) {
			g_kRealtimeTimerManager.DeleteTimer( timerID );
		} else {
			g_kTimerManager.DeleteTimer( timerID );
		}
		timerID = 0;
	}
	TGAction::Completed();
}

/*
================
TGTimedAction::Skip
================
*/
// FUNCTION: STBC 0x00702500
void TGTimedAction::Skip( void ) {
	if ( timerID != 0 ) {
		if ( useRealTime ) {
			g_kRealtimeTimerManager.DeleteTimer( timerID );
		} else {
			g_kTimerManager.DeleteTimer( timerID );
		}
		timerID = 0;
	}
	TGAction::Skip();
}

/*
================
TGTimedAction::WriteToStream
================
*/
// FUNCTION: STBC 0x00702540
bool TGTimedAction::WriteToStream( TGStream *stream ) {
	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( duration );
	stream->WriteID( timerID );
	stream->WriteBool( useRealTime );
	return true;
}

/*
================
TGTimedAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x00702590
bool TGTimedAction::ReadFromStream( TGStream *stream ) {
	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}
	duration = stream->ReadFloat();
	timerID = stream->ReadID();
	useRealTime = stream->ReadBool();
	return true;
}
