/*
===============================================================================

	TGLocalizationManager.h

	The localization databases that are loaded, each with a count of its
	users. There is one, the global g_kLocalizationManager.

	Evidence: the SWIG wrappers TGLocalizationManager_* name the members.
	The databases are kept in a list of entries (the list's count is at
	0x04, its first item at 0x08): GetIfRegistered (0x6D03F0) walks it
	comparing each database's file name, RegisterDatabase (0x6D0470) adds an
	entry of two members to its tail, Unload (0x6D04F0) takes a user from
	the entry of a database, DeleteAll (0x6D0520) clears every entry's users
	and Purge (0x6D0550) deletes the databases that have none. The entry
	and its names are invented. The constructor (0x6D03A0) builds the list
	(two items per block) and stores the vtable 0x895D58: two slots, the
	save (0x6D0670) and the load (0x6D06D0) of the entries.

===============================================================================
*/

#ifndef __TGLOCALIZATIONMANAGER_H__
#define __TGLOCALIZATIONMANAGER_H__

#include "NiTList.h"

class TGLocalizationDatabase;
class TGStream;

// A database the manager holds and how many users it has (invented name).
struct TGLocalizationEntry {
	TGLocalizationDatabase *	database;
	unsigned int				users;
};

// FUNCTION: STBC 0x006d0350 ??1TGLocalizationManager@@QAE@XZ @game/TGLocalizationManager
class TGLocalizationManager {
public:
					TGLocalizationManager( void );

	TGLocalizationDatabase *	Load( char *file, bool addUser );
	void	Unload( TGLocalizationDatabase *database );
	void	DeleteAll( void );
	void	Purge( void );
	TGLocalizationDatabase *	GetIfRegistered( char *file, bool addUser );
	void	RegisterDatabase( TGLocalizationDatabase *database );

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// Forgets the databases without deleting them, before a saved game is
	// loaded (invented name, after TGEventManager::ForgetAll).
	void	ForgetAll( void );

protected:
	NiTList<TGLocalizationEntry *>		databases;		// 0x04 the entries, owned
};

extern TGLocalizationManager	g_kLocalizationManager;

#endif /* !__TGLOCALIZATIONMANAGER_H__ */
