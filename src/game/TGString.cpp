/*
===============================================================================

	TGString.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGString.h"
#include "PyEmbed.h"
#include "NiMain.h"
#include "TGStream.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006f5c50 _$E429
// FUNCTION: STBC 0x006f5c70 _$E428

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006f3550 _$E32
// FUNCTION: STBC 0x006f3560 _$E29
// FUNCTION: STBC 0x006f3570 _$E31
// FUNCTION: STBC 0x006f3580 _$E30
// FUNCTION: STBC 0x006f3590 _$E38
// FUNCTION: STBC 0x006f35a0 _$E35
// FUNCTION: STBC 0x006f35b0 _$E37
// FUNCTION: STBC 0x006f35c0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006f35d0 _$E66
// FUNCTION: STBC 0x006f35e0 _$E65
// FUNCTION: STBC 0x006f35f0 _$E70
// FUNCTION: STBC 0x006f3600 _$E69
// FUNCTION: STBC 0x006f3610 _$E81
// FUNCTION: STBC 0x006f3620 _$E80
// FUNCTION: STBC 0x006f3630 _$E85
// FUNCTION: STBC 0x006f3640 _$E84
// FUNCTION: STBC 0x006f3650 _$E89
// FUNCTION: STBC 0x006f3660 _$E88
// FUNCTION: STBC 0x006f3670 _$E93
// FUNCTION: STBC 0x006f3680 _$E92
// FUNCTION: STBC 0x006f3690 _$E217
// FUNCTION: STBC 0x006f36a0 _$E216
// FUNCTION: STBC 0x006f36b0 _$E159
// FUNCTION: STBC 0x006f36c0 _$E158
// FUNCTION: STBC 0x006f36d0 _$E172
// FUNCTION: STBC 0x006f36e0 _$E171
// FUNCTION: STBC 0x006f36f0 _$E222
// FUNCTION: STBC 0x006f3700 _$E221
// FUNCTION: STBC 0x006f3710 _$E227
// FUNCTION: STBC 0x006f3720 _$E226
// FUNCTION: STBC 0x006f3730 _$E231
// FUNCTION: STBC 0x006f3740 _$E230
// FUNCTION: STBC 0x006f3750 _$E235
// FUNCTION: STBC 0x006f3760 _$E234
// FUNCTION: STBC 0x006f3770 _$E242
// FUNCTION: STBC 0x006f3780 _$E241
// FUNCTION: STBC 0x006f3790 _$E267
// FUNCTION: STBC 0x006f37a0 _$E266
// FUNCTION: STBC 0x006f37b0 _$E271
// FUNCTION: STBC 0x006f37c0 _$E270
// FUNCTION: STBC 0x006f37d0 _$E117
// FUNCTION: STBC 0x006f37e0 _$E116
// FUNCTION: STBC 0x006f37f0 _$E253
// FUNCTION: STBC 0x006f3800 _$E252
// FUNCTION: STBC 0x006f3810 _$E260
// FUNCTION: STBC 0x006f3820 _$E259
// FUNCTION: STBC 0x006f3830 _$E283
// FUNCTION: STBC 0x006f3840 _$E282
// FUNCTION: STBC 0x006f3850 _$E287
// FUNCTION: STBC 0x006f3860 _$E286
// FUNCTION: STBC 0x006f3870 _$E327
// FUNCTION: STBC 0x006f3880 _$E326
// FUNCTION: STBC 0x006f3890 _$E333
// FUNCTION: STBC 0x006f38a0 _$E332
// FUNCTION: STBC 0x006f38b0 _$E275
// FUNCTION: STBC 0x006f38c0 _$E274
// FUNCTION: STBC 0x006f38d0 _$E279
// FUNCTION: STBC 0x006f38e0 _$E278
// FUNCTION: STBC 0x006f38f0 _$E291
// FUNCTION: STBC 0x006f3900 _$E290
// FUNCTION: STBC 0x006f3910 _$E295
// FUNCTION: STBC 0x006f3920 _$E294
// FUNCTION: STBC 0x006f3930 _$E299
// FUNCTION: STBC 0x006f3940 _$E298
// FUNCTION: STBC 0x006f3950 _$E303
// FUNCTION: STBC 0x006f3960 _$E302
// FUNCTION: STBC 0x006f3970 _$E307
// FUNCTION: STBC 0x006f3980 _$E306
// FUNCTION: STBC 0x006f3990 _$E311
// FUNCTION: STBC 0x006f39a0 _$E310
// FUNCTION: STBC 0x006f39b0 _$E315
// FUNCTION: STBC 0x006f39c0 _$E314
// FUNCTION: STBC 0x006f39d0 _$E319
// FUNCTION: STBC 0x006f39e0 _$E318
// FUNCTION: STBC 0x006f39f0 _$E122
// FUNCTION: STBC 0x006f3a00 _$E121
// FUNCTION: STBC 0x006f3a10 _$E323
// FUNCTION: STBC 0x006f3a20 _$E322
// FUNCTION: STBC 0x006f3a30 _$E127
// FUNCTION: STBC 0x006f3a40 _$E126
// FUNCTION: STBC 0x006f3a50 _$E337
// FUNCTION: STBC 0x006f3a60 _$E336
// FUNCTION: STBC 0x006f3a70 _$E341
// FUNCTION: STBC 0x006f3a80 _$E340
// FUNCTION: STBC 0x006f3a90 _$E345
// FUNCTION: STBC 0x006f3aa0 _$E344
// FUNCTION: STBC 0x006f3ab0 _$E349
// FUNCTION: STBC 0x006f3ac0 _$E348
// FUNCTION: STBC 0x006f3ad0 _$E136
// FUNCTION: STBC 0x006f3ae0 _$E135
// FUNCTION: STBC 0x006f3af0 _$E353
// FUNCTION: STBC 0x006f3b00 _$E352
// FUNCTION: STBC 0x006f3b10 _$E357
// FUNCTION: STBC 0x006f3b20 _$E356
// FUNCTION: STBC 0x006f3b30 _$E361
// FUNCTION: STBC 0x006f3b40 _$E360
// FUNCTION: STBC 0x006f3b50 _$E142
// FUNCTION: STBC 0x006f3b60 _$E141
// FUNCTION: STBC 0x006f3b70 _$E147
// FUNCTION: STBC 0x006f3b80 _$E146
// FUNCTION: STBC 0x006f3b90 _$E177
// FUNCTION: STBC 0x006f3ba0 _$E176
// FUNCTION: STBC 0x006f3bb0 _$E365
// FUNCTION: STBC 0x006f3bc0 _$E364
// FUNCTION: STBC 0x006f3bd0 _$E182
// FUNCTION: STBC 0x006f3be0 _$E181
// FUNCTION: STBC 0x006f3bf0 _$E369
// FUNCTION: STBC 0x006f3c00 _$E368
// FUNCTION: STBC 0x006f3c10 _$E373
// FUNCTION: STBC 0x006f3c20 _$E372
// FUNCTION: STBC 0x006f3c30 _$E187
// FUNCTION: STBC 0x006f3c40 _$E186
// FUNCTION: STBC 0x006f3c50 _$E377
// FUNCTION: STBC 0x006f3c60 _$E376
// FUNCTION: STBC 0x006f3c70 _$E152
// FUNCTION: STBC 0x006f3c80 _$E151
// FUNCTION: STBC 0x006f3c90 _$E381
// FUNCTION: STBC 0x006f3ca0 _$E380
// FUNCTION: STBC 0x006f3cb0 _$E385
// FUNCTION: STBC 0x006f3cc0 _$E384
// FUNCTION: STBC 0x006f3cd0 _$E192
// FUNCTION: STBC 0x006f3ce0 _$E191
// FUNCTION: STBC 0x006f3cf0 _$E197
// FUNCTION: STBC 0x006f3d00 _$E196
// FUNCTION: STBC 0x006f3d10 _$E389
// FUNCTION: STBC 0x006f3d20 _$E388
// FUNCTION: STBC 0x006f3d30 _$E393
// FUNCTION: STBC 0x006f3d40 _$E392
// FUNCTION: STBC 0x006f3d50 _$E397
// FUNCTION: STBC 0x006f3d60 _$E396
// FUNCTION: STBC 0x006f3d70 _$E401
// FUNCTION: STBC 0x006f3d80 _$E400
// FUNCTION: STBC 0x006f3d90 _$E405
// FUNCTION: STBC 0x006f3da0 _$E404
// FUNCTION: STBC 0x006f3db0 _$E409
// FUNCTION: STBC 0x006f3dc0 _$E408
// FUNCTION: STBC 0x006f3dd0 _$E413
// FUNCTION: STBC 0x006f3de0 _$E412
// FUNCTION: STBC 0x006f3df0 _$E417
// FUNCTION: STBC 0x006f3e00 _$E416
// FUNCTION: STBC 0x006f3e10 _$E202
// FUNCTION: STBC 0x006f3e20 _$E201
// FUNCTION: STBC 0x006f3e30 _$E421
// FUNCTION: STBC 0x006f3e40 _$E420
// FUNCTION: STBC 0x006f3e50 _$E207
// FUNCTION: STBC 0x006f3e60 _$E206
// FUNCTION: STBC 0x006f3e70 _$E212
// FUNCTION: STBC 0x006f3e80 _$E211

/*
================
TGString::TGString
================
*/
// FUNCTION: STBC 0x006f3e90
TGString::TGString( char *string ) {
	SetString( string );
}

/*
================
TGString::~TGString
================
*/
// FUNCTION: STBC 0x006f3ef0
TGString::~TGString( void ) {
}

/*
================
TGString::WriteToStream

The length, then the characters.
================
*/
// FUNCTION: STBC 0x006f3f30
bool TGString::WriteToStream( TGStream *stream ) {
	int length = GetLength();
	stream->WriteInt( length );
	for ( int i = 0; i < length; i++ ) {
		stream->WriteWChar( (*this)[i] );
	}
	return true;
}

/*
================
TGString::ReadFromStream

The length, then the characters (what WriteToStream writes).
================
*/
// FUNCTION: STBC 0x006f4000
bool TGString::ReadFromStream( TGStream *stream ) {
	int length = stream->ReadInt();
	resize( length );
	for ( int i = 0; i < length; i++ ) {
		(*this)[i] = stream->ReadWChar();
	}
	return true;
}

/*
================
TGString::FindC

The position of string at or after start, or -1.
================
*/
// FUNCTION: STBC 0x006f41e0
int TGString::FindC( char *string, int start, bool caseSensitive ) {
	TGString wide( "" );
	wide.AssignC( string );
	// BUG: caseSensitive is dropped; the search always ignores case.
	return Find( &wide, start );
}

/*
================
TGString::Find

The position of string at or after start, or -1. Unlike Compare's flag,
this one asks for a case-sensitive search.
================
*/
// FUNCTION: STBC 0x006f4260
int TGString::Find( TGString *string, int start, bool caseSensitive ) {
	if ( start >= 0 ) {
		if ( caseSensitive ) {
			const unsigned short *found = wcsstr( c_str() + start, string->c_str() );
			if ( found != NULL ) {
				return found - c_str();
			}
		} else {
			TGString lowerString = *string;
			lowerString.ToLower();
			TGString lowerThis( "" );
			lowerThis.Unknown6F4D90( this );
			lowerThis.ToLower();
			const unsigned short *found = wcsstr( lowerThis.c_str() + start, lowerString.c_str() );
			if ( found != NULL ) {
				return found - lowerThis.c_str();
			}
		}
	}
	return -1;
}

/*
================
TGString::CompareC
================
*/
// FUNCTION: STBC 0x006f43e0
int TGString::CompareC( char *string, bool ignoreCase ) {
	TGString wide( "" );
	wide.AssignC( string );
	// BUG: ignoreCase is dropped; the comparison is always case sensitive.
	return Compare( &wide );
}

/*
================
TGString::Compare
================
*/
// FUNCTION: STBC 0x006f4450
int TGString::Compare( TGString *string, bool ignoreCase ) {
	if ( !ignoreCase ) {
		return wcscmp( c_str(), string->c_str() );
	}
	return _wcsicmp( c_str(), string->c_str() );
}

/*
================
TGString::ToLower

Lowers the case of the ASCII letters. Invented name, from the behaviour:
no string names it.
================
*/
// FUNCTION: STBC 0x006f44b0
void TGString::ToLower( void ) {
	for ( int i = 0; i < GetLength(); i++ ) {
		unsigned short c = ( *this )[i];
		if ( !( c & 0xFF00 ) && c >= 'A' && c <= 'Z' ) {
			( *this )[i] = c + ( 'a' - 'A' );
		}
	}
}

/*
================
TGString::ToUpper

Raises the case of the ASCII letters. Invented name, from the behaviour:
no string names it.
================
*/
// FUNCTION: STBC 0x006f4610
void TGString::ToUpper( void ) {
	for ( int i = 0; i < GetLength(); i++ ) {
		unsigned short c = ( *this )[i];
		if ( !( c & 0xFF00 ) && c >= 'a' && c <= 'z' ) {
			( *this )[i] = c - ( 'a' - 'A' );
		}
	}
}

/*
================
TGString::Unknown6F4770
Narrows the string into a buffer it frees again: what used the buffer
is compiled out of this build.
================
*/
// FUNCTION: STBC 0x006f4770
void TGString::Unknown6F4770( void ) const {
	char *narrow = new char[length() + 1];
	if ( narrow != NULL ) {
		unsigned int i;
		for ( i = 0; i < length(); i++ ) {
			narrow[i] = (char)( *this )[i];
		}
		narrow[i] = '\0';
		delete narrow;
	}
}

/*
================
TGString::SetString

Widens a C string.
================
*/
// FUNCTION: STBC 0x006f47c0
void TGString::SetString( char *string ) {
	int length = 0;
	if ( string != NULL ) {
		length = strlen( string );
	}
	resize( length );
	for ( int i = 0; i < length; i++ ) {
		( *this )[i] = (unsigned char)string[i];
	}
}

/*
================
TGString::Unknown6F4970
Appends string.
================
*/
// FUNCTION: STBC 0x006f4970
void TGString::Unknown6F4970( const TGString *string ) {
	append( *string );
}

/*
================
TGString::Unknown6F4A40
Appends a C string.
================
*/
// FUNCTION: STBC 0x006f4a40
void TGString::Unknown6F4A40( char *string ) {
	TGString wide( "" );
	wide.AssignC( string );
	append( wide );
}

/*
================
TGString::Unknown6F4B50
Appends a wide string.
================
*/
// FUNCTION: STBC 0x006f4b50
void TGString::Unknown6F4B50( unsigned short *string ) {
	append( string );
}

/*
================
TGString::Unknown6F4C20
The character at index.
================
*/
// FUNCTION: STBC 0x006f4c20
unsigned short TGString::Unknown6F4C20( int index ) {
	return ( *this )[index];
}

/*
================
TGString::AssignC

Sets the string from a C string, keeping it when string is NULL. Invented
name, from the behaviour and the class's C suffix for C-string arguments
(FindC, CompareC).
================
*/
// FUNCTION: STBC 0x006f4cc0
void TGString::AssignC( char *string ) {
	if ( string != NULL ) {
		SetString( string );
	}
}

/*
================
TGString::Unknown6F4CE0
================
*/
// FUNCTION: STBC 0x006f4ce0
void TGString::Unknown6F4CE0( unsigned short *string ) {
	if ( string != NULL ) {
		assign( string );
	}
}

/*
================
TGString::Unknown6F4D90
================
*/
// FUNCTION: STBC 0x006f4d90
void TGString::Unknown6F4D90( TGString *string ) {
	if ( string != NULL ) {
		assign( *string );
	}
}

/*
================
TGString::operator=
================
*/
// FUNCTION: STBC 0x006f4ee0 ??4TGString@@QAEXABV0@@Z
void TGString::operator=( const TGString &string ) {
	assign( string );
}

/*
================
TGString::Unknown6F5020
Inserts a C string at position.
================
*/
// FUNCTION: STBC 0x006f5020
void TGString::Unknown6F5020( int position, char *string ) {
	TGString wide( "" );
	wide.AssignC( string );
	insert( position, wide );
}

/*
================
TGString::Unknown6F5160
Inserts string at position.
================
*/
// FUNCTION: STBC 0x006f5160
void TGString::Unknown6F5160( int position, TGString *string ) {
	insert( position, *string );
}

/*
================
TGString::Unknown6F5260
The same as Unknown6F5160.
================
*/
// FUNCTION: STBC 0x006f5260
void TGString::Unknown6F5260( int position, TGString *string ) {
	insert( position, *string );
}

/*
================
TGString::Unknown6F5360
Inserts a wide string at position.
================
*/
// FUNCTION: STBC 0x006f5360
void TGString::Unknown6F5360( int position, unsigned short *string ) {
	insert( position, string );
}

/*
================
TGString::Unknown6F54A0
Inserts a character at position.
================
*/
// FUNCTION: STBC 0x006f54a0
void TGString::Unknown6F54A0( int position, char c ) {
	Unknown6F54C0( position, c );
}

/*
================
TGString::Unknown6F54C0
Inserts a wide character at position.
================
*/
// FUNCTION: STBC 0x006f54c0
void TGString::Unknown6F54C0( int position, unsigned short c ) {
	insert( position, 1, c );
}

/*
================
TGString::Unknown6F55C0
Removes count characters at position.
================
*/
// FUNCTION: STBC 0x006f55c0
void TGString::Unknown6F55C0( int position, int count ) {
	erase( position, count );
}

/*
================
TGString::Unknown6F56F0

The string narrowed to a new C string, which the caller deletes. A character
that does not fit in a char becomes 1.
================
*/
// FUNCTION: STBC 0x006f56f0
char *TGString::Unknown6F56F0( void ) const {
	int length = this->length();
	char *narrow = new char[length + 1];
	for ( int i = 0; i < length; i++ ) {
		unsigned short c = (*this)[i];
		if ( c > 0xFF ) {
			c = 1;
		}
		narrow[i] = (char)c;
	}
	narrow[length] = '\0';
	return narrow;
}

/*
================
TGString_GetCString

The Python method TGString.GetCString (added to App's shadow class by the
objects module). The string's narrowed copy is deleted even when it is
NULL.

The result is given one reference too many.
================
*/
// FUNCTION: STBC 0x006f5770
PyObject *TGString_GetCString( PyObject *self, PyObject *args ) {
	PyObject *result = NULL;

	PyObject *instance = PyTuple_GetItem( args, 0 );
	if ( instance != NULL ) {
		TGString *string = NULL;
		PyEmbed::Unknown6F98E0( instance, (void **)&string, "TGString" );
		if ( string != NULL ) {
			char *narrow = string->Unknown6F56F0();
			if ( narrow != NULL ) {
				result = PyString_FromString( narrow );
			} else {
				result = PyString_FromString( "" );
			}
			Py_INCREF( result );
			delete [] narrow;
		}
	}
	return result;
}
