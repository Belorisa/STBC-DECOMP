/*
===============================================================================

	TGActionManager.h

	The handler of the action events and the registry of live actions:
	every TGAction adds itself in its constructor and removes itself in its
	destructor, so FindAction tells whether a pointer kept by a sequence
	still names an action. A script can file an action under a name
	(RegisterAction) and later skip every action of that name
	(KillActions); the name "Game" stands for every named action.
	g_kTGActionManager (0x99B010) is the one instance (the SWIG variable of
	that name).

	0x20 bytes. Evidence: the constructors (0x6FE550, 0x6FCF20) call
	TGEventHandlerObject's, clear the record array at 0x14 (items, count,
	capacity) and store the vtable 0x8963A0 (TGEventHandlerObject's 21
	slots); GetObjType returns 0x702. The records are 8 bytes, the action
	and its name, kept in ascending order of the action pointer: FindAction
	(0x6FF040) and RemoveAction (0x6FF170) binary-search them and AddAction
	(0x6FF090) inserts with the doubling and memmove of SortedArray. The
	destructor (0x6FD010) deletes the records and their names.

	Names: TGActionManager, SkipEvents, RegisterAction and KillActions are
	the SWIG interface's; the handlers are named by the strings they are
	registered under ("TGActionManager::ActionCompletedHandler" and so
	on); "Shutting down Action Manager, deleting all valid actions" names
	DeleteAllActions. The other names are invented.

===============================================================================
*/

#ifndef __TGACTIONMANAGER_H__
#define __TGACTIONMANAGER_H__

#include <string.h>

#include "TGEventHandlerObject.h"
#include "SortedArray.h"

class TGAction;
class TGAnimNode;
class TGEvent;
class TGSequence;
class TGStream;

// GetObjType's value for a TGActionManager.
#define TGACTIONMANAGER_TYPE			0x702

// The events the manager handles: an action's end and skip (the App
// constants of those names), and a sequence's start of an action, now or
// after a delay (TGSequence; invented names).
#define ET_ACTION_COMPLETED				0x50001
#define ET_ACTION_SKIP					0x50002
#define ET_PLAY_SEQUENCE_ACTION			0x50003
#define ET_PLAY_SEQUENCE_ACTION_DELAYED	0x50005

// The name a registered action gets when the script gives none; killing
// it kills every registered action.
#define ACTION_NAME_ALL					"Game"

// The markers around each action in a saved game.
#define ACTION_STREAM_START				0x15A7		// invented name
#define ACTION_STREAM_END				0x15B3		// invented name

// GetObjType's value for a TGSoundAction (its IsTypeOf at 0x6FD520):
// sounds are not restarted after a load.
#define TGSOUNDACTION_TYPE				0x70C

/*
================
TGActionRecord

A live action and the name a script registered it under, if any. The
records sort by the action pointer. The name is invented.
================
*/
class TGActionRecord {			// invented name
public:
					TGActionRecord( TGAction *action_ ) { name = NULL; action = action_; }
					~TGActionRecord( void ) { if ( name != NULL ) { delete [] name; } }

	// SortedArray's ordering: by the action's address.
	int				CompareKey( TGAction *key ) { return (int)action - (int)key; }
	int				Compare( TGActionRecord *other ) { return (int)action - (int)other->action; }

	// Replaces the name with a copy of newName.
	void			SetName( const char *newName ) {
						if ( name != NULL ) {
							delete [] name;
						}
						name = NULL;
						if ( newName != NULL ) {
							name = new char[strlen( newName ) + 1];
							strcpy( name, newName );
						}
					}

	TGAction *		action;			// 0x00
	char *			name;			// 0x04 a heap copy; NULL until the action is registered
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006fcfe0 ??_GTGActionManager@@UAEPAXI@Z
class TGActionManager : public TGEventHandlerObject {
public:
	// FUNCTION: STBC 0x006fe550 ??0TGActionManager@@QAE@XZ
					TGActionManager( void ) {}
	// FUNCTION: STBC 0x006fcf20 ??0TGActionManager@@QAE@J@Z
					TGActionManager( long id ) : TGEventHandlerObject( id ) {}
	// FUNCTION: STBC 0x006fd010 ??1TGActionManager@@UAE@XZ
	virtual			~TGActionManager( void ) {}

	// TGObject
	// FUNCTION: STBC 0x006fcf50 ?GetObjType@TGActionManager@@UAEHXZ
	virtual int		GetObjType( void ) { return TGACTIONMANAGER_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x006fcfa0 ?GetClassName@TGActionManager@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGActionManager"; }
	// FUNCTION: STBC 0x006fcfb0 ?GetSwigTypeName@TGActionManager@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGActionManager"; }
	// FUNCTION: STBC 0x006fcfc0 ?GetPythonClassName@TGActionManager@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGActionManagerPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x006fcfd0 ?GetClassHandlers@TGActionManager@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// The scripts' entry points: they act on g_kTGActionManager.
	static void		SkipEvents( void );
	static void		RegisterAction( TGAction *action, char *name = NULL );
	static void		KillActions( char *name = NULL );

	// Skips every skippable action that is playing.
	void			SkipPlayingActions( void );								// invented name
	// Files the action under the name; ACTION_NAME_ALL when there is none.
	void			SetActionName( TGAction *action, char *name );			// invented name
	// Skips every action registered under the name, or every registered
	// one for ACTION_NAME_ALL, without its completion events.
	void			KillNamedActions( char *name );							// invented name

	// The index of the action's record; -1 when the action is not alive.
	int				FindAction( TGAction *action );							// invented name
	// Every action registers in its constructor and unregisters in its
	// destructor.
	void			AddAction( TGAction *action );							// invented name
	void			RemoveAction( TGAction *action );						// invented name

	// Deletes every live action, without sending its completion events.
	void			DeleteAllActions( void );								// invented name
	void			Unknown6FEBA0( void );
	// Ends every action that does not survive a global abort, without its
	// completion events.
	void			AbortAllActions( void );								// invented name
	// Skips every action that plays on the node (its VirtualSlot21).
	void			SkipActionsFor( TGAnimNode *node );						// invented name
	// After a load: restarts the playing actions (sounds excepted) from the
	// times they were saved with.
	void			RestartLoadedActions( void );							// invented name
	// Before a load: forgets the records without deleting them.
	void			ForgetAllActions( void );								// invented name

	static void		RegisterHandlerNames( void );							// invented name
	static void		RegisterHandlers( void );								// invented name

	void			ActionCompletedHandler( TGEvent *event );
	void			ActionSkipHandler( TGEvent *event );
	void			PlaySequenceActionHandler( TGEvent *event );
	void			PlaySequenceActionDelayedHandler( TGEvent *event );

protected:
	SortedArray<TGActionRecord, TGAction *>	actions;	// 0x14 the live actions, by address

	static TGEventHandlerTable	classHandlers;		// invented name
};

extern TGActionManager	g_kTGActionManager;

/*
================
TGActionManager::IsTypeOf
================
*/
// FUNCTION: STBC 0x006fcf60 ?IsTypeOf@TGActionManager@@UAE_NH@Z
inline bool TGActionManager::IsTypeOf( int type ) {
	if ( type == TGACTIONMANAGER_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

#endif /* !__TGACTIONMANAGER_H__ */
