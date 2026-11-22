/*
===============================================================================

	NiClusterAccumulator.h

	The sorter that collects the geometry registered during a camera's click
	and draws it, when the click finishes, in the order the cluster sort
	gives.

	Evidence: the NiRTTI (0x9A1960, initialised at 0x7EB2B0) is named
	"NiClusterAccumulator" with NiAccumulator's as its base; the vtable at
	0x8990DC has NiAccumulator's 15 slots. The creation function (0x7EB850)
	allocates 0x44 bytes. The constructor (0x7EB440) builds the sorter at
	0x10 without a camera and allocates the object list at 0x0C with two
	items per block. Slot 14 (0x7EB710) has the camera wrap the geometry
	(0x7E8B10) and appends it to the list; FinishAccumulating (0x7EB630)
	has the sorter order the list, draws each object through the camera
	(0x7E8B90) and empties the list.

===============================================================================
*/

#ifndef __NICLUSTERACCUMULATOR_H__
#define __NICLUSTERACCUMULATOR_H__

#include "NiAccumulator.h"
#include "NiCamera.h"
#include "NiClusterSorter.h"
#include "NiSortedObject.h"
#include "NiStream.h"
#include "NiStreamMacros.h"

/*
================
NiClusterAccumulatorLoadRecord

The record CreateObject hands the stream: NiObject's link ID under a vtable
of its own (0x899118). The class name is invented.
================
*/
// FUNCTION: STBC 0x7EB920 ??_GNiClusterAccumulatorLoadRecord@@UAEPAXI@Z
// FUNCTION: STBC 0x7EB940 ??1NiClusterAccumulatorLoadRecord@@UAE@XZ
class NiClusterAccumulatorLoadRecord : public NiObjectLoadRecord {
};

// FUNCTION: STBC 0x7EBB20 ??_GNiClusterAccumulator@@UAEPAXI@Z
class NiClusterAccumulator : public NiAccumulator {
public:
	// FUNCTION: STBC 0x7EBB10 ?GetRTTI@NiClusterAccumulator@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiClusterAccumulator();

	virtual bool			StartAccumulating( NiCamera *camera_ );
	virtual void			FinishAccumulating();
	virtual bool			RegisterObject( NiAVObject *geometry );

							NiClusterAccumulator();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiClusterAccumulator *dest );

	NiTList<NiSortedObject *> *	objects;	// the objects registered since the click started
	NiClusterSorter			sorter;
};

NiRegisterStream( NiClusterAccumulator );

#endif /* !__NICLUSTERACCUMULATOR_H__ */
