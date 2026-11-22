/*
===============================================================================

	Game.h

	The game being played: the script object above the episodes, which
	owns the player's ship, its group and camera, the player's record
	(score, rating, kills, torpedo counts), the difficulty and its damage
	multipliers, and the episode being played. There is one at a time
	(currentGame).

	0x74 bytes, allocated from the pools. Evidence: the constructor
	(0x405C10) calls ScriptObject's with the first event type 0x804001,
	stores the vtable 0x8887E8 (28 slots: ScriptObject's, with 24, 26 and
	27 overridden), sets currentGame (0x97E238) and builds the player's
	group (0x58) and the game's windows; the stream constructor (0x405B10)
	is called by MultiplayerGame's. Both clear the members through
	0x405AD0. GetObjType returns 0x80CE; the class strings are "Game",
	"_p_Game" and "GamePtr". The SWIG inline accessors read GetScore 0x38,
	GetRating 0x3C, GetKills 0x40, GetTorpsFired 0x44, GetTorpsHit 0x48,
	GetLastSavedGame 0x50, GetPlayer 0x54, GetPlayerGroup 0x58,
	SetGodMode/InGodMode 0x60 and GetCurrentEpisode 0x70; the stream
	functions (0x406D80, 0x406F30) give the types of the rest. The
	shadow class carries the difficulty constants EASY, MEDIUM and HARD.

===============================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "ScriptObject.h"

// GetObjType's value for a Game.
#define GAME_TYPE			0x80CE

// Event types (the SWIG constants of the App module).
#define ET_QUIT						0x800002	// the game ends: ReallyTerminate
#define ET_LOAD_EPISODE				0x80000C	// a TGStringEvent naming the episode to load
#define ET_SET_PLAYER				0x80000E	// the player's ship changes
#define ET_FIRST_GAME_SCRIPT_EVENT	0x804001	// the first type the game hands its script

class CameraObjectClass;
class Episode;
class ObjectGroup;
class SetClass;
class ShipClass;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00405be0 ??_GGame@@UAEPAXI@Z @game/Game
class Game : public ScriptObject {
public:
	// The difficulty levels (the shadow class's constants).
	enum difficulty_t {
		EASY,
		MEDIUM,
		HARD
	};

					Game( char *script );
					Game( long id );
	virtual			~Game( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// ScriptObject
	virtual void	OnIdle( void );
	virtual void	Terminate( TGEvent *event );
	virtual void	AddPersistentModule( char *module );

	static int		GetNextEventType( void );

	static Game *	GetCurrentGame( void ) { return currentGame; }
	Episode *		GetCurrentEpisode( void ) { return currentEpisode; }

	int				GetScore( void ) { return score; }
	int				GetRating( void ) { return rating; }
	int				GetKills( void ) { return kills; }
	int				GetTorpsFired( void ) { return torpsFired; }
	int				GetTorpsHit( void ) { return torpsHit; }
	char *			GetLastSavedGame( void ) { return lastSavedGame; }
	void			SetLastSavedGame( char *name );
	void			SetGodMode( bool godMode_ ) { godMode = godMode_; }
	bool			InGodMode( void ) { return godMode; }

	// The player's ship: setting it posts ET_SET_PLAYER from the old ship.
	ShipClass *		GetPlayer( void ) { return player; }
	void			SetPlayer( ShipClass *ship );
	static ShipClass *	GetCurrentPlayer( void );
	ObjectGroup *	GetPlayerGroup( void ) { return playerGroup; }
	SetClass *		GetPlayerSet( void );
	CameraObjectClass *	GetPlayerCamera( void );
	// Points the tactical displays at the player's ship.
	void			SetUIShipID( void );

	// The event sent once the episode has preloaded; a reference is taken.
	void			SetPreLoadDoneEvent( TGEvent *event );

	// Starts the game once the CD check passes: the check calls
	// CDCheckPassed, which initializes the script with the event
	// (invented names).
	void			Start( TGEvent *event );
	void			CDCheckPassed( void );

	// Called by each mission as it initializes: the first takes the
	// splash screen down and shows the bridge; the episode's done event
	// is sent (invented name).
	void			MissionLoaded( void );

	// Makes the named episode current, terminating the one played first;
	// doneEvent is sent once it has loaded.
	void			LoadEpisode( char *name, TGEvent *doneEvent );

	// The difficulty, which a running single-player game keeps; the
	// multipliers scale the damage the player deals (offensive) and takes
	// (defensive) at each level.
	static void		SetDifficulty( int difficulty );
	static void		SetDifficultyReallyIMeanIt( int difficulty );
	static int		GetDifficulty( void ) { return difficulty; }
	static void		SetDifficultyMultipliers( float easyOffensive, float easyDefensive, float mediumOffensive, float mediumDefensive, float hardOffensive, float hardDefensive );
	static void		SetDefaultDifficultyMultipliers( float easyOffensive, float easyDefensive, float mediumOffensive, float mediumDefensive, float hardOffensive, float hardDefensive );
	static float	GetOffensiveDifficultyMultiplier( void );
	static float	GetDefensiveDifficultyMultiplier( void );
	// Puts the defaults back (invented name).
	static void		ResetDifficultyMultipliers( void );

	// The hardpoint file of the player's ship; a heap copy.
	static void		SetPlayerHardpointFileName( char *name );
	static char *	GetPlayerHardpointFileName( void ) { return playerHardpointFileName; }

	// Forgets the current game and the hardpoint file name without freeing
	// either (invented name).
	static void		ClearStatics( void );

	// A new sound in the named group; NULL without a file or a name.
	TGSound *		LoadSoundInGroup( char *fileName, char *name, char *group, long flags );
	// The sound of the name's file in the database (Game_LoadDatabaseSoundInGroup).
	TGSound *		LoadDatabaseSoundInGroup( TGLocalizationDatabase *db, char *name, char *group, long flags ) { return LoadSoundInGroup( db->GetFilename( name ), name, group, flags ); }

	// Register the class's handler methods by name, and add them to its
	// table; the episodes' and missions' go with them (invented names).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );

	// The handlers.
	void			HandleSetPlayer( TGEvent *event );
	void			DestroyedPlayer( TGEvent *event );
	void			LoadEpisodeHandler( TGEvent *event );
	void			ReallyTerminate( TGEvent *event );

	// The shutdown (VirtualSlot03) clears unknown62 before terminating the game.
	friend class UtopiaModule;

protected:
	// Clears the members both constructors set alike (invented name).
	void			Init( void );
	// Deletes the saved missions ("*.msv") in MissionLib's
	// TEMP_MISSION_SAVE_FOLDER.
	void			ClearMissionTempDir( void );

	int				score;					// 0x38
	int				rating;					// 0x3C
	int				kills;					// 0x40
	int				torpsFired;				// 0x44
	int				torpsHit;				// 0x48
	float			unknown4C;				// 0x4C
	char *			lastSavedGame;			// 0x50 a heap copy
	ShipClass *		player;					// 0x54 an ID between ReadFromStream and PostFixUp
	ObjectGroup *	playerGroup;			// 0x58 owned; likewise an ID while loading
	long			playerCameraID;			// 0x5C the camera MakePlayerCamera made (invented name)
	bool			godMode;				// 0x60
	bool			missionLoaded;			// 0x61 set by the first MissionLoaded (invented name)
	bool			unknown62;				// 0x62 set at construction, never saved; UtopiaModule's shutdown clears it
	TGEvent *		unknown64;				// 0x64 a reference held; sent by MissionLoaded
	TGEvent *		preLoadDoneEvent;		// 0x68 a reference held, sent by OnIdle once preloading ends
	TGEvent *		unknown6C;				// 0x6C the event Terminate was given; a reference held
	Episode *		currentEpisode;			// 0x70 the episode being played

	static Game *	currentGame;			// the game being played; NULL without one
	static char *	playerHardpointFileName;
	static int		difficulty;				// a difficulty_t
	static float	difficultyMultipliers[6];			// offensive and defensive, per difficulty (invented name)
	static float	defaultDifficultyMultipliers[6];	// likewise (invented name)
	static TGEventHandlerTable	classHandlers;			// invented name

	// Declared for the SWIG wrappers that call them.
public:
	static Game *	Create( char *script ) { return new Game( script ); }	// the SWIG wrapper Game_Create expands it
};

#endif /* !__GAME_H__ */
