/*
===============================================================================

	TGSoundAction.h

	An action that plays a sound from the sound manager and completes when
	the sound ends. The sound can be put in a region, attached to a node,
	given a priority, and played with the TGSAF_* flags.

	0x44 bytes, allocated from the pools.

	Evidence: the constructor (0x702FA0) calls TGAction's, stores the
	vtable 0x896534, copies the sound's name and, when given, the region's,
	and keeps the flags (TGSAF_ALLOW_SOUND_TO_CONTINUE_PLAYING_WHEN_STOPPED
	is dropped from a looping sound), the volume and the invalid sound ID;
	the destructor (0x7030C0) frees the names. The load's constructor
	(0x6FD4F0) and the creator (0x6FD4C0, type 0x70C) are the actions
	module's. Play (0x703140) sends the action manager the action's
	completion event when the sound ends. The SWIG wrappers name Create,
	Play, Restart, Completed, Skip, SetPriority and SetNode; the TGSAF_*
	constants are the App module's (0x652950 on).

===============================================================================
*/

#ifndef __TGSOUNDACTION_H__
#define __TGSOUNDACTION_H__

#include "TGAction.h"
#include "NiNode.h"
#include "NiPointer.h"

// GetObjType's value for a TGSoundAction.
#define TGSOUNDACTION_TYPE		0x70C

// How the sound plays (the SWIG constants of the App module).
#define TGSAF_DEFAULTS			0x03		// TGSAF_RESTART | TGSAF_ALLOW_DUPLICATES
#define TGSAF_RESTART			0x01		// the sound starts over if it is playing
#define TGSAF_ALLOW_DUPLICATES	0x02		// other sounds of the name are not stopped first
#define TGSAF_LOOPING			0x04
#define TGSAF_ALLOW_SOUND_TO_CONTINUE_PLAYING_WHEN_STOPPED	0x08
#define TGSAF_VOICE				0x10

// FUNCTION: STBC 0x006fd580 ??_GTGSoundAction@@UAEPAXI@Z @game/TGActionModule
class TGSoundAction : public TGAction {
public:
					TGSoundAction( char *sound_, int flags_, char *region_, float volume_ );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006fd4f0 ??0TGSoundAction@@QAE@J@Z @game/TGActionModule
					TGSoundAction( long id ) : TGAction( id ) {}
	virtual			~TGSoundAction( void );

	// TGObject
	// FUNCTION: STBC 0x006fd510 ?GetObjType@TGSoundAction@@UAEHXZ @game/TGActionModule
	virtual int		GetObjType( void ) { return TGSOUNDACTION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x006fd550 ?GetClassName@TGSoundAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetClassName( void ) { return "TGSoundAction"; }
	// FUNCTION: STBC 0x006fd560 ?GetSwigTypeName@TGSoundAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGSoundAction"; }
	// FUNCTION: STBC 0x006fd570 ?GetPythonClassName@TGSoundAction@@UAEPADXZ @game/TGActionModule
	virtual char *	GetPythonClassName( void ) { return "TGSoundActionPtr"; }

	// TGAction
	virtual void	Play( void );
	virtual void	Restart( float delay = 0.0f );
	virtual void	Skip( void );
	virtual void	Completed( void );
	virtual void	ReleaseCompletedEvents( void );

	void			SetPriority( float priority_ ) { usePriority = true; priority = priority_; }
	void			SetNode( NiNode *node_ ) { node = node_; }

protected:
	char *			sound;					// 0x20 a copy of the sound's name
	char *			region;					// 0x24 a copy of the region's name, or NULL
	int				flags;					// 0x28 TGSAF_*
	float			priority;				// 0x2C 0.5 until SetPriority
	float			volume;					// 0x30
	NiPointer<NiNode>	node;				// 0x34 the node the sound is attached to, or NULL
	long			savedNode;				// 0x38 the node's address when saved, until PostFixUp (invented name)
	long			playingID;				// 0x3C the sound playing, or the invalid ID (unknown896A9C)
	bool			usePriority;			// 0x40 set by SetPriority

	// Declared for the SWIG wrappers that call them.
public:
	static TGSoundAction *	Create( char *sound, int flags, char *region, float volume ) { return new TGSoundAction( sound, flags, region, volume ); }	// the SWIG wrapper TGSoundAction_Create expands it
};

/*
================
TGSoundAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fd520 @game/TGActionModule
inline bool TGSoundAction::IsTypeOf( int type ) {
	if ( type == TGSOUNDACTION_TYPE ) {
		return true;
	}
	return TGAction::IsTypeOf( type );
}

#endif /* !__TGSOUNDACTION_H__ */
