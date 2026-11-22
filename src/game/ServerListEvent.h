/*
===============================================================================

	ServerListEvent.h

	The events of the multiplayer server list: ServerListEvent carries one
	GameSpy server, SortServerListEvent the key to sort the list by.

	ServerListEvent is 0x2C bytes and SortServerListEvent 0x30, allocated
	from the pools.

	Evidence: the class strings "ServerListEvent", "_p_ServerListEvent",
	"ServerListEventPtr" and the SortServerListEvent ones; GetObjType
	returns 0x80D8 (0x442A30) and 0x80D9 (0x442BF0), the types the game's
	class registration files the creators 0x4429E0 and 0x442B70 under. The
	vtables 0x88B4F0 and 0x88B52C override TGEvent's type, name and stream
	slots and Copy; the streams save nothing of ServerListEvent's and
	ignore TGEvent's result. The constructors (0x442A10, 0x442BC0) take
	the ID with a default: the SWIG wrappers new_ServerListEvent and
	new_SortServerListEvent call them. The SWIG wrappers SetServer and
	GetServer copy the GServer at 0x28; GetServerName and the other server
	queries call the GameSpy SDK on it. SetSortBy copies the string at 0x28
	(so does Copy; the destructor frees it), and SetStringField and
	IsStringField the flag at 0x2C, true from the constructor. Every
	function is inline: the out-of-line copies are those of the class
	registration's unit.

===============================================================================
*/

#ifndef __SERVERLISTEVENT_H__
#define __SERVERLISTEVENT_H__

#include <string.h>
#include "TGEvent.h"
#include "TGStream.h"
#include "UtopiaModule.h"
#include "goaceng.h"

// The string the server queries give for a value the server did not send.
#define UNKNOWN_SERVER_VALUE		"(?]"

// GetObjType's values.
#define SERVERLISTEVENT_TYPE		0x80D8
#define SORTSERVERLISTEVENT_TYPE	0x80D9

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00442aa0 ??_GServerListEvent@@UAEPAXI@Z
class ServerListEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x00442a10 ??0ServerListEvent@@QAE@J@Z
					ServerListEvent( long id = 0 ) : TGEvent( id ) { server = NULL; }
	// FUNCTION: STBC 0x00442ad0 ??1ServerListEvent@@UAE@XZ
	virtual			~ServerListEvent( void ) { server = NULL; }

	static ServerListEvent *	Create( void ) { return new ServerListEvent; }

	// TGObject
	// FUNCTION: STBC 0x00442a30 ?GetObjType@ServerListEvent@@UAEHXZ
	virtual int		GetObjType( void ) { return SERVERLISTEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x00442af0 ?WriteToStream@ServerListEvent@@UAE_NPAVTGStream@@@Z
	virtual bool	WriteToStream( TGStream *stream ) { TGEvent::WriteToStream( stream ); return true; }
	// FUNCTION: STBC 0x00442b00 ?ReadFromStream@ServerListEvent@@UAE_NPAVTGStream@@@Z
	virtual bool	ReadFromStream( TGStream *stream ) { TGEvent::ReadFromStream( stream ); return true; }
	// FUNCTION: STBC 0x00442a70 ?GetClassName@ServerListEvent@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ServerListEvent"; }
	// FUNCTION: STBC 0x00442a80 ?GetSwigTypeName@ServerListEvent@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ServerListEvent"; }
	// FUNCTION: STBC 0x00442a90 ?GetPythonClassName@ServerListEvent@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ServerListEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	void			SetServer( GServer server_ ) { server = server_; }
	GServer			GetServer( void ) { return server; }

	// The server's values; NULL or 0 without a server or once the game's
	// GameSpy is gone.
	char *			GetServerName( void ) {
						if ( g_kUtopiaModule.GetGameSpy() != NULL && server != NULL ) {
							return ServerGetStringValue( server, "hostname", UNKNOWN_SERVER_VALUE );
						}
						return NULL;
					}
	char *			GetMissionName( void ) {
						if ( g_kUtopiaModule.GetGameSpy() != NULL && server != NULL ) {
							return ServerGetStringValue( server, "mapname", UNKNOWN_SERVER_VALUE );
						}
						return NULL;
					}
	char *			GetAddress( void ) {
						if ( g_kUtopiaModule.GetGameSpy() != NULL && server != NULL ) {
							return ServerGetAddress( server );
						}
						return NULL;
					}
	char *			GetVersion( void ) {
						if ( g_kUtopiaModule.GetGameSpy() != NULL && server != NULL ) {
							return ServerGetStringValue( server, "gamever", UNKNOWN_SERVER_VALUE );
						}
						return NULL;
					}
	int				GetPortNumber( void ) {
						if ( g_kUtopiaModule.GetGameSpy() != NULL && server != NULL ) {
							return ServerGetQueryPort( server );
						}
						return 0;
					}
	int				GetPing( void ) {
						if ( g_kUtopiaModule.GetGameSpy() != NULL && server != NULL ) {
							return ServerGetPing( server );
						}
						return 0;
					}
	int				GetNumPlayers( void ) {
						if ( g_kUtopiaModule.GetGameSpy() != NULL && server != NULL ) {
							return ServerGetIntValue( server, "numplayers", 0 );
						}
						return 0;
					}
	int				GetMaxPlayers( void ) {
						if ( g_kUtopiaModule.GetGameSpy() != NULL && server != NULL ) {
							return ServerGetIntValue( server, "maxplayers", 0 );
						}
						return 0;
					}

protected:
	GServer			server;					// 0x28 not owned; not saved
};

/*
================
ServerListEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00442a40 ?IsTypeOf@ServerListEvent@@UAE_NH@Z
inline bool ServerListEvent::IsTypeOf( int type ) {
	if ( type == SERVERLISTEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
ServerListEvent::Copy

The event is taken to be a ServerListEvent.
================
*/
// FUNCTION: STBC 0x00442b50 ?Copy@ServerListEvent@@UAE_NPAVTGEvent@@@Z
inline bool ServerListEvent::Copy( TGEvent *event ) {
	TGEvent::Copy( event );
	server = ( (ServerListEvent *)event )->server;
	return true;
}

// FUNCTION: STBC 0x00442c80 ??_GSortServerListEvent@@UAEPAXI@Z
class SortServerListEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x00442bc0 ??0SortServerListEvent@@QAE@J@Z
					SortServerListEvent( long id = 0 ) : TGEvent( id ) { sortBy = NULL; stringField = true; }
	// FUNCTION: STBC 0x00442cb0 ??1SortServerListEvent@@UAE@XZ
	virtual			~SortServerListEvent( void ) {
						if ( sortBy != NULL ) {
							delete sortBy;
							sortBy = NULL;
						}
					}

	static SortServerListEvent *	Create( void ) { return new SortServerListEvent; }

	// TGObject
	// FUNCTION: STBC 0x00442bf0 ?GetObjType@SortServerListEvent@@UAEHXZ
	virtual int		GetObjType( void ) { return SORTSERVERLISTEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00442c30 ?GetClassName@SortServerListEvent@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "SortServerListEvent"; }
	// FUNCTION: STBC 0x00442c40 ?GetSwigTypeName@SortServerListEvent@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_SortServerListEvent"; }
	// FUNCTION: STBC 0x00442c50 ?GetPythonClassName@SortServerListEvent@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "SortServerListEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	// Copies the key; NULL clears it.
	void			SetSortBy( char *key ) {
						if ( sortBy != NULL ) {
							delete sortBy;
							sortBy = NULL;
						}
						if ( key != NULL ) {
							sortBy = new char[strlen( key ) + 1];
							strcpy( sortBy, key );
						}
					}
	char *			GetSortBy( void ) { return sortBy; }
	void			SetStringField( bool stringField_ ) { stringField = stringField_; }
	bool			IsStringField( void ) { return stringField; }

protected:
	char *			sortBy;					// 0x28 owned; the server key to sort by
	bool			stringField;			// 0x2C the key's values sort as strings, not numbers
};

/*
================
SortServerListEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00442c00 ?IsTypeOf@SortServerListEvent@@UAE_NH@Z
inline bool SortServerListEvent::IsTypeOf( int type ) {
	if ( type == SORTSERVERLISTEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
SortServerListEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x00442ce0 ?WriteToStream@SortServerListEvent@@UAE_NPAVTGStream@@@Z
inline bool SortServerListEvent::WriteToStream( TGStream *stream ) {
	TGEvent::WriteToStream( stream );
	stream->VirtualSlot11( sortBy );
	stream->WriteBool( stringField );
	return true;
}

/*
================
SortServerListEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x00442d10 ?ReadFromStream@SortServerListEvent@@UAE_NPAVTGStream@@@Z
inline bool SortServerListEvent::ReadFromStream( TGStream *stream ) {
	TGEvent::ReadFromStream( stream );
	sortBy = stream->VirtualSlot10( -1 );
	stringField = stream->ReadBool();
	return true;
}

/*
================
SortServerListEvent::Copy

The event is taken to be a SortServerListEvent.
================
*/
// FUNCTION: STBC 0x00442d40 ?Copy@SortServerListEvent@@UAE_NPAVTGEvent@@@Z
inline bool SortServerListEvent::Copy( TGEvent *event ) {
	TGEvent::Copy( event );
	SetSortBy( ( (SortServerListEvent *)event )->sortBy );
	stringField = ( (SortServerListEvent *)event )->stringField;
	return true;
}

#endif /* !__SERVERLISTEVENT_H__ */
