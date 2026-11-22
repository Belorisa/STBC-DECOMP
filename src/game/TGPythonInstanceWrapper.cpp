/*
===============================================================================

	TGPythonInstanceWrapper.cpp

===============================================================================
*/

#include <iostream>
#include "TGPythonInstanceWrapper.h"
#include "TGEventHandlerTable.h"
#include "PyEmbed.h"
#include "TGStream.h"
#include "NiMain.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006df380 _$E75
// FUNCTION: STBC 0x006df390 _$E74
// FUNCTION: STBC 0x006df3a0 _$E79
// FUNCTION: STBC 0x006df3b0 _$E78
// FUNCTION: STBC 0x006df3c0 _$E90
// FUNCTION: STBC 0x006df3d0 _$E89
// FUNCTION: STBC 0x006df3e0 _$E94
// FUNCTION: STBC 0x006df3f0 _$E93
// FUNCTION: STBC 0x006df400 _$E98
// FUNCTION: STBC 0x006df410 _$E97
// FUNCTION: STBC 0x006df420 _$E102
// FUNCTION: STBC 0x006df430 _$E101
// FUNCTION: STBC 0x006df440 _$E226
// FUNCTION: STBC 0x006df450 _$E225
// FUNCTION: STBC 0x006df460 _$E168
// FUNCTION: STBC 0x006df470 _$E167
// FUNCTION: STBC 0x006df480 _$E181
// FUNCTION: STBC 0x006df490 _$E180
// FUNCTION: STBC 0x006df4a0 _$E231
// FUNCTION: STBC 0x006df4b0 _$E230
// FUNCTION: STBC 0x006df4c0 _$E236
// FUNCTION: STBC 0x006df4d0 _$E235
// FUNCTION: STBC 0x006df4e0 _$E240
// FUNCTION: STBC 0x006df4f0 _$E239
// FUNCTION: STBC 0x006df500 _$E244
// FUNCTION: STBC 0x006df510 _$E243
// FUNCTION: STBC 0x006df520 _$E251
// FUNCTION: STBC 0x006df530 _$E250
// FUNCTION: STBC 0x006df540 _$E276
// FUNCTION: STBC 0x006df550 _$E275
// FUNCTION: STBC 0x006df560 _$E280
// FUNCTION: STBC 0x006df570 _$E279
// FUNCTION: STBC 0x006df580 _$E126
// FUNCTION: STBC 0x006df590 _$E125
// FUNCTION: STBC 0x006df5a0 _$E262
// FUNCTION: STBC 0x006df5b0 _$E261
// FUNCTION: STBC 0x006df5c0 _$E269
// FUNCTION: STBC 0x006df5d0 _$E268
// FUNCTION: STBC 0x006df5e0 _$E292
// FUNCTION: STBC 0x006df5f0 _$E291
// FUNCTION: STBC 0x006df600 _$E296
// FUNCTION: STBC 0x006df610 _$E295
// FUNCTION: STBC 0x006df620 _$E336
// FUNCTION: STBC 0x006df630 _$E335
// FUNCTION: STBC 0x006df640 _$E342
// FUNCTION: STBC 0x006df650 _$E341
// FUNCTION: STBC 0x006df660 _$E284
// FUNCTION: STBC 0x006df670 _$E283
// FUNCTION: STBC 0x006df680 _$E288
// FUNCTION: STBC 0x006df690 _$E287
// FUNCTION: STBC 0x006df6a0 _$E300
// FUNCTION: STBC 0x006df6b0 _$E299
// FUNCTION: STBC 0x006df6c0 _$E304
// FUNCTION: STBC 0x006df6d0 _$E303
// FUNCTION: STBC 0x006df6e0 _$E308
// FUNCTION: STBC 0x006df6f0 _$E307
// FUNCTION: STBC 0x006df700 _$E312
// FUNCTION: STBC 0x006df710 _$E311
// FUNCTION: STBC 0x006df720 _$E316
// FUNCTION: STBC 0x006df730 _$E315
// FUNCTION: STBC 0x006df740 _$E320
// FUNCTION: STBC 0x006df750 _$E319
// FUNCTION: STBC 0x006df760 _$E324
// FUNCTION: STBC 0x006df770 _$E323
// FUNCTION: STBC 0x006df780 _$E328
// FUNCTION: STBC 0x006df790 _$E327
// FUNCTION: STBC 0x006df7a0 _$E131
// FUNCTION: STBC 0x006df7b0 _$E130
// FUNCTION: STBC 0x006df7c0 _$E332
// FUNCTION: STBC 0x006df7d0 _$E331
// FUNCTION: STBC 0x006df7e0 _$E136
// FUNCTION: STBC 0x006df7f0 _$E135
// FUNCTION: STBC 0x006df800 _$E346
// FUNCTION: STBC 0x006df810 _$E345
// FUNCTION: STBC 0x006df820 _$E350
// FUNCTION: STBC 0x006df830 _$E349
// FUNCTION: STBC 0x006df840 _$E354
// FUNCTION: STBC 0x006df850 _$E353
// FUNCTION: STBC 0x006df860 _$E358
// FUNCTION: STBC 0x006df870 _$E357
// FUNCTION: STBC 0x006df880 _$E145
// FUNCTION: STBC 0x006df890 _$E144
// FUNCTION: STBC 0x006df8a0 _$E362
// FUNCTION: STBC 0x006df8b0 _$E361
// FUNCTION: STBC 0x006df8c0 _$E366
// FUNCTION: STBC 0x006df8d0 _$E365
// FUNCTION: STBC 0x006df8e0 _$E370
// FUNCTION: STBC 0x006df8f0 _$E369
// FUNCTION: STBC 0x006df900 _$E151
// FUNCTION: STBC 0x006df910 _$E150
// FUNCTION: STBC 0x006df920 _$E156
// FUNCTION: STBC 0x006df930 _$E155
// FUNCTION: STBC 0x006df940 _$E186
// FUNCTION: STBC 0x006df950 _$E185
// FUNCTION: STBC 0x006df960 _$E374
// FUNCTION: STBC 0x006df970 _$E373
// FUNCTION: STBC 0x006df980 _$E191
// FUNCTION: STBC 0x006df990 _$E190
// FUNCTION: STBC 0x006df9a0 _$E378
// FUNCTION: STBC 0x006df9b0 _$E377
// FUNCTION: STBC 0x006df9c0 _$E382
// FUNCTION: STBC 0x006df9d0 _$E381
// FUNCTION: STBC 0x006df9e0 _$E196
// FUNCTION: STBC 0x006df9f0 _$E195
// FUNCTION: STBC 0x006dfa00 _$E386
// FUNCTION: STBC 0x006dfa10 _$E385
// FUNCTION: STBC 0x006dfa20 _$E161
// FUNCTION: STBC 0x006dfa30 _$E160
// FUNCTION: STBC 0x006dfa40 _$E390
// FUNCTION: STBC 0x006dfa50 _$E389
// FUNCTION: STBC 0x006dfa60 _$E394
// FUNCTION: STBC 0x006dfa70 _$E393
// FUNCTION: STBC 0x006dfa80 _$E201
// FUNCTION: STBC 0x006dfa90 _$E200
// FUNCTION: STBC 0x006dfaa0 _$E206
// FUNCTION: STBC 0x006dfab0 _$E205
// FUNCTION: STBC 0x006dfac0 _$E398
// FUNCTION: STBC 0x006dfad0 _$E397
// FUNCTION: STBC 0x006dfae0 _$E402
// FUNCTION: STBC 0x006dfaf0 _$E401
// FUNCTION: STBC 0x006dfb00 _$E406
// FUNCTION: STBC 0x006dfb10 _$E405
// FUNCTION: STBC 0x006dfb20 _$E410
// FUNCTION: STBC 0x006dfb30 _$E409
// FUNCTION: STBC 0x006dfb40 _$E414
// FUNCTION: STBC 0x006dfb50 _$E413
// FUNCTION: STBC 0x006dfb60 _$E418
// FUNCTION: STBC 0x006dfb70 _$E417
// FUNCTION: STBC 0x006dfb80 _$E422
// FUNCTION: STBC 0x006dfb90 _$E421
// FUNCTION: STBC 0x006dfba0 _$E426
// FUNCTION: STBC 0x006dfbb0 _$E425
// FUNCTION: STBC 0x006dfbc0 _$E211
// FUNCTION: STBC 0x006dfbd0 _$E210
// FUNCTION: STBC 0x006dfbe0 _$E430
// FUNCTION: STBC 0x006dfbf0 _$E429
// FUNCTION: STBC 0x006dfc00 _$E216
// FUNCTION: STBC 0x006dfc10 _$E215
// FUNCTION: STBC 0x006dfc20 _$E221
// FUNCTION: STBC 0x006dfc30 _$E220

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006df2d0 _$E444
// FUNCTION: STBC 0x006df2f0 _$E443
// FUNCTION: STBC 0x006df300 _$E32
// FUNCTION: STBC 0x006df310 _$E29
// FUNCTION: STBC 0x006df320 _$E31
// FUNCTION: STBC 0x006df330 _$E30
// FUNCTION: STBC 0x006df340 _$E38
// FUNCTION: STBC 0x006df350 _$E35
// FUNCTION: STBC 0x006df360 _$E37
// FUNCTION: STBC 0x006df370 _$E36

/*
================
TGPythonInstanceWrapper::TGPythonInstanceWrapper
================
*/
// FUNCTION: STBC 0x006dfc90 ??0TGPythonInstanceWrapper@@QAE@XZ
TGPythonInstanceWrapper::TGPythonInstanceWrapper( void ) : instance( NULL ) {
}

/*
================
TGPythonInstanceWrapper_SetPyWrapper

Takes the wrapper's shadow instance and the instance to wrap.
================
*/
// FUNCTION: STBC 0x006dfcb0
PyObject *TGPythonInstanceWrapper_SetPyWrapper( PyObject *self, PyObject *args ) {
	PyObject *instance = NULL;
	PyObject *wrapperInstance = NULL;
	PyArg_ParseTuple( args, "OO", &wrapperInstance, &instance );
	if ( PyErr_Occurred() ) {
		PyErr_Print();
		PyErr_Clear();
	}

	TGPythonInstanceWrapper *wrapper = NULL;
	PyEmbed::Unknown6F98E0( wrapperInstance, (void **)&wrapper, "TGPythonInstanceWrapper *" );
	if ( wrapper != NULL ) {
		wrapper->instance = instance;	// BUG: no reference is taken on the instance
	}

	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
TGPythonInstanceWrapper::WriteToStream
================
*/
// FUNCTION: STBC 0x006dfd40
bool TGPythonInstanceWrapper::WriteToStream( TGStream *stream ) {
	return TGEventHandlerObject::WriteToStream( stream );
}


/*
================
TGPythonInstanceWrapper::GetState

An instance that owns its wrapper files the wrapper's write for the stream.
================
*/
// FUNCTION: STBC 0x006dfd50
bool TGPythonInstanceWrapper::GetState( PyObject *instance, PyObject **state ) {
	PyObject *thisown = PyObject_GetAttrString( instance, "thisown" );
	if ( thisown != NULL ) {
		if ( PyInt_AsLong( thisown ) == 1 ) {
			PyStreamCallback *callback = new PyStreamCallback;
			callback->function = WriteWrapper;
			callback->object = instance;
			PyEmbed::streamCallbacks.AddTail( callback );
		}
		Py_DECREF( thisown );
	}
	PyErr_Clear();
	return true;
}

/*
================
TGPythonInstanceWrapper::SetState

An instance that owns its wrapper files the wrapper's read for the stream,
for the instance that contains it; false when there is none.
================
*/
// FUNCTION: STBC 0x006dfe10
bool TGPythonInstanceWrapper::SetState( PyObject *instance, PyObject *state ) {
	bool result = true;

	PyObject *thisown = PyObject_GetAttrString( instance, "thisown" );
	if ( thisown != NULL ) {
		if ( PyInt_AsLong( thisown ) == 1 ) {
			PyObject *containingInstance = PyObject_GetAttrString( instance, "pContainingInstance" );
			if ( containingInstance == NULL ) {
				containingInstance = PyString_FromString( "Python instance containing a TGPythonInstanceWrapper doesn't have the necessary __getstate__ and __setstate__ functions.  (see KDeus)" );
				PyErr_Clear();
				result = false;
			}

			PyStreamCallback *callback = new PyStreamCallback;
			callback->function = ReadWrapper;
			callback->object = containingInstance;
			PyEmbed::streamCallbacks.AddTail( callback );
		}
		Py_DECREF( thisown );
	}
	PyErr_Clear();
	return result;
}

/*
================
TGPythonInstanceWrapper::WriteWrapper

Drops the instance's link to its container, then writes its wrapper.
================
*/
// FUNCTION: STBC 0x006dff00
bool TGPythonInstanceWrapper::WriteWrapper( PyObject *instance, TGStream *stream ) {
	if ( PyObject_HasAttrString( instance, "pContainingInstance" ) ) {
		PyObject_SetAttrString( instance, "pContainingInstance", NULL );

		TGPythonInstanceWrapper *wrapper = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&wrapper, "TGPythonInstanceWrapper" );
		if ( wrapper != NULL ) {
			return wrapper->WriteToStream( stream );
		}
	}
	return false;
}

/*
================
TGPythonInstanceWrapper::ReadWrapper

Reads the wrapper back and makes it wrap the containing instance, whose
reference the callback held.
================
*/
// FUNCTION: STBC 0x006dff60
bool TGPythonInstanceWrapper::ReadWrapper( PyObject *containingInstance, TGStream *stream ) {
	bool result = true;

	TGObject *object = TGObject::LoadObject( stream, 0 );
	if ( object != NULL && object->IsTypeOf( TGPYTHONINSTANCEWRAPPER_TYPE ) ) {
		( (TGPythonInstanceWrapper *)object )->instance = containingInstance;
	} else {
		result = false;
	}
	Py_DECREF( containingInstance );
	return result;
}

// The constructor and destructor of classHandlers.
// FUNCTION: STBC 0x006dfc40 _$E436
// FUNCTION: STBC 0x006dfc50 _$E432
// FUNCTION: STBC 0x006dfc60 _$E435
// FUNCTION: STBC 0x006dfc70 _$E433

// DATA: STBC 0x00998d90
TGEventHandlerTable TGPythonInstanceWrapper::classHandlers;
