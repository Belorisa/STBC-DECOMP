/*
===============================================================================

	StarSphere.h

	The members of StarSphere that the SWIG wrappers StarSphere_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __STARSPHERE_H__
#define __STARSPHERE_H__

#include "BackdropSphere.h"

class NiCamera;
class TGObject;

// GetObjType's value for a StarSphere (the type its Cast checks).
#define STARSPHERE_TYPE		0x800D

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0058e4e0 ??_GStarSphere@@UAEPAXI@Z @game/StarSphere
// FUNCTION: STBC 0x0058e510 ??1StarSphere@@UAE@XZ @game/StarSphere
class StarSphere : public BackdropSphere {
public:
	// 0xAC bytes: the constructor (0x58E400) runs BackdropSphere's, stores
	// the vtable 0x893C08 and clears 0xA4 and 0xA8 (WarpSet's constructor
	// calls it).
					StarSphere( long id = 0 );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// Backdrop
	virtual bool	VirtualSlot67( FILE *file );

	// BackdropSphere
	virtual void	SetTextureFileName( char *name );
	virtual NiPointer<NiAVObject>	VirtualSlot71( void );

	// Rescales the sphere's texture coordinates to the camera's field of
	// view and the screen width, when the scale has moved by more than 2
	// (0x58EBE0; SetClass::VirtualSlot25 calls it; invented name).
	void	UpdateTextureScale( NiCamera *camera );

	static StarSphere *	Create( void );
	static StarSphere *	Cast( TGObject * );

protected:
	float			textureWidth;				// 0xA4 cleared by the constructor; SetTextureFileName sets it to the width of the texture file; the streams carry it
	float			textureScale;				// 0xA8 cleared by the constructor; the scale UpdateTextureScale last applied (invented name)
};

#endif /* !__STARSPHERE_H__ */
