/*
===============================================================================

	UtopiaModule.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <direct.h>

#include "UtopiaModule.h"
#include "Game.h"
#include "GameSpy.h"
#include "MultiplayerWindow.h"
#include "NetFile.h"
#include "NiCamera.h"
#include "NiColor.h"
#include "ModalDialogWindow.h"
#include "PyEmbed.h"
#include "TGEventManager.h"
#include "TGIconManager.h"
#include "TGInputManager.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGMovieManager.h"
#include "TGNetwork.h"
#include "TGWinsockNetwork.h"
#include "TGStream.h"
#include "TGStringEvent.h"
#include "TopWindow.h"
#include "ObjectGroup.h"
#include "MetaNebula.h"
#include "ProximityEvent.h"
#include "VarManagerClass.h"
#include "CameraObjectClass.h"
#include "ObjectClass.h"
#include "ScriptObject.h"
#include "SetClass.h"
#include "BaseObjectClass.h"
#include "ObjectExplodingEvent.h"
#include "ServerListEvent.h"
#include "SubtitleAction.h"
#include "WaypointEvent.h"
#include "Episode.h"
#include "Mission.h"
#include "SetFloatVarEvent.h"
#include "Backdrop.h"
#include "BackdropSphere.h"
#include "StarSphere.h"
#include "SpaceCamera.h"
#include "AsteroidField.h"
#include "MapWindow.h"
#include "STNumericBar.h"
#include "PhysicsObjectClass.h"
#include "CollisionEvent.h"
#include "DamageableObject.h"
#include "ShipClass.h"
#include "Torpedo.h"
#include "WeaponHitEvent.h"
#include "StartFiringEvent.h"
#include "ShipSubsystem.h"
#include "HullClass.h"
#include "PoweredSubsystem.h"
#include "WeaponSystem.h"
#include "SubsystemProperty.h"
#include "WeaponSystemProperty.h"
#include "TorpedoSystemProperty.h"
#include "TorpedoSystem.h"
#include "PhaserSystem.h"
#include "PowerSubsystem.h"
#include "SensorProperty.h"
#include "SensorSubsystem.h"
#include "ImpulseEngineSubsystem.h"
#include "WarpEngineProperty.h"
#include "EngineProperty.h"
#include "WarpEngineSubsystem.h"
#include "PulseWeaponSystem.h"
#include "TractorBeamSystem.h"
#include "RepairSubsystem.h"
#include "CloakingSubsystem.h"
#include "Weapon.h"
#include "EnergyWeapon.h"
#include "TractorBeamProjector.h"
#include "PulseWeapon.h"
#include "ShieldProperty.h"
#include "ShieldClass.h"
#include "Nebula.h"
#include "CloakingSubsystemProperty.h"
#include "WeaponProperty.h"
#include "EnergyWeaponProperty.h"
#include "PlacementObject.h"
#include "Waypoint.h"
#include "LightPlacement.h"
#include "Editor.h"
#include "PlacementEditor.h"
#include "BackgroundEditor.h"
#include "AsteroidFieldPlacement.h"
#include "LightObjectClass.h"
#include "WarpSet.h"
#include "WarpFlash.h"
#include "WarpSequence.h"
#include "ZoomCameraObjectClass.h"
#include "ViewScreenObject.h"
#include "CharacterSpeakingQueue.h"
#include "CharacterClass.h"
#include "SayLineEvent.h"
#include "BridgeSet.h"
#include "ConditionScript.h"
#include "ConditionEventCreator.h"
#include "PositionOrientationProperty.h"
#include "PhaserProperty.h"
#include "HullProperty.h"
#include "TorpedoTubeProperty.h"
#include "PulseWeaponProperty.h"
#include "ImpulseEngineProperty.h"
#include "TractorBeamProperty.h"
#include "ShipProperty.h"
#include "EffectEmitterProperty.h"
#include "SmokeEmitterProperty.h"
#include "SparkEmitterProperty.h"
#include "ExplodeEmitterProperty.h"
#include "RepairSubsystemProperty.h"
#include "PowerProperty.h"
#include "BlinkingLightProperty.h"
#include "ObjectEmitterProperty.h"
#include "TorpedoTube.h"
#include "PhaserBank.h"
#include "ReticleManagerWindow.h"
#include "NetFile.h"
#include "GameSpy.h"
#include "ChangeRenderedSetAction.h"
#include "AIManager.h"
#include "GraphicsMenu.h"
#include "GraphicsModeInfo.h"
#include "ImageManagerClass.h"
#include "SetManager.h"
#include "TextureAnimManager.h"
#include "TGModelManager.h"
#include "TGRedbookClass.h"
#include "TGRootPane.h"
#include "VisibleDamage.h"
#include "CDCheckWindow.h"
#include "MainWindow.h"
#include "NiGeometry.h"
#include "renderer/NiD3DRender.h"
#include "renderer/NiDDImage.h"
#include "TimeSliceProcess.h"
#include "TGTimerManager.h"
#include "TGSystemWrapperClass.h"
#include "TGDirectory.h"
#include "ArtificialIntelligence.h"
#include "CPyDebug.h"
#include "Sun.h"
#include "Planet.h"
#include "CameraMode.h"
#include "MultiplayerGame.h"
#include "TGSoundManager.h"
#include "renderer/NiImage.h"
#include "ChatObjectClass.h"
#include "ObjectGroupWithInfo.h"
#include "AsteroidTile.h"
#include "ProximityCheck.h"
#include "GridClass.h"
#include "BridgeObjectClass.h"
#include "CharacterAction.h"
#include "PulsingLight.h"
#include "WarpEvent.h"
#include "PlainAI.h"
#include "ConditionalAI.h"
#include "PreprocessingAI.h"
#include "BuilderAI.h"
#include "PythonMethodProcess.h"
#include "STStylizedWindow.h"
#include "renderer/NiD3DDirectDraw.h"
#include "TGBufferedFileStream.h"
#include "TGEvent.h"
#include "TGTimer.h"
#include "TGActionManager.h"
#include "TGMusic.h"
#include "TGSoundRegion.h"
#include "TGParagraph.h"
#include "TGModelPropertyManager.h"
#include "TGCondition.h"
#include "FocusManager.h"
#include "LensFlare.h"
#include "InterfaceModule.h"
#include "TGAnimationManagerClass.h"
#include "LODModelManager.h"
#include "TGIntEvent.h"
#include "TGFontManager.h"
#include "TGModule.h"
#include "TGIcon.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00439b60 _$E32
// FUNCTION: STBC 0x00439b70 _$E29
// FUNCTION: STBC 0x00439b80 _$E31
// FUNCTION: STBC 0x00439b90 _$E30
// FUNCTION: STBC 0x00439ba0 _$E38
// FUNCTION: STBC 0x00439bb0 _$E35
// FUNCTION: STBC 0x00439bc0 _$E37
// FUNCTION: STBC 0x00439bd0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00439be0 _$E52
// FUNCTION: STBC 0x00439bf0 _$E51
// FUNCTION: STBC 0x00439c00 _$E208
// FUNCTION: STBC 0x00439c10 _$E207
// FUNCTION: STBC 0x00439c20 _$E63
// FUNCTION: STBC 0x00439c30 _$E62
// FUNCTION: STBC 0x00439c40 _$E67
// FUNCTION: STBC 0x00439c50 _$E66
// FUNCTION: STBC 0x00439c60 _$E71
// FUNCTION: STBC 0x00439c70 _$E70
// FUNCTION: STBC 0x00439c80 _$E75
// FUNCTION: STBC 0x00439c90 _$E74
// FUNCTION: STBC 0x00439ca0 _$E195
// FUNCTION: STBC 0x00439cb0 _$E194
// FUNCTION: STBC 0x00439cc0 _$E137
// FUNCTION: STBC 0x00439cd0 _$E136
// FUNCTION: STBC 0x00439ce0 _$E150
// FUNCTION: STBC 0x00439cf0 _$E149
// FUNCTION: STBC 0x00439d00 _$E200
// FUNCTION: STBC 0x00439d10 _$E199
// FUNCTION: STBC 0x00439d20 _$E213
// FUNCTION: STBC 0x00439d30 _$E212
// FUNCTION: STBC 0x00439d40 _$E204
// FUNCTION: STBC 0x00439d50 _$E203
// FUNCTION: STBC 0x00439d60 _$E217
// FUNCTION: STBC 0x00439d70 _$E216
// FUNCTION: STBC 0x00439d80 _$E224
// FUNCTION: STBC 0x00439d90 _$E223
// FUNCTION: STBC 0x00439da0 _$E591
// FUNCTION: STBC 0x00439db0 _$E590
// FUNCTION: STBC 0x00439dc0 _$E595
// FUNCTION: STBC 0x00439dd0 _$E594
// FUNCTION: STBC 0x00439de0 _$E99
// FUNCTION: STBC 0x00439df0 _$E98
// FUNCTION: STBC 0x00439e00 _$E318
// FUNCTION: STBC 0x00439e10 _$E317
// FUNCTION: STBC 0x00439e20 _$E325
// FUNCTION: STBC 0x00439e30 _$E324
// FUNCTION: STBC 0x00439e40 _$E329
// FUNCTION: STBC 0x00439e50 _$E328
// FUNCTION: STBC 0x00439e60 _$E333
// FUNCTION: STBC 0x00439e70 _$E332
// FUNCTION: STBC 0x00439e80 _$E370
// FUNCTION: STBC 0x00439e90 _$E369
// FUNCTION: STBC 0x00439ea0 _$E376
// FUNCTION: STBC 0x00439eb0 _$E375
// FUNCTION: STBC 0x00439ec0 _$E407
// FUNCTION: STBC 0x00439ed0 _$E406
// FUNCTION: STBC 0x00439ee0 _$E411
// FUNCTION: STBC 0x00439ef0 _$E410
// FUNCTION: STBC 0x00439f00 _$E337
// FUNCTION: STBC 0x00439f10 _$E336
// FUNCTION: STBC 0x00439f20 _$E341
// FUNCTION: STBC 0x00439f30 _$E340
// FUNCTION: STBC 0x00439f40 _$E599
// FUNCTION: STBC 0x00439f50 _$E598
// FUNCTION: STBC 0x00439f60 _$E603
// FUNCTION: STBC 0x00439f70 _$E602
// FUNCTION: STBC 0x00439f80 _$E421
// FUNCTION: STBC 0x00439f90 _$E420
// FUNCTION: STBC 0x00439fa0 _$E607
// FUNCTION: STBC 0x00439fb0 _$E606
// FUNCTION: STBC 0x00439fc0 _$E611
// FUNCTION: STBC 0x00439fd0 _$E610
// FUNCTION: STBC 0x00439fe0 _$E615
// FUNCTION: STBC 0x00439ff0 _$E614
// FUNCTION: STBC 0x0043a000 _$E104
// FUNCTION: STBC 0x0043a010 _$E103
// FUNCTION: STBC 0x0043a020 _$E619
// FUNCTION: STBC 0x0043a030 _$E618
// FUNCTION: STBC 0x0043a040 _$E109
// FUNCTION: STBC 0x0043a050 _$E108
// FUNCTION: STBC 0x0043a060 _$E623
// FUNCTION: STBC 0x0043a070 _$E622
// FUNCTION: STBC 0x0043a080 _$E627
// FUNCTION: STBC 0x0043a090 _$E626
// FUNCTION: STBC 0x0043a0a0 _$E631
// FUNCTION: STBC 0x0043a0b0 _$E630
// FUNCTION: STBC 0x0043a0c0 _$E635
// FUNCTION: STBC 0x0043a0d0 _$E634
// FUNCTION: STBC 0x0043a0e0 _$E114
// FUNCTION: STBC 0x0043a0f0 _$E113
// FUNCTION: STBC 0x0043a100 _$E639
// FUNCTION: STBC 0x0043a110 _$E638
// FUNCTION: STBC 0x0043a120 _$E643
// FUNCTION: STBC 0x0043a130 _$E642
// FUNCTION: STBC 0x0043a140 _$E647
// FUNCTION: STBC 0x0043a150 _$E646
// FUNCTION: STBC 0x0043a160 _$E120
// FUNCTION: STBC 0x0043a170 _$E119
// FUNCTION: STBC 0x0043a180 _$E125
// FUNCTION: STBC 0x0043a190 _$E124
// FUNCTION: STBC 0x0043a1a0 _$E155
// FUNCTION: STBC 0x0043a1b0 _$E154
// FUNCTION: STBC 0x0043a1c0 _$E651
// FUNCTION: STBC 0x0043a1d0 _$E650
// FUNCTION: STBC 0x0043a1e0 _$E160
// FUNCTION: STBC 0x0043a1f0 _$E159
// FUNCTION: STBC 0x0043a200 _$E655
// FUNCTION: STBC 0x0043a210 _$E654
// FUNCTION: STBC 0x0043a220 _$E659
// FUNCTION: STBC 0x0043a230 _$E658
// FUNCTION: STBC 0x0043a240 _$E165
// FUNCTION: STBC 0x0043a250 _$E164
// FUNCTION: STBC 0x0043a260 _$E663
// FUNCTION: STBC 0x0043a270 _$E662
// FUNCTION: STBC 0x0043a280 _$E130
// FUNCTION: STBC 0x0043a290 _$E129
// FUNCTION: STBC 0x0043a2a0 _$E667
// FUNCTION: STBC 0x0043a2b0 _$E666
// FUNCTION: STBC 0x0043a2c0 _$E671
// FUNCTION: STBC 0x0043a2d0 _$E670
// FUNCTION: STBC 0x0043a2e0 _$E170
// FUNCTION: STBC 0x0043a2f0 _$E169
// FUNCTION: STBC 0x0043a300 _$E175
// FUNCTION: STBC 0x0043a310 _$E174
// FUNCTION: STBC 0x0043a320 _$E675
// FUNCTION: STBC 0x0043a330 _$E674
// FUNCTION: STBC 0x0043a340 _$E679
// FUNCTION: STBC 0x0043a350 _$E678
// FUNCTION: STBC 0x0043a360 _$E683
// FUNCTION: STBC 0x0043a370 _$E682
// FUNCTION: STBC 0x0043a380 _$E687
// FUNCTION: STBC 0x0043a390 _$E686
// FUNCTION: STBC 0x0043a3a0 _$E691
// FUNCTION: STBC 0x0043a3b0 _$E690
// FUNCTION: STBC 0x0043a3c0 _$E695
// FUNCTION: STBC 0x0043a3d0 _$E694
// FUNCTION: STBC 0x0043a3e0 _$E699
// FUNCTION: STBC 0x0043a3f0 _$E698
// FUNCTION: STBC 0x0043a400 _$E703
// FUNCTION: STBC 0x0043a410 _$E702
// FUNCTION: STBC 0x0043a420 _$E180
// FUNCTION: STBC 0x0043a430 _$E179
// FUNCTION: STBC 0x0043a440 _$E707
// FUNCTION: STBC 0x0043a450 _$E706
// FUNCTION: STBC 0x0043a460 _$E185
// FUNCTION: STBC 0x0043a470 _$E184
// FUNCTION: STBC 0x0043a480 _$E190
// FUNCTION: STBC 0x0043a490 _$E189
// FUNCTION: STBC 0x0043a9d0 _$E711
// FUNCTION: STBC 0x0043a9e0 _$E710
// FUNCTION: STBC 0x0043a9f0 _$E716
// FUNCTION: STBC 0x0043aa00 _$E715
// FUNCTION: STBC 0x0043aa10 _$E720
// FUNCTION: STBC 0x0043aa20 _$E719
// FUNCTION: STBC 0x0043aa30 _$E724
// FUNCTION: STBC 0x0043aa40 _$E723
// FUNCTION: STBC 0x0043aa50 _$E728
// FUNCTION: STBC 0x0043aa60 _$E727

// The first event type GetNextEventType hands out, and the number of
// types it cycles through.
#define FIRST_MODULE_EVENT_TYPE		0x80110D	// invented name
#define NUM_MODULE_EVENT_TYPES		0x4000		// invented name

// The client IDs of one player.
#define CLIENT_ID_BLOCK_SHIFT		18			// invented name

// The game's key under HKEY_LOCAL_MACHINE (invented name).
#define GAME_REGISTRY_KEY	"SOFTWARE\\Activision\\Star Trek: Bridge Commander\\V1.0"
#define REGISTRY_PATH_SIZE	1024	// the path buffers' size (invented name)

/*
================
GetRegistryInstallPath

The "Install Path" value of the game's key, without a trailing backslash,
as a new string. Without the key, the current directory, which is then
stored as the value; without the value, the current directory.
================
*/
// FUNCTION: STBC 0x0043a4a0
char *GetRegistryInstallPath( void ) {
	HKEY key;
	HKEY newKey;
	DWORD disposition;
	DWORD size;
	char path[REGISTRY_PATH_SIZE];

	if ( RegOpenKeyEx( HKEY_LOCAL_MACHINE, GAME_REGISTRY_KEY, 0, KEY_READ, &key ) != ERROR_SUCCESS ) {
		_getcwd( path, REGISTRY_PATH_SIZE );
		char keyClass = '\0';
		if ( RegCreateKeyEx( HKEY_LOCAL_MACHINE, GAME_REGISTRY_KEY, 0, &keyClass, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &newKey, &disposition ) == ERROR_SUCCESS ) {
			// BUG: RegSetValue writes the default value of a subkey named "Install Path",
			// not the value of that name the query above reads
			RegSetValue( newKey, "Install Path", REG_SZ, path, REGISTRY_PATH_SIZE );
			RegCloseKey( newKey );
		}
	} else {
		size = REGISTRY_PATH_SIZE;
		if ( RegQueryValueEx( key, "Install Path", NULL, NULL, (BYTE *)path, &size ) != ERROR_SUCCESS ) {
			_getcwd( path, REGISTRY_PATH_SIZE );
		} else if ( path[strlen( path ) - 1] == '\\' ) {
			path[strlen( path ) - 1] = '\0';
		}
		RegCloseKey( key );
	}

	char *copy = new char[strlen( path ) + 1];
	strcpy( copy, path );
	return copy;
}

/*
================
GetRegistryDataPath

The "Data Path" value of the game's key, without a trailing backslash, as a
new string; without it, "D:\Build" on the drive FindCDDrive finds.
================
*/
// FUNCTION: STBC 0x0043a5f0
char *GetRegistryDataPath( void ) {
	HKEY key;
	DWORD size;
	char path[REGISTRY_PATH_SIZE];

	if ( RegOpenKeyEx( HKEY_LOCAL_MACHINE, GAME_REGISTRY_KEY, 0, KEY_READ, &key ) != ERROR_SUCCESS ) {
		strcpy( path, "D:\\Build" );
		char drive = FindCDDrive();
		if ( drive != '\0' ) {
			path[0] = drive;
		}
	} else {
		size = REGISTRY_PATH_SIZE;
		if ( RegQueryValueEx( key, "Data Path", NULL, NULL, (BYTE *)path, &size ) != ERROR_SUCCESS ) {
			strcpy( path, "D:\\Build" );
			char drive = FindCDDrive();
			if ( drive != '\0' ) {
				path[0] = drive;
			}
		} else if ( path[strlen( path ) - 1] == '\\' ) {
			path[strlen( path ) - 1] = '\0';
		}
		RegCloseKey( key );
	}

	char *copy = new char[strlen( path ) + 1];
	strcpy( copy, path );
	return copy;
}

/*
================
GetRegistryExecutablePath

The "Executable" value of the game's key, as a new string; without it,
stbc.exe in the current directory.
================
*/
// FUNCTION: STBC 0x0043a730
char *GetRegistryExecutablePath( void ) {
	HKEY key;
	DWORD size;
	char path[REGISTRY_PATH_SIZE];

	if ( RegOpenKeyEx( HKEY_LOCAL_MACHINE, GAME_REGISTRY_KEY, 0, KEY_READ, &key ) != ERROR_SUCCESS ) {
		_getcwd( path, REGISTRY_PATH_SIZE );
		if ( path[strlen( path ) - 1] == '\\' ) {
			path[strlen( path ) - 1] = '\0';
		}
		strcat( path, "\\stbc.exe" );
	} else {
		size = REGISTRY_PATH_SIZE;
		if ( RegQueryValueEx( key, "Executable", NULL, NULL, (BYTE *)path, &size ) != ERROR_SUCCESS ) {
			_getcwd( path, REGISTRY_PATH_SIZE );
			if ( path[strlen( path ) - 1] == '\\' ) {
				path[strlen( path ) - 1] = '\0';
			}
			strcat( path, "\\stbc.exe" );
		} else if ( path[strlen( path ) - 1] == '\\' ) {
			path[strlen( path ) - 1] = '\0';
		}
		RegCloseKey( key );
	}

	char *copy = new char[strlen( path ) + 1];
	strcpy( copy, path );
	return copy;
}

/*
================
FindCDDrive

The letter of the first CD-ROM drive with the game's alarm sound on it,
or 0 (invented name).
================
*/
// FUNCTION: STBC 0x0043a8d0
char FindCDDrive( void ) {
	char fileName[80];
	char drives[MAX_PATH];

	int length = GetLogicalDriveStrings( 0, NULL );
	GetLogicalDriveStrings( length, drives );
	char *drive = drives;
	for ( int i = 0; i < length; i += 4 ) {
		if ( GetDriveType( drive ) == DRIVE_CDROM ) {
			strcpy( fileName, drive );
			// BUG: the drive string already ends in ":\", so the name is "D:\:\Build\...", which no drive has
			strcat( fileName, ":\\Build\\sfx\\alarm1.wav" );
			SetErrorMode( SEM_FAILCRITICALERRORS );
			FILE *file = fopen( fileName, "r" );
			SetErrorMode( 0 );
			if ( file != NULL ) {
				fclose( file );
				return *drive;
			}
		}
		drive += 4;
	}
	return '\0';
}

// DATA: STBC 0x0097fb38
int UtopiaModule::nextEventType;

// DATA: STBC 0x008d9af0
float UtopiaModule::gameUnitConversionFactor = 0.175f;

// DATA: STBC 0x008d9af4
char *UtopiaModule::gameVersion = "60";

// g_kUtopiaModule's construction and its destruction at exit.
// FUNCTION: STBC 0x0043aa70 _$E733
// FUNCTION: STBC 0x0043aa80 _$E730
// FUNCTION: STBC 0x0043aa90 _$E732
// FUNCTION: STBC 0x0043aaa0 _$E731
// DATA: STBC 0x0097fa00
UtopiaModule g_kUtopiaModule;

/*
================
UtopiaModule::UtopiaModule

Clears the members, names the captain "Player", keeps the program's path
and checksum, and tells the Python state saving which modules it skips.
================
*/
// FUNCTION: STBC 0x0043ab30
UtopiaModule::UtopiaModule( void ) : TGModule( 0x20000000 ) {
	saveToFileName = NULL;
	loadFromFileName = NULL;
	internalLoadFileName = NULL;
	unknown38 = NULL;
	pauseCount = 0;
	programPath = NULL;
	gameName = NULL;
	gamePath = NULL;
	dataPath = NULL;
	unknown20 = NULL;
	unknown24 = NULL;
	network = NULL;
	gameSpy = NULL;
	netFile = NULL;
	playerNumber = 0;
	isClient = false;
	multiplayer = false;
	isHost = false;
	processingPackets = false;
	unusedClientID = 0;
	captainName.AssignC( "Player" );
	unknownA0 = false;
	ignoreClientIDForObjectCreation = false;
	loading = false;
	modemConnection = true;

	char path[1024];
	GetModuleFileName( NULL, path, sizeof( path ) );
	programPath = new char[strlen( path ) + 1];
	strcpy( programPath, path );
	programChecksum = NetFile::GetFileChecksum( path );

	// The modules whose state a saved game leaves out, and the prefixes of
	// the module names it leaves out.
	char *unsavedModules[] = {
		"sys", "ConsoleIO", "App", "Appc", "new", "strop", "cStringIO", "copy_reg",
		"__builtin__", "__main__", "signal", "cPickle", "imp", "socket", "_socket",
		"GlobalPropertyTemplates", "Bridge.DBridgeProperties", "Bridge.EBridgeProperties",
		"AILogging", NULL
	};
	char *unsavedPrefixes[] = {
		"Hardpoints.", "ships.", "Icons.", "Multiplayer.", NULL
	};
	PyEmbed::SetUnsavedModules( unsavedModules, unsavedPrefixes );
}

/*
================
UtopiaModule::VirtualSlot00

The module's start: the class creators and handler names, the default
camera on the renderer, the friendly-fire limits and the multiplayer state
at their defaults, the save's state functions, the Python methods written
by hand, the App module and its fix-ups, the splash screen's removal, the
game and data paths from the registry and the card-specific setup.
================
*/
// FUNCTION: STBC 0x0043ad70
void UtopiaModule::VirtualSlot00( void ) {
	pauseCount = 0;
	RegisterClasses();
	RegisterHandlerNames();

	camera = new NiCamera;
	camera->SetRenderer( UtopiaApp::GetRenderer() );
	g_kIconManager.SetDisplayInfo( camera, 1.0f );
	g_kSoundManager.SetUnitsPerMeter( 1.0f );

	friendlyFireTolerance = 5000.0f;
	currentFriendlyFire = 0.0f;
	friendlyFireWarningPoints = 1000.0f;
	friendlyTractorTime = 0.0f;
	friendlyTractorWarning = 15.0f;
	maxFriendlyTractorTime = 60.0f;
	for ( int i = 0; i < 4; i++ ) {
		// BUG: the index is missing: only the first torpedo type's loads are cleared
		maxTorpedoLoad[0] = 0;
		currentStarbaseTorpedoLoad[0] = 0;
	}
	saveToFileName = NULL;
	loadFromFileName = NULL;
	internalLoadFileName = NULL;
	network = NULL;
	gameSpy = NULL;
	netFile = NULL;
	playerNumber = 0;
	isClient = true;
	multiplayer = false;
	isHost = true;
	processingPackets = false;
	unusedClientID = 0x3FFFFFFF;

	AddStateFunctions();
	PyEmbed::AddClassMethod( TGString_GetCString, "TGString", "GetCString" );
	PyEmbed::AddClassMethod( ObjectGroup_GetNameTuple, "ObjectGroup", "GetNameTuple" );
	PyEmbed::AddClassMethod( ObjectGroup_GetActiveObjectTuple, "ObjectGroup", "GetActiveObjectTuple" );
	PyEmbed::AddClassMethod( ObjectGroup_GetActiveObjectTupleInSet, "ObjectGroup", "GetActiveObjectTupleInSet" );
	PyEmbed::AddClassMethod( ObjectGroupWithInfo_AddNameAndInfo, "ObjectGroupWithInfo", "AddNameAndInfo" );
	PyEmbed::AddClassMethod( ObjectGroupWithInfo_SetInfoForName, "ObjectGroupWithInfo", "SetInfoForName" );
	PyEmbed::AddClassMethod( ObjectGroupWithInfo_GetInfo, "ObjectGroupWithInfo", "GetInfo" );
	PyEmbed::AddClassMethod( SetClass_GetTargetableObjects, "SetClass", "GetTargetableObjects" );
	PyEmbed::AddClassMethod( SetClass_GetClassObjectList, "SetClass", "GetClassObjectList" );
	PyEmbed::AddClassMethod( SetClass_GetNavPoints, "SetClass", "GetNavPoints" );
	PyEmbed::AddClassMethod( SetManager_GetAllSets, "SetManager", "GetAllSets" );
	PyEmbed::AddClassMethod( ObjectClass_GetRelativePositionInfo, "ObjectClass", "GetRelativePositionInfo" );
	PyEmbed::AddClassMethod( ArtificialIntelligence_GetAllAIsInTree, "ArtificialIntelligence", "GetAllAIsInTree" );
	PyEmbed::AddClassMethod( ArtificialIntelligence_GetFocusAIs, "ArtificialIntelligence", "GetFocusAIs" );
	PyEmbed::AddClassMethod( ArtificialIntelligence_RegisterExternalFunction, "ArtificialIntelligence", "RegisterExternalFunction" );
	PyEmbed::AddClassMethod( ArtificialIntelligence_UnregisterExternalFunction, "ArtificialIntelligence", "UnregisterExternalFunction" );
	PyEmbed::AddClassMethod( ArtificialIntelligence_CallExternalFunction, "ArtificialIntelligence", "CallExternalFunction" );
	PyEmbed::AddClassMethod( PlainAI_GetScriptInstance, "PlainAI", "GetScriptInstance" );
	PyEmbed::AddClassMethod( PlainAI_SetScriptInstance, "PlainAI", "SetScriptInstance" );
	PyEmbed::AddClassMethod( ConditionalAI_SetEvaluationFunction, "ConditionalAI", "SetEvaluationFunction" );
	PyEmbed::AddClassMethod( PreprocessingAI_SetPreprocessingMethod, "PreprocessingAI", "SetPreprocessingMethod" );
	PyEmbed::AddClassMethod( PreprocessingAI_GetPreprocessingInstance, "PreprocessingAI", "GetPreprocessingInstance" );
	PyEmbed::AddClassMethod( ConditionScript_GetConditionScript, "ConditionScript", "GetConditionScript" );
	PyEmbed::AddClassMethod( ShipClass_GetSubsystems, "ShipClass", "GetSubsystems" );
	PyEmbed::AddClassMethod( Mission_GetFriendlyGroup, "Mission", "GetFriendlyGroup" );
	PyEmbed::AddClassMethod( Mission_GetEnemyGroup, "Mission", "GetEnemyGroup" );
	PyEmbed::AddClassMethod( Mission_GetNeutralGroup, "Mission", "GetNeutralGroup" );
	PyEmbed::AddClassMethod( Mission_GetTractorGroup, "Mission", "GetTractorGroup" );
	PyEmbed::AddClassMethod( PythonMethodProcess_SetInstance, "PythonMethodProcess", "SetInstance" );
	PyEmbed::AddClassMethod( WeaponSystem_GetTargetList, "WeaponSystem", "GetTargetList" );
	PyEmbed::AddClassMethod( STStylizedWindow_GetStyleInstance, "STStylizedWindow", "GetStyleInstance" );
	PyEmbed::AddClassMethod( MetaNebula_GetNebulaSpheres, "MetaNebula", "GetNebulaSpheres" );
	PyEmbed::AddClassMethod( BuilderAI_AddDependencyObject, "BuilderAI", "AddDependencyObject" );

	PyObject *app = PyEmbed::GetModule( "App", false );
	PyErr_Print();
	if ( app == NULL ) {
		abort();
	}
	PyEmbed::CallFunction( "FixApp", "FixApp", "", NULL, "()" );
	PyErr_Print();
	PyEmbed::GetModule( "Autoexec", false );
	PyErr_Print();
	PyEmbed::CallFunction( "loadsplash", "RemoveSplashScreen", NULL, NULL, NULL, 0 );
	PyErr_Print();
	g_kVarManager.SetStringVariable( "Multiplayer", "Mission", "Multiplayer.Episode.Mission1.Mission1" );

	char *path = GetRegistryInstallPath();
	SetGamePath( path );
	delete path;
	path = GetRegistryDataPath();
	SetDataPath( path );
	delete path;

	Ni3DDeviceDesc *device = ( (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer() )->Unknown7C2EE0();
	if ( device != NULL && device->name != NULL ) {
		PyEmbed::CallFunction( "MainMenu.CardSpecificSetup", "ResChanged", NULL, NULL, "(s)", device->name );
	}
	PyErr_Print();
}

/*
================
UtopiaModule::VirtualSlot01

Starts the interface: the handlers, the cursor, the graphics mode and its
LCARS icons, the display and the configuration, the top window and the
reticles; then the opening movies, or straight to the renderer on a
direct start or a restart.
================
*/
// FUNCTION: STBC 0x0043b2f0
void UtopiaModule::VirtualSlot01( void ) {
	RegisterHandlers();
	g_kRootWindow.KillChildren();
	PyEmbed::CallFunction( "SystemIcons", "SetTrekCursor", NULL, NULL, NULL );
	GraphicsModeInfo::UpdateCurrentMode();
	PyEmbed::CallFunction( GraphicsModeInfo::GetCurrentMode()->GetLcarsModule(), "Switch", NULL, NULL, NULL );
	PyEmbed::CallFunction( "Display", "Initialize", NULL, NULL, "()" );
	PyEmbed::CallFunction( "MainMenu.mainmenu", "LoadConfigPart1", NULL, NULL, NULL );
	g_kRootWindow.AddChild( new TopWindow );
	g_kRootWindow.AddChild( new ReticleManagerWindow );
	PyRun_SimpleString( "from SimpleAPI import *" );
	PyEmbed::CallFunction( "MainMenu.mainmenu", "LoadConfigPart2", NULL, NULL, NULL );
	if ( !unknownA0 && !IsDirectStart() ) {
		PyEmbed::CallFunction( "MainMenu.mainmenu", "PlayOpeningMovies", NULL, NULL, NULL, 0 );
	} else {
		VirtualSlot07( (int)(NiRender *)UtopiaApp::GetRenderer() );
	}
	unknownA0 = true;
	g_kInputManager.SetCenterMouse( true );
	VisibleDamage::Init();
}

/*
================
UtopiaModule::VirtualSlot02

The frame: the clocks, the timers, the events, the AIs and the time-slice
processes, the saves and loads asked for, the windows; then, outside
movies, a lock of the back buffer that waits for the card to finish the
last frame, and the draw of the rendered set or of the default camera.
================
*/
// FUNCTION: STBC 0x0043b4f0
void UtopiaModule::VirtualSlot02( void ) {
	g_kSystemWrapper.UpdateTime();
	float gameTime = GetGameTime();
	g_kTimerManager.Update( gameTime );
	g_kRealtimeTimerManager.Update( GetRealTime() );
	g_kEventManager.Unknown6DA2C0();
	aiManager.Update();
	TimeSliceProcess::UpdateProcesses();
	ProcessSave();
	ProcessLoadFromFile();
	ProcessInternalLoad();
	UpdateWindows();
	if ( gameSpy != NULL ) {
		gameSpy->VirtualSlot21( GetRealTime() );
	}

	if ( !g_kMovieManager.IsAnyPlaying() ) {
		NiD3DRender *renderer = (NiD3DRender *)(NiRender *)UtopiaApp::GetRenderer();
		if ( renderer != NULL ) {
			renderer->Unknown7C1320();
			NiPointer<NiImage> image = (NiImage *)renderer->VirtualSlot22( 1 );
			NiPointer<NiDDBufferImage> buffer = NiDynamicCast( NiDDBufferImage, image );
			IDirectDrawSurface7 *surface;
			buffer->Unknown7C70C0( &surface );
			DDSURFACEDESC2 desc;
			desc.dwSize = sizeof( desc );
			while ( true ) {
				HRESULT result = surface->Lock( NULL, &desc, 0, NULL );
				if ( result == DDERR_SURFACELOST ) {
					break;
				}
				if ( result != DDERR_WASSTILLDRAWING && result != DDERR_SURFACEBUSY && result != DDERR_SURFACEISOBSCURED ) {
					surface->Unlock( NULL );
					break;
				}
			}
		}
	}

	if ( g_kSetManager.GetRenderedSet() != NULL && !g_kMovieManager.IsAnyPlaying() ) {
		NiGeometry::ResetDrawCounts();
		g_kSetManager.Unknown418660();
		unsigned int shapes, triangles, vertices;
		NiGeometry::GetDrawCounts( &shapes, &triangles, &vertices );
	} else {
		g_kIconManager.SetCamera( camera );
		RenderDefaultCamera();
	}
	g_kRootWindow.UpdateFPS();
	g_kSoundManager.Update( true );
}

/*
================
UtopiaModule::UpdateWindows

Runs the top window's cutscene and fade, updates the sets, then the main
windows that show (the bridge outside multiplayer, the CD check always),
the reticles and the game.
================
*/
// FUNCTION: STBC 0x0043b790
// DRAFT: retail copies the game time into its local through fld/fstp before the frame-step compare; this source copies it with integer moves after it, and the register choices that follow differ
void UtopiaModule::UpdateWindows( void ) {
	float gameTime = GetGameTime();
	float frameStep = UtopiaApp::GetApp()->GetGameFrameStep();
	TopWindow *top = TopWindow::GetTopWindow();
	MainWindow *bridge = top->FindMainWindow( MWT_BRIDGE );
	MainWindow *tactical = top->FindMainWindow( MWT_TACTICAL );
	MainWindow *tacticalMap = top->FindMainWindow( MWT_TACTICAL_MAP );
	MainWindow *editor = top->FindMainWindow( MWT_EDITOR );
	MainWindow *cinematic = top->FindMainWindow( MWT_CINEMATIC );
	MainWindow *multiplayerWindow = top->FindMainWindow( MWT_MULTIPLAYER );
	MainWindow *cdCheck = top->FindMainWindow( MWT_CD_CHECK );

	top = TopWindow::GetTopWindow();
	if ( top->IsCutsceneMode() ) {
		top->UpdateCutscene( frameStep );
	}
	if ( TopWindow::IsFading( top ) ) {
		top->UpdateFade();
	}

	NiTList<MainWindow *> windows( 2 );
	if ( bridge != NULL && !IsMultiplayer() ) {
		windows.AddTail( bridge );
	}
	if ( tactical != NULL && tactical->IsVisible() ) {
		windows.AddTail( tactical );
	}
	if ( tacticalMap != NULL && tacticalMap->IsVisible() ) {
		windows.AddTail( tacticalMap );
	}
	if ( cinematic != NULL && cinematic->IsVisible() ) {
		windows.AddTail( cinematic );
	}
	if ( editor != NULL && editor->IsVisible() ) {
		windows.AddTail( editor );
	}
	if ( multiplayerWindow != NULL && multiplayerWindow->IsVisible() ) {
		windows.AddTail( multiplayerWindow );
	}
	if ( cdCheck != NULL ) {
		windows.AddTail( cdCheck );
	}

	g_kSetManager.Update( gameTime );
	NiTListIterator pos = windows.GetHeadPos();
	while ( pos ) {
		windows.GetNext( pos )->Update( gameTime );
	}
	ReticleManagerWindow *reticles = ReticleManagerWindow::GetReticleManagerWindow();
	if ( reticles != NULL && reticles->IsVisible() ) {
		reticles->VirtualSlot82( gameTime );
	}
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		game->OnIdle();
	}
	g_kVarManager.SetFloatVariable( "global", "fLockMouse", 0.0f );
}

/*
================
UtopiaModule::Pause

Counted: the game runs again once every pause has been undone. A
multiplayer game is never paused.
================
*/
// FUNCTION: STBC 0x0043bbd0
void UtopiaModule::Pause( bool pause ) {
	if ( IsMultiplayer() == true ) {
		return;
	}
	if ( pause == true ) {
		pauseCount++;
		UtopiaApp *app = UtopiaApp::GetApp();
		if ( !app->IsGamePaused() ) {
			app->Pause( true );
		}
		g_kInputManager.PauseMouseCursorMoveTo();
	} else {
		pauseCount--;
		if ( pauseCount <= 0 ) {
			pauseCount = 0;
			UtopiaApp::GetApp()->Pause( false );
		}
		g_kInputManager.ResumeMouseCursorMoveTo();
	}
}

/*
================
UtopiaModule::VirtualSlot03

The first half of the shutdown: frees the names, unloads the graphics
menu's strings, the damage, the models, the images and the AIs, ends the
game and the sets, drops the glow and ends the network.
================
*/
// FUNCTION: STBC 0x0043bc50
void UtopiaModule::VirtualSlot03( void ) {
	if ( unknown38 != NULL ) {
		delete unknown38;
	}
	unknown38 = NULL;
	if ( gameName != NULL ) {
		delete gameName;
	}
	gameName = NULL;
	if ( gamePath != NULL ) {
		delete gamePath;
	}
	gamePath = NULL;
	if ( dataPath != NULL ) {
		delete dataPath;
	}
	dataPath = NULL;
	if ( unknown20 != NULL ) {
		delete unknown20;
	}
	unknown20 = NULL;
	if ( unknown24 != NULL ) {
		delete unknown24;
	}
	unknown24 = NULL;

	if ( graphicsMenuInfo != NULL && graphicsMenuInfo->database != NULL ) {
		g_kLocalizationManager.Unload( graphicsMenuInfo->database );
		graphicsMenuInfo->database = NULL;
	}
	VisibleDamage::Shutdown();
	g_kModelManager.FreeAllModels();
	g_kImageManager.PurgeImages();
	aiManager.RemoveAllAIs();
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		game->unknown62 = false;
		game->Terminate( NULL );
	}
	g_kSetManager.Terminate();
	SetClass::ResetGlow( NULL );
	TerminateNetwork();
}

/*
================
UtopiaModule::VirtualSlot05

The last of the shutdown: the graphics mode, the CD audio, the camera, the
script's Autoexec.Terminate, the texture animations and the glow.
================
*/
// FUNCTION: STBC 0x0043bd40
void UtopiaModule::VirtualSlot05( void ) {
	GraphicsModeInfo::DeleteCurrentMode();
	g_kRedbook.CloseRedbookCD();
	camera = NULL;
	PyObject *autoexec = PyEmbed::GetModule( "Autoexec", false );
	if ( autoexec != NULL && PyObject_HasAttrString( autoexec, "Terminate" ) ) {
		PyEmbed::CallFunction( "Autoexec", "Terminate", "", NULL, "()" );
		PyErr_Print();
	}
	TextureAnimManager::DeleteAllContainers();
	SetClass::ShutdownGlow();
	if ( programPath != NULL ) {
		delete programPath;
		programPath = NULL;
	}
}

/*
================
UtopiaModule::RegisterClasses

Each game class's creator, for the saved games; the handler tables take
their parents' as parents and join the array of the class tables. The
subsystems without handlers of their own, the properties and the events
register only their creators.
================
*/
// FUNCTION: STBC 0x0043bdf0
void UtopiaModule::RegisterClasses( void ) {
	TGObject::RegisterCreator( VARMANAGERCLASS_TYPE, CreateVarManagerClass );

	VarManagerClass::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( VarManagerClass::GetClassTable() );

	TGObject::RegisterCreator( BASEOBJECTCLASS_TYPE, CreateBaseObjectClass );
	TGObject::RegisterCreator( CHATOBJECTCLASS_TYPE, CreateChatObjectClass );
	TGObject::RegisterCreator( OBJECTCLASS_TYPE, CreateObjectClass );
	TGObject::RegisterCreator( PHYSICSOBJECTCLASS_TYPE, CreatePhysicsObjectClass );
	TGObject::RegisterCreator( DAMAGEABLEOBJECT_TYPE, CreateDamageableObject );
	TGObject::RegisterCreator( SHIPCLASS_TYPE, CreateShipClass );
	TGObject::RegisterCreator( TORPEDO_TYPE, CreateTorpedo );
	TGObject::RegisterCreator( BACKDROP_TYPE, CreateBackdrop );
	TGObject::RegisterCreator( BACKDROPSPHERE_TYPE, CreateBackdropSphere );
	TGObject::RegisterCreator( STARSPHERE_TYPE, CreateStarSphere );
	TGObject::RegisterCreator( NEBULA_TYPE, CreateNebula );
	TGObject::RegisterCreator( METANEBULA_TYPE, CreateMetaNebula );
	TGObject::RegisterCreator( PLANET_TYPE, CreatePlanet );
	TGObject::RegisterCreator( SUN_TYPE, CreateSun );
	TGObject::RegisterCreator( CHARACTERCLASS_TYPE, CreateCharacterClass );
	TGObject::RegisterCreator( CHARACTERSPEAKINGQUEUE_TYPE, CreateCharacterSpeakingQueue );
	TGObject::RegisterCreator( CHARACTERACTION_TYPE, CreateCharacterAction );
	TGObject::RegisterCreator( BRIDGEOBJECTCLASS_TYPE, CreateBridgeObjectClass );
	TGObject::RegisterCreator( VIEWSCREENOBJECT_TYPE, CreateViewScreenObject );
	TGObject::RegisterCreator( SETCLASS_TYPE, CreateSetClass );
	TGObject::RegisterCreator( BRIDGESET_TYPE, CreateBridgeSet );
	TGObject::RegisterCreator( WARPSET_TYPE, CreateWarpSet );
	TGObject::RegisterCreator( OBJECTGROUP_TYPE, CreateObjectGroup );
	TGObject::RegisterCreator( OBJECTGROUPWITHINFO_TYPE, CreateObjectGroupWithInfo );
	TGObject::RegisterCreator( WARPFLASH_TYPE, CreateWarpFlash );
	TGObject::RegisterCreator( WARPSEQUENCE_TYPE, CreateWarpSequence );
	TGObject::RegisterCreator( ASTEROIDFIELD_TYPE, CreateAsteroidField );
	TGObject::RegisterCreator( ASTEROIDTILE_TYPE, CreateAsteroidTile );
	TGObject::RegisterCreator( SUBTITLEACTION_TYPE, CreateSubtitleAction );
	TGObject::RegisterCreator( STNUMERICBAR_TYPE, CreateSTNumericBar );
	TGObject::RegisterCreator( CHANGERENDEREDSETACTION_TYPE, CreateChangeRenderedSetAction );

	BaseObjectClass::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( BaseObjectClass::GetClassTable() );
	ChatObjectClass::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ChatObjectClass::GetClassTable() );
	ObjectClass::GetClassTable()->SetParent( BaseObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ObjectClass::GetClassTable() );
	PhysicsObjectClass::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PhysicsObjectClass::GetClassTable() );
	DamageableObject::GetClassTable()->SetParent( PhysicsObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( DamageableObject::GetClassTable() );
	ShipClass::GetClassTable()->SetParent( DamageableObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ShipClass::GetClassTable() );
	Torpedo::GetClassTable()->SetParent( PhysicsObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Torpedo::GetClassTable() );
	Backdrop::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Backdrop::GetClassTable() );
	Planet::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Planet::GetClassTable() );
	Sun::GetClassTable()->SetParent( Planet::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Sun::GetClassTable() );
	CharacterSpeakingQueue::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( CharacterSpeakingQueue::GetClassTable() );
	CharacterClass::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( CharacterClass::GetClassTable() );
	BridgeObjectClass::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( BridgeObjectClass::GetClassTable() );
	ViewScreenObject::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ViewScreenObject::GetClassTable() );
	SetClass::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( SetClass::GetClassTable() );
	WarpSet::GetClassTable()->SetParent( SetClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( WarpSet::GetClassTable() );
	ObjectGroup::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ObjectGroup::GetClassTable() );
	ObjectGroupWithInfo::GetClassTable()->SetParent( ObjectGroup::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ObjectGroupWithInfo::GetClassTable() );
	AsteroidField::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( AsteroidField::GetClassTable() );
	AsteroidTile::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( AsteroidTile::GetClassTable() );

	TGObject::RegisterCreator( SHIPSUBSYSTEM_TYPE, CreateShipSubsystem );
	TGObject::RegisterCreator( POWEREDSUBSYSTEM_TYPE, CreatePoweredSubsystem );
	TGObject::RegisterCreator( POWERSUBSYSTEM_TYPE, CreatePowerSubsystem );
	TGObject::RegisterCreator( HULLCLASS_TYPE, CreateHullClass );
	TGObject::RegisterCreator( SENSORSUBSYSTEM_TYPE, CreateSensorSubsystem );
	TGObject::RegisterCreator( CLOAKINGSUBSYSTEM_TYPE, CreateCloakingSubsystem );
	TGObject::RegisterCreator( REPAIRSUBSYSTEM_TYPE, CreateRepairSubsystem );
	TGObject::RegisterCreator( SHIELDCLASS_TYPE, CreateShieldClass );
	TGObject::RegisterCreator( IMPULSEENGINESUBSYSTEM_TYPE, CreateImpulseEngineSubsystem );
	TGObject::RegisterCreator( WARPENGINESUBSYSTEM_TYPE, CreateWarpEngineSubsystem );
	TGObject::RegisterCreator( WEAPON_TYPE, CreateWeapon );
	TGObject::RegisterCreator( ENERGYWEAPON_TYPE, CreateEnergyWeapon );
	TGObject::RegisterCreator( WEAPONSYSTEM_TYPE, CreateWeaponSystem );
	TGObject::RegisterCreator( TORPEDOSYSTEM_TYPE, CreateTorpedoSystem );
	TGObject::RegisterCreator( TORPEDOTUBE_TYPE, CreateTorpedoTube );
	TGObject::RegisterCreator( PHASERSYSTEM_TYPE, CreatePhaserSystem );
	TGObject::RegisterCreator( PHASERBANK_TYPE, CreatePhaserBank );
	TGObject::RegisterCreator( PULSEWEAPONSYSTEM_TYPE, CreatePulseWeaponSystem );
	TGObject::RegisterCreator( PULSEWEAPON_TYPE, CreatePulseWeapon );
	TGObject::RegisterCreator( TRACTORBEAMSYSTEM_TYPE, CreateTractorBeamSystem );
	TGObject::RegisterCreator( TRACTORBEAMPROJECTOR_TYPE, CreateTractorBeamProjector );

	HullClass::GetClassTable()->SetParent( ShipSubsystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( HullClass::GetClassTable() );
	SensorSubsystem::GetClassTable()->SetParent( PoweredSubsystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( SensorSubsystem::GetClassTable() );
	ShieldClass::GetClassTable()->SetParent( PoweredSubsystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ShieldClass::GetClassTable() );
	ImpulseEngineSubsystem::GetClassTable()->SetParent( PoweredSubsystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ImpulseEngineSubsystem::GetClassTable() );
	WarpEngineSubsystem::GetClassTable()->SetParent( PoweredSubsystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( WarpEngineSubsystem::GetClassTable() );
	PhaserSystem::GetClassTable()->SetParent( WeaponSystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PhaserSystem::GetClassTable() );
	TorpedoSystem::GetClassTable()->SetParent( PoweredSubsystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TorpedoSystem::GetClassTable() );
	TorpedoTube::GetClassTable()->SetParent( Weapon::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TorpedoTube::GetClassTable() );
	PulseWeaponSystem::GetClassTable()->SetParent( WeaponSystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PulseWeaponSystem::GetClassTable() );
	TractorBeamSystem::GetClassTable()->SetParent( WeaponSystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TractorBeamSystem::GetClassTable() );
	RepairSubsystem::GetClassTable()->SetParent( PoweredSubsystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( RepairSubsystem::GetClassTable() );
	WeaponSystem::GetClassTable()->SetParent( PoweredSubsystem::GetClassTable() );
	TGEventHandlerObject::classTables.Add( WeaponSystem::GetClassTable() );

	TGObject::RegisterCreator( POSITIONORIENTATIONPROPERTY_TYPE, CreatePositionOrientationProperty );
	TGObject::RegisterCreator( SUBSYSTEMPROPERTY_TYPE, CreateSubsystemProperty );
	TGObject::RegisterCreator( WEAPONSYSTEMPROPERTY_TYPE, CreateWeaponSystemProperty );
	TGObject::RegisterCreator( WEAPONPROPERTY_TYPE, CreateWeaponProperty );
	TGObject::RegisterCreator( ENERGYWEAPONPROPERTY_TYPE, CreateEnergyWeaponProperty );
	TGObject::RegisterCreator( PHASERPROPERTY_TYPE, CreatePhaserProperty );
	TGObject::RegisterCreator( SHIELDPROPERTY_TYPE, CreateShieldProperty );
	TGObject::RegisterCreator( HULLPROPERTY_TYPE, CreateHullProperty );
	TGObject::RegisterCreator( SENSORPROPERTY_TYPE, CreateSensorProperty );
	TGObject::RegisterCreator( CLOAKINGSUBSYSTEMPROPERTY_TYPE, CreateCloakingSubsystemProperty );
	TGObject::RegisterCreator( REPAIRSUBSYSTEMPROPERTY_TYPE, CreateRepairSubsystemProperty );
	TGObject::RegisterCreator( PULSEWEAPONPROPERTY_TYPE, CreatePulseWeaponProperty );
	TGObject::RegisterCreator( SHIPPROPERTY_TYPE, CreateShipProperty );
	TGObject::RegisterCreator( IMPULSEENGINEPROPERTY_TYPE, CreateImpulseEngineProperty );
	TGObject::RegisterCreator( WARPENGINEPROPERTY_TYPE, CreateWarpEngineProperty );
	TGObject::RegisterCreator( ENGINEPROPERTY_TYPE, CreateEngineProperty );
	TGObject::RegisterCreator( POWERPROPERTY_TYPE, CreatePowerProperty );
	TGObject::RegisterCreator( TORPEDOTUBEPROPERTY_TYPE, CreateTorpedoTubeProperty );
	TGObject::RegisterCreator( TORPEDOSYSTEMPROPERTY_TYPE, CreateTorpedoSystemProperty );
	TGObject::RegisterCreator( TRACTORBEAMPROPERTY_TYPE, CreateTractorBeamProperty );
	TGObject::RegisterCreator( EFFECTEMITTERPROPERTY_TYPE, CreateEffectEmitterProperty );
	TGObject::RegisterCreator( SMOKEEMITTERPROPERTY_TYPE, CreateSmokeEmitterProperty );
	TGObject::RegisterCreator( SPARKEMITTERPROPERTY_TYPE, CreateSparkEmitterProperty );
	TGObject::RegisterCreator( EXPLODEEMITTERPROPERTY_TYPE, CreateExplodeEmitterProperty );
	TGObject::RegisterCreator( BLINKINGLIGHTPROPERTY_TYPE, CreateBlinkingLightProperty );
	TGObject::RegisterCreator( OBJECTEMITTERPROPERTY_TYPE, CreateObjectEmitterProperty );
	TGObject::RegisterCreator( PROXIMITYCHECK_TYPE, CreateProximityCheck );
	TGObject::RegisterCreator( PROXIMITYEVENT_TYPE, CreateProximityEvent );
	TGObject::RegisterCreator( COLLISIONEVENT_TYPE, CreateCollisionEvent );
	TGObject::RegisterCreator( WEAPONHITEVENT_TYPE, CreateWeaponHitEvent );
	TGObject::RegisterCreator( STARTFIRINGEVENT_TYPE, CreateStartFiringEvent );
	TGObject::RegisterCreator( OBJECTEXPLODINGEVENT_TYPE, CreateObjectExplodingEvent );
	TGObject::RegisterCreator( WARPEVENT_TYPE, CreateWarpEvent );

	ProximityCheck::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ProximityCheck::GetClassTable() );

	TGObject::RegisterCreator( CONDITIONSCRIPT_TYPE, CreateConditionScript );
	TGObject::RegisterCreator( CONDITIONEVENTCREATOR_TYPE, CreateConditionEventCreator );
	TGObject::RegisterCreator( EDITOR_TYPE, CreateEditor );
	TGObject::RegisterCreator( PLACEMENTEDITOR_TYPE, CreatePlacementEditor );
	TGObject::RegisterCreator( BACKGROUNDEDITOR_TYPE, CreateBackgroundEditor );
	TGObject::RegisterCreator( GRIDCLASS_TYPE, CreateGridClass );
	TGObject::RegisterCreator( PLACEMENTOBJECT_TYPE, CreatePlacementObject );
	TGObject::RegisterCreator( ASTEROIDFIELDPLACEMENT_TYPE, CreateAsteroidFieldPlacement );
	TGObject::RegisterCreator( WAYPOINT_TYPE, CreateWaypoint );
	TGObject::RegisterCreator( LIGHTPLACEMENT_TYPE, CreateLightPlacement );

	Editor::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Editor::GetClassTable() );
	PlacementEditor::GetClassTable()->SetParent( Editor::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PlacementEditor::GetClassTable() );
	BackgroundEditor::GetClassTable()->SetParent( Editor::GetClassTable() );
	TGEventHandlerObject::classTables.Add( BackgroundEditor::GetClassTable() );
	GridClass::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( GridClass::GetClassTable() );
	PlacementObject::GetClassTable()->SetParent( ObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PlacementObject::GetClassTable() );
	Waypoint::GetClassTable()->SetParent( PlacementObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Waypoint::GetClassTable() );
	LightPlacement::GetClassTable()->SetParent( PlacementObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( LightPlacement::GetClassTable() );

	TGObject::RegisterCreator( LIGHTOBJECTCLASS_TYPE, CreateLightObjectClass );
	TGObject::RegisterCreator( PULSINGLIGHT_TYPE, CreatePulsingLight );
	TGObject::RegisterCreator( CAMERAOBJECTCLASS_TYPE, CreateCameraObjectClass );
	TGObject::RegisterCreator( ZOOMCAMERAOBJECTCLASS_TYPE, CreateZoomCameraObjectClass );
	TGObject::RegisterCreator( SPACECAMERA_TYPE, CreateSpaceCamera );

	LightObjectClass::GetClassTable()->SetParent( BaseObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( LightObjectClass::GetClassTable() );
	PulsingLight::GetClassTable()->SetParent( LightObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PulsingLight::GetClassTable() );
	CameraObjectClass::GetClassTable()->SetParent( BaseObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( CameraObjectClass::GetClassTable() );
	SpaceCamera::GetClassTable()->SetParent( CameraObjectClass::GetClassTable() );
	TGEventHandlerObject::classTables.Add( SpaceCamera::GetClassTable() );
	ZoomCameraObjectClass::GetClassTable()->SetParent( SpaceCamera::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ZoomCameraObjectClass::GetClassTable() );

	TGObject::RegisterCreator( CAMERAMODE_TYPE, CreateCameraMode );
	TGObject::RegisterCreator( IDEALCONTROLLEDCAMERAMODE_TYPE, CreateIdealControlledCameraMode );
	TGObject::RegisterCreator( CHASECAMERAMODE_TYPE, CreateChaseCameraMode );
	TGObject::RegisterCreator( TARGETCAMERAMODE_TYPE, CreateTargetCameraMode );
	TGObject::RegisterCreator( ZOOMTARGETMODE_TYPE, CreateZoomTargetMode );
	TGObject::RegisterCreator( MAPCAMERAMODE_TYPE, CreateMapCameraMode );
	TGObject::RegisterCreator( DROPANDWATCHMODE_TYPE, CreateDropAndWatchMode );
	TGObject::RegisterCreator( PLACEMENTWATCHMODE_TYPE, CreatePlacementWatchMode );
	TGObject::RegisterCreator( LOCKEDPOSITIONMODE_TYPE, CreateLockedPositionMode );
	TGObject::RegisterCreator( TORPCAMERAMODE_TYPE, CreateTorpCameraMode );
	TGObject::RegisterCreator( PLACEBYDIRECTIONMODE_TYPE, CreatePlaceByDirectionMode );

	CameraMode::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( CameraMode::GetClassTable() );
	IdealControlledCameraMode::GetClassTable()->SetParent( CameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( IdealControlledCameraMode::GetClassTable() );
	ChaseCameraMode::GetClassTable()->SetParent( IdealControlledCameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ChaseCameraMode::GetClassTable() );
	TargetCameraMode::GetClassTable()->SetParent( IdealControlledCameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TargetCameraMode::GetClassTable() );
	ZoomTargetMode::GetClassTable()->SetParent( IdealControlledCameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ZoomTargetMode::GetClassTable() );
	MapCameraMode::GetClassTable()->SetParent( IdealControlledCameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( MapCameraMode::GetClassTable() );
	DropAndWatchMode::GetClassTable()->SetParent( CameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( DropAndWatchMode::GetClassTable() );
	PlacementWatchMode::GetClassTable()->SetParent( CameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PlacementWatchMode::GetClassTable() );
	LockedPositionMode::GetClassTable()->SetParent( CameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( LockedPositionMode::GetClassTable() );
	TorpCameraMode::GetClassTable()->SetParent( IdealControlledCameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( TorpCameraMode::GetClassTable() );
	PlaceByDirectionMode::GetClassTable()->SetParent( CameraMode::GetClassTable() );
	TGEventHandlerObject::classTables.Add( PlaceByDirectionMode::GetClassTable() );

	TGObject::RegisterCreator( SCRIPTOBJECT_TYPE, CreateScriptObject );
	TGObject::RegisterCreator( GAME_TYPE, CreateGame );
	TGObject::RegisterCreator( EPISODE_TYPE, CreateEpisode );
	TGObject::RegisterCreator( MISSION_TYPE, CreateMission );

	ScriptObject::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ScriptObject::GetClassTable() );
	Game::GetClassTable()->SetParent( ScriptObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Game::GetClassTable() );
	Episode::GetClassTable()->SetParent( ScriptObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Episode::GetClassTable() );
	Mission::GetClassTable()->SetParent( ScriptObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( Mission::GetClassTable() );

	TGObject::RegisterCreator( MULTIPLAYERGAME_TYPE, CreateMultiplayerGame );
	TGObject::RegisterCreator( NETFILE_TYPE, CreateNetFile );
	TGObject::RegisterCreator( GAMESPY_TYPE, CreateGameSpyObject );
	TGObject::RegisterCreator( SERVERLISTEVENT_TYPE, CreateServerListEvent );
	TGObject::RegisterCreator( SORTSERVERLISTEVENT_TYPE, CreateSortServerListEvent );

	MultiplayerGame::GetClassTable()->SetParent( Game::GetClassTable() );
	TGEventHandlerObject::classTables.Add( MultiplayerGame::GetClassTable() );

	NetFile::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( NetFile::GetClassTable() );
	GameSpy::GetClassTable()->SetParent( TGEventHandlerObject::GetClassTable() );
	TGEventHandlerObject::classTables.Add( GameSpy::GetClassTable() );

	TGObject::RegisterCreator( WAYPOINTEVENT_TYPE, CreateWaypointEvent );
	TGObject::RegisterCreator( SETFLOATVAREVENT_TYPE, CreateSetFloatVarEvent );
	TGObject::RegisterCreator( SAYLINEEVENT_TYPE, CreateSayLineEvent );
	TGObject::RegisterCreator( RETICLEMANAGERWINDOW_TYPE, CreateReticleManagerWindow );

	ReticleManagerWindow::GetClassTable()->SetParent( TGPane::GetClassTable() );
	TGEventHandlerObject::classTables.Add( ReticleManagerWindow::GetClassTable() );

	TGObject::RegisterCreator( MAPWINDOW_TYPE, CreateMapWindow );

	MapWindow::GetClassTable()->SetParent( MainWindow::GetClassTable() );
	TGEventHandlerObject::classTables.Add( MapWindow::GetClassTable() );
}

/*
================
UtopiaModule::CreateBaseObjectClass
================
*/
// FUNCTION: STBC 0x0043d6c0
TGObject *UtopiaModule::CreateBaseObjectClass( long id ) {
	return new BaseObjectClass( id );
}

/*
================
UtopiaModule::CreateSetClass
================
*/
// FUNCTION: STBC 0x0043d6f0
TGObject *UtopiaModule::CreateSetClass( long id ) {
	return new SetClass( id );
}

/*
================
UtopiaModule::CreateVarManagerClass
================
*/
// FUNCTION: STBC 0x0043d730
TGObject *UtopiaModule::CreateVarManagerClass( long id ) {
	return new VarManagerClass( id );
}

/*
================
UtopiaModule::CreateScriptObject
================
*/
// FUNCTION: STBC 0x0043d850
TGObject *UtopiaModule::CreateScriptObject( long id ) {
	return new ScriptObject( id );
}

/*
================
UtopiaModule::CreateEpisode
================
*/
// FUNCTION: STBC 0x0043d880
TGObject *UtopiaModule::CreateEpisode( long id ) {
	return new Episode( id );
}

/*
================
UtopiaModule::CreateMission
================
*/
// FUNCTION: STBC 0x0043d8d0
TGObject *UtopiaModule::CreateMission( long id ) {
	return new Mission( id );
}

/*
================
UtopiaModule::CreateGame
================
*/
// FUNCTION: STBC 0x0043d940
TGObject *UtopiaModule::CreateGame( long id ) {
	return new Game( id );
}

/*
================
UtopiaModule::CreateSetFloatVarEvent
================
*/
// FUNCTION: STBC 0x0043d970
TGObject *UtopiaModule::CreateSetFloatVarEvent( long id ) {
	return new SetFloatVarEvent( id );
}

/*
================
UtopiaModule::CreateObjectClass
================
*/
// FUNCTION: STBC 0x0043d9a0
TGObject *UtopiaModule::CreateObjectClass( long id ) {
	return new ObjectClass( id );
}

/*
================
UtopiaModule::CreateBackdrop
================
*/
// FUNCTION: STBC 0x0043d9d0
TGObject *UtopiaModule::CreateBackdrop( long id ) {
	return new Backdrop( id );
}

/*
================
UtopiaModule::CreateBackdropSphere
================
*/
// FUNCTION: STBC 0x0043da00
TGObject *UtopiaModule::CreateBackdropSphere( long id ) {
	return new BackdropSphere( id );
}

/*
================
UtopiaModule::CreateStarSphere
================
*/
// FUNCTION: STBC 0x0043da40
TGObject *UtopiaModule::CreateStarSphere( long id ) {
	return new StarSphere( id );
}

/*
================
UtopiaModule::CreateCameraMode
================
*/
// FUNCTION: STBC 0x0043da80
TGObject *UtopiaModule::CreateCameraMode( long id ) {
	return new CameraMode( id );
}

/*
================
UtopiaModule::CreateIdealControlledCameraMode
================
*/
// FUNCTION: STBC 0x0043dbe0
TGObject *UtopiaModule::CreateIdealControlledCameraMode( long id ) {
	return new IdealControlledCameraMode( id );
}

/*
================
UtopiaModule::CreateChaseCameraMode
================
*/
// FUNCTION: STBC 0x0043dd40
TGObject *UtopiaModule::CreateChaseCameraMode( long id ) {
	return new ChaseCameraMode( id );
}

/*
================
UtopiaModule::CreateTargetCameraMode
================
*/
// FUNCTION: STBC 0x0043de20
TGObject *UtopiaModule::CreateTargetCameraMode( long id ) {
	return new TargetCameraMode( id );
}

/*
================
UtopiaModule::CreateZoomTargetMode
================
*/
// FUNCTION: STBC 0x0043df80
TGObject *UtopiaModule::CreateZoomTargetMode( long id ) {
	return new ZoomTargetMode( id );
}

/*
================
UtopiaModule::CreateMapCameraMode
================
*/
// FUNCTION: STBC 0x0043e0e0
TGObject *UtopiaModule::CreateMapCameraMode( long id ) {
	return new MapCameraMode( id );
}

/*
================
UtopiaModule::CreateDropAndWatchMode
================
*/
// FUNCTION: STBC 0x0043e1c0
TGObject *UtopiaModule::CreateDropAndWatchMode( long id ) {
	return new DropAndWatchMode( id );
}

/*
================
UtopiaModule::CreatePlacementWatchMode
================
*/
// FUNCTION: STBC 0x0043e320
TGObject *UtopiaModule::CreatePlacementWatchMode( long id ) {
	return new PlacementWatchMode( id );
}

/*
================
UtopiaModule::CreateLockedPositionMode
================
*/
// FUNCTION: STBC 0x0043e480
TGObject *UtopiaModule::CreateLockedPositionMode( long id ) {
	return new LockedPositionMode( id );
}

/*
================
UtopiaModule::CreateTorpCameraMode
================
*/
// FUNCTION: STBC 0x0043e5e0
TGObject *UtopiaModule::CreateTorpCameraMode( long id ) {
	return new TorpCameraMode( id );
}

/*
================
UtopiaModule::CreatePlaceByDirectionMode
================
*/
// FUNCTION: STBC 0x0043e740
TGObject *UtopiaModule::CreatePlaceByDirectionMode( long id ) {
	return new PlaceByDirectionMode( id );
}

/*
================
UtopiaModule::CreateCameraObjectClass
================
*/
// FUNCTION: STBC 0x0043e8a0
TGObject *UtopiaModule::CreateCameraObjectClass( long id ) {
	return new CameraObjectClass( id );
}

/*
================
UtopiaModule::CreateSpaceCamera
================
*/
// FUNCTION: STBC 0x0043e8e0
TGObject *UtopiaModule::CreateSpaceCamera( long id ) {
	return new SpaceCamera( id );
}

/*
================
UtopiaModule::CreateChatObjectClass
================
*/
// FUNCTION: STBC 0x0043e920
TGObject *UtopiaModule::CreateChatObjectClass( long id ) {
	return new ChatObjectClass( id );
}

/*
================
UtopiaModule::CreateAsteroidField
================
*/
// FUNCTION: STBC 0x0043ea30
TGObject *UtopiaModule::CreateAsteroidField( long id ) {
	return new AsteroidField( id, 3 );
}

/*
================
UtopiaModule::CreateAsteroidTile
================
*/
// FUNCTION: STBC 0x0043ea70
TGObject *UtopiaModule::CreateAsteroidTile( long id ) {
	return new AsteroidTile( id );
}

/*
================
UtopiaModule::CreateMapWindow
================
*/
// FUNCTION: STBC 0x0043eab0
TGObject *UtopiaModule::CreateMapWindow( long id ) {
	return new MapWindow( id );
}

/*
================
UtopiaModule::CreateSTNumericBar
================
*/
// FUNCTION: STBC 0x0043ecb0
TGObject *UtopiaModule::CreateSTNumericBar( long id ) {
	return new STNumericBar( id );
}

/*
================
UtopiaModule::CreatePhysicsObjectClass
================
*/
// FUNCTION: STBC 0x0043ee10
TGObject *UtopiaModule::CreatePhysicsObjectClass( long id ) {
	return new PhysicsObjectClass( id );
}

/*
================
UtopiaModule::CreateCollisionEvent
================
*/
// FUNCTION: STBC 0x0043ee50
TGObject *UtopiaModule::CreateCollisionEvent( long id ) {
	return new CollisionEvent( id );
}

/*
================
UtopiaModule::CreateDamageableObject
================
*/
// FUNCTION: STBC 0x0043ee80
TGObject *UtopiaModule::CreateDamageableObject( long id ) {
	return new DamageableObject( id );
}

/*
================
UtopiaModule::CreateShipClass
================
*/
// FUNCTION: STBC 0x0043eec0
TGObject *UtopiaModule::CreateShipClass( long id ) {
	return new ShipClass( id );
}

/*
================
UtopiaModule::CreateTorpedo
================
*/
// FUNCTION: STBC 0x0043ef00
TGObject *UtopiaModule::CreateTorpedo( long id ) {
	return new Torpedo( id );
}

/*
================
UtopiaModule::CreateObjectGroup
================
*/
// FUNCTION: STBC 0x0043ef40
TGObject *UtopiaModule::CreateObjectGroup( long id ) {
	return new ObjectGroup( id );
}

/*
================
UtopiaModule::CreateObjectGroupWithInfo
================
*/
// FUNCTION: STBC 0x0043f0d0
TGObject *UtopiaModule::CreateObjectGroupWithInfo( long id ) {
	return new ObjectGroupWithInfo( id );
}

/*
================
UtopiaModule::CreateProximityCheck
================
*/
// FUNCTION: STBC 0x0043f2b0
TGObject *UtopiaModule::CreateProximityCheck( long id ) {
	return new ProximityCheck( id );
}

/*
================
UtopiaModule::CreateProximityEvent
================
*/
// FUNCTION: STBC 0x0043f4e0
TGObject *UtopiaModule::CreateProximityEvent( long id ) {
	return new ProximityEvent( id );
}

/*
================
UtopiaModule::CreateWeaponHitEvent
================
*/
// FUNCTION: STBC 0x0043f730
TGObject *UtopiaModule::CreateWeaponHitEvent( long id ) {
	return new WeaponHitEvent( id );
}

/*
================
UtopiaModule::CreateStartFiringEvent
================
*/
// FUNCTION: STBC 0x0043f760
TGObject *UtopiaModule::CreateStartFiringEvent( long id ) {
	return new StartFiringEvent( id );
}

/*
================
UtopiaModule::CreateObjectExplodingEvent
================
*/
// FUNCTION: STBC 0x0043f880
TGObject *UtopiaModule::CreateObjectExplodingEvent( long id ) {
	return new ObjectExplodingEvent( id );
}

/*
================
UtopiaModule::CreateWarpEvent
================
*/
// FUNCTION: STBC 0x0043fa50
TGObject *UtopiaModule::CreateWarpEvent( long id ) {
	return new WarpEvent( id );
}

/*
================
UtopiaModule::CreateShipSubsystem
================
*/
// FUNCTION: STBC 0x0043fe30
TGObject *UtopiaModule::CreateShipSubsystem( long id ) {
	return new ShipSubsystem( id );
}

/*
================
UtopiaModule::CreateHullClass
================
*/
// FUNCTION: STBC 0x0043fe70
TGObject *UtopiaModule::CreateHullClass( long id ) {
	return new HullClass( id );
}

/*
================
UtopiaModule::CreatePoweredSubsystem
================
*/
// FUNCTION: STBC 0x0043feb0
TGObject *UtopiaModule::CreatePoweredSubsystem( long id ) {
	return new PoweredSubsystem( id );
}

/*
================
UtopiaModule::CreateWeaponSystem
================
*/
// FUNCTION: STBC 0x0043fef0
TGObject *UtopiaModule::CreateWeaponSystem( long id ) {
	return new WeaponSystem( id );
}

/*
================
UtopiaModule::CreateSubsystemProperty
================
*/
// FUNCTION: STBC 0x0043ff30
TGObject *UtopiaModule::CreateSubsystemProperty( long id ) {
	return new SubsystemProperty( id );
}

/*
================
UtopiaModule::CreateWeaponSystemProperty
================
*/
// FUNCTION: STBC 0x0043ff60
TGObject *UtopiaModule::CreateWeaponSystemProperty( long id ) {
	return new WeaponSystemProperty( id );
}

/*
================
UtopiaModule::CreateTorpedoSystemProperty
================
*/
// FUNCTION: STBC 0x0043ff90
TGObject *UtopiaModule::CreateTorpedoSystemProperty( long id ) {
	return new TorpedoSystemProperty( id );
}

/*
================
UtopiaModule::CreateTorpedoSystem
================
*/
// FUNCTION: STBC 0x0043ffd0
TGObject *UtopiaModule::CreateTorpedoSystem( long id ) {
	return new TorpedoSystem( id );
}

/*
================
UtopiaModule::CreatePhaserSystem
================
*/
// FUNCTION: STBC 0x00440010
TGObject *UtopiaModule::CreatePhaserSystem( long id ) {
	return new PhaserSystem( id );
}

/*
================
UtopiaModule::CreatePowerSubsystem
================
*/
// FUNCTION: STBC 0x00440050
TGObject *UtopiaModule::CreatePowerSubsystem( long id ) {
	return new PowerSubsystem( id );
}

/*
================
UtopiaModule::CreateSensorProperty
================
*/
// FUNCTION: STBC 0x00440200
TGObject *UtopiaModule::CreateSensorProperty( long id ) {
	return new SensorProperty( id );
}

/*
================
UtopiaModule::CreateSensorSubsystem
================
*/
// FUNCTION: STBC 0x00440230
TGObject *UtopiaModule::CreateSensorSubsystem( long id ) {
	return new SensorSubsystem( id );
}

/*
================
UtopiaModule::CreateImpulseEngineSubsystem
================
*/
// FUNCTION: STBC 0x00440270
TGObject *UtopiaModule::CreateImpulseEngineSubsystem( long id ) {
	return new ImpulseEngineSubsystem( id );
}

/*
================
UtopiaModule::CreateWarpEngineProperty
================
*/
// FUNCTION: STBC 0x004402b0
TGObject *UtopiaModule::CreateWarpEngineProperty( long id ) {
	return new WarpEngineProperty( id );
}

/*
================
UtopiaModule::CreateEngineProperty
================
*/
// FUNCTION: STBC 0x004402e0
TGObject *UtopiaModule::CreateEngineProperty( long id ) {
	return new EngineProperty( id );
}

/*
================
UtopiaModule::CreateWarpEngineSubsystem
================
*/
// FUNCTION: STBC 0x00440310
TGObject *UtopiaModule::CreateWarpEngineSubsystem( long id ) {
	return new WarpEngineSubsystem( id );
}

/*
================
UtopiaModule::CreatePulseWeaponSystem
================
*/
// FUNCTION: STBC 0x00440350
TGObject *UtopiaModule::CreatePulseWeaponSystem( long id ) {
	return new PulseWeaponSystem( id );
}

/*
================
UtopiaModule::CreateTractorBeamSystem
================
*/
// FUNCTION: STBC 0x00440390
TGObject *UtopiaModule::CreateTractorBeamSystem( long id ) {
	return new TractorBeamSystem( id );
}

/*
================
UtopiaModule::CreateRepairSubsystem
================
*/
// FUNCTION: STBC 0x004403d0
TGObject *UtopiaModule::CreateRepairSubsystem( long id ) {
	return new RepairSubsystem( id );
}

/*
================
UtopiaModule::CreateCloakingSubsystem
================
*/
// FUNCTION: STBC 0x00440410
TGObject *UtopiaModule::CreateCloakingSubsystem( long id ) {
	return new CloakingSubsystem( id );
}

/*
================
UtopiaModule::CreateWeapon
================
*/
// FUNCTION: STBC 0x00440450
TGObject *UtopiaModule::CreateWeapon( long id ) {
	return new Weapon( id );
}

/*
================
UtopiaModule::CreateEnergyWeapon
================
*/
// FUNCTION: STBC 0x00440490
TGObject *UtopiaModule::CreateEnergyWeapon( long id ) {
	return new EnergyWeapon( id );
}

/*
================
UtopiaModule::CreateTractorBeamProjector
================
*/
// FUNCTION: STBC 0x004404d0
TGObject *UtopiaModule::CreateTractorBeamProjector( long id ) {
	return new TractorBeamProjector( id );
}

/*
================
UtopiaModule::CreatePulseWeapon
================
*/
// FUNCTION: STBC 0x00440510
TGObject *UtopiaModule::CreatePulseWeapon( long id ) {
	return new PulseWeapon( id );
}

/*
================
UtopiaModule::CreateShieldProperty
================
*/
// FUNCTION: STBC 0x00440550
TGObject *UtopiaModule::CreateShieldProperty( long id ) {
	return new ShieldProperty( id );
}

/*
================
UtopiaModule::CreateShieldClass
================
*/
// FUNCTION: STBC 0x00440590
TGObject *UtopiaModule::CreateShieldClass( long id ) {
	return new ShieldClass( id );
}

/*
================
UtopiaModule::CreateNebula
================
*/
// FUNCTION: STBC 0x004405d0
TGObject *UtopiaModule::CreateNebula( long id ) {
	return new Nebula( id );
}

/*
================
UtopiaModule::CreateMetaNebula
================
*/
// FUNCTION: STBC 0x004407d0
TGObject *UtopiaModule::CreateMetaNebula( long id ) {
	return new MetaNebula( id );
}

/*
================
UtopiaModule::CreatePlanet
================
*/
// FUNCTION: STBC 0x00440960
TGObject *UtopiaModule::CreatePlanet( long id ) {
	return new Planet( id );
}

/*
================
UtopiaModule::CreateSun
================
*/
// FUNCTION: STBC 0x00440ac0
TGObject *UtopiaModule::CreateSun( long id ) {
	return new Sun( id );
}

/*
================
UtopiaModule::CreateCloakingSubsystemProperty
================
*/
// FUNCTION: STBC 0x00440c40
TGObject *UtopiaModule::CreateCloakingSubsystemProperty( long id ) {
	return new CloakingSubsystemProperty( id );
}

/*
================
UtopiaModule::CreateWeaponProperty
================
*/
// FUNCTION: STBC 0x00440c70
TGObject *UtopiaModule::CreateWeaponProperty( long id ) {
	return new WeaponProperty( id );
}

/*
================
UtopiaModule::CreateEnergyWeaponProperty
================
*/
// FUNCTION: STBC 0x00440ca0
TGObject *UtopiaModule::CreateEnergyWeaponProperty( long id ) {
	return new EnergyWeaponProperty( id );
}

/*
================
UtopiaModule::CreateGridClass
================
*/
// FUNCTION: STBC 0x00440ce0
TGObject *UtopiaModule::CreateGridClass( long id ) {
	return new GridClass( id );
}

/*
================
UtopiaModule::CreatePlacementObject
================
*/
// FUNCTION: STBC 0x00440d10
TGObject *UtopiaModule::CreatePlacementObject( long id ) {
	return new PlacementObject( id );
}

/*
================
UtopiaModule::CreateWaypoint
================
*/
// FUNCTION: STBC 0x00440e50
TGObject *UtopiaModule::CreateWaypoint( long id ) {
	return new Waypoint( id );
}

/*
================
UtopiaModule::CreateLightPlacement
================
*/
// FUNCTION: STBC 0x00440f90
TGObject *UtopiaModule::CreateLightPlacement( long id ) {
	return new LightPlacement( id );
}

/*
================
UtopiaModule::CreateEditor
================
*/
// FUNCTION: STBC 0x004410c0
TGObject *UtopiaModule::CreateEditor( long id ) {
	return new Editor( id );
}

/*
================
UtopiaModule::CreatePlacementEditor
================
*/
// FUNCTION: STBC 0x00441200
TGObject *UtopiaModule::CreatePlacementEditor( long id ) {
	return new PlacementEditor( id );
}

/*
================
UtopiaModule::CreateBackgroundEditor
================
*/
// FUNCTION: STBC 0x00441350
TGObject *UtopiaModule::CreateBackgroundEditor( long id ) {
	return new BackgroundEditor( id );
}

/*
================
UtopiaModule::CreateAsteroidFieldPlacement
================
*/
// FUNCTION: STBC 0x004414b0
TGObject *UtopiaModule::CreateAsteroidFieldPlacement( long id ) {
	return new AsteroidFieldPlacement( id );
}

/*
================
UtopiaModule::CreateLightObjectClass
================
*/
// FUNCTION: STBC 0x004415f0
TGObject *UtopiaModule::CreateLightObjectClass( long id ) {
	return new LightObjectClass( id );
}

/*
================
UtopiaModule::CreatePulsingLight
================
*/
// FUNCTION: STBC 0x00441620
TGObject *UtopiaModule::CreatePulsingLight( long id ) {
	return new PulsingLight( id );
}

/*
================
UtopiaModule::CreateWarpSet
================
*/
// FUNCTION: STBC 0x00441780
TGObject *UtopiaModule::CreateWarpSet( long id ) {
	return new WarpSet( id );
}

/*
================
UtopiaModule::CreateWarpFlash
================
*/
// FUNCTION: STBC 0x004417c0
TGObject *UtopiaModule::CreateWarpFlash( long id ) {
	return new WarpFlash( id );
}

/*
================
UtopiaModule::CreateWarpSequence
================
*/
// FUNCTION: STBC 0x004418e0
TGObject *UtopiaModule::CreateWarpSequence( long id ) {
	return new WarpSequence( id );
}

/*
================
UtopiaModule::CreateZoomCameraObjectClass
================
*/
// FUNCTION: STBC 0x004419f0
TGObject *UtopiaModule::CreateZoomCameraObjectClass( long id ) {
	return new ZoomCameraObjectClass( id );
}

/*
================
UtopiaModule::CreateViewScreenObject
================
*/
// FUNCTION: STBC 0x00441a30
TGObject *UtopiaModule::CreateViewScreenObject( long id ) {
	return new ViewScreenObject( id );
}

/*
================
UtopiaModule::CreateCharacterSpeakingQueue
================
*/
// FUNCTION: STBC 0x00441a60
TGObject *UtopiaModule::CreateCharacterSpeakingQueue( long id ) {
	return new CharacterSpeakingQueue( id );
}

/*
================
UtopiaModule::CreateCharacterClass
================
*/
// FUNCTION: STBC 0x00441cb0
TGObject *UtopiaModule::CreateCharacterClass( long id ) {
	return new CharacterClass( id );
}

/*
================
UtopiaModule::CreateCharacterAction
================
*/
// FUNCTION: STBC 0x00441cf0
TGObject *UtopiaModule::CreateCharacterAction( long id ) {
	return new CharacterAction( id );
}

/*
================
UtopiaModule::CreateSayLineEvent
================
*/
// FUNCTION: STBC 0x00441de0
TGObject *UtopiaModule::CreateSayLineEvent( long id ) {
	return new SayLineEvent( id );
}

/*
================
UtopiaModule::CreateBridgeSet
================
*/
// FUNCTION: STBC 0x00441e10
TGObject *UtopiaModule::CreateBridgeSet( long id ) {
	return new BridgeSet( id );
}

/*
================
UtopiaModule::CreateBridgeObjectClass
================
*/
// FUNCTION: STBC 0x00441e50
TGObject *UtopiaModule::CreateBridgeObjectClass( long id ) {
	return new BridgeObjectClass( id );
}

/*
================
UtopiaModule::CreateWaypointEvent
================
*/
// FUNCTION: STBC 0x00441e90
TGObject *UtopiaModule::CreateWaypointEvent( long id ) {
	return new WaypointEvent( id );
}

/*
================
UtopiaModule::CreateConditionScript
================
*/
// FUNCTION: STBC 0x00441fb0
TGObject *UtopiaModule::CreateConditionScript( long id ) {
	return new ConditionScript( id );
}

/*
================
UtopiaModule::CreateConditionEventCreator
================
*/
// FUNCTION: STBC 0x004420a0
TGObject *UtopiaModule::CreateConditionEventCreator( long id ) {
	return new ConditionEventCreator( id );
}

/*
================
UtopiaModule::CreatePositionOrientationProperty
================
*/
// FUNCTION: STBC 0x00442270
TGObject *UtopiaModule::CreatePositionOrientationProperty( long id ) {
	return new PositionOrientationProperty( id );
}

/*
================
UtopiaModule::CreatePhaserProperty
================
*/
// FUNCTION: STBC 0x004422a0
TGObject *UtopiaModule::CreatePhaserProperty( long id ) {
	return new PhaserProperty( id );
}

/*
================
UtopiaModule::CreateHullProperty
================
*/
// FUNCTION: STBC 0x004422e0
TGObject *UtopiaModule::CreateHullProperty( long id ) {
	return new HullProperty( id );
}

/*
================
UtopiaModule::CreateTorpedoTubeProperty
================
*/
// FUNCTION: STBC 0x00442310
TGObject *UtopiaModule::CreateTorpedoTubeProperty( long id ) {
	return new TorpedoTubeProperty( id );
}

/*
================
UtopiaModule::CreatePulseWeaponProperty
================
*/
// FUNCTION: STBC 0x00442350
TGObject *UtopiaModule::CreatePulseWeaponProperty( long id ) {
	return new PulseWeaponProperty( id );
}

/*
================
UtopiaModule::CreateImpulseEngineProperty
================
*/
// FUNCTION: STBC 0x00442390
TGObject *UtopiaModule::CreateImpulseEngineProperty( long id ) {
	return new ImpulseEngineProperty( id );
}

/*
================
UtopiaModule::CreateTractorBeamProperty
================
*/
// FUNCTION: STBC 0x004423c0
TGObject *UtopiaModule::CreateTractorBeamProperty( long id ) {
	return new TractorBeamProperty( id );
}

/*
================
UtopiaModule::CreateShipProperty
================
*/
// FUNCTION: STBC 0x00442400
TGObject *UtopiaModule::CreateShipProperty( long id ) {
	return new ShipProperty( id );
}

/*
================
UtopiaModule::CreateEffectEmitterProperty
================
*/
// FUNCTION: STBC 0x00442430
TGObject *UtopiaModule::CreateEffectEmitterProperty( long id ) {
	return new EffectEmitterProperty( id );
}

/*
================
UtopiaModule::CreateSmokeEmitterProperty
================
*/
// FUNCTION: STBC 0x00442460
TGObject *UtopiaModule::CreateSmokeEmitterProperty( long id ) {
	return new SmokeEmitterProperty( id );
}

/*
================
UtopiaModule::CreateSparkEmitterProperty
================
*/
// FUNCTION: STBC 0x00442490
TGObject *UtopiaModule::CreateSparkEmitterProperty( long id ) {
	return new SparkEmitterProperty( id );
}

/*
================
UtopiaModule::CreateExplodeEmitterProperty
================
*/
// FUNCTION: STBC 0x004424c0
TGObject *UtopiaModule::CreateExplodeEmitterProperty( long id ) {
	return new ExplodeEmitterProperty( id );
}

/*
================
UtopiaModule::CreateRepairSubsystemProperty
================
*/
// FUNCTION: STBC 0x004424f0
TGObject *UtopiaModule::CreateRepairSubsystemProperty( long id ) {
	return new RepairSubsystemProperty( id );
}

/*
================
UtopiaModule::CreatePowerProperty
================
*/
// FUNCTION: STBC 0x00442520
TGObject *UtopiaModule::CreatePowerProperty( long id ) {
	return new PowerProperty( id );
}

/*
================
UtopiaModule::CreateBlinkingLightProperty
================
*/
// FUNCTION: STBC 0x00442550
TGObject *UtopiaModule::CreateBlinkingLightProperty( long id ) {
	return new BlinkingLightProperty( id );
}

/*
================
UtopiaModule::CreateObjectEmitterProperty
================
*/
// FUNCTION: STBC 0x00442580
TGObject *UtopiaModule::CreateObjectEmitterProperty( long id ) {
	return new ObjectEmitterProperty( id );
}

/*
================
UtopiaModule::CreateTorpedoTube
================
*/
// FUNCTION: STBC 0x004425b0
TGObject *UtopiaModule::CreateTorpedoTube( long id ) {
	return new TorpedoTube( id );
}

/*
================
UtopiaModule::CreatePhaserBank
================
*/
// FUNCTION: STBC 0x004425f0
TGObject *UtopiaModule::CreatePhaserBank( long id ) {
	return new PhaserBank( id );
}

/*
================
UtopiaModule::CreateReticleManagerWindow
================
*/
// FUNCTION: STBC 0x00442630
TGObject *UtopiaModule::CreateReticleManagerWindow( long id ) {
	return new ReticleManagerWindow( id );
}

/*
================
UtopiaModule::CreateMultiplayerGame
================
*/
// FUNCTION: STBC 0x004427f0
TGObject *UtopiaModule::CreateMultiplayerGame( long id ) {
	return new MultiplayerGame( id );
}

/*
================
UtopiaModule::CreateNetFile
================
*/
// FUNCTION: STBC 0x00442970
TGObject *UtopiaModule::CreateNetFile( long id ) {
	return new NetFile( id );
}

/*
================
UtopiaModule::CreateGameSpyObject
================
*/
// FUNCTION: STBC 0x004429a0
TGObject *UtopiaModule::CreateGameSpyObject( long id ) {
	return new GameSpy( id );
}

/*
================
UtopiaModule::CreateServerListEvent
================
*/
// FUNCTION: STBC 0x004429e0
TGObject *UtopiaModule::CreateServerListEvent( long id ) {
	return new ServerListEvent( id );
}

/*
================
UtopiaModule::CreateSortServerListEvent
================
*/
// FUNCTION: STBC 0x00442b70
TGObject *UtopiaModule::CreateSortServerListEvent( long id ) {
	return new SortServerListEvent( id );
}

/*
================
UtopiaModule::CreateSubtitleAction
================
*/
// FUNCTION: STBC 0x00442dc0
TGObject *UtopiaModule::CreateSubtitleAction( long id ) {
	return new SubtitleAction( id );
}

/*
================
UtopiaModule::CreateChangeRenderedSetAction
================
*/
// FUNCTION: STBC 0x00442f60
TGObject *UtopiaModule::CreateChangeRenderedSetAction( long id ) {
	return new ChangeRenderedSetAction( id );
}

/*
================
UtopiaModule::RegisterHandlerNames

The handler methods of the game's classes by name, for the saved games.
================
*/
// FUNCTION: STBC 0x00442fb0
void UtopiaModule::RegisterHandlerNames( void ) {
	CharacterSpeakingQueue::RegisterHandlerNames();
	CharacterClass::RegisterHandlerNames();
	ViewScreenObject::RegisterHandlerNames();
	CameraObjectClass::RegisterHandlerNames();
	BridgeObjectClass::RegisterHandlerNames();
	ObjectClass::RegisterHandlerNames();
	Planet::RegisterHandlerNames();
	DamageableObject::RegisterHandlerNames();
	PhysicsObjectClass::RegisterHandlerNames();
	ShipClass::RegisterHandlerNames();
	Torpedo::RegisterHandlerNames();
	ObjectGroup::RegisterHandlerNames();
	AsteroidField::RegisterHandlerNames();
	AsteroidTile::RegisterHandlerNames();
	Game::RegisterHandlerNames();
	MultiplayerGame::RegisterHandlerNames();
	NetFile::RegisterHandlerNames();
	GameSpy::RegisterHandlerNames();
	VarManagerClass::RegisterHandlerNames();
	ChatObjectClass::RegisterHandlerNames();
	ReticleManagerWindow::RegisterHandlerNames();
	MultiplayerWindow::RegisterHandlerNames();
}

/*
================
UtopiaModule::RegisterHandlers

The handler methods of the game's classes in their class tables.
================
*/
// FUNCTION: STBC 0x00443020
void UtopiaModule::RegisterHandlers( void ) {
	CharacterSpeakingQueue::RegisterHandlers();
	CharacterClass::RegisterHandlers();
	ViewScreenObject::RegisterHandlers();
	CameraObjectClass::RegisterHandlers();
	BridgeObjectClass::RegisterHandlers();
	ObjectClass::RegisterHandlers();
	Planet::RegisterHandlers();
	ObjectGroup::RegisterHandlers();
	DamageableObject::RegisterHandlers();
	PhysicsObjectClass::RegisterHandlers();
	ShipClass::RegisterHandlers();
	Torpedo::RegisterHandlers();
	AsteroidField::RegisterHandlers();
	AsteroidTile::RegisterHandlers();
	Game::RegisterHandlers();
	MultiplayerGame::RegisterHandlers();
	NetFile::RegisterHandlers();
	VarManagerClass::RegisterHandlers();
	ChatObjectClass::RegisterHandlers();
}

/*
================
UtopiaModule::AddStateFunctions
================
*/
// FUNCTION: STBC 0x00443080
// DRAFT: inline budget: retail calls AddTail out of line for both TGPoint3 entries; this source still expands the get-state one
void UtopiaModule::AddStateFunctions( void ) {
	PyEmbed::AddGetStateFunction( ArtificialIntelligence_SaveInstance, "ArtificialIntelligence" );
	PyEmbed::AddSetStateFunction( ArtificialIntelligence_LoadInstance, "ArtificialIntelligence" );
	PyEmbed::AddGetStateFunction( CPyDebug_SaveInstance, "CPyDebug" );
	PyEmbed::AddSetStateFunction( CPyDebug_LoadInstance, "CPyDebug" );
	PyEmbed::AddGetStateFunction( TGPoint3_SaveInstance, "TGPoint3" );
	PyEmbed::AddSetStateFunction( TGPoint3_LoadInstance, "TGPoint3" );
}

/*
================
UtopiaModule::RenderDefaultCamera

Draws the default camera's view and the icons over it, with the next
movie frame when a movie plays; nothing while a forced movie is shown.
================
*/
// FUNCTION: STBC 0x004433e0
void UtopiaModule::RenderDefaultCamera( void ) {
	if ( !g_kMovieManager.IsForcedMoviePlaying() ) {
		camera->GetRenderer()->SetBackgroundColor( NiColor::BLACK );
		camera->Clear( 1 );
		if ( g_kMovieManager.IsAnyPlaying() ) {
			g_kMovieManager.PlayNextFrame();
		}
		camera->Click();
		g_kIconManager.Draw();
		camera->Unknown7E81B0();
	}
}

/*
================
UtopiaModule::GetNextEventType
================
*/
// FUNCTION: STBC 0x00443440
int UtopiaModule::GetNextEventType( void ) {
	int type = FIRST_MODULE_EVENT_TYPE + nextEventType;
	nextEventType = ( nextEventType + 1 ) % NUM_MODULE_EVENT_TYPES;
	return type;
}

/*
================
UtopiaModule::SaveToFile

Names the file the game is saved to.
================
*/
// FUNCTION: STBC 0x00443470
void UtopiaModule::SaveToFile( char *name ) {
	if ( name != NULL ) {
		if ( saveToFileName != NULL ) {
			delete saveToFileName;
		}
		saveToFileName = new char[strlen( name ) + 1];
		strcpy( saveToFileName, name );
	}
}

/*
================
UtopiaModule::LoadFromFile

During a single-player game with a ship, the player is asked first; the
dialog's OK hands the name to the script's SetLoadFromFileName.
================
*/
// FUNCTION: STBC 0x004434d0
void UtopiaModule::LoadFromFile( char *name ) {
	ModalDialogWindow *dialog = (ModalDialogWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MODAL_DIALOG );
	Game *game = Game::GetCurrentGame();
	if ( name != NULL && dialog != NULL && game != NULL && !dialog->IsVisible() && Game::GetCurrentPlayer() != NULL && !g_kUtopiaModule.IsMultiplayer() ) {
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );

		TGStringEvent *loadEvent = new TGStringEvent;
		loadEvent->SetEventType( ET_SET_LOAD_FILE );
		loadEvent->SetDestination( game );
		loadEvent->SetString( name );
		game->AddPythonFuncHandlerForInstance( ET_SET_LOAD_FILE, "MissionLib.SetLoadFromFileName" );

		TGEvent *cancelEvent = new TGEvent;
		cancelEvent->SetEventType( ET_CANCEL_LOAD );
		cancelEvent->SetDestination( game );
		game->AddPythonFuncHandlerForInstance( ET_CANCEL_LOAD, "MissionLib.CancelLoad" );

		dialog->Run( NULL, database->GetString( "Are you sure you want to quit" ), database->GetString( "OK" ), loadEvent, database->GetString( "Cancel" ), cancelEvent );
		g_kLocalizationManager.Unload( database );
	} else {
		SetLoadFromFileName( name );
	}
}

/*
================
UtopiaModule::SetLoadFromFileName

An empty name takes the default name, when there is one.
================
*/
// FUNCTION: STBC 0x00443640
void UtopiaModule::SetLoadFromFileName( char *name ) {
	if ( name != NULL && strcmp( name, "" ) != 0 ) {
		if ( loadFromFileName != NULL ) {
			delete loadFromFileName;
		}
		loadFromFileName = new char[strlen( name ) + 1];
		strcpy( loadFromFileName, name );
	} else if ( unknown38 != NULL ) {
		if ( loadFromFileName != NULL ) {
			delete loadFromFileName;
		}
		loadFromFileName = new char[strlen( unknown38 ) + 1];
		strcpy( loadFromFileName, unknown38 );
	}
}

/*
================
UtopiaModule::SetInternalLoadFileName

An empty name takes the default name, when there is one.
================
*/
// FUNCTION: STBC 0x00443710
void UtopiaModule::SetInternalLoadFileName( char *name ) {
	if ( name != NULL && strcmp( name, "" ) != 0 ) {
		if ( internalLoadFileName != NULL ) {
			delete internalLoadFileName;
		}
		internalLoadFileName = new char[strlen( name ) + 1];
		strcpy( internalLoadFileName, name );
	} else if ( unknown38 != NULL ) {
		if ( internalLoadFileName != NULL ) {
			delete internalLoadFileName;
		}
		internalLoadFileName = new char[strlen( unknown38 ) + 1];
		strcpy( internalLoadFileName, unknown38 );
	}
}

/*
================
UtopiaModule::SaveMissionState

The named module's state goes to Data\MissionTemp\<name>.msv.
================
*/
// FUNCTION: STBC 0x004437e0
bool UtopiaModule::SaveMissionState( char *name ) {
	if ( name == NULL ) {
		return false;
	}
	char path[1024];
	strcpy( path, "Data\\MissionTemp\\" );
	strcat( path, name );
	strcat( path, ".msv" );
	TGStream stream;
	stream.VirtualSlot00( path, "wb" );
	if ( stream.VirtualSlot58() != 0 ) {
		return false;
	}
	if ( PyEmbed::StartSave( &stream ) ) {
		bool success = true;
		if ( !PyEmbed::SaveModuleState( name, &stream, true ) ) {
			success = false;
		}
		if ( !PyEmbed::FinishSave( &stream ) ) {
			success = false;
		}
		return success;
	}
	return false;
}

/*
================
UtopiaModule::LoadMissionState

Reads back what SaveMissionState wrote for the module.
================
*/
// FUNCTION: STBC 0x00443940
bool UtopiaModule::LoadMissionState( char *name ) {
	char path[1024];
	strcpy( path, "Data\\MissionTemp\\" );
	strcat( path, name );
	strcat( path, ".msv" );
	TGStream stream;
	stream.VirtualSlot00( path, "rb" );
	if ( stream.VirtualSlot58() != 0 ) {
		return false;
	}
	bool success = true;
	if ( !PyEmbed::StartLoad( &stream ) ) {
		return false;
	}
	if ( !PyEmbed::LoadModuleState( name, &stream ) ) {
		success = false;
	}
	if ( !PyEmbed::FinishLoad( &stream ) ) {
		return false;
	}
	return success;
}

/*
================
UtopiaModule::ProcessSave

Writes the game SaveToFile named: the header, the friendly fire and torpedo
settings, the paths, then every manager's and class's saved state in turn,
the times, the rendered set and its camera, the interface, the captain's
name, the viewscreen and the saved mission states. A save that fails
removes the file; one that succeeds becomes the name the loads fall back
on. Every object of the map is logged first, and the top window hears
ET_GAME_SAVED either way.
================
*/
// FUNCTION: STBC 0x00443ac0
// DRAFT: same calls; retail keeps this in a frame slot (ours in a register, 4 bytes less frame)
void UtopiaModule::ProcessSave( void ) {
	if ( saveToFileName == NULL ) {
		return;
	}

	// BUG: redundant: the log file's name is built, and never used
	char logName[256];
	strcpy( logName, "SaveFile-" );
	strcat( logName, saveToFileName );
	strcat( logName, ".txt" );
	char *separator;
	while ( ( separator = strchr( logName, '/' ) ) != NULL ) {
		*separator = '_';
	}
	while ( ( separator = strchr( logName, '\\' ) ) != NULL ) {
		*separator = '_';
	}

	char text[256];
	NiTMap<long, TGObject *> *objects = TGObject::objectTable;
	NiTMapIterator pos = objects->GetFirstPos();
	while ( pos != NULL ) {
		long id;
		TGObject *object;
		objects->GetNext( pos, id, object );

		// An event from a damageable object without a model is not saved.
		if ( object->IsTypeOf( TGEVENT_TYPE ) ) {
			TGObject *source = ( (TGEvent *)object )->GetSource();
			if ( source != NULL && source->IsTypeOf( DAMAGEABLEOBJECT_TYPE ) && ( (DamageableObject *)source )->GetLODModelName() == NULL ) {
				( (TGEvent *)object )->SetNotSaved( true );
			}
		}

		sprintf( text, "Object in map: ID %d Type %d Name %s\n", object->GetObjID(), object->GetObjType(), object->GetClassName() );
		TGObject::Log( text );
		object->GetClassName();	// BUG: redundant: the class name is fetched again and not used

		if ( object->IsTypeOf( CHARACTERCLASS_TYPE ) ) {
			CharacterClass *character = (CharacterClass *)object;
			SetClass *set = character->GetContainingSet();
			if ( set != NULL ) {
				if ( character->GetCharacterName() != NULL ) {
					sprintf( text, "Character's name is %s, containing set name is %s (ID %d)\n", character->GetCharacterName(), set->GetName(), set->GetObjID() );
				} else {
					sprintf( text, "Character's name is UNDEFINED, containing set name is %s (ID %d)\n", set->GetName(), set->GetObjID() );
				}
			} else {
				sprintf( text, "Character's name is %s, NO containing set??\n", character->GetCharacterName() );
			}
			TGObject::Log( text );
		}

		if ( object->IsTypeOf( BASEOBJECTCLASS_TYPE ) && !object->IsTypeOf( PLACEMENTOBJECT_TYPE ) && !object->IsTypeOf( PROXIMITYCHECK_TYPE ) ) {
			BaseObjectClass *baseObject = (BaseObjectClass *)object;
			SetClass *set = baseObject->GetContainingSet();
			if ( set != NULL ) {
				sprintf( text, "Object's name is %s, containing set name is %s (ID %d)\n", baseObject->GetName(), set->GetName(), set->GetObjID() );
			} else {
				sprintf( text, "Object's name is %s, NO CONTAINING SET, DANGER, DANGER\n", baseObject->GetName() );
			}
			TGObject::Log( text );
		}
	}

	PyEmbed::CallFunction( "MainMenu.mainmenu", "PreSave", NULL, NULL, NULL, 0 );

	TGBufferedFileStream file( 0x100000 );
	bool saved = false;
	file.VirtualSlot00( saveToFileName, "wb" );
	if ( file.VirtualSlot58() == 0 ) {
		file.VirtualSlot11( "UtopiaSV" );
		file.WriteFloat( SAVE_VERSION );
		file.WriteFloat( friendlyFireTolerance );
		file.WriteFloat( currentFriendlyFire );
		file.WriteFloat( friendlyFireWarningPoints );
		file.WriteFloat( friendlyTractorTime );
		file.WriteFloat( friendlyTractorWarning );
		file.WriteFloat( maxFriendlyTractorTime );
		for ( int i = 0; i < 4; i++ ) {
			file.WriteInt( maxTorpedoLoad[i] );
			file.WriteInt( currentStarbaseTorpedoLoad[i] );
		}
		file.VirtualSlot11( gameName );
		file.VirtualSlot11( gamePath );
		file.VirtualSlot11( dataPath );
		file.VirtualSlot11( unknown20 );
		file.VirtualSlot11( unknown24 );

		if ( PyEmbed::StartSave( &file ) &&
			TGTimer::SaveTimers( &file ) &&
			g_kLocalizationManager.WriteToStream( &file ) &&
			g_kTimerManager.WriteToStream( &file ) &&
			g_kRealtimeTimerManager.WriteToStream( &file ) &&
			TGEvent::SaveEvents( &file ) &&
			g_kEventManager.WriteToStream( &file ) &&
			g_kLODModelManager.WriteToStream( &file ) &&
			g_kTGActionManager.WriteToStream( &file ) &&
			g_kSoundManager.WriteToStream( &file ) &&
			g_kMusicManager.WriteToStream( &file ) &&
			TGSoundRegion::SaveRegions( &file ) &&
			g_kRootWindow.WriteToStream( &file ) &&
			g_kMovieManager.WriteToStream( &file ) &&
			g_kInputManager.WriteToStream( &file ) &&
			TGParagraph::WriteCursorTimer( &file ) ) {
			Game *game = Game::GetCurrentGame();
			file.WriteBool( game != NULL );
			if ( ( game == NULL || game->WriteToStream( &file ) ) &&
				g_kSetManager.WriteToStream( &file ) &&
				PlacementObject::SaveStatics( &file ) &&
				CameraMode::SaveModes( &file ) &&
				g_kModelPropertyManager.WriteToStream( &file ) &&
				ObjectGroup::SaveGroups( &file ) &&
				TGCondition::SaveConditions( &file ) &&
				aiManager.WriteToStream( &file ) &&
				g_kAnimationManager.WriteToStream( &file ) &&
				g_kFocusManager.WriteToStream( &file ) &&
				PyEmbed::SaveModules( &file, "scripts" ) &&
				SensorSubsystem::WriteStatics( &file ) &&
				LensFlare::SaveStatics( &file ) &&
				WarpFlash::SaveStatics( &file ) &&
				AsteroidTile::WriteStaticData( &file ) &&
				ConditionEventCreator::SaveConditionEventCreators( &file ) ) {
				char timeText[256];
				sprintf( timeText, "Saving Utopia time, %f\n", GetGameTime() );
				TGObject::Log( timeText );
				file.WriteFloat( GetGameTime() );
				file.WriteFloat( UtopiaApp::GetApp()->GetGameFrameStep() );
				file.WriteLong( pauseCount );
				file.WriteLong( GraphicsModeInfo::GetCurrentMode()->GetCurrentResolution() );

				SetClass *renderedSet = g_kSetManager.GetRenderedSet();
				if ( renderedSet != NULL ) {
					file.WriteID( renderedSet->GetObjID() );
					file.WriteID( TGObject::GetSavedID( renderedSet->GetActiveCamera() ) );
				} else {
					file.WriteID( 0 );
				}
				g_kInterfaceModule.WriteToStream( &file );
				captainName.WriteToStream( &file );

				int viewscreenOn;
				if ( PyEmbed::CallFunction( "MissionLib", "IsViewscreenOn", "i", &viewscreenOn, NULL, 0 ) == -1 ) {
					PyErr_Print();
				}
				file.WriteBool( viewscreenOn != 0 );
				WriteTempMissionSaves( &file );
				if ( PyEmbed::FinishSave( &file ) ) {
					saved = true;
				}
			}
		}
	}
	file.Close();

	if ( !saved ) {
		remove( saveToFileName );
	} else {
		if ( unknown38 != NULL ) {
			delete unknown38;
		}
		unknown38 = new char[strlen( saveToFileName ) + 1];
		strcpy( unknown38, saveToFileName );
	}
	delete saveToFileName;
	saveToFileName = NULL;

	TGEvent *event = new TGEvent;
	event->SetDestination( TopWindow::GetTopWindow() );
	event->SetEventType( ET_GAME_SAVED );
	g_kEventManager.AddEvent( event );
	PyEmbed::CallFunction( "MainMenu.mainmenu", "PostSave", NULL, NULL, NULL, 0 );
}

/*
================
UtopiaModule::WriteTempMissionSaves

Every saved mission state in the temporary folder (MissionLib's
TEMP_MISSION_SAVE_FOLDER, under the current directory) into the stream,
behind their count. Without the folder's name nothing is written, not
even the count's final value.
================
*/
// FUNCTION: STBC 0x00444470
void UtopiaModule::WriteTempMissionSaves( TGStream *stream ) {
	char currentDir[MAX_PATH];
	char pattern[MAX_PATH];
	char folder[MAX_PATH];
	WIN32_FIND_DATA data;
	char *tempFolder;

	long countPos = stream->GetPos();
	int count = 0;
	stream->WriteInt( 0 );
	if ( GetCurrentDirectory( MAX_PATH, currentDir ) != 0 ) {
		tempFolder = NULL;
		if ( PyEmbed::GetValue( "MissionLib", "TEMP_MISSION_SAVE_FOLDER", "s", &tempFolder ) == -1 ) {
			return;
		}
		strcpy( folder, currentDir );
		strcat( folder, tempFolder );
		strcpy( pattern, folder );
		strcat( pattern, "*.msv" );
		// BUG: the search handle is never closed
		HANDLE find = FindFirstFile( pattern, &data );
		if ( find != INVALID_HANDLE_VALUE ) {
			WriteTempMissionSave( stream, data.cFileName );
			count++;
			while ( FindNextFile( find, &data ) ) {
				WriteTempMissionSave( stream, data.cFileName );
				count++;
			}
		}
	}
	long endPos = stream->GetPos();
	stream->VirtualSlot55( countPos, 0 );
	stream->WriteInt( count );
	stream->VirtualSlot55( endPos, 0 );
}

/*
================
UtopiaModule::WriteTempMissionSave

One saved mission state from the temporary folder into the stream: its
file name, its length and its bytes.
================
*/
// FUNCTION: STBC 0x00444640
void UtopiaModule::WriteTempMissionSave( TGStream *stream, char *fileName ) {
	char *tempFolder = NULL;
	if ( PyEmbed::GetValue( "MissionLib", "TEMP_MISSION_SAVE_FOLDER", "s", &tempFolder ) == -1 ) {
		return;
	}
	char currentDir[MAX_PATH];
	char path[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, currentDir );
	strcpy( path, currentDir );
	strcat( path, tempFolder );
	strcat( path, fileName );

	TGStream file;
	file.VirtualSlot00( path, "rb" );
	int length = file.GetLength();
	stream->VirtualSlot11( fileName );
	stream->WriteInt( length );
	char *buffer = new char[file.GetLength()];
	file.Read( buffer, file.GetLength() );
	stream->Write( buffer, file.GetLength() );
	delete buffer;
	file.Close();
}

/*
================
UtopiaModule::ProcessLoadFromFile

Hands the name SetLoadFromFileName kept to the CD check window, which
loads the game once the check passes.
================
*/
// FUNCTION: STBC 0x004447f0
void UtopiaModule::ProcessLoadFromFile( void ) {
	if ( loadFromFileName != NULL ) {
		CDCheckWindow *window = (CDCheckWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_CD_CHECK );
		if ( window != NULL ) {
			window->SetLoadFileName( loadFromFileName );
			window->RunCheck( false );
			delete loadFromFileName;
			loadFromFileName = NULL;
		}
	}
}

/*
================
UtopiaModule::ProcessInternalLoad

Loads the game SetInternalLoadFileName named, behind the loading movie: a
file without the tag or of another version gets a dialog. Otherwise the
running game is torn down, ProcessSave's state is read back in its order
and the loaded objects' pointers are fixed up (the objects' own pass is
tried three times). A load that succeeds restores the sounds, the sets,
the actions, the rendered set's camera, the viewscreen and the bridge's
alert, and tells the top window and the interface; one that fails clears
what was read and offers to exit.
================
*/
// FUNCTION: STBC 0x00444840
// DRAFT: same calls and flow; retail keeps this in ebp and 0 in ebx (ours this in ebx), lays the locals' slots and zeroing in another order, and calls the out-of-line TGBufferedFileStream::ReadID where ours expands it (inline budget, as in ProcessSave)
void UtopiaModule::ProcessInternalLoad( void ) {
	if ( internalLoadFileName == NULL ) {
		return;
	}

	loading = true;
	TGBufferedFileStream file( 0x100000 );
	bool loaded = false;
	int resolution = 0;
	long renderedSetID = 0;
	long cameraID = 0;
	bool viewscreenOn = false;
	file.VirtualSlot00( internalLoadFileName, "rb" );
	if ( file.VirtualSlot58() == 0 ) {
		// BUG: redundant: the log file's name is built, and never used
		char logName[256];
		strcpy( logName, "LoadFile-" );
		strcat( logName, internalLoadFileName );
		strcat( logName, ".txt" );
		char *separator;
		while ( ( separator = strchr( logName, '/' ) ) != NULL ) {
			*separator = '_';
		}
		while ( ( separator = strchr( logName, '\\' ) ) != NULL ) {
			*separator = '_';
		}

		// BUG: the tag read is never freed
		char *tag = file.VirtualSlot10( strlen( "UtopiaSV" ) + 16 );
		if ( tag == NULL || strcmp( tag, "UtopiaSV" ) != 0 ) {
			ModalDialogWindow *dialog = (ModalDialogWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MODAL_DIALOG );
			TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
			dialog->Run( NULL, database->GetString( "Invalid Save File" ), database->GetString( "OK" ), NULL, NULL, NULL );
			g_kLocalizationManager.Unload( database );
		} else if ( file.ReadFloat() != SAVE_VERSION ) {
			ModalDialogWindow *dialog = (ModalDialogWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MODAL_DIALOG );
			TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
			dialog->Run( NULL, database->GetString( "Invalid Save Version" ), database->GetString( "OK" ), NULL, NULL, NULL );
			g_kLocalizationManager.Unload( database );
		} else {
			// The loading movie plays while the game is torn down.
			g_kRootWindow.GetMouseCursor()->SetNotVisible( true );
			g_kMovieManager.LoadForcedMovie( "data/Movies/Loading.bik" );
			g_kMovieManager.PlayForcedMovieFrame();
			TGModule::CallVirtualSlots03And04();
			g_kImageManager.SetRenderer( UtopiaApp::GetRenderer() );
			TGModule::CallVirtualSlot01();
			if ( PyEmbed::CallFunction( "MainMenu.mainmenu", "StopBackgroundMovies", NULL, NULL, NULL, 0 ) == -1 ) {
				PyErr_Print();
			}
			if ( PyEmbed::CallFunction( "Multiplayer.MultiplayerMenus", "DeleteMenus", NULL, NULL, NULL, 0 ) == -1 ) {
				PyErr_Print();
			}
			if ( PyEmbed::CallFunction( "Multiplayer.MultiplayerMenus", "ResetFlags", NULL, NULL, NULL, 0 ) == -1 ) {
				PyErr_Print();
			}
			if ( PyEmbed::CallFunction( "MissionLib", "ResetViewscreen", NULL, NULL, NULL ) == -1 ) {
				PyErr_Print();
				return;
			}

			g_kMovieManager.PlayForcedMovieFrame();
			g_kUtopiaModule.SetMultiplayer( false );
			g_kRootWindow.KillChildren();
			g_kRootWindow.Shutdown();
			g_kMovieManager.PlayForcedMovieFrame();
			g_kSoundManager.DeleteAllSounds();
			g_kTimerManager.DeleteAllTimers();
			g_kRealtimeTimerManager.DeleteAllTimers();
			TGTimer::DeleteAllTimers();
			TGTimer::RemoveAllTimers();
			g_kMovieManager.PlayForcedMovieFrame();
			g_kEventManager.ClearEvents();
			TGEvent::DeleteAllEvents();
			TGEvent::RemoveAllEvents();
			TGSoundRegion::DeleteAllRegions();
			ObjectGroup::DeleteAllGroups();

			// ProcessSave's state, in its order.
			g_kMovieManager.PlayForcedMovieFrame();
			friendlyFireTolerance = file.ReadFloat();
			currentFriendlyFire = file.ReadFloat();
			friendlyFireWarningPoints = file.ReadFloat();
			friendlyTractorTime = file.ReadFloat();
			friendlyTractorWarning = file.ReadFloat();
			maxFriendlyTractorTime = file.ReadFloat();
			for ( int i = 0; i < 4; i++ ) {
				maxTorpedoLoad[i] = file.ReadInt();
				currentStarbaseTorpedoLoad[i] = file.ReadInt();
			}
			g_kMovieManager.PlayForcedMovieFrame();
			gameName = file.VirtualSlot10( -1 );
			gamePath = file.VirtualSlot10( -1 );
			dataPath = file.VirtualSlot10( -1 );
			char *episode = file.VirtualSlot10( -1 );
			char *mission = file.VirtualSlot10( -1 );
			g_kMovieManager.PlayForcedMovieFrame();
			if ( episode != NULL ) {
				LoadEpisodeSounds( episode, mission );
				delete episode;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( mission != NULL ) {
				delete mission;
			}

			if ( !PyEmbed::StartLoad( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !TGTimer::LoadTimers( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kLocalizationManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kTimerManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kRealtimeTimerManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !TGEvent::LoadEvents( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kEventManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kLODModelManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			// The action manager, the sound manager and the music.
			g_kMovieManager.PlayForcedMovieFrame();
			if ( TGObject::LoadObject( &file, 0 ) == NULL ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( TGObject::LoadObject( &file, 0 ) == NULL ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( TGObject::LoadObject( &file, 0 ) == NULL ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !TGSoundRegion::LoadRegions( &file ) ) {
				goto fileRead;
			}
			// The root window.
			g_kMovieManager.PlayForcedMovieFrame();
			if ( TGObject::LoadObject( &file, 0 ) == NULL ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kMovieManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kInputManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !TGParagraph::ReadCursorTimer( &file ) ) {
				goto fileRead;
			}
			// The game, when there was one.
			g_kMovieManager.PlayForcedMovieFrame();
			if ( file.ReadBool() && TGObject::LoadObject( &file, 0 ) == NULL ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kSetManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !PlacementObject::LoadStatics( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !CameraMode::LoadModes( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kModelPropertyManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !ObjectGroup::LoadGroups( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !TGCondition::LoadConditions( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !aiManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kAnimationManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !g_kFocusManager.ReadFromStream( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !PyEmbed::LoadModules( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !SensorSubsystem::ReadStatics( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !LensFlare::LoadStatics( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !WarpFlash::LoadStatics( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !AsteroidTile::ReadStaticData( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			if ( !ConditionEventCreator::LoadConditionEventCreators( &file ) ) {
				goto fileRead;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			SetGameTime( file.ReadFloat() );
			UtopiaApp::GetApp()->SetGameFrameTime( file.ReadFloat() );
			pauseCount = file.ReadLong();
			resolution = file.ReadLong();
			renderedSetID = file.ReadID();
			if ( renderedSetID != 0 ) {
				cameraID = file.ReadID();
			} else {
				cameraID = 0;
			}
			g_kInterfaceModule.ReadFromStream( &file );
			captainName.ReadFromStream( &file );
			viewscreenOn = file.ReadBool();
			g_kMovieManager.PlayForcedMovieFrame();
			ReadTempMissionSaves( &file );
			g_kMovieManager.PlayForcedMovieFrame();
			if ( PyEmbed::FinishLoad( &file ) ) {
				g_kMovieManager.PlayForcedMovieFrame();
				loaded = true;
			}
fileRead:
			file.Close();

			// BUG: redundant: the log file's name is built, and never used
			strcpy( logName, "FixupFile-" );
			strcat( logName, internalLoadFileName );
			strcat( logName, ".txt" );
			while ( ( separator = strchr( logName, '/' ) ) != NULL ) {
				*separator = '_';
			}
			while ( ( separator = strchr( logName, '\\' ) ) != NULL ) {
				*separator = '_';
			}

			// The fix-up passes.
			bool fixedUp = false;
			int pass;
			if ( !loaded ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up EventManager\n" );
			if ( !g_kEventManager.FixUpPointers() ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			g_kEventManager.PostFixUp();
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up TimerManager\n" );
			if ( !g_kTimerManager.FixUpPointers() ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			g_kTimerManager.PostFixUp();
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up realtime timer manager\n" );
			if ( !g_kRealtimeTimerManager.FixUpPointers() ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			g_kRealtimeTimerManager.PostFixUp();
			for ( pass = 0; pass < 3; pass++ ) {
				g_kMovieManager.PlayForcedMovieFrame();
				TGObject::Log( "Fixing up TGObjects\n" );
				if ( TGObject::FixUpLoadedObjects() ) {
					break;
				}
			}
			if ( pass >= 3 ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			g_kRealtimeTimerManager.RebaseTime( GetRealTime() );
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up TGParagraph statics\n" );
			if ( !TGParagraph::FixUpCursorTimer() ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			TGParagraph::PostFixUpCursorTimer();
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up SensorSubsystem statics\n" );
			if ( !SensorSubsystem::CanFixUpStatics() ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			SensorSubsystem::FixUpStatics();
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up LensFlares statics\n" );
			LensFlare::FixUpStatics();
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up Interface Module statics\n" );
			if ( !g_kInterfaceModule.FixUpPointers() ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			g_kInterfaceModule.PostFixUp();
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up MovieManager\n" );
			if ( !g_kMovieManager.FixUpPointers() ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			g_kMovieManager.PostFixUp();
			g_kMovieManager.PlayForcedMovieFrame();
			TGObject::Log( "Fixing up AI\n" );
			if ( !aiManager.FixUpPointers() ) {
				goto fixedUpDone;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			aiManager.PostFixUp();
			g_kMovieManager.PlayForcedMovieFrame();
			if ( PlacementObject::FixUpStatics() ) {
				fixedUp = true;
			}
fixedUpDone:
			g_kMovieManager.CloseForcedMovie();

			if ( !fixedUp ) {
				// Forget what was read without deleting it.
				g_kRootWindow.Unknown728D90();
				g_kFontManager.ForgetAll();
				g_kIconManager.ForgetAll();
				TGParagraph::ClearCursor();
				g_kEventManager.ForgetAll();
				g_kTimerManager.ForgetAll();
				g_kSoundManager.ForgetAll();
				g_kLocalizationManager.ForgetAll();
				g_kTGActionManager.ForgetAllActions();
				TopWindow::ForgetTopWindow();
				aiManager.Clear();
				TimeSliceProcess::RemoveAllProcesses();
				g_kSetManager.Clear();
				Game::ClearStatics();
				PyEmbed::UnloadModule( "MainMenu.mainmenu" );
				PyEmbed::UnloadModule( "FontsAndIcons" );
				PyEmbed::GetModule( "FontsAndIcons", false );
				TGModule::CallVirtualSlots03And04();
				TGModule::CallVirtualSlot01();
			} else {
				if ( unknown38 != NULL ) {
					delete unknown38;
				}
				unknown38 = new char[strlen( internalLoadFileName ) + 1];
				strcpy( unknown38, internalLoadFileName );
			}
			delete internalLoadFileName;
			internalLoadFileName = NULL;

			CameraObjectClass::SetAllRenderers( g_kSystemWrapper.GetRenderer() );
			g_kRootWindow.VirtualSlot60();
			g_kRootWindow.Layout();
			TopWindow *topWindow = TopWindow::GetTopWindow();
			MainWindow *options = topWindow->FindMainWindow( MWT_OPTIONS );
			if ( topWindow->FindMainWindow( MWT_CONSOLE )->IsVisible() ) {
				topWindow->ToggleConsole();
			}

			if ( !fixedUp ) {
				loading = false;
				PyEmbed::CallFunction( "MainMenu.mainmenu", "BuildInterface", NULL, NULL, NULL, 0 );
				options->SetNotVisible( true );
				options->SetVisible( true );
				topWindow->MoveToFront( options, true );
				topWindow->SetFocus( options, true );
				ModalDialogWindow *dialog = (ModalDialogWindow *)topWindow->FindMainWindow( MWT_MODAL_DIALOG );
				TGEvent *exitEvent = new TGEvent;
				exitEvent->SetEventType( ET_EXIT_PROGRAM );
				exitEvent->SetDestination( dialog );
				TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
				dialog->Run( database->GetString( "Failed to Load" ), database->GetString( "Failed to Load Text" ), database->GetString( "Exit Program" ), exitEvent, NULL, NULL );
				g_kLocalizationManager.Unload( database );
				loading = false;	// BUG: redundant: cleared above already
			} else {
				if ( options->IsVisible() ) {
					topWindow->ToggleOptionsMenu();
				} else if ( pauseCount == 0 ) {
					Pause( false );
				}
				float volume = g_kSoundManager.GetMasterVolume();
				g_kSoundManager.SetMasterVolume( 0.0f );
				g_kSoundManager.RestorePlayingSounds();
				g_kSetManager.RestoreSets();
				g_kTGActionManager.RestartLoadedActions();

				SetClass *renderedSet = (SetClass *)TGObject::GetTGObjectPtr( renderedSetID );
				CameraObjectClass *camera = (CameraObjectClass *)TGObject::GetTGObjectPtr( cameraID );
				if ( renderedSet != NULL && camera != NULL ) {
					renderedSet->SetActiveCamera( camera->GetName() );
				}

				char functionName[256];
				if ( viewscreenOn ) {
					sprintf( functionName, "SetViewscreenOn" );
				} else {
					sprintf( functionName, "ResetViewscreen" );
				}
				if ( PyEmbed::CallFunction( "MissionLib", functionName, NULL, NULL, NULL ) == -1 ) {
					PyErr_Print();
				}

				BridgeObjectClass *bridge = BridgeObjectClass::Cast( g_kSetManager.GetSet( "bridge" )->GetObject( "bridge" ) );
				int alertLevel = Game::GetCurrentPlayer()->GetAlertLevel();
				if ( alertLevel == ShipClass::GREEN_ALERT ) {
					bridge->GoToGreenAllert();
				} else if ( alertLevel == ShipClass::YELLOW_ALERT ) {
					bridge->GoToYellowAllert();
				} else if ( alertLevel == ShipClass::RED_ALERT ) {
					bridge->GoToRedAllert();
				}

				TGEvent *loadedEvent = new TGEvent;
				loadedEvent->SetDestination( TopWindow::GetTopWindow() );
				loadedEvent->SetEventType( ET_GAME_LOADED );
				g_kEventManager.AddEvent( loadedEvent );

				// The interface takes the saved resolution.
				TGIntEvent *uiEvent = new TGIntEvent;
				uiEvent->SetDestination( &g_kRootWindow );
				uiEvent->SetEventType( ET_UI_SWITCH_ICON_GROUPS );
				uiEvent->SetInt( resolution );
				g_kEventManager.ProcessEvent( uiEvent );
				delete uiEvent;
				uiEvent = new TGIntEvent;
				uiEvent->SetDestination( &g_kRootWindow );
				uiEvent->SetEventType( ET_UI_RESIZE );
				uiEvent->SetInt( resolution );
				g_kEventManager.ProcessEvent( uiEvent );
				delete uiEvent;
				uiEvent = new TGIntEvent;
				uiEvent->SetDestination( &g_kRootWindow );
				uiEvent->SetEventType( ET_UI_REPOSITION );
				uiEvent->SetInt( resolution );
				g_kEventManager.ProcessEvent( uiEvent );
				delete uiEvent;

				PyEmbed::CallFunction( "MainMenu.mainmenu", "LoadConfigPart2", NULL, NULL, NULL );
				PyEmbed::CallFunction( "MainMenu.mainmenu", "PostLoad", NULL, NULL, NULL, 0 );
				loading = false;
				InterfaceModule::DoTheRightThing();
				g_kSoundManager.SetMasterVolume( volume );
			}
			return;
		}
	}
	loading = false;
	delete internalLoadFileName;
	internalLoadFileName = NULL;
}

/*
================
UtopiaModule::ReadTempMissionSaves

The saved mission states WriteTempMissionSaves wrote, each back into its
file in the temporary folder.
================
*/
// FUNCTION: STBC 0x00445bc0
void UtopiaModule::ReadTempMissionSaves( TGStream *stream ) {
	for ( int count = stream->ReadInt(); count > 0; count-- ) {
		char *fileName = stream->VirtualSlot10( -1 );
		ReadTempMissionSave( stream, fileName );
		delete fileName;
	}
}

/*
================
UtopiaModule::ReadTempMissionSave

One saved mission state from the stream into the temporary folder (MissionLib's
TEMP_MISSION_SAVE_FOLDER, under the current directory), under the file
name without its directory.
================
*/
// FUNCTION: STBC 0x00445c00
void UtopiaModule::ReadTempMissionSave( TGStream *stream, char *fileName ) {
	int length = stream->ReadInt();
	char *buffer = new char[length];
	stream->Read( buffer, length );

	char currentDir[MAX_PATH];
	char path[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, currentDir );
	char *tempFolder = NULL;
	PyEmbed::GetValue( "MissionLib", "TEMP_MISSION_SAVE_FOLDER", "s", &tempFolder );
	strcpy( path, currentDir );
	strcat( path, tempFolder );
	char *slash = strrchr( fileName, '\\' );
	if ( slash == NULL ) {
		slash = strrchr( fileName, '/' );
	}
	if ( slash != NULL ) {
		fileName = slash + 1;
	}
	strcat( path, fileName );

	TGStream file;
	file.VirtualSlot00( path, "wb" );
	file.Write( buffer, length );
	file.Close();
	delete buffer;
}

/*
================
UtopiaModule::InitializeNetwork

Makes the network and connects it: a host listens on the game's port, a client
connects to the address on the port it is given. A connection that is made,
or still under way, gets the file transfer and the GameSpy client.
================
*/
// FUNCTION: STBC 0x00445d90 ?InitializeNetwork@UtopiaModule@@QAEHPADPAVTGString@@H@Z
int UtopiaModule::InitializeNetwork( char *address, TGString *password, int port ) {
	network = new TGWinsockNetwork;
	char *host;
	int listenPort = TGWinsockNetwork::WINSOCK_PORT;
	if ( isHost ) {
		host = NULL;
	} else {
		listenPort = port;
		if ( gameSpy != NULL ) {
			delete gameSpy;
			gameSpy = NULL;
		}
		host = address;
	}
	( (TGWinsockNetwork *)network )->SetPortNumber( listenPort, false );

	int status;
	if ( password->GetLength() > 0 ) {
		status = network->Connect( host, password );
	} else {
		status = network->Connect( host, NULL );
	}
	if ( status != 0 && status != 2 && status != 3 ) {
		TerminateNetwork();
		modemConnection = DetermineModemConnection();
		return status;
	}

	netFile = new NetFile( 0 );
	if ( gameSpy == NULL ) {
		gameSpy = new GameSpy( 0 );
	}
	modemConnection = DetermineModemConnection();
	return status;
}

/*
================
UtopiaModule::DetermineModemConnection

Whether the machine is on a connected dial-up line.
================
*/
// FUNCTION: STBC 0x00445ec0
bool UtopiaModule::DetermineModemConnection( void ) {
	return GetRasConnectionStatus() == 0 ? true : false;
}

/*
================
UtopiaModule::TerminateNetwork

The network disconnects and is deleted through an event; the file
transfers and the GameSpy client go with it.
================
*/
// FUNCTION: STBC 0x00445ed0
void UtopiaModule::TerminateNetwork( void ) {
	if ( network != NULL ) {
		network->Disconnect();
		TGEvent *event = new TGEvent;
		event->SetDestination( network );
		event->SetEventType( ET_DELETE_ME_STAGE_1 );
		event->SetPrivate( true );
		g_kEventManager.AddEvent( event );
		network = NULL;
	}
	if ( netFile != NULL ) {
		delete netFile;
		netFile = NULL;
	}
	if ( gameSpy != NULL ) {
		delete gameSpy;
		gameSpy = NULL;
	}
}

/*
================
UtopiaModule::IncrementClientID
================
*/
// FUNCTION: STBC 0x00445f70
void UtopiaModule::IncrementClientID( void ) {
	do {
		unusedClientID++;
		int base = playerNumber << CLIENT_ID_BLOCK_SHIFT;
		if ( unusedClientID >= base + 0x4003FFFF ) {
			unusedClientID = base + 0x3FFFFFFF;
		}
	} while ( TGObject::GetTGObjectPtr( unusedClientID ) != NULL );
}

/*
================
UtopiaModule::CreateGameSpy

A new GameSpy client replaces the old.
================
*/
// FUNCTION: STBC 0x00445fc0
void UtopiaModule::CreateGameSpy( void ) {
	if ( gameSpy != NULL ) {
		delete gameSpy;
		gameSpy = NULL;
	}
	gameSpy = new GameSpy;
}

/*
================
UtopiaModule::TerminateGameSpy

The multiplayer window forgets the server it had chosen.
================
*/
// FUNCTION: STBC 0x00446010
void UtopiaModule::TerminateGameSpy( void ) {
	if ( gameSpy != NULL ) {
		delete gameSpy;
		gameSpy = NULL;
	}
	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow != NULL ) {
		MultiplayerWindow *window = (MultiplayerWindow *)topWindow->FindMainWindow( MWT_MULTIPLAYER );
		if ( window != NULL ) {
			window->SetCurrentServer( NULL );
		}
	}
}

/*
================
UtopiaModule::VirtualSlot06

Before the display mode changes: lets go of the renderer everywhere.
================
*/
// FUNCTION: STBC 0x00446050
void UtopiaModule::VirtualSlot06( void ) {
	NiImage::RemoveRenderer( UtopiaApp::GetRenderer() );
	UtopiaApp::SetRenderer( NULL );
	g_kSystemWrapper.SetRenderer( NULL );
	CameraObjectClass::SetAllRenderers( NULL );
	g_kImageManager.SetRenderer( NULL );
}

/*
================
UtopiaModule::VirtualSlot07

After the display mode changes: hands the new renderer (the argument)
everywhere, lets the card's script force Z buffering, and reloads the
icons.
================
*/
// FUNCTION: STBC 0x00446180
// PARKED: register/schedule: retail loads device->name after the GetRenderer call, between it and the bit-depth slot call; this source loads it after the comparison
void UtopiaModule::VirtualSlot07( int argument ) {
	NiD3DRender *renderer = (NiD3DRender *)argument;
	NiImage::RemoveRenderer( renderer );
	NiImage::AddRenderer( renderer );
	g_kSystemWrapper.SetRenderer( renderer );
	UtopiaApp::SetRenderer( renderer );
	CameraObjectClass::SetAllRenderers( renderer );
	g_kImageManager.SetRenderer( renderer );
	if ( renderer != NULL ) {
		Ni3DDeviceDesc *device = renderer->Unknown7C2EE0();
		int forceZBuffering;
		PyEmbed::CallFunction( "MainMenu.CardSpecificSetup", "ForceZBuffering", "i", &forceZBuffering, "(si)", device->name, UtopiaApp::GetRenderer()->VirtualSlot21() > 16 );
		if ( forceZBuffering ) {
			renderer->Unknown7C1400( false );
		}
	}
	g_kIconManager.ReloadIconGroups();
}

/*
================
UtopiaModule::ClearEpisodeSounds

Empties <game path>/sfx/<game name>, the copies LoadEpisodeSounds makes.
================
*/
// FUNCTION: STBC 0x004462a0
bool UtopiaModule::ClearEpisodeSounds( void ) {
	char path[256];
	strcpy( path, gamePath );
	strcat( path, "/sfx/" );
	strcat( path, gameName );
	g_kSystemWrapper.DeleteDirectoryContents( path );
	CreateDirectory( path, NULL );
	return true;
}

/*
================
UtopiaModule::LoadEpisodeSounds

Copies the episode's sounds from the data path's sfx directory to the game
path's, unless they are there already, and keeps the episode's and the
mission's names.
================
*/
// FUNCTION: STBC 0x00446360
bool UtopiaModule::LoadEpisodeSounds( char *episode, char *mission ) {
	if ( unknown20 != NULL ) {
		delete unknown20;
	}
	unknown20 = NULL;
	if ( unknown24 != NULL ) {
		delete unknown24;
	}
	unknown24 = NULL;

	char source[256];
	strcpy( source, dataPath );
	strcat( source, "/sfx/" );
	strcat( source, gameName );
	strcat( source, "/" );
	strcat( source, episode );
	char destination[256];
	strcpy( destination, gamePath );
	strcat( destination, "/sfx/" );
	strcat( destination, gameName );
	strcat( destination, "/" );
	strcat( destination, episode );

	TGDirectory directory;
	directory.Read( destination, NULL, true );
	if ( directory.GetFiles() == NULL ) {
		if ( !ClearEpisodeSounds() ) {
			return false;
		}
		g_kSystemWrapper.CopyDirectory( destination, source );
		unknown20 = new char[strlen( episode ) + 1];
		strcpy( unknown20, episode );
		if ( mission != NULL ) {
			unknown24 = new char[strlen( mission ) + 1];
			strcpy( unknown24, mission );
		}
	}
	return true;
}

/*
================
UtopiaModule::SetGameName
================
*/
// FUNCTION: STBC 0x00446690
void UtopiaModule::SetGameName( char *name ) {
	if ( gameName != NULL ) {
		delete gameName;
	}
	if ( name != NULL ) {
		gameName = new char[strlen( name ) + 1];
		strcpy( gameName, name );
	} else {
		gameName = NULL;
	}
}

/*
================
UtopiaModule::SetGamePath
================
*/
// FUNCTION: STBC 0x00446700
void UtopiaModule::SetGamePath( char *path ) {
	if ( gamePath != NULL ) {
		delete gamePath;
	}
	if ( path != NULL ) {
		gamePath = new char[strlen( path ) + 1];
		strcpy( gamePath, path );
	} else {
		gamePath = NULL;
	}
}

/*
================
UtopiaModule::SetDataPath
================
*/
// FUNCTION: STBC 0x00446770
void UtopiaModule::SetDataPath( char *path ) {
	if ( dataPath != NULL ) {
		delete dataPath;
	}
	if ( path != NULL ) {
		dataPath = new char[strlen( path ) + 1];
		strcpy( dataPath, path );
	} else {
		dataPath = NULL;
	}
}

// The <list> instance retail kept from this unit (the proximity checks'
// type list, through their inline destructor).
// FUNCTION: STBC 0x004467e0 ??1?$list@U?$pair@HH@std@@V?$allocator@U?$pair@HH@std@@@2@@std@@QAE@XZ
