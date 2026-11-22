/*
===============================================================================

	NiFloatKey.h

	A key of a float animation. The interpolation type (the key type) is
	kept by the owner of the keys, not by each key; what a key holds beyond
	its time depends on that type, so the streaming and comparison go
	through tables of functions indexed by the key type, which the key
	classes fill in at startup.

	Evidence: the register function of the base class (0x791B20) stores
	entry 0 of the six tables at 0x99FF68, 0x99FF88, 0x99FFA4, 0x99FFC4,
	0x99FFE0 and 0x99FFFC (in .bss, six entries each). NiFloatData's
	LoadBinary (0x795320), SaveBinary (0x795410) and IsEqual (0x795480)
	call through the creation, derived-value, save and comparison tables,
	indexed by the key type at NiFloatData+0x10; GenInterp (0x791B80)
	through the interpolation table. The base keys save the time then the
	value (0x791D00); NiFloatController (0x794A90) reads the time at 0 of
	the first and last keys. The names of the tables and of the key types
	follow the Gamebryo lineage. The TCB functions (0x79B9F0 interpolates,
	0x79BA30 copies) read the value at 4.

===============================================================================
*/

#ifndef __NIFLOATKEY_H__
#define __NIFLOATKEY_H__

#include "NiStreamMacros.h"

class NiStream;

class NiFloatKey {
public:
							~NiFloatKey() {}

	// the interpolation types (Gamebryo lineage names)
	enum KeyType {
		NOINTERP,
		LINKEY,
		BEZKEY,
		TCBKEY,
		KEYTYPE4,	// NiKeyType4FloatKey; TODO identity: TCB-laid-out keys that do not interpolate
		MORPHKEY,	// NiMorphKey (name invented)
		KEYTYPE6,	// NiKeyType6FloatKey; TODO identity: TCB keys whose value is a morph target index
		NUMKEYTYPES
	};

	float					GetTime() const { return time; }
	float					GetValue() const { return value; }
	void					SetTime( float time_ ) { time = time_; }		// Gamebryo lineage name
	void					SetValue( float value_ ) { value = value_; }	// Gamebryo lineage name

	// the value at time, interpolated by the keys' type; lastIndex keeps
	// the key found by the previous call (Gamebryo lineage name)
	static float			GenInterp( float time, NiFloatKey **keys, KeyType keyType, unsigned int numKeys, unsigned int &lastIndex );

	// fills in the NOINTERP entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the per-type functions; the last argument is 0 for plain float keys
	// and the number of values of a morph key (NiMorphController's
	// CopyMembers passes its target count)
	typedef float			( *InterpFunction )( float t, NiFloatKey *key0, NiFloatKey *key1 );
	typedef void			( *FillDerivedValuesFunction )( NiFloatKey **keys, unsigned int numKeys, int unknown );
	typedef NiFloatKey *	( *CreateFunction )( NiStream &stream, int unknown );
	typedef bool			( *EqualFunction )( NiFloatKey *key0, NiFloatKey *key1, int unknown );
	typedef NiFloatKey *	( *CopyFunction )( NiFloatKey *key, int unknown );
	typedef void			( *SaveFunction )( NiStream &stream, NiFloatKey *key, int unknown );

	static InterpFunction				interpFunctions[NUMKEYTYPES];
	static FillDerivedValuesFunction	fillDerivedValuesFunctions[NUMKEYTYPES];
	static CreateFunction				createFunctions[NUMKEYTYPES];
	static EqualFunction				equalFunctions[NUMKEYTYPES];
	static CopyFunction					copyFunctions[NUMKEYTYPES];
	static SaveFunction					saveFunctions[NUMKEYTYPES];

	// a type's functions, as the other classes read them (Gamebryo lineage
	// names; AnimOnly.exe 0x479E30, 0x479E60, 0x479E40, 0x479E50)
	static CreateFunction	GetCreateFunction( KeyType type ) { return createFunctions[type]; }
	static FillDerivedValuesFunction	GetFillDerivedFunction( KeyType type ) { return fillDerivedValuesFunctions[type]; }
	static EqualFunction	GetEqualFunction( KeyType type ) { return equalFunctions[type]; }
	static SaveFunction	GetSaveFunction( KeyType type ) { return saveFunctions[type]; }
	static CopyFunction	GetCopyFunction( KeyType type ) { return copyFunctions[type]; }	// AnimOnly.exe 0x47E030

	// the NOINTERP entries, shared by the key classes that add nothing of
	// their own (names invented)
	static NiFloatKey *		Create( NiStream &stream, int unknown );
	static float			Interpolate( float t, NiFloatKey *key0, NiFloatKey *key1 );
	static void				FillDerivedValues( NiFloatKey **keys, unsigned int numKeys, int unknown );
	static bool				Equal( NiFloatKey *key0, NiFloatKey *key1, int unknown );
	static void				Save( NiStream &stream, NiFloatKey *key, int unknown );
	static NiFloatKey *		Copy( NiFloatKey *key, int unknown );

	// the time, then the value
	void					LoadBinary( NiStream &stream, int unknown );

	float					time;
	float					value;
};

NiRegisterStream( NiFloatKey );

#endif /* !__NIFLOATKEY_H__ */
