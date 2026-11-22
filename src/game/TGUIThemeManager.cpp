/*
===============================================================================

	TGUIThemeManager.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>
#include <new>

#include "TGUIThemeManager.h"
#include "TGUITheme.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x007461d0 _$E32
// FUNCTION: STBC 0x007461e0 _$E29
// FUNCTION: STBC 0x007461f0 _$E31
// FUNCTION: STBC 0x00746200 _$E30
// FUNCTION: STBC 0x00746210 _$E38
// FUNCTION: STBC 0x00746220 _$E35
// FUNCTION: STBC 0x00746230 _$E37
// FUNCTION: STBC 0x00746240 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00746250 _$E45
// FUNCTION: STBC 0x00746260 _$E44
// FUNCTION: STBC 0x00746270 _$E49
// FUNCTION: STBC 0x00746280 _$E48
// FUNCTION: STBC 0x00746290 _$E60
// FUNCTION: STBC 0x007462a0 _$E59
// FUNCTION: STBC 0x007462b0 _$E64
// FUNCTION: STBC 0x007462c0 _$E63
// FUNCTION: STBC 0x007462d0 _$E68
// FUNCTION: STBC 0x007462e0 _$E67
// FUNCTION: STBC 0x007462f0 _$E72
// FUNCTION: STBC 0x00746300 _$E71
// FUNCTION: STBC 0x00746310 _$E196
// FUNCTION: STBC 0x00746320 _$E195
// FUNCTION: STBC 0x00746330 _$E138
// FUNCTION: STBC 0x00746340 _$E137
// FUNCTION: STBC 0x00746350 _$E151
// FUNCTION: STBC 0x00746360 _$E150
// FUNCTION: STBC 0x00746370 _$E201
// FUNCTION: STBC 0x00746380 _$E200
// FUNCTION: STBC 0x00746390 _$E206
// FUNCTION: STBC 0x007463a0 _$E205
// FUNCTION: STBC 0x007463b0 _$E210
// FUNCTION: STBC 0x007463c0 _$E209
// FUNCTION: STBC 0x007463d0 _$E214
// FUNCTION: STBC 0x007463e0 _$E213
// FUNCTION: STBC 0x007463f0 _$E221
// FUNCTION: STBC 0x00746400 _$E220
// FUNCTION: STBC 0x00746410 _$E246
// FUNCTION: STBC 0x00746420 _$E245
// FUNCTION: STBC 0x00746430 _$E250
// FUNCTION: STBC 0x00746440 _$E249
// FUNCTION: STBC 0x00746450 _$E96
// FUNCTION: STBC 0x00746460 _$E95
// FUNCTION: STBC 0x00746470 _$E232
// FUNCTION: STBC 0x00746480 _$E231
// FUNCTION: STBC 0x00746490 _$E239
// FUNCTION: STBC 0x007464a0 _$E238
// FUNCTION: STBC 0x007464b0 _$E262
// FUNCTION: STBC 0x007464c0 _$E261
// FUNCTION: STBC 0x007464d0 _$E266
// FUNCTION: STBC 0x007464e0 _$E265
// FUNCTION: STBC 0x007464f0 _$E306
// FUNCTION: STBC 0x00746500 _$E305
// FUNCTION: STBC 0x00746510 _$E312
// FUNCTION: STBC 0x00746520 _$E311
// FUNCTION: STBC 0x00746530 _$E254
// FUNCTION: STBC 0x00746540 _$E253
// FUNCTION: STBC 0x00746550 _$E258
// FUNCTION: STBC 0x00746560 _$E257
// FUNCTION: STBC 0x00746570 _$E270
// FUNCTION: STBC 0x00746580 _$E269
// FUNCTION: STBC 0x00746590 _$E274
// FUNCTION: STBC 0x007465a0 _$E273
// FUNCTION: STBC 0x007465b0 _$E278
// FUNCTION: STBC 0x007465c0 _$E277
// FUNCTION: STBC 0x007465d0 _$E282
// FUNCTION: STBC 0x007465e0 _$E281
// FUNCTION: STBC 0x007465f0 _$E286
// FUNCTION: STBC 0x00746600 _$E285
// FUNCTION: STBC 0x00746610 _$E290
// FUNCTION: STBC 0x00746620 _$E289
// FUNCTION: STBC 0x00746630 _$E294
// FUNCTION: STBC 0x00746640 _$E293
// FUNCTION: STBC 0x00746650 _$E298
// FUNCTION: STBC 0x00746660 _$E297
// FUNCTION: STBC 0x00746670 _$E101
// FUNCTION: STBC 0x00746680 _$E100
// FUNCTION: STBC 0x00746690 _$E302
// FUNCTION: STBC 0x007466a0 _$E301
// FUNCTION: STBC 0x007466b0 _$E106
// FUNCTION: STBC 0x007466c0 _$E105
// FUNCTION: STBC 0x007466d0 _$E316
// FUNCTION: STBC 0x007466e0 _$E315
// FUNCTION: STBC 0x007466f0 _$E320
// FUNCTION: STBC 0x00746700 _$E319
// FUNCTION: STBC 0x00746710 _$E324
// FUNCTION: STBC 0x00746720 _$E323
// FUNCTION: STBC 0x00746730 _$E328
// FUNCTION: STBC 0x00746740 _$E327
// FUNCTION: STBC 0x00746750 _$E115
// FUNCTION: STBC 0x00746760 _$E114
// FUNCTION: STBC 0x00746770 _$E332
// FUNCTION: STBC 0x00746780 _$E331
// FUNCTION: STBC 0x00746790 _$E336
// FUNCTION: STBC 0x007467a0 _$E335
// FUNCTION: STBC 0x007467b0 _$E340
// FUNCTION: STBC 0x007467c0 _$E339
// FUNCTION: STBC 0x007467d0 _$E121
// FUNCTION: STBC 0x007467e0 _$E120
// FUNCTION: STBC 0x007467f0 _$E126
// FUNCTION: STBC 0x00746800 _$E125
// FUNCTION: STBC 0x00746810 _$E156
// FUNCTION: STBC 0x00746820 _$E155
// FUNCTION: STBC 0x00746830 _$E344
// FUNCTION: STBC 0x00746840 _$E343
// FUNCTION: STBC 0x00746850 _$E161
// FUNCTION: STBC 0x00746860 _$E160
// FUNCTION: STBC 0x00746870 _$E348
// FUNCTION: STBC 0x00746880 _$E347
// FUNCTION: STBC 0x00746890 _$E352
// FUNCTION: STBC 0x007468a0 _$E351
// FUNCTION: STBC 0x007468b0 _$E166
// FUNCTION: STBC 0x007468c0 _$E165
// FUNCTION: STBC 0x007468d0 _$E356
// FUNCTION: STBC 0x007468e0 _$E355
// FUNCTION: STBC 0x007468f0 _$E131
// FUNCTION: STBC 0x00746900 _$E130
// FUNCTION: STBC 0x00746910 _$E360
// FUNCTION: STBC 0x00746920 _$E359
// FUNCTION: STBC 0x00746930 _$E364
// FUNCTION: STBC 0x00746940 _$E363
// FUNCTION: STBC 0x00746950 _$E171
// FUNCTION: STBC 0x00746960 _$E170
// FUNCTION: STBC 0x00746970 _$E176
// FUNCTION: STBC 0x00746980 _$E175
// FUNCTION: STBC 0x00746990 _$E368
// FUNCTION: STBC 0x007469a0 _$E367
// FUNCTION: STBC 0x007469b0 _$E372
// FUNCTION: STBC 0x007469c0 _$E371
// FUNCTION: STBC 0x007469d0 _$E376
// FUNCTION: STBC 0x007469e0 _$E375
// FUNCTION: STBC 0x007469f0 _$E380
// FUNCTION: STBC 0x00746a00 _$E379
// FUNCTION: STBC 0x00746a10 _$E384
// FUNCTION: STBC 0x00746a20 _$E383
// FUNCTION: STBC 0x00746a30 _$E388
// FUNCTION: STBC 0x00746a40 _$E387
// FUNCTION: STBC 0x00746a50 _$E392
// FUNCTION: STBC 0x00746a60 _$E391
// FUNCTION: STBC 0x00746a70 _$E396
// FUNCTION: STBC 0x00746a80 _$E395
// FUNCTION: STBC 0x00746a90 _$E181
// FUNCTION: STBC 0x00746aa0 _$E180
// FUNCTION: STBC 0x00746ab0 _$E400
// FUNCTION: STBC 0x00746ac0 _$E399
// FUNCTION: STBC 0x00746ad0 _$E186
// FUNCTION: STBC 0x00746ae0 _$E185
// FUNCTION: STBC 0x00746af0 _$E191
// FUNCTION: STBC 0x00746b00 _$E190

// The list's destructor, emitted for the EH unwind of the manager's
// destructor (0x880F90 calls it); the body is NiTList.h's.
// FUNCTION: STBC 0x00746fe0 ??1?$NiTList@PAVTGUITheme@@@@QAE@XZ @game/TGUIThemeManager

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x00746b10 _$E405
// FUNCTION: STBC 0x00746b20 _$E402
// FUNCTION: STBC 0x00746b30 _$E404
// FUNCTION: STBC 0x00746b40 _$E403
// DATA: STBC 0x0099ea70
TGUIThemeManager g_kUIThemeManager;

/*
================
TGUIThemeManager::TGUIThemeManager
================
*/
// FUNCTION: STBC 0x00746b50
TGUIThemeManager::TGUIThemeManager( void ) : themes( 2 ) {
}

/*
================
TGUIThemeManager::~TGUIThemeManager
================
*/
// FUNCTION: STBC 0x00746ba0
TGUIThemeManager::~TGUIThemeManager( void ) {
	NiTListIterator pos = themes.GetHeadPos();
	while ( pos ) {
		delete themes.GetNext( pos );
	}
}

/*
================
TGUIThemeManager::AddTheme

Appends a copy of theme; the caller keeps theme.
================
*/
// FUNCTION: STBC 0x00746c30
void TGUIThemeManager::AddTheme( TGUITheme *theme ) {
	TGUITheme *copy = new TGUITheme( theme->GetThemeName() );
	copy->Copy( theme );
	themes.AddTail( copy );
}

/*
================
TGUIThemeManager::SetCurrentTheme

Makes the theme of that name current; false when there is none.
================
*/
// FUNCTION: STBC 0x00746d50
bool TGUIThemeManager::SetCurrentTheme( char *name ) {
	TGUITheme *theme = FindTheme( name );
	if ( theme ) {
		currentTheme = theme;
		return true;
	}
	return false;
}

/*
================
TGUIThemeManager::FindTheme

The theme of that name, or NULL.
================
*/
// FUNCTION: STBC 0x00746d70
TGUITheme *TGUIThemeManager::FindTheme( char *name ) {
	NiTListIterator pos = themes.GetHeadPos();
	while ( pos ) {
		TGUITheme *theme = themes.GetNext( pos );
		if ( strcmp( theme->GetThemeName(), name ) == 0 ) {
			return theme;
		}
	}
	return NULL;
}

/*
================
TGUIThemeManager::WriteToStream

The current theme, the count, then each theme: its address as its ID,
then its own fields.
================
*/
// FUNCTION: STBC 0x00746de0
bool TGUIThemeManager::WriteToStream( TGStream *stream ) {
	stream->WriteID( (long)currentTheme );
	stream->WriteLong( themes.GetSize() );
	NiTListIterator pos = themes.GetHeadPos();
	while ( pos ) {
		TGUITheme *theme = themes.GetNext( pos );
		if ( theme ) {
			stream->WriteID( (long)theme );
			if ( !theme->WriteToStream( stream ) ) {
				return false;
			}
		} else {
			stream->WriteID( 0 );
		}
	}
	return true;
}

/*
================
TGUIThemeManager::ReadFromStream

Reads the themes WriteToStream wrote, and makes current the one whose ID
was the current theme's.
================
*/
// FUNCTION: STBC 0x00746e50
bool TGUIThemeManager::ReadFromStream( TGStream *stream ) {
	long currentID = stream->ReadID();
	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		long id = stream->ReadID();
		if ( id ) {
			TGUITheme *theme = new TGUITheme;
			if ( !theme->ReadFromStream( stream ) ) {
				return false;
			}
			if ( currentID == id ) {
				currentTheme = theme;
			}
			themes.AddTail( theme );
		}
	}
	return true;
}
