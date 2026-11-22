/*
===============================================================================

	TGMessageEvent.h

	The event that hands a received network message to its handlers
	(ET_NETWORK_MESSAGE_EVENT). The event owns the message.

	0x2C bytes. Evidence: the default constructor 0x6BFE80 calls TGEvent's,
	stores the vtable 0x89580C and clears the message at 0x28; the load's
	constructor (0x6B35F0, in the network module's unit) stores only the
	vtable. GetObjType returns 0x865; the class names are "TGMessageEvent",
	"_p_TGMessageEvent" and "TGMessageEventPtr". The vtable's slot 12 is
	Copy (0x6BFF00); slots 13 and 14 are TGEvent's. The SWIG wrappers
	TGMessageEvent_* call Create, GetMessage and SetMessage.

===============================================================================
*/

#ifndef __TGMESSAGEEVENT_H__
#define __TGMESSAGEEVENT_H__

#include "TGEvent.h"

class TGMessage;

// GetObjType's value for a TGMessageEvent.
#define TGMESSAGEEVENT_TYPE		0x865

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006b3680 ??_GTGMessageEvent@@UAEPAXI@Z @game/TGNetworkModule
class TGMessageEvent : public TGEvent {
public:
					TGMessageEvent( void );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006b35f0 ??0TGMessageEvent@@QAE@J@Z @game/TGNetworkModule
					TGMessageEvent( long id ) : TGEvent( id ) {}
	virtual			~TGMessageEvent( void );

	// TGObject
	// FUNCTION: STBC 0x006b3610 ?GetObjType@TGMessageEvent@@UAEHXZ @game/TGNetworkModule
	virtual int		GetObjType( void ) { return TGMESSAGEEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x006b3650 ?GetClassName@TGMessageEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetClassName( void ) { return "TGMessageEvent"; }
	// FUNCTION: STBC 0x006b3660 ?GetSwigTypeName@TGMessageEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGMessageEvent"; }
	// FUNCTION: STBC 0x006b3670 ?GetPythonClassName@TGMessageEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetPythonClassName( void ) { return "TGMessageEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	static TGMessageEvent *	Create( void ) { return new TGMessageEvent; }

	TGMessage *		GetMessage( void ) { return message; }
	void			SetMessage( TGMessage * );

protected:
	TGMessage *		message;			// 0x28 owned (invented name)
};

/*
================
TGMessageEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x006b3620 ?IsTypeOf@TGMessageEvent@@UAE_NH@Z @game/TGNetworkModule
inline bool TGMessageEvent::IsTypeOf( int type ) {
	if ( type == TGMESSAGEEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGMESSAGEEVENT_H__ */
