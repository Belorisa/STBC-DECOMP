/*
===============================================================================

	TGMusicFadeEvent.h

	The event a timer sends the music player to fade a piece of music in or
	out: the piece, the volumes to fade between and whether it loops.

	0x40 bytes, allocated from the pools. Evidence: the load constructor
	(0x70AED0, which the sound module's creator builds in 0x40 bytes) calls
	TGEvent's, stores the vtable 0x8969DC (TGEvent's 18 slots) and clears
	the name at 0x28; the destructor (0x70AF90) deletes the name. GetObjType
	returns 0x367; the class strings are "TGMusicFadeEvent",
	"_p_TGMusicFadeEvent" and "TGMusicFadeEventPtr". TGMusic::StartMusic
	(0x713D60) and PlayFanfare (0x714260) build the events: they copy the
	piece's name to 0x28, set the volumes at 0x2C and 0x30 and the looping
	flag at 0x3C. The overrides of slots 4, 5 and 12 to 14 (0x715D40 to
	0x715EC0) copy, write and read the members. The member names are
	invented.

===============================================================================
*/

#ifndef __TGMUSICFADEEVENT_H__
#define __TGMUSICFADEEVENT_H__

#include <string.h>
#include "TGEvent.h"

// GetObjType's value for a TGMusicFadeEvent.
#define TGMUSICFADEEVENT_TYPE	0x367

// FUNCTION: STBC 0x0070af60 ??_GTGMusicFadeEvent@@UAEPAXI@Z
class TGMusicFadeEvent : public TGEvent {
	friend class TGMusic;		// the player fills in the fades it starts
public:
	// FUNCTION: STBC 0x0070aed0 ??0TGMusicFadeEvent@@QAE@J@Z
					TGMusicFadeEvent( long id = 0 ) : TGEvent( id ) { musicName = NULL; }
	// FUNCTION: STBC 0x0070af90 ??1TGMusicFadeEvent@@UAE@XZ
	virtual			~TGMusicFadeEvent( void ) { delete musicName; }

	// TGObject
	// FUNCTION: STBC 0x0070aef0 ?GetObjType@TGMusicFadeEvent@@UAEHXZ
	virtual int		GetObjType( void ) { return TGMUSICFADEEVENT_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0070af30 ?GetClassName@TGMusicFadeEvent@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGMusicFadeEvent"; }
	// FUNCTION: STBC 0x0070af40 ?GetSwigTypeName@TGMusicFadeEvent@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGMusicFadeEvent"; }
	// FUNCTION: STBC 0x0070af50 ?GetPythonClassName@TGMusicFadeEvent@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGMusicFadeEventPtr"; }

	// TGEvent
	virtual bool	Copy( TGEvent *event );
	virtual bool	WriteToNetStream( TGStream *stream );
	virtual bool	ReadFromNetStream( TGStream *stream );

	// Replaces the piece's name with a copy (invented name).
	void			SetMusicName( char *name ) { delete musicName; musicName = new char[strlen( name ) + 1]; strcpy( musicName, name ); }
	void			SetStartPosition( float position ) { unknown38 = position; }	// invented name
	void			SetFadeTime( float time ) { fadeTime = time; }	// invented name
	// The fade's volumes and length (invented names: TGMusic::HandleFadeEvent
	// copies them through the FPU, as VC6 does an inline's float result).
	float			GetFromVolume( void ) { return fromVolume; }
	float			GetToVolume( void ) { return toVolume; }
	float			GetFadeTime( void ) { return fadeTime; }
	float			GetStartPosition( void ) { return unknown38; }

protected:
	char *			musicName;				// 0x28 owned copy: the piece to fade
	float			fromVolume;				// 0x2C
	float			toVolume;				// 0x30
	float			fadeTime;				// 0x34 the fade's length in seconds (invented name)
	float			unknown38;				// 0x38
	bool			looping;				// 0x3C
};

/*
================
TGMusicFadeEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x0070af00 ?IsTypeOf@TGMusicFadeEvent@@UAE_NH@Z
inline bool TGMusicFadeEvent::IsTypeOf( int type ) {
	if ( type == TGMUSICFADEEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

#endif /* !__TGMUSICFADEEVENT_H__ */
