// NiMorphController.cpp -- morphing between vertex sets

#include "NiRTLib.h"
#include <string.h>

#include "NiMorphController.h"
#include "NiBezierMesh.h"
#include "NiBound.h"
#include "NiMorphKey.h"
#include "NiKeyType6FloatKey.h"
#include "NiMorpherController.h"
#include "NiTCBFloatKey.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a97e0 _$E30
// FUNCTION: STBC 0x007a97f0 _$E29
// FUNCTION: STBC 0x007a9800 _$E41
// FUNCTION: STBC 0x007a9810 _$E40
// FUNCTION: STBC 0x007a9840 _$E45
// FUNCTION: STBC 0x007a9850 _$E44
// FUNCTION: STBC 0x007a9860 _$E49
// FUNCTION: STBC 0x007a9870 _$E48
// FUNCTION: STBC 0x007a9880 _$E53
// FUNCTION: STBC 0x007a9890 _$E52
// FUNCTION: STBC 0x007a98a0 _$E177
// FUNCTION: STBC 0x007a98b0 _$E176
// FUNCTION: STBC 0x007a98c0 _$E186
// FUNCTION: STBC 0x007a98d0 _$E185
// FUNCTION: STBC 0x007a98e0 _$E245
// FUNCTION: STBC 0x007a98f0 _$E244
// FUNCTION: STBC 0x007a9900 _$E248
// FUNCTION: STBC 0x007a9910 _$E247
// FUNCTION: STBC 0x007a9950 _$E213
// FUNCTION: STBC 0x007a9960 _$E212
// FUNCTION: STBC 0x007a99a0 _$E194
// FUNCTION: STBC 0x007a99b0 _$E193
// FUNCTION: STBC 0x007a99c0 _$E201
// FUNCTION: STBC 0x007a99d0 _$E200
// FUNCTION: STBC 0x007a99e0 _$E205
// FUNCTION: STBC 0x007a99f0 _$E204
// FUNCTION: STBC 0x007a9a00 _$E209
// FUNCTION: STBC 0x007a9a10 _$E208
// FUNCTION: STBC 0x007a9a20 _$E226
// FUNCTION: STBC 0x007a9a30 _$E225
// FUNCTION: STBC 0x007a9a60 _$E258
// FUNCTION: STBC 0x007a9a70 _$E257
// FUNCTION: STBC 0x007a9a80 _$E262
// FUNCTION: STBC 0x007a9a90 _$E261
// FUNCTION: STBC 0x007a9c40 _$E251
// FUNCTION: STBC 0x007a9c50 _$E250
// FUNCTION: STBC 0x007a9c60 _$E254
// FUNCTION: STBC 0x007a9c70 _$E253
// FUNCTION: STBC 0x007a9ca0 _$E218
// FUNCTION: STBC 0x007a9cb0 _$E217
// FUNCTION: STBC 0x007a9cc0 _$E222
// FUNCTION: STBC 0x007a9cd0 _$E221
// FUNCTION: STBC 0x007a9ce0 _$E230
// FUNCTION: STBC 0x007a9cf0 _$E229
// FUNCTION: STBC 0x007a9d00 _$E238
// FUNCTION: STBC 0x007a9d10 _$E237
// FUNCTION: STBC 0x007a9d20 _$E242
// FUNCTION: STBC 0x007a9d30 _$E241

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007a97b0 _$E279
// FUNCTION: STBC 0x007a97d0 _$E278
// FUNCTION: STBC 0x007a9730 _$E17
// FUNCTION: STBC 0x007a9740 _$E14
// FUNCTION: STBC 0x007a9750 _$E16
// FUNCTION: STBC 0x007a9760 _$E15
// FUNCTION: STBC 0x007a9770 _$E23
// FUNCTION: STBC 0x007a9780 _$E20
// FUNCTION: STBC 0x007a9790 _$E22
// FUNCTION: STBC 0x007a97a0 _$E21

// FUNCTION: STBC 0x007a82f0 _$E268
// FUNCTION: STBC 0x007a8300 _$E264
// FUNCTION: STBC 0x007a8320 _$E267
// FUNCTION: STBC 0x007a8330 _$E265
// DATA: STBC 0x9A0558 ?rtti@NiMorphController@@2VNiRTTI@@B
const NiRTTI NiMorphController::rtti( "NiMorphController", &NiTimeController::rtti );

/*
================
NiMorphController::Register
================
*/
// FUNCTION: STBC 0x7A8350 ?Register@NiMorphController@@SAHXZ
NiImplementRegister( NiMorphController )

/*
================
NiMorphController::FindController

The first NiMorphController of the target's controllers.
================
*/
// FUNCTION: STBC 0x7A84A0
NiMorphController *NiMorphController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiMorphController *morphController = NiDynamicCast( NiMorphController, controller );
		if ( morphController ) {
			return morphController;
		}
	}
	return 0;
}

/*
================
NiMorphController::NiMorphController

No vertex sets and no keys yet.
================
*/
// FUNCTION: STBC 0x7A84F0
NiMorphController::NiMorphController() {
	SetUnknown10( true );
	numTargets = 0;
	numVertices = 0;
	numKeys = 0;
	unknown40 = 0;
	bound = 0;
	keys = 0;
	targets = 0;
	keyType = NiFloatKey::NOINTERP;
	triBasedTarget = false;
}

/*
================
NiMorphController::~NiMorphController

Morph keys own arrays of their own, which their destructor frees.
================
*/
// FUNCTION: STBC 0x7A8530
NiMorphController::~NiMorphController() {
	unsigned int i;
	if ( targets ) {
		for ( i = 0; i < numTargets; i++ ) {
			delete [] targets[i];
		}
		delete [] targets;
	}

	if ( keys ) {
		if ( keyType == NiFloatKey::MORPHKEY ) {
			for ( i = 0; i < numKeys; i++ ) {
				delete (NiMorphKey *)keys[i];
			}
		} else {
			for ( i = 0; i < numKeys; i++ ) {
				delete keys[i];
			}
		}
		delete [] keys;
	}

	delete bound;
}

/*
================
NiMorphController::SetTarget

Notes whether the new target is an NiTriBasedGeom, then rebuilds the
bound.
================
*/
// FUNCTION: STBC 0x7A8660
void NiMorphController::SetTarget( NiObjectNET *target_ ) {
	if ( NiIsKindOf( NiTriBasedGeom, target_ ) ) {
		triBasedTarget = true;
	} else {
		assert( NiIsKindOf( NiBezierMesh, target_ ) );
		triBasedTarget = false;
	}
	NiTimeController::SetTarget( target_ );
	VirtualSlot20();
}

/*
================
NiMorphController::SetKeys

The key times bound the controller's time.
================
*/
// FUNCTION: STBC 0x7A86C0
void NiMorphController::SetKeys( NiFloatKey **keys_, unsigned int numKeys_, NiFloatKey::KeyType keyType_ ) {
	if ( !keys_ || !numKeys_ ) {
		numKeys = 0;
		keys = 0;
		keyType = NiFloatKey::NOINTERP;
		return;
	}

	numKeys = numKeys_;
	keys = keys_;
	keyType = keyType_;
	loKeyTime = keys[0]->GetTime();
	hiKeyTime = keys[numKeys - 1]->GetTime();
}

/*
================
NiMorphController::SetTargets
================
*/
// FUNCTION: STBC 0x7A8710
void NiMorphController::SetTargets( NiPoint3 **targets_, unsigned int numTargets_, unsigned int numVertices_ ) {
	if ( !targets_ || !numTargets_ ) {
		numTargets = 0;
		numVertices = 0;
		targets = 0;
		return;
	}

	numTargets = numTargets_;
	numVertices = numVertices_;
	targets = targets_;
	Unknown7A8AE0();
}

/*
================
NiMorphController::Update

Morphs the target at the scaled time. An NiTriBasedGeom's model data is
marked changed; an NiBezierMesh, the other kind of target, rebuilds its
edge points, its patches and its bound.
================
*/
// FUNCTION: STBC 0x7A8750
void NiMorphController::Update( float time ) {
	if ( !keys || DontDoUpdate( time ) ) {
		return;
	}

	// morph keys weight offsets from the first vertex set
	if ( GetUnknown10() && keyType == NiFloatKey::MORPHKEY ) {
		SetUnknown10( false );
		for ( unsigned int i = 1; i < numTargets; i++ ) {
			for ( unsigned int j = 0; j < numVertices; j++ ) {
				targets[i][j] -= targets[0][j];
			}
		}
	}

	if ( !bound ) {
		Unknown7A8AE0();
	}

	float scaledTime = ComputeScaledTime( time );
	Unknown7A8880( scaledTime );

	if ( triBasedTarget ) {
		NiTriBasedGeom *geom = (NiTriBasedGeom *)target;
		if ( GetUpdateNormals() && geom->GetNormals() ) {
			geom->CalculateNormals();
		}
		geom->GetModelDataPointer()->IncRevisionID();
	} else {
		NiBezierMesh *mesh = (NiBezierMesh *)target;
		mesh->Unknown830B80();
		mesh->Unknown82FA40();
		mesh->UpdateWorldBound();
	}
}

/*
================
NiMorphController::Unknown7A8880

Morphs the target's vertices to the time (statements as the debug copy,
AnimOnly.exe 0x4960F9): finds the keys around it
(starting from the pair of the previous call), takes the target weights
between them, and sums the weighted targets. Morph keys keep target 0 as
the base; the other keys start from zero.
================
*/
// FUNCTION: STBC 0x7A8880
void NiMorphController::Unknown7A8880( float time ) {
	if ( numKeys < 2 ) {
		return;
	}

	unsigned int next;
	float nextTime;
	float lastTime;
	if ( keys[0]->GetTime() >= time ) {
		unknown40 = 0;
		next = 1;
		lastTime = keys[0]->GetTime();
		nextTime = keys[1]->GetTime();
	} else if ( keys[numKeys - 1]->GetTime() <= time ) {
		unknown40 = numKeys - 2;
		next = numKeys - 1;
		lastTime = keys[unknown40]->GetTime();
		nextTime = keys[next]->GetTime();
	} else {
		if ( keys[unknown40]->GetTime() > time ) {
			unknown40 = 0;
			next = 1;
		} else {
			next = unknown40 + 1;
		}
		lastTime = keys[unknown40]->GetTime();
		nextTime = keys[next]->GetTime();
		for ( ; next <= numKeys - 1; next++ ) {
			nextTime = keys[next]->GetTime();
			if ( time < nextTime ) {
				break;
			}
			unknown40++;
			lastTime = nextTime;
		}
		assert( nextTime != lastTime );
	}

	float t = ( time - lastTime ) / ( nextTime - lastTime );
	float *weights;
	if ( keyType == NiFloatKey::MORPHKEY ) {
		weights = NiMorphKey::InterpolateValues( t, (NiMorphKey *)keys[unknown40], (NiMorphKey *)keys[next], numTargets );
	} else {
		// KEYTYPE6 is CUBICKEY in the original
		assert( keyType == NiFloatKey::KEYTYPE6 );
		weights = NiKeyType6FloatKey::InterpolateValues( t, keys[unknown40], keys[next], numTargets );
	}

	unsigned short vertexCount;
	NiPoint3 *vertices;
	if ( triBasedTarget ) {
		NiTriBasedGeom *geom = (NiTriBasedGeom *)target;
		vertexCount = geom->GetVertexCount();
		vertices = geom->GetVertices();
	} else {
		NiBezierMesh *mesh = (NiBezierMesh *)target;
		vertexCount = mesh->GetVertexCount();
		vertices = mesh->GetVertices();
	}

	unsigned int first;	// the first target summed (name invented)
	unsigned short j;
	if ( keyType == NiFloatKey::MORPHKEY ) {
		for ( j = 0; j < vertexCount; j++ ) {
			vertices[j] = targets[0][j];
		}
		first = 1;
	} else {
		for ( j = 0; j < vertexCount; j++ ) {
			vertices[j] = NiPoint3::ZERO;
		}
		first = 0;
	}
	for ( unsigned int i = first; i < numTargets; i++ ) {
		for ( j = 0; j < vertexCount; j++ ) {
			vertices[j] += weights[i] * targets[i][j];
		}
	}

	delete [] weights;
}

/*
================
NiMorphController::Unknown7A8AE0

Rebuilds the bound: the target morphed at each key's time, the bounds of
its vertices merged. An NiTriBasedGeom takes the result as its model
bound.
================
*/
// FUNCTION: STBC 0x7A8AE0
void NiMorphController::Unknown7A8AE0() {
	if ( !keys || !target ) {
		return;
	}

	if ( !bound ) {
		bound = new NiBound;
	}
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		Unknown7A8880( keys[i]->GetTime() );

		unsigned short vertexCount;
		NiPoint3 *vertices;
		if ( triBasedTarget ) {
			NiTriBasedGeom *geom = (NiTriBasedGeom *)target;
			vertexCount = geom->GetVertexCount();
			vertices = geom->GetVertices();
		} else {
			NiBezierMesh *mesh = (NiBezierMesh *)target;
			vertexCount = mesh->GetVertexCount();
			vertices = mesh->GetVertices();
		}

		NiBound keyBound;
		keyBound.ComputeFromData( vertexCount, vertices );
		if ( i == 0 ) {
			*bound = keyBound;
		} else {
			bound->Merge( &keyBound );
		}
	}

	if ( triBasedTarget ) {
		( (NiTriBasedGeom *)target )->SetModelBound( *bound );
	}
}

/*
================
NiMorphController::CreateClone
================
*/
// FUNCTION: STBC 0x7A8C20
NiObject *NiMorphController::CreateClone() {
	NiMorphController *clone = new NiMorphController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiMorphController::CopyMembers

The clone gets copies of the keys, the vertex sets and the bound.
================
*/
// FUNCTION: STBC 0x7A8C80
void NiMorphController::CopyMembers( NiMorphController *dest ) {
	NiTimeController::CopyMembers( dest );

	dest->numTargets = numTargets;
	dest->numVertices = numVertices;
	dest->numKeys = numKeys;
	dest->keyType = keyType;

	dest->keys = new NiFloatKey *[numKeys];
	assert( dest->keys );
	NiFloatKey::CopyFunction copy = NiFloatKey::GetCopyFunction( keyType );
	assert( copy );
	unsigned int i;
	for ( i = 0; i < numKeys; i++ ) {
		dest->keys[i] = copy( keys[i], numTargets );
	}

	dest->targets = new NiPoint3 *[numTargets];
	assert( dest->targets );
	for ( i = 0; i < numTargets; i++ ) {
		dest->targets[i] = new NiPoint3[numVertices];
		assert( dest->targets[i] );
		memcpy( dest->targets[i], targets[i], numVertices * sizeof( NiPoint3 ) );
	}

	dest->bound = new NiBound;
	assert( dest->bound );
	*dest->bound = *bound;
}

/*
================
NiMorphController::CreateObject

A file of the old format holds the controller itself: it is loaded, then
converted at once to an NiMorpherController over an NiMorphData. Its keys
become TCB keys holding the weight of each target: a morph key's own value
for the target, or, for the keys that name a target, one for that target
and zero for the others. The new controller takes the old one's flags,
timing and, once the old one is released, nothing else.
================
*/
// FUNCTION: STBC 0x7A8DD0
// PARKED: declaration count: the frame slots flip with the declarations the unit sees (ctxscan: exact only at 13, 26-33 and 39-60 more; listed in config/declaration_sensitive.txt)
void NiMorphController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiMorphController *controller = new NiMorphController;
	stream.SetLoadRecord( new NiMorphControllerLoadRecord );
	controller->LoadBinary( stream );

	unsigned int numTargets_;
	unsigned int numVertices_;
	NiPoint3 **oldTargets = controller->GetTargetVerts( numTargets_, numVertices_ );
	unsigned int numKeys_;
	NiFloatKey::KeyType oldKeyType;
	NiFloatKey **oldKeys = controller->GetMorphKeys( numKeys_, oldKeyType, numTargets_ );
	// MORPHKEY and KEYTYPE6 are BARYKEY and CUBICKEY in the original
	assert( oldKeyType == NiFloatKey::MORPHKEY || oldKeyType == NiFloatKey::KEYTYPE6 );

	bool relative = oldKeyType == NiFloatKey::MORPHKEY;

	NiMorphData *data = new NiMorphData;
	NiMorphData::MorphTarget *morphTargets = new NiMorphData::MorphTarget[numTargets_];
	for ( unsigned int i = 0; i < numTargets_; i++ ) {
		NiPoint3 *verts = new NiPoint3[numVertices_];
		for ( unsigned int j = 0; j < numVertices_; j++ ) {
			verts[j] = oldTargets[i][j];
		}
		morphTargets[i].ReplaceTargetVerts( verts );

		NiFloatKey **newKeys = new NiFloatKey *[numKeys_];
		if ( relative ) {
			for ( j = 0; j < numKeys_; j++ ) {
				NiMorphKey *key = (NiMorphKey *)oldKeys[j];
				newKeys[j] = new NiTCBFloatKey( key->GetTime(), key->GetValues()[i], key->GetTension(), key->GetContinuity(), key->GetBias() );
			}
		} else {
			for ( j = 0; j < numKeys_; j++ ) {
				NiTCBFloatKey *key = (NiTCBFloatKey *)oldKeys[j];
				float weight = 0.0f;
				if ( (unsigned int)( oldKeys[j]->GetValue() + 0.5f ) == i ) {
					weight = 1.0f;
				}
				newKeys[j] = new NiTCBFloatKey( key->GetTime(), weight, key->GetTension(), key->GetContinuity(), key->GetBias() );
			}
		}
		NiFloatKey::FillDerivedValuesFunction fill = NiFloatKey::GetFillDerivedFunction( NiFloatKey::TCBKEY );
		fill( newKeys, numKeys_, 0 );
		morphTargets[i].ReplaceAnim( newKeys, numKeys_, NiFloatKey::TCBKEY );
	}
	data->SetRelativeTargets( relative );
	data->ReplaceTargets( morphTargets, numTargets_, numVertices_ );

	NiMorpherController *morpher = new NiMorpherController( data );
	morpher->SetUpdateNormals( controller->GetUpdateNormals() );
	morpher->SetAnimType( controller->GetAnimType() );
	morpher->SetCycleType( controller->GetCycleType() );
	morpher->SetFrequency( controller->GetFrequency() );
	morpher->SetPhase( controller->GetPhase() );
	morpher->SetBeginKeyTime( controller->GetBeginKeyTime() );
	morpher->SetEndKeyTime( controller->GetEndKeyTime() );
	object = morpher;
	NiLoadRecord *record = stream.GetLoadRecord();
	stream.SetObjectFromLinkID( record->linkID, object );
	delete controller;
}

/*
================
NiMorphController::LoadBinary

The counts, the bound, the keys (each read by the function of their
type, with one value per target) and the vertex sets. Only files older
than 3.05 hold this controller. The locals keep their original Hungarian
names, which the debug copy's asserts give (ppKeys, derived, pVerts) and
whose hashes give its /Od frame order (AnimOnly.exe 0x497039).
================
*/
// FUNCTION: STBC 0x7A9280
void NiMorphController::LoadBinary( NiStream &stream ) {
	assert( strcmp( stream.GetFileVersion(), "3.05" ) < 0 );
	NiTimeController::LoadBinary( stream );

	unsigned int uiNumTargets;
	NiStreamLoadBinary( stream, uiNumTargets );
	unsigned int uiNumVertsPerTarget;
	NiStreamLoadBinary( stream, uiNumVertsPerTarget );
	unsigned int uiNumKeys;
	NiStreamLoadBinary( stream, uiNumKeys );

	bound = new NiBound;
	assert( bound );
	bound->LoadBinary( stream );

	NiFloatKey::KeyType eType;
	NiStreamLoadEnum( stream, eType );
	keyType = eType;

	NiFloatKey **ppKeys = new NiFloatKey *[uiNumKeys];
	assert( ppKeys );
	NiFloatKey::CreateFunction cf = NiFloatKey::GetCreateFunction( keyType );
	unsigned int uiI;
	for ( uiI = 0; uiI < uiNumKeys; uiI++ ) {
		ppKeys[uiI] = cf( stream, uiNumTargets );
	}
	NiFloatKey::FillDerivedValuesFunction derived = NiFloatKey::GetFillDerivedFunction( eType );
	assert( derived );
	derived( ppKeys, uiNumKeys, uiNumTargets );
	SetKeys( ppKeys, uiNumKeys, eType );

	NiPoint3 **pVerts = new NiPoint3 *[uiNumTargets];
	assert( pVerts );
	for ( uiI = 0; uiI < uiNumTargets; uiI++ ) {
		pVerts[uiI] = new NiPoint3[uiNumVertsPerTarget];
		assert( pVerts[uiI] );
		for ( unsigned int uiJ = 0; uiJ < uiNumVertsPerTarget; uiJ++ ) {
			pVerts[uiI][uiJ].LoadBinary( stream );
		}
	}
	SetTargets( pVerts, uiNumTargets, uiNumVertsPerTarget );
}

/*
================
NiMorphController::LinkObject

Notes whether the linked target is an NiTriBasedGeom.
================
*/
// FUNCTION: STBC 0x7A9450
void NiMorphController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	if ( NiIsKindOf( NiTriBasedGeom, target ) ) {
		triBasedTarget = true;
	} else {
		assert( NiIsKindOf( NiBezierMesh, target ) );
		triBasedTarget = false;
	}
}

/*
================
NiMorphController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7A9490
bool NiMorphController::RegisterStreamables( NiStream &stream ) {
	return NiTimeController::RegisterStreamables( stream );
}

/*
================
NiMorphController::SaveBinary

As LoadBinary reads it; a controller without a bound saves an empty one
at the origin, which it keeps.
================
*/
// FUNCTION: STBC 0x7A94A0
void NiMorphController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, numTargets );
	NiStreamSaveBinary( stream, numVertices );
	NiStreamSaveBinary( stream, numKeys );

	if ( !bound ) {
		bound = new NiBound;
		assert( bound );
		bound->SetCenter( NiPoint3::ZERO );
		bound->SetRadius( 0.0f );
	}
	bound->SaveBinary( stream );

	NiStreamSaveEnum( stream, keyType );
	NiFloatKey::SaveFunction saveFunc = NiFloatKey::GetSaveFunction( keyType );	// (name invented; fits the /Od frame order)
	unsigned int i;
	for ( i = 0; i < numKeys; i++ ) {
		saveFunc( stream, keys[i], numTargets );
	}

	for ( i = 0; i < numTargets; i++ ) {
		for ( unsigned int j = 0; j < numVertices; j++ ) {
			targets[i][j].SaveBinary( stream );
		}
	}
}

/*
================
NiMorphController::IsEqual

The two flags, the counts, the bound, the keys and every vertex.
================
*/
// FUNCTION: STBC 0x7A95F0
bool NiMorphController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiMorphController *other = (NiMorphController *)object;	// (name invented; fits the /Od frame order)
	if ( GetUnknown10() != other->GetUnknown10()
		|| GetUpdateNormals() != other->GetUpdateNormals()
		|| numTargets != other->numTargets
		|| numVertices != other->numVertices
		|| numKeys != other->numKeys
		|| keyType != other->keyType ) {
		return false;
	}

	if ( *bound != *other->bound ) {
		return false;
	}

	NiFloatKey::EqualFunction equal = NiFloatKey::GetEqualFunction( keyType );
	assert( equal );
	unsigned int i;
	for ( i = 0; i < numKeys; i++ ) {
		if ( !equal( keys[i], other->keys[i], numTargets ) ) {
			return false;
		}
	}

	for ( i = 0; i < numTargets; i++ ) {
		for ( unsigned int j = 0; j < numVertices; j++ ) {
			if ( targets[i][j] != other->targets[i][j] ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
NiMorphController::TargetIsRequiredType

The target must be an NiAVObject.
================
*/
// FUNCTION: STBC 0x7A9970
bool NiMorphController::TargetIsRequiredType() const {
	return NiIsKindOf( NiAVObject, target );
}
