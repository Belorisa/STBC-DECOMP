// NiSkinControllerDeform.cpp -- the skinning loop
//
// NiSkinController::Deform sits alone, far from the rest of the class
// (0x8239D0, against 0x8049A0-0x805800), so it is kept in a unit of its
// own. The file name is invented.

#include "NiRTLib.h"
#include <string.h>

#include "NiSkinController.h"
#include "NiBone.h"
#include "NiDynamicEffect.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00823b30 _$E201
// FUNCTION: STBC 0x00823b50 _$E17
// FUNCTION: STBC 0x00823b70 _$E15
// FUNCTION: STBC 0x00823b80 _$E23
// FUNCTION: STBC 0x00823ba0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00823bb0 _$E30
// FUNCTION: STBC 0x00823bc0 _$E41
// FUNCTION: STBC 0x00823bd0 _$E52
// FUNCTION: STBC 0x00823be0 _$E56
// FUNCTION: STBC 0x00823bf0 _$E60
// FUNCTION: STBC 0x00823c00 _$E64
// FUNCTION: STBC 0x00823c10 _$E181
// FUNCTION: STBC 0x00823c20 _$E186
// FUNCTION: STBC 0x00823c30 _$E190
// FUNCTION: STBC 0x00823c40 _$E194

/*
================
NiSkinController::Deform

Clears the target's vertices, then adds each bone's world transform of its
offsets, weighted, to the vertices they move.
================
*/
// FUNCTION: STBC 0x8239D0
void NiSkinController::Deform( NiPoint3 *vertices ) {
	memset( vertices, 0, GetVertexCount() * sizeof( NiPoint3 ) );

	for ( unsigned int i = 0; i < boneCount; i++ ) {
		NiBone *bone = bones[i];
		BoneVertex *boneVertex = boneVertices[i];
		unsigned short *indices = vertexIndices[i];
		NiMatrix3 rotateScale = bone->GetBoneRotate() * bone->GetBoneScale();

		for ( unsigned int j = 0; j < vertexCounts[i]; j++ ) {
			NiPoint3 offset = boneVertex->offset;
			NiPoint3 position = rotateScale * offset + bone->GetBoneTranslate();
			vertices[indices[j]] += position * boneVertex->weight;
			boneVertex++;
		}
	}
}
