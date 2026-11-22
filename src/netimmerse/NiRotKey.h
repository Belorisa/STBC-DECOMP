/*
===============================================================================

	NiRotKey.h

	A key of a rotation animation: a time and a rotation. As with the
	position and float keys, the interpolation type is kept by the owner of
	the keys, and the streaming, comparison, copying and interpolation go
	through tables of functions indexed by that type, which each key class
	fills in from a static initializer.

	Evidence: the tables hold five entries (NOINTERP to EULERKEY): the
	register functions of the base class (0x791010), the linear keys
	(0x7961E0) and the Euler keys (0x7964B0) store entries 0, 1 and 4 of
	the six tables at 0x99FE44, 0x99FE58, 0x99FE6C, 0x99FE84, 0x99FE9C and
	0x99FEB0 (in .bss). NiKeyframeData (0x792FF0) passes its rotation keys
	and type to GenInterp (0x791070), which hands EULERKEY to the Euler
	interpolation with the first key alone. The linear keys allocate 0x14
	bytes: the time, then the quaternion. The names of the tables and of
	the key types follow the Gamebryo lineage.

===============================================================================
*/

#ifndef __NIROTKEY_H__
#define __NIROTKEY_H__

#include "NiQuaternion.h"
#include "NiStreamMacros.h"

class NiStream;

class NiRotKey {
public:
							NiRotKey() : quat( 0.0f, 0.0f, 0.0f, 0.0f ) { time = 0.0f; }
							NiRotKey( float time_, const NiQuaternion &quat_ ) : quat( quat_ ) { time = time_; }
							// Gamebryo 1.2.2 (NiRotKey.inl): the quaternion built in place from the angle and axis
							NiRotKey( float time_, float angle, const NiPoint3 &axis ) : quat( angle, axis ) { time = time_; }

	// the interpolation types (Gamebryo lineage names)
	enum KeyType {
		NOINTERP,
		LINKEY,
		BEZKEY,
		TCBKEY,
		EULERKEY,
		NUMKEYTYPES
	};

	float					GetTime() const { return time; }
	const NiQuaternion &	GetQuaternion() const { return quat; }
	// Gamebryo lineage names (AnimOnly.exe 0x485280, 0x4852A0)
	void					SetTime( float time_ ) { time = time_; }
	void					SetQuaternion( const NiQuaternion &quat_ ) { quat = quat_; }

	// the rotation at time, interpolated by the keys' type; lastIndex
	// keeps the key found by the previous call (Gamebryo lineage name)
	static NiQuaternion		GenInterp( float time, NiRotKey **keys, KeyType keyType, unsigned int numKeys, unsigned int &lastIndex );

	// fills in the NOINTERP entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the per-type functions
	typedef NiRotKey *		( *CreateFunction )( NiStream &stream );
	typedef NiQuaternion	( *InterpFunction )( float t, NiRotKey *key0, NiRotKey *key1 );
	typedef void			( *FillDerivedValuesFunction )( NiRotKey **keys, unsigned int numKeys );
	typedef bool			( *EqualFunction )( NiRotKey *key0, NiRotKey *key1 );
	typedef void			( *SaveFunction )( NiStream &stream, NiRotKey *key );
	typedef NiRotKey *		( *CopyFunction )( NiRotKey *key );

	// Gamebryo lineage name; AnimOnly.exe 0x4852C0, which
	// NiAnimBlender::GatherKeys calls
	static CopyFunction		GetCopyFunction( KeyType type ) { return copyFunctions[type]; }

	static EqualFunction				equalFunctions[NUMKEYTYPES];
	static InterpFunction				interpFunctions[NUMKEYTYPES];
	static CopyFunction					copyFunctions[NUMKEYTYPES];
	static CreateFunction				createFunctions[NUMKEYTYPES];
	static FillDerivedValuesFunction	fillDerivedValuesFunctions[NUMKEYTYPES];
	static SaveFunction					saveFunctions[NUMKEYTYPES];

	// a type's functions, as the other classes read them (Gamebryo lineage
	// names)
	static CreateFunction	GetCreateFunction( KeyType type ) { return createFunctions[type]; }
	static InterpFunction	GetInterpFunction( KeyType type ) { return interpFunctions[type]; }
	static FillDerivedValuesFunction	GetFillDerivedFunction( KeyType type ) { return fillDerivedValuesFunctions[type]; }
	static EqualFunction	GetEqualFunction( KeyType type ) { return equalFunctions[type]; }
	static SaveFunction	GetSaveFunction( KeyType type ) { return saveFunctions[type]; }

	// the NOINTERP entries, shared by the key classes that add nothing of
	// their own (names invented)
	static NiRotKey *		Create( NiStream &stream );
	static NiQuaternion		Interpolate( float t, NiRotKey *key0, NiRotKey *key1 );
	static void				FillDerivedValues( NiRotKey **keys, unsigned int numKeys );
	static bool				Equal( NiRotKey *key0, NiRotKey *key1 );
	static void				Save( NiStream &stream, NiRotKey *key );
	static NiRotKey *		Copy( NiRotKey *key );

	// the time, then the quaternion
	void					LoadBinary( NiStream &stream );

	float					time;
	NiQuaternion			quat;
};

NiRegisterStream( NiRotKey );

#endif /* !__NIROTKEY_H__ */
