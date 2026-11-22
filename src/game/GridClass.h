/*
===============================================================================

	GridClass.h

	The members of GridClass that the SWIG wrappers GridClass_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __GRIDCLASS_H__
#define __GRIDCLASS_H__

#include "ObjectClass.h"

#include "TGPoint3.h"
class SetClass;
class NiLines;

// GetObjType's value for a GridClass (the type the set's class lookups check).
#define GRIDCLASS_TYPE		0x80E2

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// Evidence: Create (0x498F80) allocates 0x70 bytes from the pool and calls
// the constructor 0x498E20, which stores the vtable 0x88C304 (ObjectClass's
// 67 slots) after ObjectClass's constructor; the deleting destructor
// 0x498F40 frees through the pool of that size.
// FUNCTION: STBC 0x00498f40 ??_GGridClass@@UAEPAXI@Z
class GridClass : public ObjectClass {
public:
					GridClass( long id = 0 );
	virtual			~GridClass( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// BaseObjectClass
	virtual void	Update( float time );
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	int	GetStep( void ) { return step; }
	int	GetLineLength( void ) { return lineLength; }
	void	SetStep( int step_ ) { step = step_; }
	void	SetLineLength( int lineLength_ ) { lineLength = lineLength_; }
	static GridClass *	Create( void );
	void	UpdatePosition( NiPoint3 * );
	static GridClass *	GetObject( SetClass *, char * );
	static int	GetNumClassObjects( SetClass * );
	// the grid with the ID in the set, or in any set when the set is NULL
	// (invented name, as its family's SWIG-named AsteroidField::GetObjectByID)
	static GridClass *	GetObjectByID( SetClass *, long id );
	// the set's grids and their number (invented name, as
	// Backdrop::GetClassObjects)
	static GridClass **	GetClassObjects( SetClass *set, int *count );
	// TODO identity: where the ray from the origin along the direction meets
	// the grid's plane (its rotation's third column is the normal); the
	// placement editor drops waypoints there (0x499590)
	NiPoint3	Unknown499590( NiPoint3 origin, NiPoint3 direction );
	// The same for the scripts (the SWIG wrapper GridClass_GetIntersectionPoint).
	// FUNCTION: STBC 0x006390a0 ?GetIntersectionPoint@GridClass@@QAE?AVTGPoint3@@V2@0@Z @game/AppWrap
	TGPoint3	GetIntersectionPoint( TGPoint3 origin, TGPoint3 direction ) { return Unknown499590( origin, direction ); }
protected:
	// Builds the grid's lines: 125 pairs of lines lineLength long, step
	// apart along both axes, white and untextured, as the object's model
	// (Create and ReadFromStream call it; 0x498FD0). The name is a
	// placeholder.
	void			Unknown498FD0( void );
	// the object's model as NiLines, or NULL (0x4993C0; invented name)
	NiLines *		GetLines( void );

	int				lineLength;				// 0x68 (GridClass_SetLineLength)
	int				step;					// 0x6C (GridClass_SetStep)
	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__GRIDCLASS_H__ */
