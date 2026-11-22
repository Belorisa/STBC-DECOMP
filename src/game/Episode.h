/*
===============================================================================

	Episode.h

	The script object of an episode: it loads its missions one at a time,
	passes the stages on to the current one, and while idle picks the bridge
	character under the middle of the view for its tool tip. The wrapper
	Episode_GetCurrentMission reads the mission (0x3C) inline.

	0x44 bytes, allocated from the pools. The constructor (0x4040A0) calls
	ScriptObject's with the first event type 0x808001, stores the vtable
	0x888738 (28 slots: the destructor, GetObjType, IsTypeOf, the stream
	functions, the class strings, GetClassHandlers, OnIdle, Terminate and
	AddPersistentModule overridden) and clears 0x38-0x40; GetObjType returns
	0x80CF; the class strings are "Episode", "_p_Episode" and "EpisodePtr";
	Game::LoadEpisode makes the episodes. The stream functions log "Saving
	%s with init event ID %d" with the event at 0x40, which LoadMission
	holds a reference to.

===============================================================================
*/

#ifndef __EPISODE_H__
#define __EPISODE_H__

#include "ScriptObject.h"

class Mission;
class TGEvent;
class TGStream;

// GetObjType's value for an Episode.
#define EPISODE_TYPE				0x80CF

// Event types (the SWIG constants of the App module).
#define ET_LOAD_MISSION				0x80000D	// the episode loads a mission
#define ET_REPORT_GOAL_INFO			0x800100	// the goal report the episode answers
#define ET_MISSION_START			0x800015	// a loaded mission starts

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x004041a0 ??_GEpisode@@UAEPAXI@Z
// FUNCTION: STBC 0x004041d0 ??1Episode@@UAE@XZ
class Episode : public ScriptObject {
public:
					Episode( char *script );
	// FUNCTION: STBC 0x0043d8b0 ??0Episode@@QAE@J@Z @game/UtopiaModule
					Episode( long id ) : ScriptObject( id ) {}	// the loader's: the stream fills the members

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

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ScriptObject
	virtual void	OnIdle( void );
	virtual void	Terminate( TGEvent *event );
	virtual void	AddPersistentModule( char *module );

	static int	GetNextEventType( void );

	// Register the class's handler methods by name, and add them to its
	// table (invented names; Game's registration calls them).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );
	void	LoadMissionHandler( TGEvent *event );		// 0x4047B0
	void	ReportGoalInfoHandler( TGEvent *event );	// 0x404B60
	// Loads the mission module; the event is sent when it has loaded
	// (Episode_LoadMission's arguments).
	void	LoadMission( char *name, TGEvent *doneEvent );
	void	RegisterGoal( char * );
	void	RemoveGoal( char * );

	Mission *		GetCurrentMission( void ) { return currentMission; }

protected:
	float			toolTipPickTime;	// 0x38 the game time of OnIdle's last tool tip pick (invented name)
	Mission *		currentMission;		// 0x3C the mission being played
	TGEvent *		missionLoadedEvent;	// 0x40 a reference held: LoadMission's event, passed to the mission's Initialize (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__EPISODE_H__ */
