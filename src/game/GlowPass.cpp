/*
===============================================================================

	GlowPass.cpp

===============================================================================
*/

#include "GlowPass.h"
#include <iostream>

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004e1a70 _$E21
// FUNCTION: STBC 0x004e1a80 _$E20
// FUNCTION: STBC 0x004e1a90 _$E32
// FUNCTION: STBC 0x004e1aa0 _$E31
// FUNCTION: STBC 0x004e1ab0 _$E36
// FUNCTION: STBC 0x004e1ac0 _$E35
// FUNCTION: STBC 0x004e1ad0 _$E40
// FUNCTION: STBC 0x004e1ae0 _$E39
// FUNCTION: STBC 0x004e1af0 _$E44
// FUNCTION: STBC 0x004e1b00 _$E43

// The guard of std::ctype<unsigned short>::id and its exit-time thunk (<iostream>).
// FUNCTION: STBC 0x004e19c0 _$E82
// FUNCTION: STBC 0x004e19e0 _$E81

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004e19f0 _$E63
// FUNCTION: STBC 0x004e1a00 _$E60
// FUNCTION: STBC 0x004e1a10 _$E62
// FUNCTION: STBC 0x004e1a20 _$E61
// FUNCTION: STBC 0x004e1a30 _$E69
// FUNCTION: STBC 0x004e1a40 _$E66
// FUNCTION: STBC 0x004e1a50 _$E68
// FUNCTION: STBC 0x004e1a60 _$E67

/*
================
GlowPass::CreateClone
================
*/
// FUNCTION: STBC 0x004e18e0
NiObject *GlowPass::CreateClone() {
	GlowPass *clone = new GlowPass;
	CopyMembers( clone );
	return clone;
}

/*
================
GlowPass::CopyMembers
================
*/
// FUNCTION: STBC 0x004e1970
void GlowPass::CopyMembers( GlowPass *dest ) {
	NiExtraData::CopyMembers( dest );
	dest->unknown14 = unknown14;
	if ( !properties.IsEmpty() ) {
		NiPropertyList *node = &properties;
		do {
			dest->properties.AddHead( node->GetItem() );
			node = node->next;
		} while ( node != NULL );
	}
}

// The RTTI's dynamic initialiser, its construction, the registration of its
// destructor and the guarded destructor.
// FUNCTION: STBC 0x004e1880 _$E75
// FUNCTION: STBC 0x004e1890 _$E71
// FUNCTION: STBC 0x004e18b0 _$E74
// FUNCTION: STBC 0x004e18c0 _$E72
// DATA: STBC 0x009861b0
const NiRTTI GlowPass::rtti( "GlowPass", &NiExtraData::rtti );
