/*
===============================================================================

	TGLocalizationManager.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGLocalizationManager.h"
#include "TGLocalizationDatabase.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006d0860 _$E414
// FUNCTION: STBC 0x006d0880 _$E413

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006cf9d0 _$E32
// FUNCTION: STBC 0x006cf9e0 _$E29
// FUNCTION: STBC 0x006cf9f0 _$E31
// FUNCTION: STBC 0x006cfa00 _$E30
// FUNCTION: STBC 0x006cfa10 _$E38
// FUNCTION: STBC 0x006cfa20 _$E35
// FUNCTION: STBC 0x006cfa30 _$E37
// FUNCTION: STBC 0x006cfa40 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006cfa50 _$E46
// FUNCTION: STBC 0x006cfa60 _$E45
// FUNCTION: STBC 0x006cfa70 _$E50
// FUNCTION: STBC 0x006cfa80 _$E49
// FUNCTION: STBC 0x006cfa90 _$E61
// FUNCTION: STBC 0x006cfaa0 _$E60
// FUNCTION: STBC 0x006cfab0 _$E65
// FUNCTION: STBC 0x006cfac0 _$E64
// FUNCTION: STBC 0x006cfad0 _$E69
// FUNCTION: STBC 0x006cfae0 _$E68
// FUNCTION: STBC 0x006cfaf0 _$E73
// FUNCTION: STBC 0x006cfb00 _$E72
// FUNCTION: STBC 0x006cfb10 _$E197
// FUNCTION: STBC 0x006cfb20 _$E196
// FUNCTION: STBC 0x006cfb30 _$E139
// FUNCTION: STBC 0x006cfb40 _$E138
// FUNCTION: STBC 0x006cfb50 _$E152
// FUNCTION: STBC 0x006cfb60 _$E151
// FUNCTION: STBC 0x006cfb70 _$E202
// FUNCTION: STBC 0x006cfb80 _$E201
// FUNCTION: STBC 0x006cfb90 _$E207
// FUNCTION: STBC 0x006cfba0 _$E206
// FUNCTION: STBC 0x006cfbb0 _$E211
// FUNCTION: STBC 0x006cfbc0 _$E210
// FUNCTION: STBC 0x006cfbd0 _$E215
// FUNCTION: STBC 0x006cfbe0 _$E214
// FUNCTION: STBC 0x006cfbf0 _$E222
// FUNCTION: STBC 0x006cfc00 _$E221
// FUNCTION: STBC 0x006cfc10 _$E247
// FUNCTION: STBC 0x006cfc20 _$E246
// FUNCTION: STBC 0x006cfc30 _$E251
// FUNCTION: STBC 0x006cfc40 _$E250
// FUNCTION: STBC 0x006cfc50 _$E97
// FUNCTION: STBC 0x006cfc60 _$E96
// FUNCTION: STBC 0x006cfc70 _$E233
// FUNCTION: STBC 0x006cfc80 _$E232
// FUNCTION: STBC 0x006cfc90 _$E240
// FUNCTION: STBC 0x006cfca0 _$E239
// FUNCTION: STBC 0x006cfcb0 _$E263
// FUNCTION: STBC 0x006cfcc0 _$E262
// FUNCTION: STBC 0x006cfcd0 _$E267
// FUNCTION: STBC 0x006cfce0 _$E266
// FUNCTION: STBC 0x006cfcf0 _$E307
// FUNCTION: STBC 0x006cfd00 _$E306
// FUNCTION: STBC 0x006cfd10 _$E313
// FUNCTION: STBC 0x006cfd20 _$E312
// FUNCTION: STBC 0x006cfd30 _$E255
// FUNCTION: STBC 0x006cfd40 _$E254
// FUNCTION: STBC 0x006cfd50 _$E259
// FUNCTION: STBC 0x006cfd60 _$E258
// FUNCTION: STBC 0x006cfd70 _$E271
// FUNCTION: STBC 0x006cfd80 _$E270
// FUNCTION: STBC 0x006cfd90 _$E275
// FUNCTION: STBC 0x006cfda0 _$E274
// FUNCTION: STBC 0x006cfdb0 _$E279
// FUNCTION: STBC 0x006cfdc0 _$E278
// FUNCTION: STBC 0x006cfdd0 _$E283
// FUNCTION: STBC 0x006cfde0 _$E282
// FUNCTION: STBC 0x006cfdf0 _$E287
// FUNCTION: STBC 0x006cfe00 _$E286
// FUNCTION: STBC 0x006cfe10 _$E291
// FUNCTION: STBC 0x006cfe20 _$E290
// FUNCTION: STBC 0x006cfe30 _$E295
// FUNCTION: STBC 0x006cfe40 _$E294
// FUNCTION: STBC 0x006cfe50 _$E299
// FUNCTION: STBC 0x006cfe60 _$E298
// FUNCTION: STBC 0x006cfe70 _$E102
// FUNCTION: STBC 0x006cfe80 _$E101
// FUNCTION: STBC 0x006cfe90 _$E303
// FUNCTION: STBC 0x006cfea0 _$E302
// FUNCTION: STBC 0x006cfeb0 _$E107
// FUNCTION: STBC 0x006cfec0 _$E106
// FUNCTION: STBC 0x006cfed0 _$E317
// FUNCTION: STBC 0x006cfee0 _$E316
// FUNCTION: STBC 0x006cfef0 _$E321
// FUNCTION: STBC 0x006cff00 _$E320
// FUNCTION: STBC 0x006cff10 _$E325
// FUNCTION: STBC 0x006cff20 _$E324
// FUNCTION: STBC 0x006cff30 _$E329
// FUNCTION: STBC 0x006cff40 _$E328
// FUNCTION: STBC 0x006cff50 _$E116
// FUNCTION: STBC 0x006cff60 _$E115
// FUNCTION: STBC 0x006cff70 _$E333
// FUNCTION: STBC 0x006cff80 _$E332
// FUNCTION: STBC 0x006cff90 _$E337
// FUNCTION: STBC 0x006cffa0 _$E336
// FUNCTION: STBC 0x006cffb0 _$E341
// FUNCTION: STBC 0x006cffc0 _$E340
// FUNCTION: STBC 0x006cffd0 _$E122
// FUNCTION: STBC 0x006cffe0 _$E121
// FUNCTION: STBC 0x006cfff0 _$E127
// FUNCTION: STBC 0x006d0000 _$E126
// FUNCTION: STBC 0x006d0010 _$E157
// FUNCTION: STBC 0x006d0020 _$E156
// FUNCTION: STBC 0x006d0030 _$E345
// FUNCTION: STBC 0x006d0040 _$E344
// FUNCTION: STBC 0x006d0050 _$E162
// FUNCTION: STBC 0x006d0060 _$E161
// FUNCTION: STBC 0x006d0070 _$E349
// FUNCTION: STBC 0x006d0080 _$E348
// FUNCTION: STBC 0x006d0090 _$E353
// FUNCTION: STBC 0x006d00a0 _$E352
// FUNCTION: STBC 0x006d00b0 _$E167
// FUNCTION: STBC 0x006d00c0 _$E166
// FUNCTION: STBC 0x006d00d0 _$E357
// FUNCTION: STBC 0x006d00e0 _$E356
// FUNCTION: STBC 0x006d00f0 _$E132
// FUNCTION: STBC 0x006d0100 _$E131
// FUNCTION: STBC 0x006d0110 _$E361
// FUNCTION: STBC 0x006d0120 _$E360
// FUNCTION: STBC 0x006d0130 _$E365
// FUNCTION: STBC 0x006d0140 _$E364
// FUNCTION: STBC 0x006d0150 _$E172
// FUNCTION: STBC 0x006d0160 _$E171
// FUNCTION: STBC 0x006d0170 _$E177
// FUNCTION: STBC 0x006d0180 _$E176
// FUNCTION: STBC 0x006d0190 _$E369
// FUNCTION: STBC 0x006d01a0 _$E368
// FUNCTION: STBC 0x006d01b0 _$E373
// FUNCTION: STBC 0x006d01c0 _$E372
// FUNCTION: STBC 0x006d01d0 _$E377
// FUNCTION: STBC 0x006d01e0 _$E376
// FUNCTION: STBC 0x006d01f0 _$E381
// FUNCTION: STBC 0x006d0200 _$E380
// FUNCTION: STBC 0x006d0210 _$E385
// FUNCTION: STBC 0x006d0220 _$E384
// FUNCTION: STBC 0x006d0230 _$E389
// FUNCTION: STBC 0x006d0240 _$E388
// FUNCTION: STBC 0x006d0250 _$E393
// FUNCTION: STBC 0x006d0260 _$E392
// FUNCTION: STBC 0x006d0270 _$E397
// FUNCTION: STBC 0x006d0280 _$E396
// FUNCTION: STBC 0x006d0290 _$E182
// FUNCTION: STBC 0x006d02a0 _$E181
// FUNCTION: STBC 0x006d02b0 _$E401
// FUNCTION: STBC 0x006d02c0 _$E400
// FUNCTION: STBC 0x006d02d0 _$E187
// FUNCTION: STBC 0x006d02e0 _$E186
// FUNCTION: STBC 0x006d02f0 _$E192
// FUNCTION: STBC 0x006d0300 _$E191

// Its construction and exit-time destruction.
// FUNCTION: STBC 0x006d0310 _$E406
// FUNCTION: STBC 0x006d0320 _$E403
// FUNCTION: STBC 0x006d0330 _$E405
// FUNCTION: STBC 0x006d0340 _$E404
// DATA: STBC 0x00997fd8
TGLocalizationManager g_kLocalizationManager;

/*
================
TGLocalizationManager::TGLocalizationManager
================
*/
// FUNCTION: STBC 0x006d03a0
TGLocalizationManager::TGLocalizationManager( void ) : databases( 2 ) {
}

/*
================
TGLocalizationManager::Load

Reads the database of the file; the manager registers it itself.
================
*/
// FUNCTION: STBC 0x006d03d0 ?Load@TGLocalizationManager@@QAEPAVTGLocalizationDatabase@@PAD_N@Z
TGLocalizationDatabase *TGLocalizationManager::Load( char *file, bool addUser ) {
	return TGLocalizationDatabase::Create( file, addUser );
}

/*
================
TGLocalizationManager::GetIfRegistered

The database loaded from the file, or NULL; with addUser, the caller becomes
one of its users.
================
*/
// FUNCTION: STBC 0x006d03f0 ?GetIfRegistered@TGLocalizationManager@@QAEPAVTGLocalizationDatabase@@PAD_N@Z
TGLocalizationDatabase *TGLocalizationManager::GetIfRegistered( char *file, bool addUser ) {
	NiTListIterator pos = databases.GetHeadPos();
	while ( pos != NULL ) {
		TGLocalizationEntry *entry = databases.GetNext( pos );
		if ( strcmp( entry->database->GetName(), file ) == 0 ) {
			if ( addUser ) {
				entry->users++;
			}
			return entry->database;
		}
	}
	return NULL;
}

/*
================
TGLocalizationManager::RegisterDatabase

Adds the database, with one user.
================
*/
// FUNCTION: STBC 0x006d0470 ?RegisterDatabase@TGLocalizationManager@@QAEXPAVTGLocalizationDatabase@@@Z
void TGLocalizationManager::RegisterDatabase( TGLocalizationDatabase *database ) {
	database->registered = true;
	TGLocalizationEntry *entry = new TGLocalizationEntry;
	entry->database = database;
	entry->users = 1;
	databases.AddTail( entry );
}

/*
================
TGLocalizationManager::Unload

Takes a user from the database.
================
*/
// FUNCTION: STBC 0x006d04f0 ?Unload@TGLocalizationManager@@QAEXPAVTGLocalizationDatabase@@@Z
void TGLocalizationManager::Unload( TGLocalizationDatabase *database ) {
	NiTListIterator pos = databases.GetHeadPos();
	while ( pos != NULL ) {
		TGLocalizationEntry *entry = databases.GetNext( pos );
		if ( entry->database == database ) {
			entry->users--;
			return;
		}
	}
}

/*
================
TGLocalizationManager::DeleteAll

Leaves every database without users.
================
*/
// FUNCTION: STBC 0x006d0520 ?DeleteAll@TGLocalizationManager@@QAEXXZ
void TGLocalizationManager::DeleteAll( void ) {
	NiTListIterator pos = databases.GetHeadPos();
	while ( pos != NULL ) {
		TGLocalizationEntry *entry = databases.GetNext( pos );
		if ( entry->users > 0 ) {
			entry->users = 0;
		}
	}
}

/*
================
TGLocalizationManager::Purge

Deletes the databases that have no users.
================
*/
// FUNCTION: STBC 0x006d0550 ?Purge@TGLocalizationManager@@QAEXXZ
void TGLocalizationManager::Purge( void ) {
	NiTListIterator pos = databases.GetHeadPos();
	while ( pos != NULL ) {
		TGLocalizationEntry *entry = databases.GetNext( pos );
		if ( entry->users == 0 ) {
			if ( entry->database != NULL ) {
				delete entry->database;
			}
			delete entry;
			databases.Remove( entry );
		}
	}
}

/*
================
TGLocalizationManager::WriteToStream

Writes the number of databases, then the file name and the user count of each.
================
*/
// FUNCTION: STBC 0x006d0670 ?WriteToStream@TGLocalizationManager@@UAE_NPAVTGStream@@@Z
bool TGLocalizationManager::WriteToStream( TGStream *stream ) {
	stream->WriteLong( databases.GetSize() );
	NiTListIterator pos = databases.GetHeadPos();
	while ( pos != NULL ) {
		TGLocalizationEntry *entry = databases.GetNext( pos );
		if ( entry != NULL ) {
			stream->VirtualSlot11( entry->database->GetName() );
			stream->WriteLong( entry->users );
		} else {
			stream->VirtualSlot11( NULL );
		}
	}
	return true;
}

/*
================
TGLocalizationManager::ReadFromStream

Loads each database the stream names and restores its user count.
================
*/
// FUNCTION: STBC 0x006d06d0 ?ReadFromStream@TGLocalizationManager@@UAE_NPAVTGStream@@@Z
bool TGLocalizationManager::ReadFromStream( TGStream *stream ) {
	int count = stream->ReadLong();
	while ( count > 0 ) {
		char *file = stream->VirtualSlot10( -1 );
		if ( file != NULL ) {
			unsigned int users = stream->ReadLong();
			TGLocalizationDatabase *database = Load( file, true );
			NiTListIterator pos = databases.GetHeadPos();
			while ( pos != NULL ) {
				TGLocalizationEntry *entry = databases.GetNext( pos );
				if ( entry->database == database ) {
					entry->users = users;
					break;
				}
			}
		}
		count--;
	}
	return true;
}

/*
================
TGLocalizationManager::ForgetAll

Forgets the databases without deleting them.
================
*/
// FUNCTION: STBC 0x006d0750
void TGLocalizationManager::ForgetAll( void ) {
	databases.RemoveAll();
}
