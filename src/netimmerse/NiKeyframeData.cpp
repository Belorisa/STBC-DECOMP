// NiKeyframeData.cpp -- the rotation, position and scale keys of a transform animation

#include "NiRTLib.h"
#include <assert.h>
#include "NiKeyframeData.h"
#include "NiEulerRotKey.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00792960 _$E97
// FUNCTION: STBC 0x00792980 _$E96
// FUNCTION: STBC 0x00792990 _$E17
// FUNCTION: STBC 0x007929a0 _$E14
// FUNCTION: STBC 0x007929b0 _$E16
// FUNCTION: STBC 0x007929c0 _$E15
// FUNCTION: STBC 0x007929d0 _$E23
// FUNCTION: STBC 0x007929e0 _$E20
// FUNCTION: STBC 0x007929f0 _$E22
// FUNCTION: STBC 0x00792a00 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00792520 _$E65
// FUNCTION: STBC 0x00792530 _$E64
// FUNCTION: STBC 0x00792a10 _$E76
// FUNCTION: STBC 0x00792a20 _$E75
// FUNCTION: STBC 0x00792a30 _$E84
// FUNCTION: STBC 0x00792a40 _$E83
// FUNCTION: STBC 0x00792a50 _$E30
// FUNCTION: STBC 0x00792a60 _$E29
// FUNCTION: STBC 0x00792a70 _$E56
// FUNCTION: STBC 0x00792a80 _$E55
// FUNCTION: STBC 0x00792ac0 _$E80
// FUNCTION: STBC 0x00792ad0 _$E79

// FUNCTION: STBC 0x00791de0 _$E90
// FUNCTION: STBC 0x00791df0 _$E86
// FUNCTION: STBC 0x00791e10 _$E89
// FUNCTION: STBC 0x00791e20 _$E87
// DATA: STBC 0x9A0028 ?rtti@NiKeyframeData@@2VNiRTTI@@B
const NiRTTI NiKeyframeData::rtti( "NiKeyframeData", &NiObject::rtti );

/*
================
NiKeyframeData::Register
================
*/
// FUNCTION: STBC 0x791E40 ?Register@NiKeyframeData@@SAHXZ
NiImplementRegister( NiKeyframeData )

/*
================
NiKeyframeData::NiKeyframeData
================
*/
// FUNCTION: STBC 0x791F90
NiKeyframeData::NiKeyframeData() {
	numRotKeys = 0;
	rotKeys = 0;
	rotKeyType = NiRotKey::NOINTERP;
	numPosKeys = 0;
	posKeys = 0;
	posKeyType = NiPosKey::NOINTERP;
	numScaleKeys = 0;
	scaleKeys = 0;
	scaleKeyType = NiFloatKey::NOINTERP;
}

/*
================
NiKeyframeData::~NiKeyframeData
================
*/
// FUNCTION: STBC 0x791FC0
NiKeyframeData::~NiKeyframeData() {
	DeleteRotKeys( rotKeys, numRotKeys, rotKeyType );

	unsigned int i;
	for ( i = 0; i < numPosKeys; i++ ) {
		delete posKeys[i];
	}
	delete [] posKeys;

	for ( i = 0; i < numScaleKeys; i++ ) {
		delete scaleKeys[i];
	}
	delete [] scaleKeys;
}

/*
================
NiKeyframeData::ReplaceRotAnim
================
*/
// FUNCTION: STBC 0x792080
void NiKeyframeData::ReplaceRotAnim( NiRotKey **keys, unsigned int numKeys, NiRotKey::KeyType keyType ) {
	if ( !keys || !numKeys ) {
		numRotKeys = 0;
		rotKeys = 0;
		rotKeyType = NiRotKey::NOINTERP;
		return;
	}

	for ( unsigned int i = 0; i < numRotKeys; i++ ) {
		delete rotKeys[i];
	}
	delete [] rotKeys;

	numRotKeys = numKeys;
	rotKeys = keys;
	rotKeyType = keyType;
}

/*
================
NiKeyframeData::ReplacePosAnim
================
*/
// FUNCTION: STBC 0x7920F0
void NiKeyframeData::ReplacePosAnim( NiPosKey **keys, unsigned int numKeys, NiPosKey::KeyType keyType ) {
	if ( !keys || !numKeys ) {
		numPosKeys = 0;
		posKeys = 0;
		posKeyType = NiPosKey::NOINTERP;
		return;
	}

	for ( unsigned int i = 0; i < numPosKeys; i++ ) {
		delete posKeys[i];
	}
	delete [] posKeys;

	numPosKeys = numKeys;
	posKeys = keys;
	posKeyType = keyType;
}

/*
================
NiKeyframeData::ReplaceScaleAnim
================
*/
// FUNCTION: STBC 0x792170
void NiKeyframeData::ReplaceScaleAnim( NiFloatKey **keys, unsigned int numKeys, NiFloatKey::KeyType keyType ) {
	if ( !keys || !numKeys ) {
		numScaleKeys = 0;
		scaleKeys = 0;
		scaleKeyType = NiFloatKey::NOINTERP;
		return;
	}

	for ( unsigned int i = 0; i < numScaleKeys; i++ ) {
		delete scaleKeys[i];
	}
	delete [] scaleKeys;

	numScaleKeys = numKeys;
	scaleKeys = keys;
	scaleKeyType = keyType;
}

/*
================
NiKeyframeData::DeleteRotKeys

An Euler rotation keeps its three float-key groups in its first key, which
frees them first.
================
*/
// FUNCTION: STBC 0x7921F0
void NiKeyframeData::DeleteRotKeys( NiRotKey **keys, unsigned int numKeys, NiRotKey::KeyType keyType ) {
	if ( keys && keyType == NiRotKey::EULERKEY ) {
		( (NiEulerRotKey *)keys[0] )->Cleanup();
	}
	for ( unsigned int i = 0; i < numKeys; i++ ) {
		delete keys[i];
	}
	delete [] keys;
}

/*
================
NiKeyframeData::CreateClone

The clones of a controller share its keys.
================
*/
// FUNCTION: STBC 0x792240
NiObject *NiKeyframeData::CreateClone() {
	return this;
}

/*
================
NiKeyframeData::CreateObject
================
*/
// FUNCTION: STBC 0x792260
void NiKeyframeData::CreateObject( NiStream &stream, NiObject *&object ) {
	NiKeyframeData *data = new NiKeyframeData;
	stream.SetLoadRecord( new NiKeyframeDataLoadRecord );
	data->LoadBinary( stream );
	object = data;
}

/*
================
NiKeyframeData::LoadBinary

Each group: the key count, then, when there are keys, their type and the
keys, each read by the function of that type.
================
*/
// FUNCTION: STBC 0x792320
void NiKeyframeData::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	unsigned int count;
	unsigned int i;

	NiStreamLoadBinary( stream, count );
	if ( count > 0 ) {
		NiRotKey::KeyType type;
		NiStreamLoadBinary( stream, type );

		NiRotKey **loaded = new NiRotKey *[count];
		assert( loaded );
		NiRotKey::CreateFunction create = NiRotKey::GetCreateFunction( type );
		assert( create );
		for ( i = 0; i < count; i++ ) {
			loaded[i] = create( stream );
		}
		NiRotKey::FillDerivedValuesFunction derived = NiRotKey::GetFillDerivedFunction( type );
		assert( derived );
		derived( loaded, count );

		ReplaceRotAnim( loaded, count, type );
	}

	NiStreamLoadBinary( stream, count );
	if ( count > 0 ) {
		NiPosKey::KeyType type;
		NiStreamLoadBinary( stream, type );

		NiPosKey **loaded = new NiPosKey *[count];
		assert( loaded );
		NiPosKey::CreateFunction create = NiPosKey::GetCreateFunction( type );
		for ( i = 0; i < count; i++ ) {
			loaded[i] = create( stream );
		}
		NiPosKey::FillDerivedValuesFunction derived = NiPosKey::GetFillDerivedFunction( type );
		assert( derived );
		derived( loaded, count );

		ReplacePosAnim( loaded, count, type );
	}

	NiStreamLoadBinary( stream, count );
	if ( count > 0 ) {
		NiFloatKey::KeyType type;
		NiStreamLoadBinary( stream, type );

		NiFloatKey **loaded = new NiFloatKey *[count];
		assert( loaded );
		NiFloatKey::CreateFunction create = NiFloatKey::GetCreateFunction( type );
		assert( create );
		for ( i = 0; i < count; i++ ) {
			loaded[i] = create( stream, 0 );
		}
		NiFloatKey::FillDerivedValuesFunction derived = NiFloatKey::GetFillDerivedFunction( type );
		assert( derived );
		derived( loaded, count, 0 );

		ReplaceScaleAnim( loaded, count, type );
	}
}

/*
================
NiKeyframeData::LinkObject
================
*/
// FUNCTION: STBC 0x792540
void NiKeyframeData::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiKeyframeData::RegisterStreamables
================
*/
// FUNCTION: STBC 0x792550
bool NiKeyframeData::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiKeyframeData::SaveBinary
================
*/
// FUNCTION: STBC 0x792560
void NiKeyframeData::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );

	unsigned int i;

	assert( rotKeyType != NiRotKey::EULERKEY || numRotKeys == 1 );
	NiStreamSaveBinary( stream, numRotKeys );
	if ( numRotKeys > 0 ) {
		NiStreamSaveEnum( stream, rotKeyType );
		NiRotKey::SaveFunction save = NiRotKey::GetSaveFunction( rotKeyType );
		for ( i = 0; i < numRotKeys; i++ ) {
			save( stream, rotKeys[i] );
		}
	}

	NiStreamSaveBinary( stream, numPosKeys );
	if ( numPosKeys > 0 ) {
		NiStreamSaveEnum( stream, posKeyType );
		NiPosKey::SaveFunction save = NiPosKey::GetSaveFunction( posKeyType );
		for ( i = 0; i < numPosKeys; i++ ) {
			save( stream, posKeys[i] );
		}
	}

	NiStreamSaveBinary( stream, numScaleKeys );
	if ( numScaleKeys > 0 ) {
		NiStreamSaveEnum( stream, scaleKeyType );
		NiFloatKey::SaveFunction save = NiFloatKey::GetSaveFunction( scaleKeyType );
		for ( i = 0; i < numScaleKeys; i++ ) {
			save( stream, scaleKeys[i], 0 );
		}
	}
}

/*
================
NiKeyframeData::IsEqual
================
*/
// FUNCTION: STBC 0x792670
bool NiKeyframeData::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	assert( rotKeyType != NiRotKey::EULERKEY || numRotKeys == 1 );
	NiKeyframeData *other = (NiKeyframeData *)object;	// (name invented; fits the /Od frame order)
	if ( numRotKeys != other->numRotKeys || rotKeyType != other->rotKeyType
		|| numPosKeys != other->numPosKeys || posKeyType != other->posKeyType
		|| numScaleKeys != other->numScaleKeys || scaleKeyType != other->scaleKeyType ) {
		return false;
	}

	unsigned int i;

	NiRotKey::EqualFunction rotEqual = NiRotKey::GetEqualFunction( rotKeyType );
	assert( rotEqual );
	for ( i = 0; i < numRotKeys; i++ ) {
		if ( !rotEqual( rotKeys[i], other->rotKeys[i] ) ) {
			return false;
		}
	}

	NiPosKey::EqualFunction posEqual = NiPosKey::GetEqualFunction( posKeyType );
	assert( posEqual );
	for ( i = 0; i < numPosKeys; i++ ) {
		if ( !posEqual( posKeys[i], other->posKeys[i] ) ) {
			return false;
		}
	}

	NiFloatKey::EqualFunction fltEqual = NiFloatKey::GetEqualFunction( scaleKeyType );	// the name of AnimOnly.exe's assert (NiKeyframeData.cpp(333))
	assert( fltEqual );
	for ( i = 0; i < numScaleKeys; i++ ) {
		if ( !fltEqual( scaleKeys[i], other->scaleKeys[i], 0 ) ) {
			return false;
		}
	}
	return true;
}

/*
================
NiKeyframeData::GetViewerStrings
================
*/
// FUNCTION: STBC 0x792780
void NiKeyframeData::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_uiNumRotKeys", numRotKeys ) );
	strings->Add( NiGetViewerString( "m_uiNumPosKeys", numPosKeys ) );
	strings->Add( NiGetViewerString( "m_uiNumScaleKeys", numScaleKeys ) );
}
