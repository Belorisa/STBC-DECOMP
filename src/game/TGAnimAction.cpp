/*
===============================================================================

	TGAnimAction.cpp

===============================================================================
*/

#include <windows.h>
#include <string.h>

// <windows.h>'s ANSI/wide macro would rename TGObject's method.
#undef GetClassName

#include "TGAnimAction.h"
#include "TGAnimationManagerClass.h"
#include "TGPointerMap.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include <iostream>
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00703650 _$E266
// FUNCTION: STBC 0x00703660 _$E263
// FUNCTION: STBC 0x00703670 _$E265
// FUNCTION: STBC 0x00703680 _$E264
// FUNCTION: STBC 0x00703690 _$E272
// FUNCTION: STBC 0x007036a0 _$E269
// FUNCTION: STBC 0x007036b0 _$E271
// FUNCTION: STBC 0x007036c0 _$E270

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007036d0 _$E28
// FUNCTION: STBC 0x007036e0 _$E27
// FUNCTION: STBC 0x007036f0 _$E39
// FUNCTION: STBC 0x00703700 _$E38
// FUNCTION: STBC 0x00703710 _$E43
// FUNCTION: STBC 0x00703720 _$E42
// FUNCTION: STBC 0x00703730 _$E47
// FUNCTION: STBC 0x00703740 _$E46
// FUNCTION: STBC 0x00703750 _$E51
// FUNCTION: STBC 0x00703760 _$E50
// FUNCTION: STBC 0x00703770 _$E175
// FUNCTION: STBC 0x00703780 _$E174
// FUNCTION: STBC 0x00703790 _$E180
// FUNCTION: STBC 0x007037a0 _$E179
// FUNCTION: STBC 0x007037b0 _$E184
// FUNCTION: STBC 0x007037c0 _$E183
// FUNCTION: STBC 0x007037d0 _$E237
// FUNCTION: STBC 0x007037e0 _$E236
// FUNCTION: STBC 0x007037f0 _$E117
// FUNCTION: STBC 0x00703800 _$E116
// FUNCTION: STBC 0x00703810 _$E130
// FUNCTION: STBC 0x00703820 _$E129
// FUNCTION: STBC 0x00703830 _$E242
// FUNCTION: STBC 0x00703840 _$E241
// FUNCTION: STBC 0x00703850 _$E246
// FUNCTION: STBC 0x00703860 _$E245
// FUNCTION: STBC 0x00703870 _$E253
// FUNCTION: STBC 0x00703880 _$E252
// FUNCTION: STBC 0x00703890 _$E297
// FUNCTION: STBC 0x007038a0 _$E296
// FUNCTION: STBC 0x007038b0 _$E301
// FUNCTION: STBC 0x007038c0 _$E300
// FUNCTION: STBC 0x007038d0 _$E75
// FUNCTION: STBC 0x007038e0 _$E74
// FUNCTION: STBC 0x007038f0 _$E283
// FUNCTION: STBC 0x00703900 _$E282
// FUNCTION: STBC 0x00703910 _$E290
// FUNCTION: STBC 0x00703920 _$E289
// FUNCTION: STBC 0x00703930 _$E313
// FUNCTION: STBC 0x00703940 _$E312
// FUNCTION: STBC 0x00703950 _$E317
// FUNCTION: STBC 0x00703960 _$E316
// FUNCTION: STBC 0x00703970 _$E357
// FUNCTION: STBC 0x00703980 _$E356
// FUNCTION: STBC 0x00703990 _$E363
// FUNCTION: STBC 0x007039a0 _$E362
// FUNCTION: STBC 0x007039b0 _$E305
// FUNCTION: STBC 0x007039c0 _$E304
// FUNCTION: STBC 0x007039d0 _$E309
// FUNCTION: STBC 0x007039e0 _$E308
// FUNCTION: STBC 0x007039f0 _$E321
// FUNCTION: STBC 0x00703a00 _$E320
// FUNCTION: STBC 0x00703a10 _$E325
// FUNCTION: STBC 0x00703a20 _$E324
// FUNCTION: STBC 0x00703a30 _$E329
// FUNCTION: STBC 0x00703a40 _$E328
// FUNCTION: STBC 0x00703a50 _$E333
// FUNCTION: STBC 0x00703a60 _$E332
// FUNCTION: STBC 0x00703a70 _$E337
// FUNCTION: STBC 0x00703a80 _$E336
// FUNCTION: STBC 0x00703a90 _$E341
// FUNCTION: STBC 0x00703aa0 _$E340
// FUNCTION: STBC 0x00703ab0 _$E345
// FUNCTION: STBC 0x00703ac0 _$E344
// FUNCTION: STBC 0x00703ad0 _$E349
// FUNCTION: STBC 0x00703ae0 _$E348
// FUNCTION: STBC 0x00703af0 _$E80
// FUNCTION: STBC 0x00703b00 _$E79
// FUNCTION: STBC 0x00703b10 _$E353
// FUNCTION: STBC 0x00703b20 _$E352
// FUNCTION: STBC 0x00703b30 _$E85
// FUNCTION: STBC 0x00703b40 _$E84
// FUNCTION: STBC 0x00703b50 _$E367
// FUNCTION: STBC 0x00703b60 _$E366
// FUNCTION: STBC 0x00703b70 _$E371
// FUNCTION: STBC 0x00703b80 _$E370
// FUNCTION: STBC 0x00703b90 _$E375
// FUNCTION: STBC 0x00703ba0 _$E374
// FUNCTION: STBC 0x00703bb0 _$E379
// FUNCTION: STBC 0x00703bc0 _$E378
// FUNCTION: STBC 0x00703bd0 _$E94
// FUNCTION: STBC 0x00703be0 _$E93
// FUNCTION: STBC 0x00703bf0 _$E383
// FUNCTION: STBC 0x00703c00 _$E382
// FUNCTION: STBC 0x00703c10 _$E387
// FUNCTION: STBC 0x00703c20 _$E386
// FUNCTION: STBC 0x00703c30 _$E391
// FUNCTION: STBC 0x00703c40 _$E390
// FUNCTION: STBC 0x00703c50 _$E100
// FUNCTION: STBC 0x00703c60 _$E99
// FUNCTION: STBC 0x00703c70 _$E105
// FUNCTION: STBC 0x00703c80 _$E104
// FUNCTION: STBC 0x00703c90 _$E135
// FUNCTION: STBC 0x00703ca0 _$E134
// FUNCTION: STBC 0x00703cb0 _$E395
// FUNCTION: STBC 0x00703cc0 _$E394
// FUNCTION: STBC 0x00703cd0 _$E140
// FUNCTION: STBC 0x00703ce0 _$E139
// FUNCTION: STBC 0x00703cf0 _$E399
// FUNCTION: STBC 0x00703d00 _$E398
// FUNCTION: STBC 0x00703d10 _$E403
// FUNCTION: STBC 0x00703d20 _$E402
// FUNCTION: STBC 0x00703d30 _$E145
// FUNCTION: STBC 0x00703d40 _$E144
// FUNCTION: STBC 0x00703d50 _$E407
// FUNCTION: STBC 0x00703d60 _$E406
// FUNCTION: STBC 0x00703d70 _$E110
// FUNCTION: STBC 0x00703d80 _$E109
// FUNCTION: STBC 0x00703d90 _$E411
// FUNCTION: STBC 0x00703da0 _$E410
// FUNCTION: STBC 0x00703db0 _$E415
// FUNCTION: STBC 0x00703dc0 _$E414
// FUNCTION: STBC 0x00703dd0 _$E150
// FUNCTION: STBC 0x00703de0 _$E149
// FUNCTION: STBC 0x00703df0 _$E155
// FUNCTION: STBC 0x00703e00 _$E154
// FUNCTION: STBC 0x00703e10 _$E419
// FUNCTION: STBC 0x00703e20 _$E418
// FUNCTION: STBC 0x00703e30 _$E423
// FUNCTION: STBC 0x00703e40 _$E422
// FUNCTION: STBC 0x00703e50 _$E427
// FUNCTION: STBC 0x00703e60 _$E426
// FUNCTION: STBC 0x00703e70 _$E431
// FUNCTION: STBC 0x00703e80 _$E430
// FUNCTION: STBC 0x00703e90 _$E435
// FUNCTION: STBC 0x00703ea0 _$E434
// FUNCTION: STBC 0x00703eb0 _$E439
// FUNCTION: STBC 0x00703ec0 _$E438
// FUNCTION: STBC 0x00703ed0 _$E443
// FUNCTION: STBC 0x00703ee0 _$E442
// FUNCTION: STBC 0x00703ef0 _$E447
// FUNCTION: STBC 0x00703f00 _$E446
// FUNCTION: STBC 0x00703f10 _$E160
// FUNCTION: STBC 0x00703f20 _$E159
// FUNCTION: STBC 0x00703f30 _$E451
// FUNCTION: STBC 0x00703f40 _$E450
// FUNCTION: STBC 0x00703f50 _$E165
// FUNCTION: STBC 0x00703f60 _$E164
// FUNCTION: STBC 0x00703f70 _$E170
// FUNCTION: STBC 0x00703f80 _$E169

// The unit's static initialisers and exit-time registrations (<iostream> and header statics).

// FUNCTION: STBC 0x00704580 _$E459
// FUNCTION: STBC 0x007045a0 _$E458

// The longest time a blended animation takes to blend in, and the share of
// a shorter action's duration it takes.
#define MAX_BLEND_TIME			0.34f
#define BLEND_TIME_FRACTION		0.75f

/*
================
TGAnimAction::TGAnimAction

The duration is the animation's length.
================
*/
// FUNCTION: STBC 0x00703f90 ??0TGAnimAction@@QAE@PAVTGAnimNode@@PADHHHH@Z
TGAnimAction::TGAnimAction( TGAnimNode *node_, char *animation_, int exclusive_, int unknown36_, int useDefault_, int blend_ ) {
	node = node_;
	animation = NULL;
	if ( animation_ != NULL ) {
		animation = new char[strlen( animation_ ) + 1];
		strcpy( animation, animation_ );
	}
	exclusive = exclusive_ != 0;
	useDefault = useDefault_ != 0;
	unknown36 = unknown36_ != 0;
	blend = blend_ != 0;
	duration = g_kAnimationManager.GetAnimationLength( animation );
	startTime = 0.0f;
}

/*
================
TGAnimAction::~TGAnimAction
================
*/
// FUNCTION: STBC 0x007040a0
TGAnimAction::~TGAnimAction( void ) {
	delete animation;
}

/*
================
TGAnimAction::AnimationDone
================
*/
// FUNCTION: STBC 0x00704120
void TGAnimAction::AnimationDone( long id ) {
	TGAnimAction *action = (TGAnimAction *)TGObject::GetSavedObject( id );
	if ( action != NULL ) {
		action->Completed();
	}
}

/*
================
TGAnimAction::Play

An animation the manager has not loaded completes at once. A timed action
also completes after its length, as given.
================
*/
// FUNCTION: STBC 0x00704140
void TGAnimAction::Play( void ) {
	float length = duration;
	if ( !g_kAnimationManager.IsLoaded( animation ) ) {
		Completed();
		return;
	}

	if ( duration <= 0.0f ) {
		duration = g_kAnimationManager.GetAnimationLength( animation );
	}
	if ( duration <= 0.0f ) {
		return;
	}

	float now = g_kUtopiaModule.GetGameTime();
	startTime = now;
	if ( blend ) {
		if ( length < MAX_BLEND_TIME && length > 0.0f ) {
			node->blendTime = length * BLEND_TIME_FRACTION;
		} else {
			node->blendTime = MAX_BLEND_TIME;
		}

		if ( !exclusive ) {
			node->SetNonExclusiveAnimation( animation, startTime, unknown36, AnimationDone, id );
		} else if ( useDefault ) {
			node->SetExclusiveAnimationUseDefault( animation, startTime, unknown36, AnimationDone, id );
		} else {
			node->SetExclusiveAnimation( animation, startTime, unknown36, AnimationDone, id );
		}
		duration = g_kAnimationManager.GetAnimationLength( animation );
	} else {
		node->UseAnimation( animation, exclusive, now, 0, AnimationDone, id );
	}

	if ( length > 0.0f && unknown36 == true ) {
		duration = length;
		TGTimedAction::Play();
	}
}

/*
================
TGAnimAction::Restart

As Play, started delay seconds ago.
================
*/
// FUNCTION: STBC 0x007042d0
void TGAnimAction::Restart( float delay ) {
	float length = duration;
	if ( duration <= 0.0f ) {
		duration = g_kAnimationManager.GetAnimationLength( animation );
	}
	if ( duration <= 0.0f ) {
		return;
	}

	float start = g_kUtopiaModule.GetGameTime() - delay;
	startTime = start;
	if ( blend ) {
		if ( !exclusive ) {
			node->SetNonExclusiveAnimation( animation, start, unknown36, AnimationDone, id );
		} else if ( useDefault ) {
			node->SetExclusiveAnimationUseDefault( animation, start, unknown36, AnimationDone, id );
		} else {
			node->SetExclusiveAnimation( animation, start, unknown36, AnimationDone, id );
		}
		duration = g_kAnimationManager.GetAnimationLength( animation );
	} else {
		node->UseAnimation( animation, exclusive, start, 0, AnimationDone, id );
	}

	if ( length > 0.0f && unknown36 == true ) {
		TGTimedAction::Restart( delay );
	}
}

/*
================
TGAnimAction::Completed
================
*/
// FUNCTION: STBC 0x00704400
void TGAnimAction::Completed( void ) {
	TGTimedAction::Completed();
}

/*
================
TGAnimAction::Skip
================
*/
// FUNCTION: STBC 0x00704410
void TGAnimAction::Skip( void ) {
	Completed();
}

/*
================
TGAnimAction::WriteToStream
================
*/
// FUNCTION: STBC 0x00704420
bool TGAnimAction::WriteToStream( TGStream *stream ) {
	if ( !TGTimedAction::WriteToStream( stream ) ) {
		return false;
	}

	stream->VirtualSlot17( (long)(TGAnimNode *)node );
	stream->VirtualSlot11( animation );
	stream->WriteBool( exclusive );
	stream->WriteBool( useDefault );
	stream->WriteBool( unknown36 );
	stream->WriteBool( blend );
	return true;
}

/*
================
TGAnimAction::ReadFromStream
================
*/
// FUNCTION: STBC 0x00704480
bool TGAnimAction::ReadFromStream( TGStream *stream ) {
	if ( !TGTimedAction::ReadFromStream( stream ) ) {
		return false;
	}

	savedNode = stream->VirtualSlot16();
	animation = stream->VirtualSlot10( -1 );
	exclusive = stream->ReadBool();
	useDefault = stream->ReadBool();
	unknown36 = stream->ReadBool();
	blend = stream->ReadBool();
	return true;
}

/*
================
TGAnimAction::FixUpPointers
================
*/
// FUNCTION: STBC 0x007044e0
bool TGAnimAction::FixUpPointers( void ) {
	return TGTimedAction::FixUpPointers() ? true : false;
}

/*
================
TGAnimAction::PostFixUp
================
*/
// FUNCTION: STBC 0x007044f0
void TGAnimAction::PostFixUp( void ) {
	TGTimedAction::PostFixUp();
	node = (TGAnimNode *)pointerMap.Find( savedNode );
}

/*
================
TGAnimAction::PlaysOn

Whether the action plays on the node.
================
*/
// FUNCTION: STBC 0x00704570
bool TGAnimAction::PlaysOn( TGAnimNode *node_ ) {
	return ( node_ == node ) ? true : false;
}
