/*
===============================================================================

	BoneState.h

	The rotation tracks a BoneStateController plays on one bone.

	BoneState, 0x74 bytes, has no RTTI or vtable of its own. Evidence:
	RotateBonesController::SetTarget allocates 0x74 bytes, calls the
	constructor (0x65F0A0: the array of tracks at 4, NiTArray vtable
	0x894690, one element growing by one; the bone cleared), sets the bone
	(0x65F540) and the tracks (0x65F550). Update (0x65F390) reads the bone's
	local rotation at 0x30; the tracks hold linear rotation keys
	(NiRotKey's LINKEY entries of the interpolation and copy tables). The
	class and member names are invented.

===============================================================================
*/

#ifndef __BONESTATE_H__
#define __BONESTATE_H__

#include "NiTArray.h"
#include "NiMatrix3.h"
#include "NiRotKey.h"

class NiAVObject;

/*
================
BoneKeyList

One track of a BoneState: its rotation keys, each allocated on its own,
owned with the block that holds them. 8 bytes.
================
*/
class BoneKeyList {
public:
							BoneKeyList() { keys = NULL; numKeys = 0; }
							~BoneKeyList() {
								if ( keys != NULL && numKeys != 0 ) {
									for ( int i = 0; i < numKeys; i++ ) {
										delete keys[i];
									}
									delete [] keys;
								}
							}

	NiRotKey **				keys;		// 0x00 linear rotation keys, by time
	int						numKeys;	// 0x04
};

/*
================
BoneState

The rotation tracks of one bone and where the playing one is.
================
*/
class BoneState {
public:
							BoneState();
							~BoneState();

	BoneState *				Clone();
	void					Reset();
	void					Update( float delta );
	void					SetBone( NiAVObject *bone_ );
	void					SetTrack( int index, NiRotKey **keys, int numKeys );

protected:
	friend class RotateBonesController;

	NiAVObject *			bone;			// 0x00
	NiTArray<BoneKeyList *>	tracks;			// 0x04 owned
	float					time;			// 0x1C seconds into the current track
	NiMatrix3				rotation;		// 0x20 the rotation last applied
	NiMatrix3				lastRotate;		// 0x44 the bone's rotation after it was applied
	float					delay;			// 0x68 seconds left before the track plays
	unsigned int			currentTrack;	// 0x6C
	bool					finished;		// 0x70 the track has reached its last key
};

#endif /* !__BONESTATE_H__ */
