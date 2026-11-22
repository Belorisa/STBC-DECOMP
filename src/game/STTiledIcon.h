/*
===============================================================================

	STTiledIcon.h

	An icon whose art can repeat across it instead of stretching, per
	axis: a tiling axis repeats the art either as often as the icon's
	size holds the art's own size (tile size) or a set number of times
	(the tile number).

	0x58 bytes (its scalar deleting destructor frees 0x58).

	Evidence: the constructor (0x53A700) builds the icon, stores the
	vtable 0x891644 (TGIcon's 64 slots) and sets both tile numbers (0x50,
	0x54) to 1. GetObjType returns 0x80FA; the class name is
	"STTiledIcon". The SWIG wrappers STTiledIcon_* name IsTiling and
	SetTiling (flag bits 31 and 30), IsTileSize and SetTileSize (bits 29
	and 28) and GetTileNum and SetTileNum, each by axis (0 horizontal,
	1 vertical).

===============================================================================
*/

#ifndef __STTILEDICON_H__
#define __STTILEDICON_H__

#include "TGIcon.h"

class TGObject;

// GetObjType's value for an STTiledIcon.
#define STTILEDICON_TYPE		0x80FA

// The icon's own state flags, above the TGUIO_* flags (invented names).
#define STTIF_TILE_SIZE_Y		0x10000000	// IsTileSize( 1 )
#define STTIF_TILE_SIZE_X		0x20000000	// IsTileSize( 0 )
#define STTIF_TILING_Y			0x40000000	// IsTiling( 1 )
#define STTIF_TILING_X			0x80000000	// IsTiling( 0 )

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0053a7d0 ??_GSTTiledIcon@@UAEPAXI@Z
class STTiledIcon : public TGIcon {
public:
	// The stream loader's: the stream fills the members (InterfaceModule's creator).
	// FUNCTION: STBC 0x00558330 ??0STTiledIcon@@QAE@J@Z @game/InterfaceModule
					STTiledIcon( long id ) : TGIcon( id ) {}
					STTiledIcon( char *groupName, int iconNum, NiColorA *color = NULL );
	static STTiledIcon *	Create( char *groupName, int iconNum, NiColorA *color = NULL ) { return new STTiledIcon( groupName, iconNum, color ); }	// the SWIG wrapper STTiledIcon_Create expands it
	virtual			~STTiledIcon( void );

	// TGObject
	// FUNCTION: STBC 0x0053a730 ?GetObjType@STTiledIcon@@UAEHXZ
	virtual int		GetObjType( void ) { return STTILEDICON_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0053a7a0 ?GetClassName@STTiledIcon@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STTiledIcon"; }
	// FUNCTION: STBC 0x0053a7b0 ?GetSwigTypeName@STTiledIcon@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STTiledIcon"; }
	// FUNCTION: STBC 0x0053a7c0 ?GetPythonClassName@STTiledIcon@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STTiledIconPtr"; }

	// TGUIObject: the poly's texture coordinates repeat the art on the
	// tiling axes.
	virtual void	Layout( void );

	static STTiledIcon *	Cast( TGObject *object );

	bool			IsTiling( int axis );
	bool			IsTileSize( int axis );
	void			SetTiling( int axis, bool tiling );
	void			SetTileSize( int axis, bool tileSize );
	void			SetTileNum( int axis, float tileNum );
	float			GetTileNum( int axis );

protected:
	float			tileNumX;				// 0x50 the times the art repeats across, without tile size
	float			tileNumY;				// 0x54
};

/*
================
STTiledIcon::IsTypeOf
================
*/
// FUNCTION: STBC 0x0053a740
inline bool STTiledIcon::IsTypeOf( int type ) {
	if ( type == STTILEDICON_TYPE ) {
		return true;
	}
	return TGIcon::IsTypeOf( type );
}

#endif /* !__STTILEDICON_H__ */
