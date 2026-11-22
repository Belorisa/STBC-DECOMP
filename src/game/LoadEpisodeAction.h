/*
===============================================================================

	LoadEpisodeAction.h

	An action that loads an episode: Play has the game load the episode
	module, and the action completes when the game reports it loaded.

	0x2C bytes (TGScriptAction's), allocated from the pools.

	Evidence: the constructor (0x4027D0, which the SWIG wrapper
	LoadEpisodeAction_Create calls with its string) expands
	TGScriptAction's and stores the vtable 0x8885DC, which overrides only
	the destructor and Play (0x4028B0); the name goes in TGScriptAction's
	module, copied. Play hands Game::LoadEpisode an ET_ACTION_COMPLETED
	event for the action manager.

===============================================================================
*/

#ifndef __LOADEPISODEACTION_H__
#define __LOADEPISODEACTION_H__

#include "TGScriptAction.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00402870 ??_GLoadEpisodeAction@@UAEPAXI@Z @game/LoadEpisodeAction
// FUNCTION: STBC 0x004028a0 ??1LoadEpisodeAction@@UAE@XZ @game/LoadEpisodeAction
class LoadEpisodeAction : public TGScriptAction {
public:
					LoadEpisodeAction( char *episodeName );

	static LoadEpisodeAction *	Create( char *episodeName ) { return new LoadEpisodeAction( episodeName ); }

	// TGAction
	virtual void	Play( void );
};

#endif /* !__LOADEPISODEACTION_H__ */
