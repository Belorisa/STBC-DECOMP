/******
gserver.h

A server of the goaceng server list: its address, ping and key/value
pairs (see goaceng.h). Fitted to retail.
******/

#ifndef _GSERVER_H_
#define _GSERVER_H_

#include "goaceng.h"
#include "hashtable.h"

#ifdef __cplusplus
extern "C" {
#endif

// 0x14 bytes: ServerNew (0x6A9B80) allocates them.
struct GServerImplementation
{
	unsigned long ip;			// network order
	unsigned short port;		// the query port, host order
	short ping;					// 9999 until the server answers
	GQueryType querytype;
	HashTable keyvals;			// GKeyValuePair, keys and values interned in keylist
	HashTable keylist;			// the list's interned strings (char *)
};

typedef struct
{
	char *key;
	char *value;
} GKeyValuePair;

GServer ServerNew(unsigned long ip, unsigned short port, GQueryType qtype, HashTable keylist);
GServer ServerNewData(char **fieldlist, int fieldcount, char *serverdata, GQueryType qtype, HashTable keylist);
void ServerFree(void *server);
void ServerParseKeyVals(GServer server, char *keyvals);
int GStringHash(const void *elem, int numbuckets);
int GCaseInsensitiveCompare(const void *entry1, const void *entry2);
void GStringFree(void *elem);

#ifdef __cplusplus
}
#endif

#endif
