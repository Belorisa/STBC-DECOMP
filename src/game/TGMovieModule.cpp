/*
===============================================================================

	TGMovieModule.cpp

	The movie module: the creators of the movie classes, the credits' clock
	and the movies' stop at the end. The unit's name is invented, after its
	invented class: retail's unit runs from its <iostream> quads at 0x6AD620,
	its NetImmerse registrations (0x6AD6A0-0x6ADF5F) and the module's static
	quad to its locale-id pair at 0x6AE2C0; TGMovieManager's unit follows.

===============================================================================
*/

#include <iostream>

#include "TGMovieModule.h"
#include "TGMovieManager.h"
#include "TGMovieAction.h"
#include "TGCreditAction.h"
#include "TGObject.h"
#include "UtopiaApp.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006ae2c0 _$E430
// FUNCTION: STBC 0x006ae2e0 _$E429

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ad620 _$E32
// FUNCTION: STBC 0x006ad630 _$E29
// FUNCTION: STBC 0x006ad640 _$E31
// FUNCTION: STBC 0x006ad650 _$E30
// FUNCTION: STBC 0x006ad660 _$E38
// FUNCTION: STBC 0x006ad670 _$E35
// FUNCTION: STBC 0x006ad680 _$E37
// FUNCTION: STBC 0x006ad690 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ad6a0 _$E61
// FUNCTION: STBC 0x006ad6b0 _$E60
// FUNCTION: STBC 0x006ad6c0 _$E217
// FUNCTION: STBC 0x006ad6d0 _$E216
// FUNCTION: STBC 0x006ad6e0 _$E146
// FUNCTION: STBC 0x006ad6f0 _$E145
// FUNCTION: STBC 0x006ad700 _$E159
// FUNCTION: STBC 0x006ad710 _$E158
// FUNCTION: STBC 0x006ad720 _$E72
// FUNCTION: STBC 0x006ad730 _$E71
// FUNCTION: STBC 0x006ad740 _$E76
// FUNCTION: STBC 0x006ad750 _$E75
// FUNCTION: STBC 0x006ad760 _$E80
// FUNCTION: STBC 0x006ad770 _$E79
// FUNCTION: STBC 0x006ad780 _$E84
// FUNCTION: STBC 0x006ad790 _$E83
// FUNCTION: STBC 0x006ad7a0 _$E204
// FUNCTION: STBC 0x006ad7b0 _$E203
// FUNCTION: STBC 0x006ad7c0 _$E209
// FUNCTION: STBC 0x006ad7d0 _$E208
// FUNCTION: STBC 0x006ad7e0 _$E222
// FUNCTION: STBC 0x006ad7f0 _$E221
// FUNCTION: STBC 0x006ad800 _$E213
// FUNCTION: STBC 0x006ad810 _$E212
// FUNCTION: STBC 0x006ad820 _$E226
// FUNCTION: STBC 0x006ad830 _$E225
// FUNCTION: STBC 0x006ad840 _$E233
// FUNCTION: STBC 0x006ad850 _$E232
// FUNCTION: STBC 0x006ad860 _$E263
// FUNCTION: STBC 0x006ad870 _$E262
// FUNCTION: STBC 0x006ad880 _$E267
// FUNCTION: STBC 0x006ad890 _$E266
// FUNCTION: STBC 0x006ad8a0 _$E108
// FUNCTION: STBC 0x006ad8b0 _$E107
// FUNCTION: STBC 0x006ad8c0 _$E249
// FUNCTION: STBC 0x006ad8d0 _$E248
// FUNCTION: STBC 0x006ad8e0 _$E256
// FUNCTION: STBC 0x006ad8f0 _$E255
// FUNCTION: STBC 0x006ad900 _$E279
// FUNCTION: STBC 0x006ad910 _$E278
// FUNCTION: STBC 0x006ad920 _$E283
// FUNCTION: STBC 0x006ad930 _$E282
// FUNCTION: STBC 0x006ad940 _$E323
// FUNCTION: STBC 0x006ad950 _$E322
// FUNCTION: STBC 0x006ad960 _$E329
// FUNCTION: STBC 0x006ad970 _$E328
// FUNCTION: STBC 0x006ad980 _$E271
// FUNCTION: STBC 0x006ad990 _$E270
// FUNCTION: STBC 0x006ad9a0 _$E275
// FUNCTION: STBC 0x006ad9b0 _$E274
// FUNCTION: STBC 0x006ad9c0 _$E287
// FUNCTION: STBC 0x006ad9d0 _$E286
// FUNCTION: STBC 0x006ad9e0 _$E291
// FUNCTION: STBC 0x006ad9f0 _$E290
// FUNCTION: STBC 0x006ada00 _$E295
// FUNCTION: STBC 0x006ada10 _$E294
// FUNCTION: STBC 0x006ada20 _$E299
// FUNCTION: STBC 0x006ada30 _$E298
// FUNCTION: STBC 0x006ada40 _$E303
// FUNCTION: STBC 0x006ada50 _$E302
// FUNCTION: STBC 0x006ada60 _$E307
// FUNCTION: STBC 0x006ada70 _$E306
// FUNCTION: STBC 0x006ada80 _$E311
// FUNCTION: STBC 0x006ada90 _$E310
// FUNCTION: STBC 0x006adaa0 _$E315
// FUNCTION: STBC 0x006adab0 _$E314
// FUNCTION: STBC 0x006adac0 _$E113
// FUNCTION: STBC 0x006adad0 _$E112
// FUNCTION: STBC 0x006adae0 _$E319
// FUNCTION: STBC 0x006adaf0 _$E318
// FUNCTION: STBC 0x006adb00 _$E118
// FUNCTION: STBC 0x006adb10 _$E117
// FUNCTION: STBC 0x006adb20 _$E333
// FUNCTION: STBC 0x006adb30 _$E332
// FUNCTION: STBC 0x006adb40 _$E337
// FUNCTION: STBC 0x006adb50 _$E336
// FUNCTION: STBC 0x006adb60 _$E341
// FUNCTION: STBC 0x006adb70 _$E340
// FUNCTION: STBC 0x006adb80 _$E345
// FUNCTION: STBC 0x006adb90 _$E344
// FUNCTION: STBC 0x006adba0 _$E123
// FUNCTION: STBC 0x006adbb0 _$E122
// FUNCTION: STBC 0x006adbc0 _$E349
// FUNCTION: STBC 0x006adbd0 _$E348
// FUNCTION: STBC 0x006adbe0 _$E353
// FUNCTION: STBC 0x006adbf0 _$E352
// FUNCTION: STBC 0x006adc00 _$E357
// FUNCTION: STBC 0x006adc10 _$E356
// FUNCTION: STBC 0x006adc20 _$E129
// FUNCTION: STBC 0x006adc30 _$E128
// FUNCTION: STBC 0x006adc40 _$E134
// FUNCTION: STBC 0x006adc50 _$E133
// FUNCTION: STBC 0x006adc60 _$E164
// FUNCTION: STBC 0x006adc70 _$E163
// FUNCTION: STBC 0x006adc80 _$E361
// FUNCTION: STBC 0x006adc90 _$E360
// FUNCTION: STBC 0x006adca0 _$E169
// FUNCTION: STBC 0x006adcb0 _$E168
// FUNCTION: STBC 0x006adcc0 _$E365
// FUNCTION: STBC 0x006adcd0 _$E364
// FUNCTION: STBC 0x006adce0 _$E369
// FUNCTION: STBC 0x006adcf0 _$E368
// FUNCTION: STBC 0x006add00 _$E174
// FUNCTION: STBC 0x006add10 _$E173
// FUNCTION: STBC 0x006add20 _$E373
// FUNCTION: STBC 0x006add30 _$E372
// FUNCTION: STBC 0x006add40 _$E139
// FUNCTION: STBC 0x006add50 _$E138
// FUNCTION: STBC 0x006add60 _$E377
// FUNCTION: STBC 0x006add70 _$E376
// FUNCTION: STBC 0x006add80 _$E381
// FUNCTION: STBC 0x006add90 _$E380
// FUNCTION: STBC 0x006adda0 _$E179
// FUNCTION: STBC 0x006addb0 _$E178
// FUNCTION: STBC 0x006addc0 _$E184
// FUNCTION: STBC 0x006addd0 _$E183
// FUNCTION: STBC 0x006adde0 _$E385
// FUNCTION: STBC 0x006addf0 _$E384
// FUNCTION: STBC 0x006ade00 _$E389
// FUNCTION: STBC 0x006ade10 _$E388
// FUNCTION: STBC 0x006ade20 _$E393
// FUNCTION: STBC 0x006ade30 _$E392
// FUNCTION: STBC 0x006ade40 _$E397
// FUNCTION: STBC 0x006ade50 _$E396
// FUNCTION: STBC 0x006ade60 _$E401
// FUNCTION: STBC 0x006ade70 _$E400
// FUNCTION: STBC 0x006ade80 _$E405
// FUNCTION: STBC 0x006ade90 _$E404
// FUNCTION: STBC 0x006adea0 _$E409
// FUNCTION: STBC 0x006adeb0 _$E408
// FUNCTION: STBC 0x006adec0 _$E413
// FUNCTION: STBC 0x006aded0 _$E412
// FUNCTION: STBC 0x006adee0 _$E189
// FUNCTION: STBC 0x006adef0 _$E188
// FUNCTION: STBC 0x006adf00 _$E417
// FUNCTION: STBC 0x006adf10 _$E416
// FUNCTION: STBC 0x006adf20 _$E194
// FUNCTION: STBC 0x006adf30 _$E193
// FUNCTION: STBC 0x006adf40 _$E199
// FUNCTION: STBC 0x006adf50 _$E198

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x006adf60 _$E422
// FUNCTION: STBC 0x006adf70 _$E419
// FUNCTION: STBC 0x006adf80 _$E421
// FUNCTION: STBC 0x006adf90 _$E420
// DATA: STBC 0x00995988
TGMovieModule movieModule;		// invented name

/*
================
TGMovieModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x006adfd0
void TGMovieModule::VirtualSlot05( void ) {
}

/*
================
TGMovieModule::VirtualSlot00

The startup: the creators of the movie classes.
================
*/
// FUNCTION: STBC 0x006adfe0
void TGMovieModule::VirtualSlot00( void ) {
	TGObject::RegisterCreator( TGMOVIEACTION_TYPE, CreateTGMovieAction );
	TGObject::RegisterCreator( TGCREDITACTION_TYPE, CreateTGCreditAction );
}

/*
================
TGMovieModule::CreateTGMovieAction
================
*/
// FUNCTION: STBC 0x006ae150
TGObject *TGMovieModule::CreateTGMovieAction( long id ) {
	return new TGMovieAction( id );
}

/*
================
TGMovieModule::CreateTGCreditAction
================
*/
// FUNCTION: STBC 0x006ae250
TGObject *TGMovieModule::CreateTGCreditAction( long id ) {
	return new TGCreditAction( id );
}

/*
================
TGMovieModule::VirtualSlot02

Each frame the credits move on to the application's clock.
================
*/
// FUNCTION: STBC 0x006ae280
void TGMovieModule::VirtualSlot02( void ) {
	TGCreditAction::UpdateFades( NiApplication::application->GetCurrentTime() );
}

/*
================
TGMovieModule::VirtualSlot04

The movies stop, and the display mode comes back.
================
*/
// FUNCTION: STBC 0x006ae2a0
void TGMovieModule::VirtualSlot04( void ) {
	g_kMovieManager.StopAllMovies();
	g_kMovieManager.SwitchOutOfMovieMode();
}
