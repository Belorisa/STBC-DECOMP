/*
===============================================================================

	WarpSet.h

	The members of WarpSet that the SWIG wrappers WarpSet_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __WARPSET_H__
#define __WARPSET_H__

#include "SetClass.h"

class CameraObjectClass;
class TGObject;
class WarpTrail;

// GetObjType's value for a WarpSet (the type its Cast checks).
#define WARPSET_TYPE		0x80D3

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// 0x148 bytes. Evidence: the constructor (0x4D7790) calls SetClass's,
// stores the vtable 0x88CDF8, clears 0x140 and calls SetNumTrails with its
// argument, or the default count at 0x8E0E74 when it is 0, then puts a star
// sphere in the set; WarpSequence::GetWarpSet (0x4D6390) allocates 0x148
// bytes for it. The stream loader's creator in UtopiaModule's unit
// (0x4417A8) calls the ID constructor (0x4D7A00). The vtable keeps
// SetClass's 31 slots and replaces the destructor, the stream functions,
// the class strings, GetClassHandlers, AddObjectToSet, RemoveObjectFromSet
// and Update.
// FUNCTION: STBC 0x004d79d0 ??_GWarpSet@@UAEPAXI@Z @game/WarpSet
class WarpSet : public SetClass {
public:
	virtual			~WarpSet( void );

	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );
	virtual TGEventHandlerTable *	GetClassHandlers( void );
	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	virtual bool	AddObjectToSet( ObjectClass *object, char *name_ );
	virtual ObjectClass *	RemoveObjectFromSet( long id );
	virtual void	Update( float time );

					WarpSet( int numTrails = 0 );
					WarpSet( long id );
	int	GetNumTrails( void ) { return numTrails; }
	void	SetForceUpdate( bool forceUpdate_ ) { forceUpdate = forceUpdate_; }

	static WarpSet *	Create( void );
	static WarpSet *	Cast( TGObject * );
	void	SetNumTrails( int count, bool anywhere );

protected:
	// Puts the trail at a random point of the tunnel: anywhere along it, or
	// in the stretch ahead where the trails come from (invented name).
	void			PlaceTrail( int index, bool anywhere );
	// Whether the trail is still on this side of the tunnel's end (invented name).
	bool			IsTrailInTunnel( int index );
	// The camera that sees the set: the bridge's viewscreen camera when it
	// is in the set, or the set's active camera while the set is the one
	// rendered; NULL when neither (invented name).
	CameraObjectClass *	GetViewingCamera( void );
	// Takes the trails out of the scene and frees their array (invented name).
	void			DeleteTrails( void );

	int				numTrails;				// 0x13C
	WarpTrail **	trails;					// 0x140 numTrails trails under the effect root (invented name)
	bool			forceUpdate;			// 0x144

	static int		defaultNumTrails;		// the trails of a set made with none (invented name)

	static TGEventHandlerTable	classHandlers;		// invented name
};

#endif /* !__WARPSET_H__ */
