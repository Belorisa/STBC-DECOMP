/*
===============================================================================

	NiSkinController.h

	The abstract base of the skinning controllers: it binds the vertices of
	its target to a set of bones, each bone moving its vertices by a weight.
	The derived classes say what the target is (NiTriShapeSkinController,
	NiBezierSkinController).

	Evidence: the NiRTTI (0x9A2320, built at 0x804810) is named
	"NiSkinController" with NiTimeController's as its base; the vtable at
	0x899FBC has NiTimeController's 20 slots and adds 4, two of them
	_purecall (as Update and TargetIsRequiredType are); the creation
	function the stream registers (0x805320) does nothing, the class being
	abstract. The destructor (0x8049F0) frees the bone array (0x30), the
	bone bound array (0x34, an NiBound array new), the vertex count array
	(0x3C) and the two per-bone arrays of arrays (0x40, 0x44), all
	boneCount (0x38) long. LoadBinary (0x805330) reads, per bone and vertex,
	a float weight, an unsigned short vertex index and a point.

===============================================================================
*/

#ifndef __NISKINCONTROLLER_H__
#define __NISKINCONTROLLER_H__

#include "NiTimeController.h"
#include "NiBound.h"
#include "NiStreamMacros.h"

class NiBone;

/*
================
NiSkinControllerLoadRecord

NiTimeController's link IDs and an array of the bones' link IDs, which
LinkObject frees. The class name is invented; only the derived classes
create records.
================
*/
class NiSkinControllerLoadRecord : public NiTimeControllerLoadRecord {
public:
							NiSkinControllerLoadRecord() { boneLinkIDs = 0; }

	unsigned int *			boneLinkIDs;	// heap array of boneCount link IDs
};

// FUNCTION: STBC 0x00805970 ??_GNiSkinController@@UAEPAXI@Z
class NiSkinController : public NiTimeController {
public:
	/*
	================
	NiSkinController::BoneVertex

	A vertex as one bone moves it: the vertex in the bone's space and the
	bone's weight in the vertex's position. The struct name is invented.
	================
	*/
	struct BoneVertex {
		// the SDK's debug NetImmerse 3.1 reads them through these
		// (AnimOnly.exe 0x44A3D0 and 0x44A390; names invented)
		NiPoint3			GetOffset() const { return offset; }
		float				GetWeight() const { return weight; }

		NiPoint3			offset;
		float				weight;
	};

	// FUNCTION: STBC 0x00805960 ?GetRTTI@NiSkinController@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();
	virtual void			ProcessClone( NiObject *clone );

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );

	virtual					~NiSkinController();

	// a bone's target vertex index (AnimOnly.exe 0x44A350; name invented)
	unsigned short			GetVertexIndex( unsigned int bone, unsigned int vertex ) const { return vertexIndices[bone][vertex]; }

	// TODO identity: moves the vertex arrays to a new vertex numbering
	// (newIndices maps each old vertex to its new index, negative when
	// dropped) when the target's vertex count changes; the second argument
	// is unused
	virtual void			VirtualSlot20( unsigned short oldVertexCount, unsigned int unknown, unsigned short newVertexCount, const int *newIndices );

	// writes the skinned vertices, the weighted sum of each bone's world
	// transform of its offsets (name invented; defined at 0x8239D0, outside
	// this unit)
	virtual void			Deform( NiPoint3 *vertices );

	// the target's vertex count (name invented)
	virtual unsigned short	GetVertexCount() const = 0;

	// a new default object of the derived class, for CreateClone to fill
	// (name invented)
	virtual NiSkinController *	CreateNew() = 0;

							NiSkinController() {
								bones = 0;
								boneBounds = 0;
								boneCount = 0;
								vertexCounts = 0;
								boneVertices = 0;
								vertexIndices = 0;
							}

	// the bones and their count (Gamebryo lineage names)
	NiBone **				GetBones() const { return bones; }
	void					SetBones( NiBone **bones_ ) { bones = bones_; }	// AnimOnly.exe 0x44A2A0
	unsigned int			GetBoneCount() const { return boneCount; }

	// the bound of the bones' vertices, by the bones' transforms (name
	// invented)
	void					ComputeWorldBound( NiBound &bound );

	// the first NiSkinController of the target's list (name invented;
	// nothing calls it)
	static NiSkinController *	FindController( NiObjectNET *target );

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

protected:
	void					CopyMembers( NiSkinController *dest );

	// builds the bone bounds once every array is there (name invented)
	void					ComputeBoneBounds();

	// set an array and rebuild the bone bounds, as the SDK's debug
	// NetImmerse 3.1 compiles CopyMembers (AnimOnly.exe 0x44A2C0, 0x44A300
	// and 0x44A320; names invented)
	void					SetBoneCount( unsigned int count ) { boneCount = count; ComputeBoneBounds(); }
	void					SetVertexCounts( unsigned int *counts ) { vertexCounts = counts; ComputeBoneBounds(); }
	void					SetBoneVertices( BoneVertex **vertices, unsigned short **indices ) {
								vertexIndices = indices;
								boneVertices = vertices;
								ComputeBoneBounds();
							}

	NiBone **				bones;			// heap array of boneCount bones, not referenced
	NiBound *				boneBounds;		// array new of boneCount bounds of each bone's vertices
	unsigned int			boneCount;
	unsigned int *			vertexCounts;	// heap array: each bone's vertex count
	BoneVertex **			boneVertices;	// heap array of each bone's heap array of vertices
	unsigned short **		vertexIndices;	// heap array of each bone's heap array of target vertex indices
};

NiRegisterStream( NiSkinController );

#endif /* !__NISKINCONTROLLER_H__ */
