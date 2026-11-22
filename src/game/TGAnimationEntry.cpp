/*
===============================================================================

	TGAnimationEntry.cpp

===============================================================================
*/

#include <iostream>
#include <new>
#include <string.h>

#include "TGAnimationEntry.h"
#include "TGNodeAnimation.h"
#include "TGAnimBlender.h"
#include "TGModelManager.h"
#include "NiKeyframeController.h"
#include "NiVisController.h"
#include "NiNode.h"
#include "NiStream.h"
#include "NiMath.h"
#include "TGAnimTrack.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006cc7e0 _$E279
// FUNCTION: STBC 0x006cc800 _$E278

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006cc5b0 _$E32
// FUNCTION: STBC 0x006cc5c0 _$E29
// FUNCTION: STBC 0x006cc5d0 _$E31
// FUNCTION: STBC 0x006cc5e0 _$E30
// FUNCTION: STBC 0x006cc5f0 _$E38
// FUNCTION: STBC 0x006cc600 _$E35
// FUNCTION: STBC 0x006cc610 _$E37
// FUNCTION: STBC 0x006cc620 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006cc6e0 _$E77
// FUNCTION: STBC 0x006cc6f0 _$E76
// FUNCTION: STBC 0x006cc700 _$E66
// FUNCTION: STBC 0x006cc710 _$E65
// FUNCTION: STBC 0x006cc7b0 _$E47
// FUNCTION: STBC 0x006cc7c0 _$E46
// FUNCTION: STBC 0x006cc810 _$E86
// FUNCTION: STBC 0x006cc820 _$E85
// FUNCTION: STBC 0x006cc830 _$E97
// FUNCTION: STBC 0x006cc840 _$E96
// FUNCTION: STBC 0x006cc880 _$E101
// FUNCTION: STBC 0x006cc890 _$E100
// FUNCTION: STBC 0x006cc8a0 _$E105
// FUNCTION: STBC 0x006cc8b0 _$E104
// FUNCTION: STBC 0x006cc8c0 _$E109
// FUNCTION: STBC 0x006cc8d0 _$E108
// FUNCTION: STBC 0x006cc8e0 _$E113
// FUNCTION: STBC 0x006cc8f0 _$E112
// FUNCTION: STBC 0x006cc900 _$E117
// FUNCTION: STBC 0x006cc910 _$E116
// FUNCTION: STBC 0x006cc920 _$E232
// FUNCTION: STBC 0x006cc930 _$E231
// FUNCTION: STBC 0x006cc960 _$E241
// FUNCTION: STBC 0x006cc970 _$E240
// FUNCTION: STBC 0x006cc980 _$E268
// FUNCTION: STBC 0x006cc990 _$E267
// FUNCTION: STBC 0x006cc9a0 _$E272
// FUNCTION: STBC 0x006cc9b0 _$E271
// FUNCTION: STBC 0x006cc9c0 _$E174
// FUNCTION: STBC 0x006cc9d0 _$E173
// FUNCTION: STBC 0x006cc9e0 _$E187
// FUNCTION: STBC 0x006cc9f0 _$E186

/*
================
TGAnimationEntry::TGAnimationEntry

Keeps a copy of the name; no nodes yet.
================
*/
// FUNCTION: STBC 0x006ca880 ??0TGAnimationEntry@@QAE@PAD@Z
TGAnimationEntry::TGAnimationEntry( char *name_ ) {
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
	special = false;
}

/*
================
TGAnimationEntry::~TGAnimationEntry
================
*/
// FUNCTION: STBC 0x006ca930 ??1TGAnimationEntry@@QAE@XZ
TGAnimationEntry::~TGAnimationEntry( void ) {
	if ( name != NULL ) {
		delete name;
	}
}

/*
================
TGAnimationEntry::Load

Reads the animation file and takes the animation of its first top-level
object. A file whose name starts with '_' is a special animation. The
file's shared objects come from the model manager's map.
================
*/
// FUNCTION: STBC 0x006ca9b0 ?Load@TGAnimationEntry@@QAEHPAD@Z
int TGAnimationEntry::Load( char *file ) {
	NiPointer<NiNode> root;
	NiStream stream;
	int i;

	for ( i = strlen( file ) - 1; i >= 0 && file[i] != '\\' && file[i] != '/'; i-- ) {
	}
	if ( file[i + 1] == '_' ) {
		special = true;
	}
	stream.SetImageMap( &g_kModelManager.unknown00 );
	if ( !stream.Load( file ) ) {
		return 0;
	}
	NiObject *object = stream.GetObjectAt( 0 );
	if ( object != NULL && object->IsKindOf( &NiNode::rtti ) ) {
		root = (NiNode *)object;
	}
	Unknown6CAAE0( root );
	return 1;
}

/*
================
TGAnimationEntry::Unknown6CAAE0
================
*/
// FUNCTION: STBC 0x006caae0 ?Unknown6CAAE0@TGAnimationEntry@@QAEHPAVNiNode@@@Z
int TGAnimationEntry::Unknown6CAAE0( NiNode *node ) {
	AddNode( node );
	UpdateLength();
	return 1;
}

/*
================
TGAnimationEntry::AddNode

Copies the object's keyframe keys and visibility keys into a new
TGNodeAnimation under the object's name, with the object's transform, and
does the same for each child of a node. A node whose position keys are
copied and that is named "Bip01" makes the animation move the pelvis.
================
*/
// FUNCTION: STBC 0x006cab00 ?AddNode@TGAnimationEntry@@QAEXPAVNiAVObject@@@Z
// PARKED: inline budget: retail calls NiPointer<NiKeyframeData>::operator= (0x6CC6A0) and the visibility path's DecRefCount out of line, which this build expands; frame slots follow
void TGAnimationEntry::AddNode( NiAVObject *object ) {
	TGNodeAnimation *node = NULL;
	unsigned int i;

	NiKeyframeController *keyframes = FindKeyframeController( object );
	if ( keyframes != NULL ) {
		unsigned int numRotKeys;
		NiRotKey::KeyType rotKeyType;
		NiRotKey **rotKeys = keyframes->GetRotAnim( numRotKeys, rotKeyType );
		unsigned int numPosKeys;
		NiPosKey::KeyType posKeyType;
		NiPosKey **posKeys = keyframes->GetPosAnim( numPosKeys, posKeyType );
		unsigned int numScaleKeys;
		NiFloatKey::KeyType scaleKeyType;
		NiFloatKey **scaleKeys = keyframes->GetScaleAnim( numScaleKeys, scaleKeyType );

		const char *name = object->GetName();
		NiKeyframeData *copy = new NiKeyframeData;
		node = new TGNodeAnimation;
		node->keyframeData = copy;
		node->SetName( name );
		nodes.Insert( node );
		node->rotate = object->GetRotate();
		node->translate = object->GetTranslate();
		node->scale = object->GetScale();

		if ( rotKeys != NULL ) {
			NiRotKey::CopyFunction copyKey = NiRotKey::copyFunctions[rotKeyType];
			NiRotKey **keys = new NiRotKey *[numRotKeys];
			for ( i = 0; i < numRotKeys; i++ ) {
				keys[i] = copyKey( rotKeys[i] );
			}
			copy->ReplaceRotAnim( keys, numRotKeys, rotKeyType );
		} else {
			copy->ReplaceRotAnim( NULL, 0, NiRotKey::NOINTERP );
		}
		if ( posKeys != NULL ) {
			NiPosKey::CopyFunction copyKey = NiPosKey::copyFunctions[posKeyType];
			NiPosKey **keys = new NiPosKey *[numPosKeys];
			for ( i = 0; i < numPosKeys; i++ ) {
				keys[i] = copyKey( posKeys[i] );
			}
			copy->ReplacePosAnim( keys, numPosKeys, posKeyType );
			if ( strcmp( name, "Bip01" ) == 0 ) {
				movesPelvis = true;
			}
		} else {
			copy->ReplacePosAnim( NULL, 0, NiPosKey::NOINTERP );
		}
		if ( scaleKeys != NULL ) {
			NiFloatKey::CopyFunction copyKey = NiFloatKey::copyFunctions[scaleKeyType];
			NiFloatKey **keys = new NiFloatKey *[numScaleKeys];
			for ( i = 0; i < numScaleKeys; i++ ) {
				keys[i] = copyKey( scaleKeys[i], 0 );
			}
			copy->ReplaceScaleAnim( keys, numScaleKeys, scaleKeyType );
		} else {
			copy->ReplaceScaleAnim( NULL, 0, NiFloatKey::NOINTERP );
		}
	}

	NiVisController *visibility = FindVisController( object );
	if ( visibility != NULL ) {
		unsigned int numVisKeys;
		NiVisKey **visKeys = visibility->GetVisData( numVisKeys );
		if ( node == NULL ) {
			const char *name = object->GetName();
			node = new TGNodeAnimation;
			node->keyframeData = NULL;	// BUG: redundant: the constructor cleared it
			node->SetName( name );
			nodes.Insert( node );
			node->appCulled = object->GetAppCulled();
		}
		NiVisData *copy = new NiVisData;
		node->visData = copy;
		if ( visKeys != NULL ) {
			NiVisKey **keys = new NiVisKey *[numVisKeys];
			for ( i = 0; i < numVisKeys; i++ ) {
				keys[i] = new NiVisKey;
				keys[i]->time = visKeys[i]->time;
				keys[i]->visible = visKeys[i]->visible;
			}
			copy->ReplaceAnim( keys, numVisKeys );
		} else {
			copy->ReplaceAnim( NULL, 0 );
		}
	}

	NiNode *parent = NiDynamicCast( NiNode, object );
	if ( parent != NULL ) {
		for ( i = 0; i < parent->GetArrayCount(); i++ ) {
			NiAVObject *child = parent->GetAt( i );
			if ( NiDynamicCast( NiAVObject, child ) != NULL ) {
				AddNode( child );
			}
		}
	}
}

/*
================
TGAnimationEntry::Unknown6CB200

Gives each track of the blender this animation's keys for its node, started
at start - end, and stops the track in the blender's other animations
unless unknown is set. A track whose node this animation does not move is
stopped and cleared when unknown is set. Always 1.
================
*/
// FUNCTION: STBC 0x006cb200 ?Unknown6CB200@TGAnimationEntry@@QAEHPAVTGAnimBlender@@_NMM@Z
// PARKED: register allocation: retail keeps the count in a frame slot and the visibility controller in EBX (pushes EBP after the count test); this build keeps the count in EBP
int TGAnimationEntry::Unknown6CB200( TGAnimBlender *blender, bool unknown, float start, float end ) {
	int count = blender->GetTrackCount();
	TGAnimTrack **tracks = blender->GetTracks();
	for ( int i = 0; i < count; i++ ) {
		NiKeyframeController *keyframes = tracks[i]->keyframeController;
		NiVisController *visibility = tracks[i]->visController;
		TGNodeAnimation *node = nodes.Get( tracks[i]->name );
		if ( node != NULL ) {
			NiKeyframeData *keyframeData = node->keyframeData;
			NiVisData *visData = node->visData;
			if ( ( keyframeData != NULL && ( keyframeData->GetNumPosKeys() != 0 || keyframeData->GetNumRotKeys() != 0 ) )
				|| ( visData != NULL && visData->GetNumVisKeys() != 0 ) ) {
				if ( !tracks[i]->IsDone() ) {
					if ( keyframes != NULL ) {
						keyframes->Update( start );
					}
					if ( visibility != NULL ) {
						visibility->Update( start );
					}
				}
				float time = start - end;
				if ( keyframes != NULL ) {
					keyframes->Start( -NI_INFINITY );
					keyframes->SetData( keyframeData );
					keyframes->SetPhase( -time );
				}
				if ( visibility != NULL ) {
					visibility->Start( -NI_INFINITY );
					visibility->SetVisData( visData );
					visibility->SetPhase( -time );
				}
				tracks[i]->SetDone( false );
				tracks[i]->time = time;
				tracks[i]->SetNextAnimation( NULL );
				tracks[i]->duration = GetLength();
				if ( !unknown ) {
					blender->StopTrackInOthers( i );
				}
				continue;
			}
		}
		if ( unknown ) {
			if ( keyframes != NULL ) {
				keyframes->ClearData();
			}
			if ( visibility != NULL ) {
				visibility->SetEndKeyTime( 0.0f );
			}
			tracks[i]->time = start - end;
			tracks[i]->SetDone( true );
			tracks[i]->SetNextAnimation( NULL );
			tracks[i]->duration = GetLength();
		}
	}
	return 1;
}

/*
================
TGAnimationEntry::Unknown6CB450

Blends each track of the blender into this animation over the duration:
when blend is set, the track's frames move from the node's pose in the
animation file toward the keys (BlendRotKeys and its siblings), else from
the node's current pose. A track whose node the animation does not move is
stopped and cleared when unknownFlag is set. Each track then runs from
start, looping as asked, and goes on to nextAnimation; unless unknownFlag
is set, the blender's other animations stop the track. Always 1.
================
*/
// FUNCTION: STBC 0x006cb450 ?Unknown6CB450@TGAnimationEntry@@QAEHPAVTGAnimBlender@@MM_N11PAD@Z
// DRAFT: inline budget: retail calls the node array's Find (0x6CC730) and the no-keys ClearData's DecRefCount out of line where this build expands them, and keeps the blend branch's scale call apart; the rest follows from there
int TGAnimationEntry::Unknown6CB450( TGAnimBlender *blender, float start, float duration, bool looping, bool unknownFlag, bool blend, char *nextAnimation ) {
	int count = blender->GetTrackCount();
	TGAnimTrack **tracks = blender->GetTracks();
	for ( int i = 0; i < count; i++ ) {
		NiKeyframeController *keyframes = tracks[i]->keyframeController;
		NiVisController *visibility = tracks[i]->visController;
		TGNodeAnimation *node = nodes.Get( tracks[i]->name );
		NiKeyframeData *keyframeData = NULL;
		NiVisData *visData = NULL;
		if ( node != NULL ) {
			keyframeData = node->keyframeData;
			visData = node->visData;
		}
		if ( node != NULL && ( ( keyframeData != NULL && ( keyframeData->GetNumPosKeys() != 0 || keyframeData->GetNumRotKeys() != 0 || blend ) )
						   || ( visData != NULL && visData->GetNumVisKeys() != 0 ) ) ) {
			if ( !tracks[i]->IsDone() ) {
				if ( keyframes != NULL ) {
					keyframes->Update( start );
				}
				if ( visibility != NULL ) {
					visibility->Update( start );
				}
			}
			if ( keyframeData != NULL ) {
				unsigned int numRotKeys;
				NiRotKey::KeyType rotKeyType;
				NiRotKey **rotKeys = keyframeData->GetRotAnim( numRotKeys, rotKeyType );
				unsigned int numPosKeys;
				NiPosKey::KeyType posKeyType;
				NiPosKey **posKeys = keyframeData->GetPosAnim( numPosKeys, posKeyType );
				unsigned int numScaleKeys;
				NiFloatKey::KeyType scaleKeyType;
				NiFloatKey **scaleKeys = keyframeData->GetScaleAnim( numScaleKeys, scaleKeyType );
				bool rotBlended;
				bool posBlended;
				bool scaleBlended;
				if ( blend ) {
					NiMatrix3 rotate = node->rotate;
					NiQuaternion rotation;
					rotation.FromRotation( rotate );
					rotBlended = BlendRotKeys( tracks[i], rotKeys, numRotKeys, rotKeyType, duration, &rotation );
					NiPoint3 translate = node->translate;
					posBlended = BlendPosKeys( tracks[i], posKeys, numPosKeys, posKeyType, duration, &translate );
					float scale = node->scale;
					scaleBlended = BlendScaleKeys( tracks[i], scaleKeys, numScaleKeys, scaleKeyType, duration, &scale );
				} else {
					rotBlended = BlendRotKeys( tracks[i], rotKeys, numRotKeys, rotKeyType, duration, NULL );
					posBlended = BlendPosKeys( tracks[i], posKeys, numPosKeys, posKeyType, duration, NULL );
					scaleBlended = BlendScaleKeys( tracks[i], scaleKeys, numScaleKeys, scaleKeyType, duration, NULL );
				}
				if ( !rotBlended && !posBlended && !scaleBlended ) {
					keyframes->ClearData();
				} else {
					NiKeyframeData *data = tracks[i]->data;
					data->ReplaceRotAnim( NULL, 0, NiRotKey::NOINTERP );
					if ( rotBlended ) {
						data->ReplaceRotAnim( tracks[i]->rotKeys, tracks[i]->frameCount, NiRotKey::LINKEY );
					}
					data->ReplacePosAnim( NULL, 0, NiPosKey::NOINTERP );
					if ( posBlended ) {
						data->ReplacePosAnim( tracks[i]->posKeys, tracks[i]->frameCount, NiPosKey::LINKEY );
					}
					data->ReplaceScaleAnim( NULL, 0, NiFloatKey::LINKEY );
					if ( scaleBlended ) {
						data->ReplaceScaleAnim( tracks[i]->scaleKeys, tracks[i]->frameCount, NiFloatKey::LINKEY );
					}
					keyframes->SetData( data );
				}
			}
			if ( visData != NULL && visibility != NULL ) {
				visibility->SetVisData( visData );
			}
			if ( keyframes != NULL ) {
				keyframes->Start( -NI_INFINITY );
				keyframes->SetPhase( -start );
			}
			if ( visibility != NULL ) {
				visibility->Start( -NI_INFINITY );
				visibility->SetPhase( -start );
			}
			tracks[i]->SetDone( false );
			tracks[i]->time = start;
			tracks[i]->SetLooping( looping );
			tracks[i]->SetUnknownFlag( unknownFlag );
			tracks[i]->SetNextAnimation( nextAnimation );
			tracks[i]->duration = duration;
			if ( !unknownFlag ) {
				blender->StopTrackInOthers( i );
			}
		} else if ( unknownFlag ) {
			if ( keyframes != NULL ) {
				keyframes->ClearData();
				keyframes->Start( -NI_INFINITY );
				keyframes->SetPhase( -start );
			}
			if ( visibility != NULL ) {
				visibility->SetEndKeyTime( 0.0f );
				visibility->Start( -NI_INFINITY );
				visibility->SetPhase( -start );
			}
			tracks[i]->time = start;
			tracks[i]->SetLooping( false );
			tracks[i]->SetDone( true );
			tracks[i]->SetNextAnimation( nextAnimation );
			tracks[i]->duration = duration;
		}
	}
	return 1;
}

/*
================
TGAnimationEntry::BlendRotKeys

Gives each of the track's rotation frames, spread evenly over the
duration, a rotation that starts at the node's own and turns step by step
toward the keys' rotation at the frame's time (or toward start when there
are no keys). False when there is nothing to blend toward, when the track
has no node, or when a key lies after a frame. The name is invented.
================
*/
// FUNCTION: STBC 0x006cb9a0
bool TGAnimationEntry::BlendRotKeys( TGAnimTrack *track, NiRotKey **keys, int keyCount, NiRotKey::KeyType &keyType, float duration, NiQuaternion *start ) {
	if ( ( keys == NULL || keyCount == 0 ) && start == NULL ) {
		return false;
	}
	NiKeyframeController *controller = track->keyframeController;
	if ( controller == NULL ) {
		return false;
	}
	NiRotKey unused;	// BUG: redundant: constructed and never used

	unsigned int frameCount = track->frameCount;
	NiRotKey **frames = track->rotKeys;
	float time = 0.0f;
	float step = duration / (float)( frameCount - 1 );
	unsigned int i;
	for ( i = 0; i < frameCount; i++ ) {
		frames[i]->time = time;
		time += step;
	}

	NiObjectNET *target = controller->GetTarget();
	if ( target == NULL || !target->IsKindOf( &NiAVObject::rtti ) ) {
		return false;
	}
	NiQuaternion rotation;
	NiDynamicCast( NiAVObject, target )->GetRotate( rotation );

	step = 1.0f / (float)( frameCount - 1 );
	time = 0.0f;
	int k = 0;
	for ( i = 0; i < frameCount; i++ ) {
		NiQuaternion quat;
		if ( start == NULL || keyCount != 0 ) {
			NiRotKey *frame = frames[i];
			if ( keys[k]->GetTime() > frame->GetTime() ) {
				return false;
			}
			while ( k < keyCount - 1 && keys[k + 1]->GetTime() <= frame->GetTime() ) {
				k++;
			}
			if ( k >= keyCount - 1 ) {
				quat = keys[keyCount - 1]->GetQuaternion();
			} else {
				float ratio = ( frame->GetTime() - keys[k]->GetTime() ) / ( keys[k + 1]->GetTime() - keys[k]->GetTime() );
				if ( ratio > 1.0f ) {
					ratio = 1.0f;
				}
				NiRotKey::InterpFunction interpolate = NiRotKey::interpFunctions[keyType];
				quat = interpolate( ratio, keys[k], keys[k + 1] );
			}
		} else {
			quat = *start;
		}
		SlerpNearer( rotation, quat, time );
		frames[i]->quat = rotation;
		time += step;
	}
	return true;
}

/*
================
TGAnimationEntry::SlerpNearer

Turns p by t toward q, or toward -q when that is nearer (the same
rotation the shorter way). The name is invented.
================
*/
// FUNCTION: STBC 0x006cbbc0
void TGAnimationEntry::SlerpNearer( NiQuaternion &p, const NiQuaternion &q, float t ) {
	NiQuaternion nearer;
	nearer = Nearer( p, q );
	p = NiQuaternion::Slerp( t, p, nearer );
}

/*
================
TGAnimationEntry::Nearer

q or -q, whichever is nearer p. The name is invented.
================
*/
// FUNCTION: STBC 0x006cbc10
NiQuaternion TGAnimationEntry::Nearer( const NiQuaternion &p, const NiQuaternion &q ) {
	NiQuaternion difference;
	difference = ( p - q ) * ( p - q );
	float distance = difference.w * difference.w + difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;
	NiQuaternion sum;
	sum = ( p + q ) * ( p + q );
	if ( sum.w * sum.w + sum.x * sum.x + sum.y * sum.y + sum.z * sum.z < distance ) {
		return -q;
	}
	return q;
}

/*
================
TGAnimationEntry::UpdateLength
================
*/
// FUNCTION: STBC 0x006cbd10 ?UpdateLength@TGAnimationEntry@@QAEXXZ
void TGAnimationEntry::UpdateLength( void ) {
	TGNodeAnimation **items = nodes.GetItems();
	length = 0.0f;
	for ( int i = 0; i < nodes.GetCount(); i++ ) {
		NiKeyframeData *data = items[i]->keyframeData;
		if ( data != NULL ) {
			unsigned int numRotKeys;
			NiRotKey::KeyType rotKeyType;
			NiRotKey **rotKeys = data->GetRotAnim( numRotKeys, rotKeyType );
			unsigned int numPosKeys;
			NiPosKey::KeyType posKeyType;
			NiPosKey **posKeys = data->GetPosAnim( numPosKeys, posKeyType );
			if ( rotKeys != NULL ) {
				float time = rotKeys[numRotKeys - 1]->GetTime();
				if ( time > length ) {
					length = time;
				}
			}
			if ( posKeys != NULL ) {
				float time = posKeys[numPosKeys - 1]->GetTime();
				if ( time > length ) {
					length = time;
				}
			}
		}
	}
}

/*
================
TGAnimationEntry::Unknown6CBD80

Stops each track at the start of the animation: clears its keys and puts its
node where the animation file had it, with the node's culling flag when the
file kept visibility keys. Always 1.
================
*/
// FUNCTION: STBC 0x006cbd80 ?Unknown6CBD80@TGAnimationEntry@@QAEHPAVTGAnimBlender@@@Z
int TGAnimationEntry::Unknown6CBD80( TGAnimBlender *blender ) {
	int count = blender->GetTrackCount();
	TGAnimTrack **tracks = blender->GetTracks();
	for ( int i = 0; i < count; i++ ) {
		NiKeyframeController *keyframes = tracks[i]->keyframeController;
		NiVisController *visibility = tracks[i]->visController;
		TGNodeAnimation *node = nodes.Get( tracks[i]->name );
		NiKeyframeData *keyframeData;
		NiVisData *visData;
		if ( node != NULL ) {
			keyframeData = node->keyframeData;
			visData = node->visData;
		} else {
			keyframeData = NULL;
			visData = NULL;
		}
		keyframes->ReplaceRotAnim( NULL, 0, NiRotKey::LINKEY, true );
		keyframes->ReplacePosAnim( NULL, 0, NiPosKey::LINKEY, true );
		keyframes->ReplaceScaleAnim( NULL, 0, NiFloatKey::NOINTERP, true );
		tracks[i]->SetDone( true );
		tracks[i]->SetNextAnimation( NULL );
		tracks[i]->duration = 0.0f;
		if ( keyframes != NULL ) {
			keyframes->hiKeyTime = 0.0f;
		}
		if ( visibility != NULL ) {
			visibility->hiKeyTime = 0.0f;
		}
		if ( keyframeData != NULL ) {
			NiObjectNET *target = keyframes->GetTarget();
			if ( target == NULL || !target->IsKindOf( &NiAVObject::rtti ) ) {
				continue;
			}
			NiAVObject *object = NiDynamicCast( NiAVObject, target );
			NiMatrix3 rotate = node->rotate;
			object->SetRotate( rotate );
			NiPoint3 translate = node->translate;
			object->SetTranslate( translate );
		}
		if ( visData != NULL ) {
			NiObjectNET *target = visibility->GetTarget();
			if ( target == NULL || !target->IsKindOf( &NiAVObject::rtti ) ) {
				continue;
			}
			NiAVObject *object = NiDynamicCast( NiAVObject, target );
			object->SetAppCulled( node->appCulled );
		}
	}
	return 1;
}

/*
================
TGAnimationEntry::BlendPosKeys

BlendRotKeys for the track's position frames: each position moves from
the node's own toward the keys' position at the frame's time (or toward
start). The name is invented.
================
*/
// FUNCTION: STBC 0x006cbf90
bool TGAnimationEntry::BlendPosKeys( TGAnimTrack *track, NiPosKey **keys, int keyCount, NiPosKey::KeyType &keyType, float duration, NiPoint3 *start ) {
	if ( ( keys == NULL || keyCount == 0 ) && start == NULL ) {
		return false;
	}
	NiKeyframeController *controller = track->keyframeController;
	if ( controller == NULL ) {
		return false;
	}
	NiPosKey unused;	// BUG: redundant: constructed and never used

	unsigned int frameCount = track->frameCount;
	NiPosKey **frames = track->posKeys;
	float time = 0.0f;
	float step = duration / (float)( frameCount - 1 );
	unsigned int i;
	for ( i = 0; i < frameCount; i++ ) {
		frames[i]->time = time;
		time += step;
	}

	NiObjectNET *target = controller->GetTarget();
	if ( target == NULL || !target->IsKindOf( &NiAVObject::rtti ) ) {
		return false;
	}
	NiPoint3 position = NiDynamicCast( NiAVObject, target )->GetTranslate();

	step = 1.0f / (float)( frameCount - 1 );
	time = 0.0f;
	int k = 0;
	for ( i = 0; i < frameCount; i++ ) {
		NiPoint3 pos;
		if ( start == NULL || keyCount != 0 ) {
			while ( k < keyCount - 1 && keys[k + 1]->GetTime() <= frames[i]->GetTime() ) {
				k++;
			}
			if ( k >= keyCount - 1 ) {
				pos = keys[keyCount - 1]->GetPos();
			} else {
				float ratio = ( frames[i]->GetTime() - keys[k]->GetTime() ) / ( keys[k + 1]->GetTime() - keys[k]->GetTime() );
				if ( ratio > 1.0f ) {
					ratio = 1.0f;
				}
				NiPosKey::InterpFunction interpolate = NiPosKey::interpFunctions[keyType];
				pos = interpolate( ratio, keys[k], keys[k + 1] );
			}
		} else {
			pos = *start;
		}
		position = position * ( 1.0f - time ) + pos * time;
		frames[i]->pos = position;
		time += step;
	}
	return true;
}

/*
================
TGAnimationEntry::BlendScaleKeys

BlendRotKeys for the track's scale frames: each scale moves from the
node's own toward the keys' scale at the frame's time (or toward start).
The name is invented.
================
*/
// FUNCTION: STBC 0x006cc210
bool TGAnimationEntry::BlendScaleKeys( TGAnimTrack *track, NiFloatKey **keys, int keyCount, NiFloatKey::KeyType &keyType, float duration, float *start ) {
	if ( ( keys == NULL || keyCount == 0 ) && start == NULL ) {
		return false;
	}
	NiKeyframeController *controller = track->keyframeController;
	if ( controller == NULL ) {
		return false;
	}
	NiFloatKey unused;	// BUG: redundant: constructed and never used

	unsigned int frameCount = track->frameCount;
	NiFloatKey **frames = track->scaleKeys;
	float time = 0.0f;
	float step = duration / (float)( frameCount - 1 );
	unsigned int i;
	for ( i = 0; i < frameCount; i++ ) {
		frames[i]->time = time;
		time += step;
	}

	NiObjectNET *target = controller->GetTarget();
	if ( target == NULL || !target->IsKindOf( &NiAVObject::rtti ) ) {
		return false;
	}
	float scale = NiDynamicCast( NiAVObject, target )->GetScale();

	step = 1.0f / (float)( frameCount - 1 );
	time = 0.0f;
	int k = 0;
	for ( i = 0; i < frameCount; i++ ) {
		float value;
		if ( start == NULL || keyCount != 0 ) {
			while ( k < keyCount - 1 && keys[k + 1]->GetTime() <= frames[i]->GetTime() ) {
				k++;
			}
			if ( k >= keyCount - 1 ) {
				value = keys[keyCount - 1]->GetValue();
			} else {
				float ratio = ( frames[i]->GetTime() - keys[k]->GetTime() ) / ( keys[k + 1]->GetTime() - keys[k]->GetTime() );
				if ( ratio > 1.0f ) {
					ratio = 1.0f;
				}
				NiFloatKey::InterpFunction interpolate = NiFloatKey::interpFunctions[keyType];
				value = interpolate( ratio, keys[k], keys[k + 1] );
			}
		} else {
			value = *start;
		}
		scale = scale * ( 1.0f - time ) + value * time;
		frames[i]->value = scale;
		time += step;
	}
	return true;
}

/*
================
TGAnimationEntry::Unknown6CC400

Clears each track's keys and moves its node to where this animation starts:
the transform the animation file gave the node, then the first rotation
and position keys when they are at time 0. Always 1.
================
*/
// FUNCTION: STBC 0x006cc400 ?Unknown6CC400@TGAnimationEntry@@QAEHPAVTGAnimBlender@@@Z
int TGAnimationEntry::Unknown6CC400( TGAnimBlender *blender ) {
	int count = blender->GetTrackCount();
	TGAnimTrack **tracks = blender->GetTracks();
	for ( int i = 0; i < count; i++ ) {
		NiKeyframeController *keyframes = tracks[i]->keyframeController;
		keyframes->ClearData();
		TGNodeAnimation *node = nodes.Get( tracks[i]->name );
		if ( node != NULL ) {
			NiKeyframeData *data = node->keyframeData;
			if ( data != NULL ) {
				NiObjectNET *target = keyframes->GetTarget();
				if ( target != NULL && target->IsKindOf( &NiAVObject::rtti ) ) {
					NiAVObject *object = NiDynamicCast( NiAVObject, target );
					NiMatrix3 rotate = node->rotate;
					object->SetRotate( rotate );
					NiPoint3 translate = node->translate;
					object->SetTranslate( translate );
					unsigned int numRotKeys;
					NiRotKey::KeyType rotKeyType;
					NiRotKey **rotKeys = data->GetRotAnim( numRotKeys, rotKeyType );
					unsigned int numPosKeys;
					NiPosKey::KeyType posKeyType;
					NiPosKey **posKeys = data->GetPosAnim( numPosKeys, posKeyType );
					if ( rotKeys != NULL && rotKeys[0]->GetTime() == 0.0f ) {
						NiQuaternion quat;
						quat = rotKeys[0]->GetQuaternion();
						NiMatrix3 keyRotate;
						quat.ToRotation( keyRotate );
						object->SetRotate( keyRotate );
					}
					if ( posKeys != NULL && posKeys[0]->GetTime() == 0.0f ) {
						NiPoint3 keyTranslate = posKeys[0]->GetPos();
						object->SetTranslate( keyTranslate );
					}
				}
			}
		}
	}
	return 1;
}
