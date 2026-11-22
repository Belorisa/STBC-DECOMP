/*
===============================================================================

	ScriptObject.h

	An event handler run by a Python script module: the game, its episodes
	and their missions. The object calls the module's functions of the
	stage names (PreLoadAssets, Initialize, Load, OnIdle, Unload,
	Terminate) with its shadow instance, hands out event types of its own
	to the script, and keeps the names of the modules the script wants
	kept loaded.

	0x38 bytes. Evidence: the constructor (0x409D20) calls
	TGEventHandlerObject's, stores the vtable 0x8888D4 (28 slots), copies
	the script name to 0x14 and the first event type to 0x1C; the stream
	constructor (0x409E90) clears 0x14-0x34. GetObjType returns 0x80CD;
	the class strings are "ScriptObject", "_p_ScriptObject" and
	"ScriptObjectPtr". The SWIG wrappers name slots 21-27 (PreLoadAssets,
	Initialize, Load, OnIdle, Unload, Terminate, AddPersistentModule) and
	the inline accessors GetScript 0x14, GetInstanceNextEventType and
	SetEventCount 0x1C, SetAssetsLoaded and AreAssetsLoaded 0x28,
	GetDatabase 0x34. SetOnIdleFunc writes 0x18; Initialize and Terminate
	hold their events at 0x24 and 0x20.

	The inline stage calls (PreLoadAssets, Load, Unload at 0x4040D0 to
	0x4040F0, OnIdle at 0x408210) are the copies of other units.

===============================================================================
*/

#ifndef __SCRIPTOBJECT_H__
#define __SCRIPTOBJECT_H__

#include "TGEventHandlerObject.h"
#include "TGLocalizationDatabase.h"

class TGSound;

// GetObjType's value for a ScriptObject.
#define SCRIPTOBJECT_TYPE		0x80CD

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class ScriptObject : public TGEventHandlerObject {
public:
					ScriptObject( char *script, int firstEventType );
					ScriptObject( long id );
	virtual			~ScriptObject( void );

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

	// The stages of the script's life, each a call of the module function
	// of that name.
	// FUNCTION: STBC 0x4040D0 ?PreLoadAssets@ScriptObject@@UAEXXZ
	virtual void	PreLoadAssets( void ) { CallScriptFunction( "PreLoadAssets" ); }
	virtual void	Initialize( TGEvent *event );
	// FUNCTION: STBC 0x4040E0 ?Load@ScriptObject@@UAEXXZ
	virtual void	Load( void ) { CallScriptFunction( "Load" ); }
	// FUNCTION: STBC 0x408210 ?OnIdle@ScriptObject@@UAEXXZ
	virtual void	OnIdle( void ) { if ( onIdleFunc != NULL ) { CallScriptFunction( "OnIdle" ); } }
	// FUNCTION: STBC 0x4040F0 ?Unload@ScriptObject@@UAEXXZ
	virtual void	Unload( void ) { CallScriptFunction( "Unload" ); }
	virtual void	Terminate( TGEvent *event );
	virtual void	AddPersistentModule( char *module );

	// A new sound in the script's group; NULL without a file.
	TGSound *		LoadSound( char *fileName, char *name, long flags );
	// The sound of the name's file in the database (ScriptObject_LoadDatabaseSound).
	TGSound *		LoadDatabaseSound( TGLocalizationDatabase *db, char *name, long flags ) { return LoadSound( db->GetFilename( name ), name, flags ); }

	void			SetOnIdleFunc( char *function );
	TGLocalizationDatabase *	SetDatabase( char *name );
	TGLocalizationDatabase *	GetDatabase( void ) { return database; }

	char *			GetScript( void ) { return script; }
	int				GetInstanceNextEventType( void ) { return ++eventCount; }
	void			SetEventCount( int count ) { eventCount = count; }
	void			SetAssetsLoaded( bool loaded ) { assetsLoaded = loaded; }
	bool			AreAssetsLoaded( void ) { return assetsLoaded; }

protected:
	// Calls the script module's function with the object's shadow
	// instance; a Python error is printed (invented name).
	void			CallScriptFunction( char *function );
	// Makes the modules loaded now the persistent ones (invented name).
	void			RecordPersistentModules( void );

	char *			script;					// 0x14 the module name, a heap copy
	char *			onIdleFunc;				// 0x18 a heap copy; OnIdle calls the module only with one set
	int				eventCount;				// 0x1C the last event type handed out
	TGEvent *		terminateEvent;			// 0x20 a reference held, sent when the script has terminated (invented name)
	TGEvent *		initializeEvent;		// 0x24 a reference held, sent when the script has initialized (invented name)
	bool			assetsLoaded;			// 0x28
	int				numPersistentModules;	// 0x2C (invented name)
	char **			persistentModules;		// 0x30 heap copies of the module names (invented name)
	TGLocalizationDatabase *	database;	// 0x34 loaded through g_kLocalizationManager

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
ScriptObject::IsTypeOf

Derived classes inline it into their own IsTypeOf.
================
*/
// FUNCTION: STBC 0x00409de0 @game/ScriptObject
inline bool ScriptObject::IsTypeOf( int type ) {
	if ( type == SCRIPTOBJECT_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

#endif /* !__SCRIPTOBJECT_H__ */
