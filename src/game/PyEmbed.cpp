/*
===============================================================================

	PyEmbed.cpp

===============================================================================
*/

#include <iostream>
#include <windows.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "PyEmbed.h"
#include "SWIGRuntime.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006fbbd0 _$E454
// FUNCTION: STBC 0x006fbbf0 _$E453

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006fb160 _$E32
// FUNCTION: STBC 0x006fb170 _$E29
// FUNCTION: STBC 0x006fb180 _$E31
// FUNCTION: STBC 0x006fb190 _$E30
// FUNCTION: STBC 0x006fb3d0 _$E38
// FUNCTION: STBC 0x006fb3e0 _$E35
// FUNCTION: STBC 0x006fb3f0 _$E37
// FUNCTION: STBC 0x006fb400 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006fb7d0 _$E65
// FUNCTION: STBC 0x006fb7e0 _$E64
// FUNCTION: STBC 0x006fb7f0 _$E69
// FUNCTION: STBC 0x006fb800 _$E68
// FUNCTION: STBC 0x006fb810 _$E80
// FUNCTION: STBC 0x006fb820 _$E79
// FUNCTION: STBC 0x006fb830 _$E84
// FUNCTION: STBC 0x006fb840 _$E83
// FUNCTION: STBC 0x006fb850 _$E88
// FUNCTION: STBC 0x006fb860 _$E87
// FUNCTION: STBC 0x006fb870 _$E92
// FUNCTION: STBC 0x006fb880 _$E91
// FUNCTION: STBC 0x006fb890 _$E216
// FUNCTION: STBC 0x006fb8a0 _$E215
// FUNCTION: STBC 0x006fb930 _$E158
// FUNCTION: STBC 0x006fb940 _$E157
// FUNCTION: STBC 0x006fb950 _$E171
// FUNCTION: STBC 0x006fb960 _$E170
// FUNCTION: STBC 0x006fb970 _$E221
// FUNCTION: STBC 0x006fb980 _$E220
// FUNCTION: STBC 0x006fb990 _$E226
// FUNCTION: STBC 0x006fb9a0 _$E225
// FUNCTION: STBC 0x006fb9b0 _$E230
// FUNCTION: STBC 0x006fb9c0 _$E229
// FUNCTION: STBC 0x006fb9d0 _$E234
// FUNCTION: STBC 0x006fb9e0 _$E233
// FUNCTION: STBC 0x006fb9f0 _$E241
// FUNCTION: STBC 0x006fba00 _$E240
// FUNCTION: STBC 0x006fba10 _$E266
// FUNCTION: STBC 0x006fba20 _$E265
// FUNCTION: STBC 0x006fba30 _$E270
// FUNCTION: STBC 0x006fba40 _$E269
// FUNCTION: STBC 0x006fba50 _$E116
// FUNCTION: STBC 0x006fba60 _$E115
// FUNCTION: STBC 0x006fbb30 _$E252
// FUNCTION: STBC 0x006fbb40 _$E251
// FUNCTION: STBC 0x006fbb50 _$E259
// FUNCTION: STBC 0x006fbb60 _$E258
// FUNCTION: STBC 0x006fbb70 _$E282
// FUNCTION: STBC 0x006fbb80 _$E281
// FUNCTION: STBC 0x006fbb90 _$E286
// FUNCTION: STBC 0x006fbba0 _$E285
// FUNCTION: STBC 0x006fbbb0 _$E326
// FUNCTION: STBC 0x006fbbc0 _$E325
// FUNCTION: STBC 0x006fbc00 _$E332
// FUNCTION: STBC 0x006fbc10 _$E331
// FUNCTION: STBC 0x006fbc20 _$E274
// FUNCTION: STBC 0x006fbc30 _$E273
// FUNCTION: STBC 0x006fbc40 _$E278
// FUNCTION: STBC 0x006fbc50 _$E277
// FUNCTION: STBC 0x006fbc60 _$E290
// FUNCTION: STBC 0x006fbc70 _$E289
// FUNCTION: STBC 0x006fbc80 _$E294
// FUNCTION: STBC 0x006fbc90 _$E293
// FUNCTION: STBC 0x006fbca0 _$E298
// FUNCTION: STBC 0x006fbcb0 _$E297
// FUNCTION: STBC 0x006fbcc0 _$E302
// FUNCTION: STBC 0x006fbcd0 _$E301
// FUNCTION: STBC 0x006fbce0 _$E306
// FUNCTION: STBC 0x006fbcf0 _$E305
// FUNCTION: STBC 0x006fbd00 _$E310
// FUNCTION: STBC 0x006fbd10 _$E309
// FUNCTION: STBC 0x006fbd20 _$E314
// FUNCTION: STBC 0x006fbd30 _$E313
// FUNCTION: STBC 0x006fbd40 _$E318
// FUNCTION: STBC 0x006fbd50 _$E317
// FUNCTION: STBC 0x006fbd60 _$E121
// FUNCTION: STBC 0x006fbd70 _$E120
// FUNCTION: STBC 0x006fbd80 _$E322
// FUNCTION: STBC 0x006fbd90 _$E321
// FUNCTION: STBC 0x006fbda0 _$E126
// FUNCTION: STBC 0x006fbdb0 _$E125
// FUNCTION: STBC 0x006fbdc0 _$E336
// FUNCTION: STBC 0x006fbdd0 _$E335
// FUNCTION: STBC 0x006fbde0 _$E340
// FUNCTION: STBC 0x006fbdf0 _$E339
// FUNCTION: STBC 0x006fbe00 _$E344
// FUNCTION: STBC 0x006fbe10 _$E343
// FUNCTION: STBC 0x006fbe20 _$E348
// FUNCTION: STBC 0x006fbe30 _$E347
// FUNCTION: STBC 0x006fbe40 _$E135
// FUNCTION: STBC 0x006fbe50 _$E134
// FUNCTION: STBC 0x006fbe60 _$E352
// FUNCTION: STBC 0x006fbe70 _$E351
// FUNCTION: STBC 0x006fbe80 _$E356
// FUNCTION: STBC 0x006fbe90 _$E355
// FUNCTION: STBC 0x006fbea0 _$E360
// FUNCTION: STBC 0x006fbeb0 _$E359
// FUNCTION: STBC 0x006fbec0 _$E141
// FUNCTION: STBC 0x006fbed0 _$E140
// FUNCTION: STBC 0x006fbee0 _$E146
// FUNCTION: STBC 0x006fbef0 _$E145
// FUNCTION: STBC 0x006fbf00 _$E176
// FUNCTION: STBC 0x006fbf10 _$E175
// FUNCTION: STBC 0x006fbf20 _$E364
// FUNCTION: STBC 0x006fbf30 _$E363
// FUNCTION: STBC 0x006fbf40 _$E181
// FUNCTION: STBC 0x006fbf50 _$E180
// FUNCTION: STBC 0x006fbf60 _$E368
// FUNCTION: STBC 0x006fbf70 _$E367
// FUNCTION: STBC 0x006fbf80 _$E372
// FUNCTION: STBC 0x006fbf90 _$E371
// FUNCTION: STBC 0x006fbfa0 _$E186
// FUNCTION: STBC 0x006fbfb0 _$E185
// FUNCTION: STBC 0x006fbfc0 _$E376
// FUNCTION: STBC 0x006fbfd0 _$E375
// FUNCTION: STBC 0x006fbfe0 _$E151
// FUNCTION: STBC 0x006fbff0 _$E150
// FUNCTION: STBC 0x006fc000 _$E380
// FUNCTION: STBC 0x006fc010 _$E379
// FUNCTION: STBC 0x006fc020 _$E384
// FUNCTION: STBC 0x006fc030 _$E383
// FUNCTION: STBC 0x006fc040 _$E191
// FUNCTION: STBC 0x006fc050 _$E190
// FUNCTION: STBC 0x006fc060 _$E196
// FUNCTION: STBC 0x006fc070 _$E195
// FUNCTION: STBC 0x006fc080 _$E388
// FUNCTION: STBC 0x006fc090 _$E387
// FUNCTION: STBC 0x006fc0a0 _$E392
// FUNCTION: STBC 0x006fc0b0 _$E391
// FUNCTION: STBC 0x006fc0c0 _$E396
// FUNCTION: STBC 0x006fc0d0 _$E395
// FUNCTION: STBC 0x006fc0e0 _$E400
// FUNCTION: STBC 0x006fc0f0 _$E399
// FUNCTION: STBC 0x006fc100 _$E404
// FUNCTION: STBC 0x006fc110 _$E403
// FUNCTION: STBC 0x006fc120 _$E408
// FUNCTION: STBC 0x006fc130 _$E407
// FUNCTION: STBC 0x006fc140 _$E412
// FUNCTION: STBC 0x006fc150 _$E411
// FUNCTION: STBC 0x006fc160 _$E416
// FUNCTION: STBC 0x006fc170 _$E415
// FUNCTION: STBC 0x006fc180 _$E201
// FUNCTION: STBC 0x006fc190 _$E200
// FUNCTION: STBC 0x006fc1a0 _$E420
// FUNCTION: STBC 0x006fc1b0 _$E419
// FUNCTION: STBC 0x006fc1c0 _$E206
// FUNCTION: STBC 0x006fc1d0 _$E205
// FUNCTION: STBC 0x006fc1e0 _$E211
// FUNCTION: STBC 0x006fc1f0 _$E210

// DATA: STBC 0x0099adec
PyObject *PyEmbed::appModule;

// DATA: STBC 0x0099adf0
PyObject *PyEmbed::appDict;

// DATA: STBC 0x0099add4
bool PyEmbed::alwaysReload;

// DATA: STBC 0x0099add5
bool PyEmbed::quietErrors;

// DATA: STBC 0x0099add6
bool PyEmbed::unknown99ADD6;

// DATA: STBC 0x0099add8
int PyEmbed::consoleCount;

// DATA: STBC 0x0099addc
PyObject *PyEmbed::pickler;

// DATA: STBC 0x0099ade4
PyObject *PyEmbed::savedStdout;

// DATA: STBC 0x0099ade0
long PyEmbed::pickleStart;

// DATA: STBC 0x0099ade8
PyObject *PyEmbed::savedStderr;

// DATA: STBC 0x0099adf4
bool PyEmbed::stateFunctionsInstalled;

// DATA: STBC 0x0099adf8
char **PyEmbed::unsavedModules;

// DATA: STBC 0x0099adfc
char **PyEmbed::unsavedPrefixes;

// DATA: STBC 0x0099ae00
int PyEmbed::pickleCount;

// DATA: STBC 0x0099ae08
int PyEmbed::moduleObjectCount;

// A method queued for App's shadow class until App is loaded.
struct PendingClassMethod {		// invented name
	PyMethodDef *	method;
	char *			className;		// heap copy

					PendingClassMethod( PyMethodDef *method_, char *className_ ) {
						method = method_;
						className = new char[strlen( className_ ) + 1];
						strcpy( className, className_ );
					}
};

// The guard bits of the exit destructors give the definition order:
// pendingClassMethods 1, installedClassMethods 2, then the state
// function lists 4, 8 and 16. Their initialiser quads (hi, constructor,
// atexit thunk, exit destructor) open the unit's code, in that order.
// FUNCTION: STBC 0x006f7640 _$E426
// FUNCTION: STBC 0x006f7650 _$E422
// FUNCTION: STBC 0x006f7680 _$E425
// FUNCTION: STBC 0x006f7690 _$E423
// FUNCTION: STBC 0x006f7700 _$E431
// FUNCTION: STBC 0x006f7710 _$E428
// FUNCTION: STBC 0x006f7740 _$E430
// FUNCTION: STBC 0x006f7750 _$E429
// FUNCTION: STBC 0x006f77c0 _$E436
// FUNCTION: STBC 0x006f77d0 _$E433
// FUNCTION: STBC 0x006f7800 _$E435
// FUNCTION: STBC 0x006f7810 _$E434
// FUNCTION: STBC 0x006f7880 _$E441
// FUNCTION: STBC 0x006f7890 _$E438
// FUNCTION: STBC 0x006f78c0 _$E440
// FUNCTION: STBC 0x006f78d0 _$E439
// FUNCTION: STBC 0x006f7940 _$E446
// FUNCTION: STBC 0x006f7950 _$E443
// FUNCTION: STBC 0x006f7980 _$E445
// FUNCTION: STBC 0x006f7990 _$E444

// DATA: STBC 0x0099ad38 ?pendingClassMethods@PyEmbed@@2V?$NiTList@PAUPendingClassMethod@@@@A
NiTList<PendingClassMethod *> PyEmbed::pendingClassMethods( 2 );

// DATA: STBC 0x0099ace8 ?installedClassMethods@PyEmbed@@2V?$NiTList@PAUPyMethodDef@@@@A
NiTList<PyMethodDef *> PyEmbed::installedClassMethods( 2 );

// DATA: STBC 0x0099ad08 ?getStateFunctions@PyEmbed@@2V?$NiTList@PAUPyGetStateFunction@@@@A
NiTList<PyGetStateFunction *> PyEmbed::getStateFunctions( 2 );

// DATA: STBC 0x0099ac68 ?setStateFunctions@PyEmbed@@2V?$NiTList@PAUPySetStateFunction@@@@A
NiTList<PySetStateFunction *> PyEmbed::setStateFunctions( 2 );

// DATA: STBC 0x0099aca8 ?streamCallbacks@PyEmbed@@2V?$NiTList@PAUPyStreamCallback@@@@A
NiTList<PyStreamCallback *> PyEmbed::streamCallbacks( 2 );

/*
================
PyEmbed::GetModuleName

The module a NULL name means: __main__.
================
*/
// FUNCTION: STBC 0x006f7a00
char *PyEmbed::GetModuleName( char *name ) {
	if ( name == NULL ) {
		return "__main__";
	}
	return name;
}

/*
================
PyEmbed::FreeRegistrations

Deletes the class methods still queued for App and the ones installed,
with their names, the state functions with their class names, and the
unsaved module lists.
================
*/
// FUNCTION: STBC 0x006f7a10
void PyEmbed::FreeRegistrations( void ) {
	while ( pendingClassMethods.GetSize() != 0 ) {
		PendingClassMethod *pending = pendingClassMethods.RemoveHead();
		delete [] pending->method->ml_name;
		delete pending->method;
		if ( pending != NULL ) {
			delete [] pending->className;
			delete pending;
		}
	}
	while ( installedClassMethods.GetSize() != 0 ) {
		PyMethodDef *method = installedClassMethods.RemoveHead();
		delete [] method->ml_name;
		delete method;
	}
	while ( getStateFunctions.GetSize() != 0 ) {
		PyGetStateFunction *function = getStateFunctions.RemoveHead();
		delete [] function->className;
		delete function;
	}
	while ( setStateFunctions.GetSize() != 0 ) {
		PySetStateFunction *function = setStateFunctions.RemoveHead();
		delete [] function->className;
		delete function;
	}
	SetUnsavedModules( NULL, NULL );
}

/*
================
PyEmbed::LoadAppModule

Imports App once and keeps its dictionary.
================
*/
// FUNCTION: STBC 0x006f7bc0
void PyEmbed::LoadAppModule( void ) {
	if ( appModule == NULL ) {
		PyEmbedLock lock;
		appModule = PyImport_ImportModule( "App" );
		CheckError();
		if ( appDict == NULL && appModule != NULL ) {
			appDict = PyModule_GetDict( appModule );
		}
		CheckError();
	}
}

/*
================
PyEmbed::ReleaseAppModule
================
*/
// FUNCTION: STBC 0x006f7c60
void PyEmbed::ReleaseAppModule( void ) {
	if ( appModule != NULL ) {
		PyEmbedLock lock;
		Py_DECREF( appModule );
		appModule = NULL;
	}
	appDict = NULL;
}

/*
================
PyEmbed::CreateModule

Makes an empty module of the name, marked as the game's own with
__dummy__; -1 on error.
================
*/
// FUNCTION: STBC 0x006f7ce0
int PyEmbed::CreateModule( char *name ) {
	PyEmbedLock lock;
	PyObject *module = PyImport_AddModule( name );
	bool error = CheckError();
	if ( module == NULL || error == true ) {
		return -1;
	}

	PyObject *dict = PyModule_GetDict( module );
	PyDict_SetItemString( dict, "__dummy__", Py_None );
	PyDict_SetItemString( dict, "__builtins__", PyEval_GetBuiltins() );
	if ( CheckError() == true ) {
		return -1;
	}
	return 0;
}

/*
================
PyEmbed::GetModule

The module of the name, imported when it is not loaded yet; a loaded one is
reloaded on request (or always, with alwaysReload) unless CreateModule made
it. Importing App also runs InstallPendingClassMethods. A borrowed reference; NULL on
error.
================
*/
// FUNCTION: STBC 0x006f7d90
// PARKED: logic and calls match; retail lays the shared "return result" tail out after the __main__ branch, this source after the reload branch (one or two result variables give the same layout)
PyObject *PyEmbed::GetModule( char *name, bool reload ) {
	PyEmbedLock lock;
	name = GetModuleName( name );
	PyObject *module = PyDict_GetItemString( PyImport_GetModuleDict(), name );
	PyObject *result;

	if ( strcmp( name, "__main__" ) == 0 ) {
		result = PyImport_AddModule( name );
		if ( CheckError() == true ) {
			return NULL;
		}
		return result;
	}

	if ( module != NULL && PyDict_GetItemString( PyModule_GetDict( module ), "__dummy__" ) != NULL ) {
		if ( CheckError() == true ) {
			return NULL;
		}
		return module;
	}

	if ( ( reload || alwaysReload ) && module != NULL && PyModule_Check( module ) ) {
		result = PyImport_ReloadModule( module );
		Py_XDECREF( result );
		if ( CheckError() == true ) {
			return NULL;
		}
		return result;
	}

	module = PyImport_ImportModule( name );
	if ( module != NULL ) {
		if ( strcmp( name, "App" ) == 0 ) {
			InstallPendingClassMethods();
		}
		Py_DECREF( module );
	}
	if ( CheckError() == true ) {
		return NULL;
	}
	return module;
}

/*
================
PyEmbed::ReloadModule

Reloads the named module if it is loaded. A borrowed reference to the new
module; NULL when it is not loaded or on error.
================
*/
// FUNCTION: STBC 0x006f7fc0
PyObject *PyEmbed::ReloadModule( char *name ) {
	if ( name == NULL ) {
		return NULL;
	}

	PyEmbedLock lock;
	PyObject *module = PyDict_GetItemString( PyImport_GetModuleDict(), name );
	if ( module != NULL ) {
		// the reloaded module is the same object
		module = PyImport_ReloadModule( module );
		Py_XDECREF( module );
		if ( CheckError() == true ) {
			return NULL;
		}
	}
	return module;
}

/*
================
PyEmbed::UnloadModule

Drops the named module from sys.modules, so that the next GetModule
imports it afresh.
================
*/
// FUNCTION: STBC 0x006f8080
void PyEmbed::UnloadModule( char *name ) {
	PyEmbedLock lock;
	PyObject *modules = PyImport_GetModuleDict();
	if ( PyDict_GetItemString( modules, name ) != NULL ) {
		PyDict_DelItemString( modules, name );
	}
}

/*
================
PyEmbed::UnloadOtherModules

Walks the module dictionary until a pass deletes nothing. A module whose
name is not in the list is unloaded once nothing else holds it: a
submodule is first removed from its parent's dictionary.
================
*/
// FUNCTION: STBC 0x006f80c0
void PyEmbed::UnloadOtherModules( int count, char **keep ) {
	PyEmbedLock lock;
	PyObject *modules = PyImport_GetModuleDict();
	if ( modules == NULL ) {
		return;
	}
	int deleted;
	do {
		deleted = 0;
		PyObject *items = PyDict_Items( modules );
		for ( int i = 0; i < PyList_Size( items ); i++ ) {
			PyObject *item = PyList_GetItem( items, i );
			PyObject *key = PyTuple_GetItem( item, 0 );
			PyObject *value = PyTuple_GetItem( item, 1 );
			if ( value != NULL && PyModule_Check( value ) ) {
				bool kept = false;
				PyObject *moduleName = PyObject_GetAttrString( value, "__name__" );
				if ( moduleName != NULL && PyString_Check( moduleName ) ) {
					char *nameString = PyString_AsString( moduleName );
					for ( int j = 0; j < count; j++ ) {
						if ( strcmp( keep[j], nameString ) == 0 ) {
							kept = true;
							break;
						}
					}
				}
				Py_DECREF( moduleName );
				if ( kept ) {
					continue;
				}
				if ( value->ob_refcnt == 3 ) {
					char *keyString = PyString_AsString( key );
					char *lastDot = NULL;
					char *dot = keyString - 1;
					while ( true ) {
						dot = strchr( dot + 1, '.' );
						if ( dot == NULL ) {
							break;
						}
						lastDot = dot;
					}
					if ( lastDot != NULL ) {
						*lastDot = '\0';
						PyObject *parent = PyDict_GetItemString( modules, keyString );
						*lastDot = '.';
						if ( parent != NULL ) {
							if ( PyObject_HasAttrString( parent, lastDot + 1 ) ) {
								PyObject *dict = PyModule_GetDict( parent );
								PyDict_DelItemString( dict, lastDot + 1 );
							}
						}
					}
				}
			}
			if ( value->ob_refcnt < 3 || value == Py_None ) {
				PyDict_DelItem( modules, key );
				deleted++;
			}
		}
		Py_DECREF( items );
	} while ( deleted != 0 );
}

/*
================
PyEmbed::GetLoadedModuleNames

false when a loaded module has no string __name__.
================
*/
// FUNCTION: STBC 0x006f8310
bool PyEmbed::GetLoadedModuleNames( int *count, char ***names ) {
	PyObject *modules = PyImport_GetModuleDict();
	if ( modules == NULL ) {
		return false;
	}
	PyObject *values = PyDict_Values( modules );
	*count = 0;
	*names = NULL;
	for ( int i = 0; i < PyList_Size( values ); i++ ) {
		PyObject *value = PyList_GetItem( values, i );
		if ( value != NULL && PyModule_Check( value ) ) {
			( *count )++;
		}
	}
	if ( *count != 0 ) {
		*names = new char *[*count];
		int n = 0;
		for ( int j = 0; j < PyList_Size( values ); j++ ) {
			PyObject *value = PyList_GetItem( values, j );
			if ( value != NULL && PyModule_Check( value ) ) {
				PyObject *moduleName = PyObject_GetAttrString( value, "__name__" );
				if ( moduleName == NULL || !PyString_Check( moduleName ) ) {
					return false;
				}
				char *nameString = PyString_AsString( moduleName );
				char *copy = new char[strlen( nameString ) + 1];
				strcpy( copy, nameString );
				( *names )[n] = copy;
				n++;
				Py_DECREF( moduleName );
			}
		}
	}
	Py_DECREF( values );
	return true;
}

/*
================
PyEmbed::GetFunction
================
*/
// FUNCTION: STBC 0x006f8490
PyObject *PyEmbed::GetFunction( char *module, char *function ) {
	PyEmbedLock lock;
	module = GetModuleName( module );
	PyObject *moduleObject = GetModule( module, false );
	if ( CheckError() == true ) {
		return NULL;
	}
	if ( moduleObject == NULL ) {
		return NULL;
	}
	return PyObject_GetAttrString( moduleObject, function );
}

/*
================
PyEmbed::RunConsole

Nothing in the game calls it.
================
*/
// FUNCTION: STBC 0x006f8540
int PyEmbed::RunConsole( char *name ) {
	if ( name != NULL ) {
		printf( "[%s <ctrl-d exits>]\n", name );
	}
	Py_Unknown74BBF0( 1 );
	int result = PyRun_InteractiveLoop( stdin, "<stdin>" );
	Py_Unknown74BBF0( 0 );
	return result;
}

/*
================
PyEmbed::ConvertResult

Converts a new reference with PyArg_Parse into result and releases it;
an "O" conversion keeps the reference for the caller. Without result the
object is only released. -1 on error.
================
*/
// FUNCTION: STBC 0x006f8580
int PyEmbed::ConvertResult( PyObject *object, char *resultFormat, void *result ) {
	if ( object == NULL ) {
		return -1;
	}

	if ( result == NULL ) {
		Py_DECREF( object );
	} else {
		if ( !PyArg_Parse( object, resultFormat, result ) ) {
			Py_DECREF( object );
			return -1;
		}
		if ( CheckError() == true ) {
			return -1;
		}
		if ( strcmp( resultFormat, "O" ) != 0 ) {
			Py_DECREF( object );
		}
	}
	return 0;
}

/*
================
PyEmbed::GetValue

The module's attribute of the name, converted into result.
================
*/
// FUNCTION: STBC 0x006f8650
int PyEmbed::GetValue( char *module, char *name, char *resultFormat, void *result ) {
	PyObject *value = GetFunction( module, name );
	return ConvertResult( value, resultFormat, result );
}

/*
================
PyEmbed::SetValue

Sets the module's attribute to Py_BuildValue( format, ... ).
================
*/
// FUNCTION: STBC 0x006f8680
int PyEmbed::SetValue( char *module, char *name, char *format, ... ) {
	PyEmbedLock lock;
	PyObject *moduleObject = GetModule( module, false );
	if ( moduleObject == NULL ) {
		return -1;
	}
	va_list va;
	va_start( va, format );
	PyObject *value = Py_VaBuildValue( format, va );
	if ( CheckError() == true ) {
		return -1;
	}
	if ( value == NULL ) {
		return -1;
	}
	int status = PyObject_SetAttrString( moduleObject, name, value );
	if ( CheckError() == true ) {
		return -1;
	}
	Py_DECREF( value );
	return status;
}

/*
================
PyEmbed::RunInDebugger

RunString's path with quietErrors set.
================
*/
// FUNCTION: STBC 0x006f87a0
PyObject *PyEmbed::RunInDebugger( int statement, char *code, PyObject *dict ) {
	char *name = "runeval";
	if ( statement ) {
		name = "run";
	}
	PyEmbedLock lock;
	PyObject *function = GetFunction( "TGdb", name );
	if ( function == NULL ) {
		return NULL;
	}
	PyObject *result = PyEval_CallFunction( function, "(sOO)", code, dict, dict );
	Py_DECREF( function );
	if ( CheckError() == true ) {
		return NULL;
	}
	return result;
}

/*
================
PyEmbed::RunString

A statement gives 0, or -1 when it fails; an expression's value is
converted by the format. With quietErrors set the code runs under the
debugger.
================
*/
// FUNCTION: STBC 0x006f8880
int PyEmbed::RunString( int statement, char *code, char *module, char *resultFormat, void *result ) {
	PyEmbedLock lock;
	PyObject *moduleObject = GetModule( module, false );
	if ( moduleObject == NULL ) {
		return -1;
	}
	PyObject *dict = PyModule_GetDict( moduleObject );
	if ( dict == NULL ) {
		return -1;
	}
	PyObject *value;
	if ( quietErrors ) {
		value = RunInDebugger( statement, code, dict );
	} else {
		value = PyRun_String( code, statement ? Py_file_input : Py_eval_input, dict, dict );
	}
	if ( CheckError() == true ) {
		return -1;
	}
	if ( statement == 1 ) {
		int status = value != NULL ? 0 : -1;
		Py_XDECREF( value );
		return status;
	}
	return ConvertResult( value, resultFormat, result );
}

/*
================
PyEmbed::CallInDebugger

Runs the call under the script debugger: the callable is put in front of
the arguments (the tuple grows at its front) and TGdb.runcall is called
with them.
================
*/
// FUNCTION: STBC 0x006f89e0
PyObject *PyEmbed::CallInDebugger( PyObject *callable, PyObject *args ) {
	PyEmbedLock lock;
	PyObject *value;
	if ( args != NULL ) {
		int error = _PyTuple_Resize( &args, PyTuple_Size( args ) + 1, 1 );
		error |= PyTuple_SetItem( args, 0, callable );
		if ( error ) {
			return NULL;
		}
	}
	int status = CallFunction( "TGdb", "runcall", "O", &value, "O", args );
	return status == 0 ? value : NULL;
}

/*
================
PyEmbed::CallFunction

The callable goes through CallInDebugger only when the module is TGdb
itself (and quietErrors is set).
================
*/
// FUNCTION: STBC 0x006f8ab0
int PyEmbed::CallFunction( char *module, char *function, char *resultFormat, void *result, char *format, ... ) {
	PyEmbedLock lock;
	PyObject *callable = GetFunction( module, function );
	if ( callable == NULL || CheckError() == true ) {
		return -1;
	}

	PyObject *args;
	if ( format != NULL && *format != '\0' ) {
		va_list va;
		va_start( va, format );
		args = Py_VaBuildValue( format, va );
		if ( args == NULL || CheckError() == true ) {
			Py_DECREF( callable );
			return -1;
		}
	} else {
		args = NULL;
	}
	if ( CheckError() == true ) {
		return -1;
	}
	return CallObject( callable, resultFormat, result, args, strcmp( module, "TGdb" ) == 0, true );
}

/*
================
PyEmbed::CallObject
================
*/
// FUNCTION: STBC 0x006f8bd0 ?CallObject@PyEmbed@@SAHPAU_object@@PADPAX0@Z
int PyEmbed::CallObject( PyObject *callable, char *resultFormat, void *result, PyObject *args ) {
	return CallObject( callable, resultFormat, result, args, false, false );
}

/*
================
PyEmbed::CallObject
================
*/
// FUNCTION: STBC 0x006f8c00 ?CallObject@PyEmbed@@SAHPAU_object@@PADPAX0_N3@Z
int PyEmbed::CallObject( PyObject *callable, char *resultFormat, void *result, PyObject *args, bool unknownFlag, bool releaseCallable ) {
	PyEmbedLock lock;
	PyObject *value;
	if ( quietErrors && unknownFlag ) {
		value = CallInDebugger( callable, args );
	} else {
		value = PyEval_CallObjectWithKeywords( callable, args, NULL );
	}
	if ( releaseCallable ) {
		Py_DECREF( callable );
	}
	Py_XDECREF( args );

	int status = ConvertResult( value, resultFormat, result );
	if ( CheckError() == true ) {
		return -1;
	}
	return status;
}

/*
================
PyEmbed::CallMethod
================
*/
// FUNCTION: STBC 0x006f8cf0
int PyEmbed::CallMethod( PyObject *object, char *method, char *resultFormat, void *result, char *format, ... ) {
	PyEmbedLock lock;
	int status;
	PyObject *callable = PyObject_GetAttrString( object, method );
	if ( callable == NULL ) {
		status = -1;
	} else {
		va_list va;
		va_start( va, format );
		PyObject *args = Py_VaBuildValue( format, va );
		if ( args == NULL ) {
			Py_DECREF( callable );
			status = -1;
		} else {
			PyObject *value;
			if ( quietErrors ) {
				value = CallInDebugger( callable, args );
			} else {
				value = PyEval_CallObjectWithKeywords( callable, args, NULL );
			}
			Py_DECREF( callable );
			Py_DECREF( args );
			status = ConvertResult( value, resultFormat, result );
		}
	}
	if ( CheckError() == true ) {
		return -1;
	}
	return status;
}

/*
================
PyEmbed::GetAttribute
================
*/
// FUNCTION: STBC 0x006f8e10
int PyEmbed::GetAttribute( PyObject *object, char *name, char *resultFormat, void *result ) {
	PyEmbedLock lock;
	PyObject *value = PyObject_GetAttrString( object, name );
	int status = ConvertResult( value, resultFormat, result );
	if ( CheckError() == true ) {
		return -1;
	}
	return status;
}

/*
================
PyEmbed::SetAttribute
================
*/
// FUNCTION: STBC 0x006f8ea0
int PyEmbed::SetAttribute( PyObject *object, char *name, char *format, ... ) {
	PyEmbedLock lock;
	va_list va;
	va_start( va, format );
	PyObject *value = Py_VaBuildValue( format, va );
	if ( value == NULL ) {
		return -1;
	}
	int status = PyObject_SetAttrString( object, name, value );
	Py_DECREF( value );
	if ( CheckError() == true ) {
		return -1;
	}
	return status;
}

/*
================
PyEmbed::GetObjectPointer
================
*/
// FUNCTION: STBC 0x006f8f70
void *PyEmbed::GetObjectPointer( PyObject *instance ) {
	void *object = NULL;
	SWIG_ConvertPtr( instance, &object, NULL, 0 );
	if ( CheckError() == true ) {
		return NULL;
	}
	return object;
}

/*
================
PyEmbed::HasAttribute
================
*/
// FUNCTION: STBC 0x006f8fb0
bool PyEmbed::HasAttribute( PyObject *object, char *name ) {
	bool found = false;
	PyEmbedLock lock;
	PyObject *value = PyObject_GetAttrString( object, name );
	if ( value != NULL ) {
		found = true;
		Py_DECREF( value );
	} else {
		PyErr_Clear();
	}
	return found;
}

/*
================
PyEmbed::GetBoolAttribute

An int or a long counts as a truth value.
================
*/
// FUNCTION: STBC 0x006f9030
bool PyEmbed::GetBoolAttribute( PyObject *object, char *name, bool *value ) {
	PyEmbedLock lock;
	if ( !PyObject_HasAttrString( object, name ) ) {
		return false;
	}

	bool found = true;
	PyObject *attribute = PyObject_GetAttrString( object, name );
	if ( PyInt_Check( attribute ) ) {
		*value = PyInt_AsLong( attribute ) != 0;
	} else if ( PyLong_Check( attribute ) ) {
		*value = PyLong_AsLong( attribute ) != 0;
	} else {
		found = false;
	}
	Py_DECREF( attribute );
	return found;
}

/*
================
PyEmbed::GetStringAttribute
================
*/
// FUNCTION: STBC 0x006f9110
bool PyEmbed::GetStringAttribute( PyObject *object, char *name, char **value ) {
	*value = NULL;
	PyEmbedLock lock;
	if ( !PyObject_HasAttrString( object, name ) ) {
		return false;
	}

	bool found = true;
	PyObject *attribute = PyObject_GetAttrString( object, name );
	if ( PyString_Check( attribute ) ) {
		char *string = PyString_AsString( attribute );
		*value = new char[strlen( string ) + 1];
		strcpy( *value, string );
	} else {
		found = false;
	}
	Py_DECREF( attribute );
	return found;
}

/*
================
PyEmbed::GetFloatAttribute
================
*/
// FUNCTION: STBC 0x006f9210
bool PyEmbed::GetFloatAttribute( PyObject *object, char *name, float *value ) {
	PyEmbedLock lock;
	if ( !PyObject_HasAttrString( object, name ) ) {
		return false;
	}

	bool found = true;
	PyObject *attribute = PyObject_GetAttrString( object, name );
	if ( PyFloat_Check( attribute ) ) {
		*value = (float)PyFloat_AsDouble( attribute );
	} else {
		found = false;
	}
	Py_DECREF( attribute );
	return found;
}

/*
================
PyEmbed::GetIntAttribute
================
*/
// FUNCTION: STBC 0x006f92d0
bool PyEmbed::GetIntAttribute( PyObject *object, char *name, int *value ) {
	PyEmbedLock lock;
	if ( !PyObject_HasAttrString( object, name ) ) {
		return false;
	}

	bool found = true;
	PyObject *attribute = PyObject_GetAttrString( object, name );
	if ( PyInt_Check( attribute ) ) {
		*value = PyInt_AsLong( attribute );
	} else {
		found = false;
	}
	Py_DECREF( attribute );
	return found;
}

/*
================
PyEmbed::SaveStdout
================
*/
// FUNCTION: STBC 0x006f9390
PyObject *PyEmbed::SaveStdout( void ) {
	if ( savedStdout == NULL ) {
		savedStdout = PySys_GetObject( "stdout" );
	}
	return savedStdout;
}

/*
================
PyEmbed::SaveStderr
================
*/
// FUNCTION: STBC 0x006f93b0
PyObject *PyEmbed::SaveStderr( void ) {
	if ( savedStderr == NULL ) {
		savedStderr = PySys_GetObject( "stderr" );
	}
	return savedStderr;
}

/*
================
PyEmbed::ShowConsole
================
*/
// FUNCTION: STBC 0x006f93d0
void PyEmbed::ShowConsole( bool show ) {
	if ( show ) {
		consoleCount++;
	} else {
		consoleCount--;
	}
	if ( consoleCount == 1 ) {
		AllocConsole();
	} else if ( consoleCount == 0 ) {
		FreeConsole();
	}
}

/*
================
PyEmbed::ConsolePrintf

Writes the formatted text, at most 255 characters, to the console when there
is one.
================
*/
// FUNCTION: STBC 0x006f9410
void PyEmbed::ConsolePrintf( char *format, ... ) {
	DWORD written;
	char buffer[256];
	va_list args;
	va_start( args, format );
	vsprintf( buffer, format, args );
	HANDLE consoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if ( consoleOut != INVALID_HANDLE_VALUE ) {
		WriteConsole( consoleOut, buffer, strlen( buffer ), &written, NULL );
	}
}

/*
================
PyEmbed::Unknown6F9470

The exception debug console CheckError shows when the flag is set: prints
what the script wrote to stderr and stdout so far, then reads lines from the
console and runs them as Python until "resume" (keep running) or "abort"
(end the process).
================
*/
// FUNCTION: STBC 0x006f9470
void PyEmbed::Unknown6F9470( void ) {
	char buffer[256];
	DWORD written;
	PyObject *out = SaveStdout();
	PyObject *err = SaveStderr();
	written = 0;
	ShowConsole( true );

	HANDLE consoleOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if ( consoleOut == INVALID_HANDLE_VALUE ) {
		OutputDebugString( "Error creating console! hConOut is invalid\n" );
	}
	HANDLE consoleIn = GetStdHandle( STD_INPUT_HANDLE );
	if ( consoleIn == INVALID_HANDLE_VALUE ) {
		OutputDebugString( "Error creating console!  hConIn is invalid\n" );
	}

	strcpy( buffer, "TG Python Exception Debug Console.\n" );
	WriteConsole( consoleOut, buffer, strlen( buffer ), &written, NULL );
	strcpy( buffer, "Type 'resume' to keep running, or 'abort' to kill process.\n\n" );
	WriteConsole( consoleOut, buffer, strlen( buffer ), &written, NULL );

	if ( err != NULL ) {
		PyObject *text = PyObject_CallMethod( err, "getvalue", NULL );
		PyObject_CallMethod( err, "close", NULL );
		if ( text != NULL ) {
			char *string = PyString_AsString( text );
			WriteConsole( consoleOut, string, strlen( string ), &written, NULL );
			Py_DECREF( text );
		}
	}

	buffer[0] = 0;
	for ( ;; ) {
		PyRun_SimpleString( buffer );
		if ( out != NULL ) {
			PyObject *text = PyObject_CallMethod( out, "getvalue", NULL );
			PyObject_CallMethod( out, "close", NULL );
			if ( text != NULL ) {
				char *string = PyString_AsString( text );
				WriteConsole( consoleOut, string, strlen( string ), &written, NULL );
				Py_DECREF( text );
			}
		}
		WriteConsole( consoleOut, ">>> ", strlen( ">>> " ), &written, NULL );
		ReadConsole( consoleIn, buffer, 255, &written, NULL );
		if ( buffer[written - 2] == '\r' ) {
			buffer[written - 2] = 0;
		} else if ( buffer[written - 1] == '\n' ) {
			buffer[written - 1] = 0;
		}
		if ( strcmp( buffer, "resume" ) == 0 ) {
			break;
		}
		if ( strcmp( buffer, "abort" ) == 0 ) {
			ExitProcess( 1 );
		}
	}
	ShowConsole( false );
}

/*
===============================================================================

	Functions of the App module written by hand: the SWIG interface lists
	them as native functions (the App method table names them IsNull,
	DeleteBuffer and Breakpoint), so they keep SWIG's wrapper names and C
	linkage, and they parse their arguments without SWIG's typemaps.

===============================================================================
*/

/*
================
_wrap_IsNull

1 when the object's SWIG pointer is NULL (or the call fails), None
otherwise.
================
*/
// FUNCTION: STBC 0x006f9730
extern "C" PyObject *_wrap_IsNull( PyObject *self, PyObject *args ) {
	PyObject *result = Py_None;
	PyObject *object;
	void *pointer;

	PyArg_ParseTuple( args, "O", &object );
	if ( PyEmbed::CheckError() == true ) {
		result = Py_BuildValue( "i", 1 );
		Py_INCREF( result );
		return result;
	}
	SWIG_ConvertPtr( object, &pointer, NULL, 0 );
	if ( pointer == NULL ) {
		result = Py_BuildValue( "i", 1 );
	}
	if ( PyEmbed::CheckError() == true && result == Py_None ) {
		result = Py_BuildValue( "i", 1 );
	}
	Py_INCREF( result );
	return result;
}

/*
================
_wrap_DeleteBuffer

Frees the block a SWIG pointer string points to.
================
*/
// FUNCTION: STBC 0x006f97d0
extern "C" PyObject *_wrap_DeleteBuffer( PyObject *self, PyObject *args ) {
	PyObject *object;
	void *buffer;

	PyArg_ParseTuple( args, "O", &object );
	if ( PyEmbed::CheckError() == true ) {
		PyObject *result = Py_BuildValue( "i", 1 );
		Py_INCREF( result );
		return result;
	}
	SWIG_ConvertPtr( object, &buffer, NULL, 0 );
	if ( buffer != NULL ) {
		delete buffer;
	}
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
PyEmbed::MakePointerString

Writes the SWIG pointer string of the pointer into string: "_", the address
in hex without its leading zeros, then the type part given; "NULL" for a NULL
pointer.
================
*/
// FUNCTION: STBC 0x006f9840
void PyEmbed::MakePointerString( char *string, unsigned int pointer, char *type ) {
	static char hex[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};
	int skipped = 0;

	if ( pointer != 0 ) {
		*string++ = '_';
		while ( ( pointer & 0xF0000000 ) == 0 ) {
			pointer <<= 4;
			skipped++;
		}
		if ( skipped < 8 ) {
			int digits = 8 - skipped;
			do {
				*string++ = hex[pointer >> 28];
				pointer <<= 4;
				digits--;
			} while ( digits != 0 );
		}
		strcpy( string, type );
	} else {
		strcpy( string, "NULL" );
	}
}

/*
================
PyEmbed::Unknown6F98E0
================
*/
// FUNCTION: STBC 0x006f98e0
char *PyEmbed::Unknown6F98E0( PyObject *instance, void **object, char *typeName ) {
	if ( SWIG_ConvertPtr( instance, object, SWIG_TypeQuery( typeName ), 0 ) != 0 ) {
		return NULL;
	}
	if ( CheckError() == true ) {
		return NULL;
	}
	return typeName;
}

// The hex digits, without a terminator (the array is exactly 16 long).
static char hexDigits[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

/*
================
PyEmbed::NewPointerString

The SWIG pointer string of an object: "_", the address in hex without its
leading zeros (at least one digit), then "_p_" and the type name.
================
*/
// FUNCTION: STBC 0x006f9a10
PyObject *PyEmbed::NewPointerString( unsigned int pointer, char *typeName ) {
	char string[128];
	char *out = string;
	int skipped = 0;

	*out++ = '_';
	while ( ( pointer & 0xF0000000 ) == 0 && skipped < 7 ) {
		pointer <<= 4;
		skipped++;
	}
	if ( skipped < 8 ) {
		int digits = 8 - skipped;
		do {
			*out++ = hexDigits[pointer >> 28];
			pointer <<= 4;
			digits--;
		} while ( digits != 0 );
	}
	sprintf( out, "_p_%s", typeName );
	return PyString_FromString( string );
}

/*
================
PyEmbed::Unknown6F9920

A new instance of App's shadow class "<className>Ptr" on the object.
================
*/
// FUNCTION: STBC 0x006f9920
PyObject *PyEmbed::Unknown6F9920( void *object, char *className ) {
	char shadowName[128];
	PyObject *pointer = NewPointerString( (unsigned int)object, className );
	if ( CheckError() == true ) {
		return NULL;
	}
	sprintf( shadowName, "%sPtr", className );
	PyObject *shadowClass = PyDict_GetItemString( appDict, shadowName );
	if ( CheckError() == true ) {
		Py_XDECREF( pointer );
		return NULL;
	}
	PyObject *args = PyTuple_New( 1 );
	PyTuple_SetItem( args, 0, pointer );
	PyObject *instance = PyInstance_New( shadowClass, args, NULL );
	if ( CheckError() == true ) {
		Py_DECREF( pointer );
		return NULL;
	}
	// BUG: PyTuple_SetItem stole the pointer string's reference, so this
	// release gives back a reference the code doesn't own, and the args tuple
	// is never released. The shadow instance's __init__ keeps its own
	// reference in self.this, so the string survives; the net effect is the
	// leaked one-element tuple on every call.
	Py_DECREF( pointer );
	return instance;
}

/*
================
PyEmbed::Unknown6F9A90

The pointer string of the object with the type part of the given one.
================
*/
// FUNCTION: STBC 0x006f9a90
PyObject *PyEmbed::Unknown6F9A90( void *object, PyObject *pointer ) {
	char string[256];
	// BUG: the temporary pointer string is never released.
	char *objectString = PyString_AsString( NewPointerString( (unsigned int)object, "" ) );
	char *pointerString = PyString_AsString( pointer );
	char *type = strstr( pointerString, "_p_" );
	if ( type != NULL ) {
		char *objectType = strstr( objectString, "_p_" );
		if ( objectType != NULL ) {
			*objectType = '\0';
			strcpy( string, objectString );
			strcat( string, type );
			return PyString_FromString( string );
		}
	}
	return NULL;
}

/*
================
PyEmbed::InstallClassMethod

Adds the method to App's class of the name as a built-in function. false
when App has no such class or the attribute can't be set.
================
*/
// FUNCTION: STBC 0x006f9eb0
bool PyEmbed::InstallClassMethod( PyMethodDef *method, char *className ) {
	PyObject *app = PyDict_GetItemString( PyImport_GetModuleDict(), "App" );
	if ( app == NULL ) {
		return false;
	}
	PyObject *shadowClass = PyObject_GetAttrString( app, className );
	if ( shadowClass == NULL ) {
		return false;
	}
	PyObject *function = PyCFunction_New( method, NULL );
	if ( function == NULL ) {
		Py_DECREF( shadowClass );
		return false;
	}
	// BUG: the new function object's reference is never released.
	if ( PyObject_SetAttrString( shadowClass, method->ml_name, function ) == -1 ) {
		Py_DECREF( shadowClass );
		return false;
	}
	installedClassMethods.AddTail( method );
	Py_DECREF( shadowClass );
	return true;
}

/*
================
PyEmbed::AddClassMethod
================
*/
// FUNCTION: STBC 0x006f9bc0
bool PyEmbed::AddClassMethod( PyCFunction function, char *className, char *methodName ) {
	PyMethodDef *method = new PyMethodDef;
	method->ml_meth = function;
	method->ml_doc = NULL;
	method->ml_flags = METH_VARARGS;
	method->ml_name = new char[strlen( methodName ) + 1];
	strcpy( method->ml_name, methodName );

	if ( Py_IsInitialized() && PyDict_GetItemString( PyImport_GetModuleDict(), "App" ) != NULL ) {
		return InstallClassMethod( method, className );
	}

	PendingClassMethod *pending = new PendingClassMethod( method, className );
	pendingClassMethods.AddTail( pending );
	return true;
}

/*
================
PyEmbed::InstallPendingClassMethods

Installs the queued class methods now that App is loaded; the ones that
can't be installed stay queued.
================
*/
// FUNCTION: STBC 0x006f9db0
void PyEmbed::InstallPendingClassMethods( void ) {
	NiTListIterator pos = pendingClassMethods.GetHeadPos();
	while ( pos != NULL ) {
		NiTListIterator itemPos = pos;
		PendingClassMethod *pending = pendingClassMethods.GetNext( pos );
		if ( InstallClassMethod( pending->method, pending->className ) ) {
			pendingClassMethods.RemovePos( itemPos );
			if ( pending != NULL ) {
				delete [] pending->className;
				delete pending;
			}
		}
	}
}

/*
================
_wrap_Breakpoint

Stops in the debugger: the scripts' breakpoint.
================
*/
// FUNCTION: STBC 0x006f9b60
// ORIGINAL-ASM: the `int 3` is the original's inline `__asm int 3` (VC6 has no breakpoint intrinsic, and DebugBreak would be a call into kernel32)
extern "C" PyObject *_wrap_Breakpoint( PyObject *self, PyObject *args ) {
	__asm int 3
	Py_INCREF( Py_None );
	return Py_None;
}

/*
================
PyEmbed::CheckError
================
*/
// FUNCTION: STBC 0x006f9b80
bool PyEmbed::CheckError( void ) {
	if ( PyErr_Occurred() != NULL ) {
		if ( unknown99ADD6 == true ) {
			if ( !quietErrors ) {
				Unknown6F9470();
			}
		} else if ( !quietErrors ) {
			PyErr_Print();
		}
		PyErr_Clear();
		return true;
	}
	return false;
}

/*
================
PyEmbed::StartSave

Makes the pickler and leaves room in the stream for the link to the end
of the pickled data.
================
*/
// FUNCTION: STBC 0x006f9fb0
bool PyEmbed::StartSave( TGStream *stream ) {
	if ( pickler != NULL ) {
		return false;
	}
	if ( CallFunction( "cPickle", "Pickler", "O", &pickler, "(i)", 1 ) == -1 ) {
		return false;
	}
	pickleStart = stream->GetPos();
	stream->WriteLong( 0 );
	pickleCount = 0;
	return true;
}

/*
================
PyEmbed::FinishSave

Pickles the closing count, links the save's start to here and writes the
pickled data: its length, then the bytes.
================
*/
// FUNCTION: STBC 0x006fa020
bool PyEmbed::FinishSave( TGStream *stream ) {
	if ( pickler == NULL ) {
		return false;
	}

	bool success = true;
	if ( CallMethod( pickler, "dump", "", NULL, "(i)", pickleCount ) == -1 ) {
		success = false;
		PyErr_Print();
	}
	PyObject *data = NULL;
	if ( CallMethod( pickler, "getvalue", "O", &data, "()" ) == -1 ) {
		success = false;
	}
	Py_DECREF( pickler );
	pickler = NULL;
	if ( !success ) {
		return false;
	}

	long end = stream->GetPos();
	stream->VirtualSlot55( pickleStart, 0 );
	stream->WriteLong( end );
	stream->VirtualSlot55( end, 0 );
	int size = PyString_Size( data );
	stream->WriteInt( size );
	stream->Write( PyString_AsString( data ), size );
	Py_DECREF( data );
	return true;
}

/*
================
PyEmbed::StartLoad

Reads the pickled data found at the save's link and makes the unpickler
over it; the stream is left where it was.
================
*/
// FUNCTION: STBC 0x006fa150
bool PyEmbed::StartLoad( TGStream *stream ) {
	if ( pickler != NULL ) {
		return false;
	}

	long end = stream->ReadLong();
	long position = stream->GetPos();
	if ( end == 0 ) {
		return false;
	}
	stream->VirtualSlot55( end, 0 );
	int size = stream->ReadInt();
	char *buffer = new char[size];
	stream->Read( buffer, size );
	PyObject *data = PyString_FromStringAndSize( buffer, size );
	delete buffer;
	stream->VirtualSlot55( position, 0 );

	PyObject *file = NULL;
	int status = CallFunction( "cStringIO", "StringIO", "O", &file, "(O)", data );
	if ( status != -1 ) {
		status = CallFunction( "cPickle", "Unpickler", "O", &pickler, "(O)", file );
	}
	Py_XDECREF( file );
	Py_DECREF( data );
	if ( status == -1 ) {
		return false;
	}
	pickleCount = 0;
	return true;
}

/*
================
PyEmbed::FinishLoad
================
*/
// FUNCTION: STBC 0x006fa270
bool PyEmbed::FinishLoad( TGStream *stream ) {
	if ( pickler == NULL ) {
		return false;
	}

	int count = 0;
	int status = CallMethod( pickler, "load", "i", &count, "()" );
	Py_DECREF( pickler );
	pickler = NULL;
	if ( status != -1 && count == pickleCount ) {
		return true;
	}
	PyErr_Print();
	return false;
}

/*
================
PyEmbed::FindNoCase

The first place pattern occurs in string, ignoring case; NULL if none.
================
*/
// FUNCTION: STBC 0x006fa9f0
// PARKED: same calls; retail tests pattern[j] at the bottom of the inner loop (rotated, before the increment) and keeps the outer index test as je/jmp; this source tests it at the top. Retail calls it only from 0x6FAA90, so it may be a file-static helper
char *PyEmbed::FindNoCase( char *string, char *pattern ) {
	if ( *pattern == '\0' ) {
		return string;
	}
	for ( int i = 0; string[i] != '\0'; i++ ) {
		if ( tolower( string[i] ) == tolower( *pattern ) ) {
			for ( int j = 1; ; j++ ) {
				if ( pattern[j] == '\0' ) {
					return &string[i];
				}
				if ( tolower( string[i + j] ) != tolower( pattern[j] ) ) {
					break;
				}
			}
		}
	}
	return NULL;
}

/*
================
PyEmbed::SaveModules

Writes the state of every loaded module the save does not skip: a true,
the name to load it by and its state; a false ends the list. A module
loaded from under the scripts path is named by its path below it, with
dots for the separators and without the .py or .pyc suffix. The name is
invented.
================
*/
// FUNCTION: STBC 0x006faa90
// DRAFT: same calls and branches; retail keeps the module name in ebp and the save name in ebx (ours the other way), their frame slots swapped, and the extension pointer in ebp across the inline strcmp (ours ecx)
bool PyEmbed::SaveModules( TGStream *stream, char *scriptsPath ) {
	PyObject *modules = PyImport_GetModuleDict();
	if ( modules == NULL ) {
		return false;
	}
	PyObject *keys = PyDict_Keys( modules );	// BUG: never released
	if ( keys == NULL ) {
		return false;
	}
	int count = PyList_Size( keys );
	for ( int i = 0; i < count; i++ ) {
		char *name = PyString_AsString( PyList_GetItem( keys, i ) );
		if ( name == NULL ) {
			return false;
		}
		if ( IsUnsavedModule( name ) ) {
			continue;
		}
		char *saveName = name;
		// BUG: the key string made for the lookup is never released
		PyObject *module = PyDict_GetItem( modules, PyString_FromString( name ) );
		if ( module == NULL || !PyModule_Check( module ) ) {
			continue;
		}
		PyObject *file = PyObject_GetAttrString( module, "__file__" );
		if ( file == NULL ) {
			continue;
		}

		char *path = PyString_AsString( file );
		char *relative = FindNoCase( path, scriptsPath );
		if ( relative != NULL ) {
			relative += strlen( scriptsPath );
			if ( *relative == '/' || *relative == '\\' ) {
				relative++;
			}
			saveName = new char[strlen( relative ) + 1];
			strcpy( saveName, relative );
			for ( char *c = saveName; *c != '\0'; c++ ) {
				if ( *c == '/' || *c == '\\' ) {
					*c = '.';
				}
			}
			int length = strlen( saveName );
			if ( length > 3 && strcmp( ".py", &saveName[length - 3] ) == 0 ) {
				saveName[length - 3] = '\0';
			} else if ( length > 4 && strcmp( ".pyc", &saveName[length - 4] ) == 0 ) {
				saveName[length - 4] = '\0';
			}
		}
		Py_DECREF( file );

		if ( !IsUnsavedModule( saveName ) && PyModule_Check( PyDict_GetItemString( modules, name ) ) ) {
			stream->WriteBool( true );
			stream->VirtualSlot11( saveName );
			if ( !SaveModuleState( name, stream, false ) ) {
				return false;
			}
		}
		if ( saveName != name ) {
			delete saveName;
		}
	}
	stream->WriteBool( false );
	return true;
}

/*
================
PyEmbed::SetUnsavedModules

Replaces the lists with heap copies of the NULL-terminated lists given.
================
*/
// FUNCTION: STBC 0x006fad20
void PyEmbed::SetUnsavedModules( char **modules, char **prefixes ) {
	int i;
	if ( unsavedModules != NULL ) {
		for ( i = 0; unsavedModules[i] != NULL; i++ ) {
			delete unsavedModules[i];
		}
		unsavedModules = NULL;	// BUG: the array itself is never freed
	}
	if ( unsavedPrefixes != NULL ) {
		for ( i = 0; unsavedPrefixes[i] != NULL; i++ ) {
			delete unsavedPrefixes[i];
		}
		unsavedPrefixes = NULL;	// BUG: the array itself is never freed
	}
	if ( modules != NULL ) {
		int count = 0;
		while ( modules[count] != NULL ) {
			count++;
		}
		unsavedModules = new char *[count + 1];
		for ( i = 0; modules[i] != NULL; i++ ) {
			char *module = modules[i];
			char *copy = new char[strlen( module ) + 1];
			strcpy( copy, module );
			unsavedModules[i] = copy;
		}
		unsavedModules[i] = NULL;
	}
	if ( prefixes != NULL ) {
		int count = 0;
		while ( prefixes[count] != NULL ) {
			count++;
		}
		unsavedPrefixes = new char *[count + 1];
		for ( i = 0; prefixes[i] != NULL; i++ ) {
			char *prefix = prefixes[i];
			char *copy = new char[strlen( prefix ) + 1];
			strcpy( copy, prefix );
			unsavedPrefixes[i] = copy;
		}
		unsavedPrefixes[i] = NULL;
	}
}

/*
================
PyEmbed::IsUnsavedModule
================
*/
// FUNCTION: STBC 0x006faed0
bool PyEmbed::IsUnsavedModule( char *name ) {
	if ( unsavedModules != NULL ) {
		for ( int i = 0; unsavedModules[i] != NULL; i++ ) {
			if ( strcmp( unsavedModules[i], name ) == 0 ) {
				return true;
			}
		}
	}
	if ( unsavedPrefixes != NULL ) {
		for ( int i = 0; unsavedPrefixes[i] != NULL; i++ ) {
			if ( strstr( name, unsavedPrefixes[i] ) == name ) {
				return true;
			}
		}
	}
	return false;
}

/*
================
PyEmbed::LoadModules

Reads the module states SaveModules wrote; false when a name is missing or
a state fails to load. The names read are never freed. The name is
invented.
================
*/
// FUNCTION: STBC 0x006faf70
bool PyEmbed::LoadModules( TGStream *stream ) {
	while ( stream->ReadBool() ) {
		char *module = stream->VirtualSlot10( -1 );
		if ( module == NULL || !LoadModuleState( module, stream ) ) {
			return false;
		}
	}
	return true;
}

/*
================
PyEmbed::InstallStateFunctions

Lets cPickle save and restore the game's script classes through App's
state functions.
================
*/
// FUNCTION: STBC 0x006fafb0
bool PyEmbed::InstallStateFunctions( void ) {
	if ( stateFunctionsInstalled ) {
		return true;
	}

	PyObject *dict = NULL;
	if ( GetValue( "App", "__dict__", "O", &dict ) == -1 || dict == NULL ) {
		return false;
	}

	PyObject *getState = NULL;
	PyObject *setState = NULL;
	if ( GetValue( "App", "SerializeGetState", "O", &getState ) == -1 ||
		GetValue( "App", "UnserializeSetState", "O", &setState ) == -1 ) {
		PyErr_Print();
		return false;
	}

	// the list of values is never released
	PyObject *values = PyDict_Values( dict );
	if ( values != NULL ) {
		for ( int i = 0; i < PyList_Size( values ); i++ ) {
			PyObject *value = PyList_GetItem( values, i );
			if ( value != NULL && PyClass_Check( value ) ) {
				PyObject_SetAttrString( value, "__getstate__", getState );
				PyObject_SetAttrString( value, "__setstate__", setState );
			}
		}
	}
	stateFunctionsInstalled = true;
	return true;
}

/*
================
PyEmbed::SerializePython

Pickles the sequence number of the object, then the object, and runs the
stream functions its pickling filed; false when anything fails.
================
*/
// FUNCTION: STBC 0x006fa2f0
bool PyEmbed::SerializePython( PyObject *object, TGStream *stream, int mode ) {
	if ( mode == 1 ) {
		return false;
	}
	if ( pickler == NULL ) {
		return false;
	}
	bool success = true;
	success &= InstallStateFunctions();
	if ( !success ) {
		return false;
	}
	while ( streamCallbacks.GetSize() != 0 ) {
		delete streamCallbacks.RemoveHead();
	}
	if ( object == NULL ) {
		return false;
	}
	if ( CallMethod( pickler, "dump", "", NULL, "(i)", pickleCount ) == -1 ) {
		PyErr_Print();
		return false;
	}
	pickleCount++;
	if ( CallMethod( pickler, "dump", "", NULL, "(O)", object ) == -1 ) {
		return false;
	}
	success &= RunStreamCallbacks( stream );
	return success;
}

/*
================
PyEmbed::UnserializePython

Unpickles the next object after checking its sequence number, and runs
the stream functions its unpickling filed; NULL when anything fails. A
wrong sequence number is a system error: the load cannot go on.
================
*/
// FUNCTION: STBC 0x006fa410
PyObject *PyEmbed::UnserializePython( TGStream *stream, int mode ) {
	PyObject *object = NULL;
	if ( mode == 1 || pickler == NULL || !InstallStateFunctions() ) {
		return NULL;
	}
	while ( streamCallbacks.GetSize() != 0 ) {
		delete streamCallbacks.RemoveHead();
	}
	int sequence = 0;
	int status = CallMethod( pickler, "load", "i", &sequence, "()" );
	if ( status == -1 || sequence != pickleCount ) {
		PyErr_Print();
		if ( status != -1 ) {
			PyErr_SetString( PyExc_SystemError, "UnserializePython read an invalid sequence number.  Cannot continue load." );
			CheckError();
		}
		return NULL;
	}
	pickleCount++;
	if ( CallMethod( pickler, "load", "O", &object, "()" ) == -1 ) {
		return NULL;
	}
	if ( !RunStreamCallbacks( stream ) ) {
		Py_XDECREF( object );
		return NULL;
	}
	return object;
}

/*
================
PyEmbed::SaveModuleState

Pickles the module's dictionary entries into the save in progress, each as
a key and value pair behind a 1, then a closing 0. Skipped: the module's
own names (__name__, __file__, __builtins__), the names its
NonSerializedObjects lists, keys that are not strings, modules and
functions, and with unknownFlag every value whose type is not a plain
data type. False when the module or an entry fails; an unsaved module is
not written at all.
================
*/
// FUNCTION: STBC 0x006fa560
// PARKED: register allocation flips with the declarations the unit sees: one more member function in NiMultiTextureProperty.h (reached through NiMain.h) swaps the __dict__ and NonSerializedObjects slots and EDI/EBP
bool PyEmbed::SaveModuleState( char *module, TGStream *stream, bool unknownFlag ) {
	bool success = true;
	if ( IsUnsavedModule( module ) ) {
		return false;
	}

	char *savedTypes[] = { "int", "long", "float", "string", "tuple", "list", "dictionary", "" };	// invented name
	char *unsavedNames[] = { "__name__", "__file__", "__builtins__", "" };	// invented name
	char message[256];
	// DATA: STBC 0x0099ae04 _?objectNumber@?4??SaveModuleState@PyEmbed@@SA_NPADPAVTGStream@@_N@Z@4HA
	static int objectNumber;	// invented name
	sprintf( message, "Python serializing %s, object number %d\n", module, objectNumber++ );
	OutputDebugStringA( message );

	PyObject *moduleObject = GetModule( module, false );
	if ( moduleObject == NULL ) {
		CheckError();
		success = false;
	} else {
		PyObject *unsaved = PyObject_GetAttrString( moduleObject, "NonSerializedObjects" );
		if ( unsaved == NULL ) {
			PyErr_Clear();
		}
		PyObject *dict = PyObject_GetAttrString( moduleObject, "__dict__" );
		if ( dict == NULL ) {
			success = false;
		} else {
			// BUG: the key list is never released.
			PyObject *keys = PyObject_CallMethod( dict, "keys", NULL );
			if ( keys == NULL ) {
				success = false;
			} else {
				for ( int i = 0; success && i < PyList_Size( keys ); i++ ) {
					bool skip = false;
					PyObject *key = PyList_GetItem( keys, i );
					PyObject *value = PyDict_GetItem( dict, key );
					if ( PyString_Check( key ) ) {
						int j;
						for ( j = 0; unsavedNames[j][0] != '\0'; j++ ) {
							if ( strcmp( unsavedNames[j], PyString_AsString( key ) ) == 0 ) {
								skip = true;
							}
						}
						if ( unsaved != NULL && PySequence_Contains( unsaved, key ) ) {
							skip = true;
						}
						if ( unknownFlag && !skip ) {
							skip = true;
							for ( j = 0; savedTypes[j][0] != '\0'; j++ ) {
								if ( strcmp( value->ob_type->tp_name, savedTypes[j] ) == 0 ) {
									skip = false;
									break;
								}
							}
						}
					} else {
						skip = true;
					}
					if ( !PyModule_Check( value ) && !PyFunction_Check( value ) && !skip ) {
						stream->WriteChar( 1 );
						success &= SerializePython( key, stream, 0 );
						if ( success ) {
							PyObject *item = PyDict_GetItem( dict, key );
							if ( item != NULL ) {
								success &= SerializePython( item, stream, 0 );
							} else {
								CheckError();
								success = false;
							}
						}
					}
				}
			}
			Py_DECREF( dict );
		}
		Py_XDECREF( unsaved );
	}
	stream->WriteChar( 0 );
	return success;
}

/*
================
PyEmbed::LoadModuleState

Reads the module's dictionary entries back from the load in progress, as
key and value pairs until the stream says there are no more; false when an
entry or the module fails.
================
*/
// FUNCTION: STBC 0x006fa8c0
bool PyEmbed::LoadModuleState( char *module, TGStream *stream ) {
	char message[256];
	bool success = true;

	sprintf( message, "Python unserializing %s, object number %d\n", module, moduleObjectCount++ );
	OutputDebugStringA( message );

	PyObject *moduleObject = GetModule( module, false );
	if ( moduleObject == NULL ) {
		CheckError();
		return false;
	}
	PyObject *dict = PyObject_GetAttrString( moduleObject, "__dict__" );
	if ( dict == NULL ) {
		CheckError();
		return false;
	}
	while ( stream->ReadChar() ) {
		PyObject *key = UnserializePython( stream, 0 );
		PyObject *value = UnserializePython( stream, 0 );
		if ( key == NULL || value == NULL || PyDict_SetItem( dict, key, value ) == -1 ) {
			CheckError();
			success = false;
		}
		Py_XDECREF( key );
		Py_XDECREF( value );
	}
	Py_DECREF( dict );
	return success;
}

/*
================
PyEmbed::RunStreamCallbacks

Runs and deletes the stream functions filed, oldest first; false when any
of them fails, though all run.
================
*/
// FUNCTION: STBC 0x006fb0e0
bool PyEmbed::RunStreamCallbacks( TGStream *stream ) {
	bool success = true;
	while ( streamCallbacks.GetSize() != 0 ) {
		PyStreamCallback *callback = streamCallbacks.RemoveHead();
		success &= callback->function( callback->object, stream );
		delete callback;
	}
	return success;
}

/*
================
PyEmbed::RunGetStateFunctions

Runs the get-state functions of every class the instance belongs to, each
class's before its subclasses', while they succeed. An instance no function
is filed for gets a debug message, unless it is the debugger's.
================
*/
// FUNCTION: STBC 0x006fb1a0
bool PyEmbed::RunGetStateFunctions( PyObject *instance, PyObject **state ) {
	bool success = true;
	NiTList<PyGetStateFunction *> functions( 2 );

	NiTListIterator pos = getStateFunctions.GetHeadPos();
	while ( pos ) {
		PyGetStateFunction *entry = getStateFunctions.Get( pos );
		if ( IsInstance( instance, entry->className ) ) {
			NiTListIterator before = functions.GetHeadPos();
			while ( before ) {
				PyGetStateFunction *filed = functions.Get( before );
				if ( IsSubclass( entry->className, filed->className ) ) {
					break;
				}
				before = functions.GetNextPos( before );
			}
			if ( before ) {
				functions.InsertBefore( before, entry );
			} else {
				functions.AddTail( entry );
			}
		}
		pos = getStateFunctions.GetNextPos( pos );
	}

	if ( functions.GetSize() != 0 ) {
		pos = functions.GetHeadPos();
		while ( pos && success ) {
			PyGetStateFunction *entry = functions.Get( pos );
			if ( entry != NULL && entry->function != NULL ) {
				success &= entry->function( instance, state );
			}
			pos = functions.GetNextPos( pos );
		}
	} else {
		if ( strcmp( PyString_AsString( ( (PyInstanceObject *)instance )->in_class->cl_name ), "CPyDebug" ) != 0 ) {
			OutputDebugString( "Unhandled PyEmbed wrapper in Serialize, this could cause problems in UnSerialize - " );
			OutputDebugString( PyString_AsString( ( (PyInstanceObject *)instance )->in_class->cl_name ) );
			OutputDebugString( "\n" );
		}
	}
	return success;
}

/*
================
PyEmbed::RunSetStateFunctions

Runs the set-state functions of every class the instance belongs to, each
class's before its subclasses', while they succeed. An instance no function
is filed for just takes the state as its dictionary, with a debug message.
================
*/
// FUNCTION: STBC 0x006fb410
bool PyEmbed::RunSetStateFunctions( PyObject *instance, PyObject *state ) {
	bool success = true;
	NiTList<PySetStateFunction *> functions( 2 );

	NiTListIterator pos = setStateFunctions.GetHeadPos();
	while ( pos ) {
		PySetStateFunction *entry = setStateFunctions.Get( pos );
		if ( IsInstance( instance, entry->className ) ) {
			NiTListIterator before = functions.GetHeadPos();
			while ( before ) {
				PySetStateFunction *filed = functions.Get( before );
				if ( IsSubclass( entry->className, filed->className ) ) {
					break;
				}
				before = functions.GetNextPos( before );
			}
			if ( before ) {
				functions.InsertBefore( before, entry );
			} else {
				functions.AddTail( entry );
			}
		}
		pos = setStateFunctions.GetNextPos( pos );
	}

	if ( functions.GetSize() != 0 ) {
		pos = functions.GetHeadPos();
		while ( pos && success ) {
			PySetStateFunction *entry = functions.Get( pos );
			if ( entry != NULL && entry->function != NULL ) {
				success &= entry->function( instance, state );
			}
			pos = functions.GetNextPos( pos );
		}
	} else {
		PyObject_SetAttrString( instance, "__dict__", state );
		OutputDebugString( "Unhandled PyEmbed wrapper in UnSerialize - " );
		OutputDebugString( PyString_AsString( ( (PyInstanceObject *)instance )->in_class->cl_name ) );
		OutputDebugString( "\n" );
		success = false;
	}
	return success;
}

/*
================
PyEmbed::IsInstance

Whether the object is an instance of the App module's class of that name
(the built-in isinstance); false, with the error printed, when either can't
be had.
================
*/
// FUNCTION: STBC 0x006fb610
bool PyEmbed::IsInstance( PyObject *object, char *className ) {
	PyObject *classObject = NULL;
	int result;
	if ( GetValue( "App", className, "O", &classObject ) != -1
		&& CallFunction( "__builtin__", "isinstance", "i", &result, "(OO)", object, classObject ) != -1 ) {
		if ( result ) {
			return true;
		}
	} else {
		PyErr_Print();
	}
	return false;
}

/*
================
PyEmbed::IsSubclass

Whether the App module's class className derives from its class baseName
(PyClass_IsSubclass); false, with the error printed, when either can't be
had.
================
*/
// FUNCTION: STBC 0x006fb680
bool PyEmbed::IsSubclass( char *baseName, char *className ) {
	PyObject *baseClass = NULL;
	if ( GetValue( "App", baseName, "O", &baseClass ) != -1 ) {
		PyObject *derivedClass = NULL;
		if ( GetValue( "App", className, "O", &derivedClass ) != -1 ) {
			if ( PyClass_IsSubclass( derivedClass, baseClass ) ) {
				return true;
			}
		} else {
			PyErr_Print();
		}
	} else {
		PyErr_Print();
	}
	return false;
}

/*
================
_wrap_SerializeGetState

App.SerializeGetState, the classes' __getstate__: the state the get-state
functions hand back, else the instance's dictionary.
================
*/
// FUNCTION: STBC 0x006fb700
extern "C" PyObject *_wrap_SerializeGetState( PyObject *self, PyObject *args ) {
	PyObject *dict = NULL;
	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance != NULL ) {
		PyObject *state = NULL;
		if ( PyEmbed::RunGetStateFunctions( instance, &state ) ) {
			if ( state != NULL ) {
				return state;
			}
			if ( PyEmbed::GetAttribute( instance, "__dict__", "O", &dict ) != -1 ) {
				return dict;
			}
		} else {
			PyErr_SetString( PyExc_SystemError, "Wrapper serialization functions failed.  Can't save object." );
		}
	}
	return NULL;
}

/*
================
_wrap_UnserializeSetState

App.UnserializeSetState, the classes' __setstate__.
================
*/
// FUNCTION: STBC 0x006fb780
extern "C" PyObject *_wrap_UnserializeSetState( PyObject *self, PyObject *args ) {
	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance == NULL ) {
		return NULL;
	}
	PyObject *state = PyTuple_GetItem( args, 1 );
	if ( state == NULL ) {
		return NULL;
	}
	PyEmbed::RunSetStateFunctions( instance, state );
	Py_INCREF( Py_None );
	return Py_None;
}
