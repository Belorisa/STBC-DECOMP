/*
===============================================================================

	TGSequence.h

	An action made of actions: each one starts when the action it waits for
	(its prerequisite) completes, or when the sequence plays if it waits for
	none, after its own delay. The sequence completes when all of them have.

	0x34 bytes. A prerequisite's completion reaches the sequence through a
	TGSequenceEvent the sequence added to the prerequisite's completion
	events; delays run on timers of g_kTimerManager (or of
	g_kRealtimeTimerManager for an action that uses real time), whose IDs
	the sequence keeps so that it can kill them.

	Evidence: the constructor (0x7004C0) calls TGAction's, stores the vtable
	0x896718 (TGAction's 24 slots) and clears the members; GetObjType
	returns 0x707, the type TGSequence::Cast (0x700F10) checks. The SWIG
	wrappers TGSequence_* name AddAction, AppendAction, Play (slot 16),
	Skip (slot 18), GetNumActions and GetAction; the other names are
	invented from their use.

===============================================================================
*/

#ifndef __TGSEQUENCE_H__
#define __TGSEQUENCE_H__

#include "TGAction.h"

class TGObject;

// GetObjType's value for a TGSequence.
#define TGSEQUENCE_TYPE			0x707

// One action of a sequence. 0xC bytes.
struct TGSequenceEntry {				// invented name
	TGAction *		action;				// 0x00 NULL once it has completed; an ID between ReadFromStream and PostFixUp
	TGAction *		prerequisite;		// 0x04 the action it starts after, NULL to start with the sequence; likewise
	float			delay;				// 0x08 seconds after that
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00700560 ??_GTGSequence@@UAEPAXI@Z
class TGSequence : public TGAction {
public:
					TGSequence( long id = 0 );
	virtual			~TGSequence( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGAction
	virtual void	Play( void );
	virtual void	Skip( void );
	virtual void	Completed( void );

	static TGSequence *	Cast( TGObject * );
	// Adds the action to start delay seconds after prerequisite completes
	// (with the sequence for none). An action whose prerequisite is not in
	// the sequence is deleted.
	void			AddAction( TGAction *action, TGAction *prerequisite = NULL, float delay = 0.0f );
	// Adds the action to start after the last action added.
	void			AppendAction( TGAction *action, float delay = 0.0f );
	int				GetNumActions( void ) { return numActions; }
	TGAction *		GetAction( int index ) { return actions[index].action; }

	// One of the sequence's actions has completed; the sequence completes
	// with the last one.
	void			ActionCompleted( TGAction *action );			// invented name
	// Remembers a timer to kill when the sequence ends.
	void			AddTimerID( long id );							// invented name

protected:
	void			AddEntry( TGAction *action, TGAction *prerequisite, float delay );	// invented name
	// Deletes the sequence's timers.
	void			KillTimers( void );								// invented name

	int				numActions;				// 0x20
	TGSequenceEntry *	actions;			// 0x24 from the pools (realloc)
	bool			completing;				// 0x28 Completed is ending the actions: ActionCompleted does not complete the sequence again
	int				numTimers;				// 0x2C
	long *			timerIDs;				// 0x30 from the pools (realloc)

	// Declared for the SWIG wrappers that call them.
public:
	static TGSequence *	Create( void ) { return new TGSequence; }	// the SWIG wrapper TGSequence_Create expands it
};

/*
================
TGSequence::IsTypeOf
================
*/
// FUNCTION: STBC 0x00700500
inline bool TGSequence::IsTypeOf( int type ) {
	if ( type == TGSEQUENCE_TYPE ) {
		return true;
	}
	return TGAction::IsTypeOf( type );
}

#endif /* !__TGSEQUENCE_H__ */
