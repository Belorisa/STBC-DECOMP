/*
===============================================================================

	TGTimerManager.h

	The members of TGTimerManager that the SWIG wrappers TGTimerManager_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __TGTIMERMANAGER_H__
#define __TGTIMERMANAGER_H__

class TGTimer;
class TGEventManager;
class TGStream;
class TGObject;
class TGEventHandlerObject;

// The timers of a manager, in ascending order of the time they are due next,
// linked through TGTimer's next member (invented name). The manager's
// wrappers forward to it: AddTimer (0x6DC3F0) to 0x6DF000, RemoveTimer(timer)
// to 0x6DF150, RemoveTimer(ID) to 0x6DF0E0 and DeleteTimer to 0x6DF170.
class TGTimerList {
public:
				TGTimerList( void );
				~TGTimerList( void );
	bool	AddTimer( TGTimer *timer );				// before the first timer due later; true
	bool	RemoveTimer( long id );					// unlinks the timer of the ID; false when there is none
	bool	RemoveTimer( TGTimer *timer );			// by its ID; false for NULL
	bool	DeleteTimer( long id );					// removes the timer of the ID and deletes it
	bool	DeleteTimer( TGTimer *timer );			// removes the timer and deletes it; false for NULL
	TGTimer *	GetHead( void ) { return head; }	// invented name
	TGTimer *	RemoveHead( void );					// unlinks the timer due first and returns it, NULL for none (invented name)
	void		DeleteHead( void );					// removes the timer due first and deletes it (invented name)
	void		DeleteAll( void );					// removes and deletes every timer (invented name)
	bool		WriteToStream( TGStream *stream );
	bool		ReadFromStream( TGStream *stream );
	bool		FixUpPointers( void );
	void		PostFixUp( void );
	// Deletes the timers whose event goes to the destination, of the type when
	// it is not 0; true when there was one (invented name).
	bool		DeleteTimersForDestination( TGEventHandlerObject *destination, int eventType );
	// Clears the source of the timers' events from the object; true when one had it (invented name).
	bool		ClearSource( TGObject *source );
	bool		IsTimerInList( TGTimer *timer );			// invented name
	// The links agree with the count (invented name).
	bool		IsValid( void );
	// Forgets the timers without deleting them (invented name).
	void		Unknown6DEFC0( void );

protected:
	TGTimer *	head;			// 0x00 the timer due first
	TGTimer *	tail;			// 0x04 the timer due last
	int			count;			// 0x08
};

class TGTimerManager {
public:
	TGTimerManager( void );
	// Deletes the list of timers; only the exit-time destructors of the two
	// managers call it (0x437F30).
	~TGTimerManager( void );
	bool	AddTimer( TGTimer * );
	bool	RemoveTimer( long );
	bool	RemoveTimer( TGTimer *timer );		// invented overload; TGParagraph::LostTrueFocus passes its cursor timer
	bool	DeleteTimer( long );
	bool	DeleteTimer( TGTimer *timer );
	// Deletes the timers whose event goes to the destination, of the type
	// when it is not 0; false before the manager is set up (invented name;
	// WeaponsDisplay's StopWatchingShip expands it).
	bool	DeleteTimersForDestination( TGEventHandlerObject *destination, int eventType ) {
		if ( timers != NULL ) {
			return timers->DeleteTimersForDestination( destination, eventType );
		}
		return false;
	}
	// Sends the events of the timers due by the time, and sets up the
	// repeating ones again (invented name; UtopiaModule's frame passes the
	// game time to g_kTimerManager and the real time to the other).
	bool	Update( float time );
	// Moves every timer by the time passed since the last update, which becomes
	// the time: the real-time timers after a load (invented name).
	void	RebaseTime( float time );
	// Deletes every timer (invented name).
	void	DeleteAllTimers( void );
	// Forgets the time of the last update (invented name).
	void	ResetTime( void );
	bool	WriteToStream( TGStream *stream );
	bool	ReadFromStream( TGStream *stream );
	bool	FixUpPointers( void );
	void	PostFixUp( void );
	// Forgets the timers without deleting them, before a saved game is
	// loaded (invented name, after TGEventManager::ForgetAll).
	void	ForgetAll( void );

protected:
	TGTimerList *	timers;			// 0x00 NULL until the manager is set up
	float			unknown04;		// 0x04 the time of the last update
	TGEventManager *	eventManager;	// 0x08 the manager the timers' events go to (invented name; the SWIG setter g_kTimerManager_set copies 12 bytes)
};

extern TGTimerManager	g_kTimerManager;
// The timers of real time, not game time.
extern TGTimerManager	g_kRealtimeTimerManager;

#endif /* !__TGTIMERMANAGER_H__ */
