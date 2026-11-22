/*
===============================================================================

	TGConsole.h

	The Python console pane: it evaluates strings in Python and shows the
	lines of output.

	0x70 bytes, allocated from the pools (the scalar deleting destructor
	0x726460 frees through the pool of that size).

	Evidence: the constructor (0x7366A0) builds the pane, a list of C
	strings at 0x4C (an NiTList with two items per block), stores the
	vtable 0x897270 (82 slots, none new) and clears 0x64 to 0x6C; the
	destructor (0x736710) frees every string of the list. The stream slots
	write the lines, then the font name (0x68) and size (0x6C). The vtable
	overrides the type members, WriteToStream and ReadFromStream (0x737140,
	0x7371A0), Layout (0x736DF0) and slot 59 (0x736F10). The handler is
	named by the string "TGConsole::KeyboardHandler". 0x7365D0 points
	Python's sys.stdout and sys.stderr at a cStringIO.StringIO object.
	GetObjType returns 0x205. The load constructor and the type's inline
	members are emitted with the UI module's class factories (0x726370 to
	0x726460).

===============================================================================
*/

#ifndef __TGCONSOLE_H__
#define __TGCONSOLE_H__

#include "TGPane.h"
#include "NiTList.h"

class TGObject;
class TGKeyboardEvent;
class TGParagraph;

// GetObjType's value for a TGConsole.
#define TGCONSOLE_TYPE			0x205

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00726460 ??_GTGConsole@@UAEPAXI@Z
class TGConsole : public TGPane {
public:
	// FUNCTION: STBC 0x00726370 ??0TGConsole@@QAE@J@Z
					TGConsole( long id ) : TGPane( id ), lines( 2 ) {}
					TGConsole( void );
	virtual			~TGConsole( void );

	// TGObject
	// FUNCTION: STBC 0x007263b0 ?GetObjType@TGConsole@@UAEHXZ
	virtual int		GetObjType( void ) { return TGCONSOLE_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// FUNCTION: STBC 0x00726420 ?GetClassName@TGConsole@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGConsole"; }
	// FUNCTION: STBC 0x00726430 ?GetSwigTypeName@TGConsole@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGConsole"; }
	// FUNCTION: STBC 0x00726440 ?GetPythonClassName@TGConsole@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGConsolePtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x00726450 ?GetClassHandlers@TGConsole@@UAEPAVTGEventHandlerTable@@XZ
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// TGUIObject
	virtual void	Layout( void );
	virtual void	VirtualSlot59( void );

	static char *	GetPyString( void );
	static TGConsole *	Cast( TGObject * );
	bool	EvalString( char * );
	void	AddConsoleString( char * );
	void	SetConsoleFont( char *, float );

	// Points Python's standard output and error at a string buffer
	// (0x7365D0, invented name).
	static void		RedirectOutput( void );
	// Empty (0x736660; invented name): the UI module calls it as it ends.
	static void		Unknown736660( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			KeyboardHandler( TGKeyboardEvent *event );

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// The gap of the lines from the console's left edge, and of the
	// prompt's from the right.
	static void		SetTextIndent( float indent ) { textIndent = indent; }
	static float	GetTextIndent( void ) { return textIndent; }

protected:
	void			AddLine( TGParagraph *line );		// invented name
	void			KeepInView( void );					// invented name
	bool			Execute( TGParagraph *prompt );		// invented name
	void			ShowOutput( void );					// invented name

	NiTList<char *>	lines;					// 0x4C the commands run, owned copies (invented name)
	NiTListIterator	historyPos;				// 0x64 the command the up and down keys show, or NULL (invented name)
	char *			fontName;				// 0x68 SetConsoleFont's arguments (invented names)
	float			fontSize;				// 0x6C

	static TGEventHandlerTable	classHandlers;		// invented name
	static float	textIndent;
};

/*
================
TGConsole::IsTypeOf
================
*/
// FUNCTION: STBC 0x007263c0
inline bool TGConsole::IsTypeOf( int type ) {
	if ( type == TGCONSOLE_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

#endif /* !__TGCONSOLE_H__ */
