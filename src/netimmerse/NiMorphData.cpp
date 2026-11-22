// NiMorphData.cpp -- the morph targets of a morphing animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiMorphData.h"
#include "NiBezFloatKey.h"
#include "NiBezPosKey.h"
#include "NiBezRotKey.h"
#include "NiBezierCylinder.h"
#include "NiBezierMesh.h"
#include "NiBezierPatch.h"
#include "NiBezierRectangle.h"
#include "NiBezierRectangle2.h"
#include "NiBezierRectangle3.h"
#include "NiBezierSkinController.h"
#include "NiBezierTriangle.h"
#include "NiBezierTriangle2.h"
#include "NiBezierTriangle3.h"
#include "NiBezierTriangle4.h"
#include "NiColorKey.h"
#include "NiDynamicEffect.h"
#include "NiEulerRotKey.h"
#include "NiKeyType6FloatKey.h"
#include "NiLinColorKey.h"
#include "NiLinFloatKey.h"
#include "NiLinPosKey.h"
#include "NiLinRotKey.h"
#include "NiMorphKey.h"
#include "NiMorpherController.h"
#include "NiNode.h"
#include "NiPosKey.h"
#include "NiRotKey.h"
#include "NiSkinController.h"
#include "NiTCBPosKey.h"
#include "NiTCBRotKey.h"
#include "NiTriShapeDynamicData.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007a9aa0 _$E72
// FUNCTION: STBC 0x007a9ab0 _$E71
// FUNCTION: STBC 0x007a9ac0 _$E337
// FUNCTION: STBC 0x007a9ad0 _$E336
// FUNCTION: STBC 0x007a9ae0 _$E75
// FUNCTION: STBC 0x007a9af0 _$E74
// FUNCTION: STBC 0x007a9b00 _$E350
// FUNCTION: STBC 0x007a9b10 _$E349
// FUNCTION: STBC 0x007a9b20 _$E86
// FUNCTION: STBC 0x007a9b30 _$E85
// FUNCTION: STBC 0x007a9b40 _$E340
// FUNCTION: STBC 0x007a9b50 _$E339
// FUNCTION: STBC 0x007a9b60 _$E89
// FUNCTION: STBC 0x007a9b70 _$E88
// FUNCTION: STBC 0x007a9b80 _$E353
// FUNCTION: STBC 0x007a9b90 _$E352
// FUNCTION: STBC 0x007a9ba0 _$E319
// FUNCTION: STBC 0x007a9bb0 _$E318
// FUNCTION: STBC 0x007a9c00 _$E315
// FUNCTION: STBC 0x007a9c10 _$E314
// FUNCTION: STBC 0x007a9c20 _$E331
// FUNCTION: STBC 0x007a9c30 _$E330
// FUNCTION: STBC 0x007a9c80 _$E285
// FUNCTION: STBC 0x007a9c90 _$E284
// FUNCTION: STBC 0x007a9d40 _$E289
// FUNCTION: STBC 0x007a9d50 _$E288
// FUNCTION: STBC 0x007a9d60 _$E295
// FUNCTION: STBC 0x007a9d70 _$E294
// FUNCTION: STBC 0x007a9d80 _$E299
// FUNCTION: STBC 0x007a9d90 _$E298
// FUNCTION: STBC 0x007a9da0 _$E303
// FUNCTION: STBC 0x007a9db0 _$E302
// FUNCTION: STBC 0x007a9dc0 _$E307
// FUNCTION: STBC 0x007a9dd0 _$E306
// FUNCTION: STBC 0x007a9de0 _$E273
// FUNCTION: STBC 0x007a9df0 _$E272
// FUNCTION: STBC 0x007a9e00 _$E277
// FUNCTION: STBC 0x007a9e10 _$E276
// FUNCTION: STBC 0x007a9e20 _$E281
// FUNCTION: STBC 0x007a9e30 _$E280
// FUNCTION: STBC 0x007a9e40 _$E254
// FUNCTION: STBC 0x007a9e50 _$E253
// FUNCTION: STBC 0x007aab10 _$E46
// FUNCTION: STBC 0x007aab20 _$E45
// FUNCTION: STBC 0x007aab30 _$E322
// FUNCTION: STBC 0x007aab40 _$E321
// FUNCTION: STBC 0x007aab50 _$E325
// FUNCTION: STBC 0x007aab60 _$E324
// FUNCTION: STBC 0x007aab70 _$E343
// FUNCTION: STBC 0x007aab80 _$E342
// FUNCTION: STBC 0x007aab90 _$E328
// FUNCTION: STBC 0x007aaba0 _$E327
// FUNCTION: STBC 0x007aabb0 _$E334
// FUNCTION: STBC 0x007aabc0 _$E333
// FUNCTION: STBC 0x007aabd0 _$E58
// FUNCTION: STBC 0x007aabe0 _$E57
// FUNCTION: STBC 0x007aabf0 _$E30
// FUNCTION: STBC 0x007aac00 _$E29
// FUNCTION: STBC 0x007aac40 _$E55
// FUNCTION: STBC 0x007aac50 _$E54

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x007aaa60 _$E368
// FUNCTION: STBC 0x007aaa80 _$E367
// FUNCTION: STBC 0x007aaa90 _$E17
// FUNCTION: STBC 0x007aaaa0 _$E14
// FUNCTION: STBC 0x007aaab0 _$E16
// FUNCTION: STBC 0x007aaac0 _$E15
// FUNCTION: STBC 0x007aaad0 _$E23
// FUNCTION: STBC 0x007aaae0 _$E20
// FUNCTION: STBC 0x007aaaf0 _$E22
// FUNCTION: STBC 0x007aab00 _$E21

// FUNCTION: STBC 0x007a9e60 _$E359
// FUNCTION: STBC 0x007a9e70 _$E355
// FUNCTION: STBC 0x007a9e90 _$E358
// FUNCTION: STBC 0x007a9ea0 _$E356
// DATA: STBC 0x9A05D0 ?rtti@NiMorphData@@2VNiRTTI@@B
const NiRTTI NiMorphData::rtti( "NiMorphData", &NiObject::rtti );

/*
================
NiMorphData::Register
================
*/
// FUNCTION: STBC 0x7A9EC0 ?Register@NiMorphData@@SAHXZ
NiImplementRegister( NiMorphData )

/*
================
NiMorphData::MorphTarget::MorphTarget
================
*/
// FUNCTION: STBC 0x7AA010 ??0MorphTarget@NiMorphData@@QAE@XZ
NiMorphData::MorphTarget::MorphTarget() {
	targetVerts = 0;
	numKeys = 0;
	keyType = NiFloatKey::NOINTERP;
	keys = 0;
	lastIndex = 0;
}

/*
================
NiMorphData::MorphTarget::~MorphTarget
================
*/
// FUNCTION: STBC 0x7AA030 ??1MorphTarget@NiMorphData@@QAE@XZ
NiMorphData::MorphTarget::~MorphTarget() {
	delete [] targetVerts;
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		delete keys[i];
	}
	delete [] keys;
}

/*
================
NiMorphData::MorphTarget::ReplaceAnim
================
*/
// FUNCTION: STBC 0x7AA080
void NiMorphData::MorphTarget::ReplaceAnim( NiFloatKey **keys_, unsigned int numKeys_, NiFloatKey::KeyType keyType_ ) {
	delete [] keys;
	keys = keys_;
	numKeys = numKeys_;
	keyType = keyType_;
	lastIndex = 0;
}

/*
================
NiMorphData::MorphTarget::ReplaceTargetVerts
================
*/
// FUNCTION: STBC 0x7AA0B0
void NiMorphData::MorphTarget::ReplaceTargetVerts( NiPoint3 *targetVerts_ ) {
	delete [] targetVerts;
	targetVerts = targetVerts_;
	lastIndex = 0;
}

/*
================
NiMorphData::NiMorphData
================
*/
// FUNCTION: STBC 0x7AA0D0
NiMorphData::NiMorphData() {
	numTargets = 0;
	numVertsPerTarget = 0;
	targets = 0;
	relativeTargets = false;
}

/*
================
NiMorphData::~NiMorphData
================
*/
// FUNCTION: STBC 0x7AA0F0
NiMorphData::~NiMorphData() {
	delete [] targets;
}

/*
================
NiMorphData::ReplaceTargets
================
*/
// FUNCTION: STBC 0x7AA160
void NiMorphData::ReplaceTargets( MorphTarget *targets_, unsigned int numTargets_, unsigned int numVertsPerTarget_ ) {
	delete [] targets;
	targets = targets_;
	numTargets = numTargets_;
	numVertsPerTarget = numVertsPerTarget_;
}

/*
================
NiMorphData::SetAndAdjustRelativeTargets

Converts the vertices of the targets after the first between offsets from
the first and absolute positions.
================
*/
// FUNCTION: STBC 0x7AA1B0
void NiMorphData::SetAndAdjustRelativeTargets( bool relativeTargets_ ) {
	if ( relativeTargets == relativeTargets_ ) {
		return;
	}
	relativeTargets = relativeTargets_;
	if ( !targets ) {
		return;
	}

	NiPoint3 *base = targets[0].GetTargetVerts();
	if ( relativeTargets ) {
		for ( unsigned int i = 1; i < numTargets; i++ ) {
			NiPoint3 *verts = targets[i].GetTargetVerts();
			for ( unsigned int j = 0; j < numVertsPerTarget; j++ ) {
				verts[j] -= base[j];
			}
		}
	} else {
		for ( unsigned int i = 1; i < numTargets; i++ ) {
			NiPoint3 *verts = targets[i].GetTargetVerts();
			for ( unsigned int j = 0; j < numVertsPerTarget; j++ ) {
				verts[j] += base[j];
			}
		}
	}
}

/*
================
NiMorphData::CreateClone
================
*/
// FUNCTION: STBC 0x7AA2C0
NiObject *NiMorphData::CreateClone() {
	return this;
}

/*
================
NiMorphData::CreateObject
================
*/
// FUNCTION: STBC 0x7AA2E0
void NiMorphData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiMorphData *data = new NiMorphData;
	stream.SetLoadRecord( new NiMorphDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiMorphData::LoadBinary

The counts and the relative flag, then per target its keys (a count, a
type, the keys) and its vertices.
================
*/
// FUNCTION: STBC 0x7AA3A0
void NiMorphData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	NiStreamLoadBinary( stream, numTargets );
	NiStreamLoadBinary( stream, numVertsPerTarget );
	bool relative;
	NiStreamLoadBinary( stream, relative );
	relativeTargets = ( relative == true );

	assert( numTargets && numVertsPerTarget );
	targets = new MorphTarget[numTargets];
	for ( unsigned int i = 0; i < numTargets; i++ ) {
		unsigned int numKeys;
		NiStreamLoadBinary( stream, numKeys );
		NiFloatKey::KeyType keyType;
		NiStreamLoadEnum( stream, keyType );

		NiFloatKey::CreateFunction create = NiFloatKey::GetCreateFunction( keyType );
		NiFloatKey **keys = new NiFloatKey *[numKeys];
		for ( unsigned int j = 0; j < numKeys; j++ ) {
			keys[j] = create( stream, numTargets );
		}
		NiFloatKey::FillDerivedValuesFunction derived = NiFloatKey::GetFillDerivedFunction( keyType );
		derived( keys, numKeys, 0 );
		targets[i].ReplaceAnim( keys, numKeys, keyType );

		NiPoint3 *verts = new NiPoint3[numVertsPerTarget];
		for ( j = 0; j < numVertsPerTarget; j++ ) {
			verts[j].LoadBinary( stream );
		}
		targets[i].ReplaceTargetVerts( verts );
	}
}

/*
================
NiMorphData::LinkObject
================
*/
// FUNCTION: STBC 0x7AA5A0
void NiMorphData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiMorphData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7AA5B0
bool NiMorphData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiMorphData::SaveBinary
================
*/
// FUNCTION: STBC 0x7AA5C0
void NiMorphData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );

	NiStreamSaveBinary( stream, numTargets );
	NiStreamSaveBinary( stream, numVertsPerTarget );
	bool relative = relativeTargets;
	NiStreamSaveBinary( stream, relative );

	for ( unsigned int i = 0; i < numTargets; i++ ) {
		unsigned int numKeys = targets[i].GetNumKeys();
		NiFloatKey::KeyType keyType = targets[i].GetKeyType();
		NiStreamSaveBinary( stream, numKeys );
		NiStreamSaveEnum( stream, keyType );

		NiFloatKey::SaveFunction save = NiFloatKey::GetSaveFunction( keyType );
		NiFloatKey **keys = targets[i].GetKeys();
		for ( unsigned int j = 0; j < numKeys; j++ ) {
			save( stream, keys[j], numTargets );
		}

		NiPoint3 *verts = targets[i].GetTargetVerts();
		for ( j = 0; j < numVertsPerTarget; j++ ) {
			verts[j].SaveBinary( stream );
		}
	}
}

/*
================
NiMorphData::IsEqual

Written as the SDK's debug NetImmerse 3.1 compiles it (AnimOnly.exe
0x498B04): the per-target accessors, the type's equality test through
NiFloatKey::GetEqualFunction, one index for both inner loops and the
vertices compared by value.
================
*/
// FUNCTION: STBC 0x7AA6F0
bool NiMorphData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiMorphData *data = (NiMorphData *)object;
	if ( numTargets != data->numTargets || numVertsPerTarget != data->numVertsPerTarget ||
		relativeTargets != data->relativeTargets ) {
		return false;
	}

	for ( unsigned int i = 0; i < numTargets; i++ ) {
		unsigned int morphKeyCount = GetNumKeys( i );	// (name invented; fits the /Od frame order)
		NiFloatKey::KeyType keyType = GetKeyType( i );
		if ( GetNumKeys( i ) != data->GetNumKeys( i ) || GetKeyType( i ) != data->GetKeyType( i ) ) {
			return false;
		}

		NiFloatKey::EqualFunction equal = NiFloatKey::GetEqualFunction( keyType );
		assert( equal );
		unsigned int j;
		for ( j = 0; j < morphKeyCount; j++ ) {
			if ( !equal( targets[i].GetKeyAt( j ), data->targets[i].GetKeyAt( j ), 0 ) ) {
				return false;
			}
		}

		// the vertex array is read and left unused: the vertices are compared
		// one by one through GetTargetVert (AnimOnly.exe 0x498C75)
		NiPoint3 *morphVerts = targets[i].GetTargetVerts();	// (name invented; fits the /Od frame order)
		for ( j = 0; j < numVertsPerTarget; j++ ) {
			if ( targets[i].GetTargetVert( j ) != data->targets[i].GetTargetVert( j ) ) {
				return false;
			}
		}
	}
	return true;
}

/*
================
NiMorphData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7AA880
void NiMorphData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiNumTargets", numTargets ) );
	strings->Add( NiGetViewerString( "m_uiNumVertsPerTarget", numVertsPerTarget ) );
}
