/*
===============================================================================

	TGNameChangeMessage.h

	The message that tells the host a player's new name: type 0, sent
	guaranteed and high-priority. It has no multi-part header, so its size
	field is 14 bits wide.

	Evidence: new_TGNameChangeMessage calls the constructor 0x6BC5B0, which
	stores the vtable 0x89598C (TGMessage's eight slots: its own type 0x6BD100,
	destructor, Serialize, GetBufferSpaceRequired and Copy) and sets the
	guaranteed and high-priority flags.

===============================================================================
*/

#ifndef __TGNAMECHANGEMESSAGE_H__
#define __TGNAMECHANGEMESSAGE_H__

#include "TGMessage.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// A name change's type byte.
#define TGNAMECHANGEMESSAGE_TYPE	0

class TGNameChangeMessage : public TGMessage {
public:
					TGNameChangeMessage( void );
					TGNameChangeMessage( const TGNameChangeMessage &message );

	// FUNCTION: STBC 0x006bd100 ?VirtualSlot00@TGNameChangeMessage@@UAEHXZ @game/TGNameChangeMessage
	virtual int		VirtualSlot00( void ) { return TGNAMECHANGEMESSAGE_TYPE; }
	virtual			~TGNameChangeMessage( void );
	virtual int		Serialize( char *buffer, int bufferSize );
	virtual int		GetBufferSpaceRequired( void );
	virtual TGMessage *	Copy( void );

	static TGMessage *	UnSerialize( char *buffer );
	// Hands UnSerialize to the network for the class's type (invented name).
	static void	RegisterMessageType( void );
};

#endif /* !__TGNAMECHANGEMESSAGE_H__ */
