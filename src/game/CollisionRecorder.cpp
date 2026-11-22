/*
===============================================================================

	CollisionRecorder.cpp

===============================================================================
*/

#include <iostream>
#include <new>
#include "CollisionRecorder.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0058ab20 _$E421
// FUNCTION: STBC 0x0058ab40 _$E420

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0058a5e0 _$E32
// FUNCTION: STBC 0x0058a5f0 _$E29
// FUNCTION: STBC 0x0058a600 _$E31
// FUNCTION: STBC 0x0058a610 _$E30
// FUNCTION: STBC 0x0058a620 _$E38
// FUNCTION: STBC 0x0058a630 _$E35
// FUNCTION: STBC 0x0058a640 _$E37
// FUNCTION: STBC 0x0058a650 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0058a660 _$E50
// FUNCTION: STBC 0x0058a670 _$E49
// FUNCTION: STBC 0x0058a680 _$E54
// FUNCTION: STBC 0x0058a690 _$E53
// FUNCTION: STBC 0x0058a6a0 _$E65
// FUNCTION: STBC 0x0058a6b0 _$E64
// FUNCTION: STBC 0x0058a6c0 _$E69
// FUNCTION: STBC 0x0058a6d0 _$E68
// FUNCTION: STBC 0x0058a6e0 _$E73
// FUNCTION: STBC 0x0058a6f0 _$E72
// FUNCTION: STBC 0x0058a700 _$E77
// FUNCTION: STBC 0x0058a710 _$E76
// FUNCTION: STBC 0x0058a720 _$E196
// FUNCTION: STBC 0x0058a730 _$E195
// FUNCTION: STBC 0x0058a740 _$E138
// FUNCTION: STBC 0x0058a750 _$E137
// FUNCTION: STBC 0x0058a760 _$E151
// FUNCTION: STBC 0x0058a770 _$E150
// FUNCTION: STBC 0x0058a780 _$E201
// FUNCTION: STBC 0x0058a790 _$E200
// FUNCTION: STBC 0x0058a7a0 _$E206
// FUNCTION: STBC 0x0058a7b0 _$E205
// FUNCTION: STBC 0x0058a7c0 _$E210
// FUNCTION: STBC 0x0058a7d0 _$E209
// FUNCTION: STBC 0x0058a7e0 _$E214
// FUNCTION: STBC 0x0058a7f0 _$E213
// FUNCTION: STBC 0x0058a800 _$E221
// FUNCTION: STBC 0x0058a810 _$E220
// FUNCTION: STBC 0x0058a820 _$E246
// FUNCTION: STBC 0x0058a830 _$E245
// FUNCTION: STBC 0x0058a840 _$E250
// FUNCTION: STBC 0x0058a850 _$E249
// FUNCTION: STBC 0x0058a860 _$E96
// FUNCTION: STBC 0x0058a870 _$E95
// FUNCTION: STBC 0x0058a880 _$E232
// FUNCTION: STBC 0x0058a890 _$E231
// FUNCTION: STBC 0x0058a8a0 _$E239
// FUNCTION: STBC 0x0058a8b0 _$E238
// FUNCTION: STBC 0x0058a8c0 _$E262
// FUNCTION: STBC 0x0058a8d0 _$E261
// FUNCTION: STBC 0x0058a8e0 _$E266
// FUNCTION: STBC 0x0058a8f0 _$E265
// FUNCTION: STBC 0x0058a900 _$E306
// FUNCTION: STBC 0x0058a910 _$E305
// FUNCTION: STBC 0x0058a920 _$E312
// FUNCTION: STBC 0x0058a930 _$E311
// FUNCTION: STBC 0x0058a940 _$E254
// FUNCTION: STBC 0x0058a950 _$E253
// FUNCTION: STBC 0x0058a960 _$E258
// FUNCTION: STBC 0x0058a970 _$E257
// FUNCTION: STBC 0x0058a980 _$E270
// FUNCTION: STBC 0x0058a990 _$E269
// FUNCTION: STBC 0x0058a9a0 _$E274
// FUNCTION: STBC 0x0058a9b0 _$E273
// FUNCTION: STBC 0x0058a9c0 _$E278
// FUNCTION: STBC 0x0058a9d0 _$E277
// FUNCTION: STBC 0x0058a9e0 _$E282
// FUNCTION: STBC 0x0058a9f0 _$E281
// FUNCTION: STBC 0x0058aa00 _$E286
// FUNCTION: STBC 0x0058aa10 _$E285
// FUNCTION: STBC 0x0058aa20 _$E290
// FUNCTION: STBC 0x0058aa30 _$E289
// FUNCTION: STBC 0x0058ab00 _$E294
// FUNCTION: STBC 0x0058ab10 _$E293
// FUNCTION: STBC 0x0058ab50 _$E298
// FUNCTION: STBC 0x0058ab60 _$E297
// FUNCTION: STBC 0x0058ab70 _$E101
// FUNCTION: STBC 0x0058ab80 _$E100
// FUNCTION: STBC 0x0058ab90 _$E302
// FUNCTION: STBC 0x0058aba0 _$E301
// FUNCTION: STBC 0x0058abb0 _$E106
// FUNCTION: STBC 0x0058abc0 _$E105
// FUNCTION: STBC 0x0058abd0 _$E316
// FUNCTION: STBC 0x0058abe0 _$E315
// FUNCTION: STBC 0x0058abf0 _$E320
// FUNCTION: STBC 0x0058ac00 _$E319
// FUNCTION: STBC 0x0058ac10 _$E324
// FUNCTION: STBC 0x0058ac20 _$E323
// FUNCTION: STBC 0x0058ac30 _$E328
// FUNCTION: STBC 0x0058ac40 _$E327
// FUNCTION: STBC 0x0058ac50 _$E115
// FUNCTION: STBC 0x0058ac60 _$E114
// FUNCTION: STBC 0x0058ac70 _$E332
// FUNCTION: STBC 0x0058ac80 _$E331
// FUNCTION: STBC 0x0058ac90 _$E336
// FUNCTION: STBC 0x0058aca0 _$E335
// FUNCTION: STBC 0x0058acb0 _$E340
// FUNCTION: STBC 0x0058acc0 _$E339
// FUNCTION: STBC 0x0058acd0 _$E121
// FUNCTION: STBC 0x0058ace0 _$E120
// FUNCTION: STBC 0x0058acf0 _$E126
// FUNCTION: STBC 0x0058ad00 _$E125
// FUNCTION: STBC 0x0058ad10 _$E156
// FUNCTION: STBC 0x0058ad20 _$E155
// FUNCTION: STBC 0x0058ad30 _$E344
// FUNCTION: STBC 0x0058ad40 _$E343
// FUNCTION: STBC 0x0058ad50 _$E161
// FUNCTION: STBC 0x0058ad60 _$E160
// FUNCTION: STBC 0x0058ad70 _$E348
// FUNCTION: STBC 0x0058ad80 _$E347
// FUNCTION: STBC 0x0058ad90 _$E352
// FUNCTION: STBC 0x0058ada0 _$E351
// FUNCTION: STBC 0x0058adb0 _$E166
// FUNCTION: STBC 0x0058adc0 _$E165
// FUNCTION: STBC 0x0058add0 _$E356
// FUNCTION: STBC 0x0058ade0 _$E355
// FUNCTION: STBC 0x0058adf0 _$E131
// FUNCTION: STBC 0x0058ae00 _$E130
// FUNCTION: STBC 0x0058ae10 _$E360
// FUNCTION: STBC 0x0058ae20 _$E359
// FUNCTION: STBC 0x0058ae30 _$E364
// FUNCTION: STBC 0x0058ae40 _$E363
// FUNCTION: STBC 0x0058ae50 _$E171
// FUNCTION: STBC 0x0058ae60 _$E170
// FUNCTION: STBC 0x0058ae70 _$E176
// FUNCTION: STBC 0x0058ae80 _$E175
// FUNCTION: STBC 0x0058ae90 _$E368
// FUNCTION: STBC 0x0058aea0 _$E367
// FUNCTION: STBC 0x0058aeb0 _$E372
// FUNCTION: STBC 0x0058aec0 _$E371
// FUNCTION: STBC 0x0058aed0 _$E376
// FUNCTION: STBC 0x0058aee0 _$E375
// FUNCTION: STBC 0x0058aef0 _$E380
// FUNCTION: STBC 0x0058af00 _$E379
// FUNCTION: STBC 0x0058af10 _$E384
// FUNCTION: STBC 0x0058af20 _$E383
// FUNCTION: STBC 0x0058af30 _$E388
// FUNCTION: STBC 0x0058af40 _$E387
// FUNCTION: STBC 0x0058af50 _$E392
// FUNCTION: STBC 0x0058af60 _$E391
// FUNCTION: STBC 0x0058af70 _$E396
// FUNCTION: STBC 0x0058af80 _$E395
// FUNCTION: STBC 0x0058af90 _$E181
// FUNCTION: STBC 0x0058afa0 _$E180
// FUNCTION: STBC 0x0058afb0 _$E400
// FUNCTION: STBC 0x0058afc0 _$E399
// FUNCTION: STBC 0x0058afd0 _$E186
// FUNCTION: STBC 0x0058afe0 _$E185
// FUNCTION: STBC 0x0058aff0 _$E191
// FUNCTION: STBC 0x0058b000 _$E190
// FUNCTION: STBC 0x0058b010 _$E404
// FUNCTION: STBC 0x0058b020 _$E403
// FUNCTION: STBC 0x0058b030 _$E409
// FUNCTION: STBC 0x0058b040 _$E408
// FUNCTION: STBC 0x0058b050 _$E413
// FUNCTION: STBC 0x0058b060 _$E412

/*
================
CollisionRecorder::CollisionRecorder
================
*/
// FUNCTION: STBC 0x0058a1a0
CollisionRecorder::CollisionRecorder( void ) : records( 2 ) {
	unknown00 = false;
	current = NULL;
}

/*
================
CollisionRecorder::~CollisionRecorder

Deletes the records and their points.
================
*/
// FUNCTION: STBC 0x0058a1c0
CollisionRecorder::~CollisionRecorder( void ) {
	while ( !records.IsEmpty() ) {
		CollisionRecord *record = records.RemoveHead();
		while ( !record->points.IsEmpty() ) {
			delete record->points.RemoveHead();
		}
		delete record;
	}
}

/*
================
CollisionRecorder::BeginRecord
================
*/
// FUNCTION: STBC 0x0058a2d0
void CollisionRecorder::BeginRecord( int type, DamageableObject *objectA, DamageableObject *objectB ) {
	if ( current != NULL ) {
		EndRecord();
	}
	current = new CollisionRecord;
	current->type = type;
	current->objectA = objectA;
	current->objectB = objectB;
}

/*
================
CollisionRecorder::EndRecord
================
*/
// FUNCTION: STBC 0x0058a330
void CollisionRecorder::EndRecord( void ) {
	if ( current != NULL ) {
		if ( !current->points.IsEmpty() ) {
			records.AddTail( current );
		} else {
			delete current;
		}
		current = NULL;
	}
}

/*
================
CollisionRecorder::AddPoint
================
*/
// FUNCTION: STBC 0x0058a430
void CollisionRecorder::AddPoint( NiPoint3 *point ) {
	if ( current != NULL ) {
		current->points.AddTail( point );
	}
}

/*
================
CollisionRecorder::GetPoints

A record found with the objects the other way round is turned to the
caller's order first.
================
*/
// FUNCTION: STBC 0x0058a4a0
bool CollisionRecorder::GetPoints( NiTList<NiPoint3 *> *list, int type, DamageableObject *objectA, DamageableObject *objectB ) {
	for ( NiTListIterator pos = records.GetHeadPos(); pos != NULL; pos = records.GetNextPos( pos ) ) {
		CollisionRecord *record = records.Get( pos );
		if ( record->type == type ) {
			if ( record->objectA == objectA && record->objectB == objectB ) {
				for ( NiTListIterator point = record->points.GetHeadPos(); point != NULL; point = record->points.GetNextPos( point ) ) {
					list->AddTail( record->points.Get( point ) );
				}
			} else if ( record->objectA == objectB && record->objectB == objectA && type != 1 ) {
				DamageableObject *swap = record->objectB;
				record->objectB = record->objectA;
				record->objectA = swap;
				for ( NiTListIterator point = record->points.GetHeadPos(); point != NULL; point = record->points.GetNextPos( point ) ) {
					list->AddTail( record->points.Get( point ) );
				}
			}
		}
	}
	return !list->IsEmpty();
}
