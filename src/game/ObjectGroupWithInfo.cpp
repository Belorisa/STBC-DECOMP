/*
===============================================================================

	ObjectGroupWithInfo.cpp

===============================================================================
*/

#include <iostream>
#include "ObjectGroupWithInfo.h"
#include "TGEventHandlerTable.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0059f570 _$E458
// FUNCTION: STBC 0x0059f590 _$E457

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0059f0b0 _$E32
// FUNCTION: STBC 0x0059f0c0 _$E29
// FUNCTION: STBC 0x0059f0d0 _$E31
// FUNCTION: STBC 0x0059f0e0 _$E30
// FUNCTION: STBC 0x0059f0f0 _$E38
// FUNCTION: STBC 0x0059f100 _$E35
// FUNCTION: STBC 0x0059f110 _$E37
// FUNCTION: STBC 0x0059f120 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0059f130 _$E76
// FUNCTION: STBC 0x0059f140 _$E75
// FUNCTION: STBC 0x0059f150 _$E80
// FUNCTION: STBC 0x0059f160 _$E79
// FUNCTION: STBC 0x0059f170 _$E91
// FUNCTION: STBC 0x0059f180 _$E90
// FUNCTION: STBC 0x0059f190 _$E95
// FUNCTION: STBC 0x0059f1a0 _$E94
// FUNCTION: STBC 0x0059f1b0 _$E99
// FUNCTION: STBC 0x0059f1c0 _$E98
// FUNCTION: STBC 0x0059f1d0 _$E103
// FUNCTION: STBC 0x0059f1e0 _$E102
// FUNCTION: STBC 0x0059f1f0 _$E227
// FUNCTION: STBC 0x0059f200 _$E226
// FUNCTION: STBC 0x0059f210 _$E169
// FUNCTION: STBC 0x0059f220 _$E168
// FUNCTION: STBC 0x0059f230 _$E182
// FUNCTION: STBC 0x0059f240 _$E181
// FUNCTION: STBC 0x0059f250 _$E232
// FUNCTION: STBC 0x0059f260 _$E231
// FUNCTION: STBC 0x0059f270 _$E237
// FUNCTION: STBC 0x0059f280 _$E236
// FUNCTION: STBC 0x0059f290 _$E241
// FUNCTION: STBC 0x0059f2a0 _$E240
// FUNCTION: STBC 0x0059f2b0 _$E245
// FUNCTION: STBC 0x0059f2c0 _$E244
// FUNCTION: STBC 0x0059f2d0 _$E252
// FUNCTION: STBC 0x0059f2e0 _$E251
// FUNCTION: STBC 0x0059f2f0 _$E277
// FUNCTION: STBC 0x0059f300 _$E276
// FUNCTION: STBC 0x0059f310 _$E281
// FUNCTION: STBC 0x0059f320 _$E280
// FUNCTION: STBC 0x0059f330 _$E127
// FUNCTION: STBC 0x0059f340 _$E126
// FUNCTION: STBC 0x0059f350 _$E263
// FUNCTION: STBC 0x0059f360 _$E262
// FUNCTION: STBC 0x0059f370 _$E270
// FUNCTION: STBC 0x0059f380 _$E269
// FUNCTION: STBC 0x0059f390 _$E293
// FUNCTION: STBC 0x0059f3a0 _$E292
// FUNCTION: STBC 0x0059f3b0 _$E297
// FUNCTION: STBC 0x0059f3c0 _$E296
// FUNCTION: STBC 0x0059f3d0 _$E337
// FUNCTION: STBC 0x0059f3e0 _$E336
// FUNCTION: STBC 0x0059f3f0 _$E343
// FUNCTION: STBC 0x0059f400 _$E342
// FUNCTION: STBC 0x0059f410 _$E285
// FUNCTION: STBC 0x0059f420 _$E284
// FUNCTION: STBC 0x0059f430 _$E289
// FUNCTION: STBC 0x0059f440 _$E288
// FUNCTION: STBC 0x0059f450 _$E301
// FUNCTION: STBC 0x0059f460 _$E300
// FUNCTION: STBC 0x0059f470 _$E305
// FUNCTION: STBC 0x0059f480 _$E304
// FUNCTION: STBC 0x0059f490 _$E309
// FUNCTION: STBC 0x0059f4a0 _$E308
// FUNCTION: STBC 0x0059f4b0 _$E313
// FUNCTION: STBC 0x0059f4c0 _$E312
// FUNCTION: STBC 0x0059f4d0 _$E317
// FUNCTION: STBC 0x0059f4e0 _$E316
// FUNCTION: STBC 0x0059f4f0 _$E321
// FUNCTION: STBC 0x0059f500 _$E320
// FUNCTION: STBC 0x0059f510 _$E325
// FUNCTION: STBC 0x0059f520 _$E324
// FUNCTION: STBC 0x0059f530 _$E329
// FUNCTION: STBC 0x0059f540 _$E328
// FUNCTION: STBC 0x0059f550 _$E132
// FUNCTION: STBC 0x0059f560 _$E131
// FUNCTION: STBC 0x0059f5a0 _$E333
// FUNCTION: STBC 0x0059f5b0 _$E332
// FUNCTION: STBC 0x0059f5c0 _$E137
// FUNCTION: STBC 0x0059f5d0 _$E136
// FUNCTION: STBC 0x0059f5e0 _$E347
// FUNCTION: STBC 0x0059f5f0 _$E346
// FUNCTION: STBC 0x0059f600 _$E351
// FUNCTION: STBC 0x0059f610 _$E350
// FUNCTION: STBC 0x0059f620 _$E355
// FUNCTION: STBC 0x0059f630 _$E354
// FUNCTION: STBC 0x0059f640 _$E359
// FUNCTION: STBC 0x0059f650 _$E358
// FUNCTION: STBC 0x0059f660 _$E146
// FUNCTION: STBC 0x0059f670 _$E145
// FUNCTION: STBC 0x0059f680 _$E363
// FUNCTION: STBC 0x0059f690 _$E362
// FUNCTION: STBC 0x0059f6a0 _$E367
// FUNCTION: STBC 0x0059f6b0 _$E366
// FUNCTION: STBC 0x0059f6c0 _$E371
// FUNCTION: STBC 0x0059f6d0 _$E370
// FUNCTION: STBC 0x0059f6e0 _$E152
// FUNCTION: STBC 0x0059f6f0 _$E151
// FUNCTION: STBC 0x0059f700 _$E157
// FUNCTION: STBC 0x0059f710 _$E156
// FUNCTION: STBC 0x0059f720 _$E187
// FUNCTION: STBC 0x0059f730 _$E186
// FUNCTION: STBC 0x0059f740 _$E375
// FUNCTION: STBC 0x0059f750 _$E374
// FUNCTION: STBC 0x0059f760 _$E192
// FUNCTION: STBC 0x0059f770 _$E191
// FUNCTION: STBC 0x0059f780 _$E379
// FUNCTION: STBC 0x0059f790 _$E378
// FUNCTION: STBC 0x0059f7a0 _$E383
// FUNCTION: STBC 0x0059f7b0 _$E382
// FUNCTION: STBC 0x0059f7c0 _$E197
// FUNCTION: STBC 0x0059f7d0 _$E196
// FUNCTION: STBC 0x0059f7e0 _$E387
// FUNCTION: STBC 0x0059f7f0 _$E386
// FUNCTION: STBC 0x0059f800 _$E162
// FUNCTION: STBC 0x0059f810 _$E161
// FUNCTION: STBC 0x0059f820 _$E391
// FUNCTION: STBC 0x0059f830 _$E390
// FUNCTION: STBC 0x0059f840 _$E395
// FUNCTION: STBC 0x0059f850 _$E394
// FUNCTION: STBC 0x0059f860 _$E202
// FUNCTION: STBC 0x0059f870 _$E201
// FUNCTION: STBC 0x0059f880 _$E207
// FUNCTION: STBC 0x0059f890 _$E206
// FUNCTION: STBC 0x0059f8a0 _$E399
// FUNCTION: STBC 0x0059f8b0 _$E398
// FUNCTION: STBC 0x0059f8c0 _$E403
// FUNCTION: STBC 0x0059f8d0 _$E402
// FUNCTION: STBC 0x0059f8e0 _$E407
// FUNCTION: STBC 0x0059f8f0 _$E406
// FUNCTION: STBC 0x0059f900 _$E411
// FUNCTION: STBC 0x0059f910 _$E410
// FUNCTION: STBC 0x0059f920 _$E415
// FUNCTION: STBC 0x0059f930 _$E414
// FUNCTION: STBC 0x0059f940 _$E419
// FUNCTION: STBC 0x0059f950 _$E418
// FUNCTION: STBC 0x0059f960 _$E423
// FUNCTION: STBC 0x0059f970 _$E422
// FUNCTION: STBC 0x0059f980 _$E427
// FUNCTION: STBC 0x0059f990 _$E426
// FUNCTION: STBC 0x0059f9a0 _$E212
// FUNCTION: STBC 0x0059f9b0 _$E211
// FUNCTION: STBC 0x0059f9c0 _$E431
// FUNCTION: STBC 0x0059f9d0 _$E430
// FUNCTION: STBC 0x0059f9e0 _$E217
// FUNCTION: STBC 0x0059f9f0 _$E216
// FUNCTION: STBC 0x0059fa00 _$E222
// FUNCTION: STBC 0x0059fa10 _$E221
// FUNCTION: STBC 0x0059fa20 _$E435
// FUNCTION: STBC 0x0059fa30 _$E434
// FUNCTION: STBC 0x0059fa40 _$E440
// FUNCTION: STBC 0x0059fa50 _$E439
// FUNCTION: STBC 0x0059fa60 _$E444
// FUNCTION: STBC 0x0059fa70 _$E443

/*
================
ObjectGroupWithInfo::Cast

The object as an ObjectGroupWithInfo; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0059e860
ObjectGroupWithInfo *ObjectGroupWithInfo::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( OBJECTGROUPWITHINFO_TYPE ) ) {
		return (ObjectGroupWithInfo *)object;
	}
	return NULL;
}

/*
================
ObjectGroupWithInfo::ObjectGroupWithInfo
================
*/
// FUNCTION: STBC 0x0059e890 ??0ObjectGroupWithInfo@@QAE@XZ
ObjectGroupWithInfo::ObjectGroupWithInfo( void ) {
}

/*
================
ObjectGroupWithInfo::~ObjectGroupWithInfo

Releases the object of each name and empties the map.
================
*/
// FUNCTION: STBC 0x0059e920
ObjectGroupWithInfo::~ObjectGroupWithInfo( void ) {
	NiTMapIterator pos = info.GetFirstPos();
	while ( pos ) {
		const char *name;
		PyObject *object;
		info.GetNext( pos, name, object );
		Py_DECREF( object );
	}
	info.RemoveAll();
}

/*
================
ObjectGroupWithInfo::AddNameAndInfo
================
*/
// FUNCTION: STBC 0x0059ead0
void ObjectGroupWithInfo::AddNameAndInfo( char *name, PyObject *object ) {
	AddName( name );
	SetInfoForName( name, object );
}

/*
================
ObjectGroupWithInfo_AddNameAndInfo

The Python method ObjectGroupWithInfo.AddNameAndInfo.
================
*/
// FUNCTION: STBC 0x0059eaf0
PyObject *ObjectGroupWithInfo_AddNameAndInfo( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	char *name = NULL;
	PyObject *object = NULL;
	if ( PyArg_ParseTuple( args, "OsO", &instance, &name, &object ) != -1 ) {
		ObjectGroupWithInfo *group = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&group, "ObjectGroupWithInfo" );
		if ( group != NULL ) {
			group->AddNameAndInfo( name, object );
		}
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
ObjectGroupWithInfo::RemoveName

Takes the name out of the group; its object is released with it.
================
*/
// FUNCTION: STBC 0x0059eb70
bool ObjectGroupWithInfo::RemoveName( char *name ) {
	bool removed = ObjectGroup::RemoveName( name );
	if ( removed ) {
		PyObject *object;
		if ( info.GetAt( name, object ) ) {
			Py_DECREF( object );
			info.RemoveAt( name );
		}
	}
	return removed;
}

/*
================
ObjectGroupWithInfo::SetInfoForName

Keeps the object for a name of the group, holding a reference to it and
releasing the object it replaces; a name outside the group is ignored.
================
*/
// FUNCTION: STBC 0x0059ec70
void ObjectGroupWithInfo::SetInfoForName( char *name, PyObject *object ) {
	if ( IsNameInGroup( name ) ) {
		PyObject *old;
		if ( info.GetAt( name, old ) ) {
			Py_DECREF( old );
		}
		Py_INCREF( object );
		info.SetAt( name, object );
	}
}

/*
================
ObjectGroupWithInfo_SetInfoForName

The Python method ObjectGroupWithInfo.SetInfoForName.
================
*/
// FUNCTION: STBC 0x0059ed60
PyObject *ObjectGroupWithInfo_SetInfoForName( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	char *name = NULL;
	PyObject *object = NULL;
	if ( PyArg_ParseTuple( args, "OsO", &instance, &name, &object ) != -1 ) {
		ObjectGroupWithInfo *group = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&group, "ObjectGroupWithInfo" );
		if ( group != NULL ) {
			group->SetInfoForName( name, object );
		}
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
ObjectGroupWithInfo::GetInfo
================
*/
// FUNCTION: STBC 0x0059ede0
PyObject *ObjectGroupWithInfo::GetInfo( char *name ) {
	if ( name == NULL ) {
		return NULL;
	}
	PyObject *object = NULL;
	info.GetAt( name, object );
	return object;
}

/*
================
ObjectGroupWithInfo_GetInfo

The Python method ObjectGroupWithInfo.GetInfo: a KeyError for a name
outside the group.
================
*/
// FUNCTION: STBC 0x0059ee40
PyObject *ObjectGroupWithInfo_GetInfo( PyObject *self, PyObject *args ) {
	PyObject *result = Py_None;
	PyObject *instance = NULL;
	char *name = NULL;
	if ( PyArg_ParseTuple( args, "Os", &instance, &name ) != -1 ) {
		ObjectGroupWithInfo *group = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&group, "ObjectGroupWithInfo" );
		if ( group != NULL ) {
			if ( group->IsNameInGroup( name ) ) {
				result = group->GetInfo( name );
			} else {
				result = NULL;
				PyErr_SetString( PyExc_KeyError, "Name is not in the group." );
			}
		}
	}
	Py_XINCREF( result );
	return result;
}

/*
================
ObjectGroupWithInfo::WriteToStream

The group, then the count of names with an object and each name with its
pickled object.
================
*/
// FUNCTION: STBC 0x0059eef0
bool ObjectGroupWithInfo::WriteToStream( TGStream *stream ) {
	if ( !ObjectGroup::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteInt( info.GetCount() );
	NiTMapIterator pos = info.GetFirstPos();
	while ( pos ) {
		const char *name;
		PyObject *object;
		info.GetNext( pos, name, object );
		stream->VirtualSlot11( (char *)name );
		if ( !PyEmbed::SerializePython( object, stream, 0 ) ) {
			return false;
		}
	}
	return true;
}

/*
================
ObjectGroupWithInfo::ReadFromStream
================
*/
// FUNCTION: STBC 0x0059efb0
bool ObjectGroupWithInfo::ReadFromStream( TGStream *stream ) {
	if ( !ObjectGroup::ReadFromStream( stream ) ) {
		return false;
	}

	int count = stream->ReadInt();
	for ( int i = count - 1; i >= 0; i-- ) {
		char *name = stream->VirtualSlot10( -1 );
		PyObject *object = PyEmbed::UnserializePython( stream, 0 );
		if ( name == NULL || object == NULL ) {
			return false;
		}
		info.SetAt( name, object );
		delete [] name;
	}
	return true;
}
// The handler table's dynamic initialiser, the registration of its exit-time
// destructor and the guarded destructor.
// FUNCTION: STBC 0x0059e810 _$E450
// FUNCTION: STBC 0x0059e820 _$E446
// FUNCTION: STBC 0x0059e830 _$E449
// FUNCTION: STBC 0x0059e840 _$E447
// DATA: STBC 0x0098e0e0
TGEventHandlerTable ObjectGroupWithInfo::classHandlers;

