/*
===============================================================================

	NetFile.h

	The multiplayer script check: the host asks each joining player for
	checksums of its scripts and sends it the files that differ. The game
	module keeps one (UtopiaModule's member at 0x80, deleted by
	TerminateNetwork). The class name is the string's; the method and
	member names are invented.

	0x48 bytes, allocated from the pools. Evidence: the constructor
	(0x6A30C0) calls TGEventHandlerObject's with the ID, builds three
	NiTMaps of 0x25 buckets at 0x18, 0x28 and 0x38 (vtables 0x895648,
	0x895634 and 0x895620), stores the vtable 0x8955CC (TGEventHandlerObject's
	21 slots: the type getters, slots 4 and 5 returning true, the class
	strings "NetFile", "_p_NetFile" and "NetFilePtr" and the class handler
	table 0x9948F0) and adds the broadcast handler "NetFile ::
	ReceiveMessageHandler" for network messages; the scalar deleting
	destructor (0x6A3280) frees 0x48 bytes. The destructor deletes the
	messages of the map at 0x18 (through TGMessage's virtual destructor),
	the message lists of the map at 0x28 (0x6A63B0) and the file name lists
	of the map at 0x38. Each map is keyed by a player's net ID:
	0x6A35B0 files the copy of the message it sends a player in the first,
	0x6A6500 deletes a player's list in the second, 0x6A5860 sends a player
	the files named in its list in the third.

===============================================================================
*/

#ifndef __NETFILE_H__
#define __NETFILE_H__

#include "TGEventHandlerObject.h"
#include "TGPoolManager.h"
#include "NiTList.h"
#include "NiTMap.h"

class TGDirectory;
class TGMessage;
class TGMessageEvent;
class TGStream;

// GetObjType's value for a NetFile.
#define NETFILE_TYPE			0x80D6

// The App event types the script check uses (the SWIG constant table).
#define ET_CHECKSUM_COMPLETE			0x8000E6
#define ET_SYSTEM_CHECKSUM_FAILED		0x8000E7
#define ET_SYSTEM_CHECKSUM_COMPLETE		0x8000E8
#define ET_CANCEL_CONNECT				0x8000EB

// The first byte of the script check's messages (invented names; the
// values are ReceiveMessageHandler's cases and the senders' first bytes).
#define NETFILE_CHECKSUM_REQUEST_MESSAGE	0x20	// the host asks for a directory's checksums
#define NETFILE_CHECKSUM_MESSAGE			0x21	// a player's checksums
#define NETFILE_SYSTEM_CHECKSUM_FAIL_MESSAGE	0x22	// the player can't play: a system file differs
#define NETFILE_VERSION_DIFFERENT_MESSAGE	0x23	// the player can't play: another version
#define NETFILE_FILE_MESSAGE				0x25	// a file's name and contents
#define NETFILE_FILE_RECEIVED_MESSAGE		0x27	// the player asks for the next file
#define NETFILE_FILES_DONE_MESSAGE			0x28	// no files are left to send

// The boot reason the network takes when the script check fails (invented
// name; TGNetwork's bootReason_t stops at 5).
#define BOOT_SCRIPT_CHECK_FAILED		0x65

// A list of file names, each a heap string the list owns (invented name).
// Evidence: the destructor (0x6A3500, inline in NetFile::SendNextFile)
// deletes each name, then frees the items; NiTList's own destructor frees
// them again, out of line in SendNextFile (0x478480).
class NetFileNameList : public NiTList<char *> {
public:
					NetFileNameList( void ) : NiTList<char *>( 2 ) {}
					// FUNCTION: STBC 0x006a3500 ??1NetFileNameList@@QAE@XZ @game/NetFile
					// The loop body has no braces: a braced body is a scope, two more
					// tuples, and NetFile::ReceiveScriptChecksums (0x6A4560) then calls
					// this destructor where retail expands it.
					~NetFileNameList( void ) {
						NiTListIterator pos = GetHeadPos();
						while ( pos )
							delete GetNext( pos );
						RemoveAll();
					}
};

// Set by the "SkipChecksum" command-line switch (the application's
// command-line reader, 0x4389D3, defines it): the host sends no script
// files (invented name).
extern bool		skipChecksum;

// FUNCTION: STBC 0x006a3280 ??_GNetFile@@UAEPAXI@Z
class NetFile : public TGEventHandlerObject {
public:
	TGPOOL_ALLOCATION

					NetFile( long id = 0 );
	virtual			~NetFile( void );

	// TGObject
	// FUNCTION: STBC 0x006a31f0 ?GetObjType@NetFile@@UAEHXZ @game/NetFile
	virtual int		GetObjType( void ) { return NETFILE_TYPE; }
	// FUNCTION: STBC 0x006a3200 ?IsTypeOf@NetFile@@UAE_NH@Z @game/NetFile
	virtual bool	IsTypeOf( int type ) {
						if ( type == NETFILE_TYPE ) {
							return true;
						}
						return TGEventHandlerObject::IsTypeOf( type );
					}
	// FUNCTION: STBC 0x006a3590 ?WriteToStream@NetFile@@UAE_NPAVTGStream@@@Z @game/NetFile
	virtual bool	WriteToStream( TGStream *stream ) { return true; }
	// FUNCTION: STBC 0x006a35a0 ?ReadFromStream@NetFile@@UAE_NPAVTGStream@@@Z @game/NetFile
	virtual bool	ReadFromStream( TGStream *stream ) { return true; }
	// FUNCTION: STBC 0x006a3240 ?GetClassName@NetFile@@UAEPADXZ @game/NetFile
	virtual char *	GetClassName( void ) { return "NetFile"; }
	// FUNCTION: STBC 0x006a3250 ?GetSwigTypeName@NetFile@@UAEPADXZ @game/NetFile
	virtual char *	GetSwigTypeName( void ) { return "_p_NetFile"; }
	// FUNCTION: STBC 0x006a3260 ?GetPythonClassName@NetFile@@UAEPADXZ @game/NetFile
	virtual char *	GetPythonClassName( void ) { return "NetFilePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x006a3270 ?GetClassHandlers@NetFile@@UAEPAVTGEventHandlerTable@@XZ @game/NetFile
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );

	void			ReceiveMessageHandler( TGMessageEvent *event );

	// Starts the script check of a player that joins: false when there is
	// no network, this isn't the host, or a check of the player is already
	// under way (invented name).
	bool			RequestChecksums( int playerID );

	// Asks a player for the checksums of one directory, outside the
	// script check's list (index -1); false when there is no network,
	// this isn't the host, or the player's request is still unanswered
	// (invented name).
	bool			RequestSystemChecksum( int playerID, char *directory, char *filter, bool recursive );

	// UtopiaModule's constructor checksums the program file.
	friend class UtopiaModule;

protected:
	// The handlers of the message types (invented names).
	void			ReceiveChecksumRequest( TGMessage *message );
	void			ReceiveChecksums( TGMessage *message );
	void			ReceiveChecksumFailure( TGMessage *message );
	void			ReceiveFile( TGMessage *message );
	void			ReceiveFileReceived( TGMessage *message );

	// Builds the checksum request for a directory: the first (index 0)
	// is sent at once, a copy of it and the others are queued in the
	// player's message list (invented name).
	void			SendChecksumRequest( int index, int playerID, char *directory, char *filter, bool recursive );

	// Sends the player the next file of its list (invented name).
	void			SendNextFile( int playerID );

	// Adds a file, then the files of a directory and its subdirectories,
	// to the player's list of files to send (invented names).
	void			AddFileToSend( char *name, int playerID );
	void			AddDirectoryToSend( TGDirectory *directory, int playerID );

	// Compares the checksums a player sent for a directory with this
	// machine's and adds the files that differ or that the player lacks to
	// the player's list; with skip (a directory this machine lacks), only
	// reads past them (invented names).
	void			CompareDirectoryChecksums( TGStream *stream, TGDirectory *directory, int playerID, bool skip );
	// The answer to a request of the script check's list (invented name).
	void			ReceiveScriptChecksums( TGMessage *message );
	// The same for the script check: a file that differs or that the
	// player lacks rejects it (invented name).
	void			CompareScriptChecksums( TGStream *stream, TGDirectory *directory, int playerID, bool skip );

	// Writes the names and checksums of a directory's files, then the names
	// of its subdirectories, then each subdirectory the same way (invented
	// name).
	void			WriteDirectoryChecksums( TGStream *stream, TGDirectory *directory );

	// The checksum of a file's contents, every 32-bit word but the second
	// (a compiled script's time stamp); 0 when it can't be read (invented
	// name).
	static int		GetFileChecksum( char *path );

	// The Python module name of a script path: "Scripts/" dropped, the
	// slashes made dots, the extension cut (invented name).
	void			GetModuleName( char *path, char *moduleName, int size );

	// Tells this object the player failed the check and tells the player
	// why: the file that differs, or the other game version (invented
	// name).
	void			RejectPlayer( int playerID, char *fileName, bool versionDifferent );

	// Tells this object the player passed the system check (invented name).
	void			PostSystemChecksumComplete( int playerID );

	// Reads the directory, the file filter and the recursion flag of a
	// checksum request; the buffers must be large enough (invented name).
	static bool		ReadChecksumRequest( TGMessage *message, char *directory, char *filter, bool *recursive );

	// Creates the directories of a path whose last part is a file name,
	// one by one from the working directory, which it restores; false
	// when one could not be made (invented name).
	bool			CreateDirectories( char *path );

	// Reloads Autoexec and the scripts the host may have sent: the ship,
	// system and multiplayer scripts (invented name).
	void			ReloadScripts( void );
	// Reloads the module of each script of the directory and its
	// subdirectories, making each compiled script writable first; with no
	// listing given, it lists the path itself (invented name).
	void			ReloadDirectory( char *path, TGDirectory *directory );

	// Deletes every player's message list (invented name).
	void			DeleteAllMessageLists( void );
	// Deletes the player's message list, if there is one (invented name).
	void			DeleteMessageList( int playerID );

	bool			unknown14;			// 0x14
	bool			unknown15;			// 0x15
	NiTMap<int, TGMessage *>	sentMessages;		// 0x18 by player: the copy of the last message sent it (owned)
	NiTMap<int, NiTList<TGMessage *> *>	messageLists;	// 0x28 by player (owned lists of owned messages)
	NiTMap<int, NetFileNameList *>	fileLists;		// 0x38 by player: the files still to send it (owned)

	static TGEventHandlerTable	classHandlers;
};

#endif /* !__NETFILE_H__ */
