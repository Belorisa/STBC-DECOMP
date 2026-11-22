/*
===============================================================================

	NiSource.h

	A sound in the scene: an NiAVObject that plays a Miles sample or stream,
	and hands its world position, velocity and (when it has a cone) its
	orientation to Miles on each update when the sample is a 3D one.

	Evidence: the NiRTTI (0x99FDF4) is named "NiSource" with NiAVObject's as
	its base; the vtable at 0x898060 (NiAVObject's 39 slots, overriding
	GetRTTI, CreateClone, the streaming slots, the destructor and slot 26).
	CreateObject (0x7904C0) allocates 0x100 bytes. The constructor
	(0x78F380) sets the type at 0xD0 to "none" (3), clears the file name
	(0xC8), the block at 0xCC, the provider (0xD8), the file image (0xDC)
	and the Miles handle (0xE0), faces the cone down -x (0xE4) with y up
	(0xF0), marks the default playback rate at 0xFC unknown (-1) and adds
	the source to the sound system. The loaders set the type: a 3D sample
	(0) on a provider, a 2D sample (1) of the digital driver, or a stream
	(2); every Miles call is chosen by it. The flags of NiAVObject carry
	four more bits: the cone was set (0x08), loaded (0x10), looping (0x20)
	and streamed (0x40).

	Names: the class's own; the member names follow the Gamebryo lineage
	(NiAudioSource: the cone data, the min/max distance, Play, Stop, Rewind,
	the status) where marked, the rest are invented after the Miles calls
	they wrap.

===============================================================================
*/

#ifndef __NISOURCE_H__
#define __NISOURCE_H__

#include "NiAVObject.h"
#include "NiMilesImports.h"
#include "NiStreamMacros.h"

class NiProvider_Info;

/*
================
NiSourceLoadRecord

The record CreateObject hands the stream: NiAVObject's, under a vtable of
its own (0x898134). The class name is invented.
================
*/
// FUNCTION: STBC 0x7905A0 ??_GNiSourceLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7905C0 ??1NiSourceLoadRecord@@UAE@XZ
class NiSourceLoadRecord : public NiAVObjectLoadRecord {
};

// FUNCTION: STBC 0x790C50 ??_GNiSource@@UAEPAXI@Z
class NiSource : public NiAVObject {
public:
	// FUNCTION: STBC 0x790C40 ?GetRTTI@NiSource@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiSource();

	// FUNCTION: STBC 0x790C90 ?VirtualSlot26@NiSource@@UAE_NXZ
	virtual bool			VirtualSlot26() { return false; }

							NiSource();

	// what the Miles handle is
	enum SourceType {
		TYPE_3D,			// a 3D sample on a provider
		TYPE_2D,			// a 2D sample of the digital driver
		TYPE_STREAM,		// a stream of the digital driver
		TYPE_NONE			// nothing loaded
	};

	// what GetStatus reports (names invented)
	enum Status {
		STATUS_PLAYING,
		STATUS_LOOPING,
		STATUS_2,			// never reported
		STATUS_DONE,		// finished or stopped
		STATUS_UNKNOWN		// nothing loaded, or a status Miles has no match for
	};

	// loading: a stream opened from the file with streamMemory bytes of
	// buffer; a 2D sample from a file image, its type given by the file
	// name's suffix; a 3D sample from a file image on a provider, in a
	// handle the caller allocated (the size is unused). False when already
	// loaded or Miles refuses (names invented)
	bool					LoadStream( const char *filename_, int streamMemory );
	bool					LoadSample( int size, void *fileImage_, const char *fileTypeSuffix );
	bool					Load3DSample( int size, void *fileImage_, NiProvider_Info *provider_, H3DSAMPLE handle_ );
	// releases what a loader opened; false when nothing was (name invented)
	bool					Unload();
	// releases the Miles handle by its type, loaded or not; the sound
	// system's Shutdown calls it (name invented)
	void					ReleaseHandle();

	// the cone of a 3D sample: the angles in degrees and the volume outside
	// it, 0 to 1 (Gamebryo lineage names)
	bool					SetConeData( float innerAngle, float outerAngle, float outerVolume );
	void					GetConeData( float &innerAngle, float &outerAngle, float &outerVolume );
	// the distances of a 3D sample (Gamebryo lineage names)
	bool					SetMinMaxDistance( float minDistance, float maxDistance );
	void					GetMinMaxDistance( float &minDistance, float &maxDistance );

	// the volume, 0 to 1 (names invented)
	bool					SetVolume( float volume );
	float					GetVolume();

	// a 3D sample's provider preferences and attributes, passed through to
	// Miles (names invented)
	void					SetPreference( const char *name, const void *value );
	void					GetAttribute( const char *name, void *value );

	// samples per second; -1 restores the rate the sound was loaded with
	// (names invented)
	bool					SetPlaybackRate( int rate );
	int						GetPlaybackRate();
	int						GetDefaultPlaybackRate();

	// playing (Gamebryo lineage names); Play loops when the loop flag is set
	bool					Play();
	bool					Stop();
	bool					Resume();
	void					Rewind();
	Status					GetStatus();

	// TODO identity: always false (0x78FCC0); TGSound::SetPlayPositionSeconds
	// passes it a time in seconds
	bool					Unknown78FCC0( float );

	// the play position and the length, in seconds; the position in bytes
	// (names invented)
	float					GetPlayTime();
	float					GetPlayLength();
	bool					SetPosition( int position );
	int						GetPosition();

	// hands the world position, velocity and orientation to a 3D sample
	// (Gamebryo lineage name)
	void					Update();

	// a 3D sample's effects level, occlusion and obstruction (Miles' names)
	bool					SetEffectsLevel( float effectsLevel );
	float					GetEffectsLevel();
	bool					SetOcclusion( float occlusion );
	float					GetOcclusion();
	bool					SetObstruction( float obstruction );
	float					GetObstruction();

	// what Miles reports for a 3D sample, in this object's axes; ZERO when
	// it is not one (names invented)
	NiPoint3				GetSourcePosition();
	void					GetSourceOrientation( NiPoint3 &dir, NiPoint3 &upDir );
	NiPoint3				GetSourceVelocity();

	// the model-space cone direction and up that Update hands a 3D sample
	// (names invented)
	void					SetDirection( const NiPoint3 &dir ) { direction = dir; }
	const NiPoint3 &		GetDirection() const { return direction; }
	void					SetUp( const NiPoint3 &upDir ) { up = upDir; }
	const NiPoint3 &		GetUp() const { return up; }

	// the loop flag Play reads (names invented)
	void					SetLoop( bool loop ) { if ( loop ) { flags |= LOOP_MASK; } else { flags &= ~LOOP_MASK; } }
	bool					GetLoop() const { return ( flags & LOOP_MASK ) != 0; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiSource *dest );

	// flags beyond NiAVObject's
	enum {
		CONE_POS			= 3,	// SetConeData was called: Update orients the sample
		LOADED_POS			= 4,
		LOOP_MASK			= 0x0020,
		STREAMED_POS		= 6
	};

	bool					GetBit( unsigned short pos ) const { return ( flags >> pos ) & 1; }
	void					SetBit( bool value, unsigned short pos ) { if ( value ) { flags |= 1 << pos; } else { flags &= ~( 1 << pos ); } }

	char *					filename;		// owned copy of the stream's file name
	char *					unknownCC;		// freed by the destructor
	SourceType				type;
	int						unknownD4;
	NiProvider_Info *		provider;		// the provider of a 3D sample
	void *					fileImage;		// the sample's file image, owned by the caller
	void *					handle;			// the Miles sample or stream
	NiPoint3				direction;		// model-space cone direction
	NiPoint3				up;				// model-space up
	int						defaultPlaybackRate;	// the rate at load time, -1 until read
};

NiRegisterStream( NiSource );

#endif /* !__NISOURCE_H__ */
