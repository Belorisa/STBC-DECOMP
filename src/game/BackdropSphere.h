/*
===============================================================================

	BackdropSphere.h

	The members of BackdropSphere that the SWIG wrappers BackdropSphere_* call. Only what the
	wrappers prove is declared: no data members and no virtual functions yet.

===============================================================================
*/

#ifndef __BACKDROPSPHERE_H__
#define __BACKDROPSPHERE_H__

#include "Backdrop.h"
#include "NiPointer.h"
#include "renderer/NiImage.h"

class SetClass;
class TGObject;

// GetObjType's value for a BackdropSphere (the type its Cast checks).
#define BACKDROPSPHERE_TYPE		0x800C

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0058d3c0 ??_GBackdropSphere@@UAEPAXI@Z @game/BackdropSphere
class BackdropSphere : public Backdrop {
public:
	// The constructor (0x58D2A0) runs Backdrop's, stores the vtable 0x893AE8
	// and sets the defaults below.
					BackdropSphere( long id = 0 );
	virtual			~BackdropSphere( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// BaseObjectClass
	virtual void	AddToSet( SetClass *set );
	virtual void	RemoveFromSet( SetClass *set );

	// Backdrop's slots 67 to 69, overridden
	virtual bool	VirtualSlot67( FILE *file );
	virtual bool	VirtualSlot68( FILE *file );
	virtual bool	VirtualSlot69( FILE *file );

	// BackdropSphere's own slot (the SWIG wrapper
	// BackdropSphere_SetTextureFileName calls slot 70).
	virtual void	SetTextureFileName( char *name );
	virtual NiPointer<NiAVObject>	VirtualSlot71( void );		// makes the sphere's node

	static BackdropSphere *	Create( void );
	static BackdropSphere *	Cast( TGObject * );
	void	SetHorizontalSpan( float );
	void	SetVerticalSpan( float );
	void	SetTextureHTile( float );
	void	SetTextureVTile( float );
	void	SetDetailTextureFileName( char * );
	void	SetDetailTextureHTile( float );
	void	SetDetailTextureVTile( float );
	void	SetDecimate( bool );
	void	Rebuild( void );
	void	SetSphereRadius( float );
	void	SetTargetPolyCount( int );
	static BackdropSphere *	GetObject( SetClass *set, char *name );
	static BackdropSphere *	GetObjectByID( SetClass *set, long id );		// invented name
	static int	GetNumClassObjects( SetClass *set );
	void	SetAlphaBlendModes( int, int );

	// The getters, out of line beside the setters (0x58D4C0 to 0x58D560;
	// the background editor calls them; names after the setters).
	float	GetHorizontalSpan( void );
	float	GetVerticalSpan( void );
	float	GetTextureHTile( void );
	float	GetTextureVTile( void );
	float	GetDetailTextureHTile( void );
	float	GetDetailTextureVTile( void );
	int		GetTargetPolyCount( void );
	float	GetSphereRadius( void );
	char *	GetTextureFileName( void );			// invented names
	char *	GetDetailTextureFileName( void );
	bool	GetDecimate( void );

protected:
	// How many vertices the mesh has across and down: the texture's own size
	// when decimating, else from the polygon count at the texture's aspect
	// (0x58D6A0; slot 71 calls it; invented name).
	void	GetGridSize( int *across, int *down );

	// Evidence: the constructor 0x58D2A0 (after Backdrop's, which clears the
	// word at 0x68) and the setters; the allocation is 0xA4 bytes.
	NiPointer<NiImage>	image;			// 0x6C the texture, from g_kImageManager by textureFileName; Rebuild does nothing while it is NULL
	char *			textureFileName;	// 0x70 owned; the sphere's script lines and the streams carry it
	char *			detailTextureFileName;	// 0x74 owned; nothing sets it but the stream (SetDetailTextureFileName does nothing)
	float			horizontalSpan;		// 0x78 0.25 at first, up to 1 (invented name)
	float			verticalSpan;		// 0x7C 0.5 at first, up to 1 (invented name)
	float			detailTextureHTile;	// 0x80 10 at first
	float			detailTextureVTile;	// 0x84 10 at first
	float			textureHTile;		// 0x88 1 at first
	float			textureVTile;		// 0x8C 1 at first
	float			sphereRadius;		// 0x90 300 at first
	int				srcBlendMode;			// 0x94 the alpha property's source mode; SetAlphaBlendModes sets it (invented name)
	int				destBlendMode;			// 0x98 its destination mode (invented name)
	bool			decimate;			// 0x9C false at first (SetDecimate does nothing)
	int				targetPolyCount;	// 0xA0 256 at first
};

/*
================
BackdropSphere::GetObjType
================
*/
// FUNCTION: STBC 0x0058d320
inline int BackdropSphere::GetObjType( void ) {
	return BACKDROPSPHERE_TYPE;
}

/*
================
BackdropSphere::IsTypeOf
================
*/
// FUNCTION: STBC 0x0058d330
inline bool BackdropSphere::IsTypeOf( int type ) {
	if ( type == BACKDROPSPHERE_TYPE ) {
		return true;
	}
	return Backdrop::IsTypeOf( type );
}

#endif /* !__BACKDROPSPHERE_H__ */
