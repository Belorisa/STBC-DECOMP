// NiViewerStrings.cpp -- the viewer strings of plain members
//
// Each function formats "prefix = value" into a new string the caller
// deletes, sized for the longest value of its type. They form an object of
// their own between NiPoint3Unitize.cpp and NiCloneExtraData.cpp
// (0x821960 to 0x821D3F); the file name is invented.

#include "NiRTLib.h"
#include <stdio.h>
#include <string.h>

#include "NiObject.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00821d40 _$E37
// FUNCTION: STBC 0x00821d60 _$E17
// FUNCTION: STBC 0x00821d80 _$E15
// FUNCTION: STBC 0x00821d90 _$E23
// FUNCTION: STBC 0x00821db0 _$E21

/*
================
NiGetViewerString

"--- text ---": the banner of a class's strings.
================
*/
// FUNCTION: STBC 0x821960 ?NiGetViewerString@@YAPADPBD@Z
char *NiGetViewerString( const char *text ) {
	char *string = new char[strlen( text ) + 9];
	sprintf( string, "--- %s ---", text );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x8219A0 ?NiGetViewerString@@YAPADPBD_N@Z
char *NiGetViewerString( const char *prefix, bool value ) {
	char *string = new char[strlen( prefix ) + 9];
	if ( value ) {
		sprintf( string, "%s = true", prefix );
	} else {
		sprintf( string, "%s = false", prefix );
	}
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821A00 ?NiGetViewerString@@YAPADPBDD@Z
char *NiGetViewerString( const char *prefix, char value ) {
	char *string = new char[strlen( prefix ) + 5];
	sprintf( string, "%s = %c", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821A40 ?NiGetViewerString@@YAPADPBDE@Z
char *NiGetViewerString( const char *prefix, unsigned char value ) {
	char *string = new char[strlen( prefix ) + 7];
	sprintf( string, "%s = %u", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821A80 ?NiGetViewerString@@YAPADPBDF@Z
char *NiGetViewerString( const char *prefix, short value ) {
	char *string = new char[strlen( prefix ) + 10];
	sprintf( string, "%s = %hd", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821AC0 ?NiGetViewerString@@YAPADPBDG@Z
char *NiGetViewerString( const char *prefix, unsigned short value ) {
	char *string = new char[strlen( prefix ) + 9];
	sprintf( string, "%s = %hu", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821B00 ?NiGetViewerString@@YAPADPBDH@Z
char *NiGetViewerString( const char *prefix, int value ) {
	char *string = new char[strlen( prefix ) + 15];
	sprintf( string, "%s = %d", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821B40 ?NiGetViewerString@@YAPADPBDI@Z
char *NiGetViewerString( const char *prefix, unsigned int value ) {
	char *string = new char[strlen( prefix ) + 14];
	sprintf( string, "%s = %u", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821B80 ?NiGetViewerString@@YAPADPBDJ@Z
char *NiGetViewerString( const char *prefix, long value ) {
	char *string = new char[strlen( prefix ) + 15];
	sprintf( string, "%s = %ld", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821BC0 ?NiGetViewerString@@YAPADPBDK@Z
char *NiGetViewerString( const char *prefix, unsigned long value ) {
	char *string = new char[strlen( prefix ) + 14];
	sprintf( string, "%s = %lu", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821C00 ?NiGetViewerString@@YAPADPBDM@Z
char *NiGetViewerString( const char *prefix, float value ) {
	char *string = new char[strlen( prefix ) + 20];
	sprintf( string, "%s = %.6e", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821C40 ?NiGetViewerString@@YAPADPBDN@Z
char *NiGetViewerString( const char *prefix, double value ) {
	char *string = new char[strlen( prefix ) + 20];
	sprintf( string, "%s = %.6e", prefix, value );
	return string;
}

/*
================
NiGetViewerString
================
*/
// FUNCTION: STBC 0x821C80 ?NiGetViewerString@@YAPADPBDPBX@Z
char *NiGetViewerString( const char *prefix, const void *value ) {
	char *string = new char[strlen( prefix ) + 16];
	sprintf( string, "%s = %p", prefix, value );
	return string;
}

/*
================
NiGetViewerString

"prefix = NULL" for a NULL value.
================
*/
// FUNCTION: STBC 0x821CC0 ?NiGetViewerString@@YAPADPBD0@Z
char *NiGetViewerString( const char *prefix, const char *value ) {
	char *string;
	if ( value ) {
		string = new char[strlen( prefix ) + strlen( value ) + 4];
		sprintf( string, "%s = %s", prefix, value );
	} else {
		string = new char[strlen( prefix ) + 8];
		sprintf( string, "%s = NULL", prefix );
	}
	return string;
}
