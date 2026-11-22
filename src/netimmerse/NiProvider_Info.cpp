// NiProvider_Info.cpp -- the Miles 3D providers

#include <string.h>
#include "NiStream.h"
#include "NiProvider_Info.h"
#include "NiAVObject.h"
#include "NiExtraData.h"
#include "NiListener.h"
#include "NiObjectNET.h"
#include "NiProperty.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiTimeController.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00790c10 _$E158

// DATA: STBC 0x99FE1C ?rtti@NiProvider_Info@@2VNiRTTI@@B
const NiRTTI NiProvider_Info::rtti( "NiProvider_Info", &NiObject::rtti );

/*
================
NiProvider_Info::NiProvider_Info
================
*/
// FUNCTION: STBC 0x790D00
NiProvider_Info::NiProvider_Info( const char *name_, HPROVIDER provider_ ) {
	name = new char[strlen( name_ ) + 1];
	const char *source = name_;
	char *dest = name;
	char c;
	do {
		c = *source++;
		*dest++ = c;
	} while ( c );
	provider = provider_;
	open = false;
}

/*
================
NiProvider_Info::~NiProvider_Info
================
*/
// FUNCTION: STBC 0x790DA0
NiProvider_Info::~NiProvider_Info() {
	Close();
	delete [] name;
}

/*
================
NiProvider_Info::Open
================
*/
// FUNCTION: STBC 0x790DE0
bool NiProvider_Info::Open() {
	if ( AIL_open_3D_provider( provider ) == 0 ) {
		open = true;
		return true;
	}
	return false;
}

/*
================
NiProvider_Info::Close
================
*/
// FUNCTION: STBC 0x790E00
bool NiProvider_Info::Close() {
	if ( open ) {
		AIL_close_3D_provider( provider );
		provider = 0;
		return true;
	}
	return false;
}

/*
================
NiProvider_Info::GetActiveSampleCount
================
*/
// FUNCTION: STBC 0x790E30
int NiProvider_Info::GetActiveSampleCount() const {
	if ( !open ) {
		return 0;
	}
	return AIL_active_3D_sample_count( provider );
}
