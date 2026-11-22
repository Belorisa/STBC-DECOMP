// NiSource.cpp -- the Miles samples and streams of the scene

#include "NiRTLib.h"
#include <string.h>
#include "NiProvider_Info.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiStream.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id pair.
// FUNCTION: STBC 0x00790b10 _$E200
// FUNCTION: STBC 0x00790b30 _$E199
// FUNCTION: STBC 0x00790a90 _$E17
// FUNCTION: STBC 0x00790aa0 _$E14
// FUNCTION: STBC 0x00790ab0 _$E16
// FUNCTION: STBC 0x00790ac0 _$E15
// FUNCTION: STBC 0x00790ad0 _$E23
// FUNCTION: STBC 0x00790ae0 _$E20
// FUNCTION: STBC 0x00790af0 _$E22
// FUNCTION: STBC 0x00790b00 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00790b40 _$E30
// FUNCTION: STBC 0x00790b50 _$E29
// FUNCTION: STBC 0x00790b60 _$E47
// FUNCTION: STBC 0x00790b70 _$E46
// FUNCTION: STBC 0x00790b80 _$E51
// FUNCTION: STBC 0x00790b90 _$E50
// FUNCTION: STBC 0x00790ba0 _$E55
// FUNCTION: STBC 0x00790bb0 _$E54
// FUNCTION: STBC 0x00790bc0 _$E59
// FUNCTION: STBC 0x00790bd0 _$E58
// FUNCTION: STBC 0x00790be0 _$E183
// FUNCTION: STBC 0x00790bf0 _$E182
// FUNCTION: STBC 0x00790c20 _$E43
// FUNCTION: STBC 0x00790c30 _$E42
// FUNCTION: STBC 0x00790c70 _$E187
// FUNCTION: STBC 0x00790c80 _$E186

// FUNCTION: STBC 0x0078f1d0 _$E193
// FUNCTION: STBC 0x0078f1e0 _$E189
// FUNCTION: STBC 0x0078f200 _$E192
// FUNCTION: STBC 0x0078f210 _$E190
// DATA: STBC 0x99FDF4 ?rtti@NiSource@@2VNiRTTI@@B
const NiRTTI NiSource::rtti( "NiSource", &NiAVObject::rtti );

/*
================
NiSource::Register
================
*/
// FUNCTION: STBC 0x78F230 ?Register@NiSource@@SAHXZ
NiImplementRegister( NiSource )

/*
================
NiSource::NiSource

Nothing loaded; the cone faces down -x with y up. Adds itself to the sound
system.
================
*/
// FUNCTION: STBC 0x78F380
NiSource::NiSource() {
	flags &= ~LOOP_MASK;
	SetBit( false, STREAMED_POS );
	type = TYPE_NONE;
	filename = 0;
	unknownCC = 0;
	fileImage = 0;
	handle = 0;
	provider = 0;
	direction = -NiPoint3::UNIT_X;
	up = NiPoint3::UNIT_Y;
	SetBit( false, CONE_POS );
	SetBit( false, LOADED_POS );
	defaultPlaybackRate = -1;
	NiSoundSystem::GetSoundSystem()->AddSource( this );
}

/*
================
NiSource::~NiSource
================
*/
// FUNCTION: STBC 0x78F460
NiSource::~NiSource() {
	ReleaseHandle();
	delete [] filename;
	delete [] unknownCC;
	NiSoundSystem::GetSoundSystem()->RemoveSource( this );
}

/*
================
NiSource::ReleaseHandle

A 3D sample's handle belongs to whoever allocated it and is only
forgotten. The loaded flag stays set.
================
*/
// FUNCTION: STBC 0x78F4E0
void NiSource::ReleaseHandle() {
	if ( !handle ) {
		return;
	}
	if ( type == TYPE_STREAM ) {
		AIL_pause_stream( handle, 1 );
		AIL_close_stream( handle );
	} else if ( type == TYPE_2D ) {
		AIL_end_sample( handle );
		AIL_release_sample_handle( handle );
	}
	fileImage = 0;
	handle = 0;
	type = TYPE_NONE;
}

/*
================
NiSource::LoadStream
================
*/
// FUNCTION: STBC 0x78F550
bool NiSource::LoadStream( const char *filename_, int streamMemory ) {
	if ( GetBit( LOADED_POS ) ) {
		return false;
	}
	filename = new char[strlen( filename_ ) + 1];
	strcpy( filename, filename_ );
	if ( streamMemory < 0 ) {
		streamMemory = 0;
	}
	handle = AIL_open_stream( NiSoundSystem::GetSoundSystem()->GetDriver(), filename, streamMemory );
	if ( !handle ) {
		delete [] filename;
		filename = 0;
		return false;
	}
	SetBit( true, LOADED_POS );
	SetBit( true, STREAMED_POS );
	type = TYPE_STREAM;
	return true;
}

/*
================
NiSource::LoadSample
================
*/
// FUNCTION: STBC 0x78F600
bool NiSource::LoadSample( int size, void *fileImage_, const char *fileTypeSuffix ) {
	if ( GetBit( LOADED_POS ) ) {
		return false;
	}
	handle = AIL_allocate_sample_handle( NiSoundSystem::GetSoundSystem()->GetDriver() );
	if ( handle && AIL_set_named_sample_file( handle, fileTypeSuffix, fileImage_, size, 0 ) ) {
		SetBit( true, LOADED_POS );
		fileImage = fileImage_;
		type = TYPE_2D;
		return true;
	}
	if ( handle ) {
		AIL_release_sample_handle( handle );
		handle = 0;
	}
	return false;
}

/*
================
NiSource::Load3DSample

Opens the provider first when it is closed.
================
*/
// FUNCTION: STBC 0x78F690
bool NiSource::Load3DSample( int size, void *fileImage_, NiProvider_Info *provider_, H3DSAMPLE handle_ ) {
	if ( GetBit( LOADED_POS ) ) {
		return false;
	}
	if ( !provider_->IsOpen() && !provider_->Open() ) {
		return false;
	}
	handle = handle_;
	if ( handle && AIL_set_3D_sample_file( handle, fileImage_ ) ) {
		SetBit( true, LOADED_POS );
		fileImage = fileImage_;
		type = TYPE_3D;
		provider = provider_;
		return true;
	}
	handle = 0;
	return false;
}

/*
================
NiSource::Unload
================
*/
// FUNCTION: STBC 0x78F710
bool NiSource::Unload() {
	if ( GetBit( LOADED_POS ) && handle ) {
		ReleaseHandle();
		return true;
	}
	return false;
}

/*
================
NiSource::SetConeData
================
*/
// FUNCTION: STBC 0x78F730
bool NiSource::SetConeData( float innerAngle, float outerAngle, float outerVolume ) {
	if ( handle && type == TYPE_3D ) {
		AIL_set_3D_sample_cone( handle, innerAngle, outerAngle, (int)( outerVolume * 127.0 ) );
		SetBit( true, CONE_POS );
		return true;
	}
	return false;
}

/*
================
NiSource::GetConeData
================
*/
// FUNCTION: STBC 0x78F780
void NiSource::GetConeData( float &innerAngle, float &outerAngle, float &outerVolume ) {
	if ( handle && type == TYPE_3D ) {
		int volume;
		AIL_3D_sample_cone( handle, &innerAngle, &outerAngle, &volume );
		outerVolume = volume * ( 1.0 / 127.0 );
	}
}

/*
================
NiSource::SetMinMaxDistance
================
*/
// FUNCTION: STBC 0x78F7C0
bool NiSource::SetMinMaxDistance( float minDistance, float maxDistance ) {
	if ( handle && type == TYPE_3D ) {
		AIL_set_3D_sample_distances( handle, maxDistance, minDistance );
		return true;
	}
	return false;
}

/*
================
NiSource::GetMinMaxDistance
================
*/
// FUNCTION: STBC 0x78F7F0
void NiSource::GetMinMaxDistance( float &minDistance, float &maxDistance ) {
	if ( handle && type == TYPE_3D ) {
		AIL_3D_sample_distances( handle, &maxDistance, &minDistance );
	}
}

/*
================
NiSource::SetVolume

Clamps the volume to 0 to 1.
================
*/
// FUNCTION: STBC 0x78F820
bool NiSource::SetVolume( float volume ) {
	if ( !handle ) {
		return false;
	}
	if ( volume < 0.0f ) {
		volume = 0.0f;
	} else if ( volume > 1.0f ) {
		volume = 1.0f;
	}
	if ( type == TYPE_3D ) {
		AIL_set_3D_sample_volume( handle, (int)( volume * 127.0f ) );
	} else if ( type == TYPE_STREAM ) {
		AIL_set_stream_volume( handle, (int)( volume * 127.0f ) );
	} else {
		AIL_set_sample_volume( handle, (int)( volume * 127.0f ) );
	}
	return true;
}

/*
================
NiSource::GetVolume
================
*/
// FUNCTION: STBC 0x78F8C0
float NiSource::GetVolume() {
	if ( !handle ) {
		return 0.0f;
	}
	if ( type == TYPE_3D ) {
		return AIL_3D_sample_volume( handle ) * ( 1.0f / 127.0f );
	}
	if ( type == TYPE_STREAM ) {
		return AIL_stream_volume( handle ) * ( 1.0f / 127.0f );
	}
	return AIL_sample_volume( handle ) * ( 1.0f / 127.0f );
}

/*
================
NiSource::SetPreference
================
*/
// FUNCTION: STBC 0x78F930
void NiSource::SetPreference( const char *name, const void *value ) {
	if ( GetBit( LOADED_POS ) && handle ) {
		AIL_set_3D_sample_preference( handle, name, value );
	}
}

/*
================
NiSource::GetAttribute
================
*/
// FUNCTION: STBC 0x78F960
void NiSource::GetAttribute( const char *name, void *value ) {
	if ( GetBit( LOADED_POS ) && handle ) {
		AIL_3D_sample_attribute( handle, name, value );
	}
}

/*
================
NiSource::SetPlaybackRate

Miles takes 5 to 176000 samples per second.
================
*/
// FUNCTION: STBC 0x78F990
bool NiSource::SetPlaybackRate( int rate ) {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return false;
	}
	if ( rate == -1 ) {
		rate = GetDefaultPlaybackRate();
	}
	if ( rate < 5 || rate > 176000 ) {
		return false;
	}
	GetDefaultPlaybackRate();
	if ( type == TYPE_3D ) {
		AIL_set_3D_sample_playback_rate( handle, rate );
	} else if ( type == TYPE_STREAM ) {
		AIL_set_stream_playback_rate( handle, rate );
	} else {
		AIL_set_sample_playback_rate( handle, rate );
	}
	return true;
}

/*
================
NiSource::GetPlaybackRate
================
*/
// FUNCTION: STBC 0x78FA20
int NiSource::GetPlaybackRate() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return -1;
	}
	if ( type == TYPE_3D ) {
		return AIL_3D_sample_playback_rate( handle );
	}
	if ( type == TYPE_STREAM ) {
		return AIL_stream_playback_rate( handle );
	}
	return AIL_sample_playback_rate( handle );
}

/*
================
NiSource::GetDefaultPlaybackRate

The rate the first call finds.
================
*/
// FUNCTION: STBC 0x78FA60
int NiSource::GetDefaultPlaybackRate() {
	if ( defaultPlaybackRate == -1 ) {
		defaultPlaybackRate = GetPlaybackRate();
	}
	return defaultPlaybackRate;
}

/*
================
NiSource::Play

Plays once, or without end when the loop flag is set.
================
*/
// FUNCTION: STBC 0x78FA80
bool NiSource::Play() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return false;
	}
	if ( !( flags & LOOP_MASK ) ) {
		if ( type == TYPE_3D ) {
			AIL_set_3D_sample_loop_count( handle, 1 );
		} else if ( type == TYPE_STREAM ) {
			AIL_set_stream_loop_count( handle, 1 );
		} else if ( type == TYPE_2D ) {
			AIL_set_sample_loop_count( handle, 1 );
		}
	} else {
		if ( type == TYPE_3D ) {
			AIL_set_3D_sample_loop_count( handle, 0 );
		} else if ( type == TYPE_STREAM ) {
			AIL_set_stream_loop_count( handle, 0 );
		} else if ( type == TYPE_2D ) {
			AIL_set_sample_loop_count( handle, 0 );
		}
	}
	if ( type == TYPE_3D ) {
		AIL_start_3D_sample( handle );
	} else if ( type == TYPE_STREAM ) {
		AIL_start_stream( handle );
	} else {
		AIL_start_sample( handle );
	}
	return true;
}

/*
================
NiSource::Stop

A stream is paused.
================
*/
// FUNCTION: STBC 0x78FB50
bool NiSource::Stop() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return false;
	}
	if ( type == TYPE_3D ) {
		AIL_stop_3D_sample( handle );
	} else if ( type == TYPE_STREAM ) {
		AIL_pause_stream( handle, 1 );
	} else {
		AIL_stop_sample( handle );
	}
	return true;
}

/*
================
NiSource::Resume
================
*/
// FUNCTION: STBC 0x78FBA0
bool NiSource::Resume() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return false;
	}
	if ( type == TYPE_3D ) {
		AIL_resume_3D_sample( handle );
	} else if ( type == TYPE_STREAM ) {
		AIL_pause_stream( handle, 0 );
	} else {
		AIL_resume_sample( handle );
	}
	return true;
}

/*
================
NiSource::Rewind

A stream is restarted.
================
*/
// FUNCTION: STBC 0x78FBF0
void NiSource::Rewind() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return;
	}
	if ( type == TYPE_3D ) {
		AIL_set_3D_sample_offset( handle, 0 );
	} else if ( type == TYPE_STREAM ) {
		AIL_start_stream( handle );
	} else {
		AIL_set_sample_position( handle, 0 );
	}
}

/*
================
NiSource::GetStatus
================
*/
// FUNCTION: STBC 0x78FC30
NiSource::Status NiSource::GetStatus() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return STATUS_UNKNOWN;
	}
	unsigned int status;
	if ( type == TYPE_3D ) {
		status = AIL_3D_sample_status( handle );
	} else if ( type == TYPE_STREAM ) {
		status = AIL_stream_status( handle );
	} else {
		status = AIL_sample_status( handle );
	}
	if ( status == SMP_STOPPED || status == SMP_DONE ) {
		return STATUS_DONE;
	}
	if ( ( status == SMP_PLAYING ) & ( ( flags & LOOP_MASK ) == LOOP_MASK ) ) {
		return STATUS_LOOPING;
	}
	if ( ( status == SMP_PLAYING ) & !( flags & LOOP_MASK ) ) {
		return STATUS_PLAYING;
	}
	return STATUS_UNKNOWN;
}

/*
================
NiSource::Unknown78FCC0
================
*/
// FUNCTION: STBC 0x78FCC0 ?Unknown78FCC0@NiSource@@QAE_NM@Z
bool NiSource::Unknown78FCC0( float ) {
	return false;
}

/*
================
NiSource::GetPlayTime

The position over the bytes per second, taking a sample to be bits / 8
bytes whatever its channels.
================
*/
// FUNCTION: STBC 0x78FCD0
float NiSource::GetPlayTime() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return 0.0f;
	}
	AILSOUNDINFO info;
	if ( type == TYPE_3D ) {
		AIL_WAV_info( fileImage, &info );
		double offset = AIL_3D_sample_offset( handle );
		float time = offset / ( AIL_3D_sample_playback_rate( handle ) * ( info.bits / 8.0 ) );
		return time;
	}
	if ( type == TYPE_STREAM ) {
		int milliseconds;
		AIL_stream_ms_position( handle, 0, &milliseconds );
		return milliseconds * 0.001;
	}
	AIL_WAV_info( fileImage, &info );
	double position = AIL_sample_position( handle );
	return position / ( AIL_sample_playback_rate( handle ) * ( info.bits / 8.0 ) );
}

/*
================
NiSource::GetPlayLength
================
*/
// FUNCTION: STBC 0x78FDF0
float NiSource::GetPlayLength() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return 0.0f;
	}
	AILSOUNDINFO info;
	if ( type == TYPE_3D ) {
		AIL_WAV_info( fileImage, &info );
		float samples = info.samples;
		return samples / AIL_3D_sample_playback_rate( handle );
	}
	if ( type == TYPE_STREAM ) {
		int milliseconds;
		AIL_stream_ms_position( handle, &milliseconds, 0 );
		return milliseconds * 0.001f;
	}
	AIL_WAV_info( fileImage, &info );
	float samples = info.samples;
	return samples / AIL_sample_playback_rate( handle );
}

/*
================
NiSource::SetPosition
================
*/
// FUNCTION: STBC 0x78FEE0
bool NiSource::SetPosition( int position ) {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return false;
	}
	if ( type == TYPE_3D ) {
		AIL_set_3D_sample_offset( handle, position );
	} else if ( type == TYPE_STREAM ) {
		AIL_set_stream_position( handle, position );
	} else {
		AIL_set_sample_position( handle, position );
	}
	return true;
}

/*
================
NiSource::GetPosition
================
*/
// FUNCTION: STBC 0x78FF40
int NiSource::GetPosition() {
	if ( !GetBit( LOADED_POS ) || !handle ) {
		return 0;
	}
	if ( type == TYPE_3D ) {
		return AIL_3D_sample_offset( handle );
	}
	if ( type == TYPE_STREAM ) {
		return AIL_stream_position( handle );
	}
	return AIL_sample_position( handle );
}

/*
================
NiSource::Update

As NiListener::Update: Miles' z axis points the other way, and its
velocities are in metres per millisecond.
================
*/
// FUNCTION: STBC 0x78FF80
// PARKED: x87 scheduling, as NiListener::Update: retail computes 1/scale as fld 1.0; fdiv st(1) and pops the scale after; this build folds it to fdivr [1.0]
void NiSource::Update() {
	if ( !handle || type != TYPE_3D ) {
		return;
	}

	NiPoint3 position = worldTranslate;
	NiPoint3 velocity = worldVelocity;
	velocity *= 1.0f / ( NiSoundSystem::GetSoundSystem()->GetUnitsPerMeter() * 1000.0f );
	velocity.z = -velocity.z;
	position.z *= -1.0;
	AIL_set_3D_position( handle, position.x, position.y, position.z );
	AIL_set_3D_velocity_vector( handle, velocity.x, velocity.y, velocity.z );

	if ( GetBit( CONE_POS ) ) {
		NiPoint3 dir = worldRotate * direction;
		NiPoint3 upDir = worldRotate * up;
		dir.z *= -1.0;
		upDir.z *= -1.0;
		AIL_set_3D_orientation( handle, dir.x, dir.y, dir.z, upDir.x, upDir.y, upDir.z );
	}
}

/*
================
NiSource::SetEffectsLevel
================
*/
// FUNCTION: STBC 0x7900E0
bool NiSource::SetEffectsLevel( float effectsLevel ) {
	if ( handle && type == TYPE_3D ) {
		AIL_set_3D_sample_effects_level( handle, effectsLevel );
		return true;
	}
	return false;
}

/*
================
NiSource::GetEffectsLevel
================
*/
// FUNCTION: STBC 0x790110
float NiSource::GetEffectsLevel() {
	if ( handle && type == TYPE_3D ) {
		return AIL_3D_sample_effects_level( handle );
	}
	return 0.0f;
}

/*
================
NiSource::SetOcclusion
================
*/
// FUNCTION: STBC 0x790140
bool NiSource::SetOcclusion( float occlusion ) {
	if ( handle && type == TYPE_3D ) {
		AIL_set_3D_sample_occlusion( handle, occlusion );
		return true;
	}
	return false;
}

/*
================
NiSource::GetOcclusion
================
*/
// FUNCTION: STBC 0x790170
float NiSource::GetOcclusion() {
	if ( handle && type == TYPE_3D ) {
		return AIL_3D_sample_occlusion( handle );
	}
	return 0.0f;
}

/*
================
NiSource::SetObstruction
================
*/
// FUNCTION: STBC 0x7901A0
bool NiSource::SetObstruction( float obstruction ) {
	if ( handle && type == TYPE_3D ) {
		AIL_set_3D_sample_obstruction( handle, obstruction );
		return true;
	}
	return false;
}

/*
================
NiSource::GetObstruction
================
*/
// FUNCTION: STBC 0x7901D0
float NiSource::GetObstruction() {
	if ( handle && type == TYPE_3D ) {
		return AIL_3D_sample_obstruction( handle );
	}
	return 0.0f;
}

/*
================
NiSource::GetSourcePosition
================
*/
// FUNCTION: STBC 0x790200
NiPoint3 NiSource::GetSourcePosition() {
	if ( handle && type == TYPE_3D ) {
		NiPoint3 position;
		AIL_3D_position( handle, &position.x, &position.y, &position.z );
		position.z *= -1.0;
		return position;
	}
	return NiPoint3::ZERO;
}

/*
================
NiSource::GetSourceOrientation
================
*/
// FUNCTION: STBC 0x790280
void NiSource::GetSourceOrientation( NiPoint3 &dir, NiPoint3 &upDir ) {
	if ( handle && type == TYPE_3D ) {
		AIL_3D_orientation( handle, &dir.x, &dir.y, &dir.z, &upDir.x, &upDir.y, &upDir.z );
		dir.z *= -1.0;
		upDir.z *= -1.0;
	}
}

/*
================
NiSource::GetSourceVelocity
================
*/
// FUNCTION: STBC 0x7902E0
NiPoint3 NiSource::GetSourceVelocity() {
	if ( handle && type == TYPE_3D ) {
		NiPoint3 velocity;
		AIL_3D_velocity( handle, &velocity.x, &velocity.y, &velocity.z );
		float scale = NiSoundSystem::GetSoundSystem()->GetUnitsPerMeter() * 1000.0f;
		velocity = velocity * scale;
		velocity.z = -velocity.z;
		return velocity;
	}
	return NiPoint3::ZERO;
}

/*
================
NiSource::CreateClone
================
*/
// FUNCTION: STBC 0x790380
NiObject *NiSource::CreateClone() {
	NiSource *clone = new NiSource;
	CopyMembers( clone );
	return clone;
}

/*
================
NiSource::CopyMembers

Copies the type and the orientation, and through Miles the cone, the
distances and the volume; not the sound itself.
================
*/
// FUNCTION: STBC 0x7903F0
void NiSource::CopyMembers( NiSource *dest ) {
	NiAVObject::CopyMembers( dest );
	dest->type = type;
	dest->direction = direction;
	dest->up = up;

	// RULE-EXCEPTION: R30, retail keeps the cone and the distances in the
	// same three frame slots, first with first: one set of locals serves both
	float value0, value1, value2;
	GetConeData( value0, value1, value2 );
	dest->SetConeData( value0, value1, value2 );
	GetMinMaxDistance( value0, value1 );
	dest->SetMinMaxDistance( value0, value1 );

	dest->SetVolume( GetVolume() );
}

/*
================
NiSource::CreateObject
================
*/
// FUNCTION: STBC 0x7904C0
void NiSource::CreateObject( NiStream &stream, NiObject *&object ) {
	NiSource *source = new NiSource;
	stream.SetLoadRecord( new NiSourceLoadRecord );
	source->LoadBinary( stream );
	object = source;
}

/*
================
NiSource::LoadBinary

Reads the file name and the provider's name and drops both: the sound is
not loaded, and the provider found is not kept.
================
*/
// FUNCTION: STBC 0x7905E0
void NiSource::LoadBinary( NiStream &stream ) {
	NiAVObject::LoadBinary( stream );
	NiStreamLoadBinary( stream, type );

	char *name = 0;
	stream.LoadCString( name );
	if ( type == TYPE_3D ) {
		char *providerName = 0;
		stream.LoadCString( providerName );
		NiTListIterator pos;
		NiProvider_Info *provider_ = NiSoundSystem::GetSoundSystem()->GetFirstProvider( pos );
		while ( provider_ ) {
			if ( strstr( provider_->GetName(), providerName ) ) {
				break;
			}
			provider_ = NiSoundSystem::GetSoundSystem()->GetNextProvider( pos );
		}
		if ( !provider_ ) {
			// BUG: the provider found or chosen here is never stored
			provider_ = NiSoundSystem::GetSoundSystem()->GetFirstProvider( pos );
		}
		delete [] providerName;
	}
	delete [] name;

	direction.LoadBinary( stream );
	up.LoadBinary( stream );

	// RULE-EXCEPTION: R30, retail reads the cone, the distances, the volume
	// and the last float into the same frame slots, first with first
	float value0, value1, value2;
	NiStreamLoadBinary( stream, value0 );
	NiStreamLoadBinary( stream, value1 );
	NiStreamLoadBinary( stream, value2 );
	SetConeData( value0, value1, value2 );

	NiStreamLoadBinary( stream, value0 );
	NiStreamLoadBinary( stream, value1 );
	bool loop;
	// BUG: the loop flag is read and dropped
	NiStreamLoadBinary( stream, loop );
	SetMinMaxDistance( value0, value1 );

	NiStreamLoadBinary( stream, value0 );
	SetVolume( value0 );
	// BUG: reads one float more than SaveBinary writes
	NiStreamLoadBinary( stream, value0 );
}

/*
================
NiSource::LinkObject
================
*/
// FUNCTION: STBC 0x790770
void NiSource::LinkObject( NiStream &stream ) {
	NiAVObject::LinkObject( stream );
}

/*
================
NiSource::RegisterStreamables
================
*/
// FUNCTION: STBC 0x790780
bool NiSource::RegisterStreamables( NiStream &stream ) {
	return NiAVObject::RegisterStreamables( stream );
}

/*
================
NiSource::SaveBinary
================
*/
// FUNCTION: STBC 0x790790
void NiSource::SaveBinary( NiStream &stream ) {
	NiAVObject::SaveBinary( stream );
	NiStreamSaveBinary( stream, type );
	stream.SaveCString( filename );
	if ( type == TYPE_3D ) {
		stream.SaveCString( provider->GetName() );
	}
	direction.SaveBinary( stream );
	up.SaveBinary( stream );

	// RULE-EXCEPTION: R30, as LoadBinary: one set of locals for the cone and
	// the distances
	float value0, value1, value2;
	GetConeData( value0, value1, value2 );
	NiStreamSaveBinary( stream, value0 );
	NiStreamSaveBinary( stream, value1 );
	NiStreamSaveBinary( stream, value2 );

	GetMinMaxDistance( value0, value1 );
	NiStreamSaveBinary( stream, value0 );
	NiStreamSaveBinary( stream, value1 );
	bool loop;
	// BUG: writes the loop byte without ever setting it
	NiStreamSaveBinary( stream, loop );

	float volume = GetVolume();
	NiStreamSaveBinary( stream, volume );
}

/*
================
NiSource::IsEqual
================
*/
// FUNCTION: STBC 0x7908A0
bool NiSource::IsEqual( NiObject *object ) {
	if ( !NiAVObject::IsEqual( object ) ) {
		return false;
	}

	NiSource *source = (NiSource *)object;
	if ( type != source->type ) {
		return false;
	}
	if ( provider != source->provider ) {
		return false;
	}
	if ( strcmp( filename, source->filename ) ) {
		return false;
	}
	if ( direction != source->direction ) {
		return false;
	}
	if ( up != source->up ) {
		return false;
	}
	if ( ( flags & LOOP_MASK ) != ( source->flags & LOOP_MASK ) ) {
		return false;
	}

	// RULE-EXCEPTION: R30, as LoadBinary: one set of locals per object for
	// the cone and the distances
	float value0, value1, value2;
	float sourceValue0, sourceValue1, sourceValue2;
	GetConeData( value0, value1, value2 );
	source->GetConeData( sourceValue0, sourceValue1, sourceValue2 );
	if ( value0 != sourceValue0 || value1 != sourceValue1 || value2 != sourceValue2 ) {
		return false;
	}

	GetMinMaxDistance( value0, value1 );
	source->GetMinMaxDistance( sourceValue0, sourceValue1 );
	if ( value0 != sourceValue0 || value1 != sourceValue1 ) {
		return false;
	}

	if ( GetVolume() != source->GetVolume() ) {
		return false;
	}
	return true;
}
