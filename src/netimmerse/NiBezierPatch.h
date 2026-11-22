/*
===============================================================================

	NiBezierPatch.h

	A Bezier surface patch: a leaf of an NiBezierMesh that refers to the
	mesh's control vertices and tessellates them into triangle data at a
	level of detail.

	Evidence: the NiRTTI (0x9B2F98, built at 0x832320) is named
	"NiBezierPatch" with NiAVObject's as its base; the vtable at 0x89A898
	has NiAVObject's 39 slots, overrides 27-35, 37 and 38, and adds 8; the
	derived triangles, rectangles and cylinders (NiBezierTriangle's vtable
	0x89A9CC) keep slots 27-39 and 41-43 and override 44 and 45, the two
	tessellators that slot 39 picks by flag 0x10 (0x8330E0). The
	constructors (0x8324F0, 0x8326D0, 0x832880) and the destructor
	(0x8329C0) give the members: the property and effect states at 0xC8 and
	0xCC as NiGeometry keeps them, the degree, the control point counts and arrays, the
	level of detail at 0xF4 and two tessellation parameters (0.1 and 4.0),
	the tessellated vertex sets (0x104-0x110, 0x144, 0x148), the triangle
	index set (0x118), the triangle data built from them (0x124) and the
	model bound (0x12C) that slot 38 carries into world space.

===============================================================================
*/

#ifndef __NIBEZIERPATCH_H__
#define __NIBEZIERPATCH_H__

#include <string.h>
#include "NiAVObject.h"
#include "NiTriShapeDynamicData.h"
#include "NiTSet.h"

class NiColorA;
#include "NiPoint2.h"
#include "NiStreamMacros.h"
class NiTriShapeData;

/*
================
NiBezierTextureSets

The tessellated texture coordinates: setCount sets in one block, each with
room for maxSize coordinates and its own used count, growing all sets
together. An NiTSet of NiPoint2 per texture set; the class name is
invented.

Evidence: NiBezierPatch's buffer allocation (0x832C30) builds it (0x18
bytes, vtable 0x89A958) when the patch has texture coordinates; the
destructor (0x834B60) frees the counts, then the coordinates. Add
(0x850D50) appends to one set and Realloc (0x850EC0) moves every set into
a larger block. Retail keeps the NiBezierCylinder unit's out-of-line
copies (its slot 41 calls Add); the evaluators' unit emits the same
bodies.
================
*/
// FUNCTION: STBC 0x834B60 ??_GNiBezierTextureSets@@UAEPAXI@Z @netimmerse/NiBezierPatch
class NiBezierTextureSets {
public:
							// as its debug copy (AnimOnly.exe 0x4D4D80, NiTSimpleMultArray.inl)
							NiBezierTextureSets( unsigned int setCount_, unsigned int maxSize_, unsigned int growBy_ ) {
								assert( maxSize_ > 0 );
								setCount = setCount_;
								maxSize = maxSize_;
								growBy = growBy_;
								sizes = new unsigned int[setCount];
								assert( sizes );
								memset( sizes, 0, setCount * sizeof( unsigned int ) );
								base = new NiPoint2[maxSize * setCount];
								assert( base );
							}
	virtual					~NiBezierTextureSets() { delete [] sizes; delete [] base; }

	NiPoint2 *				GetBase() const { return base; }
	// the coordinate at the index of the set
	// (out of line at 0x83A520, the NiBezierTriangle2 unit's copy)
	// FUNCTION: STBC 0x83A520 ?GetAt@NiBezierTextureSets@@QBE?AVNiPoint2@@II@Z @netimmerse/NiBezierTriangle2
	NiPoint2				GetAt( unsigned int set, unsigned int index ) const { return base[set * maxSize + index]; }
	// appends the coordinate to the set and returns its index there
	unsigned int			Add( unsigned int set, NiPoint2 coord );
	// moves the sets into blocks of newMaxSize (Gamebryo lineage name)
	void					Realloc( unsigned int newMaxSize );
	// TODO identity: takes setCount_ sets (keeping the count when it is 0)
	// and allocates every set anew, empty
	// (out of line at 0x83A680, the NiBezierTriangle2 unit's copy)
	// FUNCTION: STBC 0x83A680 ?Unknown83A680@NiBezierTextureSets@@QAEXI@Z @netimmerse/NiBezierTriangle2
	void					Unknown83A680( unsigned int setCount_ ) {
								if ( setCount_ ) {
									setCount = setCount_;
								}
								delete [] base;
								base = new NiPoint2[setCount * maxSize];
								delete [] sizes;
								sizes = new unsigned int[setCount];
								memset( sizes, 0, setCount * sizeof( unsigned int ) );
							}

protected:
	NiPoint2 *				base;			// setCount blocks of maxSize coordinates
	unsigned int			setCount;
	unsigned int			maxSize;		// coordinates each set has room for
	unsigned int *			sizes;			// the used coordinates per set
	unsigned int			growBy;			// 2, as NiTSet's
};

/*
================
NiBezierTextureSets::Add
================
*/
// FUNCTION: STBC 0x850D50 ?Add@NiBezierTextureSets@@QAEIIVNiPoint2@@@Z @netimmerse/NiBezierCylinder
inline unsigned int NiBezierTextureSets::Add( unsigned int set, NiPoint2 coord ) {
	while ( sizes[set] >= maxSize ) {
		Realloc( sizes[set] * growBy );
	}
	base[set * maxSize + sizes[set]] = coord;
	return sizes[set]++;
}

/*
================
NiBezierTextureSets::Realloc

Moves each set into a block of newMaxSize coordinates, unless that is the
current size or would not hold the first set.
================
*/
// FUNCTION: STBC 0x850EC0 ?Realloc@NiBezierTextureSets@@QAEXI@Z @netimmerse/NiBezierCylinder
inline void NiBezierTextureSets::Realloc( unsigned int newMaxSize ) {
	// BUG: only the first set's count is checked; a newMaxSize below a
	// later set's count overruns that set's new block in the copy
	if ( newMaxSize != maxSize && newMaxSize >= sizes[0] ) {
		unsigned int oldMaxSize = maxSize;
		maxSize = newMaxSize;
		NiPoint2 *oldBase = base;
		base = new NiPoint2[setCount * maxSize];
		for ( unsigned int i = 0; i < setCount; i++ ) {
			memcpy( base + i * maxSize, oldBase + i * oldMaxSize, sizes[i] * sizeof( NiPoint2 ) );
		}
		delete [] oldBase;
	}
}

/*
================
NiBezierVertexCache

The tessellators' map from a point of the parameter grid to the index of
the vertex made for it, so that the triangles meeting at a point share its
vertex. An entry holds while its stamp is the cache's; NextStamp empties
the whole cache at once by moving to a new stamp, skipping 0. The class
and member names are invented.

Evidence: the tessellators reach it through the pointer at 0x9B2F80,
bump the word at 0x04 (wrapping to 1) and store a value word and the stamp
word into 4-byte entries; Get is out of line in the NiBezierTriangle unit
(0x838D90) and Set in the NiBezierCylinder unit (0x850D20); the static
destructor (0x8324D0) frees the entries, then the cache.
================
*/
class NiBezierVertexCache {
public:
							~NiBezierVertexCache() { delete [] entries; }

	// the vertex cached for the grid point, when it was set under the
	// current stamp
	// (out of line at 0x838D90, the NiBezierTriangle unit's copy)
	// FUNCTION: STBC 0x838D90 ?Get@NiBezierVertexCache@@QAE_NGAAG@Z @netimmerse/NiBezierTriangle
	bool					Get( unsigned short index, unsigned short &value ) {
								if ( entries[index].stamp != stamp ) {
									return false;
								}
								value = entries[index].value;
								return true;
							}
	// whether the grid point has a vertex under the current stamp
	bool					IsSet( unsigned short index ) const { return entries[index].stamp == stamp; }
	// (out of line at 0x850D20, the NiBezierCylinder unit's copy)
	// FUNCTION: STBC 0x850D20 ?Set@NiBezierVertexCache@@QAEXGG@Z @netimmerse/NiBezierCylinder
	void					Set( unsigned short index, unsigned short value ) {
								entries[index].value = value;
								entries[index].stamp = stamp;
							}
	// empties the cache
	void					NextStamp() {
								if ( ++stamp == 0 ) {
									stamp = 1;
								}
							}

protected:
	struct Entry {
		unsigned short		value;		// the vertex index
		unsigned short		stamp;		// the cache's stamp when it was set
	};

	Entry *					entries;	// one per grid point
	unsigned short			stamp;
};

/*
================
NiBezierVertexCacheOwner

Holds the one cache the tessellators share and frees it at exit. The class
name is invented.

Evidence: the static initialiser (0x8324B0) zeroes the pointer at
0x9B2F80 and registers a destructor (0x8324D0) that deletes the cache
through it. Nothing in retail stores a cache into it. The owner is a
global, not a static member of NiBezierPatch: that destructor has no
destroy-once guard, which VC6 gives the exit destructors of static data
members (the NiRTTI objects' at 0x832340).
================
*/
class NiBezierVertexCacheOwner {
public:
							NiBezierVertexCacheOwner() { cache = 0; }
							~NiBezierVertexCacheOwner() { delete cache; }

	NiBezierVertexCache *	cache;
};

// the grid-point vertex cache the tessellators share (name invented)
extern NiBezierVertexCacheOwner	vertexCache;

// FUNCTION: STBC 0x8326A0 ??_GNiBezierPatch@@UAEPAXI@Z
class NiBezierPatch : public NiAVObject {
	friend class NiBezierMesh;
public:
	// FUNCTION: STBC 0x832690 ?GetRTTI@NiBezierPatch@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiBezierPatch();

	virtual void			UpdatePropertiesDownward( NiPropertyState *parentState );
	virtual void			UpdateEffectsDownward( NiDynamicEffectState *parentState );
	virtual NiPointer<NiPropertyState>	GetPropertyState();
	virtual NiPointer<NiDynamicEffectState>	GetEffectState();
	virtual void			Display( NiCamera *camera );
	virtual bool			TestCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual int				FindCollisions( float deltaTime, NiCollisionRecord &record0, NiCollisionRecord &record1 );
	virtual bool			FindIntersections( const NiPoint3 &origin, const NiPoint3 &dir, NiPick &pick );
	virtual void			VirtualSlot35( NiCamera *camera );
	virtual void			UpdateWorldData();
	virtual void			UpdateWorldBound();

	// TODO identity: retessellates at the level, when it differs from the
	// current one and does not exceed slot 40's
	virtual void			VirtualSlot39( unsigned char level_ );
	// TODO identity: the highest level slot 39 accepts
	// FUNCTION: STBC 0x8326C0 ?VirtualSlot40@NiBezierPatch@@UAEEXZ
	virtual unsigned char	VirtualSlot40() { return 0; }
	// slot 43, then the evaluator: returns the index of the vertex added
	virtual unsigned int	VirtualSlot41( float, float );
	virtual bool			VirtualSlot42( unsigned short, unsigned short, unsigned char, unsigned char, unsigned char, short );
	virtual void			VirtualSlot43( float, float );
	// TODO identity: the two tessellators slot 39 picks by flag 0x10
	virtual void			VirtualSlot44( unsigned char level_ );
	virtual void			VirtualSlot45( unsigned char level_ );
	virtual bool			VirtualSlot46( unsigned short, unsigned short, unsigned short, unsigned short );

							NiBezierPatch( unsigned short degree_, unsigned short controlPointCount_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08 );
							NiBezierPatch( unsigned short degree_, unsigned short controlPointCount_, unsigned short *unknownE4_, NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_, bool unknown08, unsigned short unknownD6_ );
							NiBezierPatch();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	// the mesh's control vertex arrays, which the patch does not own
	void					SetArrays( NiPoint3 *vertices_, NiPoint2 *textures_, NiColorA *colors_ );
	// the degree, the control points and their indices (names invented
	// after the debug copy's m_pCtrlPoint and m_pCtrlIndex; AnimOnly.exe
	// 0x4DA120, 0x4DA140 and 0x4DA160, which NiBezierMesh's patch linking
	// calls)
	unsigned short			GetDegree() const { return degree; }
	NiPoint3 *				GetCtrlPoint() const { return vertices; }
	unsigned short *		GetCtrlIndex() const { return unknownE4; }
	// the setters and the retessellation at the current level that
	// NiBezierMesh's loops call (names invented; AnimOnly.exe 0x4DA370,
	// 0x4DA390, 0x4DA3D0 and 0x4DA340)
	void					SetUnknown128( NiCamera *camera ) { unknown128 = camera; }
	void					SetUnknownF8( float value ) {
								if ( unknownF8 != value ) {
									unknownF8 = value;
								}
							}
	void					SetUnknownFC( float value ) {
								if ( unknownFC != value ) {
									unknownFC = value;
								}
							}
	void					Retessellate() { Unknown8330E0( level ); }
	// TODO identity: the texture set count, handed to the tessellated
	// coordinates
	// (out of line at 0x83A650, the NiBezierTriangle2 unit's copy)
	// FUNCTION: STBC 0x83A650 ?Unknown83A650@NiBezierPatch@@QAEXG@Z @netimmerse/NiBezierTriangle2
	void					Unknown83A650( unsigned short textureSets_ ) {
								textureSets = textureSets_;
								if ( textureCoordinates ) {
									textureCoordinates->Unknown83A680( textureSets_ );
								}
							}
	// TODO identity: sets flag 0x10 and retessellates when it changes
	void					SetUnknown10( int unknown10 );
	// the tessellated positions and normals in world space, updated first
	// when the world transform has changed (names invented; nothing in
	// retail calls them)
	NiPoint3 *				GetWorldPositions();
	NiPoint3 *				GetWorldNormals();

	// TODO identity: flag 0x08, the constructors' last flag argument
	bool					GetUnknown08() const { return ( flags >> UNKNOWN_08_POS ) & 1; }

	// flags, past NiAVObject's
	enum {
		UNKNOWN_10_MASK		= 0x0010,	// picks slot 45 over slot 44 to tessellate
		UNKNOWN_10_POS		= 4
	};

	// unknown11C: the tessellated sets the patch has
	enum {
		SETS_TEXTURES		= 0x0001,
		SETS_COLORS			= 0x0002,
		SETS_NORMALS		= 0x0004,
		SETS_UNKNOWN_E4		= 0x0008
	};

	static const NiRTTI		rtti;

protected:
	// the clone gets copies of the control arrays
	void					CopyMembers( NiBezierPatch *dest );

	// frees the tessellated sets and allocates them anew for vertexCount
	// vertices and triangleCount triangles, with the sets the patch's
	// arrays and flags call for; picks the evaluator for them (name
	// invented)
	void					CreateSets();

	// the evaluators slot 41 calls, one per combination of SETS_ bits: each
	// adds the point at the parameters to the sets and returns the index of
	// its vertex (TODO identity)
	unsigned int			Unknown851330( float u, float v );
	unsigned int			Unknown851400( float u, float v );
	unsigned int			Unknown851620( float u, float v );
	unsigned int			Unknown851840( float u, float v );
	unsigned int			Unknown851B80( float u, float v );
	unsigned int			Unknown851D90( float u, float v );
	unsigned int			Unknown852130( float u, float v );
	unsigned int			Unknown8524D0( float u, float v );
	unsigned int			Unknown852980( float u, float v );
	unsigned int			Unknown852A80( float u, float v );
	unsigned int			Unknown852CD0( float u, float v );
	unsigned int			Unknown852F00( float u, float v );
	unsigned int			Unknown853260( float u, float v );
	unsigned int			Unknown8534B0( float u, float v );
	unsigned int			Unknown853870( float u, float v );
	unsigned int			Unknown853C20( float u, float v );
	static unsigned int		( NiBezierPatch::*evaluators[16] )( float u, float v );

	// TODO identity: rebuilds the vertex sets at the level and hands them
	// to the triangle data
	void					Unknown8330E0( unsigned char level_ );

	// carries the tessellated sets into world space (name invented)
	void					UpdateWorldSets();

	NiPointer<NiPropertyState>	propertyState;
	NiPointer<NiDynamicEffectState>	effectState;
	unsigned short			degree;				// of the surface: NiBezierTriangle has ( degree + 1 ) * ( degree + 2 ) / 2 control points, NiBezierRectangle ( degree + 1 ) squared
	unsigned short			controlPointCount;
	unsigned short			textureSets;		// 1 with texture coordinates, else 0
	unsigned short			unknownD6;
	NiPoint3 *				vertices;			// the mesh's arrays, not owned when unknownE4 is set
	NiPoint2 *				textures;
	NiColorA *				colors;
	unsigned short *		unknownE4;			// owned: the control points' indices into the arrays (the evaluators read vertices[unknownE4[i]]); freed in place of the arrays when set
	// heap arrays of controlPointCount words, set by slot 43: the control
	// points' blending weights at a parameter, and their derivatives with
	// each of the two parameters (the latter when the patch has normals)
	float *				weights;
	float *				uDerivatives;
	float *				vDerivatives;
	unsigned char			level;
	float					unknownF8;			// 0.1 (TODO identity)
	float					unknownFC;			// 4.0 (TODO identity)
	unsigned short			vertexCount;		// of the tessellated sets
	NiTSet<NiPoint3> *		positions;			// owned tessellated sets
	NiTSet<NiPoint3> *		normals;
	NiBezierTextureSets *	textureCoordinates;
	NiTSet<NiColorA> *		vertexColors;
	unsigned short			triangleCount;
	NiTSet<unsigned short> *	triangles;
	unsigned short			unknown11C;			// SETS_ bits
	unsigned int			( NiBezierPatch::*unknown120 )( float, float );	// evaluators[unknown11C]
	NiPointer<NiTriShapeData>	data;
	NiCamera *				unknown128;			// the first camera slot 35 is given (TODO identity)
	NiBound					modelBound;
	bool					worldSetsStale;		// set by UpdateWorldData, cleared by UpdateWorldSets
	NiTSet<NiPoint3> *		worldPositions;		// owned: the tessellated sets in world space
	NiTSet<NiPoint3> *		worldNormals;
};

NiRegisterStream( NiBezierPatch );

#endif /* !__NIBEZIERPATCH_H__ */
