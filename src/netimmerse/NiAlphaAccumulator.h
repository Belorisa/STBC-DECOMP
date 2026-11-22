/*
===============================================================================

	NiAlphaAccumulator.h

	The cluster accumulator restricted to alpha-blended geometry: what is
	drawn without blending, or with an alpha property that asks to skip the
	sorter, is refused and drawn at once by the caller.

	Evidence: the NiRTTI (0x9A19B0, initialised at 0x7EBB50) is named
	"NiAlphaAccumulator" with NiClusterAccumulator's as its base. The
	creation function (0x7EBD80) allocates 0x48 bytes, builds the cluster
	accumulator (0x7EB440), stores the vptr 0x889B44 and sets the bool at
	0x44. The vtable, GetRTTI (0x439210), the destructor (0x439220) and
	slot 14 (0x439290) are the copies of a game unit: slot 14 takes the
	geometry only when the renderer's current alpha property blends, and
	not when the bool at 0x44 is set and the property's bit 13 is. The
	member and method names follow the Gamebryo lineage
	(m_bObserveNoSortHint).

===============================================================================
*/

#ifndef __NIALPHAACCUMULATOR_H__
#define __NIALPHAACCUMULATOR_H__

#include "NiClusterAccumulator.h"
#include "NiAlphaProperty.h"
#include "NiCamera.h"
#include "NiStreamMacros.h"

/*
================
NiAlphaAccumulatorLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x89911C). The class name is invented.
================
*/
// FUNCTION: STBC 0x7EBE20 ??_GNiAlphaAccumulatorLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7EBE40 ??1NiAlphaAccumulatorLoadRecord@@UAE@XZ
class NiAlphaAccumulatorLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x439220 ??_GNiAlphaAccumulator@@UAEPAXI@Z
class NiAlphaAccumulator : public NiClusterAccumulator {
public:
	// FUNCTION: STBC 0x439210 ?GetRTTI@NiAlphaAccumulator@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	// FUNCTION: STBC 0x439240 ??1NiAlphaAccumulator@@UAE@XZ
	virtual					~NiAlphaAccumulator() {}

	virtual bool			RegisterObject( NiAVObject *geometry );

							NiAlphaAccumulator() { observeNoSortHint = true; }

	void					SetObserveNoSortHint( bool observe ) { observeNoSortHint = observe; }
	bool					GetObserveNoSortHint() const { return observeNoSortHint; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiAlphaAccumulator *dest );

	bool					observeNoSortHint;	// honour the alpha property's no-sorter bit
};

NiRegisterStream( NiAlphaAccumulator );

/*
================
NiAlphaAccumulator::RegisterObject

Takes the geometry when the renderer draws it alpha blended.
================
*/
// FUNCTION: STBC 0x439290 ?RegisterObject@NiAlphaAccumulator@@UAE_NPAVNiAVObject@@@Z
inline bool NiAlphaAccumulator::RegisterObject( NiAVObject *geometry ) {
	NiAlphaProperty *alpha = camera->GetRenderer()->GetPropertyState()->GetAlpha();
	if ( !alpha || !alpha->GetAlphaBlending() ) {
		return false;
	}
	if ( observeNoSortHint && alpha->GetNoSorter() ) {
		return false;
	}
	objects->AddTail( camera->Unknown7E8B10( geometry ) );
	return true;
}

#endif /* !__NIALPHAACCUMULATOR_H__ */
