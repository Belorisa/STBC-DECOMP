/*
===============================================================================

	TGPrompt.h

	The console's input line: a paragraph whose string and cursor exclude
	the prompt in front of the text.

	0x94 bytes, the size of a TGParagraph: it adds no members (the scalar
	deleting destructor 0x726600 frees through the pool of that size).

	Evidence: the vtable 0x8973B8 (94 slots, none new) overrides the type
	members and TGParagraph's slots 82 (GetString, 0x737090), 90
	(SetCursorPosition, 0x737070) and 91 (GetCString, 0x7370B0); the
	functions are in TGConsole's unit, whose handler names include
	"TGPrompt::KeyboardHandler". GetObjType returns 0x20A. The load
	constructor, the implicit destructor and the type's inline members are emitted
	with the UI module's class factories (0x7264D0 to 0x726630).

===============================================================================
*/

#ifndef __TGPROMPT_H__
#define __TGPROMPT_H__

#include "TGParagraph.h"

class TGKeyboardEvent;

// GetObjType's value for a TGPrompt.
#define TGPROMPT_TYPE			0x20A

// FUNCTION: STBC 0x00726630 ??1TGPrompt@@UAE@XZ
// FUNCTION: STBC 0x00726600 ??_GTGPrompt@@UAEPAXI@Z
class TGPrompt : public TGParagraph {
public:
	// FUNCTION: STBC 0x007264d0 ??0TGPrompt@@QAE@J@Z
					TGPrompt( long id = 0 ) : TGParagraph( id ) {}
					TGPrompt( float width, char *fontName, float fontSize );

	// TGObject
	// FUNCTION: STBC 0x00726550 ?GetObjType@TGPrompt@@UAEHXZ
	virtual int		GetObjType( void ) { return TGPROMPT_TYPE; }
	virtual bool	IsTypeOf( int type );
	// FUNCTION: STBC 0x007265c0 ?GetClassName@TGPrompt@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGPrompt"; }
	// FUNCTION: STBC 0x007265d0 ?GetSwigTypeName@TGPrompt@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGPrompt"; }
	// FUNCTION: STBC 0x007265e0 ?GetPythonClassName@TGPrompt@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGPromptPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x007265f0 ?GetClassHandlers@TGPrompt@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGParagraph
	virtual void	GetString( TGString *string );
	virtual void	SetCursorPosition( int position );
	virtual char *	GetCString( void );

	void			KeyboardHandler( TGKeyboardEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

protected:
	static TGEventHandlerTable	classHandlers;		// invented name
};

/*
================
TGPrompt::IsTypeOf
================
*/
// FUNCTION: STBC 0x00726560
inline bool TGPrompt::IsTypeOf( int type ) {
	if ( type == TGPROMPT_TYPE ) {
		return true;
	}
	return TGParagraph::IsTypeOf( type );
}

#endif /* !__TGPROMPT_H__ */
