/*
===============================================================================

	PyEmbed.h

	The game's embedding of Python: the App module and its dictionary, the
	loading of script modules, error reporting, and the saving and loading
	of Python state with the game.

	The class name is that of the SWIG wrappers PyEmbed_SerializeModule,
	UnserializeModule, StartSave, FinishSave, StartLoad and FinishLoad
	(static functions: the wrappers pass no object) and of the messages
	"Unhandled PyEmbed wrapper in Serialize ...". The unit runs from 0x6F7640
	to 0x6FB160; only part of it is reconstructed. Its functions bracket
	their calls into Python with a PyEmbedLock, which raises Python's
	securelevel for the scope.

===============================================================================
*/

#ifndef __PYEMBED_H__
#define __PYEMBED_H__

#include <string.h>

#include "Python.h"
#include "NiTList.h"

class TGStream;

// Builds the import table of the SWIG modules (App and its siblings): the
// game's cut of SWIG's embed15.i main, defined in AppWrap.cpp and run just
// before Py_Initialize (invented name).
void swig_main( void );

/*
===============================================================================

	PyEmbedLock

	Raises Python's securelevel (Py_Unknown74BBF0 in pythonrun.c) while it
	lives: script code run from the game is denied file access. Empty; the
	constructor and the destructor are inlined, and the out-of-line
	destructor (0x6F7C50) is called only by the unwind funclets of the
	functions that hold one. The name is invented.

===============================================================================
*/

class PyEmbedLock {		// invented name
public:
					PyEmbedLock( void ) { Py_Unknown74BBF0( 1 ); }
					~PyEmbedLock( void );
};

/*
================
PyEmbedLock::~PyEmbedLock
================
*/
// FUNCTION: STBC 0x006f7c50
inline PyEmbedLock::~PyEmbedLock( void ) {
	Py_Unknown74BBF0( 0 );
}

// A Python state function of the shadow instances of a class: called
// with the instance and its state dictionary (invented name).
typedef bool ( *pyStateFunction_t )( PyObject *instance, PyObject *state );

// A get-state function: it may hand back a new state object through state
// (TGPoint3_SaveInstance, 0x45EAA0, stores a tuple there; invented name).
typedef bool ( *pyGetStateFunction_t )( PyObject *instance, PyObject **state );

// The state functions of a class's shadow instances, filed by the class
// name: the get-state functions add to the state a pickled instance
// saves, the set-state functions restore an instance from it. Two
// separate types: each list has its own NewItem (0x446F40, 0x447000).
// The names are invented.
struct PyGetStateFunction {
	pyGetStateFunction_t	function;
	char *					className;		// heap copy
};

struct PySetStateFunction {
	pyStateFunction_t		function;
	char *					className;		// heap copy
};

// A function to run once the object being pickled or unpickled is done,
// with the stream: an instance's state function files one for what it
// writes or reads beside the pickle (0x6DFD50 files 0x6DFF00). The names
// are invented.
typedef bool ( *pyStreamFunction_t )( PyObject *object, TGStream *stream );

struct PyStreamCallback {
	pyStreamFunction_t		function;
	PyObject *				object;			// borrowed
};

struct PendingClassMethod;		// PyEmbed.cpp; invented name

class PyEmbed {
public:
	static char *		GetModuleName( char *name );						// invented name
	static void			LoadAppModule( void );								// invented name
	static void			ReleaseAppModule( void );							// invented name
	// Frees the class methods and state functions filed with the embedding and the unsaved
	// lists: the end of the program (0x6F6CC0 runs it after Py_Finalize; invented name).
	static void			FreeRegistrations( void );
	static int			CreateModule( char *name );							// invented name
	static PyObject *	GetModule( char *name, bool reload );				// invented name
	static PyObject *	ReloadModule( char *name );							// invented name
	static void			UnloadModule( char *name );							// invented name
	// The names of the loaded modules (sys.modules' __name__s), new[]
	// copies in a new[] array; false when a module has no string name
	// (invented name).
	static bool			GetLoadedModuleNames( int *count, char ***names );	// invented name
	// Unloads every loaded module whose name is not one of the given, and
	// clears the dictionaries of the modules it unloads (invented name).
	static void			UnloadOtherModules( int count, char **keep );		// invented name

	// The named function of the module (NULL: __main__).
	static PyObject *	GetFunction( char *module, char *function );		// invented name
	// Converts and releases a new reference; -1 on error.
	static int			ConvertResult( PyObject *object, char *resultFormat, void *result );	// invented name
	// The module's attribute of the name, converted; -1 on error.
	static int			GetValue( char *module, char *name, char *resultFormat, void *result );	// invented name
	// Sets the module's attribute to Py_BuildValue( format, ... ); -1 on error.
	static int			SetValue( char *module, char *name, char *format, ... );	// invented name
	// Call a callable or an object's method; resultFormat and result,
	// when given, convert the result. -1 on error.
	static int			CallObject( PyObject *callable, char *resultFormat, void *result, PyObject *args );		// invented name
	// The full form: unknownFlag (meaning open) calls through
	// CallInDebugger when quietErrors is set; releaseCallable releases the
	// callable. args is always released.
	static int			CallObject( PyObject *callable, char *resultFormat, void *result, PyObject *args, bool unknownFlag, bool releaseCallable );	// invented name
	// Python's interactive loop on stdin, after a banner naming it when the
	// name is given; the loop's result (invented name).
	static int			RunConsole( char *name );
	// Runs the code in the module's dictionary, as a statement or as an
	// expression whose value is converted; -1 on error (invented name).
	static int			RunString( int statement, char *code, char *module, char *resultFormat, void *result );
	// Runs the code under TGdb.run (a statement) or TGdb.runeval (an
	// expression) with the dictionary as globals and locals; a new
	// reference, NULL on error (invented name).
	static PyObject *	RunInDebugger( int statement, char *code, PyObject *dict );
	// Calls TGdb.runcall( callable, *args ); a new reference, NULL on error.
	static PyObject *	CallInDebugger( PyObject *callable, PyObject *args );	// invented name
	// Calls the module's function with Py_BuildValue( format, ... ).
	static int			CallFunction( char *module, char *function, char *resultFormat, void *result, char *format, ... );	// invented name
	static int			CallMethod( PyObject *object, char *method, char *resultFormat, void *result, char *format, ... );	// invented name
	// The object's attribute, converted; -1 on error.
	static int			GetAttribute( PyObject *object, char *name, char *resultFormat, void *result );	// invented name
	// Sets the object's attribute to Py_BuildValue( format, ... ).
	static int			SetAttribute( PyObject *object, char *name, char *format, ... );	// invented name
	// Whether the object has the attribute; a failed lookup's error is cleared.
	static bool			HasAttribute( PyObject *object, char *name );		// invented name
	// The App module's classes by name: an instance test and a subclass test
	// (invented names; ObjectGroup_ForceToGroup's wrapper and the stream
	// callbacks use them).
	static bool			IsInstance( PyObject *object, char *className );
	static bool			IsSubclass( char *baseName, char *className );
	// Typed attribute reads: false when the attribute is missing or of
	// another type. GetStringAttribute returns a new[] copy (NULL if none).
	static bool			GetBoolAttribute( PyObject *object, char *name, bool *value );		// invented name
	static bool			GetStringAttribute( PyObject *object, char *name, char **value );	// invented name
	static bool			GetFloatAttribute( PyObject *object, char *name, float *value );	// invented name
	static bool			GetIntAttribute( PyObject *object, char *name, int *value );		// invented name

	// Keep the first sys.stdout and sys.stderr seen.
	static PyObject *	SaveStdout( void );									// invented name
	static PyObject *	SaveStderr( void );									// invented name
	// Counted: the first show allocates a console, the last hide frees it.
	static void			ShowConsole( bool show );							// invented name
	// Writes the formatted text to the console (unreferenced in retail).
	static void			ConsolePrintf( char *format, ... );					// invented name
	// Gives every class in App's dictionary App.SerializeGetState and
	// App.UnserializeSetState as __getstate__ and __setstate__, once.
	// The first place pattern occurs in string, ignoring case; NULL if none.
	static char *		FindNoCase( char *string, char *pattern );			// invented name
	// Whether the save skips the module: its name is listed, or starts
	// with a listed prefix.
	static bool			IsUnsavedModule( char *name );						// invented name
	// Replaces the unsaved module names and prefixes with heap copies of
	// the NULL-terminated lists given; a NULL list leaves none (invented
	// name; UtopiaModule's constructor, 0x43AB30, sets them).
	static void			SetUnsavedModules( char **modules, char **prefixes );
	static bool			InstallStateFunctions( void );						// invented name
	// Saving and loading Python state with the game: a cPickle pickler
	// collects the objects during the save, and its data is written to the
	// stream behind a link to the end; StartLoad makes the unpickler.
	// FinishLoad ends an unpickling: it reads the closing count, checks it
	// against pickleCount and releases the unpickler, reporting errors; its
	// callers pass the stream, which it does not use (invented name).
	static bool			StartSave( TGStream *stream );
	static bool			FinishSave( TGStream *stream );
	static bool			StartLoad( TGStream *stream );
	static bool			FinishLoad( TGStream *stream );
	// The state of the named module's dictionary, pickled into or read
	// back from the save or load in progress; the unsaved modules are
	// skipped (invented names). Called by UtopiaModule's
	// SaveMissionState and LoadMissionState.
	static bool			SaveModuleState( char *module, TGStream *stream, bool unknownFlag );
	static bool			LoadModuleState( char *module, TGStream *stream );
	// Every saved module's state, for the saved game (UtopiaModule::ProcessSave
	// and ProcessInternalLoad; invented names).
	static bool			SaveModules( TGStream *stream, char *scriptsPath );
	static bool			LoadModules( TGStream *stream );
	// An object pickled into, and read back from, the save or load in
	// progress; a mode of 1 does nothing. Unserialize's error message names
	// it, Serialize is named after it.
	static bool			SerializePython( PyObject *object, TGStream *stream, int mode );
	static PyObject *	UnserializePython( TGStream *stream, int mode );

	// The C++ object of a shadow instance of the SWIG type typeName, into
	// object; returns typeName, or NULL when the instance does not
	// convert or a Python error is pending.
	static char *		Unknown6F98E0( PyObject *instance, void **object, char *typeName );
	// The C++ object of a shadow instance of any SWIG type; NULL when a
	// Python error is pending (invented name).
	static void *		GetObjectPointer( PyObject *instance );
	// Adds the function to App's shadow class as the named method: at once
	// when App is loaded, else when it is (invented name).
	static bool			AddClassMethod( PyCFunction function, char *className, char *methodName );
	// The SWIG pointer string of an address and type name, with the address
	// in hex (invented name).
	static PyObject *	NewPointerString( unsigned int pointer, char *typeName );
	// Adds the method to App's class at once (invented name).
	static bool			InstallClassMethod( PyMethodDef *method, char *className );
	// A new instance of App's shadow class "<className>Ptr" for the object.
	static PyObject *	Unknown6F9920( void *object, char *className );
	// A new SWIG pointer string for the object, with the type part of the
	// pointer string given; NULL when either lacks one.
	static PyObject *	Unknown6F9A90( void *object, PyObject *pointer );
	// Writes the SWIG pointer string of the pointer, "_", its hex digits and
	// the type part given, or "NULL" (unreferenced in retail).
	static void			MakePointerString( char *string, unsigned int pointer, char *type );	// invented name

	// Reports and clears a pending Python error; true when there was one.
	static bool			CheckError( void );									// invented name

	static void			Unknown6F9470( void );
	// Installs the queued class methods once App is loaded (invented name).
	static void			InstallPendingClassMethods( void );

	// The class methods queued for App's shadow classes until App is
	// loaded, and the ones installed; FreeRegistrations deletes both
	// (invented names). Class statics: their exit destructors are guarded.
	static NiTList<PendingClassMethod *>	pendingClassMethods;
	static NiTList<PyMethodDef *>			installedClassMethods;

	static PyObject *	appModule;			// the module App, imported by LoadAppModule
	static PyObject *	appDict;			// its dictionary
	static bool			alwaysReload;		// GetModule reloads modules it finds already loaded; invented name
	static bool			quietErrors;		// CheckError clears errors without reporting them; invented name
	static bool			unknown99ADD6;		// CheckError reports through Unknown6F9470 instead of PyErr_Print
	static int			consoleCount;		// ShowConsole's count of shows; invented name
	static PyObject *	savedStdout;		// borrowed, from SaveStdout; invented name
	static PyObject *	savedStderr;		// borrowed, from SaveStderr; invented name
	static bool			stateFunctionsInstalled;	// set by InstallStateFunctions; invented name
	static PyObject *	pickler;			// the cPickle pickler or unpickler of the save or load in progress; invented name
	static int			moduleObjectCount;	// the number of the module state object being pickled or unpickled; invented name
	static int			pickleCount;		// objects pickled or unpickled so far; invented name
	static long			pickleStart;		// the stream position of the save's link to its end; invented name
	static char **		unsavedModules;		// NULL-terminated names of modules the save skips; invented name
	static char **		unsavedPrefixes;	// NULL-terminated name prefixes of modules the save skips; invented name

	// Files a state function of the shadow instances of the class
	// (invented names). The modules call them at startup.
	static void			AddGetStateFunction( pyGetStateFunction_t function, char *className );
	static void			AddSetStateFunction( pyStateFunction_t function, char *className );
	// Run the state functions filed for the classes the instance belongs
	// to, base classes first: App.SerializeGetState and
	// App.UnserializeSetState call them (invented names).
	static bool			RunGetStateFunctions( PyObject *instance, PyObject **state );
	static bool			RunSetStateFunctions( PyObject *instance, PyObject *state );

	// The state functions filed, oldest first (invented names).
	static NiTList<PyGetStateFunction *>	getStateFunctions;
	static NiTList<PySetStateFunction *>	setStateFunctions;

	// The functions filed while the current object is pickled or
	// unpickled; RunStreamCallbacks runs them in order and empties the
	// list (invented names).
	static NiTList<PyStreamCallback *>	streamCallbacks;
	static bool			RunStreamCallbacks( TGStream *stream );
};

/*
================
PyEmbed::AddGetStateFunction

Evidence: the objects module (0x6EF850) and the creators' unit (0x443080)
expand it.
================
*/
inline void PyEmbed::AddGetStateFunction( pyGetStateFunction_t function, char *className ) {
	PyGetStateFunction *entry = new PyGetStateFunction;
	entry->function = function;
	entry->className = new char[strlen( className ) + 1];
	strcpy( entry->className, className );
	getStateFunctions.AddTail( entry );
}

/*
================
PyEmbed::AddSetStateFunction
================
*/
inline void PyEmbed::AddSetStateFunction( pyStateFunction_t function, char *className ) {
	PySetStateFunction *entry = new PySetStateFunction;
	entry->function = function;
	entry->className = new char[strlen( className ) + 1];
	strcpy( entry->className, className );
	setStateFunctions.AddTail( entry );
}

/*
================
GetTacticalFloat

Calls a function of the Tactical script that returns a float; the default
when the call fails (invented name; where the original kept it is a guess:
the reticle and the multiplayer windows both expand it).
================
*/
inline float GetTacticalFloat( char *function, float defaultValue ) {
	float value;
	if ( PyEmbed::CallFunction( "Tactical.Tactical", function, "f", &value, "()" ) == -1 ) {
		PyErr_Print();
		return defaultValue;
	}
	return value;
}

#endif /* !__PYEMBED_H__ */
