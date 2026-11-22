/*
===============================================================================

	TGSound.h

	A sound: a file, played through a NetImmerse sound source. The sound
	manager files the sounds by name and plays copies of them.

	0x98 bytes, allocated from the pools, without a vtable. The members are
	those the SWIG wrappers TGSound_* call. The constructor body (0x70B360,
	behind the constructors 0x70B030 and 0x70B0C0) copies the file name to
	0x04 and the name to 0x08, sets the flags 0x13 and 0x14 from its flags
	argument, builds the node at 0x20, clears the ID at 0x8C to the invalid
	ID, sets the cone (360 degrees, volume 1) at 0x40, the distances (50
	and 700) at 0x5C, the volume (1) at 0x64 and the priority (0.5) at 0x68
	and, unless the sound is a copy (0x11), files it with the manager. A
	filed sound is a template; each Play builds a copy that owns a source
	(0x18) and plays it. The fade points at 0x70 are an NiTList of 8-byte
	records, sorted by time.

===============================================================================
*/

#ifndef __TGSOUND_H__
#define __TGSOUND_H__

#include <string.h>

#include "NiMatrix3.h"
#include "NiNode.h"
#include "NiPoint3.h"
#include "NiPointer.h"
#include "NiSource.h"
#include "NiTList.h"

class TGEvent;
class TGSoundFile;
class TGSoundRegion;
class TGStream;

// The flags of a sound (the SWIG constants TGSound.LS_*).
#define LS_3D				0x01
#define LS_STREAMED			0x02
#define LS_DELAY_LOADING	0x04

// What GetStatus reports (the SWIG constants TGSound.SS_*).
#define SS_PLAYING			0
#define SS_STOPPED			1
#define SS_UNLOADED			2
#define SS_UNKNOWN			3

/*
================
TGFadePoint

A point of a sound's fade: the volume it has at a time. 8 bytes. The name of
the type is invented.
================
*/
struct TGFadePoint {
	float			time;				// seconds into the sound
	float			volume;				// 0 to 1
};

class TGSound {
public:
	TGSound( char *fileName_, char *name_, long flags );
	// The script-side constructor (the SWIG wrapper TGSound_Create).
	// FUNCTION: STBC 0x005d92f0 ?Create@TGSound@@SAPAV1@PAD0H@Z @game/AppWrap
	static TGSound *	Create( char *fileName, char *name, int flags ) { return new TGSound( fileName, name, flags ); }
	// A copy of the sound to play: the manager plays copies of its filed
	// sounds (invented name).
	TGSound( TGSound *original );
	~TGSound( void );

	bool	Load( void );
	void	Unload( void );
	void	SetGroup( char * );
	int	Play( bool );
	int	PlayAndNotify( TGEvent *, bool );
	bool	Stop( void );
	bool	Rewind( void );
	void	Pause( void );
	void	Unpause( void );
	int	GetStatus( void );
	bool	Is3D( void );
	bool	AttachToNode( NiNode * );
	bool	DetachFromNode( void );
	NiNode *	GetParentNode( void );
	bool	SetOrientation( NiPoint3 *, NiPoint3 * );
	bool	GetOrientation( NiPoint3 *, NiPoint3 * );
	void	SetPosition( NiPoint3 * );
	// The position of the source, else of the node, else the origin
	// (invented name).
	NiPoint3	GetPosition( void );
	bool	SetConeData( float, float, float );
	bool	GetConeData( float *, float *, float * );
	bool	SetMinMaxDistance( float, float );
	bool	GetMinMaxDistance( float *, float * );
	bool	SetVolume( float );
	bool	SetPitch( float );
	float	GetPitch( void );
	bool	SetPlayPositionSeconds( float );
	bool	SetPlayPosition( unsigned long position );	// SWIG's DWORD
	float	GetPlayPositionSeconds( void );
	unsigned long	GetPlayPosition( void );	// SWIG's DWORD
	// FUNCTION: STBC 0x005db020 ?GetNiSourceObj@TGSound@@QAE?AV?$NiPointer@VNiSource@@@@XZ @game/AppWrap
	NiPointer<NiSource>	GetNiSourceObj( void ) { return source; }
	float	GetDurationSeconds( void );
	bool	SetEndEvent( TGEvent * );
	TGEvent *	GetEndEvent( void );
	void	Update( void );
	void	AddFadePoint( float, float );
	void	ClearFadePoints( void );

	// Starts the sound's source, from the start when rewind is set; while
	// the manager pauses the sounds, a pausable sound is only marked paused.
	bool	Unknown70BB80( bool rewind );
	// Silences the sound or lets it be heard again: the filter of the sound
	// regions (TGSoundRegion's filter 1).
	void	Unknown70CE60( bool silenced );

	// The state of a playing copy, for the manager's save (invented names).
	bool	WriteToStream( TGStream *stream );
	bool	ReadFromStream( TGStream *stream );
	bool	FixUpPointers( void );
	void	PostFixUp( void );

	void			SetVoice( bool voice_ ) { voice = voice_; }		// invented name
	void			SetLooping( bool looping_ ) { looping = looping_; }
	bool			GetLooping( void ) { return looping; }
	bool			SetPriority( float priority_ ) { priority = priority_; return true; }
	float			GetPriority( void ) { return priority; }
	float			GetVolume( void ) { return volume; }			// invented name
	// Whether the sound is in the group; the NULL group is that of the
	// sounds in none (invented name).
	bool			IsInGroup( char *group_ );
	char *	GetGroup( void ) { return group; }
	char *	GetFileName( void ) { return fileName; }
	char *	GetSoundName( void ) { return name; }
	bool	IsInterface( void ) { return interfaceSound; }
	bool	IsVoice( void ) { return voice; }
	bool	IsSFX( void ) { return sfx; }
	bool	IsLoaded( void ) { return loaded; }
	bool	IsStreamed( void ) { return streamed; }
	bool	IsSingleShot( void ) { return singleShot; }
	bool	IsPaused( void ) { return paused; }
	void	SetInterface( bool interfaceSound_ ) { interfaceSound = interfaceSound_; }
	void	SetSFX( bool sfx_ ) { sfx = sfx_; }
	void	SetSingleShot( bool singleShot_ ) { singleShot = singleShot_; }

protected:
	// The sound manager files, plays and deletes the sounds through their
	// members.
	friend class TGSoundManager;
	friend class TGSoundRegion;
	friend class TGSoundFile;
	friend class TGMusic;

	// The constructors' shared body (0x70B360, invented name): copies the
	// names, sets the members from the flags and, unless the sound is a
	// copy, files it with the manager.
	bool			CanPlay( void );		// invented name
	void			Init( char *fileName_, char *name_, unsigned long flags, bool copy );

	// The sounds built and their rough memory, which the manager reports
	// (invented names).
	static int		numAllocated;
	static int		roughMemoryUsage;

	char *			group;				// 0x00 owned copy (SetGroup); NULL when in none
	char *			fileName;			// 0x04 owned copy (invented name)
	char *			name;				// 0x08 owned copy: the manager's key (invented name)
	bool			sfx;			// 0x0C true at first; PauseAllSounds pauses the sounds with it or voice set
	bool			voice;				// 0x0D a voice, not an effect: TGSoundAction sets it for TGSAF_VOICE (invented name)
	bool			interfaceSound;			// 0x0E
	bool			loaded;				// 0x0F Load's result (invented name)
	bool			unknown10;			// 0x10 Load failed (invented name)
	bool			unknown11;			// 0x11 a played copy of a filed sound: the manager deletes it once it stops
	bool			looping;			// 0x12 (the SWIG wrappers TGSound_SetLooping and _GetLooping)
	bool			streamed;			// 0x13 LS_STREAMED
	bool			is3D;				// 0x14 LS_3D (Is3D)
	NiPointer<NiSource>	source;			// 0x18 the NetImmerse sound source a loaded copy plays through (invented name)
	void *			sample;				// 0x1C the 3D sample handle reserved for the source (invented name)
	NiPointer<NiNode>	node;			// 0x20 the node the sound hangs on (invented name)
	long			savedParentNode;	// 0x24 the address the parent node had when saved (invented name)
	NiPoint3		facing;				// 0x28 the cone's direction while there is no source (invented name)
	NiPoint3		upDirection;		// 0x34 its up direction (invented name)
	float			innerConeAngle;		// 0x40 360 at first (SetConeData)
	float			outerConeAngle;		// 0x44 360 at first
	float			outerConeVolume;	// 0x48 1 at first
	float			pitch;				// 0x4C 1 at first (SetPitch; invented name)
	bool			silenced;			// 0x50 the sound regions' filter 1 (invented name)
	bool			unknown51;			// 0x51
	bool			paused;			// 0x52 set by Pause, cleared by Unpause
	bool			singleShot;		// 0x53 Stop posts the event that deletes the sound (invented name)
	bool			hasPlayPosition;	// 0x54 SetPlayPositionSeconds ran while there was no source (invented name)
	float			playPosition;		// 0x58 that position (invented name)
	float			minDistance;		// 0x5C 50 at first
	float			maxDistance;		// 0x60 700 at first
	float			volume;				// 0x64 1 at first; the manager reapplies it when its volumes change (invented name)
	float			priority;			// 0x68 (the SWIG wrappers TGSound_SetPriority and _GetPriority)
	float			savedPlayPosition;	// 0x6C -1 until a saved game gives one (invented name)
	NiTList<TGFadePoint *>	fadePoints;	// 0x70 sorted by time
	TGSoundFile *	file;				// 0x88 the sound's file in the cache (invented name)
	long			id;					// 0x8C the playing sound's ID; INVALID_SOUND_ID when not playing (invented name)
	long			endEventID;			// 0x90 the ID of the end event (SetEndEvent, GetEndEvent; invented name)
	TGSoundRegion *	region;				// 0x94 the region the sound was added to (invented name)
};

/*
================
TGSound::IsInGroup
================
*/
inline bool TGSound::IsInGroup( char *group_ ) {
	if ( group == NULL ) {
		if ( group_ == NULL ) {
			return true;
		}
	} else if ( group_ != NULL && strcmp( group, group_ ) == 0 ) {
		return true;
	}
	return false;
}

#endif /* !__TGSOUND_H__ */
