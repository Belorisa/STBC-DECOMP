/*
===============================================================================

	ModelMerge.h

	Turns a model into as few triangle shapes as it has textures: the
	shapes that share a texture image are welded into one, in the model's
	own space, under a new node named "Scene Root". LODModel builds the
	model it keeps for a detail level this way (0x450580 calls 0x46DCD0).

	No string or wrapper names this code: the unit's name and every
	function name are invented. Evidence for the unit: its functions
	(0x46D9F0 to 0x46ED60) sit between DistanceModifier's unit and
	TimeSliceProcess's, with <iostream>'s statics and a run of class
	registrations of their own (0x46ED60, 0x46F060 to 0x46F2C0), and they
	call only one another.

===============================================================================
*/

#ifndef __MODELMERGE_H__
#define __MODELMERGE_H__

#include "NiTList.h"

class NiAVObject;
class NiNode;
class NiTriShape;

// Gives every triangle shape under object geometry data of its own, copied
// from the data it shares.
void			ModelMerge_CopyGeometry( NiAVObject *object );

// The merged model of root, which is taken out of its parent; the new node
// takes its place there.
NiAVObject *	ModelMerge_Merge( NiNode *root );

// True when both objects have a texture property and the two show the same
// image (by file name when both images have one).
bool			ModelMerge_HaveSameTexture( NiAVObject *object1, NiAVObject *object2 );

// Appends the textured triangle shapes under object to the list.
void			ModelMerge_FindShapes( NiAVObject *object, NiTList<NiTriShape *> *shapes );

// Removes the property of the type from object and everything under it.
void			ModelMerge_RemoveProperty( NiAVObject *object, int type );

#endif /* !__MODELMERGE_H__ */
