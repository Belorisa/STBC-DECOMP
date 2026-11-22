/*
===============================================================================

	TGDisconnectMessage.h

	The message that ends a connection: type 5.

	Evidence: the constructor 0x6BF2E0 stores the vtable 0x895A0C, which
	overrides TGMessage's type (0x6BFE70), destructor, Serialize (0x6BF380),
	GetBufferSpaceRequired (0x6BF4E0) and Copy (0x6BF4B0).

===============================================================================
*/

#ifndef __TGDISCONNECTMESSAGE_H__
#define __TGDISCONNECTMESSAGE_H__

#include "TGMessage.h"

// A disconnect message's type byte (TGDisconnectMessage::UnSerialize is
// entry 5 of the network's message types).
#define TGDISCONNECTMESSAGE_TYPE	5

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006bf340 ??_GTGDisconnectMessage@@UAEPAXI@Z
class TGDisconnectMessage : public TGMessage {
public:
					TGDisconnectMessage( void );
					TGDisconnectMessage( const TGDisconnectMessage &message );

	// FUNCTION: STBC 0x006bfe70 ?VirtualSlot00@TGDisconnectMessage@@UAEHXZ @game/TGDisconnectMessage
	virtual int		VirtualSlot00( void ) { return TGDISCONNECTMESSAGE_TYPE; }
	virtual			~TGDisconnectMessage( void );
	virtual int		Serialize( char *buffer, int bufferSize );
	virtual int		GetBufferSpaceRequired( void );
	virtual TGMessage *	Copy( void );

	static TGMessage *	UnSerialize( char * );
	// Hands UnSerialize to the network for the class's type (invented name).
	static void	RegisterMessageType( void );
};

#endif /* !__TGDISCONNECTMESSAGE_H__ */
