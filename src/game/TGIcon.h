/*
===============================================================================

	TGIcon.h

	A UI object that draws one icon of an icon group: the group's name, the
	icon's number in the group, a colour and a texture clamp mode. Layout
	asks the icon manager for a poly in the object's batch and keeps it.

	0x50 bytes, allocated from the pools (the scalar deleting destructor
	0x726730 frees through the pool of that size).

	Evidence: the constructor (0x73D2F0, called by TGIcon_Create with the
	group name, the icon number and the colour) calls TGUIObject's, copies
	the name (0x34), stores the number (0x38), the colour (0x3C, white
	without one), clears the clamp mode (0x4C) and the poly (0x30), and
	stores the vtable 0x897530 (64 slots, none new). GetObjType returns
	0x202. The SWIG wrappers name the members: TGIcon_GetIconPoly reads
	0x30, TGIcon_GetIconGroupName 0x34, TGIcon_GetIconNum 0x38,
	TGIcon_GetColor copies 0x3C, TGIcon_GetClamp and TGIcon_SetClamp
	0x4C (the SWIG constants TGIconManager_CLAMP_*). The type's inline
	members are emitted with the UI module's class factories (0x7266A0 to
	0x726730).

===============================================================================
*/

#ifndef __TGICON_H__
#define __TGICON_H__

#include "TGUIObject.h"
#include "NiColor.h"

class TGObject;
class TGIconPoly;

// GetObjType's value for a TGIcon.
#define TGICON_TYPE				0x202

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00726730 ??_GTGIcon@@UAEPAXI@Z
class TGIcon : public TGUIObject {
public:
	// FUNCTION: STBC 0x00726670 ??0TGIcon@@QAE@J@Z
					TGIcon( long id = 0 ) : TGUIObject( id ) {}
					TGIcon( char *groupName, int iconNum, NiColorA *color = NULL );
	virtual			~TGIcon( void );

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

	// TGUIObject
	virtual void	SetPosition( float x, float y, bool layout = true );
	virtual void	Move( float dx, float dy, bool layout = true );
	virtual void	Layout( void );
	virtual void	VirtualSlot60( void );
	virtual void	VirtualSlot61( NiPoint2 *offset );
	virtual void	VirtualSlot62( TGRect *clipRect );

	static TGIcon *	Create( char *groupName, int iconNum, NiColorA *color = NULL ) { return new TGIcon( groupName, iconNum, color ); }
	static TGIcon *	Cast( TGObject *object );

	void			SetIconNum( int iconNum, bool layout = true );
	int				GetIconNum( void ) { return iconNum; }
	void			SetColor( NiColorA *color );
	void			GetColor( NiColorA *color ) { *color = this->color; }
	float			GetAlpha( void ) { return color.a; }	// invented name
	void			SizeToArtwork( bool layout = true );
	void			SetClamp( int clamp ) { this->clamp = clamp; }
	int				GetClamp( void ) { return clamp; }
	TGIconPoly *	GetIconPoly( void ) { return iconPoly; }
	char *			GetIconGroupName( void ) { return iconGroupName; }
	void			SetIconGroupName( char *groupName, bool layout = true );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	TGIconPoly *	iconPoly;				// 0x30 made by Layout; NULL while the icon is not drawn
	char *			iconGroupName;			// 0x34 owned copy, or NULL
	int				iconNum;				// 0x38 the icon's number in its group
	NiColorA		color;					// 0x3C
	int				clamp;					// 0x4C TGIconManager_CLAMP_*

	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGIcon::GetObjType
================
*/
// FUNCTION: STBC 0x007266a0
inline int TGIcon::GetObjType( void ) {
	return TGICON_TYPE;
}

/*
================
TGIcon::IsTypeOf
================
*/
// FUNCTION: STBC 0x007266b0
inline bool TGIcon::IsTypeOf( int type ) {
	if ( type == TGICON_TYPE ) {
		return true;
	}
	return TGUIObject::IsTypeOf( type );
}

/*
================
TGIcon::GetClassName
================
*/
// FUNCTION: STBC 0x00726700
inline char *TGIcon::GetClassName( void ) {
	return "TGIcon";
}

/*
================
TGIcon::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00726710
inline char *TGIcon::GetSwigTypeName( void ) {
	return "_p_TGIcon";
}

/*
================
TGIcon::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00726720
inline char *TGIcon::GetPythonClassName( void ) {
	return "TGIconPtr";
}

/*
================
TGIcon::GetClassHandlers

The linker kept a copy of this inline emitted outside the icon's unit.
================
*/
// FUNCTION: STBC 0x0053b4e0
inline TGEventHandlerTable *TGIcon::GetClassHandlers( void ) {
	return &classHandlers;
}

#endif /* !__TGICON_H__ */
