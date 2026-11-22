/*
===============================================================================

	TGFontManager.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "TGFontManager.h"
#include "TGFontGroup.h"
#include "TGIconManager.h"
#include "TGStream.h"
#include "PyEmbed.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00733e90 _$E32
// FUNCTION: STBC 0x00733ea0 _$E29
// FUNCTION: STBC 0x00733eb0 _$E31
// FUNCTION: STBC 0x00733ec0 _$E30
// FUNCTION: STBC 0x00733ed0 _$E38
// FUNCTION: STBC 0x00733ee0 _$E35
// FUNCTION: STBC 0x00733ef0 _$E37
// FUNCTION: STBC 0x00733f00 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00733f10 _$E45
// FUNCTION: STBC 0x00733f20 _$E44
// FUNCTION: STBC 0x00733f30 _$E101
// FUNCTION: STBC 0x00733f40 _$E100
// FUNCTION: STBC 0x00733f50 _$E56
// FUNCTION: STBC 0x00733f60 _$E55
// FUNCTION: STBC 0x00733f70 _$E60
// FUNCTION: STBC 0x00733f80 _$E59
// FUNCTION: STBC 0x00733f90 _$E64
// FUNCTION: STBC 0x00733fa0 _$E63
// FUNCTION: STBC 0x00733fb0 _$E68
// FUNCTION: STBC 0x00733fc0 _$E67
// FUNCTION: STBC 0x00733fd0 _$E200
// FUNCTION: STBC 0x00733fe0 _$E199
// FUNCTION: STBC 0x00733ff0 _$E75
// FUNCTION: STBC 0x00734000 _$E74
// FUNCTION: STBC 0x00734010 _$E88
// FUNCTION: STBC 0x00734020 _$E87
// FUNCTION: STBC 0x00734030 _$E205
// FUNCTION: STBC 0x00734040 _$E204
// FUNCTION: STBC 0x00734050 _$E210
// FUNCTION: STBC 0x00734060 _$E209
// FUNCTION: STBC 0x00734070 _$E214
// FUNCTION: STBC 0x00734080 _$E213
// FUNCTION: STBC 0x00734090 _$E218
// FUNCTION: STBC 0x007340a0 _$E217
// FUNCTION: STBC 0x007340b0 _$E225
// FUNCTION: STBC 0x007340c0 _$E224
// FUNCTION: STBC 0x007340d0 _$E267
// FUNCTION: STBC 0x007340e0 _$E266
// FUNCTION: STBC 0x007340f0 _$E271
// FUNCTION: STBC 0x00734100 _$E270
// FUNCTION: STBC 0x00734110 _$E125
// FUNCTION: STBC 0x00734120 _$E124
// FUNCTION: STBC 0x00734130 _$E253
// FUNCTION: STBC 0x00734140 _$E252
// FUNCTION: STBC 0x00734150 _$E260
// FUNCTION: STBC 0x00734160 _$E259
// FUNCTION: STBC 0x00734170 _$E283
// FUNCTION: STBC 0x00734180 _$E282
// FUNCTION: STBC 0x00734190 _$E287
// FUNCTION: STBC 0x007341a0 _$E286
// FUNCTION: STBC 0x007341b0 _$E327
// FUNCTION: STBC 0x007341c0 _$E326
// FUNCTION: STBC 0x007341d0 _$E333
// FUNCTION: STBC 0x007341e0 _$E332
// FUNCTION: STBC 0x007341f0 _$E275
// FUNCTION: STBC 0x00734200 _$E274
// FUNCTION: STBC 0x00734210 _$E279
// FUNCTION: STBC 0x00734220 _$E278
// FUNCTION: STBC 0x00734230 _$E291
// FUNCTION: STBC 0x00734240 _$E290
// FUNCTION: STBC 0x00734250 _$E295
// FUNCTION: STBC 0x00734260 _$E294
// FUNCTION: STBC 0x00734270 _$E299
// FUNCTION: STBC 0x00734280 _$E298
// FUNCTION: STBC 0x00734290 _$E303
// FUNCTION: STBC 0x007342a0 _$E302
// FUNCTION: STBC 0x007342b0 _$E307
// FUNCTION: STBC 0x007342c0 _$E306
// FUNCTION: STBC 0x007342d0 _$E311
// FUNCTION: STBC 0x007342e0 _$E310
// FUNCTION: STBC 0x007342f0 _$E315
// FUNCTION: STBC 0x00734300 _$E314
// FUNCTION: STBC 0x00734310 _$E319
// FUNCTION: STBC 0x00734320 _$E318
// FUNCTION: STBC 0x00734330 _$E130
// FUNCTION: STBC 0x00734340 _$E129
// FUNCTION: STBC 0x00734350 _$E323
// FUNCTION: STBC 0x00734360 _$E322
// FUNCTION: STBC 0x00734370 _$E135
// FUNCTION: STBC 0x00734380 _$E134
// FUNCTION: STBC 0x00734390 _$E337
// FUNCTION: STBC 0x007343a0 _$E336
// FUNCTION: STBC 0x007343b0 _$E341
// FUNCTION: STBC 0x007343c0 _$E340
// FUNCTION: STBC 0x007343d0 _$E345
// FUNCTION: STBC 0x007343e0 _$E344
// FUNCTION: STBC 0x007343f0 _$E349
// FUNCTION: STBC 0x00734400 _$E348
// FUNCTION: STBC 0x00734410 _$E144
// FUNCTION: STBC 0x00734420 _$E143
// FUNCTION: STBC 0x00734430 _$E353
// FUNCTION: STBC 0x00734440 _$E352
// FUNCTION: STBC 0x00734450 _$E357
// FUNCTION: STBC 0x00734460 _$E356
// FUNCTION: STBC 0x00734470 _$E361
// FUNCTION: STBC 0x00734480 _$E360
// FUNCTION: STBC 0x00734490 _$E150
// FUNCTION: STBC 0x007344a0 _$E149
// FUNCTION: STBC 0x007344b0 _$E155
// FUNCTION: STBC 0x007344c0 _$E154
// FUNCTION: STBC 0x007344d0 _$E165
// FUNCTION: STBC 0x007344e0 _$E164
// FUNCTION: STBC 0x007344f0 _$E365
// FUNCTION: STBC 0x00734500 _$E364
// FUNCTION: STBC 0x00734510 _$E170
// FUNCTION: STBC 0x00734520 _$E169
// FUNCTION: STBC 0x00734530 _$E369
// FUNCTION: STBC 0x00734540 _$E368
// FUNCTION: STBC 0x00734550 _$E373
// FUNCTION: STBC 0x00734560 _$E372
// FUNCTION: STBC 0x00734570 _$E175
// FUNCTION: STBC 0x00734580 _$E174
// FUNCTION: STBC 0x00734590 _$E377
// FUNCTION: STBC 0x007345a0 _$E376
// FUNCTION: STBC 0x007345b0 _$E160
// FUNCTION: STBC 0x007345c0 _$E159
// FUNCTION: STBC 0x007345d0 _$E381
// FUNCTION: STBC 0x007345e0 _$E380
// FUNCTION: STBC 0x007345f0 _$E385
// FUNCTION: STBC 0x00734600 _$E384
// FUNCTION: STBC 0x00734610 _$E93
// FUNCTION: STBC 0x00734620 _$E92
// FUNCTION: STBC 0x00734630 _$E180
// FUNCTION: STBC 0x00734640 _$E179
// FUNCTION: STBC 0x00734650 _$E389
// FUNCTION: STBC 0x00734660 _$E388
// FUNCTION: STBC 0x00734670 _$E393
// FUNCTION: STBC 0x00734680 _$E392
// FUNCTION: STBC 0x00734690 _$E397
// FUNCTION: STBC 0x007346a0 _$E396
// FUNCTION: STBC 0x007346b0 _$E401
// FUNCTION: STBC 0x007346c0 _$E400
// FUNCTION: STBC 0x007346d0 _$E405
// FUNCTION: STBC 0x007346e0 _$E404
// FUNCTION: STBC 0x007346f0 _$E409
// FUNCTION: STBC 0x00734700 _$E408
// FUNCTION: STBC 0x00734710 _$E413
// FUNCTION: STBC 0x00734720 _$E412
// FUNCTION: STBC 0x00734730 _$E417
// FUNCTION: STBC 0x00734740 _$E416
// FUNCTION: STBC 0x00734750 _$E185
// FUNCTION: STBC 0x00734760 _$E184
// FUNCTION: STBC 0x00734770 _$E421
// FUNCTION: STBC 0x00734780 _$E420
// FUNCTION: STBC 0x00734790 _$E190
// FUNCTION: STBC 0x007347a0 _$E189
// FUNCTION: STBC 0x007347b0 _$E195
// FUNCTION: STBC 0x007347c0 _$E194

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x007347d0 _$E426
// FUNCTION: STBC 0x007347e0 _$E423
// FUNCTION: STBC 0x007347f0 _$E425
// FUNCTION: STBC 0x00734800 _$E424
// DATA: STBC 0x0099d8e0
TGFontManager g_kFontManager;

/*
================
TGFontManager::TGFontManager
================
*/
// FUNCTION: STBC 0x00734810
TGFontManager::TGFontManager( void ) : families( 2 ) {
	defaultFontName = NULL;
	defaultFontSize = 0.0f;
}

/*
================
TGFontManager::~TGFontManager

Deletes the families and the default font's name.
================
*/
// FUNCTION: STBC 0x00734840
TGFontManager::~TGFontManager( void ) {
	NiTListIterator pos = families.GetHeadPos();
	while ( pos ) {
		delete families.GetNext( pos );
	}
	if ( defaultFontName ) {
		delete [] defaultFontName;
	}
}

/*
================
TGFontFamily::TGFontFamily
================
*/
// FUNCTION: STBC 0x007348f0
TGFontFamily::TGFontFamily( char *name_ ) : registrations( 2 ), fontGroups( 2 ) {
	if ( name_ == NULL ) {
		name = NULL;
	} else {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
}

/*
================
TGFontFamily::~TGFontFamily

The registrations and the groups belong to the managers; only the lists'
items are freed here.
================
*/
// FUNCTION: STBC 0x007349b0
TGFontFamily::~TGFontFamily( void ) {
	delete [] name;
}

/*
================
TGFontManager::RegisterFont

Registers size of the font name, which the script module's function
creates on demand; the first font registered becomes the default.
================
*/
// FUNCTION: STBC 0x00734a30
// PARKED: retail keeps the family in a stack slot through the search and pushes ebp only in the creation path; the search spellings tried (break, a search helper, a loop condition) keep it in ebp
void TGFontManager::RegisterFont( char *name, float size, char *module, char *function ) {
	TGFontFamily *family = NULL;
	NiTListIterator pos = families.GetHeadPos();
	while ( pos ) {
		TGFontFamily *candidate = families.GetNext( pos );
		if ( strcmp( name, candidate->name ) == 0 ) {
			family = candidate;
			break;
		}
	}
	if ( family == NULL ) {
		family = new TGFontFamily( name );
		families.AddTail( family );
	}
	family->RegisterSize( size, module, function );

	if ( defaultFontName == NULL ) {
		defaultFontName = new char[strlen( name ) + 1];
		strcpy( defaultFontName, name );
		defaultFontSize = size;
	}
}

/*
================
TGFontFamily::RegisterSize

Records that the script module's function creates the family's font of
this size, and registers the icon group of that name ("<name><size>").
================
*/
// FUNCTION: STBC 0x00734c20
void TGFontFamily::RegisterSize( float size, char *module, char *function ) {
	TGFontRegistration *registration = new TGFontRegistration( name, module, function, size );
	registrations.AddTail( registration );

	char groupName[128];
	sprintf( groupName, "%s%.2f", name, size );
	g_kIconManager.RegisterIconGroup( groupName, module, function );
}

/*
================
TGFontManager::CreateFontGroup

A new, empty group of the font name in size; the caller adds it.
================
*/
// FUNCTION: STBC 0x00734db0
TGFontGroup *TGFontManager::CreateFontGroup( char *name, float size ) {
	return new TGFontGroup( name, size );
}

/*
================
TGFontManager::AddFontGroup

Gives group to the family of its font; nothing when the font is not
registered.
================
*/
// FUNCTION: STBC 0x00734e20
void TGFontManager::AddFontGroup( TGFontGroup *group ) {
	NiTListIterator pos = families.GetHeadPos();
	while ( pos ) {
		TGFontFamily *family = families.GetNext( pos );
		if ( strcmp( group->GetFontName(), family->name ) == 0 ) {
			family->AddFontGroup( group );
			return;
		}
	}
}

/*
================
TGFontFamily::AddFontGroup

Adds a loaded group to the family and to the icon manager's groups.
================
*/
// FUNCTION: STBC 0x00734e90 ?AddFontGroup@TGFontFamily@@QAEXPAVTGFontGroup@@@Z
void TGFontFamily::AddFontGroup( TGFontGroup *group ) {
	// BUG: walks the whole list and does nothing with the groups it passes.
	NiTListIterator pos = fontGroups.GetHeadPos();
	while ( pos ) {
		fontGroups.GetNext( pos );
	}
	fontGroups.AddTail( group );
	g_kIconManager.AddIconGroup( group );
}

/*
================
TGFontManager::SetDefaultFont

The font a paragraph falls back on when its own is not found.
================
*/
// FUNCTION: STBC 0x00734f10
void TGFontManager::SetDefaultFont( char *name, float size ) {
	if ( defaultFontName ) {
		delete [] defaultFontName;
	}
	defaultFontName = new char[strlen( name ) + 1];
	strcpy( defaultFontName, name );
	defaultFontSize = size;
}

/*
================
TGFontManager::GetFontGroup

The group of the font name in size, loaded on demand; NULL when no font of
that name is registered.
================
*/
// FUNCTION: STBC 0x00734f70
TGFontGroup *TGFontManager::GetFontGroup( char *name, float size ) {
	if ( name ) {
		NiTListIterator pos = families.GetHeadPos();
		while ( pos ) {
			TGFontFamily *family = families.GetNext( pos );
			if ( family && strcmp( name, family->name ) == 0 ) {
				return family->GetFontGroup( size );
			}
		}
	}
	return NULL;
}

/*
================
TGFontFamily::GetFontGroup

The family's group of this size. One not loaded yet is made from the
registration of that size, else of twice the size, else of the largest
size: its script function creates that group, which is then copied at the
size asked for and handed to the font manager.
================
*/
// FUNCTION: STBC 0x00734fe0
// DRAFT: register residue only: retail swaps ECX/EDX in the inline GetNext and pushes ESI/EDI after the early return
TGFontGroup *TGFontFamily::GetFontGroup( float size ) {
	TGFontGroup *group = FindFontGroup( size );
	if ( group != NULL ) {
		return group;
	}

	TGFontRegistration *best = NULL;
	NiTListIterator pos = registrations.GetHeadPos();
	while ( pos ) {
		TGFontRegistration *registration = registrations.GetNext( pos );
		if ( registration->size == size ) {
			best = registration;
			break;
		}
		float twice = size * 2.0f;
		if ( registration->size == twice || best == NULL || ( best->size != twice && registration->size > best->size ) ) {
			best = registration;
		}
	}

	// BUG: a family with no registrations dereferences NULL here
	group = FindFontGroup( best->size );
	if ( group == NULL ) {
		PyEmbed::CallFunction( best->module, best->function, NULL, NULL, "(O)", Py_None );
		group = FindFontGroup( best->size );
	}
	if ( group->GetFontSize() != size ) {
		TGFontGroup *scaled = new TGFontGroup( NULL, 0.0f );
		group->Unknown744430( scaled, size );
		g_kFontManager.AddFontGroup( scaled );
		group = scaled;
	}
	return group;
}

/*
================
TGFontFamily::FindFontGroup
================
*/
// FUNCTION: STBC 0x00735120
TGFontGroup *TGFontFamily::FindFontGroup( float size ) {
	NiTListIterator pos = fontGroups.GetHeadPos();
	while ( pos ) {
		TGFontGroup *group = fontGroups.GetNext( pos );
		if ( group != NULL && group->GetFontSize() == size ) {
			return group;
		}
	}
	return NULL;
}

/*
================
TGFontManager::Purge

Deletes the font groups nothing holds.
================
*/
// FUNCTION: STBC 0x00735150
void TGFontManager::Purge( void ) {
	NiTListIterator pos = families.GetHeadPos();
	while ( pos ) {
		families.GetNext( pos )->Purge();
	}
}

/*
================
TGFontFamily::Purge

Deletes the groups nothing holds.
================
*/
// FUNCTION: STBC 0x00735170 ?Purge@TGFontFamily@@QAEXXZ
void TGFontFamily::Purge( void ) {
	NiTListIterator pos = fontGroups.GetHeadPos();
	while ( pos ) {
		TGFontGroup *group = fontGroups.Get( pos );
		// BUG: a missing group never moves the position on, so the loop would not end.
		if ( group != NULL ) {
			if ( group->GetRefCount() == 0 ) {
				fontGroups.RemovePos( pos );
				delete group;
			} else {
				pos = fontGroups.GetNextPos( pos );
			}
		}
	}
}

/*
================
TGFontManager::RemoveGroup

Takes group out of the family of its font (the group's destructor calls
this).
================
*/
// FUNCTION: STBC 0x00735280
void TGFontManager::RemoveGroup( TGFontGroup *group ) {
	NiTListIterator pos = families.GetHeadPos();
	while ( pos ) {
		TGFontFamily *family = families.GetNext( pos );
		if ( strcmp( family->name, group->GetFontName() ) == 0 ) {
			family->RemoveGroup( group );
			return;
		}
	}
}

/*
================
TGFontFamily::RemoveGroup

Takes the group out of the family without deleting it.
================
*/
// FUNCTION: STBC 0x007352f0 ?RemoveGroup@TGFontFamily@@QAEXPAVTGFontGroup@@@Z
void TGFontFamily::RemoveGroup( TGFontGroup *group ) {
	NiTListIterator pos = fontGroups.GetHeadPos();
	while ( pos ) {
		TGFontGroup *candidate = fontGroups.GetNext( pos );
		if ( candidate == group ) {
			fontGroups.Remove( candidate );
			return;
		}
	}
}

/*
================
TGFontManager::WriteToStream

The families, then the default font's name and size.
================
*/
// FUNCTION: STBC 0x00735410
bool TGFontManager::WriteToStream( TGStream *stream ) {
	stream->WriteLong( families.GetSize() );
	NiTListIterator pos = families.GetHeadPos();
	while ( pos != NULL ) {
		families.GetNext( pos )->WriteToStream( stream );
	}
	stream->VirtualSlot11( defaultFontName );
	stream->WriteFloat( defaultFontSize );
	return true;
}

/*
================
TGFontFamily::WriteToStream

The name, the registrations and the groups.
================
*/
// FUNCTION: STBC 0x00735460
bool TGFontFamily::WriteToStream( TGStream *stream ) {
	stream->VirtualSlot11( name );
	stream->WriteLong( registrations.GetSize() );
	NiTListIterator pos = registrations.GetHeadPos();
	while ( pos != NULL ) {
		registrations.GetNext( pos )->WriteToStream( stream );
	}
	stream->WriteLong( fontGroups.GetSize() );
	pos = fontGroups.GetHeadPos();
	while ( pos != NULL ) {
		fontGroups.GetNext( pos )->WriteToStream( stream );
	}
	return true;
}

/*
================
TGFontRegistration::WriteToStream

The registration, then the size.
================
*/
// FUNCTION: STBC 0x007354d0
bool TGFontRegistration::WriteToStream( TGStream *stream ) {
	// BUG: reads the registration's strings from the stream it should
	// write them to (TGIconGroupRegistration::ReadFromStream).
	if ( !TGIconGroupRegistration::ReadFromStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( size );
	return true;
}

/*
================
TGFontManager::ReadFromStream

Reads what WriteToStream wrote.
================
*/
// FUNCTION: STBC 0x00735500
bool TGFontManager::ReadFromStream( TGStream *stream ) {
	int count = stream->ReadLong();
	for ( int i = count; i > 0; i-- ) {
		TGFontFamily *family = new TGFontFamily( NULL );
		family->ReadFromStream( stream );
		families.AddTail( family );
	}
	defaultFontName = stream->VirtualSlot10( -1 );
	defaultFontSize = stream->ReadFloat();
	return true;
}

/*
================
TGFontFamily::ReadFromStream

Reads what WriteToStream wrote.
================
*/
// FUNCTION: STBC 0x00735660
bool TGFontFamily::ReadFromStream( TGStream *stream ) {
	name = stream->VirtualSlot10( -1 );
	int count = stream->ReadLong();
	int i;
	for ( i = count; i > 0; i-- ) {
		TGFontRegistration *registration = new TGFontRegistration;
		registration->ReadFromStream( stream );
		registrations.AddTail( registration );
	}
	count = stream->ReadLong();
	for ( i = count; i > 0; i-- ) {
		TGFontGroup *group = new TGFontGroup;
		group->ReadFromStream( stream );
		fontGroups.AddTail( group );
	}
	return true;
}

/*
================
TGFontRegistration::ReadFromStream

Reads what WriteToStream should have written.
================
*/
// FUNCTION: STBC 0x007358e0
bool TGFontRegistration::ReadFromStream( TGStream *stream ) {
	if ( !TGIconGroupRegistration::ReadFromStream( stream ) ) {
		return false;
	}
	size = stream->ReadFloat();
	return true;
}

/*
================
TGFontManager::ForgetAll

Forgets the font families and the default font without deleting them.
================
*/
// FUNCTION: STBC 0x00735910
void TGFontManager::ForgetAll( void ) {
	families.RemoveAll();
	defaultFontName = NULL;
	defaultFontSize = 0.0f;
}
