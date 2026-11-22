/*
===============================================================================

	NiListener.h

	The scene's ear: an NiAVObject whose world transform and velocity are
	handed to a Miles 3D listener on each open provider.

	Evidence: the NiRTTI (0x99FD60) is named "NiListener" with NiAVObject's
	as its base; the vtable at 0x897F68 (NiAVObject's 39 slots, overriding
	GetRTTI, the streaming slots, the destructor and slot 26). The
	constructor (0x78CD20) sets the direction at 0xC8 to (1, 0, 0), the up
	vector at 0xD4 to (0, 1, 0) and builds the list of Miles listener
	handles at 0xE0 with two items per block; NiSoundSystem allocates 0xF8
	bytes for it (0x78D8F2) and owns the one listener. LoadBinary (0x78D340)
	and SaveBinary (0x78D3B0) stream the two vectors after NiAVObject's
	members.

===============================================================================
*/

#ifndef __NILISTENER_H__
#define __NILISTENER_H__

#include "NiAVObject.h"
#include "NiMilesImports.h"
#include "NiTList.h"
#include "NiStreamMacros.h"

class NiProvider_Info;

/*
================
NiListenerLoadRecord

The record CreateObject hands the stream: NiAVObject's, under a vtable of
its own (0x898018). The class name is invented.
================
*/
// FUNCTION: STBC 0x78D300 ??_GNiListenerLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x78D320 ??1NiListenerLoadRecord@@UAE@XZ
class NiListenerLoadRecord : public NiAVObjectLoadRecord {
};

// FUNCTION: STBC 0x0078CDF0 ??_GNiListener@@UAEPAXI@Z
class NiListener : public NiAVObject {
public:
	// FUNCTION: STBC 0x78D610 ?GetRTTI@NiListener@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiListener();

	// FUNCTION: STBC 0x78D640 ?VirtualSlot26@NiListener@@UAE_NXZ
	virtual bool			VirtualSlot26() { return false; }

							NiListener();

	// opens a Miles listener on the provider, opening the provider first
	// when it is closed (name invented)
	void					AddProvider( NiProvider_Info *provider );
	// closes every Miles listener (name invented)
	void					CloseListeners();
	// hands the world position, orientation and velocity to every Miles
	// listener: Miles' z axis points the other way, and its velocities are
	// in metres per millisecond (Gamebryo lineage name)
	void					Update();

	// what the first Miles listener reports, in this object's axes; the
	// position is NiPoint3::ZERO when there is none (names invented)
	NiPoint3				GetListenerPosition();
	NiPoint3				GetListenerVelocity();
	void					GetListenerOrientation( NiPoint3 &dir, NiPoint3 &upDir );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	NiPoint3				direction;	// model-space facing (Gamebryo lineage name)
	NiPoint3				up;			// model-space up (Gamebryo lineage name)
	NiTList<H3DPOBJECT>		listeners;	// one Miles listener per open provider
};

NiRegisterStream( NiListener );

#endif /* !__NILISTENER_H__ */
