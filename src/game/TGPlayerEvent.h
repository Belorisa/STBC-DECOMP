/*
===============================================================================

	TGPlayerEvent.h

	The event that names a network player by ID: the network sends it when
	a player joins or leaves (ET_NETWORK_NEW_PLAYER, ET_NETWORK_DELETE_PLAYER).

	0x2C bytes. Evidence: the default constructor 0x6BB840 calls TGEvent's,
	stores the vtable 0x895848 and clears the ID at 0x28; the load's
	constructor (0x6B36E0, in the network module's unit) stores only the
	vtable. GetObjType returns 0x866; the class names are "TGPlayerEvent",
	"_p_TGPlayerEvent" and "TGPlayerEventPtr". The vtable's slots 12 to 14
	are 0x6BB870, 0x6BB890 and 0x6BB8B0. The SWIG wrappers
	TGPlayerEvent_GetPlayerID and SetPlayerID are inline: they read and
	write the int at 0x28.

===============================================================================
*/

#ifndef __TGPLAYEREVENT_H__
#define __TGPLAYEREVENT_H__

#include "TGEvent.h"

// GetObjType's value for a TGPlayerEvent.
#define TGPLAYEREVENT_TYPE		0x866

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006b3770 ??_GTGPlayerEvent@@UAEPAXI@Z @game/TGNetworkModule
class TGPlayerEvent : public TGEvent {
public:
					TGPlayerEvent( void );
	// The load's: the members are read after.
	// FUNCTION: STBC 0x006b36e0 ??0TGPlayerEvent@@QAE@J@Z @game/TGNetworkModule
					TGPlayerEvent( long id ) : TGEvent( id ) {}
	virtual			~TGPlayerEvent( void );

	// TGObject
	// FUNCTION: STBC 0x006b3700 ?GetObjType@TGPlayerEvent@@UAEHXZ @game/TGNetworkModule
	virtual int		GetObjType( void ) { return TGPLAYEREVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x006b3740 ?GetClassName@TGPlayerEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetClassName( void ) { return "TGPlayerEvent"; }
	// FUNCTION: STBC 0x006b3750 ?GetSwigTypeName@TGPlayerEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGPlayerEvent"; }
	// FUNCTION: STBC 0x006b3760 ?GetPythonClassName@TGPlayerEvent@@UAEPADXZ @game/TGNetworkModule
	virtual char *	GetPythonClassName( void ) { return "TGPlayerEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	static TGPlayerEvent *	Create( void ) { return new TGPlayerEvent; }

	int				GetPlayerID( void ) { return playerID; }
	void			SetPlayerID( int id ) { playerID = id; }

protected:
	int				playerID;			// 0x28 a byte on the network
};

/*
================
TGPlayerEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x006b3710 ?IsTypeOf@TGPlayerEvent@@UAE_NH@Z @game/TGNetworkModule
inline bool TGPlayerEvent::IsTypeOf( int type ) {
	if ( type == TGPLAYEREVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGPLAYEREVENT_H__ */
