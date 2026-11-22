/*
===============================================================================

	Episode.cpp

===============================================================================
*/

#include <stdio.h>
#include <string.h>

#include "Episode.h"
#include "CharacterClass.h"
#include "Game.h"
#include "Mission.h"
#include "TGMovieManager.h"
#include "UtopiaModule.h"
#include "TGBufferStream.h"
#include "TGNetwork.h"
#include "TGMessage.h"
#include "MultiplayerGame.h"
#include "PyEmbed.h"
#include "STButton.h"
#include "STMenu.h"
#include "SetManager.h"
#include "STMissionLog.h"
#include "STTopLevelMenu.h"
#include "TacticalControlWindow.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGString.h"
#include "TGStream.h"
#include "TGStringEvent.h"
#include "TopWindow.h"
#include "UtopiaApp.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00405640 _$E537
// FUNCTION: STBC 0x00405660 _$E536

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00403fd0 _$E320
// FUNCTION: STBC 0x00403fe0 _$E317
// FUNCTION: STBC 0x00403ff0 _$E319
// FUNCTION: STBC 0x00404000 _$E318
// FUNCTION: STBC 0x00404010 _$E326
// FUNCTION: STBC 0x00404020 _$E323
// FUNCTION: STBC 0x00404030 _$E325
// FUNCTION: STBC 0x00404040 _$E324

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004046f0 _$E57
// FUNCTION: STBC 0x00404700 _$E56
// FUNCTION: STBC 0x00404710 _$E233
// FUNCTION: STBC 0x00404720 _$E232
// FUNCTION: STBC 0x00404dc0 _$E68
// FUNCTION: STBC 0x00404dd0 _$E67
// FUNCTION: STBC 0x00404de0 _$E72
// FUNCTION: STBC 0x00404df0 _$E71
// FUNCTION: STBC 0x00404e00 _$E76
// FUNCTION: STBC 0x00404e10 _$E75
// FUNCTION: STBC 0x00404e20 _$E80
// FUNCTION: STBC 0x00404e30 _$E79
// FUNCTION: STBC 0x00404e40 _$E197
// FUNCTION: STBC 0x00404e50 _$E196
// FUNCTION: STBC 0x00404e60 _$E139
// FUNCTION: STBC 0x00404e70 _$E138
// FUNCTION: STBC 0x00404e80 _$E152
// FUNCTION: STBC 0x00404e90 _$E151
// FUNCTION: STBC 0x00404ea0 _$E202
// FUNCTION: STBC 0x00404eb0 _$E201
// FUNCTION: STBC 0x00404ec0 _$E238
// FUNCTION: STBC 0x00404ed0 _$E237
// FUNCTION: STBC 0x00404ee0 _$E206
// FUNCTION: STBC 0x00404ef0 _$E205
// FUNCTION: STBC 0x00404f00 _$E242
// FUNCTION: STBC 0x00404f10 _$E241
// FUNCTION: STBC 0x00404f20 _$E249
// FUNCTION: STBC 0x00404f30 _$E248
// FUNCTION: STBC 0x00404f40 _$E348
// FUNCTION: STBC 0x00404f50 _$E347
// FUNCTION: STBC 0x00404f60 _$E352
// FUNCTION: STBC 0x00404f70 _$E351
// FUNCTION: STBC 0x00404f80 _$E97
// FUNCTION: STBC 0x00404f90 _$E96
// FUNCTION: STBC 0x00404fa0 _$E334
// FUNCTION: STBC 0x00404fb0 _$E333
// FUNCTION: STBC 0x00404fc0 _$E341
// FUNCTION: STBC 0x00404fd0 _$E340
// FUNCTION: STBC 0x00404fe0 _$E364
// FUNCTION: STBC 0x00404ff0 _$E363
// FUNCTION: STBC 0x00405000 _$E368
// FUNCTION: STBC 0x00405010 _$E367
// FUNCTION: STBC 0x00405020 _$E408
// FUNCTION: STBC 0x00405030 _$E407
// FUNCTION: STBC 0x00405040 _$E414
// FUNCTION: STBC 0x00405050 _$E413
// FUNCTION: STBC 0x00405060 _$E356
// FUNCTION: STBC 0x00405070 _$E355
// FUNCTION: STBC 0x00405080 _$E360
// FUNCTION: STBC 0x00405090 _$E359
// FUNCTION: STBC 0x004050a0 _$E372
// FUNCTION: STBC 0x004050b0 _$E371
// FUNCTION: STBC 0x004050c0 _$E376
// FUNCTION: STBC 0x004050d0 _$E375
// FUNCTION: STBC 0x004050e0 _$E380
// FUNCTION: STBC 0x004050f0 _$E379
// FUNCTION: STBC 0x00405100 _$E384
// FUNCTION: STBC 0x00405110 _$E383
// FUNCTION: STBC 0x00405120 _$E388
// FUNCTION: STBC 0x00405130 _$E387
// FUNCTION: STBC 0x00405140 _$E392
// FUNCTION: STBC 0x00405150 _$E391
// FUNCTION: STBC 0x00405160 _$E396
// FUNCTION: STBC 0x00405170 _$E395
// FUNCTION: STBC 0x00405180 _$E400
// FUNCTION: STBC 0x00405190 _$E399
// FUNCTION: STBC 0x004051a0 _$E102
// FUNCTION: STBC 0x004051b0 _$E101
// FUNCTION: STBC 0x004051c0 _$E404
// FUNCTION: STBC 0x004051d0 _$E403
// FUNCTION: STBC 0x004051e0 _$E107
// FUNCTION: STBC 0x004051f0 _$E106
// FUNCTION: STBC 0x00405200 _$E418
// FUNCTION: STBC 0x00405210 _$E417
// FUNCTION: STBC 0x00405220 _$E422
// FUNCTION: STBC 0x00405230 _$E421
// FUNCTION: STBC 0x00405240 _$E426
// FUNCTION: STBC 0x00405250 _$E425
// FUNCTION: STBC 0x00405260 _$E430
// FUNCTION: STBC 0x00405270 _$E429
// FUNCTION: STBC 0x00405280 _$E116
// FUNCTION: STBC 0x00405290 _$E115
// FUNCTION: STBC 0x004052a0 _$E434
// FUNCTION: STBC 0x004052b0 _$E433
// FUNCTION: STBC 0x004052c0 _$E438
// FUNCTION: STBC 0x004052d0 _$E437
// FUNCTION: STBC 0x004052e0 _$E442
// FUNCTION: STBC 0x004052f0 _$E441
// FUNCTION: STBC 0x00405300 _$E122
// FUNCTION: STBC 0x00405310 _$E121
// FUNCTION: STBC 0x00405320 _$E127
// FUNCTION: STBC 0x00405330 _$E126
// FUNCTION: STBC 0x00405340 _$E157
// FUNCTION: STBC 0x00405350 _$E156
// FUNCTION: STBC 0x00405360 _$E446
// FUNCTION: STBC 0x00405370 _$E445
// FUNCTION: STBC 0x00405380 _$E162
// FUNCTION: STBC 0x00405390 _$E161
// FUNCTION: STBC 0x004053a0 _$E450
// FUNCTION: STBC 0x004053b0 _$E449
// FUNCTION: STBC 0x004053c0 _$E454
// FUNCTION: STBC 0x004053d0 _$E453
// FUNCTION: STBC 0x004053e0 _$E167
// FUNCTION: STBC 0x004053f0 _$E166
// FUNCTION: STBC 0x00405400 _$E458
// FUNCTION: STBC 0x00405410 _$E457
// FUNCTION: STBC 0x00405420 _$E132
// FUNCTION: STBC 0x00405430 _$E131
// FUNCTION: STBC 0x00405440 _$E462
// FUNCTION: STBC 0x00405450 _$E461
// FUNCTION: STBC 0x00405460 _$E466
// FUNCTION: STBC 0x00405470 _$E465
// FUNCTION: STBC 0x00405480 _$E172
// FUNCTION: STBC 0x00405490 _$E171
// FUNCTION: STBC 0x004054a0 _$E177
// FUNCTION: STBC 0x004054b0 _$E176
// FUNCTION: STBC 0x004054c0 _$E470
// FUNCTION: STBC 0x004054d0 _$E469
// FUNCTION: STBC 0x004054e0 _$E474
// FUNCTION: STBC 0x004054f0 _$E473
// FUNCTION: STBC 0x00405500 _$E478
// FUNCTION: STBC 0x00405510 _$E477
// FUNCTION: STBC 0x00405520 _$E482
// FUNCTION: STBC 0x00405530 _$E481
// FUNCTION: STBC 0x00405540 _$E486
// FUNCTION: STBC 0x00405550 _$E485
// FUNCTION: STBC 0x00405560 _$E490
// FUNCTION: STBC 0x00405570 _$E489
// FUNCTION: STBC 0x00405580 _$E494
// FUNCTION: STBC 0x00405590 _$E493
// FUNCTION: STBC 0x00405600 _$E498
// FUNCTION: STBC 0x00405610 _$E497
// FUNCTION: STBC 0x00405620 _$E182
// FUNCTION: STBC 0x00405630 _$E181
// FUNCTION: STBC 0x00405670 _$E502
// FUNCTION: STBC 0x00405680 _$E501
// FUNCTION: STBC 0x00405690 _$E187
// FUNCTION: STBC 0x004056a0 _$E186
// FUNCTION: STBC 0x004056b0 _$E192
// FUNCTION: STBC 0x004056c0 _$E191
// FUNCTION: STBC 0x004057c0 _$E506
// FUNCTION: STBC 0x004057d0 _$E505
// FUNCTION: STBC 0x004057e0 _$E511
// FUNCTION: STBC 0x004057f0 _$E510
// FUNCTION: STBC 0x00405800 _$E515
// FUNCTION: STBC 0x00405810 _$E514
// FUNCTION: STBC 0x00405820 _$E519
// FUNCTION: STBC 0x00405830 _$E518
// FUNCTION: STBC 0x00405840 _$E523
// FUNCTION: STBC 0x00405850 _$E522

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00404050 _$E529
// FUNCTION: STBC 0x00404060 _$E525
// FUNCTION: STBC 0x00404070 _$E528
// FUNCTION: STBC 0x00404080 _$E526
// DATA: STBC 0x0097e060
TGEventHandlerTable Episode::classHandlers;

/*
================
Episode::Episode
================
*/
// FUNCTION: STBC 0x004040a0
Episode::Episode( char *script ) : ScriptObject( script, 0x808001 ) {
	currentMission = NULL;
	missionLoadedEvent = NULL;
	toolTipPickTime = 0.0f;
}

/*
================
Episode::GetObjType
================
*/
// FUNCTION: STBC 0x00404100
int Episode::GetObjType( void ) {
	return EPISODE_TYPE;
}

/*
================
Episode::IsTypeOf
================
*/
// FUNCTION: STBC 0x00404110
bool Episode::IsTypeOf( int type ) {
	if ( type == EPISODE_TYPE ) {
		return true;
	}
	return ScriptObject::IsTypeOf( type );
}

/*
================
Episode::GetClassName
================
*/
// FUNCTION: STBC 0x00404160
char *Episode::GetClassName( void ) {
	return "Episode";
}

/*
================
Episode::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00404170
char *Episode::GetSwigTypeName( void ) {
	return "_p_Episode";
}

/*
================
Episode::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00404180
char *Episode::GetPythonClassName( void ) {
	return "EpisodePtr";
}

/*
================
Episode::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00404190
TGEventHandlerTable *Episode::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
Episode::OnIdle

Passes the idle time to the mission. Twice a second, with tool tips on,
picks the bridge characters under the middle of the view and gives the
one nearest the line of sight its tool tip; the script drops the tool
tips while that character's status box is hidden.
================
*/
// FUNCTION: STBC 0x004041e0
void Episode::OnIdle( void ) {
	ScriptObject::OnIdle();
	if ( currentMission == NULL ) {
		return;
	}
	currentMission->OnIdle();

	if ( !CharacterClass::AreToolTipsEnabled() ) {
		return;
	}
	if ( toolTipPickTime + 0.5f >= UtopiaApp::GetApp()->GetGameTime() ) {
		return;
	}
	toolTipPickTime = UtopiaApp::GetApp()->GetGameTime();

	SetClass *bridge = g_kSetManager.GetSet( "bridge" );
	if ( bridge == NULL ) {
		return;
	}

	float bestDot = -1.0f;
	bool statusShown = false;
	CharacterClass *nearest = NULL;
	bridge->StartPick( 0.5f, 0.5f, true );
	while ( true ) {
		ObjectClass *picked = bridge->GetPickedObject();
		if ( picked == NULL ) {
			break;
		}
		CharacterClass *character = CharacterClass::Cast( picked );
		if ( character == NULL ) {
			continue;
		}
		float dot = bridge->GetDotProductToObject( character );
		if ( dot > bestDot ) {
			bestDot = dot;
			nearest = character;
			TGPane *statusBox = character->GetStatusBox();
			if ( statusBox != NULL && statusBox->IsVisible() ) {
				statusShown = true;
			} else {
				statusShown = false;
			}
		}
	}

	if ( !statusShown ) {
		PyEmbed::CallFunction( "BridgeHandlers", "DropCharacterToolTips", NULL, NULL, NULL, NULL );
	}
	if ( nearest != NULL && STTopLevelMenu::GetOpenMenu() == NULL && TopWindow::GetTopWindow()->IsMouseInputAllowed() ) {
		nearest->ToolTip();
	}
}

/*
================
Episode::Terminate

Ends the mission and the episode's timers, then the episode.
================
*/
// FUNCTION: STBC 0x00404330
void Episode::Terminate( TGEvent *event ) {
	if ( currentMission != NULL ) {
		currentMission->Terminate( NULL );
		currentMission = NULL;
	}
	if ( STMissionLog::GetMissionLog() != NULL ) {
		STMissionLog::GetMissionLog()->ClearLines();
	}
	PyEmbed::CallFunction( "MissionLib", "DeleteAllEpisodeTimers", NULL, NULL, NULL, NULL );
	ScriptObject::Terminate( event );
}

/*
================
Episode::LoadMission

Starts the mission of the name, as Game::LoadEpisode starts an episode: a
mission of the same name that has loaded only has the event sent, a
different one is terminated with an ET_LOAD_MISSION event, whose handler
loads the named mission once the old one is gone, and one that has not
loaded yet is initialized, after which a client tells the host that it is
ready.
================
*/
// FUNCTION: STBC 0x00404390
void Episode::LoadMission( char *name, TGEvent *doneEvent ) {
	Mission *mission = currentMission;
	if ( mission != NULL ) {
		if ( mission->AreAssetsLoaded() == true ) {
			if ( strcmp( mission->GetScript(), name ) == 0 ) {
				if ( doneEvent != NULL ) {
					g_kEventManager.AddEvent( doneEvent );
				}
				return;
			}
			g_kMovieManager.PlayForcedMovieFrame();
			TGStringEvent *event = new TGStringEvent;
			event->SetDestination( this );
			event->SetEventType( ET_LOAD_MISSION );
			event->SetString( name );
			mission->Terminate( event );
			currentMission = NULL;
			missionLoadedEvent = doneEvent;
			if ( doneEvent != NULL ) {
				doneEvent->IncRefCount();
			}
			return;
		}
		g_kMovieManager.PlayForcedMovieFrame();
		mission->SetAssetsLoaded( true );
		mission->Initialize( missionLoadedEvent );
		g_kMovieManager.PlayForcedMovieFrame();
		if ( missionLoadedEvent != NULL ) {
			missionLoadedEvent->DecRefCount();
			missionLoadedEvent = NULL;
		}
		if ( g_kUtopiaModule.IsMultiplayer() && !g_kUtopiaModule.IsHost() ) {
			TGNetwork *network = g_kUtopiaModule.GetNetwork();
			if ( network != NULL ) {
				TGBufferStream stream;
				char buffer[MAX_MESSAGE_SIZE];
				stream.VirtualSlot60( buffer, 0x100 );
				stream.WriteChar( CLIENT_READY_MESSAGE );
				stream.WriteBool( g_kUtopiaModule.IsModemConnection() );
				TGMessage *message = new TGMessage;
				message->SetData( buffer, stream.GetPos() );
				message->SetGuaranteed( true );
				network->SendTGMessage( network->GetHostID(), message, 0 );
			}
		}
		return;
	}

	g_kMovieManager.PlayForcedMovieFrame();
	mission = new Mission( name );
	if ( mission == NULL ) {
		return;
	}
	currentMission = mission;
	if ( missionLoadedEvent == NULL ) {
		missionLoadedEvent = doneEvent;
		if ( doneEvent != NULL ) {
			doneEvent->IncRefCount();
		}
	}
	g_kMovieManager.PlayForcedMovieFrame();
	mission->PreLoadAssets();
	TGStringEvent *event = new TGStringEvent;
	event->SetDestination( this );
	event->SetEventType( ET_LOAD_MISSION );
	event->SetString( name );
	Game::GetCurrentGame()->SetPreLoadDoneEvent( event );
	g_kMovieManager.PlayForcedMovieFrame();
}

/*
================
Episode::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00404730
void Episode::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Episode::LoadMissionHandler, "Episode::LoadMissionHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&Episode::ReportGoalInfoHandler, "Episode::ReportGoalInfoHandler" );
}

/*
================
Episode::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00404760
void Episode::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_LOAD_MISSION, "Episode::LoadMissionHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_REPORT_GOAL_INFO, "Episode::ReportGoalInfoHandler" );
}

/*
================
Episode::AddPersistentModule

The mission keeps the module too.
================
*/
// FUNCTION: STBC 0x00404790
void Episode::AddPersistentModule( char *module ) {
	ScriptObject::AddPersistentModule( module );
	if ( currentMission != NULL ) {
		currentMission->AddPersistentModule( module );
	}
}

/*
================
Episode::LoadMissionHandler

Loads the mission the string event names.
================
*/
// FUNCTION: STBC 0x004047b0 ?LoadMissionHandler@Episode@@QAEXPAVTGEvent@@@Z
void Episode::LoadMissionHandler( TGEvent *event ) {
	char *name = static_cast<TGStringEvent *>( event )->GetCString();
	LoadMission( name, NULL );
	delete name;
}

/*
================
Episode::GetNextEventType

A new event type of the current game's episode; 0 without one.
================
*/
// FUNCTION: STBC 0x004047e0
int Episode::GetNextEventType( void ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		Episode *episode = game->GetCurrentEpisode();
		if ( episode != NULL ) {
			return episode->GetInstanceNextEventType();
		}
	}
	return 0;
}

/*
================
Episode::RegisterGoal

Adds a button for the goal to the Objectives menu of the Commander's
menu, if it has none yet. The button sends the goal report event, with
the name of the goal's audio, to the episode.
================
*/
// FUNCTION: STBC 0x00404800
void Episode::RegisterGoal( char *goal ) {
	if ( goal == NULL ) {
		return;
	}
	TGLocalizationDatabase *menuDatabase = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
	TacticalControlWindow *window = TacticalControlWindow::GetTacticalControlWindow();
	char *commanderName = menuDatabase->GetString( "Commander" )->Unknown6F56F0();
	TGString commanderMenuName( commanderName );
	if ( commanderName != NULL ) {
		delete commanderName;
	}
	STTopLevelMenu *commanderMenu = window->FindMenu( &commanderMenuName );
	if ( commanderMenu == NULL ) {
		g_kLocalizationManager.Unload( menuDatabase );
		return;
	}
	STMenu *objectives = commanderMenu->GetSubmenuW( menuDatabase->GetString( "Objectives" ) );
	g_kLocalizationManager.Unload( menuDatabase );
	if ( objectives->GetButtonW( database->GetString( goal ) ) == NULL ) {
		TGStringEvent *event = new TGStringEvent;
		event->SetEventType( ET_REPORT_GOAL_INFO );
		event->SetDestination( this );
		char *audioName = new char[strlen( goal ) + strlen( "Audio" ) + 1];
		strcpy( audioName, goal );
		strcat( audioName, "Audio" );
		event->SetString( audioName );
		delete audioName;
		TGString *goalName = database->GetString( goal );
		objectives->InsertChild( 0x7FFFFFFF, new STButton( goalName, event, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 ) );
	}
}

/*
================
Episode::RemoveGoal

Takes the goal's button out of the Objectives menu of the Commander's menu.
================
*/
// FUNCTION: STBC 0x00404a60
void Episode::RemoveGoal( char *goal ) {
	if ( goal == NULL ) {
		return;
	}
	TGLocalizationDatabase *menuDatabase = g_kLocalizationManager.Load( "data/TGL/Bridge Menus.tgl", true );
	TacticalControlWindow *window = TacticalControlWindow::GetTacticalControlWindow();
	char *commanderName = menuDatabase->GetString( "Commander" )->Unknown6F56F0();
	TGString commanderMenuName( commanderName );
	if ( commanderName != NULL ) {
		delete commanderName;
	}
	STTopLevelMenu *commanderMenu = window->FindMenu( &commanderMenuName );
	if ( commanderMenu == NULL ) {
		g_kLocalizationManager.Unload( menuDatabase );
		return;
	}
	STMenu *objectives = commanderMenu->GetSubmenuW( menuDatabase->GetString( "Objectives" ) );
	g_kLocalizationManager.Unload( menuDatabase );
	objectives->RemoveItemW( database->GetString( goal ) );
}

/*
================
Episode::ReportGoalInfoHandler

The first officer on the bridge speaks the goal line the string event carries.
================
*/
// FUNCTION: STBC 0x00404b60 ?ReportGoalInfoHandler@Episode@@QAEXPAVTGEvent@@@Z
void Episode::ReportGoalInfoHandler( TGEvent *event ) {
	SetClass *bridge = g_kSetManager.GetSet( "bridge" );
	CharacterClass *firstOfficer = CharacterClass::GetObject( bridge, "XO" );
	char *line = static_cast<TGStringEvent *>( event )->GetCString();
	firstOfficer->SpeakLine( database, line, 1, 0, NULL );
	delete line;
}

/*
================
Episode::WriteToStream
================
*/
// FUNCTION: STBC 0x00404c00
bool Episode::WriteToStream( TGStream *stream ) {
	char text[256];

	if ( !ScriptObject::WriteToStream( stream ) ) {
		return false;
	}

	long eventID = GetSavedID( missionLoadedEvent );
	char *className = GetClassName();
	sprintf( text, "Saving %s with init event ID %d\n", className, eventID );
	Log( text );

	stream->WriteID( GetSavedID( missionLoadedEvent ) );
	stream->WriteFloat( toolTipPickTime );
	stream->WriteBool( currentMission != NULL );
	if ( currentMission != NULL && !currentMission->WriteToStream( stream ) ) {
		return false;
	}
	return true;
}

/*
================
Episode::ReadFromStream
================
*/
// FUNCTION: STBC 0x00404cc0
bool Episode::ReadFromStream( TGStream *stream ) {
	char text[256];

	if ( !ScriptObject::ReadFromStream( stream ) ) {
		return false;
	}

	missionLoadedEvent = (TGEvent *)stream->ReadID();
	toolTipPickTime = stream->ReadFloat();
	char *className = GetClassName();
	long eventID = (long)missionLoadedEvent;
	sprintf( text, "Loaded %s with init event ID %d\n", className, eventID );
	Log( text );

	currentMission = NULL;
	if ( stream->ReadBool() == true ) {
		currentMission = (Mission *)LoadObject( stream, 0 );
	}
	return true;
}

/*
================
Episode::FixUpPointers
================
*/
// FUNCTION: STBC 0x00404d60
bool Episode::FixUpPointers( void ) {
	if ( !ScriptObject::FixUpPointers() ) {
		return false;
	}
	if ( missionLoadedEvent != NULL && TGEvent::GetTGEventPtr( (long)missionLoadedEvent ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
Episode::PostFixUp
================
*/
// FUNCTION: STBC 0x00404d90
void Episode::PostFixUp( void ) {
	ScriptObject::PostFixUp();
	missionLoadedEvent = TGEvent::GetSavedEvent( (long)missionLoadedEvent );
}
