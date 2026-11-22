/*
===============================================================================

	TGTimedAction.h

	An action that completes by itself after its duration: Play starts a
	timer that sends the action manager the action's completion event. A
	negative duration completes at once.

	0x28 bytes, allocated from the pools.

	Evidence: the constructor (0x702220) calls TGAction's, stores the vtable
	0x8967E0 (TGAction's 24 slots), sets the duration to -1 and clears the
	timer; the destructor (0x702280) deletes the timer. GetObjType returns
	0x708 (the copies at 0x4DD180 accept TGAction's type too). The SWIG
	wrappers TGTimedAction_GetDuration and SetDuration read and write 0x20.
	The timer is the real-time manager's when the action uses real time,
	the game-time manager's otherwise.

===============================================================================
*/

#ifndef __TGTIMEDACTION_H__
#define __TGTIMEDACTION_H__

#include "TGAction.h"

// GetObjType's value for a TGTimedAction.
#define TGTIMEDACTION_TYPE		0x708

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class TGTimedAction : public TGAction {
public:
					TGTimedAction( long id = 0 );
	virtual			~TGTimedAction( void );

	// TGObject
	// FUNCTION: STBC 0x004dd180 ?GetObjType@TGTimedAction@@UAEHXZ @game/BridgeEffectAction
	virtual int		GetObjType( void ) { return TGTIMEDACTION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x004dd1c0 ?GetClassName@TGTimedAction@@UAEPADXZ @game/BridgeEffectAction
	virtual char *	GetClassName( void ) { return "TGTimedAction"; }
	// FUNCTION: STBC 0x004dd1d0 ?GetSwigTypeName@TGTimedAction@@UAEPADXZ @game/BridgeEffectAction
	virtual char *	GetSwigTypeName( void ) { return "_p_TGTimedAction"; }
	// FUNCTION: STBC 0x004dd1e0 ?GetPythonClassName@TGTimedAction@@UAEPADXZ @game/BridgeEffectAction
	virtual char *	GetPythonClassName( void ) { return "TGTimedActionPtr"; }

	// TGAction
	virtual void	Play( void );
	virtual void	Restart( float delay = 0.0f );
	virtual void	Skip( void );
	virtual void	Completed( void );

	float			GetDuration( void ) { return duration; }
	void			SetDuration( float duration_ ) { duration = duration_; }

protected:
	float			duration;				// 0x20 seconds; negative completes at once
	long			timerID;				// 0x24 the completion timer, 0 for none
};

/*
================
TGTimedAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x004dd190 ?IsTypeOf@TGTimedAction@@UAE_NH@Z @game/BridgeEffectAction
inline bool TGTimedAction::IsTypeOf( int type ) {
	if ( type == TGTIMEDACTION_TYPE ) {
		return true;
	}
	return TGAction::IsTypeOf( type );
}

#endif /* !__TGTIMEDACTION_H__ */
