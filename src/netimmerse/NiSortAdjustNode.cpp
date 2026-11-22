// NiSortAdjustNode.cpp -- nodes that change how their subtree is sorted

#include "NiRTLib.h"
#include <assert.h>
#include "NiSortAdjustNode.h"
#include "NiCamera.h"
#include "NiStream.h"
#include "NiBone.h"

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x00806210 _$E227
// FUNCTION: STBC 0x008061b0 _$E17
// FUNCTION: STBC 0x008061d0 _$E15
// FUNCTION: STBC 0x008061e0 _$E23
// FUNCTION: STBC 0x00806200 _$E21

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00806230 _$E30
// FUNCTION: STBC 0x00806240 _$E41
// FUNCTION: STBC 0x00806250 _$E45
// FUNCTION: STBC 0x00806260 _$E49
// FUNCTION: STBC 0x00806270 _$E53
// FUNCTION: STBC 0x00806280 _$E177
// FUNCTION: STBC 0x00806290 _$E190
// FUNCTION: STBC 0x008062a0 _$E119
// FUNCTION: STBC 0x008062b0 _$E132
// FUNCTION: STBC 0x008062d0 _$E199
// FUNCTION: STBC 0x008062e0 _$E186
// FUNCTION: STBC 0x008062f0 _$E203
// FUNCTION: STBC 0x00806300 _$E210
// FUNCTION: STBC 0x00806340 _$E194
// FUNCTION: STBC 0x008062c0 _$E182

// FUNCTION: STBC 0x00805a10 _$E220
// FUNCTION: STBC 0x00805a30 _$E217
// DATA: STBC 0x9A2370 ?rtti@NiSortAdjustNode@@2VNiRTTI@@B
const NiRTTI NiSortAdjustNode::rtti( "NiSortAdjustNode", &NiNode::rtti );

/*
================
NiSortAdjustNode::Register
================
*/
// FUNCTION: STBC 0x805A50 ?Register@NiSortAdjustNode@@SAHXZ
NiImplementRegister( NiSortAdjustNode )

/*
================
NiSortAdjustNode::Display

Draws the subtree with the renderer's sorter replaced as the mode says,
and restores the sorter after. An accumulator that declines the camera
leaves the subtree unsorted.
================
*/
// FUNCTION: STBC 0x805BA0
void NiSortAdjustNode::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	NiPointer<NiAccumulator> sorter = renderer->GetSorter();

	switch ( sortingMode ) {
	case SORTING_OFF:
		renderer->SetSorter( 0 );
		NiNode::Display( camera );
		break;
	case SORTING_SUBSORT:
		renderer->SetSorter( accumulator );
		if ( accumulator && accumulator->StartAccumulating( camera ) ) {
			NiNode::Display( camera );
			accumulator->FinishAccumulating();
		} else {
			renderer->SetSorter( 0 );
			NiNode::Display( camera );
		}
		break;
	default:
		NiNode::Display( camera );
		break;
	}

	camera->GetRenderer()->SetSorter( sorter );
}

/*
================
NiSortAdjustNode::CreateClone
================
*/
// FUNCTION: STBC 0x805CF0
NiObject *NiSortAdjustNode::CreateClone() {
	NiSortAdjustNode *clone = new NiSortAdjustNode;
	CopyMembers( clone );
	return clone;
}

/*
================
NiSortAdjustNode::CopyMembers

The clone gets a clone of the accumulator.
================
*/
// FUNCTION: STBC 0x805DE0
void NiSortAdjustNode::CopyMembers( NiSortAdjustNode *dest ) {
	NiNode::CopyMembers( dest );
	dest->sortingMode = sortingMode;
	dest->accumulator = (NiAccumulator *)accumulator->CreateClone();
}

/*
================
NiSortAdjustNode::CreateObject

Creates the node and loads it.
================
*/
// FUNCTION: STBC 0x805E40
void NiSortAdjustNode::CreateObject( NiStream &stream, NiObject *&object ) {
	NiSortAdjustNode *node = new NiSortAdjustNode;
	stream.SetLoadRecord( new NiSortAdjustNodeLoadRecord );
	node->LoadBinary( stream );
	object = node;
}

/*
================
NiSortAdjustNode::LoadBinary

The mode; the accumulator's link ID from "3.01" on (older files get none).
================
*/
// FUNCTION: STBC 0x805FE0
void NiSortAdjustNode::LoadBinary( NiStream &stream ) {
	NiNode::LoadBinary( stream );
	NiSortAdjustNodeLoadRecord *record = (NiSortAdjustNodeLoadRecord *)stream.GetLoadRecord();
	assert( record );

	NiStreamLoadEnum( stream, sortingMode );

	if ( strcmp( stream.GetFileVersion(), "3.01" ) >= 0 ) {
		NiStreamLoadBinary( stream, record->accumulatorLinkID );
	} else {
		record->accumulatorLinkID = 0;
	}
}

/*
================
NiSortAdjustNode::LinkObject
================
*/
// FUNCTION: STBC 0x806070
void NiSortAdjustNode::LinkObject( NiStream &stream ) {
	NiNode::LinkObject( stream );
	NiSortAdjustNodeLoadRecord *record = (NiSortAdjustNodeLoadRecord *)stream.GetLoadRecord();
	accumulator = (NiAccumulator *)stream.GetObjectFromLinkID( record->accumulatorLinkID );
}

/*
================
NiSortAdjustNode::RegisterStreamables

Registers the accumulator after the node.
================
*/
// FUNCTION: STBC 0x8060D0
bool NiSortAdjustNode::RegisterStreamables( NiStream &stream ) {
	if ( !NiNode::RegisterStreamables( stream ) ) {
		return false;
	}
	if ( accumulator ) {
		accumulator->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiSortAdjustNode::SaveBinary
================
*/
// FUNCTION: STBC 0x806100
void NiSortAdjustNode::SaveBinary( NiStream &stream ) {
	NiNode::SaveBinary( stream );
	NiStreamSaveEnum( stream, sortingMode );
	NiStreamSaveBinary( stream, (NiAccumulator *)accumulator );
}

/*
================
NiSortAdjustNode::IsEqual
================
*/
// FUNCTION: STBC 0x806140
bool NiSortAdjustNode::IsEqual( NiObject *object ) {
	if ( !NiNode::IsEqual( object ) ) {
		return false;
	}

	NiSortAdjustNode *node = (NiSortAdjustNode *)object;
	if ( sortingMode != node->sortingMode ) {
		return false;
	}
	if ( accumulator ) {
		if ( !accumulator->IsEqual( node->accumulator ) ) {
			return false;
		}
	} else if ( node->accumulator ) {
		return false;
	}
	return true;
}
