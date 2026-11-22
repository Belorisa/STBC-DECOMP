// NiSkinController.cpp -- skinning by bones

#include "NiRTLib.h"
#include <assert.h>
#include <string.h>

#include "NiSkinController.h"
#include "NiBone.h"
#include "NiCloneExtraData.h"
#include "NiObjectNET.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00805940 _$E30
// FUNCTION: STBC 0x00805950 _$E41
// FUNCTION: STBC 0x00805990 _$E52
// FUNCTION: STBC 0x008059a0 _$E56
// FUNCTION: STBC 0x008059b0 _$E60
// FUNCTION: STBC 0x008059c0 _$E64
// FUNCTION: STBC 0x008059d0 _$E181
// FUNCTION: STBC 0x008059e0 _$E186
// FUNCTION: STBC 0x008059f0 _$E190
// FUNCTION: STBC 0x00805a00 _$E194

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00805920 _$E215
// FUNCTION: STBC 0x008058c0 _$E17
// FUNCTION: STBC 0x008058e0 _$E15
// FUNCTION: STBC 0x008058f0 _$E23
// FUNCTION: STBC 0x00805910 _$E21

// FUNCTION: STBC 0x00804810 _$E201
// FUNCTION: STBC 0x00804830 _$E198
// DATA: STBC 0x9A2320 ?rtti@NiSkinController@@2VNiRTTI@@B
const NiRTTI NiSkinController::rtti( "NiSkinController", &NiTimeController::rtti );

/*
================
NiSkinController::Register
================
*/
// FUNCTION: STBC 0x804850 ?Register@NiSkinController@@SAHXZ
NiImplementRegister( NiSkinController )

/*
================
NiSkinController::FindController
================
*/
// FUNCTION: STBC 0x8049A0
NiSkinController *NiSkinController::FindController( NiObjectNET *target ) {
	for ( NiTimeController *controller = target->GetControllers(); controller; controller = controller->GetNext() ) {
		NiSkinController *skinController = NiDynamicCast( NiSkinController, controller );
		if ( skinController ) {
			return skinController;
		}
	}
	return 0;
}

/*
================
NiSkinController::~NiSkinController
================
*/
// FUNCTION: STBC 0x8049F0
NiSkinController::~NiSkinController() {
	delete [] bones;
	delete [] boneBounds;
	delete [] vertexCounts;

	unsigned int i;
	if ( boneVertices ) {
		for ( i = 0; i < boneCount; i++ ) {
			delete [] boneVertices[i];
		}
		delete [] boneVertices;
	}
	if ( vertexIndices ) {
		for ( i = 0; i < boneCount; i++ ) {
			delete [] vertexIndices[i];
		}
		delete [] vertexIndices;
	}
}

/*
================
NiSkinController::ComputeBoneBounds

Once the vertex arrays and the target are there, and no bounds yet: blends
every bone's offsets by their weights into the target's vertices, then
bounds each bone's vertices. The statements are AnimOnly.exe 0x44882A's
(NiSkinController.cpp, asserts at its lines 0x52, 0x56 and 0x6D).
================
*/
// FUNCTION: STBC 0x804B40
void NiSkinController::ComputeBoneBounds() {
	if ( !boneCount || !vertexCounts || !boneVertices || !vertexIndices || !target ) {
		return;
	}
	if ( boneBounds ) {
		return;
	}

	boneBounds = new NiBound[boneCount];
	assert( boneBounds );

	unsigned int vertexCount = GetVertexCount();
	NiPoint3 *vertices = new NiPoint3[vertexCount];
	assert( vertices );
	memset( vertices, 0, vertexCount * sizeof( NiPoint3 ) );

	unsigned int maxVertexCount = 0;
	unsigned int i, j;
	for ( i = 0; i < boneCount; i++ ) {
		if ( vertexCounts[i] > maxVertexCount ) {
			maxVertexCount = vertexCounts[i];
		}
		for ( j = 0; j < vertexCounts[i]; j++ ) {
			vertices[vertexIndices[i][j]] += boneVertices[i][j].GetWeight() * boneVertices[i][j].GetOffset();
		}
	}

	NiPoint3 *boneVertexPoints = new NiPoint3[maxVertexCount];
	assert( boneVertexPoints );
	for ( i = 0; i < boneCount; i++ ) {
		for ( j = 0; j < vertexCounts[i]; j++ ) {
			boneVertexPoints[j] = vertices[vertexIndices[i][j]];
		}
		boneBounds[i].ComputeFromData( vertexCounts[i], boneVertexPoints );
	}

	delete [] vertices;
	delete [] boneVertexPoints;
}

/*
================
NiSkinController::ComputeWorldBound

The union of the bone bounds, each carried by its bone's transform.
================
*/
// FUNCTION: STBC 0x804D90
void NiSkinController::ComputeWorldBound( NiBound &bound ) {
	NiBone *bone = bones[0];
	bound.Update( boneBounds[0], bone->GetBoneRotate(), bone->GetBoneTranslate(), bone->GetBoneScale() );
	for ( unsigned int i = 1; i < boneCount; i++ ) {
		NiBound boneBound;
		bone = bones[i];
		boneBound.Update( boneBounds[i], bone->GetBoneRotate(), bone->GetBoneTranslate(), bone->GetBoneScale() );
		bound.Merge( &boneBound );
	}
}

/*
================
NiSkinController::CreateClone

The derived class makes the object; the members are copied here.
================
*/
// FUNCTION: STBC 0x804E70
NiObject *NiSkinController::CreateClone() {
	NiSkinController *clone = CreateNew();
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiSkinController::CopyMembers

Copies the bounds and the vertex arrays; the bones wait for ProcessClone.
The clone's bone bounds are rebuilt after each array arrives, which does
nothing: the first two calls lack the vertex arrays, the last finds the
bounds already copied. Each array goes in through its setter, as the SDK's
debug NetImmerse 3.1 compiles it (AnimOnly.exe 0x448D15).
================
*/
// FUNCTION: STBC 0x804E90
void NiSkinController::CopyMembers( NiSkinController *dest ) {
	NiTimeController::CopyMembers( dest );

	dest->SetBoneCount( GetBoneCount() );

	dest->boneBounds = new NiBound[boneCount];
	unsigned int i;
	for ( i = 0; i < boneCount; i++ ) {
		dest->boneBounds[i] = boneBounds[i];
	}

	unsigned int *destVertexCounts = new unsigned int[boneCount];
	for ( i = 0; i < boneCount; i++ ) {
		destVertexCounts[i] = vertexCounts[i];
	}
	dest->SetVertexCounts( destVertexCounts );

	BoneVertex **destBoneVertices = new BoneVertex *[boneCount];
	unsigned short **destVertexIndices = new unsigned short *[boneCount];
	for ( i = 0; i < boneCount; i++ ) {
		destBoneVertices[i] = new BoneVertex[vertexCounts[i]];
		destVertexIndices[i] = new unsigned short[vertexCounts[i]];
		for ( unsigned int j = 0; j < vertexCounts[i]; j++ ) {
			destBoneVertices[i][j] = boneVertices[i][j];
			destVertexIndices[i][j] = vertexIndices[i][j];
		}
	}
	dest->SetBoneVertices( destBoneVertices, destVertexIndices );
}

/*
================
NiSkinController::ProcessClone

The clone's bones are the clones of this controller's bones, which the
clone extra data each bone holds while cloning names.
================
*/
// FUNCTION: STBC 0x805080
void NiSkinController::ProcessClone( NiObject *clone ) {
	NiTimeController::ProcessClone( clone );

	NiBone **cloneBones = new NiBone *[boneCount];
	for ( unsigned int i = 0; i < boneCount; i++ ) {
		NiCloneExtraData *extra = (NiCloneExtraData *)bones[i]->GetExtraData();
		cloneBones[i] = (NiBone *)extra->GetClone();
	}
	( (NiSkinController *)clone )->bones = cloneBones;
}

/*
================
NiSkinController::VirtualSlot20

Rebuilds the vertex arrays for the new vertex numbering. Each bone's new
arrays are newVertexCount long and indexed by the new vertex index; every
bone's vertex count becomes newVertexCount.

Quirk: the entries of vertices a bone did not move are never written, so
each bone keeps uninitialised weights and offsets for them.
================
*/
// FUNCTION: STBC 0x8050D0
void NiSkinController::VirtualSlot20( unsigned short oldVertexCount, unsigned int unknown, unsigned short newVertexCount, const int *newIndices ) {
	if ( !target ) {
		return;
	}
	if ( oldVertexCount == newVertexCount ) {
		ComputeBoneBounds();
		return;
	}

	BoneVertex **newBoneVertices = new BoneVertex *[boneCount];
	unsigned short **newVertexIndices = new unsigned short *[boneCount];
	unsigned short i, j;
	for ( i = 0; i < boneCount; i++ ) {
		newBoneVertices[i] = new BoneVertex[newVertexCount];
		newVertexIndices[i] = new unsigned short[newVertexCount];
		for ( j = 0; j < vertexCounts[i]; j++ ) {
			unsigned short index = vertexIndices[i][j];
			NiPoint3 offset = boneVertices[i][j].offset;
			float weight = boneVertices[i][j].weight;
			if ( newIndices[index] >= 0 ) {
				unsigned short newIndex = (unsigned short)newIndices[index];
				newBoneVertices[i][newIndex].weight = weight;
				newBoneVertices[i][newIndex].offset = offset;
				newVertexIndices[i][newIndex] = newIndex;
			}
		}
	}

	unsigned int k;
	if ( boneVertices ) {
		for ( k = 0; k < boneCount; k++ ) {
			if ( boneVertices[k] ) {
				delete [] boneVertices[k];
			}
		}
		delete [] boneVertices;
		boneVertices = 0;
	}
	if ( vertexIndices ) {
		for ( k = 0; k < boneCount; k++ ) {
			if ( vertexIndices[k] ) {
				delete [] vertexIndices[k];
			}
		}
		delete [] vertexIndices;
		vertexIndices = 0;
	}
	if ( boneBounds ) {
		delete [] boneBounds;
		boneBounds = 0;
	}

	for ( k = 0; k < boneCount; k++ ) {
		vertexCounts[k] = newVertexCount;
	}
	vertexIndices = newVertexIndices;
	boneVertices = newBoneVertices;
	ComputeBoneBounds();
}

/*
================
NiSkinController::CreateObject

The class is abstract: there is nothing to create.
================
*/
// FUNCTION: STBC 0x805320
void NiSkinController::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiSkinController::LoadBinary

The bone count, each bone's vertex count, the bones' link IDs, then per
bone and vertex the weight, the target vertex index and the offset.
================
*/
// FUNCTION: STBC 0x805330
void NiSkinController::LoadBinary( NiStream &stream ) {
	NiTimeController::LoadBinary( stream );
	NiSkinControllerLoadRecord *record = (NiSkinControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );

	NiStreamLoadBinary( stream, boneCount );

	unsigned int *loadedVertexCounts = new unsigned int[boneCount];
	assert( loadedVertexCounts );
	unsigned int i;
	for ( i = 0; i < boneCount; i++ ) {
		NiStreamLoadBinary( stream, loadedVertexCounts[i] );
	}
	vertexCounts = loadedVertexCounts;
	ComputeBoneBounds();

	unsigned int *boneLinkIDs = new unsigned int[boneCount];
	assert( boneLinkIDs );
	for ( i = 0; i < boneCount; i++ ) {
		NiStreamLoadBinary( stream, boneLinkIDs[i] );
	}
	record->boneLinkIDs = boneLinkIDs;

	BoneVertex **loadedBoneVertices = new BoneVertex *[boneCount];
	assert( loadedBoneVertices );
	unsigned short **loadedVertexIndices = new unsigned short *[boneCount];
	assert( loadedVertexIndices );
	for ( i = 0; i < boneCount; i++ ) {
		loadedBoneVertices[i] = new BoneVertex[loadedVertexCounts[i]];
		assert( loadedBoneVertices[i] );
		loadedVertexIndices[i] = new unsigned short[loadedVertexCounts[i]];
		assert( loadedVertexIndices[i] );
		for ( unsigned int j = 0; j < loadedVertexCounts[i]; j++ ) {
			float weight;
			unsigned short index;
			NiPoint3 offset;
			NiStreamLoadBinary( stream, weight );
			NiStreamLoadBinary( stream, index );
			NiStreamLoadBinary( stream, offset.x );
			NiStreamLoadBinary( stream, offset.y );
			NiStreamLoadBinary( stream, offset.z );
			loadedVertexIndices[i][j] = index;
			loadedBoneVertices[i][j].offset = offset;
			loadedBoneVertices[i][j].weight = weight;
		}
	}
	vertexIndices = loadedVertexIndices;
	boneVertices = loadedBoneVertices;
	ComputeBoneBounds();
}

/*
================
NiSkinController::LinkObject

Resolves the bones and frees their link IDs.
================
*/
// FUNCTION: STBC 0x805560
void NiSkinController::LinkObject( NiStream &stream ) {
	NiTimeController::LinkObject( stream );
	NiSkinControllerLoadRecord *record = (NiSkinControllerLoadRecord *)stream.GetLoadRecord();
	assert( record );
	if ( record->boneLinkIDs ) {
		NiBone **boneArray = new NiBone *[boneCount];	// (name invented; fits the /Od frame order)
		assert( boneArray );
		for ( unsigned int i = 0; i < boneCount; i++ ) {
			boneArray[i] = (NiBone *)stream.GetObjectFromLinkID( record->boneLinkIDs[i] );
		}
		delete [] record->boneLinkIDs;
		record->boneLinkIDs = 0;
		SetBones( boneArray );
	}
	ComputeBoneBounds();
}

/*
================
NiSkinController::RegisterStreamables
================
*/
// FUNCTION: STBC 0x8055E0
bool NiSkinController::RegisterStreamables( NiStream &stream ) {
	if ( !NiTimeController::RegisterStreamables( stream ) ) {
		return false;
	}
	for ( unsigned int i = 0; i < boneCount; i++ ) {
		if ( !bones[i]->RegisterStreamables( stream ) ) {
			return false;
		}
	}
	return true;
}

/*
================
NiSkinController::SaveBinary

Each vertex's weight and offset are read through BoneVertex's accessors,
as the SDK's debug NetImmerse 3.1 does (AnimOnly.exe 0x449B5C).
================
*/
// FUNCTION: STBC 0x805630
void NiSkinController::SaveBinary( NiStream &stream ) {
	NiTimeController::SaveBinary( stream );

	NiStreamSaveBinary( stream, boneCount );
	unsigned int i;
	for ( i = 0; i < boneCount; i++ ) {
		NiStreamSaveBinary( stream, vertexCounts[i] );
	}
	for ( i = 0; i < boneCount; i++ ) {
		NiStreamSaveBinary( stream, bones[i] );
	}
	for ( i = 0; i < boneCount; i++ ) {
		for ( unsigned int j = 0; j < vertexCounts[i]; j++ ) {
			NiStreamSaveBinary( stream, boneVertices[i][j].GetWeight() );
			NiStreamSaveBinary( stream, vertexIndices[i][j] );
			NiStreamSaveBinary( stream, boneVertices[i][j].GetOffset().x );
			NiStreamSaveBinary( stream, boneVertices[i][j].GetOffset().y );
			NiStreamSaveBinary( stream, boneVertices[i][j].GetOffset().z );
		}
	}
}

/*
================
NiSkinController::IsEqual

Written as the SDK's debug NetImmerse 3.1 compiles it (AnimOnly.exe
0x449D2F): the accessors, and the vertex count read at each test.
================
*/
// FUNCTION: STBC 0x805780
bool NiSkinController::IsEqual( NiObject *object ) {
	if ( !NiTimeController::IsEqual( object ) ) {
		return false;
	}

	NiSkinController *controller = (NiSkinController *)object;
	if ( boneCount != controller->boneCount ) {
		return false;
	}
	for ( unsigned int i = 0; i < boneCount; i++ ) {
		if ( !bones[i]->IsEqual( controller->bones[i] ) ) {
			return false;
		}
		if ( vertexCounts[i] != controller->vertexCounts[i] ) {
			return false;
		}
		for ( unsigned int j = 0; j < vertexCounts[i]; j++ ) {
			if ( boneVertices[i][j].GetWeight() != controller->boneVertices[i][j].GetWeight() ) {
				return false;
			}
			if ( vertexIndices[i][j] != controller->GetVertexIndex( i, j ) ) {
				return false;
			}
			if ( boneVertices[i][j].GetOffset() != controller->boneVertices[i][j].GetOffset() ) {
				return false;
			}
		}
	}
	return true;
}
