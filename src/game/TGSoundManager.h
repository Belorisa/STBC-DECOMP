/*
===============================================================================

	TGSoundManager.h

	The sound manager: starts the NetImmerse sound system, files the sounds
	by name, plays copies of them in a fixed number of slots, and keeps the
	volumes of the sound categories. Each Miles 3D provider gets a pool of
	sample handles reserved for it, as the SoundConfig script lists them.

	0xB4 bytes. Evidence: the constructor (0x70EE80) calls
	TGEventHandlerObject's, builds a string map of 37 buckets at 0x3C
	(vtables 0x89694C, then 0x896938), two lists of two items per block at
	0x74 and 0x8C and a map of 37 buckets at 0xA4 (vtable 0x896924), stores
	the vtable 0x8968D0 (TGEventHandlerObject's 21 slots) and sets 32 slots
	(0x896AA4). GetObjType returns 0x301; the scalar deleting destructor
	(0x70A9F0) frees through the pool of 0xB4 bytes. The one instance is the
	SWIG variable g_kSoundManager (0x99BA00). The names of the members the
	wrappers do not call are invented.

===============================================================================
*/

#ifndef __TGSOUNDMANAGER_H__
#define __TGSOUNDMANAGER_H__

#include "TGEventHandlerObject.h"
#include "NiTList.h"
#include "NiTMap.h"
#include "NiTStringMap.h"

class NiProvider_Info;
class TGSound;
class TGStringEvent;

// GetObjType's value for a TGSoundManager.
#define TGSOUNDMANAGER_TYPE		0x301

// The event that deletes the sound its string names (invented name).
#define ET_DELETE_SOUND			0x20005

/*
================
TGProviderReserve

An entry of a provider reserve list of SoundConfig: the number of sample
handles to reserve on the provider of the name. 8 bytes; the name is an
owned copy. The name of the type is invented.
================
*/
struct TGProviderReserve {
	int				numSamples;			// -1: as many as the manager plays at once
	char *			providerName;
};

/*
================
TGSamplePool

The sample handles reserved on one provider: those free to hand out and
those handed out. 0x30 bytes. The name of the type is invented.
================
*/
// FUNCTION: STBC 0x007111b0 ??1TGSamplePool@@QAE@XZ
struct TGSamplePool {
					TGSamplePool( void ) : freeSamples( 2 ), usedSamples( 2 ) {}

	NiTList<void *>	freeSamples;
	NiTList<void *>	usedSamples;
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0070a9f0 ??_GTGSoundManager@@UAEPAXI@Z @game/TGSoundModule
class TGSoundManager : public TGEventHandlerObject {
public:
					TGSoundManager( void );
	// FUNCTION: STBC 0x0070a450 ??0TGSoundManager@@QAE@J@Z
					TGSoundManager( long id ) : TGEventHandlerObject( id ), reserves( 2 ), stoppedSounds( 2 ) {}
	virtual			~TGSoundManager( void );

	// TGObject
	// FUNCTION: STBC 0x0070a960 ?GetObjType@TGSoundManager@@UAEHXZ @game/TGSoundModule
	virtual int		GetObjType( void ) { return TGSOUNDMANAGER_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0070a9b0 ?GetClassName@TGSoundManager@@UAEPADXZ @game/TGSoundModule
	virtual char *	GetClassName( void ) { return "TGSoundManager"; }
	// FUNCTION: STBC 0x0070a9c0 ?GetSwigTypeName@TGSoundManager@@UAEPADXZ @game/TGSoundModule
	virtual char *	GetSwigTypeName( void ) { return "_p_TGSoundManager"; }
	// FUNCTION: STBC 0x0070a9d0 ?GetPythonClassName@TGSoundManager@@UAEPADXZ @game/TGSoundModule
	virtual char *	GetPythonClassName( void ) { return "TGSoundManagerPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x0070a9e0 ?GetClassHandlers@TGSoundManager@@UAEPAVTGEventHandlerTable@@XZ @game/TGSoundModule
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// Starts the sound system on the window with the Miles files of the
	// directory, then opens the providers SoundConfig reserves samples on
	// (invented name).
	bool			Startup( void *hWnd_, char *redistDirectory_ );
	// Closes the providers and shuts the sound system down (invented name).
	void			Shutdown( void );

	bool			Reload( bool );
	// Updates the sounds: drops the stopped ones, lowers the effects while a
	// voice plays, and updates the sound system (invented name).
	void			Update( bool updateAll );

	void			SetMaxSoundsAtOnce( int );
	bool			SetUnitsPerMeter( float );
	float			GetUnitsPerMeter( void );
	void			SetMasterVolume( float );
	void			SetSFXVolume( float );
	void			SetSFXEnabled( bool );
	void			SetVoiceVolume( float );
	void			SetVoiceEnabled( bool );
	void			SetInterfaceVolume( float );
	TGSound *		GetSound( char * );
	TGSound *		GetPlayingSound( long );
	// The playing sound of the name; NULL when none is (invented overload).
	TGSound *		GetPlayingSound( char *name );
	int				PlaySound( char * );
	bool			StopSound( char * );
	// Stops the playing sound of the ID, if any; the manager must be
	// enabled (invented overload).
	bool			StopSound( long id );
	// Pause and unpause the effects and voices that play (invented names).
	void			PauseAllSounds( void );
	void			UnpauseAllSounds( void );
	void			SetSFXAdjustmentMinimum( float );
	bool			StopAllSounds( void );
	bool			StopAllSoundsInGroup( char * );
	bool			DeleteSound( char * );
	void			DeleteAllSounds( void );
	void			DeleteAllSoundsInGroup( char * );
	int				GetNumSoundsAllocated( void );
	int				GetRoughMemoryUsage( void );
	bool			IsA3DSupported( void );
	bool			IsEAXSupported( void );

	// The sounds' filing, by name (invented names).
	bool			AddSound( TGSound *sound );
	void			RemoveSound( TGSound *sound );

	// The playing sounds' slots (invented names).
	long			AllocateSoundID( TGSound *sound );
	void			SetPlayingSound( TGSound *sound, long id );
	void			ReleaseSoundID( TGSound *sound );

	// The sample handles (invented names).
	void			GetSample( void **sample, NiProvider_Info **provider );
	bool			ReleaseSample( void *sample );

	void			HandleDeleteSoundEvent( TGStringEvent *event );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	float	GetInterfaceVolume( void ) { return interfaceVolume; }
	bool	IsVoiceEnabled( void ) { return voiceEnabled; }
	bool	IsSFXEnabled( void ) { return sfxEnabled; }
	int	GetMaxSoundsAtOnce( void ) { return maxSoundsAtOnce; }
	float	GetVoiceVolume( void ) { return voiceVolume; }
	float	GetSFXVolume( void ) { return sfxVolume; }
	float	GetMasterVolume( void ) { return masterVolume; }
	void	SetEnabled( bool enabled_ ) { enabled = enabled_; }
	bool	IsEnabled( void ) { return unknown1C && enabled; }

	// Forgets the sound slots without deleting them, before a saved game is
	// loaded (invented name, after TGEventManager::ForgetAll).
	void	ForgetAll( void );
	// Moves the playing sounds to their slots after a reload (invented
	// name; ProcessInternalLoad calls it).
	void	RestorePlayingSounds( void );

protected:
	// The sounds read the manager's switches and volumes.
	friend class TGSound;

	// Reads SoundConfig and the sound options (invented name).
	bool			LoadConfig( char *module );
	// The providers (invented names).
	bool			OpenProviders( void );
	bool			OpenReservedProviders( void );
	bool			CloseProviders( void );
	NiProvider_Info *	FindProvider( char *name );
	int				AllocateSamples( NiProvider_Info *provider, int count );
	int				GetNumSamples( NiProvider_Info *provider );
	int				GetNumFreeSamples( NiProvider_Info *provider );
	void			FreeSamples( NiProvider_Info *provider );
	// Reapplies every playing sound's volume (invented name).
	void			UpdateVolumes( void );
	// Deletes the stopped copies (invented name).
	void			DeleteStoppedSounds( void );
	// The slots (invented names).
	bool			IsSlotReserved( int slot );
	int				GetSlot( long id );
	long			GetSlotID( int slot );

	char *			redistDirectory;			// 0x14 owned copy
	void *			hWnd;						// 0x18
	bool			unknown1C;					// 0x1C Startup's result
	bool			enabled;					// 0x1D Startup's result; nothing plays until it is set
	bool			providersOpen;				// 0x1E
	float			masterVolume;				// 0x20 0 to 1
	float			sfxVolume;					// 0x24 0 to 1
	float			voiceVolume;				// 0x28 0 to 1
	float			interfaceVolume;			// 0x2C 0 to 1
	bool			sfxEnabled;					// 0x30
	bool			voiceEnabled;				// 0x31
	float			sfxAdjustment;				// 0x34 the effects' factor: lowered toward the minimum while a voice plays
	float			sfxAdjustmentMinimum;		// 0x38 0.8 at first
	NiTStringMap<TGSound *>	sounds;			// 0x3C the filed sounds, by name
	int				maxSoundsAtOnce;			// 0x4C the slots
	int				configMaxSounds;			// 0x50 SoundConfig's number of slots
	TGSound **		playingSounds;				// 0x54 one per slot
	bool *			slotReserved;				// 0x58 one per slot
	unsigned long	slotIDBase;					// 0x5C the IDs of the slots start here; a multiple of the slots
	int				outputRate;					// 0x60 Hz (the option OutputRate)
	int				outputBits;					// 0x64 (the option OutputBits)
	char *			a3dTestProvider;			// 0x68 owned copy (SoundConfig's A3DTestProvider)
	char *			eaxTestProvider;			// 0x6C owned copy (SoundConfig's EAXTestProvider)
	bool			paused;						// 0x70 PauseAllSounds'
	NiTList<TGProviderReserve *>	reserves;	// 0x74 the provider reserve list SoundConfig gives
	NiTList<TGSound *>	stoppedSounds;			// 0x8C the copies to delete
	NiTMap<NiProvider_Info *, TGSamplePool *>	samplePools;	// 0xA4 by provider

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGSoundManager::IsTypeOf
================
*/
// FUNCTION: STBC 0x0070a970 ?IsTypeOf@TGSoundManager@@UAE_NH@Z @game/TGSoundModule
inline bool TGSoundManager::IsTypeOf( int type ) {
	if ( type == TGSOUNDMANAGER_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

// The sound manager (the SWIG variable g_kSoundManager).
extern TGSoundManager	g_kSoundManager;

// A .data int, 0 at first, that the sound code (TGSound::PlayAndNotify
// 0x70BA60, 0x711230) reads; ShipClass::InitObject copies it. The ID of
// no playing sound: TGSound's constructor sets it, and the slot of it is
// none. Its owner is open.
extern const int	unknown896A9C;
// The ID of the first slot of the first round (1), the slots at first (32)
// and the rate at which the effects' factor moves (5 per second); read
// from memory as unknown896A9C is.
extern const unsigned long	unknown896AA0;
extern const int		unknown896AA4;
extern const float	unknown896A98;

#endif /* !__TGSOUNDMANAGER_H__ */
