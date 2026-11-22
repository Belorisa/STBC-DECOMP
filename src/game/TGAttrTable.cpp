/*
===============================================================================

	TGAttrTable.cpp

===============================================================================
*/

#include <iostream>
#include "TGAttrTable.h"
#include "TGStream.h"
#include "NiMain.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006f5c80 _$E32
// FUNCTION: STBC 0x006f5c90 _$E29
// FUNCTION: STBC 0x006f5ca0 _$E31
// FUNCTION: STBC 0x006f5cb0 _$E30
// FUNCTION: STBC 0x006f5cc0 _$E38
// FUNCTION: STBC 0x006f5cd0 _$E35
// FUNCTION: STBC 0x006f5ce0 _$E37
// FUNCTION: STBC 0x006f5cf0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006f5d00 _$E45
// FUNCTION: STBC 0x006f5d10 _$E44
// FUNCTION: STBC 0x006f5d20 _$E49
// FUNCTION: STBC 0x006f5d30 _$E48
// FUNCTION: STBC 0x006f5d40 _$E60
// FUNCTION: STBC 0x006f5d50 _$E59
// FUNCTION: STBC 0x006f5d60 _$E64
// FUNCTION: STBC 0x006f5d70 _$E63
// FUNCTION: STBC 0x006f5d80 _$E68
// FUNCTION: STBC 0x006f5d90 _$E67
// FUNCTION: STBC 0x006f5da0 _$E72
// FUNCTION: STBC 0x006f5db0 _$E71
// FUNCTION: STBC 0x006f5dc0 _$E196
// FUNCTION: STBC 0x006f5dd0 _$E195
// FUNCTION: STBC 0x006f5de0 _$E138
// FUNCTION: STBC 0x006f5df0 _$E137
// FUNCTION: STBC 0x006f5e00 _$E151
// FUNCTION: STBC 0x006f5e10 _$E150
// FUNCTION: STBC 0x006f5e20 _$E201
// FUNCTION: STBC 0x006f5e30 _$E200
// FUNCTION: STBC 0x006f5e40 _$E206
// FUNCTION: STBC 0x006f5e50 _$E205
// FUNCTION: STBC 0x006f5e60 _$E210
// FUNCTION: STBC 0x006f5e70 _$E209
// FUNCTION: STBC 0x006f5e80 _$E214
// FUNCTION: STBC 0x006f5e90 _$E213
// FUNCTION: STBC 0x006f5ea0 _$E221
// FUNCTION: STBC 0x006f5eb0 _$E220
// FUNCTION: STBC 0x006f5ec0 _$E246
// FUNCTION: STBC 0x006f5ed0 _$E245
// FUNCTION: STBC 0x006f5ee0 _$E250
// FUNCTION: STBC 0x006f5ef0 _$E249
// FUNCTION: STBC 0x006f5f00 _$E96
// FUNCTION: STBC 0x006f5f10 _$E95
// FUNCTION: STBC 0x006f5f20 _$E232
// FUNCTION: STBC 0x006f5f30 _$E231
// FUNCTION: STBC 0x006f5f40 _$E239
// FUNCTION: STBC 0x006f5f50 _$E238
// FUNCTION: STBC 0x006f5f60 _$E262
// FUNCTION: STBC 0x006f5f70 _$E261
// FUNCTION: STBC 0x006f5f80 _$E266
// FUNCTION: STBC 0x006f5f90 _$E265
// FUNCTION: STBC 0x006f5fa0 _$E306
// FUNCTION: STBC 0x006f5fb0 _$E305
// FUNCTION: STBC 0x006f5fc0 _$E312
// FUNCTION: STBC 0x006f5fd0 _$E311
// FUNCTION: STBC 0x006f5fe0 _$E254
// FUNCTION: STBC 0x006f5ff0 _$E253
// FUNCTION: STBC 0x006f6000 _$E258
// FUNCTION: STBC 0x006f6010 _$E257
// FUNCTION: STBC 0x006f6020 _$E270
// FUNCTION: STBC 0x006f6030 _$E269
// FUNCTION: STBC 0x006f6040 _$E274
// FUNCTION: STBC 0x006f6050 _$E273
// FUNCTION: STBC 0x006f6060 _$E278
// FUNCTION: STBC 0x006f6070 _$E277
// FUNCTION: STBC 0x006f6080 _$E282
// FUNCTION: STBC 0x006f6090 _$E281
// FUNCTION: STBC 0x006f60a0 _$E286
// FUNCTION: STBC 0x006f60b0 _$E285
// FUNCTION: STBC 0x006f60c0 _$E290
// FUNCTION: STBC 0x006f60d0 _$E289
// FUNCTION: STBC 0x006f60e0 _$E294
// FUNCTION: STBC 0x006f60f0 _$E293
// FUNCTION: STBC 0x006f6100 _$E298
// FUNCTION: STBC 0x006f6110 _$E297
// FUNCTION: STBC 0x006f6120 _$E101
// FUNCTION: STBC 0x006f6130 _$E100
// FUNCTION: STBC 0x006f6140 _$E302
// FUNCTION: STBC 0x006f6150 _$E301
// FUNCTION: STBC 0x006f6160 _$E106
// FUNCTION: STBC 0x006f6170 _$E105
// FUNCTION: STBC 0x006f6180 _$E316
// FUNCTION: STBC 0x006f6190 _$E315
// FUNCTION: STBC 0x006f61a0 _$E320
// FUNCTION: STBC 0x006f61b0 _$E319
// FUNCTION: STBC 0x006f61c0 _$E324
// FUNCTION: STBC 0x006f61d0 _$E323
// FUNCTION: STBC 0x006f61e0 _$E328
// FUNCTION: STBC 0x006f61f0 _$E327
// FUNCTION: STBC 0x006f6200 _$E115
// FUNCTION: STBC 0x006f6210 _$E114
// FUNCTION: STBC 0x006f6220 _$E332
// FUNCTION: STBC 0x006f6230 _$E331
// FUNCTION: STBC 0x006f6240 _$E336
// FUNCTION: STBC 0x006f6250 _$E335
// FUNCTION: STBC 0x006f6260 _$E340
// FUNCTION: STBC 0x006f6270 _$E339
// FUNCTION: STBC 0x006f6280 _$E121
// FUNCTION: STBC 0x006f6290 _$E120
// FUNCTION: STBC 0x006f62a0 _$E126
// FUNCTION: STBC 0x006f62b0 _$E125
// FUNCTION: STBC 0x006f62c0 _$E156
// FUNCTION: STBC 0x006f62d0 _$E155
// FUNCTION: STBC 0x006f62e0 _$E344
// FUNCTION: STBC 0x006f62f0 _$E343
// FUNCTION: STBC 0x006f6300 _$E161
// FUNCTION: STBC 0x006f6310 _$E160
// FUNCTION: STBC 0x006f6320 _$E348
// FUNCTION: STBC 0x006f6330 _$E347
// FUNCTION: STBC 0x006f6340 _$E352
// FUNCTION: STBC 0x006f6350 _$E351
// FUNCTION: STBC 0x006f6360 _$E166
// FUNCTION: STBC 0x006f6370 _$E165
// FUNCTION: STBC 0x006f6380 _$E356
// FUNCTION: STBC 0x006f6390 _$E355
// FUNCTION: STBC 0x006f63a0 _$E131
// FUNCTION: STBC 0x006f63b0 _$E130
// FUNCTION: STBC 0x006f63c0 _$E360
// FUNCTION: STBC 0x006f63d0 _$E359
// FUNCTION: STBC 0x006f63e0 _$E364
// FUNCTION: STBC 0x006f63f0 _$E363
// FUNCTION: STBC 0x006f6400 _$E171
// FUNCTION: STBC 0x006f6410 _$E170
// FUNCTION: STBC 0x006f6420 _$E176
// FUNCTION: STBC 0x006f6430 _$E175
// FUNCTION: STBC 0x006f6440 _$E368
// FUNCTION: STBC 0x006f6450 _$E367
// FUNCTION: STBC 0x006f6460 _$E372
// FUNCTION: STBC 0x006f6470 _$E371
// FUNCTION: STBC 0x006f6480 _$E376
// FUNCTION: STBC 0x006f6490 _$E375
// FUNCTION: STBC 0x006f64a0 _$E380
// FUNCTION: STBC 0x006f64b0 _$E379
// FUNCTION: STBC 0x006f64c0 _$E384
// FUNCTION: STBC 0x006f64d0 _$E383
// FUNCTION: STBC 0x006f64e0 _$E388
// FUNCTION: STBC 0x006f64f0 _$E387
// FUNCTION: STBC 0x006f6500 _$E392
// FUNCTION: STBC 0x006f6510 _$E391
// FUNCTION: STBC 0x006f6520 _$E396
// FUNCTION: STBC 0x006f6530 _$E395
// FUNCTION: STBC 0x006f6540 _$E181
// FUNCTION: STBC 0x006f6550 _$E180
// FUNCTION: STBC 0x006f6560 _$E400
// FUNCTION: STBC 0x006f6570 _$E399
// FUNCTION: STBC 0x006f6580 _$E186
// FUNCTION: STBC 0x006f6590 _$E185
// FUNCTION: STBC 0x006f65a0 _$E191
// FUNCTION: STBC 0x006f65b0 _$E190

/*
================
TGAttrTable::TGAttrTable
================
*/
// FUNCTION: STBC 0x006f65c0
TGAttrTable::TGAttrTable( void ) {
}

/*
================
TGAttrTable::~TGAttrTable
================
*/
// FUNCTION: STBC 0x006f6630
TGAttrTable::~TGAttrTable( void ) {
}

/*
================
TGAttrTable::CopyFrom

Replaces every value with those of other.
================
*/
// FUNCTION: STBC 0x006f66a0
bool TGAttrTable::CopyFrom( TGAttrTable *other ) {
	RemoveAll();

	NiTMapIterator pos = other->values.GetFirstPos();
	while ( pos ) {
		AttrType type;
		float value;
		other->values.GetNext( pos, type, value );
		Set( type, value );
	}
	return true;
}

/*
================
TGAttrTable::Merge

Adds the values of other; values already here are replaced only when
overwrite is set.
================
*/
// FUNCTION: STBC 0x006f6730
bool TGAttrTable::Merge( TGAttrTable *other, bool overwrite ) {
	NiTMapIterator pos = other->values.GetFirstPos();
	while ( pos ) {
		AttrType type;
		float value;
		other->values.GetNext( pos, type, value );
		if ( !overwrite ) {
			float current;
			if ( !Lookup( type, &current ) ) {
				Set( type, value );
			}
		} else {
			Set( type, value );
		}
	}
	return true;
}

/*
================
TGAttrTable::Lookup

Copies the value into value; false when there is none.
================
*/
// FUNCTION: STBC 0x006f67e0
bool TGAttrTable::Lookup( AttrType type, float *value ) {
	return values.GetAt( type, *value );
}

/*
================
TGAttrTable::Set
================
*/
// FUNCTION: STBC 0x006f6830
bool TGAttrTable::Set( AttrType type, float value ) {
	values.SetAt( type, value );
	return true;
}

/*
================
TGAttrTable::Has
================
*/
// FUNCTION: STBC 0x006f68c0
bool TGAttrTable::Has( AttrType type ) {
	float value;
	return Lookup( type, &value );
}

/*
================
TGAttrTable::Remove

False when there was no value.
================
*/
// FUNCTION: STBC 0x006f68e0
bool TGAttrTable::Remove( AttrType type ) {
	return values.RemoveAt( type );
}

/*
================
TGAttrTable::RemoveAll
================
*/
// FUNCTION: STBC 0x006f6980
void TGAttrTable::RemoveAll( void ) {
	values.RemoveAll();
}

/*
================
TGAttrTable::GetCount
================
*/
// FUNCTION: STBC 0x006f69e0
int TGAttrTable::GetCount( void ) {
	return values.GetCount();
}

/*
================
TGAttrTable::WriteToStream

The count, then each key and value.
================
*/
// FUNCTION: STBC 0x006f69f0
bool TGAttrTable::WriteToStream( TGStream *stream ) {
	int count = GetCount();
	stream->WriteInt( count );

	NiTMapIterator pos = values.GetFirstPos();
	while ( pos ) {
		AttrType type;
		float value;
		values.GetNext( pos, type, value );
		stream->WriteInt( type );
		stream->WriteFloat( value );
	}
	return true;
}

/*
================
TGAttrTable::ReadFromStream

Replaces the values with those WriteToStream wrote; false when one could
not be stored.
================
*/
// FUNCTION: STBC 0x006f6a90
bool TGAttrTable::ReadFromStream( TGStream *stream ) {
	bool ok = true;

	if ( GetCount() != 0 ) {
		RemoveAll();
	}

	int count = stream->ReadInt();
	for ( int i = 0; i < count; i++ ) {
		AttrType type = stream->ReadInt();
		float value = stream->ReadFloat();
		if ( !Set( type, value ) ) {
			ok = false;
		}
	}
	return ok;
}
