/*
===============================================================================

	TGAnimBlender.h

	The controller that plays the animation manager's animations on a
	model: it keeps a track (TGAnimTrack) for each node under its target
	that has a keyframe or visibility controller, has the manager start or
	blend an animation into those tracks, steps them each update and calls
	back when all of them have run out.

	0x60 bytes, from the game's pools. Evidence: the NiRTTI at 0x997970 is
	built with the name "TGAnimBlender" (0x95ABCC) and NiTimeController's
	NiRTTI as its base (0x6C6330); CreateClone (0x6C6590) allocates 0x60
	bytes and calls the constructor (0x6C6400), which runs
	NiTimeController's, clears the array at 0x30, stores the vtable 0x895B04
	(22 slots) and resets the rest (0x6C6460). The vtable overrides GetRTTI,
	CreateClone, the destructor (0x6C64B0), Start, Stop, Update, SetTarget
	and TargetIsRequiredType (an NiAVObject); slot 20 starts it at the game
	time and slot 21 returns NiAVObject's NiRTTI. The manager's
	UseAnimation, BlendAnimation, UseDefault and PositionUsingAnimation take
	the blender. The names of the members are invented, except the blend
	time (TGAnimNode_GetBlendTime and _SetBlendTime).

===============================================================================
*/

#ifndef __TGANIMBLENDER_H__
#define __TGANIMBLENDER_H__

#include "NiTimeController.h"
#include "SortedArray.h"
#include "TGAnimNode.h"
#include "TGAnimTrack.h"
#include "TGPoolManager.h"

class NiAVObject;
class NiKeyframeController;
class NiNode;
class NiVisController;


// The first keyframe or visibility controller of the object's list; NULL
// when it has none (ModelHelpers.cpp; TODO identity: their unit is not known,
// the blender, TGAnimNode and the animation manager's entries call them).
// The names are invented.
NiKeyframeController *	FindKeyframeController( NiObjectNET *object );
NiVisController *		FindVisController( NiObjectNET *object );

// FUNCTION: STBC 0x006c73e0 ??_GTGAnimBlender@@UAEPAXI@Z
class TGAnimBlender : public NiTimeController {
public:
	TGPOOL_ALLOCATION

	// FUNCTION: STBC 0x006c73d0 ?GetRTTI@TGAnimBlender@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual					~TGAnimBlender();

	virtual void			Start( float time );
	virtual void			Stop();
	virtual void			Update( float time );
	virtual void			SetTarget( NiObjectNET *target_ );
	virtual bool			TargetIsRequiredType() const;

	// Starts the blender at the game time.
	virtual void			VirtualSlot20();
	// NiAVObject's NiRTTI: the type the target must have.
	virtual const NiRTTI *	VirtualSlot21();

							TGAnimBlender();

	TGAnimTrack **			GetTracks( void );
	int						GetTrackCount( void );
	void					SetPlaying( bool playing_ );
	void					SetCallback( TGAnimCallbackPtr callback_, long id_ );
	void					SetAnimationName( char *name );
	void					SetBlendTime( float time );
	float					GetBlendTime( void ) { return blendTime; }
	float					GetUnknown48( void ) { return unknown48; }
	bool					IsAnimating( void );

	// Have the animation manager play the animation of the name on the
	// tracks: from the start time to the end time, or blended in over the
	// blend time.
	void					UseAnimation( char *animation, bool flag, float start, float end );
	void					SetExclusiveAnimation( char *animation, float time, bool flag );
	void					SetNonExclusiveAnimation( char *animation, float time, bool flag );
	void					SetExclusiveAnimationUseDefault( char *animation, float time, bool flag );
	int						BlendAnimation( char *animation, float time, float blendTime_, bool flag, bool flag2, bool flag3 );
	int						PositionUsingAnimation( char *animation );
	void					Unknown6C70C0( char *animation );
	void					Unknown6C7110( char *animation );
	int						UseDefault( char *animation );

	void					StopTrack( int index );
	void					StopTrackInOthers( int index );
	void					ClearAnimations( void );

	// the tracks of the node and of the nodes under it
	void					AddNode( NiAVObject *object );
	void					RemoveNode( NiAVObject *object );

	static const NiRTTI		rtti;

protected:
	friend class TGAnimNode;

	void					Init( void );
	void					CopyMembers( TGAnimBlender *dest );
	void					CopyFrom( TGAnimBlender *source );
	void					UpdateTracks( float time );

	// Calls the callback once, if there is one.
	void					DoCallback( void ) {
								if ( callback != NULL ) {
									if ( id != 0 ) {
										callback( id );
									}
									callback = NULL;
								}
							}

	SortedArray<TGAnimTrack, const char *>	tracks;	// 0x30 owned, by node name
	NiNode *				rootNode;				// 0x3C the target's root node, when the target is a TGAnimNode
	float					blendTime;				// 0x40 seconds
	float					startTime;				// 0x44 the game time playing last changed
	float					unknown48;				// 0x48 seconds; TGAnimNode removes an idle blender older than this
	bool					playing;				// 0x4C
	char *					animationName;			// 0x50 owned copy; what Start plays
	// how Start plays the animation: blended in, alongside the others or
	// not, taking the default; set by TGAnimNode
	bool					blend;					// 0x54
	bool					unknown55;				// 0x55 the flag Start passes on
	bool					exclusive;				// 0x56
	bool					useDefault;				// 0x57
	TGAnimCallbackPtr		callback;				// 0x58
	long					id;						// 0x5C the callback's argument
};

#endif /* !__TGANIMBLENDER_H__ */
