/*
===============================================================================

	EffectAction.h

	A timed action that plays an effect controller: Play starts the
	controller and runs for the action's duration, the controller's
	lifetime when the action has none; completing or skipping the action
	stops the controller, and the action's end detaches the effect.

	0x30 bytes, allocated from the pools. Evidence: Create (0x4DE610)
	allocates 0x30 bytes and calls the constructor (0x4DE640), which calls
	TGTimedAction's, stores the vtable 0x88D204 (TGTimedAction's 24 slots,
	overriding the destructor, Play, Skip and Completed) and takes the
	controller (0x2C) and the controller's target (0x28); the destructor
	(0x4DE710) detaches the controller's effect.

===============================================================================
*/

#ifndef __EFFECTACTION_H__
#define __EFFECTACTION_H__

#include "TGTimedAction.h"
#include "NiObjectNET.h"

class EffectController;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class EffectAction : public TGTimedAction {
public:
					EffectAction( EffectController *controller_ );
	virtual			~EffectAction( void );

	// TGAction
	virtual void	Play( void );
	virtual void	Skip( void );
	virtual void	Completed( void );

	static EffectAction *	Create( EffectController *controller );

protected:
	NiPointer<NiObjectNET>	target;			// 0x28 the controller's target, held while the effect plays
	EffectController *		controller;		// 0x2C
};

#endif /* !__EFFECTACTION_H__ */
