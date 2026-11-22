/******
gserverlist.c

The goaceng server list (goaceng.h), retail's 2001 version: it fetches a
game's list from the master server over TCP, or broadcasts for the LAN's
servers, and then queries each server over UDP. Fitted to retail; the
public names follow the goaceng API, the file's own helpers keep the names
of the goaceng source as far as they are known (CreateServerListSocket,
SendListRequest, InitUpdateList, ServerListModeChange and the rest);
ServerListParseInfoList, BackslashOffset, ServerListAddServerData,
ServerListFindServerMax, ServerListInsertServer, FindServerUpdate,
numoldservers and parsestate are invented.

TG: ServerListNew reads the master server's host name from the first line
of masterserver.txt, and the default is Activision's master server, not
GameSpy's.
******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nonport.h"
#include "darray.h"
#include "gserver.h"
#include "gqueryreporting.h"

#define MASTER_ADDR "stbridgecmnd01.activision.com"
#define MASTER_FILE "masterserver.txt"
#define MSPORT 28900
#define ENGINE_VERSION "1.6"
#define SERVER_GROWBY 64
#define KEYLIST_BUCKETS 500
#define SERVER_TIMEOUT 3000
#define MAX_FIELDS 20
#define LIST_BUFFER_SIZE 2048
#define INDATA_SIZE 1500

// The socket a server is being queried on (0x1C bytes).
typedef struct
{
	SOCKET s;
	GServer currentserver;		// NULL while the socket is free
	unsigned long starttime;	// when the query went out
	struct sockaddr_in saddr;
} UpdateInfo;

// 0xA0 bytes: ServerListNew (0x6AA100) allocates them.
struct GServerListImplementation
{
	GServerListState state;
	DArray servers;				// GServer
	UpdateInfo *updatelist;		// maxupdates of them
	char gamename[32];			// the game whose servers are listed
	char seckey[32];
	char enginename[32];		// the game that answers the master's challenge
	int maxupdates;
	int nextupdate;				// the next server to query
	int abortupdate;
	ListCallBackFn CallBackFn;
	void *instance;
	char *sortkey;
	gbool sortascending;
	SOCKET slsocket;			// the master server's or the LAN broadcast's socket
	unsigned long lanstarttime;
	GQueryType querytype;
	HashTable keylist;			// the interned key and value strings (char *)
	int numoldservers;			// the servers of the list before the update
	int parsestate;				// ServerListParseInfoList: 0 the field count, 1 the field names, 2 the servers
};

// DATA: STBC 0x95A4FC
char ServerListHost[64] = MASTER_ADDR;
// DATA: STBC 0x95A53C
static char *querystrings[] = {"\\basic\\","\\info\\","\\rules\\","\\players\\","\\info\\\\rules\\","\\status\\"};
// DATA: STBC 0x895674
static const int querylengths[] = {7,6,7,9,13,8};

// DATA: STBC 0x9B3E38
static GServerList g_sortserverlist;
// DATA: STBC 0x99551C
static int numfields;
// DATA: STBC 0x995520
static char *fieldlist[MAX_FIELDS];

static GError CreateServerListSocket(GServerList serverlist);
static GError SendListRequest(GServerList serverlist, char *filter);
static GError InitUpdateList(GServerList serverlist);
static GError FreeUpdateList(GServerList serverlist);
static void ServerListModeChange(GServerList serverlist, GServerListState newstate);
static void DoSyncLoop(GServerList serverlist);
static GError CreateServerListLANSocket(GServerList serverlist);
static GError SendBroadcastRequest(GServerList serverlist, int startport, int endport, int delta);
static GServer ServerListAddServer(GServerList serverlist, unsigned long ip, unsigned short port, GQueryType qtype);
static GServer ServerListInsertServer(GServerList serverlist, unsigned long ip, unsigned short port, int pos, GQueryType qtype);
static GServer ServerListAddServerData(GServerList serverlist, char **fieldlist, int fieldcount, char *serverdata, GQueryType qtype);
static int ServerListFindServer(GServerList serverlist, unsigned int ip, int port);
static int ServerListFindServerMax(GServerList serverlist, unsigned int ip, int port, int count);
static int FindServerUpdate(GServerList serverlist, GServer server);
static GError ServerListLANList(GServerList serverlist);
static GError ServerListReadList(GServerList serverlist);
static int ServerListParseInfoList(GServerList serverlist, char *data, int len);
static int BackslashOffset(char *data, int len, int nslashes);
static GError ServerListQueryLoop(GServerList serverlist);
static int IntKeyCompare(const void *entry1, const void *entry2);
static int FloatKeyCompare(const void *entry1, const void *entry2);
static int StrCaseKeyCompare(const void *entry1, const void *entry2);
static int StrNoCaseKeyCompare(const void *entry1, const void *entry2);

/* ServerListNew: a server list for gamename; the master server is asked
for it on behalf of enginename, whose secret key is seckey */
// FUNCTION: STBC 0x6AA100
GServerList ServerListNew(char *gamename, char *enginename, char *seckey, int maxconcupdates, void *CallBackFn, int CallBackFnType, void *instance)
{
	GServerList list;
	char hostname[256];
	FILE *f;
	int len;

	// TG: the master server's host name from masterserver.txt
	f = fopen(MASTER_FILE, "rt");
	if (f != NULL && fgets(hostname, sizeof(hostname), f) != NULL)
	{
		strncpy(ServerListHost, hostname, sizeof(ServerListHost));
		len = strlen(ServerListHost);
		if (ServerListHost[len - 1] == '\n')
			ServerListHost[len - 1] = '\0';
	} else
		strcpy(ServerListHost, MASTER_ADDR);

	list = (GServerList) malloc(sizeof(struct GServerListImplementation));
	list->state = sl_idle;
	list->servers = ArrayNew(sizeof(GServer), SERVER_GROWBY, ServerFree);
	list->keylist = TableNew2(sizeof(char *),KEYLIST_BUCKETS,4, GStringHash, GCaseInsensitiveCompare, GStringFree);
	list->maxupdates = maxconcupdates;
	list->updatelist = malloc(maxconcupdates * sizeof(UpdateInfo));
	memset(list->updatelist, 0, maxconcupdates * sizeof(UpdateInfo));
	strcpy(list->gamename, gamename);
	strcpy(list->seckey, seckey);
	strcpy(list->enginename, enginename);
	list->CallBackFn = (ListCallBackFn)CallBackFn;
	list->instance = instance;
	list->sortkey = "";
	list->numoldservers = 0;
	SocketStartUp();
	return list;
}

/* ServerListFree: frees the list, its servers and its keys */
// FUNCTION: STBC 0x6AA2B0
void ServerListFree(GServerList serverlist)
{
	ArrayFree(serverlist->servers);
	TableFree(serverlist->keylist);
	free(serverlist->updatelist);
	free(serverlist);
	SocketShutDown();
}

/* ServerListUpdate: fetches the list and queries every server's status */
// FUNCTION: STBC 0x6AA2F0
GError ServerListUpdate(GServerList serverlist, gbool async)
{
	return ServerListUpdate2(serverlist, async, NULL, qt_status);
}

/* ServerListUpdate2: fetches the servers that match filter and queries
them with querytype */
// FUNCTION: STBC 0x6AA310
GError ServerListUpdate2(GServerList serverlist, gbool async, char *filter, GQueryType querytype)
{
	GError error;

	if (serverlist->state != sl_idle)
		return GE_BUSY;
	serverlist->querytype = querytype;
	error = CreateServerListSocket(serverlist);
	if (error) return error;
	error = SendListRequest(serverlist, filter);
	if (error) return error;
	if (querytype != qt_grouprooms && querytype != qt_masterinfo)
	{
		error = InitUpdateList(serverlist);
		if (error) return error;
	}
	serverlist->nextupdate = 0;
	serverlist->abortupdate = 0;
	serverlist->numoldservers = ServerListCount(serverlist);
	if (!async)
		DoSyncLoop(serverlist);
	return 0;
}

/* InitUpdateList: opens the query sockets; fewer when the system runs out */
// FUNCTION: STBC 0x6AA3A0
static GError InitUpdateList(GServerList serverlist)
{
	int i;

	for (i = 0 ; i < serverlist->maxupdates ; i++)
	{
		serverlist->updatelist[i].s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (serverlist->updatelist[i].s == INVALID_SOCKET)
		{ //ran out of sockets, just cap maxupdates here, unless we don't have any
			if (i == 0)
				return GE_NOSOCKET;
			serverlist->maxupdates = i;
			return 0;
		}
		serverlist->updatelist[i].currentserver = NULL;
		serverlist->updatelist[i].starttime = 0;
	}
	return 0;
}

/* CreateServerListSocket: connects to the master server */
// FUNCTION: STBC 0x6AA410
static GError CreateServerListSocket(GServerList serverlist)
{
	struct sockaddr_in saddr;
	struct hostent *hent;

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(MSPORT);
	saddr.sin_addr.s_addr = inet_addr(ServerListHost);
	if (saddr.sin_addr.s_addr == INADDR_NONE)
	{
		hent = gethostbyname(ServerListHost);
		if (!hent)
			return GE_NODNS;
		saddr.sin_addr.s_addr = *(u_long *)hent->h_addr_list[0];
	}
	serverlist->slsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverlist->slsocket == INVALID_SOCKET)
		return GE_NOSOCKET;
	if (connect(serverlist->slsocket, (struct sockaddr *) &saddr, sizeof saddr) != 0)
	{
		closesocket(serverlist->slsocket);
		return GE_NOCONNECT;
	}
	return 0;
}

/* SendListRequest: answers the master's challenge and asks for the list */
// FUNCTION: STBC 0x6AA4C0
static GError SendListRequest(GServerList serverlist, char *filter)
{
	char data[256], *ptr, result[64];
	int len;
	char *listtype;

	len = recv(serverlist->slsocket, data, sizeof(data) - 1, 0);
	if (len == SOCKET_ERROR)
		return GE_NOCONNECT;
	data[len] = '\0'; //null terminate it
	ptr = strstr ( data, "\\secure\\" );
	if (!ptr)
		return GE_DATAERROR;
	ptr = ptr + 8;
	gs_encrypt ( (unsigned char *)serverlist->seckey, 6, (unsigned char *)ptr, 6 );
	gs_encode ( (unsigned char *)ptr, 6, (unsigned char *)result );
	//validate to the master
	sprintf(data, "\\gamename\\%s\\gamever\\%s\\location\\0\\validate\\%s\\final\\\\queryid\\1.1\\",
		serverlist->enginename, ENGINE_VERSION, result);
	len = send ( serverlist->slsocket, data, strlen(data), 0 );
	if (len == SOCKET_ERROR || len == 0)
		return GE_NOCONNECT;
	if (serverlist->querytype == qt_grouprooms)
	{
		listtype = "groups";
		serverlist->parsestate = 0;
	} else if (serverlist->querytype == qt_masterinfo)
	{
		listtype = "info2";
		serverlist->parsestate = 0;
	} else
		listtype = "cmp";
	//send the list request
	if (filter)
		sprintf(data, "\\list\\%s\\gamename\\%s\\where\\%s\\final\\", listtype, serverlist->gamename, filter);
	else
		sprintf(data, "\\list\\%s\\gamename\\%s\\final\\", listtype, serverlist->gamename);
	len = send ( serverlist->slsocket, data, strlen(data), 0 );
	if (len == SOCKET_ERROR || len == 0)
		return GE_NOCONNECT;
	ServerListModeChange(serverlist, sl_listxfer);
	return 0;
}

/* ServerListModeChange: sets the state and tells the callback */
// FUNCTION: STBC 0x6AA660
static void ServerListModeChange(GServerList serverlist, GServerListState newstate)
{
	serverlist->state = newstate;
	serverlist->CallBackFn(serverlist, LIST_STATECHANGED, serverlist->instance, NULL, NULL);
}

/* DoSyncLoop: thinks until the update is done */
// FUNCTION: STBC 0x6AA680
static void DoSyncLoop(GServerList serverlist)
{
	while (serverlist->state != sl_idle)
	{
		ServerListThink(serverlist);
		msleep(10);
	}
}

/* ServerListLANUpdate: broadcasts a status query on the ports from
startsearchport to endsearchport and queries the servers that answer */
// FUNCTION: STBC 0x6AA6B0
GError ServerListLANUpdate(GServerList serverlist, gbool async, int startsearchport, int endsearchport, int searchdelta)
{
	GError error;

	if (serverlist->state != sl_idle)
		return GE_BUSY;
	error = InitUpdateList(serverlist);
	if (error) return error;
	error = CreateServerListLANSocket(serverlist);
	if (error) return error;
	error = SendBroadcastRequest(serverlist, startsearchport, endsearchport, searchdelta);
	if (error) return error;
	serverlist->nextupdate = 0;
	serverlist->abortupdate = 0;
	if (!async)
		DoSyncLoop(serverlist);
	return 0;
}

/* CreateServerListLANSocket: the broadcast socket */
// FUNCTION: STBC 0x6AA720
static GError CreateServerListLANSocket(GServerList serverlist)
{
	int optval = 1;

	serverlist->slsocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverlist->slsocket == INVALID_SOCKET)
		return GE_NOSOCKET;
	if (setsockopt(serverlist->slsocket, SOL_SOCKET, SO_BROADCAST, (char *)&optval, sizeof(optval)) != 0)
		return GE_NOSOCKET;
	return 0;
}

/* SendBroadcastRequest: sends the status query to every port in the range */
// FUNCTION: STBC 0x6AA770
static GError SendBroadcastRequest(GServerList serverlist, int startport, int endport, int delta)
{
	struct sockaddr_in saddr;
	short i;

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = 0xFFFFFFFF; //broadcast
	for (i = startport ; i <= endport ; i += delta)
	{
		saddr.sin_port = htons(i);
		sendto(serverlist->slsocket, "\\status\\",8,0,(struct sockaddr *)&saddr,sizeof(saddr));
	}
	ServerListModeChange(serverlist, sl_lanlist);
	serverlist->lanstarttime = current_time();
	return 0;
}

/* ServerListAddServer: appends a new server */
// FUNCTION: STBC 0x6AA7F0
static GServer ServerListAddServer(GServerList serverlist, unsigned long ip, unsigned short port, GQueryType qtype)
{
	GServer server;

	if (serverlist == NULL)
		return NULL;
	server =  ServerNew(ip, port, qtype, serverlist->keylist);
	ArrayAppend(serverlist->servers,&server);
	return server;
}

/* ServerListFindServer: the index of the server with ip and port, or -1 */
// FUNCTION: STBC 0x6AA840
static int ServerListFindServer(GServerList serverlist, unsigned int ip, int port)
{
	return ServerListFindServerMax(serverlist, ip, port, ArrayLength(serverlist->servers));
}

/* ServerListFindServerMax: the same among the first count servers */
// FUNCTION: STBC 0x6AA870
static int ServerListFindServerMax(GServerList serverlist, unsigned int ip, int port, int count)
{
	int i;
	GServer server;

	for (i = 0; i < count ; i++)
	{
		server = *(GServer *)ArrayNth(serverlist->servers,i);
		if (port == ServerGetQueryPort(server) && ServerGetInetAddress(server) == ip)
			return i;
	}
	return -1;
}

/* ServerListRemoveServer: removes the server at ip and port */
// FUNCTION: STBC 0x6AA8D0
void ServerListRemoveServer(GServerList serverlist, char *ip, int port)
{
	int currentindex;
	int updateindex;

	currentindex = ServerListFindServer(serverlist, inet_addr(ip), port);
	if (currentindex == -1)
		return; //can't do anything, it's not in the list
	if (serverlist->state != sl_idle && serverlist->nextupdate > currentindex)
	{ //it's been queried or is being queried
		updateindex = FindServerUpdate(serverlist, *(GServer *)ArrayNth(serverlist->servers,currentindex));
		if (updateindex != -1) //it's being queried, stop
			serverlist->updatelist[updateindex].currentserver = NULL;
		serverlist->nextupdate--;
	}
	ArrayDeleteAt(serverlist->servers,currentindex);
}

/* FindServerUpdate: the query socket that is querying server, or -1 */
// FUNCTION: STBC 0x6AA950
static int FindServerUpdate(GServerList serverlist, GServer server)
{
	int i;

	for (i = 0 ; i < serverlist->maxupdates ; i++)
		if (serverlist->updatelist[i].currentserver == server)
			return i;
	return -1;
}

/* ServerListAuxUpdate: queries one server, adding it when it is new */
// FUNCTION: STBC 0x6AA980
GError ServerListAuxUpdate(GServerList serverlist, char *ip, int port, gbool async, GQueryType querytype)
{
	unsigned int real_ip;
	int currentindex;
	int updateindex;
	GError error;

	real_ip = inet_addr(ip);
	currentindex = ServerListFindServer(serverlist, real_ip, port);
	if (serverlist->state == sl_idle)
	{ //start a query of this server only
		error = InitUpdateList(serverlist);
		if (error) return error;
		if (currentindex != -1)
		{ //move it to the end, to be queried again
			GServer server;

			server = *(GServer *)ArrayNth(serverlist->servers,currentindex);
			server->ping = 9999;
			ArrayRemoveAt(serverlist->servers,currentindex);
			ArrayAppend(serverlist->servers,&server);
		} else
			ServerListAddServer(serverlist, real_ip, (unsigned short)port, querytype);
		serverlist->nextupdate = ArrayLength(serverlist->servers) - 1;
		serverlist->abortupdate = 0;
		ServerListModeChange(serverlist, sl_querying);
		if (!async)
			DoSyncLoop(serverlist);
		return 0;
	}
	//an update is running: queue it next
	if (currentindex == -1)
	{
		ServerListInsertServer(serverlist, real_ip, (unsigned short)port, serverlist->nextupdate, querytype);
		if (serverlist->state == sl_listxfer || serverlist->state == sl_lanlist)
			serverlist->numoldservers++;
	} else
	{
		GServer server;

		server = *(GServer *)ArrayNth(serverlist->servers,currentindex);
		if (currentindex < serverlist->nextupdate)
		{ //it has been queried already: query it again
			server->ping = 9999;
			updateindex = FindServerUpdate(serverlist, server);
			if (updateindex != -1)
				serverlist->updatelist[updateindex].currentserver = NULL;
			ArrayInsertAt(serverlist->servers,&server,serverlist->nextupdate);
			ArrayRemoveAt(serverlist->servers,currentindex);
			serverlist->nextupdate--;
		}
	}
	return 0;
}

/* ServerListInsertServer: inserts a new server at pos */
// FUNCTION: STBC 0x6AAB00
static GServer ServerListInsertServer(GServerList serverlist, unsigned long ip, unsigned short port, int pos, GQueryType qtype)
{
	GServer server;

	server =  ServerNew(ip, port, qtype, serverlist->keylist);
	ArrayInsertAt(serverlist->servers,&server,pos);
	return server;
}

/* ServerListThink: does the update's next step; call it often */
// FUNCTION: STBC 0x6AAB40
GError ServerListThink(GServerList serverlist)
{
	if (serverlist == NULL)
		return GE_DATAERROR;
	switch (serverlist->state)
	{
	case sl_idle:
		return 0;
	case sl_listxfer:
		return ServerListReadList(serverlist);
	case sl_lanlist:
		return ServerListLANList(serverlist);
	case sl_querying:
		return ServerListQueryLoop(serverlist);
	}
	return 0;
}

/* ServerListLANList: adds the servers that answered the broadcast */
// FUNCTION: STBC 0x6AAB90
static GError ServerListLANList(GServerList serverlist)
{
	struct timeval timeout = {0,0};
	fd_set set;
	char indata[INDATA_SIZE];
	struct sockaddr_in saddr;
	int saddrlen = sizeof(saddr);
	int error;

	while (1) //we break if the select fails
	{
		FD_ZERO(&set);
		FD_SET( serverlist->slsocket, &set);
		error = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (SOCKET_ERROR == error || 0 == error) //no data
			break;
		error = recvfrom(serverlist->slsocket, indata, sizeof(indata) - 1, 0, (struct sockaddr *)&saddr, &saddrlen );
		if (SOCKET_ERROR == error)
			continue;
		//we got data, add the server to the list to update
		if (strstr(indata,"\\final\\") != NULL) // BUG: indata is not terminated at the length received
			if (ServerListFindServer(serverlist, saddr.sin_addr.s_addr, ntohs(saddr.sin_port)) == -1)
				ServerListAddServer(serverlist, saddr.sin_addr.s_addr, ntohs(saddr.sin_port), qt_status);
	}
	if (current_time() - serverlist->lanstarttime > SERVER_TIMEOUT) //done waiting for replies
	{
		closesocket(serverlist->slsocket);
		serverlist->slsocket = INVALID_SOCKET;
		ServerListModeChange(serverlist, sl_querying);
	}
	return 0;
}

/* ServerListReadList: reads the master's list: 6 bytes per server (address
and port, network order), or the info lists' text */
// FUNCTION: STBC 0x6AACC0
static GError ServerListReadList(GServerList serverlist)
{
	static char data[LIST_BUFFER_SIZE]; //2k input buffer
	static int oldlen = 0;
	fd_set set;
	struct timeval timeout = {0,0};
	char *p;
	int len;
	unsigned long ip;
	unsigned short port;

	FD_ZERO(&set);
	FD_SET( serverlist->slsocket, &set);
	if (select(FD_SETSIZE, &set, NULL, NULL, &timeout) <= 0)
		return 0;
	//append to data
	len = recv(serverlist->slsocket, data + oldlen, sizeof(data) - oldlen - 1, 0);
	if (len == SOCKET_ERROR || len == 0)
	{
		closesocket(serverlist->slsocket);
		serverlist->slsocket = INVALID_SOCKET;
		oldlen = 0; //clear data so it can be used again
		ServerListHalt(serverlist);
		ServerListModeChange(serverlist, sl_querying);
		return GE_NOCONNECT;
	}
	oldlen += len;
	p = data;
	while (p - data <= oldlen - 6)
	{
		if (strncmp(p,"\\final\\",7) == 0 || serverlist->abortupdate)
		{ //we are done
			closesocket(serverlist->slsocket);
			serverlist->slsocket = INVALID_SOCKET;
			oldlen = 0; //clear data so it can be used again
			if (serverlist->querytype == qt_grouprooms || serverlist->querytype == qt_masterinfo)
				ServerListModeChange(serverlist, sl_idle);
			else
				ServerListModeChange(serverlist, sl_querying);
			return 0; //get out!!
		}
		if (oldlen < 6) //no way it could be a full IP, quit
			break;
		if (serverlist->querytype == qt_grouprooms || serverlist->querytype == qt_masterinfo)
		{
			len = ServerListParseInfoList(serverlist, p, oldlen - (p - data));
			if (len < 0) //error
				serverlist->abortupdate = 1;
			else if (len == 0) //not enough data
				break;
			else
				p += len;
		} else
		{
			memcpy(&ip,p,4);
			p += 4;
			memcpy(&port,p,2);
			p += 2;
			if (ServerListFindServerMax(serverlist, ip, ntohs(port), serverlist->numoldservers) == -1)
				ServerListAddServer(serverlist, ip, ntohs(port), serverlist->querytype);
		}
	}
	//now, shift the leftover data
	oldlen = oldlen - (p - data);
	memmove(data,p,oldlen);
	return 0;
}

/* ServerListParseInfoList: one item of an info list: the field count,
a field name, or a server's values; the length used, 0 when the item is
not complete, -1 on bad data */
// FUNCTION: STBC 0x6AAF00
static int ServerListParseInfoList(GServerList serverlist, char *data, int len)
{
	char tempfield[64];
	char *tempptr;
	int tlen;
	int i;
	GServer server;

	switch (serverlist->parsestate)
	{
	case 0: //\fieldcount\N\ //
		tlen = BackslashOffset(data, len, 3);
		if (tlen == -1) //not enough data
			return 0;
		if (tlen < 12) //invalid data
			return -1;
		strncpy(tempfield,data + 12, tlen - 12);
		tempfield[tlen - 13] = 0;
		numfields = atoi(tempfield);
		if (numfields > MAX_FIELDS)
			return -1;
		serverlist->parsestate = 1;
		for (i = 0 ; i < numfields ; i++)
			fieldlist[i] = NULL;
		return tlen - 1;
	case 1: //\fieldname//
		tlen = BackslashOffset(data, len, 2);
		if (tlen == -1) //not enough data
			return 0;
		strncpy(tempfield, data + 1, tlen - 2);
		tempfield[tlen - 2] = 0;
		tempptr = _strdup(tempfield);
		TableEnter(serverlist->keylist, &tempptr);
		for (i = 0 ; i < numfields ; i++)
			if (fieldlist[i] == NULL)
			{
				fieldlist[i] = tempptr;
				if (i == numfields - 1) //the last one
					serverlist->parsestate = 2;
				return tlen - 1;
			}
		break; //too many fields
	case 2: //a server's values
		tlen = BackslashOffset(data, len, numfields + 1);
		if (tlen == -1) //not enough data
			return 0;
		server = ServerListAddServerData(serverlist, fieldlist, numfields, data, serverlist->querytype);
		data[tlen - 1] = '\\'; //the parser cut it
		serverlist->CallBackFn(serverlist, LIST_PROGRESS, serverlist->instance, server, 0);
		return tlen - 1;
	}
	return -1;
}

/* ServerListAddServerData: appends a server of an info list */
// FUNCTION: STBC 0x6AB0D0
static GServer ServerListAddServerData(GServerList serverlist, char **fieldlist, int fieldcount, char *serverdata, GQueryType qtype)
{
	GServer server;

	server = ServerNewData(fieldlist, fieldcount, serverdata, qtype, serverlist->keylist);
	ArrayAppend(serverlist->servers,&server);
	return server;
}

/* BackslashOffset: the length of data up to and with its nslashes'th
backslash, or -1 when len doesn't hold that many */
// FUNCTION: STBC 0x6AB110
static int BackslashOffset(char *data, int len, int nslashes)
{
	char *p;

	for (p = data ; nslashes != 0 && p != data + len ; p++)
		if (*p == '\\')
			nslashes--;
	if (nslashes == 0)
		return p - data;
	return -1;
}

/* ServerListQueryLoop: reads the servers' answers, expires the ones that
don't answer and sends the next queries */
// FUNCTION: STBC 0x6AB150
static GError ServerListQueryLoop(GServerList serverlist)
{
	int i, scount = 0, error, final;
	fd_set set;
	struct timeval timeout = {0,0};
	char indata[INDATA_SIZE];
	struct sockaddr_in saddr;
	int saddrlen = sizeof(saddr);
	GServer server;

	//first, check for available data
	FD_ZERO(&set);
	for (i = 0 ; i < serverlist->maxupdates ; i++)
		if (serverlist->updatelist[i].currentserver != NULL) //there is a server waiting
		{
			scount++;
			FD_SET( serverlist->updatelist[i].s, &set);
		}
	if (scount > 0) //there are sockets to check for data
	{
		error = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (SOCKET_ERROR != error && 0 != error)
			for (i = 0 ; i < serverlist->maxupdates ; i++)
				if (serverlist->updatelist[i].currentserver != NULL && FD_ISSET(serverlist->updatelist[i].s, &set) ) //there is a server waiting
				{ //we can read data!
					saddrlen = sizeof(saddr);
					error = recvfrom(serverlist->updatelist[i].s, indata, sizeof(indata) - 1, 0,(struct sockaddr *)&saddr, &saddrlen);
					if (saddr.sin_addr.s_addr != serverlist->updatelist[i].saddr.sin_addr.s_addr ||
						saddr.sin_port != serverlist->updatelist[i].saddr.sin_port)
						continue; //it wasn't from this server
					if (SOCKET_ERROR != error) //we got data
					{
						indata[error] = 0; //truncate and parse it
						final = (strstr(indata,"\\final\\") != NULL);
						server = serverlist->updatelist[i].currentserver;
						if (server->ping == 9999) //set the ping
							server->ping = (short)(current_time() - serverlist->updatelist[i].starttime);
						ServerParseKeyVals(server, indata);
						if (final) //it's all done
						{
							serverlist->CallBackFn(serverlist, LIST_PROGRESS, serverlist->instance, server,
								(void *)((serverlist->nextupdate * 100) / ArrayLength(serverlist->servers))); //percent done
							serverlist->updatelist[i].currentserver = NULL; //reuse the updatelist
						}
					} else
						serverlist->updatelist[i].currentserver = NULL; //reuse the updatelist
				}
	}
	//kill expired ones
	for (i = 0 ; i < serverlist->maxupdates ; i++)
		if (serverlist->updatelist[i].currentserver != NULL && current_time() - serverlist->updatelist[i].starttime > SERVER_TIMEOUT)
			serverlist->updatelist[i].currentserver = NULL; //reuse the updatelist

	if (serverlist->abortupdate || (serverlist->nextupdate >= ArrayLength(serverlist->servers) && scount == 0))
	{ //we are done!!
		FreeUpdateList(serverlist);
		ServerListModeChange(serverlist, sl_idle);
		return 0;
	}

	//now, send out queries on available sockets
	for (i = 0 ; i < serverlist->maxupdates && serverlist->nextupdate < ArrayLength(serverlist->servers) ; i++)
		if (serverlist->updatelist[i].currentserver == NULL) //it's availalbe
		{
			server = *(GServer *)ArrayNth(serverlist->servers,serverlist->nextupdate);
			serverlist->nextupdate++;
			serverlist->updatelist[i].currentserver = server;
			serverlist->updatelist[i].saddr.sin_family = AF_INET;
			serverlist->updatelist[i].saddr.sin_addr.s_addr = inet_addr(ServerGetAddress(server));
			serverlist->updatelist[i].saddr.sin_port = htons((short)ServerGetQueryPort(server));
			sendto(serverlist->updatelist[i].s,querystrings[server->querytype],querylengths[server->querytype],0,(struct sockaddr *)&serverlist->updatelist[i].saddr,sizeof(struct sockaddr_in));
			serverlist->updatelist[i].starttime = current_time();
		}
	return 0;
}

/* FreeUpdateList: closes the query sockets */
// FUNCTION: STBC 0x6AB480
static GError FreeUpdateList(GServerList serverlist)
{
	int i;

	for (i = 0 ; i < serverlist->maxupdates ; i++)
		closesocket(serverlist->updatelist[i].s);
	return 0;
}

/* ServerListHalt: stops the update at its next step */
// FUNCTION: STBC 0x6AB4C0
GError ServerListHalt(GServerList serverlist)
{
	if (serverlist->state != sl_idle)
		serverlist->abortupdate = 1;
	return 0;
}

/* ServerListClear: removes every server and key */
// FUNCTION: STBC 0x6AB4E0
GError ServerListClear(GServerList serverlist)
{
	if (serverlist->state != sl_idle) //can't clear while updating
		return GE_BUSY;
	ArrayFree(serverlist->servers);
	serverlist->servers = ArrayNew(sizeof(GServer), SERVER_GROWBY, ServerFree);
	TableFree(serverlist->keylist);
	serverlist->keylist = TableNew2(sizeof(char *),KEYLIST_BUCKETS,4, GStringHash, GCaseInsensitiveCompare, GStringFree);
	serverlist->numoldservers = 0;
	return 0;
}

/* ServerListState: what the list is doing */
// FUNCTION: STBC 0x6AB550
GServerListState ServerListState(GServerList serverlist)
{
	return serverlist->state;
}

/* ServerListErrorDesc: the text of an error code */
// FUNCTION: STBC 0x6AB560
char *ServerListErrorDesc(GServerList serverlist, GError error)
{
	switch (error)
	{
	case GE_NOERROR:
		return "";
	case GE_NOSOCKET:
		return "Unable to create socket";
	case GE_NODNS:
		return "Unable to resolve master";
	case GE_NOCONNECT:
		return "Connection to master reset";
	case GE_BUSY:
		return "Server List is busy";
	case GE_DATAERROR:
		return "Unexpected data in server list";
	}
	return "UNKNOWN ERROR CODE";
}

/* ServerListGetServer: the index'th server, or NULL */
// FUNCTION: STBC 0x6AB5C0
GServer ServerListGetServer(GServerList serverlist, int index)
{
	if (index < 0 || index >= ArrayLength(serverlist->servers))
		return NULL;
	return *(GServer *)ArrayNth(serverlist->servers,index);
}

/* ServerListCount: the number of servers */
// FUNCTION: STBC 0x6AB600
int ServerListCount(GServerList serverlist)
{
	return ArrayLength(serverlist->servers);
}

/* ServerListSort: sorts the servers by the value of sortkey */
// FUNCTION: STBC 0x6AB620
void ServerListSort(GServerList serverlist, gbool ascending, char *sortkey, GCompareMode comparemode)
{
	ArrayCompareFn comparator;

	switch (comparemode)
	{
	case cm_int:
		comparator = IntKeyCompare;
		break;
	case cm_float:
		comparator = FloatKeyCompare;
		break;
	case cm_strcase:
		comparator = StrCaseKeyCompare;
		break;
	case cm_stricase:
		comparator = StrNoCaseKeyCompare;
		break;
	default:
		comparator = StrNoCaseKeyCompare;
		break;
	}
	serverlist->sortkey = sortkey;
	serverlist->sortascending = ascending;
	g_sortserverlist = serverlist;
	ArraySort(serverlist->servers,comparator);
}

// FUNCTION: STBC 0x6AB690
static int IntKeyCompare(const void *entry1, const void *entry2)
{
	GServer server1 = *(GServer *)entry1;
	GServer server2 = *(GServer *)entry2;
	int diff;

	diff = ServerGetIntValue(server1, g_sortserverlist->sortkey, 0) -
		ServerGetIntValue(server2, g_sortserverlist->sortkey, 0);
	if (!g_sortserverlist->sortascending)
		diff = -diff;
	return diff;
}

// FUNCTION: STBC 0x6AB6F0
static int FloatKeyCompare(const void *entry1, const void *entry2)
{
	GServer server1 = *(GServer *)entry1;
	GServer server2 = *(GServer *)entry2;
	float f;

	f = ServerGetFloatValue(server1, g_sortserverlist->sortkey, 0) -
		ServerGetFloatValue(server2, g_sortserverlist->sortkey, 0);
	if (!g_sortserverlist->sortascending)
		f = -f;
	if (f > 0)
		return 1;
	else if (f < 0)
		return -1;
	else
		return 0;
}

// FUNCTION: STBC 0x6AB780
static int StrCaseKeyCompare(const void *entry1, const void *entry2)
{
	GServer server1 = *(GServer *)entry1;
	GServer server2 = *(GServer *)entry2;
	int diff;

	diff = strcmp(ServerGetStringValue(server1, g_sortserverlist->sortkey, ""),
		ServerGetStringValue(server2, g_sortserverlist->sortkey, ""));
	if (!g_sortserverlist->sortascending)
		diff = -diff;
	return diff;
}

// FUNCTION: STBC 0x6AB810
static int StrNoCaseKeyCompare(const void *entry1, const void *entry2)
{
	GServer server1 = *(GServer *)entry1;
	GServer server2 = *(GServer *)entry2;
	int diff;

	diff = _stricmp(ServerGetStringValue(server1, g_sortserverlist->sortkey, ""),
		ServerGetStringValue(server2, g_sortserverlist->sortkey, ""));
	if (!g_sortserverlist->sortascending)
		diff = -diff;
	return diff;
}
