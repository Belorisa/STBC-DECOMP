/*
===============================================================================

	GameSpy.h

	The GameSpy server-list and query client of a multiplayer game; the
	game module keeps one (UtopiaModule::CreateGameSpy, GetGameSpy).

	0xF4 bytes, allocated from the pools. Evidence: the constructor
	(0x69BFA0) calls TGEventHandlerObject's with the ID, stores the vtable
	0x895564 (22 slots: TGEventHandlerObject's and a new slot 21), sets
	the flags at 0xEC-0xF0, clears 0xE4 and loads a localization
	database into 0xE8; the class strings are "GameSpy", "_p_GameSpy" and
	"GameSpyPtr", and its handlers "GameSpy :: ProcessQueryHandler" and
	"GameSpy :: SetGameModeHandler". UtopiaModule::CreateGameSpy
	(0x445FC0) allocates 0xF4 bytes for it. No SWIG wrappers name its
	methods.

	A host reports its game through the SDK's query reporting (qr_* on
	0xDC, InitQueryReporting); a client fetches the server list through
	the SDK's goaceng (ServerList* on 0xE0, CreateServerList) and passes
	what the list does to the multiplayer window (main window 8) as
	events. Retail's code lies in two objects: 0x69BFA0-0x69CEAF (GameSpy.cpp)
	and 0x69CEB0-0x69D75F (GameSpyServerList.cpp, an invented file name:
	the object starts with an <iostream> initialiser run).

===============================================================================
*/

#ifndef __GAMESPY_H__
#define __GAMESPY_H__

#include "TGEventHandlerObject.h"
#include "goaceng.h"

// The query reporting's handle (gqueryreporting.h, which brings in
// <windows.h>; GameSpy.cpp includes it after the game's headers).
typedef struct qr_implementation_s *qr_t;

class TGEvent;
class TGLocalizationDatabase;

// GetObjType's value for a GameSpy (its creator's type in UtopiaModule's registration).
#define GAMESPY_TYPE		0x80D7

// The events of the server list the multiplayer window gets (the App constants).
#define ET_SET_GAME_MODE				0x8000F2
#define ET_SERVER_LIST_PERCENTAGE		0x8000F3
#define ET_SERVER_LIST_STATE			0x8000F4
#define ET_SERVER_ENTRY_EVENT			0x8000F5
#define ET_REFRESH_SERVER_LIST_DONE		0x8000F8
#define ET_SERVER_PLAYER_EVENT			0x8000FA

// The game's name and the ports of the LAN search.
#define GAMESPY_GAME_NAME		"bcommander"
#define GAMESPY_LAN_FIRST_PORT	22101
#define GAMESPY_LAN_LAST_PORT	22201

// FUNCTION: STBC 0x0069c110 ??_GGameSpy@@UAEPAXI@Z
class GameSpy : public TGEventHandlerObject {
public:
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	// The class's handler methods by name, and in the class's table (invented names).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );
					GameSpy( long id = 0 );
	virtual			~GameSpy( void );

	// TGObject
	// FUNCTION: STBC 0x0069c080 ?GetObjType@GameSpy@@UAEHXZ @game/GameSpy
	virtual int		GetObjType( void ) { return GAMESPY_TYPE; }
	// FUNCTION: STBC 0x0069c090 ?IsTypeOf@GameSpy@@UAE_NH@Z @game/GameSpy
	virtual bool	IsTypeOf( int type ) {
						if ( type == GAMESPY_TYPE ) {
							return true;
						}
						return TGEventHandlerObject::IsTypeOf( type );
					}
	// FUNCTION: STBC 0x0069c530 ?WriteToStream@GameSpy@@UAE_NPAVTGStream@@@Z @game/GameSpy
	virtual bool	WriteToStream( TGStream *stream ) { return true; }
	// FUNCTION: STBC 0x0069c540 ?ReadFromStream@GameSpy@@UAE_NPAVTGStream@@@Z @game/GameSpy
	virtual bool	ReadFromStream( TGStream *stream ) { return true; }
	// FUNCTION: STBC 0x0069c0d0 ?GetClassName@GameSpy@@UAEPADXZ @game/GameSpy
	virtual char *	GetClassName( void ) { return "GameSpy"; }
	// FUNCTION: STBC 0x0069c0e0 ?GetSwigTypeName@GameSpy@@UAEPADXZ @game/GameSpy
	virtual char *	GetSwigTypeName( void ) { return "_p_GameSpy"; }
	// FUNCTION: STBC 0x0069c0f0 ?GetPythonClassName@GameSpy@@UAEPADXZ @game/GameSpy
	virtual char *	GetPythonClassName( void ) { return "GameSpyPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0069c100 ?GetClassHandlers@GameSpy@@UAEPAVTGEventHandlerTable@@XZ @game/GameSpy
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// UtopiaModule's frame passes the real time (0x69C440 ends ret 4): a
	// host answers the queries, a client lets the server list work.
	virtual void	VirtualSlot21( float time );

	// Starts answering the master server's and the players' queries for the
	// hosted game: qr_init_socket with the game's name and key on the
	// network's socket; local (no heartbeats) is stored at 0xEF. False when
	// it was started already or fails (invented name).
	bool			InitQueryReporting( bool local );
	// Copies the mode ("openplaying", "debriefing") into the game mode the
	// queries report, at 0x14 (invented name, after SetGameModeHandler).
	void			SetGameMode( char *mode );
	// UtopiaApp::OnIdle slows the idle turns while it is clear
	bool			GetUnknownEC( void ) const { return listIdle; }

	// The server list (invented names): RefreshServerList asks the master
	// (or the LAN) for the servers; StopServerList halts and frees the list;
	// SortServerList sorts it by key, the other way each time, and sends
	// the servers again; SendServerPlayers sends a server's players.
	void			RefreshServerList( bool lan );
	void			StopServerList( void );
	void			SortServerList( char *key, bool isString );
	void			SendServerPlayers( GServer server );
	// A server's time and frag limits, species system and mission script
	// (invented name); true.
	bool			GetServerInfo( GServer server, int *timeLimit, int *fragLimit, char *system, int systemLength, char *mission, int missionLength );

	// Handlers.
	void			SetGameModeHandler( TGEvent *event );
	void			ProcessQueryHandler( TGEvent *event );

protected:
	// Creates the server list when nothing is running (invented name).
	bool			CreateServerList( void );
	// What the server list reports (goaceng's ListCallBackFn; invented name).
	void			ServerListCallBack( GServerList serverList, int msg, void *param1, void *param2 );

	// The query reporting's callbacks: the \key\value pairs of each query
	// (invented names).
	void			BasicCallback( char *outbuf, int maxlen );
	void			InfoCallback( char *outbuf, int maxlen );
	void			RulesCallback( char *outbuf, int maxlen );
	void			PlayersCallback( char *outbuf, int maxlen );
	// The SDK's callbacks: userdata and instance are the GameSpy (invented names).
	static void		OnBasicQuery( char *outbuf, int maxlen, void *userdata );
	static void		OnInfoQuery( char *outbuf, int maxlen, void *userdata );
	static void		OnRulesQuery( char *outbuf, int maxlen, void *userdata );
	static void		OnPlayersQuery( char *outbuf, int maxlen, void *userdata );
	static void		OnServerList( GServerList serverList, int msg, void *instance, void *param1, void *param2 );

	char			gameMode[0xC8];			// 0x14 the mode the queries report (invented name)
	qr_t			queryReporting;			// 0xDC the hosted game's query reporting (invented name)
	GServerList		serverList;				// 0xE0 the client's server list (invented name)
	float			lastUpdateTime;			// 0xE4 the time VirtualSlot21 last ran (invented name)
	TGLocalizationDatabase *	database;	// 0xE8 Multiplayer.tgl, loaded by the constructor (invented name)
	bool			listIdle;				// 0xEC true while no list update runs (invented name)
	bool			started;				// 0xED the reporting or the list is running (invented name)
	bool			hosting;				// 0xEE reporting a hosted game, not listing (invented name)
	bool			local;					// 0xEF no heartbeats to the master server (invented name)
	bool			sortAscending;			// 0xF0 toggled by each SortServerList (invented name)
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__GAMESPY_H__ */
