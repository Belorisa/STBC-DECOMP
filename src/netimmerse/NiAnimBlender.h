/*
===============================================================================

	NiAnimBlender.h

	A controller that holds several keyframe sequences for one node tree
	and plays one of them through the tree's keyframe controllers, blending
	or morphing from the sequence playing to the next.

	Evidence: the NiRTTI at 0x9A0220 is named "NiAnimBlender" with
	NiTimeController's as parent; Register (0x797660) registers CreateObject
	(0x79A630), which allocates 0xAC bytes. The vtable at 0x8982BC has
	NiTimeController's 20 slots and none added. The constructor (0x7977B0)
	and destructor (0x797880) give the layout: one array per node of the
	tree (the nodes and their keyframe controllers), arrays per sequence of
	per-node key arrays, the sequences' text keys, and four scratch key
	arrays per node for a blend. The "morph:" text keys (0x976068) mark the
	points a morph may leave or join a sequence.

	The member and method names are invented, after the Gamebryo lineage's
	sequence vocabulary (NiKeyframeManager); no string names them.

===============================================================================
*/

#ifndef __NIANIMBLENDER_H__
#define __NIANIMBLENDER_H__

#include "NiTimeController.h"
#include "NiKeyframeController.h"
#include "NiPosKey.h"
#include "NiRotKey.h"
#include "NiTextKey.h"

#include "NiAVObject.h"
#include "NiStreamMacros.h"

class NiNode;

/*
================
NiAnimBlenderLoadRecord

NiTimeController's link IDs, the root's, and one link ID array each for
the nodes and their controllers, which LinkObject frees. The record's
vtable is 0x898310. The class name is invented.
================
*/
// FUNCTION: STBC 0x0079b8a0 ??_GNiAnimBlenderLoadRecord@@UAEPAXI@Z @netimmerse/NiAnimBlender
class NiAnimBlenderLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiAnimBlenderLoadRecord() { rootLinkID = 0; nodeLinkIDs = 0; controllerLinkIDs = 0; }

	unsigned int			rootLinkID;
	unsigned int *			nodeLinkIDs;		// heap array of numNodes link IDs
	unsigned int *			controllerLinkIDs;	// heap array of numNodes link IDs
};

// FUNCTION: STBC 0x79B880 ??_GNiAnimBlender@@UAEPAXI@Z
class NiAnimBlender : public NiTimeController {
public:
	// FUNCTION: STBC 0x79B870 ?GetRTTI@NiAnimBlender@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiAnimBlender();

	virtual void			Update( float time );
	// FUNCTION: STBC 0x79B8F0 ?TargetIsRequiredType@NiAnimBlender@@UBE_NXZ
	// the target must be an NiAVObject
	virtual bool			TargetIsRequiredType() const {
								return NiIsKindOf( NiAVObject, target );
							}

							NiAnimBlender();

	// takes the tree whose nodes the sequences animate: one keyframe
	// controller per node, created where a node has none
	void					SetRoot( NiNode *root_ );

	// adds the keys of a tree shaped like the root's as a new sequence;
	// its index, or -1 when the tree's shape differs. copyKeys copies the
	// keys instead of sharing them.
	int						AddSequence( NiNode *sequence, CycleType cycleType, bool copyKeys );

	// strips the controllers of their keys and clamps them
	void					ClearSequence();

	// whether the last keys of one sequence meet the first of the next
	bool					SequencesConnect( int from, int to );

	// blends from the pose at startTime to the sequence's pose at the text
	// key (its start without one) over duration
	void					BlendToSequence( int sequence, const char *textKey, float startTime, float duration );

	// the time from time until the active sequence next reaches its text
	// key named name; sequence goes unused
	float					GetTimeToTextKey( int sequence, float time, const char *name );

	// leaves the sequence playing at its next "morph:" key for the
	// sequence's matching key, over duration
	void					MorphToSequence( int sequence, float time, float duration );

	// sets the tree to the sequence's pose at time
	void					ApplySequencePose( int sequence, float time );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiAnimBlender *dest );

	// the number of nodes in the tree under node, node included, added to
	// count
	static void				CountNodes( NiNode *node, unsigned int &count );

	// fills the node and controller arrays from index on with the tree
	// under node
	void					GatherNodes( NiNode *node, unsigned int &index );

	// fills one sequence's per-node key arrays from index on with the keys
	// of the tree under object, widening loTime and hiTime by the key
	// times; a node without keys gets one key of its current transform
	void					GatherKeys( NiNode *object, NiNode *node, unsigned int *numPos, NiPosKey::KeyType *posTypes,
										NiPosKey ***pos, unsigned int *numRot, NiRotKey::KeyType *rotTypes, NiRotKey ***rot,
										unsigned int &index, float &loTime, float &hiTime, bool copyKeys );

	// widens loTime or hiTime to take in time
	static void				WidenTimes( float time, float &loTime, float &hiTime ) {
								if ( time < loTime ) {
									loTime = time;
								} else if ( time > hiTime ) {
									hiTime = time;
								}
							}

	// the tree the sequences animate (name invented; AnimOnly.exe 0x4855D0)
	NiNode *				GetRoot() const { return root; }

	// hands the active sequence's keys to the controllers, or none
	void					ApplyActiveSequence();
	// a sequence to play ends any blend or morph
	void					SetActiveSequence( int sequence ) {
								assert( -1 <= sequence && sequence < (int)numSequences );	// NiAnimBlender.inl, AnimOnly.exe 0x485560
								if ( sequence >= 0 ) {
									blending = false;
									morphing = false;
								}
								if ( activeSequence != sequence ) {
									activeSequence = sequence;
									ApplyActiveSequence();
								}
							}

	// turns rot to the same side of the rotation sphere as reference and
	// clamps its w
	void					AlignRotation( NiRotKey *rot, NiRotKey *reference );

	// the sequence's pose at time into pos and rot, one key per node
	void					SamplePose( float time, int sequence, NiPosKey *pos, NiRotKey *rot );

	// the blend's destination pose: at the text key's time, or the first
	// keys
	void					SetBlendDestination( const char *textKey );

	// the tree's current pose into the blend's source keys
	void					CapturePose();

	void					UpdateBlend( float time );
	void					UpdateMorph( float time );

	// the "morph:" text keys after and before time, wrapping around unless
	// the sequence clamps; -1 or numKeys where there is none
	void					FindMorphKeys( float time, int &next, int &previous, NiTextKey *keys, unsigned int numKeys, CycleType cycleType );

	// the indices of the text keys named first and second, the second
	// searched from the first on, wrapping around unless the sequence
	// clamps; -1 where there is none
	void					FindTextKeys( const char *first, const char *second, int &secondIndex, int &firstIndex,
										  NiTextKey *keys, unsigned int numKeys, CycleType cycleType );

	NiNode *				root;				// the tree the sequences animate
	unsigned int			numNodes;			// m_uiNumNodes (AnimOnly.exe 0x47F64F's assert)
	NiNode **				nodes;				// numNodes, the tree's nodes in depth-first order
	NiKeyframeController **	controllers;		// numNodes, each node's keyframe controller
	unsigned int			numSequences;
	int						activeSequence;		// the sequence the controllers play, or -1

	// per sequence, per node: the keys and their count and type
	unsigned int **			numPosKeys;
	NiPosKey::KeyType **	posKeyTypes;
	NiPosKey ****			posKeys;
	unsigned int **			numRotKeys;
	NiRotKey::KeyType **	rotKeyTypes;
	NiRotKey ****			rotKeys;

	// per sequence
	float *					sequenceLengths;	// the keys' time span
	unsigned int *			numTextKeys;
	NiTextKey **			textKeys;			// owned, from the tree's NiTextKeyExtraData
	CycleType *				cycleTypes;

	// the blend
	int						blendSequence;		// the sequence it goes to
	float					blendDestinationTime;	// the time in that sequence it reaches
	float					blendStartTime;
	float					blendDuration;

	// the morph
	int						morphDestination;
	int						morphSource;
	float					unknown88;
	float					morphPhase;			// the destination's start time
	float					morphStartTime;
	float					morphDuration;

	// the blend's source and destination poses, one key per node
	NiPosKey *				sourcePos;
	NiRotKey *				sourceRot;
	NiPosKey *				destinationPos;
	NiRotKey *				destinationRot;

	bool					blending;
	bool					morphing;
};

NiRegisterStream( NiAnimBlender );

#endif /* !__NIANIMBLENDER_H__ */
