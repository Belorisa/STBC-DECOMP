/*
===============================================================================

	TGModelPropertySet.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGModelPropertySet.h"
#include "TGModelProperty.h"
#include "TGModelPropertyInstance.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006ebd60 _$E416
// FUNCTION: STBC 0x006ebd80 _$E415

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006ebb40 _$E32
// FUNCTION: STBC 0x006ebb50 _$E29
// FUNCTION: STBC 0x006ebb60 _$E31
// FUNCTION: STBC 0x006ebb70 _$E30
// FUNCTION: STBC 0x006ebb80 _$E38
// FUNCTION: STBC 0x006ebb90 _$E35
// FUNCTION: STBC 0x006ebba0 _$E37
// FUNCTION: STBC 0x006ebbb0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006ebbc0 _$E53
// FUNCTION: STBC 0x006ebbd0 _$E52
// FUNCTION: STBC 0x006ebbe0 _$E57
// FUNCTION: STBC 0x006ebbf0 _$E56
// FUNCTION: STBC 0x006ebc00 _$E68
// FUNCTION: STBC 0x006ebc10 _$E67
// FUNCTION: STBC 0x006ebc20 _$E72
// FUNCTION: STBC 0x006ebc30 _$E71
// FUNCTION: STBC 0x006ebc40 _$E76
// FUNCTION: STBC 0x006ebc50 _$E75
// FUNCTION: STBC 0x006ebc60 _$E80
// FUNCTION: STBC 0x006ebc70 _$E79
// FUNCTION: STBC 0x006ebc80 _$E204
// FUNCTION: STBC 0x006ebc90 _$E203
// FUNCTION: STBC 0x006ebd90 _$E146
// FUNCTION: STBC 0x006ebda0 _$E145
// FUNCTION: STBC 0x006ebdb0 _$E159
// FUNCTION: STBC 0x006ebdc0 _$E158
// FUNCTION: STBC 0x006ebdd0 _$E209
// FUNCTION: STBC 0x006ebde0 _$E208
// FUNCTION: STBC 0x006ebdf0 _$E214
// FUNCTION: STBC 0x006ebe00 _$E213
// FUNCTION: STBC 0x006ebe10 _$E218
// FUNCTION: STBC 0x006ebe20 _$E217
// FUNCTION: STBC 0x006ebe30 _$E222
// FUNCTION: STBC 0x006ebe40 _$E221
// FUNCTION: STBC 0x006ebe50 _$E229
// FUNCTION: STBC 0x006ebe60 _$E228
// FUNCTION: STBC 0x006ebe70 _$E254
// FUNCTION: STBC 0x006ebe80 _$E253
// FUNCTION: STBC 0x006ebe90 _$E258
// FUNCTION: STBC 0x006ebea0 _$E257
// FUNCTION: STBC 0x006ebeb0 _$E104
// FUNCTION: STBC 0x006ebec0 _$E103
// FUNCTION: STBC 0x006ebed0 _$E240
// FUNCTION: STBC 0x006ebee0 _$E239
// FUNCTION: STBC 0x006ebef0 _$E247
// FUNCTION: STBC 0x006ebf00 _$E246
// FUNCTION: STBC 0x006ebf10 _$E270
// FUNCTION: STBC 0x006ebf20 _$E269
// FUNCTION: STBC 0x006ebf30 _$E274
// FUNCTION: STBC 0x006ebf40 _$E273
// FUNCTION: STBC 0x006ebf50 _$E314
// FUNCTION: STBC 0x006ebf60 _$E313
// FUNCTION: STBC 0x006ebf70 _$E320
// FUNCTION: STBC 0x006ebf80 _$E319
// FUNCTION: STBC 0x006ebf90 _$E262
// FUNCTION: STBC 0x006ebfa0 _$E261
// FUNCTION: STBC 0x006ebfb0 _$E266
// FUNCTION: STBC 0x006ebfc0 _$E265
// FUNCTION: STBC 0x006ebfd0 _$E278
// FUNCTION: STBC 0x006ebfe0 _$E277
// FUNCTION: STBC 0x006ebff0 _$E282
// FUNCTION: STBC 0x006ec000 _$E281
// FUNCTION: STBC 0x006ec010 _$E286
// FUNCTION: STBC 0x006ec020 _$E285
// FUNCTION: STBC 0x006ec030 _$E290
// FUNCTION: STBC 0x006ec040 _$E289
// FUNCTION: STBC 0x006ec050 _$E294
// FUNCTION: STBC 0x006ec060 _$E293
// FUNCTION: STBC 0x006ec070 _$E298
// FUNCTION: STBC 0x006ec080 _$E297
// FUNCTION: STBC 0x006ec090 _$E302
// FUNCTION: STBC 0x006ec0a0 _$E301
// FUNCTION: STBC 0x006ec0b0 _$E306
// FUNCTION: STBC 0x006ec0c0 _$E305
// FUNCTION: STBC 0x006ec0d0 _$E109
// FUNCTION: STBC 0x006ec0e0 _$E108
// FUNCTION: STBC 0x006ec0f0 _$E310
// FUNCTION: STBC 0x006ec100 _$E309
// FUNCTION: STBC 0x006ec110 _$E114
// FUNCTION: STBC 0x006ec120 _$E113
// FUNCTION: STBC 0x006ec130 _$E324
// FUNCTION: STBC 0x006ec140 _$E323
// FUNCTION: STBC 0x006ec150 _$E328
// FUNCTION: STBC 0x006ec160 _$E327
// FUNCTION: STBC 0x006ec170 _$E332
// FUNCTION: STBC 0x006ec180 _$E331
// FUNCTION: STBC 0x006ec190 _$E336
// FUNCTION: STBC 0x006ec1a0 _$E335
// FUNCTION: STBC 0x006ec1b0 _$E123
// FUNCTION: STBC 0x006ec1c0 _$E122
// FUNCTION: STBC 0x006ec1d0 _$E340
// FUNCTION: STBC 0x006ec1e0 _$E339
// FUNCTION: STBC 0x006ec1f0 _$E344
// FUNCTION: STBC 0x006ec200 _$E343
// FUNCTION: STBC 0x006ec210 _$E348
// FUNCTION: STBC 0x006ec220 _$E347
// FUNCTION: STBC 0x006ec230 _$E129
// FUNCTION: STBC 0x006ec240 _$E128
// FUNCTION: STBC 0x006ec250 _$E134
// FUNCTION: STBC 0x006ec260 _$E133
// FUNCTION: STBC 0x006ec270 _$E164
// FUNCTION: STBC 0x006ec280 _$E163
// FUNCTION: STBC 0x006ec290 _$E352
// FUNCTION: STBC 0x006ec2a0 _$E351
// FUNCTION: STBC 0x006ec2b0 _$E169
// FUNCTION: STBC 0x006ec2c0 _$E168
// FUNCTION: STBC 0x006ec2d0 _$E356
// FUNCTION: STBC 0x006ec2e0 _$E355
// FUNCTION: STBC 0x006ec2f0 _$E360
// FUNCTION: STBC 0x006ec300 _$E359
// FUNCTION: STBC 0x006ec310 _$E174
// FUNCTION: STBC 0x006ec320 _$E173
// FUNCTION: STBC 0x006ec330 _$E364
// FUNCTION: STBC 0x006ec340 _$E363
// FUNCTION: STBC 0x006ec350 _$E139
// FUNCTION: STBC 0x006ec360 _$E138
// FUNCTION: STBC 0x006ec370 _$E368
// FUNCTION: STBC 0x006ec380 _$E367
// FUNCTION: STBC 0x006ec390 _$E372
// FUNCTION: STBC 0x006ec3a0 _$E371
// FUNCTION: STBC 0x006ec3b0 _$E179
// FUNCTION: STBC 0x006ec3c0 _$E178
// FUNCTION: STBC 0x006ec3d0 _$E184
// FUNCTION: STBC 0x006ec3e0 _$E183
// FUNCTION: STBC 0x006ec3f0 _$E376
// FUNCTION: STBC 0x006ec400 _$E375
// FUNCTION: STBC 0x006ec410 _$E380
// FUNCTION: STBC 0x006ec420 _$E379
// FUNCTION: STBC 0x006ec430 _$E384
// FUNCTION: STBC 0x006ec440 _$E383
// FUNCTION: STBC 0x006ec450 _$E388
// FUNCTION: STBC 0x006ec460 _$E387
// FUNCTION: STBC 0x006ec470 _$E392
// FUNCTION: STBC 0x006ec480 _$E391
// FUNCTION: STBC 0x006ec490 _$E396
// FUNCTION: STBC 0x006ec4a0 _$E395
// FUNCTION: STBC 0x006ec4b0 _$E400
// FUNCTION: STBC 0x006ec4c0 _$E399
// FUNCTION: STBC 0x006ec4d0 _$E404
// FUNCTION: STBC 0x006ec4e0 _$E403
// FUNCTION: STBC 0x006ec4f0 _$E189
// FUNCTION: STBC 0x006ec500 _$E188
// FUNCTION: STBC 0x006ec510 _$E408
// FUNCTION: STBC 0x006ec520 _$E407
// FUNCTION: STBC 0x006ec530 _$E194
// FUNCTION: STBC 0x006ec540 _$E193
// FUNCTION: STBC 0x006ec550 _$E199
// FUNCTION: STBC 0x006ec560 _$E198

/*
================
TGModelPropertySet::~TGModelPropertySet

Deletes the lists with their instances.
================
*/
// FUNCTION: STBC 0x006eb0d0
TGModelPropertySet::~TGModelPropertySet( void ) {
	NiTListIterator pos = lists->GetHeadPos();
	while ( pos ) {
		TGNamedModelPropertyList *list = lists->GetNext( pos );
		if ( list != NULL ) {
			NiTListIterator instancePos = list->GetHeadPos();
			while ( instancePos ) {
				delete list->GetNext( instancePos );
			}
			list->RemoveAll();
			delete list;
		}
	}
	lists->RemoveAll();
	delete lists;
}

/*
================
TGModelPropertySet::AddToSet

Places a copy of the property in the list of the name, which is made when
there is none; an instance already made from a property of the same name
takes the new one instead.
================
*/
// FUNCTION: STBC 0x006eb220 ?AddToSet@TGModelPropertySet@@QAE_NPADPAVTGModelProperty@@@Z
bool TGModelPropertySet::AddToSet( char *name, TGModelProperty *property ) {
	TGNamedModelPropertyList *list = (TGNamedModelPropertyList *)GetPropertyList( name );
	if ( list == NULL ) {
		list = new TGNamedModelPropertyList;
		list->SetName( name );
		lists->AddTail( list );
	}

	TGString propertyName = property->GetName();
	bool found = false;
	NiTListIterator pos = list->GetHeadPos();
	while ( pos != NULL && !found ) {
		TGModelPropertyInstance *instance = list->GetNext( pos );
		if ( instance->GetOriginalTemplate()->GetName().Compare( &propertyName, true ) == 0 ) {
			instance->SetProperty( property->Duplicate() );
			instance->SetOriginalTemplate( property );
			found = true;
		}
	}

	if ( !found ) {
		list->AddTail( new TGModelPropertyInstance( property, name ) );
	}
	return true;
}

/*
================
TGModelPropertySet::GetLists
================
*/
// FUNCTION: STBC 0x006eb4e0
NiTList<TGNamedModelPropertyList *> *TGModelPropertySet::GetLists( void ) {
	return lists;
}

/*
================
TGModelPropertySet::GetPropertyList

The list of the name; NULL when there is none.
================
*/
// FUNCTION: STBC 0x006eb4f0
TGModelPropertyList *TGModelPropertySet::GetPropertyList( char *name ) {
	NiTListIterator pos = lists->GetHeadPos();
	while ( pos ) {
		TGNamedModelPropertyList *list = lists->GetNext( pos );
		if ( strcmp( list->GetName(), name ) == 0 ) {
			return list;
		}
	}
	return NULL;
}

/*
================
TGModelPropertySet::Create
================
*/
// FUNCTION: STBC 0x006eb560 ?Create@TGModelPropertySet@@SAPAV1@J@Z
TGModelPropertySet *TGModelPropertySet::Create( long id ) {
	return new TGModelPropertySet( id );
}

/*
================
TGModelPropertySet::GetPropertiesByType

A new list of the instances, from every list, whose property is of the
type; the caller frees it.
================
*/
// FUNCTION: STBC 0x006eb590
TGModelPropertyList *TGModelPropertySet::GetPropertiesByType( int type ) {
	NiTListIterator pos = lists->GetHeadPos();
	TGModelPropertyList *result = new TGModelPropertyList;
	while ( pos ) {
		TGNamedModelPropertyList *list = lists->GetNext( pos );
		if ( list != NULL ) {
			NiTListIterator instancePos = list->GetHeadPos();
			while ( instancePos ) {
				TGModelPropertyInstance *instance = list->GetNext( instancePos );
				if ( instance->GetProperty()->IsTypeOf( type ) ) {
					result->AddTail( instance );
				}
			}
		}
	}
	return result;
}

/*
================
TGModelPropertySet::WriteToStream
================
*/
// FUNCTION: STBC 0x006eb730 ?WriteToStream@TGModelPropertySet@@UAE_NPAVTGStream@@@Z
bool TGModelPropertySet::WriteToStream( TGStream *stream ) {
	if ( !TGObject::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteLong( lists->GetSize() );
	NiTListIterator pos = lists->GetHeadPos();
	while ( pos ) {
		if ( !lists->GetNext( pos )->WriteToStream( stream ) ) {
			return false;
		}
	}
	return true;
}

/*
================
TGModelPropertySet::ReadFromStream
================
*/
// FUNCTION: STBC 0x006eb780 ?ReadFromStream@TGModelPropertySet@@UAE_NPAVTGStream@@@Z
bool TGModelPropertySet::ReadFromStream( TGStream *stream ) {
	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		TGNamedModelPropertyList *list = new TGNamedModelPropertyList;
		if ( !list->ReadFromStream( stream ) ) {
			return false;
		}
		lists->AddTail( list );
	}
	return true;
}

/*
================
TGModelPropertySet::FixUpPointers

Fails when an instance cannot find the objects it was saved with.
================
*/
// FUNCTION: STBC 0x006eb8f0
bool TGModelPropertySet::FixUpPointers( void ) {
	NiTListIterator pos = lists->GetHeadPos();
	while ( pos ) {
		TGNamedModelPropertyList *list = lists->GetNext( pos );
		if ( list != NULL ) {
			NiTListIterator instancePos = list->GetHeadPos();
			while ( instancePos ) {
				if ( !list->GetNext( instancePos )->FixUpPointers() ) {
					return false;
				}
			}
		}
	}
	return true;
}

/*
================
TGModelPropertySet::PostFixUp
================
*/
// FUNCTION: STBC 0x006eb940
void TGModelPropertySet::PostFixUp( void ) {
	NiTListIterator pos = lists->GetHeadPos();
	while ( pos ) {
		TGNamedModelPropertyList *list = lists->GetNext( pos );
		if ( list != NULL ) {
			NiTListIterator instancePos = list->GetHeadPos();
			while ( instancePos ) {
				list->GetNext( instancePos )->PostFixUp();
			}
		}
	}
}

/*
================
TGNamedModelPropertyList::WriteToStream
================
*/
// FUNCTION: STBC 0x006eb980
bool TGNamedModelPropertyList::WriteToStream( TGStream *stream ) {
	stream->VirtualSlot11( name );
	NiTListIterator pos = GetHeadPos();
	while ( pos ) {
		TGModelPropertyInstance *instance = GetNext( pos );
		if ( instance != NULL ) {
			stream->WriteBool( true );
			if ( !instance->WriteToStream( stream ) ) {
				return false;
			}
		}
	}
	stream->WriteBool( false );
	return true;
}

/*
================
TGNamedModelPropertyList::ReadFromStream
================
*/
// FUNCTION: STBC 0x006eb9e0
bool TGNamedModelPropertyList::ReadFromStream( TGStream *stream ) {
	name = stream->VirtualSlot10( -1 );
	while ( stream->ReadBool() == true ) {
		TGModelPropertyInstance *instance = new TGModelPropertyInstance;
		if ( !instance->ReadFromStream( stream ) ) {
			return false;
		}
		AddTail( instance );
	}
	return true;
}
