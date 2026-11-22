/*
===============================================================================

	NiColorKey.h

	A key of a colour animation. The interpolation type (the key type) is
	kept by the owner of the keys, not by each key; what a key holds beyond
	its time depends on that type, so the streaming and comparison go
	through tables of functions indexed by the key type, which the key
	classes fill in at startup.

	Evidence: NiColorData's LoadBinary (0x79DAF0), SaveBinary (0x79DBE0)
	and IsEqual (0x79DC50) call through the tables at 0x9A02C0, 0x9A02C8
	and 0x9A02B4 (in .bss), indexed by the key type at NiColorData+0x10.
	Unlike the float keys, the functions take no third argument and no
	derived values are filled in after loading. The names of the tables
	and of the key types follow the Gamebryo lineage; only the time is
	known of the key.

===============================================================================
*/

#ifndef __NICOLORKEY_H__
#define __NICOLORKEY_H__

#include "NiColor.h"
#include "NiStreamMacros.h"

class NiStream;

class NiColorKey {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x79D4A0)
	static int				Register();

							~NiColorKey() {}

	// the interpolation types (Gamebryo lineage names)
	enum KeyType {
		NOINTERP,
		LINKEY,
		BEZKEY,
		TCBKEY,
		NUMKEYTYPES
	};

	float					GetTime() const { return time; }

	// the color at time, searching from the key the last call found
	// (0x79D4F0; Gamebryo lineage name)
	static NiColorA			GenInterp( float time, NiColorKey **keys, KeyType keyType, unsigned int numKeys, unsigned int &lastIndex );

	// the per-type functions
	typedef NiColorKey *	( *CreateFunction )( NiStream &stream );
	typedef bool			( *EqualFunction )( NiColorKey *key0, NiColorKey *key1 );
	typedef NiColorKey *	( *CopyFunction )( NiColorKey *key );
	typedef void			( *SaveFunction )( NiStream &stream, NiColorKey *key );

	typedef NiColorA		( *InterpFunction )( float t, NiColorKey *key0, NiColorKey *key1 );

	static InterpFunction	interpFunctions[NUMKEYTYPES];
	static CreateFunction	createFunctions[NUMKEYTYPES];
	static EqualFunction	equalFunctions[NUMKEYTYPES];
	static CopyFunction		copyFunctions[NUMKEYTYPES];
	static SaveFunction		saveFunctions[NUMKEYTYPES];

	// a type's functions, as the other classes read them (Gamebryo lineage
	// names)
	static CreateFunction	GetCreateFunction( KeyType type ) { return createFunctions[type]; }
	static EqualFunction	GetEqualFunction( KeyType type ) { return equalFunctions[type]; }
	static SaveFunction	GetSaveFunction( KeyType type ) { return saveFunctions[type]; }

	// the NOINTERP entries, shared by the key classes that add nothing of
	// their own (the base class is abstract: it creates and copies nothing)
	static NiColorKey *		Create( NiStream &stream );
	static NiColorA			Interpolate( float t, NiColorKey *key0, NiColorKey *key1 );
	static bool				Equal( NiColorKey *key0, NiColorKey *key1 );
	static NiColorKey *		Copy( NiColorKey *key );
	static void				Save( NiStream &stream, NiColorKey *key );

	// the time, then the colour
	void					LoadBinary( NiStream &stream );

	float					time;
	NiColorA				color;
};

NiRegisterStream( NiColorKey );

#endif /* !__NICOLORKEY_H__ */
