/*
===============================================================================

	STFileMenu.cpp

===============================================================================
*/

#include <string.h>

#include "STFileMenu.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "STSubPane.h"
#include "STButton.h"
#include "TGDirectory.h"
#include "TGFontGroup.h"
#include "TGFontManager.h"
#include "TGObject.h"
#include "TGParagraph.h"
#include "TGString.h"
#include "TGStream.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include <iostream>
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00521690 _$E33
// FUNCTION: STBC 0x005216a0 _$E32
// FUNCTION: STBC 0x005216b0 _$E37
// FUNCTION: STBC 0x005216c0 _$E36
// FUNCTION: STBC 0x005216d0 _$E48
// FUNCTION: STBC 0x005216e0 _$E47
// FUNCTION: STBC 0x005216f0 _$E52
// FUNCTION: STBC 0x00521700 _$E51
// FUNCTION: STBC 0x00521710 _$E56
// FUNCTION: STBC 0x00521720 _$E55
// FUNCTION: STBC 0x00521730 _$E60
// FUNCTION: STBC 0x00521740 _$E59
// FUNCTION: STBC 0x00521750 _$E184
// FUNCTION: STBC 0x00521760 _$E183
// FUNCTION: STBC 0x00521770 _$E126
// FUNCTION: STBC 0x00521780 _$E125
// FUNCTION: STBC 0x00521790 _$E139
// FUNCTION: STBC 0x005217a0 _$E138
// FUNCTION: STBC 0x005217b0 _$E189
// FUNCTION: STBC 0x005217c0 _$E188
// FUNCTION: STBC 0x005217d0 _$E194
// FUNCTION: STBC 0x005217e0 _$E193
// FUNCTION: STBC 0x005217f0 _$E198
// FUNCTION: STBC 0x00521800 _$E197
// FUNCTION: STBC 0x00521810 _$E202
// FUNCTION: STBC 0x00521820 _$E201
// FUNCTION: STBC 0x00521830 _$E209
// FUNCTION: STBC 0x00521840 _$E208
// FUNCTION: STBC 0x00521850 _$E290
// FUNCTION: STBC 0x00521860 _$E289
// FUNCTION: STBC 0x00521870 _$E294
// FUNCTION: STBC 0x00521880 _$E293
// FUNCTION: STBC 0x00521890 _$E84
// FUNCTION: STBC 0x005218a0 _$E83
// FUNCTION: STBC 0x005218b0 _$E276
// FUNCTION: STBC 0x005218c0 _$E275
// FUNCTION: STBC 0x005218d0 _$E283
// FUNCTION: STBC 0x005218e0 _$E282
// FUNCTION: STBC 0x005218f0 _$E306
// FUNCTION: STBC 0x00521900 _$E305
// FUNCTION: STBC 0x00521910 _$E310
// FUNCTION: STBC 0x00521920 _$E309
// FUNCTION: STBC 0x00521930 _$E350
// FUNCTION: STBC 0x00521940 _$E349
// FUNCTION: STBC 0x00521950 _$E356
// FUNCTION: STBC 0x00521960 _$E355
// FUNCTION: STBC 0x00521970 _$E298
// FUNCTION: STBC 0x00521980 _$E297
// FUNCTION: STBC 0x00521990 _$E302
// FUNCTION: STBC 0x005219a0 _$E301
// FUNCTION: STBC 0x005219b0 _$E314
// FUNCTION: STBC 0x005219c0 _$E313
// FUNCTION: STBC 0x005219d0 _$E318
// FUNCTION: STBC 0x005219e0 _$E317
// FUNCTION: STBC 0x005219f0 _$E322
// FUNCTION: STBC 0x00521a00 _$E321
// FUNCTION: STBC 0x00521a10 _$E326
// FUNCTION: STBC 0x00521a20 _$E325
// FUNCTION: STBC 0x00521a30 _$E330
// FUNCTION: STBC 0x00521a40 _$E329
// FUNCTION: STBC 0x00521a50 _$E334
// FUNCTION: STBC 0x00521a60 _$E333
// FUNCTION: STBC 0x00521a70 _$E338
// FUNCTION: STBC 0x00521a80 _$E337
// FUNCTION: STBC 0x00521a90 _$E342
// FUNCTION: STBC 0x00521aa0 _$E341
// FUNCTION: STBC 0x00521ab0 _$E89
// FUNCTION: STBC 0x00521ac0 _$E88
// FUNCTION: STBC 0x00521ad0 _$E346
// FUNCTION: STBC 0x00521ae0 _$E345
// FUNCTION: STBC 0x00521af0 _$E94
// FUNCTION: STBC 0x00521b00 _$E93
// FUNCTION: STBC 0x00521b10 _$E360
// FUNCTION: STBC 0x00521b20 _$E359
// FUNCTION: STBC 0x00521b30 _$E364
// FUNCTION: STBC 0x00521b40 _$E363
// FUNCTION: STBC 0x00521b50 _$E368
// FUNCTION: STBC 0x00521b60 _$E367
// FUNCTION: STBC 0x00521b70 _$E372
// FUNCTION: STBC 0x00521b80 _$E371
// FUNCTION: STBC 0x00521b90 _$E103
// FUNCTION: STBC 0x00521ba0 _$E102
// FUNCTION: STBC 0x00521bb0 _$E376
// FUNCTION: STBC 0x00521bc0 _$E375
// FUNCTION: STBC 0x00521bd0 _$E380
// FUNCTION: STBC 0x00521be0 _$E379
// FUNCTION: STBC 0x00521bf0 _$E384
// FUNCTION: STBC 0x00521c00 _$E383
// FUNCTION: STBC 0x00521c10 _$E109
// FUNCTION: STBC 0x00521c20 _$E108
// FUNCTION: STBC 0x00521c60 _$E114
// FUNCTION: STBC 0x00521c70 _$E113
// FUNCTION: STBC 0x00521c80 _$E144
// FUNCTION: STBC 0x00521c90 _$E143
// FUNCTION: STBC 0x00521ca0 _$E388
// FUNCTION: STBC 0x00521cb0 _$E387
// FUNCTION: STBC 0x00521cc0 _$E149
// FUNCTION: STBC 0x00521cd0 _$E148
// FUNCTION: STBC 0x00521ce0 _$E392
// FUNCTION: STBC 0x00521cf0 _$E391
// FUNCTION: STBC 0x00521d00 _$E396
// FUNCTION: STBC 0x00521d10 _$E395
// FUNCTION: STBC 0x00521d20 _$E154
// FUNCTION: STBC 0x00521d30 _$E153
// FUNCTION: STBC 0x00521d40 _$E400
// FUNCTION: STBC 0x00521d50 _$E399
// FUNCTION: STBC 0x00521d60 _$E119
// FUNCTION: STBC 0x00521d70 _$E118
// FUNCTION: STBC 0x00521d80 _$E404
// FUNCTION: STBC 0x00521d90 _$E403
// FUNCTION: STBC 0x00521da0 _$E408
// FUNCTION: STBC 0x00521db0 _$E407
// FUNCTION: STBC 0x00521dc0 _$E159
// FUNCTION: STBC 0x00521dd0 _$E158
// FUNCTION: STBC 0x00521de0 _$E164
// FUNCTION: STBC 0x00521df0 _$E163
// FUNCTION: STBC 0x00521e00 _$E412
// FUNCTION: STBC 0x00521e10 _$E411
// FUNCTION: STBC 0x00521e20 _$E416
// FUNCTION: STBC 0x00521e30 _$E415
// FUNCTION: STBC 0x00521e40 _$E420
// FUNCTION: STBC 0x00521e50 _$E419
// FUNCTION: STBC 0x00521e60 _$E424
// FUNCTION: STBC 0x00521e70 _$E423
// FUNCTION: STBC 0x00521e80 _$E428
// FUNCTION: STBC 0x00521e90 _$E427
// FUNCTION: STBC 0x00521ea0 _$E432
// FUNCTION: STBC 0x00521eb0 _$E431
// FUNCTION: STBC 0x00521ec0 _$E436
// FUNCTION: STBC 0x00521ed0 _$E435
// FUNCTION: STBC 0x00521ee0 _$E440
// FUNCTION: STBC 0x00521ef0 _$E439
// FUNCTION: STBC 0x00521f00 _$E169
// FUNCTION: STBC 0x00521f10 _$E168
// FUNCTION: STBC 0x00521f20 _$E444
// FUNCTION: STBC 0x00521f30 _$E443
// FUNCTION: STBC 0x00521f40 _$E174
// FUNCTION: STBC 0x00521f50 _$E173
// FUNCTION: STBC 0x00521f60 _$E179
// FUNCTION: STBC 0x00521f70 _$E178
// FUNCTION: STBC 0x00521f80 _$E248
// FUNCTION: STBC 0x00521f90 _$E247
// FUNCTION: STBC 0x00521fa0 _$E253
// FUNCTION: STBC 0x00521fb0 _$E252
// FUNCTION: STBC 0x00521fc0 _$E257
// FUNCTION: STBC 0x00521fd0 _$E256

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00521610 _$E262
// FUNCTION: STBC 0x00521620 _$E259
// FUNCTION: STBC 0x00521630 _$E261
// FUNCTION: STBC 0x00521640 _$E260
// FUNCTION: STBC 0x00521650 _$E268
// FUNCTION: STBC 0x00521660 _$E265
// FUNCTION: STBC 0x00521670 _$E267
// FUNCTION: STBC 0x00521680 _$E266

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00520e10 _$E450
// FUNCTION: STBC 0x00520e20 _$E446
// FUNCTION: STBC 0x00520e30 _$E449
// FUNCTION: STBC 0x00520e40 _$E447
// FUNCTION: STBC 0x00520aa0 _$E459
// FUNCTION: STBC 0x00520ac0 _$E458

// DATA: STBC 0x00988ea8
TGEventHandlerTable STFileMenu::classHandlers;

/*
================
STFileMenu::Cast

The object as a file menu; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00520e60
STFileMenu *STFileMenu::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( STFILEMENU_TYPE ) ) {
		return (STFileMenu *)object;
	}
	return NULL;
}

/*
================
STFileMenu::STFileMenu

The same, named by a string.
================
*/
// FUNCTION: STBC 0x00520e90 ??0STFileMenu@@QAE@PAD00@Z
STFileMenu::STFileMenu( char *name, char *dir_, char *filter_ ) : STTopLevelMenu( name, STMF_DEFAULTS, NULL, NULL ) {
	dir = NULL;
	filter = NULL;
	SetDir( dir_ );
	SetFilter( filter_ );
	flags &= ~STMF_CLOSEABLE;
	Refresh();
}

/*
================
STFileMenu::STFileMenu

A closeable, openable menu that lists the files of the directory matching
the filter.
================
*/
// FUNCTION: STBC 0x00521030 ??0STFileMenu@@QAE@PAVTGString@@PAD1@Z
STFileMenu::STFileMenu( TGString *name, char *dir_, char *filter_ ) : STTopLevelMenu( name, STMF_DEFAULTS, NULL, NULL ) {
	dir = NULL;
	filter = NULL;
	SetDir( dir_ );
	SetFilter( filter_ );
	flags &= ~STMF_CLOSEABLE;
	Refresh();
}

/*
================
STFileMenu::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x005210c0
void STFileMenu::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&STFileMenu::HandleKeyboard, "STFileMenu::HandleKeyboard" );
}

/*
================
STFileMenu::RegisterHandlers

The menu's selection keys come to HandleKeyboard.
================
*/
// FUNCTION: STBC 0x005210e0
void STFileMenu::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_X, "STFileMenu::HandleKeyboard" );
	AddMethodHandlerForClass( &classHandlers, ET_INPUT_SELECT_OPTION, "STFileMenu::HandleKeyboard" );
}

/*
================
STFileMenu::SetDir

Keeps a copy of the directory to list.
================
*/
// FUNCTION: STBC 0x00521110 ?SetDir@STFileMenu@@QAEXPAD@Z
void STFileMenu::SetDir( char *newDir ) {
	if ( dir != NULL ) {
		delete [] dir;
		dir = NULL;
	}
	if ( newDir != NULL ) {
		dir = new char[strlen( newDir ) + 1];
		strcpy( dir, newDir );
	}
}

/*
================
STFileMenu::SetFilter

Keeps a copy of the file name filter.
================
*/
// FUNCTION: STBC 0x00521180 ?SetFilter@STFileMenu@@QAEXPAD@Z
void STFileMenu::SetFilter( char *newFilter ) {
	if ( filter != NULL ) {
		delete [] filter;
		filter = NULL;
	}
	if ( newFilter != NULL ) {
		filter = new char[strlen( newFilter ) + 1];
		strcpy( filter, newFilter );
	}
}

/*
================
STFileMenu::Refresh

Rebuilds the menu as a button per file of the directory matching the
filter, in name order, the filter's extension cut from the names. The
buttons are built in the font size of the first button's text.
================
*/
// FUNCTION: STBC 0x005211f0
// PARKED: complete; retail copies the default font size into its local through the FPU (this build: integer moves, as for any inline float accessor into a local), and swaps esi/ebp for the index and the extension
void STFileMenu::Refresh( void ) {
	float defaultFontSize = g_kFontManager.GetDefaultFont()->GetFontSize();
	if ( GetNumChildren() > 0 ) {
		STButton *button = STButton::Cast( GetNthChild( 0 ) );
		if ( button ) {
			TGPane *textPane = TGPane::Cast( button->TGPane::GetNthChild( STBUTTON_TEXT_PANE ) );
			TGParagraph *text = TGParagraph::Cast( textPane->GetFirstChild() );
			if ( text ) {
				float fontSize = text->GetFontGroup()->GetFontSize();
				if ( fontSize != 0.0f ) {
					g_kFontManager.SetDefaultFont( g_kFontManager.GetDefaultFont()->GetFontName(), fontSize );
				}
			}
		}
	}
	KillChildren();

	TGDirectory directory;
	directory.Read( GetDir(), GetFilter(), false );
	TGDirectoryFile *file = directory.GetFiles();
	bool noFocus = ( flags >> 8 ) & 1;
	SetNoFocus();
	TGString buttonName( "" );
	while ( file ) {
		int numButtons = GetNumChildren();
		int i;
		for ( i = 0; i < numButtons; i++ ) {
			STButton *button = STButton::Cast( GetNthChild( i ) );
			if ( button ) {
				button->GetName( &buttonName );
				char *name = buttonName.Unknown6F56F0();
				if ( strcmp( name, file->name ) > 0 ) {
					delete name;
					break;
				}
				delete name;
			}
		}
		char *extension = NULL;
		if ( GetFilter() ) {
			extension = strrchr( file->name, '.' );
			if ( extension ) {
				*extension = '\0';
			}
		}
		if ( i >= numButtons ) {
			AddChild( new STButton( file->name, NULL, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 ) );
		} else {
			InsertChild( i, new STButton( file->name, NULL, STBF_FILL_PARENT_WIDTH, 0.0f, 0.0f, NULL, 200 ) );
		}
		if ( extension ) {
			*extension = '.';
		}
		file = file->next;
	}

	STButton *first = STButton::Cast( GetNthChild( 0 ) );
	if ( first ) {
		SetFocus( first, true );
	}
	if ( !noFocus ) {
		SetNotNoFocus();
	}
	g_kFontManager.SetDefaultFont( g_kFontManager.GetDefaultFont()->GetFontName(), defaultFontSize );
}

/*
================
STFileMenu::SetFocus

The menu announces itself as the active one before the focus moves.
================
*/
// FUNCTION: STBC 0x00521550 ?SetFocus@STFileMenu@@UAEXPAVTGUIObject@@_N@Z
void STFileMenu::SetFocus( TGUIObject *child, bool layout ) {
	SendActivationEvent();
	STMenu::SetFocus( child, layout );
}

/*
================
STFileMenu::HandleKeyboard

Selecting the menu with a keyboard event activates it.
================
*/
// FUNCTION: STBC 0x00521570
void STFileMenu::HandleKeyboard( TGEvent *event ) {
	CallNextHandler( event );
	SendActivationEvent();
}

/*
================
STFileMenu::WriteToStream

The directory and the filter follow the menu.
================
*/
// FUNCTION: STBC 0x00521590 ?WriteToStream@STFileMenu@@UAE_NPAVTGStream@@@Z
bool STFileMenu::WriteToStream( TGStream *stream ) {
	if ( !STTopLevelMenu::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot11( dir );
	stream->VirtualSlot11( filter );
	return true;
}

/*
================
STFileMenu::ReadFromStream
================
*/
// FUNCTION: STBC 0x005215d0 ?ReadFromStream@STFileMenu@@UAE_NPAVTGStream@@@Z
bool STFileMenu::ReadFromStream( TGStream *stream ) {
	if ( !STTopLevelMenu::ReadFromStream( stream ) ) {
		return false;
	}
	dir = stream->VirtualSlot10( -1 );
	filter = stream->VirtualSlot10( -1 );
	return true;
}
