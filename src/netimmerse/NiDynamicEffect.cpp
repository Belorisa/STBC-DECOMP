// NiDynamicEffect.cpp -- scene graph effects and the nodes they affect

#include "NiRTLib.h"
#include <stdio.h>

#include "NiDynamicEffect.h"
#include "NiCloneExtraData.h"
#include "NiNode.h"
#include "NiStream.h"
#include "renderer/NiRender.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007e28f0 _$E209
// FUNCTION: STBC 0x007e2890 _$E17
// FUNCTION: STBC 0x007e28b0 _$E15
// FUNCTION: STBC 0x007e28c0 _$E23
// FUNCTION: STBC 0x007e28e0 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007e2910 _$E30
// FUNCTION: STBC 0x007e2920 _$E41
// FUNCTION: STBC 0x007e2930 _$E45
// FUNCTION: STBC 0x007e2940 _$E49
// FUNCTION: STBC 0x007e2950 _$E53
// FUNCTION: STBC 0x007e2960 _$E177
// FUNCTION: STBC 0x007e29a0 _$E182
// FUNCTION: STBC 0x007e29b0 _$E187
// FUNCTION: STBC 0x007e29c0 _$E191
// FUNCTION: STBC 0x007e29d0 _$E119
// FUNCTION: STBC 0x007e29e0 _$E132
// FUNCTION: STBC 0x007e29f0 _$E196

// FUNCTION: STBC 0x007e2070 _$E202
// FUNCTION: STBC 0x007e2090 _$E199
// DATA: STBC 0x9A16C8 ?rtti@NiDynamicEffect@@2VNiRTTI@@B
const NiRTTI NiDynamicEffect::rtti( "NiDynamicEffect", &NiAVObject::rtti );

// DATA: STBC 0x9784D4
int NiDynamicEffect::nextIndex = 1;

/*
================
NiDynamicEffect::Register
================
*/
// FUNCTION: STBC 0x7E20B0 ?Register@NiDynamicEffect@@SAHXZ
NiImplementRegister( NiDynamicEffect )

/*
================
NiDynamicEffect::NiDynamicEffect
================
*/
// FUNCTION: STBC 0x7E2200
NiDynamicEffect::NiDynamicEffect() {
	on = true;
	index = nextIndex;
	nextIndex++;
	unknownD0 = 0;
	revision = 1;
}

/*
================
NiDynamicEffect::~NiDynamicEffect
================
*/
// FUNCTION: STBC 0x7E2250
NiDynamicEffect::~NiDynamicEffect() {
	DetachAllAffectedNodes();
}

/*
================
NiDynamicEffect::AttachAffectedNode

The node attaches the effect in turn; each side ignores a partner it
already lists.
================
*/
// FUNCTION: STBC 0x7E2330
void NiDynamicEffect::AttachAffectedNode( NiNode *node ) {
	if ( !affectedNodes.IsInList( node ) ) {
		affectedNodes.AddHead( node );
		node->AttachEffect( this );
	}
}

/*
================
NiDynamicEffect::DetachAffectedNode
================
*/
// FUNCTION: STBC 0x7E2380
void NiDynamicEffect::DetachAffectedNode( NiNode *node ) {
	if ( affectedNodes.IsInList( node ) ) {
		affectedNodes.Remove( node );
		node->DetachEffect( this );
	}
}

/*
================
NiDynamicEffect::DetachAllAffectedNodes
================
*/
// FUNCTION: STBC 0x7E2430
void NiDynamicEffect::DetachAllAffectedNodes() {
	while ( !affectedNodes.IsEmpty() ) {
		NiNode *node = affectedNodes.item;
		affectedNodes.Remove( node );
		node->DetachEffect( this );
	}
}

/*
================
NiDynamicEffect::CreateClone

NiDynamicEffect itself is never cloned; the derived classes create the
clone.
================
*/
// FUNCTION: STBC 0x7E24C0
NiObject *NiDynamicEffect::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiDynamicEffect::CopyMembers

The clone gets its own index and affects no nodes.
================
*/
// FUNCTION: STBC 0x7E24D0
void NiDynamicEffect::CopyMembers( NiDynamicEffect *dest ) {
	NiAVObject::CopyMembers( dest );
	dest->unknownD0 = unknownD0;
	dest->revision = revision;
}

/*
================
NiDynamicEffect::VirtualSlot03

Also drops the NiCloneExtraData that cloning left at the head of the
extra data list.
================
*/
// FUNCTION: STBC 0x7E2500
void NiDynamicEffect::VirtualSlot03() {
	NiAVObject::VirtualSlot03();
	NiExtraData *extraData = GetExtraData();
	if ( extraData && NiIsExactKindOf( NiCloneExtraData, extraData ) ) {
		assert( extraData->GetRefCount() == 1 );
		RemoveExtraData( extraData );
	}
}

/*
================
NiDynamicEffect::CreateObject

The class is abstract: the stream creates nothing for it.
================
*/
// FUNCTION: STBC 0x7E2530
void NiDynamicEffect::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiDynamicEffect::LoadBinary

Reads the link IDs of the affected nodes and drops them: the nodes attach
their effects when they are linked.
================
*/
// FUNCTION: STBC 0x7E2540
void NiDynamicEffect::LoadBinary( NiStream &stream ) {
	NiAVObject::LoadBinary( stream );

	int count;
	NiStreamLoadBinary( stream, count );
	for ( int i = 0; i < count; i++ ) {
		unsigned int linkID;
		NiStreamLoadBinary( stream, linkID );
	}
}

/*
================
NiDynamicEffect::LinkObject
================
*/
// FUNCTION: STBC 0x7E2590
void NiDynamicEffect::LinkObject( NiStream &stream ) {
	NiAVObject::LinkObject( stream );
}

/*
================
NiDynamicEffect::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7E25A0
bool NiDynamicEffect::RegisterStreamables( NiStream &stream ) {
	return NiAVObject::RegisterStreamables( stream );
}

/*
================
NiDynamicEffect::SaveBinary

The link IDs (addresses) of the affected nodes follow NiAVObject's
members.
================
*/
// FUNCTION: STBC 0x7E25B0
void NiDynamicEffect::SaveBinary( NiStream &stream ) {
	NiAVObject::SaveBinary( stream );

	int count = affectedNodes.ItemsInList();
	NiStreamSaveBinary( stream, count );
	if ( count > 0 ) {
		NiNodeList *list = &affectedNodes;
		while ( list ) {
			NiNode *node = list->GetItem();
			NiStreamSaveBinary( stream, node );
			list = list->GetNext();
		}
	}
}

/*
================
NiDynamicEffect::IsEqual

Compares the affected nodes by presence only.
================
*/
// FUNCTION: STBC 0x7E2630
bool NiDynamicEffect::IsEqual( NiObject *object ) {
	if ( !NiAVObject::IsEqual( object ) ) {
		return false;
	}

	NiDynamicEffect *pEffect = (NiDynamicEffect *)object;	// (name invented; fits the /Od frame order)
	unsigned int uiCount = affectedNodes.ItemsInList();	// (name invented; fits the /Od frame order)
	unsigned int uiEffectCount = pEffect->affectedNodes.ItemsInList();	// (name invented; fits the /Od frame order)
	if ( uiCount != uiEffectCount ) {
		return false;
	}
	if ( uiCount > 0 ) {
		NiNodeList *pList0 = &affectedNodes;	// (name invented; fits the /Od frame order)
		NiNodeList *pList1 = &pEffect->affectedNodes;	// (name invented; fits the /Od frame order)
		while ( pList0 ) {
			NiNode *pNode0 = pList0->GetItem();	// (name invented; fits the /Od frame order)
			NiNode *pNode1 = pList1->GetItem();	// (name invented; fits the /Od frame order)
			if ( ( pNode0 && !pNode1 ) || ( !pNode0 && pNode1 ) ) {
				return false;
			}
			pList0 = pList0->GetNext();
			pList1 = pList1->GetNext();
		}
	}
	return true;
}

/*
================
NiDynamicEffect::GetViewerStrings

NiAVObject's strings, then the address of every affected node. The list
is walked through its accessors, as the SDK's debug NetImmerse 3.1 does
(AnimOnly.exe 0x4183D7).
================
*/
// FUNCTION: STBC 0x7E26D0
void NiDynamicEffect::GetViewerStrings( NiTArray<char *> *strings ) {
	// nodeName: (name invented; fits the /Od frame order)
	NiAVObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );

	if ( affectedNodes.ItemsInList() > 0 ) {
		char nodeName[64];
		int i = 0;
		NiNodeList *list = &affectedNodes;
		while ( list ) {
			NiNode *node = list->GetItem();
			sprintf( nodeName, "node[%d]", i );
			strings->Add( NiGetViewerString( nodeName, node ) );
			list = list->GetNext();
			i++;
		}
	}
}

