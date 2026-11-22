/*
===============================================================================

	TGAnimTrack.h

	The animation of one node under a blender (TGAnimBlender): the node's
	keyframe and visibility controllers, the keys blended into them and
	where the animation is. The blender keeps its tracks in an array sorted
	by the node's name. The class and its names are invented: no string
	names it.

	0x30 bytes, from the game's pools, not polymorphic. Evidence: the
	blender's AddNode (0x6C6980) allocates one from the pool of size 0x30,
	runs the constructor (0x6C9BE0), stores the keyframe controller at 0x00
	and the visibility controller at 0x04, and names it with the node's
	name (0x6CA470, a copy at 0x24); the destructor (0x6C9C60) frees the
	two strings at 0x24 and 0x28, empties the keyframe data at 0x08 and
	frees the three per-frame arrays at 0x0C, 0x14 and 0x18 (0x10 counts
	their frames). The constructor sets the flags at 0x2C to 6; the setters
	0x6CA1A0, 0x6CA1C0 and 0x6CA1E0 set bits 0, 1 and 2. The step
	(0x6CA0B0) sets bit 1 once the animation has run out; the blender
	counts the track done then.

===============================================================================
*/

#ifndef __TGANIMTRACK_H__
#define __TGANIMTRACK_H__

#include <string.h>

#include "NiPointer.h"
#include "NiKeyframeData.h"
#include "TGPoolManager.h"

class NiKeyframeController;
class NiVisController;
class TGAnimBlender;

class TGAnimTrack {
public:
	TGPOOL_ALLOCATION

					TGAnimTrack( void );
					~TGAnimTrack( void );

	// The order of the blender's array: by the node's name.
	int				Compare( TGAnimTrack *track ) { return strcmp( track->name, name ); }
	int				CompareKey( const char *key ) { return strcmp( key, name ); }

	// flags
	enum {
		LOOPING			= 0x01,		// (invented name)
		DONE			= 0x02,		// the animation has run out
		UNKNOWN_FLAG	= 0x04
	};

	bool			IsDone( void ) { return ( flags >> 1 ) & 1; }
	unsigned char	GetFlags( void ) { return flags; }
	void			SetKeyframeController( NiKeyframeController *controller ) { keyframeController = controller; }
	void			SetVisController( NiVisController *controller ) { visController = controller; }

	void			SetFrameCount( int frames );
	void			Update( float time, TGAnimBlender *blender );
	void			SetLooping( bool looping );
	void			SetDone( bool done );
	void			SetUnknownFlag( bool flag );
	void			CopyFrom( TGAnimTrack *track );
	void			SetName( const char *name_ );
	void			SetNextAnimation( const char *animation );

	NiKeyframeController *	keyframeController;		// 0x00 the node's, not owned
	NiVisController *		visController;			// 0x04 the node's, not owned
	NiPointer<NiKeyframeData>	data;				// 0x08 the keys blended into the controller
	NiRotKey **		rotKeys;				// 0x0C owned, with the keys
	int				frameCount;				// 0x10 frames of the three key arrays
	NiPosKey **		posKeys;				// 0x14 owned, with the keys
	NiFloatKey **	scaleKeys;				// 0x18 owned, with the keys
	float			time;					// 0x1C where the animation started (invented name)
	float			duration;				// 0x20 how long it runs (invented name)
	char *			name;					// 0x24 owned copy of the node's name
	char *			nextAnimation;			// 0x28 owned copy; started when this one runs out
	unsigned char	flags;					// 0x2C the flags above
};

#endif /* !__TGANIMTRACK_H__ */
