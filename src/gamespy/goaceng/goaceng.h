/******
goaceng.h

The GameSpy SDK's server-list client (goaceng, the "CEngine" SDK) as
retail's 2001 version has it: it fetches a game's server list from the
master server, or finds the servers of the LAN, and queries each server
for its keys.

Fitted to retail (0x6A9910-0x6AB87F). IGN's 2011 SDK has only the later
generation (serverbrowsing); the names here follow the goaceng API that
games of the time called (ServerList*, Server*).
******/

#ifndef _GOACENG_H_
#define _GOACENG_H_

#ifdef __cplusplus
extern "C" {
#endif

// The query a server is sent, or the kind of list the master sends.
typedef enum {qt_basic, qt_info, qt_rules, qt_players, qt_info_rules, qt_status, qt_grouprooms, qt_masterinfo} GQueryType;

// What a server list is doing.
typedef enum {sl_idle, sl_listxfer, sl_lanlist, sl_querying} GServerListState;

// The error codes the server list functions return.
#define GE_NOERROR		0
#define GE_NOSOCKET		1
#define GE_NODNS		2
#define GE_NOCONNECT	3
#define GE_BUSY			4
#define GE_DATAERROR	5

// The messages of a list's callback.
#define LIST_STATECHANGED		1	// the list's state changed (param1: the server list's state)
#define LIST_PROGRESS			2	// a server answered (param1: the server, param2: the percentage done)

typedef int gbool;

// A server of the list (the SWIG type "_p_GServer").
typedef struct GServerImplementation *GServer;
// A server list.
typedef struct GServerListImplementation *GServerList;

typedef void (*ListCallBackFn)(GServerList serverlist, int msg, void *instance, void *param1, void *param2);
typedef void (*KeyEnumFn)(char *key, char *value, void *instance);

int		ServerGetPing(GServer server);
char *	ServerGetAddress(GServer server);
unsigned int ServerGetInetAddress(GServer server);
int		ServerGetQueryPort(GServer server);
char *	ServerGetStringValue(GServer server, char *key, char *sdefault);
int		ServerGetIntValue(GServer server, char *key, int idefault);
double	ServerGetFloatValue(GServer server, char *key, double fdefault);
gbool	ServerGetBoolValue(GServer server, char *key, gbool bdefault);
char *	ServerGetPlayerStringValue(GServer server, int playernum, char *key, char *sdefault);
int		ServerGetPlayerIntValue(GServer server, int playernum, char *key, int idefault);
double	ServerGetPlayerFloatValue(GServer server, int playernum, char *key, double fdefault);
void	ServerEnumKeys(GServer server, KeyEnumFn KeyFn, void *instance);

// How ServerListSort compares the sort key's values.
typedef enum {cm_int, cm_float, cm_strcase, cm_stricase} GCompareMode;

typedef int GError;

GServerList	ServerListNew(char *gamename, char *enginename, char *seckey, int maxconcupdates, void *CallBackFn, int CallBackFnType, void *instance);
void	ServerListFree(GServerList serverlist);
GError	ServerListUpdate(GServerList serverlist, gbool async);
GError	ServerListUpdate2(GServerList serverlist, gbool async, char *filter, GQueryType querytype);
GError	ServerListLANUpdate(GServerList serverlist, gbool async, int startsearchport, int endsearchport, int searchdelta);
GError	ServerListAuxUpdate(GServerList serverlist, char *ip, int port, gbool async, GQueryType querytype);
void	ServerListRemoveServer(GServerList serverlist, char *ip, int port);
GError	ServerListThink(GServerList serverlist);
GError	ServerListHalt(GServerList serverlist);
GError	ServerListClear(GServerList serverlist);
GServerListState ServerListState(GServerList serverlist);
char *	ServerListErrorDesc(GServerList serverlist, GError error);
GServer	ServerListGetServer(GServerList serverlist, int index);
int		ServerListCount(GServerList serverlist);
void	ServerListSort(GServerList serverlist, gbool ascending, char *sortkey, GCompareMode comparemode);

#ifdef __cplusplus
}
#endif

#endif
