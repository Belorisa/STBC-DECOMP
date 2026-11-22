/*
===============================================================================

	SetObjectTable.cpp

===============================================================================
*/

#include <iostream>
#include "SetObjectTable.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0040baf0 _$E437
// FUNCTION: STBC 0x0040bb10 _$E436

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0040b570 _$E32
// FUNCTION: STBC 0x0040b580 _$E29
// FUNCTION: STBC 0x0040b590 _$E31
// FUNCTION: STBC 0x0040b5a0 _$E30
// FUNCTION: STBC 0x0040b5b0 _$E38
// FUNCTION: STBC 0x0040b5c0 _$E35
// FUNCTION: STBC 0x0040b5d0 _$E37
// FUNCTION: STBC 0x0040b5e0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0040b5f0 _$E66
// FUNCTION: STBC 0x0040b600 _$E65
// FUNCTION: STBC 0x0040b610 _$E221
// FUNCTION: STBC 0x0040b620 _$E220
// FUNCTION: STBC 0x0040b630 _$E77
// FUNCTION: STBC 0x0040b640 _$E76
// FUNCTION: STBC 0x0040b650 _$E81
// FUNCTION: STBC 0x0040b660 _$E80
// FUNCTION: STBC 0x0040b670 _$E85
// FUNCTION: STBC 0x0040b680 _$E84
// FUNCTION: STBC 0x0040b690 _$E89
// FUNCTION: STBC 0x0040b6a0 _$E88
// FUNCTION: STBC 0x0040b6b0 _$E206
// FUNCTION: STBC 0x0040b6c0 _$E205
// FUNCTION: STBC 0x0040b6d0 _$E148
// FUNCTION: STBC 0x0040b6e0 _$E147
// FUNCTION: STBC 0x0040b6f0 _$E161
// FUNCTION: STBC 0x0040b700 _$E160
// FUNCTION: STBC 0x0040b710 _$E211
// FUNCTION: STBC 0x0040b720 _$E210
// FUNCTION: STBC 0x0040b730 _$E226
// FUNCTION: STBC 0x0040b740 _$E225
// FUNCTION: STBC 0x0040b750 _$E215
// FUNCTION: STBC 0x0040b760 _$E214
// FUNCTION: STBC 0x0040b770 _$E230
// FUNCTION: STBC 0x0040b780 _$E229
// FUNCTION: STBC 0x0040b790 _$E237
// FUNCTION: STBC 0x0040b7a0 _$E236
// FUNCTION: STBC 0x0040b7b0 _$E262
// FUNCTION: STBC 0x0040b7c0 _$E261
// FUNCTION: STBC 0x0040b7d0 _$E266
// FUNCTION: STBC 0x0040b7e0 _$E265
// FUNCTION: STBC 0x0040b7f0 _$E106
// FUNCTION: STBC 0x0040b800 _$E105
// FUNCTION: STBC 0x0040b810 _$E248
// FUNCTION: STBC 0x0040b820 _$E247
// FUNCTION: STBC 0x0040b830 _$E255
// FUNCTION: STBC 0x0040b840 _$E254
// FUNCTION: STBC 0x0040b850 _$E278
// FUNCTION: STBC 0x0040b860 _$E277
// FUNCTION: STBC 0x0040b870 _$E282
// FUNCTION: STBC 0x0040b880 _$E281
// FUNCTION: STBC 0x0040b890 _$E322
// FUNCTION: STBC 0x0040b8a0 _$E321
// FUNCTION: STBC 0x0040b8b0 _$E328
// FUNCTION: STBC 0x0040b8c0 _$E327
// FUNCTION: STBC 0x0040b8d0 _$E270
// FUNCTION: STBC 0x0040b8e0 _$E269
// FUNCTION: STBC 0x0040b8f0 _$E274
// FUNCTION: STBC 0x0040b900 _$E273
// FUNCTION: STBC 0x0040b910 _$E286
// FUNCTION: STBC 0x0040b920 _$E285
// FUNCTION: STBC 0x0040b930 _$E290
// FUNCTION: STBC 0x0040b940 _$E289
// FUNCTION: STBC 0x0040b950 _$E294
// FUNCTION: STBC 0x0040b960 _$E293
// FUNCTION: STBC 0x0040b970 _$E298
// FUNCTION: STBC 0x0040b980 _$E297
// FUNCTION: STBC 0x0040b990 _$E302
// FUNCTION: STBC 0x0040b9a0 _$E301
// FUNCTION: STBC 0x0040b9b0 _$E306
// FUNCTION: STBC 0x0040b9c0 _$E305
// FUNCTION: STBC 0x0040b9d0 _$E310
// FUNCTION: STBC 0x0040b9e0 _$E309
// FUNCTION: STBC 0x0040b9f0 _$E314
// FUNCTION: STBC 0x0040ba00 _$E313
// FUNCTION: STBC 0x0040ba10 _$E111
// FUNCTION: STBC 0x0040ba20 _$E110
// FUNCTION: STBC 0x0040ba30 _$E318
// FUNCTION: STBC 0x0040ba40 _$E317
// FUNCTION: STBC 0x0040ba50 _$E116
// FUNCTION: STBC 0x0040ba60 _$E115
// FUNCTION: STBC 0x0040ba70 _$E332
// FUNCTION: STBC 0x0040ba80 _$E331
// FUNCTION: STBC 0x0040ba90 _$E336
// FUNCTION: STBC 0x0040baa0 _$E335
// FUNCTION: STBC 0x0040bab0 _$E340
// FUNCTION: STBC 0x0040bac0 _$E339
// FUNCTION: STBC 0x0040bad0 _$E344
// FUNCTION: STBC 0x0040bae0 _$E343
// FUNCTION: STBC 0x0040bb20 _$E125
// FUNCTION: STBC 0x0040bb30 _$E124
// FUNCTION: STBC 0x0040bb40 _$E348
// FUNCTION: STBC 0x0040bb50 _$E347
// FUNCTION: STBC 0x0040bb60 _$E352
// FUNCTION: STBC 0x0040bb70 _$E351
// FUNCTION: STBC 0x0040bb80 _$E356
// FUNCTION: STBC 0x0040bb90 _$E355
// FUNCTION: STBC 0x0040bba0 _$E131
// FUNCTION: STBC 0x0040bbb0 _$E130
// FUNCTION: STBC 0x0040bbc0 _$E136
// FUNCTION: STBC 0x0040bbd0 _$E135
// FUNCTION: STBC 0x0040bbe0 _$E166
// FUNCTION: STBC 0x0040bbf0 _$E165
// FUNCTION: STBC 0x0040bc00 _$E360
// FUNCTION: STBC 0x0040bc10 _$E359
// FUNCTION: STBC 0x0040bc20 _$E171
// FUNCTION: STBC 0x0040bc30 _$E170
// FUNCTION: STBC 0x0040bc40 _$E364
// FUNCTION: STBC 0x0040bc50 _$E363
// FUNCTION: STBC 0x0040bc60 _$E368
// FUNCTION: STBC 0x0040bc70 _$E367
// FUNCTION: STBC 0x0040bc80 _$E176
// FUNCTION: STBC 0x0040bc90 _$E175
// FUNCTION: STBC 0x0040bca0 _$E372
// FUNCTION: STBC 0x0040bcb0 _$E371
// FUNCTION: STBC 0x0040bcc0 _$E141
// FUNCTION: STBC 0x0040bcd0 _$E140
// FUNCTION: STBC 0x0040bce0 _$E376
// FUNCTION: STBC 0x0040bcf0 _$E375
// FUNCTION: STBC 0x0040bd00 _$E380
// FUNCTION: STBC 0x0040bd10 _$E379
// FUNCTION: STBC 0x0040bd20 _$E181
// FUNCTION: STBC 0x0040bd30 _$E180
// FUNCTION: STBC 0x0040bd40 _$E186
// FUNCTION: STBC 0x0040bd50 _$E185
// FUNCTION: STBC 0x0040bd60 _$E384
// FUNCTION: STBC 0x0040bd70 _$E383
// FUNCTION: STBC 0x0040bd80 _$E388
// FUNCTION: STBC 0x0040bd90 _$E387
// FUNCTION: STBC 0x0040bda0 _$E392
// FUNCTION: STBC 0x0040bdb0 _$E391
// FUNCTION: STBC 0x0040bdc0 _$E396
// FUNCTION: STBC 0x0040bdd0 _$E395
// FUNCTION: STBC 0x0040bde0 _$E400
// FUNCTION: STBC 0x0040bdf0 _$E399
// FUNCTION: STBC 0x0040be00 _$E404
// FUNCTION: STBC 0x0040be10 _$E403
// FUNCTION: STBC 0x0040be20 _$E408
// FUNCTION: STBC 0x0040be30 _$E407
// FUNCTION: STBC 0x0040be40 _$E412
// FUNCTION: STBC 0x0040be50 _$E411
// FUNCTION: STBC 0x0040be60 _$E191
// FUNCTION: STBC 0x0040be70 _$E190
// FUNCTION: STBC 0x0040be80 _$E416
// FUNCTION: STBC 0x0040be90 _$E415
// FUNCTION: STBC 0x0040bea0 _$E196
// FUNCTION: STBC 0x0040beb0 _$E195
// FUNCTION: STBC 0x0040bec0 _$E201
// FUNCTION: STBC 0x0040bed0 _$E200
// FUNCTION: STBC 0x0040bee0 _$E420
// FUNCTION: STBC 0x0040bef0 _$E419
// FUNCTION: STBC 0x0040bf00 _$E425
// FUNCTION: STBC 0x0040bf10 _$E424
// FUNCTION: STBC 0x0040bf20 _$E429
// FUNCTION: STBC 0x0040bf30 _$E428

/*
================
SetObjectTable::SetObjectTable
================
*/
// FUNCTION: STBC 0x0040af40
SetObjectTable::SetObjectTable( void ) {
	lastType = -1;
	lastList = NULL;
}

/*
================
SetObjectTable::~SetObjectTable
================
*/
// FUNCTION: STBC 0x0040af80
SetObjectTable::~SetObjectTable( void ) {
}

/*
================
SetObjectTable::GetObjects

The objects of the type and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x0040afe0 ?GetObjects@SetObjectTable@@QAEPAPAVBaseObjectClass@@HPAH@Z
BaseObjectClass **SetObjectTable::GetObjects( int type, int *count_ ) {
	SetObjectTypeList *list = lists.Get( type );
	if ( list != NULL ) {
		lastType = type;
		lastList = list;
		*count_ = list->objects.GetCount();
		return list->objects.GetItems();
	}
	lastType = -1;
	lastList = NULL;
	*count_ = 0;
	return NULL;
}

/*
================
SetObjectTable::GetObjects

The objects of the type; NULL when there are none.
================
*/
// FUNCTION: STBC 0x0040b070 ?GetObjects@SetObjectTable@@QAEPAPAVBaseObjectClass@@H@Z
BaseObjectClass **SetObjectTable::GetObjects( int type ) {
	SetObjectTypeList *list = lists.Get( type );
	if ( list != NULL ) {
		lastList = list;
		lastType = type;
		return list->objects.GetItems();
	}
	lastType = -1;
	lastList = NULL;
	return NULL;
}

/*
================
SetObjectTable::GetNumObjects

The number of objects of the type.
================
*/
// FUNCTION: STBC 0x0040b0f0
int SetObjectTable::GetNumObjects( int type ) {
	if ( type == lastType && lastList != NULL ) {
		return lastList->objects.GetCount();
	}
	SetObjectTypeList *list = lists.Get( type );
	if ( list != NULL ) {
		return list->objects.GetCount();
	}
	return 0;
}

/*
================
SetObjectTable::GetObject

The object of the type with the ID; NULL when there is none.
================
*/
// FUNCTION: STBC 0x0040b170
BaseObjectClass *SetObjectTable::GetObject( int type, long id ) {
	SetObjectTypeList *list = lists.Get( type );
	if ( list != NULL ) {
		return list->objects.Get( id );
	}
	return NULL;
}

/*
================
SetObjectTable::AddObject

Adds the object under its class type and those of its base classes.
================
*/
// FUNCTION: STBC 0x0040b220
void SetObjectTable::AddObject( BaseObjectClass *object, SetClass *set ) {
	object->AddToSet( set );
}

/*
================
SetObjectTable::Add

Adds the object to the list of the type, which is created for the first
object of its type.
================
*/
// FUNCTION: STBC 0x0040b240
void SetObjectTable::Add( BaseObjectClass *object, int type ) {
	SetObjectTypeList *list = lists.Get( type );
	if ( list == NULL ) {
		list = new SetObjectTypeList;
		list->type = type;
		lists.Insert( list );
	}
	list->objects.Insert( object );
}

/*
================
SetObjectTable::RemoveObject

Takes the object out of the lists of its class type and its base classes.
================
*/
// FUNCTION: STBC 0x0040b460
void SetObjectTable::RemoveObject( BaseObjectClass *object, SetClass *set ) {
	object->RemoveFromSet( set );
}

/*
================
SetObjectTable::Remove

Takes the object out of the list of the type.
================
*/
// FUNCTION: STBC 0x0040b480
void SetObjectTable::Remove( BaseObjectClass *object, int type ) {
	SetObjectTypeList *list = lists.Get( type );
	if ( list != NULL ) {
		list->objects.Remove( object );
	}
}

/*
================
SetObjectTypeList::SetObjectTypeList
================
*/
// FUNCTION: STBC 0x0040bf40
SetObjectTypeList::SetObjectTypeList( void ) {
	type = 0;
}

/*
================
SetObjectTypeList::Compare
================
*/
// FUNCTION: STBC 0x0040bf60
int SetObjectTypeList::Compare( SetObjectTypeList *other ) {
	return CompareKey( other->GetType() );
}

/*
================
SetObjectTypeList::~SetObjectTypeList

The objects belong to the set: the list forgets them before the array
would delete them.
================
*/
// FUNCTION: STBC 0x0040bf90
SetObjectTypeList::~SetObjectTypeList( void ) {
	objects.RemoveAll();
}

/*
================
SetObjectTypeList::CompareKey
================
*/
// FUNCTION: STBC 0x0040bff0
int SetObjectTypeList::CompareKey( int type_ ) {
	if ( type_ == type ) {
		return 0;
	}
	return type_ < type ? -1 : 1;
}
