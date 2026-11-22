/*
===============================================================================

	NiAccumulator.h

	The base of the sorters a renderer hands the visible geometry to during
	a camera's click: the camera starts the accumulation, the objects are
	registered with the sorter, and the camera finishes it.

	Evidence: the NiRTTI (0x9A1490, initialised at 0x7D8D30) is named
	"NiAccumulator" with NiObject's as its base; the constructor (0x7D8EC0)
	stores the vtable 0x88D2A0 (15 slots, slot 14 _purecall) and clears
	the camera at 8. Slot 12 (0x7D8EE0) takes the camera unless one is
	already set, and says whether it did; slot 13 (0x7D8F00) clears it.
	NiCamera::Click (0x7E7FA0) calls slots 12 and 13 around the drawing and
	keeps the sorter in the renderer. Slot 14 takes a geometry object
	(NiClusterAccumulator's, 0x7EB710, hands it to the camera's 0x7E8B10). The CreateObject is empty (the class
	is abstract) and CreateClone returns NULL. Names follow the Gamebryo
	lineage (m_pkCamera, StartAccumulating, FinishAccumulating).

===============================================================================
*/

#ifndef __NIACCUMULATOR_H__
#define __NIACCUMULATOR_H__

#include "NiObject.h"
#include "NiStreamMacros.h"

class NiCamera;
class NiAVObject;

// FUNCTION: STBC 0x004e1450 ??_GNiAccumulator@@UAEPAXI@Z @netimmerse/NiAccumulator
class NiAccumulator : public NiObject {
public:
	// FUNCTION: STBC 0x4E1430 ?GetRTTI@NiAccumulator@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiAccumulator() {}

	// the accumulation for one camera
	virtual bool			StartAccumulating( NiCamera *camera_ );
	virtual void			FinishAccumulating();
	// files a geometry object for drawing when the accumulation finishes
	// (name invented); it takes an NiAVObject, since NiBezierPatch, which
	// is not an NiGeometry, registers through NiRender's (0x833490)
	virtual bool			RegisterObject( NiAVObject *geometry ) = 0;

							NiAccumulator();

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiAccumulator *dest );

	NiCamera *				camera;		// the camera accumulating, NULL between clicks
};

NiRegisterStream( NiAccumulator );

#endif /* !__NIACCUMULATOR_H__ */
