/*
===============================================================================

	NiPosKey.h

	A key of a position animation (NiMaterialColorController animates its
	colours with them too): a time and a position. The interpolation type
	(the key type) is kept by the owner of the keys, not by each key; what a
	key holds beyond its time and position depends on that type, so the
	streaming, comparison, copying and interpolation go through tables of
	functions indexed by the key type, which the key classes fill in at
	startup.

	Evidence: the register functions of the base class (0x791420) and of
	the linear keys (0x795EF0) store entries 0 and 1 of the nine tables at
	0x99FEC8, 0x99FED8, 0x99FEE8, 0x99FEFC, 0x99FF10, 0x99FF20, 0x99FF30,
	0x99FF40 and 0x99FF54 (in .bss, four entries each). NiPosData's
	LoadBinary (0x7A5F70), SaveBinary (0x7A60A0) and IsEqual (0x7A6110)
	call through the creation, save and comparison tables, indexed by the
	key type at NiPosData+0x10; GenInterp (0x7914D0) through the
	interpolation table, and CoordinateFrame (0x791660) through the two
	derivative tables. The base keys save the time then an NiPoint3
	(0x791980), and the linear keys allocate 0x10 bytes. The names of the
	tables, of the key types and of GenInterp and CoordinateFrame follow
	the Gamebryo lineage.

===============================================================================
*/

#ifndef __NIPOSKEY_H__
#define __NIPOSKEY_H__

#include "NiPoint3.h"
#include "NiStreamMacros.h"

class NiStream;

class NiPosKey {
public:
							NiPosKey() {}
							NiPosKey( float time_, const NiPoint3 &pos_ ) { time = time_; pos = pos_; }	// assigns: retail calls NiPoint3::operator= (0x414560)
							~NiPosKey() {}

	// the interpolation types (Gamebryo lineage names)
	enum KeyType {
		NOINTERP,
		LINKEY,
		BEZKEY,
		TCBKEY,
		NUMKEYTYPES
	};

	float					GetTime() const { return time; }
	const NiPoint3 &		GetPos() const { return pos; }
	// Gamebryo lineage names (AnimOnly.exe 0x4852D0, 0x4852F0)
	void					SetTime( float time_ ) { time = time_; }
	void					SetPos( const NiPoint3 &pos_ ) { pos = pos_; }

	// the value at time, interpolated by the keys' type; lastIndex keeps
	// the key found by the previous call (Gamebryo lineage name)
	static NiPoint3			GenInterp( float time, NiPosKey **keys, KeyType keyType, unsigned int numKeys, unsigned int &lastIndex );

	// the unit tangent, normal and binormal of the path at t between two
	// keys, and its curvature (Gamebryo lineage name)
	static void				CoordinateFrame( float t, NiPosKey *key0, NiPosKey *key1, KeyType keyType, NiPoint3 &tangent, NiPoint3 &normal, NiPoint3 &binormal, float &curvature );

	// fills in the NOINTERP entries of the tables, once; 1 on the first call
	// (name invented)
	static int				Register();

	// the per-type functions
	typedef float			( *CurvatureFunction )( NiPosKey **keys, unsigned int numKeys );
	typedef NiPosKey *		( *CopyFunction )( NiPosKey *key );
	typedef void			( *SaveFunction )( NiStream &stream, NiPosKey *key );
	typedef NiPoint3		( *InterpFunction )( float t, NiPosKey *key0, NiPosKey *key1 );
	typedef NiPosKey *		( *CreateFunction )( NiStream &stream );
	typedef void			( *FillDerivedValuesFunction )( NiPosKey **keys, unsigned int numKeys );
	typedef bool			( *EqualFunction )( NiPosKey *key0, NiPosKey *key1 );

	// Gamebryo lineage name; AnimOnly.exe 0x485310, which
	// NiAnimBlender::GatherKeys calls
	static CopyFunction		GetCopyFunction( KeyType type ) { return copyFunctions[type]; }

	static CurvatureFunction			curvatureFunctions[NUMKEYTYPES];
	static CopyFunction					copyFunctions[NUMKEYTYPES];
	static SaveFunction					saveFunctions[NUMKEYTYPES];
	static InterpFunction				interpD1Functions[NUMKEYTYPES];
	static InterpFunction				interpD2Functions[NUMKEYTYPES];
	static CreateFunction				createFunctions[NUMKEYTYPES];
	static FillDerivedValuesFunction	fillDerivedValuesFunctions[NUMKEYTYPES];
	static EqualFunction				equalFunctions[NUMKEYTYPES];
	static InterpFunction				interpFunctions[NUMKEYTYPES];

	// a type's functions, as the other classes read them (Gamebryo lineage
	// names)
	static CreateFunction	GetCreateFunction( KeyType type ) { return createFunctions[type]; }
	static CurvatureFunction	GetCurvatureFunction( KeyType type ) { return curvatureFunctions[type]; }
	static InterpFunction	GetInterpFunction( KeyType type ) { return interpFunctions[type]; }
	static InterpFunction	GetInterpD1Function( KeyType type ) { return interpD1Functions[type]; }
	static FillDerivedValuesFunction	GetFillDerivedFunction( KeyType type ) { return fillDerivedValuesFunctions[type]; }
	static EqualFunction	GetEqualFunction( KeyType type ) { return equalFunctions[type]; }
	static SaveFunction	GetSaveFunction( KeyType type ) { return saveFunctions[type]; }

	// the number of segments a curve is sampled in, and the parameter step
	// of one segment (names invented)
	static int				sampleSegments;
	static float			sampleDelta;

	// the NOINTERP entries, shared by the key classes that add nothing of
	// their own (names invented)
	static NiPosKey *		Create( NiStream &stream );
	static NiPoint3			Interpolate( float t, NiPosKey *key0, NiPosKey *key1 );
	static NiPoint3			InterpolateD1( float t, NiPosKey *key0, NiPosKey *key1 );
	static NiPoint3			InterpolateD2( float t, NiPosKey *key0, NiPosKey *key1 );
	static float			ComputeMaxCurvature( NiPosKey **keys, unsigned int numKeys );
	static void				FillDerivedValues( NiPosKey **keys, unsigned int numKeys );
	static bool				Equal( NiPosKey *key0, NiPosKey *key1 );
	static void				Save( NiStream &stream, NiPosKey *key );
	static NiPosKey *		Copy( NiPosKey *key );

	// the time, then the position
	void					LoadBinary( NiStream &stream );

	float					time;
	NiPoint3				pos;
};

NiRegisterStream( NiPosKey );

#endif /* !__NIPOSKEY_H__ */
