/*
===============================================================================

	TGAckMessage.h

	The acknowledgement of a guaranteed message: type 1, carrying the
	acknowledged sequence number and its multi-part position.

	Evidence: the constructor 0x6BD120 stores the vtable 0x8959AC, which
	overrides TGMessage's type (0x6BDC20), destructor, Serialize (0x6BD190),
	GetBufferSpaceRequired (0x6BD2A0) and Copy (0x6BD270).

===============================================================================
*/

#ifndef __TGACKMESSAGE_H__
#define __TGACKMESSAGE_H__

#include "TGMessage.h"

// The message's type byte (its slot 0, 0x6BDC20, returns 1).
#define TGACKMESSAGE_TYPE	1

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006bd150 ??_GTGAckMessage@@UAEPAXI@Z
class TGAckMessage : public TGMessage {
public:
					TGAckMessage( void );
					TGAckMessage( const TGAckMessage &message );

	// FUNCTION: STBC 0x006bdc20 ?VirtualSlot00@TGAckMessage@@UAEHXZ @game/TGAckMessage
	virtual int		VirtualSlot00( void ) { return TGACKMESSAGE_TYPE; }
	virtual			~TGAckMessage( void );
	virtual int		Serialize( char *buffer, int bufferSize );
	virtual int		GetBufferSpaceRequired( void );
	virtual TGMessage *	Copy( void );

	static TGMessage *	UnSerialize( char * );
	// Hands UnSerialize to the network for the class's type (invented name).
	static void	RegisterMessageType( void );

	// Whether the acknowledged message was a system message (a type below
	// TGMESSAGE_TYPE); system and user messages are numbered apart.
	bool			IsSystemMessage( void ) { return systemMessage; }
	void			SetSystemMessage( bool system ) { systemMessage = system; }

protected:
	bool			systemMessage;		// 0x40 (TGAckMessage_IsSystemMessage, SetSystemMessage)
};

#endif /* !__TGACKMESSAGE_H__ */
