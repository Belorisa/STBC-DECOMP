/*
===============================================================================

	TGAnimationEntry.h

	An animation the animation manager holds, found by name. The class and
	its names are invented (no string names it); the members are named from
	how the manager uses them.

	0x18 bytes, from the game's pools. Evidence: LoadAnimation (0x6C8290)
	and MapDefaultAnimation (0x6C80A0) allocate one from the pool of size
	0x18 and run the constructor 0x6CA880 with the name; the destructor is
	0x6CA930. The manager's array is sorted by the name at 0x0C, which
	every lookup compares with strcmp, expanded inline. GetAnimationLength
	reads the member at 0x10. UseAnimation, BlendAnimation, UseDefault and
	PositionUsingAnimation hand the blender they were given to a method of
	the entry. The array at 0x00 holds the animation of each node of the
	file (TGNodeAnimation), sorted by the node's name: the destructor
	deletes them into the pool of size 0x44.

===============================================================================
*/

#ifndef __TGANIMATIONENTRY_H__
#define __TGANIMATIONENTRY_H__

#include <string.h>

#include "SortedArray.h"
#include "NiFloatKey.h"
#include "NiPosKey.h"
#include "NiRotKey.h"
#include "TGPoolManager.h"

class NiAVObject;
class NiNode;
class TGAnimBlender;
class TGAnimTrack;
class TGNodeAnimation;

class TGAnimationEntry {
public:
	TGPOOL_ALLOCATION

					TGAnimationEntry( char *name_ );
					~TGAnimationEntry( void );

	// Reads the animation from the file; 0 when it cannot (name invented).
	int				Load( char *file );
	// Takes the animation of the node tree; always 1.
	int				Unknown6CAAE0( NiNode *node );
	// Adds a TGNodeAnimation for the object and each object under it that
	// has keyframe or visibility keys (name invented).
	void			AddNode( NiAVObject *object );
	// The length: the time of the last rotation or position key of any
	// node (name invented).
	void			UpdateLength( void );

	// The order of the manager's array: by name.
	int				Compare( TGAnimationEntry *entry ) { return strcmp( entry->name, name ); }
	int				CompareKey( char *key ) { return strcmp( key, name ); }

	float			GetLength( void ) { return length; }
	bool			IsSpecial( void ) { return special; }
	bool			MovesPelvis( void ) { return movesPelvis; }
	char *			GetName( void ) { return name; }

	int				Unknown6CB200( TGAnimBlender *blender, bool unknown, float start, float end );
	int				Unknown6CB450( TGAnimBlender *blender, float start, float duration, bool looping, bool unknownFlag, bool blend, char *nextAnimation );
	int				Unknown6CBD80( TGAnimBlender *blender );
	int				Unknown6CC400( TGAnimBlender *blender );

	// The track's frames of one kind, spread evenly over the duration, each
	// moved from the node's own value toward the keys' (or start's) by an
	// even step (names invented).
	bool			BlendRotKeys( TGAnimTrack *track, NiRotKey **keys, int keyCount, NiRotKey::KeyType &keyType, float duration, NiQuaternion *start );
	bool			BlendPosKeys( TGAnimTrack *track, NiPosKey **keys, int keyCount, NiPosKey::KeyType &keyType, float duration, NiPoint3 *start );
	bool			BlendScaleKeys( TGAnimTrack *track, NiFloatKey **keys, int keyCount, NiFloatKey::KeyType &keyType, float duration, float *start );
	// p turned by t toward q the shorter way, and the sign of q that gives
	// it (names invented)
	void			SlerpNearer( NiQuaternion &p, const NiQuaternion &q, float t );
	NiQuaternion	Nearer( const NiQuaternion &p, const NiQuaternion &q );

protected:
	SortedArray<TGNodeAnimation, const char *>	nodes;	// 0x00 owned, by the node's name (invented name)
	char *			name;				// 0x0C owned copy
	float			length;				// 0x10 seconds
	bool			special;			// 0x14 SAT_SPECIAL (invented name)
	bool			movesPelvis;		// 0x15 SAT_MOVES_PELVIS (invented name)
	unsigned char	unknown16[2];		// 0x16
};

#endif /* !__TGANIMATIONENTRY_H__ */
