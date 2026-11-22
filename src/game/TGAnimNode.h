/*
===============================================================================

	TGAnimNode.h

	A model's node that plays the animation manager's animations: it keeps
	blenders (TGAnimBlender) among its controllers, one per animation
	playing, and remembers its root node's rest transform and that of the
	root's parent, which an animation that moves the root folds into the
	parent.

	Its NiRTTI is at 0x9977B8: BaseObjectClass::GetAnimNode (0x419890)
	casts the object's NetImmerse object to a TGAnimNode by walking the
	NiRTTI chain up to it. The wrapper TGAnimNode_GetRootNode reads the
	root node (0xE8) inline; Copy (0x6C4BB0) finds the copy's root node by
	name and casts it to a NiNode.

	0x150 bytes, from the game's pools: CameraObjectClass::CreateFromNiCamera
	(0x42A120) allocates one from the pool manager and calls the
	constructor (0x6C4A70), which calls NiNode's, stores the vtable
	0x895A54 (NiNode's 44 slots; GetRTTI, CreateClone and the destructor
	overridden), sets the two rest transforms (0xEC, 0x11C) to identity and
	the blend time (0x14C) to 0.34. The SWIG wrappers TGAnimNode_* name the
	members; their Python signatures ("OOiff|OO", "OOfi|OO") give the
	parameters. The names of the rest transforms are invented.

===============================================================================
*/

#ifndef __TGANIMNODE_H__
#define __TGANIMNODE_H__

#include "NiNode.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "TGPoolManager.h"

class TGAnimBlender;

// The callback a blender calls with its ID when its animation has run out
// (the SWIG type TGAnimCallbackPtr).
typedef void ( *TGAnimCallbackPtr )( long id );

// The SWIG interface derives TGAnimNode from NiNode.
// FUNCTION: STBC 0x006c5a50 ??_GTGAnimNode@@UAEPAXI@Z
class TGAnimNode : public NiNode {
public:
	TGPOOL_ALLOCATION

	// FUNCTION: STBC 0x006c5a40 ?GetRTTI@TGAnimNode@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	virtual					~TGAnimNode();

							TGAnimNode( void );

	static const NiRTTI		rtti;

	// Play the animation of the name on a blender of the node's: from the
	// start time (UseAnimation), or blended in over the blend time,
	// stopping the other animations (SetExclusiveAnimation, which may
	// also take the default) or alongside them. The callback gets the ID
	// when the animation has run out.
	void	UseAnimation( char *animation, bool unknown, float start, float end, TGAnimCallbackPtr callback, long id );
	void	SetExclusiveAnimation( char *animation, float time, bool unknown, TGAnimCallbackPtr callback, long id );
	void	SetNonExclusiveAnimation( char *animation, float time, bool unknown, TGAnimCallbackPtr callback, long id );
	void	SetExclusiveAnimationUseDefault( char *animation, float time, bool unknown, TGAnimCallbackPtr callback, long id );

	void	StopNonExclusiveAnimation( char *animation );
	void	Stop( void );
	// Stops every blender of the animation (invented name).
	void	StopAnimation( char *animation );
	void	Copy( TGAnimNode *source );
	bool	IsAnimate( void );
	void	UseAnimationPosition( char *animation );

	// Gives the node a new vertex colour property and a new material
	// property (white, with no emittance) in place of its own; retail
	// 0x6C5520, no source yet. ShipClass::VirtualSlot66 calls it.
	void	Unknown6C5520( void );
	void	Unknown6C5680( void );

	NiNode *		GetRootNode( void ) { return rootNode; }
	// The node of the name under this one; NULL when there is none or it is no NiNode.
	NiNode *		FindNode( char *name ) { return NiDynamicCast( NiNode, GetObjectByName( name ) ); }
	// Makes the named node the root and keeps its rest transform and its
	// parent's.
	void			SetRootNode( char *name ) {
						rootNode = FindNode( name );
						if ( rootNode != NULL ) {
							rootNode->Update( 0.0f );
							rootTranslate = rootNode->GetTranslate();
							rootRotate = rootNode->GetRotate();
							parentTranslate = rootNode->GetParent()->GetTranslate();
							parentRotate = rootNode->GetParent()->GetRotate();
						}
					}
	float	GetBlendTime( void ) { return blendTime; }
	void	SetBlendTime( float blendTime_ ) { blendTime = blendTime_; }

protected:
	void	CopyMembers( TGAnimNode *dest );
	// A blender to play an animation on: the first of the node's that is
	// not playing, or the first at all when exclusive (the others are
	// stopped then); a new one when there is none.
	TGAnimBlender *	FindBlender( bool exclusive );
	void	Unknown6C5690( NiAVObject *object );

public:
	NiNode *		rootNode;			// 0xE8 the node whose transform places the animation
	NiPoint3		rootTranslate;		// 0xEC the root node's rest transform
	NiMatrix3		rootRotate;			// 0xF8
	NiPoint3		parentTranslate;	// 0x11C the rest transform of the root node's parent
	NiMatrix3		parentRotate;		// 0x128
	float			blendTime;			// 0x14C seconds; TGAnimAction::Play sets it before starting a blended animation
};

#endif /* !__TGANIMNODE_H__ */
