/*
===============================================================================

	FloatRangeWatcher.h

	Watches a float variable and sends an event when it crosses one of the
	values registered with AddRangeCheck.

	0xC bytes. Evidence: the constructor (0x44C1B0) clears the watched
	variable's pointer at 0x00, allocates the 0x1C-byte head of the range
	check list into 0x04 and sets the next range check ID at 0x08 to
	RID_INVALID + 1; the destructor is 0x44C220. ShipSubsystem points its
	two watchers at its condition percentages. RID_INVALID is named by the
	SWIG variable FloatRangeWatcher_RID_INVALID (string 0x956F48).

===============================================================================
*/

#ifndef __FLOATRANGEWATCHER_H__
#define __FLOATRANGEWATCHER_H__

#include "TGFloatEvent.h"

class TGStream;

/*
===============================================================================

	FloatRangeCheck

	A node of the watcher's list: the interval of values from low to high
	over which the event and mode apply. The list covers 0 to 1 from the
	start as a single node; AddRangeCheck splits an interval at its value.
	The class and its names are invented (no string names it).

	0x1C bytes. Evidence: the constructor (0x44C290) sets low to 0, high to
	1, the event and mode to 0, the ID to RID_INVALID and the links to NULL;
	the destructor (0x44C2C0) deletes the event.

===============================================================================
*/

class FloatRangeCheck {		// invented name
public:
					FloatRangeCheck( void );
					~FloatRangeCheck( void );

	float				low;			// 0x00 the interval starts here
	float				high;			// 0x04 and ends here
	TGFloatEvent *		event;			// 0x08 owned; sent when the variable crosses into the interval
	int					mode;			// 0x0C FRW_*: which crossings send the event
	int					id;				// 0x10 what AddRangeCheck returned
	FloatRangeCheck *	previous;		// 0x14 the interval below
	FloatRangeCheck *	next;			// 0x18 the interval above
};

class FloatRangeWatcher {
public:
					FloatRangeWatcher( void );
					~FloatRangeWatcher( void );

	int				AddRangeCheck( float, int, TGFloatEvent * );
	void			RemoveRangeCheck( int );

	// Compares the variable with the range checks and sends the events of those it crossed (invented name).
	void			CheckRanges( void );

	// The TGObject stream protocol, called by the owner's own overrides (names after the slots that call them).
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );
	bool			FixUpPointers( void );
	void			PostFixUp( void );

	void			SetVariable( float *variable_ ) { variable = variable_; }	// invented name
	float			GetWatchedVariable( void ) { return *variable; }

	// Which crossings of its value a range check reports: the SWIG constants
	// FloatRangeWatcher_FRW_* (the constant table gives NONE 0 and ABOVE 1;
	// ShipSubsystem's checks for falling below a value pass 2).
	enum {
		FRW_NONE,
		FRW_ABOVE,
		FRW_BELOW,
		FRW_BOTH
	};

	// The type of a range check's ID.
	typedef int		RangeID;

	// The ID no range check has.
	static RangeID		RID_INVALID;

private:
	// Sends a copy of the event carrying the value (0x44C520; invented name).
	void			SendEvent( float value, TGFloatEvent *event );

private:
	float *			variable;			// 0x00 the watched variable (invented name)
	FloatRangeCheck *	current;		// 0x04 the interval the variable is in (invented name)
	int				nextID;				// 0x08 the ID the next range check takes (invented name)
};

#endif /* !__FLOATRANGEWATCHER_H__ */
