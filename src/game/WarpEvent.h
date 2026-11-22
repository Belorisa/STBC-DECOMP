/*
===============================================================================

	WarpEvent.h

	The event that starts a ship's warp (ET_START_WARP): the destination
	set's module, the placement to arrive at, the exit point and the
	duration.

	0x40 bytes. Evidence: the constructor (0x43FA80) runs TGEvent's, stores
	the vtable 0x88A1B4 (TGEvent's 15 slots; Copy is TGEvent's own) and
	clears 0x28, 0x2C and 0x3C; the destructor (0x43FB50) frees the two
	names; the deleting destructor (0x43FB20) and the creator (0x43FA50)
	free and allocate 0x40 bytes. The class strings are "WarpEvent",
	"_p_WarpEvent" and "WarpEventPtr"; GetObjType returns 0x812A. The SWIG
	wrappers read GetDestinationSetName at 0x28, GetPlacementName at 0x2C,
	GetExitPoint at 0x30 and GetDuration at 0x3C.

===============================================================================
*/

#ifndef __WARPEVENT_H__
#define __WARPEVENT_H__

#include <string.h>
#include "TGEvent.h"
#include "TGPoint3.h"
#include "TGStream.h"

// GetObjType's value for a WarpEvent (the slot 1 at 0x43FAB0).
#define WARPEVENT_TYPE		0x812A

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0043fb20 ??_GWarpEvent@@UAEPAXI@Z @game/UtopiaModule
class WarpEvent : public TGEvent {
public:
	// Inline: WarpSequence::Play (0x4D66B0) expands it; the out-of-line copy is UtopiaModule::CreateWarpEvent's.
	// FUNCTION: STBC 0x0043fa80 ??0WarpEvent@@QAE@J@Z @game/UtopiaModule
					WarpEvent( long id = 0 ) : TGEvent( id ) {
						destinationSetName = NULL;
						duration = 0.0f;
						placementName = NULL;
					}
	virtual			~WarpEvent( void );


	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEvent
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	char *			GetDestinationSetName( void ) { return destinationSetName; }
	char *			GetPlacementName( void ) { return placementName; }
	float			GetDuration( void ) { return duration; }

	// the setters WarpSequence::Play (0x4D66B0) expands: each name is
	// replaced by a heap copy (invented names)
	void			SetDestinationSetName( char *name ) {
						if ( destinationSetName != NULL ) {
							delete destinationSetName;
							destinationSetName = NULL;
						}
						if ( name != NULL ) {
							destinationSetName = new char[strlen( name ) + 1];
							strcpy( destinationSetName, name );
						}
					}
	void			SetPlacementName( char *name ) {
						if ( placementName != NULL ) {
							delete placementName;
							placementName = NULL;
						}
						if ( name != NULL ) {
							placementName = new char[strlen( name ) + 1];
							strcpy( placementName, name );
						}
					}
	void			SetExitPoint( const TGPoint3 &point ) { exitPoint = point; }
	// FUNCTION: STBC 0x0061c7d0 ?GetExitPoint@WarpEvent@@QAE?AVTGPoint3@@XZ @game/AppWrap
	TGPoint3		GetExitPoint( void ) { return exitPoint; }
	void			SetDuration( float duration_ ) { duration = duration_; }

protected:
	char *			destinationSetName;	// 0x28 owned; the destination set's module name
	char *			placementName;		// 0x2C owned; not sent over the network
	NiPoint3		exitPoint;			// 0x30 (GetExitPoint)
	float			duration;			// 0x3C
};

/*
================
WarpEvent::GetObjType
================
*/
// FUNCTION: STBC 0x0043fab0 ?GetObjType@WarpEvent@@UAEHXZ @game/UtopiaModule
inline int WarpEvent::GetObjType( void ) {
	return WARPEVENT_TYPE;
}

/*
================
WarpEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x0043fac0 ?IsTypeOf@WarpEvent@@UAE_NH@Z @game/UtopiaModule
inline bool WarpEvent::IsTypeOf( int type ) {
	if ( type == WARPEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
WarpEvent::GetClassName
================
*/
// FUNCTION: STBC 0x0043faf0 ?GetClassName@WarpEvent@@UAEPADXZ @game/UtopiaModule
inline char *WarpEvent::GetClassName( void ) {
	return "WarpEvent";
}

/*
================
WarpEvent::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0043fb00 ?GetSwigTypeName@WarpEvent@@UAEPADXZ @game/UtopiaModule
inline char *WarpEvent::GetSwigTypeName( void ) {
	return "_p_WarpEvent";
}

/*
================
WarpEvent::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0043fb10 ?GetPythonClassName@WarpEvent@@UAEPADXZ @game/UtopiaModule
inline char *WarpEvent::GetPythonClassName( void ) {
	return "WarpEventPtr";
}

/*
================
WarpEvent::~WarpEvent
================
*/
// FUNCTION: STBC 0x0043fb50 ??1WarpEvent@@UAE@XZ @game/UtopiaModule
inline WarpEvent::~WarpEvent( void ) {
	if ( destinationSetName != NULL ) {
		delete destinationSetName;
	}
	if ( placementName != NULL ) {
		delete placementName;
	}
}

/*
================
WarpEvent::WriteToNetStream

The destination set's name, the exit point and the duration; the
placement name stays behind.
================
*/
// FUNCTION: STBC 0x0043fb90 ?WriteToNetStream@WarpEvent@@UAE_NPAVTGStream@@@Z @game/UtopiaModule
inline bool WarpEvent::WriteToNetStream( TGStream *stream ) {
	TGEvent::WriteToNetStream( stream );
	if ( destinationSetName != NULL ) {
		stream->WriteChar( (char)strlen( destinationSetName ) );
		stream->Write( destinationSetName, strlen( destinationSetName ) );
	} else {
		stream->WriteChar( 0 );
	}
	stream->WriteFloat( exitPoint.x );
	stream->WriteFloat( exitPoint.y );
	stream->WriteFloat( exitPoint.z );
	stream->WriteFloat( duration );
	return true;
}

/*
================
WarpEvent::ReadFromNetStream
================
*/
// FUNCTION: STBC 0x0043fc20 ?ReadFromNetStream@WarpEvent@@UAE_NPAVTGStream@@@Z @game/UtopiaModule
inline bool WarpEvent::ReadFromNetStream( TGStream *stream ) {
	TGEvent::ReadFromNetStream( stream );
	int length = stream->ReadChar();
	if ( length == 0 ) {
		destinationSetName = NULL;
	} else {
		destinationSetName = new char[length + 1];
		stream->Read( destinationSetName, length );
		destinationSetName[length] = '\0';
	}
	exitPoint.x = stream->ReadFloat();
	exitPoint.y = stream->ReadFloat();
	exitPoint.z = stream->ReadFloat();
	placementName = NULL;
	duration = stream->ReadFloat();
	return true;
}

/*
================
WarpEvent::WriteToStream
================
*/
// FUNCTION: STBC 0x0043fca0 ?WriteToStream@WarpEvent@@UAE_NPAVTGStream@@@Z @game/UtopiaModule
inline bool WarpEvent::WriteToStream( TGStream *stream ) {
	TGEvent::WriteToStream( stream );
	if ( destinationSetName != NULL ) {
		stream->WriteChar( (char)strlen( destinationSetName ) );
		stream->Write( destinationSetName, strlen( destinationSetName ) );
	} else {
		stream->WriteChar( 0 );
	}
	stream->WriteFloat( exitPoint.x );
	stream->WriteFloat( exitPoint.y );
	stream->WriteFloat( exitPoint.z );
	if ( placementName != NULL ) {
		stream->WriteChar( (char)strlen( placementName ) );
		stream->Write( placementName, strlen( placementName ) );
	} else {
		stream->WriteChar( 0 );
	}
	stream->WriteFloat( duration );
	return true;
}

/*
================
WarpEvent::ReadFromStream
================
*/
// FUNCTION: STBC 0x0043fd70 ?ReadFromStream@WarpEvent@@UAE_NPAVTGStream@@@Z @game/UtopiaModule
inline bool WarpEvent::ReadFromStream( TGStream *stream ) {
	TGEvent::ReadFromStream( stream );
	int length = stream->ReadChar();
	if ( length == 0 ) {
		destinationSetName = NULL;
	} else {
		destinationSetName = new char[length + 1];
		stream->Read( destinationSetName, length );
		destinationSetName[length] = '\0';
	}
	exitPoint.x = stream->ReadFloat();
	exitPoint.y = stream->ReadFloat();
	exitPoint.z = stream->ReadFloat();
	length = stream->ReadChar();
	if ( length == 0 ) {
		placementName = NULL;
	} else {
		placementName = new char[length + 1];
		stream->Read( placementName, length );
		placementName[length] = '\0';
	}
	duration = stream->ReadFloat();
	return true;
}

#endif /* !__WARPEVENT_H__ */
