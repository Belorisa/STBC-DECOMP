/*
===============================================================================

	STTargetMenuSubPane.h

	The sub pane of the tactical target list (STTargetMenu builds its menu
	on one), 0x54 bytes. When it gets the focus with no child focused or
	chosen, its first menu takes the focus and is highlighted.

	Evidence: the constructors (0x539AF0, 0x539BF0) call STSubPane's and
	store the vtable 0x8914E4 (STSubPane's 88 slots; the destructor, the
	type, name, class handler and stream slots and GotFocus overridden);
	GetObjType returns 0x80EB; the class strings are "STTargetMenuSubPane",
	"_p_STTargetMenuSubPane" and "STTargetMenuSubPanePtr". Retail keeps the
	class in a unit of its own after STTargetMenu's (its handler table at
	0x989E98 heads that unit's .bss, 0x989E58-0x989F98); the file name is
	invented.

===============================================================================
*/

#ifndef __STTARGETMENUSUBPANE_H__
#define __STTARGETMENUSUBPANE_H__

#include "STSubPane.h"

class TGObject;
class TGStream;

// GetObjType's value for an STTargetMenuSubPane.
#define STTARGETMENUSUBPANE_TYPE	0x80EB

// FUNCTION: STBC 0x00539bc0 ??_GSTTargetMenuSubPane@@UAEPAXI@Z
class STTargetMenuSubPane : public STSubPane {
public:
	// The class's handler table, for the module that links the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

					STTargetMenuSubPane( float vertSpacing, bool expandToFillParent );
					STTargetMenuSubPane( long id, bool expandToFillParent );
					// FUNCTION: STBC 0x00539c10 ??1STTargetMenuSubPane@@UAE@XZ
	virtual			~STTargetMenuSubPane( void ) {}

	// TGObject
	// FUNCTION: STBC 0x00539b10 ?GetObjType@STTargetMenuSubPane@@UAEHXZ
	virtual int		GetObjType( void ) { return STTARGETMENUSUBPANE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00539b80 ?GetClassName@STTargetMenuSubPane@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "STTargetMenuSubPane"; }
	// FUNCTION: STBC 0x00539b90 ?GetSwigTypeName@STTargetMenuSubPane@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_STTargetMenuSubPane"; }
	// FUNCTION: STBC 0x00539ba0 ?GetPythonClassName@STTargetMenuSubPane@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "STTargetMenuSubPanePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00539bb0 ?GetClassHandlers@STTargetMenuSubPane@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	GotFocus( bool layout = true );

	static STTargetMenuSubPane *	Cast( TGObject *object );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
STTargetMenuSubPane::IsTypeOf
================
*/
// FUNCTION: STBC 0x00539b20 ?IsTypeOf@STTargetMenuSubPane@@UAE_NH@Z
inline bool STTargetMenuSubPane::IsTypeOf( int type ) {
	if ( type == STTARGETMENUSUBPANE_TYPE ) {
		return true;
	}
	return STSubPane::IsTypeOf( type );
}

#endif /* !__STTARGETMENUSUBPANE_H__ */
