// NiAnimBlender.cpp -- keyframe sequences blended and morphed on one tree

#include "NiRTLib.h"
#include <assert.h>
#include <math.h>
#include <string.h>

#include "NiAnimBlender.h"
#include "NiAVObject.h"
#include "NiCloneExtraData.h"
#include "NiLinPosKey.h"
#include "NiLinRotKey.h"
#include "NiEulerRotKey.h"
#include "NiMath.h"
#include "NiNode.h"
#include "NiTextKeyExtraData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0079b620 _$E30
// FUNCTION: STBC 0x0079b630 _$E29
// FUNCTION: STBC 0x0079b640 _$E41
// FUNCTION: STBC 0x0079b650 _$E40
// FUNCTION: STBC 0x0079b660 _$E92
// FUNCTION: STBC 0x0079b670 _$E91
// FUNCTION: STBC 0x0079b680 _$E96
// FUNCTION: STBC 0x0079b690 _$E95
// FUNCTION: STBC 0x0079b6a0 _$E100
// FUNCTION: STBC 0x0079b6b0 _$E99
// FUNCTION: STBC 0x0079b6c0 _$E217
// FUNCTION: STBC 0x0079b6d0 _$E216
// FUNCTION: STBC 0x0079b6e0 _$E237
// FUNCTION: STBC 0x0079b6f0 _$E236
// FUNCTION: STBC 0x0079b700 _$E241
// FUNCTION: STBC 0x0079b710 _$E240
// FUNCTION: STBC 0x0079b720 _$E80
// FUNCTION: STBC 0x0079b730 _$E79
// FUNCTION: STBC 0x0079b740 _$E60
// FUNCTION: STBC 0x0079b750 _$E59
// FUNCTION: STBC 0x0079b760 _$E69
// FUNCTION: STBC 0x0079b770 _$E68
// FUNCTION: STBC 0x0079b780 _$E84
// FUNCTION: STBC 0x0079b790 _$E83
// FUNCTION: STBC 0x0079b7a0 _$E88
// FUNCTION: STBC 0x0079b7b0 _$E87
// FUNCTION: STBC 0x0079b7f0 _$E225
// FUNCTION: STBC 0x0079b800 _$E224
// FUNCTION: STBC 0x0079b810 _$E228
// FUNCTION: STBC 0x0079b820 _$E227
// FUNCTION: STBC 0x0079b830 _$E232
// FUNCTION: STBC 0x0079b840 _$E231
// FUNCTION: STBC 0x0079b850 _$E245
// FUNCTION: STBC 0x0079b860 _$E244
// FUNCTION: STBC 0x0079b8d0 _$E221
// FUNCTION: STBC 0x0079b8e0 _$E220

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0079b7c0 _$E273
// FUNCTION: STBC 0x0079b7e0 _$E272
// FUNCTION: STBC 0x0079b280 _$E17
// FUNCTION: STBC 0x0079b290 _$E14
// FUNCTION: STBC 0x0079b2a0 _$E16
// FUNCTION: STBC 0x0079b2b0 _$E15
// FUNCTION: STBC 0x0079b2c0 _$E23
// FUNCTION: STBC 0x0079b2d0 _$E20
// FUNCTION: STBC 0x0079b2e0 _$E22
// FUNCTION: STBC 0x0079b2f0 _$E21

// FUNCTION: STBC 0x00797600 _$E251
// FUNCTION: STBC 0x00797610 _$E247
// FUNCTION: STBC 0x00797630 _$E250
// FUNCTION: STBC 0x00797640 _$E248
// DATA: STBC 0x9A0220 ?rtti@NiAnimBlender@@2VNiRTTI@@B
const NiRTTI NiAnimBlender::rtti( "NiAnimBlender", &NiTimeController::rtti );

// the prefix of the text keys a morph may leave or join a sequence at
// DATA: STBC 0x976068
const char MORPH_PREFIX[] = "morph:";

/*
================
NiAnimBlender::Register
================
*/
// FUNCTION: STBC 0x797660 ?Register@NiAnimBlender@@SAHXZ
NiImplementRegister( NiAnimBlender )

/*
================
NiAnimBlender::NiAnimBlender
================
*/
// FUNCTION: STBC 0x7977B0
NiAnimBlender::NiAnimBlender() {
	loKeyTime = 0.0f;
	numNodes = 0;
	root = 0;
	nodes = 0;
	controllers = 0;
	numSequences = 0;
	activeSequence = -1;
	posKeys = 0;
	numPosKeys = 0;
	posKeyTypes = 0;
	rotKeys = 0;
	numRotKeys = 0;
	rotKeyTypes = 0;
	sequenceLengths = 0;
	textKeys = 0;
	numTextKeys = 0;
	cycleTypes = 0;
	blendSequence = -1;
	blendDestinationTime = NI_INFINITY;
	blendStartTime = NI_INFINITY;
	blendDuration = -NI_INFINITY;
	morphDestination = -1;
	morphSource = -1;
	unknown88 = NI_INFINITY;
	morphPhase = NI_INFINITY;
	morphStartTime = NI_INFINITY;
	morphDuration = -NI_INFINITY;
	sourcePos = 0;
	sourceRot = 0;
	destinationPos = 0;
	destinationRot = 0;
	blending = false;
	morphing = false;
}

/*
================
NiAnimBlender::~NiAnimBlender

Frees the node arrays, every sequence's keys and text keys, and the blend's
poses. An Euler rotation key owns three groups of float keys, which it
frees first.
================
*/
// FUNCTION: STBC 0x797880
NiAnimBlender::~NiAnimBlender() {
	delete [] nodes;
	delete [] controllers;

	unsigned int i, j, k;

	if ( posKeys ) {
		for ( i = 0; i < numSequences; i++ ) {
			for ( j = 0; j < numNodes; j++ ) {
				for ( k = 0; k < numPosKeys[i][j]; k++ ) {
					delete posKeys[i][j][k];
				}
				delete [] posKeys[i][j];
			}
			delete [] posKeys[i];
			delete [] numPosKeys[i];
			delete [] posKeyTypes[i];
		}
		delete [] posKeys;
		delete [] posKeyTypes;
		delete [] numPosKeys;
	}

	if ( rotKeys ) {
		for ( i = 0; i < numSequences; i++ ) {
			for ( j = 0; j < numNodes; j++ ) {
				if ( rotKeyTypes[i][j] == NiRotKey::EULERKEY ) {
					( (NiEulerRotKey *)rotKeys[i][j][0] )->Cleanup();
				}
				for ( k = 0; k < numRotKeys[i][j]; k++ ) {
					delete rotKeys[i][j][k];
				}
				delete [] rotKeys[i][j];
			}
			delete [] rotKeys[i];
			delete [] numRotKeys[i];
			delete [] rotKeyTypes[i];
		}
		delete [] rotKeys;
		delete [] numRotKeys;
		delete [] rotKeyTypes;
	}

	delete [] sequenceLengths;

	if ( textKeys ) {
		for ( i = 0; i < numSequences; i++ ) {
			delete [] textKeys[i];
		}
		delete [] textKeys;
	}
	delete [] numTextKeys;
	delete [] cycleTypes;

	delete [] sourcePos;
	delete [] sourceRot;
	delete [] destinationPos;
	delete [] destinationRot;
}

/*
================
NiAnimBlender::CountNodes
================
*/
// FUNCTION: STBC 0x797B70
void NiAnimBlender::CountNodes( NiNode *node, unsigned int &count ) {
	count++;
	for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
		NiNode *child = NiDynamicCast( NiNode, node->GetAt( i ) );
		if ( child ) {
			CountNodes( child, count );
		}
	}
}

/*
================
NiAnimBlender::GatherNodes

A node without a keyframe controller gets a new one.
================
*/
// FUNCTION: STBC 0x797BF0
void NiAnimBlender::GatherNodes( NiNode *node, unsigned int &index ) {
	nodes[index] = node;
	controllers[index] = NiKeyframeController::FindKeyframeController( node );
	if ( !controllers[index] ) {
		controllers[index] = new NiKeyframeController;
		controllers[index]->SetTarget( node );
	}
	index++;

	for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
		NiNode *child = NiDynamicCast( NiNode, node->GetAt( i ) );
		if ( child ) {
			GatherNodes( child, index );
		}
	}
}

/*
================
NiAnimBlender::SetRoot
================
*/
// FUNCTION: STBC 0x797D70
void NiAnimBlender::SetRoot( NiNode *root_ ) {
	assert( !root && root_ );
	root = root_;
	numNodes = 0;
	CountNodes( root, numNodes );

	nodes = new NiNode *[numNodes];
	controllers = new NiKeyframeController *[numNodes];
	unsigned int uiIndex = 0;
	GatherNodes( root, uiIndex );
	assert( uiIndex == numNodes );

	sourcePos = new NiPosKey[numNodes];
	sourceRot = new NiRotKey[numNodes];
	destinationPos = new NiPosKey[numNodes];
	destinationRot = new NiRotKey[numNodes];
}

/*
================
NiAnimBlender::GatherKeys

The keys of each node of the tree under object, depth first. node walks the
root's tree alongside, checked only by asserts. Without copyKeys the
sequence shares the controllers' key arrays; a node without keys gets one
linear key at its current transform. Statements, asserts and the linear
key classes as the debug copy has them (AnimOnly.exe 0x47F900,
NiAnimBlender.cpp), which reads the copy functions through accessors this
tree's NiPosKey and NiRotKey don't declare yet.
================
*/
// FUNCTION: STBC 0x797F30
void NiAnimBlender::GatherKeys( NiNode *object, NiNode *node, unsigned int *numPos, NiPosKey::KeyType *posTypes,
								NiPosKey ***pos, unsigned int *numRot, NiRotKey::KeyType *rotTypes, NiRotKey ***rot,
								unsigned int &index, float &loTime, float &hiTime, bool copyKeys ) {
	assert( node == nodes[index] );

	NiKeyframeController *controller = NiKeyframeController::FindKeyframeController( object );
	if ( controller ) {
		unsigned int numPosKeys;
		NiPosKey::KeyType posKeyType;
		NiPosKey **posKeys = controller->GetPosAnim( numPosKeys, posKeyType );
		unsigned int numRotKeys;
		NiRotKey::KeyType rotKeyType;
		NiRotKey **rotKeys = controller->GetRotAnim( numRotKeys, rotKeyType );

		if ( copyKeys ) {
			unsigned int i;
			if ( posKeys ) {
				NiPosKey::CopyFunction copyPos = NiPosKey::GetCopyFunction( posKeyType );
				numPos[index] = numPosKeys;
				posTypes[index] = posKeyType;
				pos[index] = new NiPosKey *[numPosKeys];
				for ( i = 0; i < numPosKeys; i++ ) {
					pos[index][i] = copyPos( posKeys[i] );
				}
				WidenTimes( posKeys[0]->GetTime(), loTime, hiTime );
				WidenTimes( posKeys[numPosKeys - 1]->GetTime(), loTime, hiTime );
			} else {
				numPos[index] = 1;
				posTypes[index] = NiPosKey::LINKEY;
				pos[index] = new NiPosKey *[1];
				pos[index][0] = new NiLinPosKey( 0.0f, object->GetTranslate() );
				WidenTimes( 0.0f, loTime, hiTime );
			}

			if ( rotKeys ) {
				NiRotKey::CopyFunction copyRot = NiRotKey::GetCopyFunction( rotKeyType );
				numRot[index] = numRotKeys;
				rotTypes[index] = rotKeyType;
				if ( rotKeyType == NiRotKey::EULERKEY ) {
					// the one Euler key holds the times, in its float keys
					rot[index] = new NiRotKey *[1];
					rot[index][0] = copyRot( rotKeys[0] );
					for ( unsigned int axis = 0; axis < 3; axis++ ) {
						int numAxisKeys = ( (NiEulerRotKey *)rotKeys[0] )->GetNumKeys( axis );
						if ( numAxisKeys > 0 ) {
							NiFloatKey **axisKeys = ( (NiEulerRotKey *)rotKeys[0] )->GetKeys( axis );
							WidenTimes( axisKeys[0]->GetTime(), loTime, hiTime );
							WidenTimes( axisKeys[numAxisKeys - 1]->GetTime(), loTime, hiTime );
						}
					}
				} else {
					rot[index] = new NiRotKey *[numRotKeys];
					for ( i = 0; i < numRotKeys; i++ ) {
						rot[index][i] = copyRot( rotKeys[i] );
					}
					WidenTimes( rotKeys[0]->GetTime(), loTime, hiTime );
					WidenTimes( rotKeys[numRotKeys - 1]->GetTime(), loTime, hiTime );
				}
			} else {
				numRot[index] = 1;
				rotTypes[index] = NiRotKey::LINKEY;
				NiQuaternion quat;
				object->GetRotate( quat );
				rot[index] = new NiRotKey *[1];
				rot[index][0] = new NiLinRotKey( 0.0f, quat );
				WidenTimes( 0.0f, loTime, hiTime );
			}
		} else {
			pos[index] = controller->GetPosAnim( numPosKeys, posKeyType );
			numPos[index] = numPosKeys;
			posTypes[index] = posKeyType;
			if ( pos[index] ) {
				WidenTimes( pos[index][0]->GetTime(), loTime, hiTime );
				WidenTimes( pos[index][numPosKeys - 1]->GetTime(), loTime, hiTime );
			} else {
				numPos[index] = 1;
				posTypes[index] = NiPosKey::LINKEY;
				pos[index] = new NiPosKey *[1];
				pos[index][0] = new NiLinPosKey( 0.0f, object->GetTranslate() );
				WidenTimes( 0.0f, loTime, hiTime );
			}

			rot[index] = controller->GetRotAnim( numRotKeys, rotKeyType );
			numRot[index] = numRotKeys;
			rotTypes[index] = rotKeyType;
			if ( rot[index] ) {
				WidenTimes( rot[index][0]->GetTime(), loTime, hiTime );
				WidenTimes( rot[index][numRotKeys - 1]->GetTime(), loTime, hiTime );
			} else {
				numRot[index] = 1;
				rotTypes[index] = NiRotKey::LINKEY;
				NiQuaternion quat;
				object->GetRotate( quat );
				rot[index] = new NiRotKey *[1];
				rot[index][0] = new NiLinRotKey( 0.0f, quat );
				WidenTimes( 0.0f, loTime, hiTime );
			}
		}
	} else {
		numPos[index] = 1;
		posTypes[index] = NiPosKey::LINKEY;
		pos[index] = new NiPosKey *[1];
		pos[index][0] = new NiLinPosKey( 0.0f, object->GetTranslate() );

		numRot[index] = 1;
		rotTypes[index] = NiRotKey::LINKEY;
		NiQuaternion quat;
		object->GetRotate( quat );
		rot[index] = new NiRotKey *[1];
		rot[index][0] = new NiLinRotKey( 0.0f, quat );

		WidenTimes( 0.0f, loTime, hiTime );
	}

	for ( unsigned int i = 0; i < object->GetArrayCount(); i++ ) {
		NiNode *child = NiDynamicCast( NiNode, object->GetAt( i ) );
		if ( child ) {
			NiNode *nodeChild = NiDynamicCast( NiNode, node->GetAt( i ) );
			assert( nodeChild );
			index++;
			GatherKeys( child, nodeChild, numPos, posTypes, pos, numRot, rotTypes, rot, index, loTime, hiTime, copyKeys );
		}
	}
}

/*
================
NiAnimBlender::AddSequence

Grows every per-sequence array by one, takes the tree's first text keys
that are not empty, and gathers the tree's keys. The length is the span of
the keys' times. Statements as the debug copy has them (AnimOnly.exe
0x480324, NiAnimBlender.cpp: assert( bSameTopology ), one size local for the
copies, the result read back from the member).
================
*/
// FUNCTION: STBC 0x7988E0
int NiAnimBlender::AddSequence( NiNode *sequence, CycleType cycleType, bool copyKeys ) {
	bool sameTopology = NiNode::IsSameShape( root, sequence );
	assert( sameTopology );
	if ( !sameTopology ) {
		return -1;
	}

	unsigned int count = numSequences + 1;

	unsigned int **newNumPosKeys = new unsigned int *[count];
	newNumPosKeys[numSequences] = new unsigned int[numNodes];
	NiPosKey::KeyType **newPosKeyTypes = new NiPosKey::KeyType *[count];
	newPosKeyTypes[numSequences] = new NiPosKey::KeyType[numNodes];
	NiPosKey ****newPosKeys = new NiPosKey ***[count];
	newPosKeys[numSequences] = new NiPosKey **[numNodes];
	unsigned int **newNumRotKeys = new unsigned int *[count];
	newNumRotKeys[numSequences] = new unsigned int[numNodes];
	NiRotKey::KeyType **newRotKeyTypes = new NiRotKey::KeyType *[count];
	newRotKeyTypes[numSequences] = new NiRotKey::KeyType[numNodes];
	NiRotKey ****newRotKeys = new NiRotKey ***[count];
	newRotKeys[numSequences] = new NiRotKey **[numNodes];
	float *newSequenceLengths = new float[count];
	unsigned int *newNumTextKeys = new unsigned int[count];
	NiTextKey **newTextKeys = new NiTextKey *[count];
	CycleType *newCycleTypes = new CycleType[count];

	if ( numSequences ) {
		unsigned int size;
		size = numSequences * sizeof( unsigned int * );
		memcpy( newNumPosKeys, numPosKeys, size );
		size = numSequences * sizeof( NiPosKey *** );
		memcpy( newPosKeys, posKeys, size );
		size = numSequences * sizeof( NiPosKey::KeyType * );
		memcpy( newPosKeyTypes, posKeyTypes, size );
		size = numSequences * sizeof( unsigned int * );
		memcpy( newNumRotKeys, numRotKeys, size );
		size = numSequences * sizeof( NiRotKey *** );
		memcpy( newRotKeys, rotKeys, size );
		size = numSequences * sizeof( NiRotKey::KeyType * );
		memcpy( newRotKeyTypes, rotKeyTypes, size );
		size = numSequences * sizeof( float );
		memcpy( newSequenceLengths, sequenceLengths, size );
		size = numSequences * sizeof( unsigned int );
		memcpy( newNumTextKeys, numTextKeys, size );
		size = numSequences * sizeof( NiTextKey * );
		memcpy( newTextKeys, textKeys, size );
		size = numSequences * sizeof( CycleType );
		memcpy( newCycleTypes, cycleTypes, size );
	}

	newNumTextKeys[numSequences] = 0;
	newTextKeys[numSequences] = 0;
	for ( NiExtraData *extra = sequence->GetExtraData(); extra; extra = extra->GetNext() ) {
		if ( NiIsKindOf( NiTextKeyExtraData, extra ) ) {
			NiTextKeyExtraData *textKeyData = (NiTextKeyExtraData *)extra;
			unsigned int numKeys;
			NiTextKey *keys = textKeyData->GetKeys( numKeys );
			if ( numKeys ) {
				newTextKeys[numSequences] = new NiTextKey[numKeys];
				newNumTextKeys[numSequences] = numKeys;
				for ( unsigned int i = 0; i < numKeys; i++ ) {
					newTextKeys[numSequences][i].SetTime( keys[i].GetTime() );
					newTextKeys[numSequences][i].SetText( keys[i].GetText() );
				}
				break;
			}
		}
	}

	float loTime = NI_INFINITY;
	float hiTime = -NI_INFINITY;
	unsigned int index = 0;
	GatherKeys( sequence, root, newNumPosKeys[numSequences], newPosKeyTypes[numSequences], newPosKeys[numSequences],
				newNumRotKeys[numSequences], newRotKeyTypes[numSequences], newRotKeys[numSequences], index, loTime, hiTime,
				copyKeys );
	newSequenceLengths[numSequences] = hiTime - loTime;
	newCycleTypes[numSequences] = cycleType;

	if ( numSequences ) {
		delete [] posKeys;
		delete [] numPosKeys;
		delete [] posKeyTypes;
		delete [] rotKeys;
		delete [] numRotKeys;
		delete [] rotKeyTypes;
		delete [] sequenceLengths;
		delete [] textKeys;
		delete [] numTextKeys;
		delete [] cycleTypes;
	}

	numPosKeys = newNumPosKeys;
	posKeyTypes = newPosKeyTypes;
	posKeys = newPosKeys;
	numRotKeys = newNumRotKeys;
	rotKeyTypes = newRotKeyTypes;
	rotKeys = newRotKeys;
	sequenceLengths = newSequenceLengths;
	numTextKeys = newNumTextKeys;
	textKeys = newTextKeys;
	cycleTypes = newCycleTypes;
	numSequences = count;
	return numSequences - 1;
}

/*
================
NiAnimBlender::ClearSequence
================
*/
// FUNCTION: STBC 0x798D70
void NiAnimBlender::ClearSequence() {
	assert( root );
	if ( !root ) {
		return;
	}
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		controllers[i]->ReplacePosAnim( 0, 0, NiPosKey::NOINTERP, true );
		controllers[i]->ReplaceRotAnim( 0, 0, NiRotKey::NOINTERP, true );
		controllers[i]->SetCycleType( CLAMP );
	}
}

/*
================
NiAnimBlender::ApplyActiveSequence
================
*/
// FUNCTION: STBC 0x798DD0
void NiAnimBlender::ApplyActiveSequence() {
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		NiKeyframeController *controller = controllers[i];
		if ( activeSequence >= 0 ) {
			controller->ReplacePosAnim( posKeys[activeSequence][i], numPosKeys[activeSequence][i], posKeyTypes[activeSequence][i], true );
			controller->ReplaceRotAnim( rotKeys[activeSequence][i], numRotKeys[activeSequence][i], rotKeyTypes[activeSequence][i], true );
			controller->SetCycleType( cycleTypes[activeSequence] );
		} else {
			controller->ReplacePosAnim( 0, 0, NiPosKey::NOINTERP, true );
			controller->ReplaceRotAnim( 0, 0, NiRotKey::NOINTERP, true );
			controller->SetCycleType( CLAMP );
		}
	}
}

/*
================
NiAnimBlender::SequencesConnect
================
*/
// FUNCTION: STBC 0x798EA0
bool NiAnimBlender::SequencesConnect( int from, int to ) {
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		NiPoint3 first = posKeys[to][i][0]->GetPos();
		NiPoint3 last = posKeys[from][i][numPosKeys[from][i] - 1]->GetPos();
		if ( fabsf( last.x - first.x ) >= 0.0001f ) {
			return false;
		}
		if ( fabsf( last.y - first.y ) >= 0.0001f ) {
			return false;
		}
		if ( fabsf( last.z - first.z ) >= 0.0001f ) {
			return false;
		}

		unsigned int lastRot = numRotKeys[from][i] - 1;
		NiQuaternion firstQuat = rotKeys[to][i][0]->GetQuaternion();
		NiQuaternion lastQuat = rotKeys[from][i][lastRot]->GetQuaternion();
		if ( fabsf( lastQuat.x - firstQuat.x ) >= 0.0001f ) {
			return false;
		}
		if ( fabsf( lastQuat.y - firstQuat.y ) >= 0.0001f ) {
			return false;
		}
		if ( fabsf( lastQuat.z - firstQuat.z ) >= 0.0001f ) {
			return false;
		}
		if ( fabsf( lastQuat.w - firstQuat.w ) >= 0.0001f ) {
			return false;
		}
	}
	return true;
}

/*
================
NiAnimBlender::AlignRotation
================
*/
// FUNCTION: STBC 0x799010
void NiAnimBlender::AlignRotation( NiRotKey *rot, NiRotKey *reference ) {
	if ( NiQuaternion::Dot( rot->GetQuaternion(), reference->GetQuaternion() ) < 0.0f ) {
		rot->SetQuaternion( -rot->GetQuaternion() );
	}
	NiQuaternion quat = rot->GetQuaternion();
	if ( quat.GetW() < -1.0f ) {
		quat.SetW( -1.0f );
		rot->SetQuaternion( quat );
	} else if ( quat.GetW() > 1.0f ) {
		quat.SetW( 1.0f );
		rot->SetQuaternion( quat );
	}
}

/*
================
NiAnimBlender::SamplePose

Times the sequence through this controller's own time settings.
================
*/
// FUNCTION: STBC 0x7990B0
void NiAnimBlender::SamplePose( float time, int sequence, NiPosKey *pos, NiRotKey *rot ) {
	assert( GetAnimType() == APP_TIME );
	assert( loKeyTime == 0.0f );
	assert( frequency == 1.0f );
	assert( phase == 0.0f );
	startTime = 0.0f;
	hiKeyTime = sequenceLengths[sequence];
	SetCycleType( cycleTypes[sequence] );
	float fScaledTime = ComputeScaledTime( time );	// (name invented; fits the /Od frame order)

	for ( int i = 0; i < (int)numNodes; i++ ) {	// a signed compare: retail's jl, AnimOnly.exe 0x481087's jge
		unsigned int lastIndex = 0;
		NiPoint3 position = NiPosKey::GenInterp( fScaledTime, posKeys[sequence][i], posKeyTypes[sequence][i], numPosKeys[sequence][i], lastIndex );
		pos[i].SetPos( position );
		pos[i].SetTime( 0.0f );
		lastIndex = 0;
		NiQuaternion rotation = NiRotKey::GenInterp( fScaledTime, rotKeys[sequence][i], rotKeyTypes[sequence][i], numRotKeys[sequence][i], lastIndex );	// (name invented; fits the /Od frame order)
		rot[i].SetQuaternion( rotation );
		rot[i].SetTime( 0.0f );
	}
}

/*
================
NiAnimBlender::SetBlendDestination
================
*/
// FUNCTION: STBC 0x7991E0
void NiAnimBlender::SetBlendDestination( const char *textKey ) {
	if ( textKey ) {
		unsigned int numKeys = numTextKeys[blendSequence];
		unsigned int i;
		for ( i = 0; i < numKeys; i++ ) {
			if ( !strcmp( textKeys[blendSequence][i].GetText(), textKey ) ) {
				break;
			}
		}
		float time = textKeys[blendSequence][i].GetTime();
		SamplePose( time, blendSequence, destinationPos, destinationRot );
		blendDestinationTime = time;
	} else {
		for ( unsigned int i = 0; i < numNodes; i++ ) {
			destinationPos[i].time = 1.0f;
			destinationPos[i].pos = posKeys[blendSequence][i][0]->GetPos();
			destinationRot[i].time = 1.0f;
			destinationRot[i].quat = rotKeys[blendSequence][i][0]->GetQuaternion();
		}
		blendDestinationTime = 0.0f;
	}
}

/*
================
NiAnimBlender::CapturePose
================
*/
// FUNCTION: STBC 0x799320
void NiAnimBlender::CapturePose() {
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		sourcePos[i].time = 0.0f;
		sourcePos[i].pos = nodes[i]->GetTranslate();
		NiQuaternion quat;
		nodes[i]->GetRotate( quat );
		sourceRot[i].time = 0.0f;
		sourceRot[i].quat = quat;
		AlignRotation( &sourceRot[i], &destinationRot[i] );
	}
}

/*
================
NiAnimBlender::BlendToSequence
================
*/
// FUNCTION: STBC 0x7993E0
void NiAnimBlender::BlendToSequence( int sequence, const char *textKey, float startTime, float duration ) {
	assert( 0 <= sequence && sequence < ( (int)numSequences ) );
	blendSequence = sequence;
	blendStartTime = startTime;
	blendDuration = duration;
	SetBlendDestination( textKey );
	blending = true;
}

/*
================
NiAnimBlender::UpdateBlend

Stops the sequence playing at the blend's start, then moves the tree from
its pose then to the destination's; at the end the destination sequence
plays on from the destination time.
================
*/
// FUNCTION: STBC 0x799410
void NiAnimBlender::UpdateBlend( float time ) {
	float t = time - blendStartTime;
	if ( t < -0.0001f ) {
		return;
	}
	if ( t < 0.0f ) {
		t = 0.0f;
	}
	t /= blendDuration;

	if ( activeSequence >= 0 ) {
		StopAnimations( root );
		SetActiveSequence( -1 );
		CapturePose();
	}

	if ( t >= 1.0f ) {
		SetActiveSequence( blendSequence );
		StartAnimations( root, blendDuration + blendStartTime - blendDestinationTime );
		blending = false;
		return;
	}

	NiPosKey::InterpFunction posInterp = NiPosKey::interpFunctions[NiPosKey::LINKEY];
	NiRotKey::InterpFunction rotInterp = NiRotKey::interpFunctions[NiRotKey::LINKEY];
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		NiPoint3 translate = posInterp( t, &sourcePos[i], &destinationPos[i] );
		nodes[i]->SetTranslate( translate );
		NiQuaternion quat = rotInterp( t, &sourceRot[i], &destinationRot[i] );
		nodes[i]->SetRotate( quat );
	}
}

/*
================
NiAnimBlender::Update
================
*/
// FUNCTION: STBC 0x7995B0
void NiAnimBlender::Update( float time ) {
	assert( ( !morphing && blending ) || ( morphing && !blending ) || ( !morphing && !blending ) );
	if ( blending ) {
		UpdateBlend( time );
	} else if ( morphing ) {
		UpdateMorph( time );
	}
}

/*
================
NiAnimBlender::FindMorphKeys
================
*/
// FUNCTION: STBC 0x7995E0
void NiAnimBlender::FindMorphKeys( float time, int &next, int &previous, NiTextKey *keys, unsigned int numKeys, CycleType cycleType ) {
	int i;
	int iNumKeys = numKeys;	// named by AnimOnly.exe 0x481896's asserts
	for ( i = 0; i < iNumKeys; i++ ) {
		if ( keys[i].GetTime() > time && !strncmp( keys[i].GetText(), MORPH_PREFIX, 6 ) ) {
			break;
		}
	}
	if ( i == iNumKeys && cycleType != CLAMP ) {
		for ( i = 0; i < iNumKeys; i++ ) {
			if ( !strncmp( keys[i].GetText(), MORPH_PREFIX, 6 ) ) {
				break;
			}
		}
	}
	assert( i < iNumKeys );
	next = i;

	for ( i--; i >= 0; i-- ) {
		if ( !strncmp( keys[i].GetText(), MORPH_PREFIX, 6 ) ) {
			break;
		}
	}
	if ( i == -1 && cycleType != CLAMP ) {
		for ( i = iNumKeys - 1; i >= 0; i-- ) {
			if ( !strncmp( keys[i].GetText(), MORPH_PREFIX, 6 ) ) {
				break;
			}
		}
	}
	assert( i >= 0 );
	previous = i;
}

/*
================
NiAnimBlender::FindTextKeys
================
*/
// FUNCTION: STBC 0x7996F0
void NiAnimBlender::FindTextKeys( const char *first, const char *second, int &secondIndex, int &firstIndex,
								  NiTextKey *keys, unsigned int numKeys, CycleType cycleType ) {
	secondIndex = -1;
	firstIndex = -1;

	unsigned int i;
	for ( i = 0; i < numKeys; i++ ) {
		if ( !strcmp( first, keys[i].GetText() ) ) {
			firstIndex = i;
			break;
		}
	}
	assert( firstIndex >= 0 && firstIndex < (int)numKeys );

	for ( i = firstIndex; i < numKeys; i++ ) {
		if ( !strcmp( second, keys[i].GetText() ) ) {
			secondIndex = i;
			break;
		}
	}
	if ( i == numKeys && cycleType != CLAMP ) {
		for ( i = 0; i < firstIndex; i++ ) {
			if ( !strcmp( second, keys[i].GetText() ) ) {
				secondIndex = i;
				break;
			}
		}
	}
	assert( secondIndex >= 0 && secondIndex < (int)numKeys );
}

/*
================
NiAnimBlender::GetTimeToTextKey

Written as the SDK's debug NetImmerse 3.1 compiles it (AnimOnly.exe
0x481B7E): the start time in a local, the key count read at each test, the
search stepping i in the loop's increment, and one return of a result; the
sequence argument only fed an assert there.
================
*/
// FUNCTION: STBC 0x799830
float NiAnimBlender::GetTimeToTextKey( int sequence, float time, const char *name ) {
	float start = controllers[0]->GetStartTime();
	float phase = fmodf( time - start, sequenceLengths[activeSequence] );

	unsigned int i;
	for ( i = 0; i < numTextKeys[activeSequence]; i++ ) {
		if ( textKeys[activeSequence][i].GetTime() > phase ) {
			break;
		}
	}

	i %= numTextKeys[activeSequence];
	for ( unsigned int tries = 0; tries < numTextKeys[activeSequence];
		tries++, i = ( i + 1 ) % numTextKeys[activeSequence] ) {
		if ( !strcmp( name, textKeys[activeSequence][i].GetText() ) ) {
			break;
		}
	}

	float result;
	if ( textKeys[activeSequence][i].GetTime() > phase ) {
		result = textKeys[activeSequence][i].GetTime() - phase;
	} else {
		result = sequenceLengths[activeSequence] - phase + textKeys[activeSequence][i].GetTime();
	}
	return result;
}

/*
================
NiAnimBlender::MorphToSequence

Finds the "morph:" keys around the active sequence's current time and the
same-named keys of the destination; the destination starts at the matching
point between them. Without a duration the destination plays at once.
================
*/
// FUNCTION: STBC 0x799930
// PARKED: schedule: the fmodf result's store to phase comes two loads after retail's (c2trace --sched: the fstp needs 7 more height to pass the cycle-type loads); statements as the debug copy, AnimOnly.exe 0x481DE4
void NiAnimBlender::MorphToSequence( int sequence, float time, float duration ) {
	if ( blending || morphing ) {
		return;
	}

	assert( 0 <= activeSequence && activeSequence < (int)numSequences );
	assert( 0 <= sequence && sequence < (int)numSequences );
	assert( numTextKeys[activeSequence] > 0 );
	assert( numTextKeys[sequence] > 0 );

	morphSource = activeSequence;
	morphDestination = sequence;

	float startTime = controllers[0]->GetStartTime();
	float phase = fmodf( time - startTime, sequenceLengths[morphSource] );
	int next, previous;
	FindMorphKeys( phase, next, previous, textKeys[morphSource], numTextKeys[morphSource], cycleTypes[morphSource] );

	float nextTime = textKeys[morphSource][next].GetTime();
	float previousTime = textKeys[morphSource][previous].GetTime();
	if ( nextTime < previousTime ) {
		nextTime += sequenceLengths[morphSource];
	}
	float span = nextTime - previousTime;	// (name invented)
	if ( phase < previousTime ) {
		phase += sequenceLengths[morphSource];
	}
	float fraction = ( phase - previousTime ) / span;
	assert( fraction >= 0.0f && fraction <= 1.0f );

	int destinationNext, destinationPrevious;
	FindTextKeys( textKeys[morphSource][previous].GetText(), textKeys[morphSource][next].GetText(), destinationNext, destinationPrevious,
				  textKeys[morphDestination], numTextKeys[morphDestination], cycleTypes[morphDestination] );

	float destinationNextTime = textKeys[morphDestination][destinationNext].GetTime();
	float destinationPreviousTime = textKeys[morphDestination][destinationPrevious].GetTime();
	if ( destinationNextTime < destinationPreviousTime ) {
		destinationNextTime += sequenceLengths[morphDestination];
	}
	float destinationSpan = destinationNextTime - destinationPreviousTime;	// (name invented)
	float destinationTime = fraction * destinationSpan + destinationPreviousTime;
	if ( destinationTime > sequenceLengths[morphDestination] ) {
		destinationTime -= sequenceLengths[morphDestination];
	}

	morphPhase = time - destinationTime;
	morphStartTime = time;
	morphDuration = duration;

	if ( duration == 0.0f ) {
		StopAnimations( GetRoot() );
		SetActiveSequence( morphDestination );
		StartAnimations( GetRoot(), morphPhase );
	} else {
		morphing = true;
	}
}

/*
================
NiAnimBlender::UpdateMorph

Moves the tree from the source sequence's pose to the destination's, both
running at their matching times; at the end the destination plays on.
================
*/
// FUNCTION: STBC 0x799B20
// PARKED: register: in the blend loop two of the key-array addresses take base and index in the other order; statements as the debug copy, AnimOnly.exe 0x4821E6 (/Od identical but for frame slots)
void NiAnimBlender::UpdateMorph( float time ) {
	assert( !blending && morphing );

	float t = time - morphStartTime;
	if ( t < -0.0001f ) {
		return;
	}
	if ( t < 0.0f ) {
		t = 0.0f;
	}
	t /= morphDuration;

	if ( activeSequence >= 0 ) {
		StopAnimations( GetRoot() );
		SetActiveSequence( -1 );
	}
	assert( activeSequence < 0 );

	float phase = fmodf( time - morphPhase, sequenceLengths[morphDestination] );
	if ( t >= 1.0f ) {
		SetActiveSequence( morphDestination );
		StartAnimations( GetRoot(), time - phase );
		morphing = false;
		return;
	}

	int previous, next;
	FindMorphKeys( phase, next, previous, textKeys[morphDestination], numTextKeys[morphDestination], cycleTypes[morphDestination] );

	float nextTime = textKeys[morphDestination][next].GetTime();
	float previousTime = textKeys[morphDestination][previous].GetTime();
	if ( nextTime < previousTime ) {
		nextTime += sequenceLengths[morphDestination];
	}
	float span = nextTime - previousTime;	// (name invented)
	if ( phase < previousTime ) {
		phase += sequenceLengths[morphDestination];
	}
	float fraction = ( phase - previousTime ) / span;
	assert( fraction >= 0.0f && fraction <= 1.0f );

	int sourcePrevious, sourceNext;
	FindTextKeys( textKeys[morphDestination][previous].GetText(), textKeys[morphDestination][next].GetText(), sourceNext, sourcePrevious,
				  textKeys[morphSource], numTextKeys[morphSource], cycleTypes[morphSource] );

	float sourceNextTime = textKeys[morphSource][sourceNext].GetTime();
	float sourcePreviousTime = textKeys[morphSource][sourcePrevious].GetTime();
	if ( sourceNextTime < sourcePreviousTime ) {
		sourceNextTime += sequenceLengths[morphSource];
	}
	float sourceSpan = sourceNextTime - sourcePreviousTime;	// (name invented)
	float sourceTime = fraction * sourceSpan + sourcePreviousTime;

	SamplePose( phase, morphDestination, destinationPos, destinationRot );
	SamplePose( sourceTime, morphSource, sourcePos, sourceRot );

	NiPosKey::InterpFunction posInterp = NiPosKey::GetInterpFunction( NiPosKey::LINKEY );
	assert( posInterp );
	NiRotKey::InterpFunction rotInterp = NiRotKey::GetInterpFunction( NiRotKey::LINKEY );
	assert( rotInterp );
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		AlignRotation( &sourceRot[i], &destinationRot[i] );
		NiPoint3 translate = posInterp( t, &sourcePos[i], &destinationPos[i] );
		nodes[i]->SetTranslate( translate );
		NiQuaternion quat = rotInterp( t, &sourceRot[i], &destinationRot[i] );
		nodes[i]->SetRotate( quat );
	}
}

/*
================
NiAnimBlender::ApplySequencePose

Runs each controller once with the sequence's keys at time, then strips
the keys again.
================
*/
// FUNCTION: STBC 0x799E50
void NiAnimBlender::ApplySequencePose( int sequence, float time ) {
	assert( activeSequence < 0 );
	assert( 0 <= sequence && sequence < ( (int)numSequences ) );
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		NiKeyframeController *controller = controllers[i];
		controller->ReplacePosAnim( posKeys[sequence][i], numPosKeys[sequence][i], posKeyTypes[sequence][i], true );
		controller->ReplaceRotAnim( rotKeys[sequence][i], numRotKeys[sequence][i], rotKeyTypes[sequence][i], true );
		controller->Start( 0.0f );
		controller->Update( time );
		controller->Stop();
		controller->ReplacePosAnim( 0, 0, NiPosKey::NOINTERP, true );
		controller->ReplaceRotAnim( 0, 0, NiRotKey::NOINTERP, true );
	}
}

/*
================
NiAnimBlender::CreateClone
================
*/
// FUNCTION: STBC 0x799F10
NiObject *NiAnimBlender::CreateClone() {
	NiAnimBlender *clone = new NiAnimBlender;
	CopyMembers( clone );
	return clone;
}

/*
================
NiAnimBlender::CopyMembers

The clone shares the nodes, controllers and keys until ProcessClone points
it at the cloned tree; the text keys are copied member-wise.
================
*/
// FUNCTION: STBC 0x799F80
void NiAnimBlender::CopyMembers( NiAnimBlender *dest ) {
	NiTimeController::CopyMembers( dest );

	dest->root = root;
	dest->numNodes = numNodes;
	dest->numSequences = numSequences;

	unsigned int i, j, k;

	dest->nodes = new NiNode *[numNodes];
	dest->controllers = new NiKeyframeController *[numNodes];
	for ( i = 0; i < numNodes; i++ ) {
		dest->nodes[i] = nodes[i];
		dest->controllers[i] = controllers[i];
	}

	dest->numPosKeys = new unsigned int *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->numPosKeys[i] = new unsigned int[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			dest->numPosKeys[i][j] = numPosKeys[i][j];
		}
	}
	dest->posKeyTypes = new NiPosKey::KeyType *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->posKeyTypes[i] = new NiPosKey::KeyType[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			dest->posKeyTypes[i][j] = posKeyTypes[i][j];
		}
	}
	dest->posKeys = new NiPosKey ***[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->posKeys[i] = new NiPosKey **[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			if ( dest->numPosKeys[i][j] > 0 ) {
				dest->posKeys[i][j] = new NiPosKey *[numPosKeys[i][j]];
				for ( k = 0; k < numPosKeys[i][j]; k++ ) {
					dest->posKeys[i][j][k] = posKeys[i][j][k];
				}
			}
		}
	}

	dest->numRotKeys = new unsigned int *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->numRotKeys[i] = new unsigned int[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			dest->numRotKeys[i][j] = numRotKeys[i][j];
		}
	}
	dest->rotKeyTypes = new NiRotKey::KeyType *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->rotKeyTypes[i] = new NiRotKey::KeyType[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			dest->rotKeyTypes[i][j] = rotKeyTypes[i][j];
		}
	}
	dest->rotKeys = new NiRotKey ***[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->rotKeys[i] = new NiRotKey **[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			if ( dest->numRotKeys[i][j] > 0 ) {
				dest->rotKeys[i][j] = new NiRotKey *[numRotKeys[i][j]];
				for ( k = 0; k < numRotKeys[i][j]; k++ ) {
					dest->rotKeys[i][j][k] = rotKeys[i][j][k];
				}
			}
		}
	}

	dest->sequenceLengths = new float[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->sequenceLengths[i] = sequenceLengths[i];
	}
	dest->numTextKeys = new unsigned int[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->numTextKeys[i] = numTextKeys[i];
	}
	dest->textKeys = new NiTextKey *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		if ( !numTextKeys[i] ) {
			// BUG: clears this blender's empty entry, leaving the clone's
			// unset
			textKeys[i] = 0;
		} else {
			dest->textKeys[i] = new NiTextKey[numTextKeys[i]];
			for ( j = 0; j < numTextKeys[i]; j++ ) {
				// BUG: the copy shares the text with this blender's key, and
				// both free it
				dest->textKeys[i][j] = textKeys[i][j];
			}
		}
	}
	dest->cycleTypes = new CycleType[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		dest->cycleTypes[i] = cycleTypes[i];
	}

	dest->sourcePos = new NiPosKey[dest->numNodes];
	dest->sourceRot = new NiRotKey[dest->numNodes];
	dest->destinationPos = new NiPosKey[dest->numNodes];
	dest->destinationRot = new NiRotKey[dest->numNodes];
}

/*
================
NiAnimBlender::ProcessClone

Points the clone at the cloned root and nodes, and at each cloned node's
keyframe controller.
================
*/
// FUNCTION: STBC 0x79A5B0
void NiAnimBlender::ProcessClone( NiObject *clone ) {
	assert( NiIsKindOf( NiAnimBlender, clone ) );
	NiTimeController::ProcessClone( clone );

	NiAnimBlender *pBlender = (NiAnimBlender *)clone;	// (name invented; fits the /Od frame order)
	NiExtraData *extraData = pBlender->root->GetExtraData();
	assert( extraData && NiIsExactKindOf( NiCloneExtraData, extraData ) );
	NiCloneExtraData *cloneData = (NiCloneExtraData *)extraData;
	pBlender->root = (NiNode *)cloneData->GetClone();
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		NiExtraData *extraData = pBlender->nodes[i]->GetExtraData();
		assert( extraData && NiIsExactKindOf( NiCloneExtraData, extraData ) );
		NiCloneExtraData *cloneData = (NiCloneExtraData *)extraData;
		pBlender->nodes[i] = (NiNode *)cloneData->GetClone();
		for ( NiTimeController *controller = pBlender->nodes[i]->GetControllers(); controller;
			  controller = controller->GetNext() ) {
			if ( NiIsExactKindOf( NiKeyframeController, controller ) ) {
				break;
			}
		}
		assert( controller && NiIsExactKindOf( NiKeyframeController, controller ) );
		pBlender->controllers[i] = (NiKeyframeController *)controller;
	}
}

/*
================
NiAnimBlender::CreateObject
================
*/
// FUNCTION: STBC 0x79A630
void NiAnimBlender::CreateObject( NiStream &stream, NiObject *&object ) {
	NiAnimBlender *blender = new NiAnimBlender;
	stream.SetLoadRecord( new NiAnimBlenderLoadRecord );
	blender->LoadBinary( stream );
	object = blender;
}

/*
================
NiAnimBlender::LoadBinary

The root's and the nodes' and controllers' link IDs, then each sequence:
the per-node key counts, the per-node keys (each group's type first when it
has keys), the length, the text keys and the cycle type.
================
*/
// FUNCTION: STBC 0x79A6D0
void NiAnimBlender::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );

	NiAnimBlenderLoadRecord *record = (NiAnimBlenderLoadRecord *)stream.GetLoadRecord();
	NiStreamLoadBinary( stream, record->rootLinkID );
	NiStreamLoadBinary( stream, numNodes );
	record->nodeLinkIDs = new unsigned int[numNodes];
	record->controllerLinkIDs = new unsigned int[numNodes];

	unsigned int i, j, k;

	for ( i = 0; i < numNodes; i++ ) {
		NiStreamLoadBinary( stream, record->nodeLinkIDs[i] );
	}
	for ( i = 0; i < numNodes; i++ ) {
		NiStreamLoadBinary( stream, record->controllerLinkIDs[i] );
	}

	NiStreamLoadBinary( stream, numSequences );

	numPosKeys = new unsigned int *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		numPosKeys[i] = new unsigned int[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			NiStreamLoadBinary( stream, numPosKeys[i][j] );
		}
	}
	posKeyTypes = new NiPosKey::KeyType *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		posKeyTypes[i] = new NiPosKey::KeyType[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			posKeyTypes[i][j] = NiPosKey::NOINTERP;
		}
	}
	posKeys = new NiPosKey ***[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		posKeys[i] = new NiPosKey **[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			if ( numPosKeys[i][j] > 0 ) {
				NiPosKey::KeyType keyType;
				NiStreamLoadEnum( stream, keyType );
				posKeyTypes[i][j] = keyType;
				NiPosKey::CreateFunction create = NiPosKey::GetCreateFunction( keyType );
				posKeys[i][j] = new NiPosKey *[numPosKeys[i][j]];
				for ( k = 0; k < numPosKeys[i][j]; k++ ) {
					posKeys[i][j][k] = create( stream );
				}
				NiPosKey::FillDerivedValuesFunction fill = NiPosKey::GetFillDerivedFunction( keyType );
				fill( posKeys[i][j], numPosKeys[i][j] );
			}
		}
	}

	numRotKeys = new unsigned int *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		numRotKeys[i] = new unsigned int[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			NiStreamLoadBinary( stream, numRotKeys[i][j] );
		}
	}
	rotKeyTypes = new NiRotKey::KeyType *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		rotKeyTypes[i] = new NiRotKey::KeyType[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			rotKeyTypes[i][j] = NiRotKey::NOINTERP;
		}
	}
	rotKeys = new NiRotKey ***[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		rotKeys[i] = new NiRotKey **[numNodes];
		for ( j = 0; j < numNodes; j++ ) {
			if ( numRotKeys[i][j] > 0 ) {
				NiRotKey::KeyType keyType;
				NiStreamLoadEnum( stream, keyType );
				rotKeyTypes[i][j] = keyType;
				NiRotKey::CreateFunction create = NiRotKey::GetCreateFunction( keyType );
				rotKeys[i][j] = new NiRotKey *[numRotKeys[i][j]];
				for ( k = 0; k < numRotKeys[i][j]; k++ ) {
					rotKeys[i][j][k] = create( stream );
				}
				NiRotKey::FillDerivedValuesFunction fill = NiRotKey::GetFillDerivedFunction( keyType );
				fill( rotKeys[i][j], numRotKeys[i][j] );
			}
		}
	}

	sequenceLengths = new float[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		NiStreamLoadBinary( stream, sequenceLengths[i] );
	}
	numTextKeys = new unsigned int[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		NiStreamLoadBinary( stream, numTextKeys[i] );
	}
	textKeys = new NiTextKey *[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		unsigned int numKeys = numTextKeys[i];
		if ( !numKeys ) {
			textKeys[i] = 0;
		} else {
			textKeys[i] = new NiTextKey[numKeys];
			for ( j = 0; j < numTextKeys[i]; j++ ) {
				textKeys[i][j].LoadBinary( stream );
			}
		}
	}
	cycleTypes = new CycleType[numSequences];
	for ( i = 0; i < numSequences; i++ ) {
		NiStreamLoadEnum( stream, cycleTypes[i] );
	}

	sourcePos = new NiPosKey[numNodes];
	sourceRot = new NiRotKey[numNodes];
	destinationPos = new NiPosKey[numNodes];
	destinationRot = new NiRotKey[numNodes];
}

/*
================
NiAnimBlender::LinkObject
================
*/
// FUNCTION: STBC 0x79AE30
void NiAnimBlender::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );

	NiAnimBlenderLoadRecord *record = (NiAnimBlenderLoadRecord *)stream.GetLoadRecord();
	assert( record );
	assert( record->rootLinkID );
	assert( record->nodeLinkIDs );
	assert( record->controllerLinkIDs );
	root = (NiNode *)stream.GetObjectFromLinkID( record->rootLinkID );

	unsigned int i;

	nodes = new NiNode *[numNodes];
	for ( i = 0; i < numNodes; i++ ) {
		nodes[i] = (NiNode *)stream.GetObjectFromLinkID( record->nodeLinkIDs[i] );
	}
	delete [] record->nodeLinkIDs;
	record->nodeLinkIDs = 0;

	controllers = new NiKeyframeController *[numNodes];
	for ( i = 0; i < numNodes; i++ ) {
		controllers[i] = (NiKeyframeController *)stream.GetObjectFromLinkID( record->controllerLinkIDs[i] );
	}
	delete [] record->controllerLinkIDs;
	record->controllerLinkIDs = 0;
}

/*
================
NiAnimBlender::RegisterStreamables
================
*/
// FUNCTION: STBC 0x79AEF0
bool NiAnimBlender::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	for ( unsigned int i = 0; i < numNodes; i++ ) {
		nodes[i]->RegisterStreamables( stream );
		controllers[i]->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiAnimBlender::SaveBinary
================
*/
// FUNCTION: STBC 0x79AF40
void NiAnimBlender::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );

	NiStreamSaveBinary( stream, root );
	NiStreamSaveBinary( stream, numNodes );

	unsigned int i, j, k;

	for ( i = 0; i < numNodes; i++ ) {
		NiStreamSaveBinary( stream, nodes[i] );
	}
	for ( i = 0; i < numNodes; i++ ) {
		NiStreamSaveBinary( stream, controllers[i] );
	}

	NiStreamSaveBinary( stream, numSequences );

	for ( i = 0; i < numSequences; i++ ) {
		for ( j = 0; j < numNodes; j++ ) {
			NiStreamSaveBinary( stream, numPosKeys[i][j] );
		}
	}
	for ( i = 0; i < numSequences; i++ ) {
		for ( j = 0; j < numNodes; j++ ) {
			if ( numPosKeys[i][j] > 0 ) {
				NiStreamSaveEnum( stream, posKeyTypes[i][j] );
				NiPosKey::SaveFunction save = NiPosKey::GetSaveFunction( posKeyTypes[i][j] );
				for ( k = 0; k < numPosKeys[i][j]; k++ ) {
					save( stream, posKeys[i][j][k] );
				}
			}
		}
	}

	for ( i = 0; i < numSequences; i++ ) {
		for ( j = 0; j < numNodes; j++ ) {
			NiStreamSaveBinary( stream, numRotKeys[i][j] );
		}
	}
	for ( i = 0; i < numSequences; i++ ) {
		for ( j = 0; j < numNodes; j++ ) {
			if ( numRotKeys[i][j] > 0 ) {
				NiStreamSaveEnum( stream, rotKeyTypes[i][j] );
				NiRotKey::SaveFunction save = NiRotKey::GetSaveFunction( rotKeyTypes[i][j] );
				for ( k = 0; k < numRotKeys[i][j]; k++ ) {
					save( stream, rotKeys[i][j][k] );
				}
			}
		}
	}

	for ( i = 0; i < numSequences; i++ ) {
		NiStreamSaveBinary( stream, sequenceLengths[i] );
	}
	for ( i = 0; i < numSequences; i++ ) {
		NiStreamSaveBinary( stream, numTextKeys[i] );
	}
	for ( i = 0; i < numSequences; i++ ) {
		for ( j = 0; j < numTextKeys[i]; j++ ) {
			textKeys[i][j].SaveBinary( stream );
		}
	}
	for ( i = 0; i < numSequences; i++ ) {
		NiStreamSaveEnum( stream, cycleTypes[i] );
	}
}

/*
================
NiAnimBlender::IsEqual

The text keys of each sequence are compared once per node.
================
*/
// FUNCTION: STBC 0x79B300
bool NiAnimBlender::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiAnimBlender *blender = (NiAnimBlender *)object;
	if ( root != blender->root ) {
		return false;
	}
	if ( numNodes != blender->numNodes ) {
		return false;
	}
	if ( numSequences != blender->numSequences ) {
		return false;
	}

	unsigned int i, j, k;

	for ( i = 0; i < numNodes; i++ ) {
		if ( ( nodes[i] && !blender->nodes[i] ) || ( !nodes[i] && blender->nodes[i] ) ) {
			return false;
		}
		if ( nodes[i] && !nodes[i]->IsEqual( blender->nodes[i] ) ) {
			return false;
		}
		if ( ( controllers[i] && !blender->controllers[i] ) || ( !controllers[i] && blender->controllers[i] ) ) {
			return false;
		}
		if ( controllers[i] && !controllers[i]->IsEqual( blender->controllers[i] ) ) {
			return false;
		}
	}

	for ( i = 0; i < numSequences; i++ ) {
		for ( j = 0; j < numNodes; j++ ) {
			if ( numPosKeys[i][j] != blender->numPosKeys[i][j] ) {
				return false;
			}
			if ( posKeyTypes[i][j] != blender->posKeyTypes[i][j] ) {
				return false;
			}
			if ( numRotKeys[i][j] != blender->numRotKeys[i][j] ) {
				return false;
			}
			if ( rotKeyTypes[i][j] != blender->rotKeyTypes[i][j] ) {
				return false;
			}

			NiPosKey::EqualFunction posEqual = NiPosKey::GetEqualFunction( posKeyTypes[i][j] );
			for ( k = 0; k < numPosKeys[i][j]; k++ ) {
				if ( !posEqual( posKeys[i][j][k], blender->posKeys[i][j][k] ) ) {
					return false;
				}
			}
			NiRotKey::EqualFunction rotEqual = NiRotKey::GetEqualFunction( rotKeyTypes[i][j] );
			for ( k = 0; k < numRotKeys[i][j]; k++ ) {
				if ( !rotEqual( rotKeys[i][j][k], blender->rotKeys[i][j][k] ) ) {
					return false;
				}
			}

			for ( k = 0; k < numTextKeys[i]; k++ ) {
				if ( textKeys[i][k].time != blender->textKeys[i][k].time ) {
					return false;
				}
				if ( strcmp( textKeys[i][k].text, blender->textKeys[i][k].text ) ) {
					return false;
				}
			}
		}

		if ( sequenceLengths[i] != blender->sequenceLengths[i] ) {
			return false;
		}
		if ( numTextKeys[i] != blender->numTextKeys[i] ) {
			return false;
		}
		if ( cycleTypes[i] != blender->cycleTypes[i] ) {
			return false;
		}
	}
	return true;
}
