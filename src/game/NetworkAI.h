/*
===============================================================================

	NetworkAI.h

	The AI a multiplayer game gives each object another player controls:
	it carries the object along the last state the network reported (the
	position, the direction of travel, the up vector and the speed) and
	steers it toward where that state says it should be, until the next
	state update arrives.

	0x58 bytes: MultiplayerGame (0x69F7EC) allocates that much and names
	the AI "Network". The class name is invented after that name; no
	string names the class.

	Evidence: the constructor (0x47DAB0) and the destructor (0x47DE40)
	store the vtable 0x88BD04 (ArtificialIntelligence's 21 slots, with
	the destructor, GetAllAIsInTree, GetFocusAIs, Update, WriteToStream,
	GetAIType and VirtualSlot20 replaced). GetAIType returns 9; Cast
	(0x47DE50), which the state update readers PhysicsObjectClass::
	VirtualSlot73 and ShipClass::VirtualSlot73 call on the object's AI,
	checks it, and the readers then write the reported state into the
	members at 0x2C-0x54. The type has no create function filed: nothing
	refers to Create (0x47E630).

===============================================================================
*/

#ifndef __NETWORKAI_H__
#define __NETWORKAI_H__

#include "ArtificialIntelligence.h"
#include "NiPoint3.h"

class NetworkAI : public ArtificialIntelligence {
public:
					NetworkAI( PhysicsObjectClass *object, char *name_ );
					NetworkAI( TGStream *stream, bool *ok );
	virtual			~NetworkAI( void );

	// ArtificialIntelligence
	virtual void	GetAllAIsInTree( NiTList<ArtificialIntelligence *> &ais );
	virtual void	GetFocusAIs( NiTList<ArtificialIntelligence *> &ais );
	virtual int		Update( double time );
	virtual bool	WriteToStream( TGStream *stream );
	virtual int		GetAIType( void );
	virtual bool	VirtualSlot20( int type );

	// The AI as a NetworkAI; NULL when it is another kind or NULL.
	static NetworkAI *	Cast( ArtificialIntelligence *ai );
	// The stream constructor (no create function is filed for it).
	static ArtificialIntelligence *	Create( TGStream *stream );

protected:
	// Moves the object toward the reported position: part of the way,
	// scaled by the elapsed time (at most one second's worth). Invented name.
	void			PullTowardPosition( PhysicsObjectClass *object, float elapsed );
	// Turns the object to face the target, keeping the reported up vector.
	// Invented name.
	void			TurnTowardTarget( PhysicsObjectClass *object, NiPoint3 target );

public:
	// The reported state; the state update readers write it directly.
	float			lastUpdateTime;		// 0x28 game time of the last Update (invented name)
	float			stateTime;			// 0x2C game time of the reported state (invented name)
	NiPoint3		position;			// 0x30 (invented name)
	NiPoint3		direction;			// 0x3C unit direction of travel (invented name)
	NiPoint3		up;					// 0x48 (invented name)
	float			speed;				// 0x54 along direction; negative when backing (invented name)
};

#endif /* !__NETWORKAI_H__ */
