/*
===============================================================================

	TGConnectMessage.h

	The request a client sends to join a host's game: type 3.

	Evidence: the constructor 0x6BE730 stores the vtable 0x8959EC, which
	overrides TGMessage's type (0x6BF2C0), destructor, Serialize (0x6BE7D0),
	GetBufferSpaceRequired (0x6BE930) and Copy (0x6BE900).

===============================================================================
*/

#ifndef __TGCONNECTMESSAGE_H__
#define __TGCONNECTMESSAGE_H__

#include "TGMessage.h"

// The message's type byte (ReceiveMessages reads a type-3 message from an unknown address as a connect request).
#define TGCONNECTMESSAGE_TYPE	3

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006be790 ??_GTGConnectMessage@@UAEPAXI@Z
class TGConnectMessage : public TGMessage {
public:
					TGConnectMessage( void );
					TGConnectMessage( const TGConnectMessage &message );

	// FUNCTION: STBC 0x006bf2c0 ?VirtualSlot00@TGConnectMessage@@UAEHXZ @game/TGConnectMessage
	virtual int		VirtualSlot00( void ) { return TGCONNECTMESSAGE_TYPE; }
	virtual			~TGConnectMessage( void );
	virtual int		Serialize( char *buffer, int bufferSize );
	virtual int		GetBufferSpaceRequired( void );
	virtual TGMessage *	Copy( void );

	static TGMessage *	UnSerialize( char * );
	// Hands UnSerialize to the network for the class's type (invented name).
	static void	RegisterMessageType( void );
};

#endif /* !__TGCONNECTMESSAGE_H__ */
