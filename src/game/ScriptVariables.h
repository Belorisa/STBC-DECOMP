/*
===============================================================================

	ScriptVariables.h

	The script variables VarManagerClass keeps: a scope per scope name, each
	holding its variables sorted by name. A float scope's variables hold a
	float, a string scope's a copy of a string.

	All names here are invented: no string or SWIG wrapper names the classes.
	Evidence: VarManagerClass::SetFloatVariable (0x44B310) allocates 0x14
	bytes for a scope, constructs it (0x447F50), names it (0x448060), inserts
	it and sets the variable through it (0x4480D0), which allocates 0xC bytes
	per variable (0x447480) and names it (0x447510). The string side is the
	same at 0x44B500, 0x44A400, 0x44A510, 0x44A580, 0x4498C0, 0x449950. The
	vtables 0x88B608, 0x88B610, 0x88B658 and 0x88B660 sit beside
	SetFloatVarEvent's, in the order of the units.

	The elements of a SortedArray compare themselves: CompareKey( name ) and
	Compare( other ), both virtual. The variables have no virtual destructor;
	the scopes do.

===============================================================================
*/

#ifndef __SCRIPTVARIABLES_H__
#define __SCRIPTVARIABLES_H__

#include "SortedArray.h"

// A float variable of a scope (invented name).
class FloatVariable {
public:
					FloatVariable( void );
					~FloatVariable( void );

	virtual int		CompareKey( char *key );
	virtual int		Compare( FloatVariable *other );

	char *			GetName( void ) { return name; }
	void			SetName( char *name_ );

	float			value;		// 0x04
	char *			name;		// 0x08 owned copy
};

// The float variables of one scope (invented name; the scope's name is its
// key).
// FUNCTION: STBC 0x00447fc0 ??_GFloatVariableScope@@UAEPAXI@Z @game/FloatVariableScope
class FloatVariableScope {
public:
					FloatVariableScope( void );
	virtual			~FloatVariableScope( void );

	virtual int		CompareKey( char *key );
	virtual int		Compare( FloatVariableScope *other );

	char *			GetName( void ) { return name; }
	void			SetName( char *name_ );
	// Sets the variable, creating it when the scope has none of the name.
	void			SetVariable( char *variableName, float variableValue );
	// The variable's value; 0 when the scope has none of the name.
	float			GetVariable( char *variableName );

	SortedArray<FloatVariable, char *>	variables;	// 0x04
	char *			name;		// 0x10 owned copy
};

// A string variable of a scope (invented name).
class StringVariable {
public:
					StringVariable( void );
					~StringVariable( void );

	virtual int		CompareKey( char *key );
	virtual int		Compare( StringVariable *other );

	char *			GetName( void ) { return name; }
	void			SetName( char *name_ );
	void			SetValue( char *value_ );

	char *			value;		// 0x04 owned copy
	char *			name;		// 0x08 owned copy
};

// The string variables of one scope (invented name).
// FUNCTION: STBC 0x0044a470 ??_GStringVariableScope@@UAEPAXI@Z @game/StringVariableScope
class StringVariableScope {
public:
					StringVariableScope( void );
	virtual			~StringVariableScope( void );

	virtual int		CompareKey( char *key );
	virtual int		Compare( StringVariableScope *other );

	char *			GetName( void ) { return name; }
	void			SetName( char *name_ );
	// Sets the variable, creating it when the scope has none of the name.
	void			SetVariable( char *variableName, char *variableValue );
	// The variable's value; NULL when the scope has none of the name.
	char *			GetVariable( char *variableName );

	SortedArray<StringVariable, char *>	variables;	// 0x04
	char *			name;		// 0x10 owned copy
};

#endif /* !__SCRIPTVARIABLES_H__ */
