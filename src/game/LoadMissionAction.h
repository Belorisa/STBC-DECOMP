/*
===============================================================================

	LoadMissionAction.h

	An action that loads a mission into the current episode: Play has the
	episode load the mission module, and the action completes when the
	episode reports it loaded. Completing sends the episode
	ET_MISSION_START.

	0x34 bytes, allocated from the pools.

	Evidence: the constructor (0x403460, which the SWIG wrapper
	LoadMissionAction_Create calls with its string) expands
	TGScriptAction's and stores the vtable 0x8886D8, which overrides only
	the destructor and Play (0x403540); the name goes in TGScriptAction's
	module, copied. The SWIG wrappers GetGame and GetEpisode read 0x2C and
	0x30, which no code in the binary writes.

===============================================================================
*/

#ifndef __LOADMISSIONACTION_H__
#define __LOADMISSIONACTION_H__

#include "TGScriptAction.h"

class Episode;
class Game;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00403500 ??_GLoadMissionAction@@UAEPAXI@Z @game/LoadMissionAction
// FUNCTION: STBC 0x00403530 ??1LoadMissionAction@@UAE@XZ @game/LoadMissionAction
class LoadMissionAction : public TGScriptAction {
public:
					LoadMissionAction( char *missionName );

	static LoadMissionAction *	Create( char *missionName ) { return new LoadMissionAction( missionName ); }

	// TGAction
	virtual void	Play( void );

	Game *			GetGame( void ) { return game; }
	Episode *		GetEpisode( void ) { return episode; }

protected:
	// BUG: never set: the constructor leaves both uninitialised, and
	// Play uses the current game's episode instead.
	Game *			game;				// 0x2C
	Episode *		episode;			// 0x30
};

#endif /* !__LOADMISSIONACTION_H__ */
