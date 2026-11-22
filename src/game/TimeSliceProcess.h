/*
===============================================================================

	TimeSliceProcess.h

	Work done a little each frame: a process is filed under its priority,
	and each frame the processes that are due run in turn until the frame's
	time is used up.

	0x30 bytes. Evidence: the constructor 0x46F380 stores the vtable
	0x88BB14 (2 slots: Update, which the SWIG wrapper TimeSliceProcess_Update
	calls through slot 0, and the scalar deleting destructor 0x46F3C0). It
	sets the priority (0x08) to 2, clears the delay (0x10) and the byte at
	0x18, sets 0x19, fills the double at 0x20 with -1.0e20 and copies the
	system wrapper's update number to 0x28, then files the process in the
	array of its priority (four arrays at 0x981490); nothing is stored at
	0x04 or 0x0C: the doubles align the members after the vtable pointer
	to 8. The class, SetPriority and the delay accessors are named by the
	SWIG wrappers TimeSliceProcess_*; the other names are invented.

===============================================================================
*/

#ifndef __TIMESLICEPROCESS_H__
#define __TIMESLICEPROCESS_H__

#include "NiTArray.h"

// The frames the frame-time average is taken over (invented name).
#define TSP_NUM_FRAME_TIMES		16

// FUNCTION: STBC 0x0046f3c0 ??_GTimeSliceProcess@@UAEPAXI@Z
class TimeSliceProcess {
public:
	// The priorities (the SWIG constants TimeSliceProcess_UNSTOPPABLE to
	// _NUM_PRIORITIES): the unstoppable processes run every frame, the
	// others share the time left.
	enum {
		UNSTOPPABLE,
		CRITICAL,
		NORMAL,
		LOW,
		NUM_PRIORITIES
	};

			TimeSliceProcess( void );
	bool	GetDelayUsesGameTime( void ) { return delayUsesGameTime; }
	void	SetDelayUsesGameTime( bool delayUsesGameTime_ ) { delayUsesGameTime = delayUsesGameTime_; }
	int	GetPriority( void ) { return priority; }
	virtual void	Update( double time );	// the SWIG wrapper passes a double ("Od")
	virtual			~TimeSliceProcess();

	void	SetPriority( int );
	void	SetDelay( double delay_ ) { delay = delay_; }
	double	GetDelay( void ) { return delay; }

	// Runs the frame's share of the processes (invented name).
	static void	UpdateProcesses( void );
	// Empties the arrays without deleting the processes (invented name).
	static void	RemoveAllProcesses( void );

protected:
	// True when the delay has passed since the last update; never in the
	// frame the process was made in (invented name).
	bool	IsReady( void );
	// The process's array, by the priority of process (invented names;
	// both are called on the process itself).
	void	AddToArray( TimeSliceProcess *process );
	void	RemoveFromArray( TimeSliceProcess *process );
	// One round of a priority's processes that are due: each one not yet
	// run this round, until the time left runs out; when all of them had
	// their turn the round starts again (invented name).
	static void	UpdatePriority( int priority_, double *timeLeft, double endTime );

	int				priority;				// 0x08 (TimeSliceProcess_GetPriority)
	double			delay;					// 0x10 (TimeSliceProcess_Set/GetDelay)
	bool			updated;				// 0x18 set by Update: the process had its turn this round (invented name)
	bool			delayUsesGameTime;		// 0x19 (TimeSliceProcess_Set/GetDelayUsesGameTime)
	double			lastUpdateTime;			// 0x20 -1.0e20 after construction (invented name)
	int				createdUpdate;			// 0x28 the update number the process was made in (invented name)

	// The processes by priority (invented names).
	static NiTArray<TimeSliceProcess *>	processes[NUM_PRIORITIES];
	// The time into the frame at which the last frames' updates started, the
	// slot the next frame writes and the count of frames, which picks the
	// priority that goes first.
	static double	frameTimes[TSP_NUM_FRAME_TIMES];
	static int		frameTimeIndex;
	static int		frameCount;
};

#endif /* !__TIMESLICEPROCESS_H__ */
