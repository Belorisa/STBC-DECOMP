/*
===============================================================================

	CharacterAction.cpp

===============================================================================
*/

#include <iostream>

#include <stdio.h>
#include <string.h>

#include "CharacterAction.h"
#include "CharacterClass.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "SetManager.h"
#include "TGStream.h"
#include "TGDirectory.h"
#include "TGOverlayManager.h"
#include "Episode.h"
#include "Game.h"
#include "MainWindow.h"
#include "Mission.h"
#include "STMissionLog.h"
#include "STStylizedWindow.h"
#include "SubtitleAction.h"
#include "SubtitleWindow.h"
#include "TopWindow.h"
#include "TGActionManager.h"
#include "TGAnimationManagerClass.h"
#include "TGConfigMapping.h"
#include "TGObjPtrEvent.h"
#include "TGParagraph.h"
#include "TGPhonemeAction.h"
#include "TGSequence.h"
#include "TGSequenceEvent.h"
#include "TGSound.h"
#include "TGSoundAction.h"
#include "TGSoundManager.h"
#include "TGStringEvent.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "UtopiaApp.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00672070 _$E544
// FUNCTION: STBC 0x00672090 _$E543

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00670fd0 _$E67
// FUNCTION: STBC 0x00670fe0 _$E66
// FUNCTION: STBC 0x00670ff0 _$E250
// FUNCTION: STBC 0x00671000 _$E249
// FUNCTION: STBC 0x00671480 _$E78
// FUNCTION: STBC 0x00671490 _$E77
// FUNCTION: STBC 0x00671810 _$E82
// FUNCTION: STBC 0x00671820 _$E81
// FUNCTION: STBC 0x00671830 _$E86
// FUNCTION: STBC 0x00671840 _$E85
// FUNCTION: STBC 0x00671850 _$E90
// FUNCTION: STBC 0x00671860 _$E89
// FUNCTION: STBC 0x00671870 _$E207
// FUNCTION: STBC 0x00671880 _$E206
// FUNCTION: STBC 0x00671890 _$E149
// FUNCTION: STBC 0x006718a0 _$E148
// FUNCTION: STBC 0x006718b0 _$E162
// FUNCTION: STBC 0x006718c0 _$E161
// FUNCTION: STBC 0x006718d0 _$E212
// FUNCTION: STBC 0x006718e0 _$E211
// FUNCTION: STBC 0x006718f0 _$E255
// FUNCTION: STBC 0x00671900 _$E254
// FUNCTION: STBC 0x00671910 _$E216
// FUNCTION: STBC 0x00671920 _$E215
// FUNCTION: STBC 0x00671930 _$E259
// FUNCTION: STBC 0x00671940 _$E258
// FUNCTION: STBC 0x00671950 _$E266
// FUNCTION: STBC 0x00671960 _$E265
// FUNCTION: STBC 0x00671970 _$E373
// FUNCTION: STBC 0x00671980 _$E372
// FUNCTION: STBC 0x00671990 _$E377
// FUNCTION: STBC 0x006719a0 _$E376
// FUNCTION: STBC 0x006719b0 _$E107
// FUNCTION: STBC 0x006719c0 _$E106
// FUNCTION: STBC 0x006719d0 _$E359
// FUNCTION: STBC 0x006719e0 _$E358
// FUNCTION: STBC 0x006719f0 _$E366
// FUNCTION: STBC 0x00671a00 _$E365
// FUNCTION: STBC 0x00671a10 _$E389
// FUNCTION: STBC 0x00671a20 _$E388
// FUNCTION: STBC 0x00671a30 _$E393
// FUNCTION: STBC 0x00671a40 _$E392
// FUNCTION: STBC 0x00671a50 _$E433
// FUNCTION: STBC 0x00671a60 _$E432
// FUNCTION: STBC 0x00671a70 _$E439
// FUNCTION: STBC 0x00671a80 _$E438
// FUNCTION: STBC 0x00671a90 _$E381
// FUNCTION: STBC 0x00671aa0 _$E380
// FUNCTION: STBC 0x00671ab0 _$E385
// FUNCTION: STBC 0x00671ac0 _$E384
// FUNCTION: STBC 0x00671ad0 _$E397
// FUNCTION: STBC 0x00671ae0 _$E396
// FUNCTION: STBC 0x00671af0 _$E401
// FUNCTION: STBC 0x00671b00 _$E400
// FUNCTION: STBC 0x00671b10 _$E405
// FUNCTION: STBC 0x00671b20 _$E404
// FUNCTION: STBC 0x00671b30 _$E409
// FUNCTION: STBC 0x00671b40 _$E408
// FUNCTION: STBC 0x00671b50 _$E413
// FUNCTION: STBC 0x00671b60 _$E412
// FUNCTION: STBC 0x00671b70 _$E417
// FUNCTION: STBC 0x00671b80 _$E416
// FUNCTION: STBC 0x00671b90 _$E421
// FUNCTION: STBC 0x00671ba0 _$E420
// FUNCTION: STBC 0x00671bb0 _$E425
// FUNCTION: STBC 0x00671bc0 _$E424
// FUNCTION: STBC 0x00671bd0 _$E112
// FUNCTION: STBC 0x00671be0 _$E111
// FUNCTION: STBC 0x00671bf0 _$E429
// FUNCTION: STBC 0x00671c00 _$E428
// FUNCTION: STBC 0x00671c10 _$E117
// FUNCTION: STBC 0x00671c20 _$E116
// FUNCTION: STBC 0x00671c30 _$E443
// FUNCTION: STBC 0x00671c40 _$E442
// FUNCTION: STBC 0x00671c50 _$E447
// FUNCTION: STBC 0x00671c60 _$E446
// FUNCTION: STBC 0x00671c70 _$E451
// FUNCTION: STBC 0x00671c80 _$E450
// FUNCTION: STBC 0x00671c90 _$E455
// FUNCTION: STBC 0x00671ca0 _$E454
// FUNCTION: STBC 0x00671cb0 _$E126
// FUNCTION: STBC 0x00671cc0 _$E125
// FUNCTION: STBC 0x00671cd0 _$E459
// FUNCTION: STBC 0x00671ce0 _$E458
// FUNCTION: STBC 0x00671cf0 _$E463
// FUNCTION: STBC 0x00671d00 _$E462
// FUNCTION: STBC 0x00671d10 _$E467
// FUNCTION: STBC 0x00671d20 _$E466
// FUNCTION: STBC 0x00671d30 _$E132
// FUNCTION: STBC 0x00671d40 _$E131
// FUNCTION: STBC 0x00671d50 _$E137
// FUNCTION: STBC 0x00671d60 _$E136
// FUNCTION: STBC 0x00671d70 _$E167
// FUNCTION: STBC 0x00671d80 _$E166
// FUNCTION: STBC 0x00671d90 _$E471
// FUNCTION: STBC 0x00671da0 _$E470
// FUNCTION: STBC 0x00671db0 _$E172
// FUNCTION: STBC 0x00671dc0 _$E171
// FUNCTION: STBC 0x00671dd0 _$E475
// FUNCTION: STBC 0x00671de0 _$E474
// FUNCTION: STBC 0x00671df0 _$E479
// FUNCTION: STBC 0x00671e00 _$E478
// FUNCTION: STBC 0x00671e10 _$E177
// FUNCTION: STBC 0x00671e20 _$E176
// FUNCTION: STBC 0x00671e30 _$E483
// FUNCTION: STBC 0x00671e40 _$E482
// FUNCTION: STBC 0x00671e50 _$E142
// FUNCTION: STBC 0x00671e60 _$E141
// FUNCTION: STBC 0x00671e70 _$E487
// FUNCTION: STBC 0x00671e80 _$E486
// FUNCTION: STBC 0x00671e90 _$E491
// FUNCTION: STBC 0x00671ea0 _$E490
// FUNCTION: STBC 0x00671eb0 _$E182
// FUNCTION: STBC 0x00671ec0 _$E181
// FUNCTION: STBC 0x00671ed0 _$E187
// FUNCTION: STBC 0x00671ee0 _$E186
// FUNCTION: STBC 0x00671ef0 _$E495
// FUNCTION: STBC 0x00671f00 _$E494
// FUNCTION: STBC 0x00671f10 _$E499
// FUNCTION: STBC 0x00671f20 _$E498
// FUNCTION: STBC 0x00671f30 _$E503
// FUNCTION: STBC 0x00671f40 _$E502
// FUNCTION: STBC 0x00671f50 _$E507
// FUNCTION: STBC 0x00671f60 _$E506
// FUNCTION: STBC 0x00671f70 _$E511
// FUNCTION: STBC 0x00671f80 _$E510
// FUNCTION: STBC 0x00671f90 _$E515
// FUNCTION: STBC 0x00671fa0 _$E514
// FUNCTION: STBC 0x00671fb0 _$E519
// FUNCTION: STBC 0x00671fc0 _$E518
// FUNCTION: STBC 0x00671fd0 _$E523
// FUNCTION: STBC 0x00671fe0 _$E522
// FUNCTION: STBC 0x00671ff0 _$E192
// FUNCTION: STBC 0x00672000 _$E191
// FUNCTION: STBC 0x00672010 _$E527
// FUNCTION: STBC 0x00672020 _$E526
// FUNCTION: STBC 0x00672030 _$E197
// FUNCTION: STBC 0x00672040 _$E196
// FUNCTION: STBC 0x00672050 _$E202
// FUNCTION: STBC 0x00672060 _$E201
// FUNCTION: STBC 0x006720a0 _$E531
// FUNCTION: STBC 0x006720b0 _$E530
// FUNCTION: STBC 0x006720c0 _$E536
// FUNCTION: STBC 0x006720d0 _$E535
// FUNCTION: STBC 0x006720e0 _$E341
// FUNCTION: STBC 0x006720f0 _$E340

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0066f810 _$E32
// FUNCTION: STBC 0x0066f820 _$E29
// FUNCTION: STBC 0x0066f830 _$E31
// FUNCTION: STBC 0x0066f840 _$E30
// FUNCTION: STBC 0x0066f850 _$E38
// FUNCTION: STBC 0x0066f860 _$E35
// FUNCTION: STBC 0x0066f870 _$E37
// FUNCTION: STBC 0x0066f880 _$E36

// DATA: STBC 0x00991dec
bool CharacterAction::skipAllLines;

// The glyph a logged or subtitled line starts with before the speaker's
// name: a character of the font's private range (invented name).
#define SPEAKER_SEPARATOR_CHAR	( (short)0xE098 )

// The number of entries of phonemeCodes (invented name).
#define NUM_PHONEME_CODES		0x95

// The overlay phonemes of the lip sync codes (Play and ConvertFolderLWVs
// read them). The name is invented.
// DATA: STBC 0x009574b0
static PhonemeCode phonemeCodes[NUM_PHONEME_CODES] = {
	{ "0x0000", 1 }, { "0x005f", 1 }, { "0x0041", 3 }, { "0x0042", 5 }, { "0x0043", 5 }, { "0x0044", 5 },
	{ "0x0045", 4 }, { "0x0046", 5 }, { "0x0047", 2 }, { "0x0048", 2 }, { "0x0049", 3 }, { "0x004a", 4 },
	{ "0x004b", 5 }, { "0x004c", 2 }, { "0x004d", 5 }, { "0x004e", 5 }, { "0x004f", 3 }, { "0x0050", 5 },
	{ "0x0051", 2 }, { "0x0052", 2 }, { "0x0053", 4 }, { "0x0054", 4 }, { "0x0055", 2 }, { "0x0056", 4 },
	{ "0x0057", 2 }, { "0x0058", 4 }, { "0x0059", 3 }, { "0x005a", 4 }, { "0x0061", 3 }, { "0x0062", 5 },
	{ "0x0063", 5 }, { "0x0064", 5 }, { "0x0065", 3 }, { "0x0066", 5 }, { "0x0067", 2 }, { "0x0068", 2 },
	{ "0x0069", 4 }, { "0x006a", 4 }, { "0x006b", 2 }, { "0x006c", 2 }, { "0x006d", 5 }, { "0x006e", 5 },
	{ "0x006f", 2 }, { "0x0070", 5 }, { "0x0071", 2 }, { "0x0072", 2 }, { "0x0073", 4 }, { "0x0074", 4 },
	{ "0x0075", 2 }, { "0x0076", 4 }, { "0x0077", 2 }, { "0x0078", 4 }, { "0x0079", 3 }, { "0x007a", 4 },
	{ "0x00f0", 2 }, { "0x0250", 3 }, { "0x0251", 3 }, { "0x0252", 3 }, { "0x0253", 5 }, { "0x0254", 3 },
	{ "0x0255", 5 }, { "0x0256", 4 }, { "0x0257", 4 }, { "0x0258", 4 }, { "0x0259", 4 }, { "0x025a", 4 },
	{ "0x025b", 2 }, { "0x025c", 4 }, { "0x025d", 4 }, { "0x025e", 4 }, { "0x025f", 2 }, { "0x0260", 2 },
	{ "0x0261", 2 }, { "0x0262", 2 }, { "0x0263", 2 }, { "0x0264", 2 }, { "0x0265", 2 }, { "0x0266", 2 },
	{ "0x0267", 3 }, { "0x0268", 3 }, { "0x0269", 2 }, { "0x026a", 3 }, { "0x026b", 3 }, { "0x026c", 3 },
	{ "0x026d", 2 }, { "0x026e", 2 }, { "0x026f", 3 }, { "0x0270", 3 }, { "0x0271", 2 }, { "0x0272", 2 },
	{ "0x0273", 2 }, { "0x0274", 2 }, { "0x0275", 2 }, { "0x0276", 2 }, { "0x0277", 2 }, { "0x0278", 2 },
	{ "0x0279", 2 }, { "0x027a", 2 }, { "0x027b", 2 }, { "0x027c", 2 }, { "0x027d", 2 }, { "0x027e", 2 },
	{ "0x027f", 2 }, { "0x0280", 2 }, { "0x0281", 2 }, { "0x0282", 4 }, { "0x0283", 4 }, { "0x0284", 4 },
	{ "0x0285", 4 }, { "0x0286", 4 }, { "0x0287", 4 }, { "0x0288", 4 }, { "0x0289", 2 }, { "0x028a", 2 },
	{ "0x028b", 2 }, { "0x028c", 2 }, { "0x028d", 2 }, { "0x028e", 2 }, { "0x028f", 2 }, { "0x0290", 4 },
	{ "0x0291", 4 }, { "0x0292", 2 }, { "0x0293", 2 }, { "0x0294", 3 }, { "0x0295", 3 }, { "0x0296", 3 },
	{ "0x0297", 4 }, { "0x0298", 3 }, { "0x0299", 4 }, { "0x029a", 4 }, { "0x029b", 2 }, { "0x029c", 2 },
	{ "0x029d", 3 }, { "0x029e", 2 }, { "0x029f", 2 }, { "0x02a0", 2 }, { "0x02a1", 2 }, { "0x02a2", 2 },
	{ "0x02a3", 2 }, { "0x02a4", 2 }, { "0x02a5", 2 }, { "0x02a6", 2 }, { "0x02a7", 2 }, { "0x02a8", 2 },
	{ "0x02a9", 2 }, { "0x02aa", 2 }, { "0x02ab", 2 }, { "0x02ac", 2 }, { "0x02ad", 2 }
};

/*
================
CharacterAction::Cast

The object as a CharacterAction; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0066f890
CharacterAction *CharacterAction::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( CHARACTERACTION_TYPE ) ) {
		return (CharacterAction *)object;
	}
	return NULL;
}

/*
================
CharacterAction::ConvertFolderLWVs

Writes a .LIP file for each .LWV file of the folder. A file whose name has
a space keeps the .LIP file of the name before the space at the start of
its own: "line part2.LWV" appends its lip sync to "line.LIP".
================
*/
// FUNCTION: STBC 0x0066f8c0
void CharacterAction::ConvertFolderLWVs( char *folder ) {
	char file[_MAX_PATH];
	char lipFile[_MAX_PATH];

	TGDirectory *directory = new TGDirectory;
	if ( directory->Read( folder, "*.lwv", false ) && directory->GetNumFiles() > 0 ) {
		int size;
		for ( TGDirectoryFile *entry = directory->GetFiles(); entry != NULL; entry = entry->next ) {
			char *space = NULL;
			sprintf( file, "%s/%s", folder, entry->name );
			char *extension = strrchr( file, '.' );
			char *data = NULL;
			if ( extension == NULL ) {
				continue;
			}
			*extension = '\0';
			if ( strrchr( file, ' ' ) != NULL ) {
				space = strrchr( file, ' ' );
				*space = '\0';
			}
			sprintf( lipFile, "%s.LIP", file );
			if ( space != NULL ) {
				*space = ' ';
				TGStream first;
				first.VirtualSlot00( lipFile, "rb" );
				if ( first.VirtualSlot58() == 0 ) {
					size = first.GetLength();
					data = new char[size];
					first.Read( data, size );
					first.Close();
				}
			}
			*extension = '.';
			TGOverlayManager::ConvertLWVToLIP( file, lipFile, phonemeCodes, NUM_PHONEME_CODES );
			if ( data != NULL ) {
				TGStream converted;
				converted.VirtualSlot00( lipFile, "rb" );
				int convertedSize = converted.GetLength();
				char *convertedData = new char[convertedSize];
				converted.Read( convertedData, convertedSize );
				converted.Close();
				TGStream joined;
				joined.VirtualSlot00( lipFile, "wb" );
				joined.Write( data, size );
				joined.Write( convertedData, convertedSize );
				joined.Close();
				delete convertedData;
				delete data;
			}
		}
		delete directory;
	}
	// BUG: a folder that can't be read or has no .lwv files leaks the directory
}

/*
================
CharacterAction::CharacterAction

Keeps owned copies of both strings and loads the database the lines come from:
the one given, else the character's.
================
*/
// FUNCTION: STBC 0x0066fb90
CharacterAction::CharacterAction( CharacterClass *character, int type_, char *detail_, char *detail2_, int unknown, TGLocalizationDatabase *database_, int unknown2, int unknown3 ) : TGAction( 0 ) {
	characterID = character ? character->GetObjID() : 0;
	objectName = NULL;
	setName = NULL;
	type = type_;
	if ( detail_ != NULL ) {
		detail = new char[strlen( detail_ ) + 1];
		strcpy( detail, detail_ );
	} else {
		detail = NULL;
	}
	if ( detail2_ != NULL ) {
		detail2 = new char[strlen( detail2_ ) + 1];
		strcpy( detail2, detail2_ );
	} else {
		detail2 = NULL;
	}
	unknown34 = unknown;
	priority = unknown2;
	subPriority = unknown3;
	database = database_;
	if ( database == NULL && character != NULL ) {
		database = character->GetDatabase();
	}
	if ( database != NULL ) {
		g_kLocalizationManager.Load( database->GetName(), true );
	}
}

/*
================
CharacterAction::~CharacterAction

Finds the character by its two names first, as Play does, which can still
record its ID and load its database; then lets go of the database and frees
the strings. With names, the set is looked up by objectName and the
character in it by setName: the members are named after
UseNameAndSetInsteadOfObject's order, and which of the two is the original's
slip is open.
================
*/
// FUNCTION: STBC 0x0066fcd0
CharacterAction::~CharacterAction( void ) {
	if ( objectName != NULL && setName != NULL ) {
		SetClass *set = g_kSetManager.GetSet( objectName );
		if ( set != NULL ) {
			CharacterClass *character = CharacterClass::GetObject( set, setName );
			if ( character != NULL ) {
				characterID = character->GetObjID();
			}
			if ( database == NULL && character != NULL ) {
				database = character->GetDatabase();
				if ( database != NULL ) {
					g_kLocalizationManager.Load( database->GetName(), true );
				}
			}
		}
	} else {
		// BUG: redundant: looks the character up by its ID and drops the result
		GetTGObjectPtr( characterID );
	}
	if ( database != NULL ) {
		g_kLocalizationManager.Unload( database );
	}
	if ( detail != NULL ) {
		delete detail;
	}
	if ( detail2 != NULL ) {
		delete detail2;
	}
	if ( objectName != NULL ) {
		delete objectName;
	}
	if ( setName != NULL ) {
		delete setName;
	}
}

/*
================
CharacterAction::Completed
================
*/
// FUNCTION: STBC 0x0066fe40
void CharacterAction::Completed( void ) {
	TGAction::Completed();
}

/*
================
CharacterAction::Play

While the game is paused the action plays again a moment later. Otherwise
it finds its character (by name and set when it was made so) and does what
its type says. Most types act at once and complete; the moves, turns,
glances, breaths, animations and said lines complete when the character's
own action does. A spoken line logs its text to the mission log (for the
first line type), loads the voice when it is not loaded (and deletes it
again after), and plays the voice with its subtitle and, for a speaking
character, its lip movements, in the character's queue of lines.
================
*/
// FUNCTION: STBC 0x0066fe50
// DRAFT: calls, branches and tails as retail; the frame sorts the stream (refs 35, size 28: key 1250) after the four pointer slots of key 1500, retail before them (stream at 0x1C in retail, 0x2C here)
void CharacterAction::Play( void ) {
	TGAction::Play();

	if ( UtopiaApp::GetApp()->IsGamePaused() ) {
		TGSequenceEvent *event = new TGSequenceEvent;
		event->SetEventType( ET_PLAY_SEQUENCE_ACTION );
		event->SetDestination( &g_kTGActionManager );
		event->SetObjPtr( this );
		event->SetDelay( 0.0f );
		TGTimer *timer = new TGTimer;
		timer->SetEvent( event );
		timer->SetTimerStart( UtopiaApp::GetApp()->GetGameTime() + 0.001f );
		g_kTimerManager.AddTimer( timer );
		return;
	}

	char path[256];
	char randomFile[256];
	char lipFile[256];
	char errorText[256];
	TGSequence *lipSequence = NULL;
	TGAction *phonemeAction = NULL;
	TGStream stream;
	NiColorA nameColor( 0.9f, 0.9f, 0.0f, 1.0f );
	STMissionLog *missionLog = STMissionLog::GetMissionLog();
	STStylizedWindow *logWindow = (STStylizedWindow *)missionLog->GetFirstChild();

	CharacterClass *character = NULL;
	TGString *speakerName = NULL;
	if ( objectName != NULL && setName != NULL ) {
		SetClass *set = g_kSetManager.GetSet( objectName );
		if ( set != NULL ) {
			character = CharacterClass::GetObject( set, setName );
			if ( character != NULL ) {
				characterID = character->GetObjID();
			}
			if ( database == NULL && character != NULL ) {
				database = character->GetDatabase();
				if ( database != NULL ) {
					g_kLocalizationManager.Load( database->GetName(), true );
				}
			}
		}
	} else {
		character = (CharacterClass *)TGObject::GetTGObjectPtr( characterID );
	}

	if ( type != AT_SPEAK_LINE && type != AT_SPEAK_LINE_NO_FLAP_LIPS ) {
		if ( characterID == 0 ) {
			Completed();
			return;
		}
		if ( character == NULL ) {
			Completed();
			return;
		}
		if ( type == AT_LOOK_AT_ME ) {
			CharacterAction *lookAtMe = new CharacterAction( character, AT_LOOK_AT_ME_NOW, NULL, NULL, 0, NULL, 0, 0 );
			TGSequence *sequence = new TGSequence;
			sequence->AddAction( lookAtMe, NULL, 0.5f );
			sequence->Play();
			Completed();
		} else if ( type == AT_LOOK_AT_ME_NOW ) {
			character->LookAtMe();
			Completed();
		} else if ( type == AT_WATCH_ME ) {
			character->WatchMe();
			Completed();
		} else if ( type == AT_STOP_WATCHING_ME ) {
			character->StopWatchingMe();
			Completed();
		} else if ( type == AT_ENABLE_RANDOM_ANIMATIONS || type == AT_DISABLE_RANDOM_ANIMATIONS ) {
			character->SetRandomAnimationEnabled( type == AT_ENABLE_RANDOM_ANIMATIONS );
			Completed();
		} else if ( type == AT_BECOME_ACTIVE || type == AT_BECOME_INACTIVE ) {
			character->SetActive( type == AT_BECOME_ACTIVE );
			Completed();
		} else if ( type == AT_ENABLE_INITIATIVE || type == AT_DISABLE_INITIATIVE ) {
			character->SetInitiative( type == AT_ENABLE_INITIATIVE );
			Completed();
		} else if ( type == AT_ENABLE_MENU || type == AT_DISABLE_MENU ) {
			character->SetMenuEnabled( type == AT_ENABLE_MENU );
			Completed();
		} else if ( type == AT_MENU_DOWN ) {
			if ( character->IsMenuUp() ) {
				character->MenuDown();
			}
			Completed();
		} else if ( type == AT_MENU_UP ) {
			if ( !character->IsMenuUp() ) {
				character->MenuUp();
			}
			Completed();
		} else if ( type == AT_SET_AUDIO_MODE ) {
			character->SetAudioMode( priority );
			Completed();
		} else if ( type == AT_SET_STATUS ) {
			character->SetStatus( *database->GetString( detail ) );
			Completed();
		} else if ( type == AT_SET_LOCATION ) {
			character->SetLocation( detail );
			Completed();
		} else if ( type == AT_SET_LOCATION_NAME ) {
			character->SetLocationName( detail );
			Completed();
		} else if ( type == AT_TURN_BACK_NOW || type == AT_TURN_BACK ) {
			character->TurnBack();
			Completed();
		} else if ( type == AT_TURN_NOW ) {
			character->TurnTowards( detail );
			Completed();
		} else if ( type == AT_FORCE_BREATHE ) {
			character->ForceBreathe();
			Completed();
		} else {
			// the character's action completes this one
			TGObjPtrEvent *event = new TGObjPtrEvent;
			event->SetEventType( ET_ACTION_COMPLETED );
			event->SetDestination( &g_kTGActionManager );
			event->SetObjPtr( this );
			switch ( type ) {
				case AT_MOVE:
					if ( character->MoveTo( detail, event ) ) {
						return;
					}
					break;
				case AT_TURN:
					if ( character->TurnTowards( detail, event ) ) {
						return;
					}
					break;
				case AT_TURN_BACK:
					if ( character->TurnBack( event ) ) {
						return;
					}
					break;
				case AT_DEFAULT:
				case AT_BREATHE:
					if ( character->Breathe( event ) ) {
						return;
					}
					break;
				case AT_SAY_LINE:
					if ( character->SayLine( database, detail, detail2, unknown34, priority, subPriority, 0, event ) ) {
						return;
					}
					break;
				case AT_SAY_LINE_AFTER_TURN:
					if ( character->SayLine( database, detail, detail2, unknown34, priority, subPriority, 1, event ) ) {
						return;
					}
					break;
				case AT_PLAY_ANIMATION:
					if ( character->PlayAnimation( detail, unknown34 == 0, event ) ) {
						return;
					}
					break;
				case AT_PLAY_ANIMATION_FILE:
					if ( character->PlayAnimationFile( detail, unknown34 == 0, event ) ) {
						return;
					}
					break;
				case AT_GLANCE_AT:
					if ( character->GlanceAt( detail, event ) ) {
						return;
					}
					break;
				case AT_GLANCE_AWAY:
					if ( character->GlanceAway( event ) ) {
						return;
					}
					break;
			}
			delete event;
			Completed();
		}
		return;
	}

	// a spoken line
	if ( database == NULL ) {
		Completed();
		return;
	}
	char *file = database->GetFilename( detail );
	if ( file == NULL ) {
		Completed();
		return;
	}
	if ( CharacterClass::GetVolumeForLineType( priority ) <= 0.0f ) {
		Completed();
		return;
	}

	TGLocalizationDatabase *names = g_kLocalizationManager.Load( "data/TGL/Names.TGL", true );
	if ( character != NULL ) {
		speakerName = names->GetString( character->GetCharacterName() );
	} else if ( detail2 != NULL ) {
		speakerName = names->GetString( detail2 );
	}
	g_kLocalizationManager.Unload( names );

	if ( priority == 0 ) {
		// the line goes to the mission log, after the speaker's name
		TGParagraph *line = new TGParagraph( database->GetString( detail ), logWindow->GetMaximumWidth() - 2.0f * SubtitleWindow::GetWidthMinusTextWidth(),
											 NULL, "", 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP );
		if ( speakerName != NULL ) {
			line->SetCursorPosition( 0 );
			line->InsertChar( SPEAKER_SEPARATOR_CHAR, true );
			TGParagraph *speaker = new TGParagraph( speakerName, logWindow->GetMaximumWidth() - 2.0f * SubtitleWindow::GetWidthMinusTextWidth(),
													NULL, "", 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP );
			TGString separator( ": " );
			speaker->AppendString( &separator, true );
			speaker->SetColor( &nameColor );
			line->InsertChild( 0x7FFFFFFF, speaker, 0.0f, 0.0f, false );
		}
		missionLog->AddLineParagraph( line );
	}

	if ( skipAllLines == true ) {
		Completed();
		return;
	}
	if ( character != NULL && character->GetAudioMode() == CharacterClass::CAM_MUTE ) {
		Completed();
		return;
	}
	if ( database->GetString( detail ) == NULL ) {
		Completed();
		return;
	}

	// the voice, loaded for the line when it is not loaded yet
	bool loadedOnDemand = false;
	if ( g_kSoundManager.GetSound( detail ) == NULL ) {
		long flags = 0;
		if ( g_kConfigMapping.HasValue( "Sound", "StreamVoices" ) && g_kConfigMapping.GetIntValue( "Sound", "StreamVoices" ) != 0 ) {
			flags = LS_STREAMED;
		}
		loadedOnDemand = true;
		Game *game = Game::GetCurrentGame();
		if ( game == NULL ) {
			Completed();
			return;
		}
		Episode *episode = game->GetCurrentEpisode();
		if ( episode == NULL ) {
			Completed();
			return;
		}
		Mission *mission = episode->GetCurrentMission();
		if ( mission == NULL ) {
			Completed();
			return;
		}
		if ( character != NULL ) {
			if ( ( game->GetDatabase() != NULL && strcmp( database->GetName(), game->GetDatabase()->GetName() ) == 0 ) ||
				 ( character->GetDatabase() != NULL && strcmp( database->GetName(), character->GetDatabase()->GetName() ) == 0 ) ) {
				game->LoadSoundInGroup( database->GetFilename( detail ), detail, "LoadedOnDemand", flags );
			}
		}
		if ( g_kSoundManager.GetSound( detail ) == NULL ) {
			if ( episode->GetDatabase() != NULL && strcmp( database->GetName(), episode->GetDatabase()->GetName() ) == 0 ) {
				episode->LoadSound( database->GetFilename( detail ), detail, flags );
			} else {
				mission->LoadSound( database->GetFilename( detail ), detail, flags );
			}
		}
		if ( g_kSoundManager.GetSound( detail ) == NULL ) {
			Completed();
			return;
		}
	}

	// the lips follow the line's .LIP file, else a random one, else random phonemes
	if ( character != NULL && type == AT_SPEAK_LINE ) {
		g_kAnimationManager.LoadAnimation( "data/animations/mouth_flapping.NIF", "__movelips" );
		g_kAnimationManager.LoadAnimation( "data/animations/mouth_close.NIF", "__closelips" );
		strcpy( path, file );
		char *extension = strrchr( path, '.' );
		*extension = '\0';
		sprintf( lipFile, "%s.LIP", path );
		stream.VirtualSlot00( lipFile, "rb" );
		*extension = '.';
		if ( stream.VirtualSlot58() == 0 ) {
			lipSequence = character->GetOverlayManager()->BuildSequenceFromStream( &stream, phonemeCodes, NUM_PHONEME_CODES, 0 );
			stream.Close();
		} else {
			sprintf( randomFile, "data/LipsRandom%d.LIP", rand() % 5 + 1 );
			stream.VirtualSlot00( randomFile, "rb" );
			if ( stream.VirtualSlot58() == 0 ) {
				lipSequence = character->GetOverlayManager()->BuildSequenceFromStream( &stream, NULL, 0, 0 );
				stream.Close();
			} else {
				lipSequence = character->GetOverlayManager()->BuildRandomSequence( 120.0f, randomFile, 2, 4 );
			}
		}
	}

	TGSoundAction *sound = new TGSoundAction( detail, TGSAF_RESTART | TGSAF_ALLOW_SOUND_TO_CONTINUE_PLAYING_WHEN_STOPPED | TGSAF_VOICE, "bridge",
											  CharacterClass::GetVolumeForLineType( priority ) );
	if ( sound == NULL ) {
		Completed();
		return;
	}
	sound->SetSkippable( true );
	sound->SetPriority( 1.0f );
	if ( loadedOnDemand ) {
		TGStringEvent *deleteSound = new TGStringEvent;
		deleteSound->SetEventType( ET_DELETE_SOUND );
		deleteSound->SetString( detail );
		deleteSound->SetDestination( &g_kSoundManager );
		sound->AddCompletedEvent( deleteSound );
	}
	if ( character != NULL && type == AT_SPEAK_LINE ) {
		TGObjPtrEvent *lipsDone = new TGObjPtrEvent;
		lipsDone->SetEventType( ET_ACTION_COMPLETED );
		lipsDone->SetDestination( &g_kTGActionManager );
		lipsDone->SetObjPtr( lipSequence );
		sound->AddCompletedEvent( lipsDone );
	}

	// the subtitle
	TGAction *subtitle;
	if ( database != NULL ) {
		float width = SubtitleWindow::GetBridgeSubtitleWidth();
		if ( TopWindow::GetTopWindow() != NULL ) {
			SubtitleWindow *window = (SubtitleWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_SUBTITLE );
			if ( window != NULL ) {
				width = window->GetSubtitleWidth();
			}
		}
		TGParagraph *line = new TGParagraph( database->GetString( detail ), width - SubtitleWindow::GetWidthMinusTextWidth(),
											 NULL, "", 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP );
		if ( speakerName != NULL ) {
			line->SetCursorPosition( 0 );
			line->InsertChar( SPEAKER_SEPARATOR_CHAR, true );
			TGParagraph *speaker = new TGParagraph( speakerName, width - SubtitleWindow::GetWidthMinusTextWidth(),
													NULL, "", 0.0f, TGPF_READ_ONLY | TGPF_WORD_WRAP );
			TGString separator( ": " );
			speaker->AppendString( &separator, true );
			speaker->SetColor( &nameColor );
			line->InsertChild( 0x7FFFFFFF, speaker, 0.0f, 0.0f, false );
		}
		subtitle = new SubtitleAction( line, 1.25f );
	} else {
		sprintf( errorText, "Error, no database given for %s", detail );
		subtitle = new SubtitleAction( errorText, 1.25f );
	}
	if ( subtitle != NULL ) {
		TGObjPtrEvent *subtitleDone = new TGObjPtrEvent;
		subtitleDone->SetEventType( ET_ACTION_COMPLETED );
		subtitleDone->SetDestination( &g_kTGActionManager );
		subtitleDone->SetObjPtr( subtitle );
		sound->AddCompletedEvent( subtitleDone );
	}

	if ( character != NULL && type == AT_SPEAK_LINE ) {
		TGOverlayManager *overlays = character->GetOverlayManager();
		phonemeAction = new TGPhonemeAction( overlays, overlays->GetPhoneme( 1 ), -1.0f );
	}

	TGSequence *sequence = new TGSequence;
	if ( sequence == NULL ) {
		Completed();
		return;
	}
	sequence->AddAction( sound );
	if ( lipSequence != NULL ) {
		sequence->AddAction( lipSequence );
	}
	if ( subtitle != NULL ) {
		sequence->AddAction( subtitle );
	}
	if ( phonemeAction != NULL ) {
		sequence->AddAction( phonemeAction, sound );
	}
	TGObjPtrEvent *done = new TGObjPtrEvent;
	done->SetEventType( ET_ACTION_COMPLETED );
	done->SetDestination( &g_kTGActionManager );
	done->SetObjPtr( this );
	sequence->AddCompletedEvent( done );
	if ( character != NULL ) {
		character->AddSoundToQueue( sequence, priority, subPriority );
	} else {
		sequence->Play();
	}
}

/*
================
CharacterAction::UseNameAndSetInsteadOfObject

Makes the action find its character by name in the named set rather than through
an object: keeps owned copies of both names, freeing the previous ones. Does nothing
unless both names are given.
================
*/
// FUNCTION: STBC 0x00671010
void CharacterAction::UseNameAndSetInsteadOfObject( char *name, char *set ) {
	if ( name != NULL && set != NULL ) {
		if ( objectName != NULL ) {
			delete objectName;
		}
		if ( setName != NULL ) {
			delete setName;
		}
		objectName = new char[strlen( name ) + 1];
		strcpy( objectName, name );
		setName = new char[strlen( set ) + 1];
		strcpy( setName, set );
	}
}

/*
================
CharacterAction::WriteToStream

Logs what is saved, then writes the character's ID, the type, the name of the
database the lines come from, the two strings, the three numbers and the
character's names.
================
*/
// FUNCTION: STBC 0x006710d0
bool CharacterAction::WriteToStream( TGStream *stream ) {
	char typeName[256];
	char characterName[256];
	char text[256];

	CharacterClass *character = (CharacterClass *)GetTGObjectPtr( characterID );
	if ( character != NULL ) {
		strcpy( characterName, character->GetName() );
	} else {
		strcpy( characterName, "Unknown" );
	}
	switch ( type ) {
		case AT_SET_LOCATION:
			strcpy( typeName, "SetLocation" );
			break;
		case AT_SET_LOCATION_NAME:
			strcpy( typeName, "SetLocationName" );
			break;
		case AT_MOVE:
			strcpy( typeName, "Move" );
			break;
		case AT_TURN:
			strcpy( typeName, "Turn" );
			break;
		case AT_TURN_BACK:
			strcpy( typeName, "TurnBack" );
			break;
		case AT_TURN_BACK_NOW:
			strcpy( typeName, "TurnBackNow" );
			break;
		case AT_DEFAULT:
			strcpy( typeName, "Default" );
			break;
		case AT_BREATHE:
			strcpy( typeName, "Breathe" );
			break;
		case AT_FORCE_BREATHE:
			strcpy( typeName, "Force Breathe" );
			break;
		case AT_SPEAK_LINE:
			strcpy( typeName, "SpeakLine" );
			break;
		case AT_SPEAK_LINE_NO_FLAP_LIPS:
			strcpy( typeName, "SpeakLineNoFlap" );
			break;
		case AT_SAY_LINE:
			strcpy( typeName, "SayLine" );
			break;
		case AT_SAY_LINE_AFTER_TURN:
			strcpy( typeName, "SayLineAfterTurn" );
			break;
		case AT_PLAY_ANIMATION:
			strcpy( typeName, "PlayAnimation" );
			break;
		case AT_PLAY_ANIMATION_FILE:
			strcpy( typeName, "PlayAnimationFile" );
			break;
		case AT_LOOK_AT_ME:
			strcpy( typeName, "LookAtMe" );
			break;
		case AT_LOOK_AT_ME_NOW:
			strcpy( typeName, "LookAtMeNow" );
			break;
		case AT_WATCH_ME:
			strcpy( typeName, "WatchMe" );
			break;
		case AT_STOP_WATCHING_ME:
			strcpy( typeName, "StopWatchingMe!" );
			break;
		case AT_MENU_UP:
			strcpy( typeName, "MenuUp" );
			break;
		case AT_MENU_DOWN:
			strcpy( typeName, "MenuDown" );
			break;
		case AT_SET_AUDIO_MODE:
			strcpy( typeName, "SetAudioMode" );
			break;
		case AT_ENABLE_RANDOM_ANIMATIONS:
			strcpy( typeName, "EnableRandom" );
			break;
		case AT_DISABLE_RANDOM_ANIMATIONS:
			strcpy( typeName, "DisableRandom" );
			break;
		case AT_GLANCE_AT:
			strcpy( typeName, "GlanceAt" );
			break;
		case AT_GLANCE_AWAY:
			strcpy( typeName, "GlanceAway" );
			break;
		case AT_BECOME_ACTIVE:
			strcpy( typeName, "BecomeActive" );
			break;
		case AT_BECOME_INACTIVE:
			strcpy( typeName, "BecomeInactive" );
			break;
		case AT_ENABLE_MENU:
			strcpy( typeName, "EnableMenu" );
			break;
		case AT_DISABLE_MENU:
			strcpy( typeName, "DisableMenu" );
			break;
		case AT_ENABLE_INITIATIVE:
			strcpy( typeName, "EnableInitiative" );
			break;
		case AT_DISABLE_INITIATIVE:
			strcpy( typeName, "DisableInitiative" );
			break;
		case AT_SET_STATUS:
			strcpy( typeName, "SetStatus" );
			break;
	}
	sprintf( text, "Saving CharacterAction %s on %s ID %d with start time %f\n", typeName, characterName, id, startTime );
	Log( text );

	if ( !TGAction::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteID( characterID );
	stream->WriteLong( type );
	if ( database == NULL ) {
		stream->WriteBool( false );
	} else {
		stream->WriteBool( true );
		stream->VirtualSlot11( database->GetName() );
	}
	stream->VirtualSlot11( detail );
	stream->VirtualSlot11( detail2 );
	stream->WriteLong( unknown34 );
	stream->WriteLong( priority );
	stream->WriteLong( subPriority );
	stream->VirtualSlot11( objectName );
	stream->VirtualSlot11( setName );
	return true;
}

/*
================
CharacterAction::ReadFromStream

The inverse of WriteToStream; the database is loaded again by its name. Logs what was loaded.
================
*/
// FUNCTION: STBC 0x006714a0
bool CharacterAction::ReadFromStream( TGStream *stream ) {
	char typeName[256];
	char text[256];

	if ( !TGAction::ReadFromStream( stream ) ) {
		return false;
	}

	database = NULL;
	characterID = stream->ReadID();
	type = stream->ReadLong();
	if ( stream->ReadBool() == true ) {
		char *databaseName = stream->VirtualSlot10( -1 );
		if ( databaseName != NULL ) {
			database = g_kLocalizationManager.Load( databaseName, false );
			delete databaseName;
		}
	}
	detail = stream->VirtualSlot10( -1 );
	detail2 = stream->VirtualSlot10( -1 );
	unknown34 = stream->ReadLong();
	priority = stream->ReadLong();
	subPriority = stream->ReadLong();
	objectName = stream->VirtualSlot10( -1 );
	setName = stream->VirtualSlot10( -1 );
	switch ( type ) {
		case AT_SET_LOCATION:
			strcpy( typeName, "SetLocation" );
			break;
		case AT_SET_LOCATION_NAME:
			strcpy( typeName, "SetLocationName" );
			break;
		case AT_MOVE:
			strcpy( typeName, "Move" );
			break;
		case AT_TURN:
			strcpy( typeName, "Turn" );
			break;
		case AT_TURN_BACK:
			strcpy( typeName, "TurnBack" );
			break;
		case AT_TURN_BACK_NOW:
			strcpy( typeName, "TurnBackNow" );
			break;
		case AT_DEFAULT:
			strcpy( typeName, "Default" );
			break;
		case AT_BREATHE:
			strcpy( typeName, "Breathe" );
			break;
		case AT_FORCE_BREATHE:
			strcpy( typeName, "Force Breathe" );
			break;
		case AT_SPEAK_LINE:
			strcpy( typeName, "SpeakLine" );
			break;
		case AT_SPEAK_LINE_NO_FLAP_LIPS:
			strcpy( typeName, "SpeakLineNoFlap" );
			break;
		case AT_SAY_LINE:
			strcpy( typeName, "SayLine" );
			break;
		case AT_SAY_LINE_AFTER_TURN:
			strcpy( typeName, "SayLineAfterTurn" );
			break;
		case AT_PLAY_ANIMATION:
			strcpy( typeName, "PlayAnimation" );
			break;
		case AT_PLAY_ANIMATION_FILE:
			strcpy( typeName, "PlayAnimationFile" );
			break;
		case AT_LOOK_AT_ME:
			strcpy( typeName, "LookAtMe" );
			break;
		case AT_LOOK_AT_ME_NOW:
			strcpy( typeName, "LookAtMeNow" );
			break;
		case AT_WATCH_ME:
			strcpy( typeName, "WatchMe" );
			break;
		case AT_STOP_WATCHING_ME:
			strcpy( typeName, "StopWatchingMe!" );
			break;
		case AT_MENU_UP:
			strcpy( typeName, "MenuUp" );
			break;
		case AT_MENU_DOWN:
			strcpy( typeName, "MenuDown" );
			break;
		case AT_SET_AUDIO_MODE:
			strcpy( typeName, "SetAudioMode" );
			break;
		case AT_ENABLE_RANDOM_ANIMATIONS:
			strcpy( typeName, "EnableRandom" );
			break;
		case AT_DISABLE_RANDOM_ANIMATIONS:
			strcpy( typeName, "DisableRandom" );
			break;
		case AT_GLANCE_AT:
			strcpy( typeName, "GlanceAt" );
			break;
		case AT_GLANCE_AWAY:
			strcpy( typeName, "GlanceAway" );
			break;
		case AT_BECOME_ACTIVE:
			strcpy( typeName, "BecomeActive" );
			break;
		case AT_BECOME_INACTIVE:
			strcpy( typeName, "BecomeInactive" );
			break;
		case AT_ENABLE_MENU:
			strcpy( typeName, "EnableMenu" );
			break;
		case AT_DISABLE_MENU:
			strcpy( typeName, "DisableMenu" );
			break;
		case AT_ENABLE_INITIATIVE:
			strcpy( typeName, "EnableInitiative" );
			break;
		case AT_DISABLE_INITIATIVE:
			strcpy( typeName, "DisableInitiative" );
			break;
		case AT_SET_STATUS:
			strcpy( typeName, "SetStatus" );
			break;
	}
	sprintf( text, "Loaded CharacterAction %s on ID %d with start time %f\n", typeName, id, startTime );
	Log( text );
	return true;
}
