// NiFltAnimationNode.cpp -- switch nodes that flip through their children

#include "NiRTLib.h"
#include <assert.h>
#include <math.h>
#include "NiFltAnimationNode.h"
#include "NiStream.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007fd6f0 _$E30
// FUNCTION: STBC 0x007fd700 _$E45
// FUNCTION: STBC 0x007fd710 _$E49
// FUNCTION: STBC 0x007fd720 _$E53
// FUNCTION: STBC 0x007fd730 _$E57
// FUNCTION: STBC 0x007fd740 _$E181
// FUNCTION: STBC 0x007fd750 _$E34
// FUNCTION: STBC 0x007fd760 _$E123
// FUNCTION: STBC 0x007fd770 _$E136
// FUNCTION: STBC 0x007fd780 _$E186
// FUNCTION: STBC 0x007fd790 _$E191
// FUNCTION: STBC 0x007fd7a0 _$E195
// FUNCTION: STBC 0x007fd7b0 _$E199
// FUNCTION: STBC 0x007fd7c0 _$E206
// FUNCTION: STBC 0x007fd7d0 _$E210
// FUNCTION: STBC 0x007fd880 _$E214

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007fd6d0 _$E227
// FUNCTION: STBC 0x007fd670 _$E17
// FUNCTION: STBC 0x007fd690 _$E15
// FUNCTION: STBC 0x007fd6a0 _$E23
// FUNCTION: STBC 0x007fd6c0 _$E21

// FUNCTION: STBC 0x007fcef0 _$E220
// FUNCTION: STBC 0x007fcf10 _$E217
// DATA: STBC 0x9A20B0 ?rtti@NiFltAnimationNode@@2VNiRTTI@@B
const NiRTTI NiFltAnimationNode::rtti( "NiFltAnimationNode", &NiSwitchNode::rtti );

/*
================
NiFltAnimationNode::Register
================
*/
// FUNCTION: STBC 0x7FCF30 ?Register@NiFltAnimationNode@@SAHXZ
NiImplementRegister( NiFltAnimationNode )

/*
================
NiFltAnimationNode::SetIndexFromTime

The time within the period picks the child, the children sharing the
period evenly; bouncing runs forward over the first half of the period
and back over the second. Without children or with no period the index
is left alone.
================
*/
// FUNCTION: STBC 0x7FD080
void NiFltAnimationNode::SetIndexFromTime( double time ) {
	if ( !children.GetSize() ) {
		return;
	}

	if ( period > 0.0 ) {
		int count = children.GetSize();
		time = fmod( time, period );
		if ( time < 0.0 ) {
			time += period;
		}

		int newIndex;
		if ( GetBounce() ) {
			if ( time > 0.5 * period ) {
				time = period - time;
			}
			newIndex = (int)( count * 2.0 * time / period );
		} else {
			newIndex = (int)( count * time / period );
		}
		if ( newIndex >= count ) {
			newIndex = count - 1;
		}
		index = newIndex;
	}
}

/*
================
NiFltAnimationNode::Display

Shows the child for the last update time; between draws no child is
active.
================
*/
// FUNCTION: STBC 0x7FD130
void NiFltAnimationNode::Display( NiCamera *camera ) {
	SetIndexFromTime( savedTime );
	NiSwitchNode::Display( camera );
	index = -1;
}

/*
================
NiFltAnimationNode::CreateClone
================
*/
// FUNCTION: STBC 0x7FD160
NiObject *NiFltAnimationNode::CreateClone() {
	NiFltAnimationNode *clone = new NiFltAnimationNode;
	assert( clone );
	CopyMembers( clone );
	return clone;
}

/*
================
NiFltAnimationNode::CopyMembers
================
*/
// FUNCTION: STBC 0x7FD200
void NiFltAnimationNode::CopyMembers( NiFltAnimationNode *dest ) {
	NiSwitchNode::CopyMembers( dest );
	dest->period = period;
}

/*
================
NiFltAnimationNode::CreateObject

Creates the node and loads it.
================
*/
// FUNCTION: STBC 0x7FD230
void NiFltAnimationNode::CreateObject( NiStream &stream, NiObject *&object ) {
	NiFltAnimationNode *node = new NiFltAnimationNode;
	stream.SetLoadRecord( new NiFltAnimationNodeLoadRecord );
	node->LoadBinary( stream );
	object = node;
}

/*
================
NiFltAnimationNode::LoadBinary

The period; the bounce flag comes with NiAVObject's flags.
================
*/
// FUNCTION: STBC 0x7FD3B0
void NiFltAnimationNode::LoadBinary( NiStream &stream ) {
	NiSwitchNode::LoadBinary( stream );
	NiStreamLoadBinary( stream, period );
}

/*
================
NiFltAnimationNode::LinkObject
================
*/
// FUNCTION: STBC 0x7FD3E0
void NiFltAnimationNode::LinkObject( NiStream &stream ) {
	NiSwitchNode::LinkObject( stream );
}

/*
================
NiFltAnimationNode::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FD3F0
bool NiFltAnimationNode::RegisterStreamables( NiStream &stream ) {
	return NiSwitchNode::RegisterStreamables( stream );
}

/*
================
NiFltAnimationNode::SaveBinary
================
*/
// FUNCTION: STBC 0x7FD400
void NiFltAnimationNode::SaveBinary( NiStream &stream ) {
	NiSwitchNode::SaveBinary( stream );
	NiStreamSaveBinary( stream, period );
}

/*
================
NiFltAnimationNode::IsEqual
================
*/
// FUNCTION: STBC 0x7FD430
bool NiFltAnimationNode::IsEqual( NiObject *object ) {
	if ( !NiSwitchNode::IsEqual( object ) ) {
		return false;
	}

	NiFltAnimationNode *node = (NiFltAnimationNode *)object;
	if ( period != node->period || GetBounce() != node->GetBounce() ) {
		return false;
	}
	return true;
}

/*
================
NiFltAnimationNode::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FD480
void NiFltAnimationNode::GetViewerStrings( NiTArray<char *> *strings ) {
	NiSwitchNode::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_dPeriod", period ) );
	strings->Add( NiGetViewerString( "m_bBounce", GetBounce() ) );
}
