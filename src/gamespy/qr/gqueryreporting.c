/******
gqueryreporting.c

The GameSpy SDK's query and reporting SDK, retail's 2001 version (see
gqueryreporting.h). Fitted to retail; names follow IGN's 2011 qr2/qr2.c
where the two generations agree.

TG: qr_init and qr_init_socket read the master server's host name from the
first line of masterserver.txt, and the default is Activision's master
server, not GameSpy's.
******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gqueryreporting.h"

#define MASTER_ADDR "stbridgecmnd01.activision.com"
#define MASTER_FILE "masterserver.txt"
#define FIRST_HB_TIME 30000 /* 30 sec */
#define HB_TIME 300000 /* 5 minutes */
#define MAX_FIRST_COUNT 10 /* 10 tries */
#define MAX_DATA_SIZE 1400
#define INBUF_LEN 256
#define NUM_PORTS_TO_TRY 100
#define LOOPBACK_ADDR 0x7F000001

typedef unsigned char uchar;

typedef enum {qtunknown, qtbasic, qtinfo, qtrules, qtplayers, qtstatus, qtpackets, qtecho, qtsecure} query_t;

struct qr_implementation_s
{
	SOCKET querysock;
	SOCKET hbsock;
	char gamename[64];
	char secret_key[64];
	char unknown88[64];
	qr_querycallback_t qr_basic_callback;
	qr_querycallback_t qr_info_callback;
	qr_querycallback_t qr_rules_callback;
	qr_querycallback_t qr_players_callback;
	unsigned long lastheartbeat;
	int queryid;
	int packetnumber;
	int qport;		// 0 when the caller owns the socket and passes the queries to qr_parse_query
	char no_query;	// counts the early heartbeats sent while no query came
	void *udata;
};

// DATA: STBC 0x95A718
static char *queries[]={"","basic","info","rules","players","status","packets","echo","secure"};
// DATA: STBC 0x95A740
static struct qr_implementation_s static_rec = {INVALID_SOCKET, INVALID_SOCKET};
// DATA: STBC 0x95A830
static qr_t current_rec = &static_rec;
// DATA: STBC 0x95A834
char qr_hostname[64] = MASTER_ADDR;
// DATA: STBC 0x995880
static struct sockaddr_in hbaddr;

static void init_qrec(qr_t *qrec, int baseport, SOCKET hbsock, SOCKET querysock, const char *gamename, const char *secret_key,
			qr_querycallback_t qr_basic_callback,
			qr_querycallback_t qr_info_callback,
			qr_querycallback_t qr_rules_callback,
			qr_querycallback_t qr_players_callback,
			void *userdata);
static void qr_check_queries(qr_t qrec);
static void check_send_heartbeat(qr_t qrec);
static int do_connect(SOCKET sock, char *addr, int port);
static int get_sockaddrin(const char *host, int port, struct sockaddr_in *saddr, struct hostent **savehent);
static char *value_for_key(const char *s, const char *key);
static void packet_send(qr_t qrec, struct sockaddr *addr, char *buffer);
static void buffer_send(qr_t qrec, struct sockaddr *sender, char *buffer, char *newdata);
static void send_basic(qr_t qrec, struct sockaddr *sender, char *outbuf);
static void send_info(qr_t qrec, struct sockaddr *sender, char *outbuf);
static void send_rules(qr_t qrec, struct sockaddr *sender, char *outbuf);
static void send_players(qr_t qrec, struct sockaddr *sender, char *outbuf);
static void send_echo(qr_t qrec, struct sockaddr *sender, char *outbuf, char *echostr);
static void send_final(qr_t qrec, struct sockaddr *sender, char *outbuf, char *validation);
static void send_heartbeat(qr_t qrec, int statechanged);

/****************************************************************************/
/* PUBLIC FUNCTIONS */
/****************************************************************************/

/* qr_init: Initializes the sockets, etc. Returns an error value
if an error occured, or 0 otherwise */
// FUNCTION: STBC 0x6AB880
int qr_init(/*[out]*/qr_t *qrec, const char *ip, int baseport, const char *gamename, const char *secret_key,
			qr_querycallback_t qr_basic_callback,
			qr_querycallback_t qr_info_callback,
			qr_querycallback_t qr_rules_callback,
			qr_querycallback_t qr_players_callback,
			void *userdata)
{
	struct sockaddr_in saddr;
	SOCKET hbsock;
	int maxport;
	int lasterror; // BUG: read uninitialised below when the port range is empty
	int saddrlen;
	char hostname[256];
	FILE *f;
	int len;

	if (qrec != NULL) //init it to empty
		*qrec = NULL;
	SocketStartUp();
	hbsock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET != hbsock)
	{
		maxport = baseport + NUM_PORTS_TO_TRY;
		for (; baseport < maxport; baseport++)
		{
			get_sockaddrin(ip,baseport,&saddr,NULL);
			if (saddr.sin_addr.s_addr == htonl(LOOPBACK_ADDR)) //localhost -- we don't want that!
				saddr.sin_addr.s_addr = INADDR_ANY;

			lasterror = bind(hbsock, (struct sockaddr *)&saddr, sizeof(saddr));
			if (lasterror == 0)
				break; //we found a port
		}
		if (lasterror != 0) //we weren't able to find a port
			return E_GOA_BINDERROR;

		if (baseport == 0) //we bound it dynamically
		{
			saddrlen = sizeof(saddr);
			lasterror = getsockname(hbsock,(struct sockaddr *)&saddr, &saddrlen);
			if (lasterror)
				return E_GOA_BINDERROR;
			baseport = ntohs(saddr.sin_port);
		}

		init_qrec(qrec, baseport, hbsock, hbsock, gamename, secret_key, qr_basic_callback,
			qr_info_callback, qr_rules_callback, qr_players_callback, userdata);

		// TG: the master server's host name from masterserver.txt
		f = fopen(MASTER_FILE, "rt");
		if (f != NULL && fgets(hostname, sizeof(hostname), f) != NULL)
		{
			strncpy(qr_hostname, hostname, sizeof(qr_hostname));
			len = strlen(qr_hostname);
			if (qr_hostname[len - 1] == '\n')
				qr_hostname[len - 1] = '\0';
		} else
			strcpy(qr_hostname, MASTER_ADDR);

		return do_connect(hbsock, qr_hostname, MASTER_PORT);
	}
	return E_GOA_WSOCKERROR;
}

// FUNCTION: STBC 0x6ABA60
static void init_qrec(qr_t *qrec, int baseport, SOCKET hbsock, SOCKET querysock, const char *gamename, const char *secret_key,
			qr_querycallback_t qr_basic_callback,
			qr_querycallback_t qr_info_callback,
			qr_querycallback_t qr_rules_callback,
			qr_querycallback_t qr_players_callback,
			void *userdata)
{
	if (qrec == NULL)
	{
		current_rec = &static_rec;
	} else
	{
		current_rec = (qr_t)malloc(sizeof(struct qr_implementation_s));
		*qrec = current_rec;
	}
	strcpy(current_rec->gamename,gamename);
	strcpy(current_rec->secret_key,secret_key);
	current_rec->qport = baseport;
	current_rec->lastheartbeat = 0;
	current_rec->hbsock = hbsock;
	current_rec->querysock = querysock;
	current_rec->packetnumber = 0;
	current_rec->queryid = 1;
	current_rec->no_query = 1;
	current_rec->udata = userdata;
	current_rec->qr_basic_callback = qr_basic_callback;
	current_rec->qr_info_callback = qr_info_callback;
	current_rec->qr_players_callback = qr_players_callback;
	current_rec->qr_rules_callback = qr_rules_callback;
}

/* qr_init_socket: as qr_init, on a socket the caller owns: the queries
that arrive on it are passed to qr_parse_query */
// FUNCTION: STBC 0x6ABBA0
int qr_init_socket(/*[out]*/qr_t *qrec, SOCKET s, const char *gamename, const char *secret_key,
			qr_querycallback_t qr_basic_callback,
			qr_querycallback_t qr_info_callback,
			qr_querycallback_t qr_rules_callback,
			qr_querycallback_t qr_players_callback,
			void *userdata)
{
	char hostname[256];
	FILE *f;
	int len;

	if (qrec != NULL) //init it to empty
		*qrec = NULL;
	SocketStartUp();

	init_qrec(qrec, 0, s, s, gamename, secret_key, qr_basic_callback,
		qr_info_callback, qr_rules_callback, qr_players_callback, userdata);

	// TG: the master server's host name from masterserver.txt
	f = fopen(MASTER_FILE, "rt");
	if (f != NULL && fgets(hostname, sizeof(hostname), f) != NULL)
	{
		strncpy(qr_hostname, hostname, sizeof(qr_hostname));
		len = strlen(qr_hostname);
		if (qr_hostname[len - 1] == '\n')
			qr_hostname[len - 1] = '\0';
	} else
		strcpy(qr_hostname, MASTER_ADDR);

	return do_connect(s, qr_hostname, MASTER_PORT);
}

/* qr_process_queries: Processes any waiting queries, and sends a
heartbeat if needed */
// FUNCTION: STBC 0x6ABCA0
void qr_process_queries(qr_t qrec)
{
	if (qrec == NULL)
		qrec = current_rec;
	check_send_heartbeat(qrec);
	qr_check_queries(qrec);
}

/* qr_process_queries_no_heartbeat: Processes any waiting queries,
without sending heartbeats */
// FUNCTION: STBC 0x6ABCC0
void qr_process_queries_no_heartbeat(qr_t qrec)
{
	if (qrec == NULL)
		qrec = current_rec;
	qr_check_queries(qrec);
}

/* qr_check_queries: Reads the queries waiting on the query socket and
answers them */
// FUNCTION: STBC 0x6ABCE0
static void qr_check_queries(qr_t qrec)
{
	static char indata[INBUF_LEN]; //256 byte input buffer
	struct sockaddr_in saddr;
	int saddrlen = sizeof(struct sockaddr_in);
	fd_set set;
	struct timeval timeout = {0,0};
	int error;

	if (qrec->qport == 0) //the caller reads the socket
		return;
	FD_ZERO(&set);
	FD_SET(qrec->querysock, &set);
	while (1)
	{
		error = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (SOCKET_ERROR == error || 0 == error)
			return;
		//else we have data
		error = recvfrom(qrec->querysock, indata, INBUF_LEN - 1, 0, (struct sockaddr *)&saddr, &saddrlen);
		if (error != SOCKET_ERROR)
		{
			indata[error] = '\0';
			qr_parse_query(qrec, indata, (struct sockaddr *)&saddr);
		}
	}
}

/* check_send_heartbeat: Perform any scheduled outgoing
heartbeats, (every HB_TIME ms) */
// FUNCTION: STBC 0x6ABD80
static void check_send_heartbeat(qr_t qrec)
{
	unsigned long tc = current_time();

	if (INVALID_SOCKET == qrec->hbsock)
		return; //no sockets to work with!

	//check if we need to send a heartbet
	if (tc - qrec->lastheartbeat > HB_TIME || qrec->lastheartbeat == 0 || tc < qrec->lastheartbeat)
		send_heartbeat(qrec, 0);
	else if (qrec->no_query > 0 && tc - qrec->lastheartbeat > FIRST_HB_TIME)
	{ //check to see if we haven't gotten a query yet
		send_heartbeat(qrec, 0);
		qrec->no_query++;
		if (qrec->no_query > MAX_FIRST_COUNT)
			qrec->no_query = 0; //stop trying!
	}
}

/* qr_send_statechanged: Sends a statechanged heartbeat, call when
your gamemode changes */
// FUNCTION: STBC 0x6ABE00
void qr_send_statechanged(qr_t qrec)
{
	if (qrec == NULL)
		qrec = current_rec;
	send_heartbeat(qrec, 1);
}

/* qr_send_exiting: Sends the heartbeat that tells the master server the
game is going away */
// FUNCTION: STBC 0x6ABE20
void qr_send_exiting(qr_t qrec)
{
	if (qrec == NULL)
		qrec = current_rec;
	send_heartbeat(qrec, 2);
}

/* qr_shutdown: Cleans up the sockets and shuts down */
// FUNCTION: STBC 0x6ABE40
void qr_shutdown(qr_t qrec)
{
	if (qrec == NULL)
		qrec = current_rec;
	if (INVALID_SOCKET != qrec->querysock && qrec->qport != 0)
		closesocket(qrec->querysock);
	if (INVALID_SOCKET != qrec->hbsock && qrec->hbsock != qrec->querysock)
		closesocket(qrec->hbsock);
	qrec->hbsock = INVALID_SOCKET;
	qrec->querysock = INVALID_SOCKET;
	qrec->lastheartbeat = 0;
	if (qrec != &static_rec) //need to free it, it was dynamically allocated
		free(qrec);
	SocketShutDown();
}

/* do_connect: Resolves the master server's address for the heartbeats */
// FUNCTION: STBC 0x6ABEB0
static int do_connect(SOCKET sock, char *addr, int port)
{
	get_sockaddrin(addr,port,&hbaddr,NULL);
	return 0;
}

/* Return a sockaddrin for the given host (numeric or DNS) and port)
Returns the hostent in savehent if it is not NULL */
// FUNCTION: STBC 0x6ABED0
static int get_sockaddrin(const char *host, int port, struct sockaddr_in *saddr, struct hostent **savehent)
{
	struct hostent *hent;

	saddr->sin_family = AF_INET;
	saddr->sin_port = htons((unsigned short)port);
	if (host == NULL)
		saddr->sin_addr.s_addr = INADDR_ANY;
	else
		saddr->sin_addr.s_addr = inet_addr(host);

	if (saddr->sin_addr.s_addr == INADDR_NONE && strcmp(host,"255.255.255.255") != 0)
	{
		hent = gethostbyname(host);
		if (!hent)
			return 0;
		saddr->sin_addr.s_addr = *(unsigned int *)hent->h_addr_list[0];
	}
	if (savehent != NULL)
		*savehent = hent;
	return 1;
}

/*****************************************************************************/
/* Various encryption / encoding routines */

static void swap_byte ( uchar *a, uchar *b );
static uchar encode_ct ( uchar c );

// FUNCTION: STBC 0x6ABF70
void gs_encode ( uchar *ins, int size, uchar *result )
{
	int    i,pos;
	uchar  trip[3];
	uchar  kwart[4];

	i=0;
	while (i < size)
	{
		for (pos=0 ; pos <= 2 ; pos++, i++)
			if (i < size) trip[pos] = *ins++;
			else trip[pos] = '\0';
			kwart[0] = (unsigned char)(  (trip[0])       >> 2);
			kwart[1] = (unsigned char)((((trip[0]) &  3) << 4) + ((trip[1]) >> 4));
			kwart[2] = (unsigned char)((((trip[1]) & 15) << 2) + ((trip[2]) >> 6));
			kwart[3] = (unsigned char)(  (trip[2]) & 63);
			for (pos=0; pos <= 3; pos++) *result++ = encode_ct(kwart[pos]);
	}
	*result='\0';
}

// FUNCTION: STBC 0x6AC020
static uchar encode_ct ( uchar c )
{
	if (c <  26) return (uchar)('A'+c);
	if (c <  52) return (uchar)('a'+c-26);
	if (c <  62) return (uchar)('0'+c-52);
	if (c == 62) return (uchar)('+');
	if (c == 63) return (uchar)('/');

	return 0;
}

// FUNCTION: STBC 0x6AC050
void gs_encrypt ( uchar *key, int key_len, uchar *buffer_ptr, int buffer_len )
{
	short counter;
	uchar x, y, xorIndex;
	uchar state[256];

	for ( counter = 0; counter < 256; counter++) state[counter] = (uchar) counter;

	x = 0; y = 0;
	for ( counter = 0; counter < 256; counter++)
	{
		y = (uchar)((key[x] + state[counter] + y) % 256);
		x = (uchar)((x + 1) % key_len);
		swap_byte ( &state[counter], &state[y] );
	}

	x = 0; y = 0;
	for ( counter = 0; counter < buffer_len; counter ++)
	{
		x = (uchar)((x + buffer_ptr[counter] + 1) % 256);
		y = (uchar)((state[x] + y) % 256);
		swap_byte ( &state[x], &state[y] );
		xorIndex = (uchar)((state[x] + state[y]) % 256);
		buffer_ptr[counter] ^= state[xorIndex];
	}
}

// FUNCTION: STBC 0x6AC1C0
static void swap_byte ( uchar *a, uchar *b )
{
	uchar swapByte;

	swapByte = *a;
	*a = *b;
	*b = swapByte;
}

/*****************************************************************************/

/* qr_parse_query: parse an incoming query and reply to each part
*/
// FUNCTION: STBC 0x6AC1E0
void qr_parse_query(qr_t qrec, char *query, struct sockaddr *sender)
{
	query_t querytype;
	char buffer[MAX_DATA_SIZE]="";
	char validation[256]="";
	char *value;

	if (qrec == NULL)
		qrec = current_rec;
	qrec->queryid++;
	qrec->packetnumber = 0;
	if (qrec->no_query > 0)
		qrec->no_query = 0;
	for (querytype = qtbasic; querytype <= qtsecure; querytype++)
	{
		if ((value = value_for_key(query,queries[querytype])) != NULL)
		switch (querytype)
		{
		case qtbasic:
			send_basic(qrec,sender,buffer);
			break;
		case qtinfo:
			send_info(qrec,sender,buffer);
			break;
		case qtrules:
			send_rules(qrec,sender,buffer);
			break;
		case qtplayers:
			send_players(qrec,sender,buffer);
			break;
		case qtstatus:
			send_basic(qrec,sender,buffer);
			send_info(qrec,sender,buffer);
			send_rules(qrec,sender,buffer);
			send_players(qrec,sender,buffer);
			break;
		case qtpackets:
			/*note: "packets" is NOT a real query type!
			it is simply here to illustrate how a large query might be broken up
			into multiple packets. */
			send_basic(qrec,sender,buffer);
			packet_send(qrec,sender,buffer);
			send_info(qrec,sender,buffer);
			packet_send(qrec,sender,buffer);
			send_rules(qrec,sender,buffer);
			packet_send(qrec,sender,buffer);
			send_players(qrec,sender,buffer);
			break;
		case qtecho:
			//note: \echo\whatever is used by the client to test the response time
			send_echo(qrec,sender,buffer,value);
			break;
		case qtsecure:
			strcpy(validation, value);
			break;
		case qtunknown:
			break;
		}
	}
	send_final(qrec,sender,buffer,validation);
}

/* value_for_key: this returns a value for a certain key in s, where s is a string
containing key\value pairs. If the key does not exist, it returns NULL.
Note: the value is stored in a common buffer. If you want to keep it, make a copy! */
// FUNCTION: STBC 0x6AC450
static char *value_for_key(const char *s, const char *key)
{
	static int valueindex = 0;
	char *pos,*pos2;
	char keyspec[256]="\\";
	static char value[2][256];

	valueindex ^= 1;
	strcat(keyspec,key);
	strcat(keyspec,"\\");
	pos = strstr(s,keyspec);
	if (!pos)
		return NULL;
	pos += strlen(keyspec);
	pos2 = value[valueindex];
	while (*pos && *pos != '\\')
		*pos2++ = *pos++;
	*pos2 = '\0';
	return value[valueindex];
}

/*packet_send: sends a key\value packet. Appends the queryid
key\value pair. Clears the buffer */
// FUNCTION: STBC 0x6AC550
static void packet_send(qr_t qrec, struct sockaddr *addr, char *buffer)
{
	char keyvalue[80];

	if (strlen(buffer) == 0)
		return; //dont need to send an empty one!
	qrec->packetnumber++; //packet numbers start at 1
	sprintf(keyvalue,"\\queryid\\%d.%d",qrec->queryid, qrec->packetnumber);
	strcat(buffer,keyvalue);
	sendto(qrec->querysock, buffer, strlen(buffer), 0, addr, sizeof(struct sockaddr_in));
	buffer[0]='\0';
}

/* send_basic: sends a response to the basic query */
// FUNCTION: STBC 0x6AC5F0
static void send_basic(qr_t qrec, struct sockaddr *sender, char *outbuf)
{
	char keyvalue[MAX_DATA_SIZE] = "";

	qrec->qr_basic_callback(keyvalue, sizeof(keyvalue), qrec->udata);
	buffer_send(qrec, sender, outbuf, keyvalue);
}

/*buffer_send: appends the newdata to the outbuffer, sending the
outbuffer first if the new data would overflow it */
// FUNCTION: STBC 0x6AC660
static void buffer_send(qr_t qrec, struct sockaddr *sender, char *buffer, char *newdata)
{
	char *pos;
	char *lastkey;
	int bcount = 0;

	if (strlen(buffer) + strlen(newdata) < MAX_DATA_SIZE - 50)
	{
		strcat(buffer,newdata);
	} else
	{
		if (strlen(newdata) > MAX_DATA_SIZE - 50) //incoming data is too big already!
		{
			lastkey = pos = newdata;
			while (pos - newdata < MAX_DATA_SIZE - 50)
			{
				if ('\\' == *pos)
				{
					if (bcount % 2 == 0)
						lastkey = pos;
					bcount++;
				}
				pos++;
			}
			if (lastkey == newdata)
				return; //no good!
			*(lastkey) = '\0';
			buffer_send(qrec, sender, buffer, newdata);
			*(lastkey) = '\\';
			buffer_send(qrec, sender, buffer, lastkey); //send the rest!
		} else
		{
			packet_send(qrec, sender, buffer);
			strcpy(buffer,newdata);
		}
	}
}

/* send_info: sends a response to the info query */
// FUNCTION: STBC 0x6AC7A0
static void send_info(qr_t qrec, struct sockaddr *sender, char *outbuf)
{
	char keyvalue[MAX_DATA_SIZE] = "";

	qrec->qr_info_callback(keyvalue, sizeof(keyvalue), qrec->udata);
	buffer_send(qrec, sender, outbuf, keyvalue);
}

/* send_rules: sends a response to the rules query. */
// FUNCTION: STBC 0x6AC810
static void send_rules(qr_t qrec, struct sockaddr *sender, char *outbuf)
{
	char keyvalue[MAX_DATA_SIZE] = "";

	qrec->qr_rules_callback(keyvalue, sizeof(keyvalue), qrec->udata);
	buffer_send(qrec, sender, outbuf, keyvalue);
}

/* send_players: sends the players and their information.*/
// FUNCTION: STBC 0x6AC880
static void send_players(qr_t qrec, struct sockaddr *sender, char *outbuf)
{
	char keyvalue[MAX_DATA_SIZE] = "";

	qrec->qr_players_callback(keyvalue, sizeof(keyvalue), qrec->udata);
	buffer_send(qrec, sender, outbuf, keyvalue);
}

/* send_echo: bounces the echostr back to sender
Note: you should always make sure that your echostr doesn't exceed the MAX_DATA_SIZE*/
// FUNCTION: STBC 0x6AC8F0
static void send_echo(qr_t qrec, struct sockaddr *sender, char *outbuf, char *echostr)
{
	char keyvalue[MAX_DATA_SIZE];

	if (strlen(echostr) > MAX_DATA_SIZE - 50)
		return;
	sprintf(keyvalue,"\\echo\\%s",echostr);
	buffer_send(qrec, sender, outbuf, keyvalue);
}

/* send_final: sends the remaining data in outbuf. Appends the final
key\value to the end. Also adds validation if required. */
// FUNCTION: STBC 0x6AC950
static void send_final(qr_t qrec, struct sockaddr *sender, char *outbuf, char *validation)
{
	char keyvalue[256];
	char encrypted_val[128]; //don't need to null terminate
	char encoded_val[200];
	int keylen;

	if (validation[0])
	{
		keylen = strlen(validation);
		if (keylen > 128)
			return;
		strcpy(encrypted_val, validation);
		gs_encrypt((uchar *)qrec->secret_key, strlen(qrec->secret_key), (uchar *)encrypted_val, keylen);
		gs_encode((uchar *)encrypted_val,keylen, (uchar *)encoded_val);
		sprintf(keyvalue,"\\validate\\%s",encoded_val);
		buffer_send(qrec, sender, outbuf, keyvalue);
	}
	sprintf(keyvalue,"\\final\\");
	buffer_send(qrec, sender, outbuf, keyvalue);
	packet_send(qrec, sender, outbuf);
}

/* send_heartbeat: Sends a heartbeat to the gamemaster,
adds \statechanged\ if statechanged != 0 */
// FUNCTION: STBC 0x6ACA60
static void send_heartbeat(qr_t qrec, int statechanged)
{
	char buf[256];

	sprintf(buf,"\\heartbeat\\%d\\gamename\\%s",qrec->qport, qrec->gamename);
	if (statechanged)
		sprintf(buf+strlen(buf),"\\statechanged\\%d",statechanged);
	sendto(qrec->hbsock, buf, strlen(buf), 0, (struct sockaddr *)&hbaddr, sizeof(struct sockaddr_in));
	qrec->lastheartbeat = current_time();
}
