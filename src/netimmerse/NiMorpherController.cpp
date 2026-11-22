// NiMorpherController.cpp -- morphing by the weighted targets of an NiMorphData

#include "NiRTLib.h"
#include <string.h>

#include "NiMorpherController.h"
#include "NiBezFloatKey.h"
#include "NiBezierMesh.h"
#include "NiMath.h"
#include "NiTriBasedGeom.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007aba40 _$E30
// FUNCTION: STBC 0x007aba50 _$E29
// FUNCTION: STBC 0x007aba90 _$E45
// FUNCTION: STBC 0x007abaa0 _$E44
// FUNCTION: STBC 0x007abab0 _$E41
// FUNCTION: STBC 0x007abac0 _$E40
// FUNCTION: STBC 0x007abad0 _$E49
// FUNCTION: STBC 0x007abae0 _$E48
// FUNCTION: STBC 0x007abaf0 _$E53
// FUNCTION: STBC 0x007abb00 _$E52
// FUNCTION: STBC 0x007abb10 _$E177
// FUNCTION: STBC 0x007abb20 _$E176
// FUNCTION: STBC 0x007abb30 _$E214
// FUNCTION: STBC 0x007abb40 _$E213
// FUNCTION: STBC 0x007abb50 _$E245
// FUNCTION: STBC 0x007abb60 _$E244
// FUNCTION: STBC 0x007abb70 _$E218
// FUNCTION: STBC 0x007abb80 _$E217
// FUNCTION: STBC 0x007abb90 _$E249
// FUNCTION: STBC 0x007abba0 _$E248
// FUNCTION: STBC 0x007abbb0 _$E222
// FUNCTION: STBC 0x007abbc0 _$E221
// FUNCTION: STBC 0x007abbf0 _$E186
// FUNCTION: STBC 0x007abc00 _$E185
// FUNCTION: STBC 0x007abc10 _$E190
// FUNCTION: STBC 0x007abc20 _$E189
// FUNCTION: STBC 0x007abc90 _$E194
// FUNCTION: STBC 0x007abca0 _$E193
// FUNCTION: STBC 0x007abe40 _$E197
// FUNCTION: STBC 0x007abe50 _$E196
// FUNCTION: STBC 0x007abf20 _$E202
// FUNCTION: STBC 0x007abf30 _$E201
// FUNCTION: STBC 0x007abf40 _$E206
// FUNCTION: STBC 0x007abf50 _$E205
// FUNCTION: STBC 0x007abf60 _$E226
// FUNCTION: STBC 0x007abf70 _$E225
// FUNCTION: STBC 0x007abf80 _$E234
// FUNCTION: STBC 0x007abf90 _$E233
// FUNCTION: STBC 0x007abfa0 _$E238
// FUNCTION: STBC 0x007abfb0 _$E237

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007aba60 _$E265
// FUNCTION: STBC 0x007aba80 _$E264
// FUNCTION: STBC 0x007ab9a0 _$E17
// FUNCTION: STBC 0x007ab9b0 _$E14
// FUNCTION: STBC 0x007ab9c0 _$E16
// FUNCTION: STBC 0x007ab9d0 _$E15
// FUNCTION: STBC 0x007ab9e0 _$E23
// FUNCTION: STBC 0x007ab9f0 _$E20
// FUNCTION: STBC 0x007aba00 _$E22
// FUNCTION: STBC 0x007aba10 _$E21

// FUNCTION: STBC 0x007aac60 _$E255
// FUNCTION: STBC 0x007aac70 _$E251
// FUNCTION: STBC 0x007aac90 _$E254
// FUNCTION: STBC 0x007aaca0 _$E252
// DATA: STBC 0x9A0650 ?rtti@NiMorpherController@@2VNiRTTI@@B
const NiRTTI NiMorpherController::rtti( "NiMorpherController", &NiTimeController::rtti );

/*
================
NiMorpherController::Register
================
*/
// FUNCTION: STBC 0x7AACC0 ?Register@NiMorpherController@@SAHXZ
NiImplementRegister( NiMorpherController )

/*
================
NiMorpherController::FindController
================
*/
// FUNCTION: STBC 0x7AAE10
NiMorpherController *NiMorpherController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiMorpherController *morpher = NiDynamicCast( NiMorpherController, controller );
		if ( morpher ) {
			return morpher;
		}
	}
	return 0;
}

/*
================
NiMorpherController::NiMorpherController
================
*/
// FUNCTION: STBC 0x7AAE60
NiMorpherController::NiMorpherController( NiMorphData *data_ ) : data( data_ ) {
	if ( data_ ) {
		ResetTimeExtrema();
	}
	updatePending = false;
	ignoreBaseTarget = false;
	updateTime = 0.0f;
}

/*
================
NiMorpherController::~NiMorpherController
================
*/
// FUNCTION: STBC 0x7AAED0
NiMorpherController::~NiMorpherController() {
	data = 0;
}

/*
================
NiMorpherController::ReplaceTargets
================
*/
// FUNCTION: STBC 0x7AAF70
void NiMorpherController::ReplaceTargets( NiMorphData::MorphTarget *targets, unsigned int numTargets, unsigned int numVertsPerTarget, bool resetTimes ) {
	if ( !data ) {
		data = new NiMorphData;
	}
	data->ReplaceTargets( targets, numTargets, numVertsPerTarget );
	if ( resetTimes ) {
		ResetTimeExtrema();
	}
}

/*
================
NiMorpherController::ResetTimeExtrema
================
*/
// FUNCTION: STBC 0x7AB030
void NiMorpherController::ResetTimeExtrema() {
	unsigned int uiNumTargets = GetNumTargets();	// Gamebryo's name; AnimOnly.exe 0x4993E2's frame order needs it

	loKeyTime = NI_INFINITY;
	hiKeyTime = -NI_INFINITY;
	for ( unsigned int i = 0; i < uiNumTargets; i++ ) {
		unsigned int numKeys = GetNumKeys( i );
		if ( numKeys ) {
			NiFloatKey **keys = GetKeys( i );
			float loTime = keys[0]->GetTime();
			float hiTime = keys[numKeys - 1]->GetTime();
			if ( loTime < loKeyTime ) {
				loKeyTime = loTime;
			}
			if ( hiTime > hiKeyTime ) {
				hiKeyTime = hiTime;
			}
		}
	}
}

/*
================
NiMorpherController::Update
================
*/
// FUNCTION: STBC 0x7AB0B0
void NiMorpherController::Update( float time ) {
	if ( !data || DontDoUpdate( time ) ) {
		return;
	}
	updateTime = time;
	updatePending = true;
}

/*
================
NiMorpherController::VirtualSlot15

Morphs the target at the scaled time. An NiTriBasedGeom's model data is
marked changed; an NiBezierMesh, the other kind of target, rebuilds its
edge points, its patches and its bound. The debug copy (AnimOnly.exe
0x4994ED) reads the model data through a non-virtual NiTriBasedGeom
accessor (0x498040) this tree doesn't declare yet.
================
*/
// FUNCTION: STBC 0x7AB0E0
void NiMorpherController::VirtualSlot15() {
	if ( updatePending ) {
		float time = ComputeScaledTime( updateTime );
		GenMorphInterp( time );

		if ( NiIsKindOf( NiTriBasedGeom, target ) ) {
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
}

/*
================
NiMorpherController::GenMorphInterp

Each target's vertices, weighted by its keys at time, are added to the
target's vertices; with relative targets the first target is the base,
weighted one.
================
*/
// FUNCTION: STBC 0x7AB190
void NiMorpherController::GenMorphInterp( float time ) {
	NiPoint3 *verts;
	if ( NiIsKindOf( NiTriBasedGeom, target ) ) {
		verts = ( (NiTriBasedGeom *)target )->GetVertices();
	} else {
		verts = ( (NiBezierMesh *)target )->GetVertices();
	}

	unsigned int numVerts = data->GetNumVertsPerTarget();
	unsigned int numTargets = data->GetNumTargets();
	if ( !ignoreBaseTarget ) {
		memset( verts, 0, numVerts * sizeof( NiPoint3 ) );
	}

	for ( unsigned int i = 0; i < numTargets; i++ ) {
		float weight;
		if ( i == 0 && data->GetRelativeTargets() ) {
			weight = ignoreBaseTarget ? 0.0f : 1.0f;
		} else {
			// AnimOnly.exe 0x4995C5 (3.1) keeps the key count, keys and key type in
			// locals; retail's bytes read the count twice, as written here
			if ( !data->GetNumKeys( i ) ) {
				continue;
			}
			weight = NiFloatKey::GenInterp( time, data->GetKeys( i ), data->GetKeyType( i ), data->GetNumKeys( i ), data->GetLastIndex( i ) );
		}

		NiPoint3 *targetVerts = data->GetTargetVerts( i );
		for ( unsigned int j = 0; j < numVerts; j++ ) {
			verts[j] += weight * targetVerts[j];
		}
	}
}

/*
================
NiMorpherController::CreateClone
================
*/
// FUNCTION: STBC 0x7AB2E0
NiObject *NiMorpherController::CreateClone() {
	NiMorpherController *clone = new NiMorpherController;
	CopyMembers( clone );
	return clone;
}

/*
================
NiMorpherController::CopyMembers

The clone shares the data.
================
*/
// FUNCTION: STBC 0x7AB340
void NiMorpherController::CopyMembers( NiMorpherController *dest ) {
	NiTimeController::CopyMembers( dest );
	if ( data ) {
		dest->SetMorphData( data );
	}
}

/*
================
NiMorpherController::CreateObject
================
*/
// FUNCTION: STBC 0x7AB390
void NiMorpherController::CreateObject( NiStream &stream, NiObject *&object ) {
	NiMorpherController *controller = new NiMorpherController;
	stream.SetLoadRecord( new NiMorpherControllerLoadRecord );
	controller->LoadBinary( stream );
	object = controller;
}

/*
================
NiMorpherController::LoadBinary

From "3.05" on, the data's link ID. Older files hold the morph in the
controller: the counts, each target's key count and key type, its keys
(weights in percent), the base vertices and each target's vertices. They
become an NiMorphData of relative targets with the base as the first.
Statements and asserts as the debug copy has them (AnimOnly.exe 0x499942,
NiMorpherController.cpp), which sets the Bezier tangents through setters
this tree's NiBezFloatKey doesn't declare yet.
================
*/
// FUNCTION: STBC 0x7AB420
void NiMorpherController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );

	if ( strcmp( stream.GetFileVersion(), "3.05" ) >= 0 ) {
		NiMorpherControllerLoadRecord *record = (NiMorpherControllerLoadRecord *)stream.GetLoadRecord();
		assert( record );
		NiStreamLoadBinary( stream, record->dataLinkID );
		return;
	}

	unsigned int numTargets;
	NiStreamLoadBinary( stream, numTargets );
	unsigned int numVerts;
	NiStreamLoadBinary( stream, numVerts );

	unsigned int i;
	unsigned int *numKeys = new unsigned int[numTargets];
	assert( numKeys );
	for ( i = 0; i < numTargets; i++ ) {
		NiStreamLoadBinary( stream, numKeys[i] );
	}

	NiFloatKey::KeyType *keyTypes = new NiFloatKey::KeyType[numTargets];
	assert( keyTypes );
	for ( i = 0; i < numTargets; i++ ) {
		NiStreamLoadEnum( stream, keyTypes[i] );
	}

	NiFloatKey ***keys = new NiFloatKey **[numTargets];
	assert( keys );
	for ( i = 0; i < numTargets; i++ ) {
		NiFloatKey::CreateFunction create = NiFloatKey::GetCreateFunction( keyTypes[i] );
		keys[i] = new NiFloatKey *[numKeys[i]];
		assert( keys[i] );
		for ( unsigned int j = 0; j < numKeys[i]; j++ ) {
			keys[i][j] = create( stream, numTargets );
			keys[i][j]->SetValue( keys[i][j]->GetValue() * 0.01f );
			if ( keyTypes[i] == NiFloatKey::BEZKEY ) {
				NiBezFloatKey *key = (NiBezFloatKey *)keys[i][j];
				key->inTangent *= 0.01f;
				key->outTangent *= 0.01f;
			}
		}
		NiFloatKey::FillDerivedValuesFunction derived = NiFloatKey::GetFillDerivedFunction( keyTypes[i] );
		assert( derived );
		derived( keys[i], numKeys[i], numTargets );
	}

	NiPoint3 *baseVerts = new NiPoint3[numVerts];
	assert( baseVerts );
	for ( i = 0; i < numVerts; i++ ) {
		baseVerts[i].LoadBinary( stream );
	}

	NiPoint3 **targetVerts = new NiPoint3 *[numTargets];
	assert( targetVerts );
	for ( i = 0; i < numTargets; i++ ) {
		targetVerts[i] = new NiPoint3[numVerts];
		assert( targetVerts[i] );
		for ( unsigned int k = 0; k < numVerts; k++ ) {
			targetVerts[i][k].LoadBinary( stream );
		}
	}

	data = new NiMorphData;
	data->SetRelativeTargets( true );

	numTargets++;
	NiMorphData::MorphTarget *targets = new NiMorphData::MorphTarget[numTargets];
	targets[0].ReplaceTargetVerts( baseVerts );
	targets[0].ReplaceAnim( 0, 0, NiFloatKey::LINKEY );
	for ( i = 1; i < numTargets; i++ ) {
		unsigned int prev = i - 1;
		targets[i].ReplaceTargetVerts( targetVerts[prev] );
		targets[i].ReplaceAnim( keys[prev], numKeys[prev], keyTypes[prev] );
	}

	delete [] numKeys;
	delete [] keyTypes;
	delete [] keys;
	delete [] targetVerts;

	data->ReplaceTargets( targets, numTargets, numVerts );
}

/*
================
NiMorpherController::LinkObject
================
*/
// FUNCTION: STBC 0x7AB880
void NiMorpherController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	if ( strcmp( stream.GetFileVersion(), "3.05" ) >= 0 ) {
		NiMorpherControllerLoadRecord *linkRecord = (NiMorpherControllerLoadRecord *)stream.GetLoadRecord();
		assert( linkRecord );
		SetMorphData( (NiMorphData *)stream.GetObjectFromLinkID( linkRecord->dataLinkID ) );
	}
}

/*
================
NiMorpherController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7AB910
bool NiMorpherController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( data ) {
		data->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiMorpherController::SaveBinary
================
*/
// FUNCTION: STBC 0x7AB940
void NiMorpherController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );
	NiStreamSaveBinary( stream, data );
}

/*
================
NiMorpherController::IsEqual
================
*/
// FUNCTION: STBC 0x7AB970
bool NiMorpherController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiMorpherController *controller = (NiMorpherController *)object;
	if ( !data->IsEqual( controller->data ) ) {
		return false;
	}
	return true;
}
