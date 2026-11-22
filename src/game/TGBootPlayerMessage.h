/*
===============================================================================

	TGBootPlayerMessage.h

	The message that tells a client it was removed from the game: type 4,
	sent guaranteed and high-priority, with the boot reason as its first
	data byte.

	Evidence: the constructor 0x6BAC70 stores the vtable 0x89596C, which
	overrides TGMessage's type (0x6BB830), destructor, Serialize (0x6BAD20),
	GetBufferSpaceRequired (0x6BAEA0) and Copy (0x6BAE60).

===============================================================================
*/

#ifndef __TGBOOTPLAYERMESSAGE_H__
#define __TGBOOTPLAYERMESSAGE_H__

#include "TGMessage.h"

// The message's type byte (the network's handler of type 4 reads the boot reason at 0x40).
#define TGBOOTPLAYERMESSAGE_TYPE	4

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006bace0 ??_GTGBootPlayerMessage@@UAEPAXI@Z
class TGBootPlayerMessage : public TGMessage {
public:
					TGBootPlayerMessage( void );
					TGBootPlayerMessage( const TGBootPlayerMessage &message );

	// FUNCTION: STBC 0x006bb830 ?VirtualSlot00@TGBootPlayerMessage@@UAEHXZ @game/TGBootPlayerMessage
	virtual int		VirtualSlot00( void ) { return TGBOOTPLAYERMESSAGE_TYPE; }
	virtual			~TGBootPlayerMessage( void );
	virtual int		Serialize( char *buffer, int bufferSize );
	virtual int		GetBufferSpaceRequired( void );
	virtual TGMessage *	Copy( void );

	static TGMessage *	UnSerialize( char * );
	// Hands UnSerialize to the network for the class's type (invented name).
	static void	RegisterMessageType( void );

	int				GetBootReason( void ) { return bootReason; }
	void			SetBootReason( int reason ) { bootReason = reason; }

protected:
	int				bootReason;			// 0x40 (TGBootPlayerMessage_GetBootReason, SetBootReason; the network's constructor sets 1)
};

#endif /* !__TGBOOTPLAYERMESSAGE_H__ */
