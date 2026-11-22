/*
===============================================================================

	SetFloatVarEvent.h

	An event that carries a float variable's scope, name and value.

	0x34 bytes. Evidence: the class strings "SetFloatVarEvent",
	"_p_SetFloatVarEvent" and "SetFloatVarEventPtr"; the constructor
	(0x448D60) calls TGEvent's, stores the vtable 0x88B61C (15 slots: the
	destructor, GetObjType, IsTypeOf, the class strings and Copy
	overridden) and clears 0x28-0x30; GetObjType returns 0x8121; the
	destructor (0x448D90) frees both strings; the SWIG wrappers
	SetFloatVarEvent_* name the accessors.

===============================================================================
*/

#ifndef __SETFLOATVAREVENT_H__
#define __SETFLOATVAREVENT_H__

#include "TGEvent.h"

// GetObjType's value for a SetFloatVarEvent.
#define SETFLOATVAREVENT_TYPE	0x8121

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00448d30 ??_GSetFloatVarEvent@@UAEPAXI@Z
class SetFloatVarEvent : public TGEvent {
public:
					SetFloatVarEvent( void );
					SetFloatVarEvent( long id );	// the loader's: the stream fills the members
	virtual			~SetFloatVarEvent( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEvent
	virtual bool	Copy( TGEvent *event );

	char *	GetName( void ) { return name; }
	char *	GetScope( void ) { return scope; }
	float	GetValue( void ) { return value; }
	void	SetValue( float value_ ) { value = value_; }

	static SetFloatVarEvent *	Create( void ) { return new SetFloatVarEvent; }	// the SWIG wrapper SetFloatVarEvent_Create expands it

	void	SetScope( char * );
	void	SetName( char * );

protected:
	char *			scope;			// 0x28 owned; SetScope's text (invented name)
	char *			name;			// 0x2C owned; SetName's text (invented name)
	float			value;			// 0x30 SetFloatVarEvent_GetValue reads it
};

#endif /* !__SETFLOATVAREVENT_H__ */
