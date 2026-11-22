/*
===============================================================================

	TGGameSpyEvent.h

	The event that hands a GameSpy query to its handlers
	(ET_NETWORK_GAMESPY_MESSAGE): the winsock network sends it for a
	received packet that starts with a backslash, with the query's text and
	the address it came from.

	0x30 bytes. Evidence: the default constructor 0x6C1760 calls TGEvent's,
	stores the vtable 0x895884 and clears the members at 0x28 and 0x2C; the
	load's constructor (0x6B37D0, in the network module's unit) stores only
	the vtable. GetObjType returns 0x867; the class names are
	"TGGameSpyEvent", "_p_TGGameSpyEvent" and "TGGameSpyEventPtr". The
	vtable's slots 12 to 14 are 0x6C17D0, 0x6C18C0 and 0x6C18E0. The winsock
	network's receive (0x6B95F0) fills the event through 0x6C1850 (the
	text, copied) and 0x6C1820 (a 16-byte copy of the sender's address
	from its recvfrom buffer). No string names the members.

===============================================================================
*/

#ifndef __TGGAMESPYEVENT_H__
#define __TGGAMESPYEVENT_H__

#include "TGEvent.h"

struct sockaddr_in;

// GetObjType's value for a TGGameSpyEvent.
#define TGGAMESPYEVENT_TYPE		0x867

// FUNCTION: STBC 0x006b3860 ??_GTGGameSpyEvent@@UAEPAXI@Z @game/TGNetworkModule
class TGGameSpyEvent : public TGEvent {
public:
					TGGameSpyEvent( void );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006b37d0 ??0TGGameSpyEvent@@QAE@J@Z @game/TGNetworkModule
					TGGameSpyEvent( long id ) : TGEvent( id ) {}
	virtual			~TGGameSpyEvent( void );

	// TGObject
	// FUNCTION: STBC 0x006b37f0 ?GetObjType@TGGameSpyEvent@@UAEHXZ @game/TGNetworkModule
	virtual int		GetObjType( void ) { return TGGAMESPYEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x006b3830 ?GetClassName@TGGameSpyEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetClassName( void ) { return "TGGameSpyEvent"; }
	// FUNCTION: STBC 0x006b3840 ?GetSwigTypeName@TGGameSpyEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGGameSpyEvent"; }
	// FUNCTION: STBC 0x006b3850 ?GetPythonClassName@TGGameSpyEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetPythonClassName( void ) { return "TGGameSpyEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	// The event takes the address; the old one is deleted (invented name).
	void			SetAddress( sockaddr_in *address_ );
	// The event keeps a copy of the text (invented name).
	void			SetText( char *text_ );
	// GameSpy::ProcessQueryHandler reads them (invented names).
	sockaddr_in *	GetAddress( void ) { return address; }
	char *			GetText( void ) { return text; }

protected:
	sockaddr_in *	address;			// 0x28 owned: the sender's (invented name)
	char *			text;				// 0x2C owned: the query (invented name)
};

/*
================
TGGameSpyEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x006b3800 ?IsTypeOf@TGGameSpyEvent@@UAE_NH@Z @game/TGNetworkModule
inline bool TGGameSpyEvent::IsTypeOf( int type ) {
	if ( type == TGGAMESPYEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGGAMESPYEVENT_H__ */
