/*
===============================================================================

	TGAnimBlender.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGAnimBlender.h"
#include "TGAnimationManagerClass.h"
#include "TGAnimNode.h"
#include "UtopiaModule.h"
#include "NiKeyframeController.h"
#include "NiVisController.h"
#include "NiNode.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>):
// retail lays them out after the unit's code, then the registrations with
// the locale-id pair among them. Their <iostream> bytes (0x9978A0) and the
// class's rtti (0x997970) lie among the registrations' stores.
// FUNCTION: STBC 0x006c7290 _$E32
// FUNCTION: STBC 0x006c72a0 _$E29
// FUNCTION: STBC 0x006c72b0 _$E31
// FUNCTION: STBC 0x006c72c0 _$E30
// FUNCTION: STBC 0x006c72d0 _$E38
// FUNCTION: STBC 0x006c72e0 _$E35
// FUNCTION: STBC 0x006c72f0 _$E37
// FUNCTION: STBC 0x006c7300 _$E36

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006c74c0 _$E475
// FUNCTION: STBC 0x006c74e0 _$E474

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006c7310 _$E45
// FUNCTION: STBC 0x006c7320 _$E44
// FUNCTION: STBC 0x006c7330 _$E56
// FUNCTION: STBC 0x006c7340 _$E55
// FUNCTION: STBC 0x006c7350 _$E234
// FUNCTION: STBC 0x006c7360 _$E233
// FUNCTION: STBC 0x006c7370 _$E216
// FUNCTION: STBC 0x006c7380 _$E215
// FUNCTION: STBC 0x006c7390 _$E225
// FUNCTION: STBC 0x006c73a0 _$E224
// FUNCTION: STBC 0x006c73b0 _$E238
// FUNCTION: STBC 0x006c73c0 _$E237
// FUNCTION: STBC 0x006c7480 _$E60
// FUNCTION: STBC 0x006c7490 _$E59
// FUNCTION: STBC 0x006c74a0 _$E64
// FUNCTION: STBC 0x006c74b0 _$E63
// FUNCTION: STBC 0x006c74f0 _$E68
// FUNCTION: STBC 0x006c7500 _$E67
// FUNCTION: STBC 0x006c7510 _$E192
// FUNCTION: STBC 0x006c7520 _$E191
// FUNCTION: STBC 0x006c7530 _$E197
// FUNCTION: STBC 0x006c7540 _$E196
// FUNCTION: STBC 0x006c7550 _$E201
// FUNCTION: STBC 0x006c7560 _$E200
// FUNCTION: STBC 0x006c7570 _$E274
// FUNCTION: STBC 0x006c7580 _$E273
// FUNCTION: STBC 0x006c7590 _$E278
// FUNCTION: STBC 0x006c75a0 _$E277
// FUNCTION: STBC 0x006c75b0 _$E282
// FUNCTION: STBC 0x006c75c0 _$E281
// FUNCTION: STBC 0x006c75d0 _$E246
// FUNCTION: STBC 0x006c75e0 _$E245
// FUNCTION: STBC 0x006c75f0 _$E134
// FUNCTION: STBC 0x006c7600 _$E133
// FUNCTION: STBC 0x006c7610 _$E147
// FUNCTION: STBC 0x006c7620 _$E146
// FUNCTION: STBC 0x006c7630 _$E251
// FUNCTION: STBC 0x006c7640 _$E250
// FUNCTION: STBC 0x006c7650 _$E255
// FUNCTION: STBC 0x006c7660 _$E254
// FUNCTION: STBC 0x006c7670 _$E262
// FUNCTION: STBC 0x006c7680 _$E261
// FUNCTION: STBC 0x006c7690 _$E307
// FUNCTION: STBC 0x006c76a0 _$E306
// FUNCTION: STBC 0x006c76b0 _$E311
// FUNCTION: STBC 0x006c76c0 _$E310
// FUNCTION: STBC 0x006c76d0 _$E92
// FUNCTION: STBC 0x006c76e0 _$E91
// FUNCTION: STBC 0x006c76f0 _$E293
// FUNCTION: STBC 0x006c7700 _$E292
// FUNCTION: STBC 0x006c7710 _$E300
// FUNCTION: STBC 0x006c7720 _$E299
// FUNCTION: STBC 0x006c7730 _$E323
// FUNCTION: STBC 0x006c7740 _$E322
// FUNCTION: STBC 0x006c7750 _$E327
// FUNCTION: STBC 0x006c7760 _$E326
// FUNCTION: STBC 0x006c7770 _$E367
// FUNCTION: STBC 0x006c7780 _$E366
// FUNCTION: STBC 0x006c7790 _$E373
// FUNCTION: STBC 0x006c77a0 _$E372
// FUNCTION: STBC 0x006c77b0 _$E315
// FUNCTION: STBC 0x006c77c0 _$E314
// FUNCTION: STBC 0x006c77d0 _$E319
// FUNCTION: STBC 0x006c77e0 _$E318
// FUNCTION: STBC 0x006c77f0 _$E331
// FUNCTION: STBC 0x006c7800 _$E330
// FUNCTION: STBC 0x006c7810 _$E335
// FUNCTION: STBC 0x006c7820 _$E334
// FUNCTION: STBC 0x006c7830 _$E339
// FUNCTION: STBC 0x006c7840 _$E338
// FUNCTION: STBC 0x006c7850 _$E343
// FUNCTION: STBC 0x006c7860 _$E342
// FUNCTION: STBC 0x006c7870 _$E347
// FUNCTION: STBC 0x006c7880 _$E346
// FUNCTION: STBC 0x006c7890 _$E351
// FUNCTION: STBC 0x006c78a0 _$E350
// FUNCTION: STBC 0x006c78b0 _$E355
// FUNCTION: STBC 0x006c78c0 _$E354
// FUNCTION: STBC 0x006c78d0 _$E359
// FUNCTION: STBC 0x006c78e0 _$E358
// FUNCTION: STBC 0x006c78f0 _$E97
// FUNCTION: STBC 0x006c7900 _$E96
// FUNCTION: STBC 0x006c7910 _$E363
// FUNCTION: STBC 0x006c7920 _$E362
// FUNCTION: STBC 0x006c7930 _$E102
// FUNCTION: STBC 0x006c7940 _$E101
// FUNCTION: STBC 0x006c7950 _$E377
// FUNCTION: STBC 0x006c7960 _$E376
// FUNCTION: STBC 0x006c7970 _$E381
// FUNCTION: STBC 0x006c7980 _$E380
// FUNCTION: STBC 0x006c7990 _$E385
// FUNCTION: STBC 0x006c79a0 _$E384
// FUNCTION: STBC 0x006c79b0 _$E389
// FUNCTION: STBC 0x006c79c0 _$E388
// FUNCTION: STBC 0x006c79d0 _$E111
// FUNCTION: STBC 0x006c79e0 _$E110
// FUNCTION: STBC 0x006c79f0 _$E393
// FUNCTION: STBC 0x006c7a00 _$E392
// FUNCTION: STBC 0x006c7a10 _$E397
// FUNCTION: STBC 0x006c7a20 _$E396
// FUNCTION: STBC 0x006c7a30 _$E401
// FUNCTION: STBC 0x006c7a40 _$E400
// FUNCTION: STBC 0x006c7a50 _$E117
// FUNCTION: STBC 0x006c7a60 _$E116
// FUNCTION: STBC 0x006c7a70 _$E122
// FUNCTION: STBC 0x006c7a80 _$E121
// FUNCTION: STBC 0x006c7a90 _$E152
// FUNCTION: STBC 0x006c7aa0 _$E151
// FUNCTION: STBC 0x006c7ab0 _$E405
// FUNCTION: STBC 0x006c7ac0 _$E404
// FUNCTION: STBC 0x006c7ad0 _$E157
// FUNCTION: STBC 0x006c7ae0 _$E156
// FUNCTION: STBC 0x006c7af0 _$E409
// FUNCTION: STBC 0x006c7b00 _$E408
// FUNCTION: STBC 0x006c7b10 _$E413
// FUNCTION: STBC 0x006c7b20 _$E412
// FUNCTION: STBC 0x006c7b30 _$E162
// FUNCTION: STBC 0x006c7b40 _$E161
// FUNCTION: STBC 0x006c7b50 _$E417
// FUNCTION: STBC 0x006c7b60 _$E416
// FUNCTION: STBC 0x006c7b70 _$E127
// FUNCTION: STBC 0x006c7b80 _$E126
// FUNCTION: STBC 0x006c7b90 _$E421
// FUNCTION: STBC 0x006c7ba0 _$E420
// FUNCTION: STBC 0x006c7bb0 _$E425
// FUNCTION: STBC 0x006c7bc0 _$E424
// FUNCTION: STBC 0x006c7bd0 _$E167
// FUNCTION: STBC 0x006c7be0 _$E166
// FUNCTION: STBC 0x006c7bf0 _$E172
// FUNCTION: STBC 0x006c7c00 _$E171
// FUNCTION: STBC 0x006c7c10 _$E429
// FUNCTION: STBC 0x006c7c20 _$E428
// FUNCTION: STBC 0x006c7c30 _$E433
// FUNCTION: STBC 0x006c7c40 _$E432
// FUNCTION: STBC 0x006c7c50 _$E437
// FUNCTION: STBC 0x006c7c60 _$E436
// FUNCTION: STBC 0x006c7c70 _$E441
// FUNCTION: STBC 0x006c7c80 _$E440
// FUNCTION: STBC 0x006c7c90 _$E445
// FUNCTION: STBC 0x006c7ca0 _$E444
// FUNCTION: STBC 0x006c7cb0 _$E449
// FUNCTION: STBC 0x006c7cc0 _$E448
// FUNCTION: STBC 0x006c7cd0 _$E453
// FUNCTION: STBC 0x006c7ce0 _$E452
// FUNCTION: STBC 0x006c7cf0 _$E457
// FUNCTION: STBC 0x006c7d00 _$E456
// FUNCTION: STBC 0x006c7d10 _$E177
// FUNCTION: STBC 0x006c7d20 _$E176
// FUNCTION: STBC 0x006c7d30 _$E461
// FUNCTION: STBC 0x006c7d40 _$E460
// FUNCTION: STBC 0x006c7d50 _$E182
// FUNCTION: STBC 0x006c7d60 _$E181
// FUNCTION: STBC 0x006c7d70 _$E187
// FUNCTION: STBC 0x006c7d80 _$E186

// The frames of a track's blend per second of blend time.
#define BLEND_FRAMES_PER_SECOND		15.0f

// The object's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006c6320 _$E467
// FUNCTION: STBC 0x006c6330 _$E463
// FUNCTION: STBC 0x006c6350 _$E466
// FUNCTION: STBC 0x006c6360 _$E464
// DATA: STBC 0x997970 ?rtti@TGAnimBlender@@2VNiRTTI@@B
const NiRTTI TGAnimBlender::rtti( "TGAnimBlender", &NiTimeController::rtti );

/*
================
TGAnimBlender::TargetIsRequiredType
================
*/
// FUNCTION: STBC 0x006c6380 ?TargetIsRequiredType@TGAnimBlender@@UBE_NXZ
bool TGAnimBlender::TargetIsRequiredType() const {
	if ( !target ) {
		return false;
	}
	return target->IsKindOf( &NiAVObject::rtti );
}

/*
================
TGAnimBlender::GetTracks
================
*/
// FUNCTION: STBC 0x006c63b0
TGAnimTrack **TGAnimBlender::GetTracks( void ) {
	return tracks.GetItems();
}

/*
================
TGAnimBlender::GetTrackCount
================
*/
// FUNCTION: STBC 0x006c63c0
int TGAnimBlender::GetTrackCount( void ) {
	return tracks.GetCount();
}

/*
================
TGAnimBlender::SetPlaying

Notes the game time of the change.
================
*/
// FUNCTION: STBC 0x006c63d0
void TGAnimBlender::SetPlaying( bool playing_ ) {
	playing = playing_;
	startTime = g_kUtopiaModule.GetGameTime();
}

/*
================
TGAnimBlender::VirtualSlot21
================
*/
// FUNCTION: STBC 0x006c63f0
const NiRTTI *TGAnimBlender::VirtualSlot21() {
	return &NiAVObject::rtti;
}

/*
================
TGAnimBlender::TGAnimBlender
================
*/
// FUNCTION: STBC 0x006c6400
TGAnimBlender::TGAnimBlender() {
	animationName = NULL;
	Init();
}

/*
================
TGAnimBlender::Init
================
*/
// FUNCTION: STBC 0x006c6460
void TGAnimBlender::Init( void ) {
	playing = false;
	blendTime = 0.34f;
	rootNode = NULL;
	SetAnimationName( NULL );
	unknown55 = false;
	exclusive = true;
	useDefault = false;
	blend = true;
	startTime = g_kUtopiaModule.GetGameTime();
	unknown48 = 0.0f;
	callback = NULL;
	id = 0;
}

/*
================
TGAnimBlender::~TGAnimBlender

Calls back if the animation was still running.
================
*/
// FUNCTION: STBC 0x006c64b0
TGAnimBlender::~TGAnimBlender() {
	DoCallback();
	if ( animationName != NULL ) {
		delete [] animationName;
	}
}

/*
================
TGAnimBlender::CreateClone
================
*/
// FUNCTION: STBC 0x006c6590
NiObject *TGAnimBlender::CreateClone() {
	TGAnimBlender *clone = new TGAnimBlender;
	CopyMembers( clone );
	return clone;
}

/*
================
TGAnimBlender::CopyMembers
================
*/
// FUNCTION: STBC 0x006c65e0
void TGAnimBlender::CopyMembers( TGAnimBlender *dest ) {
	NiTimeController::CopyMembers( dest );
	dest->CopyFrom( this );
}

/*
================
TGAnimBlender::CopyFrom

Takes the source's settings, builds the tracks of the target and copies
the source's tracks into them.
================
*/
// FUNCTION: STBC 0x006c6600
void TGAnimBlender::CopyFrom( TGAnimBlender *source ) {
	playing = source->playing;
	blendTime = source->blendTime;
	unknown55 = source->unknown55;
	exclusive = source->exclusive;
	useDefault = source->useDefault;
	blend = source->blend;
	rootNode = source->rootNode;
	startTime = source->startTime;
	unknown48 = source->unknown48;
	if ( source->animationName != NULL ) {
		SetAnimationName( source->animationName );
	}
	if ( target && target->IsKindOf( &NiNode::rtti ) ) {
		AddNode( (NiNode *)target );
	}

	unsigned int count = source->GetTrackCount();
	TGAnimTrack **sourceTracks = source->GetTracks();
	TGAnimTrack **destTracks = tracks.GetItems();
	for ( unsigned int i = 0; i < count; i++ ) {
		destTracks[i]->CopyFrom( sourceTracks[i] );
	}
}

/*
================
TGAnimBlender::SetCallback
================
*/
// FUNCTION: STBC 0x006c66b0
void TGAnimBlender::SetCallback( TGAnimCallbackPtr callback_, long id_ ) {
	callback = callback_;
	id = id_;
}

/*
================
TGAnimBlender::Update
================
*/
// FUNCTION: STBC 0x006c66d0
void TGAnimBlender::Update( float time ) {
	if ( playing ) {
		UpdateTracks( time );
	}
}

/*
================
TGAnimBlender::UpdateTracks

Steps every track; once all of them are done, calls back and stops
playing.
================
*/
// FUNCTION: STBC 0x006c66f0
void TGAnimBlender::UpdateTracks( float time ) {
	bool done = true;
	int count = tracks.GetCount();
	TGAnimTrack **items = tracks.GetItems();
	for ( int i = 0; i < count; i++ ) {
		items[i]->Update( time, this );
		done &= items[i]->IsDone();
	}
	if ( done ) {
		DoCallback();
		SetPlaying( false );
	}
}

/*
================
TGAnimBlender::Start

Plays the blender's animation the way it was last started.
================
*/
// FUNCTION: STBC 0x006c6750
void TGAnimBlender::Start( float time ) {
	NiTimeController::Start( time );
	if ( blend ) {
		if ( exclusive ) {
			if ( useDefault ) {
				SetExclusiveAnimationUseDefault( animationName, time, unknown55 );
			} else {
				SetExclusiveAnimation( animationName, time, unknown55 );
			}
		} else {
			SetNonExclusiveAnimation( animationName, time, unknown55 );
		}
	} else {
		UseAnimation( animationName, exclusive, time, 0.0f );
	}
}

/*
================
TGAnimBlender::VirtualSlot20
================
*/
// FUNCTION: STBC 0x006c67d0
void TGAnimBlender::VirtualSlot20() {
	Start( g_kUtopiaModule.GetGameTime() );
}

/*
================
TGAnimBlender::Stop

Stops the controllers of every track and marks the tracks done.
================
*/
// FUNCTION: STBC 0x006c67f0
void TGAnimBlender::Stop() {
	NiTimeController::Stop();
	int count = tracks.GetCount();
	TGAnimTrack **items = tracks.GetItems();
	for ( int i = 0; i < count; i++ ) {
		if ( items[i]->keyframeController ) {
			items[i]->keyframeController->Stop();
		}
		if ( items[i]->visController ) {
			items[i]->visController->Stop();
		}
		items[i]->SetDone( true );
	}
	SetPlaying( false );
}

/*
================
TGAnimBlender::SetAnimationName
================
*/
// FUNCTION: STBC 0x006c6840
void TGAnimBlender::SetAnimationName( char *name ) {
	if ( animationName != NULL ) {
		delete [] animationName;
		animationName = NULL;
	}
	if ( name != NULL ) {
		int length = strlen( name );
		animationName = new char[length + 1];
		strcpy( animationName, name );
		animationName[length] = '\0';
	}
}

/*
================
TGAnimBlender::UseAnimation
================
*/
// FUNCTION: STBC 0x006c68c0
void TGAnimBlender::UseAnimation( char *animation, bool flag, float start, float end ) {
	if ( g_kAnimationManager.UseAnimation( this, animation, flag, start, end ) ) {
		playing = true;
	} else {
		playing = false;
	}
}

/*
================
TGAnimBlender::SetTarget

Builds the tracks of a node target and notes a TGAnimNode's root node.
================
*/
// FUNCTION: STBC 0x006c6900
void TGAnimBlender::SetTarget( NiObjectNET *target_ ) {
	NiTimeController::SetTarget( target_ );
	if ( target_ && target_->IsKindOf( &NiNode::rtti ) ) {
		AddNode( (NiNode *)target_ );
	}
	TGAnimNode *animNode = NiDynamicCast( TGAnimNode, target_ );
	if ( animNode ) {
		rootNode = animNode->GetRootNode();
	} else {
		rootNode = NULL;
	}
}

/*
================
TGAnimBlender::AddNode

Makes a track for the object if it has a keyframe or visibility
controller, set to clamp and run at the application's time, then does the
same for the children of a node, but not for animated nodes, which have
blenders of their own.
================
*/
// FUNCTION: STBC 0x006c6980
void TGAnimBlender::AddNode( NiAVObject *object ) {
	TGAnimTrack *track = NULL;

	NiKeyframeController *keyframeController = FindKeyframeController( object );
	if ( keyframeController ) {
		keyframeController->SetCycleType( CLAMP );
		keyframeController->SetAnimType( APP_TIME );
		keyframeController->SetFrequency( 1.0f );
		track = new TGAnimTrack;
		track->SetKeyframeController( keyframeController );
		track->SetName( object->GetName() );
		track->SetFrameCount( GetBlendTime() * BLEND_FRAMES_PER_SECOND );
		tracks.Insert( track );
	}

	NiVisController *visController = FindVisController( object );
	if ( visController ) {
		visController->SetCycleType( CLAMP );
		visController->SetAnimType( APP_TIME );
		visController->SetFrequency( 1.0f );
		visController->Update( 0.0f );
		if ( track == NULL ) {
			track = new TGAnimTrack;
			track->SetName( object->GetName() );
			track->SetFrameCount( GetBlendTime() * BLEND_FRAMES_PER_SECOND );
			tracks.Insert( track );
		}
		track->SetVisController( visController );
	}

	if ( object && object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child && child->IsKindOf( &NiAVObject::rtti ) && !child->IsKindOf( &TGAnimNode::rtti ) ) {
				AddNode( child );
			}
		}
	}
}

/*
================
TGAnimBlender::RemoveNode

Deletes the object's track and those of the nodes under it.
================
*/
// FUNCTION: STBC 0x006c6d60
void TGAnimBlender::RemoveNode( NiAVObject *object ) {
	NiKeyframeController *keyframeController = FindKeyframeController( object );
	NiVisController *visController = FindVisController( object );
	if ( keyframeController || visController ) {
		tracks.Delete( object->GetName() );
	}

	if ( object && object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child && child->IsKindOf( &NiAVObject::rtti ) && !child->IsKindOf( &TGAnimNode::rtti ) ) {
				RemoveNode( child );
			}
		}
	}
}

/*
================
TGAnimBlender::ClearAnimations

Empties the keys of every track's controllers.
================
*/
// FUNCTION: STBC 0x006c6eb0
void TGAnimBlender::ClearAnimations( void ) {
	int count = tracks.GetCount();
	TGAnimTrack **items = tracks.GetItems();
	for ( int i = 0; i < count; i++ ) {
		NiKeyframeController *keyframeController = items[i]->keyframeController;
		if ( keyframeController ) {
			keyframeController->ReplaceRotAnim( NULL, 0, NiRotKey::NOINTERP, true );
			keyframeController->ReplacePosAnim( NULL, 0, NiPosKey::NOINTERP, true );
			keyframeController->ReplaceScaleAnim( NULL, 0, NiFloatKey::NOINTERP, true );
		}
		NiVisController *visController = items[i]->visController;
		if ( visController ) {
			visController->ReplaceKeys( NULL, 0, true );
		}
	}
}

/*
================
TGAnimBlender::SetExclusiveAnimation
================
*/
// FUNCTION: STBC 0x006c6f20
void TGAnimBlender::SetExclusiveAnimation( char *animation, float time, bool flag ) {
	if ( BlendAnimation( animation, time, blendTime, flag, true, false ) ) {
		playing = true;
	} else {
		playing = false;
	}
}

/*
================
TGAnimBlender::SetNonExclusiveAnimation
================
*/
// FUNCTION: STBC 0x006c6f60
void TGAnimBlender::SetNonExclusiveAnimation( char *animation, float time, bool flag ) {
	if ( BlendAnimation( animation, time, blendTime, flag, false, false ) ) {
		playing = true;
	} else {
		playing = false;
	}
}

/*
================
TGAnimBlender::SetExclusiveAnimationUseDefault
================
*/
// FUNCTION: STBC 0x006c6fa0
void TGAnimBlender::SetExclusiveAnimationUseDefault( char *animation, float time, bool flag ) {
	if ( BlendAnimation( animation, time, blendTime, flag, true, true ) ) {
		playing = true;
	} else {
		playing = false;
	}
}

/*
================
TGAnimBlender::BlendAnimation
================
*/
// FUNCTION: STBC 0x006c6fe0
int TGAnimBlender::BlendAnimation( char *animation, float time, float blendTime_, bool flag, bool flag2, bool flag3 ) {
	return g_kAnimationManager.BlendAnimation( this, time, blendTime_, flag, flag2, flag3, animation );
}

/*
================
TGAnimBlender::SetBlendTime

Resizes every track's blend to the new time.
================
*/
// FUNCTION: STBC 0x006c7010
void TGAnimBlender::SetBlendTime( float time ) {
	TGAnimTrack **items = tracks.GetItems();
	int count = tracks.GetCount();
	if ( blendTime != time ) {
		blendTime = time;
		int frames = time * BLEND_FRAMES_PER_SECOND;
		for ( int i = 0; i < count; i++ ) {
			items[i]->SetFrameCount( frames );
		}
	}
}

/*
================
TGAnimBlender::IsAnimating

True while playing and some track is not done.
================
*/
// FUNCTION: STBC 0x006c7060
bool TGAnimBlender::IsAnimating( void ) {
	if ( playing ) {
		int count = tracks.GetCount();
		TGAnimTrack **items = tracks.GetItems();
		for ( int i = 0; i < count; i++ ) {
			if ( ( items[i]->GetFlags() & TGAnimTrack::DONE ) != TGAnimTrack::DONE ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
TGAnimBlender::PositionUsingAnimation
================
*/
// FUNCTION: STBC 0x006c70a0
int TGAnimBlender::PositionUsingAnimation( char *animation ) {
	return g_kAnimationManager.PositionUsingAnimation( this, animation );
}

/*
================
TGAnimBlender::Unknown6C70C0

Puts every track at the start of the animation and marks it done.
================
*/
// FUNCTION: STBC 0x006c70c0
void TGAnimBlender::Unknown6C70C0( char *animation ) {
	g_kAnimationManager.UseAnimation( this, animation, true, 0.0f, 0.0f );
	int count = tracks.GetCount();
	TGAnimTrack **items = tracks.GetItems();
	for ( int i = 0; i < count; i++ ) {
		items[i]->Update( 0.0f, this );
		items[i]->SetDone( true );
	}
}

/*
================
TGAnimBlender::Unknown6C7110

Puts every track at the start of the animation, marks it done and stops
playing.
================
*/
// FUNCTION: STBC 0x006c7110
void TGAnimBlender::Unknown6C7110( char *animation ) {
	UseAnimation( animation, true, 0.0f, 0.0f );
	int count = tracks.GetCount();
	TGAnimTrack **items = tracks.GetItems();
	for ( int i = 0; i < count; i++ ) {
		items[i]->Update( 0.0f, this );
		items[i]->SetDone( true );
	}
	playing = false;
}

/*
================
TGAnimBlender::UseDefault
================
*/
// FUNCTION: STBC 0x006c7160
int TGAnimBlender::UseDefault( char *animation ) {
	return g_kAnimationManager.UseDefault( this, animation );
}

/*
================
TGAnimBlender::StopTrack
================
*/
// FUNCTION: STBC 0x006c7180
void TGAnimBlender::StopTrack( int index ) {
	TGAnimTrack **items = tracks.GetItems();
	if ( index < tracks.GetCount() ) {
		items[index]->SetDone( true );
		items[index]->duration = 0.0f;
	}
}

/*
================
TGAnimBlender::StopTrackInOthers

Stops the track of the index in the target's other playing blenders.
================
*/
// FUNCTION: STBC 0x006c71b0
void TGAnimBlender::StopTrackInOthers( int index ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		TGAnimBlender *blender = NiDynamicCast( TGAnimBlender, controller );
		if ( blender && blender != this && blender->playing ) {
			blender->StopTrack( index );
		}
	}
}
