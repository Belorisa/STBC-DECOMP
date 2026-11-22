/*
===============================================================================

	TGAction.h

	Something that plays over time and reports when it is done: a line of
	dialogue, a camera move, a script function, a sequence of other actions.
	Play starts it, Completed (or Skip, or Abort) ends it; at the end the
	action sends the events added with AddCompletedEvent, tells the sequence
	it belongs to, and deletes itself.

	0x20 bytes. Every action registers itself with g_kTGActionManager in
	its constructor and unregisters in its destructor; the manager's list is
	how a sequence checks that an action is still alive.

	Evidence: the constructor (0x6FE5A0) calls TGObject's, stores the vtable
	0x8966B8 (24 slots: TGObject's 12 and 12 of its own), registers the
	action and clears the members (the start time to -1). GetObjType returns
	0x701, the type TGAction::Cast (0x6FE500) checks. The SWIG wrappers
	TGAction_* call slots 12 to 20 (IsPlaying, ConstructDescription,
	SetupEditControls, ExtractControlValues, Play, Restart, Skip, Abort,
	Completed) and name the inline accessors; slots 21 to 23 and the
	members' names are invented from their use.

===============================================================================
*/

#ifndef __TGACTION_H__
#define __TGACTION_H__

#include "TGObject.h"

class TGAnimNode;
class TGEvent;
class TGSequence;
class TGString;

// GetObjType's value for a TGAction.
#define TGACTION_TYPE			0x701

// startTime of an action that is not playing.
#define ACTION_NOT_PLAYING		-1.0f

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006fe680 ??_GTGAction@@UAEPAXI@Z
class TGAction : public TGObject {
public:
					TGAction( long id = 0 );
	virtual			~TGAction( void );

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

	// FUNCTION: STBC 0x00402770 ?IsPlaying@TGAction@@UAE_NXZ
	virtual bool	IsPlaying( void ) { return startTime != ACTION_NOT_PLAYING; }
	// The editor's hooks: no action in this release does anything with them.
	// FUNCTION: STBC 0x00402790 ?ConstructDescription@TGAction@@UAEXPAVTGString@@0@Z
	virtual void	ConstructDescription( TGString *, TGString * ) {}
	// FUNCTION: STBC 0x004027a0 ?SetupEditControls@TGAction@@UAEHPAD@Z
	virtual int		SetupEditControls( char * ) { return 0; }
	// FUNCTION: STBC 0x004027b0 ?ExtractControlValues@TGAction@@UAEHPAD@Z
	virtual int		ExtractControlValues( char * ) { return 0; }
	virtual void	Play( void );
	// Plays as if started delay seconds ago.
	virtual void	Restart( float delay = 0.0f );
	virtual void	Skip( void );
	virtual void	Abort( void );
	virtual void	Completed( void );
	// Whether the action plays on the node; TGActionManager::SkipActionsFor
	// skips the actions that do (name invented: every override compares the
	// node with the one it animates, and SkipActionsFor is its one caller).
	// FUNCTION: STBC 0x004027c0 ?PlaysOn@TGAction@@UAE_NPAVTGAnimNode@@@Z
	virtual bool	PlaysOn( TGAnimNode * ) { return false; }
	virtual void	SetSequence( TGSequence *sequence_ );					// invented name
	// Releases the completion events without sending them.
	virtual void	ReleaseCompletedEvents( void );							// invented name

	static TGAction *	Cast( TGObject * );
	void			AddCompletedEvent( TGEvent *event );
	bool			IsPartOfSequence( TGSequence *sequence );
	TGSequence *	GetSequence( void ) { return sequence; }
	bool			IsSkippable( void ) { return skippable; }
	void			SetSkippable( bool skippable_ ) { skippable = skippable_; }
	void			SetUseRealTime( bool useRealTime );
	bool			IsUseRealTime( void ) { return useRealTime; }
	void			SetSurviveGlobalAbort( bool survive ) { surviveGlobalAbort = survive; }
	bool			IsGlobalAbortSurvivor( void ) { return surviveGlobalAbort; }
	float			GetStartTime( void ) { return startTime; }				// invented name

protected:
	// Sends the completion events and releases them.
	void			SendCompletedEvents( void );							// invented name

	int				numCompletedEvents : 15;	// 0x08
	bool			skippable : 1;				// 0x0C TGActionManager::SkipEvents skips the action
	TGEvent **		completedEvents;			// 0x10 sent when the action ends; IDs between ReadFromStream and FixUpPointers
	TGSequence *	sequence;					// 0x14 the sequence that plays the action, if any
	float			startTime;					// 0x18 the clock when Play started it; ACTION_NOT_PLAYING
	bool			useRealTime;				// 0x1C real time rather than game time
	bool			surviveGlobalAbort;			// 0x1D
};

/*
================
TGAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fe630
inline bool TGAction::IsTypeOf( int type ) {
	if ( type == TGACTION_TYPE ) {
		return true;
	}
	return TGObject::IsTypeOf( type );
}

#endif /* !__TGACTION_H__ */
