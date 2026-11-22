/*
===============================================================================

	BridgeSet.h

	The set of a bridge: a SetClass with the view screen (0x13C) and the
	name of the bridge configuration (0x140).

	Evidence: the constructor 0x665CC0 stores the vtable 0x894830, which
	overrides SetClass's type slots (0x665D00, 0x665D10), stream pair
	(0x665F80, 0x665FF0), fix-ups (0x666060, 0x666090), class names
	(0x665D60 to 0x665D80), handler table (0x665D90) and slot 26 (0x665F20).

===============================================================================
*/

#ifndef __BRIDGESET_H__
#define __BRIDGESET_H__

#include "SetClass.h"

class ViewScreenObject;

// GetObjType's value for a BridgeSet (the type its Cast checks).
#define BRIDGESET_TYPE		0x80D2

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00665da0 ??_GBridgeSet@@UAEPAXI@Z
class BridgeSet : public SetClass {
public:
	virtual			~BridgeSet( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// SetClass
	virtual void	VirtualSlot26( void );		// renders the set through its active camera, the view screen's way

	static BridgeSet *	Cast( SetClass * );
	void	SetViewScreen( ViewScreenObject *, char * );
	void	SetConfig( char * );
	bool	IsSameConfig( char * );

	ViewScreenObject *	GetViewScreen( void ) { return viewScreen; }
	char *			GetConfig( void ) { return config; }

protected:
	ViewScreenObject *	viewScreen;		// 0x13C
	char *			config;				// 0x140

	static TGEventHandlerTable	classHandlers;		// invented name

	// Declared for the SWIG wrappers that call them.
public:
					BridgeSet( long id = 0 );
	static BridgeSet *	Create( void ) { return new BridgeSet; }	// the SWIG wrapper BridgeSet_Create expands it
};

#endif /* !__BRIDGESET_H__ */
