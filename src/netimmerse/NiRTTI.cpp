// NiRTTI.cpp -- NetImmerse run-time type information

#include "NiRTLib.h"
#include <string.h>

#include "NiRTTI.h"
#include "NiObject.h"
#include "NiRawImageData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00818f80 _$E51
// FUNCTION: STBC 0x00818fa0 _$E17
// FUNCTION: STBC 0x00818fc0 _$E15
// FUNCTION: STBC 0x00818fd0 _$E23
// FUNCTION: STBC 0x00818ff0 _$E21

/*
================
NiRTTI::NiRTTI

Keeps its own copy of the class name.
================
*/
// FUNCTION: STBC 0x818F20
NiRTTI::NiRTTI( const char *name, const NiRTTI *baseRTTI ) {
	this->name = new char[strlen( name ) + 1];
	strcpy( this->name, name );
	this->baseRTTI = baseRTTI;
}

/*
================
NiRTTI::~NiRTTI
================
*/
// FUNCTION: STBC 0x818F70
NiRTTI::~NiRTTI() {
	delete [] name;
}
