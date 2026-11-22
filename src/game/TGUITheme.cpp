/*
===============================================================================

	TGUITheme.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGUITheme.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00745340 _$E32
// FUNCTION: STBC 0x00745350 _$E29
// FUNCTION: STBC 0x00745360 _$E31
// FUNCTION: STBC 0x00745370 _$E30
// FUNCTION: STBC 0x00745380 _$E38
// FUNCTION: STBC 0x00745390 _$E35
// FUNCTION: STBC 0x007453a0 _$E37
// FUNCTION: STBC 0x007453b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007453c0 _$E45
// FUNCTION: STBC 0x007453d0 _$E44
// FUNCTION: STBC 0x007453e0 _$E49
// FUNCTION: STBC 0x007453f0 _$E48
// FUNCTION: STBC 0x00745400 _$E60
// FUNCTION: STBC 0x00745410 _$E59
// FUNCTION: STBC 0x00745420 _$E64
// FUNCTION: STBC 0x00745430 _$E63
// FUNCTION: STBC 0x00745440 _$E68
// FUNCTION: STBC 0x00745450 _$E67
// FUNCTION: STBC 0x00745460 _$E72
// FUNCTION: STBC 0x00745470 _$E71
// FUNCTION: STBC 0x00745480 _$E196
// FUNCTION: STBC 0x00745490 _$E195
// FUNCTION: STBC 0x007454a0 _$E138
// FUNCTION: STBC 0x007454b0 _$E137
// FUNCTION: STBC 0x007454c0 _$E151
// FUNCTION: STBC 0x007454d0 _$E150
// FUNCTION: STBC 0x007454e0 _$E201
// FUNCTION: STBC 0x007454f0 _$E200
// FUNCTION: STBC 0x00745500 _$E206
// FUNCTION: STBC 0x00745510 _$E205
// FUNCTION: STBC 0x00745520 _$E210
// FUNCTION: STBC 0x00745530 _$E209
// FUNCTION: STBC 0x00745540 _$E214
// FUNCTION: STBC 0x00745550 _$E213
// FUNCTION: STBC 0x00745560 _$E221
// FUNCTION: STBC 0x00745570 _$E220
// FUNCTION: STBC 0x00745580 _$E246
// FUNCTION: STBC 0x00745590 _$E245
// FUNCTION: STBC 0x007455a0 _$E250
// FUNCTION: STBC 0x007455b0 _$E249
// FUNCTION: STBC 0x007455c0 _$E96
// FUNCTION: STBC 0x007455d0 _$E95
// FUNCTION: STBC 0x007455e0 _$E232
// FUNCTION: STBC 0x007455f0 _$E231
// FUNCTION: STBC 0x00745600 _$E239
// FUNCTION: STBC 0x00745610 _$E238
// FUNCTION: STBC 0x00745620 _$E262
// FUNCTION: STBC 0x00745630 _$E261
// FUNCTION: STBC 0x00745640 _$E266
// FUNCTION: STBC 0x00745650 _$E265
// FUNCTION: STBC 0x00745660 _$E306
// FUNCTION: STBC 0x00745670 _$E305
// FUNCTION: STBC 0x00745680 _$E312
// FUNCTION: STBC 0x00745690 _$E311
// FUNCTION: STBC 0x007456a0 _$E254
// FUNCTION: STBC 0x007456b0 _$E253
// FUNCTION: STBC 0x007456c0 _$E258
// FUNCTION: STBC 0x007456d0 _$E257
// FUNCTION: STBC 0x007456e0 _$E270
// FUNCTION: STBC 0x007456f0 _$E269
// FUNCTION: STBC 0x00745700 _$E274
// FUNCTION: STBC 0x00745710 _$E273
// FUNCTION: STBC 0x00745720 _$E278
// FUNCTION: STBC 0x00745730 _$E277
// FUNCTION: STBC 0x00745740 _$E282
// FUNCTION: STBC 0x00745750 _$E281
// FUNCTION: STBC 0x00745760 _$E286
// FUNCTION: STBC 0x00745770 _$E285
// FUNCTION: STBC 0x00745780 _$E290
// FUNCTION: STBC 0x00745790 _$E289
// FUNCTION: STBC 0x007457a0 _$E294
// FUNCTION: STBC 0x007457b0 _$E293
// FUNCTION: STBC 0x007457c0 _$E298
// FUNCTION: STBC 0x007457d0 _$E297
// FUNCTION: STBC 0x007457e0 _$E101
// FUNCTION: STBC 0x007457f0 _$E100
// FUNCTION: STBC 0x00745800 _$E302
// FUNCTION: STBC 0x00745810 _$E301
// FUNCTION: STBC 0x00745820 _$E106
// FUNCTION: STBC 0x00745830 _$E105
// FUNCTION: STBC 0x00745840 _$E316
// FUNCTION: STBC 0x00745850 _$E315
// FUNCTION: STBC 0x00745860 _$E320
// FUNCTION: STBC 0x00745870 _$E319
// FUNCTION: STBC 0x00745880 _$E324
// FUNCTION: STBC 0x00745890 _$E323
// FUNCTION: STBC 0x007458a0 _$E328
// FUNCTION: STBC 0x007458b0 _$E327
// FUNCTION: STBC 0x007458c0 _$E115
// FUNCTION: STBC 0x007458d0 _$E114
// FUNCTION: STBC 0x007458e0 _$E332
// FUNCTION: STBC 0x007458f0 _$E331
// FUNCTION: STBC 0x00745900 _$E336
// FUNCTION: STBC 0x00745910 _$E335
// FUNCTION: STBC 0x00745920 _$E340
// FUNCTION: STBC 0x00745930 _$E339
// FUNCTION: STBC 0x00745940 _$E121
// FUNCTION: STBC 0x00745950 _$E120
// FUNCTION: STBC 0x00745960 _$E126
// FUNCTION: STBC 0x00745970 _$E125
// FUNCTION: STBC 0x00745980 _$E156
// FUNCTION: STBC 0x00745990 _$E155
// FUNCTION: STBC 0x007459a0 _$E344
// FUNCTION: STBC 0x007459b0 _$E343
// FUNCTION: STBC 0x007459c0 _$E161
// FUNCTION: STBC 0x007459d0 _$E160
// FUNCTION: STBC 0x007459e0 _$E348
// FUNCTION: STBC 0x007459f0 _$E347
// FUNCTION: STBC 0x00745a00 _$E352
// FUNCTION: STBC 0x00745a10 _$E351
// FUNCTION: STBC 0x00745a20 _$E166
// FUNCTION: STBC 0x00745a30 _$E165
// FUNCTION: STBC 0x00745a40 _$E356
// FUNCTION: STBC 0x00745a50 _$E355
// FUNCTION: STBC 0x00745a60 _$E131
// FUNCTION: STBC 0x00745a70 _$E130
// FUNCTION: STBC 0x00745a80 _$E360
// FUNCTION: STBC 0x00745a90 _$E359
// FUNCTION: STBC 0x00745aa0 _$E364
// FUNCTION: STBC 0x00745ab0 _$E363
// FUNCTION: STBC 0x00745ac0 _$E171
// FUNCTION: STBC 0x00745ad0 _$E170
// FUNCTION: STBC 0x00745ae0 _$E176
// FUNCTION: STBC 0x00745af0 _$E175
// FUNCTION: STBC 0x00745b00 _$E368
// FUNCTION: STBC 0x00745b10 _$E367
// FUNCTION: STBC 0x00745b20 _$E372
// FUNCTION: STBC 0x00745b30 _$E371
// FUNCTION: STBC 0x00745b40 _$E376
// FUNCTION: STBC 0x00745b50 _$E375
// FUNCTION: STBC 0x00745b60 _$E380
// FUNCTION: STBC 0x00745b70 _$E379
// FUNCTION: STBC 0x00745b80 _$E384
// FUNCTION: STBC 0x00745b90 _$E383
// FUNCTION: STBC 0x00745ba0 _$E388
// FUNCTION: STBC 0x00745bb0 _$E387
// FUNCTION: STBC 0x00745bc0 _$E392
// FUNCTION: STBC 0x00745bd0 _$E391
// FUNCTION: STBC 0x00745be0 _$E396
// FUNCTION: STBC 0x00745bf0 _$E395
// FUNCTION: STBC 0x00745c00 _$E181
// FUNCTION: STBC 0x00745c10 _$E180
// FUNCTION: STBC 0x00745c20 _$E400
// FUNCTION: STBC 0x00745c30 _$E399
// FUNCTION: STBC 0x00745c40 _$E186
// FUNCTION: STBC 0x00745c50 _$E185
// FUNCTION: STBC 0x00745c60 _$E191
// FUNCTION: STBC 0x00745c70 _$E190

/*
================
TGUITheme::TGUITheme
================
*/
// FUNCTION: STBC 0x00745c80
TGUITheme::TGUITheme( char *name ) {
	themeName = NULL;
	windowFrameScale = 0.0f;
	windowFrameName = NULL;
	buttonFrameScale = 0.0f;
	buttonFrameName = NULL;
	dialogFrameScale = 0.0f;
	dialogFrameName = NULL;
	SetThemeName( name );
}

/*
================
TGUITheme::~TGUITheme
================
*/
// FUNCTION: STBC 0x00745cd0
TGUITheme::~TGUITheme( void ) {
	// BUG: the theme's name is never freed
	if ( buttonFrameName ) {
		delete [] buttonFrameName;
		buttonFrameName = NULL;
	}
	if ( dialogFrameName ) {
		delete [] dialogFrameName;
		dialogFrameName = NULL;
	}
	if ( windowFrameName ) {
		delete [] windowFrameName;
		windowFrameName = NULL;
	}
}

/*
================
TGUITheme::SetWindowFrameName
================
*/
// FUNCTION: STBC 0x00745d20
void TGUITheme::SetWindowFrameName( char *name ) {
	if ( windowFrameName ) {
		delete [] windowFrameName;
		windowFrameName = NULL;
	}
	if ( name ) {
		windowFrameName = new char[strlen( name ) + 1];
		strcpy( windowFrameName, name );
	}
}

/*
================
TGUITheme::SetButtonFrameName
================
*/
// FUNCTION: STBC 0x00745d90
void TGUITheme::SetButtonFrameName( char *name ) {
	if ( buttonFrameName ) {
		delete [] buttonFrameName;
		buttonFrameName = NULL;
	}
	if ( name ) {
		buttonFrameName = new char[strlen( name ) + 1];
		strcpy( buttonFrameName, name );
	}
}

/*
================
TGUITheme::SetDialogFrameName
================
*/
// FUNCTION: STBC 0x00745e00
void TGUITheme::SetDialogFrameName( char *name ) {
	if ( dialogFrameName ) {
		delete [] dialogFrameName;
		dialogFrameName = NULL;
	}
	if ( name ) {
		dialogFrameName = new char[strlen( name ) + 1];
		strcpy( dialogFrameName, name );
	}
}

/*
================
TGUITheme::Copy

Takes the other theme's frames and sizes, and its name.
================
*/
// FUNCTION: STBC 0x00745e70
void TGUITheme::Copy( TGUITheme *other ) {
	SetButtonFrameName( other->buttonFrameName );
	buttonFrameScale = other->GetButtonFrameScale();
	SetDialogFrameName( other->dialogFrameName );
	dialogFrameScale = other->GetDialogFrameScale();
	SetWindowFrameName( other->windowFrameName );
	windowFrameScale = other->GetWindowFrameScale();
	SetThemeName( other->themeName );
}

/*
================
TGUITheme::SetThemeName
================
*/
// FUNCTION: STBC 0x00745ec0
void TGUITheme::SetThemeName( char *name ) {
	if ( themeName ) {
		delete [] themeName;
		themeName = NULL;
	}
	if ( name ) {
		themeName = new char[strlen( name ) + 1];
		strcpy( themeName, name );
	}
}

/*
================
TGUITheme::Create
================
*/
// FUNCTION: STBC 0x00745f30
TGUITheme *TGUITheme::Create( char *name ) {
	return new TGUITheme( name );
}

/*
================
TGUITheme::WriteToStream

Each string is its length, then its characters with the terminator (a
length of 0 for none); each size follows its frame's name.
================
*/
// FUNCTION: STBC 0x00745f90
bool TGUITheme::WriteToStream( TGStream *stream ) {
	if ( themeName ) {
		stream->WriteLong( strlen( themeName ) );
		stream->Write( themeName, strlen( themeName ) + 1 );
	} else {
		stream->WriteLong( 0 );
	}
	stream->WriteFloat( windowFrameScale );
	if ( windowFrameName ) {
		stream->WriteLong( strlen( windowFrameName ) );
		stream->Write( windowFrameName, strlen( windowFrameName ) + 1 );
	} else {
		stream->WriteLong( 0 );
	}
	stream->WriteFloat( buttonFrameScale );
	if ( buttonFrameName ) {
		stream->WriteLong( strlen( buttonFrameName ) );
		stream->Write( buttonFrameName, strlen( buttonFrameName ) + 1 );
	} else {
		stream->WriteLong( 0 );
	}
	stream->WriteFloat( dialogFrameScale );
	if ( dialogFrameName ) {
		stream->WriteLong( strlen( dialogFrameName ) );
		stream->Write( dialogFrameName, strlen( dialogFrameName ) + 1 );
	} else {
		stream->WriteLong( 0 );
	}
	return true;
}

/*
================
TGUITheme::ReadFromStream
================
*/
// FUNCTION: STBC 0x007460c0
bool TGUITheme::ReadFromStream( TGStream *stream ) {
	long length;

	length = stream->ReadLong();
	if ( length != 0 ) {
		themeName = new char[length + 1];
		stream->Read( themeName, length + 1 );
	} else {
		themeName = NULL;
	}
	windowFrameScale = stream->ReadFloat();
	length = stream->ReadLong();
	if ( length != 0 ) {
		windowFrameName = new char[length + 1];
		stream->Read( windowFrameName, length + 1 );
	} else {
		windowFrameName = NULL;
	}
	buttonFrameScale = stream->ReadFloat();
	length = stream->ReadLong();
	if ( length != 0 ) {
		buttonFrameName = new char[length + 1];
		stream->Read( buttonFrameName, length + 1 );
	} else {
		buttonFrameName = NULL;
	}
	dialogFrameScale = stream->ReadFloat();
	length = stream->ReadLong();
	if ( length != 0 ) {
		dialogFrameName = new char[length + 1];
		stream->Read( dialogFrameName, length + 1 );
	} else {
		dialogFrameName = NULL;
	}
	return true;
}
