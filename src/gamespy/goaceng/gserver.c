/******
gserver.c

A server of the goaceng server list (goaceng.h, gserver.h), retail's 2001
version. Fitted to retail; the public names follow the goaceng API, the
file's own helpers keep the names of the goaceng source as far as they are
known (mytok, LookupKey, CheckValidKey, KeyValHashKeyP and the rest), and
ServerParseAddress is invented.
******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "nonport.h"
#include "gserver.h"

#define MULTIPLIER -1664117991

typedef struct
{
	KeyEnumFn EnumFn;
	void *instance;
	HashTable keylist;
} GEnumData;

static char *mytok(char *instr, char delim);
static char *LookupKey(GServer server, char *k);
static void ServerParseAddress(GServer server, char *addr);
static int CheckValidKey(char *key);
static GKeyValuePair *ServerRuleLookup(GServer server, char *key);
static void KeyMapF(void *elem, void *clientData);
static int StringHash(char *s, int numbuckets);
static int KeyValHashKeyP(const void *elem, int numbuckets);
static int KeyValCompareKeyP(const void *entry1, const void *entry2);

/* ServerFree: the servers array's element free function */
// FUNCTION: STBC 0x6A9910
void ServerFree(void *server)
{
	GServer gserver = *(GServer *)server;

	TableFree(gserver->keyvals);
	free(gserver);
}

/* ServerNewData: a server from a line of a list the master sent with the
servers' keys: the values in serverdata are those of the keys in
fieldlist */
// FUNCTION: STBC 0x6A9930
GServer ServerNewData(char **fieldlist, int fieldcount, char *serverdata, GQueryType qtype, HashTable keylist)
{
	GServer server;
	int i = 0;
	char *k, *v;
	char *pos;
	GKeyValuePair kvpair;

	server = ServerNew(0, 0, qtype, keylist);
	v = mytok(++serverdata,'\\');
	while (i < fieldcount)
	{
		k = fieldlist[i];
		if (v == NULL)
			v = "";
		if (strcmp(k,"ip") == 0)
			ServerParseAddress(server,v);
		else if (qtype == qt_grouprooms && strcmp(k,"other") == 0)
		{ //the other keys, with their backslashes sent as 1s
			for (pos = v; *pos; pos++)
				if (*pos == 1)
					*pos = '\\';
			ServerParseKeyVals(server, v);
		} else
		{
			kvpair.key = LookupKey(server, k);
			kvpair.value = LookupKey(server, v);
			TableEnter(server->keyvals, &kvpair);
		}
		i++;
		if (i < fieldcount)
			v = mytok(NULL,'\\');
	}
	return server;
}

/* mytok: strtok on one delimiter that returns empty tokens as NULL */
// FUNCTION: STBC 0x6A9AA0
static char *mytok(char *instr, char delim)
{
	char *result;
	static char *thestr;

	if (instr)
		thestr = instr;
	result=thestr;
	while (*thestr && *thestr != delim)
	{
		thestr++;
	}
	if (thestr == result)
		result = NULL;
	if (*thestr) //not the null term
	{
		*thestr = '\0';
		thestr++;
	}
	return result;
}

/* LookupKey: the list's interned copy of k, adding it when it is new */
// FUNCTION: STBC 0x6A9AF0
static char *LookupKey(GServer server, char *k)
{
	char **keyindex;

	keyindex = (char **)TableLookup(server->keylist,&k);
	if (keyindex != NULL)
		return *keyindex;
	k = _strdup(k);
	TableEnter(server->keylist,&k);
	return k;
}

/* ServerParseAddress: the server's address and query port from "ip:port"
(invented name) */
// FUNCTION: STBC 0x6A9B40
static void ServerParseAddress(GServer server, char *addr)
{
	char *pos;

	pos = strchr(addr,':');
	if (pos == NULL)
		return;
	*pos = '\0';
	server->ip = inet_addr(addr);
	server->port = (unsigned short)atoi(pos + 1);
}

/* ServerNew: a server with no keys yet */
// FUNCTION: STBC 0x6A9B80
GServer ServerNew(unsigned long ip, unsigned short port, GQueryType qtype, HashTable keylist)
{
	GServer server;
	int nBuckets, nChains;

	server = (GServer)malloc(sizeof(struct GServerImplementation));
	server->ip = ip;
	server->port = port;
	server->ping = 9999;
	server->querytype = qtype;
	/* optimize the number of buckets / chains based on the query type */
	switch (qtype)
	{
	case qt_basic:
		nBuckets = 4;
		nChains = 2;
		break;
	case qt_info:
		nBuckets = 6;
		nChains = 2;
		break;
	case qt_players:
	case qt_rules:
		nBuckets = 8;
		nChains = 2;
		break;
	case qt_info_rules:
	case qt_status:
	default:
		nBuckets = 8;
		nChains= 4;
		break;
	}
	server->keyvals = TableNew2(sizeof(GKeyValuePair),nBuckets,nChains, KeyValHashKeyP, KeyValCompareKeyP, NULL);
	server->keylist = keylist;
	return server;
}

/* ServerParseKeyVals: adds the \key\value pairs of a server's reply */
// FUNCTION: STBC 0x6A9C20
void ServerParseKeyVals(GServer server, char *keyvals)
{
	char *k, *v;
	GKeyValuePair kvpair;

	k = mytok(++keyvals,'\\'); //skip over starting backslash
	while (k != NULL)
	{
		v = mytok(NULL,'\\');
		if (v == NULL)
			v = "";
		if (CheckValidKey(k))
		{
			kvpair.key = LookupKey(server, k);
			kvpair.value = LookupKey(server, v);
			TableEnter(server->keyvals, &kvpair);
		}
		k = mytok(NULL,'\\');
	}
}

/* CheckValidKey: false for the keys of the protocol itself */
// FUNCTION: STBC 0x6A9CB0
static int CheckValidKey(char *key)
{
	char *keys_to_skip[] = {"queryid","final"};
	unsigned int i;

	for (i = 0; i < sizeof(keys_to_skip)/sizeof(keys_to_skip[0]); i++)
	{
		if (strcmp(key,keys_to_skip[i]) == 0)
			return 0;
	}
	return 1;
}

/* ServerGetPing: the server's ping in ms, 9999 until it answers */
// FUNCTION: STBC 0x6A9D30
int ServerGetPing(GServer server)
{
	return server->ping;
}

/* ServerGetAddress: the server's address as a dotted string */
// FUNCTION: STBC 0x6A9D40
char *ServerGetAddress(GServer server)
{
	return (char *)inet_ntoa(*(struct in_addr*)&server->ip);
}

/* ServerGetInetAddress: the server's address, network order */
// FUNCTION: STBC 0x6A9D50
unsigned int ServerGetInetAddress(GServer server)
{
	return server->ip;
}

/* ServerGetQueryPort: the server's query port */
// FUNCTION: STBC 0x6A9D60
int ServerGetQueryPort(GServer server)
{
	return server->port;
}

/* ServerGetStringValue: the value of key, or sdefault */
// FUNCTION: STBC 0x6A9D70
char *ServerGetStringValue(GServer server, char *key, char *sdefault)
{
	GKeyValuePair *kv;

	if (strcmp(key,"hostaddr") == 0) //ooh! they want the hostaddr!
		return ServerGetAddress(server);
	kv = ServerRuleLookup(server,key);
	if (!kv)
		return sdefault;
	return kv->value;
}

/* ServerRuleLookup: the key/value pair of key, if the server has it */
// FUNCTION: STBC 0x6A9DF0
static GKeyValuePair *ServerRuleLookup(GServer server, char *key)
{
	GKeyValuePair kvp;
	char **keyindex;

	keyindex = (char **)TableLookup(server->keylist, &key);
	if (keyindex == NULL)
		return NULL; //otherwise, the keylookup wont work
	kvp.key = *keyindex;
	return (GKeyValuePair *)TableLookup(server->keyvals, &kvp);
}

/* ServerGetIntValue: the value of key as an int, or idefault */
// FUNCTION: STBC 0x6A9E30
int ServerGetIntValue(GServer server, char *key, int idefault)
{
	GKeyValuePair *kv;

	if (strcmp(key,"ping") == 0) //ooh! they want the ping!
		return ServerGetPing(server);
	kv = ServerRuleLookup(server,key);
	if (!kv)
		return idefault;
	return atoi(kv->value);
}

/* ServerGetFloatValue: the value of key as a double, or fdefault */
// FUNCTION: STBC 0x6A9EB0
double ServerGetFloatValue(GServer server, char *key, double fdefault)
{
	GKeyValuePair *kv;

	kv = ServerRuleLookup(server,key);
	if (!kv)
		return fdefault;
	return atof(kv->value);
}

/* ServerGetBoolValue: false when the value of key starts with 0, F or N */
// FUNCTION: STBC 0x6A9EE0
gbool ServerGetBoolValue(GServer server, char *key, gbool bdefault)
{
	GKeyValuePair *kv;

	kv = ServerRuleLookup(server,key);
	if (!kv)
		return bdefault;
	if (kv->value[0] == '0' || kv->value[0] == 'F' || kv->value[0] == 'f' ||
		kv->value[0] == 'N' || kv->value[0] == 'n')
		return 0;
	return 1;
}

/* ServerGetPlayerStringValue: the value of key_playernum */
// FUNCTION: STBC 0x6A9F20
char *ServerGetPlayerStringValue(GServer server, int playernum, char *key, char *sdefault)
{
	char newkey[32];

	sprintf(newkey,"%s_%d",key,playernum);
	return ServerGetStringValue(server, newkey, sdefault);
}

// FUNCTION: STBC 0x6A9F60
int ServerGetPlayerIntValue(GServer server, int playernum, char *key, int idefault)
{
	char newkey[32];

	sprintf(newkey,"%s_%d",key,playernum);
	return ServerGetIntValue(server, newkey, idefault);
}

// FUNCTION: STBC 0x6A9FA0
double ServerGetPlayerFloatValue(GServer server, int playernum, char *key, double fdefault)
{
	char newkey[32];

	sprintf(newkey,"%s_%d",key,playernum);
	return ServerGetFloatValue(server, newkey, fdefault);
}

/* ServerEnumKeys: calls KeyFn for each key/value pair of the server */
// FUNCTION: STBC 0x6A9FE0
void ServerEnumKeys(GServer server, KeyEnumFn KeyFn, void *instance)
{
	GEnumData data;

	data.EnumFn = KeyFn;
	data.instance = instance;
	data.keylist = server->keylist;
	TableMap(server->keyvals, KeyMapF, &data);
}

// FUNCTION: STBC 0x6AA020
static void KeyMapF(void *elem, void *clientData)
{
	GKeyValuePair *kv = (GKeyValuePair *)elem;
	GEnumData *ped = (GEnumData *)clientData;

	ped->EnumFn(kv->key, kv->value, ped->instance);
}

/***********
 * UTILITY FUNCTIONS
 **********/

/* GStringHash: the hash of the list's interned strings */
// FUNCTION: STBC 0x6AA040
int GStringHash(const void *elem, int numbuckets)
{
	return StringHash(*(char **)elem, numbuckets);
}

/* StringHash: a case-insensitive string hash */
// FUNCTION: STBC 0x6AA060
static int StringHash(char *s, int numbuckets)
{
	unsigned long hashcode = 0;

	while (*s != 0)
	{
		hashcode = hashcode * MULTIPLIER + tolower(*s);
		s++;
	}
	return (hashcode % numbuckets);
}

// FUNCTION: STBC 0x6AA0A0
static int KeyValHashKeyP(const void *elem, int numbuckets)
{
	return StringHash(((GKeyValuePair *)elem)->key, numbuckets);
}

// FUNCTION: STBC 0x6AA0C0
int GCaseInsensitiveCompare(const void *entry1, const void *entry2)
{
	return _stricmp(*(char **)entry1,*(char **)entry2);
}

/* KeyValCompareKeyP: the keys are interned, so their pointers compare */
// FUNCTION: STBC 0x6AA0E0
static int KeyValCompareKeyP(const void *entry1, const void *entry2)
{
	return ((GKeyValuePair *)entry1)->key - ((GKeyValuePair *)entry2)->key;
}

// FUNCTION: STBC 0x6AA0F0
void GStringFree(void *elem)
{
	free(*(char **)elem);
}
