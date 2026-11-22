/*
===============================================================================

	TGString.h

	The game's string: a wide (16-bit) standard string with conversions from
	and to C strings. It is std::wstring itself, not a wrapper around one:
	the SWIG wrapper TGString_GetLength reads the string's length at offset 8
	(basic_string's _Len), and every method works on this directly with the
	VC6 basic_string<unsigned short> code inlined. 16 bytes.

	Only SetString, CompareC, Compare, Find, FindC and GetLength are named by
	the SWIG wrappers; the other methods keep placeholder names.

===============================================================================
*/

#ifndef __TGSTRING_H__
#define __TGSTRING_H__

#include <string>

#include "Python.h"

class TGStream;

// The implicit copy constructor: retail keeps the SWIG wrapper unit's copy.
// FUNCTION: STBC 0x006016c0 ??0TGString@@QAE@ABV0@@Z @game/AppWrap
class TGString : public std::wstring {
public:
					TGString( char *string = "" );
					~TGString( void );

	int				GetLength( void ) { return length(); }

	int				FindC( char *string, int start = 0, bool caseSensitive = false );
	int				Find( TGString *string, int start = 0, bool caseSensitive = false );
	int				CompareC( char *string, bool ignoreCase = false );
	int				Compare( TGString *string, bool ignoreCase = false );
	TGString		Append( TGString *string );
	void			ToLower( void );
	void			ToUpper( void );
	void			Unknown6F4770( void ) const;
	void			SetString( char *string );
	void			Unknown6F4970( const TGString *string );
	void			Unknown6F4A40( char *string );
	void			Unknown6F4B50( unsigned short *string );
	unsigned short	Unknown6F4C20( int index );
	void			AssignC( char *string );
	void			Unknown6F4CE0( unsigned short *string );
	void			Unknown6F4D90( TGString *string );
	// Out of line, and returning nothing: NiTMap<int, TGString>::SetValue
	// (0x6E7910) assigns a value through it.
	void			operator=( const TGString &string );
	void			Unknown6F5020( int position, char *string );
	void			Unknown6F5160( int position, TGString *string );
	void			Unknown6F5260( int position, TGString *string );
	void			Unknown6F5360( int position, unsigned short *string );
	void			Unknown6F54A0( int position, char c );
	void			Unknown6F54C0( int position, unsigned short c );
	void			Unknown6F55C0( int position, int count );
	char *			Unknown6F56F0( void ) const;					// a new pool block holding the string narrowed to chars

	// The length, then the characters.
	bool			WriteToStream( TGStream *stream );		// invented name
	bool			ReadFromStream( TGStream *stream );		// invented name
};

/*
================
operator+

The two strings joined. Out of line in retail (0x416A60, emitted by a unit
far from TGString's): it appends with Unknown6F4970 and destroys its copy
with ~TGString, which the template operator+ of <string> would inline.
================
*/
// FUNCTION: STBC 0x00416a60 ??H@YA?AVTGString@@ABV0@0@Z
inline TGString operator+( const TGString &left, const TGString &right ) {
	TGString result( left );
	result.Unknown6F4970( &right );
	return result;
}

/*
================
TGString::Append

The string with string appended; this one is unchanged.
================
*/
// FUNCTION: STBC 0x005c2270
inline TGString TGString::Append( TGString *string ) {
	return *this + *string;
}

// The Python method TGString.GetCString: the string as a Python string
// ("" when it narrows to nothing); NULL when the argument is not a TGString.
PyObject *		TGString_GetCString( PyObject *self, PyObject *args );

#endif /* !__TGSTRING_H__ */
