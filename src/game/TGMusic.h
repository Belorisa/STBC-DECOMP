/*
===============================================================================

	TGMusic.h

	The music player: plays one piece of music at a time through the sound
	manager, fades between pieces with fade events (TGMusicFadeEvent) that
	timers send, and plays a fanfare over the music, after which the music
	resumes.

	0x7C bytes. Evidence: the constructor (0x713760) calls
	TGEventHandlerObject's with 0, builds a list of two items per block at
	0x28, a string map of 37 buckets at 0x40 (vtables 0x8969C8, then
	0x8969B4) and a list at 0x50, stores the vtable 0x896960
	(TGEventHandlerObject's 21 slots) and sets the members after them; the
	load constructor (0x70AA50, which the sound module's creator builds in
	0x7C bytes) builds the members' objects only. GetObjType returns 0x302.
	StartMusic (0x713D60) and PlayFanfare (0x714260) show the members they
	use. The one instance is the SWIG variable g_kMusicManager (0x99BDA0).
	The member names are invented.

===============================================================================
*/

#ifndef __TGMUSIC_H__
#define __TGMUSIC_H__

#include "TGEventHandlerObject.h"
#include "NiTList.h"
#include "NiTStringMap.h"

// GetObjType's value for a TGMusic.
#define TGMUSIC_TYPE			0x302

// The events the player handles (invented names): a fade step, and the end
// of a piece.
#define ET_MUSIC_FADE			0x20003
#define ET_MUSIC_DONE			0x20004

class TGEvent;
class TGStream;

/*
================
TGMusicFade

A volume fade of one playing sound: the volume goes from fromVolume to
toVolume over the duration, and the sound stops at the end of a fade to 0.
Update steps the fades and deletes the finished ones. The struct and its
names are invented: 0x14 bytes, the playing ID, then four floats, saved in
that order (0x714F50).
================
*/
struct TGMusicFade {
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );

	long			soundID;		// 0x00 the playing ID of the sound
	float			duration;		// 0x04 seconds
	float			timeLeft;		// 0x08 seconds
	float			fromVolume;		// 0x0C at the start
	float			toVolume;		// 0x10 at the end
};

/*
================
TGMusicPeriod

The fanfare period LoadMusic files under a piece's name, in seconds;
PlayFanfare reads it. The struct and its name are invented: four bytes,
saved as a float (0x714FE0).
================
*/
struct TGMusicPeriod {
	bool			WriteToStream( TGStream *stream );
	bool			ReadFromStream( TGStream *stream );

	float			period;
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0070ae70 ??_GTGMusic@@UAEPAXI@Z
class TGMusic : public TGEventHandlerObject {
public:
					TGMusic( void );
	// FUNCTION: STBC 0x0070aa50 ??0TGMusic@@QAE@J@Z
					TGMusic( long id ) : TGEventHandlerObject( id ), fades( 2 ), fadeEvents( 2 ) {}
	virtual			~TGMusic( void );

	// TGObject
	// FUNCTION: STBC 0x0070ade0 ?GetObjType@TGMusic@@UAEHXZ
	virtual int		GetObjType( void ) { return TGMUSIC_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0070ae30 ?GetClassName@TGMusic@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGMusic"; }
	// FUNCTION: STBC 0x0070ae40 ?GetSwigTypeName@TGMusic@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGMusic"; }
	// FUNCTION: STBC 0x0070ae50 ?GetPythonClassName@TGMusic@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGMusicPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0070ae60 ?GetClassHandlers@TGMusic@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	bool			StartMusic( char *, bool );
	void			StopMusic( void );
	void			PlayFanfare( char * );
	void			SetEnabled( bool );
	void			SetVolume( float );

	// Moves on by the game time since the last call (invented name).
	void			Update( void );

	// The handlers registered under the names "TGMusic::HandleFadeEvent" and
	// "TGMusic::MusicDone".
	void			HandleFadeEvent( TGEvent *event );
	void			MusicDone( TGEvent *event );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	float	GetVolume( void ) { return volume; }
	bool	IsEnabled( void ) { return enabled; }

protected:
	// Fades the piece playing out and forgets it; the fade's start and
	// length come back (invented name).
	void			FadeOutMusic( float *time, float *fadeTime );
	// Forgets the fanfare and the piece to resume after it (invented name).
	void			ClearFanfare( void );
	// Deletes the fanfare periods of all pieces (invented name).
	void			DeletePeriods( void );

	char *			musicName;					// 0x14 owned copy: the piece playing
	long			soundID;					// 0x18 its sound's playing ID; unknown896A9C when none
	float			volume;						// 0x1C 1 at first
	float			lastUpdateTime;				// 0x20 the game time of the last Update (invented name)
	bool			enabled;					// 0x24 true at first; PlayFanfare plays nothing without it
	NiTList<TGMusicFade *>	fades;				// 0x28 owned (invented name)
	NiTStringMap<TGMusicPeriod *>	periods;	// 0x40 owned, by the piece's name (invented name)
	NiTList<long>	fadeEvents;					// 0x50 the IDs of the fade events sent
	bool			fanfarePlaying;				// 0x68 set by PlayFanfare
	char *			fanfareName;				// 0x6C owned copy
	char *			resumeName;					// 0x70 owned copy: the piece to resume after the fanfare
	bool			resumeLooping;				// 0x74 true at first
	float			resumePosition;				// 0x78 seconds

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
	bool	LoadMusic( char *, char *, float );	// retail 0x713AD0: the SWIG wrapper TGMusic_LoadMusic calls it
	bool	UnloadMusic( char * );	// retail 0x713C50: the SWIG wrapper TGMusic_UnloadMusic calls it
};

/*
================
TGMusic::IsTypeOf
================
*/
// FUNCTION: STBC 0x0070adf0 ?IsTypeOf@TGMusic@@UAE_NH@Z
inline bool TGMusic::IsTypeOf( int type ) {
	if ( type == TGMUSIC_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

// The music player (the SWIG variable g_kMusicManager).
extern TGMusic	g_kMusicManager;

#endif /* !__TGMUSIC_H__ */
