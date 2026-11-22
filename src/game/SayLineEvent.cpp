/*
===============================================================================

	SayLineEvent.cpp

===============================================================================
*/

#include <string.h>

#include "BoneState.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiLinRotKey.h"
#include <iostream>
#include "SayLineEvent.h"
#include "CharacterClass.h"
#include "TGStream.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00675bb0 _$E464
// FUNCTION: STBC 0x00675bd0 _$E463

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00675870 _$E39
// FUNCTION: STBC 0x00675880 _$E38
// FUNCTION: STBC 0x00675890 _$E43
// FUNCTION: STBC 0x006758a0 _$E42
// FUNCTION: STBC 0x006758b0 _$E54
// FUNCTION: STBC 0x006758c0 _$E53
// FUNCTION: STBC 0x006758d0 _$E58
// FUNCTION: STBC 0x006758e0 _$E57
// FUNCTION: STBC 0x006758f0 _$E62
// FUNCTION: STBC 0x00675900 _$E61
// FUNCTION: STBC 0x00675910 _$E66
// FUNCTION: STBC 0x00675920 _$E65
// FUNCTION: STBC 0x00675930 _$E181
// FUNCTION: STBC 0x00675940 _$E180
// FUNCTION: STBC 0x00675950 _$E123
// FUNCTION: STBC 0x00675960 _$E122
// FUNCTION: STBC 0x00675970 _$E136
// FUNCTION: STBC 0x00675980 _$E135
// FUNCTION: STBC 0x00675990 _$E186
// FUNCTION: STBC 0x006759a0 _$E185
// FUNCTION: STBC 0x006759b0 _$E191
// FUNCTION: STBC 0x006759c0 _$E190
// FUNCTION: STBC 0x006759d0 _$E195
// FUNCTION: STBC 0x006759e0 _$E194
// FUNCTION: STBC 0x006759f0 _$E199
// FUNCTION: STBC 0x00675a00 _$E198
// FUNCTION: STBC 0x00675a10 _$E206
// FUNCTION: STBC 0x00675a20 _$E205
// FUNCTION: STBC 0x00675a30 _$E231
// FUNCTION: STBC 0x00675a40 _$E230
// FUNCTION: STBC 0x00675a50 _$E235
// FUNCTION: STBC 0x00675a60 _$E234
// FUNCTION: STBC 0x00675a70 _$E81
// FUNCTION: STBC 0x00675a80 _$E80
// FUNCTION: STBC 0x00675a90 _$E217
// FUNCTION: STBC 0x00675aa0 _$E216
// FUNCTION: STBC 0x00675ab0 _$E224
// FUNCTION: STBC 0x00675ac0 _$E223
// FUNCTION: STBC 0x00675ad0 _$E247
// FUNCTION: STBC 0x00675ae0 _$E246
// FUNCTION: STBC 0x00675af0 _$E251
// FUNCTION: STBC 0x00675b00 _$E250
// FUNCTION: STBC 0x00675b10 _$E291
// FUNCTION: STBC 0x00675b20 _$E290
// FUNCTION: STBC 0x00675b30 _$E297
// FUNCTION: STBC 0x00675b40 _$E296
// FUNCTION: STBC 0x00675b50 _$E239
// FUNCTION: STBC 0x00675b60 _$E238
// FUNCTION: STBC 0x00675b70 _$E243
// FUNCTION: STBC 0x00675b80 _$E242
// FUNCTION: STBC 0x00675b90 _$E255
// FUNCTION: STBC 0x00675ba0 _$E254
// FUNCTION: STBC 0x00675be0 _$E259
// FUNCTION: STBC 0x00675bf0 _$E258
// FUNCTION: STBC 0x00675c00 _$E263
// FUNCTION: STBC 0x00675c10 _$E262
// FUNCTION: STBC 0x00675c20 _$E267
// FUNCTION: STBC 0x00675c30 _$E266
// FUNCTION: STBC 0x00675c40 _$E271
// FUNCTION: STBC 0x00675c50 _$E270
// FUNCTION: STBC 0x00675c60 _$E275
// FUNCTION: STBC 0x00675c70 _$E274
// FUNCTION: STBC 0x00675c80 _$E279
// FUNCTION: STBC 0x00675c90 _$E278
// FUNCTION: STBC 0x00675ca0 _$E283
// FUNCTION: STBC 0x00675cb0 _$E282
// FUNCTION: STBC 0x00675cc0 _$E86
// FUNCTION: STBC 0x00675cd0 _$E85
// FUNCTION: STBC 0x00675ce0 _$E287
// FUNCTION: STBC 0x00675cf0 _$E286
// FUNCTION: STBC 0x00675d00 _$E91
// FUNCTION: STBC 0x00675d10 _$E90
// FUNCTION: STBC 0x00675d20 _$E301
// FUNCTION: STBC 0x00675d30 _$E300
// FUNCTION: STBC 0x00675d40 _$E305
// FUNCTION: STBC 0x00675d50 _$E304
// FUNCTION: STBC 0x00675d60 _$E309
// FUNCTION: STBC 0x00675d70 _$E308
// FUNCTION: STBC 0x00675d80 _$E313
// FUNCTION: STBC 0x00675d90 _$E312
// FUNCTION: STBC 0x00675da0 _$E100
// FUNCTION: STBC 0x00675db0 _$E99
// FUNCTION: STBC 0x00675dc0 _$E317
// FUNCTION: STBC 0x00675dd0 _$E316
// FUNCTION: STBC 0x00675de0 _$E321
// FUNCTION: STBC 0x00675df0 _$E320
// FUNCTION: STBC 0x00675e00 _$E325
// FUNCTION: STBC 0x00675e10 _$E324
// FUNCTION: STBC 0x00675e20 _$E106
// FUNCTION: STBC 0x00675e30 _$E105
// FUNCTION: STBC 0x00675e40 _$E111
// FUNCTION: STBC 0x00675e50 _$E110
// FUNCTION: STBC 0x00675e60 _$E141
// FUNCTION: STBC 0x00675e70 _$E140
// FUNCTION: STBC 0x00675e80 _$E329
// FUNCTION: STBC 0x00675e90 _$E328
// FUNCTION: STBC 0x00675ea0 _$E146
// FUNCTION: STBC 0x00675eb0 _$E145
// FUNCTION: STBC 0x00675ec0 _$E333
// FUNCTION: STBC 0x00675ed0 _$E332
// FUNCTION: STBC 0x00675ee0 _$E337
// FUNCTION: STBC 0x00675ef0 _$E336
// FUNCTION: STBC 0x00675f00 _$E151
// FUNCTION: STBC 0x00675f10 _$E150
// FUNCTION: STBC 0x00675f20 _$E341
// FUNCTION: STBC 0x00675f30 _$E340
// FUNCTION: STBC 0x00675f40 _$E116
// FUNCTION: STBC 0x00675f50 _$E115
// FUNCTION: STBC 0x00675f60 _$E345
// FUNCTION: STBC 0x00675f70 _$E344
// FUNCTION: STBC 0x00675f80 _$E349
// FUNCTION: STBC 0x00675f90 _$E348
// FUNCTION: STBC 0x00675fa0 _$E156
// FUNCTION: STBC 0x00675fb0 _$E155
// FUNCTION: STBC 0x00675fc0 _$E161
// FUNCTION: STBC 0x00675fd0 _$E160
// FUNCTION: STBC 0x00675fe0 _$E353
// FUNCTION: STBC 0x00675ff0 _$E352
// FUNCTION: STBC 0x00676000 _$E357
// FUNCTION: STBC 0x00676010 _$E356
// FUNCTION: STBC 0x00676020 _$E361
// FUNCTION: STBC 0x00676030 _$E360
// FUNCTION: STBC 0x00676040 _$E365
// FUNCTION: STBC 0x00676050 _$E364
// FUNCTION: STBC 0x00676060 _$E369
// FUNCTION: STBC 0x00676070 _$E368
// FUNCTION: STBC 0x00676080 _$E373
// FUNCTION: STBC 0x00676090 _$E372
// FUNCTION: STBC 0x006760a0 _$E377
// FUNCTION: STBC 0x006760b0 _$E376
// FUNCTION: STBC 0x006760c0 _$E381
// FUNCTION: STBC 0x006760d0 _$E380
// FUNCTION: STBC 0x006760e0 _$E166
// FUNCTION: STBC 0x006760f0 _$E165
// FUNCTION: STBC 0x00676100 _$E385
// FUNCTION: STBC 0x00676110 _$E384
// FUNCTION: STBC 0x00676120 _$E171
// FUNCTION: STBC 0x00676130 _$E170
// FUNCTION: STBC 0x00676140 _$E176
// FUNCTION: STBC 0x00676150 _$E175
// FUNCTION: STBC 0x00676160 _$E389
// FUNCTION: STBC 0x00676170 _$E388
// FUNCTION: STBC 0x00676180 _$E394
// FUNCTION: STBC 0x00676190 _$E393
// FUNCTION: STBC 0x006761a0 _$E398
// FUNCTION: STBC 0x006761b0 _$E397

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006757f0 _$E419
// FUNCTION: STBC 0x00675800 _$E416
// FUNCTION: STBC 0x00675810 _$E418
// FUNCTION: STBC 0x00675820 _$E417
// FUNCTION: STBC 0x00675830 _$E425
// FUNCTION: STBC 0x00675840 _$E422
// FUNCTION: STBC 0x00675850 _$E424
// FUNCTION: STBC 0x00675860 _$E423

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).

/*
================
SayLineEvent::SayLineEvent
================
*/
// FUNCTION: STBC 0x00675550 ??0SayLineEvent@@QAE@J@Z
SayLineEvent::SayLineEvent( long id ) : TGEvent( id ) {
	unknown2C = 0;
	line = NULL;
}

/*
================
SayLineEvent::GetObjType
================
*/
// FUNCTION: STBC 0x00675580
int SayLineEvent::GetObjType( void ) {
	return SAYLINEEVENT_TYPE;
}

/*
================
SayLineEvent::IsTypeOf
================
*/
// FUNCTION: STBC 0x00675590
bool SayLineEvent::IsTypeOf( int type ) {
	if ( type == SAYLINEEVENT_TYPE ) {
		return true;
	}
	return TGEvent::IsTypeOf( type );
}

/*
================
SayLineEvent::GetClassName
================
*/
// FUNCTION: STBC 0x006755c0
char *SayLineEvent::GetClassName( void ) {
	return "SayLineEvent";
}

/*
================
SayLineEvent::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x006755d0
char *SayLineEvent::GetSwigTypeName( void ) {
	return "_p_SayLineEvent";
}

/*
================
SayLineEvent::GetPythonClassName
================
*/
// FUNCTION: STBC 0x006755e0
char *SayLineEvent::GetPythonClassName( void ) {
	return "SayLineEventPtr";
}

/*
================
SayLineEvent::SayLineEvent
================
*/
// FUNCTION: STBC 0x00675620 ??0SayLineEvent@@QAE@XZ
SayLineEvent::SayLineEvent( void ) : TGEvent( 0 ) {
	unknown2C = 0;
	line = NULL;
}

/*
================
SayLineEvent::~SayLineEvent
================
*/
// FUNCTION: STBC 0x00675640
SayLineEvent::~SayLineEvent( void ) {
	if ( line != NULL ) {
		delete [] line;
		line = NULL;
	}
}

/*
================
SayLineEvent::Copy
================
*/
// FUNCTION: STBC 0x00675670
bool SayLineEvent::Copy( TGEvent *event ) {
	TGEvent::Copy( event );
	unknown2C = ( (SayLineEvent *)event )->unknown2C;
	SetLine( ( (SayLineEvent *)event )->line );
	return true;
}

/*
================
SayLineEvent::SetLine

Keeps a copy of the line's name.
================
*/
// FUNCTION: STBC 0x006756a0
void SayLineEvent::SetLine( char *line_ ) {
	if ( line != NULL ) {
		delete [] line;
		line = NULL;
	}
	if ( line_ != NULL ) {
		line = new char[strlen( line_ ) + 1];
		strcpy( line, line_ );
	}
}

/*
================
SayLineEvent::WriteToStream

The length and the bytes of the line's name, then the flag.
================
*/
// FUNCTION: STBC 0x00675710
bool SayLineEvent::WriteToStream( TGStream *stream ) {
	if ( !TGEvent::WriteToStream( stream ) ) {
		return false;
	}

	if ( line != NULL ) {
		stream->WriteLong( strlen( line ) );
		stream->Write( line, strlen( line ) + 1 );
	} else {
		stream->WriteLong( 0 );
	}
	stream->WriteChar( unknown2C );
	return true;
}

/*
================
SayLineEvent::ReadFromStream

Reads what WriteToStream wrote.
================
*/
// FUNCTION: STBC 0x00675790
bool SayLineEvent::ReadFromStream( TGStream *stream ) {
	if ( !TGEvent::ReadFromStream( stream ) ) {
		return false;
	}

	line = NULL;
	unknown2C = 0;
	int length = stream->ReadLong();
	if ( length != 0 ) {
		line = new char[length + 1];
		stream->Read( line, length + 1 );
	}
	if ( stream->ReadChar() ) {
		unknown2C = 1;
	}
	return true;
}
