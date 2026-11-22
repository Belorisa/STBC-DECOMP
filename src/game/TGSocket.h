/*
===============================================================================

	TGSocket.h

	The socket a TGWinsockNetwork sends and receives through. The name is
	invented.

	Evidence: TGWinsockNetwork's constructor 0x6B9BF0 stores the vtable
	0x895968 at 0x190 and -1 (INVALID_SOCKET) at 0x194; the vtable's one
	slot is the scalar deleting destructor 0x6BAC20, which stores the vtable
	again and calls 0x6C25A0 (shutdown and closesocket of the handle at 0x04).
	TGWinsockNetwork's destructor 0x6B9C80 expands the same destructor.

===============================================================================
*/

#ifndef __TGSOCKET_H__
#define __TGSOCKET_H__

#include "TGPoolManager.h"

// FUNCTION: STBC 0x006bac20 ??_GTGSocket@@UAEPAXI@Z @game/TGWinsockNetwork
class TGSocket {
public:
	// INVALID_SOCKET (winsock.h stays out of the game headers).
	enum { NO_SOCKET = -1 };

					TGSocket( void ) { handle = NO_SOCKET; }
	virtual			~TGSocket( void ) { Close(); }

	// Shuts the socket down and closes it; false when either fails.
	bool			Close( void );

	unsigned int	handle;			// 0x04 the SOCKET
};

#endif /* !__TGSOCKET_H__ */
