/*
===============================================================================

	NiSoundSystem.h

	The Miles Sound System front end: starts Miles, keeps the 3D providers
	it found, the one listener and the sound sources, and updates them.

	Evidence: the NiRTTI (0x99FD88) is named "NiSoundSystem" with NiObject's
	as its base; the vtable at 0x89801C (NiObject's 12 slots, overriding
	GetRTTI, the streaming slots and the destructor). The constructor
	(0x78D8B0) creates the listener (0x18, a smart pointer) and two lists
	with two items per block, the sources (0x1C) and the providers (0x20);
	it sets the units per metre (0x10) to 1. A static object's constructor
	(0x78D670) creates the one sound system (0x99FDD4, 0x24 bytes) and its
	destructor (0x78D6E0) deletes it. Startup (0x78DB40) fills the driver
	handle at 0x14 through AIL_quick_handles.

	Names: the class's own; the member names follow the Gamebryo lineage
	(NiAudioSystem: SetUnitsPerMeter's positive test, Startup, Shutdown,
	the hWnd accessors) where marked, the rest are invented.

===============================================================================
*/

#ifndef __NISOUNDSYSTEM_H__
#define __NISOUNDSYSTEM_H__

#include "NiMilesImports.h"
#include "NiObject.h"
#include "NiPointer.h"
#include "NiStream.h"
#include "NiTList.h"
#include "NiStreamMacros.h"

class NiListener;
class NiProvider_Info;
class NiSource;

/*
================
NiSoundSystemLoadRecord

The record CreateObject hands the stream: NiObject's, under a vtable of
its own (0x898058). The class name is invented.
================
*/
// FUNCTION: STBC 0x78E720 ??_GNiSoundSystemLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x78E740 ??1NiSoundSystemLoadRecord@@UAE@XZ
class NiSoundSystemLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x0078D9C0 ??_GNiSoundSystem@@UAEPAXI@Z
class NiSoundSystem : public NiObject {
public:
	// FUNCTION: STBC 0x0078F180 ?GetRTTI@NiSoundSystem@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiSoundSystem();

							NiSoundSystem();

	// the one sound system (Gamebryo lineage: GetAudioSystem)
	static NiSoundSystem *	GetSoundSystem() { return soundSystem; }

	// the window Miles is told of (Gamebryo lineage names)
	void					SetHWnd( void *hWnd_ );
	void *					GetHWnd();

	NiListener *			GetListener();

	// starts Miles with the redistributable directory and the output format
	// and collects every 3D provider that opens; false when none does
	bool					Startup( const char *redistDirectory, int outputRate, int outputBits );
	// releases the sources and providers, closes the listener's Miles
	// listeners and shuts Miles down
	void					Shutdown();

	// the provider's speaker type; false when it is closed or refuses it
	// (names invented)
	bool					SetSpeakerType( NiProvider_Info *provider, int speakerType );
	// Miles' last error text
	const char *			GetLastError();

	// the sources (names invented)
	void					AddSource( NiSource *source );
	void					RemoveSource( NiSource *source );
	// plays, stops or updates every source (names invented); Update runs
	// the last after the listener's
	void					PlaySources();
	void					StopSources();
	void					UpdateSources();

	// updates the listener and the sources (Gamebryo lineage name)
	void					Update( bool updateAll, float time );

	// walking the sources and the providers (names invented)
	NiSource *				GetFirstSource( NiTListIterator &pos );
	NiSource *				GetNextSource( NiTListIterator &pos );
	NiProvider_Info *		GetFirstProvider( NiTListIterator &pos );
	NiProvider_Info *		GetNextProvider( NiTListIterator &pos );

	// a provider's preferences and attributes, passed through to Miles
	// (names invented)
	void					SetProviderPreference( NiProvider_Info *provider, const char *name, const void *value );
	void					GetProviderAttribute( NiProvider_Info *provider, const char *name, void *value );

	// game units per metre; false, keeping the old value, unless positive
	// (Gamebryo lineage names)
	bool					SetUnitsPerMeter( float unitsPerMeter_ );
	float					GetUnitsPerMeter() const { return unitsPerMeter; }

	// the digital driver Startup opened (Gamebryo lineage name); the game's
	// sound manager reads it
	HDIGDRIVER				GetDriver() const { return driver; }

	// the provider's room (reverb) type; Miles answers -1 when the provider
	// has none (names invented)
	bool					SupportsRoomType( NiProvider_Info *provider );
	bool					SetRoomType( NiProvider_Info *provider, int roomType );
	int						GetRoomType( NiProvider_Info *provider );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	friend class NiSoundSystemInit;

	static NiSoundSystem *	soundSystem;

	bool					unknown08;		// streamed after the units per metre
	void *					hWnd;
	float					unitsPerMeter;
	HDIGDRIVER				driver;			// the digital driver AIL_quick_startup opened
	NiPointer<NiListener>	listener;
	NiTList<NiPointer<NiSource> > *		sources;
	NiTList<NiPointer<NiProvider_Info> > *	providers;	// the providers Startup found
};

NiRegisterStream( NiSoundSystem );

#endif /* !__NISOUNDSYSTEM_H__ */
