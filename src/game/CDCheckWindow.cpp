/*
===============================================================================

	CDCheckWindow.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "CDCheckWindow.h"
#include "Game.h"
#include "ModalDialogWindow.h"
#include "OptionsWindow.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TopWindow.h"
#include "UtopiaApp.h"
#include "UtopiaModule.h"
#include "TGSystemWrapperClass.h"
#include "TGString.h"
#include "DiscCheck.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004fd1d0 _$E467
// FUNCTION: STBC 0x004fd1f0 _$E466

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004fcb70 _$E32
// FUNCTION: STBC 0x004fcb80 _$E29
// FUNCTION: STBC 0x004fcb90 _$E31
// FUNCTION: STBC 0x004fcba0 _$E30
// FUNCTION: STBC 0x004fcbb0 _$E38
// FUNCTION: STBC 0x004fcbc0 _$E35
// FUNCTION: STBC 0x004fcbd0 _$E37
// FUNCTION: STBC 0x004fcbe0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004fcbf0 _$E57
// FUNCTION: STBC 0x004fcc00 _$E56
// FUNCTION: STBC 0x004fcc10 _$E61
// FUNCTION: STBC 0x004fcc20 _$E60
// FUNCTION: STBC 0x004fcc30 _$E72
// FUNCTION: STBC 0x004fcc40 _$E71
// FUNCTION: STBC 0x004fcc50 _$E76
// FUNCTION: STBC 0x004fcc60 _$E75
// FUNCTION: STBC 0x004fcc70 _$E80
// FUNCTION: STBC 0x004fcc80 _$E79
// FUNCTION: STBC 0x004fcc90 _$E84
// FUNCTION: STBC 0x004fcca0 _$E83
// FUNCTION: STBC 0x004fccb0 _$E208
// FUNCTION: STBC 0x004fccc0 _$E207
// FUNCTION: STBC 0x004fccd0 _$E150
// FUNCTION: STBC 0x004fcce0 _$E149
// FUNCTION: STBC 0x004fccf0 _$E163
// FUNCTION: STBC 0x004fcd00 _$E162
// FUNCTION: STBC 0x004fcd10 _$E213
// FUNCTION: STBC 0x004fcd20 _$E212
// FUNCTION: STBC 0x004fcd30 _$E218
// FUNCTION: STBC 0x004fcd40 _$E217
// FUNCTION: STBC 0x004fcd50 _$E222
// FUNCTION: STBC 0x004fcd60 _$E221
// FUNCTION: STBC 0x004fcd70 _$E226
// FUNCTION: STBC 0x004fcd80 _$E225
// FUNCTION: STBC 0x004fcd90 _$E233
// FUNCTION: STBC 0x004fcda0 _$E232
// FUNCTION: STBC 0x004fcdb0 _$E286
// FUNCTION: STBC 0x004fcdc0 _$E285
// FUNCTION: STBC 0x004fcdd0 _$E290
// FUNCTION: STBC 0x004fcde0 _$E289
// FUNCTION: STBC 0x004fcdf0 _$E108
// FUNCTION: STBC 0x004fce00 _$E107
// FUNCTION: STBC 0x004fce10 _$E272
// FUNCTION: STBC 0x004fce20 _$E271
// FUNCTION: STBC 0x004fce30 _$E279
// FUNCTION: STBC 0x004fce40 _$E278
// FUNCTION: STBC 0x004fce50 _$E302
// FUNCTION: STBC 0x004fce60 _$E301
// FUNCTION: STBC 0x004fce70 _$E306
// FUNCTION: STBC 0x004fce80 _$E305
// FUNCTION: STBC 0x004fce90 _$E346
// FUNCTION: STBC 0x004fcea0 _$E345
// FUNCTION: STBC 0x004fceb0 _$E352
// FUNCTION: STBC 0x004fcec0 _$E351
// FUNCTION: STBC 0x004fced0 _$E294
// FUNCTION: STBC 0x004fcee0 _$E293
// FUNCTION: STBC 0x004fcef0 _$E298
// FUNCTION: STBC 0x004fcf00 _$E297
// FUNCTION: STBC 0x004fcf10 _$E310
// FUNCTION: STBC 0x004fcf20 _$E309
// FUNCTION: STBC 0x004fcf30 _$E314
// FUNCTION: STBC 0x004fcf40 _$E313
// FUNCTION: STBC 0x004fcf50 _$E318
// FUNCTION: STBC 0x004fcf60 _$E317
// FUNCTION: STBC 0x004fcf70 _$E322
// FUNCTION: STBC 0x004fcf80 _$E321
// FUNCTION: STBC 0x004fcf90 _$E326
// FUNCTION: STBC 0x004fcfa0 _$E325
// FUNCTION: STBC 0x004fcfb0 _$E330
// FUNCTION: STBC 0x004fcfc0 _$E329
// FUNCTION: STBC 0x004fcfd0 _$E334
// FUNCTION: STBC 0x004fcfe0 _$E333
// FUNCTION: STBC 0x004fcff0 _$E338
// FUNCTION: STBC 0x004fd000 _$E337
// FUNCTION: STBC 0x004fd010 _$E113
// FUNCTION: STBC 0x004fd020 _$E112
// FUNCTION: STBC 0x004fd030 _$E342
// FUNCTION: STBC 0x004fd040 _$E341
// FUNCTION: STBC 0x004fd050 _$E118
// FUNCTION: STBC 0x004fd060 _$E117
// FUNCTION: STBC 0x004fd070 _$E356
// FUNCTION: STBC 0x004fd080 _$E355
// FUNCTION: STBC 0x004fd090 _$E360
// FUNCTION: STBC 0x004fd0a0 _$E359
// FUNCTION: STBC 0x004fd0b0 _$E364
// FUNCTION: STBC 0x004fd0c0 _$E363
// FUNCTION: STBC 0x004fd0d0 _$E368
// FUNCTION: STBC 0x004fd0e0 _$E367
// FUNCTION: STBC 0x004fd0f0 _$E127
// FUNCTION: STBC 0x004fd100 _$E126
// FUNCTION: STBC 0x004fd110 _$E372
// FUNCTION: STBC 0x004fd120 _$E371
// FUNCTION: STBC 0x004fd130 _$E376
// FUNCTION: STBC 0x004fd140 _$E375
// FUNCTION: STBC 0x004fd150 _$E380
// FUNCTION: STBC 0x004fd160 _$E379
// FUNCTION: STBC 0x004fd170 _$E133
// FUNCTION: STBC 0x004fd180 _$E132
// FUNCTION: STBC 0x004fd190 _$E138
// FUNCTION: STBC 0x004fd1a0 _$E137
// FUNCTION: STBC 0x004fd1b0 _$E168
// FUNCTION: STBC 0x004fd1c0 _$E167
// FUNCTION: STBC 0x004fd200 _$E384
// FUNCTION: STBC 0x004fd210 _$E383
// FUNCTION: STBC 0x004fd220 _$E173
// FUNCTION: STBC 0x004fd230 _$E172
// FUNCTION: STBC 0x004fd240 _$E388
// FUNCTION: STBC 0x004fd250 _$E387
// FUNCTION: STBC 0x004fd260 _$E392
// FUNCTION: STBC 0x004fd270 _$E391
// FUNCTION: STBC 0x004fd280 _$E178
// FUNCTION: STBC 0x004fd290 _$E177
// FUNCTION: STBC 0x004fd2a0 _$E396
// FUNCTION: STBC 0x004fd2b0 _$E395
// FUNCTION: STBC 0x004fd2c0 _$E143
// FUNCTION: STBC 0x004fd2d0 _$E142
// FUNCTION: STBC 0x004fd2e0 _$E400
// FUNCTION: STBC 0x004fd2f0 _$E399
// FUNCTION: STBC 0x004fd300 _$E404
// FUNCTION: STBC 0x004fd310 _$E403
// FUNCTION: STBC 0x004fd320 _$E183
// FUNCTION: STBC 0x004fd330 _$E182
// FUNCTION: STBC 0x004fd340 _$E188
// FUNCTION: STBC 0x004fd350 _$E187
// FUNCTION: STBC 0x004fd360 _$E408
// FUNCTION: STBC 0x004fd370 _$E407
// FUNCTION: STBC 0x004fd380 _$E412
// FUNCTION: STBC 0x004fd390 _$E411
// FUNCTION: STBC 0x004fd3a0 _$E416
// FUNCTION: STBC 0x004fd3b0 _$E415
// FUNCTION: STBC 0x004fd3c0 _$E420
// FUNCTION: STBC 0x004fd3d0 _$E419
// FUNCTION: STBC 0x004fd3e0 _$E424
// FUNCTION: STBC 0x004fd3f0 _$E423
// FUNCTION: STBC 0x004fd400 _$E428
// FUNCTION: STBC 0x004fd410 _$E427
// FUNCTION: STBC 0x004fd420 _$E432
// FUNCTION: STBC 0x004fd430 _$E431
// FUNCTION: STBC 0x004fd440 _$E436
// FUNCTION: STBC 0x004fd450 _$E435
// FUNCTION: STBC 0x004fd460 _$E193
// FUNCTION: STBC 0x004fd470 _$E192
// FUNCTION: STBC 0x004fd480 _$E440
// FUNCTION: STBC 0x004fd490 _$E439
// FUNCTION: STBC 0x004fd4a0 _$E198
// FUNCTION: STBC 0x004fd4b0 _$E197
// FUNCTION: STBC 0x004fd4c0 _$E203
// FUNCTION: STBC 0x004fd4d0 _$E202
// FUNCTION: STBC 0x004fd4e0 _$E444
// FUNCTION: STBC 0x004fd4f0 _$E443
// FUNCTION: STBC 0x004fd500 _$E449
// FUNCTION: STBC 0x004fd510 _$E448
// FUNCTION: STBC 0x004fd520 _$E453
// FUNCTION: STBC 0x004fd530 _$E452

// The handler table's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004fc430 _$E459
// FUNCTION: STBC 0x004fc440 _$E455
// FUNCTION: STBC 0x004fc450 _$E458
// FUNCTION: STBC 0x004fc460 _$E456
// DATA: STBC 0x00986d30
TGEventHandlerTable CDCheckWindow::classHandlers;

/*
================
CDCheckWindow::CDCheckWindow

Hidden until a check fails.
================
*/
// FUNCTION: STBC 0x004fc480 ??0CDCheckWindow@@QAE@XZ
CDCheckWindow::CDCheckWindow( void ) : MainWindow( MWT_CD_CHECK, 0.55f, 0.25f ) {
	SetNotVisible();
	unknown50 = -1.0f;
	startingGame = false;
	loadFileName = NULL;
}

/*
================
CDCheckWindow::GetObjType
================
*/
// FUNCTION: STBC 0x004fc4f0
int CDCheckWindow::GetObjType( void ) {
	return CDCHECKWINDOW_TYPE;
}

/*
================
CDCheckWindow::IsTypeOf
================
*/
// FUNCTION: STBC 0x004fc500
bool CDCheckWindow::IsTypeOf( int type ) {
	if ( type == CDCHECKWINDOW_TYPE ) {
		return true;
	}
	return MainWindow::IsTypeOf( type );
}

/*
================
CDCheckWindow::GetClassName
================
*/
// FUNCTION: STBC 0x004fc560
char *CDCheckWindow::GetClassName( void ) {
	return "CDCheckWindow";
}

/*
================
CDCheckWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x004fc570
char *CDCheckWindow::GetSwigTypeName( void ) {
	return "_p_CDCheckWindow";
}

/*
================
CDCheckWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x004fc580
char *CDCheckWindow::GetPythonClassName( void ) {
	return "CDCheckWindowPtr";
}

/*
================
CDCheckWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x004fc590
TGEventHandlerTable *CDCheckWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
CDCheckWindow::CDCheckWindow

The window the stream loader makes before reading it.
================
*/
// FUNCTION: STBC 0x004fc5e0 ??0CDCheckWindow@@QAE@J@Z
CDCheckWindow::CDCheckWindow( long id ) : MainWindow( id ) {
	startingGame = false;
	loadFileName = NULL;
	unknown50 = -1.0f;
}

/*
================
CDCheckWindow::RunCheck

Looks for the game's CD: the setup program on a CD-ROM drive, a disc whose
options database is not the retail one's ("Quit" reading "Quit Game") only
with the signature block past the program's 0x2D0th byte from its end, and
the copy protection's own check. A pass starts the game or loads the saved
game kept for it; a failure asks for the CD again.
================
*/
// FUNCTION: STBC 0x004fc610
// PATCHED: retail's 0x4FC63A is xor ecx, ecx; inc ecx where the original's drive != 0 has setne cl: the patch sets passed whatever CDCheck returned
void CDCheckWindow::RunCheck( bool startingGame_ ) {
	startingGame = startingGame_;
	char drive = UtopiaApp::GetApp()->CDCheck( "setup/stbc.exe" );
	bool passed = drive != 0;
	if ( passed ) {
		bool signed_ = false;
		bool retailDatabase = true;
		char buffer[256];
		sprintf( buffer, "%c:/setup/data/TGL/Options.tgl", drive );
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( buffer, true );
		if ( database != NULL ) {
			TGString *quit = database->GetString( "Quit" );
			if ( quit != NULL ) {
				char *text = quit->Unknown6F56F0();
				if ( strcmp( text, "Quit Game" ) == 0 ) {
					retailDatabase = false;
				}
				delete text;
			}
		}
		g_kLocalizationManager.Unload( database );

		// BUG: the program file is never closed
		FILE *file = g_kSystemWrapper.FileOpen( g_kUtopiaModule.GetProgramPath(), "rb" );
		if ( file == NULL ) {
			passed = false;
		} else {
			g_kSystemWrapper.FileSeek( file, 0x2D0, FILE_SEEK_BEGIN );
			g_kSystemWrapper.FileRead( file, buffer, 0x4F );
			for ( int i = 0; i < 0x4F; i++ ) {
				if ( buffer[i] != 0 ) {
					signed_ = true;
					break;
				}
			}
			if ( retailDatabase && !signed_ ) {
				passed = false;
			}
		}
		if ( CheckDisc( NULL ) ) {
			passed = false;
		}
	}
	if ( !passed ) {
		TGEvent *event = new TGEvent;
		event->SetEventType( ET_SHOW_RETRY_DIALOG );
		event->SetDestination( this );
		g_kEventManager.AddEvent( event );
		return;
	}
	if ( startingGame ) {
		if ( Game::GetCurrentGame() != NULL ) {
			Game::GetCurrentGame()->CDCheckPassed();
		}
	} else {
		g_kUtopiaModule.SetInternalLoadFileName( loadFileName );
		if ( loadFileName != NULL ) {
			delete loadFileName;
			loadFileName = NULL;
		}
	}
}

/*
================
CDCheckWindow::Update
================
*/
// FUNCTION: STBC 0x004fc810
void CDCheckWindow::Update( float time ) {
}

/*
================
CDCheckWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x004fc820
void CDCheckWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CDCheckWindow::ShowRetryHandler, "CDCheckWindow::ShowRetryHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CDCheckWindow::RetryCDHandler, "CDCheckWindow::RetryCDHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CDCheckWindow::AbortAttemptHandler, "CDCheckWindow::AbortAttemptHandler" );
}

/*
================
CDCheckWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x004fc860
void CDCheckWindow::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_SHOW_RETRY_DIALOG, "CDCheckWindow::ShowRetryHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_RETRY_CD, "CDCheckWindow::RetryCDHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_ABORT_ATTEMPT, "CDCheckWindow::AbortAttemptHandler" );
}

/*
================
CDCheckWindow::RetryCDHandler

Checks again, for what the first check was for.
================
*/
// FUNCTION: STBC 0x004fc8a0
void CDCheckWindow::RetryCDHandler( TGEvent *event ) {
	RunCheck( startingGame );
}

/*
================
CDCheckWindow::AbortAttemptHandler

Gives up: forgets the saved game, ends the game, brings the options menu
back up and hides the window.
================
*/
// FUNCTION: STBC 0x004fc8b0
void CDCheckWindow::AbortAttemptHandler( TGEvent *event ) {
	if ( loadFileName != NULL ) {
		delete loadFileName;
		loadFileName = NULL;
	}
	if ( Game::GetCurrentGame() != NULL ) {
		Game::GetCurrentGame()->Terminate( NULL );
	}

	OptionsWindow *options = (OptionsWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_OPTIONS );
	if ( !options->IsVisible() ) {
		TopWindow::GetTopWindow()->ToggleOptionsMenu();
		options->RemoveFromPreviouslyVisible( TopWindow::GetTopWindow()->FindMainWindow( MWT_MODAL_DIALOG ) );
	}
	SetNotVisible();
}

/*
================
CDCheckWindow::ShowRetryHandler

Asks through the modal dialog whether to retry the check or quit.
================
*/
// FUNCTION: STBC 0x004fc930
void CDCheckWindow::ShowRetryHandler( TGEvent *event ) {
	ModalDialogWindow *dialog = (ModalDialogWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_MODAL_DIALOG );

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.TGL", true );
	TGString *title = database->GetString( "CD Not Found" );
	TGString *retry = database->GetString( "Retry CD" );
	TGString *quit = database->GetString( "Quit" );
	g_kLocalizationManager.Unload( database );

	TGEvent *retryEvent = new TGEvent;
	retryEvent->SetDestination( this );
	retryEvent->SetEventType( ET_RETRY_CD );
	TGEvent *abortEvent = new TGEvent;
	abortEvent->SetDestination( this );
	abortEvent->SetEventType( ET_ABORT_ATTEMPT );

	dialog->Run( NULL, title, retry, retryEvent, quit, abortEvent );
}

/*
================
DiscCheckHang

The ways a failed disc check can end the game, unreferenced in retail:
nothing calls them or stores their address (0x4FCA40-0x4FCAD5, between
ShowRetryHandler and SetLoadFileName). Every name is invented.

Hangs.
================
*/
// FUNCTION: STBC 0x004fca40
void DiscCheckHang( void ) {
	for ( ;; ) {
	}
}

/*
================
DiscCheckIgnore
================
*/
// FUNCTION: STBC 0x004fca50
void DiscCheckIgnore( void ) {
}

/*
================
DiscCheckExit
================
*/
// FUNCTION: STBC 0x004fca60
void DiscCheckExit( void ) {
	exit( 0 );
}

/*
================
DiscCheckSpin

Hangs halving a number forever.
================
*/
// FUNCTION: STBC 0x004fca70
void DiscCheckSpin( void ) {
	float value = 1.0f;
	for ( ;; ) {
		value *= 0.5f;
	}
}

/*
================
DiscCheckNullUpdate

Updates a window through a null pointer (CDCheckWindow's slot 83, Update).
================
*/
// FUNCTION: STBC 0x004fca80
void DiscCheckNullUpdate( void ) {
	CDCheckWindow *window = NULL;
	window->Update( 0.0f );
}

/*
================
DiscCheckExitFailure
================
*/
// FUNCTION: STBC 0x004fca90
void DiscCheckExitFailure( void ) {
	exit( 1 );
}

/*
================
DiscCheckCrash

Writes through a null pointer.
================
*/
// FUNCTION: STBC 0x004fcaa0
void DiscCheckCrash( void ) {
	*(char *)NULL = 1;
}

/*
================
DiscCheckRenderError

Prints "Render Error" forever.
================
*/
// FUNCTION: STBC 0x004fcab0
void DiscCheckRenderError( void ) {
	for ( ;; ) {
		printf( "Render Error" );
	}
}

/*
================
DiscCheckExitCode
================
*/
// FUNCTION: STBC 0x004fcad0
void DiscCheckExitCode( void ) {
	exit( 200 );
}

/*
================
CDCheckWindow::SetLoadFileName
================
*/
// FUNCTION: STBC 0x004fcae0
void CDCheckWindow::SetLoadFileName( char *name ) {
	if ( loadFileName != NULL ) {
		delete loadFileName;
		loadFileName = NULL;
	}
	if ( name != NULL ) {
		loadFileName = new char[strlen( name ) + 1];
		strcpy( loadFileName, name );
	}
}

/*
================
CDCheckWindow::PostFixUp

A loaded window has no saved game to load.
================
*/
// FUNCTION: STBC 0x004fcb50
void CDCheckWindow::PostFixUp( void ) {
	MainWindow::PostFixUp();
	loadFileName = NULL;
}
