/*
===============================================================================

	ChatObjectClass.h

	The chat connection the multiplayer menus open: a TGWinsockNetwork
	connected to the address, with the chat's handlers on its message,
	connect and disconnect events. 0x1C bytes, pooled.

	Evidence: the vtable 0x889BF8 (stored by the constructor 0x461220, the
	destructor 0x461310 and the loader's constructor 0x43E950) overrides the
	destructor, GetObjType (0x8147), IsTypeOf, the three class-name getters
	("ChatObjectClass", "_p_ChatObjectClass", "ChatObjectClassPtr") and
	GetClassHandlers; their copies, the loader's constructor and the scalar
	deleting destructor sit in UtopiaModule's unit after its creator.

	Evidence: Disconnect, IsChatObjectReady and SendChatMessage pass through the network at
	0x14 (TGNetwork::Disconnect, GetConnectStatus, SendTGMessage).

===============================================================================
*/

#ifndef __CHATOBJECTCLASS_H__
#define __CHATOBJECTCLASS_H__

#include "TGEventHandlerObject.h"

class TGNetwork;
class TGMessageEvent;

// GetObjType's value for a ChatObjectClass (its creator's type in UtopiaModule's registration).
#define CHATOBJECTCLASS_TYPE	0x8147

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0043ea00 ??_GChatObjectClass@@UAEPAXI@Z @game/UtopiaModule
class ChatObjectClass : public TGEventHandlerObject {
public:
	// FUNCTION: STBC 0x0043e950 ??0ChatObjectClass@@QAE@J@Z @game/UtopiaModule
					ChatObjectClass( long id ) : TGEventHandlerObject( id ) {}	// the loader's: the stream fills the members
					// connects to the address
					ChatObjectClass( char *address );
	virtual			~ChatObjectClass( void );

	// TGObject
	// FUNCTION: STBC 0x0043e970 ?GetObjType@ChatObjectClass@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return CHATOBJECTCLASS_TYPE; }
	// FUNCTION: STBC 0x0043e980 ?IsTypeOf@ChatObjectClass@@UAE_NH@Z @game/UtopiaModule
	virtual bool	IsTypeOf( int type ) {
		if ( type == CHATOBJECTCLASS_TYPE ) {
			return true;
		}
		return TGEventHandlerObject::IsTypeOf( type );
	}
	// FUNCTION: STBC 0x0043e9c0 ?GetClassName@ChatObjectClass@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "ChatObjectClass"; }
	// FUNCTION: STBC 0x0043e9d0 ?GetSwigTypeName@ChatObjectClass@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_ChatObjectClass"; }
	// FUNCTION: STBC 0x0043e9e0 ?GetPythonClassName@ChatObjectClass@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "ChatObjectClassPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0043e9f0 ?GetClassHandlers@ChatObjectClass@@UAEPAVTGEventHandlerTable@@XZ @game/UtopiaModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	// The class's handler methods by name, and in the class's table (invented names).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );
	// The network events this class answers: the received message, and the
	// opening and closing of the connection.
	void	ReceiveMessageHandler( TGMessageEvent *event );
	void	ConnectHandler( TGEvent *event );
	void	DisconnectHandler( TGEvent *event );
	void	Disconnect( void );
	bool	IsChatObjectReady( void );
	void	GetLocalIPAddress( void );
	bool	SendChatMessage( char * );

protected:
	TGNetwork *		network;				// 0x14 the connection the chat goes over (invented name)
	bool			unknown18;				// 0x18 set by DisconnectHandler; ConnectHandler makes the local player only while it is set (invented name)

	// Declared for the SWIG wrappers that call them.
public:
	static ChatObjectClass *	Create( char * );	// retail 0x4613F0: the SWIG wrapper ChatObjectClass_Create calls it

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__CHATOBJECTCLASS_H__ */
