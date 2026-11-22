/*
===============================================================================

	NiMilesImports.h

	The Miles Sound System entry points the NetImmerse sound classes call,
	declared as MSS32.DLL exports them (the import table's decorated names
	give the stdcall argument sizes). The SDK's own header did not survive;
	only the imports and handle types these classes use are declared here.

	Evidence: NiProvider_Info opens and closes its provider through
	_AIL_open_3D_provider@4 and _AIL_close_3D_provider@4 (IAT 0x888468,
	0x88846C); NiListener opens a listener per provider
	(_AIL_open_3D_listener@4, 0x888480), closes them
	(_AIL_close_3D_listener@4, 0x888484) and sets and reads their position,
	orientation and velocity (0x888470 to 0x88847C, 0x88854C, 0x888550);
	NiSoundSystem starts and shuts Miles down, enumerates the providers and
	passes their speaker and room types, preferences and attributes
	through (0x888440 to 0x888464, 0x888498, 0x888534).

===============================================================================
*/

#ifndef __NIMILESIMPORTS_H__
#define __NIMILESIMPORTS_H__

// a 3D provider, as AIL_enumerate_3D_providers hands it out
typedef unsigned int		HPROVIDER;

// the position of a provider enumeration; HPROENUM_FIRST starts it
typedef unsigned int		HPROENUM;
#define HPROENUM_FIRST		0

// a 3D object of a provider: a listener or a sample
typedef void *				H3DPOBJECT;

// the digital output driver AIL_quick_startup opens
typedef void *				HDIGDRIVER;

// a 2D sample of the digital driver
typedef void *				HSAMPLE;

// a 3D sample of a provider
typedef H3DPOBJECT			H3DSAMPLE;

// a stream of the digital driver
typedef void *				HSTREAM;

// what AIL_WAV_info reads from a WAV image in memory
typedef struct {
	int			format;			// the WAVE format tag
	const void *	data_ptr;		// the sample data within the image
	unsigned int	data_len;		// its size in bytes
	unsigned int	rate;			// samples per second
	int			bits;			// bits per sample
	int			channels;
	unsigned int	samples;
	unsigned int	block_size;
	const void *	initial_ptr;
} AILSOUNDINFO;

// the status a sample or stream reports
#define SMP_FREE				0x0001
#define SMP_DONE				0x0002
#define SMP_PLAYING				0x0004
#define SMP_STOPPED				0x0008
#define SMP_PLAYINGBUTRELEASED	0x0010

// the 3D provider calls' result; 0 is success
typedef int					M3DRESULT;
#define M3D_NOERR			0

#define AILCALL				__stdcall

extern "C" {

__declspec( dllimport ) char * AILCALL		AIL_set_redist_directory( const char *dir );
__declspec( dllimport ) int AILCALL			AIL_quick_startup( int useDigital, int useMIDI, unsigned int outputRate, int outputBits,
															   int outputChannels );
__declspec( dllimport ) void AILCALL		AIL_quick_handles( HDIGDRIVER *pdig, void **pmdi, void **pdls );
__declspec( dllimport ) void AILCALL		AIL_quick_shutdown( void );
__declspec( dllimport ) char * AILCALL		AIL_last_error( void );

__declspec( dllimport ) int AILCALL			AIL_enumerate_3D_providers( HPROENUM *next, HPROVIDER *dest, char **name );
__declspec( dllimport ) M3DRESULT AILCALL	AIL_open_3D_provider( HPROVIDER lib );
__declspec( dllimport ) void AILCALL		AIL_close_3D_provider( HPROVIDER lib );
__declspec( dllimport ) int AILCALL			AIL_active_3D_sample_count( HPROVIDER lib );
__declspec( dllimport ) M3DRESULT AILCALL	AIL_set_3D_provider_preference( HPROVIDER lib, const char *name, const void *value );
__declspec( dllimport ) void AILCALL		AIL_3D_provider_attribute( HPROVIDER lib, const char *name, void *value );
__declspec( dllimport ) void AILCALL		AIL_set_3D_speaker_type( HPROVIDER lib, int speakerType );
__declspec( dllimport ) int AILCALL			AIL_3D_speaker_type( HPROVIDER lib );
__declspec( dllimport ) void AILCALL		AIL_set_3D_room_type( HPROVIDER lib, int roomType );
__declspec( dllimport ) int AILCALL			AIL_3D_room_type( HPROVIDER lib );

__declspec( dllimport ) H3DPOBJECT AILCALL	AIL_open_3D_listener( HPROVIDER lib );
__declspec( dllimport ) void AILCALL		AIL_close_3D_listener( H3DPOBJECT listener );

__declspec( dllimport ) void AILCALL		AIL_set_3D_position( H3DPOBJECT obj, float x, float y, float z );
__declspec( dllimport ) void AILCALL		AIL_set_3D_velocity_vector( H3DPOBJECT obj, float dxPerMs, float dyPerMs,
																	   float dzPerMs );
__declspec( dllimport ) void AILCALL		AIL_set_3D_orientation( H3DPOBJECT obj, float xFace, float yFace, float zFace,
																   float xUp, float yUp, float zUp );
__declspec( dllimport ) void AILCALL		AIL_3D_position( H3DPOBJECT obj, float *x, float *y, float *z );
__declspec( dllimport ) void AILCALL		AIL_3D_velocity( H3DPOBJECT obj, float *dxPerMs, float *dyPerMs,
															float *dzPerMs );
__declspec( dllimport ) void AILCALL		AIL_3D_orientation( H3DPOBJECT obj, float *xFace, float *yFace, float *zFace,
															   float *xUp, float *yUp, float *zUp );

// 2D samples
__declspec( dllimport ) HSAMPLE AILCALL		AIL_allocate_sample_handle( HDIGDRIVER dig );
__declspec( dllimport ) void AILCALL		AIL_release_sample_handle( HSAMPLE s );
__declspec( dllimport ) int AILCALL			AIL_set_named_sample_file( HSAMPLE s, const char *fileTypeSuffix, const void *fileImage,
																	  int fileSize, int block );
__declspec( dllimport ) void AILCALL		AIL_start_sample( HSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_stop_sample( HSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_resume_sample( HSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_end_sample( HSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_sample_volume( HSAMPLE s, int volume );
__declspec( dllimport ) int AILCALL			AIL_sample_volume( HSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_sample_playback_rate( HSAMPLE s, int playbackRate );
__declspec( dllimport ) int AILCALL			AIL_sample_playback_rate( HSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_sample_loop_count( HSAMPLE s, int loopCount );
__declspec( dllimport ) void AILCALL		AIL_set_sample_position( HSAMPLE s, unsigned int pos );
__declspec( dllimport ) unsigned int AILCALL	AIL_sample_position( HSAMPLE s );
__declspec( dllimport ) unsigned int AILCALL	AIL_sample_status( HSAMPLE s );
__declspec( dllimport ) int AILCALL			AIL_WAV_info( const void *data, AILSOUNDINFO *info );

// 3D samples
__declspec( dllimport ) H3DSAMPLE AILCALL	AIL_allocate_3D_sample_handle( HPROVIDER lib );
__declspec( dllimport ) void AILCALL		AIL_release_3D_sample_handle( H3DSAMPLE s );
__declspec( dllimport ) unsigned int AILCALL	AIL_set_3D_sample_file( H3DSAMPLE s, const void *fileImage );
__declspec( dllimport ) void AILCALL		AIL_start_3D_sample( H3DSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_stop_3D_sample( H3DSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_resume_3D_sample( H3DSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_volume( H3DSAMPLE s, int volume );
__declspec( dllimport ) int AILCALL			AIL_3D_sample_volume( H3DSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_playback_rate( H3DSAMPLE s, int playbackRate );
__declspec( dllimport ) int AILCALL			AIL_3D_sample_playback_rate( H3DSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_loop_count( H3DSAMPLE s, unsigned int loops );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_offset( H3DSAMPLE s, unsigned int offset );
__declspec( dllimport ) unsigned int AILCALL	AIL_3D_sample_offset( H3DSAMPLE s );
__declspec( dllimport ) unsigned int AILCALL	AIL_3D_sample_status( H3DSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_distances( H3DSAMPLE s, float maxDist, float minDist );
__declspec( dllimport ) void AILCALL		AIL_3D_sample_distances( H3DSAMPLE s, float *maxDist, float *minDist );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_cone( H3DSAMPLE s, float innerAngle, float outerAngle, int outerVolume );
__declspec( dllimport ) void AILCALL		AIL_3D_sample_cone( H3DSAMPLE s, float *innerAngle, float *outerAngle, int *outerVolume );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_effects_level( H3DSAMPLE s, float effectsLevel );
__declspec( dllimport ) float AILCALL		AIL_3D_sample_effects_level( H3DSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_occlusion( H3DSAMPLE s, float occlusion );
__declspec( dllimport ) float AILCALL		AIL_3D_sample_occlusion( H3DSAMPLE s );
__declspec( dllimport ) void AILCALL		AIL_set_3D_sample_obstruction( H3DSAMPLE s, float obstruction );
__declspec( dllimport ) float AILCALL		AIL_3D_sample_obstruction( H3DSAMPLE s );
__declspec( dllimport ) M3DRESULT AILCALL	AIL_set_3D_sample_preference( H3DSAMPLE s, const char *name, const void *value );
__declspec( dllimport ) void AILCALL		AIL_3D_sample_attribute( H3DSAMPLE s, const char *name, void *value );

// streams
__declspec( dllimport ) HSTREAM AILCALL		AIL_open_stream( HDIGDRIVER dig, const char *filename, int streamMem );
__declspec( dllimport ) void AILCALL		AIL_close_stream( HSTREAM stream );
__declspec( dllimport ) void AILCALL		AIL_start_stream( HSTREAM stream );
__declspec( dllimport ) void AILCALL		AIL_pause_stream( HSTREAM stream, int onOff );
__declspec( dllimport ) void AILCALL		AIL_set_stream_volume( HSTREAM stream, int volume );
__declspec( dllimport ) int AILCALL			AIL_stream_volume( HSTREAM stream );
__declspec( dllimport ) void AILCALL		AIL_set_stream_playback_rate( HSTREAM stream, int rate );
__declspec( dllimport ) int AILCALL			AIL_stream_playback_rate( HSTREAM stream );
__declspec( dllimport ) void AILCALL		AIL_set_stream_loop_count( HSTREAM stream, int count );
__declspec( dllimport ) void AILCALL		AIL_set_stream_position( HSTREAM stream, int offset );
__declspec( dllimport ) int AILCALL			AIL_stream_position( HSTREAM stream );
__declspec( dllimport ) int AILCALL			AIL_stream_status( HSTREAM stream );
__declspec( dllimport ) void AILCALL		AIL_stream_ms_position( HSTREAM stream, int *totalMilliseconds, int *currentMilliseconds );

}

#endif /* !__NIMILESIMPORTS_H__ */
