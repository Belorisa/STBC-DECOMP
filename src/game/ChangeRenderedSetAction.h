/*
===============================================================================

	ChangeRenderedSetAction.h

	An action that makes a set the rendered one when it plays: the set of the
	name it keeps (CreateFromSet takes the name of the set given), else the
	set that the GetSet function of its script module returns (Create takes
	the module: WarpSequence.py passes the destination module). It completes
	at once.

	0x28 bytes. Evidence: the class strings "ChangeRenderedSetAction",
	"_p_ChangeRenderedSetAction" and "ChangeRenderedSetActionPtr"; the
	constructors (0x401000, 0x401110, 0x4011A0) call TGAction's and store
	the vtable 0x88857C (24 slots: the destructor, GetObjType, IsTypeOf,
	the stream functions, the class strings and Play overridden); GetObjType
	returns 0x80FB; the destructor (0x4011D0) deletes both strings; Play
	(0x401270) times itself as "ChangeRenderedSetAction::Play()" and calls
	the module's "GetSet".

===============================================================================
*/

#ifndef __CHANGERENDEREDSETACTION_H__
#define __CHANGERENDEREDSETACTION_H__

#include "TGAction.h"

// GetObjType's value for a ChangeRenderedSetAction.
#define CHANGERENDEREDSETACTION_TYPE	0x80FB

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
class SetClass;
// FUNCTION: STBC 0x004010e0 ??_GChangeRenderedSetAction@@UAEPAXI@Z
class ChangeRenderedSetAction : public TGAction {
public:
					ChangeRenderedSetAction( long id = 0 );
	virtual			~ChangeRenderedSetAction( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGAction
	virtual void	Play( void );

	void	SetName( char * );

protected:
	char *			setName;			// 0x20 a heap copy of the name of the set to render (invented name)
	char *			moduleName;			// 0x24 a heap copy of the script module whose GetSet gives the set, when no set has the name (invented name)

	// Declared for the SWIG wrappers that call them.
public:
					ChangeRenderedSetAction( char *module );
					ChangeRenderedSetAction( SetClass *set );
	static ChangeRenderedSetAction *	Create( char *module ) { return new ChangeRenderedSetAction( module ); }	// the SWIG wrapper ChangeRenderedSetAction_Create expands it
	static ChangeRenderedSetAction *	CreateFromSet( SetClass *set ) { return new ChangeRenderedSetAction( set ); }	// the SWIG wrapper ChangeRenderedSetAction_CreateFromSet expands it
};

#endif /* !__CHANGERENDEREDSETACTION_H__ */
