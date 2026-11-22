/*
===============================================================================

	TGFrame.h

	A UI object that draws a frame of icons from an icon group, laid out
	around its bounds by its stretch type.

	0x7C bytes, allocated from the pools (the scalar deleting destructor
	0x725D30 frees through the pool of that size).

	Evidence: the constructor (0x741660) calls TGUIObject's, builds the
	poly array at 0x30 (an NiTArray of one element, growing by one), the
	rectangle at 0x58 and the colour at 0x68, stores the vtable 0x896C10
	(64 slots, none new), copies the group name (0x48), stores the first
	icon number (0x4C), sets the bounds, stores the scale (0x50) and the
	stretch type (0x54), makes the colour white and clears 0x78. The SWIG
	wrappers TGFrame_SetIconGroupName, SetFirstIconNum, SetScale,
	SetStretchType and SetEdgeStretch (0x742D40, 0x742CE0, 0x742D20,
	0x742D00, 0x742DC0) store 0x48, 0x4C, 0x50, 0x54 and 0x78, then lay the
	frame out when asked. Layout (0x7418E0) picks one of three layouts by
	the stretch type. GetObjType returns 0x203. The load constructor and
	the type's inline members are emitted with the UI module's class
	factories (0x725C00 to 0x725D30).

===============================================================================
*/

#ifndef __TGFRAME_H__
#define __TGFRAME_H__

#include "TGUIObject.h"
#include "TGRect.h"
#include "NiColor.h"
#include "NiTArray.h"

class TGObject;
class TGIconPoly;

// GetObjType's value for a TGFrame.
#define TGFRAME_TYPE			0x203

// How the frame's edges and middle cover it: the stretch types (the SWIG
// constants TGFrame_FRAME_*, and TGTextButton_FRAME_* for the button's
// frame mode).
#define FRAME_STRETCH_TO_FIT		0
#define FRAME_WRAP_TEXTURES_TO_FIT	1
#define FRAME_TILE_POLYS_TO_FIT		2

// The icons of a frame, in the icon group's order from firstIconNum: a
// three by three grid, row by row (invented names).
#define FRAME_ICON_TOP_LEFT			0
#define FRAME_ICON_TOP				1
#define FRAME_ICON_TOP_RIGHT		2
#define FRAME_ICON_LEFT				3
#define FRAME_ICON_CENTER			4
#define FRAME_ICON_RIGHT			5
#define FRAME_ICON_BOTTOM_LEFT		6
#define FRAME_ICON_BOTTOM			7
#define FRAME_ICON_BOTTOM_RIGHT		8

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00725d30 ??_GTGFrame@@UAEPAXI@Z
class TGFrame : public TGUIObject {
public:
	// FUNCTION: STBC 0x00725c00 ??0TGFrame@@QAE@J@Z
					TGFrame( long id = 0 ) : TGUIObject( id ) {}
					TGFrame( char *iconGroupName, int firstIconNum, TGRect *bounds, float scale, int stretchType );
	// The script-side constructor (the SWIG wrapper TGFrame_Create): an empty
	// rectangle, unit scale, stretched to fit.
	// FUNCTION: STBC 0x005d1440 ?Create@TGFrame@@SAPAV1@PADH@Z @game/AppWrap
	static TGFrame *	Create( char *iconGroupName, int firstIconNum ) { return new TGFrame( iconGroupName, firstIconNum, &TGRect(), 1.0f, FRAME_STRETCH_TO_FIT ); }
	virtual			~TGFrame( void );

	// TGObject
	// FUNCTION: STBC 0x00725c90 ?GetObjType@TGFrame@@UAEHXZ
	virtual int		GetObjType( void ) { return TGFRAME_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00725cf0 ?GetClassName@TGFrame@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGFrame"; }
	// FUNCTION: STBC 0x00725d00 ?GetSwigTypeName@TGFrame@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGFrame"; }
	// FUNCTION: STBC 0x00725d10 ?GetPythonClassName@TGFrame@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGFramePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00725d20 ?GetClassHandlers@TGFrame@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Layout( void );
	virtual void	VirtualSlot60( void );
	virtual void	VirtualSlot61( NiPoint2 *offset );
	virtual void	VirtualSlot62( TGRect *clipRect );

	static TGFrame *	Cast( TGObject *object );
	void	SetIconGroupName( char *iconGroupName, bool layout );
	void	SetScale( float scale, bool layout );
	void	SetStretchType( int stretchType, bool layout );
	void	SetFirstIconNum( int firstIconNum, bool layout );
	void	SetEdgeStretch( int edgeStretch, bool layout );
	// FUNCTION: STBC 0x0052c620 ?SetColor@TGFrame@@QAEXVNiColorA@@@Z @game/STNumericBar
	void	SetColor( NiColorA color ) { this->color = color; }
	NiColorA &	GetNiColor( void ) { return color; }
	void	SetNiColor( float r, float g, float b, float a ) { color = NiColorA( r, g, b, a ); }

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	int	GetEdgeStretch( void ) { return edgeStretch; }
	// The middle's rectangle (the SWIG wrapper TGFrame_GetInnerRect).
	// FUNCTION: STBC 0x005d1910 ?GetInnerRect@TGFrame@@QAE?AVTGRect@@XZ @game/AppWrap
	TGRect	GetInnerRect( void ) { return innerRect; }

protected:
	// The layouts of the stretch types (invented names).
	void			StretchFrame( void );
	void			WrapFrame( void );
	void			TileFrame( void );
	TGIconPoly *	AddPoly( int iconOffset, TGRect *rect, bool noWrap );	// invented name

	NiTArray<TGIconPoly *>	polys;			// 0x30 the polys drawn for the frame (invented name)
	char *			iconGroupName;			// 0x48 owned copy, or NULL for no frame
	int				firstIconNum;			// 0x4C
	float			scale;					// 0x50 the icons' size, as a multiple of their own
	int				stretchType;			// 0x54 FRAME_*
	TGRect			innerRect;				// 0x58 the middle's rectangle (the SWIG wrapper TGFrame_GetInnerRect); streamed
	NiColorA		color;					// 0x68 white
	int				edgeStretch;			// 0x78

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGFrame::IsTypeOf
================
*/
// FUNCTION: STBC 0x00725ca0
inline bool TGFrame::IsTypeOf( int type ) {
	if ( type == TGFRAME_TYPE ) {
		return true;
	}
	return TGUIObject::IsTypeOf( type );
}

#endif /* !__TGFRAME_H__ */
