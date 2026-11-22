/*
===============================================================================

	ProximityManager.h

	A set's tracker of which objects come near which: the set creates it
	(SetClass::SetProximityManagerActive) and adds its objects to it; the
	SWIG wrappers ProximityManager_* name the calls the scripts make.

	0x64 bytes (SetProximityManagerActive 0x40E420 allocates 0x64 and runs
	the constructor 0x5A7420, then stores the set at 4). The constructor
	clears 4, builds a list at 8 (an allocator byte, the head node at 0xC
	and the count at 0x10), three 0x14-byte sort axes at 0x14, 0x28 and 0x3C
	(through the vector constructor iterator, 0x5A7510 and 0x5A7520; it then
	sizes each, 0x5A92E0, and numbers them 0, 1 and 2 with the manager as
	their owner), the arrays at 0x50 (0x5A9450) and 0x58 (0x5A9720, 0x80),
	and stores the vtable 0x8942D4, which has one slot. The destructor
	(0x5A7550) is not virtual: SetProximityManagerActive calls it and
	frees the block itself. It removes each object of the array at 0x50
	(0x1C-byte entries) first.

===============================================================================
*/

#ifndef __PROXIMITYMANAGER_H__
#define __PROXIMITYMANAGER_H__

#include <vector>
#include <list>
#include "TGPoint3.h"

class NiPoint3;

class ObjectClass;
class ProximityManager;
class SetClass;
class TGStream;

/*
================
ProximityEndpoint

One end of an object's extent along an axis (0xC bytes; the name is invented).
================
*/
struct ProximityEndpoint {
	float				value;			// 0x00 the coordinate
	bool				isMin;			// 0x04 true for the lower end of the extent, false for the upper
	int					entry;			// 0x08 the index of the object's entry in the manager's table
};

/*
================
ProximityEntry

One object of the manager's table (0x1C bytes; the name is invented). The
table's slot is free while object is NULL.
================
*/
struct ProximityEntry {
	ObjectClass *		object;			// 0x00
	int					endpoints[3][2];	// 0x04 per axis, the index of the object's lower and upper endpoint in that axis

						ProximityEntry( void ) : object( NULL ) {}
};

/*
================
ProximityObjectTable

The manager's objects, one entry each (0x8 bytes; the name is invented).
Constructor 0x5A9450, destructor 0x5A94D0.
================
*/
class ProximityObjectTable {
public:
						ProximityObjectTable( void );
						~ProximityObjectTable( void );

	int					Add( ObjectClass *object );
	int					Find( ObjectClass *object );
	ObjectClass *		GetObject( int index ) { return entries[index].object; }
	void				Remove( int index, ProximityManager *manager );

	int					numEntries;		// 0x00 the entries the block holds
	ProximityEntry *	entries;		// 0x04
};

/*
================
ProximityCandidate

An object a query found near on some axes, with on how many (0x8 bytes; the
name is invented). GetNearObjects keeps them sorted by object ID.
================
*/
struct ProximityCandidate {
	int					count;			// 0x00 the axes on which the object's extent came near
	ObjectClass *		object;			// 0x04

						ProximityCandidate( void ) : count( 0 ), object( NULL ) {}
};

/*
================
ProximityPair

Two objects, in the order of their IDs (0x8 bytes; the name is invented).
================
*/
struct ProximityPair {
	ObjectClass *		first;			// 0x00 the lower ID
	ObjectClass *		second;			// 0x04

						ProximityPair( ObjectClass *a, ObjectClass *b );
	bool				operator==( const ProximityPair &other ) const;
	bool				operator>( const ProximityPair &other ) const;
	bool				operator<( const ProximityPair &other ) const;
};

/*
================
ProximityPairCounts

For each pair of table entries, on how many of the three axes their extents
overlap (0x0C bytes; the name is invented). The triangle of pairs is stored
by index (0x5A9970).
================
*/
class ProximityPairCounts {
public:
						ProximityPairCounts( int numEntries );
						~ProximityPairCounts( void );

	void				Resize( int numEntries );
	int					PairIndex( int a, int b );
	void				GetPair( int index, int *a, int *b );	// invented name
	int					Increment( int a, int b );
	int					Decrement( int a, int b );
	// the pair's count, and setting it (invented names and helpers): retail's
	// RemoveObject reads the counts through the pair counts' own pointer, as
	// Increment and Decrement do, not through the manager (invented)
	char				GetCount( int a, int b ) { return counts[PairIndex( a, b )]; }
	void				SetCount( int a, int b, char count ) { counts[PairIndex( a, b )] = count; }

	int					numEntries;		// 0x00
	int					numCounts;		// 0x04 the counts the block holds: one per pair
	char *				counts;			// 0x08
};

/*
================
ProximityAxis

The manager's objects sorted along one axis (0x14 bytes; the name is
invented). The constructor (0x5A7510) clears 4, 8 and 0xC; the destructor
(0x5A7520) frees the block at 4.
================
*/
class ProximityAxis {
public:
						ProximityAxis( void );
						~ProximityAxis( void );

	void				Reserve( int capacity );
	void				Insert( int index, const ProximityEndpoint &endpoint, ProximityObjectTable *table );
	void				Remove( int lower, int upper, ProximityObjectTable *table );
	void				RemoveAt( int index );		// invented name
	void				Swap( int a, int b, ProximityObjectTable *table );
	void				UpdateEntry( int index, ProximityObjectTable *table );
	// TODO identity: 0x5A8CC0 puts an object's two endpoints in order on the axis (binary
	// searches), and, for each object the extent now overlaps on this axis, bumps the pair's
	// count and adds the pair to the touching list when all three axes overlap (invented name).
	void				Unknown5A8CC0( const ProximityEndpoint &lower, const ProximityEndpoint &upper, ProximityPairCounts *counts, ProximityObjectTable *table, std::list<ProximityPair> *touching );

	ProximityEndpoint &	operator[]( int index );

	int					axis;			// 0x00 0, 1 or 2 (invented name)
	ProximityEndpoint *	endpoints;		// 0x04 sorted by value
	int					numEndpoints;	// 0x08
	int					capacity;		// 0x0C the endpoints the block holds
	class ProximityManager *	owner;	// 0x10 (invented name)
};

class ProximityManager {
	// ProximityAxis::Unknown5A8CC0 (0x5A8CC0) calls ObjectsInteract.
	friend class ProximityAxis;
public:
					ProximityManager( void );
					~ProximityManager( void );

	// Writes the objects of the array at 0x50 that are ProximityChecks and
	// have nothing at 0x20 to the stream, between two markers; false when
	// one fails.
	virtual bool	VirtualSlot00( TGStream *stream );

	// The objects a query found, walked with GetNextObject and freed with
	// EndObjectIteration (SWIG's _p_ProximityManager__ObjectIterator).
	//
	// Evidence: GetNextObject (0x5A8320) walks an array of object pointers
	// from 0x08 to 0x0C by the index at 0x00, and takes its size the way
	// VC6's vector::size does (nothing when the first pointer is NULL);
	// EndObjectIteration (0x5A8350) runs the destructor 0x5A8370, which
	// frees the array and clears its three pointers (vector's destructor),
	// then frees the block. The queries (not reconstructed yet) fill it.
	// FUNCTION: STBC 0x005a8370 ??1ObjectIterator@ProximityManager@@QAE@XZ
	class ObjectIterator {
	public:
		unsigned int	index;			// 0x00 the next object GetNextObject returns (invented name)
		std::vector<ObjectClass *>	objects;	// 0x04 the result (invented name)
	};

	// The objects whose bounds come within radius of the point (the SWIG
	// wrapper's format "OOf|i").
	ObjectIterator *	GetNearObjects( const NiPoint3 &center, float radius, bool );
	// the scripts' form: the SWIG wrapper passes a copy of the TGPoint3
	ObjectIterator *	GetNearObjects( const TGPoint3 &center, float radius, bool flag ) { return GetNearObjects( (NiPoint3)center, radius, flag ); }	// the flag's meaning is unknown
	// The objects whose bounds come within radius of the segment from
	// start to end.
	ObjectIterator *	GetLineIntersectObjects( const NiPoint3 &start, const NiPoint3 &end, float radius, bool );
	ObjectIterator *	GetLineIntersectObjects( const TGPoint3 &start, const TGPoint3 &end, float radius, bool exact ) { return GetLineIntersectObjects( (NiPoint3)start, (NiPoint3)end, radius, exact ); }
	ObjectClass *	GetNextObject( ObjectIterator *iterator );
	void			EndObjectIteration( ObjectIterator *iterator );
	void	AddObject( ObjectClass * );
	void	RemoveObject( ObjectClass * );
	void	DumpCollisions( void );
	void	Update( void );
	void	UpdateObject( ObjectClass * );

private:
	void	UpdateEntryBounds( int index );			// invented name
	void	SortAxis( int axisIndex );				// invented name
	void	CollideTouching( void );				// invented name
	// Whether the two objects' collision groups let them meet (0x5A7890; invented name).
	static bool	ObjectsInteract( ObjectClass *a, ObjectClass *b );
	// What happens when the two objects touch (0x5A8810; invented name).
	void	CollidePair( ObjectClass *a, ObjectClass *b );
	// Counts the object once more among the sorted candidates, adding it with a
	// count of one when it isn't there yet (0x5A8780; invented name).
	void	AddCandidate( ProximityCandidate *candidates, int *numCandidates, ObjectClass *object );
	// The corners of the box the segment from start to end spans (0x5A8290;
	// invented name).
	void	GetSegmentBounds( const NiPoint3 &start, const NiPoint3 &end, NiPoint3 *minimum, NiPoint3 *maximum );
	// Two damageable objects that touch (0x5A88E0; invented name).
	void	CollideDamageables( class DamageableObject *a, class DamageableObject *b );

public:

	SetClass *		set;				// 0x04 the set that owns the manager
	std::list<ProximityPair>	touching;	// 0x08 the pairs of objects whose extents overlap on all three axes (invented name; allocator byte, head node at 0x0C, count at 0x10)
	ProximityAxis	axes[3];			// 0x14 (invented name)
	ProximityObjectTable	table;		// 0x50 (invented name)
	ProximityPairCounts	pairCounts;		// 0x58 (invented name)

	// Declared for the SWIG wrappers that call them.
public:
	static void	SetMultiplayerPlayerCollisionsEnabled( bool );
	static void	SetPlayerCollisionsEnabled( bool enabled ) { playerCollisionsEnabled = enabled; }
	static bool	GetPlayerCollisionsEnabled( void ) { return playerCollisionsEnabled; }

	// Whether the players' ships collide with each other: the collision
	// test (0x5A88E0) reads the multiplayer flag in multiplayer and the
	// other outside it; SetMultiplayerPlayerCollisionsEnabled and the
	// multiplayer windows set the first, and a new MultiplayerGame starts
	// it from the second (invented names).
	static bool		playerCollisionsEnabled;
	static bool		multiplayerPlayerCollisionsEnabled;
};

#endif /* !__PROXIMITYMANAGER_H__ */
