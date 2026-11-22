/*
===============================================================================

	TGSocket.cpp

===============================================================================
*/

#include <winsock.h>

#include "TGSocket.h"

// How shutdown closes the socket: no more sends (SD_SEND, winsock2.h).
#define SHUTDOWN_SENDS		1

/*
================
TGSocket::Close
================
*/
// FUNCTION: STBC 0x006c25a0
bool TGSocket::Close( void ) {
	if ( shutdown( handle, SHUTDOWN_SENDS ) == SOCKET_ERROR ) {
		return false;
	}
	return closesocket( handle ) != SOCKET_ERROR ? true : false;
}
