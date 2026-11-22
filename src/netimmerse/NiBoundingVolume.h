/*
===============================================================================

	NiBoundingVolume.h

	The alternate bounding volumes of NetImmerse's collision system: an
	abstract base whose derived shapes (sphere, box, capsule and so on) are
	created from a stream through a table of loaders indexed by type. The
	class name is from the Gamebryo lineage (NiBoundingVolume, with the
	model and world ABVs of NiAVObject).

	Evidence: NiAVObject's destructor (0x7DC210) deletes its two volumes
	through slot 2; NiAVObject::LoadBinary (0x7DD480) creates the model
	volume with CreateFromStream (0x8220F0) and the world volume with the
	model volume's slot 6; NiAVObject::SaveBinary (0x7DD6A0) calls slot 1;
	NiAVObject::IsEqual (0x7DD7B0) compares the volumes with slot 9; the
	base SaveBinary (0x822140) writes the type that slot 3 returns. No
	derived shape is linked into stbc.exe: nothing fills the loader table
	(0x9A2CE4), so no vtable of the class survives and the slots past 9 are
	unknown.

===============================================================================
*/

#ifndef __NIBOUNDINGVOLUME_H__
#define __NIBOUNDINGVOLUME_H__

class NiBound;
class NiMatrix3;
class NiPoint3;
class NiStream;

class NiBoundingVolume {
public:
	// TODO identity: slot 0 is presumed to be LoadBinary, the empty
	// function (0x822130) beside the base SaveBinary
	virtual void				LoadBinary( NiStream &stream );
	virtual void				SaveBinary( NiStream &stream );
	virtual						~NiBoundingVolume() {}
	virtual int					GetType() const = 0;
	virtual void				VirtualSlot04() = 0;
	// the world volume: the model volume carried by a rotation, a
	// translation and a scale (Gamebryo lineage name; NiAVObject's
	// collision tests, 0x7DCDB0 and 0x7DCF00, call it on worldABV)
	virtual void				UpdateWorldData( const NiBoundingVolume &modelABV, const NiMatrix3 &rotate,
									const NiPoint3 &translate, float scale ) = 0;
	virtual NiBoundingVolume *	Create() const = 0;		// a new volume of the same type
	virtual void				VirtualSlot07() = 0;
	virtual void				VirtualSlot08() = 0;
	virtual bool				operator!=( const NiBoundingVolume &volume ) const = 0;

	// the shape types, the indices of the loader table
	enum {
		MAX_TYPES			= 6
	};

	typedef NiBoundingVolume *	( *LoadFunction )( NiStream &stream );
	static NiBoundingVolume *	CreateFromStream( NiStream &stream );
	static LoadFunction			loaders[MAX_TYPES];

	// the collision tests between two moving volumes, and between a moving
	// volume and a moving triangle: whether they meet within deltaTime,
	// and where and when they first do (Gamebryo lineage names). Each goes
	// through the table entry of the shape types, trying a pair both ways
	// round; with no entry they do not meet.
	static bool					TestIntersect( float deltaTime, const NiBoundingVolume &volume0, const NiPoint3 &velocity0,
									const NiBoundingVolume &volume1, const NiPoint3 &velocity1 );
	static bool					FindIntersect( float deltaTime, const NiBoundingVolume &volume0, const NiPoint3 &velocity0,
									const NiBoundingVolume &volume1, const NiPoint3 &velocity1, float &time, NiPoint3 &point,
									bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 );
	static bool					TestTriIntersect( float deltaTime, const NiBoundingVolume &volume, const NiPoint3 &velocity0,
									const NiPoint3 &vertex0, const NiPoint3 &vertex1, const NiPoint3 &vertex2, const NiPoint3 &velocity1 );
	static bool					FindTriIntersect( float deltaTime, const NiBoundingVolume &volume, const NiPoint3 &velocity0,
									const NiPoint3 &vertex0, const NiPoint3 &vertex1, const NiPoint3 &vertex2, const NiPoint3 &velocity1,
									float &time, NiPoint3 &point, bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 );

	// a volume around a bound sphere, and around points (names invented);
	// NULL with no shape to create
	static NiBoundingVolume *	CreateFromBound( const NiBound &bound );
	static NiBoundingVolume *	CreateFromData( unsigned short count, const NiPoint3 *points );

	// the tables the shapes fill in: the pair tests by type0 * MAX_TYPES
	// + type1, the triangle tests by type, and the two creation functions
	// (names invented; nothing in stbc.exe fills them)
	typedef bool				( *TestFunction )( float deltaTime, const NiBoundingVolume &volume0, const NiPoint3 &velocity0,
									const NiBoundingVolume &volume1, const NiPoint3 &velocity1 );
	typedef bool				( *FindFunction )( float deltaTime, const NiBoundingVolume &volume0, const NiPoint3 &velocity0,
									const NiBoundingVolume &volume1, const NiPoint3 &velocity1, float &time, NiPoint3 &point,
									bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 );
	typedef bool				( *TestTriFunction )( float deltaTime, const NiBoundingVolume &volume, const NiPoint3 &velocity0,
									const NiPoint3 &vertex0, const NiPoint3 &vertex1, const NiPoint3 &vertex2, const NiPoint3 &velocity1 );
	typedef bool				( *FindTriFunction )( float deltaTime, const NiBoundingVolume &volume, const NiPoint3 &velocity0,
									const NiPoint3 &vertex0, const NiPoint3 &vertex1, const NiPoint3 &vertex2, const NiPoint3 &velocity1,
									float &time, NiPoint3 &point, bool calcNormals, NiPoint3 &normal0, NiPoint3 &normal1 );
	typedef NiBoundingVolume *	( *SphereFunction )( float radius, const NiPoint3 &center );
	typedef NiBoundingVolume *	( *DataFunction )( unsigned short count, const NiPoint3 *points );

	static TestFunction *		testFunctions;
	static FindFunction *		findFunctions;
	static TestTriFunction *	testTriFunctions;
	static FindTriFunction *	findTriFunctions;
	static SphereFunction		sphereFunction;
	static DataFunction			dataFunction;
};

#endif /* !__NIBOUNDINGVOLUME_H__ */
