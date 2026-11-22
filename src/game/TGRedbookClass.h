/*
===============================================================================

	TGRedbookClass.h

	The CD audio player: plays the tracks of the audio CD in the drive
	through the Windows MCI, keeps the volume of the CD output and reports
	when a track has ended. One instance, g_kRedbook, is the SWIG variable
	of that name.

	0x90 bytes. Evidence: OpenRedbookCD (0x712770) closes the device it had
	(0x08), clears the 30 track lengths at 0x0C, the playing track at 0x84
	and the finished flag at 0x8C, remembers the CD output's volume at 0x04
	(-1 when no output has one), opens "cdaudio", sets the time format to
	track-minute-second-frame, asks the number of tracks (0x88) and the
	length of each; PlayRedbookCDTrack (0x712920) plays a track and sets the
	time (0x00) at which it ends from the game time and the track's length;
	UpdateRedbookCD (0x712D20) compares them. The names of the members are
	invented.

===============================================================================
*/

#ifndef __TGREDBOOKCLASS_H__
#define __TGREDBOOKCLASS_H__

#define TGREDBOOK_MAX_TRACKS	30

class TGRedbookClass {
public:
	bool	OpenRedbookCD( void );
	bool	PlayRedbookCDTrack( int track, int minutes, int seconds, bool looping );
	bool	StopRedbookCDTrack( void );
	bool	CloseRedbookCD( void );
	bool	IsRedbookCDTrackFinishedPlaying( void );
	int	GetCDTrackLengthInMilliseconds( int track );
	void	SetRedbookCDVolume( int volume );
	bool	FadeRedbookCDVolume( int from, int to, int milliseconds );
	void	UpdateRedbookCD( void );

	unsigned int	GetCDID( void ) { return device; }
	void			SetCDDonePlaying( void ) { finished = true; }

protected:
	unsigned long	endTime;							// 0x00 game time at which the track ends (invented name)
	unsigned long	savedVolume;						// 0x04 the CD output's volume when opened; -1 when it has none (invented name)
	unsigned int	device;								// 0x08 the MCI device; 0 when closed (invented name)
	unsigned long	trackLengths[TGREDBOOK_MAX_TRACKS];	// 0x0C the length of track n at [n - 1], as minutes, seconds and frames (invented name)
	int				currentTrack;						// 0x84 the track playing; 0 when none (invented name)
	int				numTracks;							// 0x88 (invented name)
	bool			finished;							// 0x8C the track ended and does not loop (invented name)
	bool			looping;							// 0x8D the track starts over when it ends (invented name)
};

// The CD player (the SWIG variable g_kRedbook).
extern TGRedbookClass	g_kRedbook;

#endif /* !__TGREDBOOKCLASS_H__ */
