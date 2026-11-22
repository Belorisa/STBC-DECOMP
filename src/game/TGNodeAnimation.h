/*
===============================================================================

	TGNodeAnimation.h

	The animation of one node in an animation entry (TGAnimationEntry): the
	node's name, copies of its keyframe and visibility keys, and the node's
	transform and culling flag as the animation file had them. The entry
	keeps them in an array sorted by the node's name. The class and its
	names are invented: no string names it.

	0x44 bytes, from the game's pools, not polymorphic. Evidence: the entry's
	AddNode (0x6CAB00) allocates one from the pool of size 0x44, runs the
	constructor (0x6CCA00), stores a new NiKeyframeData at 0x04 or a new
	NiVisData at 0x08, names it with the node's name (0x6CCC60) and copies
	the node's local rotation, translation and scale to 0x0C, 0x30 and 0x40
	and its GetAppCulled to 0x3C; the destructor (0x6CCBA0) frees the name
	and releases the two data objects. The constructor sets the identity
	transform, a scale of 1 and the culling flag.

===============================================================================
*/

#ifndef __TGNODEANIMATION_H__
#define __TGNODEANIMATION_H__

#include <string.h>

#include "NiPointer.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "NiKeyframeData.h"
#include "NiVisData.h"
#include "TGPoolManager.h"

class TGNodeAnimation {
public:
	TGPOOL_ALLOCATION

					TGNodeAnimation( void );
					TGNodeAnimation( const TGNodeAnimation &other );
					~TGNodeAnimation( void );

	// The order of the entry's array: by the node's name.
	int				Compare( TGNodeAnimation *other ) { return strcmp( other->name, name ); }
	int				CompareKey( const char *key ) { return strcmp( key, name ); }

	// Replaces the name with a copy of name_ (NULL leaves none).
	void			SetName( const char *name_ );

	char *			name;					// 0x00 owned copy of the node's name
	NiPointer<NiKeyframeData>	keyframeData;	// 0x04 copies of the node's keys
	NiPointer<NiVisData>		visData;		// 0x08 copies of the node's visibility keys
	NiMatrix3		rotate;					// 0x0C the node's local rotation
	NiPoint3		translate;				// 0x30 the node's local translation
	bool			appCulled;				// 0x3C the node's GetAppCulled
	float			scale;					// 0x40 the node's local scale
};

#endif /* !__TGNODEANIMATION_H__ */
