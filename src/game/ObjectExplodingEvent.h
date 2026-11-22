/*
===============================================================================

	ObjectExplodingEvent.h

	The event a dying object sends as it starts to explode: the network ID
	of the player whose weapon destroyed it and how long the explosion
	lasts.

	0x30 bytes, allocated from the pools.

	Evidence: the class strings "ObjectExplodingEvent",
	"_p_ObjectExplodingEvent" and "ObjectExplodingEventPtr"; GetObjType
	(0x43F8E0) returns 0x8129, the type the game's class registration
	files the creator 0x43F880 under. The vtable 0x88A178 overrides
	TGEvent's type, name and stream slots and both network stream slots.
	The SWIG wrappers ObjectExplodingEvent_Set/GetFiringPlayerID and
	Set/GetLifetime write and read 0x28 and 0x2C. The constructor
	(0x43F8B0) clears both; ShipClass's death (0x5AFEA0) makes the event
	with it. The destructor is the implicit one (a bare jump to
	TGEvent's). Every function is inline: the out-of-line copies are
	those of the class registration's unit.

===============================================================================
*/

#ifndef __OBJECTEXPLODINGEVENT_H__
#define __OBJECTEXPLODINGEVENT_H__

#include "TGEvent.h"
#include "TGStream.h"

// GetObjType's value for an ObjectExplodingEvent.
#define OBJECTEXPLODINGEVENT_TYPE	0x8129

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0043f950 ??_GObjectExplodingEvent@@UAEPAXI@Z @game/UtopiaModule
// FUNCTION: STBC 0x0043f980 ??1ObjectExplodingEvent@@UAE@XZ @game/UtopiaModule
class ObjectExplodingEvent : public TGEvent {
public:
	// FUNCTION: STBC 0x0043f8b0 ??0ObjectExplodingEvent@@QAE@J@Z @game/UtopiaModule
					ObjectExplodingEvent( long id = 0 ) : TGEvent( id ) { firingPlayerID = 0; lifetime = 0.0f; }

	// TGObject
	// FUNCTION: STBC 0x0043f8e0 ?GetObjType@ObjectExplodingEvent@@UAEHXZ @game/UtopiaModule
	virtual int		GetObjType( void ) { return OBJECTEXPLODINGEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0043f920 ?GetClassName@ObjectExplodingEvent@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetClassName( void ) { return "ObjectExplodingEvent"; }
	// FUNCTION: STBC 0x0043f930 ?GetSwigTypeName@ObjectExplodingEvent@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetSwigTypeName( void ) { return "_p_ObjectExplodingEvent"; }
	// FUNCTION: STBC 0x0043f940 ?GetPythonClassName@ObjectExplodingEvent@@UAEPADXZ @game/UtopiaModule
	virtual char *	GetPythonClassName( void ) { return "ObjectExplodingEventPtr"; }

	// TGEvent
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	int				GetFiringPlayerID( void ) { return firingPlayerID; }
	void			SetFiringPlayerID( int id ) { firingPlayerID = id; }
	float			GetLifetime( void ) { return lifetime; }
	void			SetLifetime( float lifetime_ ) { lifetime = lifetime_; }

protected:
	int				firingPlayerID;		// 0x28 the network ID of the player whose weapon destroyed the object
	float			lifetime;			// 0x2C the seconds the explosion lasts
};

/*
================
ObjectExplodingEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x0043f8f0 ?IsTypeOf@ObjectExplodingEvent@@UAE_NH@Z @game/UtopiaModule
inline bool ObjectExplodingEvent::IsTypeOf( int type ) {
	if ( type == OBJECTEXPLODINGEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
ObjectExplodingEvent::WriteToNetStream

The base's result is not checked.
================
*/
// FUNCTION: STBC 0x0043f990 ?WriteToNetStream@ObjectExplodingEvent@@UAE_NPAVTGStream@@@Z @game/UtopiaModule
inline bool ObjectExplodingEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	stream->WriteLong( firingPlayerID );
	stream->WriteFloat( lifetime );
	return true;
}

/*
================
ObjectExplodingEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x0043f9c0 ?ReadFromNetStream@ObjectExplodingEvent@@UAE_NPAVTGStream@@@Z @game/UtopiaModule
inline bool ObjectExplodingEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	firingPlayerID = stream->ReadLong();
	lifetime = stream->ReadFloat();
	return true;
}

/*
================
ObjectExplodingEvent::WriteToStream

The base's result is not checked.
================
*/
// FUNCTION: STBC 0x0043f9f0 ?WriteToStream@ObjectExplodingEvent@@UAE_NPAVTGStream@@@Z @game/UtopiaModule
inline bool ObjectExplodingEvent::WriteToStream( TGStream *stream ) {
	TGEvent::WriteToStream( stream );
	stream->WriteLong( firingPlayerID );
	stream->WriteFloat( lifetime );
	return true;
}

/*
================
ObjectExplodingEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x0043fa20 ?ReadFromStream@ObjectExplodingEvent@@UAE_NPAVTGStream@@@Z @game/UtopiaModule
inline bool ObjectExplodingEvent::ReadFromStream( TGStream *stream ) {
	TGEvent::ReadFromStream( stream );
	firingPlayerID = stream->ReadLong();
	lifetime = stream->ReadFloat();
	return true;
}

#endif /* !__OBJECTEXPLODINGEVENT_H__ */
