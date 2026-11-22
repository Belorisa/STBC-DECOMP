/*
===============================================================================

	TGLocalizationModule.cpp

===============================================================================
*/

#include <iostream>

#include "TGLocalizationModule.h"
#include "NiMain.h"
#include "PyEmbed.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006cefb0 _$E436
// FUNCTION: STBC 0x006cefd0 _$E435

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ce440 _$E32
// FUNCTION: STBC 0x006ce450 _$E29
// FUNCTION: STBC 0x006ce460 _$E31
// FUNCTION: STBC 0x006ce470 _$E30
// FUNCTION: STBC 0x006ce480 _$E38
// FUNCTION: STBC 0x006ce490 _$E35
// FUNCTION: STBC 0x006ce4a0 _$E37
// FUNCTION: STBC 0x006ce4b0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ce4c0 _$E47
// FUNCTION: STBC 0x006ce4d0 _$E46
// FUNCTION: STBC 0x006ce4e0 _$E51
// FUNCTION: STBC 0x006ce4f0 _$E50
// FUNCTION: STBC 0x006ce500 _$E62
// FUNCTION: STBC 0x006ce510 _$E61
// FUNCTION: STBC 0x006ce520 _$E66
// FUNCTION: STBC 0x006ce530 _$E65
// FUNCTION: STBC 0x006ce540 _$E70
// FUNCTION: STBC 0x006ce550 _$E69
// FUNCTION: STBC 0x006ce560 _$E74
// FUNCTION: STBC 0x006ce570 _$E73
// FUNCTION: STBC 0x006ce580 _$E198
// FUNCTION: STBC 0x006ce590 _$E197
// FUNCTION: STBC 0x006ce5a0 _$E140
// FUNCTION: STBC 0x006ce5b0 _$E139
// FUNCTION: STBC 0x006ce5c0 _$E153
// FUNCTION: STBC 0x006ce5d0 _$E152
// FUNCTION: STBC 0x006ce5e0 _$E203
// FUNCTION: STBC 0x006ce5f0 _$E202
// FUNCTION: STBC 0x006ce600 _$E208
// FUNCTION: STBC 0x006ce610 _$E207
// FUNCTION: STBC 0x006ce620 _$E212
// FUNCTION: STBC 0x006ce630 _$E211
// FUNCTION: STBC 0x006ce640 _$E216
// FUNCTION: STBC 0x006ce650 _$E215
// FUNCTION: STBC 0x006ce660 _$E223
// FUNCTION: STBC 0x006ce670 _$E222
// FUNCTION: STBC 0x006ce680 _$E248
// FUNCTION: STBC 0x006ce690 _$E247
// FUNCTION: STBC 0x006ce6a0 _$E252
// FUNCTION: STBC 0x006ce6b0 _$E251
// FUNCTION: STBC 0x006ce6c0 _$E98
// FUNCTION: STBC 0x006ce6d0 _$E97
// FUNCTION: STBC 0x006ce6e0 _$E234
// FUNCTION: STBC 0x006ce6f0 _$E233
// FUNCTION: STBC 0x006ce700 _$E241
// FUNCTION: STBC 0x006ce710 _$E240
// FUNCTION: STBC 0x006ce720 _$E264
// FUNCTION: STBC 0x006ce730 _$E263
// FUNCTION: STBC 0x006ce740 _$E268
// FUNCTION: STBC 0x006ce750 _$E267
// FUNCTION: STBC 0x006ce760 _$E308
// FUNCTION: STBC 0x006ce770 _$E307
// FUNCTION: STBC 0x006ce780 _$E314
// FUNCTION: STBC 0x006ce790 _$E313
// FUNCTION: STBC 0x006ce7a0 _$E256
// FUNCTION: STBC 0x006ce7b0 _$E255
// FUNCTION: STBC 0x006ce7c0 _$E260
// FUNCTION: STBC 0x006ce7d0 _$E259
// FUNCTION: STBC 0x006ce7e0 _$E272
// FUNCTION: STBC 0x006ce7f0 _$E271
// FUNCTION: STBC 0x006ce800 _$E276
// FUNCTION: STBC 0x006ce810 _$E275
// FUNCTION: STBC 0x006ce820 _$E280
// FUNCTION: STBC 0x006ce830 _$E279
// FUNCTION: STBC 0x006ce840 _$E284
// FUNCTION: STBC 0x006ce850 _$E283
// FUNCTION: STBC 0x006ce860 _$E288
// FUNCTION: STBC 0x006ce870 _$E287
// FUNCTION: STBC 0x006ce880 _$E292
// FUNCTION: STBC 0x006ce890 _$E291
// FUNCTION: STBC 0x006ce8a0 _$E296
// FUNCTION: STBC 0x006ce8b0 _$E295
// FUNCTION: STBC 0x006ce8c0 _$E300
// FUNCTION: STBC 0x006ce8d0 _$E299
// FUNCTION: STBC 0x006ce8e0 _$E103
// FUNCTION: STBC 0x006ce8f0 _$E102
// FUNCTION: STBC 0x006ce900 _$E304
// FUNCTION: STBC 0x006ce910 _$E303
// FUNCTION: STBC 0x006ce920 _$E108
// FUNCTION: STBC 0x006ce930 _$E107
// FUNCTION: STBC 0x006ce940 _$E318
// FUNCTION: STBC 0x006ce950 _$E317
// FUNCTION: STBC 0x006ce960 _$E322
// FUNCTION: STBC 0x006ce970 _$E321
// FUNCTION: STBC 0x006ce980 _$E326
// FUNCTION: STBC 0x006ce990 _$E325
// FUNCTION: STBC 0x006ce9a0 _$E330
// FUNCTION: STBC 0x006ce9b0 _$E329
// FUNCTION: STBC 0x006ce9c0 _$E117
// FUNCTION: STBC 0x006ce9d0 _$E116
// FUNCTION: STBC 0x006ce9e0 _$E334
// FUNCTION: STBC 0x006ce9f0 _$E333
// FUNCTION: STBC 0x006cea00 _$E338
// FUNCTION: STBC 0x006cea10 _$E337
// FUNCTION: STBC 0x006cea20 _$E342
// FUNCTION: STBC 0x006cea30 _$E341
// FUNCTION: STBC 0x006cea40 _$E123
// FUNCTION: STBC 0x006cea50 _$E122
// FUNCTION: STBC 0x006cea60 _$E128
// FUNCTION: STBC 0x006cea70 _$E127
// FUNCTION: STBC 0x006cea80 _$E158
// FUNCTION: STBC 0x006cea90 _$E157
// FUNCTION: STBC 0x006ceaa0 _$E346
// FUNCTION: STBC 0x006ceab0 _$E345
// FUNCTION: STBC 0x006ceac0 _$E163
// FUNCTION: STBC 0x006cead0 _$E162
// FUNCTION: STBC 0x006ceae0 _$E350
// FUNCTION: STBC 0x006ceaf0 _$E349
// FUNCTION: STBC 0x006ceb00 _$E354
// FUNCTION: STBC 0x006ceb10 _$E353
// FUNCTION: STBC 0x006ceb20 _$E168
// FUNCTION: STBC 0x006ceb30 _$E167
// FUNCTION: STBC 0x006ceb40 _$E358
// FUNCTION: STBC 0x006ceb50 _$E357
// FUNCTION: STBC 0x006ceb60 _$E133
// FUNCTION: STBC 0x006ceb70 _$E132
// FUNCTION: STBC 0x006ceb80 _$E362
// FUNCTION: STBC 0x006ceb90 _$E361
// FUNCTION: STBC 0x006ceba0 _$E366
// FUNCTION: STBC 0x006cebb0 _$E365
// FUNCTION: STBC 0x006cebc0 _$E173
// FUNCTION: STBC 0x006cebd0 _$E172
// FUNCTION: STBC 0x006cebe0 _$E178
// FUNCTION: STBC 0x006cebf0 _$E177
// FUNCTION: STBC 0x006cec00 _$E370
// FUNCTION: STBC 0x006cec10 _$E369
// FUNCTION: STBC 0x006cec20 _$E374
// FUNCTION: STBC 0x006cec30 _$E373
// FUNCTION: STBC 0x006cec40 _$E378
// FUNCTION: STBC 0x006cec50 _$E377
// FUNCTION: STBC 0x006cec60 _$E382
// FUNCTION: STBC 0x006cec70 _$E381
// FUNCTION: STBC 0x006cec80 _$E386
// FUNCTION: STBC 0x006cec90 _$E385
// FUNCTION: STBC 0x006ceca0 _$E390
// FUNCTION: STBC 0x006cecb0 _$E389
// FUNCTION: STBC 0x006cecc0 _$E394
// FUNCTION: STBC 0x006cecd0 _$E393
// FUNCTION: STBC 0x006cece0 _$E398
// FUNCTION: STBC 0x006cecf0 _$E397
// FUNCTION: STBC 0x006ced00 _$E183
// FUNCTION: STBC 0x006ced10 _$E182
// FUNCTION: STBC 0x006ced20 _$E402
// FUNCTION: STBC 0x006ced30 _$E401
// FUNCTION: STBC 0x006ced40 _$E188
// FUNCTION: STBC 0x006ced50 _$E187
// FUNCTION: STBC 0x006ced60 _$E193
// FUNCTION: STBC 0x006ced70 _$E192

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x006ced80 _$E428
// FUNCTION: STBC 0x006ced90 _$E425
// FUNCTION: STBC 0x006ceda0 _$E427
// FUNCTION: STBC 0x006cedb0 _$E426
// DATA: STBC 0x00997dd0
TGLocalizationModule localizationModule;		// invented name

/*
================
TGLocalizationModule::VirtualSlot00

The startup: the databases' state functions.
================
*/
// FUNCTION: STBC 0x006cedf0
void TGLocalizationModule::VirtualSlot00( void ) {
	RegisterStateFunctions();
}

/*
================
TGLocalizationModule::VirtualSlot01
================
*/
// FUNCTION: STBC 0x006cee00
void TGLocalizationModule::VirtualSlot01( void ) {
}

/*
================
TGLocalizationModule::VirtualSlot04

Drops every database the manager holds.
================
*/
// FUNCTION: STBC 0x006cee10
void TGLocalizationModule::VirtualSlot04( void ) {
	g_kLocalizationManager.DeleteAll();
	g_kLocalizationManager.Purge();
}

/*
================
TGLocalizationModule::VirtualSlot05
================
*/
// FUNCTION: STBC 0x006cee30
void TGLocalizationModule::VirtualSlot05( void ) {
}

/*
================
TGLocalizationModule::RegisterStateFunctions
================
*/
// FUNCTION: STBC 0x006cee40
void TGLocalizationModule::RegisterStateFunctions( void ) {
	PyEmbed::AddGetStateFunction( TGLocDBWrapperSerialize, "TGLocalizationDatabase" );
	PyEmbed::AddSetStateFunction( TGLocDBWrapperUnserialize, "TGLocalizationDatabase" );
}
