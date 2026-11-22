/*
===============================================================================

	NiBinaryVoxelExtraData.h

	The extra data block that hangs a model's NiBinaryVoxelData on its
	node, streamed in NIF files under the name "NiBinaryVoxelExtraData"
	(0x8DD2BC).

	0x18 bytes. Evidence: the NiRTTI at 0x983910 is named
	"NiBinaryVoxelExtraData" with NiExtraData's as its base; the vtable
	0x88B764 replaces CreateClone (0x4ABF30), the five streaming slots
	from LoadBinary (0x4AC200) to IsEqual (0x4AC320), GetRTTI (0x451290)
	and the destructor (0x4512A0); the last two are inline copies kept in
	LODModel's unit, which builds the block (0x450996). The data is held
	counted at 0x14 (CopyMembers 0x4ABFB0 and LinkObject 0x4AC260 release
	the old one and add a reference to the new one).

===============================================================================
*/

#ifndef __NIBINARYVOXELEXTRADATA_H__
#define __NIBINARYVOXELEXTRADATA_H__

#include "NiExtraData.h"
#include "NiBinaryVoxelData.h"
#include "NiStreamMacros.h"

/*
================
NiBinaryVoxelExtraDataLoadRecord

The record CreateObject hands the stream (vtable 0x88C5B0): the link ID of
the voxel data, which LinkObject resolves. The class name is invented.
================
*/
// FUNCTION: STBC 0x004ac550 ??_GNiBinaryVoxelExtraDataLoadRecord@@UAEPAXI@Z
class NiBinaryVoxelExtraDataLoadRecord : public NiExtraDataLoadRecord {
public:
	// FUNCTION: STBC 0x004ac570 ??1NiBinaryVoxelExtraDataLoadRecord@@UAE@XZ
	virtual				~NiBinaryVoxelExtraDataLoadRecord() { voxelDataLinkID = 0; }

	unsigned int		voxelDataLinkID;
};

// FUNCTION: STBC 0x004512a0 ??_GNiBinaryVoxelExtraData@@UAEPAXI@Z @game/LODModel
// FUNCTION: STBC 0x004512c0 ??1NiBinaryVoxelExtraData@@UAE@XZ @game/LODModel
class NiBinaryVoxelExtraData : public NiExtraData {
public:
	// FUNCTION: STBC 0x451290 ?GetRTTI@NiBinaryVoxelExtraData@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

							NiBinaryVoxelExtraData() {}

	NiBinaryVoxelData *		GetVoxelData() const { return voxelData; }
	void					SetVoxelData( NiBinaryVoxelData *voxelData_ ) { voxelData = voxelData_; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiBinaryVoxelExtraData *dest );

	NiPointer<NiBinaryVoxelData>	voxelData;	// 0x14
};

NiRegisterStream( NiBinaryVoxelExtraData );

#endif /* !__NIBINARYVOXELEXTRADATA_H__ */
