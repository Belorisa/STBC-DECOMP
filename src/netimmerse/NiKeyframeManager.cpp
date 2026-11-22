// NiKeyframeManager.cpp -- named animation sequences

#include "NiRTLib.h"
#include <string.h>

#include "NiKeyframeManager.h"
#include "NiMath.h"
#include "NiQuaternion.h"
#include "NiFile.h"
#include "NiStream.h"
#include "NiNode.h"
#include "NiSystem.h"
#include "NiSequenceStreamHelper.h"
#include "NiStringExtraData.h"
#include "NiTextKeyExtraData.h"
#include "NiForce.h"
#include "NiPlanarCollider.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a4d10 _$E30
// FUNCTION: STBC 0x007a4d20 _$E29
// FUNCTION: STBC 0x007a4d30 _$E41
// FUNCTION: STBC 0x007a4d40 _$E40
// FUNCTION: STBC 0x007a5190 _$E92
// FUNCTION: STBC 0x007a51a0 _$E91
// FUNCTION: STBC 0x007a51b0 _$E96
// FUNCTION: STBC 0x007a51c0 _$E95
// FUNCTION: STBC 0x007a53c0 _$E100
// FUNCTION: STBC 0x007a53d0 _$E99
// FUNCTION: STBC 0x007a5400 _$E217
// FUNCTION: STBC 0x007a5410 _$E216
// FUNCTION: STBC 0x007a5620 _$E233
// FUNCTION: STBC 0x007a5630 _$E232
// FUNCTION: STBC 0x007a5640 _$E237
// FUNCTION: STBC 0x007a5650 _$E236
// FUNCTION: STBC 0x007a57b0 _$E80
// FUNCTION: STBC 0x007a57c0 _$E79
// FUNCTION: STBC 0x007a57d0 _$E60
// FUNCTION: STBC 0x007a57e0 _$E59
// FUNCTION: STBC 0x007a5820 _$E69
// FUNCTION: STBC 0x007a5830 _$E68
// FUNCTION: STBC 0x007a58c0 _$E84
// FUNCTION: STBC 0x007a58d0 _$E83
// FUNCTION: STBC 0x007a5a40 _$E88
// FUNCTION: STBC 0x007a5a50 _$E87
// FUNCTION: STBC 0x007a5ad0 _$E221
// FUNCTION: STBC 0x007a5ae0 _$E220
// FUNCTION: STBC 0x007a5b70 _$E225
// FUNCTION: STBC 0x007a5b80 _$E224
// FUNCTION: STBC 0x007a5c40 _$E241
// FUNCTION: STBC 0x007a5c50 _$E240
// FUNCTION: STBC 0x007a5c60 _$E245
// FUNCTION: STBC 0x007a5c70 _$E244

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a57f0 _$E268
// FUNCTION: STBC 0x007a5810 _$E267
// FUNCTION: STBC 0x007a4c80 _$E17
// FUNCTION: STBC 0x007a4c90 _$E14
// FUNCTION: STBC 0x007a4ca0 _$E16
// FUNCTION: STBC 0x007a4cb0 _$E15
// FUNCTION: STBC 0x007a4cc0 _$E23
// FUNCTION: STBC 0x007a4cd0 _$E20
// FUNCTION: STBC 0x007a4ce0 _$E22
// FUNCTION: STBC 0x007a4cf0 _$E21

// FUNCTION: STBC 0x007a1440 _$E259
// FUNCTION: STBC 0x007a1450 _$E255
// FUNCTION: STBC 0x007a1470 _$E258
// FUNCTION: STBC 0x007a1480 _$E256
// DATA: STBC 0x9A03E0 ?rtti@NiKeyframeManager@@2VNiRTTI@@B
const NiRTTI NiKeyframeManager::rtti( "NiKeyframeManager", &NiTimeController::rtti );

// whether a sequence that came from a file is saved as a reference to it;
// RegisterStreamables then registers none of the sequences' members
// DATA: STBC 0x9760C8
static bool saveFileReferences = true;

/*
================
NiKeyframeManager::Register
================
*/
// FUNCTION: STBC 0x7A14A0 ?Register@NiKeyframeManager@@SAHXZ
NiImplementRegister( NiKeyframeManager )

/*
================
NiKeyframeManager::Sequence::Sequence

Both arrays start with maxSize slots growing by growBy.
================
*/
// FUNCTION: STBC 0x7A15F0
NiKeyframeManager::Sequence::Sequence( const char *name_, unsigned int maxSize, unsigned int growBy ) {
	name = new char[strlen( name_ ) + 1];
	strcpy( name, name_ );
	filename = 0;
	unknown08 = 0;
	unknown0C.SetSize( maxSize );
	unknown0C.SetGrowBy( growBy );
	controllers.SetSize( maxSize );
	controllers.SetGrowBy( growBy );
	manager = 0;
	offset = 0.0f;
	state = INACTIVE;
	unknown50 = 0;
	textKeys = 0;
	unknown40 = 0;
}

/*
================
NiKeyframeManager::Sequence::~Sequence
================
*/
// FUNCTION: STBC 0x7A1890
// PARKED: inline budget: since RemoveAt compares against T( 0 ) (which closes its out-of-line copy, 0x7A4F60), the inlined NiTArray<char *>::RemoveAt costs more and the final ~NiPointer of textKeys goes out of line (DecRefCount 0x40CFE0); retail expands it
NiKeyframeManager::Sequence::~Sequence() {
	textKeys = 0;
	delete [] name;
	delete [] filename;
	for ( unsigned int i = 0; i < controllers.GetSize(); i++ ) {
		controllers.RemoveAt( i ) = 0;
		delete [] unknown0C.RemoveAt( i );
	}
}

/*
================
NiKeyframeManager::Sequence::SetFilename

Keeps a standardized copy of the path of the file the sequence came from.
================
*/
// FUNCTION: STBC 0x7A1A10
void NiKeyframeManager::Sequence::SetFilename( const char *filename_ ) {
	delete [] filename;
	filename = new char[strlen( filename_ ) + 1];
	strcpy( filename, filename_ );
	NiFile::StandardizeFilePath( &filename );
}

/*
================
NiKeyframeManager::Sequence::Load

A new sequence from the object at index in the file: an
NiSequenceStreamHelper whose extra data holds the text keys, then the
name of each controller's node in step with its controllers. NULL when
the file does not load. The name is invented.
================
*/
// FUNCTION: STBC 0x7A1A70
NiKeyframeManager::Sequence *NiKeyframeManager::Sequence::Load( const char *filename_, const char *name_, unsigned int index ) {
	NiStream stream;
	if ( !stream.Load( filename_ ) ) {
		return 0;
	}

	NiSequenceStreamHelper *helper = NiDynamicCast( NiSequenceStreamHelper, stream.GetObjectAt( index ) );
	Sequence *sequence = new Sequence( name_, 12, 12 );
	sequence->SetFilename( filename_ );
	sequence->unknown08 = index;
	sequence->unknown40 = 0;

	NiExtraData *extra = helper->GetExtraData();
	NiTextKeyExtraData *keys = NiDynamicCast( NiTextKeyExtraData, extra );
	if ( keys ) {
		sequence->SetTextKeys( keys );
		extra = extra->GetNext();
	}
	NiTimeController *controller = helper->GetControllers();
	while ( extra ) {
		NiStringExtraData *nodeName = NiDynamicCast( NiStringExtraData, extra );
		sequence->Unknown7A1EB0( nodeName->GetValue(), NiDynamicCast( NiKeyframeController, controller ) );
		extra = extra->GetNext();
		controller = controller->GetNext();
	}
	return sequence;
}

/*
================
NiKeyframeManager::Sequence::Copy

A new sequence like the source: its name, file and text keys, and a clone
of each of its controllers under the same node name.
================
*/
// FUNCTION: STBC 0x7A1C90
NiKeyframeManager::Sequence *NiKeyframeManager::Sequence::Copy( Sequence *source ) {
	// AnimOnly.exe 0x48DE9E (NiKeyframeManager.cpp): through the accessors
	Sequence *sequence = new Sequence( source->GetName(), 12, 12 );
	sequence->SetFilename( source->GetFilename() );
	sequence->SetUnknown08( source->GetUnknown08() );
	sequence->SetTextKeys( source->GetTextKeys() );
	sequence->SetUnknown40( source->GetUnknown40() );

	unsigned int count = source->GetNodeCount();
	for ( unsigned int i = 0; i < count; i++ ) {
		char *nodeName = source->GetNodeName( i );
		NiKeyframeController *controller = source->GetController( i );
		NiKeyframeController *clone = (NiKeyframeController *)controller->CreateClone();
		sequence->Unknown7A1EB0( nodeName, clone );
	}
	return sequence;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A1DB0

A new sequence of that name, added to the first sequence's manager, with
the first sequence's text keys and offset and a clone of each controller
of the two sequences, walked in step over the manager's target.
================
*/
// FUNCTION: STBC 0x7A1DB0
NiKeyframeManager::Sequence *NiKeyframeManager::Sequence::Unknown7A1DB0( Sequence *first, Sequence *second, const char *name_ ) {
	Sequence *sequence = new Sequence( name_, 12, 12 );
	NiKeyframeManager *owner = first->manager;
	sequence->SetTextKeys( first->textKeys );
	sequence->offset = first->GetOffset();

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;
	NiAVObject *target = NiDynamicCast( NiAVObject, owner->GetTarget() );
	sequence->Unknown7A2690( target, firstIndex, secondIndex, first, second );
	owner->Unknown7A3390( sequence );
	return sequence;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A1EB0

Adds a controller to the sequence under a copy of the name of the node it
controls; returns where it was put.
================
*/
// FUNCTION: STBC 0x7A1EB0
unsigned int NiKeyframeManager::Sequence::Unknown7A1EB0( const char *nodeName, NiKeyframeController *controller ) {
	unsigned int index = unknown0C.GetSize();
	char *nameCopy = new char[strlen( nodeName ) + 1];
	strcpy( nameCopy, nodeName );
	unknown0C.SetAtGrow( index, nameCopy );
	controllers.SetAtGrow( index, controller );
	return index;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A1FE0

The time from now until the sequence next reaches the text key of that
name: the first such key after the reference controller's scaled time, or
the first one of the next pass (a looping controller) or of the way back
(a reversing one); the reference controller's start when there is none.
================
*/
// FUNCTION: STBC 0x7A1FE0
float NiKeyframeManager::Sequence::Unknown7A1FE0( const char *name_, float time ) {
	NiKeyframeController *reference = GetUnknown40Controller();
	float keyTime = reference->loKeyTime;
	float scaledTime = reference->ComputeScaledTime( GetOffset() + time );
	NiTimeController::CycleType cycle = reference->GetCycleType();
	bool first = cycle != NiTimeController::CLAMP;
	bool found = false;

	if ( textKeys ) {
		unsigned int numKeys;
		NiTextKey *keys = textKeys->GetKeys( numKeys );
		for ( unsigned int i = 0; i < numKeys; i++ ) {
			if ( !strcmp( keys[i].text, name_ ) ) {
				float t = keys[i].time;
				if ( t > scaledTime ) {
					keyTime = t;
					found = true;
					break;
				}
				if ( first || cycle == NiTimeController::REVERSE ) {
					keyTime = t;
					first = false;
				}
			}
		}
	}

	if ( cycle == NiTimeController::REVERSE ) {
		float hi = reference->hiKeyTime;
		float lo = reference->loKeyTime;
		bool backward = (int)( ( ( GetOffset() + time ) * reference->frequency + reference->phase ) / ( hi - lo ) ) % 2 == 1;
		if ( found ) {
			if ( backward ) {
				keyTime += 2.0f * ( scaledTime - lo );
			}
		} else if ( backward ) {
			keyTime = 2.0f * scaledTime - keyTime;
		} else {
			keyTime = 2.0f * hi - keyTime - lo;
		}
	} else if ( !found && cycle == NiTimeController::LOOP ) {
		keyTime += reference->hiKeyTime - reference->loKeyTime;
	}
	return ( keyTime - scaledTime + time - reference->phase ) / reference->frequency;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2170

Binds the controllers to the manager target's nodes from the first one;
when a node is missing, lets go of those bound.
================
*/
// FUNCTION: STBC 0x7A2170
bool NiKeyframeManager::Sequence::Unknown7A2170() {
	unsigned int uiIndex = 0;
	NiAVObject *pTarget = NiDynamicCast( NiAVObject, manager->GetTarget() );
	assert( pTarget );
	if ( Unknown7A2270( pTarget, uiIndex ) ) {
		return true;
	}
	if ( uiIndex == 0 ) {
		return false;
	}
	uiIndex--;
	bool bResult = Unknown7A24B0( pTarget, uiIndex );
	assert( bResult );
	return false;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A21F0

Lets the controllers go from the last one; when a node is missing, binds
again those let go.
================
*/
// FUNCTION: STBC 0x7A21F0
bool NiKeyframeManager::Sequence::Unknown7A21F0() {
	unsigned int uiIndex = unknown0C.GetSize() - 1;
	NiAVObject *pTarget = NiDynamicCast( NiAVObject, manager->GetTarget() );
	assert( pTarget );
	if ( Unknown7A24B0( pTarget, uiIndex ) ) {
		return true;
	}
	if ( uiIndex == unknown0C.GetSize() - 1 ) {
		return false;
	}
	bool bResult = Unknown7A2270( pTarget, uiIndex );
	assert( bResult );
	return false;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2270

The object, then its children in order: a node named for the controller
at index takes it, stopped, and index moves on; true once every controller
is bound. A controller of the manager's own target goes right after the
manager in the target's list.
================
*/
// FUNCTION: STBC 0x7A2270
bool NiKeyframeManager::Sequence::Unknown7A2270( NiAVObject *object, unsigned int &index ) {
	if ( !object ) {
		return false;
	}

	const char *objectName = object->GetName();
	if ( objectName ) {
		if ( !strcmp( objectName, unknown0C.GetAt( index ) ) && strlen( objectName ) == strlen( unknown0C.GetAt( index ) ) ) {
			NiPointer<NiKeyframeController> controller = controllers.GetAt( index++ );
			controller->SetTarget( object );
			controller->Stop();
			if ( object == manager->GetTarget() ) {
				object->RemoveController( controller );
				controller->SetNext( manager->GetNext() );
				manager->SetNext( controller );
			}
			controller = 0;
		}
	}
	if ( index == unknown0C.GetSize() ) {
		return true;
	}

	if ( object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			if ( Unknown7A2270( node->GetAt( i ), index ) ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A24B0

The children last first, then the object: a node named for the controller
at index lets it go, and index moves back; true once the first controller
is let go.
================
*/
// FUNCTION: STBC 0x7A24B0
bool NiKeyframeManager::Sequence::Unknown7A24B0( NiAVObject *object, unsigned int &index ) {
	if ( !object ) {
		return false;
	}

	if ( object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = node->GetArrayCount(); i > 0; ) {
			i--;
			if ( Unknown7A24B0( node->GetAt( i ), index ) ) {
				return true;
			}
		}
	}

	const char *objectName = object->GetName();
	if ( objectName ) {
		if ( !strcmp( objectName, unknown0C.GetAt( index ) ) && strlen( objectName ) == strlen( unknown0C.GetAt( index ) ) ) {
			NiPointer<NiKeyframeController> controller = controllers.GetAt( index );
			object->RemoveController( controller );
			controller->SetTarget( 0 );
			controller = 0;
			if ( index == 0 ) {
				return true;
			}
			index--;
		}
	}
	return false;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2690

The object, then its children: a node named for the next controller of
only one of the two sequences gets a clone of it (one of the second
sequence shifted by the difference of their offsets); true once both
sequences are used up.
================
*/
// FUNCTION: STBC 0x7A2690
// PARKED: inline budget: written as the debug copy, registers now retail's; the first branch's ~NiPointer<NiObjectNET> leaves DecRefCount out of line (share 66, needs 68: retail has 24 more left there, own cost +12, or one fewer site after it)
bool NiKeyframeManager::Sequence::Unknown7A2690( NiAVObject *object, unsigned int &firstIndex, unsigned int &secondIndex, Sequence *first, Sequence *second ) {
	// AnimOnly.exe 0x48E935 (NiKeyframeManager.cpp)
	bool firstMatch = false;
	bool secondMatch = false;
	bool more = false;
	const char *firstName;
	const char *secondName;

	if ( firstIndex != first->GetNodeCount() ) {
		firstName = first->GetNodeName( firstIndex );
		firstMatch = !strcmp( object->GetName(), firstName ) && strlen( object->GetName() ) == strlen( firstName );
		more = true;
	}
	if ( secondIndex != second->GetNodeCount() ) {
		secondName = second->GetNodeName( secondIndex );
		secondMatch = !strcmp( object->GetName(), secondName ) && strlen( object->GetName() ) == strlen( secondName );
		more = true;
	}

	if ( firstMatch && secondMatch ) {
		assert( false );
	} else if ( firstMatch ) {
		if ( firstIndex == GetUnknown40() ) {
			SetUnknown40( firstIndex + secondIndex );
		}
		NiKeyframeController *controller = first->GetController( firstIndex++ );
		NiPointer<NiObjectNET> target = controller->GetTarget();
		controller->SetTarget( 0 );
		NiKeyframeController *clone = (NiKeyframeController *)controller->CreateClone();
		controller->SetTarget( target );
		target = 0;
		Unknown7A1EB0( firstName, clone );
	} else if ( secondMatch ) {
		NiKeyframeController *controller = second->GetController( secondIndex++ );
		NiPointer<NiObjectNET> target = controller->GetTarget();
		controller->SetTarget( 0 );
		NiKeyframeController *clone = (NiKeyframeController *)controller->CreateClone();
		controller->SetTarget( target );
		target = 0;
		clone->SetPhase( clone->GetPhase() + second->GetOffset() - first->GetOffset() );
		Unknown7A1EB0( secondName, clone );
	}

	if ( !more ) {
		return true;
	}
	if ( NiIsKindOf( NiNode, object ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetChildCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( Unknown7A2690( child, firstIndex, secondIndex, first, second ) ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2A80

The ANIMATING step: the sequence at the time plus its offset.
================
*/
// FUNCTION: STBC 0x7A2A80
void NiKeyframeManager::Sequence::Unknown7A2A80( float time ) {
	Unknown7A2E30( GetOffset() + time );
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2AA0

The BLENDING step. Before the blend's start only this sequence plays, after
the destination's start only the destination; in between this sequence is
held at its start and the destination is weighted in by the fraction of the
blend that has passed.
================
*/
// FUNCTION: STBC 0x7A2AA0
void NiKeyframeManager::Sequence::Unknown7A2AA0( float time ) {
	float start = unknown4C;
	Sequence *destination = unknown50;
	float end = destination->unknown4C;

	if ( time < start ) {
		Unknown7A2A80( time );
		return;
	}
	if ( time > end ) {
		destination->Unknown7A2A80( time );
		return;
	}
	Unknown7A2E30( GetOffset() + start );
	destination->Unknown7A2EC0( destination->GetOffset() + end, ( time - start ) / ( end - start ) );
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2B40

The MORPHING step. Before the morph's start only this sequence plays, after
the destination's start only the destination; in between the destination
runs at its own time, this sequence follows at the matching time, and the
destination is weighted in by the fraction of the morph that has passed.
================
*/
// FUNCTION: STBC 0x7A2B40
void NiKeyframeManager::Sequence::Unknown7A2B40( float time ) {
	float start = unknown4C;
	Sequence *destination = unknown50;
	float end = destination->unknown4C;

	if ( time < start ) {
		Unknown7A2A80( time );
		return;
	}
	if ( time > end ) {
		destination->Unknown7A2A80( time );
		return;
	}
	float destinationTime = destination->GetOffset() + time;
	float sourceTime = Unknown7A2BF0( destination, destinationTime );
	Unknown7A2E30( sourceTime );
	destination->Unknown7A2EC0( destinationTime, ( time - start ) / ( end - start ) );
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2BF0

The time of this sequence that matches the destination's time. The
destination's "morph:" keys around its time give how far it is between two
of them; the keys of this sequence with the same names, and the same
fraction between them, give the time. A key past the end of the destination's
morph keys wraps to its first. As the debug copy has it (AnimOnly.exe
0x48EEAC, NiKeyframeManager.cpp).
================
*/
// FUNCTION: STBC 0x7A2BF0
float NiKeyframeManager::Sequence::Unknown7A2BF0( Sequence *destination, float time ) {
	NiKeyframeController *reference = destination->GetUnknown40Controller();
	float destinationTime = reference->ComputeScaledTime( time );

	NiTextKeyExtraData *textKeyData = destination->GetTextKeys();
	unsigned int i, numKeys;
	NiTextKey *keys = textKeyData->GetKeys( numKeys );
	NiTextKey *firstMorph = 0;
	NiTextKey *before = 0;
	NiTextKey *after = 0;
	NiTextKey *lastMorph = 0;
	for ( i = 0; i < numKeys; i++ ) {
		if ( !strncmp( keys[i].GetText(), MORPH_PREFIX, 6 ) ) {
			lastMorph = &keys[i];
			if ( !firstMorph ) {
				firstMorph = lastMorph;
			}
			if ( keys[i].GetTime() < destinationTime ) {
				before = lastMorph;
			} else if ( !after ) {
				after = lastMorph;
			}
		}
	}

	// BUG: a destination without "morph:" keys leaves before null and reads it
	float fraction;
	if ( !after || !before ) {
		if ( !after ) {
			float beforeTime = before->GetTime();
			float afterTime = reference->GetEndKeyTime() - reference->GetBeginKeyTime() + firstMorph->GetTime();
			fraction = ( destinationTime - beforeTime ) / ( afterTime - beforeTime );
			after = firstMorph;
		} else {
			float beforeTime = reference->GetBeginKeyTime() - reference->GetEndKeyTime() + lastMorph->GetTime();
			float afterTime = after->GetTime();
			fraction = ( destinationTime - beforeTime ) / ( afterTime - beforeTime );
			before = lastMorph;
		}
	} else {
		float beforeTime = before->GetTime();
		float afterTime = after->GetTime();
		fraction = ( destinationTime - beforeTime ) / ( afterTime - beforeTime );
	}

	textKeyData = GetTextKeys();
	assert( textKeyData );
	keys = textKeyData->GetKeys( numKeys );
	bool beforeFound = before == 0;
	bool afterFound = after == 0;
	NiTextKey *sourceBefore;
	NiTextKey *sourceAfter;
	for ( i = 0; i < numKeys; i++ ) {
		if ( beforeFound && afterFound ) {
			break;
		}
		if ( !beforeFound && !strcmp( keys[i].GetText(), before->GetText() ) ) {
			sourceBefore = &keys[i];
			beforeFound = true;
		}
		if ( !afterFound && !strcmp( keys[i].GetText(), after->GetText() ) ) {
			sourceAfter = &keys[i];
			afterFound = true;
		}
	}

	float afterTime = sourceAfter->GetTime();
	float beforeTime = sourceBefore->GetTime();
	if ( beforeTime > afterTime ) {
		afterTime += GetUnknown40Controller()->GetEndKeyTime() - GetUnknown40Controller()->GetBeginKeyTime();
	}
	return ( afterTime - beforeTime ) * fraction + beforeTime;
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2E30

Runs every controller that has keys once at the time.
================
*/
// FUNCTION: STBC 0x7A2E30
void NiKeyframeManager::Sequence::Unknown7A2E30( float time ) {
	for ( unsigned int i = 0; i < controllers.GetSize(); i++ ) {
		NiKeyframeController *controller = controllers.GetAt( i );
		if ( controller->GetData() ) {
			controller->Start( -NI_INFINITY );
			controller->Update( time );
			controller->Stop();
		}
	}
}

/*
================
NiKeyframeManager::Sequence::Unknown7A2EC0

Runs every controller that has keys once at the time and weights the
transform it sets against the one its target had before: the translation
and the scale linearly, the rotation by slerp along the shorter arc. A
controller whose counterpart in the sequence being blended from has no keys
sets the transform outright.
================
*/
// FUNCTION: STBC 0x7A2EC0
void NiKeyframeManager::Sequence::Unknown7A2EC0( float time, float weight ) {
	float otherWeight = 1.0f - weight;
	for ( unsigned int i = 0; i < controllers.GetSize(); i++ ) {
		NiKeyframeController *controller = controllers.GetAt( i );
		if ( controller->GetData() ) {
			if ( !unknown50->controllers.GetAt( i )->GetData() ) {
				controller->Start( -NI_INFINITY );
				controller->Update( time );
				controller->Stop();
			} else {
				NiAVObject *target = NiDynamicCast( NiAVObject, controller->target );

				NiPoint3 translate = target->GetTranslate();
				NiQuaternion rotate;
				target->GetRotate( rotate );
				float scale = target->GetScale();

				controller->Start( -NI_INFINITY );
				controller->Update( time );
				controller->Stop();

				NiPoint3 newTranslate = target->GetTranslate();
				NiQuaternion newRotate;
				target->GetRotate( newRotate );
				float newScale = target->GetScale();

				NiPoint3 blendedTranslate = translate * otherWeight + newTranslate * weight;
				if ( NiQuaternion::Dot( rotate, newRotate ) < 0.0f ) {
					rotate = -rotate;
				}
				NiQuaternion blendedRotate = NiQuaternion::Slerp( weight, rotate, newRotate );
				float blendedScale = scale * otherWeight + newScale * weight;

				target->SetTranslate( blendedTranslate );
				target->SetRotate( blendedRotate );
				if ( blendedScale > 0.0f ) {
					target->SetScale( blendedScale );
				} else {
					target->SetScale( -blendedScale );
				}
			}
		}
	}
}

/*
================
NiKeyframeManager::NiKeyframeManager

Takes the target's controllers' head, out of its list first if the target
has controllers.
================
*/
// FUNCTION: STBC 0x7A3150
NiKeyframeManager::NiKeyframeManager( NiAVObject *target_ ) {
	if ( target_->GetControllers() ) {
		target_->RemoveController( this );
	}
	target_->PrependController( this );
	target = target_;
}

/*
================
NiKeyframeManager::~NiKeyframeManager

Destroys every sequence.
================
*/
// FUNCTION: STBC 0x7A3200
NiKeyframeManager::~NiKeyframeManager() {
	NiTMapIterator pos = sequences.GetFirstPos();
	while ( pos ) {
		const char *name;
		Sequence *sequence;
		sequences.GetNext( pos, name, sequence );
		delete sequence;
	}
}

/*
================
NiKeyframeManager::Unknown7A3360

Loads the sequence at index of the file under name and adds it.
================
*/
// FUNCTION: STBC 0x7A3360
void NiKeyframeManager::Unknown7A3360( const char *file, const char *name, unsigned int index ) {
	Sequence *sequence = Sequence::Load( file, name, index );
	if ( sequence ) {
		Unknown7A3390( sequence );
	}
}

/*
================
NiKeyframeManager::Unknown7A3390

Adds the sequence under its name unless another manager already owns it.
================
*/
// FUNCTION: STBC 0x7A3390
void NiKeyframeManager::Unknown7A3390( Sequence *sequence ) {
	if ( !sequence->manager ) {
		sequence->manager = this;
		sequences.SetAt( sequence->name, sequence );
	}
}

/*
================
NiKeyframeManager::Unknown7A3430

Drops the sequence of that name from the map, without deleting it.
================
*/
// FUNCTION: STBC 0x7A3430
bool NiKeyframeManager::Unknown7A3430( const char *name ) {
	Sequence *sequence;
	sequences.GetAt( name, sequence );
	// BUG: a name not in the map leaves sequence unset; and SetManager only
	// sets a manager where there is none, so the sequence keeps this one
	sequence->SetManager( 0 );
	return sequences.RemoveAt( name );
}

/*
================
NiKeyframeManager::Unknown7A3510

Drops the sequence from the map, without deleting it.
================
*/
// FUNCTION: STBC 0x7A3510
bool NiKeyframeManager::Unknown7A3510( Sequence *sequence ) {
	// BUG: SetManager only sets a manager where there is none, so the
	// sequence keeps this one
	sequence->SetManager( 0 );
	return sequences.RemoveAt( sequence->name );
}

/*
================
NiKeyframeManager::Unknown7A35C0

Drops every sequence from the map, without deleting them.
================
*/
// FUNCTION: STBC 0x7A35C0
void NiKeyframeManager::Unknown7A35C0() {
	NiTMapIterator pos = sequences.GetFirstPos();
	while ( pos ) {
		const char *name;
		Sequence *sequence;
		sequences.GetNext( pos, name, sequence );
		// BUG: as in Unknown7A3510, the sequence keeps this manager
		sequence->SetManager( 0 );
		sequences.RemoveAt( name );
	}
}

/*
================
NiKeyframeManager::Unknown7A36E0

Starts the sequence of that name animating.
================
*/
// FUNCTION: STBC 0x7A36E0
bool NiKeyframeManager::Unknown7A36E0( const char *name ) {
	Sequence *sequence;
	if ( !sequences.GetAt( name, sequence ) || !sequence ) {
		return false;
	}
	return Unknown7A3740( sequence );
}

/*
================
NiKeyframeManager::Unknown7A3740

Binds the sequence's controllers and starts it animating.
================
*/
// FUNCTION: STBC 0x7A3740
bool NiKeyframeManager::Unknown7A3740( Sequence *sequence ) {
	if ( sequence->Unknown7A2170() ) {
		sequence->state = Sequence::ANIMATING;
		return true;
	}
	return false;
}

/*
================
NiKeyframeManager::Unknown7A3770

Blends from the sequence of the first name to that of the second.
================
*/
// FUNCTION: STBC 0x7A3770
bool NiKeyframeManager::Unknown7A3770( const char *name, const char *destinationName, const char *textKey, float start, float end ) {
	Sequence *sequence = 0;
	Sequence *destination = 0;
	sequences.GetAt( name, sequence );
	sequences.GetAt( destinationName, destination );
	if ( !sequence || !destination ) {
		return false;
	}
	return Unknown7A3830( sequence, destination, textKey, start, end );
}

/*
================
NiKeyframeManager::Unknown7A3830

Starts a blend from the sequence to the destination, both inactive: the
sequence blends from start, and the destination plays from its text key
of that name (or its start) once the blend reaches end.
================
*/
// FUNCTION: STBC 0x7A3830
bool NiKeyframeManager::Unknown7A3830( Sequence *sequence, Sequence *destination, const char *textKey, float start, float end ) {
	assert( end > start );
	if ( sequence->GetState() != Sequence::INACTIVE || destination->GetState() != Sequence::INACTIVE ) {
		NiOutputDebugString( "Attempting to blend a sequence that's already animating\n" );
		return false;
	}
	if ( !destination->Unknown7A2170() || !sequence->Unknown7A2170() ) {
		NiOutputDebugString( "Cannot activate a blending sequence\n" );
		return false;
	}

	sequence->SetPartner( destination );
	destination->SetPartner( sequence );
	float keyTime = 0.0f;
	if ( textKey ) {
		NiTextKeyExtraData *extraData = destination->GetTextKeys();
		if ( extraData ) {
			unsigned int numTextKeys;	// (name invented; fits the /Od frame order)
			NiTextKey *keys = extraData->GetKeys( numTextKeys );
			for ( unsigned int i = 0; i < numTextKeys; i++ ) {
				if ( !strcmp( keys[i].GetText(), textKey ) ) {
					keyTime = keys[i].GetTime();
					NiKeyframeController *controller = destination->GetUnknown40Controller();
					keyTime -= controller->GetPhase();
					keyTime /= controller->GetFrequency();
					break;
				}
			}
		}
	}
	sequence->SetUnknown4C( start );
	sequence->SetState( Sequence::BLENDING );
	destination->SetUnknown4C( end );
	destination->SetOffset( keyTime - end );
	destination->SetState( Sequence::BLEND_DESTINATION );
	return true;
}

/*
================
NiKeyframeManager::Unknown7A3970

Morphs from the sequence of the first name to that of the second.
================
*/
// FUNCTION: STBC 0x7A3970
bool NiKeyframeManager::Unknown7A3970( const char *name, const char *destinationName, float start, float end ) {
	Sequence *sequence = 0;
	Sequence *destinationSequence = 0;	// (name invented; fits the /Od frame order)
	sequences.GetAt( name, sequence );
	sequences.GetAt( destinationName, destinationSequence );
	if ( sequence && destinationSequence ) {
		return Unknown7A3A30( sequence, destinationSequence, start, end );
	}
	return false;
}

/*
================
NiKeyframeManager::Unknown7A3A30

Starts a morph from the sequence to the destination, both inactive: the
destination starts at its time matching the sequence's at start.
================
*/
// FUNCTION: STBC 0x7A3A30
bool NiKeyframeManager::Unknown7A3A30( Sequence *sequence, Sequence *destination, float start, float end ) {
	assert( end > start );
	if ( sequence->GetState() != Sequence::INACTIVE || destination->GetState() != Sequence::INACTIVE ) {
		NiOutputDebugString( "Attempting to morph a sequence that's already animating\n" );
		return false;
	}
	if ( !destination->Unknown7A2170() || !sequence->Unknown7A2170() ) {
		NiOutputDebugString( "Cannot activate a blending sequence\n" );
		return false;
	}

	sequence->SetPartner( destination );
	destination->SetPartner( sequence );
	float time = destination->Unknown7A2BF0( sequence, sequence->GetOffset() + start );
	NiKeyframeController *controller = destination->GetUnknown40Controller();	// (name invented; fits the /Od frame order)
	time -= controller->GetPhase();
	time /= controller->GetFrequency();
	sequence->SetUnknown4C( start );
	sequence->SetState( Sequence::MORPHING );
	destination->SetUnknown4C( end );
	destination->SetOffset( time - start );
	destination->SetState( Sequence::MORPH_DESTINATION );
	return true;
}

/*
================
NiKeyframeManager::Unknown7A3AF0

Stops the sequence of that name.
================
*/
// FUNCTION: STBC 0x7A3AF0
bool NiKeyframeManager::Unknown7A3AF0( const char *name ) {
	Sequence *sequence = 0;
	sequences.GetAt( name, sequence );
	if ( sequence ) {
		return Unknown7A3B50( sequence );
	}
	return false;
}

/*
================
NiKeyframeManager::Unknown7A3B50

Lets the sequence's controllers go and makes it inactive.
================
*/
// FUNCTION: STBC 0x7A3B50
bool NiKeyframeManager::Unknown7A3B50( Sequence *sequence ) {
	if ( sequence->Unknown7A21F0() ) {
		sequence->SetState( Sequence::INACTIVE );
		return true;
	}
	return false;
}

/*
================
NiKeyframeManager::Unknown7A3B80

Stops the blend between the sequences of those names.
================
*/
// FUNCTION: STBC 0x7A3B80
bool NiKeyframeManager::Unknown7A3B80( const char *name, const char *destinationName ) {
	Sequence *sequence = 0;
	Sequence *destinationSequence = 0;	// (name invented; fits the /Od frame order)
	sequences.GetAt( name, sequence );
	sequences.GetAt( destinationName, destinationSequence );
	if ( sequence && destinationSequence ) {
		return Unknown7A3C30( sequence, destinationSequence );
	}
	return false;
}

/*
================
NiKeyframeManager::Unknown7A3C30

Lets both sequences' controllers go, the destination's first, and makes
both inactive.
================
*/
// FUNCTION: STBC 0x7A3C30
bool NiKeyframeManager::Unknown7A3C30( Sequence *sequence, Sequence *destination ) {
	if ( !destination->Unknown7A21F0() || !sequence->Unknown7A21F0() ) {
		NiOutputDebugString( "Cannot deactivate a blending sequence\n" );
		return false;
	}
	sequence->SetState( Sequence::INACTIVE );
	destination->SetState( Sequence::INACTIVE );
	sequence->SetPartner( 0 );
	destination->SetPartner( 0 );
	return true;
}

/*
================
NiKeyframeManager::Unknown7A3C80

Stops the morph between the sequences of those names.
================
*/
// FUNCTION: STBC 0x7A3C80
bool NiKeyframeManager::Unknown7A3C80( const char *name, const char *destinationName ) {
	Sequence *sequence = 0;
	Sequence *destinationSequence = 0;	// (name invented; fits the /Od frame order)
	sequences.GetAt( name, sequence );
	sequences.GetAt( destinationName, destinationSequence );
	if ( sequence && destinationSequence ) {
		return Unknown7A3D30( sequence, destinationSequence );
	}
	return false;
}

/*
================
NiKeyframeManager::Unknown7A3D30

As Unknown7A3C30, for a morph.
================
*/
// FUNCTION: STBC 0x7A3D30
bool NiKeyframeManager::Unknown7A3D30( Sequence *sequence, Sequence *destination ) {
	if ( !destination->Unknown7A21F0() || !sequence->Unknown7A21F0() ) {
		NiOutputDebugString( "Cannot deactivate a morphing sequence\n" );
		return false;
	}
	sequence->SetState( Sequence::INACTIVE );
	destination->SetState( Sequence::INACTIVE );
	sequence->SetPartner( 0 );
	destination->SetPartner( 0 );
	return true;
}

/*
================
NiKeyframeManager::Unknown7A3D80

Stops every sequence; false when any of them could not let its
controllers go.
================
*/
// FUNCTION: STBC 0x7A3D80
bool NiKeyframeManager::Unknown7A3D80() {
	bool result = true;
	const char *name;
	Sequence *pSequence;	// (name invented; fits the /Od frame order)
	NiTMapIterator pos = sequences.GetFirstPos();
	while ( pos ) {
		sequences.GetNext( pos, name, pSequence );
		result = pSequence->Unknown7A21F0() & result;
		pSequence->SetState( Sequence::INACTIVE );
	}
	return result;
}

/*
================
NiKeyframeManager::Update

Steps every sequence by its state.
================
*/
// FUNCTION: STBC 0x7A3E10
void NiKeyframeManager::Update( float time ) {
	const char *name;
	Sequence *pSequence;	// (name invented; fits the /Od frame order)
	NiTMapIterator pos = sequences.GetFirstPos();
	while ( pos ) {
		sequences.GetNext( pos, name, pSequence );
		switch ( pSequence->GetState() ) {
		case Sequence::ANIMATING:
			pSequence->Unknown7A2A80( time );
			break;
		case Sequence::BLENDING:
			pSequence->Unknown7A2AA0( time );
			break;
		case Sequence::MORPHING:
			pSequence->Unknown7A2B40( time );
			break;
		}
	}
}

/*
================
NiKeyframeManager::CreateClone

The clone takes the controller's members, not the sequences.
================
*/
// FUNCTION: STBC 0x7A3EB0
NiObject *NiKeyframeManager::CreateClone() {
	NiKeyframeManager *clone = new NiKeyframeManager;
	CopyMembers( clone );
	return clone;
}

/*
================
NiKeyframeManager::CopyMembers
================
*/
// FUNCTION: STBC 0x7A3F60
void NiKeyframeManager::CopyMembers( NiKeyframeManager *dest ) {
	NiTimeController::CopyMembers( dest );
}

/*
================
NiKeyframeManager::ProcessClone
================
*/
// FUNCTION: STBC 0x7A3F70
void NiKeyframeManager::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiKeyframeManager, clone ) );
	NiTimeController::ProcessClone( clone );
}

/*
================
NiKeyframeManager::CreateObject
================
*/
// FUNCTION: STBC 0x7A3F80
void NiKeyframeManager::CreateObject( NiStream &stream, NiObject *&object ) {
	NiKeyframeManager *manager = new NiKeyframeManager;
	stream.SetLoadRecord( new NiKeyframeManagerLoadRecord );
	manager->LoadBinary( stream );
	object = manager;
}

/*
================
NiKeyframeManager::LoadBinary

Each sequence is its name and a flag. A sequence kept in a file of its own
is the file name and the sequence's index in it. Any other is saved whole:
its 0x40 value, the link ID of its text keys, and the node name and link ID
of each controller, all for LinkObject.
================
*/
// FUNCTION: STBC 0x7A41B0
void NiKeyframeManager::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );

	NiKeyframeManagerLoadRecord *record = (NiKeyframeManagerLoadRecord *)stream.GetLoadRecord();
	unsigned int sequenceCount;	// (name invented; fits the /Od frame order)
	NiStreamLoadBinary( stream, sequenceCount );
	while ( sequenceCount-- ) {
		char *name = 0;
		stream.LoadCString( name );
		bool inFile;
		NiStreamLoadBinary( stream, inFile );
		if ( inFile ) {
			char *file = 0;
			stream.LoadCString( file );
			unsigned int index;
			NiStreamLoadBinary( stream, index );
			Unknown7A3360( file, name, index );
			delete [] name;
			delete [] file;
		} else {
			record->sequenceNames.Add( name );
			long value;
			NiStreamLoadBinary( stream, value );
			record->sequenceValues.Add( value );
			NiTextKeyExtraData *textKeysRef;	// (name invented; fits the /Od frame order)
			NiStreamLoadBinary( stream, textKeysRef );
			record->textKeyLinkIDs.Add( textKeysRef );
			long numControllers;
			NiStreamLoadBinary( stream, numControllers );
			record->controllerCounts.Add( numControllers );
			while ( numControllers-- ) {
				char *nodeName = 0;
				stream.LoadCString( nodeName );
				record->controllerNames.Add( nodeName );
				NiKeyframeController *controllerID;	// (name invented; fits the /Od frame order)
				NiStreamLoadBinary( stream, controllerID );
				record->controllerLinkIDs.Add( controllerID );
			}
		}
	}
}

/*
================
NiKeyframeManager::LinkObject

Builds the sequences LoadBinary recorded: each one is named, takes its 0x40
value and text keys, and gets its controllers from the node names and link
IDs in sequence order.
================
*/
// FUNCTION: STBC 0x7A4480
void NiKeyframeManager::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );

	NiKeyframeManagerLoadRecord *record = (NiKeyframeManagerLoadRecord *)stream.GetLoadRecord();
	unsigned int numSequences = record->controllerCounts.GetSize();
	unsigned int i, controllerIndex;
	for ( i = 0, controllerIndex = 0; i < numSequences; i++ ) {
		char *name = record->sequenceNames.GetAt( i );
		Sequence *sequence = new Sequence( name, 12, 12 );
		delete [] name;
		sequence->SetUnknown40( record->sequenceValues.GetAt( i ) );
		NiTextKeyExtraData *textKeysRef = record->textKeyLinkIDs.GetAt( i );
		NiTextKeyExtraData *textKeys = (NiTextKeyExtraData *)stream.GetObjectFromLinkID( (unsigned int)textKeysRef );
		sequence->SetTextKeys( textKeys );
		long numControllers = record->controllerCounts.GetAt( i );
		while ( numControllers-- ) {
			unsigned int k = controllerIndex++;
			char *nodeName = record->controllerNames.GetAt( k );
			NiKeyframeController *controllerID = record->controllerLinkIDs.GetAt( k );
			NiKeyframeController *controller = (NiKeyframeController *)stream.GetObjectFromLinkID( (unsigned int)controllerID );
			sequence->Unknown7A1EB0( nodeName, controller );
			delete [] nodeName;
		}
		Unknown7A3390( sequence );
	}
}

/*
================
NiKeyframeManager::RegisterStreamables

Registers each sequence's text keys and controllers, unless the sequences
are saved as references to their files.
================
*/
// FUNCTION: STBC 0x7A45D0
bool NiKeyframeManager::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( !saveFileReferences ) {
		const char *name;
		Sequence *pSequence;	// (name invented; fits the /Od frame order)
		NiTMapIterator pos = sequences.GetFirstPos();
		while ( pos ) {
			sequences.GetNext( pos, name, pSequence );
			NiTextKeyExtraData *textKeyData = pSequence->GetTextKeys();	// (name invented; fits the /Od frame order)
			if ( textKeyData ) {
				textKeyData->RegisterStreamables( stream );
			}
			for ( unsigned int i = 0; i < pSequence->GetNodeCount(); i++ ) {
				pSequence->GetController( i )->RegisterStreamables( stream );
			}
		}
	}
	return true;
}

/*
================
NiKeyframeManager::SaveBinary

Each sequence is its name and a flag. A sequence that came from a file is
saved as the file name and its index when references are saved; any other
is saved whole.
The file name is read into a local before the reference flag is tested,
as the SDK's debug NetImmerse 3.1 does (AnimOnly.exe 0x490564).
================
*/
// FUNCTION: STBC 0x7A4730
void NiKeyframeManager::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );

	unsigned int entryCount = sequences.GetCount();	// (name invented; fits the /Od frame order)
	NiStreamSaveBinary( stream, entryCount );

	const char *name;
	Sequence *pSequence;	// (name invented; fits the /Od frame order)
	NiTMapIterator pos = sequences.GetFirstPos();
	while ( pos ) {
		sequences.GetNext( pos, name, pSequence );
		stream.SaveCString( name );
		const char *filename = pSequence->GetFilename();
		if ( saveFileReferences && filename ) {
			NiStreamSaveBinary( stream, true );
			stream.SaveCString( pSequence->GetFilename() );
			NiStreamSaveBinary( stream, pSequence->GetUnknown08() );
		} else {
			NiStreamSaveBinary( stream, false );
			NiStreamSaveBinary( stream, pSequence->GetUnknown40() );
			NiStreamSaveBinary( stream, pSequence->GetTextKeys() );
			unsigned int numControllers = pSequence->GetNodeCount();
			NiStreamSaveBinary( stream, numControllers );
			for ( unsigned int i = 0; i < numControllers; i++ ) {
				stream.SaveCString( pSequence->GetNodeName( i ) );
				NiStreamSaveBinary( stream, pSequence->GetController( i ) );
			}
		}
	}
}

/*
================
NiKeyframeManager::IsEqual

The sequences are compared in map order: by name, then either by file and
index, or by text keys and, per controller, node name and controller.
================
*/
// FUNCTION: STBC 0x7A4930
bool NiKeyframeManager::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiKeyframeManager *manager = (NiKeyframeManager *)object;
	if ( GetSequenceCount() != manager->GetSequenceCount() ) {
		return false;
	}

	NiTStringMap<Sequence *> *otherSequences = manager->GetSequences();
	NiTMapIterator pos = sequences.GetFirstPos();
	NiTMapIterator otherPos = otherSequences->GetFirstPos();
	while ( pos && otherPos ) {
		const char *name;
		Sequence *sequence;
		sequences.GetNext( pos, name, sequence );
		const char *otherName;
		Sequence *otherSequence;
		otherSequences->GetNext( otherPos, otherName, otherSequence );
		if ( strcmp( name, otherName ) != 0 ) {
			return false;
		}
		const char *filename = sequence->GetFilename();
		const char *otherFilename = otherSequence->GetFilename();
		if ( filename && otherFilename ) {
			if ( strcmp( filename, otherFilename ) != 0 ) {
				return false;
			}
			if ( sequence->GetUnknown08() != otherSequence->GetUnknown08() ) {
				return false;
			}
		} else {
			NiTextKeyExtraData *textKeys = sequence->GetTextKeys();
			NiTextKeyExtraData *otherTextKeys = otherSequence->GetTextKeys();
			if ( ( textKeys && !otherTextKeys ) || ( !textKeys && otherTextKeys ) ) {
				return false;
			}
			if ( textKeys && !textKeys->IsEqual( otherTextKeys ) ) {
				return false;
			}
			unsigned int numControllers = sequence->GetNodeCount();
			if ( numControllers != otherSequence->GetNodeCount() ) {
				return false;
			}
			for ( unsigned int i = 0; i < numControllers; i++ ) {
				if ( strcmp( sequence->GetNodeName( i ), otherSequence->GetNodeName( i ) ) != 0 ) {
					return false;
				}
				if ( !sequence->GetController( i )->IsEqual( otherSequence->GetController( i ) ) ) {
					return false;
				}
			}
		}
	}
	if ( pos || otherPos ) {
		return false;
	}
	return true;
}
