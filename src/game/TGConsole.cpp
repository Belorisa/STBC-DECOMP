/*
===============================================================================

	TGConsole.cpp

===============================================================================
*/

#include <iostream>
#include "TGConsole.h"
#include "TGPrompt.h"
#include "TGEventManager.h"
#include "TGEventHandlerTable.h"
#include "TGKeyboardEvent.h"
#include "TGStream.h"
#include "TGString.h"
#include "PyEmbed.h"
#include "TGRect.h"
#include "TGParagraph.h"
#include "TGPrompt.h"
#include "TGFontManager.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00735bf0 _$E32
// FUNCTION: STBC 0x00735c00 _$E29
// FUNCTION: STBC 0x00735c10 _$E31
// FUNCTION: STBC 0x00735c20 _$E30
// FUNCTION: STBC 0x00735c30 _$E38
// FUNCTION: STBC 0x00735c40 _$E35
// FUNCTION: STBC 0x00735c50 _$E37
// FUNCTION: STBC 0x00735c60 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00735c70 _$E57
// FUNCTION: STBC 0x00735c80 _$E56
// FUNCTION: STBC 0x00735c90 _$E61
// FUNCTION: STBC 0x00735ca0 _$E60
// FUNCTION: STBC 0x00735cb0 _$E72
// FUNCTION: STBC 0x00735cc0 _$E71
// FUNCTION: STBC 0x00735cd0 _$E76
// FUNCTION: STBC 0x00735ce0 _$E75
// FUNCTION: STBC 0x00735cf0 _$E80
// FUNCTION: STBC 0x00735d00 _$E79
// FUNCTION: STBC 0x00735d10 _$E84
// FUNCTION: STBC 0x00735d20 _$E83
// FUNCTION: STBC 0x00735d30 _$E208
// FUNCTION: STBC 0x00735d40 _$E207
// FUNCTION: STBC 0x00735d50 _$E150
// FUNCTION: STBC 0x00735d60 _$E149
// FUNCTION: STBC 0x00735d70 _$E163
// FUNCTION: STBC 0x00735d80 _$E162
// FUNCTION: STBC 0x00735d90 _$E213
// FUNCTION: STBC 0x00735da0 _$E212
// FUNCTION: STBC 0x00735db0 _$E218
// FUNCTION: STBC 0x00735dc0 _$E217
// FUNCTION: STBC 0x00735dd0 _$E222
// FUNCTION: STBC 0x00735de0 _$E221
// FUNCTION: STBC 0x00735df0 _$E226
// FUNCTION: STBC 0x00735e00 _$E225
// FUNCTION: STBC 0x00735e10 _$E233
// FUNCTION: STBC 0x00735e20 _$E232
// FUNCTION: STBC 0x00735e30 _$E290
// FUNCTION: STBC 0x00735e40 _$E289
// FUNCTION: STBC 0x00735e50 _$E294
// FUNCTION: STBC 0x00735e60 _$E293
// FUNCTION: STBC 0x00735e70 _$E108
// FUNCTION: STBC 0x00735e80 _$E107
// FUNCTION: STBC 0x00735e90 _$E276
// FUNCTION: STBC 0x00735ea0 _$E275
// FUNCTION: STBC 0x00735eb0 _$E283
// FUNCTION: STBC 0x00735ec0 _$E282
// FUNCTION: STBC 0x00735ed0 _$E306
// FUNCTION: STBC 0x00735ee0 _$E305
// FUNCTION: STBC 0x00735ef0 _$E310
// FUNCTION: STBC 0x00735f00 _$E309
// FUNCTION: STBC 0x00735f10 _$E350
// FUNCTION: STBC 0x00735f20 _$E349
// FUNCTION: STBC 0x00735f30 _$E356
// FUNCTION: STBC 0x00735f40 _$E355
// FUNCTION: STBC 0x00735f50 _$E298
// FUNCTION: STBC 0x00735f60 _$E297
// FUNCTION: STBC 0x00735f70 _$E302
// FUNCTION: STBC 0x00735f80 _$E301
// FUNCTION: STBC 0x00735f90 _$E314
// FUNCTION: STBC 0x00735fa0 _$E313
// FUNCTION: STBC 0x00735fb0 _$E318
// FUNCTION: STBC 0x00735fc0 _$E317
// FUNCTION: STBC 0x00735fd0 _$E322
// FUNCTION: STBC 0x00735fe0 _$E321
// FUNCTION: STBC 0x00735ff0 _$E326
// FUNCTION: STBC 0x00736000 _$E325
// FUNCTION: STBC 0x00736010 _$E330
// FUNCTION: STBC 0x00736020 _$E329
// FUNCTION: STBC 0x00736030 _$E334
// FUNCTION: STBC 0x00736040 _$E333
// FUNCTION: STBC 0x00736050 _$E338
// FUNCTION: STBC 0x00736060 _$E337
// FUNCTION: STBC 0x00736070 _$E342
// FUNCTION: STBC 0x00736080 _$E341
// FUNCTION: STBC 0x00736090 _$E113
// FUNCTION: STBC 0x007360a0 _$E112
// FUNCTION: STBC 0x007360b0 _$E346
// FUNCTION: STBC 0x007360c0 _$E345
// FUNCTION: STBC 0x007360d0 _$E118
// FUNCTION: STBC 0x007360e0 _$E117
// FUNCTION: STBC 0x007360f0 _$E360
// FUNCTION: STBC 0x00736100 _$E359
// FUNCTION: STBC 0x00736110 _$E364
// FUNCTION: STBC 0x00736120 _$E363
// FUNCTION: STBC 0x00736130 _$E368
// FUNCTION: STBC 0x00736140 _$E367
// FUNCTION: STBC 0x00736150 _$E372
// FUNCTION: STBC 0x00736160 _$E371
// FUNCTION: STBC 0x00736170 _$E127
// FUNCTION: STBC 0x00736180 _$E126
// FUNCTION: STBC 0x00736190 _$E376
// FUNCTION: STBC 0x007361a0 _$E375
// FUNCTION: STBC 0x007361b0 _$E380
// FUNCTION: STBC 0x007361c0 _$E379
// FUNCTION: STBC 0x007361d0 _$E384
// FUNCTION: STBC 0x007361e0 _$E383
// FUNCTION: STBC 0x007361f0 _$E133
// FUNCTION: STBC 0x00736200 _$E132
// FUNCTION: STBC 0x00736210 _$E138
// FUNCTION: STBC 0x00736220 _$E137
// FUNCTION: STBC 0x00736230 _$E168
// FUNCTION: STBC 0x00736240 _$E167
// FUNCTION: STBC 0x00736250 _$E388
// FUNCTION: STBC 0x00736260 _$E387
// FUNCTION: STBC 0x00736270 _$E173
// FUNCTION: STBC 0x00736280 _$E172
// FUNCTION: STBC 0x00736290 _$E392
// FUNCTION: STBC 0x007362a0 _$E391
// FUNCTION: STBC 0x007362b0 _$E396
// FUNCTION: STBC 0x007362c0 _$E395
// FUNCTION: STBC 0x007362d0 _$E178
// FUNCTION: STBC 0x007362e0 _$E177
// FUNCTION: STBC 0x007362f0 _$E400
// FUNCTION: STBC 0x00736300 _$E399
// FUNCTION: STBC 0x00736310 _$E143
// FUNCTION: STBC 0x00736320 _$E142
// FUNCTION: STBC 0x00736330 _$E404
// FUNCTION: STBC 0x00736340 _$E403
// FUNCTION: STBC 0x00736350 _$E408
// FUNCTION: STBC 0x00736360 _$E407
// FUNCTION: STBC 0x00736370 _$E183
// FUNCTION: STBC 0x00736380 _$E182
// FUNCTION: STBC 0x00736390 _$E188
// FUNCTION: STBC 0x007363a0 _$E187
// FUNCTION: STBC 0x007363b0 _$E412
// FUNCTION: STBC 0x007363c0 _$E411
// FUNCTION: STBC 0x007363d0 _$E416
// FUNCTION: STBC 0x007363e0 _$E415
// FUNCTION: STBC 0x007363f0 _$E420
// FUNCTION: STBC 0x00736400 _$E419
// FUNCTION: STBC 0x00736410 _$E424
// FUNCTION: STBC 0x00736420 _$E423
// FUNCTION: STBC 0x00736430 _$E428
// FUNCTION: STBC 0x00736440 _$E427
// FUNCTION: STBC 0x00736450 _$E432
// FUNCTION: STBC 0x00736460 _$E431
// FUNCTION: STBC 0x00736470 _$E436
// FUNCTION: STBC 0x00736480 _$E435
// FUNCTION: STBC 0x00736490 _$E440
// FUNCTION: STBC 0x007364a0 _$E439
// FUNCTION: STBC 0x007364b0 _$E193
// FUNCTION: STBC 0x007364c0 _$E192
// FUNCTION: STBC 0x007364d0 _$E444
// FUNCTION: STBC 0x007364e0 _$E443
// FUNCTION: STBC 0x007364f0 _$E198
// FUNCTION: STBC 0x00736500 _$E197
// FUNCTION: STBC 0x00736510 _$E203
// FUNCTION: STBC 0x00736520 _$E202

// The prompt in front of the input line's text.
#define PROMPT_STRING		"> "
#define PROMPT_LENGTH		2

// DATA: STBC 0x0095d078
float TGConsole::textIndent = 0.003125f;

/*
================
TGConsole::RedirectOutput

Python's standard output and error go to one cStringIO.StringIO object.
================
*/
// FUNCTION: STBC 0x007365d0
void TGConsole::RedirectOutput( void ) {
	PyObject *buffer = NULL;
	if ( PyEmbed::CallFunction( "cStringIO", "StringIO", "O", &buffer, "()" ) != -1 ) {
		PyEmbed::SetValue( "sys", "stdout", "O", buffer );
		PyEmbed::SetValue( "sys", "stderr", "O", buffer );
		Py_DECREF( buffer );
	}
}

/*
================
TGConsole::Unknown736660
================
*/
// FUNCTION: STBC 0x00736660
void TGConsole::Unknown736660( void ) {
}

/*
================
TGConsole::Cast

The object as a console; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00736670
TGConsole *TGConsole::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( TGCONSOLE_TYPE ) == true ) {
		return (TGConsole *)object;
	}
	return NULL;
}

/*
================
TGConsole::TGConsole
================
*/
// FUNCTION: STBC 0x007366a0
TGConsole::TGConsole( void ) : TGPane( 0 ), lines( 2 ) {
	lines.RemoveAll();
	// The emptied list's head: no command shown (retail copies the cleared
	// head's zero into it).
	historyPos = lines.GetHeadPos();
	fontName = NULL;
	fontSize = 0.0f;
}

/*
================
TGConsole::~TGConsole
================
*/
// FUNCTION: STBC 0x00736710
TGConsole::~TGConsole( void ) {
	while ( !lines.IsEmpty() ) {
		delete [] lines.GetHead();
		lines.RemoveHead();
	}
}

/*
================
TGConsole::AddLine

Puts the line under the last one and brings it into view.
================
*/
// FUNCTION: STBC 0x007367a0
void TGConsole::AddLine( TGParagraph *line ) {
	TGUIObject *last = GetLastChild();
	if ( last != NULL ) {
		AddChild( line, textIndent, last->GetHeight() + last->GetTop(), true );
	} else {
		AddChild( line, textIndent, GetHeight(), true );
	}
	KeepInView();
	SetFocus( line, true );
}

/*
================
TGConsole::SetConsoleFont

Takes a copy of the font group's name and applies the font to every line.
================
*/
// FUNCTION: STBC 0x00736820
void TGConsole::SetConsoleFont( char *name, float size ) {
	if ( fontName != NULL ) {
		delete [] fontName;
	}
	if ( name != NULL ) {
		fontName = new char[strlen( name ) + 1];
		strcpy( fontName, name );
	} else {
		fontName = NULL;
	}
	fontSize = size;

	int numChildren = GetNumChildren();
	TGFontGroup *fontGroup = g_kFontManager.GetFontGroup( fontName, fontSize );
	if ( fontGroup == NULL ) {
		fontGroup = g_kFontManager.GetDefaultFont();
	}
	for ( int i = 0; i < numChildren; i++ ) {
		TGParagraph *line = (TGParagraph *)GetNthChild( i );
		line->SetFontGroup( fontGroup, false );
	}
	KeepInView();
}

/*
================
TGConsole::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00736900
void TGConsole::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGConsole::KeyboardHandler, "TGConsole::KeyboardHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&TGPrompt::KeyboardHandler, "TGPrompt::KeyboardHandler" );
}

/*
================
TGConsole::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00736930
void TGConsole::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_KEYBOARD, "TGConsole::KeyboardHandler" );
	AddMethodHandlerForClass( TGPrompt::GetClassTable(), ET_KEYBOARD, "TGPrompt::KeyboardHandler" );
}

/*
================
TGConsole::KeyboardHandler

Enter runs the prompt's line; up and down bring back the commands run
before. Other keys go on to the pane.
================
*/
// FUNCTION: STBC 0x00736960
void TGConsole::KeyboardHandler( TGKeyboardEvent *event ) {
	unsigned short unicode = event->GetUnicode();
	if ( event->GetKeyState() == KS_NORMAL && unicode == '\r' ) {
		TGUIObject *prompt = GetLastChild();
		if ( prompt != NULL ) {
			Execute( (TGParagraph *)prompt );
			event->SetHandled();
		}
	} else if ( event->GetKeyState() == KS_KEYDOWN && ( unicode == WC_UP || unicode == WC_DOWN ) ) {
		if ( !lines.IsEmpty() ) {
			if ( unicode == WC_UP ) {
				if ( historyPos == NULL ) {
					historyPos = lines.GetTailPos();
				} else if ( historyPos != lines.GetHeadPos() ) {
					lines.GetPrev( historyPos );
				} else {
					historyPos = NULL;
				}
			} else if ( unicode == WC_DOWN ) {
				if ( historyPos == NULL ) {
					historyPos = lines.GetHeadPos();
				} else if ( historyPos != lines.GetTailPos() ) {
					lines.GetNext( historyPos );
				} else {
					historyPos = NULL;
				}
			}
			TGParagraph *prompt = (TGParagraph *)GetLastChild();
			if ( prompt != NULL ) {
				TGString text( PROMPT_STRING );
				if ( historyPos != NULL ) {
					text.Unknown6F4A40( lines.Get( historyPos ) );
				}
				prompt->SetString( &text, true );
			}
			KeepInView();
		}
	} else {
		CallNextHandler( event );
		KeepInView();
	}
}

/*
================
TGConsole::Execute

Runs the prompt's text in Python, files it with the commands run, shows
what Python wrote and starts a new prompt. False when Python raised.
================
*/
// FUNCTION: STBC 0x00736ac0
bool TGConsole::Execute( TGParagraph *prompt ) {
	char *command = prompt->GetCString();
	int status = PyRun_SimpleString( command );
	lines.AddTail( command );
	historyPos = NULL;
	ShowOutput();
	AddLine( new TGPrompt( GetWidth() - textIndent, fontName, fontSize ) );
	return status == 0 ? true : false;
}

/*
================
TGConsole::KeepInView

The console grows to hold its lines and its bottom stays at the bottom of
its parent.
================
*/
// FUNCTION: STBC 0x00736ba0
// PARKED: schedule: retail stores the bounds' third dword after loading the top for the sum, this build before
void TGConsole::KeepInView( void ) {
	TGUIObject *last = GetLastChild();
	if ( last != NULL ) {
		TGRect lastBounds;
		last->GetBounds( &lastBounds );
		float bottom = lastBounds.GetBottom();
		float height = GetHeight();
		if ( bottom > height ) {
			float width = GetWidth();
			Resize( width, bottom, false );
		}
		if ( parent != NULL ) {
			float parentHeight = parent->GetHeight();
			if ( GetBottom() != parentHeight ) {
				height = GetHeight();
				SetPosition( 0.0f, parentHeight - height, false );
				Layout();
			}
		}
	}
}

/*
================
TGConsole::GetPyString

The text Python wrote to its standard output; reading it closes the buffer.
================
*/
// FUNCTION: STBC 0x00736c90
char *TGConsole::GetPyString( void ) {
	char *string = "";
	PyObject *output = PySys_GetObject( "stdout" );
	if ( output != NULL ) {
		PyObject *text = PyObject_CallMethod( output, "getvalue", NULL );
		PyObject_CallMethod( output, "close", NULL );
		if ( text != NULL ) {
			string = PyString_AsString( text );
		}
	}
	return string;
}

/*
================
TGConsole::ShowOutput

What Python wrote since the last time, one line each.
================
*/
// FUNCTION: STBC 0x00736cf0
void TGConsole::ShowOutput( void ) {
	char *scan = GetPyString();
	char *line;
	for ( ;; ) {
		line = scan;
		while ( *scan != '\0' && *scan != '\r' && *scan != '\n' ) {
			scan++;
		}
		if ( *scan == '\0' ) {
			break;
		}
		*scan++ = '\0';
		float lineWidth = GetWidth() - textIndent;
		AddLine( new TGParagraph( line, lineWidth, NULL, fontName, fontSize, TGPF_READ_ONLY ) );
	}
	float lastWidth = GetWidth() - textIndent;
	AddLine( new TGParagraph( line, lastWidth, NULL, fontName, fontSize, TGPF_READ_ONLY ) );
}

/*
================
TGConsole::Layout

An empty console takes the size of its parent and starts with a prompt.
================
*/
// FUNCTION: STBC 0x00736df0
void TGConsole::Layout( void ) {
	if ( bounds.top == 0.0f && bounds.left == 0.0f && bounds.width == 0.0f && bounds.height == 0.0f
		&& GetNumChildren() == 0 && parent != NULL ) {
		TGRect size;
		float width = parent->GetWidth();
		size.width = width;
		SetBounds( &size, true );
		AddLine( new TGPrompt( GetWidth() - textIndent, fontName, fontSize ) );
	}
	TGPane::Layout();
}

/*
================
TGConsole::VirtualSlot59
================
*/
// FUNCTION: STBC 0x00736f10
void TGConsole::VirtualSlot59( void ) {
	TGPane::VirtualSlot59();
	if ( parent != NULL ) {
		parent->SetAlwaysHandleEvents();
	}
}

/*
================
TGConsole::EvalString

SWIG: runs the string in Python as if it was typed at the prompt.
================
*/
// FUNCTION: STBC 0x00736f30
bool TGConsole::EvalString( char *string ) {
	TGPrompt *prompt = new TGPrompt( GetWidth() - textIndent, fontName, fontSize );
	prompt->AppendString( string, true );
	AddLine( prompt );
	return Execute( prompt );
}

/*
================
TGConsole::AddConsoleString

SWIG: shows the string as a line of its own above the prompt.
================
*/
// FUNCTION: STBC 0x00736fa0
void TGConsole::AddConsoleString( char *string ) {
	float width = GetWidth() - textIndent;
	TGParagraph *line = new TGParagraph( "", width, NULL, fontName, fontSize, TGPF_READ_ONLY );
	line->AppendString( string, true );
	TGUIObject *prompt = GetLastChild();
	RemoveChild( prompt, false );
	AddLine( line );
	AddLine( (TGParagraph *)prompt );
}

/*
================
TGPrompt::TGPrompt

A prompt line: the prompt string, wrapped at the width, in the font.
================
*/
// FUNCTION: STBC 0x00737040
TGPrompt::TGPrompt( float width, char *fontName, float fontSize ) : TGParagraph( PROMPT_STRING, width, NULL, fontName, fontSize, 0 ) {
}

/*
================
TGPrompt::SetCursorPosition

The cursor stays behind the prompt.
================
*/
// FUNCTION: STBC 0x00737070
void TGPrompt::SetCursorPosition( int position ) {
	TGParagraph::SetCursorPosition( position );
	if ( cursorPosition < PROMPT_LENGTH ) {
		cursorPosition = PROMPT_LENGTH;
	}
}

/*
================
TGPrompt::GetString

The text without the prompt.
================
*/
// FUNCTION: STBC 0x00737090
void TGPrompt::GetString( TGString *string ) {
	*string = this->string;
	string->Unknown6F55C0( 0, PROMPT_LENGTH );
}

/*
================
TGPrompt::GetCString

The text without the prompt, as a new C string.
================
*/
// FUNCTION: STBC 0x007370b0
char *TGPrompt::GetCString( void ) {
	string.Unknown6F55C0( 0, PROMPT_LENGTH );
	char *text = string.Unknown6F56F0();
	string.Unknown6F5020( 0, PROMPT_STRING );
	return text;
}

/*
================
TGPrompt::KeyboardHandler

Home puts the cursor behind the prompt. Backspace and left stop there.
================
*/
// FUNCTION: STBC 0x007370e0
void TGPrompt::KeyboardHandler( TGKeyboardEvent *event ) {
	unsigned short unicode = event->GetUnicode();
	int keyState = event->GetKeyState();
	if ( keyState == KS_KEYUP && unicode == WC_HOME ) {
		cursorPosition = PROMPT_LENGTH;
		event->SetHandled();
	}
	if ( cursorPosition == PROMPT_LENGTH ) {
		if ( ( keyState == KS_NORMAL && unicode == '' ) || ( keyState == KS_KEYUP && unicode == WC_LEFT ) ) {
			event->SetHandled();
		}
	}
	if ( !event->EventHandled() ) {
		CallNextHandler( event );
	}
}

/*
================
TGConsole::WriteToStream
================
*/
// FUNCTION: STBC 0x00737140
bool TGConsole::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteLong( lines.GetSize() );
	NiTListIterator pos = lines.GetHeadPos();
	while ( pos != NULL ) {
		stream->VirtualSlot11( lines.GetNext( pos ) );
	}
	stream->VirtualSlot11( fontName );
	stream->WriteFloat( fontSize );
	return true;
}

/*
================
TGConsole::ReadFromStream
================
*/
// FUNCTION: STBC 0x007371a0
bool TGConsole::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	int numLines = stream->ReadLong();
	for ( int i = numLines; i > 0; i-- ) {
		lines.AddTail( stream->VirtualSlot10( -1 ) );
	}
	historyPos = NULL;
	fontName = stream->VirtualSlot10( -1 );
	fontSize = stream->ReadFloat();
	return true;
}

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00736530 _$E450
// FUNCTION: STBC 0x00736540 _$E446
// FUNCTION: STBC 0x00736550 _$E449
// FUNCTION: STBC 0x00736560 _$E447
// DATA: STBC 0x0099da08
TGEventHandlerTable TGConsole::classHandlers;

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00736580 _$E455
// FUNCTION: STBC 0x00736590 _$E452
// FUNCTION: STBC 0x007365a0 _$E454
// FUNCTION: STBC 0x007365b0 _$E453
// DATA: STBC 0x0099da58
TGEventHandlerTable TGPrompt::classHandlers;
