/*
===============================================================================

	UtopiaModule.h

	The game module: g_kUtopiaModule (0x97FA00) is the one instance. The
	members of UtopiaModule that the SWIG wrappers UtopiaModule_* call, and
	the members the inline accessors among them read and write. No virtual
	functions are declared yet.

	Evidence: the SWIG variable g_kUtopiaModule (string 0x956AE0); the
	game passes 0x97FA00 to IncrementClientID (PhysicsObjectClass's
	InitObject 0x5A0040). The accessor wrappers read GetPlayerNumber 0x84,
	IsClient 0x88, IsMultiplayer 0x89, IsHost 0x8A, IsProcessingPackets
	0x8B, GetUnusedClientID 0x8C, IsIgnoreClientIDForObjectCreation 0xA1,
	IsModemConnection 0xA2 and IsLoading 0xAC; GetCamera 0x28,
	GetSaveFilename 0x2C and GetLoadFilename 0x30; the friendly-fire
	getters and setters the floats 0x40-0x54 (GetFriendlyFireTolerance,
	Get/SetCurrentFriendlyFire, Get/SetFriendlyFireWarningPoints,
	Get/SetFriendlyTractorTime, Get/SetFriendlyTractorWarning,
	Get/SetMaxFriendlyTractorTime); GetNetwork 0x78 and GetGameSpy 0x7C.
	UtopiaModule_ForceUnpause expands a loop of Pause( false ) calls while
	the count at 0x3C is positive. TerminateNetwork deletes the NetFile at
	0x80. The clock wrappers
	(UtopiaModule_GetGameTime, _SetGameTime, _GetRealTime, _IsGamePaused,
	_SetTimeRate, _SetTimeScale) and UtopiaModule_IsDirectStart and
	_GetTestMenuState reach the application instead, through
	NiApplication::application.

	A TGModule of priority 0x20000000. Evidence: the constructor (0x43AB30,
	run by the dynamic initializer 0x43AA80) calls TGModule's, stores the
	vtable 0x889B84 (9 slots), clears the members, builds the TGString at
	0x90 from "Player" and keeps a copy of the program's path at 0xA8; the
	destructor (0x43AAB0) releases the camera (0x28). SetGameName,
	SetGamePath and SetDataPath keep copies at 0x14, 0x18 and 0x1C;
	SaveToFile, SetLoadFromFileName and SetInternalLoadFileName at 0x2C,
	0x30 and 0x34, the last two falling back on the name at 0x38; Pause
	counts at 0x3C. The unit (0x43AA70 to 0x4468xx) also holds the class
	creators and the handler registrations of the game's classes, and the
	inline copies they expand.

===============================================================================
*/

#ifndef __UTOPIAMODULE_H__
#define __UTOPIAMODULE_H__

#include "NiPointer.h"
#include "TGModule.h"
#include "TGString.h"
#include "UtopiaApp.h"

// Event types (the SWIG constants of the App module).
#define ET_SET_LOAD_FILE			0x800003	// a TGStringEvent naming the saved game to load
#define ET_CANCEL_LOAD				0x800004
// The SWIG constants after ET_SAVE_GAME (0x800007), in their order:
// ProcessSave sends ET_GAME_SAVED to the top window.
#define ET_GAME_SAVED				0x80000A
#define ET_GAME_LOADED				0x80000B

// The version ProcessSave writes after the saved game's "UtopiaSV" tag
// (invented name).
#define SAVE_VERSION				1.0048f

class GameSpy;
class NetFile;
class NiCamera;
class TGObject;
class TGNetwork;
class TGStream;

// The status of the dial-up (RAS) connections: 0 when one is connected
// (invented name; defined with the network code, 0x6A8170).
int		GetRasConnectionStatus( void );

// The game's paths from its registry key, as new strings, and the drive
// letter of the CD with the game's data (invented names; UtopiaModule's
// VirtualSlot00 hands the first two to SetGamePath and SetDataPath).
char *	GetRegistryInstallPath( void );
char *	GetRegistryDataPath( void );
char *	GetRegistryExecutablePath( void );
char	FindCDDrive( void );

// The implicit destructor: releases the camera and the captain's name (no
// vptr store; TGModule's destructor is not virtual).
// FUNCTION: STBC 0x0043aab0 ??1UtopiaModule@@QAE@XZ
class UtopiaModule : public TGModule {
public:
					UtopiaModule( void );

	// TGModule
	virtual void	VirtualSlot00( void );
	virtual void	VirtualSlot01( void );
	virtual void	VirtualSlot02( void );
	virtual void	VirtualSlot03( void );
	virtual void	VirtualSlot05( void );
	virtual void	VirtualSlot06( void );
	virtual void	VirtualSlot07( int argument );

	void	RenderDefaultCamera( void );
	// Files the creators of the game's classes and links their handler tables
	// (invented name).
	static void	RegisterClasses( void );
	// Register the handler methods of the game's classes by name, and add
	// them to the classes' tables (invented names).
	void			RegisterHandlerNames( void );
	static void	RegisterHandlers( void );
	// Files the save's state functions of the AIs, the debug printers and
	// the points (invented name).
	void			AddStateFunctions( void );
	// The frame's work: the save SaveToFile asked for, the load
	// SetLoadFromFileName asked for (through the CD check window), the load
	// SetInternalLoadFileName asked for, and the windows' updates (invented
	// names).
	void	ProcessSave( void );
	void	ProcessLoadFromFile( void );
	void	ProcessInternalLoad( void );
	// The saved mission states of the temporary folder into a saved game's
	// stream, and one of them (invented names).
	void	WriteTempMissionSaves( TGStream *stream );
	void	WriteTempMissionSave( TGStream *stream, char *fileName );
	// The saved mission states WriteTempMissionSaves wrote, back into the
	// temporary folder (invented names).
	void	ReadTempMissionSaves( TGStream *stream );
	void	ReadTempMissionSave( TGStream *stream, char *fileName );
	void	UpdateWindows( void );
	void	Pause( bool );
	void	SaveToFile( char * );
	void	LoadFromFile( char * );
	void	SetLoadFromFileName( char * );
	void	SetInternalLoadFileName( char * );
	char *	GetProgramPath( void ) { return programPath; }	// the CD check reads the program file (invented name)
	bool	SaveMissionState( char * );
	bool	LoadMissionState( char * );
	static int	GetNextEventType( void );
	// The creators of event classes no unit of their own files: the object
	// of the class with the ID (invented names).
	static TGObject *	CreateVarManagerClass( long id );
	static TGObject *	CreateBaseObjectClass( long id );
	static TGObject *	CreateSetClass( long id );
	static TGObject *	CreateScriptObject( long id );
	static TGObject *	CreateGame( long id );
	static TGObject *	CreateObjectClass( long id );
	static TGObject *	CreateCameraMode( long id );
	static TGObject *	CreateIdealControlledCameraMode( long id );
	static TGObject *	CreateChaseCameraMode( long id );
	static TGObject *	CreateTargetCameraMode( long id );
	static TGObject *	CreateZoomTargetMode( long id );
	static TGObject *	CreateMapCameraMode( long id );
	static TGObject *	CreateDropAndWatchMode( long id );
	static TGObject *	CreatePlacementWatchMode( long id );
	static TGObject *	CreateLockedPositionMode( long id );
	static TGObject *	CreateTorpCameraMode( long id );
	static TGObject *	CreatePlaceByDirectionMode( long id );
	static TGObject *	CreateCameraObjectClass( long id );
	static TGObject *	CreateObjectGroup( long id );
	static TGObject *	CreateMetaNebula( long id );
	static TGObject *	CreateProximityEvent( long id );
	static TGObject *	CreateObjectExplodingEvent( long id );
	static TGObject *	CreateWarpEvent( long id );
	static TGObject *	CreateWaypointEvent( long id );
	static TGObject *	CreateServerListEvent( long id );
	static TGObject *	CreateSortServerListEvent( long id );
	static TGObject *	CreateSubtitleAction( long id );
	static TGObject *	CreateEpisode( long id );
	static TGObject *	CreateMission( long id );
	static TGObject *	CreateSetFloatVarEvent( long id );
	static TGObject *	CreateBackdrop( long id );
	static TGObject *	CreateBackdropSphere( long id );
	static TGObject *	CreateStarSphere( long id );
	static TGObject *	CreateSpaceCamera( long id );
	static TGObject *	CreateAsteroidField( long id );
	static TGObject *	CreateMapWindow( long id );
	static TGObject *	CreateSTNumericBar( long id );
	static TGObject *	CreatePhysicsObjectClass( long id );
	static TGObject *	CreateCollisionEvent( long id );
	static TGObject *	CreateDamageableObject( long id );
	static TGObject *	CreateShipClass( long id );
	static TGObject *	CreateTorpedo( long id );
	static TGObject *	CreateWeaponHitEvent( long id );
	static TGObject *	CreateStartFiringEvent( long id );
	static TGObject *	CreateShipSubsystem( long id );
	static TGObject *	CreateHullClass( long id );
	static TGObject *	CreatePoweredSubsystem( long id );
	static TGObject *	CreateWeaponSystem( long id );
	static TGObject *	CreateSubsystemProperty( long id );
	static TGObject *	CreateWeaponSystemProperty( long id );
	static TGObject *	CreateTorpedoSystemProperty( long id );
	static TGObject *	CreateTorpedoSystem( long id );
	static TGObject *	CreatePhaserSystem( long id );
	static TGObject *	CreatePowerSubsystem( long id );
	static TGObject *	CreateSensorProperty( long id );
	static TGObject *	CreateSensorSubsystem( long id );
	static TGObject *	CreateImpulseEngineSubsystem( long id );
	static TGObject *	CreateWarpEngineProperty( long id );
	static TGObject *	CreateEngineProperty( long id );
	static TGObject *	CreateWarpEngineSubsystem( long id );
	static TGObject *	CreatePulseWeaponSystem( long id );
	static TGObject *	CreateTractorBeamSystem( long id );
	static TGObject *	CreateRepairSubsystem( long id );
	static TGObject *	CreateCloakingSubsystem( long id );
	static TGObject *	CreateWeapon( long id );
	static TGObject *	CreateEnergyWeapon( long id );
	static TGObject *	CreateTractorBeamProjector( long id );
	static TGObject *	CreatePulseWeapon( long id );
	static TGObject *	CreateShieldProperty( long id );
	static TGObject *	CreateShieldClass( long id );
	static TGObject *	CreateNebula( long id );
	static TGObject *	CreateCloakingSubsystemProperty( long id );
	static TGObject *	CreateWeaponProperty( long id );
	static TGObject *	CreateEnergyWeaponProperty( long id );
	static TGObject *	CreatePlacementObject( long id );
	static TGObject *	CreateWaypoint( long id );
	static TGObject *	CreateLightPlacement( long id );
	static TGObject *	CreateEditor( long id );
	static TGObject *	CreatePlacementEditor( long id );
	static TGObject *	CreateBackgroundEditor( long id );
	static TGObject *	CreateAsteroidFieldPlacement( long id );
	static TGObject *	CreateLightObjectClass( long id );
	static TGObject *	CreateWarpSet( long id );
	static TGObject *	CreateWarpFlash( long id );
	static TGObject *	CreateWarpSequence( long id );
	static TGObject *	CreateZoomCameraObjectClass( long id );
	static TGObject *	CreateViewScreenObject( long id );
	static TGObject *	CreateCharacterSpeakingQueue( long id );
	static TGObject *	CreateCharacterClass( long id );
	static TGObject *	CreateSayLineEvent( long id );
	static TGObject *	CreateBridgeSet( long id );
	static TGObject *	CreateConditionScript( long id );
	static TGObject *	CreateConditionEventCreator( long id );
	static TGObject *	CreatePositionOrientationProperty( long id );
	static TGObject *	CreatePhaserProperty( long id );
	static TGObject *	CreateHullProperty( long id );
	static TGObject *	CreateTorpedoTubeProperty( long id );
	static TGObject *	CreatePulseWeaponProperty( long id );
	static TGObject *	CreateImpulseEngineProperty( long id );
	static TGObject *	CreateTractorBeamProperty( long id );
	static TGObject *	CreateShipProperty( long id );
	static TGObject *	CreateEffectEmitterProperty( long id );
	static TGObject *	CreateSmokeEmitterProperty( long id );
	static TGObject *	CreateSparkEmitterProperty( long id );
	static TGObject *	CreateExplodeEmitterProperty( long id );
	static TGObject *	CreateRepairSubsystemProperty( long id );
	static TGObject *	CreatePowerProperty( long id );
	static TGObject *	CreateBlinkingLightProperty( long id );
	static TGObject *	CreateObjectEmitterProperty( long id );
	static TGObject *	CreateTorpedoTube( long id );
	static TGObject *	CreatePhaserBank( long id );
	static TGObject *	CreateReticleManagerWindow( long id );
	static TGObject *	CreateNetFile( long id );
	static TGObject *	CreateGameSpyObject( long id );
	static TGObject *	CreateChangeRenderedSetAction( long id );
	static TGObject *	CreateMultiplayerGame( long id );
	static TGObject *	CreateChatObjectClass( long id );
	static TGObject *	CreateAsteroidTile( long id );
	static TGObject *	CreateObjectGroupWithInfo( long id );
	static TGObject *	CreateProximityCheck( long id );
	static TGObject *	CreateGridClass( long id );
	static TGObject *	CreatePulsingLight( long id );
	static TGObject *	CreateCharacterAction( long id );
	static TGObject *	CreateBridgeObjectClass( long id );
	static TGObject *	CreatePlanet( long id );
	static TGObject *	CreateSun( long id );
	// The version string players must share (UtopiaModule_GetGameVersion).
	static char *	GetGameVersion( void ) { return gameVersion; }
	// Game units to kilometres and back, by the factor below.
	static float	ConvertGameUnitsToKilometers( float units ) { return units * gameUnitConversionFactor; }
	static float	ConvertKilometersToGameUnits( float kilometers ) { return kilometers / gameUnitConversionFactor; }
	static void		SetGameUnitConversionFactor( float factor ) { gameUnitConversionFactor = factor; }
	bool	DetermineModemConnection( void );
	void	TerminateNetwork( void );
	void	CreateGameSpy( void );
	void	TerminateGameSpy( void );
	// Takes the next client ID that no object has: IDs run within the
	// player's block of 0x40000.
	void	IncrementClientID( void );
	// Undoes every pause.
	void	ForceUnpause( void ) { while ( pauseCount > 0 ) { Pause( false ); } }
	void	SetGameName( char * );
	void	SetGamePath( char * );
	void	SetDataPath( char * );

	void	SetIsClient( bool isClient_ ) { isClient = isClient_; }
	bool	IsClient( void ) { return isClient; }
	void	SetMultiplayer( bool multiplayer_ ) { multiplayer = multiplayer_; }
	bool	IsMultiplayer( void ) { return multiplayer; }
	void	SetIsHost( bool isHost_ ) { isHost = isHost_; }
	bool	IsHost( void ) { return isHost; }
	bool	IsModemConnection( void ) { return modemConnection; }
	void	SetProcessingPackets( bool processingPackets_ ) { processingPackets = processingPackets_; }
	bool	IsProcessingPackets( void ) { return processingPackets; }
	void	SetPlayerNumber( int playerNumber_ ) { playerNumber = playerNumber_; }
	int		GetPlayerNumber( void ) { return playerNumber; }
	bool	IsLoading( void ) { return loading; }
	void	SetUnusedClientID( long id ) { unusedClientID = id; }
	long	GetUnusedClientID( void ) { return unusedClientID; }
	void	SetIgnoreClientIDForObjectCreation( bool ignore ) { ignoreClientIDForObjectCreation = ignore; }
	bool	IsIgnoreClientIDForObjectCreation( void ) { return ignoreClientIDForObjectCreation; }
	TGNetwork *	GetNetwork( void ) { return network; }
	NetFile *	GetNetFile( void ) { return netFile; }		// invented name
	GameSpy *	GetGameSpy( void ) { return gameSpy; }

	// The friendly-fire limits of a multiplayer game.
	float	GetFriendlyFireTolerance( void ) { return friendlyFireTolerance; }
	void	SetMaxFriendlyFire( float tolerance ) { friendlyFireTolerance = tolerance; }
	float	GetCurrentFriendlyFire( void ) { return currentFriendlyFire; }
	void	SetCurrentFriendlyFire( float damage ) { currentFriendlyFire = damage; }
	float	GetFriendlyFireWarningPoints( void ) { return friendlyFireWarningPoints; }
	void	SetFriendlyFireWarningPoints( float points ) { friendlyFireWarningPoints = points; }
	float	GetFriendlyTractorTime( void ) { return friendlyTractorTime; }
	void	SetFriendlyTractorTime( float time ) { friendlyTractorTime = time; }
	float	GetFriendlyTractorWarning( void ) { return friendlyTractorWarning; }
	void	SetFriendlyTractorWarning( float warning ) { friendlyTractorWarning = warning; }
	float	GetMaxFriendlyTractorTime( void ) { return maxFriendlyTractorTime; }
	void	SetMaxFriendlyTractorTime( float time ) { maxFriendlyTractorTime = time; }

	// the clock and the options the application keeps
	float	GetGameTime( void ) { return UtopiaApp::GetApp()->GetGameTime(); }
	void	SetGameTime( float gameTime ) { UtopiaApp::GetApp()->SetGameTime( gameTime ); }
	float	GetRealTime( void ) { return UtopiaApp::GetApp()->GetRealTime(); }
	bool	IsGamePaused( void ) { return UtopiaApp::GetApp()->IsGamePaused(); }
	void	SetTimeRate( float timeRate ) { UtopiaApp::GetApp()->SetTimeRate( timeRate ); }
	void	SetTimeScale( float timeScale ) { UtopiaApp::GetApp()->SetTimeScale( timeScale ); }
	bool	IsDirectStart( void ) { return UtopiaApp::GetApp()->IsDirectStart(); }
	int		GetTestMenuState( void ) { return UtopiaApp::GetApp()->GetTestMenuState(); }
	char *	GetGamePath( void ) { return gamePath; }
	char *	GetDataPath( void ) { return dataPath; }
	char *	GetGameName( void ) { return gameName; }
	char *	GetLoadFilename( void ) { return loadFromFileName; }
	char *	GetSaveFilename( void ) { return saveToFileName; }

private:
	char *			gameName;			// 0x14 a heap copy
	char *			gamePath;			// 0x18 a heap copy
	char *			dataPath;			// 0x1C a heap copy
	char *			unknown20;			// 0x20 a heap block, freed by VirtualSlot03
	char *			unknown24;			// 0x24 a heap block, freed by VirtualSlot03
	NiPointer<NiCamera>	camera;			// 0x28 the camera RenderDefaultCamera draws through
	char *			saveToFileName;		// 0x2C a heap copy: the file SaveToFile names (invented name)
	char *			loadFromFileName;	// 0x30 a heap copy (invented name)
	char *			internalLoadFileName;	// 0x34 a heap copy (invented name)
	char *			unknown38;			// 0x38 the name the two load names fall back on
	int				pauseCount;			// 0x3C Pause( true ) less Pause( false ), never below 0 (invented name)
	float			friendlyFireTolerance;		// 0x40
	float			currentFriendlyFire;		// 0x44
	float			friendlyFireWarningPoints;	// 0x48
	float			friendlyTractorTime;		// 0x4C
	float			friendlyTractorWarning;		// 0x50
	float			maxFriendlyTractorTime;		// 0x54
	int				maxTorpedoLoad[4];		// 0x58 by torpedo type (UtopiaModule_SetMaxTorpedoLoad; the SWIG setter g_kUtopiaModule_set copies two arrays of four)
	int				currentStarbaseTorpedoLoad[4];		// 0x68 by torpedo type
	TGNetwork *		network;			// 0x78 NULL outside multiplayer
	GameSpy *		gameSpy;			// 0x7C owned; made by CreateGameSpy
	NetFile *		netFile;			// 0x80 owned; deleted by TerminateNetwork (invented name)
	int		playerNumber;				// 0x84
	bool	isClient;					// 0x88
	bool	multiplayer;				// 0x89
	bool	isHost;						// 0x8A
	bool	processingPackets;			// 0x8B
	long	unusedClientID;				// 0x8C the ID the next object created in multiplayer takes
	TGString	captainName;			// 0x90 "Player" after construction (UtopiaModule_SetCaptainName)
	bool	unknownA0;					// 0xA0
	bool	ignoreClientIDForObjectCreation;	// 0xA1
	bool	modemConnection;			// 0xA2
	int		programChecksum;			// 0xA4 NetFile::GetFileChecksum of the program file (invented name)
	char *	programPath;				// 0xA8 a heap copy of the program's path (GetModuleFileName; invented name)
	bool	loading;					// 0xAC

	static int	nextEventType;			// GetNextEventType's count, 0 to 0x3FFF (invented name)
	static char *	gameVersion;		// "60" (invented name; the script check and the GameSpy query send it)
	static float	gameUnitConversionFactor;	// kilometres per game unit, 0.175 at first

	// Declared for the SWIG wrappers that call them.
public:
	bool	LoadEpisodeSounds( char *, char * );	// retail 0x446360: the SWIG wrapper UtopiaModule_LoadEpisodeSounds calls it
	// Empties the game's copied sound directory (invented name).
	bool	ClearEpisodeSounds( void );
	// Whether a CD-ROM drive has the file: the application answers the drive's letter, and this, as the SWIG
	// wrapper's (bool) cast declares, whether there is one.
	bool	CDCheck( char *file ) { return UtopiaApp::GetApp()->CDCheck( file ); }
	NiCamera *	GetCamera( void ) { return camera; }
	void	SetMaxTorpedoLoad( int type, int load ) { maxTorpedoLoad[type] = load; }
	int		GetMaxTorpedoLoad( int type ) { return maxTorpedoLoad[type]; }
	void	SetCurrentStarbaseTorpedoLoad( int type, int load ) { currentStarbaseTorpedoLoad[type] = load; }
	int		GetCurrentStarbaseTorpedoLoad( int type ) { return currentStarbaseTorpedoLoad[type]; }
	// The SWIG wrapper UtopiaModule_GetCaptainName calls it out of line, building
	// the copy straight into the string it allocates.
	// FUNCTION: STBC 0x005e9f80 ?GetCaptainName@UtopiaModule@@QAE?AVTGString@@XZ
	TGString	GetCaptainName( void ) { return captainName; }
	void	SetCaptainName( TGString *name ) { captainName = *name; }	// the SWIG wrapper UtopiaModule_SetCaptainName expands it
	int	InitializeNetwork( char *, TGString *, int );	// retail 0x445D90: the SWIG wrapper UtopiaModule_InitializeNetwork calls it
};

extern UtopiaModule	g_kUtopiaModule;

#endif /* !__UTOPIAMODULE_H__ */
