/*
===============================================================================

	TGScriptAction.h

	An action that calls a Python function: module.function( action,
	arguments... ). The function returns 0 when it is done, and anything
	else when it will call the action's Completed itself later.

	0x2C bytes.

	Evidence: the constructor (0x6FF6E0) calls TGAction's, stores the vtable
	0x88863C (TGAction's 24 slots), copies the module and function names
	and makes the argument list; the destructor (0x6FF7B0) frees them.
	GetObjType returns 0x70A. The Python function TGScriptAction_Create
	(0x6FF980) builds one from its arguments, and Play (slot 16, the SWIG
	wrapper TGScriptAction_Play) prints "TGScriptAction faulted attempting to
	play %s.%s" when the call fails.

===============================================================================
*/

#ifndef __TGSCRIPTACTION_H__
#define __TGSCRIPTACTION_H__

#include "TGAction.h"

// GetObjType's value for a TGScriptAction.
#define TGSCRIPTACTION_TYPE		0x70A

// FUNCTION: STBC 0x00403430 ??_GTGScriptAction@@UAEPAXI@Z @game/LoadMissionAction
class TGScriptAction : public TGAction {
public:
	// FUNCTION: STBC 0x006fd270 ??0TGScriptAction@@QAE@J@Z
					TGScriptAction( long id = 0 ) : TGAction( id ) { module = NULL; function = NULL; arguments = PyList_New( 0 ); }
					TGScriptAction( char *module, char *function );
	virtual			~TGScriptAction( void );

	// TGObject
	// FUNCTION: STBC 0x004033c0 ?GetObjType@TGScriptAction@@UAEHXZ @game/LoadMissionAction
	virtual int		GetObjType( void ) { return TGSCRIPTACTION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00403400 ?GetClassName@TGScriptAction@@UAEPADXZ @game/LoadMissionAction
	virtual char *	GetClassName( void ) { return "TGScriptAction"; }
	// FUNCTION: STBC 0x00403410 ?GetSwigTypeName@TGScriptAction@@UAEPADXZ @game/LoadMissionAction
	virtual char *	GetSwigTypeName( void ) { return "_p_TGScriptAction"; }
	// FUNCTION: STBC 0x00403420 ?GetPythonClassName@TGScriptAction@@UAEPADXZ @game/LoadMissionAction
	virtual char *	GetPythonClassName( void ) { return "TGScriptActionPtr"; }

	// TGAction
	virtual void	Play( void );

	// Appends an argument for the function; PyList_Append's result.
	int				AddArgument( PyObject *argument );		// invented name

protected:
	char *			module;					// 0x20 copied
	char *			function;				// 0x24 copied
	PyObject *		arguments;				// 0x28 a list; Play puts the action's shadow in front
};

/*
================
TGScriptAction::IsTypeOf
================
*/
// FUNCTION: STBC 0x004033d0 ?IsTypeOf@TGScriptAction@@UAE_NH@Z @game/LoadMissionAction
inline bool TGScriptAction::IsTypeOf( int type ) {
	if ( type == TGSCRIPTACTION_TYPE ) {
		return true;
	}
	return TGAction::IsTypeOf( type );
}

// App's TGScriptAction_Create( module, function, arguments... ): the new
// action's shadow instance, None when the arguments are wrong.
extern "C" PyObject *_wrap_TGScriptAction_Create( PyObject *self, PyObject *args );

#endif /* !__TGSCRIPTACTION_H__ */
