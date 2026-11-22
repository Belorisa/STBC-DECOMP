/*
===============================================================================

	VarManagerClass.h

	The game's script variables: float and string variables by scope and
	name, in two sorted arrays of scopes.

	0x2C bytes, allocated from the pools. Evidence: the constructor
	(0x44B1C0) calls TGEventHandlerObject's, clears the two arrays and
	stores the vtable 0x889BA4 (TGEventHandlerObject's 21 slots); the
	destructor (0x44B1F0) frees both arrays. The class factory's unit keeps
	the inline virtual functions (0x43D790-0x43D820): GetObjType returns
	0x811E, the handler table is 0x980120. The SWIG setter g_kVarManager_set
	copies 0x04 to 0x2B.

===============================================================================
*/

#ifndef __VARMANAGERCLASS_H__
#define __VARMANAGERCLASS_H__

#include "TGEventHandlerObject.h"
#include "SortedArray.h"
#include "ScriptVariables.h"

// GetObjType's value for a VarManagerClass.
#define VARMANAGERCLASS_TYPE		0x811E

// The event a SetFloatVarEvent is sent as (the App constant ET_SET_FLOAT_VAR).
#define ET_SET_FLOAT_VAR			0x80002A

class SetFloatVarEvent;

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0043d820 ??_GVarManagerClass@@UAEPAXI@Z @game/VarManagerClass
class VarManagerClass : public TGEventHandlerObject {
public:
					VarManagerClass( void );
					// FUNCTION: STBC 0x0043d760 ??0VarManagerClass@@QAE@J@Z @game/UtopiaModule
					VarManagerClass( long id ) : TGEventHandlerObject( id ) {}
	virtual			~VarManagerClass( void );

	// TGObject
	// FUNCTION: STBC 0x43D790 ?GetObjType@VarManagerClass@@UAEHXZ
	virtual int		GetObjType( void ) { return VARMANAGERCLASS_TYPE; }
	virtual bool	IsTypeOf( int type );
	// The event type of the nth episode event: 0x80C001 and on (invented name).
	int				MakeEpisodeEventType( int n ) { return n + 0x80C001; }
	// FUNCTION: STBC 0x43D7E0 ?GetClassName@VarManagerClass@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "VarManagerClass"; }
	// FUNCTION: STBC 0x43D7F0 ?GetSwigTypeName@VarManagerClass@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_VarManagerClass"; }
	// FUNCTION: STBC 0x43D800 ?GetPythonClassName@VarManagerClass@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "VarManagerClassPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x43D810 ?GetClassHandlers@VarManagerClass@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	// The class's handler methods by name, and in the class's table (invented names).
	static void		RegisterHandlerNames( void );
	static void		RegisterHandlers( void );

	void	SetFloatVariable( char *, char *, float );
	float	GetFloatVariable( char *, char * );
	void	SetStringVariable( char *, char *, char * );
	char *	GetStringVariable( char *, char * );
	void	DeleteAllVariables( void );
	void	DeleteAllScopedVariables( char * );

	// A SetFloatVarEvent sets the variable it names (the handler of ET_SET_FLOAT_VAR).
	void	SetFloatVarHandler( SetFloatVarEvent *event );

	// The class's handler table (invented name; 0x980120). A class static: its
	// exit-time destructor (0x44B160) tests a guard bit first.
	static TGEventHandlerTable	classHandlers;

protected:
	SortedArray<FloatVariableScope, char *>		floatScopes;	// 0x14 by scope name (invented name)
	SortedArray<StringVariableScope, char *>	stringScopes;	// 0x20 by scope name (invented name)

};

/*
================
VarManagerClass::IsTypeOf
================
*/
// FUNCTION: STBC 0x0043d7a0 ?IsTypeOf@VarManagerClass@@UAE_NH@Z @game/VarManagerClass
inline bool VarManagerClass::IsTypeOf( int type ) {
	if ( type == VARMANAGERCLASS_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

// The script variables (the SWIG variable g_kVarManager).
extern VarManagerClass	g_kVarManager;

#endif /* !__VARMANAGERCLASS_H__ */
