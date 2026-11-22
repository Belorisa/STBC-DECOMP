// NiCloneExtraData.cpp -- the extra data that marks an original while it is cloned
//
// The unit holds only the class's NiRTTI: its static initialiser and exit
// (0x821DC0), the locale pair, the <iostream> quads and two registrations
// follow the viewer-string helpers' unit (to 0x821EA0); the class's members
// are all inline.

#include "NiRTLib.h"
#include "NiCloneExtraData.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00821e00 _$E55
// FUNCTION: STBC 0x00821e20 _$E17
// FUNCTION: STBC 0x00821e40 _$E15
// FUNCTION: STBC 0x00821e50 _$E23
// FUNCTION: STBC 0x00821e70 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00821e80 _$E30
// FUNCTION: STBC 0x00821e90 _$E41

// FUNCTION: STBC 0x00821dc0 _$E48
// FUNCTION: STBC 0x00821de0 _$E45
// DATA: STBC 0x9A2CC8 ?rtti@NiCloneExtraData@@2VNiRTTI@@B
const NiRTTI NiCloneExtraData::rtti( "NiCloneExtraData", &NiExtraData::rtti );
