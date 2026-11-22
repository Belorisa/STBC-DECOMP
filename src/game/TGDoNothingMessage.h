/*
===============================================================================

	TGDoNothingMessage.h

	A message that carries nothing but its type byte: type 2.

	Evidence: the constructor 0x6BDC40 stores the vtable 0x8959CC, which
	overrides TGMessage's type (0x6BE710), destructor, Serialize (0x6BDCE0),
	GetBufferSpaceRequired (0x6BDD90) and Copy (0x6BDD60).

===============================================================================
*/

#ifndef __TGDONOTHINGMESSAGE_H__
#define __TGDONOTHINGMESSAGE_H__

#include "TGMessage.h"

// A do-nothing message's type byte (TGDoNothingMessage::UnSerialize is entry 2 of
// TGNetwork's message types).
#define TGDONOTHINGMESSAGE_TYPE		2

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006bdca0 ??_GTGDoNothingMessage@@UAEPAXI@Z
class TGDoNothingMessage : public TGMessage {
public:
					TGDoNothingMessage( void );
					TGDoNothingMessage( const TGDoNothingMessage &message );

	// FUNCTION: STBC 0x006be710 ?VirtualSlot00@TGDoNothingMessage@@UAEHXZ @game/TGDoNothingMessage
	virtual int		VirtualSlot00( void ) { return TGDONOTHINGMESSAGE_TYPE; }
	virtual			~TGDoNothingMessage( void );
	virtual int		Serialize( char *buffer, int bufferSize );
	virtual int		GetBufferSpaceRequired( void );
	virtual TGMessage *	Copy( void );

	static TGMessage *	UnSerialize( char * );
	// Hands UnSerialize to the network for the class's type (invented name).
	static void	RegisterMessageType( void );
};

#endif /* !__TGDONOTHINGMESSAGE_H__ */
