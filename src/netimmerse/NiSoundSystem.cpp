// NiSoundSystem.cpp -- the Miles Sound System front end

#include "NiRTLib.h"
#include "NiListener.h"
#include "NiProvider_Info.h"
#include "NiSource.h"
#include "NiSoundSystem.h"
#include "NiStream.h"
#include "NiNode.h"
#include "NiTriShape.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0078ed90 _$E30
// FUNCTION: STBC 0x0078eda0 _$E29
// FUNCTION: STBC 0x0078edb0 _$E41
// FUNCTION: STBC 0x0078edc0 _$E40
// FUNCTION: STBC 0x0078efc0 _$E45
// FUNCTION: STBC 0x0078efd0 _$E44
// FUNCTION: STBC 0x0078efe0 _$E49
// FUNCTION: STBC 0x0078eff0 _$E48
// FUNCTION: STBC 0x0078f000 _$E53
// FUNCTION: STBC 0x0078f010 _$E52
// FUNCTION: STBC 0x0078f020 _$E102
// FUNCTION: STBC 0x0078f030 _$E101
// FUNCTION: STBC 0x0078f040 _$E177
// FUNCTION: STBC 0x0078f050 _$E176
// FUNCTION: STBC 0x0078f060 _$E196
// FUNCTION: STBC 0x0078f070 _$E195
// FUNCTION: STBC 0x0078f080 _$E200
// FUNCTION: STBC 0x0078f090 _$E199
// FUNCTION: STBC 0x0078f0a0 _$E208
// FUNCTION: STBC 0x0078f0b0 _$E207
// FUNCTION: STBC 0x0078f0c0 _$E215
// FUNCTION: STBC 0x0078f0d0 _$E214
// FUNCTION: STBC 0x0078f0e0 _$E219
// FUNCTION: STBC 0x0078f0f0 _$E218
// FUNCTION: STBC 0x0078f100 _$E223
// FUNCTION: STBC 0x0078f110 _$E222
// FUNCTION: STBC 0x0078f120 _$E227
// FUNCTION: STBC 0x0078f130 _$E226
// FUNCTION: STBC 0x0078f140 _$E233
// FUNCTION: STBC 0x0078f150 _$E232
// FUNCTION: STBC 0x0078f160 _$E181
// FUNCTION: STBC 0x0078f170 _$E180
// FUNCTION: STBC 0x0078f190 _$E191
// FUNCTION: STBC 0x0078f1a0 _$E190
// FUNCTION: STBC 0x0078f1b0 _$E186
// FUNCTION: STBC 0x0078f1c0 _$E185

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x0078edd0 _$E251
// FUNCTION: STBC 0x0078edf0 _$E250
// FUNCTION: STBC 0x0078e830 _$E17
// FUNCTION: STBC 0x0078e840 _$E14
// FUNCTION: STBC 0x0078e850 _$E16
// FUNCTION: STBC 0x0078e860 _$E15
// FUNCTION: STBC 0x0078e870 _$E23
// FUNCTION: STBC 0x0078e880 _$E20
// FUNCTION: STBC 0x0078e890 _$E22
// FUNCTION: STBC 0x0078e8a0 _$E21

/*
===============================================================================

	NiSoundSystemInit (name invented)

	A file-scope object whose constructor creates the sound system and whose
	destructor deletes it.

===============================================================================
*/
class NiSoundSystemInit {
public:
	// FUNCTION: STBC 0x78D670 ??0NiSoundSystemInit@@QAE@XZ
	NiSoundSystemInit() { NiSoundSystem::soundSystem = new NiSoundSystem; }
	~NiSoundSystemInit() { delete NiSoundSystem::soundSystem; }
};

// FUNCTION: STBC 0x0078d650 _$E238
// FUNCTION: STBC 0x0078d660 _$E235
// FUNCTION: STBC 0x0078d6d0 _$E237
// FUNCTION: STBC 0x0078d6e0 _$E236
// DATA: STBC 0x99FDC8
static NiSoundSystemInit init;

// FUNCTION: STBC 0x0078d700 _$E244
// FUNCTION: STBC 0x0078d710 _$E240
// FUNCTION: STBC 0x0078d730 _$E243
// FUNCTION: STBC 0x0078d740 _$E241
// DATA: STBC 0x99FD88 ?rtti@NiSoundSystem@@2VNiRTTI@@B
const NiRTTI NiSoundSystem::rtti( "NiSoundSystem", &NiObject::rtti );

// DATA: STBC 0x99FDD4 ?soundSystem@NiSoundSystem@@1PAV1@A
NiSoundSystem *NiSoundSystem::soundSystem;

/*
================
NiSoundSystem::Register
================
*/
// FUNCTION: STBC 0x78D760 ?Register@NiSoundSystem@@SAHXZ
NiImplementRegister( NiSoundSystem )

/*
================
NiSoundSystem::NiSoundSystem

Creates the listener and the two lists; one game unit per metre.
================
*/
// FUNCTION: STBC 0x78D8B0
NiSoundSystem::NiSoundSystem() {
	hWnd = 0;
	listener = new NiListener;
	sources = new NiTList<NiPointer<NiSource> >( 2 );
	providers = new NiTList<NiPointer<NiProvider_Info> >( 2 );
	unitsPerMeter = 1.0f;
	unknown08 = false;
	driver = 0;
}

/*
================
NiSoundSystem::~NiSoundSystem
================
*/
// FUNCTION: STBC 0x78D9E0
NiSoundSystem::~NiSoundSystem() {
	listener = 0;
	Shutdown();
	delete sources;
	delete providers;
}

/*
================
NiSoundSystem::SetHWnd
================
*/
// FUNCTION: STBC 0x78DB10
void NiSoundSystem::SetHWnd( void *hWnd_ ) {
	hWnd = hWnd_;
}

/*
================
NiSoundSystem::GetHWnd
================
*/
// FUNCTION: STBC 0x78DB20
void *NiSoundSystem::GetHWnd() {
	return hWnd;
}

/*
================
NiSoundSystem::GetListener
================
*/
// FUNCTION: STBC 0x78DB30
NiListener *NiSoundSystem::GetListener() {
	return listener;
}

/*
================
NiSoundSystem::Startup
================
*/
// FUNCTION: STBC 0x78DB40
bool NiSoundSystem::Startup( const char *redistDirectory, int outputRate, int outputBits ) {
	AIL_set_redist_directory( redistDirectory );
	if ( !AIL_quick_startup( 1, 0, outputRate, outputBits, 0 ) ) {
		return false;
	}
	AIL_quick_handles( &driver, 0, 0 );

	HPROENUM next = HPROENUM_FIRST;
	bool found = false;
	HPROVIDER provider;
	char *name;
	while ( AIL_enumerate_3D_providers( &next, &provider, &name ) ) {
		M3DRESULT result = AIL_open_3D_provider( provider );
		if ( result == M3D_NOERR ) {
			found = true;
			providers->AddTail( new NiProvider_Info( name, provider ) );
			AIL_close_3D_provider( provider );
		}
	}
	if ( !found ) {
		return false;
	}
	return true;
}

/*
================
NiSoundSystem::Shutdown

Unloads every source and empties both lists without releasing their
elements' references, then closes the listener's Miles listeners.
================
*/
// FUNCTION: STBC 0x78DDC0
void NiSoundSystem::Shutdown() {
	NiTListIterator pos = sources->GetHeadPos();
	while ( pos ) {
		NiSource *source = sources->GetNext( pos );
		source->ReleaseHandle();
	}
	if ( providers ) {
		providers->RemoveAll();
	}
	if ( sources ) {
		sources->RemoveAll();
	}
	if ( listener ) {
		listener->CloseListeners();
	}
	AIL_quick_shutdown();
}

/*
================
NiSoundSystem::SetSpeakerType

Reads the speaker type back once more than it needs to.
================
*/
// FUNCTION: STBC 0x78DEC0
bool NiSoundSystem::SetSpeakerType( NiProvider_Info *provider, int speakerType ) {
	if ( !provider || !provider->IsOpen() ) {
		return false;
	}
	AIL_set_3D_speaker_type( provider->GetProvider(), speakerType );
	AIL_3D_speaker_type( provider->GetProvider() );
	if ( AIL_3D_speaker_type( provider->GetProvider() ) != speakerType ) {
		return false;
	}
	return true;
}

/*
================
NiSoundSystem::GetLastError
================
*/
// FUNCTION: STBC 0x78DF10
const char *NiSoundSystem::GetLastError() {
	return AIL_last_error();
}

/*
================
NiSoundSystem::AddSource
================
*/
// FUNCTION: STBC 0x78DF20
void NiSoundSystem::AddSource( NiSource *source ) {
	sources->AddTail( source );
}

/*
================
NiSoundSystem::RemoveSource

Leaves a source alone once its count has reached zero: its destructor
calls this.
================
*/
// FUNCTION: STBC 0x78E090
void NiSoundSystem::RemoveSource( NiSource *source ) {
	if ( source->GetRefCount() ) {
		sources->Remove( source );
	}
}

/*
================
NiSoundSystem::Update
================
*/
// FUNCTION: STBC 0x78E0E0
void NiSoundSystem::Update( bool updateAll, float time ) {
	if ( listener && updateAll ) {
		listener->Update();
		UpdateSources();
	}
}

/*
================
NiSoundSystem::PlaySources
================
*/
// FUNCTION: STBC 0x78E110
void NiSoundSystem::PlaySources() {
	NiTListIterator pos = sources->GetHeadPos();
	while ( pos ) {
		NiSource *source = sources->GetNext( pos );
		source->Play();
	}
}

/*
================
NiSoundSystem::StopSources
================
*/
// FUNCTION: STBC 0x78E170
void NiSoundSystem::StopSources() {
	NiTListIterator pos = sources->GetHeadPos();
	while ( pos ) {
		NiSource *source = sources->GetNext( pos );
		source->Stop();
	}
}

/*
================
NiSoundSystem::UpdateSources
================
*/
// FUNCTION: STBC 0x78E1D0
void NiSoundSystem::UpdateSources() {
	NiTListIterator pos = sources->GetHeadPos();
	while ( pos ) {
		NiSource *source = sources->GetNext( pos );
		source->Update();
	}
}

/*
================
NiSoundSystem::GetFirstSource
================
*/
// FUNCTION: STBC 0x78E230
NiSource *NiSoundSystem::GetFirstSource( NiTListIterator &pos ) {
	if ( sources ) {
		pos = sources->GetHeadPos();
		return pos ? sources->GetNext( pos ) : 0;
	}
	return 0;
}

/*
================
NiSoundSystem::GetNextSource
================
*/
// FUNCTION: STBC 0x78E310
NiSource *NiSoundSystem::GetNextSource( NiTListIterator &pos ) {
	if ( sources ) {
		return pos ? sources->GetNext( pos ) : 0;
	}
	return 0;
}

/*
================
NiSoundSystem::GetFirstProvider
================
*/
// FUNCTION: STBC 0x78E3F0
NiProvider_Info *NiSoundSystem::GetFirstProvider( NiTListIterator &pos ) {
	if ( providers ) {
		pos = providers->GetHeadPos();
		return pos ? providers->GetNext( pos ) : 0;
	}
	return 0;
}

/*
================
NiSoundSystem::GetNextProvider
================
*/
// FUNCTION: STBC 0x78E4D0
NiProvider_Info *NiSoundSystem::GetNextProvider( NiTListIterator &pos ) {
	if ( providers ) {
		return pos ? providers->GetNext( pos ) : 0;
	}
	return 0;
}

/*
================
NiSoundSystem::SetProviderPreference
================
*/
// FUNCTION: STBC 0x78E5B0
void NiSoundSystem::SetProviderPreference( NiProvider_Info *provider, const char *name, const void *value ) {
	if ( provider ) {
		AIL_set_3D_provider_preference( provider->GetProvider(), name, value );
	}
}

/*
================
NiSoundSystem::GetProviderAttribute
================
*/
// FUNCTION: STBC 0x78E5D0
void NiSoundSystem::GetProviderAttribute( NiProvider_Info *provider, const char *name, void *value ) {
	if ( provider ) {
		AIL_3D_provider_attribute( provider->GetProvider(), name, value );
	}
}

/*
================
NiSoundSystem::SetUnitsPerMeter
================
*/
// FUNCTION: STBC 0x78E5F0
bool NiSoundSystem::SetUnitsPerMeter( float unitsPerMeter_ ) {
	if ( unitsPerMeter_ <= 0.0 ) {
		return false;
	}
	unitsPerMeter = unitsPerMeter_;
	return true;
}

/*
================
NiSoundSystem::SupportsRoomType

Asks Miles directly when the provider is closed, and opens and closes it
around the question when it is open: the test is the wrong way round.
================
*/
// FUNCTION: STBC 0x78E620
bool NiSoundSystem::SupportsRoomType( NiProvider_Info *provider ) {
	if ( !provider ) {
		return false;
	}
	if ( !provider->IsOpen() ) {
		return AIL_3D_room_type( provider->GetProvider() ) != -1;
	}
	provider->Open();
	bool supported = AIL_3D_room_type( provider->GetProvider() ) != -1;
	provider->Close();
	return supported;
}

/*
================
NiSoundSystem::SetRoomType
================
*/
// FUNCTION: STBC 0x78E680
bool NiSoundSystem::SetRoomType( NiProvider_Info *provider, int roomType ) {
	if ( !provider ) {
		return false;
	}
	AIL_set_3D_room_type( provider->GetProvider(), roomType );
	return AIL_3D_room_type( provider->GetProvider() ) != -1;
}

/*
================
NiSoundSystem::GetRoomType
================
*/
// FUNCTION: STBC 0x78E6C0
int NiSoundSystem::GetRoomType( NiProvider_Info *provider ) {
	if ( !provider ) {
		return 0;
	}
	return AIL_3D_room_type( provider->GetProvider() );
}

/*
================
NiSoundSystem::CreateObject

Loads into the one sound system rather than a new object.
================
*/
// FUNCTION: STBC 0x78E6E0
void NiSoundSystem::CreateObject( NiStream &stream, NiObject *&object ) {
	NiSoundSystem *system = soundSystem;
	stream.SetLoadRecord( new NiSoundSystemLoadRecord );
	system->LoadBinary( stream );
	object = system;
}

/*
================
NiSoundSystem::LoadBinary
================
*/
// FUNCTION: STBC 0x78E750
void NiSoundSystem::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );
	float units;
	NiStreamLoadBinary( stream, units );
	SetUnitsPerMeter( units );
	NiStreamLoadBinary( stream, unknown08 );
}

/*
================
NiSoundSystem::LinkObject
================
*/
// FUNCTION: STBC 0x78E790
void NiSoundSystem::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiSoundSystem::RegisterStreamables
================
*/
// FUNCTION: STBC 0x78E7A0
bool NiSoundSystem::RegisterStreamables( NiStream &stream ) {
	if ( !NiObject::RegisterStreamables( stream ) ) {
		return false;
	}
	return true;
}

/*
================
NiSoundSystem::SaveBinary
================
*/
// FUNCTION: STBC 0x78E7C0
void NiSoundSystem::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, unitsPerMeter );
	NiStreamSaveBinary( stream, unknown08 );
}

/*
================
NiSoundSystem::IsEqual
================
*/
// FUNCTION: STBC 0x78E7F0
bool NiSoundSystem::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}
	if ( ( (NiSoundSystem *)object )->unitsPerMeter != unitsPerMeter ) {
		return false;
	}
	return true;
}
