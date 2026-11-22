/*
===============================================================================

	CharacterSpeakingQueue.h

	A character's queue of lines to speak: three lists of queued sound
	actions, one per kind of line, and the action being spoken.

	0x60 bytes, an event handler object. Evidence: CharacterClass's
	constructor (0x666F10) inlines the constructor: TGEventHandlerObject's
	constructor, the lists at 0x14, 0x2C and 0x44 (two items per block),
	the vtable 0x88B1B8 (21 slots: no new virtual functions) and the action
	at 0x5C. The class factory's unit keeps the inline virtual functions
	(0x441AF0-0x441BB0): GetObjType returns 0x8017, GetClassName
	"CharacterSpeakingQueue", the handler table is 0x991E48; the destructor
	(0x441BB0) empties the queue (0x6723E0) before the lists go.
	AddSoundToQueue (0x672530) files a 0xC-byte record in the list of its
	kind (0, 1, else 2). The speaking count 0x991F34 rises when a line
	starts (0x672360) and falls when it ends (0x672720).

===============================================================================
*/

#ifndef __CHARACTERSPEAKINGQUEUE_H__
#define __CHARACTERSPEAKINGQUEUE_H__

#include "TGEventHandlerObject.h"
#include "NiTList.h"

class TGAction;
class TGEvent;

// GetObjType's value for a CharacterSpeakingQueue.
#define CHARACTERSPEAKINGQUEUE_TYPE		0x8017

/*
================
QueuedSound

A line waiting in the queue (0x672530 allocates and fills it). The name is
invented.
================
*/
struct QueuedSound {
	TGAction *		action;			// 0x00 the sound action
	int				type;			// 0x04 the line's priority, App's CSP_MISSION_CRITICAL (0), CSP_NORMAL (1) or
									//      CSP_SPONTANEOUS (2): the list it waits in
	int				subPriority;	// 0x08 CharacterAction's subPriority, kept and saved (name invented)
};

// FUNCTION: STBC 0x00441b80 ??_GCharacterSpeakingQueue@@UAEPAXI@Z @game/UtopiaModule
class CharacterSpeakingQueue : public TGEventHandlerObject {
public:
					CharacterSpeakingQueue( void ) : TGEventHandlerObject( 0 ), missionCriticalLines( 2 ), normalLines( 2 ), spontaneousLines( 2 ) { speakingAction = NULL; }
	// The loader's (UtopiaModule's creator): the stream fills the speaking action.
	// FUNCTION: STBC 0x00441a90 ??0CharacterSpeakingQueue@@QAE@J@Z @game/UtopiaModule
					CharacterSpeakingQueue( long id ) : TGEventHandlerObject( id ), missionCriticalLines( 2 ), normalLines( 2 ), spontaneousLines( 2 ) {}
	// FUNCTION: STBC 0x00441bb0 ??1CharacterSpeakingQueue@@UAE@XZ @game/UtopiaModule
	virtual			~CharacterSpeakingQueue( void ) { ClearQueue(); }

	// TGObject
	// FUNCTION: STBC 0x00441af0 ?GetObjType@CharacterSpeakingQueue@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return CHARACTERSPEAKINGQUEUE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x00441b40 ?GetClassName@CharacterSpeakingQueue@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "CharacterSpeakingQueue"; }
	// FUNCTION: STBC 0x00441b50 ?GetSwigTypeName@CharacterSpeakingQueue@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_CharacterSpeakingQueue"; }
	// FUNCTION: STBC 0x00441b60 ?GetPythonClassName@CharacterSpeakingQueue@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "CharacterSpeakingQueuePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00441b70 ?GetClassHandlers@CharacterSpeakingQueue@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// No line is being spoken and one is queued (invented name).
	bool			IsReadyToSpeak( void );
	// A line is being spoken (invented name).
	int				IsSpeaking( void );
	// Queues the sound action (invented name).
	void			AddSoundToQueue( TGAction *action, int type, int subPriority );
	// Empties the lists (invented name).
	void			ClearQueue( void );
	// Plays the next queued line when none is being spoken (invented name).
	bool			SpeakNextLine( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	// The number of queues speaking a line (invented name).
	static int		numSpeaking;

protected:
	// The queued lines by priority, owned; the first list with a line is
	// spoken first, and a new line drops the spontaneous ones (names
	// invented: CharacterAction passes its priority, which the scripts give
	// as App's CSP_MISSION_CRITICAL, CSP_NORMAL and CSP_SPONTANEOUS, 0 to 2).
	NiTList<QueuedSound *>	missionCriticalLines;	// 0x14 priority 0
	NiTList<QueuedSound *>	normalLines;			// 0x2C priority 1
	NiTList<QueuedSound *>	spontaneousLines;		// 0x44 any other priority
	TGAction *		speakingAction;			// 0x5C the line being spoken (invented name)

	// The event a line sends when spoken, and the next queued line's action
	// (invented names).
	TGEvent *		CreateSoundDoneEvent( void );
	TGAction *		GetNextSound( void );
	// ET_CHARACTER_SOUND_DONE: the next line starts.
	void			SoundDoneHandler( TGEvent *event );

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
CharacterSpeakingQueue::IsTypeOf
================
*/
// FUNCTION: STBC 0x00441b00 ?IsTypeOf@CharacterSpeakingQueue@@UAE_NH@Z @game/UtopiaModule
inline bool CharacterSpeakingQueue::IsTypeOf( int type ) {
	if ( type == CHARACTERSPEAKINGQUEUE_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

#endif /* !__CHARACTERSPEAKINGQUEUE_H__ */
