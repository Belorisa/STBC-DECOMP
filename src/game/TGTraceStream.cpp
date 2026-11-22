/*
===============================================================================

	TGTraceStream.cpp

	The trace stream's configuration file and its outputs.

===============================================================================
*/

// <iostream>'s statics: the ios_base::Init and _Winit quads and the locale
// id pair.
// FUNCTION: STBC 0x00723670 _$E32
// FUNCTION: STBC 0x00723680 _$E29
// FUNCTION: STBC 0x00723690 _$E31
// FUNCTION: STBC 0x007236a0 _$E30
// FUNCTION: STBC 0x007236b0 _$E38
// FUNCTION: STBC 0x007236c0 _$E35
// FUNCTION: STBC 0x007236d0 _$E37
// FUNCTION: STBC 0x007236e0 _$E36
// FUNCTION: STBC 0x007238a0 _$E443
// FUNCTION: STBC 0x007238c0 _$E442
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "TGTraceStream.h"
#include "TGSystemWrapperClass.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00723720 _$E69
// FUNCTION: STBC 0x00723730 _$E68
// FUNCTION: STBC 0x00723740 _$E73
// FUNCTION: STBC 0x00723750 _$E72
// FUNCTION: STBC 0x00723800 _$E84
// FUNCTION: STBC 0x00723810 _$E83
// FUNCTION: STBC 0x00723820 _$E88
// FUNCTION: STBC 0x00723830 _$E87
// FUNCTION: STBC 0x00723840 _$E92
// FUNCTION: STBC 0x00723850 _$E91
// FUNCTION: STBC 0x00723860 _$E96
// FUNCTION: STBC 0x00723870 _$E95
// FUNCTION: STBC 0x00723880 _$E220
// FUNCTION: STBC 0x00723890 _$E219
// FUNCTION: STBC 0x007238d0 _$E162
// FUNCTION: STBC 0x007238e0 _$E161
// FUNCTION: STBC 0x007238f0 _$E175
// FUNCTION: STBC 0x00723900 _$E174
// FUNCTION: STBC 0x00723910 _$E225
// FUNCTION: STBC 0x00723920 _$E224
// FUNCTION: STBC 0x00723930 _$E230
// FUNCTION: STBC 0x00723940 _$E229
// FUNCTION: STBC 0x00723950 _$E234
// FUNCTION: STBC 0x00723960 _$E233
// FUNCTION: STBC 0x00723970 _$E238
// FUNCTION: STBC 0x00723980 _$E237
// FUNCTION: STBC 0x00723990 _$E245
// FUNCTION: STBC 0x007239a0 _$E244
// FUNCTION: STBC 0x007239b0 _$E270
// FUNCTION: STBC 0x007239c0 _$E269
// FUNCTION: STBC 0x007239d0 _$E274
// FUNCTION: STBC 0x007239e0 _$E273
// FUNCTION: STBC 0x007239f0 _$E120
// FUNCTION: STBC 0x00723a00 _$E119
// FUNCTION: STBC 0x00723a10 _$E256
// FUNCTION: STBC 0x00723a20 _$E255
// FUNCTION: STBC 0x00723a30 _$E263
// FUNCTION: STBC 0x00723a40 _$E262
// FUNCTION: STBC 0x00723a50 _$E286
// FUNCTION: STBC 0x00723a60 _$E285
// FUNCTION: STBC 0x00723a70 _$E290
// FUNCTION: STBC 0x00723a80 _$E289
// FUNCTION: STBC 0x00723a90 _$E330
// FUNCTION: STBC 0x00723aa0 _$E329
// FUNCTION: STBC 0x00723ab0 _$E336
// FUNCTION: STBC 0x00723ac0 _$E335
// FUNCTION: STBC 0x00723ad0 _$E278
// FUNCTION: STBC 0x00723ae0 _$E277
// FUNCTION: STBC 0x00723af0 _$E282
// FUNCTION: STBC 0x00723b00 _$E281
// FUNCTION: STBC 0x00723b10 _$E294
// FUNCTION: STBC 0x00723b20 _$E293
// FUNCTION: STBC 0x00723b30 _$E298
// FUNCTION: STBC 0x00723b40 _$E297
// FUNCTION: STBC 0x00723b50 _$E302
// FUNCTION: STBC 0x00723b60 _$E301
// FUNCTION: STBC 0x00723b70 _$E306
// FUNCTION: STBC 0x00723b80 _$E305
// FUNCTION: STBC 0x00723b90 _$E310
// FUNCTION: STBC 0x00723ba0 _$E309
// FUNCTION: STBC 0x00723bb0 _$E314
// FUNCTION: STBC 0x00723bc0 _$E313
// FUNCTION: STBC 0x00723bd0 _$E318
// FUNCTION: STBC 0x00723be0 _$E317
// FUNCTION: STBC 0x00723bf0 _$E322
// FUNCTION: STBC 0x00723c00 _$E321
// FUNCTION: STBC 0x00723c10 _$E125
// FUNCTION: STBC 0x00723c20 _$E124
// FUNCTION: STBC 0x00723c30 _$E326
// FUNCTION: STBC 0x00723c40 _$E325
// FUNCTION: STBC 0x00723c50 _$E130
// FUNCTION: STBC 0x00723c60 _$E129
// FUNCTION: STBC 0x00723c70 _$E340
// FUNCTION: STBC 0x00723c80 _$E339
// FUNCTION: STBC 0x00723c90 _$E344
// FUNCTION: STBC 0x00723ca0 _$E343
// FUNCTION: STBC 0x00723cb0 _$E348
// FUNCTION: STBC 0x00723cc0 _$E347
// FUNCTION: STBC 0x00723cd0 _$E352
// FUNCTION: STBC 0x00723ce0 _$E351
// FUNCTION: STBC 0x00723cf0 _$E139
// FUNCTION: STBC 0x00723d00 _$E138
// FUNCTION: STBC 0x00723d10 _$E356
// FUNCTION: STBC 0x00723d20 _$E355
// FUNCTION: STBC 0x00723d30 _$E360
// FUNCTION: STBC 0x00723d40 _$E359
// FUNCTION: STBC 0x00723d50 _$E364
// FUNCTION: STBC 0x00723d60 _$E363
// FUNCTION: STBC 0x00723d70 _$E145
// FUNCTION: STBC 0x00723d80 _$E144
// FUNCTION: STBC 0x00723d90 _$E150
// FUNCTION: STBC 0x00723da0 _$E149
// FUNCTION: STBC 0x00723db0 _$E180
// FUNCTION: STBC 0x00723dc0 _$E179
// FUNCTION: STBC 0x00723dd0 _$E368
// FUNCTION: STBC 0x00723de0 _$E367
// FUNCTION: STBC 0x00723df0 _$E185
// FUNCTION: STBC 0x00723e00 _$E184
// FUNCTION: STBC 0x00723e10 _$E372
// FUNCTION: STBC 0x00723e20 _$E371
// FUNCTION: STBC 0x00723e30 _$E376
// FUNCTION: STBC 0x00723e40 _$E375
// FUNCTION: STBC 0x00723e50 _$E190
// FUNCTION: STBC 0x00723e60 _$E189
// FUNCTION: STBC 0x00723e70 _$E380
// FUNCTION: STBC 0x00723e80 _$E379
// FUNCTION: STBC 0x00723e90 _$E155
// FUNCTION: STBC 0x00723ea0 _$E154
// FUNCTION: STBC 0x00723eb0 _$E384
// FUNCTION: STBC 0x00723ec0 _$E383
// FUNCTION: STBC 0x00723ed0 _$E388
// FUNCTION: STBC 0x00723ee0 _$E387
// FUNCTION: STBC 0x00723ef0 _$E195
// FUNCTION: STBC 0x00723f00 _$E194
// FUNCTION: STBC 0x00723f10 _$E200
// FUNCTION: STBC 0x00723f20 _$E199
// FUNCTION: STBC 0x00723f30 _$E392
// FUNCTION: STBC 0x00723f40 _$E391
// FUNCTION: STBC 0x00723f50 _$E396
// FUNCTION: STBC 0x00723f60 _$E395
// FUNCTION: STBC 0x00723f70 _$E400
// FUNCTION: STBC 0x00723f80 _$E399
// FUNCTION: STBC 0x00723f90 _$E404
// FUNCTION: STBC 0x00723fa0 _$E403
// FUNCTION: STBC 0x00723fb0 _$E408
// FUNCTION: STBC 0x00723fc0 _$E407
// FUNCTION: STBC 0x00723fd0 _$E412
// FUNCTION: STBC 0x00723fe0 _$E411
// FUNCTION: STBC 0x00723ff0 _$E416
// FUNCTION: STBC 0x00724000 _$E415
// FUNCTION: STBC 0x00724010 _$E420
// FUNCTION: STBC 0x00724020 _$E419
// FUNCTION: STBC 0x00724030 _$E205
// FUNCTION: STBC 0x00724040 _$E204
// FUNCTION: STBC 0x00724050 _$E424
// FUNCTION: STBC 0x00724060 _$E423
// FUNCTION: STBC 0x00724070 _$E210
// FUNCTION: STBC 0x00724080 _$E209
// FUNCTION: STBC 0x00724090 _$E215
// FUNCTION: STBC 0x007240a0 _$E214

// The type maps' construction and exit destruction.
// FUNCTION: STBC 0x00722c20 _$E430
// FUNCTION: STBC 0x00722c30 _$E426
// FUNCTION: STBC 0x00722c40 _$E429
// FUNCTION: STBC 0x00722c50 _$E427
// FUNCTION: STBC 0x00722c70 _$E435
// FUNCTION: STBC 0x00722c80 _$E432
// FUNCTION: STBC 0x00722c90 _$E434
// FUNCTION: STBC 0x00722ca0 _$E433
// DATA: STBC 0x0099cc58 ?shownTypes@TGTraceStream@@1V?$NiTStringMap@_N@@A
NiTStringMap<bool> TGTraceStream::shownTypes;
// DATA: STBC 0x0099ccd8 ?excludedTypes@TGTraceStream@@1V?$NiTStringMap@_N@@A
NiTStringMap<bool> TGTraceStream::excludedTypes;

// DATA: STBC 0x0099cd88 ?numOutputs@TGTraceStream@@1HA
int TGTraceStream::numOutputs;
// DATA: STBC 0x0099cd8c ?outputs@TGTraceStream@@1PAPAVTGTraceOutput@@A
TGTraceOutput **TGTraceStream::outputs;

// DATA: STBC 0x0095cce8
static char *configFile = "TGTraceStream.cfg";

/*
================
TGTraceStream::LoadConfiguration

Without the file there are no outputs.
================
*/
// FUNCTION: STBC 0x00722cc0
void TGTraceStream::LoadConfiguration( void ) {
	FILE *file = g_kSystemWrapper.FileOpen( configFile, "r" );
	if ( file == NULL ) {
		numOutputs = 0;
		outputs = NULL;
		return;
	}

	char line[1024];
	while ( fgets( line, sizeof( line ), file ) ) {
		char *comment = strchr( line, '#' );
		if ( comment ) {
			*comment = '\0';
		}

		if ( strncmp( line, "Outputs", strlen( "Outputs" ) ) == 0 ) {
			char *colon = strchr( line, ':' );
			if ( colon ) {
				SetOutputs( colon + 1 );
			}
		} else if ( strncmp( line, "ShowType", strlen( "ShowType" ) ) == 0 ) {
			char *colon = strchr( line, ':' );
			if ( colon ) {
				AddShownType( colon + 1 );
			}
		} else if ( strncmp( line, "ExcludeType", strlen( "ExcludeType" ) ) == 0 ) {
			char *colon = strchr( line, ':' );
			if ( colon ) {
				AddExcludedType( colon + 1 );
			}
		}
	}

	g_kSystemWrapper.FileClose( file );
}

/*
================
TGTraceStream::DeleteOutputs
================
*/
// FUNCTION: STBC 0x00722e30
void TGTraceStream::DeleteOutputs( void ) {
	while ( --numOutputs >= 0 ) {
		delete outputs[numOutputs];
	}
	delete [] outputs;
}

/*
================
TGTraceStream::IsTypeShown
================
*/
// FUNCTION: STBC 0x00722e70
bool TGTraceStream::IsTypeShown( const char *type ) {
	bool value;
	if ( shownTypes.GetCount() != 0 && !shownTypes.GetAt( type, value ) ) {
		return false;
	}
	if ( excludedTypes.GetCount() != 0 && excludedTypes.GetAt( type, value ) ) {
		return false;
	}
	return true;
}

/*
================
TGTraceStream::SetOutputs

Makes an output for each comma-separated name the list holds; the list is
cut in place while it is read and put back after.
================
*/
// FUNCTION: STBC 0x00722f10
// DRAFT: register allocation: retail keeps count in the list parameter's slot and comma in ebx; the DebugWindow and Console news share one tail
void TGTraceStream::SetOutputs( char *list ) {
	int count = 0;
	numOutputs = 0;
	char *comma = list;
	while ( comma ) {
		comma = strchr( comma + 1, ',' );
		numOutputs++;
	}
	outputs = new TGTraceOutput *[numOutputs];

	char *name = list;
	while ( name ) {
		if ( *name == '\0' ) {
			return;
		}

		comma = strchr( name, ',' );
		char *next = NULL;
		if ( comma ) {
			next = comma + 1;
			*comma = '\0';
		}

		while ( *name && strchr( " \t", *name ) ) {
			name++;
		}

		if ( strncmp( name, "DebugWindow", strlen( "DebugWindow" ) ) == 0 ) {
			outputs[count++] = new TGDebugWindowOutput;
		} else if ( strncmp( name, "LogFile", strlen( "LogFile" ) ) == 0 ) {
			char *open = strchr( name, '(' );
			char *close = strchr( name, ')' );
			if ( open && close ) {
				*close = '\0';
				outputs[count++] = new TGLogFileOutput( open + 1 );
				*close = ')';
			}
		} else if ( strncmp( name, "Console", strlen( "Console" ) ) == 0 ) {
			outputs[count++] = new TGConsoleOutput;
		} else if ( strncmp( name, "PythonFunc", strlen( "PythonFunc" ) ) == 0 ) {
			char *open = strchr( name, '(' );
			char *colon = strchr( name, ':' );
			char *close = strchr( name, ')' );
			if ( open && colon && close ) {
				*colon = '\0';
				*close = '\0';
				outputs[count++] = new TGPythonFuncOutput( open + 1, colon + 1 );
				*colon = ':';
				*close = ')';
			}
		}

		if ( comma ) {
			*comma = ',';
		}
		name = next;
	}
}

/*
================
TGTraceStream::AddShownType

Trims the blanks around the type first.
================
*/
// FUNCTION: STBC 0x00723230
void TGTraceStream::AddShownType( char *type ) {
	while ( *type && strchr( " \t", *type ) ) {
		type++;
	}
	for ( char *end = type + strlen( type ) - 1; end >= type && strchr( " \n\t", *end ); end-- ) {
		*end = '\0';
	}
	if ( *type ) {
		shownTypes.SetAt( type, true );
	}
}

/*
================
TGTraceStream::AddExcludedType
================
*/
// FUNCTION: STBC 0x00723330
void TGTraceStream::AddExcludedType( char *type ) {
	while ( *type && strchr( " \t", *type ) ) {
		type++;
	}
	for ( char *end = type + strlen( type ) - 1; end >= type && strchr( " \n\t", *end ); end-- ) {
		*end = '\0';
	}
	if ( *type ) {
		excludedTypes.SetAt( type, true );
	}
}
