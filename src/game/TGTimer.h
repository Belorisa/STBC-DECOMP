/*
===============================================================================

	TGTimer.h

	Sends an event after a delay, and again every delay until its duration
	has passed (TGTimerManager drives it). A timer holds a reference to its
	event. Timers are registered by ID like objects, in a table of their
	own (GetTGTimerPtr).

	0x20 bytes, allocated from the pools (TGObject's TGPOOL_ALLOCATION).

	Evidence: the constructor (0x6DCF50) calls TGObject's and stores the
	vtable 0x8960C4 (12 slots); GetObjType returns 0x103. The member names
	are those of the SWIG accessors TGTimer_GetTimerStart, SetTimerStart,
	GetDelay, SetDelay, GetDuration, SetDuration and GetEvent, whose inline
	bodies read and write these offsets.

===============================================================================
*/

#ifndef __TGTIMER_H__
#define __TGTIMER_H__

#include "TGObject.h"

class TGEvent;

// GetObjType's value for a TGTimer.
#define TGTIMER_TYPE			0x103

// A duration that never runs out (-1). A variable, not a constant: its
// users load it from memory.
extern float	TIMER_FOREVER;			// invented name

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006dd090 ??_GTGTimer@@UAEPAXI@Z @game/TGTimer
class TGTimer : public TGObject {
public:
					TGTimer( long id = 0 );
	virtual			~TGTimer( void );

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

	static TGTimer *	Create( void ) { return new TGTimer; }

	float			GetNextTime( void ) { return nextTime; }		// invented name
	float			GetTimerStart( void ) { return timerStart; }
	void			SetTimerStart( float start ) { timerStart = start; nextTime = start; }
	float			GetDelay( void ) { return delay; }
	void			SetDelay( float delay ) { this->delay = delay; }
	float			GetDuration( void ) { return duration; }
	void			SetDuration( float duration ) { this->duration = duration; }
	TGEvent *		GetEvent( void ) { return event; }
	// The next timer in the manager's list (invented name). TGTimerManager::Update
	// clears it through this setter: retail keeps that store ahead of the
	// duration's load, which an inline setter's own `this` gives and a direct
	// store doesn't (docs/c2-backend.md, "Scheduling"); the schedule is the only
	// evidence.
	void			SetNext( TGTimer *next_ ) { next = next_; }
	void			SetEvent( TGEvent *event );
	void			ReleaseEvent( void );						// invented name

	bool			Update( float time );						// invented name

	static TGTimer *	GetTGTimerPtr( long id );

	static bool		SaveTimers( TGStream *stream );				// invented name
	static bool		LoadTimers( TGStream *stream );				// invented name
	static void		ReleaseTimerTable( void );					// invented name
	static void		RemoveAllTimers( void );					// invented name
	static void		DeleteAllTimers( void );					// invented name

protected:
	static bool		AddTimer( TGTimer *timer );					// invented name
	static bool		RemoveTimer( TGTimer *timer );				// invented name

	float			nextTime;				// 0x08 when the event is sent next
	float			delay;					// 0x0C between sends
	float			duration;				// 0x10 from timerStart; TIMER_FOREVER for no end
	float			timerStart;				// 0x14
	TGEvent *		event;					// 0x18 a reference held; an ID between ReadFromStream and PostFixUp
	TGTimer *		next;					// 0x1C the next timer due in the manager's list; an ID between ReadFromStream and PostFixUp

	friend class TGTimerList;
	friend class TGTimerManager;

	// Every live timer, by ID.
	static NiTMap<long, TGTimer *> *	timerTable;
	static bool		keepTimerTable;			// cleared at shutdown: the table is deleted once empty
};

#endif /* !__TGTIMER_H__ */
