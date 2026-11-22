/******
gqueryreporting.h

The GameSpy SDK's query and reporting SDK (qr) as retail's 2001 version
has it: a hosted game answers the players' and the master server's
queries on its port and sends the master server heartbeats.

Fitted to retail (0x6AB880-0x6ACAFF). IGN's 2011 SDK has only the later
generation (qr2/qr2.c); its names are used where the two agree
(get_sockaddrin, gs_encode, gs_encrypt, encode_ct, swap_byte, the qrec
fields). The public names follow the SDK's documented qr_ API.
******/

#ifndef _GQUERYREPORTING_H_
#define _GQUERYREPORTING_H_

#include "nonport.h"

#ifdef __cplusplus
extern "C" {
#endif

// Error codes qr_init and qr_init_socket return.
#define E_GOA_NOERROR		0
#define E_GOA_WSOCKERROR	1
#define E_GOA_BINDERROR		2
#define E_GOA_DNSERROR		3
#define E_GOA_CONNERROR		4

// The host the heartbeats go to (masterserver.txt can name another) and its port.
#define MASTER_PORT 27900

// Fills outbuf (maxlen bytes) with the \key\value pairs of one kind of query.
typedef void (*qr_querycallback_t)(char *outbuf, int maxlen, void *userdata);

typedef struct qr_implementation_s *qr_t;

int qr_init(/*[out]*/qr_t *qrec, const char *ip, int baseport, const char *gamename, const char *secret_key,
			qr_querycallback_t qr_basic_callback,
			qr_querycallback_t qr_info_callback,
			qr_querycallback_t qr_rules_callback,
			qr_querycallback_t qr_players_callback,
			void *userdata);
int qr_init_socket(/*[out]*/qr_t *qrec, SOCKET s, const char *gamename, const char *secret_key,
			qr_querycallback_t qr_basic_callback,
			qr_querycallback_t qr_info_callback,
			qr_querycallback_t qr_rules_callback,
			qr_querycallback_t qr_players_callback,
			void *userdata);
void qr_process_queries(qr_t qrec);
void qr_process_queries_no_heartbeat(qr_t qrec);
void qr_parse_query(qr_t qrec, char *query, struct sockaddr *sender);
void qr_send_statechanged(qr_t qrec);
void qr_send_exiting(qr_t qrec);
void qr_shutdown(qr_t qrec);

// The master server's validation crypt, shared with the server list (goaceng).
void gs_encode(unsigned char *ins, int size, unsigned char *result);
void gs_encrypt(unsigned char *key, int key_len, unsigned char *buffer_ptr, int buffer_len);

#ifdef __cplusplus
}
#endif

#endif
