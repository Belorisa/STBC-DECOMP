/*
===============================================================================

	TGParagraph.h

	A pane that shows a string in a font group, word wrapped to a width,
	and can be edited at a cursor.

	0x94 bytes, allocated from the pools (TGRootPane::ShowFPS allocates
	0x94 bytes for one).

	Evidence: the constructors (0x731C50 with a C string, 0x731BB0 with a
	TGString) call TGPane( 0 ), build the string (0x50), the colour (0x6C)
	and the ignored characters (0x80), store the vtable 0x896E60 (94 slots;
	82 to 93 are new) and hand the arguments to Init (0x731D20). Init sets
	the maximum length (0x90) to 0x7FFFFFFF, copies the string, sizes a
	pool block of a TGIconPoly per character (0x7C) from its length (0x64)
	and puts the cursor (0x68) at the end, stores the wrap width (0x60), merges the TGPF_* flags into the
	object's flags with TGPF_INSERT_MODE, takes the colour (white without
	one), looks the font group up (the default font when the name is
	unknown), clears 0x4C and sets the font group, then recalculates the
	bounds (slot 86). The destructor (0x731E20) frees the block, drops the
	font group, leaves its parent if it has the true focus (0x99D0B4 is
	g_kRootWindow's trueFocus) and destroys the strings. SetString
	(0x731FF0) resizes the block to the new length and pulls the cursor
	back to it. Slots 60 to 62 remove, move and clip the per-character polys
	(TGUIBatch::RemoveIconPoly, TGIconPoly::Move, SetClipRect). The SWIG wrappers TGParagraph_*
	name slots 82 to 90 (GetString to SetCursorPosition) and give their
	arguments; slot 64 is InsertChild (TGParagraph_InsertChild).

===============================================================================
*/

#ifndef __TGPARAGRAPH_H__
#define __TGPARAGRAPH_H__

#include "Python.h"
#include "TGPane.h"
#include "TGString.h"
#include "NiColor.h"

class TGFontGroup;
class TGIconPoly;
class TGKeyboardEvent;
class TGMouseEvent;
class TGTimer;
class TGObject;

// GetObjType's value for a TGParagraph.
#define TGPARAGRAPH_TYPE		0x209

// The flags (the SWIG constants TGParagraph_TGPF_*).
#define TGPF_RECALC_BOUNDS		0x10000000
#define TGPF_WORD_WRAP			0x20000000
#define TGPF_INSERT_MODE		0x40000000
#define TGPF_READ_ONLY			0x80000000
#define TGPF_FLAGS_MASK			0xF0000000

// The event a paragraph sends itself for each key it uses; the interface
// module's TGParagraphSoundHandler plays the typing sound on it (invented
// name).
#define ET_PARAGRAPH_KEY		0x10004

// The event type of the cursor timer's event, which CursorBlinkHandler
// takes (invented name).
#define ET_CURSOR_BLINK			0x10001

// The font groups' icon for the cursor (invented name).
#define CURSOR_ICON				0xE098

// The Python method TGParagraph.GetCString, which the UI module adds to the
// shadow class: the paragraph's string as a Python string.
PyObject *	TGParagraph_GetCString( PyObject *self, PyObject *args );

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x00726080 ??_GTGParagraph@@UAEPAXI@Z
class TGParagraph : public TGPane {
public:
	// FUNCTION: STBC 0x00725f20 ??0TGParagraph@@QAE@J@Z
					TGParagraph( long id = 0 ) : TGPane( id ), string( "" ), ignoreString( "" ) {}
	// TGParagraph_Create's arguments: the string, a width, a colour, a font group name, a size, the flags (invented names).
					TGParagraph( char *string, float width, NiColorA *color, char *fontGroupName, float fontSize, int flags );
					TGParagraph( TGString *string, float width, NiColorA *color, char *fontGroupName, float fontSize, int flags );
	virtual			~TGParagraph( void );

	// TGObject
	virtual int		GetObjType( void );
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	virtual char *	GetClassName( void );
	virtual char *	GetSwigTypeName( void );
	virtual char *	GetPythonClassName( void );

	// TGEventHandlerObject
	virtual TGEventHandlerTable *	GetClassHandlers( void );

	// TGUIObject
	virtual void	GotTrueFocus( bool layout = true );
	virtual void	LostTrueFocus( bool layout = true );
	virtual void	Layout( void );
	virtual void	VirtualSlot60( void );
	virtual void	VirtualSlot61( NiPoint2 *offset );
	virtual void	VirtualSlot62( TGRect *clipRect );

	// TGPane
	virtual void	InsertChild( int index, TGUIObject *child, float x = 0.0f, float y = 0.0f, bool layout = true );

	virtual void	GetString( TGString *string );
	virtual float	GetWrapWidth( void );
	virtual void	SetWrapWidth( float width, bool layout = true );
	virtual void	InsertChar( short c, bool layout = true );
	virtual void	RecalcBounds( bool layout = true );
	virtual void	InsertString( TGString *string, bool layout = true );
	virtual void	SetString( TGString *string, bool layout = true );		// TGParagraph_SetStringW
	virtual void	SetIgnoreString( TGString *string );					// TGParagraph_SetIgnoreStringW
	virtual void	SetCursorPosition( int position );
	// TGParagraph_MoveCursorToEnd: the cursor after the last character.
	void			MoveCursorToEnd( void ) { SetCursorPosition( numPolys ); }
	virtual char *	GetCString( void );							// the string as a new C string (the Python method TGParagraph.GetCString)
	virtual void	VirtualSlot92( NiPoint2 *start );			// places the characters from start
	// Places character index at *position in rect, wrapping to the next line
	// (the tallest character of the line so far in *lineHeight); true when the
	// line breaks after it. child is the number of the embedded child a
	// CURSOR_ICON character stands for.
	virtual bool	VirtualSlot93( unsigned int index, NiPoint2 *position, TGRect *rect, float *lineHeight, int child );

	static TGParagraph *	Cast( TGObject *object );

	// The cursor timer shared by every paragraph (invented names).
	static void		CreateCursorTimer( void );
	static void		DeleteCursorTimer( void );
	static bool		WriteCursorTimer( TGStream *stream );
	static bool		ReadCursorTimer( TGStream *stream );
	static bool		FixUpCursorTimer( void );
	static void		PostFixUpCursorTimer( void );
	static void		ClearCursor( void );

	static void		RegisterHandlerNames( void );		// invented name
	static void		RegisterHandlers( void );			// invented name

	void			MouseHandler( TGMouseEvent *event );
	void			KeyboardHandler( TGKeyboardEvent *event );
	void			CursorBlinkHandler( TGEvent *event );

	void	SetFontGroup( TGFontGroup *, bool );
	TGFontGroup *	GetFontGroup( void ) { return fontGroup; }
	void	SetColor( NiColorA * );
	void	SetReadOnly( bool readOnly );
	void	SetString( char *string, bool layout = true );
	void	AppendString( char *string, bool layout = true );
	// The C string at the cursor (the SWIG wrapper TGParagraph_InsertString).
	void	InsertString( char *string, bool layout = true );
	void	SetIgnoreString( char *string );
	int	IsReadOnly( void ) { return flags & TGPF_READ_ONLY; }
	void	AppendChar( short c, bool layout = true ) { cursorPosition = numPolys; InsertChar( c, layout ); }
	void	AppendString( TGString *string, bool layout = true ) { cursorPosition = numPolys; InsertString( string, layout ); }
	void	MoveCursorToStart( void ) { SetCursorPosition( 0 ); }

	// The class's handler table, for the modules that link the tables (invented name).
	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }
	static TGParagraph *	Create( char *string = "", float width = 1.0f, NiColorA *color = NULL, char *fontGroupName = "", float fontSize = 0.0f, int flags = TGPF_READ_ONLY ) { return new TGParagraph( string, width, color, fontGroupName, fontSize, flags ); }	// the SWIG wrapper TGParagraph_Create expands it
	static TGParagraph *	CreateW( TGString *string, float width = 1.0f, NiColorA *color = NULL, char *fontGroupName = "", float fontSize = 0.0f, int flags = TGPF_READ_ONLY ) { return new TGParagraph( string, width, color, fontGroupName, fontSize, flags ); }	// the SWIG wrapper TGParagraph_CreateW expands it
	int	GetMaxChars( void ) { return maxChars; }
	void	SetMaxChars( int maxChars_ ) { maxChars = maxChars_; }
	static void	SetCursorBlinkInterval( float interval ) { cursorBlinkInterval = interval; }

protected:
	// What ReadFromStream keeps of the font group in fontGroup until
	// PostFixUp looks the group up (invented name).
	struct FontRecord {
		char *		name;					// from the stream, or NULL
		float		size;
	};

	void			Init( TGString *string, float width, NiColorA *color, char *fontGroupName, float fontSize, int flags );	// invented name
	void			Unknown7337F0( void );		// empties the bounds to one line of the font
	void			Unknown732420( void );		// makes the cursor poly when the cursor is on
	void			Unknown732550( void );		// moves the cursor poly to the cursor
	int				Unknown732710( TGRect *cursorRect, TGRect *clipRect );	// RECT_*: the cursor's rectangle on the screen
	TGIconPoly *	Unknown7328A0( unsigned int index, TGRect *rect, TGRect *clipRect );	// the poly of a character, or NULL when clipped away
	bool			Unknown732C60( unsigned int index, NiPoint2 *position, float *lineHeight, int child );	// wraps before a word that doesn't fit
	void			Unknown732D50( unsigned int index, NiPoint2 *position, float *lineHeight, int child );	// a new line when the word at index doesn't fit
	bool			Unknown732EA0( unsigned int index, NiPoint2 *position );	// whether the character at index runs past the wrap width
	void			Unknown732EF0( unsigned int index, NiPoint2 *position, float *width, float *height, TGUIObject *child );	// a character's size

	TGFontGroup *	fontGroup;				// 0x4C
	TGString		string;					// 0x50 the text
	float			wrapWidth;				// 0x60
	unsigned int	numPolys;				// 0x64 the length of polys: the string's length (invented name)
	unsigned int	cursorPosition;			// 0x68 at most numPolys; the end at first (invented name)
	NiColorA		color;					// 0x6C
	TGIconPoly **	polys;					// 0x7C pool block of an entry per character, or NULL (invented name);
											// an entry is a poly, or the number (at most numPolys) of an
											// embedded child, or NULL
	TGString		ignoreString;			// 0x80 characters the paragraph ignores
	int				maxChars;				// 0x90 0x7FFFFFFF: no limit

	static TGEventHandlerTable	classHandlers;		// invented name
	static TGIconPoly *	cursorPoly;
	static float		cursorBlinkInterval;	// the cursor's time on and off, in seconds
	static TGTimer *	cursorTimer;		// blinks the cursor of the paragraph with the true focus (invented name)		// the cursor of the paragraph with the true focus, or NULL (invented name)
};

/*
================
TGParagraph::AppendString

TGParagraph_AppendString: the text goes in at the end.
================
*/
// FUNCTION: STBC 0x005d4900 ?AppendString@TGParagraph@@QAEXPAD_N@Z
inline void TGParagraph::AppendString( char *string, bool layout ) {
	TGString text( string );
	cursorPosition = numPolys;
	InsertString( &text, layout );
}

/*
================
TGParagraph::InsertString

TGParagraph_InsertString: the C string goes in at the cursor.
================
*/
// FUNCTION: STBC 0x005d47c0 ?InsertString@TGParagraph@@QAEXPAD_N@Z @game/AppWrap
inline void TGParagraph::InsertString( char *string, bool layout ) {
	TGString text( string );
	InsertString( &text, layout );
}

/*
================
TGParagraph::GetObjType
================
*/
// FUNCTION: STBC 0x00725fa0
inline int TGParagraph::GetObjType( void ) {
	return TGPARAGRAPH_TYPE;
}

/*
================
TGParagraph::IsTypeOf
================
*/
// FUNCTION: STBC 0x00725fb0
inline bool TGParagraph::IsTypeOf( int type ) {
	if ( type == TGPARAGRAPH_TYPE ) {
		return true;
	}
	return TGPane::IsTypeOf( type );
}

/*
================
TGParagraph::GetClassName
================
*/
// FUNCTION: STBC 0x00726010
inline char *TGParagraph::GetClassName( void ) {
	return "TGParagraph";
}

/*
================
TGParagraph::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00726020
inline char *TGParagraph::GetSwigTypeName( void ) {
	return "_p_TGParagraph";
}

/*
================
TGParagraph::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00726030
inline char *TGParagraph::GetPythonClassName( void ) {
	return "TGParagraphPtr";
}

/*
================
TGParagraph::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00726040
inline TGEventHandlerTable *TGParagraph::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
TGParagraph::GetString

Copies the text into string.
================
*/
// FUNCTION: STBC 0x00726050
inline void TGParagraph::GetString( TGString *string ) {
	*string = this->string;
}

/*
================
TGParagraph::GetWrapWidth
================
*/
// FUNCTION: STBC 0x00726060
inline float TGParagraph::GetWrapWidth( void ) {
	return wrapWidth;
}

/*
================
TGParagraph::GetCString
================
*/
// FUNCTION: STBC 0x00726070
inline char *TGParagraph::GetCString( void ) {
	return string.Unknown6F56F0();
}

#endif /* !__TGPARAGRAPH_H__ */
