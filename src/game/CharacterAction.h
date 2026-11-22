/*
===============================================================================

	CharacterAction.h

	A TGAction that makes a character do something: move, turn, speak a line,
	play an animation and the like, by action type.

	Evidence: the constructor 0x66FB90 stores the vtable 0x88B20C, which
	overrides TGAction's destructor (0x66FCD0, through the scalar deleting
	destructor 0x441DB0), type slots, stream pair (0x6710D0, 0x6714A0),
	class names, Play (0x66FE50) and Completed (0x66FE40). The type slots and class names (0x441D40
	to 0x441DA0) are inline copies kept beside the class factories.

	The constructor (0x66FB90) takes the character, the action type, two
	strings it copies, and the database it falls back to the character's
	(0x148); CharacterClass's MoveTo, TurnTowards and line helpers build
	their sequences from it. The action types are the App module's
	CharacterAction_AT_* constants, in the order of its constant table.

===============================================================================
*/

#ifndef __CHARACTERACTION_H__
#define __CHARACTERACTION_H__

#include "TGAction.h"

class CharacterClass;
class TGLocalizationDatabase;
class TGObject;

// GetObjType's value for a CharacterAction (the type its Cast checks).
#define CHARACTERACTION_TYPE		0x8018

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00441db0 ??_GCharacterAction@@UAEPAXI@Z @game/CharacterAction
class CharacterAction : public TGAction {
public:
	// The action types.
	enum {
		AT_SET_LOCATION,
		AT_SET_LOCATION_NAME,
		AT_MOVE,
		AT_TURN,
		AT_TURN_NOW,
		AT_TURN_BACK,
		AT_TURN_BACK_NOW,
		AT_DEFAULT,
		AT_BREATHE,
		AT_FORCE_BREATHE,
		AT_SPEAK_LINE,
		AT_SPEAK_LINE_NO_FLAP_LIPS,
		AT_SAY_LINE,
		AT_SAY_LINE_AFTER_TURN,
		AT_PLAY_ANIMATION,
		AT_PLAY_ANIMATION_FILE,
		AT_LOOK_AT_ME,
		AT_LOOK_AT_ME_NOW,
		AT_WATCH_ME,
		AT_STOP_WATCHING_ME,
		AT_MENU_UP,
		AT_MENU_DOWN,
		AT_SET_AUDIO_MODE,
		AT_ENABLE_RANDOM_ANIMATIONS,
		AT_DISABLE_RANDOM_ANIMATIONS,
		AT_GLANCE_AT,
		AT_GLANCE_AWAY,
		AT_BECOME_ACTIVE,
		AT_BECOME_INACTIVE,
		AT_ENABLE_MENU,
		AT_DISABLE_MENU,
		AT_ENABLE_INITIATIVE,
		AT_DISABLE_INITIATIVE,
		AT_SET_STATUS
	};

	// the loader's: the stream sets the rest (UtopiaModule::CreateCharacterAction)
	// FUNCTION: STBC 0x00441d20 ??0CharacterAction@@QAE@J@Z @game/UtopiaModule
					CharacterAction( long id ) : TGAction( id ) {}
	// The meanings of the last two parameters are open.
					CharacterAction( CharacterClass *character, int type, char *detail, char *detail2, int unknown,
									 TGLocalizationDatabase *database, int unknown2, int unknown3 );
	virtual			~CharacterAction( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGAction
	virtual void	Play( void );
	virtual void	Completed( void );

	char *	GetDetail( void ) { return detail; }
	void	SetSubPriority( int subPriority_ ) { subPriority = subPriority_; }
	int	GetActionType( void ) { return type; }
	void	SetPriority( int priority_ ) { priority = priority_; }

	static CharacterAction *	Cast( TGObject * );
	static void	ConvertFolderLWVs( char * );
	static void	SetSkipAllLines( bool skip ) { skipAllLines = skip; }
	// The SWIG wrapper CharacterAction_Create expands it.
	static CharacterAction *	Create( CharacterClass *character, int type, char *detail = NULL, char *detail2 = NULL, int unknown = 0,
										TGLocalizationDatabase *database = NULL, int unknown2 = 0, int unknown3 = 0 ) {
									return new CharacterAction( character, type, detail, detail2, unknown, database, unknown2, unknown3 );
								}
	// The same for the character of the name in the named set, found when
	// the action runs (the SWIG wrapper CharacterAction_CreateByName expands it).
	static CharacterAction *	CreateByName( char *name, char *set, int type, char *detail = NULL, char *detail2 = NULL, int unknown = 0,
											  TGLocalizationDatabase *database = NULL, int unknown2 = 0, int unknown3 = 0 ) {
									CharacterAction *action = new CharacterAction( NULL, type, detail, detail2, unknown, database, unknown2, unknown3 );
									action->UseNameAndSetInsteadOfObject( name, set );
									return action;
								}
	void	UseNameAndSetInsteadOfObject( char *, char * );

protected:
	// Set by SetSkipAllLines; no code of the retail binary reads it.
	static bool		skipAllLines;

	long			characterID;		// 0x20 the character's object ID; 0 without one (invented name)
	int				type;				// 0x24 AT_* (invented name)
	TGLocalizationDatabase *	database;	// 0x28 the given one, else the character's (invented name)
	char *			detail;				// 0x2C owned copy; NULL without one (invented name)
	char *			detail2;			// 0x30 owned copy; NULL without one (invented name)
	int				unknown34;			// 0x34
	int				priority;			// 0x38
	int				subPriority;			// 0x3C
	char *			objectName;			// 0x40 owned copy; NULL at first (invented name, from UseNameAndSetInsteadOfObject)
	char *			setName;			// 0x44 owned copy; NULL at first (invented name, from UseNameAndSetInsteadOfObject)
};

/*
================
CharacterAction::GetObjType
================
*/
// FUNCTION: STBC 0x00441d40 @game/CharacterAction
inline int CharacterAction::GetObjType( void ) {
	return CHARACTERACTION_TYPE;
}

/*
================
CharacterAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x00441d50 @game/CharacterAction
inline bool CharacterAction::IsTypeOf( int type ) {
	if ( type == CHARACTERACTION_TYPE ) {
		return true;
	}
	return TGAction::IsTypeOf( type );
}

/*
================
CharacterAction::GetClassName
================
*/
// FUNCTION: STBC 0x00441d80 @game/CharacterAction
inline char *CharacterAction::GetClassName( void ) {
	return "CharacterAction";
}

/*
================
CharacterAction::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00441d90 @game/CharacterAction
inline char *CharacterAction::GetSwigTypeName( void ) {
	return "_p_CharacterAction";
}

/*
================
CharacterAction::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00441da0 @game/CharacterAction
inline char *CharacterAction::GetPythonClassName( void ) {
	return "CharacterActionPtr";
}

#endif /* !__CHARACTERACTION_H__ */
