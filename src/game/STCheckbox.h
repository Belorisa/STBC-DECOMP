/*
===============================================================================

	STCheckbox.h

	The checkbox an STButton shows in place of its left end cap when it can
	be chosen: a box icon and, over it, a checkmark icon drawn in the "on"
	colour while checked and in the "off" colour otherwise.

	0x70 bytes (its scalar deleting destructor frees 0x70).

	Evidence: the constructor (0x51C090, called by STButton's slot 90 with
	the left end cap's size) builds the pane, constructs the two colours,
	stores the vtable 0x88FB48 (TGPane's 82 slots), takes
	g_kSTButtonCheckmarkOn and g_kSTButtonCheckmarkOff and adds the
	checkmark (icon 0xBBD) and the box (icon 0xBBA in g_kSTMenu1NormalBase)
	of the LCARS group. GetObjType returns 0x80F6; GetClassName gives the
	class name, GetSwigTypeName and GetPythonClassName the SWIG names. The
	class has no SWIG wrappers: the member names are invented.

===============================================================================
*/

#ifndef __STCHECKBOX_H__
#define __STCHECKBOX_H__

#include "TGPane.h"
#include "NiColor.h"

class TGObject;

// GetObjType's value for an STCheckbox.
#define STCHECKBOX_TYPE			0x80F6

// The children, in the order the checkbox adds them (invented names).
#define STCHECKBOX_CHECKMARK		0
#define STCHECKBOX_BOX				1

// The icon numbers in the LCARS icon group: the box is the art of a
// button's left end cap.
#define STCHECKBOX_ICON_CHECKMARK	0xBBD
#define STCHECKBOX_ICON_BOX			0xBBA

// FUNCTION: STBC 0x0051c2c0 ??_GSTCheckbox@@UAEPAXI@Z
class STCheckbox : public TGPane {
public:
					STCheckbox( long id );
					STCheckbox( float width, float height );
	virtual			~STCheckbox( void );

	// TGObject
	// FUNCTION: STBC 0x0051c220 ?GetObjType@STCheckbox@@UAEHXZ
	virtual int		GetObjType( void ) { return STCHECKBOX_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x0051c290 ?GetClassName@STCheckbox@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STCheckbox"; }
	// FUNCTION: STBC 0x0051c2a0 ?GetSwigTypeName@STCheckbox@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STCheckbox"; }
	// FUNCTION: STBC 0x0051c2b0 ?GetPythonClassName@STCheckbox@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STCheckboxPtr"; }

	// TGUIObject
	virtual void	Resize( float width, float height, bool layout = true );

	static STCheckbox *	Cast( TGObject *object );

	// Shows the checkmark in the "on" or "off" colour.
	void			SetChecked( bool checked );
	// Colours the box.
	void			SetBoxColor( NiColorA *color );
	void			SetOnColor( NiColorA *color );
	void			SetOffColor( NiColorA *color );

protected:
	bool			checked;				// 0x4C
	NiColorA		onColor;				// 0x50 the checkmark while checked
	NiColorA		offColor;				// 0x60 the checkmark otherwise
};

/*
================
STCheckbox::IsTypeOf
================
*/
// FUNCTION: STBC 0x0051c230
inline bool STCheckbox::IsTypeOf( int type ) {
	if ( type == STCHECKBOX_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__STCHECKBOX_H__ */
