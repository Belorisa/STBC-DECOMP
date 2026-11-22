/*
===============================================================================

	ConditionScript.h

	A condition whose status a Python script instance sets: the instance
	is made by ConditionScript_Create from a module and class name, and is
	told through its Activate and Deactivate methods, when it has them,
	as the condition is activated and deactivated. Conditions written in
	C++ file a creator under their module and class name, which
	ConditionScript_Create uses in place of a script.

	0x20 bytes, allocated from the pools.

	Evidence: the constructor (0x47BD80) calls TGCondition's, clears the
	instance and the two flags and stores the vtable 0x88B2A8 (15 slots:
	TGCondition's, with 4 and 5 for the stream and 13 and 14 for
	SetActive and SetInactive); the destructor (0x47BDB0) releases the
	instance. GetObjType returns 0x8037, the type Cast (0x47BD50) checks;
	the class strings are "ConditionScript", "_p_ConditionScript" and
	"ConditionScriptPtr". 0x47BEE0 holds the instance and looks up
	"Activate" and "Deactivate". App's method table names
	ConditionScript_Create (0x47C020), whose error text gives its
	arguments; the shadow class's Python method "GetConditionScript"
	(0x47C2D0) returns the instance. ConditionalAI hands the instance its
	external functions ("RegisterExternalFunctions") and logs its
	"GetStatusInfo".

	The inline virtuals of the class (0x442000 to 0x442070) are the copies
	of another unit.

===============================================================================
*/

#ifndef __CONDITIONSCRIPT_H__
#define __CONDITIONSCRIPT_H__

#include <string.h>

#include "Python.h"
#include "TGCondition.h"

class TGObject;

// GetObjType's value for a ConditionScript.
#define CONDITIONSCRIPT_TYPE	0x8037

// Makes a C++ condition from ConditionScript_Create's arguments; NULL on
// failure (invented name).
typedef TGObject *( *conditionCreateFunction_t )( PyObject *args );

// A condition class written in C++, filed under the module and class name
// a script would give: init is called once before the first create. The
// name is invented.
struct ConditionScriptCreator {
	char *					module;			// heap copy
	char *					className;		// heap copy
	conditionCreateFunction_t	create;
	bool					initialized;	// init has succeeded
	bool					( *init )( void );
	ConditionScriptCreator *	next;
};

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00442070 ??_GConditionScript@@UAEPAXI@Z
class ConditionScript : public TGCondition {
public:
	// FUNCTION: STBC 0x00441fe0 ??0ConditionScript@@QAE@J@Z @game/UtopiaModule
					ConditionScript( long id ) : TGCondition( id ) {}	// the loader's: the stream fills the members
					ConditionScript( void );
	virtual			~ConditionScript( void );

	// TGObject
	// FUNCTION: STBC 0x00442000 ?GetObjType@ConditionScript@@UAEHXZ
	virtual int		GetObjType( void ) { return CONDITIONSCRIPT_TYPE; }
	// FUNCTION: STBC 0x00442010 ?IsTypeOf@ConditionScript@@UAE_NH@Z
	virtual bool	IsTypeOf( int type ) { if ( type == CONDITIONSCRIPT_TYPE ) { return true; } return TGCondition::IsTypeOf( type ); }
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00442040 ?GetClassName@ConditionScript@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "ConditionScript"; }
	// FUNCTION: STBC 0x00442050 ?GetSwigTypeName@ConditionScript@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_ConditionScript"; }
	// FUNCTION: STBC 0x00442060 ?GetPythonClassName@ConditionScript@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "ConditionScriptPtr"; }

	// TGCondition
	virtual void	SetActive( void );
	virtual void	SetInactive( void );

	static ConditionScript *	Cast( TGObject * );

	// Holds a reference to the instance, and notes which of the
	// activation methods it has (invented name).
	void			SetScriptInstance( PyObject *instance );
	PyObject *		GetScriptInstance( void ) { return scriptInstance; }	// invented name

	// The create function filed under the names, after its init; NULL
	// when none is filed or the init fails (invented name). The creators'
	// registrations (0x48B5C0) inline it too.
	static conditionCreateFunction_t	GetCreateFunction( char *module, char *className );
	// Files a C++ condition that needs no init (invented name).
	static void		RegisterCreateFunction( char *module, char *className, conditionCreateFunction_t create );

	// The C++ conditions, newest first (invented name).
	static ConditionScriptCreator *	creators;

protected:
	PyObject *		scriptInstance;		// 0x18 a reference held
	bool			hasActivate;		// 0x1C the instance has Activate; cleared when it fails
	bool			hasDeactivate;		// 0x1D the instance has Deactivate; cleared when it fails
};

/*
================
ConditionScript::GetCreateFunction
================
*/
inline conditionCreateFunction_t ConditionScript::GetCreateFunction( char *module, char *className ) {
	for ( ConditionScriptCreator *creator = creators; creator != NULL; creator = creator->next ) {
		if ( strcmp( creator->className, className ) == 0 && strcmp( creator->module, module ) == 0 ) {
			if ( !creator->initialized ) {
				if ( !creator->init() ) {
					return NULL;
				}
				creator->initialized = true;
			}
			return creator->create;
		}
	}
	return NULL;
}

/*
================
ConditionScript::RegisterCreateFunction

Files a C++ condition under the module and class name a script would
give, unless one is filed; it needs no init (invented name).

Evidence: the creators' registrations (0x48B5C0) run this body inline at
startup.
================
*/
inline void ConditionScript::RegisterCreateFunction( char *module, char *className, conditionCreateFunction_t create ) {
	if ( GetCreateFunction( module, className ) != NULL ) {
		return;
	}
	ConditionScriptCreator *creator = new ConditionScriptCreator;
	creator->module = new char[strlen( module ) + 1];
	strcpy( creator->module, module );
	creator->className = new char[strlen( className ) + 1];
	strcpy( creator->className, className );
	creator->create = create;
	creator->initialized = true;
	creator->init = NULL;
	creator->next = creators;
	creators = creator;
}

// Files a C++ condition when the program starts (invented name).
class ConditionScriptCreatorRegistration {
public:
					ConditionScriptCreatorRegistration( char *module, char *className, conditionCreateFunction_t create ) {
						ConditionScript::RegisterCreateFunction( module, className, create );
					}
};

// App's ConditionScript_Create( sModule, sClass, ... ): a C++ condition
// filed under the names, else a new ConditionScript whose instance is the
// class called with the condition's shadow instance and the further
// arguments. The shadow instance; NULL on failure.
extern "C" PyObject *	_wrap_ConditionScript_Create( PyObject *self, PyObject *args );
// The shadow class's GetConditionScript( condition ): the script
// instance, or None.
PyObject *	ConditionScript_GetConditionScript( PyObject *self, PyObject *args );

#endif /* !__CONDITIONSCRIPT_H__ */
