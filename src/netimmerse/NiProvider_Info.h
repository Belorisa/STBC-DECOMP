/*
===============================================================================

	NiProvider_Info.h

	A Miles 3D provider the sound system found: its name and handle, and
	whether it is open.

	Evidence: the NiRTTI (0x99FE1C) is named "NiProvider_Info" with
	NiObject's as its base; the vtable at 0x898138 (NiObject's 12 slots,
	overriding GetRTTI and the destructor); the constructor (0x790D00) copies
	the name into a new block at 0x08 and keeps the handle at 0x0C, clearing
	the open flag at 0x10; the destructor (0x790DA0) closes an open provider
	and frees the name. NiListener opens the provider (0x790DE0) before
	opening a listener on it.

===============================================================================
*/

#ifndef __NIPROVIDER_INFO_H__
#define __NIPROVIDER_INFO_H__

#include "NiObject.h"
#include "NiMilesImports.h"

// FUNCTION: STBC 0x790D80 ??_GNiProvider_Info@@UAEPAXI@Z
class NiProvider_Info : public NiObject {
public:
	// FUNCTION: STBC 0x791000 ?GetRTTI@NiProvider_Info@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	virtual					~NiProvider_Info();

							NiProvider_Info( const char *name_, HPROVIDER provider_ );

	// opens the provider; false when Miles fails to
	bool					Open();
	// closes an open provider; false when it was not open
	bool					Close();
	// the provider's playing 3D samples, 0 when it is not open (name
	// invented)
	int						GetActiveSampleCount() const;

	const char *			GetName() const { return name; }
	HPROVIDER				GetProvider() const { return provider; }
	bool					IsOpen() const { return open; }

	static const NiRTTI		rtti;

protected:
	char *					name;		// owned copy
	HPROVIDER				provider;
	bool					open;
};

#endif /* !__NIPROVIDER_INFO_H__ */
