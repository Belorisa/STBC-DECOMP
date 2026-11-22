/*
===============================================================================

	AIScriptAssist.cpp

===============================================================================
*/

#include <iostream>
#include "AIScriptAssist.h"
#include "ObjectClass.h"
#include "PyEmbed.h"
#include "SetClass.h"
#include "Torpedo.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x004741a0 _$E467
// FUNCTION: STBC 0x004741c0 _$E466

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00473b20 _$E32
// FUNCTION: STBC 0x00473b30 _$E29
// FUNCTION: STBC 0x00473b40 _$E31
// FUNCTION: STBC 0x00473b50 _$E30
// FUNCTION: STBC 0x00473b60 _$E38
// FUNCTION: STBC 0x00473b70 _$E35
// FUNCTION: STBC 0x00473b80 _$E37
// FUNCTION: STBC 0x00473b90 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00473ba0 _$E66
// FUNCTION: STBC 0x00473bb0 _$E65
// FUNCTION: STBC 0x00473bc0 _$E254
// FUNCTION: STBC 0x00473bd0 _$E253
// FUNCTION: STBC 0x00473be0 _$E77
// FUNCTION: STBC 0x00473bf0 _$E76
// FUNCTION: STBC 0x00473c00 _$E81
// FUNCTION: STBC 0x00473c10 _$E80
// FUNCTION: STBC 0x00473c20 _$E85
// FUNCTION: STBC 0x00473c30 _$E84
// FUNCTION: STBC 0x00473c40 _$E89
// FUNCTION: STBC 0x00473c50 _$E88
// FUNCTION: STBC 0x00473c60 _$E206
// FUNCTION: STBC 0x00473c70 _$E205
// FUNCTION: STBC 0x00473c80 _$E148
// FUNCTION: STBC 0x00473c90 _$E147
// FUNCTION: STBC 0x00473ca0 _$E161
// FUNCTION: STBC 0x00473cb0 _$E160
// FUNCTION: STBC 0x00473cc0 _$E211
// FUNCTION: STBC 0x00473cd0 _$E210
// FUNCTION: STBC 0x00473ce0 _$E259
// FUNCTION: STBC 0x00473cf0 _$E258
// FUNCTION: STBC 0x00473d00 _$E215
// FUNCTION: STBC 0x00473d10 _$E214
// FUNCTION: STBC 0x00473d20 _$E263
// FUNCTION: STBC 0x00473d30 _$E262
// FUNCTION: STBC 0x00473d40 _$E270
// FUNCTION: STBC 0x00473d50 _$E269
// FUNCTION: STBC 0x00473d60 _$E292
// FUNCTION: STBC 0x00473d70 _$E291
// FUNCTION: STBC 0x00473d80 _$E296
// FUNCTION: STBC 0x00473d90 _$E295
// FUNCTION: STBC 0x00473da0 _$E106
// FUNCTION: STBC 0x00473db0 _$E105
// FUNCTION: STBC 0x00473dc0 _$E278
// FUNCTION: STBC 0x00473dd0 _$E277
// FUNCTION: STBC 0x00473de0 _$E285
// FUNCTION: STBC 0x00473df0 _$E284
// FUNCTION: STBC 0x00473e00 _$E308
// FUNCTION: STBC 0x00473e10 _$E307
// FUNCTION: STBC 0x00473e20 _$E312
// FUNCTION: STBC 0x00473e30 _$E311
// FUNCTION: STBC 0x00473e40 _$E352
// FUNCTION: STBC 0x00473e50 _$E351
// FUNCTION: STBC 0x00473e60 _$E358
// FUNCTION: STBC 0x00473e70 _$E357
// FUNCTION: STBC 0x00473e80 _$E300
// FUNCTION: STBC 0x00473e90 _$E299
// FUNCTION: STBC 0x00473ea0 _$E304
// FUNCTION: STBC 0x00473eb0 _$E303
// FUNCTION: STBC 0x00473ec0 _$E316
// FUNCTION: STBC 0x00473ed0 _$E315
// FUNCTION: STBC 0x00473ee0 _$E320
// FUNCTION: STBC 0x00473ef0 _$E319
// FUNCTION: STBC 0x00473f00 _$E324
// FUNCTION: STBC 0x00473f10 _$E323
// FUNCTION: STBC 0x00473f20 _$E328
// FUNCTION: STBC 0x00473f30 _$E327
// FUNCTION: STBC 0x00473f40 _$E332
// FUNCTION: STBC 0x00473f50 _$E331
// FUNCTION: STBC 0x00473f60 _$E336
// FUNCTION: STBC 0x00473f70 _$E335
// FUNCTION: STBC 0x00473f80 _$E340
// FUNCTION: STBC 0x00473f90 _$E339
// FUNCTION: STBC 0x00473fa0 _$E344
// FUNCTION: STBC 0x00473fb0 _$E343
// FUNCTION: STBC 0x00473fc0 _$E111
// FUNCTION: STBC 0x00473fd0 _$E110
// FUNCTION: STBC 0x00473fe0 _$E348
// FUNCTION: STBC 0x00473ff0 _$E347
// FUNCTION: STBC 0x00474000 _$E116
// FUNCTION: STBC 0x00474010 _$E115
// FUNCTION: STBC 0x00474020 _$E362
// FUNCTION: STBC 0x00474030 _$E361
// FUNCTION: STBC 0x00474040 _$E366
// FUNCTION: STBC 0x00474050 _$E365
// FUNCTION: STBC 0x00474060 _$E370
// FUNCTION: STBC 0x00474070 _$E369
// FUNCTION: STBC 0x00474080 _$E374
// FUNCTION: STBC 0x00474090 _$E373
// FUNCTION: STBC 0x004740a0 _$E125
// FUNCTION: STBC 0x004740b0 _$E124
// FUNCTION: STBC 0x004740c0 _$E378
// FUNCTION: STBC 0x004740d0 _$E377
// FUNCTION: STBC 0x004740e0 _$E382
// FUNCTION: STBC 0x004740f0 _$E381
// FUNCTION: STBC 0x00474100 _$E386
// FUNCTION: STBC 0x00474110 _$E385
// FUNCTION: STBC 0x00474120 _$E131
// FUNCTION: STBC 0x00474130 _$E130
// FUNCTION: STBC 0x00474140 _$E136
// FUNCTION: STBC 0x00474150 _$E135
// FUNCTION: STBC 0x00474160 _$E166
// FUNCTION: STBC 0x00474170 _$E165
// FUNCTION: STBC 0x00474180 _$E390
// FUNCTION: STBC 0x00474190 _$E389
// FUNCTION: STBC 0x004741d0 _$E171
// FUNCTION: STBC 0x004741e0 _$E170
// FUNCTION: STBC 0x004741f0 _$E394
// FUNCTION: STBC 0x00474200 _$E393
// FUNCTION: STBC 0x00474210 _$E398
// FUNCTION: STBC 0x00474220 _$E397
// FUNCTION: STBC 0x00474230 _$E176
// FUNCTION: STBC 0x00474240 _$E175
// FUNCTION: STBC 0x00474250 _$E402
// FUNCTION: STBC 0x00474260 _$E401
// FUNCTION: STBC 0x00474270 _$E141
// FUNCTION: STBC 0x00474280 _$E140
// FUNCTION: STBC 0x00474290 _$E406
// FUNCTION: STBC 0x004742a0 _$E405
// FUNCTION: STBC 0x004742b0 _$E410
// FUNCTION: STBC 0x004742c0 _$E409
// FUNCTION: STBC 0x004742d0 _$E181
// FUNCTION: STBC 0x004742e0 _$E180
// FUNCTION: STBC 0x004742f0 _$E186
// FUNCTION: STBC 0x00474300 _$E185
// FUNCTION: STBC 0x00474310 _$E414
// FUNCTION: STBC 0x00474320 _$E413
// FUNCTION: STBC 0x00474330 _$E418
// FUNCTION: STBC 0x00474340 _$E417
// FUNCTION: STBC 0x00474350 _$E422
// FUNCTION: STBC 0x00474360 _$E421
// FUNCTION: STBC 0x00474370 _$E426
// FUNCTION: STBC 0x00474380 _$E425
// FUNCTION: STBC 0x00474390 _$E430
// FUNCTION: STBC 0x004743a0 _$E429
// FUNCTION: STBC 0x004743b0 _$E434
// FUNCTION: STBC 0x004743c0 _$E433
// FUNCTION: STBC 0x004743d0 _$E438
// FUNCTION: STBC 0x004743e0 _$E437
// FUNCTION: STBC 0x004743f0 _$E442
// FUNCTION: STBC 0x00474400 _$E441
// FUNCTION: STBC 0x00474410 _$E191
// FUNCTION: STBC 0x00474420 _$E190
// FUNCTION: STBC 0x00474430 _$E446
// FUNCTION: STBC 0x00474440 _$E445
// FUNCTION: STBC 0x00474450 _$E196
// FUNCTION: STBC 0x00474460 _$E195
// FUNCTION: STBC 0x00474470 _$E201
// FUNCTION: STBC 0x00474480 _$E200
// FUNCTION: STBC 0x00474490 _$E450
// FUNCTION: STBC 0x004744a0 _$E449
// FUNCTION: STBC 0x004744b0 _$E455
// FUNCTION: STBC 0x004744c0 _$E454
// FUNCTION: STBC 0x004744d0 _$E459
// FUNCTION: STBC 0x004744e0 _$E458

/*
================
AIScriptAssist_GetIncomingTorpIDsInSet
================
*/
// FUNCTION: STBC 0x00473830
extern "C" PyObject *_wrap_AIScriptAssist_GetIncomingTorpIDsInSet( PyObject *self, PyObject *args ) {
	PyObject *objectInstance = NULL;
	PyObject *setInstance = NULL;
	float time = 0.0f;
	long parentID = 0;
	int checkParent = 0;
	// BUG: PyArg_ParseTuple returns 0 on failure, never -1
	if ( PyArg_ParseTuple( args, "OOfii", &objectInstance, &setInstance, &time, &parentID, &checkParent ) == -1 ) {
		return NULL;
	}

	ObjectClass *object = NULL;
	PyEmbed::Unknown6F98E0( objectInstance, (void **)&object, "ObjectClass" );
	if ( object == NULL ) {
		return NULL;
	}
	SetClass *set = NULL;
	PyEmbed::Unknown6F98E0( setInstance, (void **)&set, "SetClass" );
	if ( set == NULL ) {
		return NULL;
	}

	PyObject *list = PyList_New( 0 );
	int count = 0;
	Torpedo **torpedoes = Torpedo::GetClassObjects( set, &count );
	for ( int i = 0; i < count; i++ ) {
		if ( AIScriptAssist::TorpIsIncoming( object, torpedoes[i], time, parentID, checkParent != 0 ) ) {
			// BUG: the list takes its own reference; the new integer's is never released
			PyList_Append( list, PyInt_FromLong( torpedoes[i]->GetObjID() ) );
		}
	}
	return list;
}

/*
================
AIScriptAssist::TorpIsIncoming
================
*/
// FUNCTION: STBC 0x00473960
bool AIScriptAssist::TorpIsIncoming( ObjectClass *object, Torpedo *torpedo, float time, long parentID, bool checkParent ) {
	if ( checkParent && torpedo->GetParentID() != parentID ) {
		return false;
	}
	if ( torpedo->GetContainingSet() != object->GetContainingSet() ) {
		return false;
	}
	if ( torpedo->GetTargetID() != object->GetObjID() ) {
		return false;
	}

	NiPoint3 velocity = torpedo->GetVelocity();
	float speed = NiPoint3::VectorLength( velocity );
	NiPoint3 offset = torpedo->GetWorldLocation() - object->GetWorldLocation();
	if ( offset.Dot( velocity ) > 0.0f ) {
		return false;
	}
	float distance = NiPoint3::VectorLength( offset );
	if ( distance <= 0.0f || speed <= 0.0f ) {
		return false;
	}
	if ( distance / speed < time ) {
		return true;
	}
	return false;
}
